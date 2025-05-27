#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "answer04.h"

// macros used in Is_BMP_Header_Valid function

#define BMP_TYPE 0x4d42
#define BMP_HEADER_SIZE 54
#define DIB_HEADER_SIZE 40  

BMP_image *read_BMP_image(char *filename)
{
  FILE *fptr = fopen(filename, "rb");
  if(!fptr)
  {
    fprintf(stderr,"Failed to open file\n");
    return NULL;
  }
  rewind(fptr);
  fprintf(stderr,"BM read: Bitmap file confirmed\n");
  BMP_image *bmpimage;
  bmpimage = (BMP_image *)malloc(sizeof(BMP_image));
  if(!bmpimage)
  {
    return NULL;
  }
  fread(&bmpimage->header, sizeof(BMP_header), 1, fptr);
  bmpimage->data = (unsigned char *)malloc(bmpimage->header.imagesize);
  fseek(fptr, bmpimage->header.offset, SEEK_SET);
  fread(bmpimage->data, bmpimage->header.imagesize, 1, fptr);
  fclose(fptr);
  return bmpimage;
}

/* check whether a header is valid
 * assume that header has been read from fptr
 * the position of the indicator of fptr is not certain
 * could be at the beginning of the file, end of the file or 
 * anywhere in the file
 * note that the check is only for this exercise/assignment
 * in general, the format is more complicated
 */

bool is_BMP_header_valid(BMP_header* header, FILE *fptr) 
{
  // Make sure this is a BMP file
  if (header->type != BMP_TYPE)
  {
    return false;
  }
  // skip the two unused reserved fields

  // check the offset from beginning of file to image data
  // essentially the size of the BMP header
  // BMP_HEADER_SIZE for this exercise/assignment
  if (header->offset != BMP_HEADER_SIZE)
  {
    return false;
  }
      
  // check the DIB header size == DIB_HEADER_SIZE
  // For this exercise/assignment
  if (header->DIB_header_size != DIB_HEADER_SIZE)
  {
    return false;
  }

  // Make sure there is only one image plane
  if (header->planes != 1)
  {
    return false;
  }
  // Make sure there is no compression
  if (header->compression != 0)
  {
    return false;
  }

  // skip the test for xresolution, yresolution

  // ncolours and importantcolours should be 0
  if (header->ncolours != 0)
  {
    return false;
  }
  if (header->importantcolours != 0)
  {
    return false;
  }
  
  // Make sure we are getting 24 bits per pixel
  // or 16 bits per pixel
  if (header->bits != 24 && header->bits != 16)
  {
    return false;
  }

  // fill in code to check for file size, image size
  // based on bits, width, and height
  
  // file size check
  int size;
  fseek(fptr, 0L, SEEK_END);
  size = ftell(fptr);
  rewind(fptr);
  if(size != header->size)
  {
    return false;
  }
  
  //image size check
  if(header->imagesize * 8 != (header->width * header->height * header->bits))
  {
    return false;
  }
  return true;
}

int write_BMP_image(char *filename, BMP_image *image)
{
  FILE *fptr = fopen(filename, "wb");
  //Check file
  if(!fptr)
  {
    fprintf(stderr, "invalid file");
    return 0;
  }
  fseek(fptr, 0L, SEEK_SET);
  fwrite(&image->header, sizeof(BMP_header), 1, fptr);
  //Check image
  if(!image->data)
  {
    return 0;
  }
  fwrite(image->data, image->header.imagesize, 1, fptr);
  fclose(fptr);
  return 1;
}

void free_BMP_image(BMP_image *image)
{
  if(!image)
  {
    fprintf(stderr, "so long gay bowser");
    return;
  }
  free(image->data);
  free(image);
}

BMP_image *convert_24_to_16_BMP_image(BMP_image *image)
{
  BMP_image *converted = (BMP_image *)malloc(sizeof(BMP_image));
  if(converted == NULL)
  {
    return NULL;
  }
  converted->header = image->header;
  int inputPadding = (4 - (image->header.width * 3) % 4) % 4;
  int outputPadding = (4 - (converted->header.width * 2) % 4) % 4;
  converted->header.bits = 16;
  converted->header.imagesize = image->header.height * (image->header.width * 2 + outputPadding);
  converted->header.size = converted->header.imagesize + BMP_HEADER_SIZE;
  
  int i, j; 
  uint16_t pixel, r, g, b;
  converted->data = (unsigned char *)malloc(converted->header.imagesize);
  if(!converted->data)
  {
    free(converted);
    return NULL;
  }
  for(i = 0; i < image->header.height; i++) 
  {
    for(j = 0; j < image->header.width; j++) 
    {
      int offset = (j * 3) + (i * (image->header.width * 3 + inputPadding));
      r = image->data[offset + 2] >> 3;
      g = image->data[offset + 1] >> 3;
      b = image->data[offset] >> 3;
      pixel = r << 10 | g << 5 | b;
      int final = (j * 2) + (i * (image->header.width * 2 + outputPadding));
      converted->data[final] = pixel;
      converted->data[final + 1] = (pixel >> 8);
    }
  }
  return converted;
}

BMP_image *convert_16_to_24_BMP_image(BMP_image *image)
{
  BMP_image *converted = (BMP_image *)malloc(sizeof(BMP_image));
  if (converted == NULL)
  {
    return NULL;
  }
  converted->header = image->header;
  int inputPadding = (4 - (image->header.width * 2) % 4) % 4;
  int outputPadding = (4 - (converted->header.width * 3) % 4) % 4;
  converted->header.bits = 24;
  converted->header.imagesize = image->header.height * (image->header.width * 3 + inputPadding);
  converted->header.size = converted->header.imagesize + BMP_HEADER_SIZE;

  int i, j; 
  uint16_t pixel, r, g, b;
  converted->data = (unsigned char *)malloc(converted->header.imagesize);
  if(!converted->data)
  {
    free(converted);
    return NULL;
  }
  for(i = 0; i < image->header.height; i++) 
  {
    for(j = 0; j < image->header.width; j++) 
    {
      int offset = (j * 2) + (i * (image->header.width * 2 + inputPadding));
      pixel = image->data[offset] | (image->data[offset + 1] << 8);
      r = 0x1F & (pixel >> 10);
      g = 0x1F & (pixel >> 5);
      b = 0x1F & pixel;
      int final = (j * 3) + (i * (image->header.width * 3 + outputPadding));
      converted->data[final] = ((b * 255) / 31);
      converted->data[final + 1] = ((g * 255) / 31);
      converted->data[final + 2] = ((r * 255) / 31);
    }
  }
  return converted;
}