#include <libconfig.h>
#include <time.h>
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
  const char *direction = NULL;
  int capacity, used, length;

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

  if(!config_lookup_string(cf,"Direction",&direction)){
    printf("Error en Direction\n");
    return 1;
  }

  if(!config_lookup_int(cf,"Capacity", &capacity)){
    printf("Error en Capacity\n");
    return 1;
  }

  if(!config_lookup_int(cf,"Used", &used)){
    printf("Error en Used\n");
    return 1;
  };

  if(!config_lookup_int(cf,"Length", &length)){
    printf("Error en Length\n");
    return 1;
  }
  
  BR->name = (char*)name;
  BR->capacity = capacity;
  BR->direction = (char)direction;
  BR->length = length;
  BR->used = used;
  
  return 0;
}

/**
 * Move an Alien to a next position
 * 
*/
int moveAlien(alien *Alien, cell map[24][46]){
  int iteri[8] = {-1,-1,-1,0,0,1,1,1};
  int iterj[8] = {-1,0,1,-1,1,-1,0,1};
  for (int x = 0; x<8; x++){
    int i = Alien->posi;
    int j = Alien->posj;
    if (Alien->direction == map[i+iteri[x]][j+iterj[x]].direction){
      Alien -> posi = i + iteri[x];
      Alien -> posj = j + iterj[x];
      return 0;
    }
  }
  return 1;
}


/**
 * Create a new Alien
 * @param baseVel: Base Velocity of the aliens
 * @param home: Community of the alien
 * @return New alien struct
*/

alien * createAlien(int baseVel, int home, int type, int mul){
  alien *newAlien = malloc(sizeof(alien));
  
  newAlien -> type =  type;
  newAlien -> isAlive = 1;
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
    newAlien -> posi = 11;
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

