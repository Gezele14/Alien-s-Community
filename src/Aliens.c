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

const int SCREEN_WIDTH = 1366; //
const int SCREEN_HEIGHT = 720; //
const int TILE_SIZE = 30;

const int xTiles = SCREEN_WIDTH/TILE_SIZE+1;
const int yTiles = SCREEN_HEIGHT/TILE_SIZE;

cell map[24][46];
int exitProgram = 0;
int modeManual = 0; 


SDL_Event e;
SDL_Color fontColor = { 0, 0, 0, 255};

int useClip = 0;

int baseVel = 5;
int maxTemp = 5;

lpthread_mutex_t lock;

//Load Textures & Fonts
char *bgPath = "../assets/images/bg.png";
char *fondoPath = "../assets/images/fondo.png";
char *roadPath = "../assets/images/tile2.png";
char *biroadPath = "../assets/images/tile4.png";
char *bridgePath = "../assets/images/tile3.png";
char *alienPath = "../assets/images/alien.png";
char *castle1Path = "../assets/images/castle1.png";
char *castle2Path = "../assets/images/castle2.png";
char *button1Path = "../assets/images/button1.png";
char *button1_selPath = "../assets/images/button1_sel.png";
char *button2Path = "../assets/images/button2.png";
char *button2_selPath = "../assets/images/button2_sel.png";
char *buttonleft = "../assets/images/left.png";
char *buttonright = "../assets/images/right.png";
char *buttonOk = "../assets/images/ok.png";
char *buttonCancel = "../assets/images/cancel.png";
char *iconPath = "../assets/images/icon.png";



char *mainFontPath = "../assets/fonts/font.ttf";

//Bringes
bridge East;

//Functions
void handleEvents();
void loadMap();
int configWindow();
int threadAlien();
int delAlien(alien Alien, int x, int y);

int main(int args, char **argv){

  srand(time(NULL));  
  loadMap();

  //Initialize Mutex
  if (Lmutex_init(&lock, NULL) != 0){ 
    printf("\n Mutex init has failed\n"); 
    return 1; 
  }

  if(getBridgeData(&East, "../config/eastBridge.conf")){
    return 1;
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
  if(configWindow()){
    return 1;
  }


  if(modeManual){
    printf("Se selecciono el modo manual\n");
  } else if(!modeManual){
    printf("Se selecciono el modo Auto\n");
  }

  llist *communityA = llist_create(NULL);
  llist *threadsCommunityA = llist_create(NULL);
  llist *communityB = llist_create(NULL);
  llist *threadsCommunityB = llist_create(NULL);

  for (int i = 0; i < 5; i++){
    int type = rand()%3;
    int mul = (rand() % (200 - 50 + 1)) + 50;
    alien *temp;
    if (i == 2)
      temp = createAlien(baseVel, 0,type, mul);
    else
      temp = createAlien(baseVel, 1,type, mul);
    lpthread_t *thread = malloc(sizeof(lpthread_t));
    Lthread_create(thread,NULL,&threadAlien,temp);
    llist_addLast(threadsCommunityA,threadsCommunityA);
    llist_addLast(communityA,temp);
  }

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
    loadClips(clipsA[i],1,i+1,69,69);
  }

  //Setup the clips for Community B
	SDL_Rect clipsB[3][3];
  for (int i = 0; i < 3; i++){
    loadClips(clipsB[i],2,i+1,69,69);
  }

  int mouseX, mouseY;
  int animCounter = 0;
  //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
	while(!exitProgram){
    //Event Handler
    while (SDL_PollEvent(&e)){
    //If user closes the window
    if (e.type == SDL_QUIT){
      exitProgram = 1;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN){
      SDL_GetMouseState(&mouseX,&mouseY);
      int communityASize = llist_getSize(communityA)-1;

      for (int i = 0; i <= communityASize; i++){
        alien *DUT = (alien *)llist_getbyId(communityA, i);
        if (delAlien(*DUT,mouseX, mouseY)){
          lpthread_t *threadTemp = (lpthread_t *)llist_getbyId(threadsCommunityA,i);
          Lthread_exit(threadTemp->pid);
          llist_delById(threadsCommunityA, i);
          llist_delById(communityA, i);
          break;
        }
      }
    }

    if (e.type == SDL_KEYDOWN){
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

    renderTexture(Castle1, ren,10, 200, 170, 170);
    renderTextureFull(castleA,ren,15,390);
    renderTexture(Castle2, ren,SCREEN_WIDTH-180, 235, 190, 170);
    renderTextureFull(castleB,ren,SCREEN_WIDTH-180,220);

    for (int i = 0; i < llist_getSize(communityA)-1; i++){
      alien *temp = (alien *)llist_getbyId(communityA, i);
      renderTextureSheet(Alien, ren, temp->posj * TILE_SIZE, temp->posi * TILE_SIZE, 40, &clipsA[temp->type][useClip]);
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
  llist_free(threadsCommunityA);
  llist_free(communityB);
  llist_free(threadsCommunityB);
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
     map[i][j].type = 0;
     map[i][j].direction = 'D';
     j += 1;
   } else if (ch == 'x'){
     map[i][j].type = 1;
     map[i][j].direction = 'A';
     j += 1;
   } else if (ch == 'o'){
     map[i][j].type = 2;
     map[i][j].direction = 'C';
     j += 1;
   } else if (ch == 'p'){
     map[i][j].type = 3;
     map[i][j].direction = 'C';
     j += 1;
   } else if (ch == 'z'){
     map[i][j].type = 1;
     map[i][j].direction = 'B';
     j += 1;
   }
  }

  fclose(file);
}

int threadAlien(alien *Alien){
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

int configWindow(){
  //Opening a Window
  SDL_Window *config = SDL_CreateWindow("Alien's Community",SCREEN_WIDTH/2-150, 200, 320, 400, SDL_WINDOW_SHOWN);
  if (config == NULL){
    printf("SDL_CreateWindow Error: %s\n",SDL_GetError());
    return 1;
  }

  //Creating a renderer
  SDL_Renderer *configRen = SDL_CreateRenderer(config, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (configRen == NULL){
    SDL_DestroyWindow(config);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return 1;
  }

  char velText[5];
  char maxTempText[5];
  int isButton1 = 0;
  int isButton2 = 0;

  SDL_itoa(baseVel,velText,10);
  SDL_itoa(maxTemp,maxTempText,10);
  
  SDL_Texture *BG = loadTexture(bgPath, configRen);
  SDL_Texture *Button1 = loadTexture(isButton1? button1Path : button1_selPath,configRen);;
  SDL_Texture *Button2 = loadTexture(isButton2? button2Path : button2_selPath,configRen); ;
  SDL_Texture *Left = loadTexture(buttonleft, configRen);
  SDL_Texture *Right = loadTexture(buttonright, configRen);
  SDL_Texture *Ok = loadTexture(buttonOk, configRen);
  SDL_Texture *Cancel = loadTexture(buttonCancel, configRen);
  SDL_Texture *selection = renderText("Seleccione un modo", mainFontPath, fontColor, 25, configRen);
  SDL_Texture *velSelText = renderText("Velocidad base", mainFontPath, fontColor, 25, configRen);
  SDL_Texture *maxTempTitle = renderText("Tiempo maximo", mainFontPath, fontColor, 25, configRen);
  SDL_Texture *velSel = renderText(velText, mainFontPath, fontColor, 25, configRen);
  SDL_Texture *maxTempSel = renderText(maxTempText, mainFontPath, fontColor, 25, configRen);

  SDL_Rect Button1Rect = getTextureRect(Button1,15,75);
  SDL_Rect Button2Rect = getTextureRect(Button2,175,75);
  SDL_Rect ButtonLeftRect = getTextureRect(Left, 80,210);
  SDL_Rect ButtonRightRect = getTextureRect(Right,190,210);
  SDL_Rect ButtonLeftRect2 = getTextureRect(Left, 80,290);
  SDL_Rect ButtonRightRect2 = getTextureRect(Right,190,290);
  SDL_Rect ButtonOkRect = getTextureRect(Ok,70,350);
  SDL_Rect ButtonCancelRect = getTextureRect(Cancel,170,350);
  SDL_Rect mouseRect;
  SDL_Event e;
  int mouseX, mouseY;
  int exitWin = 0;
  while(!exitWin){
    Button1 = loadTexture(isButton1? button1_selPath : button1Path,configRen);
    Button2 = loadTexture(isButton2? button2_selPath : button2Path,configRen); 
    while (SDL_PollEvent(&e)){
      //If user closes the window
      if (e.type == SDL_QUIT){
        exitProgram = 1;
        exitWin = 1;
        return 1;
      }
      if ((e.type == SDL_MOUSEBUTTONDOWN) && SDL_BUTTON(SDL_BUTTON_LEFT)){
        SDL_GetMouseState(&mouseX,&mouseY);
        mouseRect.x = mouseX; mouseRect.y = mouseY; mouseRect.w = mouseRect.h = 1;
        if(SDL_HasIntersection(&mouseRect,&Button1Rect)){
          modeManual = 1;
          isButton1 = 1;
          isButton2 = 0;
        } else if(SDL_HasIntersection(&mouseRect,&Button2Rect)){
          modeManual = 0;
          isButton2 = 1;
          isButton1 = 0;
        } else if(SDL_HasIntersection(&mouseRect,&ButtonLeftRect)){
          if (baseVel > 5)
            baseVel -= 5;
          SDL_itoa(baseVel,velText,10);
          velSel = renderText(velText, mainFontPath, fontColor, 25, configRen);
        } else if(SDL_HasIntersection(&mouseRect,&ButtonRightRect)){
          if (baseVel < 20)
            baseVel += 5;
          SDL_itoa(baseVel,velText,10);
          velSel = renderText(velText, mainFontPath, fontColor, 25, configRen);
        }else if(SDL_HasIntersection(&mouseRect,&ButtonLeftRect2)){
          if (maxTemp > 5)
            maxTemp -= 5;
          SDL_itoa(maxTemp,maxTempText,10);
          maxTempSel = renderText(maxTempText, mainFontPath, fontColor, 25, configRen);
        } else if(SDL_HasIntersection(&mouseRect,&ButtonRightRect2)){
          if (maxTemp < 60)
            maxTemp += 5;
          SDL_itoa(maxTemp,maxTempText,10);
          maxTempSel = renderText(maxTempText, mainFontPath, fontColor, 25, configRen);
        } else if(SDL_HasIntersection(&mouseRect,&ButtonOkRect)){
          if (isButton2 || isButton1){
            exitWin = 1;
          }
        }else if(SDL_HasIntersection(&mouseRect,&ButtonCancelRect)){
          exitWin = 1;
          return 1;
        }
      }
    }
    renderTexture(BG,configRen,0,0,400,400);

    renderTextureFull(selection,configRen,35,25);
    renderTextureFull(Button1,configRen,Button1Rect.x,Button1Rect.y);
    renderTextureFull(Button2,configRen,Button2Rect.x,Button2Rect.y);

    renderTextureFull(velSelText,configRen,60,170);
    renderTextureFull(Left,configRen,ButtonLeftRect.x,ButtonLeftRect.y);
    renderTextureFull(Right,configRen,ButtonRightRect.x,ButtonRightRect.y);
    renderTextureFull(velSel,configRen,150,220);

    renderTextureFull(maxTempTitle,configRen,60,260);
    renderTextureFull(Left,configRen,ButtonLeftRect2.x,ButtonLeftRect2.y);
    renderTextureFull(Right,configRen,ButtonRightRect2.x,ButtonRightRect2.y);
    renderTextureFull(maxTempSel,configRen,150,300);

    renderTextureFull(Ok,configRen,ButtonOkRect.x,ButtonOkRect.y);
    renderTextureFull(Cancel,configRen,ButtonCancelRect.x,ButtonCancelRect.y);

    //Update the screen
		SDL_RenderPresent(configRen); 
    SDL_Delay(30);
  }

  SDL_DestroyTexture(BG);
  SDL_DestroyTexture(Button1);
  SDL_DestroyTexture(Button2);
  SDL_DestroyTexture(Left);
  SDL_DestroyTexture(Right);
  SDL_DestroyTexture(Ok);
  SDL_DestroyTexture(Cancel);
  SDL_DestroyTexture(selection);
  SDL_DestroyTexture(velSelText);
  SDL_DestroyTexture(velSel);
  SDL_DestroyTexture(maxTempTitle);
  SDL_DestroyTexture(maxTempSel);
  SDL_DestroyWindow(config);
  SDL_DestroyRenderer(configRen);

  exitProgram = 0;
  return 0;
}