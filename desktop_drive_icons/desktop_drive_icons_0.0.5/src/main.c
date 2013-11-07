/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) Akash Rawal 2012 <argmlac@gmail.com>
 * 
 * desktop-drive-icons is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * desktop-drive-icons is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "common.h"

void set_desklet_geometry(Settings *settings, gint hints,
                               Desklet *desklet)
{
	if (hints & SETTINGS_WINDOW)
		desklet_set_position(desklet, settings->xpos, settings->ypos, 
							 settings->xoff, settings->yoff);
}

int
main (int argc, char *argv[])
{
	GtkWidget *desklet, *icon_view;
	Settings *settings;

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	
	gtk_set_locale ();
	gtk_init (&argc, &argv);

	settings = SETTINGS(g_object_new(TYPE_SETTINGS, NULL));
	
	desklet = GTK_WIDGET(g_object_new(TYPE_DESKLET, NULL));
	desklet_set_position(DESKLET(desklet), settings->xpos, settings->ypos, 
							 settings->xoff, settings->yoff);
	
	icon_view = GTK_WIDGET(g_object_new(TYPE_ICON_VIEW, NULL));
	gtk_container_add(GTK_CONTAINER(desklet), icon_view);
	
	g_signal_connect(G_OBJECT(settings), "changed",
	                 G_CALLBACK(set_desklet_geometry), desklet);
	
	gtk_widget_show_all(desklet);

	gtk_main();
	
	return 0;
}
