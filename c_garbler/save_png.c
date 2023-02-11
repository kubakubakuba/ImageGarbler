#include <stdio.h>
#include <stdlib.h>

#include <png.h>

#include "save_png.h"

void save_image_png(int width, int height, unsigned char *img, const char *fname)
{
   FILE* fp = fopen(fname, "wb");
   if (!fp) {
      printf("[ERROR]: Could not open file for writing\n");
      return;
   }

   png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (!png) {
      printf("[ERROR]: Could not create png write struct\n");
      fclose(fp);
      return;
   }

   png_infop info = png_create_info_struct(png);
   if (!info) {
      printf("[ERROR]: Could not create png info struct\n");
      png_destroy_write_struct(&png, NULL);
      fclose(fp);
      return;
   }

   if(setjmp(png_jmpbuf(png))) {
      printf("[ERROR]: An error occurred while saving the image\n");
      png_destroy_write_struct(&png, &info);
      fclose(fp);
      return;
   }

   png_init_io(png, fp);

   // Set image attributes
   png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   // Allocate memory for image rows
   png_byte** row_pointers = (png_byte**)malloc(sizeof(png_byte*) * height);
   for (int y = 0; y < height; y++) {
      row_pointers[y] = (png_byte*)img + y * width * 3;
   }

   // Write image data
   png_set_rows(png, info, row_pointers);
   png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);

   // Clean up
   free(row_pointers);
   png_destroy_write_struct(&png, &info);
   fclose(fp);
}