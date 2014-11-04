#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 65536
#define ITER 1000000

float a[N];
float b[N];
float c[N];

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

int main(int argc, char** argv){
    int i,n;
    long verif, exec, alloc = nanoTime();
    for(i = 0 ; i < N; i++){
      a[i] = 0.25f;
      b[i] = 0.75f;
      c[i] = 0.0f;
    }
    printf("initialisation : %s\n", displayTimeFormat(nanoTime()-alloc));
    exec = nanoTime();
    for(n = 0 ; n < ITER; n++){
      for(i = 0 ; i < N; i++){
        c[i] = a[i]+b[i];
      }
    }
    printf("execution : %s\n", displayTimeFormat((nanoTime()-exec)/ITER));
    verif = nanoTime();
    for(i = 0 ; i < N; i++){
      if(1.0 != c[i]){
        perror("Error during verification\n");
      }
    }
    printf("verification : %s\n", displayTimeFormat(nanoTime()-verif));
}

          
