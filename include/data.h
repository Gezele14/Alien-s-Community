#ifndef _DATA_H
#define _DATA_H

typedef struct {
  int type;
  char direction;
} cell;

typedef struct{
  char *name;
  char direction;
  int capacity;
  int used;
  int length;
}bridge;

typedef struct{
  int type;
  int isAlive;
  int posi;
  int posj;
  double velocity;
  char direction;
}alien;



int getBridgeData(bridge *bridge, char* filename);
int moveAlien(alien *Alien, cell map[24][46]);
alien * createAlien(int baseVel, int home, int type, int mul);
void print(alien Alien);

#endif 