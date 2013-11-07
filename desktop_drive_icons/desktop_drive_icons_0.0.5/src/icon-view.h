/*
 * icon_view.c or icon_view.h
 * Desklet that displays icons on desktop
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
//icon_view.c
#include "common.h"
#else
//icon_view.h

typedef struct
{
	GtkContainer parent;

	GVolumeMonitor *monitor;
	Settings *settings;
	gint changed;

	GQueue icons;
} IconView;

typedef struct
{
	GtkContainerClass parent;

} IconViewClass;

//FILE_HEADER_SUBST:gobject_macro_gen ICON_VIEW IconView icon_view
#define TYPE_ICON_VIEW (icon_view_get_type())
#define ICON_VIEW(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), TYPE_ICON_VIEW, IconView))
#define IS_ICON_VIEW(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), TYPE_ICON_VIEW))
#define ICON_VIEW_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), TYPE_ICON_VIEW, IconViewClass))
#define IS_ICON_VIEW_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), TYPE_ICON_VIEW))
#define ICON_VIEW_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), TYPE_ICON_VIEW, IconViewClass))
#define ICON_VIEW_NAME "IconView"

#endif

/*Everything beyond this is automatically generated*/
void icon_view_find_icon_size(IconView *self,GtkRequisition *iconsize,gint *num_icons,gboolean use_signal);
void icon_view_changed(IconView *self,gint hints);
void icon_view_add_icon_for_volume(IconView *self,GVolume *volume);
void icon_view_foreach(GtkContainer *container,gboolean include_internals,GtkCallback callback,gpointer callback_data);
void icon_view_remove_widget(GtkContainer *container,GtkWidget *widget);
void icon_view_add_widget(GtkContainer *container,GtkWidget *widget);
gboolean icon_view_expose_event(GtkWidget *widget,GdkEventExpose *event);
void icon_view_size_allocate(GtkWidget *widget,GtkAllocation *allocation);
void icon_view_size_request(GtkWidget *widget,GtkRequisition *requisition);
#if !(!defined(COMMON_H_INSIDE))
GType icon_view_get_type();
#endif
#if !defined(COMMON_H_INSIDE)
G_DEFINE_TYPE(IconView,icon_view,GTK_TYPE_CONTAINER);
#endif
