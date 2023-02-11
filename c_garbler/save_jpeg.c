/*
 * File name: save_jpeg.c
 * Date:      2017/01/16 17:00
 * Author:    Jan Faigl
 */

#include <stdio.h>
#include <stdlib.h>

#include <jpeglib.h>

#include "save_jpeg.h"
#include "utils.h"

static void jpeg_compress(const unsigned char* src, int src_len, int w, int h, unsigned char** dst, unsigned long *dst_len, int quality, int components);

void save_image_jpeg(int w, int h, unsigned char *img, const char *fname)
{
   unsigned long jsize = 0;
   unsigned char* jbuf = NULL;
   jpeg_compress(img, w*h, w, h, &jbuf, &jsize, 85, 3);
   FILE *fd = fopen(fname, "wb");
   my_assert(fwrite(jbuf, 1, jsize, fd) == jsize, __func__, __LINE__, __FILE__);
   fclose(fd);
   free(jbuf);
}

void jpeg_compress(const unsigned char* src, int src_len, int w, int h, unsigned char** dst, unsigned long *dst_len, int quality, int components)
{
   struct jpeg_compress_struct cinfo;
   struct jpeg_error_mgr jerr;
   cinfo.err = jpeg_std_error(&jerr);
   jpeg_create_compress(&cinfo);
   cinfo.image_width = w;	
   cinfo.image_height = h;
   my_assert(components == 3 || components == 1, __func__, __LINE__, __FILE__);
   if (components == 3) {
      cinfo.input_components = components;
      cinfo.in_color_space = JCS_RGB; 
   } else {
      cinfo.input_components = components;
      cinfo.in_color_space = JCS_GRAYSCALE;
   }
   jpeg_set_defaults(&cinfo);
   jpeg_set_quality(&cinfo, quality, TRUE);
   jpeg_mem_dest (&cinfo, dst, dst_len); 
   JSAMPROW row_pointer[1];
   jpeg_start_compress(&cinfo, TRUE);
   while (cinfo.next_scanline < cinfo.image_height) {
      row_pointer[0] = (unsigned char*)&src[cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
      jpeg_write_scanlines(&cinfo, row_pointer, 1);
   }
   jpeg_finish_compress(&cinfo);
   jpeg_destroy_compress(&cinfo);
}

/* end of save_jpeg.c */
