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

//FILE_HEADER_END

//Filling structures with zeroes
void pup_vm_nullify_struct_func(gpointer data, gsize len)
{
	int i;
	for (i = 0; i < len; i++)
		((gchar *) data)[i] = 0;
}

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
/**
 \brief Replaces a group of substrings given in _targets_ with the respective
 one in _substs_.
 \param string The original string to operate on
 \param targets NULL-terminated array of strings to replace
 \param substs Array of strings that will replace each instance of string from _targets_,
 an element can be null to delete the string.
 \param return a newly allocated string, free using g_free()
 */ 
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


//Scheduling functions to be called inside main loop

static GAsyncQueue *queue = NULL;
static volatile int should_wakeup;
static GMainContext *context;

static gboolean prepare(GSource *source, gint *timeout)
{
	if (timeout) *timeout = -1;
	g_atomic_int_set(&should_wakeup, 1);
	if (g_async_queue_length(queue) > 0)
		return TRUE;
	else
		return FALSE;
}

static gboolean check(GSource *source)
{
	if (g_async_queue_length(queue) > 0)
		return TRUE;
	else
		return FALSE;
}

static gboolean dispatch(GSource *source, GSourceFunc callback,
                         gpointer user_data)
{
	PupFuncHook *hook;
	while ((hook = (PupFuncHook *) g_async_queue_try_pop(queue)))
	{
		hook->func(hook->instance, hook->user_data);

		g_slice_free(PupFuncHook, hook);
	}

	return TRUE;
}

static GSourceFuncs source_vtable =
{
	prepare,
	check,
	dispatch,
	NULL, NULL, NULL
};

void pup_func_call_queue_init()
{
	if (! queue)
	{
		queue = g_async_queue_new();
		should_wakeup = 1;
		context = g_main_context_default();

		GSource *source = g_source_new(&source_vtable, sizeof(GSource));
		g_source_attach(source, context);
	}
}

/**\brief Schedules a function to be called inside main loop.

 \param instance data passed to _func_ as _instance_
 \param func the function to call
 \param user_data data passed to _func_ as _user_data_
 */
void pup_queue_call_func(gpointer instance, PupFunc func, gpointer user_data)
{
	PupFuncHook *hook = g_slice_new(PupFuncHook);

	hook->func = func;
	hook->instance = instance;
	hook->user_data = user_data;

	g_async_queue_push(queue, hook);

	if (g_atomic_int_compare_and_exchange(&should_wakeup, 1, 0))
	{
		g_main_context_wakeup(context);
	}
}

//Protocols

guint pup_vm_extract_tag(PSDataParser *parser, gboolean *error)
{
	guint16 *ptr = ps_data_parser_parse_next_fixed_block_noalloc(parser,
	                                                           sizeof(guint16));
	if (ptr) return (guint16) *ptr;
	else
	{
		if (error) *error = TRUE;
		return 0;
	}
}

void pup_vm_encode_tag(PSDataEncoder *encoder, guint tag)
{
	ps_data_encoder_add_val(encoder, (guint16) tag, guint16, 0);
}

gboolean pup_vm_parse_str_group(PSDataParser *parser, gchar **first_string,
                                gchar **last_string)
{
	guint i;
	gboolean err_flag = FALSE;

	for (i = 0; &(first_string[i]) <= last_string; i++)
	{
		first_string[i] = ps_data_parser_parse_str0(parser, &err_flag);
	}
	if (err_flag) return FALSE;

	return TRUE;
}

void pup_vm_encode_str_group(PSDataEncoder *encoder, gchar **first_string,
                             gchar **last_string)
{
	guint i;

	for (i = 0; &(first_string[i]) <= last_string; i++)
	{
		ps_data_encoder_add_str0(encoder, first_string[i], PS_DATA_ENCODER_ALLOC);
	}
}

void pup_remote_operation_response_cb(PupConv *conv, PSDataParser *rcvd_data,
                                      gboolean is_new, gpointer dummy,
							    	  PupRemoteOperation *operation)
{
	gboolean error = FALSE;
	guint response_val =  pup_vm_extract_tag(rcvd_data, &error);
	if (error)
	{
		g_critical("An error occured while reading returned value");
	}
	if (response_val == PUP_TAG_OPERATION_RETURN)
	{
		//Operation returned
		guint exit_code = pup_vm_extract_tag(rcvd_data, &error);
		gchar *detail = ps_data_parser_parse_str0(rcvd_data, &error);
		if (error)
		{
			g_critical("An error occured while reading returned value");
		}

		operation->return_cb(operation, !exit_code, exit_code - 1, detail);

		//Free operation
		g_free(detail);
		g_free(operation->sysname);
		g_free(operation->operation);
		g_free(operation->args);
		g_free(operation);
		
		pup_conv_close(conv, PUP_CONV_FREE);
	}
	else if (response_val == PUP_TAG_OPERATION_PASSWORD)
	{
		gchar *msg = ps_data_parser_parse_str0(rcvd_data, &error);
		GAskPasswordFlags flags = pup_vm_extract_tag(rcvd_data, &error);
		if (error)
		{
			g_critical("An error occured while reading returned value");
		}
		if (operation->passwd_cb)
			operation->passwd_cb(operation, msg, flags);
		else
			pup_remote_operation_report_unhandled(conv);
		g_free(msg);
	}
	else if (response_val == PUP_TAG_OPERATION_QUESTION)
	{
		gchar *msg = ps_data_parser_parse_str0(rcvd_data, &error);
		gchar *option_str = ps_data_parser_parse_str0(rcvd_data, &error);
		if (error)
		{
			g_critical("An error occured while reading returned value");
		}
		gchar **options = g_strsplit(option_str, "|", -1);
		if (operation->question_cb)
			operation->question_cb(operation, msg, (const gchar **) options);
		else
			pup_remote_operation_report_unhandled(conv);
		g_strfreev(options);
		g_free(msg);
		g_free(option_str);
	}
	else
		g_critical("Unexpected response ID %d", response_val);
}

void pup_remote_operation_run(PupRemoteOperation *operation, PupConvMgr *cmgr)
{
	g_return_if_fail(operation);
	PSDataEncoder *encoder = ps_data_encoder_new();

	pup_vm_encode_tag(encoder, PUP_TAG_OPERATION);
	pup_vm_encode_tag(encoder, operation->catagory);
	ps_data_encoder_add_str0(encoder, operation->sysname, 0);
	ps_data_encoder_add_str0(encoder, operation->operation, 0);
	ps_data_encoder_add_str0(encoder, operation->args, 0);

	PupConv *conv = pup_conv_mgr_start_conv(cmgr);
	pup_conv_send_message(conv, encoder);
	operation->conv = conv;
	
	pup_conv_set_callback(conv, (PupConvCB) pup_remote_operation_response_cb,
	                      operation);
}

void pup_remote_operation_passwd_reply(PupConv *conv, const gchar *username,
                                       const gchar *password, const gchar *domain)
{
	PSDataEncoder *encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_OPERATION_RESPONSE_PASSWORD);
	ps_data_encoder_add_str0(encoder, username, 0);
	ps_data_encoder_add_str0(encoder, password, 0);
	ps_data_encoder_add_str0(encoder, domain, 0);
	pup_conv_send_message(conv, encoder);
}

void pup_remote_operation_choice_reply(PupConv *conv, guint choice)
{
	PSDataEncoder *encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_OPERATION_RESPONSE_SHIFT + choice);
	pup_conv_send_message(conv, encoder);
}

void pup_remote_operation_abort(PupConv *conv)
{
	PSDataEncoder *encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_OPERATION_RESPONSE_ABORT);
	pup_conv_send_message(conv, encoder);
}

void pup_remote_operation_report_unhandled(PupConv *conv)
{
	PSDataEncoder *encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_OPERATION_RESPONSE_UNHANDLED);
	pup_conv_send_message(conv, encoder);
}

void pup_vm_extract_operation_details(PSDataParser *parser,
                                      guint *catagory_return,
                                      gchar **sysname_return,
                                      gchar **operation_return,
                                      gchar **args_return,
                                      gboolean *error)
{
	*catagory_return = pup_vm_extract_tag(parser, error);
	*sysname_return = ps_data_parser_parse_str0(parser, error);
	*operation_return = ps_data_parser_parse_str0(parser, error);
	*args_return = ps_data_parser_parse_str0(parser, error);
}

//udev and plugin utilities
/**\brief tries to guess sysfs name from device node path.
 \param devnode Device node path
 \return a pointer within the string
 */
const gchar *pup_guess_sysname(const gchar *devnode)
{
	int i, d = 0;
	for (i = 0; devnode[i] != '\0'; i++)
	{
		if (devnode[i] == '/') d = i + 1;
	}

	return &(devnode[d]);
}

/**\brief gets device node corresponding to _dev_.

 It first uses udev_device_get_devnode(). If that fails it tries to guess
 device node.
 \param dev a struct udev_device
 \return A newly allocated string, free using g_free()
 */
gchar *pup_guess_devnode(struct udev_device *dev)
{
	const gchar *devnode = udev_device_get_devnode(dev);

	if (! devnode)
	{
		//Try to guess device node
		gchar *guessed_devnode = g_strconcat("/dev/",
		                                     udev_device_get_sysname(dev),
		                                     NULL);
		if (g_file_test(guessed_devnode, G_FILE_TEST_EXISTS))
		    return guessed_devnode;
		else
		{
		    g_free(guessed_devnode);
			return NULL;
		}
	}

	return g_strdup(devnode);
}

/**\brief Checks whether _dev_ represents an optical drive
 \param dev a struct udev_device 
 \return TRUE if _dev_ represents an optical drive
 **/
gboolean pup_drive_test_optical(struct udev_device *dev)
{
	gboolean result = FALSE;

	if (strstr(udev_device_get_sysname(dev), "sr")
	    || strstr(udev_device_get_sysname(dev), "pktcdvd"))
		result = TRUE;

	if (!result)
	{
		const gchar *type = udev_device_get_sysattr_value(dev, "device/type");
		if (type)
		{
			if ((type[0] == '4') || (type[0] == '5'))
				result = TRUE;
		}
	}

	return result;
}

//glib-genmarshal generated marshaller for
//void (*callback) (gpointer instance, gpointer arg1, guint arg2, gchar *arg3, gpointer user_data)
//generated using:
//		glib-genmarshal --body <<< "VOID:POINTER,UINT,STRING"
#ifdef G_ENABLE_DEBUG
#define g_marshal_value_peek_boolean(v)  g_value_get_boolean (v)
#define g_marshal_value_peek_char(v)     g_value_get_char (v)
#define g_marshal_value_peek_uchar(v)    g_value_get_uchar (v)
#define g_marshal_value_peek_int(v)      g_value_get_int (v)
#define g_marshal_value_peek_uint(v)     g_value_get_uint (v)
#define g_marshal_value_peek_long(v)     g_value_get_long (v)
#define g_marshal_value_peek_ulong(v)    g_value_get_ulong (v)
#define g_marshal_value_peek_int64(v)    g_value_get_int64 (v)
#define g_marshal_value_peek_uint64(v)   g_value_get_uint64 (v)
#define g_marshal_value_peek_enum(v)     g_value_get_enum (v)
#define g_marshal_value_peek_flags(v)    g_value_get_flags (v)
#define g_marshal_value_peek_float(v)    g_value_get_float (v)
#define g_marshal_value_peek_double(v)   g_value_get_double (v)
#define g_marshal_value_peek_string(v)   (char*) g_value_get_string (v)
#define g_marshal_value_peek_param(v)    g_value_get_param (v)
#define g_marshal_value_peek_boxed(v)    g_value_get_boxed (v)
#define g_marshal_value_peek_pointer(v)  g_value_get_pointer (v)
#define g_marshal_value_peek_object(v)   g_value_get_object (v)
#define g_marshal_value_peek_variant(v)  g_value_get_variant (v)
#else /* !G_ENABLE_DEBUG */
/* WARNING: This code accesses GValues directly, which is UNSUPPORTED API.
 *          Do not access GValues directly in your code. Instead, use the
 *          g_value_get_*() functions
 */
#define g_marshal_value_peek_boolean(v)  (v)->data[0].v_int
#define g_marshal_value_peek_char(v)     (v)->data[0].v_int
#define g_marshal_value_peek_uchar(v)    (v)->data[0].v_uint
#define g_marshal_value_peek_int(v)      (v)->data[0].v_int
#define g_marshal_value_peek_uint(v)     (v)->data[0].v_uint
#define g_marshal_value_peek_long(v)     (v)->data[0].v_long
#define g_marshal_value_peek_ulong(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_int64(v)    (v)->data[0].v_int64
#define g_marshal_value_peek_uint64(v)   (v)->data[0].v_uint64
#define g_marshal_value_peek_enum(v)     (v)->data[0].v_long
#define g_marshal_value_peek_flags(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_float(v)    (v)->data[0].v_float
#define g_marshal_value_peek_double(v)   (v)->data[0].v_double
#define g_marshal_value_peek_string(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_param(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_boxed(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_pointer(v)  (v)->data[0].v_pointer
#define g_marshal_value_peek_object(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_variant(v)  (v)->data[0].v_pointer
#endif /* !G_ENABLE_DEBUG */


/* VOID:POINTER,UINT,STRING (/dev/stdin:1) */
void
g_cclosure_user_marshal_VOID__POINTER_UINT_STRING (GClosure     *closure,
                                                   GValue       *return_value G_GNUC_UNUSED,
                                                   guint         n_param_values,
                                                   const GValue *param_values,
                                                   gpointer      invocation_hint G_GNUC_UNUSED,
                                                   gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__POINTER_UINT_STRING) (gpointer     data1,
                                                          gpointer     arg_1,
                                                          guint        arg_2,
                                                          gpointer     arg_3,
                                                          gpointer     data2);
  register GMarshalFunc_VOID__POINTER_UINT_STRING callback;
  register GCClosure *cc = (GCClosure*) closure;
  register gpointer data1, data2;

  g_return_if_fail (n_param_values == 4);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__POINTER_UINT_STRING) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_pointer (param_values + 1),
            g_marshal_value_peek_uint (param_values + 2),
            g_marshal_value_peek_string (param_values + 3),
            data2);
}


//command/script launching
typedef struct
{
	GIOChannel *stdout_fd;
	GIOChannel *stderr_fd;
	GString *output;
	GError **error;
	GMainLoop *loop;
	GMainContext *ctx;
	gint exit_status;
	gboolean success;
} Launcher;

static gboolean pup_vm_spawn_output_cb(GIOChannel *source, GIOCondition condition, 
                                Launcher *data)
{
	char buf[256];
	gsize bytes_read;
	GIOStatus stat;
	while ((stat = g_io_channel_read_chars(source, buf, 256, &bytes_read, data->error))
	       == G_IO_STATUS_NORMAL)
	{
		if (!bytes_read)
			break;

		g_string_append_len(data->output, buf, bytes_read);
	}

	if (stat == G_IO_STATUS_ERROR)
		g_main_loop_quit(data->loop);

	return TRUE;
}

static gboolean pup_vm_spawn_wait_cb(GPid pid, gint status, Launcher *data)
{
	data->exit_status = WEXITSTATUS(status);
	g_main_loop_quit(data->loop);

	return FALSE;
}

gboolean pup_vm_spawn_cmd_sync(gchar **argv, gchar **envp,
                               gint *exit_status, gchar **output,
                               GError **error)
{
	Launcher data;
	gint out_fd, err_fd;
	GSource *out_src, *err_src, *child_src;
	GPid pidof_child;

	if (! g_spawn_async_with_pipes(NULL, argv, envp,
	                               G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH,
                                   NULL, NULL, &(pidof_child),
	                               NULL, &out_fd, &err_fd, error))
	{
		return FALSE;
	}

	data.ctx = g_main_context_new();
	data.loop = g_main_loop_new(data.ctx, FALSE);
	
	data.stdout_fd = g_io_channel_unix_new(out_fd);
	g_io_channel_set_flags(data.stdout_fd,
	                       g_io_channel_get_flags(data.stdout_fd) | G_IO_FLAG_NONBLOCK,
	                       NULL);
	out_src = g_io_create_watch(data.stdout_fd, G_IO_IN);
	g_source_attach(out_src, data.ctx);
	g_source_set_callback(out_src, (GSourceFunc) pup_vm_spawn_output_cb, &data,
	                      NULL);

	data.stderr_fd = g_io_channel_unix_new(err_fd);
	g_io_channel_set_flags(data.stderr_fd,
	                       g_io_channel_get_flags(data.stderr_fd) | G_IO_FLAG_NONBLOCK,
	                       NULL);
	err_src = g_io_create_watch(data.stderr_fd, G_IO_IN);
	g_source_attach(err_src, data.ctx);
	g_source_set_callback(err_src, (GSourceFunc) pup_vm_spawn_output_cb, &data,
	                      NULL);

	child_src = g_child_watch_source_new(pidof_child);
	g_source_attach(child_src, data.ctx);
	g_source_set_callback(child_src, (GSourceFunc) pup_vm_spawn_wait_cb, &data,
	                      NULL);

	data.success = TRUE;
	data.output = g_string_new(NULL);
	data.error = error;

	g_main_loop_run(data.loop);

	if (data.success)
	{
		if (exit_status) *exit_status = data.exit_status;

		if (output)
			*output = g_string_free(data.output, FALSE);
		else
			g_string_free(data.output, TRUE);
	}
	else
		g_string_free(data.output, TRUE);

	g_source_destroy(out_src);
	g_source_destroy(err_src);
	g_source_destroy(child_src);

	g_main_loop_unref(data.loop);
	g_main_context_unref(data.ctx);

	g_spawn_close_pid(pidof_child);
	
	return data.success;
}

gboolean pup_vm_launch_script_cmdline(const gchar *cmdline, gint *exit_status, 
                                      gchar **output, GError **error)
{
	gint argcp;
	gchar **argvi, **argvf;
	gboolean res;
	gint i;

	//Extract argument vector
	if (! g_shell_parse_argv(cmdline, &argcp, &argvi, error))
		return FALSE;

	//Null-termination
	argvf = g_new(gchar *, argcp + 1);
	for (i = 1; i < argcp; i++)
		argvf[i] = argvi[i];
	argvf[i] = NULL;
	
	//Prepare script path
	if (argcp > 0)
	{
		argvf[0] = g_strconcat(PUP_VM_SCRIPTS_DIR G_DIR_SEPARATOR_S, argvi[0],
		                       NULL);
		g_free(argvi[0]);
	}
	g_free(argvi);

	res = pup_vm_spawn_cmd_sync(argvf, NULL, exit_status, output, error);

	g_strfreev(argvf);

	return res;
}

gboolean pup_vm_launch_script_v(gint *exit_status, 
                                gchar **output, GError **error,
                                const gchar *script, ...)
{
	gchar **argv, *argv0, *argvn;
	va_list v;
	gboolean res;
	GArray *argv_ar = g_array_new(FALSE, FALSE, sizeof(void *));
	
	//Prepare script path
	argv0 = g_strconcat(PUP_VM_SCRIPTS_DIR G_DIR_SEPARATOR_S, script, NULL);
	g_array_append_val(argv_ar, argv0);
	
	//Read arguments
	va_start(v, script);
	while ((argvn = va_arg(v, gchar *)))
		g_array_append_val(argv_ar, argvn);
	
	//Zero-termination
	g_array_append_val(argv_ar, argvn);
	
	//Finish the argument array
	argv = (gchar **) g_array_free(argv_ar, FALSE);
	
	//Call...
	res = pup_vm_spawn_cmd_sync(argv, NULL, exit_status, output, error);
	
	//Cleanup
	g_free(argv0);
	g_free(argv);
	
	return res;
}
