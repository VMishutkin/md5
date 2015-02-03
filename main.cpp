#include "md5_lite.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << endl << "Использование: ./md5 file_in" << endl << endl;
    
    return -1;
    }                                                          
    
    
    MD5 md5;
if(md5.ifopen(argv[1]))
{
printf("Hash for %s\n",md5.mes);
    
    MD5::install();
    
    md5.hash();
}
else
 printf("не удалось считать файл \"%s\"\n",argv[1]);
    
}