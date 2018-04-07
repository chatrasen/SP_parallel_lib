#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int u32;
typedef unsigned long long int u64;

void add32 ( u32 *c, u32 *a, u32 *b )
{
   int i;
   int carry;
   u32 t;

   carry = 0;
   for (i=0; i<8; ++i) {
      t = a[i] + b[i];
      if (carry == 0) {
         if (t < a[i]) carry = 1;
      } else {
         ++t;
         if (t > a[i]) carry = 0;
      }
      c[i] = t;
   }
}

void sub32 ( u32 *c, u32 *a, u32 *b )
{
   int i;
   int borrow;
   u32 t;

   borrow = 0;
   for (i=0; i<8; ++i) {
      t = a[i] - b[i];
      if (borrow == 0) {
         if (a[i] < b[i]) borrow = 1;
      } else {
         t--;
         if (a[i] > b[i]) borrow = 0;
      }
      c[i] = t;
   }
}

void add64 ( u64 *c, u64 *a, u64 *b )
{
   int i;
   int carry;
   u64 t;

   carry = 0;
   for (i=0; i<4; ++i) {
      t = a[i] + b[i];
      if (carry == 0) {
         if (t < a[i]) carry = 1;
      } else {
         ++t;
         if (t > a[i]) carry = 0;
      }
      c[i] = t;
   }
}

void sub64 ( u64 *c, u64 *a, u64 *b )
{
   int i;
   int borrow;
   u64 t;

   borrow = 0;
   for (i=0; i<4; ++i) {
      t = a[i] - b[i];
      if (borrow == 0) {
         if (a[i] < b[i]) borrow = 1;
      } else {
         t--;
         if (a[i] > b[i]) borrow = 0;
      }
      c[i] = t;
   }
}

void mul32(u32 *c, u32 *a, u32 *b)
{
	u64 t;
	u32 carry ;
	for(int i = 0; i < 16; i++) c[i] = 0;
	for(int i = 0; i < 8; i++)
	{
		carry = 0;
		for(int j = 0; j < 8; j++)
		{
			t = ((u64)1*b[i])*a[j] + carry + c[i+j];
			c[i+j] = (u32)t;
			carry = (u32)(t >> 32);
		}
	}
}

int main ( int argc, char *argv[] )
{
   u32 a[8], b[8], c[8], d[16];
   u64 A[4], B[4], C[4];
   int i;
   clock_t c1, c2;

   srand((unsigned int)time(NULL));

   for (i=0; i<8; ++i) {
      a[i] = rand()%1000;
      b[i] = rand()%1000;
   }

   printf("+++ a = "); for (i=7; i>=0; --i) printf("[%u]", a[i]); printf("\n");
   printf("+++ b = "); for (i=7; i>=0; --i) printf("[%u]", b[i]); printf("\n");
   
   c1 = clock(); for (i=0; i<1000000; ++i) add32(c,a,b); c2 = clock();

   printf("+++ c = "); for (i=7; i>=0; --i) printf("[%u]", c[i]); printf("\n");

   printf("+++ Addition time (u32) = %lf\n", (double)(c2-c1) / (double)CLOCKS_PER_SEC);

   printf("+++ a = "); for (i=7; i>=0; --i) printf("[%u]", a[i]); printf("\n");
   printf("+++ b = "); for (i=7; i>=0; --i) printf("[%u]", b[i]); printf("\n");
   
   c1 = clock(); for (i=0; i<1000000; ++i) mul32(d,a,b); c2 = clock();

   printf("+++ d = "); for (i=15; i>=0; --i) printf("[%u]", d[i]); printf("\n");

   printf("+++ Multiplication time (u32) = %lf\n", (double)(c2-c1) / (double)CLOCKS_PER_SEC);
}