#include <immintrin.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

__m256i MOD = _mm256_set_epi64x(1LL<<41,0,1<<10,1);

void addPoly(__m256i &a, __m256i &b, __m256i &temp)
{
	temp = _mm256_xor_si256(a,b);
}

bool isOne(__m256i &u){
	uint64_t *ptr = (uint64_t*)&u;

	return ptr[0]==1 && ptr[1]==0 && ptr[2]==0 && ptr[3]==0;
}

uint32_t leading_zero(uint32_t x)
{
    unsigned n = 0;
    if (x <= 0x0000ffff) n += 16, x <<= 16;
    if (x <= 0x00ffffff) n +=  8, x <<= 8;
    if (x <= 0x0fffffff) n +=  4, x <<= 4;
    if (x <= 0x3fffffff) n +=  2, x <<= 2;
    if (x <= 0x7fffffff) n ++;
    return n;
}


int degree(__m256i &x){
	uint32_t *ptr = (uint32_t*)&x;
	int d = 0;
	for(int i=7; i>=0; i--){
		if(ptr[i]==0){
			d += 32;
		}
		else{
			d += leading_zero(ptr[i]);
			break;
		}
	}
	if(d==256){
		return 0;
	}
	else{
		return 255-d;
	}
}

void swap(__m256i &a, __m256i &b){
	__m256i temp = a;
	a = b;
	b = temp;
}

__m256i shift(__m256i &x, int j){
	uint64_t *ptr = (uint64_t*)(&x);
	if(j<64)
		return _mm256_set_epi64x((ptr[3]<<j)^(ptr[2]>>(64-j)),(ptr[2]<<j)^(ptr[1]>>(64-j)),(ptr[1]<<j) ^ (ptr[0]>>(64-j)),ptr[0]<<j);
	else if(j<128){
		j -= 64;
		return _mm256_set_epi64x((ptr[2]<<j)^(ptr[1]>>(64-j)),(ptr[1]<<j) ^ (ptr[0]>>(64-j)),ptr[0]<<j,0);
	}
	else if(j<192){
		j -= 128;
		return _mm256_set_epi64x((ptr[1]<<j) ^ (ptr[0]>>(64-j)),ptr[0]<<j,0,0);
	}
	else{
		j -= 192;
		return _mm256_set_epi64x(ptr[0]<<j,0,0,0);
	}

}

void inversePoly(__m256i &a, __m256i &g1){
	int j=0;
	__m256i u,v,g2,temp;
	u = a;
	v = MOD;
	g1 = _mm256_set_epi64x(0,0,0,1);
	g2 = _mm256_set_epi64x(0,0,0,0);

	while(!isOne(u)){
		j = degree(u) - degree(v);
		if(j<0){
			swap(u,v);
			swap(g1,g2);
			j *= -1;
		}
		temp = shift(v,j);
		addPoly(u,temp,u);
		temp = shift(g2,j);
		addPoly(g1,temp,g1);
	}
}


int main(){
	__m256i a,res;

	a = _mm256_set_epi64x(0,0,0,1);

	inversePoly(a,res);

	uint32_t *ptr = (uint32_t*)(&res);
	for(int i=7; i>=0; i--){
		cout << ptr[i] << " ";
	}
	cout << endl;
	return 0;
}