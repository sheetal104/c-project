



#include<stdio.h>
#include"interface.h"










int main()
{  
  int ch,auth;

  main_screen();
  
  do
  { reset();
	printf("\n\n\n\t\t\t\tMAIN MENU");
	blue();
	printf("\n\n\t\t\t 1. BOOK ISSUE");
	green();
	printf("\n\n\t\t\t 2. BOOK DEPOSIT");
	yellow();
	printf("\n\n\t\t\t 3. ADMINISTRATOR MENU");
	red();
	printf("\n\n\t\t\t 4. EXIT");
	reset();
	printf("\n\n\t Please Select Your Option (1-4):  ");
	scanf("%d",&ch);
	printf("\n");
   
	switch(ch)
	{
		case 1: Issue();
		        sleep;
		        clear;
				break;
		case 2:	bookret();
			    break;
		case 3: auth= authenticate();
				if(auth)
						admin_menu(); 
				else
						printf("\t\t\tAUTHENTICATION FAILED!\n");
				sleep;
				clear;
				break;
	    case 4:	exit(1);
		        break;
				
		default : red();
		          printf("\n\t\t\t Invalid choice!\n");
		          reset();
				  break;
				 
	}
  }while(ch!=4);
}


