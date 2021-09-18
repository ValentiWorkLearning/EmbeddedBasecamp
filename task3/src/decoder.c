#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int decode(const char *input_string, size_t string_length)
{
    if (input_string == NULL || string_length == 0)
        return -2;

    for (size_t i = 0; i < string_length - 1; i+=2)
    {
        char current_char = input_string[i+1];
        int characters_count = (int)(input_string[i]) - '0';
        while(characters_count > 0)
        {
            putchar(current_char);
            --characters_count;
        }
    }
    return 0;
}

int main()
{
    char line[BUFSIZ];

    while (fgets(line, BUFSIZ, stdin) != NULL)
    {
        const size_t string_length = strlen(line);
        decode(line, string_length);
    }

    return 0;
}