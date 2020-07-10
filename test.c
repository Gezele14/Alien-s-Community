#include <stdio.h>
#include <SDL2/SDL.h>
#include <unistd.h>

int main(int args, char **argv){

  //Starting SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Unable to Init hinting: %s\n", SDL_GetError());
  }

  //Opening a Window
  SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if (win == NULL){
    printf("SDL_CreateWindow Error: %s\n",SDL_GetError());
    SDL_Quit();
    return 1;
  }

  //Creating a renderer
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL){
    SDL_DestroyWindow(win);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Delay(5000);
  // sleep(5);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}