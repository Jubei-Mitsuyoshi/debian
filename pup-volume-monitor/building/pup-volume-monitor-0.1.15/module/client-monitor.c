//client-monitor.c or client-monitor.h
//Connecting to server, getting jobs done through server, etc.

#ifndef PUP_VM_H_INSIDE
//client-monitor.c
#include "common.h"

#else // !PUP_VM_H_INSIDE
//client-monitor.h

typedef struct
{
	PupVMMonitor parent;

	PupConvMgr *cmgr;
	PupConv *event_conv;
	gboolean initialized;

	GType volume_type;
	GType drive_type;
	GObject *volume_monitor;
} PupClientMonitor;

typedef struct 
{
	PupVMMonitorClass parent;

	guint hup_signal_id;
} PupClientMonitorClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_CLIENT_MONITOR PupClientMonitor pup_client_monitor pup

typedef struct
{
	GObject parent;

	PupDevice *holder;
	PupClientMonitor *monitor;
	GObject *volume_monitor;
	gpointer iface;
} PupClientDevice;

typedef void (*PupClientDeviceSetup) (PupClientDevice *client_device,
                                      PupDevice *holder);
typedef struct
{
	GObjectClass parent;

	PupClientDeviceSetup setup_func;
} PupClientDeviceClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_CLIENT_DEVICE PupClientDevice pup_client_device pup

typedef struct 
{
	PupRemoteOperation parent;

	PupDevice *dev;
	GMountOperation *mount_operation;
	GSimpleAsyncResult *result;
	guint current_query;
} PupGIOOperation;

#endif //PUP_VM_H_INSIDE

//FILE_HEADER_END

//Client side monitor

#ifndef PUP_VM_H_INSIDE
G_DEFINE_DYNAMIC_TYPE(PupClientMonitor, pup_client_monitor, PUP_TYPE_VM_MONITOR);
#else
GType pup_client_monitor_get_type();
#endif

void pup_client_monitor_register(GIOModule *module)
{
	pup_client_monitor_register_type(G_TYPE_MODULE(module));
}

static void pup_client_monitor_class_init(PupClientMonitorClass *klass)
{
	PupVMMonitorClass *parent_class = PUP_VM_MONITOR_CLASS(klass);
	GObjectClass *basic_class = G_OBJECT_CLASS(klass);

	basic_class->finalize = pup_client_monitor_finalize;

	parent_class->device_event_cb = pup_client_monitor_device_event_cb;

	klass->hup_signal_id = g_signal_new
		("hup", PUP_TYPE_CLIENT_MONITOR, G_SIGNAL_RUN_LAST, 0, NULL, NULL,
		 g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, G_TYPE_INVALID);
}

static void pup_client_monitor_class_finalize(PupClientMonitorClass *klass)
{

}

static void pup_client_monitor_init(PupClientMonitor *self)
{
	self->volume_type = PUP_TYPE_CLIENT_VOLUME;
	self->drive_type = PUP_TYPE_CLIENT_DRIVE;
}

void pup_client_monitor_finalize(GObject *instance)
{
	PupClientMonitor *self = PUP_CLIENT_MONITOR(instance);
	//Disconnect from server, this statement is enough
	if (self->cmgr) g_object_unref(self->cmgr);

	G_OBJECT_CLASS(pup_client_monitor_parent_class)->finalize(instance);
}

void pup_client_monitor_get_devices_cb(PupConv *conv, PSDataParser *rcvd_data,
                                       gboolean is_new, PupClientMonitor *monitor,
                                       gpointer dummy)
{
	GSList *list1 = NULL, *list2 = NULL, *iter;
	ps_data_parser_parse_complex_array(rcvd_data, (gpointer *) &list1,
	                                   ps_gslist_adder, pup_device_parse, NULL);
	ps_data_parser_parse_complex_array(rcvd_data, (gpointer *) &list2,
	                                   ps_gslist_adder, pup_device_parse, NULL);
	for (iter = list2; iter; iter = iter->next)
	{
		pup_vm_monitor_add_device(PUP_VM_MONITOR(monitor),
		                          PUP_DEVICE(iter->data));
	}
	for (iter = list1; iter; iter = iter->next)
	{
		pup_vm_monitor_add_device(PUP_VM_MONITOR(monitor),
		                          PUP_DEVICE(iter->data));
	}
	g_slist_free(list1);
	g_slist_free(list2);
	
}

void pup_client_monitor_disconnect_cb(PupSock *sock, PupClientMonitor *self)
{
	if (self->cmgr) g_object_unref(self->cmgr);
	self->cmgr = NULL;

	g_signal_emit(self, PUP_CLIENT_MONITOR_GET_CLASS(self)->hup_signal_id, 0);
}

gboolean pup_client_monitor_connect(PupClientMonitor *self)
{
	//Connect to the server
	GError *error = NULL;
	PupSock *sock = pup_sock_new_local(&error);
	if (error)
	{
		g_warning("Couldn't create new socket: %s", error->message);
		g_clear_error(&error);
		return FALSE;
	}
	pup_sock_connect_local(sock, pup_get_svr_sock_path(), &error);
	if (error)
	{
		g_warning("Couldn't connect to server at %s: %s",
		          pup_get_svr_sock_path(), error->message);
		g_clear_error(&error);
		g_object_unref(sock);
		return FALSE;
	}
	//Connection established, set up conversation manager 
	self->cmgr = pup_conv_mgr_new(sock, NULL, self);
	//Get all devices
	PupConv *conv = pup_conv_mgr_start_conv(self->cmgr);
	PSDataEncoder *encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_TAG_LIST);
	pup_conv_send_message(conv, encoder);
	pup_conv_set_callback(conv, (PupConvCB) pup_client_monitor_get_devices_cb,
	                      NULL);
	ps_data_encoder_destroy(encoder);
	//Send messages
	pup_sock_send(sock, -1.0, &error);
	if (error)
	{
		g_warning("Couldn't send data: %s", error->message);
		g_clear_error(&error);
		g_object_unref(self->cmgr);
		self->cmgr = NULL;
		return FALSE;
	}
	pup_sock_receive(sock, -1.0, 2, &error);
	if (error)
	{
		g_warning("Couldn't receive data: %s", error->message);
		g_clear_error(&error);
		g_object_unref(self->cmgr);
		self->cmgr = NULL;
		return FALSE;
	}
	//Setup events
	conv = pup_conv_mgr_start_conv(self->cmgr);
	encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_TAG_LISTEN);
	pup_conv_send_message(conv, encoder);
	pup_conv_set_callback(conv, (PupConvCB) pup_client_monitor_svr_event_cb,
	                      NULL);
	ps_data_encoder_destroy(encoder);
	//Set callbacks
	pup_sock_add_to_g_main(sock, NULL);
	g_signal_connect_after(sock, "hup",
	                       G_CALLBACK(pup_client_monitor_disconnect_cb), self);
	//Done
	self->initialized=TRUE;
	return TRUE;
}

void pup_client_monitor_svr_event_cb(PupConv *conv, PSDataParser *rcvd_data,
                                     gboolean is_new, PupClientMonitor *monitor,
                                     gpointer dummy)
{
	PupVMEvent event;
	PupDevice *dev;
	//Read the device
	g_return_if_fail(pup_device_parse_event(rcvd_data, &event));
	g_return_if_fail((dev = pup_device_new_from_header(&(event.header),
	                                                   rcvd_data)));
	
	if (event.action == PUP_DEVICE_REMOVE)
	{
		pup_vm_monitor_remove_device(PUP_VM_MONITOR(monitor), dev);
		g_object_unref(dev);
	}
	else
	{
		if (pup_vm_monitor_lookup(PUP_VM_MONITOR(monitor), event.header.catagory,
			                      event.header.sysname, FALSE))
		{
			//Then update device, dev is automatically freed
			pup_vm_monitor_copy_back_device(PUP_VM_MONITOR(monitor), dev,
				                            event.detail);
		}
		else
		{
			//Else add new device
			pup_vm_monitor_add_device(PUP_VM_MONITOR(monitor), dev);
			
		}
		//G_BREAKPOINT();
	}

	pup_vm_event_free_data(&event, TRUE);
}

void pup_client_monitor_device_event_cb(PupVMMonitor *monitor, 
                                        PupDevice *dev, guint event,
                                        const gchar *detail)
{
	g_signal_emit(monitor,
	              PUP_VM_MONITOR_GET_CLASS(monitor)->device_event_signal_id,
	              0, dev, event, detail);
}

void pup_client_monitor_start_operation(PupClientMonitor *monitor,
                                        PupDevice *dev,
                                        const gchar *oper_name,
                                        const gchar *args,
                                        GMountOperation *mount_operation,
                                        GSimpleAsyncResult *result)
{
	//Create new operation
	PupGIOOperation *operation = g_new0(PupGIOOperation, 1);
	//Fill in the structure
	operation->parent.catagory = dev->catagory;
	operation->parent.sysname = g_strdup(dev->sysname);
	operation->parent.operation = g_strdup(oper_name);
	operation->parent.args = g_strdup(args);
	operation->parent.return_cb = pup_client_monitor_operation_return_cb;
	if (mount_operation)
	{
		operation->parent.passwd_cb = pup_client_monitor_ask_passwd_cb;
		operation->parent.question_cb = pup_client_monitor_ask_question_cb;
		operation->mount_operation = mount_operation;
		g_signal_connect(mount_operation, "reply",
		                 G_CALLBACK(pup_client_monitor_user_respond_cb),
		                 operation);
	}
	operation->dev = dev;
	operation->result = result;
	//Start operation
	pup_device_hold(dev);
	pup_remote_operation_run((PupRemoteOperation *) operation, monitor->cmgr);
}

void pup_client_monitor_operation_return_cb (PupRemoteOperation *operation,
                                             gboolean success, 
                                             guint error_code,
                                             const gchar *detail)
{
	PupGIOOperation *gio_operation = (PupGIOOperation *) operation;
	
	if (success)
	{
		g_simple_async_result_set_op_res_gboolean(gio_operation->result,
		                                          TRUE);
	}
	else
	{
		g_simple_async_result_set_error(gio_operation->result,
		                                G_IO_ERROR,
		                                error_code,
		                                "%s", detail);
	}
	g_simple_async_result_complete(gio_operation->result);
	pup_device_release(gio_operation->dev);
}

void pup_client_monitor_ask_passwd_cb (PupRemoteOperation *operation,
                                       const gchar *msg,
                                       GAskPasswordFlags flags)
{
	PupGIOOperation *gio_operation = (PupGIOOperation *) operation;
	
	//Store what was asked by server
	gio_operation->current_query = PUP_TAG_OPERATION_PASSWORD;

	//Raise necessary signal
	g_signal_emit_by_name(gio_operation->mount_operation,
	                      "ask-password", msg, NULL, NULL, flags);
}

void pup_client_monitor_ask_question_cb (PupRemoteOperation *operation,
                                         const gchar *question,
                                         const gchar **choices)
{
	PupGIOOperation *gio_operation = (PupGIOOperation *) operation;

	//Store what was asked by server
	gio_operation->current_query = PUP_TAG_OPERATION_QUESTION;
	
	//Raise necessary signal
	g_signal_emit_by_name(gio_operation->mount_operation,
	                      "ask-question", question, choices);
}

void pup_client_monitor_user_respond_cb(GMountOperation *mount_operation,
                                        GMountOperationResult abort,
                                        PupGIOOperation *gio_operation)
{
	if (abort == G_MOUNT_OPERATION_ABORTED)
		pup_remote_operation_abort(gio_operation->parent.conv);
	else if (abort == G_MOUNT_OPERATION_UNHANDLED)
		pup_remote_operation_report_unhandled(gio_operation->parent.conv);
	else
	{	
		if (gio_operation->current_query == PUP_TAG_OPERATION_PASSWORD)
		{
			pup_remote_operation_passwd_reply
			(gio_operation->parent.conv,
			 g_mount_operation_get_username(mount_operation),
			 g_mount_operation_get_password(mount_operation),
			 g_mount_operation_get_domain(mount_operation));
		}
		else if (gio_operation->current_query == PUP_TAG_OPERATION_QUESTION)
		{
			pup_remote_operation_choice_reply
				(gio_operation->parent.conv,
				 g_mount_operation_get_choice(mount_operation));
		}
	}
}

//Client side device
#ifndef PUP_VM_H_INSIDE
G_DEFINE_DYNAMIC_TYPE(PupClientDevice, pup_client_device, G_TYPE_OBJECT);
#else
GType pup_client_device_get_type();
#endif

GQuark pup_client_device_quark()
{
	static GQuark id_quark = 0;
	if (! id_quark)
		id_quark = g_quark_from_string("PupClientDeviceHolder");

	return id_quark;
}

void pup_client_device_register(GIOModule *module)
{
	pup_client_device_register_type(G_TYPE_MODULE(module));
}

static void pup_client_device_class_init(PupClientDeviceClass *klass)
{
	GObjectClass *basic_class = G_OBJECT_CLASS(klass);

	basic_class->finalize = pup_client_device_finalize;
}

static void pup_client_device_class_finalize(PupClientDeviceClass *klass)
{

}

static void pup_client_device_init(PupClientDevice *self)
{

}

void pup_client_device_finalize(GObject *instance)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(instance);
	
	g_object_set_qdata(G_OBJECT(self->holder), pup_client_device_quark(), NULL);

	if (! self->holder->monitor)
	{
		//The object has been detached, destroy the holder
		g_object_unref(self->holder);
	}

	G_OBJECT_CLASS(pup_client_device_parent_class)->finalize(instance);
}

void pup_client_device_release_device(gpointer dev)
{
	gpointer client_dev = g_object_get_qdata(dev, pup_client_device_quark());

	if (client_dev)
		g_object_unref(client_dev);
	else
		g_object_unref(dev);
}

PupClientDevice *pup_client_device_get_from_device(PupClientMonitor *monitor,
                                                   PupDevice *dev)
{
	gpointer tmp = g_object_get_qdata(G_OBJECT(dev), pup_client_device_quark());
	if (! tmp)
	{
		GType type = (PUP_IS_DRIVE(dev) ? monitor->drive_type : monitor->volume_type);
		tmp = g_object_new(type, NULL);
	}

	PupClientDevice *self = PUP_CLIENT_DEVICE(tmp);

	//Perform necessary setup
	if (! self->holder)
	{
		self->holder = dev;
		self->monitor = monitor;
		self->volume_monitor = monitor->volume_monitor;

		g_object_set_qdata(G_OBJECT(dev), pup_client_device_quark(), self);

		PupClientDeviceSetup setup_func
			= PUP_CLIENT_DEVICE_GET_CLASS(self)->setup_func;
		if (setup_func) setup_func(self, self->holder);
	}

	return self;
}

gint pup_client_device_compare(gconstpointer dev1, gconstpointer dev2)
{
	return g_strcmp0(PUP_CLIENT_DEVICE(dev1)->holder->sysname,
	                 PUP_CLIENT_DEVICE(dev2)->holder->sysname);
}


void pup_client_lock(PupClientDevice *self)
{
	if (self->monitor)
		pup_vm_monitor_lock(PUP_VM_MONITOR(self->monitor));
}

void pup_client_unlock(PupClientDevice *self)
{
	if (self->monitor)
		pup_vm_monitor_unlock(PUP_VM_MONITOR(self->monitor));
}
