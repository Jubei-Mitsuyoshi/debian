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
#define PUP_TYPE_CONV_MGR (pup_conv_mgr_get_type())
#define PUP_CONV_MGR(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_CONV_MGR, PupConvMgr))
#define PUP_IS_CONV_MGR(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_CONV_MGR))
#define PUP_CONV_MGR_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_CONV_MGR, PupConvMgrClass))
#define PUP_IS_CONV_MGR_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_CONV_MGR))
#define PUP_CONV_MGR_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_CONV_MGR, PupConvMgrClass))
#define PUP_CONV_MGR_NAME "PupConvMgr"

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

/*Everything beyond this is automatically generated*/
void pup_conv_send_message(PupConv *conv,PSDataEncoder *data);
void pup_conv_set_close_callback(PupConv *conv,PupConvCloseCB callback,gpointer user_data);
void pup_conv_set_msg_queue(PupConv *conv,GQueue *msg_queue);
void pup_conv_msg_cb(PupConv *conv,PSDataParser *rcvd_data,gboolean is_new,gpointer user_data,gpointer conv_user_data);
void pup_conv_set_callback(PupConv *conv,PupConvCB callback,gpointer user_data);
void pup_conv_close(PupConv *conv,PupConvCloseFlags flags);
void pup_conv_mgr_close_all_convs(PupConvMgr *cmgr,PupConvCloseFlags flags);
void pup_conv_mgr_remove_by_id(PupConvMgr *cmgr,PupConvID conv_id);
PupConv *pup_conv_mgr_lookup(PupConvMgr *cmgr,PupConvID conv_id);
PupConv *pup_conv_mgr_start_conv(PupConvMgr *cmgr);
PupConv *pup_conv_mgr_add_conv(PupConvMgr *cmgr,PupConvID conv_id);
PupSock *pup_conv_mgr_get_sock(PupConvMgr *cmgr);
void pup_conv_mgr_hup_cb(PupSock *sock,gpointer data);
void pup_conv_mgr_sorter_cb(PupSock *sock,PupSockData *data,gpointer user_data);
PupConvMgr *pup_conv_mgr_new(PupSock *sock,PupConvCB def_callback,gpointer user_data);
void pup_conv_mgr_finalize(GObject *instance);
#if !(!defined(PUP_VM_H_INSIDE))
GType pup_conv_mgr_get_type();
#endif
#if !defined(PUP_VM_H_INSIDE)
G_DEFINE_TYPE(PupConvMgr,pup_conv_mgr,G_TYPE_OBJECT);
#endif
