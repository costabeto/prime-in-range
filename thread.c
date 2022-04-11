#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NUM_THREADS 20
#define THREAD_LOAD 1000000

typedef struct
{
  int min;
  int max;
  int total;
} primeRange;

void *calcPrimes(void *arg)
{
  int i, primes = 0;
  primeRange *range;
  range = (primeRange *)arg;

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
  pthread_exit(NULL);
}

int main()
{

  struct timespec start, finish;
  double time_spent;
  clock_gettime(CLOCK_MONOTONIC, &start);

  pthread_t threads[NUM_THREADS];
  primeRange pr[NUM_THREADS];
  int rc;
  pthread_attr_t attr;
  void *status;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  for (int t = 1; t <= NUM_THREADS; t++)
  {
    pr[t].min = (t - 1) * THREAD_LOAD + 1;
    pr[t].max = t * THREAD_LOAD;
    rc = pthread_create(&threads[t], NULL, &calcPrimes, &pr[t]);
    if (rc)
    {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  int totalPrimesFound = 0;
  // free attribute and wait for the other threads
  pthread_attr_destroy(&attr);
  for (int t = 1; t <= NUM_THREADS; t++)
  {
    rc = pthread_join(threads[t], &status);
    if (rc)
    {
      printf("Error:unable to join, %d", rc);
      exit(-1);
    }
    totalPrimesFound += pr[t].total;
  }

  printf("-----------------PTHREAD------------------------\n");
  printf("Primes calculated: %d \n", totalPrimesFound);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_spent = (finish.tv_sec - start.tv_sec);
  time_spent += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  printf("The elapsed time is %0.2f seconds\n", time_spent);
  printf("---------------------------------------------");
  pthread_exit(NULL);

  return 0;
}