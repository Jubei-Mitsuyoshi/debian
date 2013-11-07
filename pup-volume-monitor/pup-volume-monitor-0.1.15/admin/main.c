//main.c
//The admin program, used to monitor and control daemon

#include "common.h"
#include <config.h>

gboolean throw_error(GError *error, gboolean terminate)
{
	if (! error) return FALSE;
	
	fprintf(stderr, "An error occurred (Domain: %s): %s\nCode: %d\n",
	        g_quark_to_string(error->domain), error->message, 
	        error->code);
	if (terminate)
		exit(1);

	return TRUE;
}

//pinging the volume monitor
int ping_monitor()
{
	PupSock *sock;
	GError *error = NULL;
	
	//Connect to the daemon, then disconnect
	sock = pup_sock_new_local(NULL);
	pup_sock_connect_local(sock, pup_get_svr_sock_path(), &error);
	g_object_unref(sock);
	
	if (!error)
		return 0;
	
	g_clear_error(&error);
	return 1;
}

void start()
{
	pid_t pid;
	
	pid = vfork();
	if (pid == 0)
	{
		//Warning: This is vfork not fork, memory is shared with parent
		
		gchar *dir, *tmp;
		int filedes, oldflags;
		
		//Create log file
		dir = g_build_filename(g_get_tmp_dir(), "pup-volume-monitor-log", NULL);
		g_mkdir_with_parents(dir, S_IRWXU | S_IRWXG | S_IRWXO);
		tmp = g_build_filename(dir, "XXXXXX.log", NULL);
		g_free(dir);
		filedes = g_mkstemp(tmp);
		g_free(tmp);
		
		if (filedes < 0)
		{
			//We can't use stderr
			const char *msg = "Error creating temporary file\n";
			int len = strlen(msg);
			write(2, (void *) msg, len);
		}
		
		//Flush output on every write 
		oldflags = fcntl(filedes, F_GETFD, 0);
		fcntl(filedes, F_SETFD, oldflags | O_FSYNC);
		
		//Attach it to fds 1 & 2
		dup2(filedes, 1);
		dup2(filedes, 2);
		
		//Start pup-volume-monitor
		execlp("pup-volume-monitor", "pup-volume-monitor", NULL);
		
		//pup-volume-monitor didn't execute
		{
			const char *msg = "Error executing pup-volume-monitor\n";
			int len = strlen(msg);
			write(2, (void *) msg, len);
		}
		
		//The best solution to overpopulation: commit suicide
		raise(SIGKILL);
	}
}

void stop()
{
	PupSock *sock;
	PupConvMgr *cmgr;
	PupConv *conv;
	GError *error = NULL;
	PSDataEncoder *encoder;

	//Connect to the daemon
	sock = pup_sock_new_local(NULL);
	pup_sock_connect_local(sock, pup_get_svr_sock_path(), &error);
	throw_error(error, TRUE);
	cmgr = pup_conv_mgr_new(sock, NULL, NULL);
	conv = pup_conv_mgr_start_conv(cmgr);
	//Prepare request
	encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_TAG_STOP);
	//Send request
	pup_conv_send_message(conv, encoder);
	ps_data_encoder_destroy(encoder);
	pup_sock_send(sock, -1.0, &error);
	throw_error(error, TRUE);
	//Close connection
	g_object_unref(cmgr);
}

//Getting list of all devices
void drv_list_cb(PupConv *conv, PSDataParser *rcvd_data,
                 gboolean is_new, gpointer user_data, gpointer conv_user_data)
{
	
	GSList *list = NULL, *iter;
	puts("Drives:");
	ps_data_parser_parse_complex_array(rcvd_data, (gpointer *) &list,
	                                   ps_gslist_adder, pup_device_parse, NULL);
	for (iter = list; iter; iter = iter->next)
	{
		pup_device_show(PUP_DEVICE(iter->data));
		g_object_unref(iter->data);
	}
	g_slist_free(list);
	list = NULL;
	puts("\nVolumes:");
	ps_data_parser_parse_complex_array(rcvd_data, (gpointer *) &list,
	                                   ps_gslist_adder, pup_device_parse, NULL);
	for (iter = list; iter; iter = iter->next)
	{
		pup_device_show(PUP_DEVICE(iter->data));
		g_object_unref(iter->data);
	}
	g_slist_free(list);
	
	
}


void list_drvs()
{
	PupSock *sock;
	PupConvMgr *cmgr;
	PupConv *conv;
	GError *error = NULL;
	PSDataEncoder *encoder;

	//Connect to the daemon
	sock = pup_sock_new_local(NULL);
	pup_sock_connect_local(sock, pup_get_svr_sock_path(), &error);
	throw_error(error, TRUE);
	cmgr = pup_conv_mgr_new(sock, drv_list_cb, NULL);
	conv = pup_conv_mgr_start_conv(cmgr);
	//Prepare request
	encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_TAG_LIST);
	//Send request
	pup_conv_send_message(conv, encoder);
	ps_data_encoder_destroy(encoder);
	pup_sock_send(sock, -1.0, &error);
	throw_error(error, TRUE);
	//Recieve data
	pup_sock_receive(sock, -1.0, 2, &error);
	throw_error(error, TRUE);
	//Close connection
	g_object_unref(cmgr);
}

//Listening to events
const gchar *pup_event_strv[] = {"none", "add", "remove", "change"};

void drv_event_cb(PupConv *conv, PSDataParser *rcvd_data,
                  gboolean is_new, gpointer user_data, gpointer conv_user_data)
{
	PupVMEvent event;
	PupDevice *dev;

	g_return_if_fail(pup_device_parse_event(rcvd_data, &event));
	g_return_if_fail((dev = pup_device_new_from_header(&(event.header), rcvd_data)));
	
	printf("Event: %s; detail=%s\n", pup_event_strv[event.action], event.detail);
	pup_device_show(dev);
	g_object_unref(dev);
	pup_vm_event_free_data(&event, FALSE);
}
void listen_to_events()
{
	PupSock *sock;
	PupConvMgr *cmgr;
	PupConv *conv;
	GError *error = NULL;
	PSDataEncoder *encoder;
	//Connect to the daemon
	sock = pup_sock_new_local(NULL);
	pup_sock_connect_local(sock, pup_get_svr_sock_path(), &error);
	throw_error(error, TRUE);
	cmgr = pup_conv_mgr_new(sock, drv_event_cb, NULL);
	conv = pup_conv_mgr_start_conv(cmgr);
	//Prepare request
	encoder = ps_data_encoder_new();
	pup_vm_encode_tag(encoder, PUP_TAG_LISTEN);
	//Send request
	pup_conv_send_message(conv, encoder);
	ps_data_encoder_destroy(encoder);
	pup_sock_send(sock, -1.0, &error);
	throw_error(error, TRUE);
	//Recieve data
	pup_sock_receive(sock, -1.0, G_MAXUINT, &error);
	throw_error(error, TRUE);
	//Close connection
	g_object_unref(cmgr);
}

//Running operation
typedef struct
{
	PupRemoteOperation parent;
} ClientOperation;

void operation_finished_cb(ClientOperation *operation,
                           gboolean success, guint error_code,
                           const gchar *detail)
{
	if (success)
	{
		printf("Success\n");
	}
	else
	{
		printf("Failed: GIO error code %d\n", error_code);
	}
	if (detail)
	{
		printf("%s\n", detail);
	}
	exit(!success);
}

void run_operation(const gchar *oper_name, const gchar *dev_address,
                   gchar **args)
{
	PupSock *sock;
	PupConvMgr *cmgr;
	GError *error = NULL;
	ClientOperation *operation;
	gchar **strv = g_strsplit(dev_address, "/", 2);
	g_assert(strv);
	if (! strv[0]) g_error("Device address is empty.");
	if (! strv[1]) g_error("You specified only catagory name.");
	//Find catagory ID
	guint catagory;
	if (strcmp(strv[0], "drives") == 0)
		catagory = PUP_CATAGORY_DRIVE;
	else if (strcmp(strv[0], "volumes") == 0)
		catagory = PUP_CATAGORY_VOLUME;
	else g_error("Invalid catagory name \"%s\"", strv[0]);
	//Connect to the daemon
	sock = pup_sock_new_local(NULL);
	pup_sock_connect_local(sock, pup_get_svr_sock_path(), &error);
	throw_error(error, TRUE);
	cmgr = pup_conv_mgr_new(sock, drv_event_cb, NULL);
	//Prepare request
	operation = g_new0(ClientOperation, 1);
	operation->parent.catagory = catagory;
	operation->parent.sysname = g_strdup(strv[1]);
	operation->parent.operation = g_strdup(oper_name);
	operation->parent.args = args ? g_strjoinv(" ", args) : NULL;
	operation->parent.return_cb = (PupRemoteReturnCB) operation_finished_cb;
	pup_remote_operation_run((PupRemoteOperation *) operation, cmgr);
	//Send request
	pup_sock_send(sock, -1.0, &error);
	throw_error(error, TRUE);
	//Recieve data
	pup_sock_receive(sock, -1.0, G_MAXUINT, &error);
	throw_error(error, TRUE);
	//Close connection
	g_object_unref(cmgr);
}

int main(int argc, char *argv[])
{
	GError *error = NULL;
	int res = 0;
	
	gboolean option_ping = FALSE;
	gboolean option_start = FALSE;
	gboolean option_stop = FALSE;
	gboolean option_list = FALSE;
	gboolean option_listen = FALSE;
	gboolean option_version = FALSE;
	gchar *option_operation_string = NULL;
	gchar **operation_args = NULL;
	gchar *dev_address = NULL;

	g_type_init();

	GOptionEntry arglist[] =
	{
		{"ping", 'p', 0, G_OPTION_ARG_NONE, &option_ping,
			"If backend is running exit with exitcode 0, else 1", NULL},
		{"start", 's', 0, G_OPTION_ARG_NONE, &option_start,
			"Start the backend", NULL},
		{"stop", 'S', 0, G_OPTION_ARG_NONE, &option_stop,
			"Stop the backend", NULL},
		{"list-all", 'l', 0, G_OPTION_ARG_NONE, &option_list,
			"Lists all drives and volumes from backend", NULL},
		{"listen", 'e', 0, G_OPTION_ARG_NONE, &option_listen,
			"Listen to drive connect and remove events", NULL},
		{"operation", 0, 0, G_OPTION_ARG_STRING, &option_operation_string,
			"Run an OPERATION on given device",
			"OPERATION"},
		{"device", 'd', 0, G_OPTION_ARG_STRING, &dev_address,
			"Selects a device to run operation on", "DEVICE"},
		{G_OPTION_REMAINING, 0, 0,  G_OPTION_ARG_STRING_ARRAY,
			&operation_args, NULL, NULL},
		{"version", 'v', 0, G_OPTION_ARG_NONE, &option_version,
			"Print version number and exit", NULL},
		{NULL}
	} ;

	GOptionContext *context;

	g_set_prgname(argv[0]);
	context = g_option_context_new ("");
	g_option_context_set_ignore_unknown_options(context, FALSE);
	g_option_context_add_main_entries(context, arglist, NULL);
	g_option_context_parse(context, &argc, &argv, &error);
	if (error)
	{
		g_error("%s", error->message);
	}

	if (option_ping)
		res = ping_monitor();
	else if (option_start)
		start();
	else if (option_stop)
		stop();
	else if (option_list)
		list_drvs();
	else if (option_listen)
		listen_to_events();
	else if (option_operation_string)
	{
		if (! dev_address)
			g_error("--operation is given but device is not mentioned");
		run_operation(option_operation_string, dev_address, operation_args);
	}
	
	else if (option_version)
		puts(PACKAGE_VERSION);
	else
		fprintf(stderr, "Error: No command specified");

	g_option_context_free(context);
	
	return res;
}


