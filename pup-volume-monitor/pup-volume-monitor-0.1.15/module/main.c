//main.c
//The actual module

#include "common.h"

//Load module
G_MODULE_EXPORT void g_io_module_load(GIOModule *module)
{
	if (! g_getenv(PUP_VOLUME_MONITOR_IGNORE))
	{
		if (g_getenv(PUP_VOLUME_MONITOR_DEBUG))
			g_warning("Module loading");
		pup_vm_impl_vtable.device_release_func
			= pup_client_device_release_device;
		//Implementation details
		pup_client_monitor_register(module);
		pup_client_device_register(module);
		//GIO
		pup_volume_monitor_register(module);
		pup_client_volume_register(module);
		pup_mount_register(module);
		pup_client_drive_register(module);
		
		g_type_module_use (G_TYPE_MODULE (module));

		if (g_getenv(PUP_VOLUME_MONITOR_DEBUG))
			g_warning("Module loaded successfully");
	}
}

G_MODULE_EXPORT void g_io_module_unload(GIOModule *module)
{
	if (g_getenv("PUP_VOLUME_MONITOR_DEBUG"))
			g_warning("Module unloaded");
}
