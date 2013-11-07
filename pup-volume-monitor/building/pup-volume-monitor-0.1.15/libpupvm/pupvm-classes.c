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

//FILE_HEADER_END

//VTable for implementation to be done elsewhere...
PupVMImplVTable pup_vm_impl_vtable;
gboolean pup_vm_is_client = TRUE;

//Class Device
#ifndef PUP_VM_H_INSIDE
G_DEFINE_TYPE(PupDevice, pup_device, G_TYPE_OBJECT);
#else 
GType pup_device_get_type();
#endif

static void pup_device_class_init(PupDeviceClass *klass)
{
	GObjectClass *basic_class = G_OBJECT_CLASS(klass);

	basic_class->finalize = pup_device_finalize;
}

void pup_device_class_register_operation(GType type, const gchar *name,
                                         gssize class_offset)
{
	g_signal_new(name, type, G_SIGNAL_RUN_FIRST, class_offset, NULL, NULL,
	             g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);
}

static void pup_device_init(PupDevice *self)
{
	self->hold_count = 0;
}

void pup_device_finalize(GObject *instance)
{
	PupDevice *self = PUP_DEVICE(instance);

	if (self->constructed)
	{
		PUP_DEVICE_GET_CLASS(self)->free_contents(self);
		g_free(self->sysname);
		g_free(self->icon_name);
		g_free(self->display_name);
	}

	//Chain up
	G_OBJECT_CLASS(pup_device_parent_class)->finalize(instance);
}

gboolean pup_device_parse_header(PupDeviceHeader *header, PSDataParser *parser)
{
	gboolean error = FALSE;
	header->catagory = pup_vm_extract_tag(parser, &error);
	header->sysname = ps_data_parser_parse_str0(parser, &error);
	g_return_val_if_fail(!error, FALSE);
	return TRUE;
}

void pup_device_header_free_data(PupDeviceHeader *header)
{
	g_free(header->sysname);
}

gboolean pup_device_update_from_parser(PupDevice *self, PSDataParser *parser)
{
	gboolean error = FALSE;
	if (self->constructed)
	{
		PUP_DEVICE_GET_CLASS(self)->free_contents(self);
		g_free(self->icon_name);
		g_free(self->display_name);
	}
	self->constructed = FALSE; //Precaution
	self->icon_name = ps_data_parser_parse_str0(parser, &error);
	self->display_name = ps_data_parser_parse_str0(parser, &error);
	g_return_val_if_fail(!error, FALSE);

	error = PUP_DEVICE_GET_CLASS(self)->parse_func(self, parser);
	if (error) self->constructed = TRUE;
	return error;
}

PupDevice *pup_device_new_from_header(PupDeviceHeader *header, PSDataParser *parser)
{
	GType type;
	switch (header->catagory)
	{
		case PUP_CATAGORY_DRIVE:
			type = PUP_TYPE_DRIVE;
			break;
		case PUP_CATAGORY_VOLUME:
			type = PUP_TYPE_VOLUME;
			break;
		default:
			g_critical("Unknown catagory (%d)", header->catagory);
			return NULL;
	}
	PupDevice *self = PUP_DEVICE(g_object_new(type, NULL));
	self->catagory = header->catagory;
	self->sysname = g_strdup(header->sysname);
	g_return_val_if_fail(pup_device_update_from_parser(self, parser), NULL);
	return self;
}

gboolean pup_device_parse(PSDataParser *parser,
                          gpointer *key_return, gpointer *data_return,
                          gpointer dummy)
{
	PupDeviceHeader header;

	g_return_val_if_fail(pup_device_parse_header(&header, parser), FALSE);
	*data_return = pup_device_new_from_header(&header, parser);
	g_return_val_if_fail(*data_return, FALSE);
	if (key_return) *key_return = ((PupDevice *) *data_return)->sysname;
	return TRUE;
}

void pup_device_encode(PSDataEncoder *encoder, gpointer data, 
                       gpointer dummy)
{
	PupDevice *self = (PupDevice *) data;
	pup_vm_encode_tag(encoder, self->catagory);
	ps_data_encoder_add_str0(encoder, self->sysname, PS_DATA_ENCODER_ALLOC);
	ps_data_encoder_add_str0(encoder, self->icon_name, PS_DATA_ENCODER_ALLOC);
	ps_data_encoder_add_str0(encoder, self->display_name, PS_DATA_ENCODER_ALLOC);

	PUP_DEVICE_GET_CLASS(self)->encode_func(self, encoder);
}

void pup_device_encode_change(PupDevice *self, guint action,
                              const gchar *detail, PSDataEncoder *encoder)
{
	pup_vm_encode_tag(encoder, action);
	ps_data_encoder_add_str0(encoder, detail, PS_DATA_ENCODER_ALLOC);
	pup_device_encode(encoder, (gpointer) self, NULL);
}

gboolean pup_device_parse_event(PSDataParser *parser, PupVMEvent *event)
{
	gboolean error = FALSE;

	event->action = pup_vm_extract_tag(parser, &error);
	event->detail = ps_data_parser_parse_str0(parser, &error);

	g_return_val_if_fail(!error, FALSE);

	g_return_val_if_fail(pup_device_parse_header(&(event->header), parser),
	                     FALSE);

	return TRUE;
}

void pup_vm_event_free_data(PupVMEvent *event, gboolean free_header)
{
	g_free(event->detail);

	if (free_header) pup_device_header_free_data(&(event->header));

	pup_vm_nullify_struct(event, PupVMEvent);
}

PupDevice *pup_device_new(GType type, const gchar *sysname)
{
	gint catagory;
	PupDevice *dev;
	if (g_type_is_a(type, PUP_TYPE_VOLUME))
		catagory = PUP_CATAGORY_VOLUME;
	else if (g_type_is_a(type, PUP_TYPE_DRIVE))
		catagory = PUP_CATAGORY_DRIVE;
	else
	{
		g_critical("A new device must be a volume or a drive");
		return NULL;
	}

	dev = PUP_DEVICE(g_object_new(type, NULL));

	dev->catagory = catagory;
	dev->sysname = g_strdup(sysname);

	return dev;
}

void pup_device_hold(PupDevice *dev)
{
	g_atomic_int_inc(&(dev->hold_count));
	g_object_ref(dev);
}

void pup_device_release(PupDevice *dev)
{
	if (g_atomic_int_dec_and_test(&(dev->hold_count)))
	{
		if (pup_vm_impl_vtable.device_release_func) 
			pup_vm_impl_vtable.device_release_func(dev);
	}
	
	g_object_unref(dev);
}

void pup_device_clear_data(PupDevice *dev)
{
	if (dev->constructed)
	{
		PUP_DEVICE_GET_CLASS(dev)->free_contents(dev);
	}
}

PupDevice *pup_device_dup(PupDevice *dev)
{
	g_return_val_if_fail(PUP_IS_DEVICE(dev), NULL);

	PupDevice *new_dev = pup_device_new(G_TYPE_FROM_INSTANCE(dev),
	                                    dev->sysname);

	new_dev->display_name = g_strdup(dev->display_name);
	new_dev->icon_name = g_strdup(dev->icon_name);
	new_dev->constructed = dev->constructed;
	new_dev->monitor = dev->monitor;

	PUP_DEVICE_GET_CLASS(dev)->dup(dev, new_dev);

	return new_dev;
}

void pup_device_swap_contents(PupDevice *dev1, PupDevice *dev2)
{
	g_return_if_fail(G_TYPE_FROM_INSTANCE(dev1) == G_TYPE_FROM_INSTANCE(dev2));
	//We should not copy GObject portion
	gsize copy_bytes = PUP_DEVICE_GET_CLASS(dev1)->size - sizeof(PupDeviceHeaderArea);
	gpointer backup = g_memdup(&(dev1->icon_name), copy_bytes);
	g_memmove(&(dev1->icon_name), &(dev2->icon_name), copy_bytes);
	g_memmove(&(dev2->icon_name), backup, copy_bytes);
	g_free(backup);
}

void pup_device_show(PupDevice *dev)
{
	printf("Device %s:\n\tdisplay_name=%s\n\ticon_name=%s\t\n\ttype=%s\t\n",
	       dev->sysname, dev->display_name, dev->icon_name,
	       g_type_name(G_TYPE_FROM_INSTANCE(dev)));
	PUP_DEVICE_GET_CLASS(dev)->display(dev);
}

void pup_device_show_hfunc(gpointer key, gpointer value, gpointer dummy)
{
	pup_device_show(PUP_DEVICE(value));
}

//Class Volume
#ifndef PUP_VM_H_INSIDE
G_DEFINE_TYPE(PupVolume, pup_volume, PUP_TYPE_DEVICE);
#else
GType pup_volume_get_type();
#endif

static void pup_volume_class_init(PupVolumeClass *klass)
{
	PupDeviceClass *dev_class = PUP_DEVICE_CLASS(klass);

	dev_class->parse_func = pup_volume_parse_func;
	dev_class->encode_func = pup_volume_encode_func;
	dev_class->display = pup_volume_display;
	dev_class->free_contents = pup_volume_free_mem;
	dev_class->dup = pup_volume_dup;
	dev_class->size = sizeof(PupVolume);

	if (pup_vm_is_client) return;

	pup_device_class_register_operation(PUP_TYPE_VOLUME, "mount",
	                                    G_STRUCT_OFFSET(PupVolumeClass, mount));
	pup_device_class_register_operation(PUP_TYPE_VOLUME, "umount",
	                                    G_STRUCT_OFFSET(PupVolumeClass, umount));

	klass->mount = pup_vm_impl_vtable.volume_mount;
	klass->umount = pup_vm_impl_vtable.volume_umount;
}

static void pup_volume_init(PupVolume *self)
{
	self->flags |= PUP_VOLUME_IS_MOUNTABLE;
}

gboolean pup_volume_parse_func(PupDevice *dev, PSDataParser *parser)
{
	gboolean error = FALSE;
	PupVolume *self = PUP_VOLUME(dev);
	g_return_val_if_fail(pup_vm_parse_str_group(parser, &(self->unix_dev),
	                                            &(self->mntpnt)),
	                     FALSE);

	self->flags = pup_vm_extract_tag(parser, &error);
	g_return_val_if_fail(!error, FALSE);

	return TRUE;
}

void pup_volume_encode_func(PupDevice *dev, PSDataEncoder *encoder)
{
	PupVolume *self = PUP_VOLUME(dev);
	pup_vm_encode_str_group(encoder, &(self->unix_dev), &(self->mntpnt));

	pup_vm_encode_tag(encoder, self->flags);
}

void pup_volume_display(PupDevice *dev)
{
	PupVolume *volume = PUP_VOLUME(dev);

	printf("\tunix_dev=%s\n", volume->unix_dev);
	printf("\tlabel=%s\n", volume->label);
	printf("\tfstype=%s\n", volume->fstype);
	printf("\tuuid=%s\n", volume->uuid);
	printf("\tdrv_sysname=%s\n", volume->drv_sysname);
	printf("\tmntpnt=%s\n", volume->mntpnt);
	printf("\tflags = 0");
	if (volume->flags & PUP_VOLUME_MNTD_READ_ONLY)
		printf (" | PUP_VOLUME_MNTD_READ_ONLY");
	if (volume->flags & PUP_VOLUME_MNTD_SYSTEM)
		printf (" | PUP_VOLUME_MNTD_SYSTEM");
	if (volume->flags & PUP_VOLUME_CAN_EJECT)
		printf (" | PUP_VOLUME_CAN_EJECT");
	if (volume->flags & PUP_VOLUME_IS_MOUNTABLE)
		printf (" | PUP_VOLUME_IS_MOUNTABLE");
	if (volume->flags & PUP_VOLUME_SHOULD_AUTOMOUNT)
		printf (" | PUP_VOLUME_SHOULD_AUTOMOUNT");
	if (volume->flags & PUP_VOLUME_OVERRIDE_MNT_INFO)
		printf (" | PUP_VOLUME_OVERRIDE_MNT_INFO");
	puts("");
}

void pup_volume_free_mem(PupDevice *dev)
{
	PupVolume *vol = PUP_VOLUME(dev);

	g_free(vol->unix_dev);
	g_free(vol->label);
	g_free(vol->uuid);
	g_free(vol->fstype);
	g_free(vol->drv_sysname);
	g_free(vol->mntpnt);

	vol->unix_dev = vol->label = vol->uuid = vol->drv_sysname = vol->mntpnt
		= NULL; //Precaution
}

void pup_volume_dup(PupDevice *src, PupDevice *dest)
{
	PupVolume *src_vol = PUP_VOLUME(src);
	PupVolume *dest_vol = PUP_VOLUME(dest);

	dest_vol->unix_dev = g_strdup(src_vol->unix_dev);
	dest_vol->label = g_strdup(src_vol->label);
	dest_vol->uuid = g_strdup(src_vol->uuid);
	dest_vol->fstype = g_strdup(src_vol->fstype);
	dest_vol->drv_sysname = g_strdup(src_vol->drv_sysname);
	dest_vol->mntpnt = g_strdup(src_vol->mntpnt);
	dest_vol->flags = src_vol->flags;
}

gchar *pup_volume_gen_mount_dir(PupVolume *vol)
{
	PupDevice *dev = PUP_DEVICE(vol);
	PupVMSettings *settings = pup_vm_settings_get();

	gchar *targets[] = {"$sysname", "$unix_dev", "$label", "$uuid", "$fstype",
		NULL};
	gchar *substs[] = {dev->sysname, vol->unix_dev, vol->label, vol->uuid,
		vol->fstype, NULL};

	gchar *template;
	if ((! vol->label) && strstr(settings->volume_disp_name, "$label"))
		template = settings->volume_mntpnt_name_fallback;
	else if ((! vol->uuid) && strstr(settings->volume_disp_name, "$uuid"))
		template = settings->volume_mntpnt_name_fallback;
	else
		template = settings->volume_mntpnt_name;

	gchar *mnt_name = pup_strrpl(template, targets, substs);
	g_strcanon(mnt_name, PUP_MNTPNT_ALLOWED_CHARS, ' ');
	
	gchar *res;
	if (mnt_name)
	{
		res = g_build_filename(settings->volume_mount_dir,
		                       mnt_name, NULL);
		g_free(mnt_name);
	}
	else
		res = g_build_filename(settings->volume_mount_dir,
		                       dev->sysname, NULL);


	return res;
}

gchar *pup_volume_gen_display_name(PupVolume *vol)
{
	PupDevice *dev = PUP_DEVICE(vol);
	PupVMSettings *settings = pup_vm_settings_get();

	gchar *targets[] = {"$sysname", "$unix_dev", "$label", "$uuid", "$fstype",
		NULL};
	gchar *substs[] = {dev->sysname, vol->unix_dev, vol->label, vol->uuid,
		vol->fstype, NULL};

	gchar *template;
	if ((! vol->label) && strstr(settings->volume_disp_name, "$label"))
		template = settings->volume_disp_name_fallback;
	else if ((! vol->uuid) && strstr(settings->volume_disp_name, "$uuid"))
		template = settings->volume_disp_name_fallback;
	else
		template = settings->volume_disp_name;

	gchar *res = pup_strrpl(template, targets, substs);
	if (! res) res = g_strdup(dev->sysname);

	return res;
}
//Class Drive

#ifndef PUP_VM_H_INSIDE
G_DEFINE_TYPE(PupDrive, pup_drive, PUP_TYPE_DEVICE);
#else
GType pup_drive_get_type();
#endif

static void pup_drive_class_init(PupDriveClass *klass)
{
	PupDeviceClass *dev_class = PUP_DEVICE_CLASS(klass);

	dev_class->parse_func = pup_drive_parse_func;
	dev_class->encode_func = pup_drive_encode_func;
	dev_class->display = pup_drive_display;
	dev_class->free_contents = pup_drive_free_mem;
	dev_class->dup = pup_drive_dup;
	dev_class->size = sizeof(PupDrive);
}

static void pup_drive_init(PupDrive *drive)
{

}

gboolean pup_drive_parse_func(PupDevice *dev, PSDataParser *parser)
{
	PupDrive *self = PUP_DRIVE(dev);
	gboolean error = FALSE;

	g_return_val_if_fail(pup_vm_parse_str_group(parser, &(self->unix_dev),
	                                            &(self->vendor)),
	                     FALSE);

	self->flags = pup_vm_extract_tag(parser, &error);
	g_return_val_if_fail(!error, FALSE);

	return TRUE;
}

void pup_drive_encode_func(PupDevice *dev, PSDataEncoder *encoder)
{
	PupDrive *self = PUP_DRIVE(dev);

	pup_vm_encode_str_group(encoder, &(self->unix_dev), &(self->vendor));
	pup_vm_encode_tag(encoder, self->flags);
}

void pup_drive_display(PupDevice *dev)
{
	PupDrive *drive = PUP_DRIVE(dev);
	printf("\tunix_dev=%s\n\
	model=%s\n\
	vendor=%s\n\
	props = 0", drive->unix_dev, drive->model, drive->vendor);
	if (drive->flags & PUP_DRIVE_USB) printf(" | PUP_DRIVE_USB");
	if (drive->flags & PUP_DRIVE_REMOVABLE) printf(" | PUP_DRIVE_REMOVABLE");
	if (drive->flags & PUP_DRIVE_FLASH) printf(" | PUP_DRIVE_FLASH");
	if (drive->flags & PUP_DRIVE_CAN_EJECT) printf(" | PUP_DRIVE_CAN_EJECT");
	printf("\n");
}

void pup_drive_free_mem(PupDevice *dev)
{
	PupDrive *drv = PUP_DRIVE(dev);
	g_free(drv->unix_dev);
	g_free(drv->model);
	g_free(drv->vendor);

	drv->unix_dev = drv->model = drv->vendor = NULL;
}

void pup_drive_dup(PupDevice *src, PupDevice *dest)
{
	PupDrive *src_drv = PUP_DRIVE(src);
	PupDrive *dest_drv = PUP_DRIVE(dest);

	dest_drv->unix_dev = g_strdup(src_drv->unix_dev);
	dest_drv->model = g_strdup(src_drv->model);
	dest_drv->vendor = g_strdup(src_drv->vendor);
	dest_drv->flags = src_drv->flags;
}

gchar *pup_drive_gen_display_name(PupDrive *drv)
{
	PupDevice *dev = PUP_DEVICE(drv);
	PupVMSettings *settings = pup_vm_settings_get();

	gchar *targets[] = {"$sysname", "$unix_dev", "$model", "$vendor", NULL};
	gchar *substs[] = {dev->sysname, drv->unix_dev, drv->model, drv->vendor, NULL};

	gchar *res = pup_strrpl(settings->drive_disp_name, targets, substs);
	if (! res) res = g_strdup(dev->sysname);

	return res;
}

//Operation (Not a class, but atleast a polymorphic type)
void pup_operation_start(PupOperation *operation, PupDevice *dev)
{
	g_return_if_fail(PUP_IS_DEVICE(dev));
	GType type = G_TYPE_FROM_INSTANCE(dev);
	guint signal_id = g_signal_lookup(operation->type, type);
	if (! signal_id)
	{
		g_critical("Operation \"%s\" is not valid for instance %p, type \"%s\"",
		           operation->type, dev, g_type_name(type));
		pup_operation_return(operation, FALSE, G_IO_ERROR_NOT_SUPPORTED,
		                     "Operation not supported");
		return;
	}

	pup_device_hold(dev);
	operation->dev = dev;
	g_signal_emit(dev, signal_id, 0, operation);
}

#ifndef PUP_VM_H_INSIDE
void pup_operation_return(PupOperation *operation,
                          gboolean success, guint g_io_error_code,
                          const gchar *detail_f, ...)
{
	PupDevice *backup = operation->dev;
	
	if (operation->trap_func)
	{
		gint exitcode;
		gchar *detail;
		if (success)
			exitcode = 0;
		else
			exitcode = g_io_error_code + 1;
		va_list arglist;
		va_start(arglist, detail_f);
		detail = g_strdup_vprintf(detail_f, arglist);
		operation->trap_func(operation->prev_oper->dev,
		                     operation->prev_oper,
		                     exitcode, detail, operation->user_data);
		if (operation->dev) pup_device_release(operation->dev);
		g_free(detail);
		g_free(operation->type);
		g_free(operation->args);
		g_slice_free(PupOperation, operation);
	}
	else
	{
		PSDataEncoder *encoder = ps_data_encoder_new();
		pup_vm_encode_tag(encoder, PUP_TAG_OPERATION_RETURN);
		if (success)
		{
			pup_vm_encode_tag(encoder, 0);
		}
		else
		{
			pup_vm_encode_tag(encoder, g_io_error_code + 1);
		}
		va_list arglist;
		va_start(arglist, detail_f);
		gchar *detail = g_strdup_vprintf(detail_f, arglist);
		ps_data_encoder_add_str0(encoder, detail, PS_DATA_ENCODER_FREE);
		operation->has_returned = TRUE;
		
		operation->msg_func(operation, encoder);

		if (backup) pup_device_release(backup);
	}
}
#else // !PUP_VM_H_INSIDE
void pup_operation_return(PupOperation *operation,
                          gboolean success, guint g_io_error_code,
                          const gchar *detail_f, ...) G_GNUC_PRINTF(4, 5);
#endif //PUP_VM_H_INSIDE

void pup_operation_ask_password(PupOperation *operation, const gchar *message,
                                GAskPasswordFlags password_flags)
{
	PSDataEncoder *encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_TAG_OPERATION_PASSWORD);
	ps_data_encoder_add_str0(encoder, message, PS_DATA_ENCODER_ALLOC);
	pup_vm_encode_tag(encoder, password_flags);

	operation->msg_func(operation, encoder);
}

void pup_operation_ask_question(PupOperation *operation, const gchar *question,
                                const gchar *choices)
{
	PSDataEncoder *encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_TAG_OPERATION_QUESTION);
	ps_data_encoder_add_str0(encoder, question, PS_DATA_ENCODER_ALLOC);
	ps_data_encoder_add_str0(encoder, choices, PS_DATA_ENCODER_ALLOC);
	
	operation->msg_func(operation, encoder);
}

void pup_operation_start_suboperation(PupOperation *operation, PupDevice *dev,
                                      const gchar *oper_name, const gchar *args,
                                      PupOperationTrapFunc func, 
                                      gpointer user_data)
{
	PupOperation *new_oper = g_slice_dup(PupOperation, operation);
	new_oper->type = g_strdup(oper_name);
	new_oper->args = g_strdup(args);
	new_oper->trap_func = func;
	new_oper->prev_oper = operation;
	new_oper->user_data = user_data;

	pup_operation_start(new_oper, dev);
}

void pup_operation_foreward(PupOperation *operation, PupDevice *target_dev,
                            const gchar *oper_name, const gchar *args)
{
	PupDevice *dev = operation->dev;
	if (oper_name)
	{
		g_free(operation->type);
		operation->type = g_strdup(oper_name);
	}
	if (args)
	{
		g_free(operation->args);
		operation->args = g_strdup(args);
	}
	pup_operation_start(operation, target_dev);
	if (dev) pup_device_release(dev);
}
