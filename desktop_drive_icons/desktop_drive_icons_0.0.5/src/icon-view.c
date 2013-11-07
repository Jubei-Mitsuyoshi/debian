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

#endif

//FILE_HEADER_END

#ifndef COMMON_H_INSIDE
G_DEFINE_TYPE(IconView, icon_view, GTK_TYPE_CONTAINER);
#else
GType icon_view_get_type();
#endif

static void icon_view_class_init(IconViewClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
	GtkContainerClass *container_class = GTK_CONTAINER_CLASS(klass);
	
	widget_class->size_request  = icon_view_size_request;
	widget_class->size_allocate = icon_view_size_allocate;
	widget_class->expose_event  = icon_view_expose_event;
	
	container_class->add        = icon_view_add_widget;
	container_class->remove     = icon_view_remove_widget;
	container_class->forall     = icon_view_foreach;
}

static gint volume_compare_func (gconstpointer v1, gconstpointer v2)
{
#if GLIB_CHECK_VERSION(2, 32, 0)
	const char *k1, *k2;
	k1 = g_volume_get_sort_key(G_VOLUME(v1));
	k2 = g_volume_get_sort_key(G_VOLUME(v2));
	if (k1 && k2)
		return strcmp(k1, k2);
	else
#endif //GLIB_CHECK_VERSION(2, 32, 0)
		return 0;
}

static void icon_view_init(IconView *self)
{
	GList *volumes, *iter;
	
	self->settings = SETTINGS(g_object_new(TYPE_SETTINGS, NULL));
	self->monitor = g_volume_monitor_get();
	
	//GTK_NO_WINDOW widget
	gtk_widget_set_has_window(GTK_WIDGET(self), FALSE);

	//Volume monitor stuff
	g_queue_init(&(self->icons));
	volumes = g_volume_monitor_get_volumes(self->monitor);
	volumes = g_list_sort(volumes, volume_compare_func);
	for (iter = volumes; iter; iter = iter->next)
	{
		icon_view_add_icon_for_volume(self, G_VOLUME(iter->data));
	}
	g_list_free(volumes);
	
	//Events
	g_signal_connect_swapped(self->monitor, "volume-added",
	                         G_CALLBACK(icon_view_add_icon_for_volume), self);
	g_signal_connect_swapped(self->settings, "changed",
	                         G_CALLBACK(icon_view_changed), self);
}

//Position and size allocation for icons:

//Size requisition
void icon_view_find_icon_size(IconView *self, GtkRequisition *iconsize,
                              gint *num_icons, gboolean use_signal)
{
	GtkRequisition per_icon;
	GList *iter;
	gint count = 0;
	
	iconsize->width = iconsize->height = 0;
	for (iter = self->icons.head; iter; iter = iter->next)
	{
		GtkWidget *icon = GTK_WIDGET(iter->data);
		
		if (use_signal)
			gtk_widget_size_request(icon, &per_icon);
		else
			gtk_widget_get_child_requisition(icon, &per_icon);
		
		iconsize->width = MAX(iconsize->width, per_icon.width);
		iconsize->height = MAX(iconsize->height, per_icon.height);
		
		count++;
	}
	
	if (num_icons) *num_icons = count;
}

void icon_view_size_request(GtkWidget *widget, GtkRequisition *requisition)
{
	IconView *self = ICON_VIEW(widget);
	
	GtkRequisition per_icon, res;
	gint num_icons, groups, per_group;
	GdkScreen *screen;
	gint scrdim;
	
	//Chain up
	GTK_WIDGET_CLASS(icon_view_parent_class)->size_request(widget, requisition);
	
	//Check whether there are any icon
	if (! self->icons.head)
		return;
	
	//Find largest requisition
	icon_view_find_icon_size(self, &per_icon, &num_icons, TRUE);
	
	//Now calculate...
	screen = gtk_widget_get_screen(GTK_WIDGET(self));
	groups = 1;
	if (screen)
	{
		if (self->settings->vertical)
		{
			scrdim = gdk_screen_get_height(screen);
			if (per_icon.height > scrdim)
				per_icon.height = scrdim;
			per_group = scrdim / per_icon.height;
		}
		else
		{
			scrdim = gdk_screen_get_width(screen);
			if (per_icon.width > scrdim)
				per_icon.width = scrdim;
			per_group = scrdim / per_icon.width;
		}
		
		groups = num_icons / per_group;
		if (! groups)
		{
			groups = 1;
			per_group = num_icons;
		}	
		else if (num_icons % per_group)
			groups++;
		
		if (self->settings->vertical)
		{
			res.width = per_icon.width * groups;
			res.height = per_icon.height * per_group;
		}
		else
		{
			res.width = per_icon.width * per_group;
			res.height = per_icon.height * groups;
		}
	}
	else
	{
		if (self->settings->vertical)
		{
			res.width = per_icon.width;
			res.height = per_icon.height * num_icons;
		}
		else
		{
			res.width = per_icon.width * num_icons;
			res.height = per_icon.height;
		}
	}
	
	
	/*
	printf("groups = %d, per_group = %d, "
	       "per_icon.width = %d,  per_icon.height = %d, "
	       "res.width = %d, res.height = %d\n",
	       groups, per_group, per_icon.width, per_icon.height, res.width, res.height);
	*/
	
	//Add space for frame
	if (self->settings->show_frame)
	{
		GtkStyle *style = gtk_widget_get_style(GTK_WIDGET(self));
		if (style)
		{
			res.width += 4 * style->xthickness;
			res.height += 4 * style->ythickness;
		}
	}
	
	//Return
	*requisition = res;
}

//Allocation
void icon_view_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
	IconView *self = ICON_VIEW(widget);
	
	GtkAllocation child_alloc;
	GtkRequisition child_req;
	gint num_icons, num_groups, per_group;
	gint xinit, yinit, i, j, avail_width, avail_height;
	GList *iter;
	gboolean vertical = self->settings->vertical;
	
	//chain up
	GTK_WIDGET_CLASS(icon_view_parent_class)->size_allocate(widget, allocation);
	
	//Check whether there are any icon
	if (! self->icons.head)
		return;
	
	//Get icon's desired size
	icon_view_find_icon_size(self, &child_req, &num_icons, FALSE);
	
	//Calculate available area
	xinit = allocation->x;
	yinit = allocation->y;
	avail_width = allocation->width;
	avail_height = allocation->height;
	if (self->settings->show_frame)
	{
		GtkStyle *style = gtk_widget_get_style(GTK_WIDGET(self));
		if (style)
		{
			xinit += 2 * style->xthickness;
			yinit += 2 * style->ythickness;
			avail_width -= 4 * style->xthickness;
			avail_height -= 4 * style->ythickness;
		}
	}
	
	//Calculate distribution
	if (vertical)
	{
		per_group = avail_height / child_req.height;
		if (! per_group)
		{
			per_group = 1;
			child_req.height = avail_height;
		}
	}
	else
	{
		per_group = allocation->width / child_req.width;
		if (! per_group)
		{
			per_group = 1;
			child_req.width = avail_width;
		}
	}
	num_groups = num_icons / per_group;
	if (num_icons % per_group)
		num_groups++;
	
	//Calculate icon size
	if (vertical)
	{
		child_alloc.width = MIN(child_req.width,
		                        avail_width / num_groups);
		child_alloc.height = child_req.height;
	}
	else
	{
		child_alloc.width = child_req.width;
		child_alloc.height = MIN(child_req.height,
		                         avail_height / num_groups);
	}
	
	//Now allocate
	iter = self->icons.head;
	for (i = 0; iter && (i < num_groups); i++)
	{
		int i_mod;
		if (self->settings->reverse_pack)
			i_mod = num_groups - i - 1;
		else
			i_mod = i;
		for (j = 0; iter && (j < per_group); j++, iter = iter->next)
		{
			GtkWidget *icon = GTK_WIDGET(iter->data);
			child_alloc.x = xinit + ((vertical ? i_mod : j) * child_alloc.width);
			child_alloc.y = xinit + ((vertical ? j : i_mod) * child_alloc.height);
			
			gtk_widget_size_allocate(icon, &child_alloc);
		}
	}
	
}

//Container tasks

void icon_view_add_widget(GtkContainer *container, GtkWidget *widget)
{
	IconView *self = ICON_VIEW(container);
	
	gtk_widget_set_parent(widget, GTK_WIDGET(self));
	
	g_queue_push_tail(&(self->icons), widget);
	
	gtk_widget_queue_resize(GTK_WIDGET(self));
}

void icon_view_remove_widget(GtkContainer *container, GtkWidget *widget)
{
	IconView *self = ICON_VIEW(container);
	
	if (! g_queue_remove(&(self->icons), widget))
	{
		fprintf(stderr, "icon_view_remove_widget() handles GtkCOntainer::remove: "
		                "Widget %p not found in the IconView %p.\n",
		        widget, container);
	}
	
	gtk_widget_unparent(widget);
	
	gtk_widget_queue_resize(GTK_WIDGET(self));
}

void icon_view_foreach(GtkContainer	 *container,
                       gboolean	  include_internals,
                       GtkCallback	  callback,
                       gpointer	  callback_data)
{
	IconView *self = ICON_VIEW(container);
	
	GList *iter;
	for (iter = self->icons.head; iter; iter = iter->next)
		callback(GTK_WIDGET(iter->data), callback_data);
}

//Drawing tasks
gboolean icon_view_expose_event(GtkWidget *widget, GdkEventExpose *event)
{
	IconView *self = ICON_VIEW(widget);
	GList *iter;
	
	GdkWindow *win = gtk_widget_get_window(GTK_WIDGET(self));
	if (!win)
	{
		g_critical("Expose event on widget with widget->window == NULL");
		return FALSE;
	}
	GtkStyle *style = gtk_widget_get_style(GTK_WIDGET(self));
	GtkAllocation allocation;
	gtk_widget_get_allocation(GTK_WIDGET(self), &allocation);
	
	//Draw frame
	if (self->settings->show_frame)
	{	
		
		gtk_paint_shadow(style, win,
		                 GTK_STATE_NORMAL, GTK_SHADOW_ETCHED_IN,
		                 NULL, GTK_WIDGET(self), NULL,
		                 allocation.x + style->xthickness,
		                 allocation.y + style->ythickness,
		                 allocation.width - (2 * style->xthickness),
		                 allocation.height - (2 * style->ythickness));
		
		/*
		cairo_t *cr = gdk_cairo_create(win);
		
		gdk_cairo_set_source_color(cr, &(style->bg[3]));
		cairo_reset_clip(cr);
		cairo_rectangle(cr,
		                allocation.x + style->xthickness,
			            allocation.y + style->ythickness,
			            allocation.width - (2 * style->xthickness),
			            allocation.height - (2 * style->ythickness));
		cairo_stroke(cr);
		
		cairo_destroy(cr);
		*/
	}
	
	//Draw children
	for (iter = self->icons.head; iter; iter = iter->next)
	{
		GtkWidget *child = GTK_WIDGET(iter->data);
		
		gtk_container_propagate_expose(GTK_CONTAINER(self), child, event);
	}
	
	return FALSE;
}

//Volume monitor events
void icon_view_add_icon_for_volume(IconView *self, GVolume *volume)
{
	GtkWidget *icon;
	
	icon = GTK_WIDGET(g_object_new(TYPE_ICON, NULL));
	icon_set_volume(ICON(icon), volume);
	
	gtk_container_add(GTK_CONTAINER(self), icon);
	
	gtk_widget_show(icon);
}

//Settings change events
void icon_view_changed(IconView *self, gint hints)
{
	if (hints & SETTINGS_UI)
		gtk_widget_queue_resize(GTK_WIDGET(self));
	else if (hints & SETTINGS_THEME)
		gtk_widget_queue_draw(GTK_WIDGET(self));
}



