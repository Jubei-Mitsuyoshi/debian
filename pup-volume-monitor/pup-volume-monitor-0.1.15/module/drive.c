//drive.c or drive.h
//GDrive class implementation: PupClientDrive

#ifndef PUP_VM_H_INSIDE
//drive.c
#	include "common.h"

#else // !PUP_VM_H_INSIDE
//drive.h

typedef struct
{
	PupClientDevice parent;

} PupClientDrive;

typedef struct
{
	PupClientDeviceClass parent;

} PupClientDriveClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_CLIENT_DRIVE PupClientDrive pup_client_drive pup

#endif //PUP_VM_H_INSIDE

//FILE_HEADER_END



#ifndef PUP_VM_H_INSIDE

G_DEFINE_DYNAMIC_TYPE_EXTENDED(PupClientDrive, pup_client_drive, PUP_TYPE_CLIENT_DEVICE, 0, 
                               G_IMPLEMENT_INTERFACE_DYNAMIC(G_TYPE_DRIVE, pup_client_drive_init_iface)
                               );
#else
GType pup_client_drive_get_type(void);
#endif

//construction and destruction

static void pup_client_drive_class_init(PupClientDriveClass *klass)
{
	GObjectClass *basic_class = G_OBJECT_CLASS(klass);

	basic_class->finalize = pup_client_drive_finalize;

}

static void pup_client_drive_class_finalize(PupClientDriveClass *klass)
{
	
}

static void pup_client_drive_init(PupClientDrive *self)
{
	PUP_CLIENT_DEVICE(self)->iface = G_DRIVE(self);
}


void pup_client_drive_finalize(GObject *instance)
{
	//PupClientDrive *self = PUP_DRIVE(instance);

	G_OBJECT_CLASS(pup_client_drive_parent_class)->finalize(instance);
}

void pup_client_drive_register(GIOModule *module)
{
	pup_client_drive_register_type(G_TYPE_MODULE(module));
}

void pup_client_drive_init_iface(GDriveIface *interface)
{
	interface->get_name = pup_client_drive_get_name;
	interface->has_volumes = pup_client_drive_has_volumes;
	interface->get_volumes = pup_client_drive_get_volumes;
	interface->is_media_removable = pup_client_drive_is_removable;
	interface->has_media = pup_client_drive_has_media;
	interface->is_media_check_automatic = pup_client_drive_is_media_check_automatic;
	interface->can_eject = pup_client_drive_can_eject;
	interface->eject = pup_client_drive_eject;
	interface->eject_finish
		= (gboolean (*)(GDrive *, GAsyncResult *, GError **))
		pup_volume_monitor_generic_finish;
	interface->get_identifier = pup_client_drive_get_identifier;
	interface->enumerate_identifiers = pup_client_drive_enum_identifiers;
	interface->get_icon = pup_client_drive_get_icon;
	
	//eject_with_operation, added since glib 2.22
#if GLIB_CHECK_VERSION(2, 22, 0)
	interface->eject_with_operation = pup_client_drive_eject_w_operation;
	interface->eject_with_operation_finish
		= (gboolean (*)(GDrive *, GAsyncResult *, GError **))
		pup_volume_monitor_generic_finish;
#endif //GLIB_CHECK_VERSION(2, 22, 0)
	
	//sorting key, added since glib 2.32
#if GLIB_CHECK_VERSION(2, 32, 0)
	interface->get_sort_key = pup_client_drive_get_sort_key;
#endif //GLIB_CHECK_VERSION(2, 32, 0)
}

//Private methods

GVolume *pup_client_drive_next_volume(PupClientMonitor *monitor,
                                      const gchar *sysname, GHashTableIter *iter)
{
	gpointer key;
	PupVolume *data = NULL;

	while (g_hash_table_iter_next(iter, &key, (gpointer *) &data))
	{
		if (g_strcmp0(data->drv_sysname, sysname) == 0)
			break;
		else
			data = NULL;
	}
	if (data)
		return G_VOLUME(pup_client_device_get_from_device(monitor,
		                                                  PUP_DEVICE(data)));
	else
		return NULL;
}

//Public methods

gchar *pup_client_drive_get_name(GDrive *drive)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(drive);
	
	pup_client_lock(self);
	gchar *ret = g_strdup(self->holder->display_name);
	pup_client_unlock(self);
	
	return ret;
}

gboolean pup_client_drive_has_volumes(GDrive *drive)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(drive);
	
	pup_client_lock(self);

	GHashTableIter iter;
	g_hash_table_iter_init(&iter,PUP_VM_MONITOR(self->monitor)->volumes);
	gboolean ret = pup_client_drive_next_volume(self->monitor,
	                                            self->holder->sysname,
	                                            &iter) ? TRUE : FALSE;
	pup_client_unlock(self);
	
	return ret;
}

GList *pup_client_drive_get_volumes(GDrive *drive)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(drive);
	
	pup_client_lock(self);
	GList *list = NULL;
	GVolume *onevol;
	GHashTableIter iter;
	g_hash_table_iter_init(&iter,PUP_VM_MONITOR(self->monitor)->volumes);
	while ((onevol = pup_client_drive_next_volume(self->monitor,
	                                            self->holder->sysname,
	                                            &iter)))
	{
		list = g_list_append(list, onevol);
		g_object_ref(onevol);
	}
	pup_client_unlock(self);

	return list;
}

gboolean pup_client_drive_is_removable(GDrive *drive)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(drive);
	return (PUP_DRIVE(self->holder)->flags & PUP_DRIVE_REMOVABLE)
		? TRUE : FALSE;
}

gboolean pup_client_drive_has_media(GDrive *drive)
{
	return pup_client_drive_has_volumes(drive);
}

gboolean pup_client_drive_is_media_check_automatic(GDrive *drive)
{
	return TRUE;
}

gboolean pup_client_drive_can_eject(GDrive *drive)
{
	gboolean res;
	PupClientDevice *self = PUP_CLIENT_DEVICE(drive);
	pup_client_lock(self);

	res = (PUP_DRIVE(self->holder)->flags & PUP_DRIVE_CAN_EJECT)
		? TRUE : FALSE;

	pup_client_unlock(self);

	return res;
}

void pup_client_drive_eject_w_operation(GDrive *drive,
                                        GMountUnmountFlags flags,
                                        GMountOperation *mount_operation,
                                        GCancellable *cancellable,
                                        GAsyncReadyCallback callback,
                                        gpointer user_data)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(drive);
	pup_client_lock(self);
	pup_client_monitor_start_operation
		(self->monitor, self->holder,
		 "eject", NULL, mount_operation,
		 g_simple_async_result_new(G_OBJECT(self), callback, user_data,
		                           pup_client_volume_mount));
	pup_client_unlock(self);
}

void pup_client_drive_eject(GDrive *drive, GMountUnmountFlags flags,
                            GCancellable *cancellable,
                            GAsyncReadyCallback callback, gpointer user_data)
{
	pup_client_drive_eject_w_operation(drive, flags, NULL, cancellable,
	                                   callback, user_data);
}

gboolean pup_client_drive_can_poll_for_media(GDrive *drive)
{
	return FALSE;
}

gchar **pup_client_drive_enum_identifiers(GDrive *drive)
{
	return g_strsplit_set("", "|", 1);
}

gchar *pup_client_drive_get_identifier(GDrive *drive, const gchar *id)
{
	return NULL;
}

GIcon *pup_client_drive_get_icon(GDrive *drive)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(drive);
	
	pup_client_lock(self);
	GIcon *icon = pup_icon_string_to_icon(g_strdup(self->holder->icon_name));
	pup_client_unlock(self);
	
	return icon;
}

const gchar *pup_client_drive_get_sort_key(GDrive *drive)
{
	PupClientDevice *self = PUP_CLIENT_DEVICE(drive);
	
	pup_client_lock(self);
	const gchar *ret = g_intern_string(self->holder->sysname);
	pup_client_unlock(self);
	
	return ret;
}
