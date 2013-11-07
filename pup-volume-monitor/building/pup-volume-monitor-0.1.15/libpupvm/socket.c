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

//FILE_HEADER_END


void pup_sock_setup_as_local_server(PupSock *sock, const char *address,
                                    GError **error)
{
	struct sockaddr *addr;
	gsize addr_size;

	psutil_fill_sockaddr_local(address, &addr, &addr_size);

	do
	{
		if (bind(sock->fd, addr, addr_size) < 0)
		{
			g_set_error(error, PUPSOCK_ERR_DOMAIN, errno, "%s",
			            g_strerror(errno));
			break;
		}

		if (listen(sock->fd, G_MAXSHORT) < 0)
		{
			g_set_error(error, PUPSOCK_ERR_DOMAIN, errno, "%s",
			            g_strerror(errno));
			break;
		}

		psutil_set_nonblock_flag(sock->fd, TRUE);

		sock->address = g_strdup(address);
		sock->props |= PUPSOCK_IS_SERVER;
	} while (0);

	g_free(addr);
}

void pup_sock_connect_local(PupSock *sock, const char *address, GError **error)
{
	struct sockaddr *addr;
	gsize addr_size;

	psutil_fill_sockaddr_local(address, &addr, &addr_size);

	do
	{
		if (connect(sock->fd, addr, addr_size) != 0)
		{
			g_set_error(error, PUPSOCK_ERR_DOMAIN, errno, "%s", g_strerror(errno));
			break;
		}
		psutil_set_nonblock_flag(sock->fd, TRUE);

		sock->props &= ~PUPSOCK_IS_SERVER;
		sock->props |= PUPSOCK_IS_CONNECTED;
	} while (0);

	g_free(addr);
}

void pup_sock_accept_connection(PupSock *server, gdouble timeout,
                                PupSock **sock_return, GError **error)
{
	int fd, retval;
	*sock_return = NULL;

	//We may need to wait...till socket is ready for reading
	struct timeval time;
	struct timeval *time_ptr;
	fd_set set;
	if (timeout < 0.0)
	{
		time_ptr = NULL;
	}
	else
	{
		time.tv_sec = (glong) floor(timeout);
		time.tv_usec = (glong) ((timeout - time.tv_sec) * G_USEC_PER_SEC);
		time_ptr = &time;
	}
	FD_ZERO (&set);
	FD_SET (server->fd, &set);

	//Now wait...
	retval = select(FD_SETSIZE, &set, NULL, NULL, time_ptr);
	if (retval == 0)
	{
		g_set_error(error, PUPSOCK_ERR_DOMAIN, EWOULDBLOCK, 
		            "No connection to accept");
		return;
	}
	else if (retval < 0)
	{
		//Usually this should not happen
		g_set_error(error, PUPSOCK_ERR_DOMAIN, errno, "%s", 
		            g_strerror(errno));
		g_critical("select() returned error: %s", g_strerror(errno));
		return;
	}
	fd = accept(server->fd, NULL, NULL);

	if (fd < 0)
	{
		g_set_error(error, PUPSOCK_ERR_DOMAIN, errno, "accept(): %s", 
		            g_strerror(errno));
		return;
	}

	//Connection established.
	*sock_return = pup_sock_new_from_fd(fd);
	psutil_set_nonblock_flag(fd, TRUE);
	(*sock_return)->props |= PUPSOCK_IS_CONNECTED;		

	//Launch callbacks on successfully establishing connection
	g_signal_emit(server, PUP_SOCK_GET_CLASS(server)->accept_signal_id,
	              0, *sock_return);
}

void pup_sock_connection_request_callback(PupSock *server)
{
	PupSock *new_client;
	GError *error = NULL;

	//Accept a connection

	pup_sock_accept_connection(server, 0.0, &new_client, &error);
	if (error)
	{
		g_critical("Couldn't accept connection: %s", error->message);
	}
}

