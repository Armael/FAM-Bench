#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <fam.h>

/* Subtract the `struct timeval' values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0.  */

int timeval_subtract (result, x, y)
     struct timeval *result, *x, *y;
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

void print_timeval(struct timeval *t)
{
    printf("%d secondes et %d microsecondes", (int)t->tv_sec, (int)t->tv_usec);
}

/* Retourne -1 en cas d'échec, 0 sinon */
int create_file(char* name)
{
    FILE* f;
    if((f = fopen(name, "w")) != NULL) {
        fclose(f);
        return 0;
    }
    return 1;
}

/* Retourne -1 en cas d'échec, 0 sinon */
int modify_file(char* name)
{
    FILE* f;
    if((f = fopen(name, "a")) != NULL) {
        fputc('a', f);
        fclose(f);
        return 0;
    }
    return 1;
}

int main()
{
    char* test_file_name = "/tmp/fam_bench_temp_file";

    struct timeval t1, t2, diff;

    FAMConnection fc;
    FAMRequest fr;
    FAMEvent fe;

    if(FAMOpen(&fc) == -1) {
        printf("Error opening a FAM connection\n");
        exit(1);
    }

    create_file(test_file_name);

    if(FAMMonitorFile(&fc, test_file_name, &fr, NULL) == -1) {
        printf("Error monitoring %s\n", test_file_name);
        FAMClose(&fc);
        exit(1);
    }

    gettimeofday(&t1, NULL);
    modify_file(test_file_name);
    while(!FAMNextEvent(&fc, &fe) && (fe.code != FAMChanged));
    gettimeofday(&t2, NULL);

    timeval_subtract(&diff, &t2, &t1);
    print_timeval(&diff);
    return 0;
}
