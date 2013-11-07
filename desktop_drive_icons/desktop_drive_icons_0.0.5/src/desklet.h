/*
 * desklet.c or desklet.h
 * Transparent window widget
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
//desklet.c
#include "common.h"
#else
//desklet.h

typedef struct
{
	GtkWindow parent;
	
	//X stuff
	Display *xdisplay;
	Atom *prop_atoms;

	//Pseudotransparency stuff
	GdkWindow *root_window;
	GdkPixmap *wallpaper;
	GdkGC *bg_gc;
	
	//Real transparency stuff
	gboolean rgba;
	GdkPixmap *trans_pix;
	GdkGC *alpha_gc;

	//Current window geometry
	gint x;
	gint y;
	gint width;
	gint height;

	//Desklet placement
	gboolean position_set;
	gdouble xpos, ypos;
	gint xoff, yoff;
	gboolean override;
	guint lower_timeout;
} Desklet;

typedef struct
{
	GtkWindowClass parent;
	
} DeskletClass;

//FILE_HEADER_SUBST:gobject_macro_gen DESKLET Desklet desklet
#define TYPE_DESKLET (desklet_get_type())
#define DESKLET(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), TYPE_DESKLET, Desklet))
#define IS_DESKLET(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), TYPE_DESKLET))
#define DESKLET_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), TYPE_DESKLET, DeskletClass))
#define IS_DESKLET_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), TYPE_DESKLET))
#define DESKLET_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), TYPE_DESKLET, DeskletClass))
#define DESKLET_NAME "Desklet"

#endif //COMMON_H_INSIDE

/*Everything beyond this is automatically generated*/
void desklet_unset_position(Desklet *self);
void desklet_set_position(Desklet *self,gdouble xpos,gdouble ypos,gint xoff,gint yoff);
void desklet_make_rgba_trans(Desklet *self);
void desklet_paint_fake_bg(Desklet *self);
void desklet_update_wallpaper(Desklet *self);
void desklet_position(Desklet *self);
gboolean desklet_check_window_moved(Desklet *self);
GdkFilterReturn desklet_event_filter(XEvent *xevent,GdkEvent *event,Desklet *self);
void desklet_composited_changed(GdkScreen *screen,Desklet *self);
gboolean desklet_lower(gpointer data);
extern const char *desklet_wp_prop_names[];
GtkWidget *desklet_new();
gboolean desklet_event(GtkWidget *widget,GdkEvent *event);
void desklet_size_request(GtkWidget *widget,GtkRequisition *requisition);
void desklet_unrealize(GtkWidget *widget);
void desklet_realize(GtkWidget *widget);
gboolean desklet_expose(GtkWidget *widget,GdkEventExpose *expose_event);
#if !(!defined(COMMON_H_INSIDE))
GType desklet_get_type();
#endif
#if !defined(COMMON_H_INSIDE)
G_DEFINE_TYPE(Desklet,desklet,GTK_TYPE_WINDOW);
#endif
