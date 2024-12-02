#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_NUM  50
#define MAX_REP  10

int main () {
  uint64_t sum = 0;
  
  srand(time(NULL));

  for (int i = 0; i < MAX_REP; i++){
    int num = rand() % MAX_NUM + 1;
    printf ("%d\n", num);
    sum += num;
  }

  fprintf(stderr, "#### sum == %lu\n", sum);
  return 0;
}