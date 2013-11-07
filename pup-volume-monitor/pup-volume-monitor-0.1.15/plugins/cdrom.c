//libcdrom: plugin to handle optical drives

//Temporary edit: jemimah reported something wrong with USB optical drive,
//possibly problematic part is made bit more verbose

//FILE_HEADER_SUBST:gobject_macro_gen PUP_CD_DRIVE PupCDDrive pup_cd_drive pup
//FILE_HEADER_SUBST:gobject_macro_gen PUP_CD PupCD pup_cd pup

//FILE_HEADER_END

#include <libpupvm/common-includes.h>
//#include <libburn.h>
#include <blkid.h>
#include <linux/cdrom.h>

#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "cdrom"

typedef struct _PupCD PupCD;

//PupCDDrive: Optical drive
typedef struct
{
	PupDrive parent;

	guint status;
} PupCDDrive;

typedef struct
{
	PupDriveClass parent;

	PupOperationFunc eject;
} PupCDDriveClass;

typedef enum 
{
	PUP_CD_DRIVE_TRAY_OPEN = 0,
	PUP_CD_DRIVE_NOT_READY = 1,
	PUP_CD_DRIVE_HAS_DISK = 2,
	PUP_CD_DRIVE_NO_DISK = 3
} PupCDDriveStatus;

const gchar *pup_cd_drive_status[] =
{
	"tray-open",
	"not-ready",
	"has-disk",
	"no-disk"
};

const gchar *pup_cd_drive_events[] =
{
	"tray-opened",
	"tray-closed",
	"disk-inserted",
	NULL
};

//Icons

//PupCD: Disk inside the drive
struct _PupCD
{
	PupVolume parent;
	
	//gchar *mediatype;
	gboolean audio_cd;
};

typedef struct
{
	PupVolumeClass parent;

	PupOperationFunc eject;
} PupCDClass;


#include "cdrom.h"

#define PUP_VM_PLUGIN_SOURCE

PupDevice *pup_overwrite_dev(PupVMMonitor *monitor, PupDevice *dev)
{
	PupDevice *orig_dev = pup_vm_monitor_lookup(monitor, dev->catagory,
		                                        dev->sysname, FALSE);
	if (orig_dev)
	{
		pup_vm_monitor_copy_back_device(monitor, dev, NULL);
		return orig_dev;
	}
	else
	{
		pup_vm_monitor_add_device(monitor, dev);
		return dev;
	}
}

GThreadPool *pup_cdrom_probe_thread_pool = NULL;

#ifdef PUP_VM_PLUGIN_SOURCE
G_DEFINE_DYNAMIC_TYPE(PupCDDrive, pup_cd_drive, PUP_TYPE_DRIVE);
#endif


#ifdef PUP_VM_PLUGIN_SOURCE
G_DEFINE_DYNAMIC_TYPE(PupCD, pup_cd, PUP_TYPE_VOLUME);
#endif


//PupCDDrive: Optical drive

static void pup_cd_drive_class_init(PupCDDriveClass *klass)
{
	PupDeviceClass *dev_class = PUP_DEVICE_CLASS(klass);

	dev_class->display = pup_cd_drive_display;
	dev_class->dup = pup_cd_drive_dup;
	//dev_class->free_contents = pup_cd_drive_free_contents;
	dev_class->size = sizeof(PupCDDrive);

	pup_device_class_register_operation(PUP_TYPE_CD_DRIVE, "eject",
	                                    G_STRUCT_OFFSET(PupCDDriveClass, eject));

	klass->eject = pup_cd_drive_eject;
}

static void pup_cd_drive_class_finalize(PupCDDriveClass *klass)
{

}

static void pup_cd_drive_init(PupCDDrive *self)
{
	PUP_DRIVE(self)->flags |= PUP_DRIVE_CAN_EJECT;
	PUP_DEVICE(self)->icon_name = g_strdup(PUP_ICON_OPTICAL_DRIVE);
}

void pup_cd_drive_dup(PupDevice *src, PupDevice *dest)
{
	//Chain up first
	PUP_DEVICE_CLASS(pup_cd_drive_parent_class)->dup(src, dest);
	
	PUP_CD_DRIVE(dest)->status = PUP_CD_DRIVE(src)->status;
}
/*
void pup_cd_drive_free_mem(PupDevice *dev)
{
	PUP_DEVICE_CLASS(pup_cd_drive_parent_class)->free_mem(dev);
}
*/
void pup_cd_drive_display(PupDevice *dev)
{
	//Chain up first
	PUP_DEVICE_CLASS(pup_cd_drive_parent_class)->display(dev);
	
	printf("\tstatus=%s\n", pup_cd_drive_status[PUP_CD_DRIVE(dev)->status]);
}

//Check for disk
guint pup_cd_drive_get_status(PupCDDrive *drv)
{
	//Open device
	gint fd = open(PUP_DRIVE(drv)->unix_dev, O_NONBLOCK | O_RDWR);
	g_return_val_if_fail(fd >= 0, PUP_CD_DRIVE_NO_DISK);
	gint status = ioctl(fd, CDROM_DRIVE_STATUS, 0);

	switch (status)
	{
		case CDS_NO_DISC:
			drv->status = PUP_CD_DRIVE_NO_DISK;
			break;
		case CDS_TRAY_OPEN:
			drv->status = PUP_CD_DRIVE_TRAY_OPEN;
			break;
		case CDS_DRIVE_NOT_READY:
			drv->status = PUP_CD_DRIVE_NOT_READY;
			break;
		case CDS_DISC_OK:
			drv->status = PUP_CD_DRIVE_HAS_DISK;
			break;
		default:
			g_critical("pup_cd_drive_get_status: ioctl() failed on %s: %s",
			           PUP_DRIVE(drv)->unix_dev, g_strerror(errno));
			drv->status = PUP_CD_DRIVE_NO_DISK;
	}

	close(fd);

	return drv->status;
}

PupDrive *pup_cd_drive_new_from_udev_device(PupVMMonitor *monitor,
                                            struct udev_device *device)
{
	PupDrive *drv = PUP_DRIVE(pup_device_new(PUP_TYPE_CD_DRIVE,
	                                         udev_device_get_sysname(device)));
	                                                
	PUP_DEVICE(drv)->icon_name = g_strdup(PUP_ICON_OPTICAL_DRIVE);
	drv->unix_dev = pup_guess_devnode(device);
	if (! drv->unix_dev)
	{
		g_critical("Device node not found for %s", udev_device_get_sysname(device));
		g_object_unref(drv);
		return NULL;
	}
	
	//Find model and vendor strings
	gchar *tmpstr = g_strdup(udev_device_get_sysattr_value(device, "device/model"));
	PUP_DRIVE(drv)->model = g_strdup(g_strstrip(tmpstr));
	g_free(tmpstr);
	tmpstr = g_strdup(udev_device_get_sysattr_value(device, "device/vendor"));
	PUP_DRIVE(drv)->vendor = g_strdup(g_strstrip(tmpstr));
	g_free(tmpstr);                            

	PUP_DEVICE(drv)->display_name = pup_drive_gen_display_name(PUP_DRIVE(drv));

	//We will check for the disk later...
	pup_vm_monitor_lock(monitor);
	PupDrive *old_drv = pup_vm_monitor_lookup_drive_unlocked
		(monitor, udev_device_get_sysname(device), FALSE);
	if (old_drv)
		PUP_CD_DRIVE(drv)->status = PUP_CD_DRIVE(old_drv)->status;
	else
		PUP_CD_DRIVE(drv)->status = PUP_CD_DRIVE_NOT_READY;
	pup_vm_monitor_unlock(monitor);
	
	g_signal_connect_object(monitor, "mounts-check",
	                        G_CALLBACK(pup_cd_drive_probe_disk_cb), drv, 0);

	pup_overwrite_dev(monitor, PUP_DEVICE(drv));
	g_debug("%s added", udev_device_get_sysname(device));

	return drv;
}

//Search for all optical drives
void pup_cd_drive_get_all(PupVMMonitor *monitor)
{
	GDir *block_dir;
	GError *error = NULL;
	g_debug("Init time scanning");
	block_dir = g_dir_open("/sys/block", 0, &error);
	if (error)
	{
		g_critical("Couldn't open /sys/block: %s", error->message);
		g_clear_error(&error);
	}
	
	struct udev *udev_ctx = udev_new();
	gchar *one_sysname;
	while ((one_sysname = (gchar *) g_dir_read_name(block_dir)))
	{
		struct udev_device *dev = udev_device_new_from_subsystem_sysname
			(udev_ctx, "block", one_sysname);
		if (dev)
		{
			g_debug("Testing %s", one_sysname);
			if (pup_drive_test_optical(dev))
			{
				g_debug("Success, now adding to monitor");
				pup_cd_drive_new_from_udev_device(monitor, dev);
			}

			udev_device_unref(dev);
		}
	}
	udev_unref(udev_ctx);
	g_dir_close(block_dir);
}

void pup_cd_drive_probe_disk_cb(PupVMMonitor *monitor, PupCDDrive *drv)
{
	guint old_status = drv->status;
	pup_cd_drive_get_status(drv);
	if (drv->status != old_status)
	{
		if (drv->status == PUP_CD_DRIVE_HAS_DISK)
		{
			g_thread_pool_push(pup_cdrom_probe_thread_pool, drv, NULL);
			
		}
		else
		{
			PupVolume *vol = pup_vm_monitor_lookup_volume
				(monitor, PUP_DEVICE(drv)->sysname, FALSE);
			if (vol) pup_vm_monitor_remove_device(monitor, PUP_DEVICE(vol));
		}
		pup_vm_monitor_change_device(monitor, PUP_DEVICE(drv),
		                             pup_cd_drive_events[drv->status]);
	}
}

void pup_cd_drive_eject(PupDevice *dev, PupOperation *operation)
{
	//Search for disk
	PupVMMonitor *monitor = pup_vm_monitor_get();
	PupCD *disk = PUP_CD(pup_vm_monitor_lookup_volume_unlocked
	                     (monitor, dev->sysname, FALSE));
	if (! disk)
	{
		pup_operation_return(operation, FALSE, G_IO_ERROR_FAILED,
		                     "No disk to eject");
	}
	else
	{
		pup_operation_foreward(operation, PUP_DEVICE(disk), NULL, NULL);
	}
}

//PupCD: the disk

static void pup_cd_class_init(PupCDClass *klass)
{
	PupDeviceClass *dev_class = PUP_DEVICE_CLASS(klass);

	dev_class->display = pup_cd_display;
	dev_class->dup = pup_cd_dup;
	//dev_class->free_contents = pup_cd_free_contents;
	dev_class->size = sizeof(PupCD);

	PUP_VOLUME_CLASS(klass)->mount = pup_cd_mount;

	pup_device_class_register_operation(PUP_TYPE_CD, "eject",
	                                    G_STRUCT_OFFSET(PupCDClass, eject));

	klass->eject = pup_cd_eject;
}

static void pup_cd_class_finalize(PupCDClass *klass)
{

}

static void pup_cd_init(PupCD *self)
{
	PUP_VOLUME(self)->flags |= PUP_VOLUME_CAN_EJECT;
}

void pup_cd_dup(PupDevice *src, PupDevice *dest)
{
	//Chain up first
	PUP_DEVICE_CLASS(pup_cd_parent_class)->dup(src, dest);
	
	PupCD *src_disk = PUP_CD(src);
	PupCD *dest_disk = PUP_CD(dest);

	//dest_disk->mediatype = g_strdup(src_disk->mediatype);
	dest_disk->audio_cd = src_disk->audio_cd;
}
/*
void pup_cd_free_contents(PupDevice *dev)
{
	PupCD *disk = PUP_CD(dev);

	g_free(disk->mediatype);
	disk->mediatype = NULL;

	//Chain up
	PUP_DEVICE_CLASS(pup_cd_parent_class)->free_contents(dev);
}
*/
void pup_cd_display(PupDevice *dev)
{
	//Chain up first
	PUP_DEVICE_CLASS(pup_cd_parent_class)->display(dev);
	
	PupCD *disk = PUP_CD(dev);
	printf("\taudio_cd=");
	if (disk->audio_cd) puts("TRUE");
	else puts("FALSE");
}

void pup_cd_eject(PupDevice *dev, PupOperation *operation)
{
	//Unmount the volume if needed
	if (PUP_VOLUME(dev)->mntpnt)
		pup_operation_start_suboperation(operation, PUP_DEVICE(dev),
		                                 "umount", NULL,
		                                 (PupOperationTrapFunc) pup_cd_eject_continue,
		                                 NULL);
	else
		pup_cd_eject_continue(dev, operation);
	
}

void pup_cd_eject_continue(PupDevice *dev, PupOperation *operation)
{
	gint fd = open(PUP_VOLUME(dev)->unix_dev, O_NONBLOCK | O_RDWR);
	if (fd < 0)
	{
		pup_operation_return(operation, FALSE, g_io_error_from_errno(errno),
		                     "Couldn't open %s: %s", PUP_VOLUME(dev)->unix_dev,
		                     g_strerror(errno));
		return;
	}
	if (ioctl(fd, CDROMEJECT, 0) < 0)
	{
		pup_operation_return(operation, FALSE, g_io_error_from_errno(errno),
		                     "Couldn't eject %s: %s", PUP_DEVICE(dev)->sysname,  
		                     g_strerror(errno));
		close(fd);
		return;
	}
	PupVMMonitor *monitor = pup_vm_monitor_get();
	PupCDDrive *drive = PUP_CD_DRIVE(pup_vm_monitor_lookup_drive
	                                 (monitor, dev->sysname, FALSE));
	g_return_if_fail(drive);
	pup_cd_drive_probe_disk_cb(monitor, drive);
	close(fd);

	pup_operation_return(operation, TRUE, 0, " ");
}

void pup_cd_mount(PupDevice *dev, PupOperation *operation)
{
	//Just present to filter out audio CDs
	if (PUP_CD(dev)->audio_cd)
	{
		pup_operation_return(operation, FALSE, G_IO_ERROR_NOT_MOUNTABLE_FILE,
		                     "Volume %s cannot be mounted because it is an "
							 "audio CD.\nIt doesn't have a filesystem.", dev->sysname);
		return;
	}

	PUP_VOLUME_CLASS(pup_cd_parent_class)->mount(dev, operation);
}

PupCD *pup_cd_new_from_drive(PupCDDrive *drv, PupVMMonitor *monitor)
{
	PupCD *disk;

	pup_vm_monitor_lock(monitor);
	disk = PUP_CD(pup_device_new(PUP_TYPE_CD, PUP_DEVICE(drv)->sysname));
	//Get volume-specific paramaters like label, uuid, ...
	PupVolume *volume = PUP_VOLUME(disk);
	volume->unix_dev = g_strdup(PUP_DRIVE(drv)->unix_dev);
	volume->drv_sysname = g_strdup(PUP_DEVICE(drv)->sysname);
	pup_vm_monitor_unlock(monitor);

	blkid_probe probe = blkid_new_probe_from_filename(volume->unix_dev);
	if (probe)
	{
		blkid_do_safeprobe(probe);
		if (blkid_probe_lookup_value(probe, "TYPE",
			                         (const char **) &(volume->fstype), NULL) == 0)
			volume->fstype = g_strdup(volume->fstype);
		else
			volume->fstype = NULL;
		if (blkid_probe_lookup_value(probe, "LABEL",
			                         (const char **) &(volume->label), NULL) == 0)
			volume->label = g_strdup(volume->label);
		else
			volume->label = NULL;
		if (blkid_probe_lookup_value(probe, "UUID",
			                         (const char **) &(volume->uuid), NULL) == 0)
			volume->uuid = g_strdup(volume->uuid);
		else
			volume->uuid = NULL;

		blkid_free_probe(probe);
	}
	/*
	if (! volume->fstype)
	{
		g_object_unref(disk);
		return NULL;
	}
	*/
	//Check for audio CD
	gint fd = open(volume->unix_dev, O_NONBLOCK | O_RDWR);
	if (fd >= 0)
	{
		if (ioctl(fd, CDROM_DISC_STATUS, 0) == CDS_AUDIO)
		{
			disk->audio_cd = TRUE;
			volume->flags &= (~PUP_VOLUME_IS_MOUNTABLE);
		}
		close (fd);
	}
	//No need to check for mounts, daemon will do it for us
	
	//Display name and icon
	PUP_DEVICE(disk)->display_name = pup_volume_gen_display_name(volume);
	if (disk->audio_cd)
		PUP_DEVICE(disk)->icon_name = g_strdup(PUP_ICON_AUDIOCD);
	else
		PUP_DEVICE(disk)->icon_name = g_strdup(PUP_ICON_CDROM);

	return PUP_CD(pup_overwrite_dev(monitor, PUP_DEVICE(disk)));
}

//Here we can only handle udev events related to plugging and removing entire 
//drives
void pup_cdrom_udev_event(PupVMMonitor *monitor, struct udev_device *dev)
{
	//An optical drive is a block device
	if (g_strcmp0(udev_device_get_subsystem(dev), "block") != 0)
		return;
	g_debug("processing event for %s...\n", udev_device_get_sysname(dev));
	if (pup_drive_test_optical(dev))
	{
		//We are handling it...
		pup_vm_monitor_stop_processing_uevent(monitor);

		const gchar *action = udev_device_get_action(dev);
		g_return_if_fail(action);
		if (strcmp(action, "remove") == 0)
		{
			g_debug("Removing %s", udev_device_get_sysname(dev));
			PupDrive *drv = pup_vm_monitor_lookup_drive
				(monitor, udev_device_get_sysname(dev), FALSE);
			if (drv) pup_vm_monitor_remove_device(monitor, PUP_DEVICE(drv));
			PupVolume *vol = pup_vm_monitor_lookup_volume
				(monitor, udev_device_get_sysname(dev), FALSE);
			if (vol) pup_vm_monitor_remove_device(monitor, PUP_DEVICE(vol));
			
		}
		else
		{
			g_debug("Optical drive, now probing...\n");
			pup_cd_drive_new_from_udev_device(monitor, dev);
		}
	}
	else
		g_debug("Not an optical drive\n");
}

//Main
G_MODULE_EXPORT void pup_vm_plugin_init(PupVMPlugin *plugin)
{
	g_debug("loaded");
	//Register our classes
	pup_cd_drive_register_type(G_TYPE_MODULE(plugin));
	pup_cd_register_type(G_TYPE_MODULE(plugin));

	PupVMMonitor *monitor = pup_vm_monitor_get();
	
	//Initialize the thread pool
	pup_cdrom_probe_thread_pool = g_thread_pool_new
		((GFunc) pup_cd_new_from_drive, monitor, 1, FALSE, NULL);
	pup_cd_drive_get_all(monitor);
	g_signal_connect(monitor, "udev", G_CALLBACK(pup_cdrom_udev_event), NULL);
}

