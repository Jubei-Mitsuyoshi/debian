//socket.c or socket.h
//Contains functions to manage socket connections

#ifndef PUP_VM_H_INSIDE
//socket.c
#	include "common-includes.h"

#else // !PUP_VM_H_INSIDE
//socket.h
typedef gboolean (*PupSockAcceptCB) (PupSock *sock,
                                     PupSock *new_client,
                                     gpointer user_data);

#endif // PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
void pup_sock_connection_request_callback(PupSock *server);
void pup_sock_accept_connection(PupSock *server,gdouble timeout,PupSock **sock_return,GError **error);
void pup_sock_connect_local(PupSock *sock,const char *address,GError **error);
void pup_sock_setup_as_local_server(PupSock *sock,const char *address,GError **error);
