//client-monitor.c or client-monitor.h
//Connecting to server, getting jobs done through server, etc.

#ifndef PUP_VM_H_INSIDE
//client-monitor.c
#include "common.h"

#else // !PUP_VM_H_INSIDE
//client-monitor.h

typedef struct
{
	PupVMMonitor parent;

	PupConvMgr *cmgr;
	PupConv *event_conv;
	gboolean initialized;

	GType volume_type;
	GType drive_type;
	GObject *volume_monitor;
} PupClientMonitor;

typedef struct 
{
	PupVMMonitorClass parent;

	guint hup_signal_id;
} PupClientMonitorClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_CLIENT_MONITOR PupClientMonitor pup_client_monitor pup
#define PUP_TYPE_CLIENT_MONITOR (pup_client_monitor_get_type())
#define PUP_CLIENT_MONITOR(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_CLIENT_MONITOR, PupClientMonitor))
#define PUP_IS_CLIENT_MONITOR(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_CLIENT_MONITOR))
#define PUP_CLIENT_MONITOR_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_CLIENT_MONITOR, PupClientMonitorClass))
#define PUP_IS_CLIENT_MONITOR_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_CLIENT_MONITOR))
#define PUP_CLIENT_MONITOR_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_CLIENT_MONITOR, PupClientMonitorClass))
#define PUP_CLIENT_MONITOR_NAME "PupClientMonitor"

typedef struct
{
	GObject parent;

	PupDevice *holder;
	PupClientMonitor *monitor;
	GObject *volume_monitor;
	gpointer iface;
} PupClientDevice;

typedef void (*PupClientDeviceSetup) (PupClientDevice *client_device,
                                      PupDevice *holder);
typedef struct
{
	GObjectClass parent;

	PupClientDeviceSetup setup_func;
} PupClientDeviceClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_CLIENT_DEVICE PupClientDevice pup_client_device pup
#define PUP_TYPE_CLIENT_DEVICE (pup_client_device_get_type())
#define PUP_CLIENT_DEVICE(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_CLIENT_DEVICE, PupClientDevice))
#define PUP_IS_CLIENT_DEVICE(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_CLIENT_DEVICE))
#define PUP_CLIENT_DEVICE_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_CLIENT_DEVICE, PupClientDeviceClass))
#define PUP_IS_CLIENT_DEVICE_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_CLIENT_DEVICE))
#define PUP_CLIENT_DEVICE_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_CLIENT_DEVICE, PupClientDeviceClass))
#define PUP_CLIENT_DEVICE_NAME "PupClientDevice"

typedef struct 
{
	PupRemoteOperation parent;

	PupDevice *dev;
	GMountOperation *mount_operation;
	GSimpleAsyncResult *result;
	guint current_query;
} PupGIOOperation;

#endif //PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
void pup_client_unlock(PupClientDevice *self);
void pup_client_lock(PupClientDevice *self);
gint pup_client_device_compare(gconstpointer dev1,gconstpointer dev2);
PupClientDevice *pup_client_device_get_from_device(PupClientMonitor *monitor,PupDevice *dev);
void pup_client_device_release_device(gpointer dev);
void pup_client_device_finalize(GObject *instance);
void pup_client_device_register(GIOModule *module);
GQuark pup_client_device_quark();
#if !(!defined(PUP_VM_H_INSIDE))
GType pup_client_device_get_type();
#endif
void pup_client_monitor_user_respond_cb(GMountOperation *mount_operation,GMountOperationResult abort,PupGIOOperation *gio_operation);
void pup_client_monitor_ask_question_cb(PupRemoteOperation *operation,const gchar *question,const gchar **choices);
void pup_client_monitor_ask_passwd_cb(PupRemoteOperation *operation,const gchar *msg,GAskPasswordFlags flags);
void pup_client_monitor_operation_return_cb(PupRemoteOperation *operation,gboolean success,guint error_code,const gchar *detail);
void pup_client_monitor_start_operation(PupClientMonitor *monitor,PupDevice *dev,const gchar *oper_name,const gchar *args,GMountOperation *mount_operation,GSimpleAsyncResult *result);
void pup_client_monitor_svr_event_cb(PupConv *conv,PSDataParser *rcvd_data,gboolean is_new,PupClientMonitor *monitor,gpointer dummy);
gboolean pup_client_monitor_connect(PupClientMonitor *self);
void pup_client_monitor_disconnect_cb(PupSock *sock,PupClientMonitor *self);
void pup_client_monitor_get_devices_cb(PupConv *conv,PSDataParser *rcvd_data,gboolean is_new,PupClientMonitor *monitor,gpointer dummy);
void pup_client_monitor_device_event_cb(PupVMMonitor *monitor,PupDevice *dev,guint event,const gchar *detail);
void pup_client_monitor_finalize(GObject *instance);
void pup_client_monitor_register(GIOModule *module);
#if !(!defined(PUP_VM_H_INSIDE))
GType pup_client_monitor_get_type();
#endif
#if !defined(PUP_VM_H_INSIDE)
G_DEFINE_DYNAMIC_TYPE(PupClientMonitor,pup_client_monitor,PUP_TYPE_VM_MONITOR);
G_DEFINE_DYNAMIC_TYPE(PupClientDevice,pup_client_device,G_TYPE_OBJECT);
#endif
