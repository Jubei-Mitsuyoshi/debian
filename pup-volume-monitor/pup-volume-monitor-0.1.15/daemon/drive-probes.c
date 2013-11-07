//drive-probes.c or drive-probes.h
//Probing for drives and volumes

#ifndef PUP_VM_H_INSIDE
//drive-probes.c
#include "common.h"
#else
//drive-probes.h

#endif //PUP_VM_H_INSIDE

//FILE_HEADER_END

//Check whether udev device is a drive
gboolean pup_drive_assert(struct udev_device *dev)
{
	const gchar *devtype = udev_device_get_devtype(dev);

	if (devtype)
	{
		if (strcmp(devtype, "disk") == 0)
		{
			//Filter out loop and swap devices
			const gchar *sysname = udev_device_get_sysname(dev);
			if (sysname)
			{
				if (strstr(sysname, "loop")
					|| strstr(sysname, "swap"))
					return FALSE;
				else
					return TRUE;
			}
		}
	}

	return FALSE;
}


void pup_drive_update(PupDrive *drive, struct udev_device *dev)
{
	//Find model and vendor strings
	gchar *tmpstr = g_strdup(udev_device_get_sysattr_value(dev, "device/model"));
	drive->model = g_strdup(g_strstrip(tmpstr));
	g_free(tmpstr);
	tmpstr = g_strdup(udev_device_get_sysattr_value(dev, "device/vendor"));
	drive->vendor = g_strdup(g_strstrip(tmpstr));
	g_free(tmpstr);                            
	
	//Device node
	drive->unix_dev = pup_guess_devnode(dev);

	//Display name 
	PUP_DEVICE(drive)->display_name = pup_drive_gen_display_name(drive);
	
	drive->flags = 0;
	const gchar *devpath = udev_device_get_devpath(dev);
	if (devpath)
	{
		//test for USB drive
		if (strstr(devpath, "usb"))
			drive->flags |= PUP_DRIVE_USB;
		//test for removable drive 
		const gchar *removable = udev_device_get_sysattr_value(dev, "removable");
		if (removable)
			if (removable[0] != '0')
				drive->flags |= PUP_DRIVE_REMOVABLE;
		//test for flash drive
		gchar *test_file = g_strconcat("/sys",
			                           udev_device_get_devpath(dev),
			                           "/device/unload_heads",
		                               NULL);
		if (! g_file_test(test_file, G_FILE_TEST_EXISTS))
			drive->flags |= PUP_DRIVE_FLASH;
		g_free(test_file);
	}
	//icon
	if (drive->flags & PUP_DRIVE_FLASH)
	{
		if (drive->flags & PUP_DRIVE_USB)
		{
			PUP_DEVICE(drive)->icon_name = g_strdup(PUP_ICON_USBFLASH);
		}
		else
		{
			PUP_DEVICE(drive)->icon_name = g_strdup(PUP_ICON_FLASH);
		}
	}
	else
	{
		if (drive->flags & PUP_DRIVE_REMOVABLE)
		{
			PUP_DEVICE(drive)->icon_name = g_strdup(PUP_ICON_USBHDD);
		}
		else
		{
			PUP_DEVICE(drive)->icon_name = g_strdup(PUP_ICON_HDD);
		}
	}
}

//Volume probing
guint pup_volume_update_mount(PupVolume *volume, PupMntEntry *entry)
{
	gint event = 0;
	if (volume->mntpnt)
	{
		if (entry)
		{
			if ((strcmp(volume->mntpnt, entry->mntpnt) != 0)
			    || ((volume->flags & PUP_VOLUME_MNT_FLAGS) != entry->flags))
			{
				event = PUP_VOLUME_REMOUNT;
			}
		}
		else
		{
			event = PUP_VOLUME_UMOUNT;
		}
	}
	else
	{
		if (entry)
		{
			event = PUP_VOLUME_MOUNT;
		}
	}

	volume->flags &= ~PUP_VOLUME_MNT_FLAGS;
	g_free(volume->mntpnt);
	volume->mntpnt = NULL;
	if (entry)
	{
		volume->flags |= entry->flags;
		volume->mntpnt = g_strdup(entry->mntpnt);
	}

	return event;
}

PupVolume *pup_volume_new_from_blkid_cache(blkid_cache cache,
                                           const gchar *devnode)
{
	//filter out loop and swap devices
	if (g_strrstr(devnode, "loop")) return NULL;
	gchar *fstype = blkid_get_tag_value(cache, "TYPE", devnode);
	if (! fstype) return NULL;
	if (g_strrstr(fstype, "swap")) return NULL;
	
	PupVolume *volume = PUP_VOLUME(pup_device_new(PUP_TYPE_VOLUME,
	                                              pup_guess_sysname(devnode)));

	volume->unix_dev = g_strdup(devnode);
	volume->fstype = g_strdup(fstype);
	free(fstype);
	gchar *tmpstr = blkid_get_tag_value(cache, "UUID", devnode);
	volume->uuid = g_strdup(tmpstr);
	if (tmpstr) free(tmpstr);
	tmpstr = blkid_get_tag_value(cache, "LABEL", devnode);
	volume->label = g_strdup(tmpstr);
	if (tmpstr) free(tmpstr);

	PupDevice *device = PUP_DEVICE(volume);
	device->display_name = pup_volume_gen_display_name(volume);

	return volume;
}

//Check whether udev device is a volume
gboolean pup_volume_assert(struct udev_device *dev, blkid_probe *probe_return)
{
	gchar *devnode = pup_guess_devnode(dev);
	if (! devnode) return FALSE;

	//Filter out loop devices, they are just an added confusion
	if (strstr(devnode, "loop"))
	{
		g_free(devnode);
		return FALSE;
	}

	blkid_probe probe = blkid_new_probe_from_filename(devnode);
	g_free(devnode);
	if (! probe)
	{
		return FALSE;
	}

	//If reached here, means a block device, checking whether it has a filesystem
	gchar *type = NULL;
	blkid_do_safeprobe(probe);
	if ((blkid_probe_lookup_value(probe, "TYPE", (const gchar **) &type, NULL) == 0) ? type : FALSE)
	{
		//Filter out swap partitions
		int res = strcmp(type, "swap");
		//free(type);
		if (res)
		{
			//mountable volume
			if (probe_return) *probe_return = probe;
			return TRUE;
		}
		else
		{
			blkid_free_probe(probe);
			return FALSE;
		}
	}
	else
	{
		blkid_free_probe(probe);
		return FALSE;
	}
}

void pup_volume_update_from_blkid_probe(PupVolume *volume,
                                        struct udev_device *dev,
                                        blkid_probe probe)
{
	volume->unix_dev = pup_guess_devnode(dev);
	
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

	PupDevice *device = PUP_DEVICE(volume);
	device->display_name = pup_volume_gen_display_name(volume);
}

struct udev_device *pup_volume_search_for_drive(struct udev_device *dev)
{
	struct udev_device *drv_dev = dev;
	if (! pup_drive_assert(dev))
		drv_dev = udev_device_get_parent(dev);
	if (! pup_drive_assert(drv_dev))
	{
		//if (volume) volume->drv_sysname = NULL;
		return NULL;
	}

	//Now drv_dev is our drive.
	//if (volume) volume->drv_sysname = g_strdup(udev_device_get_sysname(drv_dev));
	return drv_dev;
}

void pup_volume_set_icon(PupVolume *volume, PupVMMonitor *monitor)
{
	PupDrive *drive = NULL;
	if (volume->drv_sysname)
		drive = pup_vm_monitor_lookup_drive(monitor,
	                                        volume->drv_sysname,
	                                        TRUE);
	if (drive)
	{
		PUP_DEVICE(volume)->icon_name
			= g_strdup(PUP_DEVICE(drive)->icon_name);
		g_object_unref(drive);
	}
	else
	{
		volume->drv_sysname = NULL;
		PUP_DEVICE(volume)->icon_name = g_strdup(PUP_ICON_GENERIC_VOLUME);
	}
}

