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

#endif //PUP_VM_H_INSIDE

//FILE_HEADER_END

#ifndef PUP_VM_H_INSIDE
G_DEFINE_TYPE(PupServerMonitor, pup_server_monitor, PUP_TYPE_VM_MONITOR);
#else
GType pup_server_monitor_get_type();
#endif

static void pup_server_monitor_class_init(PupServerMonitorClass *klass)
{
	PupVMMonitorClass *parent_class = PUP_VM_MONITOR_CLASS(klass);

	parent_class->udev_event = pup_server_monitor_udev_event;
	parent_class->device_event_cb = pup_server_monitor_device_event_cb;

	klass->broadcast_signal_id
		= g_signal_new("broadcast", PUP_TYPE_SERVER_MONITOR,
		               G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
		               g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1,
		               G_TYPE_POINTER);
}

static void pup_server_monitor_init(PupServerMonitor *self)
{
	self->udev_ctx = udev_new();

	pup_vm_monitor_get_mounts(PUP_VM_MONITOR(self));
	g_signal_connect(self, "mounts-check",
	                 G_CALLBACK(pup_server_monitor_update_mnt_info), NULL);

	//Search for drives and volumes
	blkid_cache cache;
	if (! (blkid_get_cache(&cache, "/etc/blkid-cache") < 0))
	{
		blkid_probe_all(cache);

		blkid_dev device;
		blkid_dev_iterate iter = blkid_dev_iterate_begin(cache);
		while(blkid_dev_next(iter, &device) == 0)
		{
			const gchar *devnode = blkid_dev_devname(device);
			if (! devnode) continue;
			//Search for corresponding drive
			struct udev_device *dev = udev_device_new_from_subsystem_sysname
					(self->udev_ctx, "block", pup_guess_sysname(devnode));
			struct udev_device *drv_dev = NULL;
			if (dev)
				drv_dev = pup_volume_search_for_drive(dev);
			
			//Probe for this volume, but filter out optical drives
			PupVolume *volume = NULL;
			if (drv_dev ? (! pup_drive_test_optical(drv_dev)) : TRUE)
				volume = pup_volume_new_from_blkid_cache
					(cache, devnode);
			
			if (volume)
			{
				if (drv_dev)
				{
					//check whether we should probe for this drive  
					const gchar *drv_sysname = (drv_dev
						                        ? udev_device_get_sysname(drv_dev)
						                        : NULL);
					volume->drv_sysname = g_strdup(drv_sysname);
				
					if ((drv_sysname 
						 ? ! pup_vm_monitor_lookup_drive_unlocked
						 (PUP_VM_MONITOR(self), drv_sysname, FALSE)
						 : FALSE))
					{
						PupDrive *drive = PUP_DRIVE(pup_device_new(PUP_TYPE_DRIVE,
							                                       udev_device_get_sysname
							                                       (drv_dev)));
						if (drive)
						{
							pup_drive_update(drive, drv_dev);

							pup_vm_monitor_add_device(PUP_VM_MONITOR(self),
									                  PUP_DEVICE(drive));
						}
					}
				}

				pup_volume_set_icon(volume, PUP_VM_MONITOR(self));

				pup_vm_monitor_add_device(PUP_VM_MONITOR(self),
					                      PUP_DEVICE(volume));
			}

			if (dev) udev_device_unref(dev);
		}

		blkid_dev_iterate_end(iter);
		blkid_put_cache(cache);
	}
	
	//Setup thread pool for probing
	GError *error = NULL;
	self->dev_probe_thread_pool = g_thread_pool_new((GFunc) pup_server_monitor_probe_thread_func,
	                                                self, -1, FALSE, &error);
	if (error)
	{
		g_critical("Couldn't create a new thread pool: %s", error->message);
	}
}

void pup_server_monitor_dump_contents(PupServerMonitor *self)
{
	//Print these out for debugging
	printf("Drives: \n");
	g_hash_table_foreach(PUP_VM_MONITOR(self)->drives, pup_device_show_hfunc,
	                     NULL);
	printf("Volumes: \n");
	g_hash_table_foreach(PUP_VM_MONITOR(self)->volumes, pup_device_show_hfunc,
	                     NULL);
}

const gchar *pup_event_strv[] = {"none", "add", "remove", "change"};

const gchar *pup_volume_events[] =
{
	"none",
	PUP_VOLUME_EVENT_MOUNT,
	PUP_VOLUME_EVENT_UMOUNT,
	PUP_VOLUME_EVENT_REMOUNT
};

void pup_server_monitor_device_event_cb(PupVMMonitor *monitor, PupDevice *dev,
                                        guint event, const gchar *detail)
{
	//Don't raise events while initializing
	if (! PUP_SERVER_MONITOR(monitor)->initialized) return;
	//Filter out consecutive mount events, it is often result of updating
	// /proc/mounts out of phase
	static gchar *prev_detail = NULL;
	static PupDevice *prev_dev = NULL;
	if (prev_detail)
	{
		if (prev_dev == dev)
		{
			if (g_strcmp0(prev_detail, detail) == 0)
			{
				if ((g_strcmp0(prev_detail, PUP_VOLUME_EVENT_MOUNT) == 0)
					|| (g_strcmp0(prev_detail, PUP_VOLUME_EVENT_UMOUNT) == 0)
					|| (g_strcmp0(prev_detail, PUP_VOLUME_EVENT_REMOUNT) == 0))
				{
					//Swallow event
					return;
				}
			}
		}
		g_free(prev_detail);
	}
	prev_detail = g_strdup(detail);
	prev_dev = dev;
	//Show event for debugging
	
	printf("Event: %s; detail=%s\n", pup_event_strv[event], detail);
	
	pup_device_show(dev);
	
	PSDataEncoder *encoder = ps_data_encoder_new();
	pup_device_encode_change(dev, event, detail, encoder);
	
	g_signal_emit(monitor, 
	              PUP_VM_MONITOR_GET_CLASS(monitor)->device_event_signal_id,
	              0, dev, event, detail, NULL);

	pup_queue_call_func(monitor,
	                    (PupFunc) pup_server_monitor_device_event_broadcast,
	                    encoder);
}

void pup_server_monitor_device_event_broadcast(PupVMMonitor *monitor,
                                               PSDataEncoder *encoder)
{
	//Broadcast events to all intrested clients
	g_signal_emit(monitor, 
	              PUP_SERVER_MONITOR_GET_CLASS(monitor)->broadcast_signal_id,
	              0, encoder, NULL);

	ps_data_encoder_destroy(encoder);
}

gpointer pup_server_monitor_udev_thread(gpointer data)
{
	PupServerMonitor *self = PUP_SERVER_MONITOR(data);
	GPollFD pollfd;

	//Connect to udev
	//struct udev_monitor *udev_event_monitor = udev_monitor_new_from_socket
	//	(self->udev_ctx, pup_get_udev_sock_path());
	struct udev_monitor *udev_event_monitor = udev_monitor_new_from_netlink
		(self->udev_ctx, "udev");

	g_return_val_if_fail(udev_event_monitor, NULL);

	udev_monitor_enable_receiving(udev_event_monitor);
	
	//Prepare for poll()
	pollfd.fd = udev_monitor_get_fd(udev_event_monitor);
	pollfd.events = G_IO_IN | G_IO_HUP | G_IO_ERR;

	struct udev_device *device = NULL;
	while (1)
	{
		//iguleder says udev_monitor_receive_device() doesn't block, 
		//but it blocks for me :?
		//So adding g_poll() anyways as a safety measure
		g_poll(&pollfd, 1, -1);
		
		if (! (device = udev_monitor_receive_device(udev_event_monitor)))
			break;
		
		pup_queue_call_func(self, (PupFunc) pup_vm_monitor_raise_uevent,
		                    device);
	}
	g_warning("udev event monitor exited while daemon is running");

	return NULL;
}

void pup_server_monitor_start_udev_thread(PupServerMonitor *self)
{
	GError *error = NULL;
	self->udev_thread = g_thread_create(pup_server_monitor_udev_thread,
	                                    self, FALSE, &error);

	if (error)
	{
		g_critical("Couldn't start udev monitor thread: %s",
		           error->message);
	}
}

//Device probing routing from here onwards
void pup_server_monitor_udev_event(PupVMMonitor *monitor,
                                   struct udev_device *dev)
{
	GError *error = NULL;

	if (g_strcmp0(udev_device_get_subsystem(dev), "block") == 0)
	{
		g_thread_pool_push(PUP_SERVER_MONITOR(monitor)->dev_probe_thread_pool,
		                   dev, &error);
		if (error)
		{
			g_critical("Can't start new thread: %s", error->message);
		}

		udev_device_ref(dev);
	}
}

void pup_drive_process_event(PupVMMonitor *monitor, struct udev_device *dev,
                             gboolean process_change)
{
	PupDrive *drive = pup_vm_monitor_lookup_drive
		(monitor, udev_device_get_sysname(dev), TRUE);

	//Filter out plugin-handled devices
	if (G_TYPE_FROM_INSTANCE(drive) != PUP_TYPE_DRIVE)
	{
		//For now do nothing
	}
	if (g_strcmp0(udev_device_get_action(dev), "remove") == 0)
	{
		if (drive)
		{
			pup_vm_monitor_remove_device(monitor, PUP_DEVICE(drive));
			g_object_unref(drive);
		}
	}
	else
	{
		if (drive)
		{
			if (process_change)
			{
				pup_device_clear_data(PUP_DEVICE(drive));
				pup_drive_update(drive, dev);
				pup_vm_monitor_copy_back_device(monitor, PUP_DEVICE(drive),
				                                NULL);
			}
		}
		else
		{
			drive = PUP_DRIVE(pup_device_new
			                  (PUP_TYPE_DRIVE, 
			                   udev_device_get_sysname(dev)));
			pup_drive_update(drive, dev);
			pup_vm_monitor_add_device(monitor, PUP_DEVICE(drive));
		}
	}
}

void pup_server_monitor_probe_thread_func(struct udev_device *dev,
                                          PupVMMonitor *monitor)
{
	/*
	 printf("udev event raised\n");

	 g_print("Device: action=\"%s\", subsystem=\"%s\", devtype=\"%s\", sysname=\"%s\", devnode=%s\n\n",
	         udev_device_get_action(udev_dev),
			 udev_device_get_subsystem(udev_dev),
			 udev_device_get_devtype(udev_dev),
			 udev_device_get_sysname(udev_dev),
			 udev_device_get_devnode(udev_dev));
			 */

	//PupServerMonitor *self = PUP_SERVER_MONITOR(monitor);

	g_return_if_fail(udev_device_get_sysname(dev));

	//Filter out optical drives
	if (pup_drive_test_optical(dev)) return;
	//Filter out loopback drives
	
	
	//Check for drive 
	if (pup_drive_assert(dev))
	{
		pup_drive_process_event(monitor, dev, TRUE);
	}

	//Check for volume
	PupVolume *volume = pup_vm_monitor_lookup_volume
		(monitor, udev_device_get_sysname(dev), TRUE);
	//Don't process if volume belongs to a plugin
	if (G_TYPE_FROM_INSTANCE(volume) != PUP_TYPE_VOLUME)
	{
		//For now do nothing
	}
	if (g_strcmp0(udev_device_get_action(dev), "remove") == 0)
	{
		if (volume)
		{
			pup_vm_monitor_remove_device(monitor, PUP_DEVICE(volume));
			g_object_unref(volume);
		}
	}
	else
	{
		//Search for relevent drive if any
		struct udev_device *drv_dev = pup_volume_search_for_drive(dev);
		
		
		blkid_probe probe;
		if (pup_volume_assert(dev, &probe))
		{
			gpointer old_volume = (gpointer) volume;
			PupMntEntry *mnt_entry;
			if (volume)
			{
				pup_device_clear_data(PUP_DEVICE(volume));

			}
			else
			{
				volume = PUP_VOLUME(pup_device_new
				                    (PUP_TYPE_VOLUME, 
				                     udev_device_get_sysname(dev)));
				//Probe for relevent drive if any
				if (drv_dev)
				{
					pup_drive_process_event(monitor, drv_dev, FALSE);				
				}
			}
			pup_volume_update_from_blkid_probe(volume, dev, probe);
			blkid_free_probe(probe);

			if (drv_dev)
			{
				volume->drv_sysname = g_strdup(udev_device_get_sysname(drv_dev));
				
			}
			//Icon
			pup_volume_set_icon(volume, monitor);
			
			mnt_entry = pup_vm_monitor_get_mount_for_devnode
				(monitor, volume->unix_dev, TRUE);
			pup_volume_update_mount(volume, mnt_entry);
			if (mnt_entry) pup_mnt_entry_free(mnt_entry);

			if (old_volume)
			{
				pup_vm_monitor_copy_back_device(monitor, PUP_DEVICE(volume),
				                                NULL);
			}
			else
			{
				pup_vm_monitor_add_device(monitor, PUP_DEVICE(volume));
			}
		}
	}


	udev_device_unref(dev);
}

gboolean pup_server_monitor_mounts_check_func(gpointer data)
{
	pup_vm_monitor_get_mounts(PUP_VM_MONITOR(data));
	return TRUE;
}

void pup_server_monitor_update_mnt_info(PupVMMonitor *monitor, gpointer dummy)
{
	GHashTableIter iter;
	g_hash_table_iter_init(&iter, monitor->volumes);
	PupVolume *volume;
	while (g_hash_table_iter_next(&iter, NULL, (gpointer *) &volume))
	{
		//Filter out plugins' volumes handling mount data themselves
		if (volume->flags & PUP_VOLUME_OVERRIDE_MNT_INFO)
			continue;
		
		PupMntEntry *entry = pup_vm_monitor_get_mount_for_devnode
			(monitor, volume->unix_dev, FALSE);
		guint event = pup_volume_update_mount(volume, entry);
		if (event) pup_vm_monitor_change_device_unlocked
			(monitor, PUP_DEVICE(volume), pup_volume_events[event]);
	}
}

//Operation management
void pup_server_monitor_start_operation(PupServerMonitor *self, guint catagory,
                                   const gchar *sysname, PupOperation *operation)
{
	pup_vm_monitor_lock(PUP_VM_MONITOR(self));	

	PupDevice *dev = pup_vm_monitor_lookup(PUP_VM_MONITOR(self), catagory,
	                                       sysname, FALSE);
	if (! dev)
	{
		g_warning("Sysname %s not found in catagory %d", sysname, catagory);
		pup_operation_return(operation, FALSE, G_IO_ERROR_INVALID_ARGUMENT,
		                     "Invalid argument (requested device not found)"
		                     ": sysname %s not found in catagory %d in server",
		                     sysname, catagory);
		pup_vm_monitor_unlock(PUP_VM_MONITOR(self));
		return;
	}
	
	pup_operation_start(operation, dev);
	pup_vm_monitor_unlock(PUP_VM_MONITOR(self));
}



