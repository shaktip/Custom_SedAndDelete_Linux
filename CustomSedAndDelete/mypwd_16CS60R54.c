#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include<malloc.h>
#include<stdlib.h>
int main(int c , char *argv[]) 
{
  /*
   char cwd[1024];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
       fprintf(stdout, "Current working dir: %s\n", cwd);
   else
       perror("getcwd() error");
   */
   //printf("Inside pwd\n");
   
   char *now_working_dir;
   now_working_dir = (char *) malloc (1024 * sizeof(char));  
   //getcwd(now_working_dir , 1000);
   
   
   //getcwd(now_working_dir , 1024);
   now_working_dir = getenv("PWD");
   if(now_working_dir == NULL)
       printf("  show \n"); 
   printf("%s\n" , now_working_dir);
   
   return 0;
}
