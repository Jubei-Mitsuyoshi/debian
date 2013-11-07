//server.c or server.h
//Connecting to clients, sharing information, handling operations

#ifndef PUP_VM_H_INSIDE
//server.c
#include "common.h"
#else
//server.h

typedef struct
{
	PupSock *sock;
	GHashTable *clients;
	PupServerMonitor *monitor;
} PupServer;

typedef struct
{
	PupServer *server;
	PupConvMgr *cmgr;
	gulong event_signal_handle;
	PupConv *event_conv;
} PupClient;

typedef struct
{
	PupOperation parent;
	PupConv *conv;
	gboolean is_valid;
} PupServerOperation;

#endif //PUP_VM_H_INSIDE

//FILE_HEADER_END

//Check whether server is running
gboolean pup_server_check_is_running()
{
	PupSock *sock;
	GError *error = NULL;
	gboolean ret = TRUE;
	sock = pup_sock_new_local(&error);
	g_assert(! error);
	pup_sock_connect_local(sock, pup_get_svr_sock_path(), &error);
	if (error)
	{
		g_clear_error(&error);
		ret = FALSE;
	}
	g_object_unref(sock);
	return ret;
}

PupServer *pup_server_setup()
{
	GError *error = NULL;
	PupServer *self = g_slice_new0(PupServer);

	self->sock = pup_sock_new_local(&error);
	if (error)
	{
		g_error("Couldn't create socket: %s", error->message);
	}
	g_unlink(pup_get_svr_sock_path());
	pup_sock_setup_as_local_server(self->sock, pup_get_svr_sock_path(), &error);
	if (error)
	{
		g_error("Couldn't create socket: %s", error->message);
	}
	pup_sock_add_to_g_main(self->sock, NULL);
	g_signal_connect(self->sock, "accept", G_CALLBACK(pup_server_new_client_cb),
	                 self);

	self->clients = g_hash_table_new_full(NULL, NULL, NULL,
	                                      (GDestroyNotify) pup_client_destroy);

	return self;
}

void pup_server_new_client_cb(PupSock *sock, PupSock *new_connection, PupServer *self)
{
	PupClient *client = g_slice_new0(PupClient);

	client->cmgr = pup_conv_mgr_new(new_connection, pup_client_new_conv_cb, client);
	client->server = self;

	g_hash_table_insert(self->clients, client, client);

	g_signal_connect_after(new_connection, "hup",
	                       G_CALLBACK(pup_client_disconnect_cb), client);
	pup_sock_add_to_g_main(new_connection, NULL);
	pup_sock_set_destroy_params(new_connection, FALSE, 0);
}

void pup_client_set_event_conv(PupClient *client, PupConv *conv)
{
	g_return_if_fail(! client->event_signal_handle);
	client->event_signal_handle
		= g_signal_connect(client->server->monitor, "broadcast", 
			                 G_CALLBACK(pup_client_send_event), conv);
	client->event_conv = conv;
	pup_conv_set_close_callback(conv, (PupConvCloseCB) pup_client_unset_event_conv_cb, client);
}

void pup_client_unset_event_conv_cb(PupConv *conv, PupClient *client)
{
	if (client->event_signal_handle)
	{
		g_signal_handler_disconnect(client->server->monitor, client->event_signal_handle);
		//pup_conv_close(conv, PUP_CONV_FREE);
	}
	client->event_conv = NULL;
	client->event_signal_handle = 0;
}

void pup_client_send_event(PupServerMonitor *monitor, PSDataEncoder *encoder,
                           PupConv *conv)
{
	pup_conv_send_message(conv, encoder);
}

void pup_client_destroy(PupClient *client)
{
	g_object_unref(client->cmgr);

	g_slice_free(PupClient, client);
}

void pup_client_disconnect_cb(PupSock *sock, PupClient *client)
{
	g_hash_table_remove(client->server->clients, client);
}

void pup_client_start_operation(PupClient *client, PSDataParser *parser,
                                PupConv *conv)
{
	guint catagory;
	gchar *sysname, *type, *args;
	gboolean error = FALSE;
	pup_vm_extract_operation_details(parser, &catagory, &sysname, &type,
	                                 &args, &error);
	if (error)
	{
		g_critical("Error while reading operation details");
		return;
	}

	PupServerOperation *operation = g_slice_new0(PupServerOperation);
	operation->parent.type = type;
	operation->parent.args = args;
	operation->parent.msg_func = pup_client_operation_msg_cb;
	operation->conv = conv;
	operation->is_valid = TRUE;

	pup_server_monitor_start_operation(client->server->monitor, catagory,
	                                   sysname, (PupOperation *) operation);

	pup_conv_set_close_callback(conv, (PupConvCloseCB) pup_client_operation_invalidate,
	                            operation);

	g_free(sysname);
}

void pup_client_operation_msg_cb(PupOperation *operation,
                                    PSDataEncoder *encoder)
{
	pup_queue_call_func(operation, (PupFunc) pup_client_operation_msg_delayed,
	                    encoder);
}

void pup_client_operation_msg_delayed(PupServerOperation *operation,
                                         PSDataEncoder *encoder)
{
	if (operation->is_valid)
	{
		pup_conv_send_message(operation->conv, encoder);
		if (operation->parent.has_returned)
			pup_conv_close(operation->conv, PUP_CONV_FREE);
	}
	ps_data_encoder_destroy(encoder);
	if (operation->parent.has_returned)
	{
		g_free(operation->parent.type);
		g_free(operation->parent.args);
		g_slice_free(PupServerOperation, operation);
	}
}

void pup_client_operation_query_response_cb(PupConv *conv, PSDataParser *parser,
                                            gboolean is_new, PupClient *client,
                                            PupServerOperation *operation)
{
	gboolean error = FALSE;
	guint response = pup_vm_extract_tag(parser, &error);
	g_return_if_fail(! error);
	gchar *username = NULL, *password = NULL, *domain = NULL;
	if (response == PUP_OPERATION_RESPONSE_PASSWORD)
	{
		username = ps_data_parser_parse_str0(parser, &error);
		password = ps_data_parser_parse_str0(parser, &error);
		domain   = ps_data_parser_parse_str0(parser, &error);
		g_return_if_fail(! error);
	}
	if (operation->parent.reply_func)
	{
		((PupOperationPasswordFunc) operation->parent.reply_func)
			(operation->parent.dev, (PupOperation *) operation, response,
			 username, password, domain);
	}
}

void pup_client_operation_invalidate(PupConv *conv, PupServerOperation *operation)
{
	operation->is_valid = FALSE;
	if (! operation->parent.has_returned)
		pup_conv_close(operation->conv, PUP_CONV_FREE);
}

void pup_client_new_conv_cb(PupConv *conv, PSDataParser *recvd_data, 
                            gboolean is_new, gpointer user_data, gpointer dummy)
{
	g_return_if_fail(is_new);

	PupClient *client = (PupClient *) user_data;
	PupServer *server = client->server;

	guint request = pup_vm_extract_tag(recvd_data, NULL);
	
	switch (request)
	{
		case PUP_TAG_LIST:
		{
			PSDataEncoder *encoder = ps_data_encoder_new();
			//pup_vm_monitor_lock(PUP_VM_MONITOR(server->monitor));
			
			//Encode all devices
			
			ps_data_encoder_append_complex_array
				(encoder, PUP_VM_MONITOR(server->monitor)->drives, ps_ghashtable_iterator,
				 (PSDataEncodeFunc) pup_device_encode, NULL);
			ps_data_encoder_append_complex_array
				(encoder, PUP_VM_MONITOR(server->monitor)->volumes, ps_ghashtable_iterator,
				 (PSDataEncodeFunc) pup_device_encode, NULL);
			
			pup_vm_monitor_unlock(PUP_VM_MONITOR(server->monitor));
			//Send all data
			pup_conv_send_message(conv, encoder);
			ps_data_encoder_destroy(encoder);
			pup_conv_close(conv, PUP_CONV_FREE);
		}
			break;
		case PUP_TAG_LISTEN:
			//Add the conversation to listen to events
			pup_client_set_event_conv(client, conv);
			break;
		case PUP_TAG_OPERATION:
			//Start operation...
			pup_client_start_operation(client, recvd_data, conv);
			break;
		case PUP_TAG_REPROBE_MOUNT:
			pup_vm_monitor_get_mounts(PUP_VM_MONITOR(server->monitor));
			pup_conv_close(conv, PUP_CONV_FREE);
			break;
		case PUP_TAG_STOP:
			printf("Exiting...\n");
			exit(1);
			break;
	}
}
