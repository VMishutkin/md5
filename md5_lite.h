


#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fstream>
#include <iostream>                 
#include <sstream>   

using namespace std;
typedef unsigned char byte;
typedef unsigned long dword;
typedef uint64_t dword64; 
typedef unsigned int  uint;



//! внутреняя часть md5 (обработка данных блоками, без буферизации)
class MD5_block{
	void swap(dword&,dword&,dword&,dword&);	//!< обмен переменных по кругу
	dword fun(dword i,dword,dword,dword);
	uint lrot( uint x, int bits );
protected:
	MD5_block();
	void update_block(dword[16]);			//!< обработка блока данных
	dword abcd[4];
};


//! md5 (обертка для нормальной работы, подаёт блоки на обработку, делает завершающие операции)
class MD5:public MD5_block
{

	uint size;			//!< заполнение буфера
	uint total_size;	//!< суммарный размер всех данных
	byte buffer[64];	//!< сам буфер
	void add(byte);		//!< фукнция добавление данных в буфер (обновляет хеш при переполнении)
	void hexPrinter( unsigned char* c, int l );
public:
	MD5():size(0),total_size(0) {}
	void update(const char* str,uint size=0);
	void final(byte[16]);
	static void install();	//!< заполняет константы (вызывает один раз, перед использованием первого объекта)
	char* ifopen(char* ifile);
	void hash(const char* mas);
};


