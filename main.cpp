#include "md5_lite.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << endl << "Использование: ./md5 file_in" << endl << endl;
    
    return -1;
    }                                                          
    
    char *mas= MD5().ifopen(argv[1]);
    printf("Hash for %s\n",mas);
    MD5::install();
    MD5 md5;
    md5.hash(mas);

    
}