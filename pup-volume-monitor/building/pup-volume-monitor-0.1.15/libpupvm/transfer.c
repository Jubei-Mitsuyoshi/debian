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

//FILE_HEADER_END


PupSockBuffer *pup_sock_buffer_new(guint len)
{
	PupSockBuffer *retval = g_slice_new(PupSockBuffer);
	retval->rw_ptr = 0;
	retval->len = len;
	if (len)
	{
		retval->data = g_new(gint8, len);
	}
	else
	{
		retval->data = g_new(gint8, PUPSOCK_LEN_SIZE);
	}

	return retval;
}

void pup_sock_buffer_destroy(PupSockBuffer *buf)
{
	g_free((gpointer) buf->data);
	g_slice_free(PupSockBuffer, (gpointer) buf);
}

void pup_sock_queue_buffer(PupSock *sock, PupSockBuffer *buf)
{
	pup_sock_buffer_push(sock->write_buf, buf);
	sock->gfd.events |= G_IO_OUT;
}

void pup_sock_queue_send_block(PupSock *sock, gpointer mem, gsize size)
{
	PupSockBuffer *buf = pup_sock_buffer_new(size + PUPSOCK_LEN_SIZE);
	*((PupSockLen *) buf->data) = (PupSockLen) size;
	g_memmove((gpointer) &(buf->data[PUPSOCK_LEN_SIZE]), mem, size);
	pup_sock_queue_buffer(sock, buf);
}

PupSockData *pup_sock_pop_recvd_block(PupSock  *sock, PupSockData *dest)
{
	PupSockBuffer *buf = pup_sock_buffer_pop(sock->read_data);
	if (buf)
	{
		dest->data = (gpointer) buf->data;
		dest->len = buf->len;
		g_slice_free(PupSockBuffer, (gpointer) buf);
		return dest;
	}
	else
	{
		return NULL;
	}	
}

gboolean pup_sock_try_send_block(PupSock *sock, PSTimer *timer,
                                 gboolean *data_sent, GError **error)
{
	gssize bytes_written;
	gboolean sent = FALSE;
	if (data_sent) *data_sent = FALSE;

	struct timeval time;
	struct timeval *time_ptr;
	fd_set set;
	int retval;

	//Get current buffer
	PupSockBuffer *buf = pup_sock_buffer_peek(sock->write_buf);
	if (!buf) return sent;

	//Wait till timeout or till socket is ready for writing
	time_ptr = pstimer_get_remaining_time(timer, &time);
	FD_ZERO (&set);
	FD_SET (sock->fd, &set);
	retval = select(FD_SETSIZE, NULL, &set, NULL, time_ptr);
	if (retval == 0);
	else if (retval < 0)
	{
		//Usually this should not happen, so print it to screen
		g_critical("select() returned error: %s", g_strerror(errno));
	}
	else
	{
		//Now try sending one block
		bytes_written = write(sock->fd, &(buf->data[buf->rw_ptr]),
		                      buf->len - buf->rw_ptr);
		if (bytes_written < 0)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				g_set_error(error, PUPSOCK_ERR_DOMAIN, errno, "%s", 
				            g_strerror(errno));
				return sent;
			}
			bytes_written = 0;
		}
		if (bytes_written != 0) 
			if (data_sent) *data_sent = FALSE;

		buf->rw_ptr += bytes_written;

		//Check for completion
		//If yes then knock off current buffer, it has been sent completely.
		if (buf->rw_ptr >= buf->len)
		{
			pup_sock_buffer_knock(sock->write_buf);
			sent = TRUE;
		}
	}

	return sent;
}

guint pup_sock_send(PupSock *sock, gdouble timeout, GError **error)
{
	g_return_val_if_fail(sock->props & PUPSOCK_IS_CONNECTED, 0);
	gboolean stat, data_sent;
	guint num_blocks = 0;
	g_return_val_if_fail(error == NULL || *error == NULL, num_blocks);
	GError *err_val = NULL;
	GError **err_ptr = (error == NULL) ? &err_val : error;

	//Start timer.
	PSTimer *timer = pstimer_new(timeout);

	//Try sending as much as possible
	while (pup_sock_buffer_peek(sock->write_buf) != NULL)
	{	
		//Try sending one block;
		stat = pup_sock_try_send_block(sock, timer, &data_sent, err_ptr);
		if (*err_ptr) return num_blocks;

		if (stat) num_blocks++;

		if ((! pstimer_is_time_remaining(timer)) && !(data_sent))
		{
			break;
		}
	}

	if ((!num_blocks) && (pup_sock_buffer_peek(sock->write_buf) != NULL))
	{
		g_set_error(error, PUPSOCK_ERR_DOMAIN, EWOULDBLOCK, "%s", 
		            g_strerror(EWOULDBLOCK));
	}

	return num_blocks;
}

gboolean pup_sock_try_receive_block(PupSock *sock, PSTimer *timer,
                                    gboolean *data_read, GError **error)
{
	gssize bytes_read;
	gsize len;
	gboolean recvd = FALSE; 
	if (data_read) *data_read = FALSE;
	PupSockData cb_data;

	struct timeval time;
	struct timeval *time_ptr;
	fd_set set;
	int retval;

	//Get amount of data we have to read.
	if (sock->read_buf->len == 0)
		len = PUPSOCK_LEN_SIZE;
	else
		len = sock->read_buf->len;

	//Wait till timeout or till socket is ready for reading
	time_ptr = pstimer_get_remaining_time(timer, &time);
	FD_ZERO (&set);
	FD_SET (sock->fd, &set);
	retval = select(FD_SETSIZE, &set, NULL, NULL, time_ptr);
	if (retval == 0);
	else if (retval < 0)
	{
		//Usually this should not happen, so print it to screen
		g_critical("select() returned error: %s", g_strerror(errno));
	}
	else
	{
		//Now try reading
		bytes_read = read(sock->fd,
		                  &(sock->read_buf->data[sock->read_buf->rw_ptr]),
		                  len - sock->read_buf->rw_ptr);
		if (bytes_read < 0)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				g_set_error(error, PUPSOCK_ERR_DOMAIN, errno, "%s", 
				            g_strerror(errno));
				return recvd;
			}
			bytes_read = 0;
		}
		else if (bytes_read == 0)
		{
			if (len - sock->read_buf->rw_ptr)
			{
				//EOF
				g_set_error(error, PUPSOCK_ERR_DOMAIN, EOF, 
				            "received EOF on the socket");
				return recvd;
			}
		}
		else if (data_read) *data_read = TRUE;
		sock->read_buf->rw_ptr += bytes_read;
		//Check progress
		if (sock->read_buf->rw_ptr >= len)
		{
			if (sock->read_buf->len == 0)
			{
				//Size of the block has been received, allocate buffer for it
				len = (guint) *((PupSockLen *) sock->read_buf->data);
				pup_sock_buffer_destroy(sock->read_buf);
				sock->read_buf = pup_sock_buffer_new(len);
			}
			else
			{
				//Block received completely
				//Launch callback functions
				cb_data.data = sock->read_buf->data;
				cb_data.len = (gsize) sock->read_buf->len;
				g_signal_emit(sock,
				              PUP_SOCK_GET_CLASS(sock)->data_recvd_signal_id,
				              0, &cb_data);
				//Push the filled buffer on the queue, to be lifted off by 
				//programs
				if (sock->props & PUPSOCK_SAVE_READ_DATA) 
					pup_sock_buffer_push(sock->read_data, sock->read_buf);
				else
					pup_sock_buffer_destroy(sock->read_buf);
				//Create new buffer to get size of next block.
				sock->read_buf = pup_sock_buffer_new(0);
				recvd = TRUE;
			}
		}
	}

	return recvd;
}

guint pup_sock_receive(PupSock *sock, gdouble timeout, guint num_blocks,
                       GError **error)
{
	g_return_val_if_fail(sock->props & PUPSOCK_IS_CONNECTED, 0);
	gboolean stat, data_read;
	guint recvd_blocks = 0;

	g_return_val_if_fail(error == NULL || *error == NULL, recvd_blocks);
	GError *err_val = NULL;
	GError **err_ptr = (error == NULL) ? &err_val : error;

	//Start timer
	PSTimer *timer = pstimer_new(timeout);

	do
	{
		//Try receiving block
		stat = pup_sock_try_receive_block(sock, timer, &data_read, err_ptr);
		if (*err_ptr) return recvd_blocks;
		if (stat) recvd_blocks++;

		//Return after receiving ${num_blocks} blocks
		if (recvd_blocks >= num_blocks) break;
	} while (pstimer_is_time_remaining(timer) || data_read);

	if ((recvd_blocks < num_blocks) && (num_blocks < G_MAXUINT))
	{
		g_set_error(error, PUPSOCK_ERR_DOMAIN, EWOULDBLOCK, "%s", 
		            g_strerror(EWOULDBLOCK));
	}

	return recvd_blocks;
}

void pup_sock_get_queue_length(PupSock *sock, guint *sending, guint *recving)
{
	if (sending) *sending = g_queue_get_length(sock->write_buf);
	if (recving) *recving = g_queue_get_length(sock->read_data);
}


void pup_sock_input_callback(PupSock *sock)
{
	GError *error = NULL;

	pup_sock_receive(sock, 0.0, G_MAXUINT, &error);
	if (error)
	{
		if (error->code != EWOULDBLOCK && error->code != EAGAIN
		    && error->code != EOF)
			g_critical("pup_sock_receive(): %s", error->message);
		g_clear_error(&error);
	}

}

void pup_sock_output_callback(PupSock *sock)
{
	GError *error = NULL;
	guint qlen;

	pup_sock_send(sock, 0.0, NULL);
	if (error)
	{
		if (error->code != EWOULDBLOCK && error->code != EAGAIN)
			g_critical("pup_sock_send(): %s", error->message);
		g_clear_error(&error);
	}

	pup_sock_get_queue_length(sock, &qlen, NULL);
	if (! qlen)
		sock->gfd.events &= ~G_IO_OUT;
}

void pup_sock_broken_pipe_handler(PupSock *sock)
{
	g_source_remove(sock->event_source);
	sock->props &= ~PUPSOCK_IS_CONNECTED;

	g_signal_emit(sock, PUP_SOCK_GET_CLASS(sock)->hup_signal_id, 0);
}
