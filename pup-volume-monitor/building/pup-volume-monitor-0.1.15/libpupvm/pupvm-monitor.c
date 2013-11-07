//pupvm-monitor.c or pupvm-monitor.h
//The volume monitor, server side

#ifndef PUP_VM_H_INSIDE
//pupvm-monitor.c
#	include "common-includes.h"

#else // !PUP_VM_H_INSIDE
//pupvm-monitor.h

//icons
typedef struct
{
	gchar *devnode;
	gchar *mntpnt;
	guint flags;
} PupMntEntry;

struct _PupVMMonitor
{
	GObject parent;

	GHashTable *drives;
	GHashTable *volumes;

	gboolean udev_cancel;

	GHashTable *mounts;
	gchar *mtab_file;

	GStaticRecMutex lock;
};

typedef struct 
{
	GObjectClass parent;
	//Device management
	void (*device_event_cb) (PupVMMonitor *monitor, PupDevice *dev,
	                         guint event, const gchar *detail_if_any);
	//Udev events
	void (*udev_event) (PupVMMonitor *monitor, struct udev_device *dev);
	//Signals
	guint udev_signal_id;
	guint udev_after_signal_id;
	guint device_event_signal_id;
	guint mounts_check_signal_id;
	//icons
} PupVMMonitorClass;

typedef void (*PupUdevEventCB) (PupVMMonitor *monitor,
                                struct udev_device *dev, gpointer user_data);

typedef void (*PupDeviceEventCB) (PupVMMonitor *monitor,
                                  PupDevice *dev,
                                  guint action,
                                  const gchar *detail,
                                  gpointer user_data);

//FILE_HEADER_SUBST:gobject_macro_gen PUP_VM_MONITOR PupVMMonitor pup_vm_monitor pup

#endif // PUP_VM_H_INSIDE

//FILE_HEADER_END

void pup_mnt_entry_free(PupMntEntry *entry)
{
	g_return_if_fail(entry);
	g_free(entry->devnode);
	g_free(entry->mntpnt);
	g_slice_free(PupMntEntry, entry);
}



#ifndef PUP_VM_H_INSIDE
G_DEFINE_TYPE(PupVMMonitor, pup_vm_monitor, G_TYPE_OBJECT);
#else
GType pup_vm_monitor_get_type();
#endif

static PupVMMonitor *stored_monitor = NULL;

static void pup_vm_monitor_class_init(PupVMMonitorClass *klass)
{	
	klass->device_event_signal_id
		= g_signal_new("device-event", PUP_TYPE_VM_MONITOR, G_SIGNAL_RUN_LAST, 0,
		               NULL, NULL, g_cclosure_user_marshal_VOID__POINTER_UINT_STRING,
		               G_TYPE_NONE, 3, G_TYPE_POINTER, G_TYPE_UINT, G_TYPE_STRING);

	//Following is not required by clients
	if (pup_vm_is_client) return;

	klass->udev_signal_id
		= g_signal_new("udev", PUP_TYPE_VM_MONITOR, G_SIGNAL_RUN_LAST, 0,
		               NULL, NULL, g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE,
		               1, G_TYPE_POINTER);

	klass->udev_after_signal_id
		= g_signal_new("udev-after", PUP_TYPE_VM_MONITOR, G_SIGNAL_RUN_LAST, 0,
		               NULL, NULL, g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE,
		               1, G_TYPE_POINTER);
	
	klass->mounts_check_signal_id
		= g_signal_new("mounts-check", PUP_TYPE_VM_MONITOR, G_SIGNAL_RUN_LAST,
		               0, NULL, NULL, g_cclosure_marshal_VOID__VOID, 
		               G_TYPE_NONE, 0, G_TYPE_INVALID);
}

static void pup_vm_monitor_init(PupVMMonitor *self)
{
	self->drives = g_hash_table_new(g_str_hash, g_str_equal);
	self->volumes = g_hash_table_new(g_str_hash, g_str_equal);

	g_static_rec_mutex_init(&(self->lock));

	//Only this much is required by clients
	if (pup_vm_is_client) return;
	
	//mounts monitor
	self->mounts = g_hash_table_new_full(g_str_hash, g_str_equal, NULL,
	                                     (GDestroyNotify) pup_mnt_entry_free);

	//Check for /proc/mounts
	if (g_file_test("/proc/mounts", G_FILE_TEST_IS_REGULAR))
	{
		self->mtab_file = g_strdup("/proc/mounts");
	}
	else if (g_file_test(_PATH_MOUNTED, G_FILE_TEST_IS_REGULAR))
	{
		g_warning("Falling back from /proc/mounts to %s", _PATH_MOUNTED);
		self->mtab_file = g_strdup(_PATH_MOUNTED);
	}
	else
	{
		g_critical("No mount information is available.");
	}

	stored_monitor = self;
}

PupVMMonitor *pup_vm_monitor_get()
{
	return stored_monitor;
}

void pup_vm_monitor_lock(PupVMMonitor *self)
{
	g_static_rec_mutex_lock(&(self->lock));
}

void pup_vm_monitor_unlock(PupVMMonitor *self)
{
	g_static_rec_mutex_unlock(&(self->lock));
}

GHashTable *pup_vm_monitor_get_hash_table(PupVMMonitor *self, PupDevice *dev)
{
	GHashTable *table;
	if (PUP_IS_DRIVE(dev)) table = self->drives;
	else if (PUP_IS_VOLUME(dev)) table = self->volumes;
	else
	{
		return NULL;
	}
	return table;
}

gboolean pup_vm_monitor_add_device_unlocked(PupVMMonitor *self, PupDevice *dev)
{
	GHashTable *table = pup_vm_monitor_get_hash_table(self, dev);
	g_return_val_if_fail(table, FALSE);

	g_return_val_if_fail
		(! g_hash_table_lookup(table, dev->sysname), FALSE);

	g_hash_table_replace(table, dev->sysname, dev);
	pup_device_hold(dev);

	dev->monitor = self;
	
	PUP_VM_MONITOR_GET_CLASS(self)->device_event_cb(self, dev, PUP_DEVICE_ADD,
	                                                NULL);
	return TRUE;
}

void pup_vm_monitor_remove_device_unlocked(PupVMMonitor *self, PupDevice *dev)
{
	GHashTable *table = pup_vm_monitor_get_hash_table(self, dev);
	g_return_if_fail(table);

	PupDevice *real_dev;
	if ((real_dev = g_hash_table_lookup(table, dev->sysname)))
	{
		g_hash_table_remove(table, real_dev->sysname);
		real_dev->monitor = NULL;
		PUP_VM_MONITOR_GET_CLASS(self)->device_event_cb(self, real_dev,
		                                                PUP_DEVICE_REMOVE,
		                                                NULL);                       
		pup_device_release(real_dev);
	}
	else
	{
		g_critical("dev is not attached to monitor");
	}
}

void pup_vm_monitor_change_device_unlocked(PupVMMonitor *self, PupDevice *dev,
                                           const gchar *detail)
{
	GHashTable *table = pup_vm_monitor_get_hash_table(self, dev);
	g_return_if_fail(table);

	PupDevice *real_dev;
	g_return_if_fail((real_dev = g_hash_table_lookup(table, dev->sysname)));
	PUP_VM_MONITOR_GET_CLASS(self)->device_event_cb(self,
	                                                real_dev,
	                                                PUP_DEVICE_CHANGE,
	                                                detail);
}

PupDrive *pup_vm_monitor_lookup_drive_unlocked(PupVMMonitor *self,
                                               const gchar *sysname,
                                               gboolean dup)
{
	PupDrive *res = PUP_DRIVE(g_hash_table_lookup(self->drives, sysname));
	if (res && dup)
	{
		res = PUP_DRIVE(pup_device_dup(PUP_DEVICE(res)));
	}
	return res;
}

PupVolume *pup_vm_monitor_lookup_volume_unlocked(PupVMMonitor *self,
                                                 const gchar *sysname,
                                                 gboolean dup)
{
	PupVolume *res = PUP_VOLUME(g_hash_table_lookup(self->volumes, sysname));
	if (res && dup)
	{
		res = PUP_VOLUME(pup_device_dup(PUP_DEVICE(res)));
	}
	return res;
}

gboolean pup_vm_monitor_add_device(PupVMMonitor *self, PupDevice *dev)
{
	gboolean result;
	pup_vm_monitor_lock(self);
	result = pup_vm_monitor_add_device_unlocked(self, dev);
	pup_vm_monitor_unlock(self);

	return result;
}

void pup_vm_monitor_remove_device(PupVMMonitor *self, PupDevice *dev)
{
	pup_vm_monitor_lock(self);
	pup_vm_monitor_remove_device_unlocked(self, dev);
	pup_vm_monitor_unlock(self);
}

void pup_vm_monitor_change_device(PupVMMonitor *self, PupDevice *dev,
                                  const gchar *detail)
{
	pup_vm_monitor_lock(self);
	pup_vm_monitor_change_device_unlocked(self, dev, detail);
	pup_vm_monitor_unlock(self);
}

PupDrive *pup_vm_monitor_lookup_drive(PupVMMonitor *self, const gchar *sysname,
                                      gboolean dup)
{
	PupDrive *result;
	pup_vm_monitor_lock(self);
	result = pup_vm_monitor_lookup_drive_unlocked(self, sysname, dup);
	pup_vm_monitor_unlock(self);

	return result;
}


PupVolume *pup_vm_monitor_lookup_volume(PupVMMonitor *self, const gchar *sysname,
                                        gboolean dup)
{
	PupVolume *result;
	pup_vm_monitor_lock(self);
	result = pup_vm_monitor_lookup_volume_unlocked(self, sysname, dup);
	pup_vm_monitor_unlock(self);

	return result;
}

PupDevice *pup_vm_monitor_lookup(PupVMMonitor *self, guint catagory,
                                 const gchar *sysname, gboolean dup)
{
	if (catagory == PUP_CATAGORY_DRIVE)
		return PUP_DEVICE(pup_vm_monitor_lookup_drive(self, sysname, dup));
	else if (catagory == PUP_CATAGORY_VOLUME)
		return PUP_DEVICE(pup_vm_monitor_lookup_volume(self, sysname, dup));
	else
	{
		g_critical("%d is not a valid catagory id", catagory);
		return NULL;
	}
}

void pup_vm_monitor_copy_back_device(PupVMMonitor *self, PupDevice *dev,
                                     const gchar *detail)
{
	pup_vm_monitor_lock(self); 
	do
	{
		if (! PUP_IS_DEVICE(dev))
		{
			g_critical("Malformed device %p", dev);
			break;
		}
		GHashTable *table = pup_vm_monitor_get_hash_table(self, dev);
		if (! table)
		{
			g_critical("Device %p is neither drive or volume", dev);
			break;
		}

		PupDevice *old_dev = PUP_DEVICE(g_hash_table_lookup(table,
		                                                    dev->sysname));
		if (PUP_IS_DEVICE(old_dev))
		{
			if (old_dev != dev)
			{
				pup_device_swap_contents(old_dev, dev);
				g_object_unref(dev);

				old_dev->monitor = self;

				PUP_VM_MONITOR_GET_CLASS(self)->device_event_cb
					(self, old_dev, PUP_DEVICE_CHANGE, detail);
			}
			else
			{
				g_critical("Same object exists in the monitor.");
			}

		}
		else
		{
			g_critical("Device %p not found in monitor", dev);
			g_object_unref(dev);
		}
	} while (FALSE);
	pup_vm_monitor_unlock(self);
}

void pup_vm_monitor_raise_uevent(PupVMMonitor *self, struct udev_device *udev_dev)
{
	pup_vm_monitor_lock(self);
	self->udev_cancel = FALSE;

	g_signal_emit(self, PUP_VM_MONITOR_GET_CLASS(self)->udev_signal_id,
	              0, udev_dev, NULL);

	if (! self->udev_cancel)
	{
		PUP_VM_MONITOR_GET_CLASS(self)->udev_event(self, udev_dev);
	}

	g_signal_emit(self, PUP_VM_MONITOR_GET_CLASS(self)->udev_after_signal_id,
	              0, udev_dev, NULL);

	udev_device_unref(udev_dev);
	pup_vm_monitor_unlock(self);
}

void pup_vm_monitor_stop_processing_uevent(PupVMMonitor *self)
{
	self->udev_cancel = TRUE;
}

void pup_vm_monitor_get_mounts_unlocked(PupVMMonitor *self)
{
	struct mntent f_ent;
	char buf[1024];
	FILE *mtabfile;

	//First remove all existing entries
	g_hash_table_remove_all(self->mounts);

	//Now fill data
	mtabfile = setmntent(self->mtab_file, "r");
	while (getmntent_r(mtabfile, &f_ent, buf, 1024))
	{
		PupMntEntry entry;
		entry.devnode = g_strdup(f_ent.mnt_fsname);
		entry.mntpnt = g_strdup(f_ent.mnt_dir);
		entry.flags = 0;
		//Is the mountpoint a system volume?
		if (! entry.mntpnt)
			; //checking for null
		else if (strstr(entry.mntpnt, "/initrd/"))
		{
			entry.flags |= PUP_VOLUME_MNTD_SYSTEM;
		}
		else if (strcmp(entry.mntpnt, "/") == 0)
		{
			entry.flags |= PUP_VOLUME_MNTD_SYSTEM;
		}
		//Readonly?
		if (hasmntopt(&f_ent, MNTOPT_RO))
		{
			entry.flags |= PUP_VOLUME_MNTD_READ_ONLY;
		}
		g_hash_table_replace(self->mounts, entry.devnode,
		                     g_slice_dup(PupMntEntry, &entry));
	}
	endmntent(mtabfile);

	g_signal_emit(self, PUP_VM_MONITOR_GET_CLASS(self)->mounts_check_signal_id,
	              0);
	
}

void pup_vm_monitor_get_mounts(PupVMMonitor *self)
{
	pup_vm_monitor_lock(self);

	pup_vm_monitor_get_mounts_unlocked(self);
	
	pup_vm_monitor_unlock(self);
}

PupMntEntry *pup_vm_monitor_get_mount_for_devnode(PupVMMonitor *self,
                                                  const gchar *devnode,
                                                  gboolean dup)
{
	g_return_val_if_fail(devnode, NULL);

	PupMntEntry *res;

	pup_vm_monitor_lock(self);

	res = (PupMntEntry *) g_hash_table_lookup(self->mounts, devnode);
	if (res && dup)
	{
		res = g_slice_dup(PupMntEntry, res);
		res->devnode = g_strdup(res->devnode);
		res->mntpnt = g_strdup(res->mntpnt);
	}
	
	pup_vm_monitor_unlock(self);

	return res;
}

void pup_vm_monitor_destroy(PupVMMonitor *self)
{
	//Free all devices
	GList *list, *iter;
	
	list = g_hash_table_get_values(self->drives);
	for (iter = g_list_first(list); iter; iter = iter->next)
	{
		pup_vm_monitor_remove_device_unlocked(self, PUP_DEVICE(iter->data));
	}
	g_list_free(list);
	
	list = g_hash_table_get_values(self->volumes);
	for (iter = g_list_first(list); iter; iter = iter->next)
	{
		pup_vm_monitor_remove_device_unlocked(self, PUP_DEVICE(iter->data));
	}
	g_list_free(list);

	g_hash_table_destroy(self->drives);
	g_hash_table_destroy(self->volumes);

	g_object_unref(self);
}

