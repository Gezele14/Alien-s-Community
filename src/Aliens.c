#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <stdlib.h>

#include <util.h>
#include <graphics.h>

const int SCREEN_WIDTH = 1366; //
const int SCREEN_HEIGHT = 720; //
const int TILE_SIZE = 30;

const int xTiles = SCREEN_WIDTH/TILE_SIZE+1;
const int yTiles = SCREEN_HEIGHT/TILE_SIZE;

int map[24][46];

int exitProgram = 0;

SDL_Event e;

int useClip = 0;

//Load Textures
char *bgPath = "../assets/images/map.png";
char *roadPath = "../assets/images/tile2.png";
char *biroadPath = "../assets/images/tile4.png";
char *bridgePath = "../assets/images/tile3.png";
char *alienPath = "../assets/images/alien.png";
char *castle1Path = "../assets/images/castle1.png";
char *castle2Path = "../assets/images/castle2.png";

//Functions
void handleEvents();
void loadMap();

int main(int args, char **argv){

  loadMap();

  //Starting SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    fprintf(stderr, "could not initialize sdl2_image: %s\n", IMG_GetError());
    return 1;
  }

  //Opening a Window
  SDL_Window *win = SDL_CreateWindow("Alien's Community", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

  
  SDL_Texture *BG = loadTexture(bgPath, ren);
  SDL_Texture *BiRoad = loadTexture(biroadPath, ren);
  SDL_Texture *Bridge = loadTexture(bridgePath, ren);
  SDL_Texture *Road = loadTexture(roadPath, ren);
  SDL_Texture *Alien = loadTexture(alienPath,ren); 
  SDL_Texture *Castle1 = loadTexture(castle1Path,ren); 
  SDL_Texture *Castle2 = loadTexture(castle2Path,ren); 

  if(BG == NULL || Alien == NULL){
    printf("CreateRenderer error: %s\n", IMG_GetError());
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
  }

  int iW = 74, iH = 74;
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

  //Setup the clips for our image
	SDL_Rect clips[5];

  //Since our clips our uniform in size we can generate a list of their
	//positions using some math (the specifics of this are covered in the lesson)
	for (int i = 0; i < 4; ++i){
		clips[i].x = 1 * iW;
		clips[i].y = i * iH ;
		clips[i].w = iW;
		clips[i].h = iH;
	}

  int animCounter = 0;
  //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
	while(!exitProgram){
    //Event Handler
    handleEvents();

		//First clear the renderer
		SDL_RenderClear(ren);

    //Draw the background
    renderTexture(BG, ren,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    for(int i=0;i<yTiles;i++){
      for(int j=0;j<xTiles;j++){
        if (map[i][j] == 1){
          renderTexture(Road, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        } else if (map[i][j] == 2){
          renderTexture(BiRoad, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        } else if (map[i][j] == 3){
          renderTexture(Bridge, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
      }
    }

    renderTexture(Castle1, ren,10, 200, 170, 170);
    renderTexture(Castle2, ren,SCREEN_WIDTH-160, 235, 190, 170);

		renderTextureSheet(Alien, ren, x, y, 80, &clips[useClip]);

		//Update the screen
		SDL_RenderPresent(ren);

    SDL_Delay(17);

    animCounter += 1;
    if (animCounter == 10){
      animCounter = 0;
      useClip +=1;
      if (useClip == 4)
        useClip = 0;
    }
	}

  SDL_DestroyTexture(BG);
  SDL_DestroyTexture(Alien);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  IMG_Quit();
  SDL_Quit();

  return 0;
}

void handleEvents(){
  while (SDL_PollEvent(&e)){
    //If user closes the window
    if (e.type == SDL_QUIT){
      exitProgram = 1;
    }
    if (e.type == SDL_KEYDOWN){
      switch (e.key.keysym.sym){
        case SDLK_1:
          useClip = 0;
          break;
        case SDLK_2:
          useClip = 1;
          break;
        case SDLK_3:
          useClip = 2;
          break;
        case SDLK_4:
          useClip = 3;
          break;
        case SDLK_ESCAPE:
          exitProgram = 1;
          break;
        default:
          break;
      }
		}
  }
}

void loadMap(){
  FILE * file;
  file = fopen("../assets/map/map1.map", "r");
  char ch;
  int i = 0;
  int j = 0;
  while ((ch = fgetc(file)) != EOF){
   if(ch == '\n'){
     i += 1;
     j = 0;
   } else if (ch == '-'){
     map[i][j] = 0;
     j += 1;
   } else if (ch == 'x'){
     map[i][j] = 1;
     j += 1;
   } else if (ch == 'o'){
     map[i][j] = 2;
     j += 1;
   } else if (ch == 'p'){
     map[i][j] = 3;
     j += 1;
   }
  }
}