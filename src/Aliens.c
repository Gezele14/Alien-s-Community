#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <util.h>
#include <graphics.h>
#include <data.h>
#include <linkedlist.h>
#include <lpthread.h>

//Constants
#define SCREEN_WIDTH 1366 
#define SCREEN_HEIGHT 720
#define TILE_SIZE 30
int xTiles = SCREEN_WIDTH/TILE_SIZE+1;
int yTiles = SCREEN_HEIGHT/TILE_SIZE;

//Variables globales
cell map[24][46]; //
int Types[100]; 
int exitProgram = 0; 
int modeManual = 0; 
int useClip = 0;
int baseVel = 5;
int maxTemp = 5;
int toggleInfo = 0;
int autoGenCount = 0;

llist *communityA;
llist *communityB;

llist *eastUp, *eastDown;
llist *centerUp, *centerDown;
llist *westUp, *westDown;

lpthread_mutex_t lock;

//Bringes
bridge East;
bridge Center;
bridge West;

configs Config;

//Functions
int threadAlien();
int delAlien(alien Alien, int x, int y);
int generatorA();
int generatorB();

//Algorithms
int Y_Algorithm(llist* up,llist* down,bridge* p,int Y);
int Semaphore_Algorithm();
int Survival_Algorithm(llist* up,llist* down,bridge* p);


int main(int args, char **argv){

  srand((unsigned)time(NULL));
  //Initialize Mutex
  if (Lmutex_init(&lock, NULL) != 0){ 
    printf("\n Mutex init has failed\n"); 
    return 1; 
  }
  if(getBridgeData(&East, "../config/eastBridge.conf")){
    return 1;
  }
  if(getBridgeData(&Center, "../config/eastBridge.conf")){
    return 1;
  }
  if(getBridgeData(&West, "../config/eastBridge.conf")){
    return 1;
  }
  if(getConfigsData(&Config, "../config/generalConfig.conf")){
    return 1;
  }
  printf("Valor de mean: %d\n",Config.mean);

  for (int i = 0; i < 100; i++){
    if(i>Config.probTypeA){
      Types[i] = 0;
    } else if(i>Config.probTypeB){
      Types[i] = 1;
    } else if(i>Config.probTypeC){
      Types[i] = 2;
    }
  }
  

  //Starting SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  //Starding SDL_Image
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("IMG_Init Error: %s\n", IMG_GetError());
    SDL_Quit();
    return 1;
  }

  //Starding SDL_TTF
	if (TTF_Init() != 0){
		printf("TTF_Init Error: %s\n", SDL_GetError());
    IMG_Quit();
		SDL_Quit();
		return 1;
	}

  //show config Window
  if(configWindow(&baseVel,&maxTemp,&exitProgram,&modeManual)){
    return 1;
  }

  lpthread_t thread1,thread2;
  if(modeManual){
    printf("Se selecciono el modo manual\n");
  } else if(!modeManual){
    Lthread_create(&thread1,NULL,&generatorA,NULL);
    Lthread_create(&thread2,NULL,&generatorB,NULL);
    printf("Se selecciono el modo Auto\n");
  }

  communityA = llist_create(NULL);
  communityB = llist_create(NULL);

  //Colas
  eastUp = llist_create(NULL);
  eastDown = llist_create(NULL);
  centerUp = llist_create(NULL);
  centerDown = llist_create(NULL);
  westUp = llist_create(NULL);
  westDown = llist_create(NULL);

  //Opening a Window
  SDL_Window *win = SDL_CreateWindow("Alien's Community", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (win == NULL){
    printf("SDL_CreateWindow Error: %s\n",SDL_GetError());
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 1;
  }
  //Creating a renderer
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL){
    SDL_DestroyWindow(win);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 1;
  }
    
  SDL_Color fontColor = { 0, 0, 0, 255};
//Load Textures & Fonts
	char * fondoPath = "../assets/images/fondo.png" ;
	char * roadPath = "../assets/images/tile2.png" ;
	char * biroadPath = "../assets/images/tile4.png" ;
	char * bridgePath=  "../assets/images/tile3.png" ;
	char * alienPath = "../assets/images/alien.png" ;
	char * castle1Path = "../assets/images/castle1.png" ;
	char * castle2Path = "../assets/images/castle2.png" ;
	char * iconPath = "../assets/images/icon.png" ;
	char * mainFontPath = "../assets/fonts/font.ttf" ;

  SDL_Texture *BG = loadTexture(fondoPath, ren);
  SDL_Texture *BiRoad = loadTexture(biroadPath, ren);
  SDL_Texture *Bridge = loadTexture(bridgePath, ren);
  SDL_Texture *Road = loadTexture(roadPath, ren);
  SDL_Texture *Alien = loadTexture(alienPath,ren); 
  SDL_Texture *Castle1 = loadTexture(castle1Path,ren); 
  SDL_Texture *Castle2 = loadTexture(castle2Path,ren); 
  
  SDL_Surface *icon = IMG_Load(iconPath);
  SDL_SetWindowIcon(win, icon);

  //Texts
  SDL_Texture *castleA = renderText("Comunidad A", mainFontPath, fontColor, 25, ren);
  SDL_Texture *castleB = renderText("Comunidad B", mainFontPath, fontColor, 25, ren);


  //Setup the clips for Community A
	SDL_Rect clipsA[3][3];
  for (int i = 0; i < 3; i++){
    loadClips(clipsA[i],1,i+1,69,70);
  }

  //Setup the clips for Community B
	SDL_Rect clipsB[3][3];
  for (int i = 0; i < 3; i++){
    loadClips(clipsB[i],2,i+1,69,70);
  }

  int mouseX, mouseY;
  int animCounter = 0;
  SDL_Event e;
  //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
  
	while(!exitProgram){
    autoGenCount++;
    if(autoGenCount == 120){
      continue;
    }
    //Event Handler
    while (SDL_PollEvent(&e)){
    //If user closes the window
    if (e.type == SDL_QUIT){
      exitProgram = 1;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN ){
      SDL_GetMouseState(&mouseX,&mouseY);

      for (int i = 0; i < llist_getSize(communityA); i++){
        alien *DUT = (alien *)llist_getbyId(communityA, i);
        if (delAlien(*DUT,mouseX, mouseY)){
          if(!Lthread_exit(DUT->pid)){
            llist_delById(communityA, i);
          }
          break;
        }
      }

      for (int i = 0; i < llist_getSize(communityB); i++){
        alien *DUT = (alien *)llist_getbyId(communityB, i);
        if (delAlien(*DUT,mouseX, mouseY)){
          if(!Lthread_exit(DUT->pid)){
            llist_delById(communityB, i);
          }
          break;
        }
      }
    }

    if (e.type == SDL_KEYDOWN){
      if(e.key.keysym.sym == SDLK_i){
        toggleInfo = toggleInfo? 0 : 1;
      }
      if(modeManual){
        if(e.key.keysym.sym == SDLK_1){
          alien *Alien = createAlienManual(baseVel,1,0);
          lpthread_t *thread = malloc(sizeof(lpthread_t));
          Lthread_create(thread,NULL,&threadAlien,(void *)Alien);
          Alien->pid = thread->pid;
          llist_addLast(communityA,Alien);
        } else if(e.key.keysym.sym == SDLK_2){
          alien *Alien = createAlienManual(baseVel,1,1);
          lpthread_t *thread = malloc(sizeof(lpthread_t));
          Lthread_create(thread,NULL,&threadAlien,(void *)Alien);
          Alien->pid = thread->pid;
          llist_addLast(communityA,Alien);
        } else if(e.key.keysym.sym == SDLK_3){
          alien *Alien = createAlienManual(baseVel,1,2);
          lpthread_t *thread = malloc(sizeof(lpthread_t));
          Lthread_create(thread,NULL,&threadAlien,(void *)Alien);
          Alien->pid = thread->pid;
          llist_addLast(communityA,Alien);
        } else if(e.key.keysym.sym == SDLK_4){
          alien *Alien = createAlienManual(baseVel,0,0);
          lpthread_t *thread = malloc(sizeof(lpthread_t));
          Lthread_create(thread,NULL,&threadAlien,(void *)Alien);
          Alien->pid = thread->pid;
          llist_addLast(communityB,Alien);
        } else if(e.key.keysym.sym == SDLK_5){
          alien *Alien = createAlienManual(baseVel,0,1);
          lpthread_t *thread = malloc(sizeof(lpthread_t));
          Lthread_create(thread,NULL,&threadAlien,(void *)Alien);
          Alien->pid = thread->pid;
          llist_addLast(communityB,Alien);
        } else if(e.key.keysym.sym == SDLK_6){
          alien *Alien = createAlienManual(baseVel,0,2);
          lpthread_t *thread = malloc(sizeof(lpthread_t));
          Lthread_create(thread,NULL,&threadAlien,(void *)Alien);
          Alien->pid = thread->pid;
          llist_addLast(communityB,Alien);
        }
      }
      switch (e.key.keysym.sym){
        case SDLK_1:
          useClip = 0;
          break;
        default:
          break;
      }
		}
  }

	//First clear the renderer
		SDL_RenderClear(ren);

    //Draw the background
    renderTexture(BG, ren,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    for(int i=0;i<yTiles;i++){
      for(int j=0;j<xTiles;j++){
        if (map[i][j].type == 1){
          renderTexture(Road, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        } else if (map[i][j].type == 2){
          renderTexture(BiRoad, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        } else if (map[i][j].type == 3){
          renderTexture(Bridge, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
      }
    }

    
    for (int i = 0; i < llist_getSize(communityA); i++){
      alien *temp = (alien *)llist_getbyId(communityA, i);
      if( temp != NULL)
        renderTextureSheet(Alien, ren, temp->posj * TILE_SIZE, temp->posi * TILE_SIZE, 30, &clipsA[temp->type][useClip]);
    }

    for (int i = 0; i < llist_getSize(communityB); i++){
      alien *temp = (alien *)llist_getbyId(communityB, i);
      if( temp != NULL)
        renderTextureSheet(Alien, ren, temp->posj * TILE_SIZE, temp->posi * TILE_SIZE, 30, &clipsB[temp->type][useClip]);
    }
    
    renderTexture(Castle1, ren,10, 200, 170, 170);
    renderTextureFull(castleA,ren,15,390);
    renderTexture(Castle2, ren,SCREEN_WIDTH-180, 235, 190, 170);
    renderTextureFull(castleB,ren,SCREEN_WIDTH-180,220);

    if(toggleInfo){
      renderTexture(Castle1, ren,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

		//Update the screen
		SDL_RenderPresent(ren);

    SDL_Delay(17);

    animCounter += 1;
    if (animCounter == 20){
      animCounter = 0;
      useClip +=1;
      if (useClip == 3)
        useClip = 0;
    }
	}

  
  llist_free(communityA);
  llist_free(communityB);
  llist_free(eastUp);
  llist_free(eastDown);
  llist_free(centerDown);
  llist_free(centerUp);
  llist_free(westUp);
  llist_free(westDown);
  SDL_DestroyTexture(BG);
  SDL_DestroyTexture(BiRoad);
  SDL_DestroyTexture(Bridge);
  SDL_DestroyTexture(Road);
  SDL_DestroyTexture(Alien);
  SDL_DestroyTexture(Castle1);
  SDL_DestroyTexture(Castle2);
  SDL_DestroyTexture(castleA);
  SDL_DestroyTexture(castleB);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  Lmutex_destroy(&lock);
  return 0;
}


int threadAlien(void *param){
  alien * Alien = (alien *)param;
  while (Alien->isAlive){
    Lmutex_lock(&lock);
    if (Alien->move) 
      moveAlien(Alien,map);
    Lmutex_unlock(&lock);

    double wait = Alien->velocity;
    double newWait = 5/wait*10;
    usleep(newWait*1000000); //microsec
    // velocity = 200 then sleep 1/4 sec (max)
    // velocity = 5 then sleep 10 sec (min)
  }
  return 0;
}

int delAlien(alien Alien, int x, int y){
  SDL_Rect mouse;
  mouse.x = x; mouse.y = y;
  mouse.w = 1; mouse.h = 1;

  SDL_Rect al;
  al.x = Alien.posj*TILE_SIZE; al.y = Alien.posi*TILE_SIZE;
  al.w = TILE_SIZE; al.h = TILE_SIZE;

  if(SDL_HasIntersection(&mouse,&al))
    return 1;
  else
    return 0;
}

int Y_Algorithm(llist* up,llist* down,bridge* p,int Y) {
  return 0;
}

int Semaphore_Algorithm(){
  return 0;
}

int Survival_Algorithm(llist* up,llist* down,bridge* p){
  return 0;
}

int generatorA(){
  while(1){
    alien *Alien = createAlien(baseVel,1,Types);
    lpthread_t *thread = malloc(sizeof(lpthread_t));
    Lthread_create(thread,NULL,&threadAlien,(void *)Alien);
    Alien->pid = thread->pid;
    llist_addLast(communityA,Alien);

    double num = expRand(Config.mean);

    usleep( num * 2000000);

  }
  return 0;
}

int generatorB(){
  while(1){
    alien *Alien = createAlien(baseVel,0,Types);
    lpthread_t *thread = malloc(sizeof(lpthread_t));
    Lthread_create(thread,NULL,&threadAlien,(void *)Alien);
    Alien->pid = thread->pid;
    llist_addLast(communityB,Alien);

    double num = expRand(Config.mean);

    usleep( num * 2000000);
  }
  return 0;
}