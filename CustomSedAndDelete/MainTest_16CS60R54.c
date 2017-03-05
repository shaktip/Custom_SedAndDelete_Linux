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

#include<dirent.h>


#include  <sys/ipc.h>
#include  <sys/shm.h>


#define BUFFERSIZE 1000
#define DIRSTRINGSIZE 1024

void mycd(char **argv)
{
    //printf("Inside child %s  %s", now_working_dir,argv[1]);
    int i;
    //int shmid;
   // key_t key = 5678;
     char *shm, *s;
     int j;
     char *name , *token , *newDir , **all_tokens;
 
//     printf(" argv[0] is %s argv[1] is %s \n " , argv[0] , argv[1]);

    all_tokens = (char **) malloc(strlen(argv[1]) * sizeof(char *));
    name = (char *) malloc(1024 * sizeof(char));
    newDir = (char *) malloc(1024 * sizeof(char));
    shm = (char *) malloc(1024 * sizeof(char));
    newDir[0] = '\0';
    char *temp;
    //getcwd(name , 1024);
    name = getenv("PWD");
   // printf("pwd is : %s\n " , name);
        //printf("Check1 \n");
    if(strcmp(argv[1] , "~") == 0)
    {
       shm = getenv("HOME");
       setenv("PWD", shm,1);
       return;
    }
     
      strcpy(shm , name);
      char ptr[1024];
      if(argv[1][0] != '/')
      {
       strcpy(ptr, name);
       strcat(ptr, "/");
       strcat(ptr, argv[1]);
      }
      else
       strcpy(ptr , argv[1]);

    //printf("%s %s",ptr, argv[1]);
      DIR* directory = opendir(ptr);
    
      if (directory)
      {
        /* Directory exists. */
        //   printf("Directory exists\n");
        //fflush(stdout);     
         closedir(directory);
         if(strcmp(argv[1], "..") == 0)
         {
             //printf("Check2sdf\n");
              token = strtok (name,"/");     
              i = 0;
              while(token != NULL)
              {
                   all_tokens[i] = (char *) malloc(1024 * sizeof(char));
                   all_tokens[i] = token;
                   //token = strtok(NULL, " ");
                   //printf("%d Token : %s " , i, token);
                   i++;
                   token = strtok(NULL, "/");
              }
              for(j = 0; j<i-1; j++)
              {
                   strcat(newDir , "/");
                   strcat(newDir , all_tokens[j]);
              } 
              if(j == 0)
                strcpy(newDir , "/");       
              strcpy(shm, newDir);

         }
         else if(strcmp(argv[1] , ".") == 0)
              return; 
         else
         {   
           if( argv[1][0]  != '/')
           {  
             
             for(j= 0; shm[j] != '\0'; j++);
             if(shm[j-1] != '/')
                 strcat(shm, "/");
             strcat(shm, argv[1]);
           }
           else
             strcpy(shm, argv[1]);
         }
        // printf(" Inside child process :  %s\n",shm);
        
        //getcwd(now_working_dir , 256);
        
        char *get_path;
        get_path = (char *) malloc( 1024 * sizeof(char));
//        printf("In mycd current dire in shm is %s \n",shm);
        //chdir(shm);

        setenv("PWD", shm,1);
        get_path = getenv("PWD");
        
        if(get_path == NULL)
            printf("Error\n");
        else
        {
           
  //         printf("PWD set by cd is %s\n" , get_path);
        }   
/*
        char *pth = (char *) malloc (1024 * sizeof(char));

        strcpy(pth , shm);  
        strcat(pth,":");
        strcat(pth, argv[0]);
        setenv("PATH",pth,1);
        printf("Path set by cd is %s \n", pth);
  */
                    
    }
    else
    {
       /* Directory does not exist. */
    //   printf("Directory does not exist or no permissions to read\n");
       return;
    }
    return;
    
    
}

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


int startsWith(char *pre, char *str)
{
   // printf("Function called");
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);   //search pre in str
    return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}
struct dll
{
   char *cmd;
   struct dll *next, *prev;
};

int main(void)
{
    struct termios oldattr, newattr;
    char command[1024];
    strcpy(command, "MyUser$ ");
    int index = 8;
    struct dll *head = NULL;

    struct dll *curr = NULL, *list = NULL;
    struct dll *updown = NULL;
    char *cmds;
    
    char application_path[256];
    char *get_path;
    get_path = (char *) malloc (1000 * sizeof(char));

    getcwd(application_path , 256);
    setenv("PWD", application_path,1);

    get_path = getenv("PATH");
    if(get_path == NULL)
       printf("Error\n");
    strcat(get_path,":");
    strcat(get_path, application_path);
    setenv("PATH",get_path,1);
    getcwd(application_path , 256);
    
    char app_path[256];
    strcpy(app_path, application_path);
    setenv("APPPATH", app_path , 1);   

    //printf(" current path is  :%s\n",application_path);

    pid_t pid, parent_pid ; 
    char argv[BUFFERSIZE];
    char **argumentsToPass;
    char currentWorkingDir[DIRSTRINGSIZE];
    char *token;
    //key_t key = 5678;
   
   
    // int    ShmID;
    // char   *ShmPTR_CWD; 
    
    // ShmID = shmget(key, DIRSTRINGSIZE*sizeof(char), IPC_CREAT | 0777);
    // if (ShmID < 0) {
    //      printf("*** shmget error (server) ***\n");
    //      exit(1);
    // }
    // ShmPTR_CWD = (char *) shmat(ShmID, NULL, 0);

    parent_pid = getpid();
    //currentWorkingDir = (char *) malloc(1024 * sizeof(char));
    //if (getcwd(currentWorkingDir, DIRSTRINGSIZE) == NULL)
   //     perror("getcwd() error");
    //for(i = 0 ; i < strlen(currentWorkingDir); i++)
    //    ShmPTR_CWD[i] = currentWorkingDir[i];
    //ShmPTR_CWD[i] = '\0';
    
    int i;
    char ch;
    int CheckUpDown=0;
    //system("tput clear");
    int count = 0;
    int flag = 0;
    
    while(1)
    {
         
       printf("\33[2K\r");
       printf("%s",command);
   
       tcgetattr( STDIN_FILENO, &oldattr );
       newattr = oldattr;
       newattr.c_lflag &= ~( ICANON | ECHO );
       tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
       ch = getchar();
       if(flag == 1)
        count++;
       tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
       
       //printf("%c",ch);
       
         //system("tput clear");
          // printf("%c[2K", 27);
        
        if(ch == 127)    
        {
          if(index >= 9)
               index--;
          command[index] = '\0';
          printf("\33[2K\r");
          printf("%s",command);
        }
        else if(ch == 10)   //Enter key
        {
             //printf("Test");
             
              printf("\n");
              if(strlen(command+8) == 0)
                 continue;
              char *cmd_to_run ;
              cmd_to_run = (char *) malloc ((strlen(command+8)+1) * sizeof(char));
              strcpy(cmd_to_run , command+8);
              mytrim(cmd_to_run , cmd_to_run);
 
              if(strcmp(cmd_to_run , "exit") == 0)
              {
                    list = head;
                    while(list)
                    {
                       free(list->cmd);
                       curr = list;
                       list = list->next;
                       free(curr);
                    }
                    printf("\n");
                    return 0;
              }

             //printf(" length %d\n",(int)strlen(command));
              bool testCmd = true;
              if(list)
                 if(strcmp(command, list->cmd) == 0)
                     testCmd = false;
                //printf("Test\n");
                if(testCmd)
                {
                   //printf("\nCommand is  : %s \n",command);
                   curr = (struct dll *) malloc(sizeof(struct dll));
                   curr->cmd = (char *) malloc((strlen(command) + 1) * sizeof(char));
                   strcpy(curr->cmd , command);
                   curr->next = NULL;
                   if(head == NULL)
                   {
                     curr->prev = NULL;
                     head = curr;
                   }
                   else
                   { 
                      curr->prev = list;
                      list->next = curr;
                   }   
                   list = curr;                
                }
                updown = list;
                CheckUpDown = 0;
                index = 8;
                command[index] = '\0';
                          
               if(strchr(cmd_to_run, '|'))
               { 
                   //popen();  //call pipe code
                   char **v;
                   v = (char **) malloc(2 * sizeof(char *));
                   v[0] = (char *) malloc(50 * sizeof(char));
                   v[1] = (char *) malloc(50 * sizeof(char));
                   strcpy(v[0] , "mypipe");
                   strcpy(v[1] , cmd_to_run);
                   
                  /* const char s[] = "|";
   
                    char *token;
                    char **prcs;
                    prcs = (char **) malloc(20 * sizeof(char *));
                    int index = 0;
   // get the first token 
                    token = strtok(cmd_to_run, s);
   
   
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
                  */
                   pid = fork();
                   if(pid == 0)
                   {
                        execvp("mypipe", v);
                        _exit(1);
                   }
                   else
                   {
                      waitpid(pid, 0, 0);
                   }     
                   if(getpid() == parent_pid)                   
                    continue;
                   
                   else
                      return 0;
               }
                
                
      //printf("cmd_to_run : %s \n" , cmd_to_run);
      argumentsToPass = (char **) malloc((strlen(cmd_to_run) / 2) * sizeof(char *));

      int j;
      token = strtok (cmd_to_run," ");
      char *appToRun;
      appToRun = (char *) malloc ((strlen(token) + 1) * sizeof(char));
      strcpy(appToRun , token);
      
      //for(j=0; j<strlen(appToRun) ; j++)
       //   printf(" %d %c\n" , j, appToRun[j]);
      
        argumentsToPass[0] = (char *) malloc((strlen(appToRun) + 1) * sizeof(char));
        strcpy(argumentsToPass[0], appToRun);
        //printf("appToRun : %s \n", appToRun);
        token = strtok(NULL, " ");
        i = 1; 
        while(token != NULL)
        {
          argumentsToPass[i] = (char *) malloc((strlen(token) + 1) * sizeof(char));
          strcpy(argumentsToPass[i++],token);
          token = strtok(NULL, " ");
        }
        //for(j = 0 ; j<i ; j++)
        //    printf("argumentToPass[%d] is %s \n" , j , argumentsToPass[j]);
        if(strcmp(appToRun , "mycd") == 0 || strcmp(appToRun , "cd") == 0)
        {
           
           mycd(argumentsToPass);
           
           continue;
        }
 
       int len = strlen(cmd_to_run);

        if(cmd_to_run[len - 1] == '&')
        {
             
           // printf("argumentToPass[%d] is %s \n" , j , argumentsToPass[j]);
           // len = strlen(argumentsToPass[j]);
            if(argumentsToPass[0][len-1] == '&')
               argumentsToPass[0][len-1] = '\0';
            pid = fork();
            if(pid == 0)
            {
                 
                // printf("argumentsToPass[0] %s " , argumentsToPass[0]);
                 setpgid(0,0);
		 if(argumentsToPass[1] != NULL)
		     argumentsToPass[1] = NULL;             
                 execvp(argumentsToPass[0], argumentsToPass);
                _exit(1);                           
            }
            if(getpid() == parent_pid)
            {
              //  setpgid(0,0);
               continue;
            }
            else
               return 0;
               
        }    
        else

        {     
           pid=fork();
           if(pid==0) 
           { 
          // child process 
            if(strcmp(appToRun,"mypwd") == 0 || strcmp(appToRun, "mymkdir") == 0 || strcmp(appToRun, "myls") == 0 || strcmp(appToRun, "mymv") == 0 || strcmp(appToRun , "myps") == 0 || strcmp(appToRun,"mytail") == 0 || strcmp(appToRun, "myrm") == 0 || strcmp(appToRun , "mygrep") == 0 || strcmp(appToRun , "mysed") == 0)
            {
               //printf("Mysed\n");   
               if(strcmp(appToRun , "mysed") == 0)
               {
                 if(strstr(argumentsToPass[1] , "/"))
                 {
                     //printf("SedDelete\n");
                     execv("SedDelete", argumentsToPass);
                     _exit(1);
                 }
                 else if(strstr(argumentsToPass[1] , "["))
                 {
                    execv("LinesDelete", argumentsToPass);
                    _exit(1);
                 }
                 else
                 {
                    printf("Syntax Error\n");
                    exit(1);
                 }
               }  
               else
               {
                 execv(appToRun,argumentsToPass);
                 _exit(1);
               }  
            }
            else
            {
                 char *apppath;
                 char *now_working_dir;
                 now_working_dir = (char *) malloc (1000 * sizeof(char));  
                 now_working_dir = getenv("PWD");
   		 //apppath = (char *) malloc(256 * sizeof(char));
                 //apppath = getenv("APPPATH");
                 chdir(now_working_dir);
                 char *prgname = (char *) malloc(2000 * sizeof(char));
               //  strcpy(prgname, "/bin");
                 //int prg_length = strlen(prgname);
                 //if(prgname[prg_length - 1] != '/')
                 //     strcat(prgname , "/");
                 //strcat(prgname , appToRun);
                 argumentsToPass[i++] = NULL;
                 //printf("Program name : %s \n",prgname);
                 execvp(appToRun , argumentsToPass);
                 /*if(startsWith("cd", cmd_to_run) != 0)
                 {
                       char *newd = (char *) malloc(1000 * sizeof(char));
                       getcwd(newd , 1000);
                       printf("New path : %s \n" , newd);
                       setenv("PWD", newd,1);
                 } */   
                 //execv(appToRun, argumentsToPass);
                 chdir(apppath);
                 _exit(1); 
            } 
         }
         else
         { 
           // pid!=0; parent process
            waitpid(pid, 0, 0);
            //continue;
            for(j = 0 ; j < i ; j++)
               free(argumentsToPass[j]);
            free(argumentsToPass);
            free(appToRun);
            //printf("Parent is running \n"); 
          }
          //printf("Test \n");
          
          if(getpid() == parent_pid)
               continue;
          else
               return 0;
         }           
        }  //Enter key code closed
        else if(ch == 27 || ch == 28)
        {   
              flag = 1;
        }

       else if(flag == 1 && ch == 65)
        {  
           if(updown)
           {   
                     //strcpy(command , "MyUser$ ");
                if(CheckUpDown == 1)
                   if(updown->prev) updown = updown->prev;
                                                              
                strcpy(command, updown->cmd);
                index = strlen(command) ; 
                CheckUpDown = 1;                      
            }                  
        }
       else if(flag == 1 && ch == 66)
       {
           if(updown)
           {
                       //strcpy(command , "MyUser$ ");
                 if(CheckUpDown == 1) 
                 {               
                     if(updown->next) updown = updown->next;                   
                     strcpy(command, updown->cmd);
                 }
                 else
                      strcpy(command , "MyUser$ "); 
                index = strlen(command) ;               
           }
       }
    
       else if(isprint(ch) && count == 0)
       { 
         //printf("%c",ch);       
          command[index] = ch;
          command[++index] = '\0';
          //printf("\33[2K\r");
          //printf("%s",command);
   
       }
       if(count == 2)
       {
           flag = 0; count = 0;
       }
       
           
    }
    return 0;
}

