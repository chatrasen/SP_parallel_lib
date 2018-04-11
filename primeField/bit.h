#ifndef BIT_H
#define BIT_H
#include <immintrin.h>
#include <iostream>
#include <bits/stdc++.h>
#include <stdint.h>


__m256i ones = _mm256_set_epi64x(1,1,1,1);
__m256i val128 = _mm256_set_epi64x(128,128,128,128);
__m256i mas = _mm256_set_epi64x(-1,-1,-1,-1);
__m256i ONES = _mm256_set_epi16(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);

class bit {
public:
	__m256i  a;
	bit();
	bit(long long int b);
	bit(int b);
	bit(uint64_t *b);
	bit(const __m256i &b);
	bit operator+(const bit &b);
	bit & operator+=(const bit &b);
	bit operator-(const bit &b);
	bit & operator-=(const bit &b);
	bit operator*(const bit &b);
	bit operator/(const bit &b);
	bit & operator=(int x);
	bool operator!();
	void print();
	static void* operator new[](size_t sz);
  	static void operator delete[](void* m);
	friend std::ostream &operator<<(std::ostream &output, const bit &b );	
};

#endif