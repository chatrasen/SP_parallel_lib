#include "bit.h"
using namespace std;
#define ll long long

// __m256i MOD = _mm256_set_epi64x(1LL<<41,0,1<<10,1);

// Assumes that there is no overflow involved
void addP(const __m256i &a, const __m256i &b, __m256i &res){

	int * ptr;
	__m256i result = _mm256_add_epi64(a,b);
	__m256i carry = _mm256_cmpeq_epi64(_mm256_max_epu32(a, result), result);
	carry = _mm256_add_epi64(carry,ones);	
	ptr = (int *)&carry;
	carry = _mm256_set_epi64x(ptr[2],ptr[1],ptr[0],0);	
	result = _mm256_add_epi64(result,carry);	
	carry = _mm256_cmpeq_epi64(_mm256_max_epu32(carry, result), result);
	carry = _mm256_add_epi64(carry,ones);	
	ptr = (int *)&carry;	
	carry = _mm256_set_epi64x(ptr[2],ptr[1],ptr[0],0);	
	result = _mm256_add_epi64(result,carry);	
}

void subP(const __m256i &a, const __m256i &b, __m256i &res){

}

void mulP(__m256i &a, const __m256i &b, __m256i *result)
{
	short int *ptr = (short int *) &a;
	__m256i lo[16],hi[16];
	for(int i = 0; i < 16; i++)
	{
		lo[i] = _mm256_mullo_epi16(a,b);
		hi[i] = _mm256_mulhi_epi16(a,b);
		a = _mm256_set_epi16(ptr[(i)%16],ptr[(i+15)%16],ptr[(i+14)%16],ptr[(i+13)%16],ptr[(i+12)%16],ptr[(i+11)%16],ptr[(i+10)%16],ptr[(i+9)%16],ptr[(i+8)%16],ptr[(i+7)%16],ptr[(i+6)%16],ptr[(i+5)%16],ptr[(i+4)%16],ptr[(i+3)%16],ptr[(i+2)%16],ptr[(i+1)%16]);
	}

	short int *loptr[16], *hiptr[16];

	for(int i = 0; i < 16; i++)
	{
		loptr[i] = (short int *) &lo[i];
		hiptr[i] = (short int *) &hi[i];
	}

    short int Mlo[31][16], Mhi[31][16], M[32][32];

	memset(M,0,sizeof(M));
	memset(Mlo,0,sizeof(Mlo));
	memset(Mhi,0,sizeof(Mhi));
	for(int x = 0; x < 31; x++)
	{
	    int h = 0,i = (x < 16)?(x)%16 : (30-x)%16;
		int j = (x < 16)? 0 : (15-i);
		//cout<<"i = "<<i<<"j = "<<j<<"\n";
		while(1)
		{
			Mlo[x][h]  = loptr[i][j];
			Mhi[x][h]  = hiptr[i][j];
			i = (i+2)%16;
			j++;
			h++;
			if( j > 15 || h > x) break;
		}
	}

	for(int i = 0; i < 31; i++)
	{
	    int h = 0;
		if(i - 1 >= 0) 
		{
			for(int j = 0; j < min(i,16); j++)
				M[i][h++] = Mhi[i-1][j];
		}
		if(i <= 30)
		{
			for(int j = 0; j <  min(i+1,16); j++)
				M[i][h++] = Mlo[i][j];
		}

	}

	__m256i carry[2], shift_carry[2];

	for(int h = 0; h <2; h++)
	{
		result[h] = _mm256_set_epi16(M[15+16*h][0],M[14+16*h][0],M[13+16*h][0],M[12+16*h][0],M[11+16*h][0],M[10+16*h][0],M[9+16*h][0],M[8+16*h][0],M[7+16*h][0],M[6+16*h][0],M[5+16*h][0],M[4+16*h][0],M[3+16*h][0],M[2+16*h][0],M[1+16*h][0],M[0+16*h][0]);
		carry[h] = _mm256_set_epi16(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
		__m256i temp,temp_carry;
		for(int i = 1; i < 32; i++)
		{
			temp  = _mm256_set_epi16(M[15+16*h][i],M[14+16*h][i],M[13+16*h][i],M[12+16*h][i],M[11+16*h][i],M[10+16*h][i],M[9+16*h][i],M[8+16*h][i],M[7+16*h][i],M[6+16*h][i],M[5+16*h][i],M[4+16*h][i],M[3+16*h][i],M[2+16*h][i],M[1+16*h][i],M[0+16*h][i]);
			result[h] = _mm256_add_epi16(temp,result[h]);
			short int *f = (short int*) &result[h];
			temp_carry = _mm256_cmpeq_epi16(_mm256_max_epi16(temp, result[h]), result[h]);
			temp_carry = _mm256_add_epi16(temp_carry,ONES);
			carry[h] = _mm256_add_epi16(carry[h],temp_carry);
			f = (short int*) &carry[h];
		}
	}

	short int *cptr1 = (short int*) &carry[0];
	short int *cptr2 = (short int*) &carry[1];

	shift_carry[0] = _mm256_set_epi16(cptr1[1],cptr1[2],cptr1[3],cptr1[4],cptr1[5],cptr1[6],cptr1[7],cptr1[8],cptr1[9],cptr1[10],cptr1[11],cptr1[12],cptr1[13],cptr1[14],cptr1[15],0);
	short int *f = (short int*) &result[0];
	f = (short int*) &shift_carry[0];
	addP(result[0],shift_carry[0],result[0]);

	shift_carry[1] = _mm256_set_epi16(cptr2[1],cptr2[2],cptr2[3],cptr2[4],cptr2[5],cptr2[6],cptr2[7],cptr2[8],cptr2[9],cptr2[10],cptr2[11],cptr2[12],cptr2[13],cptr2[14],cptr2[15],0);
	addP(result[1],shift_carry[1],result[1]);


}

void inverseP(const __m256i &a, __m256i &inv){

}

void moduloP(const __m256i *a, __m256i &res){

}



bit::bit() 
{
	this->a =  _mm256_set_epi64x(0,0,0,0);
}

bit::bit(long long int b) 
{
	this->a =  _mm256_set_epi64x(0,0,0,b);

}

bit::bit(int b) 
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
	__m256i res;
	addP(this->a, b.a, res);
	return bit(res);
}
bit & bit::operator+=(const bit &b)
{
	addP(this->a,b.a,this->a);
	return *this;
}

bit bit::operator-(const bit &b)
{
	__m256i res;
	subP(this->a, b.a, res);
	return bit(res);
}

bit & bit::operator-=(const bit &b)
{
	subP(this->a,b.a,this->a);
	return *this;
}

bit bit::operator*(const bit &b)
{
	__m256i result[2];
	__m256i x = b.a;
	mulP(this->a ,x,result);
	moduloP(result,x);
	return bit(x);
}

bit bit::operator/(const bit &b)
{
	bit temp;
	__m256i c = b.a, r;
	inverseP(c,r);
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

