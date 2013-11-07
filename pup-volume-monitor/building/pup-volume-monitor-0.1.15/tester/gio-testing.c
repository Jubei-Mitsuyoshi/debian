#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>

gboolean throw_error(GError *error, gboolean terminate)
{
	if (! error) return FALSE;
	
	if (terminate)
		g_error("An error occurred (Domain: %s): %s\nCode: %d",
				g_quark_to_string(error->domain), error->message, 
				error->code);
	else
		g_warning("An error occurred (Domain: %s): %s\nCode: %d",
				g_quark_to_string(error->domain), error->message, 
				error->code);
	
	return TRUE;
}

int main(int argc, char *argv[])
{
	GVolumeMonitor *monitor;
	GList *volumes, *iter;
	
	g_type_init();
	
	monitor = g_volume_monitor_get();
	
	volumes = g_volume_monitor_get_volumes(monitor);
	g_assert(volumes != NULL);
	
	//Testing volumes...
	
	for (iter = g_list_first(volumes); iter != NULL; iter = iter->next)
	{
		printf("%s: label=\"%s\", uuid=\"%s\"\n",
				g_volume_get_identifier((GVolume *) iter->data, 
						G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE),
				g_volume_get_identifier((GVolume *) iter->data, 
						G_VOLUME_IDENTIFIER_KIND_LABEL),
				g_volume_get_identifier((GVolume *) iter->data, 
						G_VOLUME_IDENTIFIER_KIND_UUID));
		g_object_unref(iter->data);
		g_object_unref(iter->data); //Torture test
	}
	g_list_free(volumes);
	
	volumes = g_volume_monitor_get_volumes(monitor);
	g_assert(volumes != NULL);
	puts("Testing reuse of objects");
	
	for (iter = g_list_first(volumes); iter != NULL; iter = iter->next)
	{
		printf("%s: label=\"%s\", uuid=\"%s\"\n",
				g_volume_get_identifier((GVolume *) iter->data, 
						G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE),
				g_volume_get_identifier((GVolume *) iter->data, 
						G_VOLUME_IDENTIFIER_KIND_LABEL),
				g_volume_get_identifier((GVolume *) iter->data, 
						G_VOLUME_IDENTIFIER_KIND_UUID));
		g_object_unref(iter->data);
	}
	g_list_free(volumes);
	
	//Testing drives
	GList *drives = g_volume_monitor_get_connected_drives(monitor);
	for (iter = g_list_first(drives); iter != NULL; iter = iter->next)
	{
		GList *iter2;
		printf("%s: \n", g_drive_get_name((GDrive *) iter->data));
		volumes = g_drive_get_volumes((GDrive *) iter->data);
		for (iter2 = g_list_first(volumes); iter2 != NULL; iter2 = iter2->next)
		{
			printf("\t%s: label=\"%s\", uuid=\"%s\"\n",
					g_volume_get_identifier((GVolume *) iter2->data, 
							G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE),
					g_volume_get_identifier((GVolume *) iter2->data, 
							G_VOLUME_IDENTIFIER_KIND_LABEL),
					g_volume_get_identifier((GVolume *) iter2->data, 
							G_VOLUME_IDENTIFIER_KIND_UUID));
			g_object_unref(iter2->data);
			g_object_unref(iter2->data); //Torture test
		}
		g_list_free(volumes);
		
		g_object_unref(iter->data);
	}
	g_list_free(drives);
	
	//Again...
	drives = g_volume_monitor_get_connected_drives(monitor);
	for (iter = g_list_first(drives); iter != NULL; iter = iter->next)
	{
		GList *iter2;
		printf("%s: \n", g_drive_get_name((GDrive *) iter->data));
		volumes = g_drive_get_volumes((GDrive *) iter->data);
		for (iter2 = g_list_first(volumes); iter2 != NULL; iter2 = iter2->next)
		{
			printf("\t%s: label=\"%s\", uuid=\"%s\"\n",
					g_volume_get_identifier((GVolume *) iter2->data, 
							G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE),
					g_volume_get_identifier((GVolume *) iter2->data, 
							G_VOLUME_IDENTIFIER_KIND_LABEL),
					g_volume_get_identifier((GVolume *) iter2->data, 
							G_VOLUME_IDENTIFIER_KIND_UUID));
			g_object_unref(iter2->data);
			g_object_unref(iter2->data);
		}
		g_list_free(volumes);
		
		g_object_unref(iter->data);
	}
	g_list_free(drives);
	
	g_object_unref(G_OBJECT(monitor));
	
	return 0;
}
