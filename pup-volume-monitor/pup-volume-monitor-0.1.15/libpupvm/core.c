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

//FILE_HEADER_END

#ifndef PUP_VM_H_INSIDE
G_DEFINE_TYPE(PupSock, pup_sock, G_TYPE_OBJECT);
#else
GType pup_sock_get_type();
#endif

static void pup_sock_class_init(PupSockClass *klass)
{
	GObjectClass *basic_class = G_OBJECT_CLASS(klass);

	basic_class->finalize = pup_sock_finalize;
	basic_class->get_property = pup_sock_get_property;
	basic_class->set_property = pup_sock_set_property;

	GParamSpec *pspec;

	//properties:
	
	//char * "address"
	pspec = g_param_spec_string("address", "address",
	                            "Address of the socket or NULL if socket is not a server socket",
	                            NULL, G_PARAM_READABLE);
	g_object_class_install_property(basic_class, PUPSOCK_ADDRESS, pspec);

	//boolean "is-server"
	pspec = g_param_spec_boolean("is-server", "is-server",
	                             "Whether the socket is configured as server",
	                             FALSE, G_PARAM_READABLE);
	g_object_class_install_property(basic_class, PUPSOCK_IS_SERVER, pspec);

	//boolean "is-connected"
	pspec = g_param_spec_boolean("is-connected", "is-connected",
	                             "Whether the socket is connected",
	                             FALSE, G_PARAM_READABLE);
	g_object_class_install_property(basic_class, PUPSOCK_IS_CONNECTED, pspec);

	//boolean "save-read-data"
	pspec = g_param_spec_boolean("save-read-data", "save-read-data",
	                             "Whether the data read is stored in the reading queue, else it is\
								 freed after passing to callbacks",
	                             FALSE, G_PARAM_READWRITE);
	g_object_class_install_property(basic_class, PUPSOCK_SAVE_READ_DATA, pspec);

	//boolean "has-g-main"
	pspec = g_param_spec_boolean("has-g-main", "has-g-main",
	                             "Whether the socket is integrated with the main loop",
	                             FALSE, G_PARAM_READABLE);
	g_object_class_install_property(basic_class, PUPSOCK_HAS_G_MAIN, pspec);

	//Signals
	//"accept": void (*user_func) (PupSock *sock, PupSock *new_client,
	//gpointer user_data) : Run first
	klass->accept_signal_id = g_signal_new("accept", PUP_TYPE_SOCK,
	                                       G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
	                                       g_cclosure_marshal_VOID__POINTER,
	                                       G_TYPE_NONE, 1, G_TYPE_POINTER);

	//"data-recvd": void (*user_func) (PupSock *sock, PupSockData *data,
	//gpointer user_data) : Run first
	klass->data_recvd_signal_id = g_signal_new("data-recvd", PUP_TYPE_SOCK,
	                                           G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
	                                           g_cclosure_marshal_VOID__POINTER,
	                                           G_TYPE_NONE, 1, G_TYPE_POINTER);

	//"hup": void (*user_func) (PupSock *sock, gpointer user_data) : Run first
	klass->hup_signal_id = g_signal_new("hup", PUP_TYPE_SOCK,
	                                    G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
	                                    g_cclosure_marshal_VOID__VOID,
	                                    G_TYPE_NONE, 0, G_TYPE_INVALID);
}

static void pup_sock_init(PupSock *sock)
{
	sock->gfd.events = G_IO_IN | G_IO_HUP;
	sock->timeout = 0;
	sock->attempt_flush = FALSE;
	//Initialise buffers
	sock->read_data = g_queue_new();
	sock->read_buf = pup_sock_buffer_new(0);
	sock->write_buf = g_queue_new();
	sock->props |= PUPSOCK_SAVE_READ_DATA;
}

//Constructor
PupSock *pup_sock_new_from_fd(int fd)
{
	PupSock *sock = PUP_SOCK(g_object_new(PUP_TYPE_SOCK, NULL));

	sock->fd = sock->gfd.fd = fd;

	return sock;
}

PupSock *pup_sock_new_local(GError **error)
{
	int fd = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (fd < 0)
	{
		g_set_error(error, PUPSOCK_ERR_DOMAIN, errno, "%s", g_strerror(errno));
		return NULL;
	}

	return pup_sock_new_from_fd(fd);
}

void pup_sock_get_property(GObject *instance, guint property, GValue *value,
                           GParamSpec *pspec)
{
	PupSock *sock = PUP_SOCK(instance);
	//Boolean values
	if (property & PUPSOCK_BOOLEAN)
	{
		g_value_set_boolean(value, sock->props & property);
	}
	//char * values
	else if (property & PUPSOCK_ADDRESS)
	{
		g_value_set_string(value, sock->address);
	}
	else
	{
		G_OBJECT_WARN_INVALID_PROPERTY_ID(instance, property, pspec);
	}
}

void pup_sock_set_property(GObject *instance, guint property,
                           const GValue *value, GParamSpec *pspec)
{
	PupSock *sock = PUP_SOCK(instance);

	//Boolean values
	if (property & PUPSOCK_BOOLEAN)
	{
		if (g_value_get_boolean(value))
		{
			sock->props |= property;
		}
		else
		{
			sock->props &= ~property;
		}
	}
	else
	{
		G_OBJECT_WARN_INVALID_PROPERTY_ID(instance, property, pspec);
	}
}

//Main loop integration

gboolean pup_sock_event_source_prepare(GSource *source, gint *timeout)
{
	*timeout = -1;
	return FALSE;
}

gboolean pup_sock_event_source_query(GSource *source)
{
	PupSock *sock = ((PupSockEventSource *) source)->sock;
	if (sock->gfd.revents & (G_IO_IN | G_IO_OUT | G_IO_HUP))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

gboolean pup_sock_event_source_dispatch(GSource *source,
                                        GSourceFunc callback, gpointer data)
{
	//The event source is created exclusively for PupSock, no callbacks
	PupSock *sock = ((PupSockEventSource *) source)->sock;
	if (sock->props & PUPSOCK_IS_SERVER)
	{
		//Server
		if (sock->gfd.revents & G_IO_IN)
			pup_sock_connection_request_callback(sock);
	}
	else
	{
		//Client
		if (sock->gfd.revents & G_IO_HUP)
			pup_sock_broken_pipe_handler(sock);
		else if (sock->gfd.revents & G_IO_IN)
			pup_sock_input_callback(sock);
		else if (sock->gfd.revents & G_IO_OUT)
			pup_sock_output_callback(sock);
	}

	return TRUE;
}

void pup_sock_event_source_finalize(GSource *source)
{
	PupSock *sock = ((PupSockEventSource *) source)->sock;
	sock->event_source = 0;
	sock->props &= ~PUPSOCK_HAS_G_MAIN;
}

guint pup_sock_add_to_g_main(PupSock *sock, GMainContext *context)
{
	static GSourceFuncs funclist = {pup_sock_event_source_prepare,
		pup_sock_event_source_query,
		pup_sock_event_source_dispatch,
		pup_sock_event_source_finalize,
		NULL, NULL};

	GSource *source = g_source_new(&funclist, sizeof(PupSockEventSource));
	((PupSockEventSource *) source)->sock = sock;
	g_source_add_poll(source, &(sock->gfd));

	sock->event_source = g_source_attach(source, context);
	sock->props |= PUPSOCK_HAS_G_MAIN;
	return sock->event_source;
}

void pup_sock_set_destroy_params(PupSock *sock, gboolean attempt_flush,
                                 gdouble timeout)
{
	sock->attempt_flush = attempt_flush;
	sock->timeout = timeout;
}

void pup_sock_finalize(GObject *instance)
{
	PupSock *sock = PUP_SOCK(instance);

	GList *iter;

	//Flush buffers
	if (sock->attempt_flush)
	{
		pup_sock_send(sock, sock->timeout, NULL);
		pup_sock_receive(sock, 0.0, G_MAXUINT, NULL);
	}

	//Remove from main loop
	if (sock->event_source) g_source_remove(sock->event_source);

	//Destroy buffers
	for G_LIST_ITER(sock->read_data->head, iter)
	{
		pup_sock_buffer_destroy((PupSockBuffer *) iter->data);
	}
	g_queue_free(sock->read_data);
	for G_LIST_ITER(sock->write_buf->head, iter)
	{
		pup_sock_buffer_destroy((PupSockBuffer *) iter->data);
	}
	g_queue_free(sock->write_buf);
	pup_sock_buffer_destroy(sock->read_buf);

	//Free the socket address string if any
	if (sock->address) g_free(sock->address);

	//Shutdown the socket
	close(sock->fd);

	//Call parent's finalize
	G_OBJECT_CLASS(pup_sock_parent_class)->finalize(instance);
}


