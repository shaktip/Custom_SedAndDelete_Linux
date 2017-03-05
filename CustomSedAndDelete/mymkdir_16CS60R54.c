#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
/**
     mymkdir is to create a new dir or multiple directories according to the list of arguments.
*/
void main(int n, char *dirname[])
{
   int check;
   int count;
   char mode[] = "0777";
   int i;
   char *now_working_dir;
   now_working_dir = (char *) malloc (1000 * sizeof(char));  
   now_working_dir = getenv("PWD");
   //printf("now_working_dir is %s " , now_working_dir);
   //printf("\n\t n = %d",n);
   //for(count = 0 ; count < n ; count++)
   //   printf(" %s \n ", dirname[count]);
   

  if(n < 2)
  {
      printf("Invalid number of arguments(Supply the name of dirs)\n");
      return ;           
  }
  for(count = 1; count < n; count++)
  {
     char ptr[200];
     if(dirname[count][0] == '/')
        strcpy(ptr, dirname[count]);
     else
     {
        strcpy(ptr, now_working_dir);
        int j = 0;
        for(j = 0; ptr[j] != '\0' ; j++);
        if(ptr[j-1] != '/') 
             strcat(ptr, "/");
        strcat(ptr, dirname[count]);
     }
     //printf("dirname is %s and ptr is %s\n ", dirname[count],ptr);
     check = mkdir(ptr);  
     if (!check)
     {
        i = strtol(mode, 0, 8);
        chmod(ptr,i);  
       // printf("Directory created\n");
     }
     else
     {
        printf("Unable to create %s directory\n", dirname[count]);
        
     }
   }
   return;
  
  
 
}
