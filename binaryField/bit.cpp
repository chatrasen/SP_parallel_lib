#include "bit.h"
using namespace std;
#define ll long long

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
	if(j<64){
		if(j!=0)
			return _mm256_set_epi64x((ptr[3]<<j)^(ptr[2]>>(64-j)),(ptr[2]<<j)^(ptr[1]>>(64-j)),(ptr[1]<<j) ^ (ptr[0]>>(64-j)),ptr[0]<<j);
		else 
			return x;
	}
	else if(j<128){
		j -= 64;
		if(j!=0)
			return _mm256_set_epi64x((ptr[2]<<j)^(ptr[1]>>(64-j)),(ptr[1]<<j) ^ (ptr[0]>>(64-j)),ptr[0]<<j,0);
		else 
			return _mm256_set_epi64x(ptr[2],ptr[1],ptr[0],0);
	}
	else if(j<192){
		j -= 128;
		if(j != 0)
			return _mm256_set_epi64x((ptr[1]<<j) ^ (ptr[0]>>(64-j)),ptr[0]<<j,0,0);
		else
			return _mm256_set_epi64x(ptr[1],ptr[0],0,0);
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
		//cout<<"hi\n";
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


bit::bit() 
{
	this->a =  _mm256_set_epi64x(0,0,0,0);
}

bit::bit(long long int b) 
{
	this->a =  _mm256_set_epi64x(0,0,0,b);

}

bit::bit(uint64_t *b) 
{
	this->a =  _mm256_set_epi64x(b[3],b[2],b[1],b[0]);
}

bit::bit(const __m256i &b)
{
	this->a = b;
}
bit bit::operator+(const bit &b)
{
	bit temp(_mm256_xor_si256(this->a,b.a));
	return temp;
}
bit & bit::operator+=(const bit &b)
{
	this->a = _mm256_xor_si256(this->a,b.a);
	return *this;
}

bit bit::operator-(const bit &b)
{
	bit temp(_mm256_xor_si256(this->a,b.a));
	return temp;
}

bit & bit::operator-=(const bit &b)
{
	this->a = _mm256_xor_si256(this->a,b.a);
	return *this;
}

bit bit::operator*(const bit &b)
{
	__m256i result[2];
	__m256i x = b.a;
	mulP(this->a ,x,result);
	bit temp(reducePoly(result));
	return temp;
}

bit bit::operator/(const bit &b)
{
	bit temp;
	__m256i c = b.a, r;
	inversePoly(c,r);
	temp.a = r;
	return (*this)*temp;
}

bit & bit::operator=(int x)
{
	this->a =  _mm256_set_epi64x(0,0,0,x);
	return *this;
}

bool bit::operator!()
{
	uint64_t *f = (uint64_t*) &this->a;
	return !(f[0] || f[1] || f[2] || f[3]);
}
void bit::print()
{
	uint64_t *f = (uint64_t*) &this->a;
	for(int i = 3; i >= 0; i--) cout<<f[i]<<" "; cout<<"\n";
}

static void* bit::operator new[](size_t sz)
{
	void* m = _mm_malloc(sz,32);
	//std::cout<<"bit :: Operator new "<<std::endl;
	return m;
}
  // Overloading CLass specific delete operator
static void bit::operator delete[](void* m)
{
	//std::cout<<"bit :: Operator delete"<<std::endl;
	_mm_free(m);
}
 
ostream & operator<<(ostream &output, const bit &b ) 
{ 
    uint64_t *f = (uint64_t*) &b.a;
	output<<"("; for(int i = 3; i >= 0; i--) output<<f[i]<<" "; output<<")";
    return output;            
}

