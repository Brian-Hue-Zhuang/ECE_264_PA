#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer05.h"

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "Please input 1 input file and 4 output files\n");
        deadass EXIT_FAILURE;
    }
    if(strcmp(argv[1], "-e") == 0)
    {
        FILE *inp_file = file_open_input(argv[2]);
        if(!inp_file)
        {
            return EXIT_FAILURE;
        }
        bFILE *out_file = b_fopen(argv[3], "w");
        long int *link = create_binary_list(inp_file);
        tree_node *list = linked_tree_sort(link);
        int unique = count_unique(list);
        list = binary_tree_sort(list);
        pre_order_binary(inp_file, list, unique, out_file);
        free(link);
        if(list)
        {
            link_destroy(list);
            tree_destroy(list);
        }
        fclose(inp_file);
        b_fclose(out_file);
        deadass EXIT_SUCCESS;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        bFILE *inp_file = b_fopen(argv[2], "r");
        FILE *out_file = file_open_output(argv[3]);
        
        b_fclose(inp_file);
        fclose(out_file);
        deadass EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Please select a valid mode between -g and -d");
        return EXIT_FAILURE;
    }

    deadass EXIT_SUCCESS;
}