//HEADER FILES

#include<ctype.h>
#include<string.h>
#include<stdlib.h>


#ifdef __MINGW32__
	#include <conio.h>
	#include<stdio.h>
	#include<windows.h>
	#define clear system("cls")
	#define sleep Sleep(3000)
	#define flush fflush(stdin)
#elif __unix__
	#include<termios.h>
	#include<stdio_ext.h>
	#include <unistd.h>
	#define clear system("clear")
	#define sleep sleep(3)
    #define flush __fpurge(stdin)
#endif



#pragma pack(2)


#define main_admin_id 1234
#define main_admin_pass "default"



//FILE POINTERS USED

typedef struct
{  
	int uID;
	char bname[25];
	int bin;
} Status;

   
typedef struct
{ int bid;
  char bname[100];
  char author[40];
  int copies;
  Status stat;	
  
  }Book;


typedef struct
{ 
  int dd;
  int mm;
  int yyyy;
  int dmax;
  
}Date;


typedef struct 
{
  int userID;
  char name[25] ;
  char dept[25];
  int rem_book;
}User;






//FUNCTIONS USED:

void red () ;
void yellow();
void green();
void blue();
void reset();
void resetTermios(void);
void initTermios(int);
char getch_(int echo);
char getch(void);
char getche(void);
int avluserID(int uid);
void insert_user();
void disp_users();
int empty();
void search_user();
void delete_user();
void update_user();
void displayDate(Date *d);
int Date_checker(Date *d) ;
int dmax_calc(int mm,int yyyy);
void due_date_calc(Date *issue,Date *due);
int countLeapYears(Date *d) ;
int getDifference(Date *dt1, Date *dt2) ;
int calcFine(int days);
int checkBookID(int bid);
void Addbook();
void Author();
void Searchbook();
void Displaybook();
void deleteBook();
void updateBook();
void Dateinfo();
void Issue();
void bookret();
void getPassword(char *pass);
int authenticate();
void main_screen();
void admin_menu();
