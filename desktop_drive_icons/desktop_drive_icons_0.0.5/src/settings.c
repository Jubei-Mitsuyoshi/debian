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

#endif

//FILE_HEADER_END

#ifndef COMMON_H_INSIDE
G_DEFINE_TYPE(Settings, settings, GTK_TYPE_DIALOG);
#else
GType settings_get_type();
#endif

//String utility functions
typedef struct _PupStrrplStack PupStrrplStack;
struct _PupStrrplStack
{
	PupStrrplStack *next;
	PupStrrplStack *prev;
	gsize pos;
	guint idx;
	gsize orig_len;
};

gchar *pup_strrpl(const gchar *string, gchar **targets, gchar **substs)
{
	PupStrrplStack *stack = NULL;
	PupStrrplStack *stack_end = NULL;

	//Assertions
	gint i, j, new_strlen;
	if (! string) return NULL;

	new_strlen = strlen(string);
	for (i = 0; string[i]; i++)
	{
		//Search for matching string
		for (j = 0; targets[j]; j++)
		{
			if (string[i] == targets[j][0])
			{
				//Check whether the string matches with targets[j]
				gint k, l;
				for (k = i, l = 0; string[k] && targets[j][l]; k++, l++)
					if (string[k] != targets[j][l]) break;
				if (! targets[j][l])
				{
					//Match found
					PupStrrplStack *new_rpl = g_slice_new(PupStrrplStack);
					new_rpl->pos = i;
					new_rpl->idx = j;
					new_rpl->orig_len = l;
					new_rpl->next = stack;
					new_rpl->prev = NULL;
					if (stack) stack->prev = new_rpl;
					stack = new_rpl;
					if (! stack_end) stack_end = new_rpl;

					i = k - 1;
					new_strlen += ((substs[j] ? strlen(substs[j]) : 0) - l);
					break;
				}
			}
		}
	}

	//Now perform replacements
	gchar *new_str = g_new(gchar, new_strlen + 1);
	PupStrrplStack *iter, *bak;
	i = j = 0;
	int k;
	for (iter = stack_end; iter; iter = bak)
	{
		bak = iter->prev;
		for (; i < iter->pos; i++, j++)
			new_str[j] = string[i];
		if (substs[iter->idx])
			for (k = 0; substs[iter->idx][k]; k++, j++)
				new_str[j] = substs[iter->idx][k];
		i += iter->orig_len;

		g_slice_free(PupStrrplStack, iter);
	}
	//Copy rest of the string
	for(; string[i]; i++, j++)
		new_str[j] = string[i];
	new_str[j] = '\0';

	return new_str;
}


//Config files
gboolean settings_load_cfg_file(GKeyFile *key_file)
{
	gboolean res = TRUE;
	//First search in user's config dir
	gchar *user_cfg_file = g_strconcat(g_get_user_config_dir(),
	                                   G_DIR_SEPARATOR_S,
	                                   SETTINGS_FILE_PATH, NULL);
	GError *error = NULL;
	g_key_file_load_from_file(key_file, user_cfg_file,
	                          G_KEY_FILE_KEEP_COMMENTS, &error);
	if (error)
	{
		if (error->code == G_KEY_FILE_ERROR_NOT_FOUND
		    || error->code == G_FILE_ERROR_NOENT)
		{
			g_clear_error(&error);
			g_key_file_load_from_dirs(key_file, SETTINGS_FILE_PATH, 
				                      (const gchar **) g_get_system_config_dirs(),
			                          NULL, G_KEY_FILE_KEEP_COMMENTS, &error);
			if (error)
			{
				if (error->code == G_KEY_FILE_ERROR_NOT_FOUND
				    || error->code == G_FILE_ERROR_NOENT)
				{
					//not so bad condition
					//g_message(_("Config file not found"));
				}
				else
				{
					//Error
					g_critical(_("Couldn't read config file: %s"), error->message);
				}
				res = FALSE;
			}
		}
		else
		{
			//Error
			g_critical(_("Couldn't read config file: %s"), error->message);
			res = FALSE;
		}
	}
	g_free(user_cfg_file);

	return res;
}

void settings_read(Settings *self)
{
	GError *error = NULL;
	gchar *color;

	self->xpos = g_key_file_get_double(self->key_file, "Main", "XPos", &error);
	if (error)
	{
		g_clear_error(&error);
		self->xpos = 0.5;
	}
	self->xpos = CLAMP(self->xpos, 0.0, 1.0);
	
	self->ypos = g_key_file_get_double(self->key_file, "Main", "YPos", &error);
	if (error)
	{
		g_clear_error(&error);
		self->ypos = 1.0;
	}
	self->ypos = CLAMP(self->ypos, 0.0, 1.0);
	
	self->xoff = g_key_file_get_integer(self->key_file, "Main", "XOffset", &error);
	if (error)
	{
		g_clear_error(&error);
		self->xoff = 0;
	}
	
	self->yoff = g_key_file_get_integer(self->key_file, "Main", "YOffset", &error);
	if (error)
	{
		g_clear_error(&error);
		self->yoff = -40;
	}
	
	self->n_lines = g_key_file_get_integer(self->key_file, "Main", "NLines", &error);
	if (error)
	{
		g_clear_error(&error);
		self->n_lines = 2;
	}
	self->n_lines = MAX(self->n_lines, 0);
	
	self->show_frame = g_key_file_get_boolean(self->key_file, "Main", "ShowFrame", &error);
	if (error)
	{
		g_clear_error(&error);
		self->show_frame = TRUE;
	}
	
	self->vertical = g_key_file_get_boolean(self->key_file, "Main", "Vertical", &error);
	if (error)
	{
		g_clear_error(&error);
		self->vertical = FALSE;
	}
	
	self->reverse_pack = g_key_file_get_boolean(self->key_file, "Main", "ReversePack", &error);
	if (error)
	{
		g_clear_error(&error);
		self->reverse_pack = TRUE;
	}
	
	self->mnt_action = g_key_file_get_string(self->key_file, "Main", "MountAction", NULL);
	if (! self->mnt_action)
	{
		self->mnt_action = g_strdup("rox '$dir'");
	}
	
	self->umnt_action = g_key_file_get_string(self->key_file, "Main", "UMountAction", NULL);
	if (! self->umnt_action)
	{
		self->umnt_action = g_strdup("rox -D '$dir'");
	}
	
	self->draw_shadow = g_key_file_get_boolean(self->key_file, "Main", "DrawShadow", &error);
	if (error)
	{
		g_clear_error(&error);
		self->draw_shadow = TRUE;
	}
	
	color = g_key_file_get_string(self->key_file, "Main", "FontColor", NULL);
	if (! color)
	{
		color = g_strdup("#ffffff");
	}
	gdk_color_parse(color, &(self->font_color));
	g_free(color);
	
	color = g_key_file_get_string(self->key_file, "Main", "ShadowColor", NULL);
	if (! color)
	{
		color = g_strdup("#000000");
	}
	gdk_color_parse(color, &(self->shadow_color));
	g_free(color);
	
}

void settings_save(Settings *self)
{
	gchar *color;
	
	g_key_file_set_double(self->key_file, "Main", "XPos", self->xpos);
	g_key_file_set_double(self->key_file, "Main", "YPos", self->ypos);
	g_key_file_set_integer(self->key_file, "Main", "XOffset", self->xoff);
	g_key_file_set_integer(self->key_file, "Main", "YOffset", self->yoff);
	g_key_file_set_integer(self->key_file, "Main", "NLines", self->n_lines);
	g_key_file_set_boolean(self->key_file, "Main", "ShowFrame", self->show_frame);
	g_key_file_set_boolean(self->key_file, "Main", "Vertical", self->vertical);
	g_key_file_set_boolean(self->key_file, "Main", "ReversePack", self->reverse_pack);
	g_key_file_set_string(self->key_file, "Main", "MountAction", self->mnt_action);
	g_key_file_set_string(self->key_file, "Main", "UMountAction", self->umnt_action);
	g_key_file_set_boolean(self->key_file, "Main", "DrawShadow", self->draw_shadow);
	color = gdk_color_to_string(&(self->font_color));
	g_key_file_set_string(self->key_file, "Main", "FontColor", color);
	g_free(color);
	color = gdk_color_to_string(&(self->shadow_color));
	g_key_file_set_string(self->key_file, "Main", "ShadowColor", color);
	g_free(color);

	gchar *data = g_key_file_to_data(self->key_file, NULL, NULL);
	g_return_if_fail(data);
	gchar *user_cfg_file = g_strconcat(g_get_user_config_dir(),
	                                   G_DIR_SEPARATOR_S,
	                                   SETTINGS_FILE_PATH, NULL);
	gchar *user_cfg_dir = g_strconcat(g_get_user_config_dir(),
	                                  G_DIR_SEPARATOR_S,
	                                  SETTINGS_DIR, NULL);
	g_mkdir_with_parents(user_cfg_dir, 0);
	g_file_set_contents(user_cfg_file, data, -1, NULL);
	g_free(data);
	g_free(user_cfg_file);
	g_free(user_cfg_dir);
}

static void settings_class_init(SettingsClass *klass)
{
	G_OBJECT_CLASS(klass)->constructor = settings_constructor;
	
	klass->changed_signal_id
		= g_signal_new("changed", TYPE_SETTINGS, G_SIGNAL_RUN_FIRST, 0,
		               NULL, NULL, g_cclosure_marshal_VOID__INT,
		               G_TYPE_NONE, 1, G_TYPE_INT);
}

static void settings_init(Settings *self)
{
	//Load config file
	self->key_file = g_key_file_new();
	settings_load_cfg_file(self->key_file);
	settings_read(self);

	//Setup adjustments
	self->xpos_adj = GTK_ADJUSTMENT(gtk_adjustment_new(self->xpos, 0, 1, 0.01, 0.1, 0));
	g_signal_connect(self->xpos_adj, "value-changed",
	                 G_CALLBACK(settings_changed_xpos), self);
	
	self->ypos_adj = GTK_ADJUSTMENT(gtk_adjustment_new(self->ypos, 0, 1, 0.01, 0.1, 0));
	g_signal_connect(self->ypos_adj, "value-changed",
	                 G_CALLBACK(settings_changed_ypos), self);
	                 
	self->xoff_adj = GTK_ADJUSTMENT(gtk_adjustment_new(self->xoff, -4096, 4096, 1, 10, 0));
	g_signal_connect(self->xoff_adj, "value-changed",
	                 G_CALLBACK(settings_changed_xoff), self);
	                 
	self->yoff_adj = GTK_ADJUSTMENT(gtk_adjustment_new(self->yoff, -4096, 4096, 1, 10, 0));
	g_signal_connect(self->yoff_adj, "value-changed",
	                 G_CALLBACK(settings_changed_yoff), self);
	                 
	self->n_lines_adj = GTK_ADJUSTMENT(gtk_adjustment_new(self->n_lines, 0, 4096, 1, 10, 0));
	g_signal_connect(self->n_lines_adj, "value-changed",
	                 G_CALLBACK(settings_changed_n_lines), self);
	
	//Setup UI
	GtkWidget *container;
	//GtkWidget *widget;
	//position
	gtk_box_pack_start_defaults
		(GTK_BOX(GTK_DIALOG(self)->vbox),
		 gtk_label_new(_("Position of the drive icons:")));
	
	gtk_box_pack_start_defaults
		(GTK_BOX(GTK_DIALOG(self)->vbox), container = gtk_hbox_new(FALSE, 0));
	gtk_box_pack_start_defaults(GTK_BOX(container), gtk_label_new(_("X: ")));
	gtk_box_pack_start_defaults(GTK_BOX(container),
	                            gtk_spin_button_new(self->xpos_adj, 1, 3));
	gtk_box_pack_start_defaults(GTK_BOX(container), gtk_label_new(_("Offset: ")));
	gtk_box_pack_start_defaults(GTK_BOX(container),
	                            gtk_spin_button_new(self->xoff_adj, 1, 0));
	                                                       
	
	gtk_box_pack_start_defaults
		(GTK_BOX(GTK_DIALOG(self)->vbox), container = gtk_hbox_new(FALSE, 0));
	gtk_box_pack_start_defaults(GTK_BOX(container), gtk_label_new(_("Y: ")));
	gtk_box_pack_start_defaults(GTK_BOX(container),
	                            gtk_spin_button_new(self->ypos_adj, 1, 3));
	gtk_box_pack_start_defaults(GTK_BOX(container), gtk_label_new(_("Offset: ")));
	gtk_box_pack_start_defaults(GTK_BOX(container),
	                            gtk_spin_button_new(self->yoff_adj, 1, 0));
	                            
	//NLines
	gtk_box_pack_start_defaults
		(GTK_BOX(GTK_DIALOG(self)->vbox), container = gtk_hbox_new(FALSE, 0));
	gtk_box_pack_start_defaults(GTK_BOX(container),
			gtk_label_new(_("Maximum no. of lines in an icon label (0 for infinity): ")));
	gtk_box_pack_start_defaults(GTK_BOX(container),
	                            gtk_spin_button_new(self->n_lines_adj, 1, 0));
	
	//ShowFrame
	self->show_frame_checkbox = gtk_check_button_new_with_label
		(_("Show frame around the icons"));
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(self)->vbox),
	                            self->show_frame_checkbox);
	g_signal_connect(self->show_frame_checkbox, "toggled",
	                 G_CALLBACK(settings_changed_show_frame), self);
	//Vertical
	self->vertical_checkbox = gtk_check_button_new_with_label
		(_("Arrange all icons vertically"));
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(self)->vbox),
	                            self->vertical_checkbox);
	g_signal_connect(self->vertical_checkbox, "toggled",
	                 G_CALLBACK(settings_changed_vertical), self);
	//ReversePack
	self->reverse_pack_checkbox = gtk_check_button_new_with_label
		(_("Reverse icon packing order"));
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(self)->vbox),
	                            self->reverse_pack_checkbox);
	g_signal_connect(self->reverse_pack_checkbox, "toggled",
	                 G_CALLBACK(settings_changed_reverse_pack), self);
	//FontColor & ShadowColor
	gtk_box_pack_start_defaults
		(GTK_BOX(GTK_DIALOG(self)->vbox), container = gtk_hbox_new(FALSE, 0));
	
	gtk_box_pack_start_defaults
		(GTK_BOX(container), gtk_label_new(_("Font color:")));
	
	self->font_color_chooser = gtk_color_button_new();
	gtk_box_pack_start_defaults(GTK_BOX(container), self->font_color_chooser);
	g_signal_connect(self->font_color_chooser, "color-set",
	                 G_CALLBACK(settings_changed_font_color), self);
	
	self->draw_shadow_checkbox = gtk_check_button_new_with_label
		(_("Shadow color:"));
	gtk_box_pack_start_defaults(GTK_BOX(container), self->draw_shadow_checkbox);
	g_signal_connect(self->draw_shadow_checkbox, "toggled",
	                 G_CALLBACK(settings_changed_draw_shadow), self);
	
	self->shadow_color_chooser = gtk_color_button_new();
	gtk_box_pack_start_defaults(GTK_BOX(container), self->shadow_color_chooser);
	g_signal_connect(self->shadow_color_chooser, "color-set",
	                 G_CALLBACK(settings_changed_shadow_color), self);
	//MountAction
	gtk_box_pack_start_defaults
		(GTK_BOX(GTK_DIALOG(self)->vbox), container = gtk_hbox_new(FALSE, 0));
	gtk_box_pack_start_defaults
		(GTK_BOX(container), gtk_label_new(_("Post mount command:")));
	self->mnt_action_entry = gtk_entry_new();
	gtk_box_pack_start_defaults(GTK_BOX(container),
	                            self->mnt_action_entry);
	g_signal_connect(self->mnt_action_entry, "changed",
	                 G_CALLBACK(settings_changed_mnt_action), self);

	//UMountAction
	gtk_box_pack_start_defaults
		(GTK_BOX(GTK_DIALOG(self)->vbox), container = gtk_hbox_new(FALSE, 0));
	gtk_box_pack_start_defaults
		(GTK_BOX(container), gtk_label_new(_("Post unmount command:")));
	self->umnt_action_entry = gtk_entry_new();
	gtk_box_pack_start_defaults(GTK_BOX(container),
	                            self->umnt_action_entry);
	g_signal_connect(self->umnt_action_entry, "changed",
	                 G_CALLBACK(settings_changed_umnt_action), self);

	//Done
	gtk_dialog_add_button(GTK_DIALOG(self), GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE);

	gtk_widget_show_all(GTK_DIALOG(self)->vbox);


	//Some workarounds
	g_signal_connect(self, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
	g_signal_connect(self, "response", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
	
	//Signals
	self->hints = 0;
}

static GObject *settings_object = NULL;

GObject *settings_constructor(GType                  type,
                              guint                  n_construct_params,
                              GObjectConstructParam *construct_params)
{
	if (! settings_object)
		settings_object = G_OBJECT_CLASS(settings_parent_class)->constructor
			(type, n_construct_params, construct_params);

	//g_object_add_weak_pointer(settings_object, (gpointer *) &settings_object);

	return settings_object;
}

void settings_show(Settings *self)
{
	self->no_signals = TRUE;
	gtk_adjustment_set_value(self->xpos_adj, self->xpos);
	gtk_adjustment_set_value(self->ypos_adj, self->ypos);
	gtk_adjustment_set_value(self->xoff_adj, self->xoff);
	gtk_adjustment_set_value(self->yoff_adj, self->yoff);
	gtk_adjustment_set_value(self->n_lines_adj, self->n_lines);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(self->show_frame_checkbox),
	                             self->show_frame);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(self->vertical_checkbox),
	                             self->vertical);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(self->reverse_pack_checkbox),
	                             self->reverse_pack);
	gtk_entry_set_text(GTK_ENTRY(self->mnt_action_entry), self->mnt_action);
	gtk_entry_set_text(GTK_ENTRY(self->umnt_action_entry), self->umnt_action);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(self->draw_shadow_checkbox),
	                             self->draw_shadow);
	gtk_color_button_set_color(GTK_COLOR_BUTTON(self->font_color_chooser),
	                           &(self->font_color));
	gtk_color_button_set_color(GTK_COLOR_BUTTON(self->shadow_color_chooser),
	                           &(self->shadow_color));
	
	self->no_signals = FALSE;
	
	gtk_dialog_run(GTK_DIALOG(self));

	settings_save(self);
}

//Signals
gboolean settings_changed_signal_timeout(Settings *self)
{
	if (self->hints)
	{
		g_signal_emit(self, SETTINGS_GET_CLASS(self)->changed_signal_id, 0,
					  self->hints);
		
		self->hints = 0;
	}
	
	return FALSE;
}

void settings_emit_changed_signal(Settings *self, gint hint)
{
	gint bak = self->hints;
	
	self->hints |= hint;
	
	if (! bak)
		g_timeout_add(300, (GSourceFunc) settings_changed_signal_timeout, self);
}

void settings_changed_xpos(GtkAdjustment *adj, Settings *self)
{
	if (self->no_signals) return;
	
	self->xpos = adj->value;

	settings_emit_changed_signal(self, SETTINGS_WINDOW);
}

void settings_changed_ypos(GtkAdjustment *adj, Settings *self)
{
	if (self->no_signals) return;
	
	self->ypos = adj->value;

	settings_emit_changed_signal(self, SETTINGS_WINDOW);
}

void settings_changed_xoff(GtkAdjustment *adj, Settings *self)
{
	if (self->no_signals) return;
	
	self->xoff = (gint) adj->value;

	settings_emit_changed_signal(self, SETTINGS_WINDOW);
}

void settings_changed_yoff(GtkAdjustment *adj, Settings *self)
{
	if (self->no_signals) return;
	
	self->yoff = (gint) adj->value;

	settings_emit_changed_signal(self, SETTINGS_WINDOW);
}

void settings_changed_n_lines(GtkAdjustment *adj, Settings *self)
{
	if (self->no_signals) return;
	
	self->n_lines = (gint) adj->value;

	settings_emit_changed_signal(self, SETTINGS_UI);
}

void settings_changed_show_frame(GtkToggleButton *button, Settings *self)
{
	if (self->no_signals) return;
	
	self->show_frame = gtk_toggle_button_get_active(button);

	settings_emit_changed_signal(self, SETTINGS_UI);
}

void settings_changed_vertical(GtkToggleButton *button, Settings *self)
{
	if (self->no_signals) return;
	
	self->vertical = gtk_toggle_button_get_active(button);

	settings_emit_changed_signal(self, SETTINGS_UI);
}

void settings_changed_reverse_pack(GtkToggleButton *button, Settings *self)
{
	if (self->no_signals) return;
	
	self->reverse_pack = gtk_toggle_button_get_active(button);

	settings_emit_changed_signal(self, SETTINGS_UI);
}

void settings_changed_mnt_action(GtkEditable *entry, Settings *self)
{
	if (self->no_signals) return;
	
	g_free(self->mnt_action);
	self->mnt_action = g_strdup(gtk_entry_get_text
	                            (GTK_ENTRY(self->mnt_action_entry)));

	//Not required
	//g_signal_emit(self, SETTINGS_GET_CLASS(self)->changed_signal_id, 0);
}

void settings_changed_umnt_action(GtkEditable *entry, Settings *self)
{
	if (self->no_signals) return;
	
	g_free(self->umnt_action);
	self->umnt_action = g_strdup(gtk_entry_get_text
	                            (GTK_ENTRY(self->umnt_action_entry)));

	//Not required
	//g_signal_emit(self, SETTINGS_GET_CLASS(self)->changed_signal_id, 0);
}

void settings_changed_draw_shadow(GtkToggleButton *button, Settings *self)
{
	if (self->no_signals) return;
	
	self->draw_shadow = gtk_toggle_button_get_active(button);

	settings_emit_changed_signal(self, SETTINGS_THEME);
}

void settings_changed_font_color(GtkColorButton *button, Settings *self)
{
	if (self->no_signals) return;
	
	gtk_color_button_get_color(button, &(self->font_color));

	settings_emit_changed_signal(self, SETTINGS_THEME);
}

void settings_changed_shadow_color(GtkColorButton *button, Settings *self)
{
	if (self->no_signals) return;
	
	gtk_color_button_get_color(button, &(self->shadow_color));

	settings_emit_changed_signal(self, SETTINGS_THEME);
}

//API
void settings_spawn_command(Settings *self, GFile *file, SettingsSpawn cmd)
{
	gchar *path = g_file_get_path(file);
	g_object_unref(file);

	if (! path)
	{
		GtkWidget *error_dialog = gtk_message_dialog_new
			(NULL, 0, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
			 _("The volume is (probably) mounted at virtual location,"
			   "cannot open"));
		gtk_dialog_run(GTK_DIALOG(error_dialog));
		g_object_unref(error_dialog);
		return;                                                                  
	}

	gchar *targets[] = {"$dir", NULL};
	gchar *substs[] = {path, NULL};
	gchar *command;
	switch (cmd)
	{
	case SETTINGS_MOUNT:
		command = pup_strrpl(self->mnt_action, targets, substs);
		break;
	case SETTINGS_UMOUNT:
		command = pup_strrpl(self->umnt_action, targets, substs);
		break;
	default:
		g_error("settings_spawn_command() : Invalid argument cmd %d", cmd);
	}
	g_free(path);

	GError *error = NULL;
	g_spawn_command_line_async(command, &error);
	if (error)
	{
		GtkWidget *error_dialog = gtk_message_dialog_new
			(NULL, 0, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
			 _("Error while running command \"%s\": %s"), command, error->message);
		gtk_dialog_run(GTK_DIALOG(error_dialog));
		g_object_unref(error_dialog);                                                              
		g_clear_error(&error);
	}
	g_free(command);
}
