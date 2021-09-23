#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/slab.h>

#define BUFFER_SIZE 1024
char* string_processing_buffer;

int init_converter_module(void)
{
    string_processing_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (NULL == *string_processing_buffer)
        return -ENOMEM;
    return 0;
}

void deinit_converter_module(void)
{
    if (string_processing_buffer) {
        kfree(string_processing_buffer);
        string_processing_buffer = NULL;
    }
}


static int reverse_token(const char* source,int source_string_len, char* destination, int write_pos)
{
    if(!source || !destination)
        return -1;
    int write_pos_local = write_pos;

    int i;
    for(i = 0; i < source_string_len; ++i)
    {
        destination[write_pos_local+i] = source[source_string_len - i - 1];
    }
    return 0;
}
void reverse_string_words(const char* incoming_string, char* output_buffer)
{
    if(incoming_string == NULL)
        return;
    
    int source_len = strlen(incoming_string);
    if(source_len + 1 > BUFFER_SIZE)
        return;

    strcpy(string_processing_buffer,incoming_string);
    int write_position = 0;
    char* token = NULL;
    char* rest = string_processing_buffer;

     while ((token = strsep(&rest, " ")))
     {
         int token_length = strlen(token); 
         reverse_token(token,token_length,output_buffer,write_position);
         write_position += token_length;
         output_buffer[write_position] = ' ';
         write_position += 1;
     }
    output_buffer[strlen(incoming_string)+1] = '\0';
}

void to_upper_string(const char* incoming_string,char* output_buffer)
{
    if(incoming_string == NULL)
        return;
    int source_len = strlen(incoming_string);
    if(source_len + 1 > BUFFER_SIZE)
        return;

    int i;
    for(i = 0; i< source_len; ++i)
    {
        char char_it = incoming_string[i];
        char to_write = isdigit(char_it)? char_it : toupper(char_it);
        output_buffer[i] = to_write;
    }
    output_buffer[source_len+1] = '\0';
}
