/*
 * icon.c or icon.h
 * Icon corresponding to a volume
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
//icon.c
#include "common.h"
#else
//icon.h

typedef enum 
{
	ICON_UP = 0,
	ICON_OVER,
	ICON_DOWN
} IconState;

typedef enum 
{
	ICON_UMOUNT = 0,
	ICON_EJECT,
	ICON_NO_UMOUNT,
	ICON_DISABLED
} IconMntdState;

typedef struct
{
	GtkButton parent;

	GVolume *volume;
	Settings *settings;

	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *mnt_indicator;
	GtkWidget *image;
	
	GtkWidget *menu;
	GtkWidget *mount_item;
	GtkWidget *umount_item;
	GtkWidget *eject_item;
	GtkWidget *settings_item;
	
	IconState state;
	IconMntdState mntd_state;
	gboolean indic_dn;
} Icon;

typedef struct
{
	GtkButtonClass parent;

} IconClass;

//FILE_HEADER_SUBST:gobject_macro_gen ICON Icon icon
#define TYPE_ICON (icon_get_type())
#define ICON(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), TYPE_ICON, Icon))
#define IS_ICON(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), TYPE_ICON))
#define ICON_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), TYPE_ICON, IconClass))
#define IS_ICON_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), TYPE_ICON))
#define ICON_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), TYPE_ICON, IconClass))
#define ICON_NAME "Icon"

#endif

/*Everything beyond this is automatically generated*/
void icon_mnt_indic_clicked(Icon *self);
gboolean icon_check_mouse_on_indicator(Icon *self);
void icon_update(Icon *self);
void icon_set_volume(Icon *self,GVolume *volume);
void icon_handle_changed(Settings *settings,gint hints,Icon *self);
void icon_launch_settings(Icon *self);
void icon_eject(Icon *self);
void icon_umount(Icon *self);
void icon_mount(Icon *self);
void icon_clicked(Icon *self);
gboolean icon_leave(Icon *self,GdkEventCrossing *event);
gboolean icon_enter(Icon *self,GdkEventCrossing *event);
gboolean icon_button_release(Icon *self,GdkEventButton *event);
gboolean icon_button_press(Icon *self,GdkEventButton *event);
gboolean icon_expose(GtkWidget *widget,GdkEventExpose *expose_event);
void icon_finalize(GObject *instance);
#if !(!defined(COMMON_H_INSIDE))
GType icon_get_type();
#endif
#if !defined(COMMON_H_INSIDE)
G_DEFINE_TYPE(Icon,icon,GTK_TYPE_BUTTON);
#endif
