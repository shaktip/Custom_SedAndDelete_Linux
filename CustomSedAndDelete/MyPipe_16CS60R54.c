#include <sys/types.h>
#include <sys/wait.h>
#include<stdio.h>
#include<ctype.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include<stdlib.h>
#include<malloc.h>
#include<stdlib.h>
#include<stdbool.h>
#include<fcntl.h>

void mytrim(char *t , char *d)
{
   while(t[0] != '\0')
   {
       if(t[0] == ' ')
            t++;
       else 
           break;
   }
   int i = 0 ;
   for(; t[i] != '\0' ; i++) ;
   i--;
   while(t[i] == ' ')
      t[i--] = '\0';
   i = 0;
   while(t[i] != '\0')
   {
      d[i] = t[i];
      i++;
   } 
   d[i] = '\0';
}

void CallPipeProcedure(char *command)
{
 //  printf("In Process %s\n", command);
   pid_t pid;   
   //str_split(str,',');
   const char s[] = "|";
   char pq[1000];
   
   char *token;
   char **prcs;
   prcs = (char **) malloc(20 * sizeof(char *));
   int index = 0;
   // get the first token 
   token = strtok(command, s);
   
   
   // walk through other tokens 
   while( token != NULL ) 
   {
      prcs[index] = (char *) malloc((strlen(token) + 1) * sizeof(char));
      strcpy(prcs[index] , token);
      mytrim(prcs[index] , prcs[index]);
      //printf( "Process  %s\n", prcs[index] );    
      token = strtok(NULL, s);
      
      index++;
   }
   FILE *fp;
   fp = fopen("filein" , "w");
   fclose(fp);
  // printf("index is %d\n", index);
   
   int i;
  // for(i = 0 ; i< index ;i++)
   //{
   //    printf(" %s " , prcs[i]);
   //}

   for( i=0; i<index-1; i++)
    {
        int pd[2];
        pipe(pd);
        char *tk;
        char **cmd;
        cmd = (char **) malloc(20 * sizeof(char *));
        int cmd_index = 0;
        tk = strtok(prcs[i], " ");
        while( tk != NULL ) 
        {
           cmd[cmd_index] = (char *) malloc((strlen(tk) + 1) * sizeof(char));
           strcpy(cmd[cmd_index] , tk);
           mytrim(cmd[cmd_index] , cmd[cmd_index]);
          // printf( "Command  %s\n", cmd[cmd_index] );    
           tk = strtok(NULL, " ");
           cmd_index++;
        }
        if((pd[1]=open("fileout",O_CREAT|O_TRUNC|O_WRONLY,0644))<0)  //from net for linking the write pointer of pipe to fileout 
	   {
 		exit(1);
     	}
		if((pd[0]=open("filein",O_RDONLY))<0)			// from net for linking the read pointer of the pipe to filein
		{
			exit(1);
		}
        pid=fork();
        if (pid == 0)
         {
            close(pd[0]);
            close(1);
            dup2(pd[1], 1); // remap output back to parent
            execvp(cmd[0], cmd);
            _exit(0);            
        }

        // remap output from previous child to input
        else
        {
           int status;
           waitpid(pid,&status,WIFEXITED(&status));
            sprintf(pq,"cat fileout>filein");
		  system(pq);
           close(pd[1]);
           close(0);
          
           dup2(pd[0], 0);

           
        }   
    }
    char *tk;
    char **cmd;
    cmd = (char **) malloc(20 * sizeof(char *));
    int cmd_index = 0;
    tk = strtok(prcs[i], " ");
    while( tk != NULL ) 
    {
           cmd[cmd_index] = (char *) malloc((strlen(tk) + 1) * sizeof(char));
           strcpy(cmd[cmd_index] , tk);
           mytrim(cmd[cmd_index] , cmd[cmd_index]);
          // printf( "Command  %s\n", cmd[cmd_index] );    
           tk = strtok(NULL, " ");
           cmd_index++;
    }
    pid=fork();
    if(pid == 0)
    {
       execvp(cmd[0] , cmd);
       _exit(0);
    }
    else
    {
      waitpid(pid, 0 ,0);
      remove("filein");
      remove("fileout");
      printf("\n\tTest");
    } 
    return ;
  
}

void main(int argc , char *argv[])
{
  
   CallPipeProcedure(argv[1]);
   
   return;   
}




