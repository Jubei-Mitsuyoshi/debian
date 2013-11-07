#include <libpupvm/pupvm-plugin.h>
#include <stdio.h>
#include <string.h>

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
	PupSock *sock;
	GError *error = NULL;
	guint32 cnum;
	gchar *buf;
	gsize size;
	gdouble data2;
	PSDataParser *parser;
	PSDataEncoder *encoder;
	PupSockData data;

	g_type_init();
	
	sock = pup_sock_new_local(&error);
	throw_error(error, TRUE);
	
	pup_sock_connect_local(sock, "/tmp/pup_sock_testing", &error);
	throw_error(error, TRUE);
	
	//Get connection number
	pup_sock_receive(sock, -1.0, 1, &error);
	throw_error(error, TRUE);
	
	parser = ps_data_parser_new_from_sock_input(sock);
	cnum = ps_data_parser_parse_var(parser, guint32);
	ps_data_parser_destroy(parser);
	
	//Send message to server
	buf = g_strdup_printf("A little hello from client %d", cnum);
	
	encoder = ps_data_encoder_new();
	ps_data_encoder_add_str0(encoder, buf, PS_DATA_ENCODER_FREE);
	ps_data_encoder_queue_send(encoder, sock);
	ps_data_encoder_destroy(encoder);
	
	g_warn_if_fail(pup_sock_send(sock, -1.0, &error));
	throw_error(error, TRUE);
	
	//Get reply from server
	pup_sock_receive(sock, -1.0, 1, &error);
	throw_error(error, TRUE);
	parser = ps_data_parser_new_from_sock_input(sock);
	buf = ps_data_parser_parse_str0(parser, NULL);
	data2 = ps_data_parser_parse_var(parser, gdouble);
	printf("Server sent reply:\n%s\n%f\n", buf, data2);

	g_object_unref(sock);
	
	//exit
	return 0;
}	
	
	