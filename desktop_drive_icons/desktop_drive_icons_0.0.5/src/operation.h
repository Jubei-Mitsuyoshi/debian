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
#define TYPE_OPERATION (operation_get_type())
#define OPERATION(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), TYPE_OPERATION, Operation))
#define IS_OPERATION(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), TYPE_OPERATION))
#define OPERATION_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), TYPE_OPERATION, OperationClass))
#define IS_OPERATION_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), TYPE_OPERATION))
#define OPERATION_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), TYPE_OPERATION, OperationClass))
#define OPERATION_NAME "Operation"

#endif

/*Everything beyond this is automatically generated*/
void operation_finish_func(GObject *object,GAsyncResult *res,gpointer data);
void operation_close_volume(GVolume *volume,gboolean *error);
void operation_open_volume(GVolume *volume,gboolean *error);
GMountOperation *operation_get_mnt_operation(Operation *self);
Operation *operation_new(gpointer finish_func,gboolean open_volume,const gchar *error_fmt,...);
void operation_finalize(GObject *instance);
#if !(!defined(COMMON_H_INSIDE))
GType operation_get_type();
#endif
#if !defined(COMMON_H_INSIDE)
G_DEFINE_TYPE(Operation,operation,G_TYPE_OBJECT);
#endif
