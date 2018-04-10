#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int u32;
typedef unsigned long long int u64;

void add32 ( u32 *c, u32 *a, u32 *b )
{
   int i;
   for (i=0; i<8; ++i) {
      c[i] = a[i] ^ b[i];
   }
}

void sub32 ( u32 *c, u32 *a, u32 *b )
{
   int i;
   for (i=0; i<8; ++i) {
      c[i] = a[i] ^ b[i];
   }
}

void add64 ( u64 *c, u64 *a, u64 *b )
{
   int i;
   for (i=0; i<4; ++i) {
      c[i] = a[i] ^ b[i];
   }
}

void sub64 ( u64 *c, u64 *a, u64 *b )
{
   int i;
   for (i=0; i<4; ++i) {
     c[i] = a[i]^b[i];
   }
}

void mul32(u32 *c, u32 *a, u32 *b)
{
   u32 temp[16] = {0};
   for(int i = 0; i < 8; i++) temp[i] = a[i];
	for(int i = 0; i < 16; i++) c[i] = 0;
	for(int j = 0; j < 256; j++)
	{
		if(b[j/32] & (1 << (j%32)))
      {
         for(int i = 0; i < 16; i++) c[i]^=temp[i];
      }
      for(int i = 15; i > 0; i--)
      {
         temp[i] = (temp[i] << 1) ^ (temp[i-1] >> 31); 
      }
      temp[0] <<= 1;
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
      a[i] = 2;
      b[i] = 0;
   }
   b[0] = b[1] = 1;

   printf("+++ a = "); for (i=7; i>=0; --i) printf("[%u]", a[i]); printf("\n");
   printf("+++ b = "); for (i=7; i>=0; --i) printf("[%u]", b[i]); printf("\n");
   
   int t = 1;
   double di = 0;
      c1 = clock(); for (i=0; i<1000000; ++i) add32(c,a,b); c2 = clock();
      di = (double)(c2-c1) / (double)CLOCKS_PER_SEC;

   printf("+++ c = "); for (i=7; i>=0; --i) printf("[%u]", c[i]); printf("\n");

   printf("+++ Addition time (u32) = %lf\n", di);

   printf("+++ a = "); for (i=7; i>=0; --i) printf("[%u]", a[i]); printf("\n");
   printf("+++ b = "); for (i=7; i>=0; --i) printf("[%u]", b[i]); printf("\n");
   

   c1 = clock(); for (i=0; i<1000000; ++i) mul32(d,a,b); c2 = clock();
   di = (double)(c2-c1) / (double)CLOCKS_PER_SEC;

   printf("+++ d = "); for (i=15; i>=0; --i) printf("[%u]", d[i]); printf("\n");

   printf("+++ Multiplication time (u32) = %lf\n", di);

}