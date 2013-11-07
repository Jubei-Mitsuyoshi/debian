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

/*Everything beyond this is automatically generated*/
gboolean pstimer_is_time_remaining(PSTimer *timer);
struct timeval *pstimer_get_remaining_time(PSTimer *timer,struct timeval *time_return);
void pstimer_destroy(PSTimer *timer);
PSTimer *pstimer_new(const gdouble target);
int psutil_set_nonblock_flag(int fd,gboolean value);
void psutil_fill_sockaddr_local(const char *name,struct sockaddr **addr,gsize *size_return);
