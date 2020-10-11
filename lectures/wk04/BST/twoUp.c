#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RUNS 10000
#define BET  10

int main(void) {
   srandom(time(NULL));

   int coin1, coin2, n, sum = 0;
   for (n = 0; n < RUNS; n++) {
      do {
	 coin1 = random() % 2;
	 coin2 = random() % 2;
      } while (coin1 != coin2);
      if (coin1==1 && coin2==1)
	 sum += BET;
      else 
	 sum -= BET;
   }
   printf("Final result: %d\n", sum);
   printf("Average outcome: %f\n", (float) sum / RUNS);
   return 0;
}
