#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Constant Definitions

/* Return/Error Codes */
#define OK               (  0 )  // No errors, everything as should be
#define ERROR            ( -1 ) // Generic error

// structures

typedef struct vert{
  float x;
  float y;
  float distance;
}vert;

typedef struct edge{
  int src;
  int dest;
  float length;
}edge;
  
typedef struct neighbor{
  int key;
  struct neighbor* next;
  float length;
}neighbor;

typedef struct query{
  int src;
  int dest;
}query;

typedef struct finish{
  int key;
  struct finish* next;
}finish;

typedef struct unfinish{
  int key;
  struct unfinish*next;
}unfinish;
/* Function Prototypes */


query* getquery(char*,int*);

neighbor** dtbuild(char*,vert*,neighbor**,int*);
  
//void buildneighbor(neighbor**,edge*,int,int);

void insertN(neighbor**,int,int,float);

void printNB(neighbor** NB,int key);

void dijkstra(neighbor**,int,int,int);

int getsmallest(unfinish*,float*);

unfinish* enterUF(unfinish*,finish*,int);

int checkUF(unfinish*,int);

void deconstructF(finish*);

void deconstructUF(unfinish*);
