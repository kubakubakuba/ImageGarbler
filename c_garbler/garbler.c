/*
 * File name: texam.c
 * Date:      2017/01/16 08:43
 * Author:    Jan Faigl
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils.h"
#include "xwin_sdl.h"
#include "save_jpeg.h"
#include "save_png.h"


_Bool process_commands(FILE* fd, unsigned char* image, int image_w, int image_h, _Bool anim);
_Bool image_swap(unsigned char* image, int image_w, int image_h, int x1, int y1, int w, int h, int x2, int y2);

int main(int argc, char *argv[])
{
   const char* image_file = argc > 1 ? argv[1] : NULL;
   const char* instructions_file = argc > 2 ? argv[2] : NULL;
   const char* output_image = argc > 3 ? argv[3] : NULL;
   const _Bool animate = argc > 4 && strcmp(argv[4], "--anim") == 0; //animate is true if clause satisfied
   
   if(!(image_file && instructions_file && output_image)){
      fprintf(stderr, "Call as: %s input_image instructions.txt output_image [--anim]\n", argv[0]);
      exit(-1);
   }

   _Bool is_jpeg = strstr(output_image, ".jpeg") || strstr(output_image, ".jpg");

   _Bool is_png = strstr(output_image, ".png");

   int img_w, img_h; 
   unsigned char* img = xwin_load_image(image_file, &img_w, &img_h);
   if(!img){
      my_assert(img != NULL, __func__, __LINE__, __FILE__);
   }

   if(animate){ xwin_init(img_w, img_h); }

   FILE* fd = fopen(instructions_file, "r");
   
   if(fd == NULL){
      fprintf(stderr, "ERROR: Cannot open file %s!\n", instructions_file);
      free(img);
      if(animate){ xwin_close(); }
      exit(-1);
   }

   if(!process_commands(fd, img, img_w, img_h, animate)){
      fclose(fd);
      fprintf(stderr, "Cannot process commands!\n");
      exit(-1);
   }

   if(is_jpeg){
      save_image_jpeg(img_w, img_h, img, output_image);
   }
   else if(is_png){
      save_image_png(img_w, img_h, img, output_image);
   }
   else{
      save_image_rgb(img_w, img_h, img, output_image);
   }

   if(animate){
      xwin_close();
   }
   
   free(img);
   fclose(fd);
   return EXIT_SUCCESS;
}

_Bool process_commands(FILE* fd, unsigned char* image, int image_w, int image_h, _Bool anim){
   _Bool ret = EXIT_SUCCESS;
   int x1, x2, y1, y2, w, h;
   char command[3];
   int counter = 0;
   while(!ret && !feof(fd)){
      ++counter;
      int r = fscanf(fd, "%2s %d %d %d %d %d %d", command, &x1, &y1, &w, &h, &x2, &y2);
      if(r == 7 && strcmp(command, "sw") == 0){
         ret = image_swap(image, image_w, image_h, x1, y1, w, h, x2, y2);
         if(anim && counter % 15 == 0){
            xwin_redraw(image_w, image_h, image);
            delay(100);
         }
         continue;
      }
      return EXIT_FAILURE;
   }

   return ret;
}

_Bool image_swap(unsigned char* image, int image_w, int image_h, int x1, int y1, int w, int h, int x2, int y2){
   _Bool ret = EXIT_SUCCESS;
   for(int x = 0; x < w; ++x){
      for(int y = 0; y < h; ++y){
         int x_from = x1 + x;
         int y_from = y1 + y;
         int x_to = x2 + x;
         int y_to = y2 + y;
         if(x_from >= 0 && x_from < image_w && x_to >= 0 && x_to < image_w && y_from >= 0 && y_from < image_h && y_to >= 0 && y_to < image_h){
            for(int i = 0; i < 3; ++i){
               unsigned char temp = image[(y_from * image_w + x_from) * 3 + i];
               image[(y_from * image_w + x_from) * 3 + i] = image[(y_to * image_w + x_to) * 3 + i];
               image[(y_to * image_w + x_to) * 3 + i] = temp;
            }
            continue;
         }
         fprintf(stderr, "ERROR: swap out of bounds!\n");
         return EXIT_FAILURE;
      }
   }
   return ret;
}

/* end of garbler.c */
