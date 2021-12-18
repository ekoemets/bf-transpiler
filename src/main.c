#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "list.h"
#include "parser.h"
#include "bf_node.h"
#include "transpiler.h"
#include "interpretator.h"


static char *read_input_from_file(FILE *input)
{
    size_t buf_size = 20;
    char buffer[buf_size];

    size_t capacity = 1024;
    size_t length = 0;
    char *str = malloc(capacity * sizeof(char));

    if (str == NULL)
    {
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        int buf_length = strlen(buffer);
        if (length + buf_length + 1 > capacity)
        {
            int new_capacity = capacity * 2;
            str = realloc(str, new_capacity * sizeof(char));
            if (str == NULL)
            {
                return NULL;
            }
            capacity = new_capacity;
        }
        // printf("Read from file %d characters", buf_length);
        // Copy buffer to the end of our current word
        strcpy(str + length, buffer);
        length += buf_length;
    }

    return str;
}

static char *read_input(char *input_filename)
{
    char *input;
    FILE *input_file = stdin;

    if (input_filename != NULL)
    {
        // Try to read from file, return NULL if file opening fails
        input_file = fopen(input_filename, "r");
        if (input_file == NULL)
        {
            fprintf(stderr, "Failed to open file '%s'\n", input_filename);
            return NULL;
        }
    }
    input = read_input_from_file(input_file);

    if (input_file != stdin)
        fclose(input_file);

    return input;
}

int main(int argc, char **argv)
{
    int interpret_flag = 0;
    int file_mode_flag = 0;
    char *output_filename = NULL;

    int c;
    while ((c = getopt(argc, argv, "ifo:")) != -1)
    {
        switch (c)
        {
        case 'i':
            interpret_flag = 1;
            break;
        case 'f':
            file_mode_flag = 1;
            break;
        case 'o':
            output_filename = optarg;
            break;
        case '?':
            if (optopt == 'o')
                fprintf(stderr, "Option -%c requires an filename\n", optopt);
            else
                fprintf(stderr, "Unknown option `-%c'\n", optopt);
            return EXIT_FAILURE;
        default:
            abort();
        }
    }
    if (optind >= argc) {
        fprintf(stderr, "Did not find bf program or filename as argument");
        return EXIT_FAILURE;
    }

    char *input;
    char *last_arg = argv[optind];
    if (file_mode_flag) {
        input = read_input(last_arg);
        if (input == NULL)
        {
            fprintf(stderr, "Failed to read bf program from file");
            return EXIT_FAILURE;
        }
    } else {
        input = last_arg;
    }


    struct List *bf_node_list = parse_bf_str(input);
    optimize_bf_list(bf_node_list);
    // Only allocated memory if read from file
    if (file_mode_flag) {
        free(input);
    }

    if (interpret_flag == 1)
    {
        interpret_bf(bf_node_list);
    }
    else
    {
        FILE *output_file = stdout;
        if (output_filename != NULL)
        {
            output_file = fopen(output_filename, "w");
            if (output_filename == NULL)
            {
                fprintf(stderr, "Failed to open file '%s' for output\n", output_filename);
                return EXIT_FAILURE;
            }
        }

        transpile_bf(bf_node_list, output_file);

        if (output_file != stdout) {
            fclose(output_file);
            printf("Successfully printed NASM to '%s'\n", output_filename);
        }
    }

    list_free(bf_node_list);
    return EXIT_SUCCESS;
}