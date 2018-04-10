#include <immintrin.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;


void print(__m256i &a)
{
	uint64_t *f = (uint64_t*) &a;
	for(int i = 0; i < 4; i++) cout<<f[i]<<" "; cout<<"\n";
}


void addP(__m256i &a, __m256i &b, __m256i &temp)
{
	temp = _mm256_xor_si256(a,b);
}

void mulP(__m256i &a,__m256i &b, __m256i *result)
{
	uint16_t *ptr = (uint16_t*) &a;
	__m256i SA[16];
	uint16_t final[16];
	__m256i C[17];
	for(int i = 0; i < 16; i++)
	{
		SA[i] = _mm256_set_epi16(((ptr[15]<<i) ^ (ptr[14] >> (16-i))),((ptr[14]<<i) ^ (ptr[13] >> (16-i))),((ptr[13]<<i) ^ (ptr[12] >> (16-i))),((ptr[12]<<i) ^ (ptr[11] >> (16-i))),((ptr[11]<<i) ^ (ptr[10] >> (16-i))),((ptr[10]<<i) ^ (ptr[9] >> (16-i))),((ptr[9]<<i) ^ (ptr[8] >> (16-i))),((ptr[8]<<i) ^ (ptr[7] >> (16-i))),((ptr[7]<<i) ^ (ptr[6] >> (16-i))),((ptr[6]<<i) ^ (ptr[5] >> (16-i))),((ptr[5]<<i) ^ (ptr[4] >> (16-i))),((ptr[4]<<i) ^ (ptr[3] >> (16-i))),((ptr[3]<<i) ^ (ptr[2] >> (16-i))),((ptr[2]<<i) ^ (ptr[1] >> (16-i))),((ptr[1]<<i) ^ (ptr[0] >> (16-i))),ptr[0]<<i);
		final[i] = (ptr[15] >> (16-i));
	}
	for(int i = 0; i < 17; i++)
		C[i] =  _mm256_set_epi16(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

	ptr = (uint16_t*) &b;
	for(int k = 0; k < 16; k++)
	{
		__m256i bit_extract = _mm256_set_epi16((1 << k & ptr[15])? -1 : 0,(1 << k & ptr[14])? -1 : 0,(1 << k & ptr[13])? -1 : 0,(1 << k & ptr[12])? -1 : 0,(1 << k & ptr[11])? -1 : 0,(1 << k & ptr[10])? -1 : 0,(1 << k & ptr[9])? -1 : 0,(1 << k & ptr[8])? -1 : 0,(1 << k & ptr[7])? -1 : 0,(1 << k & ptr[6])? -1 : 0,(1 << k & ptr[5])? -1 : 0,(1 << k & ptr[4])? -1 : 0,(1 << k & ptr[3])? -1 : 0,(1 << k & ptr[2])? -1 : 0,(1 << k & ptr[1])? -1 : 0,(1 << k & ptr[0])? -1 : 0);
		uint16_t *gtr = (uint16_t*) &SA[k];
		for(int j = 0; j < 17; j++)
		{
			__m256i temp1;
			if(j!=16) temp1 = _mm256_set1_epi16(gtr[j]);
			else temp1 = _mm256_set1_epi16(final[k]);
			__m256i temp2 = _mm256_and_si256(temp1, bit_extract);
			C[j] = _mm256_xor_si256(temp2,C[j]);

		}
	}
	uint16_t *cptr[17];
	for(int i = 0; i < 17; i++)
	{
		cptr[i] = (uint16_t*) &C[i];
	}
	uint16_t setHelp[32] = {0};
	for(int i= 0; i<17; i++){
		for(int j=0; j<16; j++){
			setHelp[i+j] ^= cptr[i][j];
		}
	}
	result[0] = _mm256_set_epi16(setHelp[15],setHelp[14],setHelp[13],setHelp[12],setHelp[11],setHelp[10],setHelp[9],setHelp[8],setHelp[7],setHelp[6],setHelp[5],setHelp[4],setHelp[3],setHelp[2],setHelp[1],setHelp[0]);
	result[1] = _mm256_set_epi16(setHelp[16+15],setHelp[16+14],setHelp[16+13],setHelp[16+12],setHelp[16+11],setHelp[16+10],setHelp[16+9],setHelp[16+8],setHelp[16+7],setHelp[16+6],setHelp[16+5],setHelp[16+4],setHelp[16+3],setHelp[16+2],setHelp[16+1],setHelp[16+0]);
}


int main(){

	srand((unsigned int)time(NULL));

	__m256i a = _mm256_set_epi16(2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2);
	__m256i b = _mm256_set_epi16(16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1);

	__m256i result[2];
	__m256i temp;
	clock_t start,end;
	

	int i;
	start = clock();

	for(i=0; i<1000000; i++){
		addP(a,b,temp);		

	}
	end = clock();
	cout<<"a : "; print(a);
	cout<<"b : "; print(b);
	cout<<"result : "; print(temp);
	printf("addition time taken by parallel code = %.10lf\n", (double)(end-start)/CLOCKS_PER_SEC);

	start = clock();

	for(i=0; i<1000000; i++){
		mulP(a,b,result);		
	}

	end = clock();
	short int *f = (short int*) &a;
	cout<<"a : ";  for(int j = 0; j < 16; j++) cout<<f[j]<<" "; cout<<"\n";
	f = (short int*) &b;
	cout<<"b : ";  for(int j = 0; j < 16; j++) cout<<f[j]<<" "; cout<<"\n";
    f = (short int*) &result[0];
	cout<<"result[0] : "; for(int j = 0; j < 16; j++) cout<<f[j]<<" "; cout<<"\n";
	 f = (short int*) &result[1];
	cout<<"result[1] : "; for(int j = 0; j < 16; j++) cout<<f[j]<<" "; cout<<"\n";

	printf("multiplication time taken by parallel code = %.10lf\n", (double)(end-start)/CLOCKS_PER_SEC);

	return 0;
}