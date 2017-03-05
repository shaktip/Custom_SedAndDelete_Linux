#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

/**
     sig_handler is registered to handle Cltr+c i.e. SIGINT
*/


void sig_handler(int signum)
{
  printf("Received signal %d \n",signum);
  //printf("Signal originated from process %lu ", (unsigned long) info->si_pid);
}
/**
     sig_to_quit is registered to handle Cltr+\
*/
void sig_to_quit(int signum)
{
    printf( "raise SIGUSR1 signal\n" );
    kill( getpid(), SIGUSR1 );
    exit(0);
}
int main()
{
      
   signal(SIGINT, sig_handler);
   signal(SIGQUIT , sig_to_quit);
   
   
   while(1)
   {
    
   }
   return 0;
}

