#include <stdio.h>
#include <SDL2/SDL.h>
#include <unistd.h>

int FilterEvents(const SDL_Event *event);

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

  SDL_Surface* PrimarySurface = SDL_GetWindowSurface(win);

  //Creating a renderer
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL){
    SDL_DestroyWindow(win);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Event event;

  /* Ignore key events */
  SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
  SDL_EventState(SDL_KEYUP, SDL_IGNORE);

   while ( SDL_WaitEvent(&event) >= 0 ) {

   }
  // sleep(5);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}

int FilterEvents(const SDL_Event *event) {
  static int boycott = 1;

  /* This quit event signals the closing of the window */
  if ( (event->type == SDL_QUIT) && boycott ) {
    printf("Quit event filtered out -- try again.\n");
    boycott = 0;
    return(0);
  }
  if ( event->type == SDL_MOUSEMOTION ) {
    printf("Mouse moved to (%d,%d)\n",
            event->motion.x, event->motion.y);
    return(0);    /* Drop it, we've handled it */
  }
  return(1);
}