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
#define PUP_TYPE_VM_PLUGIN (pup_vm_plugin_get_type())
#define PUP_VM_PLUGIN(ptr) \
		(G_TYPE_CHECK_INSTANCE_CAST((ptr), PUP_TYPE_VM_PLUGIN, PupVMPlugin))
#define PUP_IS_VM_PLUGIN(ptr) \
		(G_TYPE_CHECK_INSTANCE_TYPE((ptr), PUP_TYPE_VM_PLUGIN))
#define PUP_VM_PLUGIN_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_CAST((ptr), PUP_TYPE_VM_PLUGIN, PupVMPluginClass))
#define PUP_IS_VM_PLUGIN_CLASS(ptr) \
		(G_TYPE_CHECK_CLASS_TYPE((ptr), PUP_TYPE_VM_PLUGIN))
#define PUP_VM_PLUGIN_GET_CLASS(ptr) \
		(G_TYPE_INSTANCE_GET_CLASS((ptr), PUP_TYPE_VM_PLUGIN, PupVMPluginClass))
#define PUP_VM_PLUGIN_NAME "PupVMPlugin"

typedef void (*PupVMPluginInit) (PupVMPlugin *plugin);
typedef void (*PupVMPluginReconf) (PupVMPlugin *plugin); //Will be implemented later

#endif

/*Everything beyond this is automatically generated*/
const gchar *pup_get_svr_sock_path();
void pup_vm_settings_load_plugins();
PupVMSettings *pup_vm_settings_get();
void pup_vm_settings_new();
gchar *pup_vm_load_setting_default(GKeyFile *key_file,const gchar *group,const gchar *key,const gchar *default_value);
extern PupVMSettings *pup_vm_settings;
PupVMPlugin *pup_vm_plugin_new(const gchar *filename,const gchar *plugin_name);
void pup_vm_plugin_unload(GTypeModule *module);
gboolean pup_vm_plugin_load(GTypeModule *module);
#if !defined(PUP_VM_H_INSIDE)
G_DEFINE_TYPE(PupVMPlugin,pup_vm_plugin,g_type_module_get_type());
#endif
gboolean pup_load_cfg_file(GKeyFile *key_file,const gchar *module);
