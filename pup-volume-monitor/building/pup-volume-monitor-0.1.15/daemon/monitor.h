//monitor.c or monitor.h
//Server side volume monitor implementation

#ifndef PUP_VM_H_INSIDE
//monitor.c
#include "common.h"
#else
//monitor.h

typedef struct
{
	PupVMMonitor parent;

	gboolean initialized;

	//udev stuff
	struct udev *udev_ctx;
	GThread *udev_thread;

	//probing
	GThreadPool *dev_probe_thread_pool;
} PupServerMonitor;

typedef struct
{
	PupVMMonitorClass parent;

	guint broadcast_signal_id;
} PupServerMonitorClass;



//FILE_HEADER_SUBST:gobject_macro_gen PUP_SERVER_MONITOR PupServerMonitor pup_server_monitor pup
#define PUP_TYPE_SERVER_MONITOR (pup_server_monitor_get_type())
#define PUP_SERVER_MONITOR(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_SERVER_MONITOR, PupServerMonitor))
#define PUP_IS_SERVER_MONITOR(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_SERVER_MONITOR))
#define PUP_SERVER_MONITOR_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_SERVER_MONITOR, PupServerMonitorClass))
#define PUP_IS_SERVER_MONITOR_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_SERVER_MONITOR))
#define PUP_SERVER_MONITOR_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_SERVER_MONITOR, PupServerMonitorClass))
#define PUP_SERVER_MONITOR_NAME "PupServerMonitor"

#endif //PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
void pup_server_monitor_start_operation(PupServerMonitor *self,guint catagory,const gchar *sysname,PupOperation *operation);
gboolean pup_server_monitor_mounts_check_func(gpointer data);
void pup_drive_process_event(PupVMMonitor *monitor,struct udev_device *dev,gboolean process_change);
void pup_server_monitor_start_udev_thread(PupServerMonitor *self);
gpointer pup_server_monitor_udev_thread(gpointer data);
void pup_server_monitor_device_event_broadcast(PupVMMonitor *monitor,PSDataEncoder *encoder);
extern const gchar *pup_volume_events[];
extern const gchar *pup_event_strv[];
void pup_server_monitor_dump_contents(PupServerMonitor *self);
void pup_server_monitor_probe_thread_func(struct udev_device *dev,PupVMMonitor *monitor);
void pup_server_monitor_update_mnt_info(PupVMMonitor *monitor,gpointer dummy);
void pup_server_monitor_device_event_cb(PupVMMonitor *monitor,PupDevice *dev,guint event,const gchar *detail);
void pup_server_monitor_udev_event(PupVMMonitor *monitor,struct udev_device *dev);
#if !(!defined(PUP_VM_H_INSIDE))
GType pup_server_monitor_get_type();
#endif
#if !defined(PUP_VM_H_INSIDE)
G_DEFINE_TYPE(PupServerMonitor,pup_server_monitor,PUP_TYPE_VM_MONITOR);
#endif
