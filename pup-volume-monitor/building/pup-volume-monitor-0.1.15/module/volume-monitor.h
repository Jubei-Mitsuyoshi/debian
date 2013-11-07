//volume_monitor.c or volume_monitor.h
//GVolumeMonitor class implementation: PupVolumeMonitor

#ifndef PUP_VM_H_INSIDE
//volume_monitor.c
#	include "common.h"

#else // !PUP_VM_H_INSIDE
//volume_monitor.h

typedef struct
{
	GNativeVolumeMonitor parent;

	PupClientMonitor *monitor;
	guint reconnect_source_tag;

	gboolean initialized;
} PupVolumeMonitor;

typedef struct
{
	GNativeVolumeMonitorClass parent;

} PupVolumeMonitorClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_VOLUME_MONITOR PupVolumeMonitor pup_volume_monitor pup
#define PUP_TYPE_VOLUME_MONITOR (pup_volume_monitor_get_type())
#define PUP_VOLUME_MONITOR(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_VOLUME_MONITOR, PupVolumeMonitor))
#define PUP_IS_VOLUME_MONITOR(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_VOLUME_MONITOR))
#define PUP_VOLUME_MONITOR_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_VOLUME_MONITOR, PupVolumeMonitorClass))
#define PUP_IS_VOLUME_MONITOR_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_VOLUME_MONITOR))
#define PUP_VOLUME_MONITOR_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_VOLUME_MONITOR, PupVolumeMonitorClass))
#define PUP_VOLUME_MONITOR_NAME "PupVolumeMonitor"

#endif // PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
gboolean pup_volume_monitor_generic_finish(GObject *object,GAsyncResult *result,GError **error);
GList *pup_volume_monitor_get_devices(PupVolumeMonitor *self,GHashTable *htable);
void pup_volume_monitor_disconnect(PupClientMonitor *monitor,PupVolumeMonitor *self);
void pup_volume_monitor_raise_events_cb(PupClientMonitor *monitor,PupDevice *dev,guint event,const gchar *detail,PupVolumeMonitor *self);
void pup_volume_monitor_register(GIOModule *module);
gboolean pup_volume_monitor_attempt_connect(PupVolumeMonitor *self);
GMount *pup_volume_monitor_get_mount_for_uuid(GVolumeMonitor *monitor,const gchar *uuid);
GVolume *pup_volume_monitor_get_volume_for_uuid(GVolumeMonitor *monitor,const gchar *uuid);
GList *pup_volume_monitor_get_mounts(GVolumeMonitor *monitor);
GList *pup_volume_monitor_get_drives(GVolumeMonitor *monitor);
GList *pup_volume_monitor_get_volumes(GVolumeMonitor *monitor);
gboolean pup_volume_monitor_is_supported();
void pup_volume_monitor_finalize(GObject *instance);
#if !(!defined(PUP_VM_H_INSIDE))
GType pup_volume_monitor_get_type();
#endif
void pup_volume_monitor_implement_extension_point();
#if !defined(PUP_VM_H_INSIDE)
G_DEFINE_DYNAMIC_TYPE_EXTENDED(PupVolumeMonitor,pup_volume_monitor,G_TYPE_NATIVE_VOLUME_MONITOR,0,pup_volume_monitor_implement_extension_point());
#endif
