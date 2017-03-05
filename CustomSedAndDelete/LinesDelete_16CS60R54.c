#include<stdio.h>
#include<stdbool.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
/*
     Test cases handled 
  mysed [1, 4]d filename
  mysed \[1, 4]d filename
  mysed [5]d filename
  mysed [1...5]d filename
  mysed \[1...5]d filename
  mysed [1, 5...10]d filename
  mysed \[1, 5...10]d filename

*/
/**
      bool contains(long int *arr, long int count, long int no)
          if used to find whether arr of size count contains a specific no or not,
          return true or false accordingly.
       
*/
bool contains(long int *arr , long int count , long int no)
{
   long int lb = 0, ub = count - 1 ,mid;
   while(lb <= ub)
   {
     mid = (lb + ub) / 2;
     if(arr[mid] == no)
       return true;
     else
     {
      if(no < arr[mid])
         ub = mid - 1;
      else
         lb = mid + 1;
     }    
   }
   return false;
    
}
/**
     void deleteLinesFromFile(char *fname, long int *arr, long int count, bool flagDelete)
        Arguments
        fname - name of file, from which the lines are supposed to delete.
        arr - contains the line numbers in sorted array.
        count - is size of array.
        flagDelete - is true if lines are required to delete
                     is false if lines are not required to delete.
        
*/
void deleteLinesFromFile(char *fname, long int *arr, long int count, bool flagDelete)
{
   FILE *fp1, *fp2;
   char tempfile[50];
   fp1 = fopen(fname,"r");
   if(fp1 == NULL)
   {
       printf("Error in opening file\n");
       exit(0);
   }
   strcpy(tempfile , fname);
   strcat(tempfile, "123.dat");
   fp2 = fopen(tempfile, "w");
   if(fp2 == NULL)
   {
      printf("Error in opening file\n");
      exit(0);
   }
   char *line = NULL;
   size_t len = 0;
   ssize_t read;
   long int i = 1;
   //printf("Flag delete is %s " , flagDelete ? "true" : "false");
   while ((read = getline(&line, &len, fp1)) != -1) 
   {
      //printf("Retrieved line of length %zu :\n", read);
      //printf("%s", line);
      if(!(flagDelete ^ contains(arr, count, i)))
      {
         i++;
         continue;         
      }
      fputs(line,fp2);
      i++;   
   }
   free(line);
   fclose(fp1);
   fclose(fp2);
   rename(tempfile, fname);
   fp1 = fopen(fname, "r");
   if(fp1 == NULL)
   {
     printf("Error in opening the file\n");
     exit(0);
   }
   char ch;
   while((ch = fgetc(fp1)) != EOF)
     printf("%c",ch);
   fclose(fp1);
}

typedef struct SLL
{
   long int lineNo;
   struct SLL *next;
}sll ;

void main(int argc , char *argv[])
{
  bool flagLines = true, flagComma = false, flagDot = false;
  int i,j, openBracketCount = 0 , closeBracketCount = 0;
  int dCount = 0, dotCount = 0;
  sll *head = NULL;
  sll *list;
  sll *curr;
  int noOfLines = 0;
  //printf("Testing char %c\n",argv[1][0]);
  if(argv[1][0] == '\\')
  {
     //printf("testing\n");
     flagLines = false;
  }
  int index = 0;
  for(i = 1; i<argc - 1 && dCount < 1 ; i++)
  {
     char lno[10];
     
     //printf("Testing %s " , argv[i]);
     for(j = 0; j<strlen(argv[i]) ; j++)
     {
        if(argv[1][0] == '\\' && i == 1 && j == 0)
          continue;
        if(argv[i][j] == '[')
        {
           openBracketCount++;
           if(openBracketCount > 1)
           {
               //printf("Test4");
               printf("Error \n");
               exit(0);
           }
           continue;
        } 
        if(argv[i][j] == ']' || argv[i][j] == ',')
        {
           if(argv[i][j] == ']')
           {
              closeBracketCount++;
              if(closeBracketCount > 1)
              {
                 //printf("Test5");
                 printf("Error \n");
                 exit(0);
              }
              
           }
           if(index == 0)
           {
               //printf("Test6");
               printf("Error\n");
               exit(0);
           }
                 
           if(argv[i][j] == ',')
           {
             flagComma = true;
	      
           }  
           if(flagDot == false)
           {
             lno[index] = '\0';           
             index = 0;
             if(atol(lno) <= 0)
             {
               //printf("Test88");
               printf("Error\n");
               exit(0);
             }
             noOfLines++;
             curr = (sll *) malloc (sizeof(sll));
 	     curr->lineNo = atol(lno);
 	     
 	     curr->next = NULL;
 	     
             if(head == NULL)
               head = curr;
             else
               list->next = curr;
             list = curr;
             continue;
          }   
          else
          {
              if(curr == NULL)
              {
                 //printf("Test7");
                 printf("Error \n");
                 exit(0);
              }
              lno[index] = '\0';
              index = 0;
              if(atol(lno) <= 0)
              {
                //printf("Test117");
                printf("Error\n");
                exit(0);
               }
              long int insertIndex = curr->lineNo;
              long int lnno = atol(lno);
              long int lines;
              bool checkRange = false;
              if(insertIndex == lnno)
                 checkRange = true; 
              for(lines = insertIndex+1 ; lines <= lnno; lines++)
              {
                 noOfLines++;
                 curr = (sll *) malloc(sizeof(sll));
                 curr->lineNo = lines;
                 curr->next = NULL;
                 if(head == NULL)
                    head = curr;
                 else
                    list->next  = curr;
                 list = curr;
                 checkRange = true;
              } 
              if(checkRange == false)
              {
                 printf("Range is incorrect\n");
                 exit(0);
              }              
              flagDot = false;
              continue;
          }
        }
        if(argv[i][j] == 'd')
        {
           dCount++;
           break;
        }
        if(argv[i][j] == ' ')
        {
           printf("Test Space index is %d\n",index);
	   if(index != 0)
	   {
	      //printf("Test8");
	      printf("Error \n");
              exit(0);	      
	   }
           continue;
        }
        
        if(argv[i][j] == '.')
        {
            if(dotCount != 0)
            {
              printf("Error \n"); exit(0);
            }
            dotCount++; flagDot = true;
            while(argv[i][++j] == '.')
            {
                dotCount++;
            }
            --j;
            if(dotCount != 3)
            {
               printf("Error \n");
               exit(0);
            }
            else
            {
                if(index == 0)
                {
                  //printf("Test 11");         
                  printf("Error\n");
                  exit(0);
                }
                lno[index] = '\0';
                noOfLines++;
                if(atol(lno) <= 0)
                {
                  //printf("Test194");
                  printf("Error\n");
                  exit(0);
                }
                curr = (sll *) malloc (sizeof(sll));
 	        curr->lineNo = atol(lno);
 	        curr->next = NULL;
 	        if(head == NULL)
                    head = curr;
                else
                   list->next = curr;
                list = curr;
             
            }
            index = 0 ;
            dotCount = 0;
            continue;
        }
        if(isdigit(argv[i][j]))
        {
           if(index != 0 && j ==0)
           {
             // printf("Test20");
              printf("Error\n");
              exit(0);
           }
           lno[index++] = argv[i][j];
          // printf("Processing %s index = %d\n",lno, index);
           continue;
        }
        else
        {
	  //printf("%c Test9", argv[i][j]);
          printf("Error\n");
          exit(0);
        }
     }    
  }
  if(noOfLines == 0)
  {
     printf("Error\n");
     exit(0);
  }
  if(openBracketCount == 0 || closeBracketCount == 0 || dCount == 0)
  {
     //printf("Test10");
     printf("Error \n");
     exit(0);
  }
  
  if(noOfLines == 1)
  {
     long int fromStart = 1;
     long int toEnd = head->lineNo;
     int iter;
     for(iter = toEnd - 1; iter >= fromStart; iter--)
     {
        curr = (sll *) malloc (sizeof(sll));
        curr->lineNo = iter;
        curr->next = head;
        head = curr;
        noOfLines++;        
     }   
  }
  /*list = head;
  printf("No of lines %d\n" ,noOfLines);
  while(list)
  {
     printf("\t %ld", list->lineNo);
     list = list->next;     
  }
  */
  list = head;
  long int *nos;
  nos = (long int *) malloc(noOfLines * sizeof(long int));
  long int insert_i = 0,insert_k = 0;
  nos[insert_i++] = list->lineNo;
  list = list->next;
  int count = 1;
  
  while(list)
  {
     //printf("Processing %ld \n insert_i = %ld", list->lineNo, insert_i);
     long int y = list->lineNo;
     for(insert_k = insert_i-1 ; insert_k >= 0 && nos[insert_k] > y ; insert_k--)
     {
        nos[insert_k + 1] = nos[insert_k];
     }
     if(nos[insert_k] == y)
     {
       int arrangeAgain;
       for(arrangeAgain = insert_k+1; arrangeAgain < insert_i; arrangeAgain++)
          nos[arrangeAgain] = nos[arrangeAgain+1];
       list = list->next;
       continue;
     }  
     nos[insert_k+1] = y;
     count++;
     insert_i++;
     list = list->next;
     
  }
   
  list = head;
  while(list)
  {
      curr = list;
      list = list->next;
      free(curr);
  }
  deleteLinesFromFile(argv[argc-1] , nos , count , flagLines);
  printf("\n");   
  return;
}
