#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "answer04.h"

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        return EXIT_FAILURE;
    }

    char *input, *output;

    input = argv[1];
    output = argv[2];

    BMP_image *image = read_BMP_image(input);
    if(!image)
    {
        fprintf(stderr, "Input File: %s is INVALID\n", input);
        return EXIT_FAILURE;
    }
    FILE *fptr;
    fptr = fopen(input, "rb");
    if(!is_BMP_header_valid(&image->header, fptr))
    {
        fprintf(stderr, "Header is invalid\n");
        fclose(fptr);
        return EXIT_FAILURE;
    }
    fclose(fptr);
    BMP_image *temp;
    fprintf(stderr, "%d\n", image->header.bits);
    if(image->header.bits == 16)
    {
        temp = convert_16_to_24_BMP_image(image);
    }
    else if(image->header.bits == 24)
    {
        temp = convert_24_to_16_BMP_image(image);
    }
    else
    {
        fprintf(stderr, "reaches else\n");
        free_BMP_image(image);
        return EXIT_FAILURE;
    }

    if(!write_BMP_image(output, temp))
    {
        fprintf(stderr, "Failed to write image into output file\n");
        free_BMP_image(temp);
        free_BMP_image(image);
        return EXIT_FAILURE;
    }
    free_BMP_image(temp);
    free_BMP_image(image);
    return EXIT_SUCCESS;
}