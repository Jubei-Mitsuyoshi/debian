//drive-probes.c or drive-probes.h
//Probing for drives and volumes

#ifndef PUP_VM_H_INSIDE
//drive-probes.c
#include "common.h"
#else
//drive-probes.h

#endif //PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
void pup_volume_set_icon(PupVolume *volume,PupVMMonitor *monitor);
struct udev_device *pup_volume_search_for_drive(struct udev_device *dev);
void pup_volume_update_from_blkid_probe(PupVolume *volume,struct udev_device *dev,blkid_probe probe);
gboolean pup_volume_assert(struct udev_device *dev,blkid_probe *probe_return);
PupVolume *pup_volume_new_from_blkid_cache(blkid_cache cache,const gchar *devnode);
guint pup_volume_update_mount(PupVolume *volume,PupMntEntry *entry);
void pup_drive_update(PupDrive *drive,struct udev_device *dev);
gboolean pup_drive_assert(struct udev_device *dev);
