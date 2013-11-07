//protocol.c or protocol.h
//Contains functions to help interpret binary data

#ifndef PUP_VM_H_INSIDE
//protocol.c
#	include "common-includes.h"

#else // !PUP_VM_H_INSIDE
//protocol.h

typedef struct
{
	PupSockData data;
	gsize rw_ptr;
	gboolean free_on_unref;
} PSDataParser;

typedef struct 
{
	GQueue *data_queue;
	gsize len;
} PSDataEncoder;

typedef struct
{
	gint8 *data;
	gsize len;
	gint flags;
} PSEncodeData;

typedef enum 
{
	PS_DATA_ENCODER_ALLOC = 1 << 0,
	PS_DATA_ENCODER_PREPEND = 1 << 1,
	PS_DATA_ENCODER_FREE = 1 << 2,
	PS_DATA_ENCODER_IS_ENCODER = 1 << 3
} PSEncodeDataOptions;

typedef void (*PSDataEncodeFunc) (PSDataEncoder *encoder,
                                  gpointer data, gpointer user_data);

typedef gboolean (*PSDataParseFunc) (PSDataParser *parser,
                                     gpointer *key_return, gpointer *data_return,
                                     gpointer user_data);

typedef gint (*PSDataIterFunc) (PSDataEncoder *encoder, 
                                gpointer data_structure, PSDataEncodeFunc func,
                                gpointer user_data);

typedef gpointer (*PSDataAddFunc) (gpointer data_structure,
                                   gpointer key, gpointer data);

#	define ps_data_parser_parse_var(p, t) \
*((t *) ps_data_parser_parse_next_fixed_block_noalloc(p, sizeof(t)))

#	define ps_data_parser_parse_array(p, t, l) \
(t *) ps_data_parser_parse_next_variable_block \
(p, sizeof(t), l)

#	define ps_data_encoder_add_val(e, v, t, f) \
*((t *) ps_data_encoder_alloc_fixed_block(e, sizeof(t), f)) = v

#	define ps_data_encoder_add_array(e, a, t, l, f) \
ps_data_encoder_add_variable_block \
(e, (gpointer) a, (l) * sizeof(t), f)

#endif // PUP_VM_H_INSIDE

/*Everything beyond this is automatically generated*/
void ps_data_encoder_destroy(PSDataEncoder *encoder);
void ps_data_encoder_queue_send(PSDataEncoder *encoder,PupSock *sock);
void ps_data_encoder_encode(PSDataEncoder *encoder,gpointer dest);
void ps_data_encoder_add_str0(PSDataEncoder *encoder,const gchar *str,gint flags);
void ps_data_encoder_append_complex_array(PSDataEncoder *encoder,gpointer data_structure,PSDataIterFunc iterator,PSDataEncodeFunc encode_func,gpointer user_data);
gint ps_ghashtable_iterator(PSDataEncoder *encoder,gpointer data_structure,PSDataEncodeFunc func,gpointer user_data);
gint ps_gslist_iterator(PSDataEncoder *encoder,gpointer data_structure,PSDataEncodeFunc func,gpointer user_data);
void ps_data_encoder_encode_func_caller(PSDataEncoder *encoder,gpointer data,PSDataEncodeFunc func,gpointer user_data);
void ps_data_encoder_add_encoder(PSDataEncoder *encoder,PSDataEncoder *data,gint flags);
gpointer ps_data_encoder_alloc_variable_block(PSDataEncoder *encoder,gsize len,gint flags);
void ps_data_encoder_add_fixed_block(PSDataEncoder *encoder,gconstpointer data,gsize len,gint flags);
PSDataEncoder *ps_data_encoder_new();
gchar *ps_data_parser_parse_str0(PSDataParser *parser,gboolean *error);
void ps_data_parser_destroy(PSDataParser *parser);
gint ps_data_parser_parse_complex_array(PSDataParser *parser,gpointer *data_structure,PSDataAddFunc add_func,PSDataParseFunc parse_func,gpointer user_data);
gpointer ps_ghashtable_adder(gpointer data_structure,gpointer key,gpointer data);
gpointer ps_gslist_adder(gpointer data_structure,gpointer key,gpointer data);
gpointer ps_data_parser_parse_next_variable_block_noalloc(PSDataParser *parser,gsize element_size,guint *len_return);
gpointer ps_data_parser_parse_next_fixed_block(PSDataParser *parser,gsize size);
PSDataParser *ps_data_parser_new_from_sock_input(PupSock *sock);
PSDataParser *ps_data_parser_new_from_data(PupSockData *data);
PSDataParser *ps_data_parser_new(gpointer data,gsize len,gboolean free_data_on_destroy);
void ps_data_encoder_add_variable_block(PSDataEncoder *encoder,gconstpointer data,gsize len,gint flags);
gpointer ps_data_encoder_alloc_fixed_block(PSDataEncoder *encoder,gsize len,gint flags);
gpointer ps_data_parser_parse_next_variable_block(PSDataParser *parser,gsize element_size,guint *len_return);
gpointer ps_data_parser_parse_next_fixed_block_noalloc(PSDataParser *parser,gsize size);
