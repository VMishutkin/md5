#include "md5_lite.h"

// таблицы

namespace _MD5{
//! таблица смещений
const byte off[64]={
 0, 1, 2, 3,
 4, 5, 6, 7,
 8, 9,10,11,
12,13,14,15,
 1, 6,11, 0,
 5,10,15, 4,
 9,14, 3, 8,
13, 2, 7,12,
 5, 8,11,14,
 1, 4, 7,10,
13, 0, 3, 6,
 9,12,15, 2,
 0, 7,14, 5,
12, 3,10, 1,
 8,15, 6,13,
 4,11, 2, 9
};

//! таблица сдвигов (можно считать на лету, так же как md5::fun)
const byte shift[64]={
 7,12,17,22,
 7,12,17,22,
 7,12,17,22,
 7,12,17,22,
 5, 9,14,20,
 5, 9,14,20,
 5, 9,14,20,
 5, 9,14,20,
 4,11,16,23,
 4,11,16,23,
 4,11,16,23,
 4,11,16,23,
 6,10,15,21,
 6,10,15,21,
 6,10,15,21,
 6,10,15,21
};

//! таблица констант

dword T[64];

 // const
/******
	T=	0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
		0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821, 
		0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8, 
		0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a, 
		0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70, 
		0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665, 
		0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1, 
		0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 
	check sum - 0x27f3a0bc
*/
}
void MD5::install()
{
	dword sum=0;
	// заполняем таблицу констант (можно забить её заранее)
	for(uint n=0; n<64; n++) _MD5::T[n]=(dword)ldexp(fabs(sin(n+1)),32), sum+=_MD5::T[n];
	// проверим правильность заполнения
	assert(sum==0x27f3a0bc);
}


MD5_block::MD5_block()
{
	
	abcd[0]=0x67452301;
	abcd[1]=0xefcdab89;
	abcd[2]=0x98badcfe;
	abcd[3]=0x10325476;
}


dword MD5_block::fun(dword n,dword x,dword y,dword z)
{
	switch(n/16) {
	case 0: return (y^z)&x^z; // =x&y|~x&z;
	case 1: return (x^y)&z^y; // =x&z|y&~z;
	case 2: return x^y^z;
	case 3: return y^(x|~z);
	}
	return -1;
}

uint MD5_block::lrot( uint x, int bits )
{
	return (x<<bits) | (x>>(32 - bits));
};
void MD5_block::swap(dword &a,dword &b,dword &c,dword &d)
{
	dword t=d;
	d=c,c=b,b=a,a=t;
}


void MD5_block::update_block(dword data[16])
{
	dword a=abcd[0];
	dword b=abcd[1];
	dword c=abcd[2];
	dword d=abcd[3];
	for(uint n=0; n<64; n++) {
		dword v=data[_MD5::off[n]];
		a=b + lrot(a+v+_MD5::T[n]+fun(n,b,c,d),_MD5::shift[n]);
		swap(a,b,c,d);
	}
	abcd[0]+=a;
	abcd[1]+=b;
	abcd[2]+=c;
	abcd[3]+=d;
}
void MD5::hexPrinter( unsigned char* c, int l )
{
	assert( c );
	assert( l > 0 );
	while( l > 0 )
	{
		printf( " %02x", *c );
		l--;
		c++;
	}
	printf("\n");
}

void MD5::add(byte x)
{
	total_size++;
	buffer[size++]=x;
	if(size>=64) update_block((dword*)buffer),size=0;
}


void MD5::update(const char *str,uint size)
{
	if(!size) size=strlen(str);
	while(size--) add(*str++);
}


void MD5::final(byte res[16])
{
	dword64 total=dword64(total_size)*8;
	// дополняем буфер нулями
	add(0x80); while(size!=56) add(0); 
	// дополняем буфер полным размером данных в битах
	update((char*)&total,8);
	// последний блок дополнен и вытоклнут на обработку
	// отдаём результат
	
	memcpy(res,abcd,sizeof(abcd));
	hexPrinter(res,16);
}

char* MD5::ifopen(char* ifile)
{
ifstream in(ifile, ios::binary);                                                     

    if (in.fail())                                                                         
        return false;                                                                      
    
    in.seekg(0, ios::end);                                                                  
    unsigned int file_size = (unsigned int)in.tellg();                                     
    in.seekg(0, ios::beg);                                                                 

    char *mas = new char[file_size]; 
    
    in.read(mas, file_size);                                                                                                      
    in.close();                                                                            
    
 
    return mas;
}

void MD5::hash(const char* mas)
{
byte res[16];
update(mas);
final(res);

}



