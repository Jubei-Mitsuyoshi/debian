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

#endif

//FILE_HEADER_END

#ifndef COMMON_H_INSIDE
G_DEFINE_TYPE(Icon, icon, GTK_TYPE_BUTTON);
#else
GType icon_get_type();
#endif

static void icon_class_init(IconClass *klass)
{
	GObjectClass *g_class = G_OBJECT_CLASS(klass);
	g_class->finalize = icon_finalize;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
	widget_class->expose_event = icon_expose;
	//widget_class->realize = icon_realize;
}

static void icon_init(Icon *self)
{
	//Get settings
	self->settings = SETTINGS(g_object_new(TYPE_SETTINGS, NULL));
	//Create all icon stuff
	gtk_container_set_border_width(GTK_CONTAINER(self), 2);
	
	self->table = gtk_table_new(3, 4, FALSE);
	gtk_container_add(GTK_CONTAINER(self), self->table);
	
	self->label = GTK_WIDGET(g_object_new(TYPE_ICON_LABEL, NULL));
	gtk_widget_set_size_request(self->label, 70, -1);
	
	gtk_table_attach(GTK_TABLE(self->table), self->label, 0, 4, 2, 3,
	                 0, 0, 0, 0);
	
	self->image = gtk_image_new();
	gtk_table_attach(GTK_TABLE(self->table), self->image, 1, 3, 0, 2,
	                 0, 0, 8, 8);
	
	self->mnt_indicator = gtk_image_new_from_icon_name("media-eject",
	                                                   GTK_ICON_SIZE_MENU);
	gtk_table_attach(GTK_TABLE(self->table), self->mnt_indicator, 2, 3, 0, 1,
	                 0, 0, 0, 0);
	gtk_widget_set_no_show_all(self->mnt_indicator, TRUE);
	
	gtk_widget_set_has_tooltip(GTK_WIDGET(self), TRUE);

	//Create menu
	self->menu = gtk_menu_new();

	self->mount_item = gtk_menu_item_new_with_label(_("Mount volume"));
	gtk_menu_shell_append(GTK_MENU_SHELL(self->menu), self->mount_item);

	self->umount_item = gtk_menu_item_new_with_label(_("Unmount volume"));
	gtk_menu_shell_append(GTK_MENU_SHELL(self->menu), self->umount_item);

	self->eject_item = gtk_menu_item_new_with_label(_("Eject removable media"));
	gtk_menu_shell_append(GTK_MENU_SHELL(self->menu), self->eject_item);

	gtk_menu_shell_append(GTK_MENU_SHELL(self->menu),
	                      gtk_separator_menu_item_new());

	self->settings_item = gtk_menu_item_new_with_label(_("Settings..."));
	gtk_menu_shell_append(GTK_MENU_SHELL(self->menu), self->settings_item);

	gtk_widget_show_all(self->menu);
	gtk_widget_show_all(self->table);
	//Connect all signals
	g_signal_connect(self, "button-press-event", G_CALLBACK(icon_button_press),
	                 NULL);
	g_signal_connect(self, "button-release-event", G_CALLBACK(icon_button_release),
	                 NULL);
	g_signal_connect(self, "enter-notify-event", G_CALLBACK(icon_enter),
	                 NULL);
	g_signal_connect(self, "leave-notify-event", G_CALLBACK(icon_leave),
	                 NULL);
	g_signal_connect(self, "clicked", G_CALLBACK(icon_clicked), NULL);

	
	g_signal_connect_swapped(self->mount_item, "activate",
	                         G_CALLBACK(icon_mount), self);
	g_signal_connect_swapped(self->umount_item, "activate",
	                         G_CALLBACK(icon_umount), self);
	g_signal_connect_swapped(self->eject_item, "activate",
	                         G_CALLBACK(icon_eject), self);
	g_signal_connect_swapped(self->settings_item, "activate",
	                         G_CALLBACK(icon_launch_settings), self);
	
	g_signal_connect_object(self->settings, "changed",
	                        G_CALLBACK(icon_handle_changed), self, 0);
}

void icon_set_volume(Icon *self, GVolume *volume)
{
	if (volume) g_object_ref(volume);
	if (self->volume) g_object_unref(self->volume);
	self->volume = NULL;
	if (! volume) return;
	self->volume = volume;

	//Connect signals
	g_signal_connect_object(self->volume, "changed", G_CALLBACK(icon_update),
	                        self, G_CONNECT_SWAPPED);
	g_signal_connect_object(self->volume, "removed", G_CALLBACK(gtk_widget_destroy),
	                        self, G_CONNECT_SWAPPED);

	//Update icon
	icon_update(self);
}

void icon_update(Icon *self)
{
	//Load the icon
	GIcon *icon = g_volume_get_icon(self->volume);
	g_object_set(self->image, "gicon", icon, NULL);
	g_object_unref(icon);
	g_object_set(self->image, "pixel-size", 48, NULL);
	//Set display name and tooltip
	{
		gchar *name, *tooltip;
		GMount *mount;
		GFile *root;
		
		name = g_volume_get_name(self->volume);
		mount = g_volume_get_mount(self->volume);
		if (mount)
		{
			root = g_mount_get_root(mount);
			g_object_unref(mount);
			gchar *path = g_file_get_path(root);
			if (path)
				tooltip = g_strdup_printf(_("%s (mounted at %s)"), name, path);
			else
			{
				path = g_file_get_uri(root);
				tooltip = g_strdup_printf(_("%s (mounted at %s)"), name, path);
			}
			g_free(path);
			g_object_unref(root);
		}
		else
			tooltip = g_strdup_printf(_("%s (not mounted)"), name);
		
		icon_label_set_text(ICON_LABEL(self->label), name);
		gtk_widget_set_tooltip_text(GTK_WIDGET(self), tooltip);
		
		g_free(tooltip);
		g_free(name);
	}
	//Set mounted indicator's state
	const gchar *tooltips[] = 
	{
		N_("Click here to unmount this volume."),
		N_("Click here to eject this volume."),
		N_("This volume cannot be unmounted."),
		N_("This is a bug, please report as soon as possible")
	};
	
	gboolean can_mount = g_volume_can_mount(self->volume);
	gboolean can_eject = g_volume_can_eject(self->volume);
	gboolean is_mounted = FALSE;
	gboolean can_umount = FALSE;
	gint state;
	GMount *mount = g_volume_get_mount(self->volume);
	if (mount)
	{
		is_mounted = TRUE;
		can_umount = g_mount_can_unmount(mount);
		can_mount = FALSE;
		g_object_unref(mount);
	}

	if (is_mounted)
		if (can_umount)
			self->mntd_state = state = ICON_UMOUNT;
		else
			self->mntd_state = state = ICON_NO_UMOUNT;
	else if (can_eject)
		self->mntd_state = state = ICON_EJECT;
	else
	{
		self->mntd_state = state = ICON_DISABLED;
	}
	gtk_widget_set_tooltip_text(self->mnt_indicator,
	                            _(tooltips[state]));
	switch (state)
	{
		case ICON_UMOUNT:
		case ICON_EJECT:
			gtk_image_set_from_icon_name(GTK_IMAGE(self->mnt_indicator),
			                             "media-eject", GTK_ICON_SIZE_MENU);
			gtk_widget_show(self->mnt_indicator);
			break;
		case ICON_NO_UMOUNT:
			gtk_image_set_from_stock(GTK_IMAGE(self->mnt_indicator),
			                         GTK_STOCK_EXECUTE,
			                         GTK_ICON_SIZE_MENU);
			gtk_widget_show(self->mnt_indicator);
			break;
		default:
			gtk_widget_hide(self->mnt_indicator);
	}
	//Set menuitems' state
	gtk_widget_set_sensitive(self->mount_item, can_mount);
	gtk_widget_set_sensitive(self->umount_item, can_umount);
	gtk_widget_set_sensitive(self->eject_item, can_eject);
}

void icon_handle_changed(Settings *settings, gint hints, Icon *self)
{
	if (hints & SETTINGS_UI)
		icon_update(self);
	if (hints & SETTINGS_THEME)
		gtk_widget_queue_draw(GTK_WIDGET(self));
}

void icon_finalize(GObject *instance)
{
	Icon *self = ICON(instance);

	g_object_unref(self->volume);
	
	G_OBJECT_CLASS(icon_parent_class)->finalize(instance);
}

gboolean icon_expose(GtkWidget *widget, GdkEventExpose *expose_event)
{
	Icon *self = ICON(widget);
	
	cairo_t *cairo_ctx = NULL;
	gdouble alpha = 0.4;
	GtkStyle *style;
	switch ((gint) self->state)
	{
		case ICON_DOWN:
			alpha = 0.6;
		case ICON_OVER:
			style = gtk_widget_get_style(widget);
			//Create new cairo
			cairo_ctx = gdk_cairo_create(GDK_DRAWABLE(widget->window));
			//Set color and operator
			gdk_cairo_set_source_color(cairo_ctx, &(style->bg[3]));
			//Set clip region
			cairo_reset_clip(cairo_ctx);
			cairo_rectangle(cairo_ctx, widget->allocation.x,
			                widget->allocation.y,
			                widget->allocation.width,
			                widget->allocation.height);
			cairo_clip_preserve(cairo_ctx);
			//Create translucent overlay
			cairo_paint_with_alpha(cairo_ctx, alpha);
			//Draw bounding rectangle
			cairo_stroke(cairo_ctx);
			
			break;
	}
	
	//Chain up to draw the child
	gtk_container_propagate_expose(GTK_CONTAINER(self),
	                               gtk_bin_get_child(GTK_BIN(self)),
	                               expose_event);

	//Draw highlight over mounted indicator if pressed
	if (self->indic_dn)
	{
		if (! cairo_ctx)
		{
			cairo_ctx = gdk_cairo_create(GDK_DRAWABLE(widget->window));
			style = gtk_widget_get_style(widget);
			gdk_cairo_set_source_color(cairo_ctx, &(style->bg[3]));
		}
		cairo_reset_clip(cairo_ctx);
		cairo_rectangle(cairo_ctx, self->mnt_indicator->allocation.x,
		                self->mnt_indicator->allocation.y,
		                self->mnt_indicator->allocation.width,
		                self->mnt_indicator->allocation.height);
		cairo_clip_preserve(cairo_ctx);
		cairo_paint_with_alpha(cairo_ctx, 0.6);
		cairo_stroke(cairo_ctx);
	}
	
	if (GTK_WIDGET_VISIBLE(self->mnt_indicator))
		gtk_widget_send_expose(self->mnt_indicator, (GdkEvent *) expose_event);

	//Destroy cairo
	if (cairo_ctx) cairo_destroy(cairo_ctx);
	
	return FALSE;
}

//Maintaining state variables
gboolean icon_enter(Icon *self, GdkEventCrossing *event)
{
	self->state = ICON_OVER;
	return FALSE;
}

gboolean icon_leave(Icon *self, GdkEventCrossing *event)
{
	self->state = ICON_UP;
	self->indic_dn = FALSE;
	return FALSE;
}

gboolean icon_check_mouse_on_indicator(Icon *self)
{
	//See whether user clicked eject icon
	gint x, y, w, h, px, py;
	x = self->mnt_indicator->allocation.x;
	y = self->mnt_indicator->allocation.y;
	w = self->mnt_indicator->allocation.width;
	h = self->mnt_indicator->allocation.height;
	gdk_window_get_pointer(GTK_WIDGET(self)->window, &px, &py, NULL);
	if ((px > x) && (px < (x + w)) && (py > y) && (py < (y + h)))
		return TRUE;
	else
		return FALSE;
}

gboolean icon_button_press(Icon *self, GdkEventButton *event)
{
	if (self->mntd_state == ICON_UMOUNT || self->mntd_state == ICON_EJECT)
	{
		if (icon_check_mouse_on_indicator(self))
		{
			self->state = ICON_OVER;
			self->indic_dn = TRUE;
			gtk_widget_queue_draw(GTK_WIDGET(self));
			return TRUE;
		}
	}
	self->state = ICON_DOWN;
	return FALSE;
}

gboolean icon_button_release(Icon *self, GdkEventButton *event)
{
	self->state = ICON_OVER;
	if (self->indic_dn)
	{
		self->indic_dn = FALSE;
		gtk_widget_queue_draw(GTK_WIDGET(self));
		if (icon_check_mouse_on_indicator(self))
		{
			icon_mnt_indic_clicked(self);
		}
		return TRUE;
	}
	if (event->button == 3)
		gtk_menu_popup(GTK_MENU(self->menu), NULL, NULL, NULL, NULL, 3,
		               event->time);
	return FALSE;
}

//Volume management
void icon_mount(Icon *self)
{
	gchar *display_name = g_volume_get_name(self->volume);
	Operation *operation = operation_new(g_volume_mount_finish, FALSE,
	                                     _("Error mounting %s"), display_name);
	g_free(display_name);
	g_volume_mount(self->volume, 0, operation_get_mnt_operation(operation),
	               NULL, operation_finish_func, operation);
}

void icon_umount(Icon *self)
{
	GMount *mount = g_volume_get_mount(self->volume);	
	gchar *display_name = g_volume_get_name(self->volume);
	Operation *operation;
#if GLIB_CHECK_VERSION(2, 22, 0)	
	operation = operation_new(g_mount_unmount_with_operation_finish, FALSE,
	                                     _("Error unmounting %s"), display_name);
#else // ! GLIB_CHECK_VERSION(2, 22, 0)
	operation = operation_new(g_mount_unmount_finish, FALSE,
	                                     _("Error unmounting %s"), display_name);
#endif // GLIB_CHECK_VERSION(2, 22, 0)	           
	g_free(display_name);
	operation_close_volume(self->volume, NULL);
#if GLIB_CHECK_VERSION(2, 22, 0)
	g_mount_unmount_with_operation
			(mount, 0, operation_get_mnt_operation(operation), NULL,
			 operation_finish_func, operation);
#else // ! GLIB_CHECK_VERSION(2, 22, 0)
	g_mount_unmount(mount, 0, NULL, operation_finish_func, operation);
#endif // GLIB_CHECK_VERSION(2, 22, 0)
	g_object_unref(mount);
}

void icon_eject(Icon *self)
{
	gchar *display_name = g_volume_get_name(self->volume);
	GMount *mount;
	Operation *operation;
#if GLIB_CHECK_VERSION(2, 22, 0)
	operation = operation_new(g_volume_eject_with_operation_finish, FALSE,
	                                     _("Error ejecting %s"), display_name);
#else // ! GLIB_CHECK_VERSION(2, 22, 0)
	operation = operation_new(g_volume_eject_finish, FALSE,
	                                     _("Error ejecting %s"), display_name);
#endif // GLIB_CHECK_VERSION(2, 22, 0)	                                 
	g_free(display_name);
	
	if ((mount = g_volume_get_mount(self->volume)))
	{
		operation_close_volume(self->volume, NULL);
		g_object_unref(mount);
	}
	
#if GLIB_CHECK_VERSION(2, 22, 0)
	g_volume_eject_with_operation
			(self->volume, 0, operation_get_mnt_operation(operation), NULL,
			 operation_finish_func, operation);
#else // ! GLIB_CHECK_VERSION(2, 22, 0)
	g_volume_eject(self->volume, 0, NULL, operation_finish_func, operation);
#endif // GLIB_CHECK_VERSION(2, 22, 0)
}

void icon_clicked(Icon *self)
{
	gboolean error = FALSE;
	operation_open_volume(self->volume, &error);
	if (error)
	{
		gchar *display_name = g_volume_get_name(self->volume);
		Operation *operation = operation_new(g_volume_mount_finish, TRUE,
			                                 _("Error mounting %s"), display_name);
		g_free(display_name);
		g_volume_mount(self->volume, 0, operation_get_mnt_operation(operation),
			           NULL, operation_finish_func, operation);
	}
}

void icon_mnt_indic_clicked(Icon *self)
{
	if (self->mntd_state == ICON_UMOUNT)
	{
		icon_umount(self);
	}
	else
	{
		//Must be eject command
		icon_eject(self);
	}
}

void icon_launch_settings(Icon *self)
{
	settings_show(self->settings);
}

