/* APPLE LOCAL file 4375453 */
/* { dg-require-effective-target vect_int } */
/* { dg-do run } */

#include <stdlib.h>
#include <stdarg.h>
#include "tree-vect.h"

/* Compile time unknown misalignment. Cannot use loop peeling to align
   the store.  */

#define N 17

struct foo {
  char x0;
  int y[N][N];
} __attribute__ ((packed));

struct foo f2;
int z[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

void fbar(struct foo *fp)
{
  int i,j;
   for (i=0; i<N; i++)
      for (j=0; j<N; j++)
        f2.y[i][j] = z[i];

   for (i=0; i<N; i++)
      for (j=0; j<N; j++)
	if (f2.y[i][j] != z[i])
	  abort ();
}

int main (void)
{
  struct foo  *fp = (struct foo *) malloc (2*sizeof (struct foo));

  fbar(fp);
  return 0;
}

