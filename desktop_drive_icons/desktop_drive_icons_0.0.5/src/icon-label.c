/*
 * icon-label.c or icon-label.h
 * Label widget for desktop icons
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
//icon-label.c
#include "common.h"
#else
//icon-label.h

typedef struct
{
	GtkWidget parent;
	
	GString *text;
	GString *mod_text;
	
	Settings *settings;
	
	PangoLayout *layout;
} IconLabel;

typedef struct
{
	GtkWidgetClass parent;
} IconLabelClass;

#endif //COMMON_H_INSIDE

//FILE_HEADER_SUBST:gobject_macro_gen ICON_LABEL IconLabel icon_label

//FILE_HEADER_END

#ifndef COMMON_H_INSIDE
G_DEFINE_TYPE(IconLabel, icon_label, GTK_TYPE_WIDGET);
#else
GType icon_label_get_type();
#endif

static void icon_label_class_init(IconLabelClass *klass)
{
	GObjectClass *base_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
	
	base_class->finalize = icon_label_finalize;
	
	widget_class->size_request = icon_label_size_request;
	widget_class->size_allocate = icon_label_size_allocate;
	widget_class->expose_event = icon_label_expose_event;
}

static void icon_label_init(IconLabel *self)
{
	//Get settings
	self->settings = SETTINGS(g_object_new(TYPE_SETTINGS, NULL));
	
	//GTK_NO_WINDOW widget
	gtk_widget_set_has_window(GTK_WIDGET(self), FALSE);
	
	//Initialize string data
	self->text = g_string_new(NULL);
	self->mod_text = g_string_new(NULL);
	
	
}

void icon_label_finalize(GObject *object)
{
	IconLabel *self = ICON_LABEL(object);
	
	if (self->layout)
		g_object_unref(self->layout);
	
	g_string_free(self->text, TRUE);
	g_string_free(self->mod_text, TRUE);
}

void icon_label_size_request(GtkWidget *widget, GtkRequisition *req)
{
	IconLabel *self = ICON_LABEL(widget);
	gint prop_width, prop_height, calc_width, calc_height, calc_lines;
	PangoLayout *tmp_layout;
	
	//Chain up
	GTK_WIDGET_CLASS(icon_label_parent_class)->size_request(widget, req);
	
	//Get width & height as set by user
	gtk_widget_get_size_request(GTK_WIDGET(self), &prop_width, &prop_height);
	if ((prop_width > -1) && (prop_height > -1))
	{
		//What's need of size_request then!
		req->width = prop_width + 1;
		req->height = prop_height + 1;
		return;
	}
	
	//Calculate size
	tmp_layout = gtk_widget_create_pango_layout(GTK_WIDGET(self),
	                                            self->text->str);
	pango_layout_set_wrap(tmp_layout, PANGO_WRAP_WORD_CHAR);
	if (prop_width > -1)
		pango_layout_set_width(tmp_layout, prop_width * PANGO_SCALE);
	pango_layout_get_pixel_size(tmp_layout, &calc_width, &calc_height);
	
	//Limit no. of lines
	calc_lines = pango_layout_get_line_count(tmp_layout);
	if ((self->settings->n_lines > 0) && (calc_lines > self->settings->n_lines))
		calc_height = (calc_height * self->settings->n_lines) / calc_lines;
	else
		calc_height = calc_height;
		
	//Now request
	req->width = (prop_width > -1 ? prop_width : calc_width) + 1;
	req->height = (prop_height > -1 ? prop_height : calc_height) + 1;
	
	//Cleanup
	g_object_unref(tmp_layout);
}

void icon_label_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
	IconLabel *self = ICON_LABEL(widget);
	
	//Chain up
	GTK_WIDGET_CLASS(icon_label_parent_class)->size_allocate(widget, allocation);
	
	if (self->layout)
		pango_layout_set_text(self->layout, self->text->str, -1);
	else
	{
		self->layout = gtk_widget_create_pango_layout(GTK_WIDGET(self),
	                                                  self->text->str);
	    pango_layout_set_ellipsize(self->layout, PANGO_ELLIPSIZE_END);
	    pango_layout_set_alignment(self->layout, PANGO_ALIGN_CENTER);
	}
	pango_layout_set_width(self->layout, allocation->width * PANGO_SCALE);
	pango_layout_set_height(self->layout, allocation->height * PANGO_SCALE);
}

gboolean icon_label_expose_event(GtkWidget *widget, GdkEventExpose *event)
{
	IconLabel *self = ICON_LABEL(widget);
	GdkWindow *win;
	GtkAllocation allocation;
	cairo_t *cr;
		
	//Prepare for drawing
	win = gtk_widget_get_window(GTK_WIDGET(self));
	gtk_widget_get_allocation(GTK_WIDGET(self), &allocation);
	cr = gdk_cairo_create(win);
	cairo_reset_clip(cr);
	cairo_rectangle(cr,
	                allocation.x,
	                allocation.y,
	                allocation.width,
	                allocation.height);
	cairo_clip(cr);
	if (self->settings->draw_shadow)
	{
		gdk_cairo_set_source_color(cr, &(self->settings->shadow_color));
		cairo_move_to(cr, allocation.x + 1, allocation.y + 1);
		pango_cairo_update_layout(cr, self->layout);
		pango_cairo_show_layout(cr, self->layout);
	}
	
	gdk_cairo_set_source_color(cr, &(self->settings->font_color));
	cairo_move_to(cr, allocation.x, allocation.y);
	pango_cairo_update_layout(cr, self->layout);
	pango_cairo_show_layout(cr, self->layout);
	
	return FALSE;
}

//API

void icon_label_set_text(IconLabel *self, const gchar *str)
{
	g_string_assign(self->text, str);
	
	gtk_widget_queue_resize(GTK_WIDGET(self));
}
 