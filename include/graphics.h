#ifndef _GRAPHICS_H
#define _GRAPHICS_H

SDL_Texture* loadTexture(const char *file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void renderTextureFull(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTextureClip(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip);
void renderTextureSheet(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,SDL_Rect *clip);

#endif 