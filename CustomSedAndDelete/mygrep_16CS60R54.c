
#include<stdio.h>
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>

/* match: search for regexp anywhere in text */
    int match(char *regexp, char *text)
    {
        if (regexp[0] == '^')
            return matchhere(regexp+1, text);
        do {   
            //printf(" %s %s " , regexp , text);
            if (matchhere(regexp, text))
                return 1;
        } while (*text++ != '\0');
        return 0;
    }

    
    int matchhere(char *regexp, char *text)
    {
        if (regexp[0] == '\0')
            return 1;
        if (regexp[1] == '*')
            return matchstar(regexp[0], regexp+2, text);
        if (regexp[1] == '+')
        {
            if(regexp[0] == text[0]) 
               return matchstar(regexp[0], regexp+2, text+1);
            return 0;
        }    
        if(regexp[1] == '?')
        {
           if(regexp[0] != text[1] && regexp[0] == text[0])
              return 1;
           return 0; 
        }    
        if (regexp[0] == '$' && regexp[1] == '\0')
            return *text == '\0';
        if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
            return matchhere(regexp+1, text+1);
        return 0;
    }

    
    int matchstar(int c, char *regexp, char *text)
    {
        do {    
            
            if (matchhere(regexp, text))
                return 1;
        } while (*text != '\0' && (*text++ == c || c == '.'));
        return 0;
    }
int main(int argc,char *argv[])
{

   FILE *fp;
   char fline[100];
   char *newline;
   FILE *fpp;
   int i,count=0,occ=0;
   int r;
   /*
   if(open(stdin , 0, 0) == -1)
     printf("Unsuccessful\n");
   else
     printf("Successful\n");
   */
   if(argc < 2)
   {
      printf("Insufficient arguments\n");
      return 0;
   }
   if(argc==2)			//if the grep command is used in pipe 
   {
	 if((fpp=fopen("fileout","r"))==NULL)
	 {
		printf("error\n");
          return 0;
	 }
	 else
	 {
	 	argc=3;
		argv[2]=(char *)malloc(sizeof(char)*10);
		strcpy(argv[2],"fileout");
	 }
		
	}
	
   for(i = 1; i < argc ; i++)
   {
      int len = strlen(argv[i]);
      if(argv[i][len - 1] == '\\')
      {
         //printf("Testing \n");
         char *ptr = (char *) malloc(1024 * sizeof(char)); 
         strcpy(ptr, argv[i]);
         ptr[len-1] = ' ';
         strcat(ptr, argv[i+1]);
         argv[i] = ptr;
         int k;
         for(k = i+1; k<argc-1; k++)
            argv[k] = argv[k+1];
         i--;
         argc--;
      }
   }
   /*
   for(i = 0; i<argc ; i++)
     printf(" %d %s \n", i , argv[i]);
   */  
   //printf("No of arguments %d \n",argc);
   /*if(argc < 3)
   {
      printf("Insufficient Arguments\n");
      return 0;
   }*/
   
   char regexp[1024] = {0};
   bool Minus_v  = false;
   bool Minus_m = false;
   int NoOfMatch = 0;
   for(i = 1 ; i < argc ; i++)
   {
       //printf("\n\t argv [ %d ] = %s ", i, argv[i]);
       if(strcmp(argv[i] , "-v") == 0)
         Minus_v = true;
       else if(strcmp(argv[i] , "-m") == 0)
       {
           Minus_m = true;
           int length, j;
           if(i+1 >= argc )
           {
               printf("Matching lines are incorrect\n");
           }    
           length = strlen (argv[i+1]);
           for (j=0;j<length; j++)
               if (!isdigit(argv[i+1][j]))
               {
                   printf ("Matching lines are incorrect\n");
                   return 0;
               }
           NoOfMatch = atoi(argv[i+1]);
           i++;
       }
       else
       {
          if(strlen(regexp) == 0)
              strcpy(regexp, argv[i]);
          //printf(" Test");
       }
   }  

   //printf("-v : %d -m : %d Regular expression : %s" , (int)Minus_v, (int)Minus_m, regexp);
    if(strlen(regexp) == 0)
    {
        printf("Invalid grep command\n");
        
        return 0;
    } 
    //char ch  = getchar();
    if(!Minus_v && !Minus_m )
    {    
     for(i = 2 ; i < argc; i++)
     {
       fp=fopen(argv[i],"r");
       if(!fp)
       {
          printf("%s is not a valid file \n", argv[i]);
          return 0;
       }
       while(fgets(fline,100,fp)!=NULL)
       {
         if(strlen(fline) == 0)
            continue; 
         if (match(regexp, fline) == 0)
            continue;
         printf("%s: %s",argv[i], fline);          
       }
       fclose(fp);
     }
     
   }
   if(Minus_v && !Minus_m )
    {
      if(argc <= 3)
      {
          printf("Insufficient number of arguments\n");
          return 0;
      }
     
     for(i = 3 ; i < argc; i++)
     {
       fp=fopen(argv[i],"r");
       if(!fp)
       {
          printf("%s is not a valid file\n ", argv[i]);
          return 0;
       }
       while(fgets(fline,100,fp)!=NULL)
       { 
         if(strlen(fline) == 0)
            continue;         
         if (match(regexp, fline) == 0)
           printf("%s: %s",argv[i], fline);          
       }
       fclose(fp);
       }
     }    

    if(!Minus_v && Minus_m)
    {
      int count = 0;
      
      if(argc <= 4)
         printf("Invalid regular expression\n");     
      for(i = 4 ; i < argc; i++)
      {
       fp=fopen(argv[i],"r");
       count = 0;
       if(!fp)
       {
          printf("%s is not a valid file\n ", argv[i]);
          return 0;
       }
       while(fgets(fline,100,fp)!=NULL && count != NoOfMatch )
       {     
         //printf("Check NoOfMatch %d\n %s ", NoOfMatch,fline);   
         if(strlen(fline) == 0)
            continue; 
         if (match(regexp, fline) == 0)
            continue;
         count++;
         printf("%s: %s",argv[i], fline);
                   
       }
       fclose(fp);
     }
     
   }

    if(Minus_v && Minus_m)
    {
      int count = 0;
      if(argc < 5)
      {
          printf("Invalid number of arguments \n");
          return 0;
      }
      for(i = 5 ; i < argc; i++)
      {
       fp=fopen(argv[i],"r");
       count = 0;
       if(!fp)
       {
          printf("%s is not a valid file\n ", argv[i]);
          return 0;
       }
       while(fgets(fline,100,fp)!=NULL && count != NoOfMatch )
       {     
         //printf("Check NoOfMatch %d\n %s ", NoOfMatch,fline);   
         if(strlen(fline) == 0)
            continue; 
         if (match(regexp , fline) == 0)
         {
           count++;
           printf("%s: %s",argv[i], fline);
         }                   
       }

       fclose(fp);
     }
     printf("\n");
     
   }


  /* printf("Inside main");
   if(matchExp("abcdef" , "abcdef ",0,0  ))
      printf("\n\t 1 Match found");
   if(matchExp( "abc*def?" , "abcccde",0,0 ))
      printf("\n\t 2 Match found"); 
  if(matchExp("abc*defaaa" , "abccdeffaaa",0,0 ))
      printf("\n\t 3 Match found");
  if(matchExp("abc*dea*" , "abccdeb",0,0 ))
      printf("\n\t 4 Match found");
   printf("\n");
*/
   return 0;
}


