#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include<stdlib.h>
#include<dirent.h>

void removeDirectory(char *ptr)
{
     struct stat statbuf;
     struct dirent *ent;
     DIR* directory = opendir(ptr);
     FILE *fp;
     if (directory)
     {
                   
       while((ent = readdir(directory)) != NULL)
       {
        //printf("Check");
         if(!strcmp(ent->d_name , "."))
               continue;
         if(!strcmp(ent->d_name , ".."))
              continue;
         else
         {
           char filename[100];
           strcpy(filename, ptr);
           strcat(filename , "/"); 
           strcat(filename, ent->d_name);
           //printf("\n%s",ent->d_name);
           removeDirectory(filename);

           if((fp = fopen(filename, "r")) != NULL)
           {
               fclose(fp);
               //printf("%s file ", filename);
              // printf("status of remove %d \n" , remove(filename));
           }
                         
           
         }  
      }
      closedir(directory);
      remove(ptr);
      //printf("%s is removed\n",ptr);
   }
   //else
     //printf("Error in reading %s directory",ptr);
}

int main(int c, char *argv[])
{
    struct stat statbuf;
    struct dirent *ent;
    int i;
    int flag;
    char *now_working_dir;
    now_working_dir = (char *) malloc(1024 * sizeof(char));
    now_working_dir = getenv("PWD");
    if(c < 2)
    {
         printf("Invalid No of arguments ");
         return;
    }
    if(strcmp(argv[1],"-d") == 0)
    {  /* remove empty directory */
         for(i = 2; i<c; i++)
         {
            char ptr[1024];
	    if(argv[i][0] != '/')
	    {
	       strcpy(ptr, now_working_dir);
	       strcat(ptr, "/");
	       strcat(ptr, argv[i]);
	    }
	    else
	       strcpy(ptr , argv[i]);

            //printf("%s %s",ptr, argv[i]);
            DIR* directory = opendir(ptr);
            flag = 1;
            if (directory)
            {
                  /* Directory exists. */
                  //printf("Directory exists\n");
                  while((ent = readdir(directory)) != NULL)
                  {
                          //printf("Check");
                         if(!strcmp(ent->d_name , "."))
                              continue;
                         if(!strcmp(ent->d_name , ".."))
                             continue;
                         else
                         {
                               flag = 0;
                               break;
                         }  
                  }
                  if(flag == 1)
                  {
                      closedir(directory);
                      remove(ptr);
                      //printf("%s is removed\n",ptr);
                  }
                  else
                      printf("%s Directory is not empty\n",ptr);
            }
            else
                printf("%s is not valid directory\n" , ptr);
          }
            return;
    }
    else if(strcmp(argv[1] , "-r") == 0)
    {  /* remove a directory and its contents */
         for(i = 2; i<c; i++)
         {
            char ptr[1024];
	    if(argv[i][0] != '/')
	    {
	       strcpy(ptr, now_working_dir);
	       strcat(ptr, "/");
	       strcat(ptr, argv[i]);
	    }
	    else
	       strcpy(ptr , argv[i]);

            //printf("%s %s",ptr, argv[i]);
             /* Directory exists. */
            removeDirectory(ptr);
            
          }
         
	return;
    } 
    for(i = 1 ;i < c; i++)
    {
       //printf(" check\n");
       /* remove files  */
       char filename[100];
       if(argv[i][0] == '/')
          strcpy(filename , argv[i]);
       else 
       {
            strcpy(filename , now_working_dir);
            strcat(filename , "/");
            strcat(filename, argv[i]);
       }  
       //printf("File name %s", filename);
       FILE *fb = fopen(filename,"r");
       if(fb==NULL)
           printf("its null\n");
       else
       {
           if(remove(filename) == 0)
             printf("file deleted %s\n",filename);
           else
             printf("Could not delete  %s is not a file \n", filename);
       } 
    
 /*      stat("/home/jeegar/", &statbuf);

       if(S_ISDIR(statbuf.st_mode))
           printf("directory\n");
       else
         printf("file\n");
*/
    }
    return 0;
}
