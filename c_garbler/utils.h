/*
 * File name: utils.h
 * Date:      2017/01/16 17:00
 * Author:    Jan Faigl
 */

#ifndef __UTILS_H__
#define __UTILS_H__

void my_assert(int r, const char *fcname, int line, const char *fname);

void save_binary(int w, int h, int threshold, int* grid, const char *fname);

/*
 * save img as PPM file to fname
 * img is the whole image as a sequence of the RGB values
 */ 
void save_image_rgb(int w, int h, unsigned char *img, const char *fname);

#endif

/* end of utils.h */
