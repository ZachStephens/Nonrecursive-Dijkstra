#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graph.h"


void insertN(neighbor ** N,int index ,int key, float length)
{
  neighbor* current;
  neighbor* temp = NULL;
  current = N[index];
  while(current)
    {
      temp = current;
      current  = current->next;
      
    }
  current = malloc(sizeof(neighbor));
  current->key = key;
  current->next = NULL;
  current->length = length;
  if(!N[index])
    {
      N[index]=current;
    }
  else
    {
      temp->next = current;
    }
  //printf("\n n[%d] \n",index);
  current = N[index];
  /* while(current) */
  /*   { */
  /*     printf("%d ",current->key); */
  /*     current = current->next; */
  /*   } */
}


void finddist(int index, edge* edges,vert* vertex)
{

  float srcx =vertex[edges[index].src].x;
  float srcy =vertex[edges[index].src].y;
  float destx =vertex[edges[index].dest].x;
  float desty =vertex[edges[index].dest].y;
  float xdist;
  float ydist;
  float dist;
  xdist = destx-srcx;
  ydist = desty-srcy;

  dist = pow(xdist,2)+pow(ydist,2);

  edges[index].length = sqrt(dist);
  //printf("\n%f,%f  %f,%f\n",srcx,srcy,destx,desty);
  //printf("\nproduces %f\n",edges[index].length);
}


neighbor ** dtbuild(char* filename,vert* vertex,neighbor** NBs,int* Vlen)
{
  int Elen;
  int i;
  float x;
  float y;
  edge * edges;
  char buffer[50];
  char* start;
  neighbor* current= NULL;


  printf("opening %s \n",filename);
  FILE * fp = fopen(filename,"r");
  if(!fp)
    {
      fprintf(stderr,"Did not open file correctly\n");
    }
  

  printf("file opened.  Now scanning Vlen and Elen\n");
 
  fgets(buffer,sizeof(buffer),fp);
  start = buffer;
  sscanf(start,"%d%d",Vlen,&Elen);

  printf("Vlen = %d  and Elen = %d \n",*Vlen,Elen);
  vertex = malloc((*Vlen)*sizeof(vert));
  
  
  //populate vertices
  for(i=0;i<*Vlen;i++)
  {
    fgets(buffer,sizeof(buffer),fp);    
    start = buffer;
    //printf("buffer = ");
    sscanf(start,"%*d %f %f",&x, &y);
    //fscanf(fp,"%*d" "%f %f", &x,&y);
    vertex[i].x =x;
    vertex[i].y =y;
    
    //printf("vertex[%d].x = %f  and vertex[%d].y = %f",i,vertex[i].x,i,vertex[i].y);
  }
 
  //populate edges
  edges = malloc(Elen*sizeof(edge));
  for(i=0;i<Elen;i++)
    {
      fgets(buffer,sizeof(buffer),fp);    
      start = buffer;
      sscanf(start,"%d%d",&(edges[i].src),&(edges[i].dest));    
      finddist(i,edges,vertex);      
    }
  fclose(fp);// could do error check for EOF
  
  NBs = malloc((*Vlen)*sizeof(neighbor*));
  for(i=0;i<*Vlen;i++)
    {
      NBs[i]=NULL;
    }

  // printf("N= %p \n",NBs);

  //build neighbors by inserting them in NBs
  for(i=0;i<Elen;i++)
    {
      //printf("%f\n",edges[i].length);
      insertN(NBs,edges[i].src,edges[i].dest,edges[i].length);      
      insertN(NBs,edges[i].dest,edges[i].src,edges[i].length);
    }
  
  /* for(i=0;i<Vlen;i++) */
  /*   { */
  /*     //printf("\nVertex %d has friends:\n",i); */
  /*     current = NBs[i]; */
  /*     printf("\n%d has friends:\n",i); */
  /*     while(current) */
  /* 	{ */
  /* 	  printf("%d(length %f)\n",current->key,current->length); */
  /* 	  current = current->next; */
  /* 	} */
  /*   } */
   //free edges
  free(edges);
  return NBs;
}

void printNB(neighbor** NB,int key)
{
  neighbor* find= NULL;
 
     if(!NB)
	{
	  printf("NULL");
	}
      else
	{
	  printf(" %d: ",key);
	  find = NB[key];
	  while(find)
	    {
	      printf("%d ",find->key);
	      find = find->next;
	    }
	}
  
}


int main(int argc, char** argv)
{
  vert* vertex=NULL;
  neighbor** NBs=NULL;
  //neighbor* find = NULL;
  //int key;
  int Vlen;
  int i;
  if(argc==2)
    {
      printf("entering dtbuild\n");
      NBs = dtbuild(argv[1],vertex,NBs,&Vlen);
      printf("exited dtbuild\n");
      // printf("Enter key\n");
      // scanf("%d",&key);
      for(i=0;i<Vlen;i++)
	{
	  printNB(NBs,i);
	  printf("\n");
	}

      //need to read NBs
      //make function that accesses NBs for given Vertex
 


  //dijkstra
  //need path list
  //output
  //reverse path list
      return 0;
    }
  return ERROR;
}

