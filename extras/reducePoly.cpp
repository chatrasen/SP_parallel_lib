#include <immintrin.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;


__m256i reducePoly(__m256i *a){
	uint32_t temp; // a temp variable
	uint32_t *ptr0 = (uint32_t*)&a[0];
	uint32_t *ptr1 = (uint32_t*)&a[1];

	for(int i=7; i>=0; i--){
		temp = ptr1[i];		
		ptr0[i] = ptr0[i] ^ (temp << 23);

		int j;
		if(i-7>=0){
			j = i-7;
			ptr1[j] = ptr1[j] ^ (temp >> 9);
		}
		else{
			ptr0[i+1] = ptr0[i+1] ^ (temp >> 9);
		}

		if(i-5>=0){
			j = i-5;
			ptr1[j] = ptr1[j] ^ (temp << 1);
		}
		else{
			ptr0[i+3] = ptr0[i+3] ^ (temp << 1);
		}

		if(i-4>=0){
			j = i-4;
			ptr1[j] = ptr1[j] ^ (temp >> 31);
		}
		else{
			ptr0[i+4] = ptr0[i+4] ^ (temp >> 31);
		}
	}
	temp = ptr0[7]>>9;
	ptr0[0] = ptr0[0] ^ temp;
	ptr0[2] = ptr0[2] ^ (temp << 10);
	ptr0[3] = ptr0[3] ^ (temp >> 22);
	ptr0[7] = ptr0[7] & 0x1FF;


	return a[0];
}


int main(){
	__m256i a[2];
	a[0] = _mm256_set_epi64x(1LL<<41,0,1<<10,1);
	a[1] = _mm256_set_epi64x(0,0,0,0);

	__m256i res = reducePoly(a);

	uint32_t *ptr = (uint32_t*)(&res);
	for(int i=7; i>=0; i--){
		cout << ptr[i] << " ";
	}
	cout << endl;
	return 0;
}