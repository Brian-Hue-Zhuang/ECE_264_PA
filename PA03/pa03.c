#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "answer03.h"

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        return EXIT_FAILURE;
    }
    char *endptr;
    errno = 0;
    long length = strtol(argv[2], &endptr, 10);
    long row = strtol(argv[3], &endptr, 10);
    long col = strtol(argv[4], &endptr, 10);
    if(*endptr != '\0' || (length & (length - 1)) != 0 || length <= 0 || (row | col) < 0 || (row | col) > length || errno == ERANGE)
    {
        return EXIT_FAILURE;
    }
    char *file = argv[5];;
    tiling_solution ts;
    ts.floor_length = (int)length;
    ts.row = (int)row;
    ts.column =(int)col;
    ts.floor_content = NULL;
    if(argv[1][0] == '-' && argv[1][1] == 'v' && argv[1][2] == '\0')
    {
        int ctgry = determine_tiling_solution_category(file, &ts);
        if(ts.floor_content != NULL || !ctgry)
        {
            return EXIT_FAILURE;
        }
        fprintf(stdout,"%d\n", ctgry);
        free_2d_array(ts.floor_content, ts.floor_length);
        return EXIT_SUCCESS;
    }
    else if(argv[1][0] == '-' && argv[1][1] == 'g' && argv[1][2] == '\0')
    {
        generate_tiling_solution(&ts);
        if(!ts.floor_content)
        {
            return EXIT_FAILURE;
        }
        if(!save_tiling_solution(file, &ts))
        {
            free_2d_array(ts.floor_content, ts.floor_length);
            return EXIT_FAILURE;
        }
        free_2d_array(ts.floor_content, ts.floor_length);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Invalid mode, please choose either '-g' (generate) or '-v' (verify) mode\n");
        return EXIT_FAILURE;
    }
    fprintf(stderr,"output is reaching end somehow\n");
    return EXIT_FAILURE;
}