//transfer.c or transfer.h
//Handles actual data transfer

#ifndef PUP_VM_H_INSIDE
//transfer.c
#	include "common-includes.h"

#else // !PUP_VM_H_INSIDE
//transfer.h
typedef guint32 PupSockLen;
#	define PUPSOCK_LEN_SIZE sizeof(PupSockLen)

typedef struct
{
	gint8 *data;
	gsize len;
} PupSockData;

typedef void (*PupSockDataRecvdCB) (PupSock *sock, PupSockData *data,
                                    gpointer user_data);

typedef void (*PupSockHungUpCB) (PupSock *sock, gpointer user_data);

#	define pup_sock_buffer_push(q, b) g_queue_push_head((q), (gpointer)( b))

#	define pup_sock_buffer_pop(q) ((PupSockBuffer *) g_queue_pop_tail((q)))

#	define pup_sock_buffer_knock(q) pup_sock_buffer_destroy(pup_sock_buffer_pop(q))

#	define pup_sock_buffer_peek(q) ((PupSockBuffer *) g_queue_peek_tail((q)))

#endif // PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
void pup_sock_broken_pipe_handler(PupSock *sock);
void pup_sock_output_callback(PupSock *sock);
void pup_sock_input_callback(PupSock *sock);
void pup_sock_get_queue_length(PupSock *sock,guint *sending,guint *recving);
guint pup_sock_receive(PupSock *sock,gdouble timeout,guint num_blocks,GError **error);
gboolean pup_sock_try_receive_block(PupSock *sock,PSTimer *timer,gboolean *data_read,GError **error);
guint pup_sock_send(PupSock *sock,gdouble timeout,GError **error);
gboolean pup_sock_try_send_block(PupSock *sock,PSTimer *timer,gboolean *data_sent,GError **error);
PupSockData *pup_sock_pop_recvd_block(PupSock *sock,PupSockData *dest);
void pup_sock_queue_send_block(PupSock *sock,gpointer mem,gsize size);
void pup_sock_queue_buffer(PupSock *sock,PupSockBuffer *buf);
PupSockBuffer *pup_sock_buffer_new(guint len);
void pup_sock_buffer_destroy(PupSockBuffer *buf);
