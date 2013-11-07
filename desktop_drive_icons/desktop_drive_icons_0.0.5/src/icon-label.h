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
#define TYPE_ICON_LABEL (icon_label_get_type())
#define ICON_LABEL(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), TYPE_ICON_LABEL, IconLabel))
#define IS_ICON_LABEL(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), TYPE_ICON_LABEL))
#define ICON_LABEL_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), TYPE_ICON_LABEL, IconLabelClass))
#define IS_ICON_LABEL_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), TYPE_ICON_LABEL))
#define ICON_LABEL_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), TYPE_ICON_LABEL, IconLabelClass))
#define ICON_LABEL_NAME "IconLabel"

/*Everything beyond this is automatically generated*/
void icon_label_set_text(IconLabel *self,const gchar *str);
gboolean icon_label_expose_event(GtkWidget *widget,GdkEventExpose *event);
void icon_label_size_allocate(GtkWidget *widget,GtkAllocation *allocation);
void icon_label_size_request(GtkWidget *widget,GtkRequisition *req);
void icon_label_finalize(GObject *object);
#if !(!defined(COMMON_H_INSIDE))
GType icon_label_get_type();
#endif
#if !defined(COMMON_H_INSIDE)
G_DEFINE_TYPE(IconLabel,icon_label,GTK_TYPE_WIDGET);
#endif
