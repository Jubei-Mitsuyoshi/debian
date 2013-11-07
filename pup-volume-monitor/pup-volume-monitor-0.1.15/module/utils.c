//utils.c or utils.h
//Utilities

#ifndef PUP_VM_H_INSIDE
//utils.c
#include "common.h"

#else // !PUP_VM_H_INSIDE
//utils.h

#endif //PUP_VM_H_INSIDE

//FILE_HEADER_END

GIcon *pup_icon_string_to_icon(const gchar *str)
{
	if (! str) return NULL;
	gchar **init_split = g_strsplit_set(str, "|", 2);
	//Assertions
	if (! init_split[0])
	{
		g_strfreev(init_split);
		return NULL;
	}
	if (! init_split[1]) 
	{
		g_strfreev(init_split);
		return NULL;
	}
	if (strcmp(init_split[0], "file_icon") == 0)
	{
		GIcon *icon = g_file_icon_new(g_file_new_for_path(init_split[1]));
		g_strfreev(init_split);
		return icon;
	}
	else if (strcmp(init_split[0], "themed_icon") == 0)
	{
		gchar **icon_names = g_strsplit_set(init_split[1], "|", -1);
		g_strfreev(init_split);
		GIcon *icon = g_themed_icon_new_from_names(icon_names, -1);
		g_strfreev(icon_names);
		return icon;
	}
	else return NULL;
}
