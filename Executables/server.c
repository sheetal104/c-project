


#include<stdio.h>
#include"interface.h"




FILE *record;
FILE *f1;
FILE *f2;
FILE *User_Records;
FILE *temp1;
FILE *temp2;
FILE *temp3;
FILE *date;
FILE *st;
char info[500];


void red () 
{
  printf("\033[1;31m");
}

void yellow ()
{
  printf("\033[1;33m");
}


void green()
{ 
	printf("\033[1;32m");
}

void blue()
{ 
	printf("\033[1;34m");
}

void reset () 
{
  printf("\033[0m");
}



#ifdef __unix__

static struct termios old, new;
 
void initTermios(int echo) //initialize new terminal i/o settings
{
  tcgetattr(0, &old);     //get old terminal settings
  new = old;              //make the new settings same as the old
  new.c_lflag &= ~ICANON; //disable buffered i/o
  new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &new); //use the new terminal settings now
}
 
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);//restore the terminal i/o to its old settings
}
 

char getch_(int echo)//echo defines echo mode
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
 
/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}
 
/* Read 1 character with echo */
char getche(void)
{
  return getch_(1);
}
 
#endif


//STRUCTURES USED

   

Book book;


Date id,dd,rd; 
Date *issueDate=&id;
Date *dueDate=&dd;
Date *returnDate=&rd; 
User user;




// FUNCTION TO CHECK USER ID EXISTS 
int avluserID(int uid)
{
 FILE *fp;
 int c = 0;
 fp = fopen("User_Records", "r");
 while (!feof(fp))
 {
  fread(&user, sizeof(user), 1, fp);

  if (uid == user.userID)
  {
   fclose(fp);
   return 1;
  }
 }
 fclose(fp);
 return 0;
}



// FUNCTION TO INSERT RECORDS TO THE FILE
void insert_user()
{
 FILE *fp;
 int res=0,tempuid;
 
 fp = fopen("User_Records", "a");
 printf("\n\tEnter User Details - \n\n");
 printf("\t\tID: ");
 scanf("%d",&tempuid);
 res=avluserID(tempuid);
 if(res==1)
 { red();
      printf("\n\t----ENTERED USER ID ALREADY EXISTS----\n");
      reset();
      return;
 }
 else
 {
    user.userID=tempuid;
 }
 flush;
 printf("\t\tName: ");
 scanf("%[^\n]%*c", user.name);
 flush;
 printf("\t\tDepartment: ");
 scanf("%s", user.dept);
 user.rem_book=3;
 fwrite(&user, sizeof(user), 1, fp);
 yellow();
 printf("\n\t\t---- RECORD ADDED ----");
 reset();
 fclose(fp);
}

// FUNCTION TO DISPLAY RECORDS
void disp_users()
{
 FILE *fp1;
 fp1 = fopen("User_Records", "r");
 blue();
 printf("\n\tUser ID\tName\tDept.\tRemaining Books\n\n");
 reset();
 while (fread(&user, sizeof(user), 1, fp1))
 {	
	printf(" \t%d\t%s\t%s\t%d\n", user.userID, user.name,user.dept,user.rem_book);
 }
 fclose(fp1);
 
}


//FUNCTION TO CHECK THE USER RECORDS FILE IS EMPTY OR NOT
int empty()
{
 int c = 0;
 FILE *fp;
 fp = fopen("User_Records", "r");
 while (fread(&user, sizeof(user), 1, fp))
  c = 1;
 fclose(fp);
 return c;
}

//FUNCTION TO SEARCH THE GIVEN RECORD
void search_user()
{
 FILE *fp2;
 int r, s, avl;
 printf("\n Enter the User ID you want to search: ");
 scanf("%d", &r);
 avl = avluserID(r);
 if (avl == 0)
  printf(" User ID %d not found!\n",r);
 else
 {
  fp2 = fopen("User_Records", "r");
  while (fread(&user, sizeof(user), 1, fp2))
  {
   s = user.userID;
   if (s == r)
   {
    printf("\n\t\tUser ID: %d", user.userID);
    printf("\n\t\tName: %s", user.name);
	printf("\n\t\tDept.: %s", user.dept);
	printf("\n\t\tRemaining Books:%d",user.rem_book);
   
   }
  }
  fclose(fp2);
 }
}

// FUNCTION TO DELETE A RECORD
void delete_user()
{
 FILE *fpo;
 FILE *fpt;
 int r, s;
 printf("\n Enter the User ID you want to delete :");
 scanf("%d", &r);
 if (avluserID(r) == 0)
  printf("\t\tUser ID %d not found!\n", r);
 else
 {
  fpo = fopen("User_Records", "r");
  fpt = fopen("TempFile", "w");
  while (fread(&user, sizeof(user), 1, fpo))
  {
   s = user.userID;
   if (s != r)
    fwrite(&user, sizeof(user), 1, fpt);
  }
  fclose(fpo);
  fclose(fpt);
  fpo = fopen("User_Records", "w");
  fpt = fopen("TempFile", "r");
  while (fread(&user, sizeof(user), 1, fpt))
   fwrite(&user, sizeof(user), 1, fpo);
  green();
  printf("\n\t\t---- RECORD DELETED ----\n");
  reset();
  fclose(fpo);
  fclose(fpt);
 }

}
// FUNCTION TO UPDATE THE RECORD
void update_user()
{
 int avl;
 FILE *fpt;
 FILE *fpo;
 int s, r, ch;
 printf("\n Enter User ID to update:");
 scanf("%d", &r);
 avl = avluserID(r);
 if (avl == 0)
 {red();
  printf("\t\tUser ID %d not found!\n", r);
  reset();
 }
 else
 {
  fpo = fopen("User_Records", "r");
  fpt = fopen("TempFile", "w");
  while (fread(&user, sizeof(user), 1, fpo))
  {
   s = user.userID;
   if (s != r)
    fwrite(&user, sizeof(user), 1, fpt);
   else
   {
    printf("\n\t\t\t1. Update Name\n");
    printf("\n\t\t\t2. Update Department\n");
    printf("\n\t\tEnter your choice:");
    scanf("%d", &ch);
    switch (ch)
    {
    case 1:
     printf("\n Enter Name:");
     scanf("%s", user.name);
     break;
    case 2:
     printf("\n Enter Dept.: ");
     scanf("%s", user.dept);
     break;
    default:
     red();
     printf("\t\tInvalid Option!");
     reset();
     break;
    }
    fwrite(&user, sizeof(user), 1, fpt);
   }
  }
  fclose(fpo);
  fclose(fpt);
  fpo = fopen("User_Records", "w");
  fpt = fopen("TempFile", "r");
  while (fread(&user, sizeof(user), 1, fpt))
  {
   fwrite(&user, sizeof(user), 1, fpo);
  }
  fclose(fpo);
  fclose(fpt);
  green();
  printf("\n\t\t---- RECORD UPDATED ----");
  reset();
 }
}


//FUNC TO DISPLAY THE DATE  
void displayDate(Date *d)
{ blue();
  printf("\t\t\t\t%d/%d/%d\t\t\t\t\t\n",d->dd,d->mm,d->yyyy);
  reset();
} 
  
//FUNC TO CHECK IF GIVEN DATE IS VALID  
int Date_checker(Date *d) 
{
  int dmax;
   
  if(d->mm==1||d->mm==3||d->mm==5||d->mm==7||d->mm==8||d->mm==10||d->mm==12)
		{ d->dmax=31;
			}
	else if(d->mm==2)
		{ if((d->yyyy%4==0 && d->yyyy%100!=0)||(d->yyyy%400==0))
			{d->dmax=29;
				}
		  else
			{d->dmax=28;
				}
			}
	else if(d->mm<1||d->mm>12)
			{ return 0;}
	else
		{ d->dmax=30;
			}
 	if(d->dd>d->dmax||d->yyyy<2016)
 		return 0;
 	return 1;
}

//FUNC TO CALCULATE MAXIMUM DAYS IN A MONTH
int dmax_calc(int mm,int yyyy)
{ int dmax;
   if(mm==1||mm==3||mm==5||mm==7||mm==8||mm==10||mm==12)
		{ dmax=31;
			}
	else if(mm==2)
		{ if((yyyy%4==0 && yyyy%100!=0)||(yyyy%400==0))
			{dmax=29;
				}
		  else
			{dmax=28;
				}
			}
	else
		{ dmax=30;
			}
   return dmax;
}

//FUNC TO CALCULATE DUE DATE ie 1 month post issue date
void due_date_calc(Date *issue,Date *due)
{  if(issue->dd!=issue->dmax)
  {
    if(issue->mm!=12 && issue->mm!=1)
     { due->dd=issue->dd;
       due->mm=issue->mm+1;
       due->yyyy=issue->yyyy;
      }
    else if(issue->dd>=28 && issue->mm==1)
    { 
      due->dd=28;
      due->mm=2;
      due->yyyy=issue->yyyy;
      }
      
     else if(issue->dd<28 && issue->mm==1)
    { 
      due->dd=issue->dd;
      due->mm=2;
      due->yyyy=issue->yyyy;
      }
        
    
   else
   { due->dd=issue->dd;
     due->mm=1;
     due->yyyy=issue->yyyy+1;
     }
     }
   else
   { if(issue->mm!=12)
     { int max=dmax_calc(issue->mm+1,issue->yyyy);
       dueDate->dd=max;
       dueDate->mm=issue->mm+1;
       dueDate->yyyy=issue->yyyy;
       }
     else
     { dueDate->dd=31;
       dueDate->mm=1;
       dueDate->yyyy=issueDate->yyyy+1;
       }
   }
  }

// function to count no.of leap years before the given date 
int countLeapYears(Date *d) 
{ 
    int years = d->yyyy; 
  
    // Check if the current year needs to be considered 
    // for the count of leap years or not 
    if (d->mm <= 2) 
        years--; 
  
    // An year is a leap year if it is a multiple of 4, 
    // multiple of 400 and not a multiple of 100. 
    return years / 4 - years / 100 + years / 400; 
} 
  
// function to return number of days between 2 dates 
int getDifference(Date *dt1, Date *dt2) 
{ 
    const int monthDays[12] = {31, 28, 31, 30, 31, 30, 
                           31, 31, 30, 31, 30, 31}; 
	
	// COUNT TOTAL NUMBER OF DAYS BEFORE FIRST DATE 'dt1' 
  
    // initialize count using years and day 
    long int n1 = dt1->yyyy*365 + dt1->dd; 
  
    // Add days for months in given date 
    for (int i=0; i<dt1->mm - 1; i++) 
        n1 += monthDays[i]; 
  
    // Since every leap year is of 366 days, 
    // Add a day for every leap year 
    n1 += countLeapYears(dt1); 
  
    // SIMILARLY, COUNT TOTAL NUMBER OF DAYS BEFORE 'dt2' 
  
    long int n2 = dt2->yyyy*365 + dt2->dd; 
    for (int i=0; i<dt2->mm - 1; i++) 
        n2 += monthDays[i]; 
    n2 += countLeapYears(dt2); 
  
    // return difference between two counts 
    return (n2 - n1); 
} 

//FUNC TO CALCULATE FINE AMOUNT
int calcFine(int days)
{
	int fine=0;
	
	if(days<11)
		fine=(days*1);       //fine = 1/day for first 10 days
	else
		fine=(days-10)*5+10; //fine = 5/day after 10 days
	
	return fine;
	
}
 
 
//FUNC TO CHECK IF GIVEN BOOK ID EXISTS
int checkBookID(int bid)
{ 
  int flag=0;
  f1=fopen("record.txt","a+");
  while(!feof(f1)&&flag!=1)
  { fscanf(f1,"%d %s %s %d", &book.bid,book.bname,book.author,&book.copies);
    if(bid==book.bid)
    	flag=1;	 	
  }
  fclose(f1);
  return flag;
}	
       

//FUNC TO ADD BOOK TO THE FILE
void Addbook()
{
   
    int i,res=0,tempbid;
    record = fopen("record.txt","a+");
    printf("\n\n Enter Book ID : ");
    scanf("%d",&tempbid);
    res=checkBookID(tempbid);
    if(res==1)
    { red();
      printf("\n\t----ENTERED BOOK ID ALREADY EXISTS----\n");
      reset();
      return;
        }
       else
       {
         book.bid=tempbid;
       }
    printf(" Enter Book Name: ");
        scanf("%s",book.bname);
    printf(" Enter Name of the Author: ");
        scanf("%s",book.author);
    printf(" Enter Number of Copies of the Book: ");
        scanf("%d",&book.copies);
    fprintf(record,"\n%d\t%s\t%s\t%d\t",book.bid,book.bname,book.author,book.copies);      
    fclose(record);
    green();
    printf("\n\t\t---- BOOK ADDED ----\n");
    reset();
}


//FUNC TO SEARCH BOOK BY AUTHOR     
void Author()
{
    int i;   
    char Target[500],line[500];
    int Found=0,fscanfResult;
    if((record=fopen("record.txt","r"))==NULL)
    printf(" File is empty. \n\n");
    else
    {
        printf("\n Enter Name of the Author: ");
            scanf("%s",Target);
        printf("\n Books:");
        while(!feof(record))
        {
            fscanf(record,"%d %s %s %d",&book.bid,book.bname,book.author,&book.copies);
            fscanfResult=fscanf(record,"%[^\n]",line);
            if(fscanfResult==0)
            { fgets(line,sizeof(line),record);
              continue;
              }
            if(strcmp(Target,book.author)==0)
            {
                Found=1;
                printf("\n\n\t\tBook ID:  %d\n\t\tBook Name:  %s\n\t\tAuthor:  %s\n\n",book.bid,book.bname,book.author);
            }
           
        }
        if(!Found)
        {
            red();
            printf(" No such entry exists.\n");
            reset();
            
            }
            
        fclose(record);
    }

}

//FUNC TO SEARCH BOOK BY NAME
void Searchbook()
{
    int i;
    char Target[25];
    int Found=0;
    if((record=fopen("record.txt","r"))==NULL)
        printf(" File is empty.\n\n");
    else
    {
        printf("\n\n Enter Book Name: ");
            scanf("%s",Target);
        while(!feof(record)&& Found==0)
        {
        fscanf(record,"%d %s %s  %d", &book.bid,book.bname,book.author,&book.copies);
            if(strcmp(Target,book.bname)==0)
                Found=1;
        }
        if(Found)
        {
          
            printf("\n\n\t\tBook ID:  %d\n\t\tBook Name:  %s\n\t\tAuthor:  %s\n\n",book.bid,book.bname,book.author);
            }
        else if(!Found)
            {
             red();
             printf("\t\tNo such entry exists.\n");
             reset();
            }
        fclose(record);
    }

}

     
//FUNC TO DISPLAY BOOK        
void Displaybook()
{  
    int fscanfResult;
    char line[50];
    record = fopen("record.txt","a+");
    green();
    printf("\n\t\tBook ID\tName\tAuthor\tCopies\n");
	reset();
    do//change this.
    {   
        fgets(info,500,record);
        printf("\t\t%s\n",info);
      
    }while(!feof(record));
    fclose(record); 
    
}

//FUNC TO DELETE BOOK 
void deleteBook()
{ int id,found=0,fscanfResult;
  char line[500],bname[100];
  printf("\nEnter ID of the book to be deleted: ");
  scanf("%d",&id);
  record=fopen("record.txt","r");
  temp1=fopen("temp1.txt","a+");
  if(temp1==NULL)
  { printf(" File does not exist.\n");
    fclose(temp1);
    }
  else
  {  while(!feof(record))
      {
        
           fscanf(record,"%d %s %s  %d", &book.bid,book.bname,book.author,&book.copies);
           fscanfResult=fscanf(record,"%[^\n]",line);
          
            if(fscanfResult==0)
            { fgets(line,sizeof(line),record);
              continue;
              }
           
         
         else if(id!=book.bid)
              { fprintf(temp1,"\n%d\t%s\t%s\t%d    \t",book.bid,book.bname,book.author,book.copies); 
                      } 
                      
      
                                                   
        }     
        
        fclose(record);
        fclose(temp1);
        green();
        printf("\n\t\t---- BOOK DELETED ----\n");
        reset();
        remove("record.txt");
        rename("temp1.txt","record.txt");
        
        
   }
}


//FUNC TO UPDATE BOOK INFO
void updateBook()
{ record=fopen("record.txt","r");
  temp1=fopen("temp1.txt","a+");
  int choice,newbid,newcopies;
  char newbname[100],newauth[40];
  if(record==NULL)
  	{  red();
  	  printf("\t\tFile does not exist.\n");
  	  reset();
  	  }
  else
  
  {  int bookid;
     printf(" Enter ID of the book to be updated: ");
     scanf("%d",&bookid);
     printf("\n 1)Update Book Name\n 2)Update Book Author\n3)Update number.of copies\n\n");
     scanf("%d",&choice);
     switch(choice)
     { 
       case 1:
       printf("\n Enter new Book Name: ");
       scanf("%s",newbname);
       break;
       case 2:
       printf("\n Enter new Book Author: ");
       scanf("%s",newauth);
       break;
       case 3:
       printf("Enter the new number of copies\n");
       scanf("%d",&newcopies);
       break;
       default:
       red();
       printf("\t\tInvalid option!\n");
       reset();
       return;
       break;
       }
     while(!feof(record))
     {   fscanf(record,"%d %s %s %d", &book.bid,book.bname,book.author,&book.copies);  
         if(feof(record))
          {  break;
          }
         else if(book.bid!=bookid)
         { fprintf(temp1,"\n%d\t%s\t%s\t%d    \t",book.bid,book.bname,book.author,book.copies);
           }

         else if(choice==1)
         {  fprintf(temp1,"\n%d\t%s\t%s\t%d    \t",book.bid,newbname,book.author,book.copies);
          }
         else if(choice==2)
         {  fprintf(temp1,"\n%d\t%s\t%s\t%d    \t",book.bid,book.bname,newauth,book.copies);
         }
         else if(choice==3)
         { fprintf(temp1,"\n%d\t%s\t%s\t%d    \t",book.bid,book.bname,book.author,newcopies);
          }
         
        } 
         
        fclose(record);
        fclose(temp1);
        green();
        printf("\n\t\t ---- BOOK UPDATED ----\n");
        reset();
        remove("record.txt");
        rename("temp1.txt","record.txt");
       
        
        }
}


//FUNC TO DISPLAY BORROWED BOOK DETAILS
void Dateinfo()
{ 
  char data[500];
  date=fopen("dates.txt","r");
  if(date==NULL)
  { printf(" File Does Not Exist.\n");
   }
  else
  { 
    printf("\nB.Name\tUserID\tIssue Date\tDue Date\n");
      
     do
     { fgets(data,500,date);
       printf("%s\n",data);
       }while(!feof(date));
   }
  
}


//FUNC TO ISSUE BOOK
void Issue()
{
    int userID,i,Found1=0,Found2=0,remBook,bid;char issuebookname[20];
	
    printf("\n\n Enter User ID: ");
        scanf("%d",&userID);
		
	if((User_Records=fopen("User_Records","r"))==NULL)
        { printf(" File is empty!\n\n");
        }
	else																																																																																																																																																																																	             
    {
        while(fread(&user, sizeof(user), 1,User_Records)&& Found1==0)
        {
          
            if(userID==user.userID)//check if user id matches
            {
                Found1=1;
                remBook=user.rem_book;
            }
        }
        if(Found1)
        {  
            if(remBook<1)
            {   red();
                printf("\n\t\t CANNOT ISSUE MORE THAN 3 BOOKS.\n");
                reset();
                return;
            }
            else
            {    printf("\n Enter Book Name: ");
                scanf("%s",issuebookname);
                /*printf("Enter the book ID:\n");
                 scanf("%d",&bid);
                 *///try this
                st=fopen("status.txt","a+");
                while(!feof(st))
                { fscanf(st,"%d %s",&book.stat.uID,book.stat.bname);
                  if(strcmp(issuebookname,book.stat.bname)==0 && userID==book.stat.uID) //check if user has already borrowed the book
                  {  red();
                     printf("\n\t\tTHIS BOOK IS ALREADY ISSUED TO YOU!\n");
                     fclose(st);
                     reset();
                     return;
                   }               
                }
                
                
                           
                if((record=fopen("record.txt","r"))==NULL)
                    printf(" File is empty.\n\n");
                else
                {
                    while(!feof(record)&& Found2==0)
                    {
                        fscanf(record,"%d %s %s %d", &book.bid,book.bname,book.author,&book.copies);
                        if(strcmp(issuebookname,book.bname)==0)//checks if book exists
                            Found2=1;
                        
                    }
                    if(Found2)
                    {
                      
                        if(book.copies<1)
                        { yellow();
                          printf("\n\t\tSORRY, NO MORE COPIES LEFT.\n");
                          reset();
                          return;
                          }
                        else
                        {   
                           
                            f2=fopen("f2","w");
                            if((temp2=fopen("User_Records","r"))==NULL)
                                printf(" File is empty.\n\n");
                            else
                            {
                                while(fread(&user, sizeof(user), 1, temp2))
                                {
                                   
                                    if(userID==user.userID)
                                    {
                                        user.rem_book--;
										fwrite(&user, sizeof(user), 1, f2);
                                       
                                    }
                                    else{ 
                                         fwrite(&user, sizeof(user), 1, f2);
										 
									   }
                                    if(feof(temp2))
                                        break;
                                }
                            }
                            //fclose(temp2);
                            //fclose(f2);
                           

                            f1=fopen("f1.txt","w");
                            st=fopen("status.txt","a");
                            if((temp1=fopen("record.txt","r"))==NULL)
                                printf(" File is empty.\n\n");
                            else
                            {
                                while(!feof(temp1))
                                {
                                      fscanf(temp1,"%d %s %s  %d", &book.bid,book.bname,book.author,&book.copies);
                                    if(feof(temp1))
                                        break;
                                    if(strcmp(issuebookname,book.bname)!=0)
                                    {   
                                        fprintf(f1,"\n%d\t%s\t%s\t%d    \t",book.bid,book.bname,book.author,book.copies);
                                        
                                        
                                    }
                                    else
                                    {   book.copies--;
                                        fprintf(f1,"\n%d\t%s\t%s\t%d\t",book.bid,book.bname,book.author,book.copies);
                                       
                                        
                                    }
                                   
                                }
                                
                                
                                
                               fprintf(st,"\n%d\t%s   \t",userID,issuebookname); 
                                
                                
                                
                            }
                            
                            fclose(temp1);
                            fclose(f1);
                            fclose(st);
                            fclose(record);
                            fclose(User_Records);
                            fclose(temp2);
                            fclose(f2);
                            remove("record.txt");
                            rename("f1.txt","record.txt");
                            remove("User_Records");
                            rename("f2","User_Records");
                            
                            
                            
                            int res=0;
                            while(!res)
                            {
                              printf("\n Enter Issue Date (dd/mm/yyyy): ");
                              scanf("%d/%d/%d",&issueDate->dd,&issueDate->mm,&issueDate->yyyy);
                              res=Date_checker(issueDate);
							  
                            }
                                
                            due_date_calc(issueDate,dueDate);	
                            date=fopen("dates.txt","a+");
                            fprintf(date,"\n%s\t%d\t%d/%d/%d\t%d/%d/%d",issuebookname,userID,issueDate->dd,issueDate->mm,issueDate->yyyy,dueDate->dd,dueDate->mm,dueDate->yyyy);
                            //fprintf(date,"\n%d\t%d\t%d/%d/%d\t%d/%d/%d",bid,userID,issueDate->dd,issueDate->mm,issueDate->yyyy,dueDate->dd,dueDate->mm,dueDate->yyyy);
                            fclose(date);
                            blue();
                            printf("\n\t\t\tIssue Date:\n\n");
                            displayDate(issueDate);
							blue();
                            printf("\n\t\t\tDue Date:\n\n");
                            displayDate(dueDate);
                            green();
                            printf("\n\t\t---- BOOK ISSUED ----\n");
                            reset();
                        }               
                    }
                    else if(!Found2)
                        printf(" No such book exists.\n");
               
                }
            }
        }
        else if(!Found1)
            printf(" Invalid User ID!\n");
       

    }
   
}

//FUNC TO RETURN THE BOOK
void bookret()
{
int userID,i,Found1=0,Found2=0,flag=0,midb,remBook;char retbookname[20],issuebookname[20];
    temp1=record;temp2=User_Records;
    
    printf("\n\n Enter User ID: ");
        scanf("%d",&userID);
    if((User_Records=fopen("User_Records","r"))==NULL)
        printf(" File is empty.\n\n");
  
    else
    {
        while(fread(&user, sizeof(user), 1,User_Records)&& Found1==0)
        {
            if(userID==user.userID)//check if user exists
            {
                Found1=1;
                remBook=user.rem_book;
            }
        }
        if(Found1)
        {
            if(remBook>3)
            {
                printf(" Error!\n");
            }
            else
            {    printf("\n\n Enter Book Name: ");
                scanf("%s",retbookname);
                
                if((record=fopen("record.txt","r"))==NULL)
                    printf(" File is empty.\n\n");
                    
                    
                else
                {
                          
                      char line[500];
                      int fscanfResult=0;
                      st=fopen("status.txt","r");
                      int c = fgetc(st);
                      //Checks if the file is empty
                      if (c == EOF) 
                      {  red();
                         printf("\t\tNo books to be returned\n");
                         reset();
                        }
                                        
                        else 
                        {
                           ungetc(c, st);
                          }
                                  
                      
                      
              
                      while(!feof(st) && flag==0)
                       {     
                          fscanf(st,"%d %s",&book.stat.uID,book.stat.bname);                
                             fscanfResult=fscanf(st,"%[^\n]",line);
                             if(fscanfResult==0)
                             { fgets(line,sizeof(line),record);
                                 continue;
                             }
                           if(userID==book.stat.uID && strcmp(retbookname,book.stat.bname)==0) //checks if book name matches
                              {   flag=1;
                               }
                             
                            
                        }
                             
                             
                       if(flag==0)
                       { 
                         red();
                         printf("\n\t\tYou have already returned this book!\n");
                         fclose(st);
                         reset();
                         return;
                        }
                       fclose(st);
                       
                
                    while(!feof(record)&& Found2==0)
                    {   
                        fscanf(record,"%d %s %s  %d", &book.bid,book.bname,book.author,&book.copies);
                        if(strcmp(retbookname,book.bname)==0)
                        Found2=1;
                        
                    }
                    if(Found2)
                    {
                        {  
                            f2=fopen("f2","w");
                            if((temp2=fopen("User_Records","a+"))==NULL)
                                printf(" File is empty.\n\n");
                            else
                            {
                                while(fread(&user, sizeof(user), 1,temp2))
                                {
                                   
                                    if(userID==user.userID)
                                    {
                                        user.rem_book++;
										fwrite(&user, sizeof(user), 1,f2);
                                       
                                    }
                                    else
                                    {    
										fwrite(&user, sizeof(user), 1, f2);
                                        
                                    }
                                    if(feof(temp2))
                                        break;
                                }
                            }
                            fclose(temp2);
                            fclose(f2);
                           

                            f1=fopen("f1.txt","w");
                            if((temp1=fopen("record.txt","r"))==NULL)
                                printf(" File is empty.\n\n");
                           else
                            {
                                while(!feof(temp1))
                                {   
                                      fscanf(temp1,"%d %s %s %d", &book.bid,book.bname,book.author,&book.copies);
                                     
                                    if(feof(temp1))
                                        break;
                                    if(strcmp(retbookname,book.bname)!=0)
                                    {   
                                        fprintf(f1,"\n%d\t%s\t%s\t%d    \t",book.bid,book.bname,book.author,book.copies);
                                        
                                    }
                                    else
                                    {  book.copies++;
                                        fprintf(f1,"\n%d\t%s\t%s\t%d\t",book.bid,book.bname,book.author,book.copies);
                                        
                                    }
                   
                                }
                                
                                
                              
                                  temp3=fopen("tempst.txt","a+");
                                  st=fopen("status.txt","r");
                                
                                
                                while(!feof(st))
                                { fscanf(st,"%d %s",&book.stat.uID,book.stat.bname);
                                  
                                  if(feof(st))
                                  		break;
                                  if(!(strcmp(retbookname,book.stat.bname)==0 && book.stat.uID==userID))
                                  { fprintf(temp3,"\n%d\t%s         \t",book.stat.uID,book.stat.bname);
                                   }
                                 }
                                 
                                 fclose(st);
                                 fclose(temp3);
                                 remove("status.txt");
                                 rename("tempst.txt","status.txt");
                                    
                                
                            }
                            fclose(temp1);
                            fclose(f1);
                            fclose(record);
                            fclose(User_Records);
                           
                           
                            remove("record.txt");
                            rename("f1.txt","record.txt");
                            remove("User_Records");
                            rename("f2","User_Records");
                            
   
                            
                            
                            int res=0;
                            while(!res)
                            { blue();
                              printf("\n Enter Return Date (dd/mm/yyyy):  ");
                              reset();
                              scanf("%d/%d/%d",&returnDate->dd,&returnDate->mm,&returnDate->yyyy);
                              res=Date_checker(returnDate);
                              }
                                
                          temp1=fopen("dates.txt","r");
                          int dayDiff,fine_amount;
                          while(!feof(temp1))
                          { fscanf(temp1,"%s %d %d/%d/%d %d/%d/%d",issuebookname,&midb,&issueDate->dd,&issueDate->mm,&issueDate->yyyy,&dueDate->dd,&dueDate->mm,&dueDate->yyyy);
                            if(strcmp(issuebookname,retbookname)==0 && midb==userID)
                           { dayDiff=getDifference(dueDate,returnDate);
                             if(dayDiff>0)
							 {  fine_amount=calcFine(dayDiff);
							    red();
                                printf("\n---- Fine to be paid = Rs. %d ----\n",fine_amount);
                                reset();
                             }
							 else 
								 break;
							}
                          
                          
                            }
                          green();
                          printf("\n----BOOK RETURNED----\n"); 	  
                          reset();  

                        }               
                    }
                    else if(!Found2)
                    {   red();
                        printf("\t\tNo such book exists.\n");
                        reset();
                       }
                }
            }
         }
        else if(!Found1)
        	printf("\t\tInvalid User ID!\n");
       

    }
   
}
    
	
#ifdef __MINGW32__

//windows

//FUNC TO READ PASSWORD AND DISPLAY AS ****
void getPassword(char *pass)
{
    int c=0;
    char buff[30]={0},ch;
    int len=0;
    while((ch=getch())!='\r')
    {
        if(ch==0x08)    // use 0x08 in windows
        {
            if(len==0)  continue;
            printf("\b \b"); len--; continue;
        }
        printf("%c",'*');
        pass[len]=ch;
        len++;
    }
    pass[len]='\0';
 
}

//FUNC TO CHECK ADMIN LOGIN CREDENTIALS
int authenticate()
{ int id,tries=0;char password_in[20];
  printf(" Enter admin ID:\n");
  scanf("%d",&id);
  if(id==main_admin_id)
  { printf(" Enter password:\n");
    fflush(stdin);
    getPassword(password_in);
    if(!strcmp(password_in,main_admin_pass))
    	{printf("\n\t\t\t\tLOGGED IN AS MAIN ADMIN\n");
        return 1;}
     else
     {	while(strcmp(password_in,main_admin_pass)&&tries<=3)
     	{ printf("\n\t\tINCORRECT PASSWORD, PLEASE TRY AGAIN\n");
     	  fflush(stdin);
     	  getPassword(password_in);
     	  tries++;
     	  }
       if(tries>=3)
       	{	printf("\t\tLOGIN FAILED.\n");
       		return 0;
       		}
       else
       	return 1;
      }
      }
   else
   { printf("\t\tNOT AN ADMIN. ACCESS DENIED\n");
     return 0;
   }
 }



#elif __unix__


//Ubuntu

void getPassword(char *pass)
{
    int c=0;
    char buff[30]={0},ch;
    int len=0;
    while((ch=getch())!='\n')
    {
        if(ch==0x7f)    // Backspace ASCII value
        {
            if(len==0)  continue;
            printf("\b \b"); len--; continue;
        }
        printf("%c",'*');
        pass[len]=ch;
        len++;
    }
    pass[len]='\0';
 
}




int authenticate()
{ 
  int id,tries=0;char password_in[20];
  printf(" Enter admin ID:\n");
  scanf("%d",&id);
  if(id==main_admin_id)
  { printf(" Enter password:\n");
    __fpurge(stdin);
    getPassword(password_in);
    if(!strcmp(password_in,main_admin_pass))
    	{ green();
    	printf("\n\t\t\t\tLOGGED IN AS MAIN ADMIN\n");
    	reset();
        return 1;}
     else
     {	while(strcmp(password_in,main_admin_pass)&&tries<=3)
     	{ yellow();  
     	 printf("\a");
     	 printf("\n\t\tINCORRECT PASSWORD. PLEASE TRY AGAIN\n");
     	  __fpurge(stdin);
     	  getPassword(password_in);
     	  tries++;
     	  reset();
     	  }
       if(tries>=3)
       	{	red();
       	    printf("\t\tLOGIN FAILED.\n");
       	    reset();
       		return 0;
       		}
       else
       	return 1;
      }
      }
   else
   { 
     red();
     printf("\t\tNOT AN ADMIN. ACCESS DENIED\n");
     return 0;
   }
 }


#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

 
       	
void main_screen()
{ 
     blue();
     printf("\t\t\t\t\t========================================================================================================\t\t\t\t\t\n"); 
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     green();
     printf("\t\t\t\t\t|\t\t\t\t~~~~~~ LIBRARY MANAGEMENT SYSTEM ~~~~~~\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     red();
     printf("\t\t\t\t\t|\t\t\t\t ------ SUBMITTED BY TEAM 1, L SECTION ------\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\n\n\n\t\t\t\t\t|\t\t\t\t *** RAMYA NARASIMHA PRABHU  Roll.No 1  PES1UG19CS380 \t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\n\t\t\t\t\t|\t\t\t\t *** CHAKITA MUTTARAJU  Roll.No 2  PES1UG19CS120 \t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\n\t\t\t\t\t|\t\t\t\t *** SHEETAL S  Roll.No 44  PES1UG19CS455 \t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     blue();
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t |\n");
     printf("\t\t\t\t\t========================================================================================================\t\t\t\t\t\n");   
     reset();
     sleep;
     system("clear");
}


//ADMIN MENU
void admin_menu()
{
	int choice,emp;
	char ch;
	blue();	
	printf("\n\n\t\t\t ADMINISTRATOR MENU \n");
	yellow();
	printf("\n\t\t 01. ADD USER");
	yellow();
    printf("\n\t\t 02. DELETE USER");
    yellow();
    printf("\n\t\t 03. UPDATE USER");
    yellow();
    printf("\n\t\t 04. SEARCH USER");
    yellow();
	printf("\n\t\t 05. DISPLAY ALL USERS");
	green();
	printf("\n\t\t 06. ADD BOOK");
	green();
	printf("\n\t\t 07. DELETE BOOK");
	green();
	printf("\n\t\t 08. UPDATE BOOK");
	green();
    printf("\n\t\t 09. SEARCH BOOK");
    green();
	printf("\n\t\t 10. DISPLAY ALL BOOKS");
	red();
	printf("\n\t\t 11. Back to Main Menu\n");
	reset();		
	printf("\n Enter your choice: ");
	scanf("%d",&choice);
			
	switch (choice)
	{   
			   case 1: insert_user();
			           sleep;
			           clear;
					   break;
			   case 2: delete_user();
			           sleep;
			           clear;
					   break;
			   case 3: update_user();
			           sleep;
			           clear;
					   break;
			   case 4: search_user();
			           sleep;
			           clear;
					   break;
			   case 5: emp = empty();
					   if (emp == 0)
							printf("\nThe file is empty\n");
					   else
							disp_users();
							sleep;
					   break;
			   case 6:Addbook();
			          sleep;
			          clear;
					   break;
			   case 7: deleteBook();
			           sleep;
			           clear;
			           break;
			   case 8: updateBook();
			           sleep;
			           clear;
					   break;
			   case 9: printf("\n Search by\n 1)Name\n 2)Author\n");
					   printf("\n Enter your choice: ");
					   flush;
					   scanf("%c",&ch);
					   if(ch=='1')
						   Searchbook();
					   else if(ch=='2')
						   Author();
					   else
						   printf("\n\n Invalid Option!\n");
			           sleep;
			           clear;
					   break;
			   case 10: Displaybook();
						sleep;
					   break;
					   
			   case 11: return;
			   
			   default: red();
			            printf("Invalid option!\n");
					    reset();
	}
			
			admin_menu();
			
}

