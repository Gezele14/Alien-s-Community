#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <util.h>
#include <data.h>
#include <linkedlist.h>
#include <lpthread.h>

//Se corre con
//gcc -o main main.c ../lib/util/libutil.a ../lib/data/libdata.a ../lib/linkedlist/liblinkedlist.a ../lib/lpthread/liblpthread.a -I ../include -Wall -lm -lconfig
//Dentro de la carpeta src
int Y_Algorithm(llist* up,llist* down,bridge* p,int Y);
int Semaphore_Algorithm(llist* up,llist* down,bridge* p,float timeN,float timeS);
int Survival_Algorithm(llist* up,llist* down,bridge* p);

int main(){

  srand(time(NULL));  

  int Types[100];

  for (int i = 0; i < 100; i++)
  {
    Types[i] = rand()%3;
  }
  

  int capacity = 10;
  int used = 0;
  int length = 10;
  int counter = 0;
  short lastAccess = 0;
  float accumulator = 0;


  bridge Puente = {"East",'A', capacity,used,length,counter,lastAccess,accumulator};


  llist *listA = llist_create(NULL);
  llist *listB = llist_create(NULL);

  for (int i = 0; i < 10; i++)
  {
    alien * Alien = createAlien(5,1,Types);
    llist_addLast(listA,Alien);
  }

  for (int i = 0; i < 8; i++)
  {
    alien * Alien = createAlien(5,1,Types);
    llist_addLast(listB,Alien);
  }

  //Ponen aqui el algoritmo
  int count_test = 10;
  int value;
  while(count_test){
    //Survival_Algorithm(listA,listB,&Puente);
    value=Semaphore_Algorithm(listA,listB,&Puente,5,3);
    printf("\nValor de retorno = %d\n",value);
    count_test--;    
  }
  
  
  llist_free(listA);
  llist_free(listB);
  return 0;
}

int Y_Algorithm(llist* up,llist* down,bridge* p,int Y) {
// 1 = los que vienen de arriba, 0 = los que vienen de abajo
  int lenUp = llist_getSize(up);
  int lenDown = llist_getSize(down);
  int direction = p->lastAccess;
  if(lenUp > lenDown){ //Si hay mas aliens arriba 
    if (direction){ //Si direccion es 1
    printf("Pasan los maes de arriba, contador=%d\n",p->counter);
      if(p->counter < Y){ //Si elcontador es menor a Y
        p->counter+=1; //Deje pasar 1 alien
        llist_pop(up); //Lo saca de la lista(Tiene que quitarse)
        printf("Dejan pasar a un alien, contador=%d\n",p->counter);
        if (p->counter == Y){ //El contador es igual a Y?
          printf("Se alcanzo la cantidad Y y se reinicia contador, contador=%d\n",p->counter);
          p->counter = 0; // reinicia el contador
          p->lastAccess = 0; //cambia la direccion a 0
        }        
        return 1; // Si el contador aun es menor a Y, sigue la misma direccion
      }
    }else{
      if (!direction){ //Si direccion es 1
      printf("Pasan los maes de abajo, contador=%d\n",p->counter);
        if(p->counter < Y){ //Si elcontador es menor a Y
          p->counter+=1; //Deje pasar 1 alien
          llist_pop(down); //Lo saca de la lista(Tiene que quitarse)
          printf("Dejan pasar a un alien, contador=%d\n",p->counter);
          if (p->counter == Y){ //El contador es igual a Y?
            printf("Se alcanzo la cantidad Y y se reinicia contador, contador=%d\n",p->counter);
            p->counter = 0; // reinicia el contador
            p->lastAccess = 1; //cambia la direccion a 0
          }        
          return 0; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }
    }
  } else if(lenUp < lenDown){
    if (!direction) {
      printf("Pasan los maes de abajo, contador=%d\n",p->counter);
      if(p->counter < Y){
        p->counter+=1;
        llist_pop(down); //Lo saca de la lista(Tiene que quitarse)
        printf("Dejan pasar a un alien, contador=%d\n",p->counter);
        if (p->counter == Y){
          printf("Se alcanzo la cantidad Y y se reinicia contador, contador=%d\n",p->counter);
          p->counter = 0;
          p->lastAccess = 1;
        }
        return 0;
      }
    }else{
      if (direction){ //Si direccion es 1Survival_Algorithm(llist* up,llist* down,bridge* p)or a Y
          p->counter+=1; //Deje pasar 1 alien
          llist_pop(up); //Lo saca de la lista(Tiene que quitarse)
          printf("Dejan pasar a un alien, contador=%d\n",p->counter);
          if (p->counter == Y){ //El contador es igual a Y?
            printf("Se alcanzo la cantidad Y y se reinicia contador, contador=%d\n",p->counter);
            p->counter = 0; // reinicia el contador
            p->lastAccess = 0; //cambia la direccion a 0
          }        
          return 1; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }
    }else{ 
      if (direction){ //Si direccion es 1
      printf("Pasan los maes de arriba, contador=%d\n",p->counter);
        if(p->counter < Y && lenUp != 0){ //Si elcontador es menor a Y
          p->counter+=1; //Deje pasar 1 alien
          llist_pop(up); //Lo saca de la lista(Tiene que quitarse)
          printf("Dejan pasar a un alien, contador=%d\n",p->counter);
          if (p->counter == Y){ //El contador es igual a Y?
            printf("Se alcanzo la cantidad Y y se reSurvival_Algorithm(llist* up,llist* down,bridge* p)inicia contador, contador=%d\n",p->counter);
            p->counter = 0; // reinicia el contador
            p->lastAccess = 0; //cambia la direccion a 0
          }
          return 1; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }else{
        if(p->counter < Y && lenDown != 0){ //Si elcontador es menor a Y
        printf("Pasan los maes de abajo, contador=%d\n",p->counter);
          p->counter+=1; //Deje pasar 1 alien
          llist_pop(down); //Lo saca de la lista(Tiene que quitarse)
          printf("Dejan pasar a un alien, contador=%d\n",p->counter);
          if (p->counter == Y){ //El contador es igual a Y?
            printf("Se alcanzo la cantidad Y y se reinicia contador, contador=%d\n",p->counter);
            p->counter = 0; // reinicia el contador
            p->lastAccess = 1; //cambia la direccion a 0
            
          }
          return 0; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }
    }
return direction;
  ///termina
}

int Semaphore_Algorithm(llist* up,llist* down,bridge* p,float timeN,float timeS){
  float time_Dif;  
  int lenUp = llist_getSize(up);
  int lenDown = llist_getSize(down);
  struct timeval tic, toc;
  int direction = p->lastAccess;
  float accum = p->accumulator;

  gettimeofday(&tic, NULL); //toma el tiempo
  for (int i=0;i<1;i++){
    if (lenUp == 0 && lenDown == 0){
      printf("No hay aliens en ningun lado\n");
      sleep(1);
    }
    else if(direction){
      if(lenUp == 0){
        printf("Ya no hay aliens arriba\n");
        break;
      }
      printf("Cruzo un alien de arriba\n");
      sleep(1);
      p->lastAccess = 1;
      direction = p->lastAccess;
      //return 1;
    }
    else{
      if(lenDown == 0){
        printf("Ya no hay aliens abajo\n");
        break;
      }
      printf("Cruzo un alien de abajo\n");
      sleep(1);
      p->lastAccess = 0;
      direction = p->lastAccess;
      //return 0;
    }
  }
  gettimeofday(&toc, NULL);
  time_Dif = (double)(toc.tv_sec - tic.tv_sec);
  printf("Diferencia de tiempo = %f\n",time_Dif);
  accum+=time_Dif;
  p->accumulator = accum;
  printf("Acumulador en = %f\n", accum); 

  if(direction){
    if(accum >= timeN){
      printf("Ya se cumplio el tiempo para cruzar - Aliens de arriba\n");
      printf("Acumulador en = %f\n", accum);
      p->lastAccess = 0;
      p->accumulator = 0; 
      //direction = p->lastAccess;
      printf("Ahora van cruzando los de abajo\n");
    }
  }
  else{
    if(accum >= timeS){
      printf("Ya se cumplio el tiempo para cruzar - Aliens de bajo\n");
      printf("Acumulador en = %f\n", accum);
      p->lastAccess = 1;
      p->accumulator = 0;
      //direction = p->lastAccess;
      printf("Ahora van cruzando los de arriba\n");
    }
  } 
  return direction;
}

int Survival_Algorithm(llist* up,llist* down,bridge* p){ //funcion miedo(Témanle)
  int temp;
  temp = Y_Algorithm(up,down,p,1);
  return temp;
}