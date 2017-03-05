#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <libgen.h>
#include <pwd.h>
#include <unistd.h>


int is_number (char *str)
{
	int i;
	for (i=0; str[i] != '\0'; i++)
	{
		if (!isdigit (str[i]))
		{
			return 0;
		}
	}
	return 1;
}

void findParameters ()
{
	DIR *dirp;
	FILE *fp;
	struct dirent *entry;

	char path[1024], read_buf[1024],temp_buf[1024];
	
        char uid[6]={0},*line;
	char uptime_str[10];
	char *user,*command;
	size_t len=0;
	dirp = opendir ("/proc/");
	if (dirp == NULL)
	{
		perror ("Fail");
		exit(0);
	}
	strcpy(path,"/proc/");
	//strcat(path,"uptime");
	//fp=fopen(path,"r");

	//if(fp!=NULL)
	//{
	//	getline(&line,&len,fp);
	//	sscanf(line,"%s ",uptime_str);
	//}

	//long uptime=atof(uptime_str);
	//long Hertz=sysconf(_SC_CLK_TCK);

	//strcpy(path,"/proc/");
	//strcat(path,"meminfo");

	//fp=fopen(path,"r");
	// long total_memory;
	//if(fp!=NULL)
	//{
	//	getline(&line,&len,fp);
	//	sscanf(line,"MemTotal: %llu kB",&total_memory);
	//}
	while ((entry = readdir (dirp)) != NULL)
	{
		if (is_number (entry->d_name))
		{
		  strcpy(path,"/proc/");
		  strcat(path,entry->d_name);
		  strcat(path,"/status");
		  unsigned long long memory_rss;
		  fp=fopen(path,"r");
		  unsigned long long vmsize;
		  if(fp!=NULL)
		  {
		     vmsize=0;
                     int i;
                     for(i = 1; i<=8 ; i++)
                         getline(&line,&len,fp);
		     
                     sscanf(line,"Uid: %s ",uid);
                     for( i = 1; i<=5; i++)  
		        getline(&line,&len,fp);
		     
                     //sscanf(line,"VmSize: %llu kB",&vmsize);
                     for(i  = 1; i<=5; i++)
                        getline(&line,&len,fp);
                     //sscanf(line,"VmRSS: %llu kB",&memory_rss);
                 }
                 else
                 {
                     fprintf(stdout,"FP is NULL\n");
                 }
                 //float memory_usage=100*memory_rss/total_memory;
                 strcpy(path,"/proc/");
                 strcat(path,entry->d_name);
                 strcat(path,"/stat");
                 fp=fopen(path,"r");
                 getline(&line,&len,fp);

                 char comm[10],state;
                 int flags;
                 int pid,ppid,pgrp,session,tty_nr,tpgid;
                 long minflt,cminflt,majflt,cmajflt,utime,stime;
                 long long starttime;
                 long cutime,cstime,priority,nice,num_threads,itreavalue;

                 sscanf(line,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu",&pid,comm,&state,&ppid,&pgrp,&session,&tty_nr,&tpgid,&flags,&minflt,&cminflt,&majflt,&cmajflt,&utime,&stime,&cutime,&cstime,&priority,&nice,&num_threads,&itreavalue,&starttime);

                 //long total_time=utime+stime;
                 //total_time=total_time+(unsigned long)cutime+(unsigned long)cstime;
                 //float seconds=uptime-(starttime/Hertz);
                 //float cpu_usage=100*((total_time/Hertz)/seconds);
                 //if(isnan(cpu_usage))
                 // {
                 //        cpu_usage=0.0;
                 // }
                //if(isnan(memory_usage))
               // {
               //      memory_usage=0.0;
               // }
                strcpy (path, "/proc/");
                strcat (path, entry->d_name);
                strcat (path, "/comm");
                fp = fopen (path, "r");
                if (fp != NULL)
                {
                    fscanf (fp, "%s", read_buf);
                     fclose(fp);
                }
		char *userName;
         	struct passwd *pw = getpwuid(atoi(uid));
	        if (pw)
	        {
		    userName =  pw->pw_name;
	        }
	        else
                    userName = NULL;

               
               if(strlen(userName)<9)
               {
                  user=userName;
               }
               else
               {
                  user=uid;
               }
               fprintf(stdout, "%7s %5s %7d %c %s\n",user, entry->d_name, tty_nr, state, read_buf);  
              // fprintf(stdout,"%7s %5s %0.1f   %0.1f %10lu %10lu %c %s\n",user,entry->d_name, cpu_usage,memory_usage,vmsize,memory_rss,state,read_buf);
          }
     } 
      closedir (dirp);
}

int main (int argc, char *argv[])
{
   findParameters();
   return 0;
}



