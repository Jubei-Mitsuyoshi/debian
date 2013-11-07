//mount.c or mount.h
//GMount class implementation: PupMount

#ifndef PUP_VM_H_INSIDE
//mount.c
#	include "common.h"

#else // !PUP_VM_H_INSIDE
//mount.h

struct _PupMount
{
	GObject parent;

	PupClientVolume *holder;
};

typedef struct
{
	GObjectClass parent;

} PupMountClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_MOUNT PupMount pup_mount pup

#endif //PUP_VM_H_INSIDE

//FILE_HEADER_END

#ifndef PUP_VM_H_INSIDE
G_DEFINE_DYNAMIC_TYPE_EXTENDED(PupMount, pup_mount, G_TYPE_OBJECT, 0, 
                               G_IMPLEMENT_INTERFACE_DYNAMIC(G_TYPE_MOUNT, pup_mount_init_iface)
                               );
#else
GType pup_mount_get_type(void);
#endif

//construction and destruction

static void pup_mount_class_init(PupMountClass *klass)
{
	GObjectClass *basic_class = G_OBJECT_CLASS(klass);

	basic_class->finalize = pup_mount_finalize;

	
}

static void pup_mount_class_finalize(PupMountClass *klass)
{
	
}

static void pup_mount_init(PupMount *self)
{

}


void pup_mount_finalize(GObject *instance)
{
	PupMount *self = PUP_MOUNT(instance);

	self->holder->mount = NULL;
	//g_object_unref(self->holder);

	G_OBJECT_CLASS(pup_mount_parent_class)->finalize(instance);
}

void pup_mount_register(GIOModule *module)
{
	pup_mount_register_type(G_TYPE_MODULE(module));
}

void pup_mount_init_iface(GMountIface *interface)
{
	interface->get_root = pup_mount_get_root;
	interface->get_name = pup_mount_get_name;
	interface->get_uuid = pup_mount_get_uuid;
	interface->get_volume = pup_mount_get_volume;
	interface->get_drive = pup_mount_get_drive;
	interface->can_unmount = pup_mount_can_umount;
	interface->can_eject = pup_mount_can_eject;
	interface->unmount = pup_mount_umount;
	interface->unmount_finish
		= (gboolean (*)(GMount *, GAsyncResult *, GError **))
		pup_volume_monitor_generic_finish;
	interface->eject = pup_mount_eject;
	interface->eject_finish
		= (gboolean (*)(GMount *, GAsyncResult *, GError **))
		pup_volume_monitor_generic_finish;
	interface->get_icon = pup_mount_get_icon;
	//unmount_with_operation and eject_with_operation, added since glib 2.22
#if GLIB_CHECK_VERSION(2, 22, 0)
	interface->unmount_with_operation = pup_mount_umount_w_operation;
	interface->unmount_with_operation_finish
		= (gboolean (*)(GMount *, GAsyncResult *, GError **))
		pup_volume_monitor_generic_finish;
	interface->eject_with_operation = pup_mount_eject_w_operation;
	interface->eject_with_operation_finish
		= (gboolean (*)(GMount *, GAsyncResult *, GError **))
		pup_volume_monitor_generic_finish;
#endif //GLIB_CHECK_VERSION(2, 22, 0)

	//sorting key, added since glib 2.32
#if GLIB_CHECK_VERSION(2, 32, 0)
	interface->get_sort_key = pup_mount_get_sort_key;
#endif //GLIB_CHECK_VERSION(2, 32, 0)		
}

//Private methods

PupMount *pup_mount_get(PupClientVolume *parent)
{
	if (! parent->mount)
	{
		parent->mount = PUP_MOUNT(g_object_new(PUP_TYPE_MOUNT, NULL));
		parent->mount->holder = parent;
		//g_object_ref(parent);
	}

	return parent->mount;
}

//Public methods

gchar *pup_mount_get_name(GMount *mount)
{
	PupMount *self = PUP_MOUNT(mount);

	return pup_client_volume_get_name(G_VOLUME(self->holder));
}

gchar *pup_mount_get_uuid(GMount *mount)
{
	PupMount *self = PUP_MOUNT(mount);

	return pup_client_volume_get_uuid(G_VOLUME(self->holder));
}

GVolume *pup_mount_get_volume(GMount *mount)
{
	PupMount *self = PUP_MOUNT(mount);

	return G_VOLUME(g_object_ref(self->holder));
}

GDrive *pup_mount_get_drive(GMount *mount)
{
	GVolume *volume = pup_mount_get_volume(mount);
	return volume ? pup_client_volume_get_drive(volume) : NULL;
}

GFile *pup_mount_get_root(GMount *mount)
{
	PupMount *self = PUP_MOUNT(mount);
	pup_client_lock(PUP_CLIENT_DEVICE(self->holder));
	gchar *mntpnt = PUP_VOLUME(PUP_CLIENT_DEVICE(self->holder)->holder)->mntpnt;
	GFile *file = mntpnt ? g_file_new_for_path(mntpnt) : NULL;
	pup_client_unlock(PUP_CLIENT_DEVICE(self->holder));
	return file;
}

GIcon *pup_mount_get_icon(GMount *mount)
{
	GVolume *volume = pup_mount_get_volume(mount);
	if (! volume) return NULL;
	return pup_client_volume_get_icon(volume);
}

gboolean pup_mount_can_umount(GMount *mount)
{
	PupMount *self = PUP_MOUNT(mount);
	pup_client_lock(PUP_CLIENT_DEVICE(self->holder));
	gboolean stat
		= PUP_VOLUME(PUP_CLIENT_DEVICE(self->holder)->holder)->flags & PUP_VOLUME_MNTD_SYSTEM
		? FALSE : TRUE;

	pup_client_unlock(PUP_CLIENT_DEVICE(self->holder));
	return stat;
}

gboolean pup_mount_can_eject(GMount *mount)
{
	GVolume *volume = pup_mount_get_volume(mount);
	if (! volume) return FALSE;
	return pup_client_volume_can_eject(volume);
}

void pup_mount_umount_w_operation(GMount *mount, GMountUnmountFlags flags,
                                  GMountOperation *mount_operation,
                                  GCancellable *cancellable,
                                  GAsyncReadyCallback callback,
                                  gpointer user_data)
{
	PupMount *self = PUP_MOUNT(mount);
	PupClientDevice *dev = PUP_CLIENT_DEVICE(self->holder);
	pup_client_lock(dev);
	pup_client_monitor_start_operation
		(dev->monitor, dev->holder,
		 "umount", NULL, mount_operation,
		 g_simple_async_result_new(G_OBJECT(self), callback, user_data,
		                           pup_mount_umount));
	pup_client_unlock(dev);
}

void pup_mount_umount(GMount *mount, GMountUnmountFlags flags,
                      GCancellable *cancellable, GAsyncReadyCallback callback,
                      gpointer user_data)
{
	pup_mount_umount_w_operation(mount, flags, NULL, cancellable, 
	                             callback, user_data);
}

void pup_mount_eject_w_operation(GMount *mount, GMountUnmountFlags flags,
                                 GMountOperation *mount_operation,
                                 GCancellable *cancellable,
                                 GAsyncReadyCallback callback,
                                 gpointer user_data)
{
	PupMount *self = PUP_MOUNT(mount);
	PupClientDevice *dev = PUP_CLIENT_DEVICE(self->holder);
	pup_client_lock(dev);
	pup_client_monitor_start_operation
		(dev->monitor, dev->holder,
		 "eject", NULL, mount_operation,
		 g_simple_async_result_new(G_OBJECT(self), callback, user_data,
		                           pup_mount_umount));
	pup_client_unlock(dev);
}

void pup_mount_eject(GMount *mount, GMountUnmountFlags flags,
                     GCancellable *cancellable, GAsyncReadyCallback callback,
                     gpointer user_data)
{
	pup_mount_eject_w_operation(mount, flags, NULL, cancellable, callback,
	                            user_data);
}

const gchar *pup_mount_get_sort_key(GMount *mount)
{
	PupMount *self = PUP_MOUNT(mount);

	return pup_client_volume_get_sort_key(G_VOLUME(self->holder));
}
