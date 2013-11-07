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

/*Everything beyond this is automatically generated*/
void pup_vm_impl_init();
void pup_device_destroy_dbg_info(gpointer dev);
void pup_volume_umount_func(PupDevice *dev,PupOperation *operation);
void pup_volume_mount_func(PupDevice *dev,PupOperation *operation);
