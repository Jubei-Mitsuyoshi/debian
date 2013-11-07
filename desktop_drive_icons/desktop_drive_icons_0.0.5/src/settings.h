/*
 * settings.c or settings.h
 * Configuration of the program
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
//settings.c
#include "common.h"
#else
//settings.h

typedef struct
{
	GtkDialog parent;

	//Settings
	GKeyFile *key_file;
	gdouble xpos, ypos;
	gint xoff, yoff;
	gint n_lines;
	gboolean show_frame;
	gboolean vertical;
	gboolean reverse_pack;
	gchar *mnt_action;
	gchar *umnt_action;
	gboolean draw_shadow;
	GdkColor font_color;
	GdkColor shadow_color;
	
	//UI
	GtkAdjustment *xpos_adj, *ypos_adj, *xoff_adj, *yoff_adj;
	GtkAdjustment *n_lines_adj;
	GtkWidget *show_frame_checkbox;
	GtkWidget *vertical_checkbox;
	GtkWidget *reverse_pack_checkbox;
	GtkWidget *mnt_action_entry;
	GtkWidget *umnt_action_entry;
	GtkWidget *draw_shadow_checkbox;
	GtkWidget *font_color_chooser;
	GtkWidget *shadow_color_chooser;
	
	//Signals
	gboolean no_signals;
	gint hints;
} Settings;

typedef struct
{
	GtkDialogClass parent;

	guint changed_signal_id;
} SettingsClass;

typedef enum 
{
	SETTINGS_WINDOW = 1 << 0,
	SETTINGS_UI = 1 << 1,
	SETTINGS_THEME = 1 << 2
} SettingsHint;

typedef enum
{
	SETTINGS_MOUNT = 1,
	SETTINGS_UMOUNT = 2
} SettingsSpawn;

#define SETTINGS_DIR PACKAGE_NAME
#define SETTINGS_FILE_PATH PACKAGE_NAME G_DIR_SEPARATOR_S "config.ini"

//FILE_HEADER_SUBST:gobject_macro_gen SETTINGS Settings settings
#define TYPE_SETTINGS (settings_get_type())
#define SETTINGS(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), TYPE_SETTINGS, Settings))
#define IS_SETTINGS(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), TYPE_SETTINGS))
#define SETTINGS_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), TYPE_SETTINGS, SettingsClass))
#define IS_SETTINGS_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), TYPE_SETTINGS))
#define SETTINGS_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), TYPE_SETTINGS, SettingsClass))
#define SETTINGS_NAME "Settings"

#endif

/*Everything beyond this is automatically generated*/
void settings_spawn_command(Settings *self,GFile *file,SettingsSpawn cmd);
void settings_emit_changed_signal(Settings *self,gint hint);
gboolean settings_changed_signal_timeout(Settings *self);
void settings_show(Settings *self);
void settings_changed_umnt_action(GtkEditable *entry,Settings *self);
void settings_changed_mnt_action(GtkEditable *entry,Settings *self);
void settings_changed_shadow_color(GtkColorButton *button,Settings *self);
void settings_changed_draw_shadow(GtkToggleButton *button,Settings *self);
void settings_changed_font_color(GtkColorButton *button,Settings *self);
void settings_changed_reverse_pack(GtkToggleButton *button,Settings *self);
void settings_changed_vertical(GtkToggleButton *button,Settings *self);
void settings_changed_show_frame(GtkToggleButton *button,Settings *self);
void settings_changed_n_lines(GtkAdjustment *adj,Settings *self);
void settings_changed_yoff(GtkAdjustment *adj,Settings *self);
void settings_changed_xoff(GtkAdjustment *adj,Settings *self);
void settings_changed_ypos(GtkAdjustment *adj,Settings *self);
void settings_changed_xpos(GtkAdjustment *adj,Settings *self);
GObject *settings_constructor(GType type,guint n_construct_params,GObjectConstructParam *construct_params);
void settings_save(Settings *self);
void settings_read(Settings *self);
gboolean settings_load_cfg_file(GKeyFile *key_file);
gchar *pup_strrpl(const gchar *string,gchar **targets,gchar **substs);
#if !(!defined(COMMON_H_INSIDE))
GType settings_get_type();
#endif
#if !defined(COMMON_H_INSIDE)
G_DEFINE_TYPE(Settings,settings,GTK_TYPE_DIALOG);
#endif
