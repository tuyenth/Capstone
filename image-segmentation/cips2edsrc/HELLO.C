

#include <stdio.h>

main()
{
   char buffer[80];
   printf("\nhello out there"); 
   
   printf("\nEnter your name:");
   gets(buffer);
   printf("\n\nyou entered>>%s<<\n", buffer);  

   hello2();
   hello3();
   return(0);
}  
