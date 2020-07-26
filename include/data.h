#ifndef _DATA_H
#define _DATA_H

typedef struct {
  int type;
  char direction;
  int usedUp;
  int usedDown;
} cell;


typedef struct{
  char *name;
  char direction;
  int capacity;
  int used;
  int length;
  int counter;
  short lastAccess;
  float accumulator;
}bridge;

typedef struct{
  int type;
  int isAlive;
  int posi;
  int posj;
  int lmove;
  int move;
  int pid;
  double tic;
  double velocity;
  char direction;
}alien;

typedef struct{
  int probTypeA;
  int probTypeB;
  int probTypeC;
  int mean;
} configs;


int getBridgeData(bridge *bridge, char* filename);
int moveAlien(alien *Alien, cell map[24][46]);
alien * createAlien(int baseVel, int home, int Types[100]);
alien * createAlienManual(int baseVel, int home, int Type);
void print(alien Alien);
int getConfigsData(configs *conf, char* filename);
void loadMap(cell map[24][46]);
int getAlienPath(alien *Alien, cell map[24][46], char dir);

#endif 