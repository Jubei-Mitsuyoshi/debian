//operations.c or operations.h
//Contains code for builtin operations
//Currently "mount" and "umount" is implemented for PupVolume.

#ifndef PUP_VM_H_INSIDE
//operations.c
#include "common.h"

#else // !PUP_VM_H_INSIDE

typedef enum
{
	PUP_EXEC_MOUNT = 0,
	PUP_EXEC_UMOUNT = 1
} PupMountAction;

typedef struct
{
	PupVolume *volume;
	PupVolume *rpl_volume;
	PupMountAction action;
	gchar *event;
	PupOperation *operation;
	gchar *retval;
} PupMountExec;

#endif //PUP_VM_H_INSIDE

//FILE_HEADER_END

//First create command executor
static GThreadPool *mount_umount_thread_pool;

//PupVolume::"mount"
void pup_volume_mount_func(PupDevice *dev, PupOperation *operation)
{
	//Check for immediate failures (whether mounting will fail)
	PupVolume *volume = PUP_VOLUME(dev);
	if (volume->mntpnt)
	{
		pup_operation_return(operation, FALSE, G_IO_ERROR_ALREADY_MOUNTED,
		                     "Volume %s is already mounted at %s",
		                     dev->sysname, volume->mntpnt);
		return;
	}
	if (! (volume->flags & PUP_VOLUME_IS_MOUNTABLE))
	{
		pup_operation_return(operation, FALSE, G_IO_ERROR_NOT_MOUNTABLE_FILE,
		                     "Volume %s cannot be mounted.",
		                     dev->sysname);
		return;
	}
	
	PupMountExec *exec = g_slice_new(PupMountExec);
	exec->volume = volume;
	exec->rpl_volume = PUP_VOLUME(pup_device_dup(dev));
	exec->rpl_volume->mntpnt = pup_volume_gen_mount_dir(volume);
	exec->action = PUP_EXEC_MOUNT;
	exec->event = g_strdup(PUP_VOLUME_EVENT_MOUNT);
	exec->operation = operation;
	exec->retval = g_strdup_printf("mount_point=%s", exec->rpl_volume->mntpnt);

	GError *error = NULL;
	//Launch new thread
	g_thread_pool_push(mount_umount_thread_pool, exec, &error);
	if (error)
	{
		g_critical("Couldn't start new thread: %s", error->message);
	}
}

//PupVolume::"umount"
void pup_volume_umount_func(PupDevice *dev, PupOperation *operation)
{
	//Check for immediate failures (whether unmounting will fail)
	PupVolume *volume = PUP_VOLUME(dev);
	if (! volume->mntpnt)
	{
		pup_operation_return(operation, FALSE, G_IO_ERROR_NOT_MOUNTED,
		                     "Volume %s is not mounted",
		                     dev->sysname);
		return;
	}
	if (volume->flags & PUP_VOLUME_MNTD_SYSTEM)
	{
		pup_operation_return(operation, FALSE, G_IO_ERROR_PERMISSION_DENIED,
		                     "Permission denied: Volume %s is a system volume",
		                     dev->sysname);
		return;
	}
	
	PupMountExec *exec = g_slice_new(PupMountExec);
	exec->volume = volume;
	exec->rpl_volume = PUP_VOLUME(pup_device_dup(dev));
	g_free(exec->rpl_volume->mntpnt);
	exec->rpl_volume->mntpnt = NULL;
	exec->action = PUP_EXEC_UMOUNT;
	exec->event = g_strdup(PUP_VOLUME_EVENT_UMOUNT);
	exec->operation = operation;
	exec->retval = g_strdup_printf(" ");

	GError *error = NULL;
	//Launch new thread
	g_thread_pool_push(mount_umount_thread_pool, exec, &error);
	if (error)
	{
		g_critical("Couldn't start new thread: %s", error->message);
	}
}


//Thread function
static void mount_umount_thread_func(gpointer data, gpointer user_data)
{
	PupMountExec *exec = (PupMountExec *) data;
	GError *error = NULL;
	gchar *output = NULL;
	gint exitcode;

	PupVMMonitor *monitor = PUP_DEVICE(exec->rpl_volume)->monitor;

	//Create directory where the volume is mounted, if needed
	if (exec->rpl_volume->mntpnt)
	{
		g_mkdir_with_parents(exec->rpl_volume->mntpnt,
		                     S_IRWXU | S_IRWXG | S_IRWXO);
	}
		
	if (exec->action)
	{
		pup_vm_launch_script_v(&exitcode, &output, &error,
				PUP_VOLUME_UMOUNT_CMD(exec->rpl_volume));
	}
	else
	{
		pup_vm_launch_script_v(&exitcode, &output, &error,
				PUP_VOLUME_MOUNT_CMD(exec->rpl_volume));
	}
	if (error)
	{
		pup_operation_return(exec->operation, FALSE, G_IO_ERROR_FAILED, 
		                     "Couldn't execute script: %s",
		                     error->message);
		g_clear_error(&error);
	}
	else
	{
		if (exitcode)
		{
			pup_operation_return(exec->operation, FALSE, G_IO_ERROR_FAILED, 
		                         "Error %sing volume %s: "
		                         "Script exited with exitcode %d\n"
								 "Output:\n%s",
			                     exec->event,
		                         PUP_DEVICE(exec->rpl_volume)->sysname,
		                         exitcode,
			                     output);
		}
		else
		{
			//Success
			pup_vm_monitor_copy_back_device(monitor,
			                                PUP_DEVICE(exec->rpl_volume),
			                                exec->event);
			
			pup_operation_return(exec->operation, TRUE, 0, 
		                         "%s",
			                     exec->retval);
		}
	}

	g_free(output);

	//Free data
	g_free(exec->retval);
	g_free(exec->event);
}

void pup_device_destroy_dbg_info(gpointer dev)
{
	//printf("\nDevice freed\n");
	//pup_device_show(PUP_DEVICE(dev));
	//G_BREAKPOINT();
	g_object_unref(dev);
}

//Initialise VTable and others with these functions
void pup_vm_impl_init()
{
	if (! g_thread_supported()) g_thread_init(NULL);
	g_type_init();
	pup_func_call_queue_init();

	pup_vm_is_client = FALSE;
	pup_vm_impl_vtable.device_release_func = pup_device_destroy_dbg_info;
	pup_vm_impl_vtable.volume_mount = pup_volume_mount_func;
	pup_vm_impl_vtable.volume_umount = pup_volume_umount_func;

	mount_umount_thread_pool = g_thread_pool_new(mount_umount_thread_func,
	                                             NULL, -1, FALSE, NULL);
	g_assert(mount_umount_thread_pool);
}

