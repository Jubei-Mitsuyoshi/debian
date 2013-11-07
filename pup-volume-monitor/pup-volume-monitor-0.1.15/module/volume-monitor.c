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

#endif // PUP_VM_H_INSIDE

//FILE_HEADER_END

#ifndef PUP_VM_H_INSIDE
G_DEFINE_DYNAMIC_TYPE_EXTENDED
	(PupVolumeMonitor, pup_volume_monitor, G_TYPE_NATIVE_VOLUME_MONITOR, 0,
	 pup_volume_monitor_implement_extension_point());
#else
GType pup_volume_monitor_get_type();
#endif

//construction and destruction

static void pup_volume_monitor_class_init(PupVolumeMonitorClass *klass)
{
	GObjectClass *basic_class = G_OBJECT_CLASS(klass);
	GVolumeMonitorClass *monitor_class = G_VOLUME_MONITOR_CLASS(klass);

	basic_class->finalize = pup_volume_monitor_finalize;

	monitor_class->is_supported = pup_volume_monitor_is_supported;
	monitor_class->get_volumes = pup_volume_monitor_get_volumes;
	monitor_class->get_connected_drives = pup_volume_monitor_get_drives;
	monitor_class->get_mounts = pup_volume_monitor_get_mounts;
	monitor_class->get_volume_for_uuid = pup_volume_monitor_get_volume_for_uuid;
	monitor_class->get_mount_for_uuid = pup_volume_monitor_get_mount_for_uuid;
}

static void pup_volume_monitor_class_finalize(PupVolumeMonitorClass *klass)
{
	
}

void pup_volume_monitor_implement_extension_point()
{
	if (g_getenv(PUP_VOLUME_MONITOR_BE_NATIVE))
		g_io_extension_point_implement(G_NATIVE_VOLUME_MONITOR_EXTENSION_POINT_NAME,
		                               PUP_TYPE_VOLUME_MONITOR,
		                               PUP_VOLUME_MONITOR_NAME,
		                               10);
	else
		g_io_extension_point_implement(G_VOLUME_MONITOR_EXTENSION_POINT_NAME,
		                               PUP_TYPE_VOLUME_MONITOR,
		                               PUP_VOLUME_MONITOR_NAME,
		                               10);
}

static void pup_volume_monitor_init(PupVolumeMonitor *self)
{
	//Connect to server
	pup_volume_monitor_attempt_connect(self);
	self->initialized = TRUE;
}

void pup_volume_monitor_finalize(GObject *instance)
{
	PupVolumeMonitor *self = PUP_VOLUME_MONITOR(instance);

	if (self->reconnect_source_tag)
		g_source_remove(self->reconnect_source_tag);

	if (self->monitor)
		pup_vm_monitor_destroy(PUP_VM_MONITOR(self->monitor));

	G_OBJECT_CLASS(pup_volume_monitor_parent_class)->finalize(instance);
}

void pup_volume_monitor_register(GIOModule *module)
{
	pup_volume_monitor_register_type(G_TYPE_MODULE(module));
}


//Private Methods

gboolean pup_volume_monitor_attempt_connect(PupVolumeMonitor *self)
{
	PupClientMonitor *monitor
		= PUP_CLIENT_MONITOR(g_object_new(PUP_TYPE_CLIENT_MONITOR, NULL));
	g_signal_connect(monitor, "device-event",
	                 G_CALLBACK(pup_volume_monitor_raise_events_cb), self);
	if (! pup_client_monitor_connect(monitor))
	{
		//Failed, will retry after 10 seconds
		pup_volume_monitor_disconnect(monitor, self);
		return TRUE;
	}
	else
	{
		self->monitor = monitor;
		if (self->reconnect_source_tag)
		{
			g_source_remove(self->reconnect_source_tag);
			self->reconnect_source_tag = 0;
		}
		g_signal_connect(self->monitor, "hup",
		                 G_CALLBACK(pup_volume_monitor_disconnect), self);
		return FALSE;
	}
}

void pup_volume_monitor_disconnect(PupClientMonitor *monitor,
                                      PupVolumeMonitor *self)
{
	pup_vm_monitor_destroy(PUP_VM_MONITOR(monitor));
	self->monitor = NULL;
	if (! self->reconnect_source_tag)
	{
		//Retry after 10 seconds
		self->reconnect_source_tag = g_timeout_add_seconds
			(10, (GSourceFunc) pup_volume_monitor_attempt_connect, self);
	}
}

GList *pup_volume_monitor_get_devices(PupVolumeMonitor *self,
                                      GHashTable *htable)
{
	GList *list = NULL;
	GHashTableIter iter;
	PupDevice *dev;
	g_hash_table_iter_init(&iter, htable);
	while (g_hash_table_iter_next(&iter, NULL, (gpointer *) &dev))
	{
		PupClientDevice *client_dev
			= pup_client_device_get_from_device(self->monitor, dev);
		g_object_ref(client_dev);
		list = g_list_prepend(list, client_dev->iface);
	}
	list = g_list_sort(list, pup_client_device_compare);

	return list;
}

void pup_volume_monitor_raise_events_cb(PupClientMonitor *monitor,
                                        PupDevice *dev,
                                        guint event,
                                        const gchar *detail,
                                        PupVolumeMonitor *self)
{
	//Don't raise events during initialization
	if (! self->initialized) return;
	PupClientDevice *client_dev = pup_client_device_get_from_device
			(monitor, dev);
	
	if (event == PUP_DEVICE_ADD)
	{
		//Add event 
		if (PUP_IS_VOLUME(dev))
		{
			//Volume
			GMount *mount = pup_client_volume_get_mount(G_VOLUME(client_dev));
			g_signal_emit_by_name(self, "volume-added", G_VOLUME(client_dev));
			if (mount)
				g_signal_emit_by_name(self, "mount-added", mount);
		}
		else
		{
			//Drive
			g_signal_emit_by_name(self, "drive-connected", G_DRIVE(client_dev));
		}
	}
	else if (event == PUP_DEVICE_REMOVE)
	{
		//Remove event
		if (PUP_IS_VOLUME(dev))
		{
			//Volume
			GMount *mount = pup_client_volume_get_mount(G_VOLUME(client_dev));
			g_signal_emit_by_name(client_dev, "removed");
			g_signal_emit_by_name(self, "volume-removed", G_VOLUME(client_dev));
			if (mount)
				g_signal_emit_by_name(self, "mount-removed", mount);
		}
		else
		{
			//Drive
			g_signal_emit_by_name(client_dev, "disconnected");
			g_signal_emit_by_name(self, "drive-disconnected", G_DRIVE(client_dev));
		}
	}
	else if (event == PUP_DEVICE_CHANGE)
	{
		//Change can be of many types
		if (PUP_IS_VOLUME(dev))
		{
			//Volume
			PupMount *mount = pup_mount_get(PUP_CLIENT_VOLUME(client_dev));
			if (g_strcmp0(detail, PUP_VOLUME_EVENT_MOUNT) == 0)
			{
				g_signal_emit_by_name(self, "mount-added", G_MOUNT(mount));
			}
			else if (g_strcmp0(detail, PUP_VOLUME_EVENT_UMOUNT) == 0)
			{
				g_signal_emit_by_name(self, "mount-removed", G_MOUNT(mount));
			}
			else if (g_strcmp0(detail, PUP_VOLUME_EVENT_REMOUNT) == 0)
			{
				g_signal_emit_by_name(self, "mount-changed", G_MOUNT(mount));
			}
			g_signal_emit_by_name(client_dev, "changed");
			g_signal_emit_by_name(self, "volume-changed", G_VOLUME(client_dev));
			
			//G_BREAKPOINT();
		}
		else
		{
			//Drive
			g_signal_emit_by_name(client_dev, "changed");
			g_signal_emit_by_name(self, "drive-changed", G_DRIVE(client_dev));
		}
	}
	else
		g_critical("Invalid event %d", event);
}

//Public methods

gboolean pup_volume_monitor_generic_finish(GObject *object, GAsyncResult *result,
                                           GError **error)
{
	gboolean res = TRUE;
	if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(result),
	                                          error))
	{
		res = FALSE;
	}

	return res;
}

gboolean pup_volume_monitor_is_supported()
{
	return TRUE;
}

GList *pup_volume_monitor_get_volumes(GVolumeMonitor *monitor)
{
	PupVolumeMonitor *self = PUP_VOLUME_MONITOR(monitor);
	g_return_val_if_fail(self, NULL);
	if (! self->monitor) return NULL;
	
	pup_vm_monitor_lock(PUP_VM_MONITOR(self->monitor));
	
	GList *list = pup_volume_monitor_get_devices
		(self, PUP_VM_MONITOR(self->monitor)->volumes);
	GList *iter;
	for (iter = list; iter; iter = iter->next)
		iter->data = (gpointer) G_VOLUME(iter->data);
	
	pup_vm_monitor_unlock(PUP_VM_MONITOR(self->monitor));
	
	return list;
}

GList *pup_volume_monitor_get_drives(GVolumeMonitor *monitor)
{
	PupVolumeMonitor *self = PUP_VOLUME_MONITOR(monitor);
	g_return_val_if_fail(self, NULL);
	if (! self->monitor) return NULL;
	
	pup_vm_monitor_lock(PUP_VM_MONITOR(self->monitor));
	
	GList *list = pup_volume_monitor_get_devices
		(self, PUP_VM_MONITOR(self->monitor)->drives);
	GList *iter;
	for (iter = list; iter; iter = iter->next)
		iter->data = (gpointer) G_DRIVE(iter->data);
	
	pup_vm_monitor_unlock(PUP_VM_MONITOR(self->monitor));
	
	return list;
}

GList *pup_volume_monitor_get_mounts(GVolumeMonitor *monitor)
{
	GList *list, *iter, *bak;
	list = pup_volume_monitor_get_volumes(monitor);
	for (iter = list; iter; iter = bak)
	{
		bak = iter->next;
		g_object_unref(G_OBJECT(iter->data));
		if (! (iter->data = pup_client_volume_get_mount(iter->data)))
			list = g_list_delete_link(list, iter);
	}

	return list;
}

GVolume *pup_volume_monitor_get_volume_for_uuid(GVolumeMonitor *monitor,
                                                const gchar *uuid)
{
	PupVolumeMonitor *self = PUP_VOLUME_MONITOR(monitor);
	PupDevice *dev;
	pup_vm_monitor_lock(PUP_VM_MONITOR(self->monitor));
	dev = PUP_DEVICE(g_hash_table_find(PUP_VM_MONITOR(self->monitor)->volumes, 
	                        pup_client_volume_uuid_test_hfunc, (gpointer) uuid));
	pup_vm_monitor_unlock(PUP_VM_MONITOR(self->monitor));
	if (! dev) return NULL;
	return G_VOLUME(pup_client_device_get_from_device(self->monitor, dev));
}

GMount *pup_volume_monitor_get_mount_for_uuid(GVolumeMonitor *monitor,
                                              const gchar *uuid)
{
	GVolume *volume = pup_volume_monitor_get_volume_for_uuid(monitor, uuid);
	if (! volume) return NULL;
	return G_MOUNT(pup_mount_get(PUP_CLIENT_VOLUME(volume)));
}

