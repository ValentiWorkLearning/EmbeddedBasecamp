#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ctype.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valentyn Korniienko");
MODULE_DESCRIPTION("A Linux module for the simple strings operations.");
MODULE_VERSION("0.01");

#define BUFFER_SIZE 1024
static char string_processing_buffer[BUFFER_SIZE];
static char string_processing_output_buffer[BUFFER_SIZE];

int reverse_token(const char* source,int source_string_len, char* destination, int write_pos)
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
void reverse_string_words(const char* incoming_string)
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
         reverse_token(token,token_length,string_processing_output_buffer,write_position);
         write_position += token_length;
         string_processing_output_buffer[write_position] = ' ';
         write_position += 1;
     }
    string_processing_output_buffer[strlen(incoming_string)+1] = '\0';
}

EXPORT_SYMBOL(reverse_string_words);

void to_upper_string(const char* incoming_string)
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
        string_processing_output_buffer[i] = to_write;
    }
    string_processing_output_buffer[source_len+1] = '\0';
}

EXPORT_SYMBOL(to_upper_string);


static int __init string_processor_init(void)
{
    char str[] = "hello for Geeks 12345";
    reverse_string_words(str);
    printk(KERN_INFO "string_processor_init completed\n");
    printk(KERN_INFO "Reversed string is: %s",string_processing_output_buffer);
    to_upper_string(string_processing_output_buffer);
    printk(KERN_INFO "Uppercase string is: %s",string_processing_output_buffer);

    return 0;
}

static void __exit string_processor_exit(void)
{
    printk(KERN_INFO "string_processor_init exit\n");
}

module_init(string_processor_init);
module_exit(string_processor_exit);
