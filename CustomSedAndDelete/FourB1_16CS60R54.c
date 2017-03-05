#include  <stdio.h>
#include  <signal.h>
#include <sys/types.h>
#include <unistd.h>
/**
      my_handler is a function to handle the signal, whose int parameter is one of the
      signals, which has generated the signal.
      void (*signal(int signo, void (*func )(int)))(int);
      has a second parameter, which is a function to handle the signal of first parameter of 
      signal function.
*/
void my_handler(int signum)
{
    if (signum == SIGUSR1)
    {
        printf("Received SIGUSR1!\n");
    }
}
void main()
{
   printf("Process is %d\n",(int)getpid());
   signal(SIGUSR1, my_handler);
   while(1)
   {
   }
}  
