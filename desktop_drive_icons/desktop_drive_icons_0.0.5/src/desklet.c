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

#endif //COMMON_H_INSIDE

//FILE_HEADER_END

#ifndef COMMON_H_INSIDE
G_DEFINE_TYPE(Desklet, desklet, GTK_TYPE_WINDOW);
#else
GType desklet_get_type();
#endif

static void desklet_class_init(DeskletClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
	widget_class->expose_event = desklet_expose;
	widget_class->realize = desklet_realize;
	widget_class->unrealize = desklet_unrealize;
	widget_class->size_request = desklet_size_request;
	widget_class->event = desklet_event;
}

static void desklet_init(Desklet *self)
{
	//If alpha compositing is available use colormap with alpha channel
	//so that we can use real transparency
	GdkScreen *screen = gdk_screen_get_default();
	if (gdk_screen_is_composited(screen))
	{
		GdkColormap *cmap = gdk_screen_get_rgba_colormap(screen);
		if (cmap)
			gtk_widget_set_colormap(GTK_WIDGET(self), cmap);
	}
	
	self->override = FALSE;
	gtk_window_set_resizable(GTK_WINDOW(self), FALSE);
	
	self->lower_timeout = 0;
}

GtkWidget *desklet_new()
{
	return GTK_WIDGET(g_object_new(TYPE_DESKLET, NULL));
}

const char* desklet_wp_prop_names[] = 
{
	//Properties of root window that may contain wallpaper
	"_XROOTPMAP_ID", "ESETROOT_PMAP_ID", NULL
};

void desklet_realize(GtkWidget *widget)
{
	Desklet *self = DESKLET(widget);
	gint n_atoms;
	const gchar *wm_name;

	//Chain up to parent's method
	GTK_WIDGET_CLASS(desklet_parent_class)->realize(widget);

	GdkWindow *win = gtk_widget_get_window(widget);
	self->root_window = gtk_widget_get_root_window(widget);

	//Setup the window to the desktop
	gtk_window_set_decorated(GTK_WINDOW(self), FALSE);
	wm_name = gdk_x11_screen_get_window_manager_name
	              (gtk_widget_get_screen(GTK_WIDGET(self)));
	printf("wm_name=\"%s\"\n", wm_name);
	
	if (strcmp(wm_name, "JWM") == 0)
	{
		gtk_window_set_type_hint(GTK_WINDOW(self), GDK_WINDOW_TYPE_HINT_DESKTOP);
	}
	else if (strcmp(wm_name, "Openbox") == 0)
	{
		self->override = TRUE;
		/*
		//gtk_window_set_type_hint(GTK_WINDOW(self), GDK_WINDOW_TYPE_HINT_DESKTOP);
		//Override window manager control for problematic window managers
		XSetWindowAttributes attrs
			= {None, 0L, 0, 0L, 0, 0, 0, 0L, 0L, False, 0L, 0L,
		       True, //override-redirect
		       0, 0 };
		gulong mask = CWOverrideRedirect;
		
		XChangeWindowAttributes(GDK_WINDOW_XDISPLAY(win),
		                        GDK_WINDOW_XID(win),
		                        mask,
		                        &attrs);
		*/
		self->lower_timeout = g_timeout_add(100, desklet_lower, self);
	}
	else
	{
		self->override = FALSE;
		gtk_window_set_type_hint(GTK_WINDOW(self), GDK_WINDOW_TYPE_HINT_DOCK);
		gtk_window_set_keep_below(GTK_WINDOW(self), TRUE);
		gtk_window_stick(GTK_WINDOW(self));
	}

	//Prepare graphics contexts
	GdkGCValues values;
	values.function = GDK_CLEAR;
	values.fill = GDK_TILED;
	self->alpha_gc = gdk_gc_new_with_values(GDK_DRAWABLE(win), &values,
	                                        GDK_GC_FUNCTION);
	self->bg_gc = gdk_gc_new_with_values(GDK_DRAWABLE(win), &values,
	                                     GDK_GC_FILL);

	//Get property atoms for desktop wallpaper
	for (n_atoms = 0; desklet_wp_prop_names[n_atoms]; n_atoms++)
		;
	self->prop_atoms = (Atom *) g_malloc(n_atoms * sizeof(Atom));
	self->xdisplay = GDK_WINDOW_XDISPLAY(win);
	XInternAtoms(self->xdisplay, (char **) desklet_wp_prop_names, 
	             n_atoms, True, self->prop_atoms);
	
	//Setup transparency data
	desklet_composited_changed(gtk_widget_get_screen(GTK_WIDGET(self)), self);
	
	//Setup events
	GdkEventMask mask = gdk_window_get_events(self->root_window);
	gdk_window_set_events(self->root_window, mask | GDK_PROPERTY_CHANGE_MASK);
	gdk_window_add_filter(self->root_window,
	                      (GdkFilterFunc) desklet_event_filter,
	                      self);
	g_signal_connect_object(gtk_widget_get_screen(GTK_WIDGET(self)), "composited-changed",
	                        G_CALLBACK(desklet_composited_changed), self, 0);
}

void desklet_unrealize(GtkWidget *widget)
{
	Desklet *self = DESKLET(widget);

	gdk_window_remove_filter(self->root_window,
	                         (GdkFilterFunc) desklet_event_filter,
	                         self);

	g_object_unref(self->alpha_gc);
	g_object_unref(self->bg_gc);
	
	if (self->wallpaper)
	{
		g_object_unref(self->wallpaper);
		self->wallpaper = NULL;
	}
	
	g_free(self->prop_atoms);
	
	if (self->override)
	{
		self->override = FALSE;
		g_source_remove(self->lower_timeout);
	}
	
	GTK_WIDGET_CLASS(desklet_parent_class)->unrealize(widget);
}

gboolean desklet_check_window_moved(Desklet *self)
{
	gint nx, ny, nw, nh;
	GdkWindow *win = gtk_widget_get_window(GTK_WIDGET(self));
	gdk_window_get_root_origin(win, &nx, &ny);
	gdk_window_get_geometry(win, NULL, NULL, &nw, &nh, NULL);

	if ((nx != self->x) || (ny != self->y) || (nw != self->width)
	    || (nh != self->height))
	{
		self->x = nx;
		self->y = ny;
		self->width = nw;
	    self->height = nh;

		return TRUE;
	}
	return FALSE;
}

void desklet_position(Desklet *self)
{
	gint width, height, winwidth = 0, winheight = 0;
	gint x, y, xup, yup;	
		
	if (! self->position_set) return;
	
	//Collect data
	GdkScreen *scr = gtk_window_get_screen(GTK_WINDOW(self));
	width = gdk_screen_get_width(scr);
	height = gdk_screen_get_height(scr);
	gtk_window_get_size(GTK_WINDOW(self), &winwidth, &winheight);
	
	//Upper limits
	xup = width - winwidth;
	if (xup < 0)
		xup = 0;
	
	yup = height - winheight;
	if (yup < 0)
		yup = 0;
	
	//Calculate window poition
	x = CLAMP(xup * self->xpos + self->xoff, 0, xup);
	y = CLAMP(yup * self->ypos + self->yoff, 0, yup);
	
	//Assign...
	gtk_window_move(GTK_WINDOW(self), x, y);
}

void desklet_size_request(GtkWidget *widget,
                          GtkRequisition *requisition)
{
	Desklet *self = DESKLET(widget);
	gint width, height;
	
	//Chain up to parent first
	GTK_WIDGET_CLASS(desklet_parent_class)->size_request
			(GTK_WIDGET(self), requisition);
	
	GdkScreen *scr = gtk_window_get_screen(GTK_WINDOW(self));
	width = gdk_screen_get_width(scr);
	height = gdk_screen_get_height(scr);
	
	if (requisition->width > width)
		requisition->width = width;
	
	if (requisition->height > height)
		requisition->height = height;
}

//Pseudotransparency implementation

void desklet_update_wallpaper(Desklet *self)
{
	Pixmap root_pixmap = None;
	
	int i;

	for (i=0; (desklet_wp_prop_names[i] && (root_pixmap == None)); i++)
	{
		if (self->prop_atoms[i] != None)
		{
			Atom actual_type = None;
			int actual_format = 0;
			unsigned long nitems = 0;
			unsigned long bytes_after = 0;
			unsigned char *properties = NULL;
			int rc = 0;

			rc = XGetWindowProperty(self->xdisplay,
			                        GDK_WINDOW_XID(self->root_window),
			                        self->prop_atoms[i], 0, 1,
					                False, XA_PIXMAP,
					                &actual_type, &actual_format, &nitems,
					                &bytes_after, &properties);

			if (rc == Success && properties)
			{
				root_pixmap = *((Pixmap*)properties);
			}
		}
	}
	
	if (self->wallpaper)
	{
		g_object_unref(self->wallpaper);
		self->wallpaper = NULL;
	}
	if (root_pixmap)
	{
		self->wallpaper = gdk_pixmap_foreign_new(root_pixmap);
	}
	else
	{
		g_warning(_("Couldn't get background image, transparency won't work"));
	}
	
}

void desklet_paint_fake_bg(Desklet *self)
{
	//Takes care of depths
	GdkWindow *win = GTK_WIDGET(self)->window;
	g_return_if_fail(win);
	g_return_if_fail(self->wallpaper);

	//Get coordinates
	gint x, y, width, height;
	gdk_window_get_root_origin(win, &x, &y);
	gdk_window_get_geometry(win, NULL, NULL, &width, &height, NULL);
	//Check whether depths are same
	if (gdk_drawable_get_depth(GDK_DRAWABLE(win))
	    == gdk_drawable_get_depth(GDK_DRAWABLE(self->wallpaper)))
	{
		gdk_gc_set_tile(self->bg_gc, self->wallpaper);
		gdk_gc_set_ts_origin(self->bg_gc, -x, -y);
		gdk_draw_rectangle(GDK_DRAWABLE(win), self->bg_gc, TRUE, 0, 0,
		                   width, height);
	}
	else
	{
		//Use GdkPixbuf to copy the wallpaper :(
		GdkColormap *cmap = gdk_colormap_new
			(gdk_visual_get_best_with_depth
			 (gdk_drawable_get_depth(GDK_DRAWABLE(self->wallpaper))), FALSE);
		//Download portion of wallpaper
		GdkPixbuf *pix = gdk_pixbuf_get_from_drawable
			(NULL, GDK_DRAWABLE(self->wallpaper), cmap, x, y, 0, 0, width, height);
		//Now upload it back to be pasted on our window 
		gdk_draw_pixbuf(GDK_DRAWABLE(win), self->bg_gc, pix, 0, 0, 0, 0,
		                width, height, GDK_RGB_DITHER_NONE, 0, 0);

		g_object_unref(cmap);
		g_object_unref(pix);
	}
}

void desklet_make_rgba_trans(Desklet *self)
{
	gint width, height;
	GdkWindow *win = GTK_WIDGET(self)->window;
	g_return_if_fail(win);
	
	gdk_window_get_geometry(win, NULL, NULL, &width, &height, NULL);
	
	gdk_draw_rectangle(GDK_DRAWABLE(win), self->alpha_gc, TRUE, 0, 0,
	                   width, height);
}

gboolean desklet_expose(GtkWidget *widget, GdkEventExpose *expose_event)
{
	Desklet *self = DESKLET(widget);

	if (self->rgba)
		desklet_make_rgba_trans(self);
	else
		desklet_paint_fake_bg(self);

	//Chain up to draw the child
	gtk_container_propagate_expose(GTK_CONTAINER(self),
	                               gtk_bin_get_child(GTK_BIN(self)),
	                               expose_event);

	return FALSE;
}

//Events
GdkFilterReturn desklet_event_filter(XEvent *xevent, GdkEvent *event, Desklet *self)
{
	if (xevent->type != PropertyNotify) return GDK_FILTER_CONTINUE;
	
	if (! self->rgba)
	{
		gchar *pname = XGetAtomName(self->xdisplay, xevent->xproperty.atom);
		if (pname)
		{
			if ((strcmp(pname, "_XROOTPMAP_ID") == 0)
				|| (strcmp(pname, "ESETROOT_PMAP_ID") == 0) )
			{
				desklet_update_wallpaper(self);
	
				gtk_widget_queue_draw(GTK_WIDGET(self));
			}
			XFree(pname);
		}
	}

	return GDK_FILTER_CONTINUE;
}

void desklet_composited_changed(GdkScreen *screen, Desklet *self)
{
	self->rgba = gdk_screen_is_composited(screen);

	if (self->rgba)
	{
		if (gdk_drawable_get_depth(GDK_DRAWABLE(GTK_WIDGET(self)->window))
		    != 32)
			self->rgba = FALSE;
	}

	if (! self->rgba)
	{
		desklet_update_wallpaper(self);
	}
	gtk_widget_queue_draw(GTK_WIDGET(self));
}

gboolean desklet_event(GtkWidget *widget, GdkEvent *event)
{
	Desklet *self = DESKLET(widget);
	
	if (GTK_WIDGET_CLASS(desklet_parent_class)->event)
		GTK_WIDGET_CLASS(desklet_parent_class)->event(widget, event);
	
	if (event->type == GDK_CONFIGURE && desklet_check_window_moved(self))
	{
		gtk_widget_queue_draw(GTK_WIDGET(self));

		desklet_position(self);
	}

	return FALSE;
}

gboolean desklet_lower(gpointer data)
{
	Desklet *self = DESKLET(data);
	
	if (GTK_WIDGET_MAPPED(GTK_WIDGET(self)) && self->override)
	{
		GdkWindow *win = gtk_widget_get_window(GTK_WIDGET(self));
		
		XLowerWindow(GDK_WINDOW_XDISPLAY(win),
					 GDK_WINDOW_XID(win));
		
		return TRUE;
	}
	
	return FALSE;
}

//API
void desklet_set_position(Desklet *self,
                          gdouble xpos, gdouble ypos, gint xoff, gint yoff)
{
	self->xpos = xpos;
	self->ypos = ypos;
	self->xoff = xoff;
	self->yoff = yoff;
	
	self->position_set = TRUE;
	desklet_position(self);
}

void desklet_unset_position(Desklet *self)
{
	self->position_set = FALSE;
}

