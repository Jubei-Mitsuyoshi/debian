//pupvm-settings.c or pupvm-settings.h
//Loading plugins and their settings

#ifndef PUP_VM_H_INSIDE
//pupvm-settings.c
#include "common-includes.h"

#else
//pupvm-settings.c

#define PUP_MNTPNT_ALLOWED_CHARS \
	"0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm_"
#define PUP_VM_DIR_NAME "pup-volume-monitor"

typedef struct
{
	GHashTable *plugins;
	//Daemon's own settings
	GKeyFile *cfg_file;
	gchar *volume_disp_name;
	gchar *drive_disp_name;
	gchar *volume_disp_name_fallback;
	gchar *volume_mntpnt_name;
	gchar *volume_mntpnt_name_fallback;
	gchar *volume_mount_dir;
} PupVMSettings;

typedef struct
{
	GTypeModule parent;

	GModule *module;
	gchar *plugin_name;
	gchar *plugin_path;
	gboolean initialised;

	GKeyFile *cfg_file;
} PupVMPlugin;

typedef struct 
{
	GTypeModuleClass parent;
	
} PupVMPluginClass;

//FILE_HEADER_SUBST:gobject_macro_gen PUP_VM_PLUGIN PupVMPlugin pup_vm_plugin pup

typedef void (*PupVMPluginInit) (PupVMPlugin *plugin);
typedef void (*PupVMPluginReconf) (PupVMPlugin *plugin); //Will be implemented later

#endif

//FILE_HEADER_END

//Config files
gboolean pup_load_cfg_file(GKeyFile *key_file, const gchar *module)
{
	gboolean res = TRUE;
	//First search in user's config dir
	gchar *cfg_file_path = g_strconcat(PUP_VM_DIR_NAME,
	                                   G_DIR_SEPARATOR_S,
	                                   module, ".conf", NULL);
	gchar *user_cfg_file = g_strconcat(g_get_user_config_dir(),
	                                   G_DIR_SEPARATOR_S,
	                                   cfg_file_path, NULL);
	GError *error = NULL;
	g_key_file_load_from_file(key_file, user_cfg_file, 0, &error);
	if (error)
	{
		if (error->code == G_KEY_FILE_ERROR_NOT_FOUND
		    || error->code == G_FILE_ERROR_NOENT)
		{
			g_clear_error(&error);
			g_key_file_load_from_dirs(key_file, cfg_file_path, 
				                      (const gchar **) g_get_system_config_dirs(),
			                          NULL, 0, &error);
			if (error)
			{
				if (error->code == G_KEY_FILE_ERROR_NOT_FOUND
				    || error->code == G_FILE_ERROR_NOENT)
				{
					//not so bad condition
					g_message("Config file for plugin %s not found, "
						      "it will use default settings", module);
				}
				else
				{
					//Error
					g_critical("Couldn't read config file: %s", error->message);
				}
				res = FALSE;
			}
		}
		else
		{
			//Error
			g_critical("Couldn't read config file: %s", error->message);
			res = FALSE;
		}
	}
	g_free(cfg_file_path);
	g_free(user_cfg_file);

	return res;
}

//Plugins
#ifndef PUP_VM_H_INSIDE
G_DEFINE_TYPE(PupVMPlugin, pup_vm_plugin, g_type_module_get_type());
#endif

gboolean pup_vm_plugin_load(GTypeModule *module)
{
	PupVMPlugin *self = PUP_VM_PLUGIN(module);
	g_return_val_if_fail(self->plugin_name && self->plugin_path, FALSE);

	self->module = g_module_open(self->plugin_path, 0);
	if (!self->module)
	{
		//g_debug("Module path: %s", self->plugin_path);
		g_critical("Plugin %s (path: %s) failed to open: %s",
		           self->plugin_name, self->plugin_path, g_module_error());
	}

	if (self->module && (! self->initialised))
	{
		//Load plugin's config
		self->cfg_file = g_key_file_new();
		pup_load_cfg_file(self->cfg_file, self->plugin_name);
		
		//Run the plugin's init func
		PupVMPluginInit plugin_init_func;

		if (g_module_symbol(self->module, "pup_vm_plugin_init",
		                    (gpointer *) &plugin_init_func))
		{
			plugin_init_func(self);
		}
		
		self->initialised = TRUE;
	}
	
	if (module) return TRUE;
	else return FALSE;
}

void pup_vm_plugin_unload(GTypeModule *module)
{
	PupVMPlugin *self = PUP_VM_PLUGIN(module);

	if (self->module)
		g_module_close(self->module);
}

static void pup_vm_plugin_class_init(PupVMPluginClass *klass)
{
	GTypeModuleClass *parent_class = G_TYPE_MODULE_CLASS(klass);

	parent_class->load = pup_vm_plugin_load;
	parent_class->unload = pup_vm_plugin_unload;
}

static void pup_vm_plugin_init(PupVMPlugin *self)
{
	//Do nothing
}

PupVMPlugin *pup_vm_plugin_new(const gchar *filename, const gchar *plugin_name)
{
	PupVMPlugin *self = PUP_VM_PLUGIN(g_object_new(PUP_TYPE_VM_PLUGIN, NULL));

	self->plugin_path = g_strdup(filename);
	self->plugin_name = g_strdup(plugin_name);

	g_type_module_use(G_TYPE_MODULE(self)); //This will load the module

	return self;
}

//Settings
PupVMSettings *pup_vm_settings = NULL;

gchar *pup_vm_load_setting_default(GKeyFile *key_file,
                                   const gchar *group,
                                   const gchar *key,
                                   const gchar *default_value)
{
	gchar *value = g_key_file_get_value(key_file, group, key, NULL);
	if (! value)
		value = g_strdup(default_value);

	return value;
}

void pup_vm_settings_new()
{
	PupVMSettings *self = g_new0(PupVMSettings, 1);

	self->cfg_file = g_key_file_new();
	
	//Try to load config file
	pup_load_cfg_file(self->cfg_file, "main");

	//Load config
	self->drive_disp_name = pup_vm_load_setting_default
		(self->cfg_file, "Drives", "DisplayName", "$vendor $model ($sysname)");
	self->volume_disp_name = pup_vm_load_setting_default
		(self->cfg_file, "Volumes", "DisplayName", "$label ($sysname)");
	self->volume_disp_name_fallback = pup_vm_load_setting_default
		(self->cfg_file, "Volumes", "FallbackDisplayName", "$sysname");
	self->volume_mntpnt_name = pup_vm_load_setting_default
		(self->cfg_file, "Volumes", "MountPointName", "$sysname");
	self->volume_mntpnt_name_fallback = pup_vm_load_setting_default
		(self->cfg_file, "Volumes", "FallbackMountPointName", "$sysname");
	self->volume_mount_dir = pup_vm_load_setting_default
		(self->cfg_file, "Volumes", "MountDir", G_DIR_SEPARATOR_S "mnt");
	
	self->plugins = g_hash_table_new(g_str_hash, g_str_equal);

	pup_vm_settings = self;
}

PupVMSettings *pup_vm_settings_get()
{
	if (! pup_vm_settings)
		pup_vm_settings_new();

	return pup_vm_settings;
}

void pup_vm_settings_load_plugins()
{
	PupVMSettings *self = pup_vm_settings_get();
	
	//Load plugins
	gchar *libdirs = g_strdup(g_getenv("LD_LIBRARY_PATH"));
	gint i, n = 0;
	
	if (! libdirs)
		libdirs = g_strdup("/lib:/usr/lib:/usr/local/lib");

	for (i = 0; libdirs[i]; i++)
	{
		if (libdirs[i] != ':') continue;
		
		libdirs[i] = '\0';

		gchar *plugin_dir = g_strconcat(libdirs + n,
		                                G_DIR_SEPARATOR_S PUP_VM_DIR_NAME,
		                                NULL);
		GDir *dir_s = g_dir_open(plugin_dir, 0, NULL);
		if (dir_s)
		{
			gchar *filename;
			while ((filename = (gchar *) g_dir_read_name(dir_s)))
			{
				if (! g_str_has_suffix(filename, "." G_MODULE_SUFFIX))
					continue;
				
				gchar *onename = g_strdup(filename);
				
				int j;
				for (j = 0; onename[j] && (onename[j] != '.'); j++)
					;
				onename[j] = '\0';

				if (! g_hash_table_lookup(self->plugins, onename))
				{
					gchar *plugin_path = g_strconcat(plugin_dir,
						                             G_DIR_SEPARATOR_S,
						                             filename,
						                             NULL);
					PupVMPlugin *plugin = pup_vm_plugin_new
						(plugin_path, onename);
					g_free(plugin_path);
					g_hash_table_insert(self->plugins, onename,
						                plugin);
				}
				else
					g_free(onename);
			
			}
			g_dir_close(dir_s);
		}
		g_free(plugin_dir);
		
		n = i + 1;
	}

	g_free(libdirs);
	
}

//Environment-based settings

static gchar *pup_svr_sock_path = NULL;

const gchar *pup_get_svr_sock_path()
{
	if (! pup_svr_sock_path)
	{
		const gchar *ipath = g_getenv("PUP_VOLUME_MONITOR_SOCKET_PATH");
		if (ipath)
		{
			pup_svr_sock_path = g_strdup(ipath);
		}
		else
		{
			pup_svr_sock_path = g_strdup(PUP_VM_SOCK);
		}
	}
	return pup_svr_sock_path;		
}

//Disabled, pup-volume-monitor now uses udev netlink socket
/*
static gchar *pup_udev_sock_path = NULL;

const gchar *pup_get_udev_sock_path()
{
	if (! pup_udev_sock_path)
	{
		const gchar *ipath = g_getenv("PUP_VOLUME_MONITOR_UDEV_SOCKET_PATH");
		if (ipath)
		{
			pup_udev_sock_path = g_strdup(ipath);
		}
		else
		{
			pup_udev_sock_path = g_strdup(PUP_VM_UDEV_SOCK);
		}
	}
	return pup_udev_sock_path;		
}
*/

