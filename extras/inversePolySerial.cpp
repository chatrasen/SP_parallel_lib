#include <immintrin.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

typedef unsigned int u32;

//__m256i MOD = _mm256_set_epi64x(1LL<<41,0,1<<10,1);

void add32 ( u32 *c, u32 *a, u32 *b )
{
   int i;
   for (i=0; i<8; ++i) {
      c[i] = a[i] ^ b[i];
   }
}
bool isOne(u32* &u){
	uint64_t *ptr = (uint64_t*)&u;

	return ptr[0]==1 && ptr[1]==0 && ptr[2]==0 && ptr[3]==0 && ptr[4]==0 && ptr[5]==0 && ptr[6]==0 && ptr[7]==0;
}

uint32_t leading_zero(uint32_t x)
{
    unsigned n = 0;
    if (x <= 0x0000ffff) n += 16, x <<= 16;
    if (x <= 0x00ffffff) n +=  8, x <<= 8;
    if (x <= 0x0fffffff) n +=  4, x <<= 4;
    if (x <= 0x3fffffff) n +=  2, x <<= 2;
    if (x <= 0x7fffffff) n++;
    return n;
}


int degree(u32* ptr){
	//uint32_t *ptr = (uint32_t*)&x;
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


void shift(u32 *ptr, int j){
	//cout<<"ptr : "; for(int i = 0; i < 8; i++) cout<<ptr[i]<<" "; cout<<"\n";
	if(j<32){
		if(j!=0)
		{
			for(int i = 7; i > 0; i--)
				ptr[i] = (ptr[i] << j)^(ptr[i-1] >> (32 - j));
			ptr[0] <<= j;
		}
	}
	else if(j<64){
		j -= 32;
		if(j!=0)
		{
			for(int i = 7; i > 1; i--)
				ptr[i] = (ptr[i-1] << j)^(ptr[i-2] >> (32 - j));
			ptr[1] = ptr[0] << j;
			ptr[0] = 0;
		}
		else
		{
			for(int i = 7; i > 0; i--) ptr[i] = ptr[i-1];
			ptr[0] = 0;
		}
		
	}
	else if(j<96){
		j -= 64;
		if(j!=0)
		{
			for(int i = 7; i > 2; i--)
				ptr[i] = (ptr[i-2] << j)^(ptr[i-3] >> (32 - j));
			ptr[2] = ptr[0] << j;
			ptr[0] = ptr[1] = 0;
		}
		else
		{
			for(int i = 7; i > 1; i--) ptr[i] = ptr[i-2];
			ptr[1] = 0;
			ptr[0] = 0;
		}
		
	}
	else if(j < 128){
		j -= 96;
		if(j!=0)
		{
			for(int i = 7; i > 3; i--)
				ptr[i] = (ptr[i-3] << j)^(ptr[i-4] >> (32 - j));
			ptr[3] = ptr[0] << j;
			ptr[0] = ptr[1]= ptr[2] = 0;
		}
		else
		{
			for(int i = 7; i > 2; i--) ptr[i] = ptr[i-3];
			ptr[2] = 0;
			ptr[1] = 0;
			ptr[0] = 0;
		}
		
	}
	else if(j < 160){
		j -= 128;
		if(j!=0)
		{
			for(int i = 7; i > 4; i--)
				ptr[i] = (ptr[i-4] << j)^(ptr[i-5] >> (32 - j));
			ptr[4] = ptr[0] << j;
			ptr[0] = ptr[1]= ptr[2] = ptr[3] = 0;
		}
		else
		{
			for(int i = 7; i > 3; i--) ptr[i] = ptr[i-4];
			ptr[3] = 0;
			ptr[2] = 0;
			ptr[1] = 0;
			ptr[0] = 0;
		}
		
	}
	else if(j < 192){
		j -= 160;
		if(j!=0)
		{
			for(int i = 7; i > 5; i--)
				ptr[i] = (ptr[i-5] << j)^(ptr[i-6] >> (32 - j));
			ptr[5] = ptr[0] << j;
			ptr[0] = ptr[1]= ptr[2] = ptr[3] = ptr[4] = 0;
		}
		else
		{
			for(int i = 7; i > 4; i--) ptr[i] = ptr[i-4];
			ptr[4] = 0;
			ptr[3] = 0;
			ptr[2] = 0;
			ptr[1] = 0;
			ptr[0] = 0;
		}
		
	}
	else if(j < 224){
		j -= 192;
		if(j!=0)
		{
			for(int i = 7; i > 6; i--)
				ptr[i] = (ptr[i-5] << j)^(ptr[i-6] >> (32 - j));
			ptr[5] = ptr[0] << j;
			ptr[0] = ptr[1]= ptr[2] = ptr[3] = ptr[4] = 0;
		}
		else
		{
			for(int i = 7; i > 5; i--) ptr[i] = ptr[i-5];
			ptr[5] = 0;
			ptr[4] = 0;
			ptr[3] = 0;
			ptr[2] = 0;
			ptr[1] = 0;
			ptr[0] = 0;
		}
		
	}
	else
	{
		j-=224;
		ptr[7] = ptr[0];
		ptr[6] = 0;
		ptr[5] = 0;
		ptr[4] = 0;
		ptr[3] = 0;
		ptr[2] = 0;
		ptr[1] = 0;
		ptr[0] = 0;

		
	}

	//cout<<"ptr : "; for(int i = 0; i < 8; i++) cout<<ptr[i]<<" "; cout<<"\n";
}

void print(u32* res){
	for(int i=7; i>=0; i-=2){
		cout << (((1ULL*res[i])<<32 ) + 1ULL*res[i-1]) << " ";
	}
	cout << endl;
}

void swap1(u32 *u, u32 *v)
{
	u32 temp[8];
	for(int i = 0; i < 8; i++) temp[i] = u[i];
	for(int i = 0; i < 8; i++) u[i] = v[i];
	for(int i = 0; i < 8; i++) v[i] = temp[i];
}

void inverseSerial(u32 *u, u32 *g1){
	//cout << "ajhsjahs\n";
	int j=0;
	//__m256i u,v,g2,temp;
	//u = a;
	u32 v[] = {1<<9,0,0,0,0,1<<10,0,1};
	reverse(v,v+8);
	//g1 = _mm256_set_epi64x(0,0,0,1);
	for(int i = 0; i < 8; i++) g1[i] = 0;
	g1[0] = 1;
	u32 g2[8] = {0};// = _mm256_set_epi64x(0,0,0,0);
	u32* temp;
	int t=2;
	//while(!isOne(u)){
	while(t--){
		j = degree(u) - degree(v);
		if(j < 0){
			swap1(u,v);
			swap1(g1,g2);
			j = -j;
		}
		u32 temp[8];
		for(int i = 0; i < 8; i++) temp[i] = v[i];
	    shift(temp,j);
		add32(u,temp,u);
		for(int i = 0; i < 8; i++) temp[i] = g2[i];
		shift(temp,j);
		add32(g1,temp,g1);
	}
}


int main(){
	u32 a[] = {875546,0,5454568,0,567889,0,0,0};
	u32 res[8];
	u32 b[] = {1,0,0,0,0,0,0,0};
	// int j;;
	// cin>>j;
	// shift(b,j);
	// for(int i = 0 ; i < 8; i++) cout<<b[i]<<" " ;cout<<"\n";

	inverseSerial(a,res);

	//uint64_t *ptr = (uint64_t*)(&res);
	for(int i=7; i>=0; i-=2){
		cout << ((1LL*res[i])<<32 ) + 1LL*res[i-1] << " ";
	}
	cout << endl;
	return 0;
}