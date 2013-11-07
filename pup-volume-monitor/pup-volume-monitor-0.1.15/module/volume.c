//volume.c or volume.h
//GVolume interface implementation: PupClientVolume

#ifndef PUP_VM_H_INSIDE
//volume.c
#include "common.h"

#else // !PUP_VM_H_INSIDE
//volume.h

typedef struct _PupMount PupMount;

typedef struct
{
	PupClientDevice parent;
	
	PupMount *mount;
} PupClientVolume;

typedef struct
{
	PupClientDeviceClass parent;

} PupClientVolumeClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_CLIENT_VOLUME PupClientVolume pup_client_volume pup

#define PUP_VOLUME_IDENTIFIER_KIND_FSTYPE "fstype"

#endif //PUP_VM_H_INSIDE

//FILE_HEADER_END

#ifndef PUP_VM_H_INSIDE
G_DEFINE_DYNAMIC_TYPE_EXTENDED(PupClientVolume, pup_client_volume, PUP_TYPE_CLIENT_DEVICE, 0, 
                               G_IMPLEMENT_INTERFACE_DYNAMIC
                               (G_TYPE_VOLUME, pup_client_volume_init_iface)
                               );
#else
GType pup_client_volume_get_type(void);
#endif

//construction and destruction

static void pup_client_volume_class_init(PupClientVolumeClass *klass)
{
	GObjectClass *basic_class = G_OBJECT_CLASS(klass);
	
	basic_class->finalize = pup_client_volume_finalize;

}

static void pup_client_volume_class_finalize(PupClientVolumeClass *klass)
{

}

static void pup_client_volume_init(PupClientVolume *self)
{
	PUP_CLIENT_DEVICE(self)->iface = G_VOLUME(self);
}


void pup_client_volume_finalize(GObject *instance)
{
	//PupClientVolume *self = PUP_VOLUME(instance);

	G_OBJECT_CLASS(pup_client_volume_parent_class)->finalize(instance);
}

void pup_client_volume_register(GIOModule *module)
{
	pup_client_volume_register_type(G_TYPE_MODULE(module));
}

void pup_client_volume_init_iface(GVolumeIface *interface)
{
	interface->get_name = pup_client_volume_get_name;
	interface->get_uuid = pup_client_volume_get_uuid;
	interface->get_drive = pup_client_volume_get_drive;
	interface->enumerate_identifiers = pup_client_volume_enum_identifiers;
	interface->get_identifier = pup_client_volume_get_identifier;
	interface->get_mount = pup_client_volume_get_mount;
	interface->can_mount = pup_client_volume_can_mount;
	interface->can_eject = pup_client_volume_can_eject;
	interface->mount_fn = pup_client_volume_mount;
	interface->mount_finish
		= (gboolean (*)(GVolume *, GAsyncResult *, GError **))
		pup_volume_monitor_generic_finish;
	interface->eject = pup_client_volume_eject;
	interface->eject_finish
		= (gboolean (*)(GVolume *, GAsyncResult *, GError **))
		pup_volume_monitor_generic_finish;
	interface->get_icon = pup_client_volume_get_icon;
	interface->should_automount = pup_client_volume_should_automount;
	
	//eject_with_operation, added since glib 2.22
#if GLIB_CHECK_VERSION(2, 22, 0)
	interface->eject_with_operation = pup_client_volume_eject_w_operation;
	interface->eject_with_operation_finish
		= (gboolean (*)(GVolume *, GAsyncResult *, GError **))
		pup_volume_monitor_generic_finish;
#endif //GLIB_CHECK_VERSION(2, 22, 0)
	
	//sorting key, added since glib 2.32
#if GLIB_CHECK_VERSION(2, 32, 0)
	interface->get_sort_key = pup_client_volume_get_sort_key;
#endif //GLIB_CHECK_VERSION(2, 32, 0)
}

//Private methods


gboolean pup_client_volume_uuid_test_hfunc(gpointer key, gpointer value,
                                    gpointer user_data)
{
	if (strcmp(((PupVolume *) value)->uuid, (gchar *) user_data) == 0)
		return TRUE;
	else
		return FALSE;
}

//Public methods

gchar *pup_client_volume_get_name(GVolume *volume)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	
	pup_client_lock(self);
	gchar *ret = g_strdup(self->holder->display_name);
	pup_client_unlock(self);
	
	return ret;
}

gchar *pup_client_volume_get_uuid(GVolume *volume)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	
	pup_client_lock(self);
	gchar *ret = g_strdup(PUP_VOLUME(self->holder)->uuid);
	pup_client_unlock(self);
	
	return ret;
}

GDrive *pup_client_volume_get_drive(GVolume *volume)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	GDrive *ret = NULL;
	PupDrive *drv = NULL;
	pup_client_lock(self);

	gchar *drv_sysname = PUP_VOLUME(self->holder)->drv_sysname;
	if (drv_sysname)
	{
		drv = pup_vm_monitor_lookup_drive
			(PUP_VM_MONITOR(self->monitor), drv_sysname, FALSE);
	}
	if (drv)
	{
		ret = G_DRIVE(g_object_ref(pup_client_device_get_from_device
		                           (self->monitor, PUP_DEVICE(drv))));
	}
	
	pup_client_unlock(self);
	
	return ret;
}

gchar *pup_client_volume_identifiers[] = {
	G_VOLUME_IDENTIFIER_KIND_LABEL,
	G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE,
	G_VOLUME_IDENTIFIER_KIND_UUID,
	PUP_VOLUME_IDENTIFIER_KIND_FSTYPE,
	NULL};

gchar **pup_client_volume_enum_identifiers(GVolume *volume)
{
	return g_strdupv(pup_client_volume_identifiers);
}

gchar *pup_client_volume_get_identifier(GVolume *volume, const gchar *id)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	gchar *ret = NULL;
	pup_client_lock(self);
	if (g_strcmp0(id, G_VOLUME_IDENTIFIER_KIND_LABEL) == 0)
		ret = g_strdup(PUP_VOLUME(self->holder)->label);
	else if (g_strcmp0(id, G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE) == 0)
		ret = g_strdup(PUP_VOLUME(self->holder)->unix_dev);
	else if (g_strcmp0(id, G_VOLUME_IDENTIFIER_KIND_UUID) == 0)
		ret = g_strdup(PUP_VOLUME(self->holder)->uuid);
	else if (g_strcmp0(id, PUP_VOLUME_IDENTIFIER_KIND_FSTYPE) == 0)
		ret = g_strdup(PUP_VOLUME(self->holder)->fstype);

	pup_client_unlock(self);
	
	return ret;
}

GMount *pup_client_volume_get_mount(GVolume *volume)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	pup_client_lock(self);
	PupMount *mount = NULL;
	if (PUP_VOLUME(self->holder)->mntpnt)
		mount = pup_mount_get(PUP_CLIENT_VOLUME(self));
	pup_client_unlock(self);
	if (mount) return G_MOUNT(g_object_ref(mount));
	else return NULL;
}

void pup_client_volume_mount(GVolume *volume, GMountMountFlags flags,
                      GMountOperation *mount_operation, GCancellable *cancellable,
                      GAsyncReadyCallback callback, gpointer user_data)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	pup_client_lock(self);
	pup_client_monitor_start_operation
		(self->monitor, self->holder,
		 "mount", NULL, mount_operation,
		 g_simple_async_result_new(G_OBJECT(self), callback, user_data,
		                           pup_client_volume_mount));
	pup_client_unlock(self);
}

GIcon *pup_client_volume_get_icon(GVolume *volume)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	
	pup_client_lock(self);
	GIcon *icon = pup_icon_string_to_icon(g_strdup(self->holder->icon_name));
	pup_client_unlock(self);
	
	return icon;
}

gboolean pup_client_volume_can_mount(GVolume *volume)
{
	gboolean res;
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	pup_client_lock(self);

	res = (PUP_VOLUME(self->holder)->flags & PUP_VOLUME_IS_MOUNTABLE)
		? TRUE : FALSE;

	pup_client_unlock(self);

	return res;
}

gboolean pup_client_volume_can_eject(GVolume *volume)
{
	gboolean res;
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	pup_client_lock(self);

	res = (PUP_VOLUME(self->holder)->flags & PUP_VOLUME_CAN_EJECT)
		? TRUE : FALSE;

	pup_client_unlock(self);

	return res;
}

void pup_client_volume_eject_w_operation(GVolume *volume,
                                         GMountUnmountFlags flags,
                                         GMountOperation *mount_operation,
                                         GCancellable *cancellable,
                                         GAsyncReadyCallback callback,
                                         gpointer user_data)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	pup_client_lock(self);
	pup_client_monitor_start_operation
		(self->monitor, self->holder,
		 "eject", NULL, mount_operation,
		 g_simple_async_result_new(G_OBJECT(self), callback, user_data,
		                           pup_client_volume_eject));
	pup_client_unlock(self);
}

void pup_client_volume_eject(GVolume *volume,
                             GMountUnmountFlags flags,
                             GCancellable *cancellable,
                             GAsyncReadyCallback callback,
                             gpointer user_data)
{
	pup_client_volume_eject_w_operation(volume, flags, NULL, cancellable,
	                                    callback, user_data);
}

gboolean pup_client_volume_should_automount(GVolume *volume)
{
	gboolean res;
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	pup_client_lock(self);

	res = (PUP_VOLUME(self->holder)->flags & PUP_VOLUME_SHOULD_AUTOMOUNT)
		? TRUE : FALSE;

	pup_client_unlock(self);

	return res;
}

const gchar *pup_client_volume_get_sort_key(GVolume *volume)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(volume);
	
	pup_client_lock(self);
	const gchar *ret = g_intern_string(self->holder->sysname);
	pup_client_unlock(self);
	
	return ret;
}

