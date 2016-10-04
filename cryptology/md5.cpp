#include <iostream>
#include <cmath> /* -lmath */
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

unsigned A = 0x67452301, /*Little-Endian x86*/
		 B = 0xefcdab89,
		 C = 0x98badcfe,
		 D = 0x10325476;

inline unsigned F(unsigned x, unsigned y, unsigned z) { return (x&y) | ((~x)&z); }
inline unsigned G(unsigned x, unsigned y, unsigned z) { return (x&z) | (y&(~z)); }
inline unsigned H(unsigned x, unsigned y, unsigned z) { return x^y^z; }
inline unsigned I(unsigned x, unsigned y, unsigned z) { return y ^ (x | (~z)); }

// inline int createTi(int i) { return ceil(4294967296*abs(sin(i))); }
unsigned Ti[65]; // 1-64
void setTi() {
	for (int i = 1; i < 65; ++i) {
		Ti[i] = floor(4294967296*abs(sin(i)));
	}
}


unsigned X[16]; 
unsigned* M;
unsigned long long ML = 0;

unsigned left_rotate(unsigned x, unsigned n)  { // fuck you !!!!!!!!!!!!!!!!!!
	return (x << n) | (x >> (32-n));
}

typedef unsigned (*pf)(unsigned,unsigned,unsigned);
inline void round1(unsigned& a, unsigned& b, unsigned& c, unsigned& d, pf p, int k, int s, int i) {
	a = b + left_rotate((a + p(b, c, d) + X[k] + Ti[i] ), s);
}

void readM(string filename) {
	ifstream fin(filename.c_str(), ios::binary);
	fin.unsetf(ios_base::skipws);
	fin.seekg( 0, fin.end );
	unsigned long long length = fin.tellg();  //overflow? so not int
	fin.seekg( 0, fin.beg );

	unsigned long long bu = 0;
	if ( length % 64 >= 56)  { // all / 8
		bu = 64 + (64 - (length % 64) );
	} else if(length % 64 == 0) {
		bu = 64;
	} else {
		bu = 64 - length % 64;
	}

	ML = (length + bu) >> 2;

	M = new unsigned[ML]();
	cout << length << '-' << bu <<  '-' << ML <<  endl;
	char it;
	unsigned long long i = 0, j = 0; /// fuck you !!!!!!!!!!!!!!!!
	for (i = 0, j = 0; fin.get(it); ++j) {
		//cout << (int)it << "-";
		if (j == 4) {
			j = 0;
			i++;
		}
		//if (j == 0) M[i] = 0;
		M[i] |= (it & 0xff) << (8 * j);
	}
//	if ((length % 64) != 56) {
		if (j == 4) {
			j = 0;
			i++;
		}
		//if (j == 0) M[i] = 0;
		M[i] |= 0x80 << (8 * j);
//	}
	

	unsigned long long len = length << 3;
	unsigned left = len >> 32, right = len;
	M[((length + bu) >> 2 )- 1] = left;
	M[((length + bu) >> 2 )- 2] = right;
	cout << left << "-" << right << endl;
}
void print(int out);

void md5() {
	unsigned int AA = A, BB = B, CC = C, DD = D;
	unsigned long long seg = ML /  16;
	for (int i = 0; i < seg; ++i) {
		for (int j = 0; j < 16; ++j) {
			X[j] = M[i*16+j];
			//print(X[j]);
			//cout << ":";
			//cout << i * 16 + j << endl;
		}
		//cout << endl;
		//1-----------------
		round1(A, B, C, D, F, 0, 7, 1);   round1(D, A, B, C, F, 1, 12, 2);   round1(C, D, A, B, F, 2, 17, 3);    round1(B, C, D, A, F, 3, 22, 4);
		round1(A, B, C, D, F, 4, 7, 5);   round1(D, A, B, C, F, 5, 12, 6);   round1(C, D, A, B, F, 6, 17, 7);    round1(B, C, D, A, F, 7, 22, 8);
		round1(A, B, C, D, F, 8, 7, 9);   round1(D, A, B, C, F, 9, 12, 10);  round1(C, D, A, B, F, 10, 17, 11);  round1(B, C, D, A, F, 11, 22, 12);
		round1(A, B, C, D, F, 12, 7, 13); round1(D, A, B, C, F, 13, 12, 14); round1(C, D, A, B, F, 14, 17, 15);  round1(B, C, D, A, F, 15, 22, 16);

		round1(A, B, C, D, G, 1, 5, 17);  round1(D, A, B, C, G, 6, 9, 18);   round1(C, D, A, B, G, 11, 14, 19);  round1(B, C, D, A, G, 0, 20, 20);
		round1(A, B, C, D, G, 5, 5, 21);  round1(D, A, B, C, G, 10, 9, 22);  round1(C, D, A, B, G, 15, 14, 23);  round1(B, C, D, A, G, 4, 20, 24);
		round1(A, B, C, D, G, 9, 5, 25);  round1(D, A, B, C, G, 14, 9, 26);  round1(C, D, A, B, G, 3, 14, 27);   round1(B, C, D, A, G, 8, 20, 28);
		round1(A, B, C, D, G, 13, 5, 29); round1(D, A, B, C, G, 2, 9, 30);   round1(C, D, A, B, G, 7, 14, 31);   round1(B, C, D, A, G, 12, 20, 32);

		round1(A, B, C, D, H, 5, 4, 33);  round1(D, A, B, C, H, 8, 11, 34);  round1(C, D, A, B, H, 11, 16, 35);  round1(B, C, D, A, H, 14, 23, 36);
		round1(A, B, C, D, H, 1, 4, 37);  round1(D, A, B, C, H, 4, 11, 38);  round1(C, D, A, B, H, 7, 16, 39);   round1(B, C, D, A, H, 10, 23, 40);
		round1(A, B, C, D, H, 13, 4, 41); round1(D, A, B, C, H, 0, 11, 42);  round1(C, D, A, B, H, 3, 16, 43);   round1(B, C, D, A, H, 6, 23, 44);
		round1(A, B, C, D, H, 9, 4, 45);  round1(D, A, B, C, H, 12, 11, 46); round1(C, D, A, B, H, 15, 16, 47);  round1(B, C, D, A, H, 2, 23, 48);

		round1(A, B, C, D, I, 0, 6, 49);  round1(D, A, B, C, I, 7, 10, 50);  round1(C, D, A, B, I, 14, 15, 51);  round1(B, C, D, A, I, 5, 21, 52);
		round1(A, B, C, D, I, 12, 6, 53); round1(D, A, B, C, I, 3, 10, 54);  round1(C, D, A, B, I, 10, 15, 55);  round1(B, C, D, A, I, 1, 21, 56);
		round1(A, B, C, D, I, 8, 6, 57);  round1(D, A, B, C, I, 15, 10, 58); round1(C, D, A, B, I, 6, 15, 59);   round1(B, C, D, A, I, 13, 21, 60);
		round1(A, B, C, D, I, 4, 6, 61);  round1(D, A, B, C, I, 11, 10, 62); round1(C, D, A, B, I, 2, 15, 63);   round1(B, C, D, A, I, 9, 21, 64);

		A += AA;
		B += BB;
		C += CC;
		D += DD;
		AA = A;
		BB = B;
		CC = C;
		DD = D;
	}
}



void print(int out) {
	int mask = 0xff;
	
	for (int i = 0; i < 4; ++i) {
		int temp = (out >> (i * 8)) & mask;
		cout <<  setfill('0') << setw(2) << std::hex << temp;
	}
}

int main() { //g++ -std=c++0x md5.cpp
	readM(string("learnyouahaskell-zh-zh-cn.pdf"));
	setTi();
	md5();
	// for (int i = 0; i < 144; ++i)
	// 	cout << M[i] << endl;
	// cout << A << " | "<< B << " | "<< C << " | "<< D << endl;
	print(A);
	print(B);
	print(C);
	print(D);
}