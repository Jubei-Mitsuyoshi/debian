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

/*Everything beyond this is automatically generated*/
void pup_client_operation_query_response_cb(PupConv *conv,PSDataParser *parser,gboolean is_new,PupClient *client,PupServerOperation *operation);
void pup_client_operation_msg_delayed(PupServerOperation *operation,PSDataEncoder *encoder);
void pup_client_operation_invalidate(PupConv *conv,PupServerOperation *operation);
void pup_client_operation_msg_cb(PupOperation *operation,PSDataEncoder *encoder);
void pup_client_start_operation(PupClient *client,PSDataParser *parser,PupConv *conv);
void pup_client_unset_event_conv_cb(PupConv *conv,PupClient *client);
void pup_client_send_event(PupServerMonitor *monitor,PSDataEncoder *encoder,PupConv *conv);
void pup_client_set_event_conv(PupClient *client,PupConv *conv);
void pup_client_disconnect_cb(PupSock *sock,PupClient *client);
void pup_client_new_conv_cb(PupConv *conv,PSDataParser *recvd_data,gboolean is_new,gpointer user_data,gpointer dummy);
void pup_client_destroy(PupClient *client);
void pup_server_new_client_cb(PupSock *sock,PupSock *new_connection,PupServer *self);
PupServer *pup_server_setup();
gboolean pup_server_check_is_running();
