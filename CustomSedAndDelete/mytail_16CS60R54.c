#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include<stdlib.h>
#define BUFF_SIZE 4096

FILE *openFile(const char *filePath)
{
  FILE *file;
  file= fopen(filePath, "r");
  if(file == NULL)
  {
    fprintf(stderr,"Error opening file: %s\n",filePath);
    exit(errno);
  }
  return(file);
}

void printLine(FILE *file, off_t startline)
{
  int fd;
  fd= fileno(file);
  int nread;
  char buffer[BUFF_SIZE];
  lseek(fd,(startline + 1),SEEK_SET);
  while((nread= read(fd,buffer,BUFF_SIZE)) > 0)
  {
    write(STDOUT_FILENO, buffer, nread);
  }
}

void walkFile(FILE *file, long nlines)
{
  off_t fposition;
  fseek(file,0,SEEK_END);
  fposition= ftell(file);
  off_t index= fposition;
  off_t end= fposition;
  long countlines= 0;
  char cbyte;

  for(index; index >= 0; index --)
  {
    cbyte= fgetc(file);
    if (cbyte == '\n' && (end - index) > 1)
    {
      countlines ++;
      if(countlines == nlines)
      {
            break;
      }
     }
    fposition--;
    fseek(file,fposition,SEEK_SET);
  }
  printLine(file, fposition);
  fclose(file);
}

int main(int argc, char *argv[])
{
   
  FILE *file;
  
  if(argc > 3 || argc <= 1)
  {
     printf("Incorrect Arguments\n");
     return;
  } 
  char temp[1000];
  char argv0[1000], argv1[1000], argv2[1000];
  long no;
  char *now_working_dir;
  now_working_dir = (char *) malloc (1024 * sizeof(char));
  now_working_dir = getenv("PWD"); 
  if(argc == 2)
  {
      strcpy(argv0, now_working_dir);
      no = -10;
      strcpy(argv2 , argv[1]);
  } 
  else
  {
      strcpy(argv0, now_working_dir);
      no = atol(argv[1]);
      strcpy(argv2 , argv[2]);
  }
  if(no >= 0)
  {
     printf("mytail: cannot open '0' for reading: No such file or directory\n");
     no = -10;
  }
  if(argv2[0] == '/') 
      file= openFile(argv[2]);
  else
  {
      strcpy(temp , argv0);
      strcat(temp, "/");
      strcat(temp , argv2);
      file = openFile(temp);
  }
  if(file == NULL)
  {
         printf("Invalid file\n");
         return;
  }
  walkFile(file, -no);
  return 0;
}
