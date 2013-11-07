//utils.c or utils.h
//Useful utility functions and macros

#ifndef PUP_VM_H_INSIDE
//utils.c
#	include "common-includes.h"

#else // !PUP_VM_H_INSIDE
//utils.h

#	define G_LIST_ITER(l, i) (i = l; i != NULL; i = i->next)

typedef struct
{
	GTimer *timer;
	gdouble target;
} PSTimer;

#	define PUP_STATIC static

#endif // PUP_VM_H_INSIDE

//FILE_HEADER_END

void psutil_fill_sockaddr_local(const char *name, struct sockaddr **addr,
                                gsize *size_return)
{
	*size_return = offsetof (struct sockaddr_un, sun_path) + strlen(name) + 1;

	*addr = (struct sockaddr *) g_malloc(*size_return);

	((struct sockaddr_un *) *addr)->sun_family = AF_LOCAL;
	strcpy(((struct sockaddr_un *) *addr)->sun_path, name);
}

//Non-blocking
int psutil_set_nonblock_flag (int fd, gboolean value)
{
	int oldflags = fcntl (fd, F_GETFL, 0);
	if (oldflags < 0)
		return -1;
	if (value)
		oldflags |= O_NONBLOCK;
	else
		oldflags &= ~O_NONBLOCK;
	return fcntl (fd, F_SETFL, oldflags);
}

//Timer
PSTimer *pstimer_new(const gdouble target)
{
	PSTimer *timer;

	if (target < 0.0) return NULL;
	timer = g_slice_new(PSTimer);
	timer->target = target;
	timer->timer = g_timer_new();
	return timer;
}

void pstimer_destroy(PSTimer *timer)
{
	g_timer_destroy(timer->timer);
	g_slice_free(PSTimer, timer);
}

struct timeval *pstimer_get_remaining_time(PSTimer *timer,
                                           struct timeval *time_return)
{
	if (!timer) return NULL;
	gdouble remaining = timer->target
		- g_timer_elapsed(timer->timer, 
	    	                 (gulong *) &(time_return->tv_usec));
	if (remaining < 0) remaining = time_return->tv_usec = 0;
	time_return->tv_sec = (gulong) floor(remaining);
	return time_return;
}

gboolean pstimer_is_time_remaining(PSTimer *timer)
{
	if (! timer) return TRUE;
	return (timer->target > g_timer_elapsed(timer->timer, NULL));
}


