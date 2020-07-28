

#ifndef _DATA_H
#define _DATA_H

#include "linkedlist.h"

typedef struct {
  int type;
  char direction;
  int usedUp;
  int usedDown;
} cell;


typedef struct{
  char *name;
  int capacity;
  int length;
  int counter;
  short lastAccess;
  float accumulator;
  int algorithm;
  int scheduler;
  int y_value;
  double tic;
  float m_time;
  float n_time;
}bridge;

typedef struct{
  int type;
  int isAlive;
  int posi;
  int posj;
  int lmove;
  int move;
  int pid;
  int weight;
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


typedef struct{
 llist *Up;
 llist *Down;
 bridge *Bridge;
} dataScheduler;


int getBridgeData(bridge *bridge, char* filename);
int moveAlien(alien *Alien, cell map[24][46]);
alien * createAlien(int baseVel, int home, int Types[100]);
alien * createAlienManual(int baseVel, int home, int Type);
void print(alien Alien);
int getConfigsData(configs *conf, char* filename);
void loadMap(cell map[24][46]);
int getAlienPath(alien *Alien, cell map[24][46], char dir);
int Y_Algorithm(llist* up,llist* down,bridge* p,int Y);
int Semaphore_Algorithm(bridge* p,float timeN,float timeS);
int Survival_Algorithm(llist* up,llist* down,bridge* p);
int FIFO(llist *Up, llist *Down, bridge *bridge,cell map[24][46]);
int Priority(llist *Up, llist *Down, bridge *Bridge,cell map[24][46]);
dataScheduler *createDataScheduler(llist * Up, llist * Down,bridge *Bridge);

#endif 