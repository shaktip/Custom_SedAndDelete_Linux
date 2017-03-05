#include<stdio.h>

#include<signal.h>

#include<stdlib.h>

/**
   sig_handler3 is registered by sig_handler2 to handle Cltr+c(SIGINT) , which is third
   attempt of Cltr + c.
   where the code is then terminated using kill function.
*/
void sig_handler3(int signum)
{
   //printf("Third Attempt \n");
   kill( getpid(), SIGINT );
   exit(0);   
}
/**
    sig_handler2 is registered by sig_handler1 to handle Cltr+c(SIGINT) , which is second
    attempt of Cltr+c.
*/
void sig_handler2(int signum)
{
   //printf("Second Attempt \n");
   printf("I am taking rest now\n");
   signal(SIGINT , sig_handler3);
   return ;
}
/**
    sig_handler1 is registered in main to handle Cltr+c(SIGINT), which is first attempt of 
    Ctrl+c, hit by the user.
*/
void sig_handler1(int signum)
{
   // printf( "First Attempt\n" );
    FILE *fp;
    fp  = fopen("temp","w");
    fputs("Interrupt" , fp);
    fclose(fp);
    signal(SIGINT , sig_handler2);
    return;
}
int main()
{
      
   signal(SIGINT, sig_handler1);
   while(1)
   {
    
   }
   return 0;
}
