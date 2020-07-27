#include <libconfig.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#include "../../include/data.h"

/**
 * Fills a bridge struct froma a config file
 * @param BR: Struct to fill
 * @param filename: Path of the cofig file
 * @return 0 if all is ok 1 if have some trouble
*/
int getBridgeData(bridge *BR, char* filename){
  config_t cfg, *cf;
  const char *name = NULL;
  int capacity, length, algorithm, scheduler, Y;

	cf = &cfg;
	config_init(cf);

	if (!config_read_file(cf, filename)) {
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(cf),
			config_error_line(cf),
			config_error_text(cf));
		config_destroy(cf);
		return 1;
	}

  if(!config_lookup_string(cf,"Name", &name)){
    printf("Error en Name\n");
    return 1;
  }

  if(!config_lookup_int(cf,"Capacity", &capacity)){
    printf("Error en Capacity\n");
    return 1;
  }

  if(!config_lookup_int(cf,"Length", &length)){
    printf("Error en Length\n");
    return 1;
  }

  if(!config_lookup_int(cf,"Algorithm", &algorithm)){
    printf("Error en Algorithm\n");
    return 1;
  }
  
  if(!config_lookup_int(cf,"Scheduler", &scheduler)){
    printf("Error en Scheduler\n");
    return 1;
  }

  if(!config_lookup_int(cf,"Y_value", &Y)){
    printf("Error en Y_value\n");
    return 1;
  }

  BR->name = (char*)name;
  BR->algorithm = algorithm;
  BR->scheduler = scheduler;
  BR->y_value = Y;
  BR->capacity = capacity;
  BR->length = length;
  BR->counter = 0;
  BR->lastAccess = 1;
  BR->accumulator = 0;  
  return 0;
}

/**
 * Move an Alien to a next position
 * 
*/
int moveAlien(alien *Alien, cell map[24][46]){
  int i = Alien->posi;
  int j = Alien->posj;
  
  int p[4]= {-1, 0, 1, 0};
  int q[4]= {0, 1, 0, -1};

  int iter = 0;

  if (Alien->lmove == 1){
    iter = 0;
  } else if (Alien->lmove == 2){
    iter = 1;
  } else if (Alien->lmove == 3){
    iter = 2;
  } else if (Alien->lmove == 4){
    iter = 3;
  }

  //(i == 5 && j == 21)

  for(int x = 0; x < 4; x++){
    char mapDir = map[i + p[iter]][j + q[iter]].direction;
    int nextUp = map[i + p[iter]][j + q[iter]].usedUp;
    int nextDown = map[i + p[iter]][j + q[iter]].usedDown;
    if (mapDir == 'C' && (i + 1 == 5 && j == 22) && !nextDown){
      int dir = rand() % 3;
      switch (dir)
      {
      case 0:
        Alien->posi += 1;
        Alien->lmove = 4;
        break;
      case 1:
        Alien->posi += 1;
        Alien->lmove = 3;
        break;
      case 2:
        Alien->posi += 1;
        Alien->lmove = 2;
        break;
      default:
        break;
      }
       if(Alien->direction == 'B'){
          map[i][j].usedDown = 0;
          map[Alien->posi][Alien->posj].usedDown = 1;
        }else{
          map[i][j].usedUp = 0;
          map[Alien->posi][Alien->posj].usedUp = 1;
        }
      return 0;
    }else if(mapDir == 'C' && (i -1 == 17 && j == 22) && !nextUp){
      int dir = rand() % 3;
      switch (dir)
      {
      case 0:
        Alien->posi -= 1;
        Alien->lmove = 4;
        break;
      case 1:
        Alien->posi -= 1;
        Alien->lmove = 1;
        break;
      case 2:
        Alien->posi -= 1;
        Alien->lmove = 2;
        break;
      default:
        break;
      }
      if(Alien->direction == 'B'){
        map[i][j].usedDown = 0;
        map[Alien->posi][Alien->posj].usedDown = 1;
      }else{
        map[i][j].usedUp = 0;
        map[Alien->posi][Alien->posj].usedUp = 1;
      }
      return 0;
    } else if(mapDir == 'C'){
      if(Alien -> direction == 'B' && nextDown){
        break;
      } else if(Alien -> direction == 'A' && nextUp){
        break;
      }
      Alien->posi = i + p[iter];
      Alien->posj = j + q[iter];
      int cmp = getAlienPath(Alien,map,Alien->direction);
      if(cmp != 5){
        Alien->lmove = cmp;
      }else if(Alien->posi == 5 && Alien -> posj == 11 && Alien->direction == 'B'){
        Alien->lmove = 3;
      }else if(Alien->posi == 17 && Alien -> posj == 11 && Alien->direction == 'B'){
        Alien->lmove = 2;
      }else if(Alien->posi == 17 && Alien -> posj == 33 && Alien->direction == 'A'){
        Alien->lmove = 1;
      }else if(Alien->posi == 5 && Alien -> posj == 33 && Alien->direction == 'A'){
        Alien->lmove = 4;
      }else{
        Alien->lmove = iter+1;
      }
      if(Alien->direction == 'B'){
        map[i][j].usedDown = 0;
        map[Alien->posi][Alien->posj].usedDown = 1;
      }else{
        map[i][j].usedUp = 0;
        map[Alien->posi][Alien->posj].usedUp = 1;
      }
      return 0;
    } else if(Alien -> direction == mapDir){
      if(Alien -> direction == 'B' && nextDown){
        break;
      } else if(Alien -> direction == 'A' && nextUp){
        break;
      }
      Alien->posi = i + p[iter];
      Alien->posj = j + q[iter];
      if(Alien->posi == 21 && Alien -> posj == 20){
        Alien->lmove = 2;
      }else if(Alien->posi == 21 && Alien -> posj == 43){
        Alien->lmove = 1;
      }else if(Alien->posi == 1 && Alien -> posj == 24){
        Alien->lmove = 4;
      }else if(Alien->posi == 1 && Alien -> posj == 1){
        Alien->lmove = 3;
      }else{
        Alien->lmove = iter + 1;
      }
      if(Alien->direction == 'B'){
        map[i][j].usedDown = 0;
        map[Alien->posi][Alien->posj].usedDown = 1;
      }else{
        map[i][j].usedUp = 0;
        map[Alien->posi][Alien->posj].usedUp = 1;
      }
      return 0;
    } 
    iter ++;
    if (iter == 4)
      iter = 0;
  }
  return 1;
}

int getAlienPath(alien *Alien, cell map[24][46], char dir){
  int i = Alien -> posi;
  int j = Alien -> posj;
  if(map[i-1][j].direction == dir){
    return 1;
  } else if(map[i][j+1].direction == dir){
    return 2;
  } else if(map[i+1][j].direction == dir){
    return 3;
  } else if(map[i][j-1].direction == dir){
    return 4;
  } else{
    return 5;
  }
}

/**
 * Create a new Alien
 * @param baseVel: Base Velocity of the aliens
 * @param home: Community of the alien
 * @return New alien struct
*/

alien * createAlien(int baseVel, int home, int Types[100]){
  alien *newAlien = malloc(sizeof(alien));
  int type = rand()%100;
  int mul = (rand() % (200 - 50 + 1)) + 50;
  int weight = (rand() % (40 - 20 + 1)) + 20;

  struct timeval tic;
  gettimeofday(&tic, NULL);
  newAlien->tic = (double)tic.tv_sec;
  
  newAlien -> weight = weight;
  newAlien -> type =  Types[type];
  newAlien -> isAlive = 1;
  newAlien -> lmove = 0;
  newAlien -> move = 1;
  if (newAlien->type == 0){
    newAlien -> velocity = baseVel;
  } else if (newAlien->type == 1){
    newAlien -> velocity = baseVel + baseVel * 0.2;
  } else if (newAlien->type == 2){
    newAlien -> velocity = baseVel + baseVel * mul / 100 ;
  }
  if(home){
    newAlien -> direction = 'B';
    newAlien -> posi = 11;
    newAlien -> posj = 3;
  }else{
    newAlien -> direction = 'A';
    newAlien -> posi = 10;
    newAlien -> posj = 41;
  }

  return newAlien;
}

/**
 * Create a new Alien
 * @param baseVel: Base Velocity of the aliens
 * @param home: Community of the alien
 * @return New alien struct
*/

alien * createAlienManual(int baseVel, int home, int Type){
  alien *newAlien = malloc(sizeof(alien));
  int mul = (rand() % (200 - 50 + 1)) + 50;
  int weight = (rand() % (40 - 20 + 1)) + 20;
  struct timeval tic;
  gettimeofday(&tic, NULL);

  newAlien -> weight = weight;
  newAlien->tic = (double)tic.tv_sec;
  newAlien -> type =  Type;
  newAlien -> isAlive = 1;
  newAlien -> lmove = 0;
  newAlien -> move = 1;
  if (newAlien->type == 0){
    newAlien -> velocity = baseVel;
  } else if (newAlien->type == 1){
    newAlien -> velocity = baseVel + baseVel * 0.2;
  } else if (newAlien->type == 2){
    newAlien -> velocity = baseVel + baseVel * mul / 100 ;
  }
  if(home){
    newAlien -> direction = 'B';
    newAlien -> posi = 11;
    newAlien -> posj = 3;
  }else{
    newAlien -> direction = 'A';
    newAlien -> posi = 10;
    newAlien -> posj = 41;
  }

  return newAlien;
}

/**
 * Prints alien values
 * @param Alien: Alien to print
*/

void print(alien Alien){
  printf("Tipo: %d\n", Alien.type);
  printf("IsAlive: %d\n", Alien.isAlive);
  printf("Velocidad: %.3f\n", Alien.velocity);
  printf("Direccion: %c\n", Alien.direction);
}

/**
 * Fills a bridge struct froma a config file
 * @param BR: Struct to fill
 * @param filename: Path of the cofig file
 * @return 0 if all is ok 1 if have some trouble
*/
int getConfigsData(configs *conf, char* filename){
  config_t cfg, *cf;
  
  int probTypeA, probTypeB, probTypeC, mean;

	cf = &cfg;
	config_init(cf);

	if (!config_read_file(cf, filename)) {
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(cf),
			config_error_line(cf),
			config_error_text(cf));
		config_destroy(cf);
		return 1;
	}

  if(!config_lookup_int(cf,"probTypeA", &probTypeA)){
    printf("Error en probTypeA\n");
    return 1;
  }

  if(!config_lookup_int(cf,"probTypeB", &probTypeB)){
    printf("Error en probTypeB\n");
    return 1;
  }

  if(!config_lookup_int(cf,"probTypeC", &probTypeC)){
    printf("Error en probTypeC\n");
    return 1;
  }

  if(!config_lookup_int(cf,"mean", &mean)){
    printf("Error en Mean\n");
    return 1;
  }
  
  conf -> probTypeA = probTypeA;
  conf -> probTypeB = probTypeB;
  conf -> probTypeC = probTypeC;
  conf -> mean = mean;

  return 0;
}

void loadMap(cell map[24][46]){
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

int Y_Algorithm(llist* up,llist* down,bridge* p,int Y) {
// 1 = los que vienen de arriba, 0 = los que vienen de abajo
  int lenUp = llist_getSize(up);
  int lenDown = llist_getSize(down);
  int direction = p->lastAccess;
  if(lenUp > lenDown){ //Si hay mas aliens arriba 
    if (direction){ //Si direccion es 1
      if(p->counter < Y){ //Si elcontador es menor a Y
        p->counter+=1; //Deje pasar 1 alien
        //printf("Dejan pasar a un alien(arriba), contador=%d\n",p->counter);
        if (p->counter == Y){ //El contador es igual a Y?
          p->counter = 0; // reinicia el contador
          p->lastAccess = 0; //cambia la direccion a 0
        }        
        return 1; // Si el contador aun es menor a Y, sigue la misma direccion
      }
    }else{
      if (!direction){ //Si direccion es 0
        if(p->counter < Y){ //Si elcontador es menor a Y
          p->counter+=1; //Deje pasar 1 alien
          //printf("Dejan pasar a un alien(abajo), contador=%d\n",p->counter);
          if (p->counter == Y){ //El contador es igual a Y?
            p->counter = 0; // reinicia el contador
            p->lastAccess = 1; //cambia la direccion a 0
          }        
          return 0; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }
    }
  } else if(lenUp < lenDown){
    if (!direction) {
      if(p->counter < Y){
        p->counter+=1;
        //printf("Dejan pasar a un alien(abajo), contador=%d\n",p->counter);
        if (p->counter == Y){
          p->counter = 0;
          p->lastAccess = 1;
        }
        return 0;
      }
    }else{
      if (direction){ //Si direccion es 1
        if(p->counter < Y){
          p->counter+=1; //Deje pasar 1 alien
          //printf("Dejan pasar a un alien(arriba), contador=%d\n",p->counter);
          if (p->counter == Y){ //El contador es igual a Y?
            p->counter = 0; // reinicia el contador
            p->lastAccess = 0; //cambia la direccion a 0
          }        
          return 1; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }
    }
  }else{ 
      if (direction){ //Si direccion es 1
        if(p->counter < Y && lenUp != 0){ //Si elcontador es menor a Y
          p->counter+=1; //Deje pasar 1 alien
          //printf("Dejan pasar a un alien(arriba), contador=%d\n",p->counter);
          if (p->counter == Y){ //El contador es igual a Y?
            p->counter = 0; // reinicia el contador
            p->lastAccess = 0; //cambia la direccion a 0
          }
          return 1; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }else{
        if(p->counter < Y && lenDown != 0){ //Si elcontador es menor a Y
          p->counter+=1; //Deje pasar 1 alien
          //printf("Dejan pasar a un alien(abajo), contador=%d\n",p->counter);
          if (p->counter == Y){ //El contador es igual a Y?
            p->counter = 0; // reinicia el contador
            p->lastAccess = 1; //cambia la direccion a 0
          }
          return 0; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }
    }
return direction;
}

int Semaphore_Algorithm(bridge* p,float timeN,float timeS){
  struct timeval tic;
  float accum = p->accumulator;

  gettimeofday(&tic, NULL); //toma el tiempo
  p->tic = (double)tic.tv_sec;
  
  if(p->lastAccess && accum >= timeN){  
    p->lastAccess = 0;
    p->accumulator = 0;          
  }
  else if(!(p->lastAccess) && accum >= timeS){   
    p->lastAccess = 1;
    p->accumulator = 0;        
  } 
  return p->lastAccess;
}

int Survival_Algorithm(llist* up,llist* down,bridge* p){ 
  int temp;
  temp = Y_Algorithm(up,down,p,1);
  return temp;
}


int FIFO(llist *Up, llist *Down, bridge *Bridge,cell map[24][46]){
  int getAlienFrom = 0;
  alien *Alien;
  float accum = Bridge->accumulator;
  struct timeval toc;

  gettimeofday(&toc, NULL);
  double time_Dif = (double)(toc.tv_sec - Bridge->tic);
  accum += time_Dif;
  Bridge->accumulator = accum;

  //Chose the algorithm
  if(Bridge->algorithm == 0){
    getAlienFrom = Y_Algorithm(Up,Down,Bridge,Bridge->y_value);
  }
  if(Bridge->algorithm == 1){
    getAlienFrom = Semaphore_Algorithm(Bridge,8, 5);
  }  
  if(Bridge->algorithm == 2){
    getAlienFrom = Survival_Algorithm(Up,Down,Bridge);
  }

  //Get Alien form Qeues
  if(getAlienFrom){
    if(llist_getSize(Up) == 0){
      return 0;
    }
    Alien = (alien *)llist_getbyId(Up,0);
    if(Alien->posi == 9 && Alien->move == 0){
      sleep(1);
      map[Alien->posi][Alien->posj].usedDown = 0;
      Alien -> posi += 2;
    }
  } else{
    if(llist_getSize(Down) == 0){
      return 0;
    }
    Alien = (alien *)llist_getbyId(Down,0);
    if(Alien->posi == 13 && Alien->move == 0){
      sleep(1);
      map[Alien->posi][Alien->posj].usedUp = 0;
      Alien -> posi -= 2;
    }
  }


  if(Alien->move){
    return 0;
  }

  double wait = (double)Bridge->length / (double)Alien->velocity;
  usleep(wait * 1000000);

  if(getAlienFrom){
    Alien -> posi += 2;
    Alien -> move = 1;
    if(Alien->isAlive){
      llist_delById(Up,0);
    }
  } else{
    Alien -> posi -= 2;
    Alien -> move = 1;
    if(Alien->isAlive){
      llist_delById(Down,0);
    }
  }
  return 0;
}

dataScheduler *createDataScheduler(llist * Up, llist * Down,bridge *Bridge){
  dataScheduler *out= malloc(sizeof(dataScheduler));
  out->Up = Up;
  out->Down = Down;
  out->Bridge = Bridge;
  return out;
}