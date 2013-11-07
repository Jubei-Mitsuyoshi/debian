//main.c
//the main program

#include "common.h"

gint main(gint argc, gchar *argv[])
{
	if (argc)
	{
		g_set_prgname(argv[0]);
		g_set_application_name(argv[0]);
	}
	
	//Remove buffering of streams
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	//Initialising all libraries
	pup_vm_impl_init();

	//Check whether server is already running
	if (pup_server_check_is_running())
	{
		g_error("Another instance is already running at address %s",
		        pup_get_svr_sock_path());
	}

	//Load settings
	pup_vm_settings_get();

	//Create new server
	PupServer *server = pup_server_setup();
	//Create new monitor
	server->monitor
		= PUP_SERVER_MONITOR(g_object_new(PUP_TYPE_SERVER_MONITOR, NULL));
	//Load plugins
	pup_vm_settings_load_plugins();
	//Get mount info
	pup_vm_monitor_get_mounts(PUP_VM_MONITOR(server->monitor));

	//Now ready to start
	server->monitor->initialized = TRUE;
	pup_server_monitor_dump_contents(server->monitor);
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	
	pup_server_monitor_start_udev_thread(server->monitor);
	g_timeout_add_seconds(2, pup_server_monitor_mounts_check_func, server->monitor);

	g_main_loop_run(loop);
	
	return 0;
}

