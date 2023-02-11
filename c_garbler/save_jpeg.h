/*
 * File name: save_jpeg.h
 * Date:      2017/01/16 17:00
 * Author:    Jan Faigl
 */

#ifndef __SAVE_JPEG_H__
#define __SAVE_JPEG_H__

/*
 * save image img to the file fname as jpeg
 * img is a sequence of R,G,B values of the image
 * starting from the top left corner, going from left to right
 */
void save_image_jpeg(int w, int h, unsigned char *img, const char *fname);

#endif

/* end of save_jpeg.h */
