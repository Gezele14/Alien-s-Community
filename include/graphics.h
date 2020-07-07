#ifndef _GRAPHICS_H
#define _GRAPHICS_H

SDL_Texture* loadTexture(const char *file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

#endif 