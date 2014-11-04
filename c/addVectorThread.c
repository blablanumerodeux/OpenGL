#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define N 65536
#define ITER 1000000

float a[N];
float b[N];
float c[N];
int pt_args[2];
int pt_argsthread2[2];

long nanoTime(){
  struct timeval val;
  long res = 0;
  if(gettimeofday(&val, NULL)){
    perror("ouch");
    exit(1);
  }
  res = val.tv_sec*1e9+val.tv_usec*1e3;
  return res;
}

char buffer[256];

char *displayTimeFormat(long time){
  sprintf(buffer, "%ds %03d %03d %03ld", (int)(time/1e9), ((int)(time/1e6)%1000), ((int)(time/1e3)%1000), time%1000);
  return buffer;
}

/* Fonction pour les threads */
static void * fn_add(void* parms)
{
    int i,n;
    int *params = parms;
    int iter_deb=params[0];
    int iter_fin=params[1];
    for(n = iter_deb ; n < iter_fin; n++){
      for(i = 0 ; i < N; i++){
        c[i] = a[i]+b[i];
      }
    }
   return NULL;
}

int main(int argc, char** argv){
    int i,n;
    pthread_t thread_1;
    pthread_t thread_2;
    int res_thread1= 0;
    int res_thread2= 0;
    long verif, exec, alloc = nanoTime();
    for(i = 0 ; i < N; i++){
      a[i] = 0.25f;
      b[i] = 0.75f;
      c[i] = 0.0f;
    }
    printf("initialisation : %s\n", displayTimeFormat(nanoTime()-alloc));
    exec = nanoTime();
    pt_args[0] = 0;
    pt_args[1] = 500000;
    res_thread1= pthread_create (
            &thread_1 ,NULL  ,
            fn_add,(void *) &pt_args
         );
    pt_argsthread2[0] = 500000;
    pt_argsthread2[1] = 1000000;
    res_thread2= pthread_create (
            &thread_2 ,NULL ,
            fn_add,(void *) &pt_argsthread2
         );
    
    printf("execution : %s\n", displayTimeFormat((nanoTime()-exec)/ITER));
    verif = nanoTime();
    for(i = 0 ; i < N; i++){
      if(1.0 != c[i]){
        perror("Error during verification\n");
      }
    }
    printf("verification : %s\n", displayTimeFormat(nanoTime()-verif));
}

