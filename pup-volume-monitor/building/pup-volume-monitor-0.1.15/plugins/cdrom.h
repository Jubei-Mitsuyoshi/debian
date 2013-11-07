//libcdrom: plugin to handle optical drives

//FILE_HEADER_SUBST:gobject_macro_gen PUP_CD_DRIVE PupCDDrive pup_cd_drive pup
#define PUP_TYPE_CD_DRIVE (pup_cd_drive_get_type())
#define PUP_CD_DRIVE(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_CD_DRIVE, PupCDDrive))
#define PUP_IS_CD_DRIVE(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_CD_DRIVE))
#define PUP_CD_DRIVE_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_CD_DRIVE, PupCDDriveClass))
#define PUP_IS_CD_DRIVE_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_CD_DRIVE))
#define PUP_CD_DRIVE_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_CD_DRIVE, PupCDDriveClass))
#define PUP_CD_DRIVE_NAME "PupCDDrive"
//FILE_HEADER_SUBST:gobject_macro_gen PUP_CD PupCD pup_cd pup
#define PUP_TYPE_CD (pup_cd_get_type())
#define PUP_CD(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_CD, PupCD))
#define PUP_IS_CD(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_CD))
#define PUP_CD_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_CD, PupCDClass))
#define PUP_IS_CD_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_CD))
#define PUP_CD_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_CD, PupCDClass))
#define PUP_CD_NAME "PupCD"

/*Everything beyond this is automatically generated*/
G_MODULE_EXPORT void pup_vm_plugin_init(PupVMPlugin *plugin);
void pup_cdrom_udev_event(PupVMMonitor *monitor,struct udev_device *dev);
PupCD *pup_cd_new_from_drive(PupCDDrive *drv,PupVMMonitor *monitor);
void pup_cd_eject_continue(PupDevice *dev,PupOperation *operation);
void pup_cd_eject(PupDevice *dev,PupOperation *operation);
void pup_cd_mount(PupDevice *dev,PupOperation *operation);
void pup_cd_dup(PupDevice *src,PupDevice *dest);
void pup_cd_display(PupDevice *dev);
void pup_cd_drive_get_all(PupVMMonitor *monitor);
void pup_cd_drive_probe_disk_cb(PupVMMonitor *monitor,PupCDDrive *drv);
PupDrive *pup_cd_drive_new_from_udev_device(PupVMMonitor *monitor,struct udev_device *device);
guint pup_cd_drive_get_status(PupCDDrive *drv);
void pup_cd_drive_eject(PupDevice *dev,PupOperation *operation);
void pup_cd_drive_dup(PupDevice *src,PupDevice *dest);
void pup_cd_drive_display(PupDevice *dev);
#if defined(PUP_VM_PLUGIN_SOURCE)
G_DEFINE_DYNAMIC_TYPE(PupCDDrive,pup_cd_drive,PUP_TYPE_DRIVE);
G_DEFINE_DYNAMIC_TYPE(PupCD,pup_cd,PUP_TYPE_VOLUME);
#endif
extern GThreadPool *pup_cdrom_probe_thread_pool;
PupDevice *pup_overwrite_dev(PupVMMonitor *monitor,PupDevice *dev);
extern const gchar *pup_cd_drive_events[];
extern const gchar *pup_cd_drive_status[];
