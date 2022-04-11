
#include <math.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
  int min;
  int max;
  int total;
} primeRange;

void calcPrimes(primeRange *range)
{
  int i, primes = 0;

  int minLimit = range->min;
  int maxLimit = range->max;
  int flag = 0;
  while (minLimit <= maxLimit)
  {
    i = 2;
    int lim = ceil(sqrt(minLimit));
    while (i <= lim)
    {
      if (minLimit % i == 0)
      {
        flag = 0;
        break;
      }
      i++;
    }
    if (!flag)
    {
      primes++;
    }
    flag = 0;
    minLimit++;
  }
  range->total = primes;
}

int main()
{

  double time_spent = 0.0;

  clock_t begin = clock();

  primeRange r1;

  r1.min = 0;
  r1.max = 20000000;

  calcPrimes(&r1);

  printf("-----------------Serial------------------------\n");
  printf("Primes calculated: %d \n", r1.total);

  clock_t end = clock();

  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

  printf("The elapsed time is %f seconds\n", time_spent);
  printf("---------------------------------------------");

  return 0;
}