#ifndef BIT_H
#define BIT_H
#include <immintrin.h>
#include <iostream>
#include <bits/stdc++.h>
#include <stdint.h>


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