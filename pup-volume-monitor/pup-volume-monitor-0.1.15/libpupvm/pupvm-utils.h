//pupvm-utils.c or pupvm-utils.h
//Useful classes...

#ifndef PUP_VM_H_INSIDE
//pupvm-utils.c
#	include "common-includes.h"
#	include <sys/wait.h>

#else // !PUP_VM_H_INSIDE
//pupvm-utils.h

//Utility

#	define PUP_STR_NULL_IS_BLANK(str) ((str) ? (str) : "")

//Data
#	define PUP_VM_SOCK "/tmp/pup_volume_monitor_socket"
#	define PUP_VM_UDEV_SOCK "@/puppy/pup-volume-monitor-events-dest"
#	define PUP_VM_DEF_MNTPNT "/mnt"

///\brief The prototype of the function passed to pup_queue_call_func().
/**  \param instance Value of the _instance_ argument
	 \param user_data Value of the _user_data_ argument
	 */
typedef void (*PupFunc) (gpointer instance, gpointer user_data);

typedef struct 
{
	PupFunc func;
	gpointer instance;
	gpointer user_data;
} PupFuncHook;

typedef enum
{
	//Warning: Changing these values implies protocol change
	PUP_TAG_INVALID = 0,
	PUP_TAG_LISTEN = 1,
	PUP_TAG_LIST = 2,
	PUP_TAG_OPERATION = 3,
	PUP_TAG_REPROBE_MOUNT = 4,
	PUP_TAG_OPERATION_RETURN = 5,
	PUP_TAG_OPERATION_PASSWORD = 6,
	PUP_TAG_OPERATION_QUESTION = 7,
	PUP_TAG_STOP = 8
} PupTags;

//Remote operation (for clients)
typedef struct _PupRemoteOperation PupRemoteOperation;

typedef void (*PupRemoteReturnCB) (PupRemoteOperation *operation,
                                   gboolean success, guint error_code,
                                   const gchar *detail);
typedef void (*PupAskPasswordCB)  (PupRemoteOperation *operation,
                                   const gchar *msg,
                                   GAskPasswordFlags flags);
typedef void (*PupAskQuestionCB)  (PupRemoteOperation *operation,
                                   const gchar *question,
                                   const gchar **choices);

struct _PupRemoteOperation
{
	guint catagory;
	gchar *sysname;
	gchar *operation;
	gchar *args;
	PupConv *conv;
	PupRemoteReturnCB return_cb;
	PupAskPasswordCB passwd_cb;
	PupAskQuestionCB question_cb;
};

#define pup_vm_nullify_struct(pointer, type) \
pup_vm_nullify_struct_func(pointer, sizeof(type))

//Icons
///Icon string describing internal harddisk.
#define PUP_ICON_HDD "themed_icon|harddrive|gtk-harddisk|drive-harddisk|gnome-dev-harddisk"
///Icon string describing internal flash disk.
#define PUP_ICON_FLASH "themed_icon|media-flash|gtk-harddisk|drive-harddisk|gnome-dev-harddisk"
///Icon string describing USB flash drives.
#define PUP_ICON_USBFLASH "themed_icon|gnome-dev-removable-usb" \
	"|drive-removable-media" \
	"|gtk-harddisk" \
	"|drive-harddisk" \
	"|gnome-dev-harddisk"
///Icon string describing external USB harddisks.
#define PUP_ICON_USBHDD "themed_icon|gnome-dev-harddisk-usb" \
	"|drive-removable-media" \
	"|gtk-harddisk" \
	"|drive-harddisk" \
	"|gnome-dev-harddisk"
///Icon string describing a generic untyped volume
#define PUP_ICON_GENERIC_VOLUME "themed_icon|gtk-harddisk"
///Icon describing an (empty) optical drive
#define PUP_ICON_OPTICAL_DRIVE "themed_icon|drive-optical"
///Icon string describing CD-ROM disk
#define PUP_ICON_CDROM "themed_icon|media-cdrom|media-optical"
///Icon string describing CD-RW disk
#define PUP_ICON_CDRW "themed_icon|media-cdrw|media-cdrom|media-optical"
///Icon string describing an audio CD
#define PUP_ICON_AUDIOCD "themed_icon|media-cdrom-audio|media-cdrom|media-optical"
///Icon string describing DVD-ROM disk
#define PUP_ICON_DVD "themed_icon|media-dvd|media-optical"
///Icon string describing DVD+/-RW disk
#define PUP_ICON_DVDRW "themed_icon|media-dvdrw|media-dvd|media-optical"

#endif // PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
gboolean pup_vm_launch_script_v(gint *exit_status,gchar **output,GError **error,const gchar *script,...);
gboolean pup_vm_launch_script_cmdline(const gchar *cmdline,gint *exit_status,gchar **output,GError **error);
gboolean pup_vm_spawn_cmd_sync(gchar **argv,gchar **envp,gint *exit_status,gchar **output,GError **error);
void g_cclosure_user_marshal_VOID__POINTER_UINT_STRING(GClosure *closure,GValue *return_value G_GNUC_UNUSED,guint n_param_values,const GValue *param_values,gpointer invocation_hint G_GNUC_UNUSED,gpointer marshal_data);
gboolean pup_drive_test_optical(struct udev_device *dev);
gchar *pup_guess_devnode(struct udev_device *dev);
const gchar *pup_guess_sysname(const gchar *devnode);
void pup_vm_extract_operation_details(PSDataParser *parser,guint *catagory_return,gchar **sysname_return,gchar **operation_return,gchar **args_return,gboolean *error);
void pup_remote_operation_abort(PupConv *conv);
void pup_remote_operation_choice_reply(PupConv *conv,guint choice);
void pup_remote_operation_passwd_reply(PupConv *conv,const gchar *username,const gchar *password,const gchar *domain);
void pup_remote_operation_run(PupRemoteOperation *operation,PupConvMgr *cmgr);
void pup_remote_operation_report_unhandled(PupConv *conv);
void pup_remote_operation_response_cb(PupConv *conv,PSDataParser *rcvd_data,gboolean is_new,gpointer dummy,PupRemoteOperation *operation);
void pup_vm_encode_str_group(PSDataEncoder *encoder,gchar **first_string,gchar **last_string);
gboolean pup_vm_parse_str_group(PSDataParser *parser,gchar **first_string,gchar **last_string);
void pup_vm_encode_tag(PSDataEncoder *encoder,guint tag);
guint pup_vm_extract_tag(PSDataParser *parser,gboolean *error);
void pup_queue_call_func(gpointer instance,PupFunc func,gpointer user_data);
void pup_func_call_queue_init();
gchar *pup_strrpl(const gchar *string,gchar **targets,gchar **substs);
void pup_vm_nullify_struct_func(gpointer data,gsize len);
