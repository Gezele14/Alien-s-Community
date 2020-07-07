#include <stdio.h>
#include <SDL2/SDL.h>
#include <unistd.h>

#include <util.h>
#include <graphics.h>

int FilterEvents(const SDL_Event *event);

int main(int args, char **argv){

  //Starting SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  //Opening a Window
  SDL_Window *win = SDL_CreateWindow("Alien's Community", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
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

  char *bgPath = "../assets/images/background.bmp";
  SDL_Texture *BG = loadTexture(bgPath, ren); 

  if(BG == NULL){
    printf("CreateRenderer error: %s\n", SDL_GetError());
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
  }

  //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
	for (int i = 0; i < 3; ++i){
		//First clear the renderer
		SDL_RenderClear(ren);
		//Draw the texture

		//We want to tile our background so draw it 4 times
		renderTexture(BG, ren, 0, 0);

		//Update the screen
		SDL_RenderPresent(ren);
		//Take a quick break after all that hard work
		SDL_Delay(2000);
	}

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
