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

//FILE_HEADER_END



PSDataParser *ps_data_parser_new(gpointer data, gsize len,
                                 gboolean free_data_on_destroy)
{
	PSDataParser *retval = g_slice_new(PSDataParser);
	retval->data.data = (gint8 *) data;
	retval->data.len = len;
	retval->rw_ptr = 0;
	retval->free_on_unref = free_data_on_destroy;

	return retval;
}

PSDataParser *ps_data_parser_new_from_data(PupSockData *data)
{
	if (! data) return NULL;
	else return ps_data_parser_new(data->data, data->len, FALSE);
}

PSDataParser *ps_data_parser_new_from_sock_input(PupSock *sock)
{
	PupSockData data;
	PSDataParser *retval = ps_data_parser_new_from_data
		(pup_sock_pop_recvd_block(sock, &data));
	if (retval) retval->free_on_unref = TRUE;

	return retval;
}

gpointer ps_data_parser_parse_next_fixed_block_noalloc
(PSDataParser *parser, gsize size)
{
	gpointer retval;
	//Assertion
	g_return_val_if_fail(parser, NULL);

	if (parser->rw_ptr + size > parser->data.len)
		return NULL;

	//Now copy
	retval = &(parser->data.data[parser->rw_ptr]);
	parser->rw_ptr += size;
	return retval;
}

gpointer ps_data_parser_parse_next_fixed_block(PSDataParser *parser,
                                               gsize size)
{
	gpointer retval = ps_data_parser_parse_next_fixed_block_noalloc
		(parser, size);
	if (retval) retval = g_memdup(retval, size);
	return retval;
}

gpointer ps_data_parser_parse_next_variable_block_noalloc
(PSDataParser *parser, gsize element_size, guint *len_return)
{
	PupSockLen *size;
	g_return_val_if_fail(parser, NULL);
	//Get size of the block
	size = ps_data_parser_parse_next_fixed_block_noalloc
		(parser, PUPSOCK_LEN_SIZE);
	if (! size) return NULL;
	if (len_return) *len_return = (guint) (*size) / element_size;
	//Now copy
	return ps_data_parser_parse_next_fixed_block_noalloc
		(parser, (gsize) *size);
}

gpointer ps_data_parser_parse_next_variable_block(PSDataParser *parser,
                                                  gsize element_size,
                                                  guint *len_return)
{
	gsize len;
	gpointer retval = ps_data_parser_parse_next_variable_block_noalloc
		(parser, element_size, &len);
	if (retval) retval = g_memdup(retval, element_size * len);
	if (len_return) *len_return = len;
	return retval;
}

gpointer ps_gslist_adder(gpointer data_structure, gpointer key,
                         gpointer data)
{
	return g_slist_prepend((GSList *) data_structure, data);
}

gpointer ps_ghashtable_adder(gpointer data_structure, gpointer key,
                             gpointer data)
{
	g_hash_table_insert((GHashTable *) data_structure, key, data);
	return data_structure;
}

gint ps_data_parser_parse_complex_array(PSDataParser *parser,
                                        gpointer *data_structure,
                                        PSDataAddFunc add_func,
                                        PSDataParseFunc parse_func,
                                        gpointer user_data)
{
	//Get no. of elements
	PupSockLen *num_elements = ps_data_parser_parse_next_fixed_block_noalloc
		(parser, PUPSOCK_LEN_SIZE);
	g_return_val_if_fail(num_elements, -1);
	gint i;

	for (i = 0; i < *num_elements; i++)
	{
		gpointer key, data, mblock;
		PSDataParser *virtual_parser;
		//Get size of the data
		guint size;
		mblock = ps_data_parser_parse_next_variable_block_noalloc
			(parser, 1, &size);
		g_return_val_if_fail(mblock, -1);
		//Parse the data
		virtual_parser = ps_data_parser_new(mblock, size, FALSE);
		g_warn_if_fail
			(parse_func(virtual_parser, &key, &data, user_data));
		ps_data_parser_destroy(virtual_parser);
		//Add the data to the data structure
		*data_structure = add_func(*data_structure, key, data);
	}

	return (gint) *num_elements;
}

gchar *ps_data_parser_parse_str0(PSDataParser *parser, gboolean *error)
{
	gchar *str;
	PupSockLen *size;
	if (error)
		if (*error) return NULL; //Easy error handling
	size = (PupSockLen *) ps_data_parser_parse_next_fixed_block_noalloc
		(parser, PUPSOCK_LEN_SIZE);
	if (! size)
	{
		if (error) *error = TRUE;
		return NULL;
	}
	if (! (*size)) return NULL;

	str = ps_data_parser_parse_next_fixed_block(parser, (gsize) *size);
	if (! str)
	{
		if (error) *error = TRUE;
		return NULL;
	}
	return str;
}

void ps_data_parser_destroy(PSDataParser *parser)
{
	g_return_if_fail(parser);
	if (parser->data.data && parser->free_on_unref)
		g_free((gpointer) parser->data.data);
	g_slice_free(PSDataParser, parser);
}

PSDataEncoder *ps_data_encoder_new()
{
	PSDataEncoder *encoder = g_slice_new(PSDataEncoder);
	encoder->len = 0;
	encoder->data_queue = g_queue_new();

	return encoder;
}

void ps_data_encoder_add_fixed_block(PSDataEncoder *encoder,
                                     gconstpointer data, gsize len, gint flags)
{
	PSEncodeData *block = g_slice_new(PSEncodeData);
	block->data = (flags & PS_DATA_ENCODER_ALLOC ? g_memdup(data, len)
	               : (gpointer) data);
	block->len = len;
	block->flags = (flags & (PS_DATA_ENCODER_ALLOC
	                         | PS_DATA_ENCODER_FREE)) ? PS_DATA_ENCODER_FREE : 0;
	if (flags & PS_DATA_ENCODER_PREPEND)
		g_queue_push_head(encoder->data_queue, block);
	else
		g_queue_push_tail(encoder->data_queue, block);
	encoder->len += len;
}

void ps_data_encoder_add_variable_block(PSDataEncoder *encoder,
                                        gconstpointer data, gsize len, gint flags)
{
	PupSockLen plen = (PupSockLen) len;
	if (flags & PS_DATA_ENCODER_PREPEND)
		ps_data_encoder_add_fixed_block(encoder, data, len, flags);
	ps_data_encoder_add_fixed_block(encoder, &plen, PUPSOCK_LEN_SIZE,
	                                flags | PS_DATA_ENCODER_ALLOC);
	if (!(flags & PS_DATA_ENCODER_PREPEND))
		ps_data_encoder_add_fixed_block(encoder, data, len, flags);
}

gpointer ps_data_encoder_alloc_fixed_block(PSDataEncoder *encoder,
                                           gsize len, gint flags)
{
	PSEncodeData *block = g_slice_new(PSEncodeData);
	block->data = g_malloc(len);
	block->len = len;
	block->flags = PS_DATA_ENCODER_FREE;
	if (flags & PS_DATA_ENCODER_PREPEND)
		g_queue_push_head(encoder->data_queue, block);
	else
		g_queue_push_tail(encoder->data_queue, block);
	encoder->len += len;

	return block->data;
}

gpointer ps_data_encoder_alloc_variable_block(PSDataEncoder *encoder,
                                              gsize len, gint flags)
{
	gpointer retval;
	PupSockLen plen = (PupSockLen) len;
	if (flags & PS_DATA_ENCODER_PREPEND)
		retval = ps_data_encoder_alloc_fixed_block(encoder, len, flags);
	ps_data_encoder_add_fixed_block(encoder, &plen, PUPSOCK_LEN_SIZE, flags);
	if (!(flags & PS_DATA_ENCODER_PREPEND))
		retval = ps_data_encoder_alloc_fixed_block(encoder, len, flags);

	return retval;
}

void ps_data_encoder_add_encoder(PSDataEncoder *encoder, PSDataEncoder *data,
                                 gint flags)
{
	PSEncodeData *block = g_slice_new(PSEncodeData);
	block->data = (gint8 *) data;
	block->len = data->len;
	block->flags = PS_DATA_ENCODER_IS_ENCODER | (flags & PS_DATA_ENCODER_FREE);
	if (flags & PS_DATA_ENCODER_PREPEND)
		g_queue_push_head(encoder->data_queue, block);
	else
		g_queue_push_tail(encoder->data_queue, block);
	encoder->len += data->len;
}

void ps_data_encoder_encode_func_caller(PSDataEncoder *encoder,
                                        gpointer data, PSDataEncodeFunc func,
                                        gpointer user_data)
{
	PupSockLen *len_ptr = ps_data_encoder_alloc_fixed_block(encoder,
	                                                        PUPSOCK_LEN_SIZE, 0);
	gsize old_len = encoder->len;
	func(encoder, data, user_data);
	*len_ptr = encoder->len - old_len;
}

gint ps_gslist_iterator(PSDataEncoder *encoder, 
                        gpointer data_structure, PSDataEncodeFunc func,
                        gpointer user_data)
{
	gint count = 0;
	GSList *iter;
	for (iter = (GSList *) data_structure; iter; iter = iter->next)
	{
		ps_data_encoder_encode_func_caller(encoder, iter->data,
		                                   func, user_data);
		count++;
	}
	return count;
}

gint ps_ghashtable_iterator(PSDataEncoder *encoder, 
                            gpointer data_structure, PSDataEncodeFunc func,
                            gpointer user_data)
{
	gint count = 0;
	gpointer data;
	GHashTableIter iter;
	g_hash_table_iter_init(&iter, (GHashTable *) data_structure);
	while (g_hash_table_iter_next(&iter, NULL, &data))
	{
		ps_data_encoder_encode_func_caller(encoder, data, func, user_data);
		count++;
	}
	return count;
}

void ps_data_encoder_append_complex_array(PSDataEncoder *encoder,
                                          gpointer data_structure,
                                          PSDataIterFunc iterator,
                                          PSDataEncodeFunc encode_func,
                                          gpointer user_data)
{
	PupSockLen *len_ptr = ps_data_encoder_alloc_fixed_block(encoder,
	                                                        PUPSOCK_LEN_SIZE, 0);
	*len_ptr = (PupSockLen) iterator(encoder, data_structure, encode_func,
	                                 user_data);
}

void ps_data_encoder_add_str0(PSDataEncoder *encoder, const gchar *str,
                              gint flags)
{
	if (!str)
	{
		static PupSockLen size = 0;
		ps_data_encoder_add_fixed_block(encoder, &size, PUPSOCK_LEN_SIZE,
		                                flags);
	}
	else
	{
		gsize size = strlen(str) + 1;
		ps_data_encoder_add_variable_block(encoder, str, size, flags);
	}
}

void ps_data_encoder_encode(PSDataEncoder *encoder, gpointer dest)
{
	gint8 *retval = (gint8 *) dest;
	gsize rw_ptr = 0;
	GList *iter;
	//Copy the data
	for G_LIST_ITER(encoder->data_queue->head, iter)
	{
		PSEncodeData *onedata = (PSEncodeData *) iter->data;
		if (onedata->flags & PS_DATA_ENCODER_IS_ENCODER)
			ps_data_encoder_encode((PSDataEncoder *) onedata->data,
			                       &(retval[rw_ptr]));
		else
			g_memmove(&(retval[rw_ptr]), onedata->data, onedata->len);
		rw_ptr += ((PSEncodeData *) iter->data)->len;
	}

}

void ps_data_encoder_queue_send(PSDataEncoder *encoder, PupSock *sock)
{
	PupSockBuffer *buffer = pup_sock_buffer_new
		(encoder->len + PUPSOCK_LEN_SIZE);
	//Copy the size of the data
	PupSockLen len = (PupSockLen) encoder->len;
	g_memmove(buffer->data, (gpointer) &len, PUPSOCK_LEN_SIZE);
	//Copy the data
	ps_data_encoder_encode(encoder, &(buffer->data[PUPSOCK_LEN_SIZE]));
	pup_sock_queue_buffer(sock, buffer);
}

void ps_data_encoder_destroy(PSDataEncoder *encoder)
{
	GList *iter;
	//Free the data
	for G_LIST_ITER(encoder->data_queue->head, iter)
	{
		if (((PSEncodeData *) iter->data)->flags & PS_DATA_ENCODER_FREE)
		{
			if (((PSEncodeData *) iter->data)->flags
			    & PS_DATA_ENCODER_IS_ENCODER)
				ps_data_encoder_destroy((PSDataEncoder *)
				                        ((PSEncodeData *) iter->data)->data);
			else
				g_free(((PSEncodeData *) iter->data)->data);
		}
		g_slice_free(PSEncodeData, iter->data);
	}
	g_queue_free(encoder->data_queue);
	g_slice_free(PSDataEncoder, encoder);
}

