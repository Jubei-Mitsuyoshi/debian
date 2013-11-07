/*
 * operation.c or operation.h
 * Managing asynchronous GIO operations
 *
 * Copyright (C) 2012 - Akash Rawal
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMMON_H_INSIDE
//operation.c
#include "common.h"
#else
//operation.h

typedef gboolean (*FinishFunc) (gpointer object, GAsyncResult *res, GError **error);

typedef struct
{
	GObject parent;

	GMountOperation *mnt_operation;
	gboolean open_volume;
	gpointer finish_func;
	gchar *error_desc;
} Operation;

typedef struct
{
	GObjectClass parent;
} OperationClass;

//FILE_HEADER_SUBST:gobject_macro_gen OPERATION Operation operation

#endif

//FILE_HEADER_END

#ifndef COMMON_H_INSIDE
G_DEFINE_TYPE(Operation, operation, G_TYPE_OBJECT);
#else
GType operation_get_type();
#endif

static void operation_class_init(OperationClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = operation_finalize;
}

static void operation_init(Operation *self)
{

}

void operation_finalize(GObject *instance)
{
	Operation *self = OPERATION(instance);

	if (self->mnt_operation) g_object_unref(self->mnt_operation);
	g_free(self->error_desc);
	
	G_OBJECT_CLASS(operation_parent_class)->finalize(instance);
}

Operation *operation_new(gpointer finish_func, gboolean open_volume,
                         const gchar *error_fmt, ...)
{
	Operation *self = OPERATION(g_object_new(TYPE_OPERATION, NULL));

	self->finish_func = finish_func;

	va_list arglist;
	va_start(arglist, error_fmt);
	self->error_desc = g_strdup_vprintf(error_fmt, arglist);

	self->open_volume = open_volume;

	return self;
}

GMountOperation *operation_get_mnt_operation(Operation *self)
{
	if (! self->mnt_operation)
		self->mnt_operation = gtk_mount_operation_new(NULL);

	return self->mnt_operation;
}

void operation_open_volume(GVolume *volume, gboolean *error)
{
	g_return_if_fail(G_IS_VOLUME(volume));

	GMount *mount = g_volume_get_mount(volume);
	if (mount)
	{
		settings_spawn_command
			(SETTINGS(g_object_new(TYPE_SETTINGS, NULL)),
			 g_mount_get_root(mount), SETTINGS_MOUNT);

		g_object_unref(mount);
	}
	else
	{
		if (error)
			*error = TRUE;
		else
			g_warning("Implementation bug: mounting successful but "
			          "g_volume_get_mount(volume) returns NULL");
	}
}

void operation_close_volume(GVolume *volume, gboolean *error)
{
	g_return_if_fail(G_IS_VOLUME(volume));

	GMount *mount = g_volume_get_mount(volume);
	if (mount)
	{
		settings_spawn_command
			(SETTINGS(g_object_new(TYPE_SETTINGS, NULL)),
			 g_mount_get_root(mount), SETTINGS_UMOUNT);

		g_object_unref(mount);
	}
	else
	{
		if (error)
			*error = TRUE;
		else
			g_warning("Implementation bug: mounting successful but "
			          "g_volume_get_mount(volume) returns NULL");
	}
}

void operation_finish_func(GObject *object, GAsyncResult *res, gpointer data)
{
	Operation *self;
	GError *error = NULL;
	g_return_if_fail((self = OPERATION(data)));

	if (! self->finish_func)
	{
		g_object_unref(self);
		return;
	}

	((FinishFunc) self->finish_func) (object, res, &error);

	if (error)
	{
		if (error->code != G_IO_ERROR_FAILED_HANDLED)
		{
			GtkWidget *dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
				                                       GTK_BUTTONS_OK, "%s",
				                                       self->error_desc);
			gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
				                                     "%s", error->message);
			g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy),
			                 NULL);
			gtk_dialog_run(GTK_DIALOG(dialog));
		}
		g_clear_error(&error);
		error = GINT_TO_POINTER(TRUE);
	}

	if ((! error) && self->open_volume)
	{
		GVolume *volume = G_VOLUME(object);
		if (volume)
		{
			operation_open_volume(volume, NULL);
		}
	}

	g_object_unref(self);
	return;
}



