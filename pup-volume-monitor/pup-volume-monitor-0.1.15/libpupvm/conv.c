//conv.c or conv.h
//Highlevel organisation of data into conversations

#ifndef PUP_VM_H_INSIDE
//conv.c
#	include "common-includes.h"

#else // !PUP_VM_H_INSIDE
//conv.h

typedef struct
{
	GPid process_id;
	gpointer host_id;
} PupConvID;

typedef struct
{
	GObject parent;
	PupSock *sock;
	gulong sorter_cb_tag;
	gulong hup_cb_tag;
	GHashTable *self_convs;
	GHashTable *other_convs;
	GPid process_id;
	gboolean assumes_primary;
	guint32 allocator;
	gpointer def_cb;
	gpointer def_cb_data;
} PupConvMgr;

typedef struct
{
	GObjectClass parent;
} PupConvMgrClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_CONV_MGR PupConvMgr pup_conv_mgr pup

typedef struct
{
	PupConvMgr *cmgr;
	PupConvID conv_id;
	gboolean first;
	gpointer cb;
	gpointer cb_data;
	gboolean closed;
	gpointer close_cb;
	gpointer close_cb_data;
} PupConv;


typedef enum 
{
	PUP_CONV_NEW = 0,
	PUP_CONV_MSG = 1,
	PUP_CONV_END = 2
} PupConvMsgs;

typedef enum 
{
	PUP_CONV_BREAK = 1 << 0,
	PUP_CONV_FREE = 1 << 1,
	PUP_CONV_NOREMOVE = 1 << 2
} PupConvCloseFlags;

typedef void (*PupConvCB) (PupConv *conv, PSDataParser *rcvd_data,
                           gboolean is_new, gpointer user_data, gpointer conv_user_data);

typedef void (*PupConvCloseCB) (PupConv *conv, gpointer user_data);

#endif // PUP_VM_H_INSIDE

//FILE_HEADER_END


#ifndef PUP_VM_H_INSIDE
G_DEFINE_TYPE(PupConvMgr, pup_conv_mgr, G_TYPE_OBJECT);
#else
GType pup_conv_mgr_get_type();
#endif

static void pup_conv_mgr_class_init(PupConvMgrClass *klass)
{
	GObjectClass *basic_class = G_OBJECT_CLASS(klass);

	basic_class->finalize = pup_conv_mgr_finalize;
}

static void pup_conv_mgr_init(PupConvMgr *cmgr)
{

}

//Constructor
PupConvMgr *pup_conv_mgr_new(PupSock *sock, PupConvCB def_callback, gpointer user_data)
{
	g_return_val_if_fail(PUP_IS_SOCK(sock), NULL);

	//g_warn_if_fail(pup_sock_get_property(sock, PUPSOCK_IS_CONNECTED));
	gboolean is_connected;
	g_object_get(sock, "is-connected", &is_connected, NULL);
	g_return_val_if_fail(is_connected, NULL);

	PupConvMgr *cmgr = PUP_CONV_MGR(g_object_new(PUP_TYPE_CONV_MGR, NULL));

	cmgr->sock = sock;

	//pup_sock_set_property(sock, PUPSOCK_SAVE_READ_DATA, FALSE);
	g_object_set(sock, "save-read-data", FALSE, NULL);
	//cmgr->sorter_cb_tag = pup_sock_add_callback(sock, PUPSOCK_DATA_RECVD,
	//		pup_conv_mgr_sorter_cb, cmgr);
	g_signal_connect_object(sock, "data-recvd",
	                        G_CALLBACK(pup_conv_mgr_sorter_cb), cmgr, 0);
	//cmgr->hup_cb_tag = pup_sock_add_callback(sock, PUPSOCK_HUP,
	//		pup_conv_mgr_hup_cb, cmgr);
	g_signal_connect_object(sock, "hup",
	                        G_CALLBACK(pup_conv_mgr_hup_cb), cmgr, 0);

	cmgr->self_convs = g_hash_table_new(NULL, NULL);
	cmgr->other_convs = g_hash_table_new(NULL, NULL);

	cmgr->process_id = getpid();

	cmgr->def_cb = def_callback;
	cmgr->def_cb_data = user_data;

	return cmgr;
}

PupSock *pup_conv_mgr_get_sock(PupConvMgr *cmgr)
{
	return cmgr->sock;
}

PupConv *pup_conv_mgr_add_conv(PupConvMgr *cmgr, PupConvID conv_id)
{
	PupConv *conv = g_slice_new0(PupConv);

	conv->cmgr = cmgr;
	if (conv_id.host_id)
	{
		conv->conv_id = conv_id;
		g_hash_table_insert(cmgr->other_convs, conv_id.host_id, conv);
	}
	else
	{
		conv->conv_id.host_id = conv;
		conv->conv_id.process_id = cmgr->process_id;
		g_hash_table_insert(cmgr->self_convs, conv->conv_id.host_id, conv);
	}

	return conv;
}

PupConv *pup_conv_mgr_start_conv(PupConvMgr *cmgr)
{
	PupConv *retval = pup_conv_mgr_add_conv(cmgr, (PupConvID) {0, NULL});
	retval->first = TRUE;
	return retval;	
}

PupConv *pup_conv_mgr_lookup(PupConvMgr *cmgr, PupConvID conv_id)
{
	if (cmgr->process_id == conv_id.process_id)
	{
		return (PupConv *) g_hash_table_lookup(cmgr->self_convs, conv_id.host_id);
	}
	else
	{
		return (PupConv *) g_hash_table_lookup(cmgr->other_convs, conv_id.host_id);
	}
}

void pup_conv_mgr_remove_by_id(PupConvMgr *cmgr, PupConvID conv_id)
{
	if (cmgr->process_id == conv_id.process_id)
	{
		g_hash_table_remove(cmgr->self_convs, conv_id.host_id);
	}
	else
	{
		g_hash_table_remove(cmgr->other_convs, conv_id.host_id);
	}
}

void pup_conv_mgr_close_all_convs(PupConvMgr *cmgr, PupConvCloseFlags flags)
{
	GHashTableIter iter;
	PupConv *oneconv;
	g_hash_table_iter_init(&iter, cmgr->self_convs);
	while(g_hash_table_iter_next(&iter, NULL, (gpointer *) &oneconv))
	{
		pup_conv_close(oneconv, flags | PUP_CONV_NOREMOVE);
		if (flags & PUP_CONV_FREE)
			g_hash_table_iter_remove(&iter);
	}
	g_hash_table_iter_init(&iter, cmgr->other_convs);
	while(g_hash_table_iter_next(&iter, NULL, (gpointer *) &oneconv))
	{
		pup_conv_close(oneconv, flags | PUP_CONV_NOREMOVE);
		if (flags & PUP_CONV_FREE)
			g_hash_table_iter_remove(&iter);
	}
}

void pup_conv_mgr_sorter_cb(PupSock *sock, PupSockData *data, 
                            gpointer user_data)
{
	PSDataParser *parser = ps_data_parser_new_from_data(data);
	PupConvID conv_id = ps_data_parser_parse_var(parser, PupConvID);
	gint conv_msgtype = (gint) ps_data_parser_parse_var(parser, gint8);

	PupConvMgr *cmgr = (PupConvMgr *) user_data;
	PupConv *conv = pup_conv_mgr_lookup(cmgr, conv_id);

	if (conv_msgtype == PUP_CONV_MSG)
	{
		//Check whether the id already exists

		if (conv)
		{
			if (!(conv->closed))
			{
				if (conv->cb)
					((PupConvCB) conv->cb) (conv, parser, FALSE,
					                        cmgr->def_cb_data, conv->cb_data);
				else if (cmgr->def_cb)
					((PupConvCB) cmgr->def_cb)(conv, parser, FALSE,
					                           cmgr->def_cb_data, conv->cb_data);
			}
			//Else swallow
		}
		else
			g_warning("Conversation with process_id=%d, host_id=%p not found",
			          conv_id.process_id, conv_id.host_id);
	}
	else if (conv_msgtype == PUP_CONV_NEW)
	{
		//New conversation
		conv = pup_conv_mgr_add_conv(cmgr, conv_id);
		((PupConvCB) cmgr->def_cb)(conv, parser, TRUE, cmgr->def_cb_data, NULL);
	}
	else if (conv_msgtype == PUP_CONV_END)
	{
		//Close conversation
		if (conv)
		{
			pup_conv_close(conv, PUP_CONV_BREAK);
		}
	}

	ps_data_parser_destroy(parser);
}

void pup_conv_mgr_hup_cb(PupSock *sock, gpointer data)
{
	//Automatically close all conversations
	PupConvMgr *cmgr = (PupConvMgr *) data;
	pup_conv_mgr_close_all_convs(cmgr, PUP_CONV_BREAK);
}

void pup_conv_mgr_finalize(GObject *instance)
{
	PupConvMgr *cmgr = PUP_CONV_MGR(instance);

	pup_conv_mgr_close_all_convs(cmgr, PUP_CONV_FREE);
	g_hash_table_destroy(cmgr->self_convs);
	g_hash_table_destroy(cmgr->other_convs);
	
	g_object_unref(cmgr->sock);

	G_OBJECT_CLASS(pup_conv_mgr_parent_class)->finalize(instance);
}

void pup_conv_set_callback(PupConv *conv, PupConvCB callback,
                           gpointer user_data)
{
	conv->cb = callback;
	conv->cb_data = user_data;
}

void pup_conv_msg_cb(PupConv *conv, PSDataParser *rcvd_data,
                     gboolean is_new, gpointer user_data, gpointer conv_user_data)
{
	GQueue *queue = (GQueue *) conv_user_data;
	gsize len = rcvd_data->data.len - rcvd_data->rw_ptr;
	PSDataParser *parser
		= ps_data_parser_new(ps_data_parser_parse_next_fixed_block(rcvd_data, len),
		                     len, TRUE);
	g_queue_push_tail(queue, parser);
}

void pup_conv_set_msg_queue(PupConv *conv, GQueue *msg_queue)
{
	pup_conv_set_callback(conv, pup_conv_msg_cb, msg_queue);
}

void pup_conv_set_close_callback(PupConv *conv, PupConvCloseCB callback,
                                 gpointer user_data)
{
	conv->close_cb = callback;
	conv->close_cb_data = user_data;
}

void pup_conv_send_message(PupConv *conv, PSDataEncoder *data)
{
	if (conv->closed) return;

	PSDataEncoder *encoder = ps_data_encoder_new();

	ps_data_encoder_add_val(encoder, conv->conv_id, PupConvID, 0);
	if (conv->first)
	{
		ps_data_encoder_add_val(encoder, PUP_CONV_NEW, gint8, 0);
		conv->first = FALSE;
	}
	else
	{
		ps_data_encoder_add_val(encoder, PUP_CONV_MSG, gint8, 0);
	}
	ps_data_encoder_add_encoder(encoder, data, 0);
	ps_data_encoder_queue_send(encoder, conv->cmgr->sock);

	g_queue_free(encoder->data_queue);
	g_slice_free(PSDataEncoder, encoder);
}

void pup_conv_close(PupConv *conv, PupConvCloseFlags flags)
{
	if (! (conv->closed))
	{
		if (!(flags & PUP_CONV_BREAK))
		{
			PSDataEncoder *encoder = ps_data_encoder_new();
			ps_data_encoder_add_val(encoder, conv->conv_id, PupConvID, 0);
			ps_data_encoder_add_val(encoder, PUP_CONV_END, gint8, 0);
			ps_data_encoder_queue_send(encoder, conv->cmgr->sock);
		}
		conv->closed = TRUE;
		if (conv->close_cb)
			((PupConvCloseCB) conv->close_cb) (conv, conv->close_cb_data);
	}

	if (flags & PUP_CONV_FREE)
	{
		if (! (flags & PUP_CONV_NOREMOVE))
		{
			pup_conv_mgr_remove_by_id(conv->cmgr, conv->conv_id);
		}
		g_slice_free(PupConv, conv);
	}
}

