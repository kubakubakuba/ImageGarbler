/*
 * File name: xwin_sdl.c
 * Date:      2017/01/16 17:00
 * Author:    Jan Faigl
 */

#include <SDL.h>
#include <SDL_image.h>

#include "xwin_sdl.h"

#include "utils.h"

static SDL_Window *win = NULL;

// - function ----------------------------------------------------------------
int xwin_init(int w, int h)
{
   int r;
   r = SDL_Init(SDL_INIT_VIDEO);
   my_assert(win == NULL, __func__, __LINE__, __FILE__);
   win = SDL_CreateWindow("prp-exam", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
   my_assert(win != NULL, __func__, __LINE__, __FILE__);
   SDL_SetWindowTitle(win, "PRP-EXAM");   
   return r;
}

// - function ----------------------------------------------------------------
void xwin_close()
{
   my_assert(win != NULL, __func__, __LINE__, __FILE__);
   SDL_DestroyWindow(win);
   SDL_Quit();
}

// - function ----------------------------------------------------------------
void xwin_redraw(int w, int h, unsigned char *img) 
{
   my_assert(win && win, __func__, __LINE__, __FILE__);
   SDL_Surface *scr = SDL_GetWindowSurface(win);
   for(int y = 0; y < scr->h; ++y) {
      for(int x = 0; x < scr->w; ++x) {
         const int idx = (y * scr->w + x) * scr->format->BytesPerPixel;
         Uint8 *px = (Uint8*)scr->pixels + idx;
         *(px + scr->format->Rshift / 8) = *(img++);
         *(px + scr->format->Gshift / 8) = *(img++);
         *(px + scr->format->Bshift / 8) = *(img++);
      }
   }
   SDL_UpdateWindowSurface(win);
}

// - function ----------------------------------------------------------------
void delay(int ms) 
{
   SDL_Delay(ms);
}

// - function ----------------------------------------------------------------
void xwin_poll_events(void)
{
   SDL_Event event;
   while (SDL_PollEvent(&event))
      ;
}

// - function ----------------------------------------------------------------
unsigned char *xwin_load_image(const char *filename, int *width, int *height)
{
   unsigned char *ret = NULL;
   SDL_Surface *img = IMG_Load(filename);
   if (img) {
      *width = img->w;
      *height = img->h;
      ret = (unsigned char *)malloc(img->w * img->h * 3 *
            sizeof(unsigned char));
      my_assert(ret != NULL, __func__, __LINE__, __FILE__);
      unsigned char *dst = ret;
      for (int y = 0; y < img->h; ++y) {
         for (int x = 0; x < img->w; ++x) {
            const int idx = (y * img->w + x) * img->format->BytesPerPixel;
            Uint8 *px = (Uint8 *)img->pixels + idx;
            *(dst++) = *(px + img->format->Rshift / 8);
            *(dst++) = *(px + img->format->Gshift / 8);
            *(dst++) = *(px + img->format->Bshift / 8);
         }
      }
      SDL_FreeSurface(img);
   }
   return ret;
}

/* end of xwin_sdl.c */
