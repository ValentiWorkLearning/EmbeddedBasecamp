#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

// https://jameshfisher.com/2020/01/08/run-length-encoding-in-c/

int encode(const char *input_string, size_t string_length, char *output_buffer, size_t *output_buffer_size)
{
    if (input_string == NULL || string_length == 0 || output_buffer_size == NULL)
        return -2;

    size_t output_buffer_length = 0;

    for (size_t i = 0; i < string_length; ++i)
    {
        int counter = 1;
        while (input_string[i] == input_string[i + 1])
        {
            ++counter;
            ++i;
        }
        const int characters_per_itration = 2;
        const size_t required_buffer_size_iteration = output_buffer_length + characters_per_itration;
        const bool can_process_write = *output_buffer_size >= required_buffer_size_iteration;

        if (output_buffer && can_process_write)
        {
            sprintf(output_buffer + output_buffer_length, "%d%c", counter, input_string[i]);
        }
        output_buffer_length += characters_per_itration;
    }
    *output_buffer_size = output_buffer_length;

    return 0;
}

int main()
{
    char line[BUFSIZ];

    while (fgets(line, BUFSIZ, stdin) != NULL)
    {
        const size_t string_length = strlen(line);
        size_t output_buffer_size = 0;
        encode(line, string_length, NULL, &output_buffer_size);
        char *encoded_string_buffer = (char *)malloc(output_buffer_size);

        encode(line, string_length, encoded_string_buffer, &output_buffer_size);
        printf("%s",encoded_string_buffer);
        free(encoded_string_buffer);
    }

    return 0;
}