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
  current = N[index];

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
  //ighbor* current= NULL;


  printf("opening %s \n",filename);
  FILE * fp = fopen(filename,"r");
  if(!fp)
    {
      fprintf(stderr,"Did not open %s file correctly\n",filename);
      return NULL;
    }
  

  //printf("file opened.  Now scanning Vlen and Elen\n");
 
  fgets(buffer,sizeof(buffer),fp);
  start = buffer;
  sscanf(start,"%d%d",Vlen,&Elen);

  // printf("Vlen = %d  and Elen = %d \n",*Vlen,Elen);
  vertex = malloc((*Vlen)*sizeof(vert));
  
  
  //populate vertices
  for(i=0;i<*Vlen;i++)
  {
    fgets(buffer,sizeof(buffer),fp);    
    start = buffer;
    sscanf(start,"%*d %f %f",&x, &y);
    vertex[i].x =x;
    vertex[i].y =y;

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

  for(i=0;i<Elen;i++)
    {
      insertN(NBs,edges[i].src,edges[i].dest,edges[i].length);      
      insertN(NBs,edges[i].dest,edges[i].src,edges[i].length);
    }
  free(edges);
  free(vertex);
  
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



query* getquery(char* filename,int* Qlen)
{
  FILE* fp  = fopen(filename,"r");
  char buffer[50];
  int x;
  int y;
  int i;
  query* Q;
  char* start;

   if(!fp)
    {
      fprintf(stderr,"Did not open %s correctly\n",filename);
      return NULL;
    }

   fgets(buffer,sizeof(buffer),fp);
   start = buffer;
   sscanf(start,"%d",Qlen);
   
   Q = malloc(sizeof(query)*(*Qlen));
     //populate vertices
  for(i=0;i<*Qlen;i++)
  {
    fgets(buffer,sizeof(buffer),fp);    
    start = buffer;
    sscanf(start,"%d %d",&x, &y);
    Q[i].src = x;
    Q[i].dest = y;
  }
  return Q;
  
}


int getsmallest(unfinish * UF,float*D)
{
  //printf("enter get smallest\n");
  unfinish* current = UF;
  float small;
  unfinish* prev = NULL;
  unfinish* tmp = NULL;
  int key;
  tmp = UF;
  small = D[UF->key];
  tmp = NULL;
  while(current!=NULL)
    {
      if(D[current->key] < small)
	{
	  small = D[current->key];	  
	  tmp = prev;
	}
      prev = current;
      current = current->next;
    }
  if(tmp==NULL)
    {
      key = UF->key;
      if(UF->next != NULL)
	{
	  UF->key = (UF->next)->key;
	  prev = UF->next;
	  UF->next = prev->next;
	  free(prev);
	}
    }
  else
    {
      current = tmp->next;  //seg faultttttttttt
      tmp->next = current->next;
      key = current->key;
      free(current);
    }
  tmp = UF;
  return key;
}


//checks if key is in finish list
int checkfinish(finish * done,int key)
{
  finish* current = done;
  //printf("checking for %d\n",key);
  while(current!=NULL)
    {
      if(key==(current->key))
	{
	  return 0;
	}
      current = current->next;
    }
  return 1;
}

//put key into unfinish list
unfinish* enterUF(unfinish* UF,finish * done,int key)
{
  unfinish* new=NULL;
  unfinish* tmp = NULL;
  if(!UF)
    {
      //printf("unfinished is empty  '%d' is key\n",key);
      new = malloc(sizeof(unfinish));
      new->key = key;
      new->next=NULL;
      return new;//returns overwrited pointer to update from NULL
    }
  else
    {
      new = UF;
      while(new!=NULL)
	{
	  if(new->key ==key)
	    {
	      return UF;
	    }
	  tmp = new;
	  new = new->next;
	
	}
      tmp->next =  malloc(sizeof(unfinish));
      new = tmp->next;
      new->key = key; 
      new->next = NULL;
      return UF;// returns original pointer with updated contents
    }
  
}


void deconstructUF(unfinish* Ndone)
{
  if(Ndone != NULL)
    {
      deconstructUF(Ndone->next);
      free(Ndone);
    }
}

void deconstructF(finish* done)
{
  if(done != NULL)
    {
      deconstructF(done->next);
      free(done);
    }  
}


void dijkstra(neighbor** NBs,int Vlen,int s,int d)
{
  int* prev = NULL;
  int* print=NULL;
  float* distance = NULL;
  int i;
  int j;
  int fcount = 0;
  int current;
  int last;
  finish* done=NULL;
  finish* new = NULL;
  unfinish* notdone=NULL;
  unfinish* nd2 = NULL;
  neighbor* curr2;
  float A;
  if(s >= Vlen)
    {
      printf("The path between %d and %d does not exists\n",s,d);
      return;
    }
  done = malloc(sizeof(finish));
  done->key = s;
  done->next = NULL;
  prev = malloc(sizeof(int)*Vlen);
  distance = malloc(sizeof(float)*Vlen);

  for(i=0;i<Vlen;i++)
    {
      prev[i]=-1;
      distance[i]=99999999;
    }
  distance[s]=0;
  current = s;
  while((fcount != Vlen))
    {
      curr2 = NBs[current];//iterates through neighbors of current
      while(curr2!=NULL)//curr2 is the neighbor to be added
	{
	  if(checkfinish(done,curr2->key)==1){
	    notdone = enterUF(notdone,done,curr2->key);
	    A = distance[current] + (curr2->length);	      
	    if(A < distance[(curr2->key)])
	      {
		distance[(curr2->key)]=A;
		prev[curr2->key] = current;
	      }
	  }
	  curr2=curr2->next;
	  // printf("\n");
	}
      current=getsmallest(notdone,distance); 
      if(done==NULL)
	{
	  printf("error finished is null\n");
	}
      new = done;
      while((new->next)!=NULL)
	{
      	  new = new->next;
	}
      new->next = malloc(sizeof(finish));
      (new->next)->key = current;
      (new->next)->next = NULL;
      fcount++;
      if(current == d)
	{
	  fcount = Vlen;
	}
    }
  if(current == d)
    {
      new =done;
      while((nd2)!=NULL)
	{
	  printf("%d ",new->key);
	  new = new->next;
	}
      printf("The path from %d to %d is %f\n",s,d,distance[current]);

      i=0;
      last = current;
      while(current != -1)
	{
	  i++;
	  current = prev[current];     
	}
      j=i;
      print=malloc(sizeof(int)*i);
      i--;
      while(last!=-1)
	{
	  print[i]=last;
	  last = prev[last];
	  i--;
	}
      for(i=0;i<j;i++)
	{
	  printf("%d ",print[i]);
	}
      printf("\n");
      if(print)
	{
	  free(print);
	}
    }
  else
      printf("The path from %d to %d does not exits\n",s,d);
  if(distance)
      free(distance);
  if(prev)
      free(prev);
  deconstructF(done);
  deconstructUF(notdone);

}



int main(int argc, char** argv)
{
  vert* vertex=NULL;
  neighbor** NBs=NULL;
  neighbor* current=NULL;
  neighbor* tmp = NULL;
  int Vlen;
  int i;
  query* Q=NULL;
  int Qlen;
  printf("%d",argc);
  if(argc==3)
    {
      printf("entering dtbuild\n");
      NBs = dtbuild(argv[1],vertex,NBs,&Vlen);
      free(vertex);
      if(NBs!=NULL)
	{
	  Q=getquery(argv[2],&Qlen);
	  for(i=0;i<Qlen;i++)
	    {
	      printf("\n");
	      printf("Enter the dijkstra\n");
	      dijkstra(NBs,Vlen,Q[i].src,Q[i].dest);
	      printf("\n");
	    }
	}
   
      for(i=0;i<Vlen;i++)
	{
	  current = NBs[i];
	  while(current!=NULL)
	    {
	      tmp = current;
	      current = current->next;
	      free(tmp);
	    }
	}
      free(Q);
  //dijkstra
  //need path list
  //output
  //reverse path list
      return 0;
    }
  return ERROR;
}

