#include <stdio.h>
#include <stdlib.h>

extern int readelf(u_char* binary, int size);
/*
        overview: input a elf format file name from control line, call the readelf function
                  to parse it.
        params:
                argc: the number of parameters
                argv: array of parameters, argv[1] shuold be the file name.

*/
int main(int argc,char *argv[])
{
        FILE* fp;
        int fsize;
        unsigned char *p;


        if(argc < 2)
        {
                printf("Please input the filename.\n");
                return 0;
        }						//print error if didn't type in the file
        if((fp = fopen(argv[1],"rb"))==NULL)
        {
                printf("File not found\n");
                return 0;
        }						//print error if can't find file
        fseek(fp,0L,SEEK_END);				//point to filr end
        fsize = ftell(fp);				//get file size (= offset of file end)
        p = (u_char *)malloc(fsize+1);			//get memory place 
        if(p == NULL)
        {
                fclose(fp);
                return 0;
        }						//end if file is empty
        fseek(fp,0L,SEEK_SET);				//point to file begin
        fread(p,fsize,1,fp);				//read in all file with the begin point of p
        p[fsize] = 0;					//define file end


	readelf(p,fsize);
        return 0;
}

