#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
  int length;
  bool flag = false;
/**
    int match(char **regexp, char *text)
       Arguments
       1. regexp - is the regular expression which is used to match some pattern in text
       2. text - is the string to match the pattern.
     returns 1 if string of regexpression is found anywhere in text
             0 otherwise.  
*/
  int match(char *regexp, char *text)
  {
     do {   
            //printf(" %s %s " , regexp , text);
          flag = false;
          if (matchhere(regexp, text))
             return 1;
      } while (*text++ != '\0');
      return 0;
  }

  /**
       int matchhere(char *regexp , char *text)
           matches the regexp at the beginning of text
           returns 1 on success
                   0 on failure.
  */  
  int matchhere(char *regexp, char *text)
  {
        
        if (regexp[0] == '\0')
            return 1;
        if (regexp[1] == '*')
        {
           flag = false;
           if(regexp[0] == text[0])
           {
             length++;
             return matchhere(regexp, text+1);
           }  
           else
             return matchhere(regexp+2, text);
        }
        if (regexp[1] == '+')
        {
           //printf("\nregexp[0] = %c regexp[1] = %c regexp[2] = %c text = %s " , regexp[0],regexp[1],regexp[2],text);
            //getchar();
            
            if(regexp[0] == text[0]) 
            {
               length++;
               flag = true;
               return matchhere(regexp, text+1);
            }  
            else if(regexp[0] != text[0] && flag)
                return matchhere(regexp+2 , text);
            else              
              return 0;
        }    
        if(regexp[1] == '?')
        {
           flag = false;
           if(regexp[0] != text[1] && regexp[0] == text[0])
           {
              length++;
              return matchhere(regexp+2, text+1);
           }   
           if(regexp[0] != text[0])
              return matchhere(regexp+2, text);   
           return 0; 
        }    
        if (regexp[0] == '$' && regexp[1] == '\0')
            return *text == '\0';
        if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
        {
            length++;
            flag = false;
            return matchhere(regexp+1, text+1);
        }
        return 0;
   }

/**
        void mytrim(char *t, char *d)
        trims a string in t for leading and trailing white spaces
        and stores it in d.
*/    
    
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

void main(int argc , char *argv[])
{
   char *token;
   int i, j ;
   char **arguments;
   FILE *fp, *fptemp;
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

   if(argc != 3)
   {
      printf("Test 1 ");
      printf("Error \n");
      exit(0);
   }
   fp = fopen(argv[2] , "r");
   fptemp = fopen("t1.txt","w");
   
   if(fp == NULL || fptemp == NULL)
   {
     //printf("Test of file opening ");
     printf("Error in opening the file(s) \n");
     exit(0);
   }
   arguments = (char **) malloc(5 * sizeof(char *));
   token = strtok(argv[1],"\'");
   i = 0; 
   while(token != NULL && i <= 2)
   {
      arguments[i] = (char *) malloc((strlen(token) + 1) * sizeof(char));
      strcpy(arguments[i++],token);
      token = strtok(NULL, "\'");
   }
   if(i != 1)
   {
      //printf("Test 2 ");
      printf("Error \n");
      exit(0);
   }
   //printf("Value of i is %d string is %s",i, arguments[0]);
   char **fields;
   fields = (char **) malloc(10 * sizeof(char *));
   int fieldCount = 0;
   token = strtok(arguments[0] , "/");
   while(token != NULL && i <= 6)
   {
      fields[fieldCount] = (char *) malloc((strlen(token) + 1) * sizeof(char));
      strcpy(fields[fieldCount++] , token);
      token = strtok(NULL , "/");
      
   }     
   if(fieldCount > 5)
   {
      //printf("Test 3");
      printf("Error \n");
      exit(0);
   }
   
     //for(j = 0; j < fieldCount ;j++)
     //printf("Field %d is  %s \n" , j, fields[j]);
     
   /* Case with Optional argument */
   if(fieldCount == 5 && strcmp(fields[1] ,"s") == 0 )
   {
      if(strcmp(fields[4] , "g") == 0)
      {
         char *line = NULL;
         size_t len = 0;
         ssize_t read;
         long int prLineNo = 0;
         //printf("Flag delete is %s " , flagDelete ? "true" : "false");
         while ((read = getline(&line, &len, fp)) != -1) 
         {
            //printf("Retrieved line of length %zu :\n", read);
            //printf("%s", line);
            prLineNo++;
            if(match(fields[0] , line) == 1)
            {
                 //printf("%s is found in prLineNo = %ld\n", fields[0],prLineNo);
                 //printf("line  : %s\n",line);
                 long int i = 0;
                 char *text;
                 
                 text = (char *) malloc(strlen(line) * sizeof(char));
                 strcpy(text , line);
                 
                 do 
                 {
                    length = 0 ; flag = false;
                    int lengthOfField = strlen(fields[2]);    
                      //printf(" %s %s " , regexp , text);
                    if (matchhere(fields[2], text))
                    {
                      //  printf("%s found , i is %ld Length is %d text is %s\n" , fields[2],i, length,text);     
                        text = text + length;
                        i = i + length - 1;
                      //  printf("text + length is  %s\n",text);
                        if(fields[2][lengthOfField-1] == '*' || fields[2][lengthOfField-1] == '+')
                        while(*text == fields[2][lengthOfField-2])
                            text++;   
                        //printf("text after * or + is %s\n",text);
                        fputs(fields[3],fptemp);
                        text--;                        
                    }    
                    else
                    {
                        //printf("%c\n",*text);
                        char ch = *text;
                        if(ch != EOF && ch != '\0' && ch != '\n')
                           fprintf(fptemp ,"%c", ch);
                        i++;
                    }        
                 } while (*text++ != '\0');
                 fputc('\n',fptemp); 
            }
            else
            {
                 //printf("%s is not found in prLineNo = %ld\n" , fields[0], prLineNo);
                 fputs(line,fptemp);
            }               
         }     
      }
      else
      {
         int no = atoi(fields[4]);
         if(no <= 0 )
         {
           //printf("Test 5");
           printf("Error \n");
           exit(0);
         }
         char *line = NULL;
         size_t len = 0;
         ssize_t read;
         long int prLineNo = 0;
         long int occurrenceNo = 0;
         //printf("Flag delete is %s " , flagDelete ? "true" : "false");
         while ((read = getline(&line, &len, fp)) != -1) 
         {
            //printf("Retrieved line of length %zu :\n", read);
            //printf("%s", line);
            prLineNo++;
            if(match(fields[0] , line) == 1)
            {
                // printf("%s is found in prLineNo = %ld\n", fields[0],prLineNo);
                // printf("line  : %s\n",line);
                 long int i = 0;
                 char *text;                 
                 text = (char *) malloc(strlen(line) * sizeof(char));
                 strcpy(text , line);                 
                 do 
                 {
                    length = 0 ; flag = false;
                    int lengthOfField = strlen(fields[2]);    
                    //printf(" %s %s " , regexp , text);
                    if (matchhere(fields[2], text))
                    {
                      //  printf("%s found , i is %ld Length is %d text is %s\n" , fields[2],i, length,text);     
                        occurrenceNo++;
                        if(occurrenceNo == no)
                        {
                          fputs(fields[3],fptemp);
                          text = text + length ;
                          i = i + length - 1;
                         // printf("text + length is  %s\n",text);
                          if(fields[2][lengthOfField-1] == '*' || fields[2][lengthOfField-1] == '+')
                             while(*text == fields[2][lengthOfField-2])
                                text++;
                           text--;        
                          //printf("text after * or + is %s\n",text);
                        }
                        else
                        {
                          int j;
                          for(j = 0 ; j < length; j++)
                              fprintf(fptemp, "%c", *text++);
                          if(fields[2][lengthOfField-1] == '*' || fields[2][lengthOfField-1] == '+')
                             while(*text == fields[2][lengthOfField-2])
                             {
                                fprintf(fptemp, "%c" , *text);
                                text++;
                             }     
                        }  
                                                
                    }    
                    else
                    {
                        //printf("%c\n",*text);
                        char ch = *text;
                        if(ch != EOF && ch != '\0' && ch != '\n')
                           fprintf(fptemp ,"%c", ch);
                        i++;
                    }        
                 } while (*text++ != '\0');
                 fputc('\n',fptemp); 
            }
            else
            {
                // printf("%s is not found in prLineNo = %ld\n" , fields[0], prLineNo);
                 fputs(line,fptemp);
            }              
         }       
      }         
   }  
   /*Case without optional argument*/
   else if(fieldCount == 4 && strcmp(fields[0], "s") == 0)
   {
      if(strcmp(fields[3] , "g") == 0)
      {
         char *line = NULL;
         size_t len = 0;
         ssize_t read;
         long int prLineNo = 0;
         //printf("Flag delete is %s " , flagDelete ? "true" : "false");
         while ((read = getline(&line, &len, fp)) != -1) 
         {
           //printf("Retrieved line of length %zu :\n", read);
           //printf("%s", line);
           prLineNo++;
            
           // printf("line  : %s\n",line);
           long int i = 0;
           char *text;
           text = (char *) malloc(strlen(line) * sizeof(char));
           strcpy(text , line);
           do
           {
                  length = 0 ; flag = false;
                  int lengthOfField = strlen(fields[1]);    
                  //printf(" %s %s " , regexp , text);
                  if (matchhere(fields[1], text))
                  {
                    //printf("%s found , i is %ld Length is %d text is %s\n" , fields[2],i, length,text);     
                    text = text + length;
                    i = i + length - 1;
                    //printf("text + length is  %s\n",text);
                    if(fields[1][lengthOfField-1] == '*' || fields[1][lengthOfField-1] == '+')
                        while(*text == fields[1][lengthOfField-2])
                            text++;   
                    //printf("text after * or + is %s\n",text);
                    fputs(fields[2],fptemp);
                    text--;                        
                  }//if closed    
                  else
                  {
                    //printf("%c\n",*text);
                    char ch = *text;
                    if(ch != EOF && ch != '\0' && ch != '\n')
                        fprintf(fptemp ,"%c", ch);
                    i++;
                  } //else closed       
              } while (*text++ != '\0');  //do closed
                 fputc('\n',fptemp); 
            } //while closed     
     
      }//if closed
      else
      {
         int no = atoi(fields[3]);
         if(no <= 0 )
         {
           //printf("Test 5");
           printf("Error \n");
           exit(0);
         }
         char *line = NULL;
         size_t len = 0;
         ssize_t read;
         long int prLineNo = 0;
         long int occurrenceNo = 0;
          //printf("Flag delete is %s " , flagDelete ? "true" : "false");
         while ((read = getline(&line, &len, fp)) != -1) 
         {
            //printf("Retrieved line of length %zu :\n", read);
            //printf("%s", line);
            prLineNo++;
           
           // printf("line  : %s\n",line);
            long int i = 0;
            char *text;
                 
            text = (char *) malloc(strlen(line) * sizeof(char));
            strcpy(text , line);
            do 
            {
                 length = 0 ; flag  = false;
                 int lengthOfField = strlen(fields[1]);    
                 //printf(" %s %s " , regexp , text);
                 if (matchhere(fields[1], text))
                 {
                   // printf("%ld line %s found , i is %ld Length is %d text is %s\n" ,prLineNo, fields[1],i, length,text);     
                    occurrenceNo++;
                    //getchar();
                    if(occurrenceNo == no)
                    {
                       fputs(fields[2],fptemp);
                       text = text + length;
                       i = i + length - 1;
                       //printf("text + length is  %s\n",text);
                       if(fields[1][lengthOfField-1] == '*' || fields[1][lengthOfField-1] == '+')
                       while(*text == fields[1][lengthOfField-2])
                             text++;   
                      // printf("text after * or + is %s\n",text);
                       text--;
                    }
                    else
                    {
                        int j;
                        for(j = 0 ; j < length; j++)
                            fprintf(fptemp, "%c", *text++);
                        if(fields[1][lengthOfField-1] == '*' || fields[1][lengthOfField-1] == '+')                            while(*text == fields[1][lengthOfField-2])
                           {
                                fprintf(fptemp, "%c" , *text);
                                text++;
                           }  
                        text--;      
                     }  
                  }  //if closed  
                  else
                  {
                       // printf("%c\n",*text);
                        char ch = *text;
                        if(ch != EOF && ch != '\0' && ch != '\n')
                           fprintf(fptemp ,"%c", ch);
                        i++;
                    }//else closed        
                 } while (*text++ != '\0');  //do closed
                 fputc('\n',fptemp); 
            }//while  closed
        }//else closed
          
   } //else if closed
   else
   {
     // printf("Test 4");
      printf("Error \n");
      exit(0);
   } 
     
   for(j=0; j < fieldCount ; j++)
      free(fields[j]);       
   free(fields);  
   free(arguments[0]);
   free(arguments);
   fclose(fp);
   fclose(fptemp);
   rename("t1.txt", argv[2]);
   fp = fopen(argv[2], "r");
   if(fp == NULL)
   {
     printf("Error in opening the file\n");
     exit(0);
   }
   char ch;
   while((ch = fgetc(fp)) != EOF)
     printf("%c",ch);
   fclose(fp);
   return;
}

