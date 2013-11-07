//pupvm-classes.c or pupvm-classes.h
//Common definitions for the pup_volume_monitor

#ifndef PUP_VM_H_INSIDE
#	include "common-includes.h"
#else // !PUP_VM_H_INSIDE

typedef struct _PupVMMonitor PupVMMonitor;

//Device base class
//No instances of this struct is created
typedef struct
{
	GObject parent;

	gint catagory;
	gchar *sysname;
	
	volatile gint hold_count;
} PupDeviceHeaderArea;

/**Structure representing a PupDevice class*/
typedef struct
{
	///Parent instance 
	GObject parent;
	
	///Catagory id, either PUP_CATAGORY_DRIVE or PUP_CATAGORY_VOLUME
	gint catagory;
	///Unique name that identifies the object, preferably sysfs name
	gchar *sysname;

	volatile gint hold_count;

	///A string describing icon that should be used
	gchar *icon_name;
	///The name by which the device should be displayed
	gchar *display_name;
	gboolean constructed;
	
	PupVMMonitor *monitor;
} PupDevice;

/**Class structure*/
typedef struct
{
	//Parent class
	GObjectClass parent;

	gboolean (*parse_func)
		(PupDevice *dev, PSDataParser *parser);
	void (*encode_func)
		(PupDevice *dev, PSDataEncoder *encoder);
	///Function used to display attributes of the device, mainly for debugging purposes.
	///If you override this make sure you call parent class' method before
	///doing your own processing.
	void (*display) (PupDevice *dev);
	///Function used to free all dynamically allocated memory in the device.
	///You must chain up after performing your own de-allocation.
	///You may also zero-initialise all instance members.
	void (*free_contents) (PupDevice *dev);
	///Function used to copy contents of _src_ into _dest_.
	///You must call the parent class' method before performing your copying
	///operation.
	void (*dup) (PupDevice *src, PupDevice *dest);
	///The size of the instance. You must set this to sizeof(<YourInstance>)
	gsize size;
} PupDeviceClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_DEVICE PupDevice pup_device pup
#define PUP_TYPE_DEVICE (pup_device_get_type())
#define PUP_DEVICE(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_DEVICE, PupDevice))
#define PUP_IS_DEVICE(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_DEVICE))
#define PUP_DEVICE_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_DEVICE, PupDeviceClass))
#define PUP_IS_DEVICE_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_DEVICE))
#define PUP_DEVICE_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_DEVICE, PupDeviceClass))
#define PUP_DEVICE_NAME "PupDevice"

typedef struct 
{
	gint catagory;
	gchar *sysname;
} PupDeviceHeader;

typedef enum 
{
	PUP_CATAGORY_DRIVE = 1,
	PUP_CATAGORY_VOLUME = 2,
	PUP_CATAGORY_ICON = 3
} PupCatagory;

//Warning: Changing these values implies protocol change
#	define PUP_DEVICE_EVENT_NONE "none"
#	define PUP_DEVICE_EVENT_ADD "add"
#	define PUP_DEVICE_EVENT_CHANGE "change"
#	define PUP_DEVICE_EVENT_REMOVE "remove"

typedef enum 
{
	PUP_DEVICE_ADD = 1,
	PUP_DEVICE_REMOVE = 2,
	PUP_DEVICE_CHANGE = 3
} PupDeviceUpdateActions;

typedef struct
{
	PupDeviceHeader header;
	gint action;
	gchar *detail;
} PupVMEvent;


//Operations
typedef struct _PupOperation PupOperation;
///\brief Prototype of the function for handling operations
typedef void (*PupOperationFunc) (PupDevice *dev, PupOperation *operation);
///\brief Prototype of the function called when user responds to a question
typedef void (*PupOperationReplyFunc) (PupDevice *dev,
                                       PupOperation *operation,
		                               guint status);
///\brief Prototype of the function called when user responds to a password prompt
typedef void (*PupOperationPasswordFunc) (PupDevice *dev,
                                          PupOperation *operation,
		                                  guint status, 
                                          gchar *username,
                                          gchar *password,
                                          gchar *domain);
///\brief Prototype of the function called when a suboperation returns
typedef void (*PupOperationTrapFunc) (PupDevice *dev,
                                      PupOperation *operation,
		                              guint status,
                                      const gchar *detail,
                                      gpointer user_data);

typedef void (*PupOperationMsgFunc) (PupOperation *operation,
                                     PSDataEncoder *encoder);
///\brief A structure that represents an operation
struct _PupOperation
{
	//Readonly
	///The device on which operation was called
	PupDevice *dev;
	///Name of the operation, i.e the task to carry out
	gchar *type;
	///Argument(s) to the operation 
	gchar *args;
	//Readwrite
	///Function called when user replies
	PupOperationReplyFunc reply_func;
	//Output trapping
	PupOperationTrapFunc trap_func;
	PupOperation *prev_oper;
	gpointer user_data;
	//Private
	PupOperationMsgFunc msg_func;
	gboolean has_returned;
};

///\brief Possible return status of a question
typedef enum 
{
	PUP_OPERATION_RESPONSE_UNHANDLED = 1,
	PUP_OPERATION_RESPONSE_ABORT = 2,
	PUP_OPERATION_RESPONSE_PASSWORD = 3,

	PUP_OPERATION_RESPONSE_SHIFT = 4 //Value to add to response number
} PupOperationResponse;

//Class volume: a normal volume
typedef struct
{
	PupDevice parent;
	gchar *unix_dev;
	gchar *label;
	gchar *fstype;
	gchar *uuid;
	gchar *drv_sysname;
	gchar *mntpnt;
	guint flags;
} PupVolume;

typedef struct
{
	PupDeviceClass parent;

	PupOperationFunc mount;
	PupOperationFunc umount;
} PupVolumeClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_VOLUME PupVolume pup_volume pup
#define PUP_TYPE_VOLUME (pup_volume_get_type())
#define PUP_VOLUME(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_VOLUME, PupVolume))
#define PUP_IS_VOLUME(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_VOLUME))
#define PUP_VOLUME_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_VOLUME, PupVolumeClass))
#define PUP_IS_VOLUME_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_VOLUME))
#define PUP_VOLUME_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_VOLUME, PupVolumeClass))
#define PUP_VOLUME_NAME "PupVolume"

typedef enum
{
	//Warning: Changing these values implies protocol change
	PUP_VOLUME_IS_MOUNTABLE = 1 << 0,
	PUP_VOLUME_MNTD_READ_ONLY = 1 << 1,
	PUP_VOLUME_MNTD_SYSTEM = 1 << 2,
	PUP_VOLUME_CAN_EJECT = 1 << 3,
	PUP_VOLUME_SHOULD_AUTOMOUNT = 1 << 4,
	PUP_VOLUME_OVERRIDE_MNT_INFO = 1 << 5,

	PUP_VOLUME_MNT_FLAGS = PUP_VOLUME_MNTD_READ_ONLY | PUP_VOLUME_MNTD_SYSTEM
} PupVolumeFlags;

#	define PUP_VOLUME_EVENT_MOUNT "mount"
#	define PUP_VOLUME_EVENT_UMOUNT "umount"
#	define PUP_VOLUME_EVENT_REMOUNT "remount"

#	define PUP_VOLUME_MOUNT_CMD(vol) "mount", \
		(vol)->parent.sysname, (vol)->unix_dev, \
		PUP_STR_NULL_IS_BLANK((vol)->fstype), \
		PUP_STR_NULL_IS_BLANK((vol)->mntpnt), \
		PUP_STR_NULL_IS_BLANK((vol)->label), \
		PUP_STR_NULL_IS_BLANK((vol)->uuid), \
		NULL

#	define PUP_VOLUME_UMOUNT_CMD(vol) "umount", \
		(vol)->parent.sysname, (vol)->unix_dev, \
		PUP_STR_NULL_IS_BLANK((vol)->fstype), \
		PUP_STR_NULL_IS_BLANK((vol)->mntpnt), \
		PUP_STR_NULL_IS_BLANK((vol)->label), \
		PUP_STR_NULL_IS_BLANK((vol)->uuid), \
		NULL

typedef enum 
{
	PUP_VOLUME_MOUNT = 1,
	PUP_VOLUME_UMOUNT = 2,
	PUP_VOLUME_REMOUNT = 3
} PupVolumeUpdateActions;

//Class drive: a physical drive
typedef struct
{
	PupDevice parent;
	gchar *unix_dev;
	gchar *model;
	gchar *vendor;
	guint flags;
} PupDrive;

typedef struct
{
	PupDeviceClass parent;

} PupDriveClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_DRIVE PupDrive pup_drive pup
#define PUP_TYPE_DRIVE (pup_drive_get_type())
#define PUP_DRIVE(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_DRIVE, PupDrive))
#define PUP_IS_DRIVE(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_DRIVE))
#define PUP_DRIVE_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_DRIVE, PupDriveClass))
#define PUP_IS_DRIVE_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_DRIVE))
#define PUP_DRIVE_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_DRIVE, PupDriveClass))
#define PUP_DRIVE_NAME "PupDrive"

typedef enum
{
	//Warning: Changing these values implies protocol change
	PUP_DRIVE_USB = 1 << 0,
	PUP_DRIVE_REMOVABLE = 1 << 1,
	PUP_DRIVE_FLASH = 1 << 2,
	PUP_DRIVE_CAN_EJECT = 1 << 3
} PupDriveFlags;

//VTable for implementation to be done elsewhere...
typedef struct
{
	//device
	GDestroyNotify device_release_func;
	//volume
	PupOperationFunc volume_mount;
	PupOperationFunc volume_umount;
} PupVMImplVTable;

#endif //PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
void pup_operation_foreward(PupOperation *operation,PupDevice *target_dev,const gchar *oper_name,const gchar *args);
void pup_operation_start_suboperation(PupOperation *operation,PupDevice *dev,const gchar *oper_name,const gchar *args,PupOperationTrapFunc func,gpointer user_data);
void pup_operation_ask_question(PupOperation *operation,const gchar *question,const gchar *choices);
void pup_operation_ask_password(PupOperation *operation,const gchar *message,GAskPasswordFlags password_flags);
#if !defined(PUP_VM_H_INSIDE)
void pup_operation_return(PupOperation *operation,gboolean success,guint g_io_error_code,const gchar *detail_f,...);
#endif
#if !(!defined(PUP_VM_H_INSIDE))
void pup_operation_return(PupOperation *operation,gboolean success,guint g_io_error_code,const gchar *detail_f,...)G_GNUC_PRINTF(4,5);
#endif
void pup_operation_start(PupOperation *operation,PupDevice *dev);
gchar *pup_drive_gen_display_name(PupDrive *drv);
void pup_drive_dup(PupDevice *src,PupDevice *dest);
void pup_drive_free_mem(PupDevice *dev);
void pup_drive_display(PupDevice *dev);
void pup_drive_encode_func(PupDevice *dev,PSDataEncoder *encoder);
gboolean pup_drive_parse_func(PupDevice *dev,PSDataParser *parser);
#if !(!defined(PUP_VM_H_INSIDE))
GType pup_drive_get_type();
#endif
gchar *pup_volume_gen_display_name(PupVolume *vol);
gchar *pup_volume_gen_mount_dir(PupVolume *vol);
void pup_volume_dup(PupDevice *src,PupDevice *dest);
void pup_volume_free_mem(PupDevice *dev);
void pup_volume_display(PupDevice *dev);
void pup_volume_encode_func(PupDevice *dev,PSDataEncoder *encoder);
gboolean pup_volume_parse_func(PupDevice *dev,PSDataParser *parser);
#if !(!defined(PUP_VM_H_INSIDE))
GType pup_volume_get_type();
#endif
void pup_device_show_hfunc(gpointer key,gpointer value,gpointer dummy);
void pup_device_show(PupDevice *dev);
void pup_device_swap_contents(PupDevice *dev1,PupDevice *dev2);
PupDevice *pup_device_dup(PupDevice *dev);
void pup_device_clear_data(PupDevice *dev);
void pup_device_release(PupDevice *dev);
void pup_device_hold(PupDevice *dev);
PupDevice *pup_device_new(GType type,const gchar *sysname);
void pup_vm_event_free_data(PupVMEvent *event,gboolean free_header);
gboolean pup_device_parse_event(PSDataParser *parser,PupVMEvent *event);
void pup_device_encode_change(PupDevice *self,guint action,const gchar *detail,PSDataEncoder *encoder);
void pup_device_encode(PSDataEncoder *encoder,gpointer data,gpointer dummy);
gboolean pup_device_parse(PSDataParser *parser,gpointer *key_return,gpointer *data_return,gpointer dummy);
PupDevice *pup_device_new_from_header(PupDeviceHeader *header,PSDataParser *parser);
gboolean pup_device_update_from_parser(PupDevice *self,PSDataParser *parser);
void pup_device_header_free_data(PupDeviceHeader *header);
gboolean pup_device_parse_header(PupDeviceHeader *header,PSDataParser *parser);
void pup_device_class_register_operation(GType type,const gchar *name,gssize class_offset);
void pup_device_finalize(GObject *instance);
#if !(!defined(PUP_VM_H_INSIDE))
GType pup_device_get_type();
#endif
#if !defined(PUP_VM_H_INSIDE)
G_DEFINE_TYPE(PupDevice,pup_device,G_TYPE_OBJECT);
G_DEFINE_TYPE(PupDrive,pup_drive,PUP_TYPE_DEVICE);
G_DEFINE_TYPE(PupVolume,pup_volume,PUP_TYPE_DEVICE);
#endif
extern gboolean pup_vm_is_client;
extern PupVMImplVTable pup_vm_impl_vtable;
