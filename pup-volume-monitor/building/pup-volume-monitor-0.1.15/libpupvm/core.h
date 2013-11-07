//core.c or core.h
//Library's central portion

#ifndef PUP_VM_H_INSIDE
//core.c
#	include "common-includes.h"

#else // !PUP_VM_H_INSIDE
//core.h

typedef struct
{
	gint8 *data;
	guint rw_ptr;
	guint len;
} PupSockBuffer;

typedef enum
{
	PUPSOCK_ADDRESS = 1 << 0,
	PUPSOCK_IS_SERVER = 1 << 1,
	PUPSOCK_IS_CONNECTED = 1 << 2,
	PUPSOCK_SAVE_READ_DATA = 1 << 3,
	PUPSOCK_HAS_G_MAIN = 1 << 4,

	PUPSOCK_READONLY = PUPSOCK_ADDRESS | PUPSOCK_IS_SERVER
		| PUPSOCK_IS_CONNECTED | PUPSOCK_HAS_G_MAIN,
	PUPSOCK_BOOLEAN = PUPSOCK_IS_SERVER | PUPSOCK_IS_CONNECTED
		| PUPSOCK_HAS_G_MAIN | PUPSOCK_SAVE_READ_DATA
} PupSockProperty;

typedef enum
{
	PUPSOCK_ACCEPT = 0,
	PUPSOCK_DATA_RECVD = 1,
	PUPSOCK_HUP = 2,

	PUPSOCK_N_EVENTS = 3
} PupSockEvent;

typedef struct
{
	//Base object
	GObject parent;

	//Destroy params
	gboolean attempt_flush;
	gdouble timeout;

	//Basic things
	int fd;
	GPollFD gfd;
	char *address;

	//Buffers
	GQueue *write_buf;
	GQueue *read_data;
	PupSockBuffer *read_buf;

	//Property bit mask
	guint props;

	//events
	guint event_source;
} PupSock;

typedef struct
{
	GObjectClass parent;

	guint accept_signal_id;
	guint data_recvd_signal_id;
	guint hup_signal_id;
} PupSockClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_SOCK PupSock pup_sock pup
#define PUP_TYPE_SOCK (pup_sock_get_type())
#define PUP_SOCK(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_SOCK, PupSock))
#define PUP_IS_SOCK(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_SOCK))
#define PUP_SOCK_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_SOCK, PupSockClass))
#define PUP_IS_SOCK_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_SOCK))
#define PUP_SOCK_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_SOCK, PupSockClass))
#define PUP_SOCK_NAME "PupSock"

typedef struct
{
	GSource parent;
	PupSock *sock;
} PupSockEventSource;

typedef gboolean (*pup_sock_common_callback)
(PupSock *sock, gpointer user_data);

typedef gboolean (*pup_sock_common_callback_data)
(PupSock *sock, gpointer cb_data, gpointer user_data);


#endif // PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
void pup_sock_set_destroy_params(PupSock *sock,gboolean attempt_flush,gdouble timeout);
guint pup_sock_add_to_g_main(PupSock *sock,GMainContext *context);
void pup_sock_event_source_finalize(GSource *source);
gboolean pup_sock_event_source_dispatch(GSource *source,GSourceFunc callback,gpointer data);
gboolean pup_sock_event_source_query(GSource *source);
gboolean pup_sock_event_source_prepare(GSource *source,gint *timeout);
PupSock *pup_sock_new_local(GError **error);
PupSock *pup_sock_new_from_fd(int fd);
void pup_sock_set_property(GObject *instance,guint property,const GValue *value,GParamSpec *pspec);
void pup_sock_get_property(GObject *instance,guint property,GValue *value,GParamSpec *pspec);
void pup_sock_finalize(GObject *instance);
#if !(!defined(PUP_VM_H_INSIDE))
GType pup_sock_get_type();
#endif
#if !defined(PUP_VM_H_INSIDE)
G_DEFINE_TYPE(PupSock,pup_sock,G_TYPE_OBJECT);
#endif
