
   /***********************************************
   *
   *  file locdiff.c
   *
   *  Functions: This file contains
   *     main
   *     is_a_loc
   *     read_until_finding
   *     convert_to_uppercase
   *
   *  Purpose:
   *     This file contains a program that will
   *     count the lines of text and lines of code
   *     in a difference file.  The difference file
   *     is generated by the DOS command fc.
   *
   *     The term lines of code must meet the 
   *     following requirements:
   *        1.  #include
   *        2.  #define
   *        3.  #undef
   *        4.  main
   *        5.  ;
   *        6.  switch
   *        7.  case
   *        8.  #ifdef
   *        9.  if
   *        10. else
   *        11. while
   *        12. for
   *        13. do
   *
   *  External Calls:
   *     none
   *
   *  Modifications:
   *     9 April 1995 - created
   *
   *************************************************/


#include "cips.h"
#include <ctype.h>

#define LENGTH 80
#define RULES  13

char rule[RULES][LENGTH] = {
               "#include",
               "#define",
               "#undef",
               "main",
               ";",
               "switch",
               "case",
               "#ifdef",
               "if",
               "else",
               "while",
               "for",
               "do"};


main(argc, argv)
   char *argv[];
   int  argc;
{
   char *place,
        src_file_1[LENGTH], 
        src_file_2[LENGTH], 
        string[LENGTH];

   FILE *diff_file;

   int  counting,
        lines_of_code = 0,
        lines_of_text = 0;

   if(argc != 4){
      printf(
      "\nusage: locdiff diff-file "
      "src-file-1 src-file-2");
      exit(1);
   }

   if((diff_file = fopen(argv[1], "rt")) == NULL){
      printf(
     "\nlocdiff: Error opening diff file %s\n", 
      argv[1]);
      exit(2);
   }

   strcpy(src_file_1, argv[2]);
   strcpy(src_file_2, argv[3]);

   convert_to_uppercase(src_file_1);
   convert_to_uppercase(src_file_2);

      /***********************************************
      *
      *   This is the major loop of the program.
      *   It reads one line at a time from the 
      *   diff file.
      *
      *   It first finds the src-file-2's name on 
      *   a line in upper case text.  Then it reads
      *   lines until it hits an occurence of the
      *   src-file-1 name.  While reading it 
      *   counts lines of text and lines of code.
      *
      ***********************************************/

   while(fgets(string, LENGTH, diff_file)){
      read_until_finding(src_file_2, diff_file);
      counting = 1;
      while(counting){
         if(fgets(string, LENGTH, diff_file) == '\0')
            counting = 0;
         place = strstr(string, src_file_1);
         if(place == '\0'){
            lines_of_text++;
            if(is_a_loc(string))
               lines_of_code++;
          }  /* ends if place != null */
          else
             counting = 0;
       }  /* ends while counting */
   }   /* ends the major loop */

   fclose(diff_file);

   printf("The difference file %s has:", argv[1]);
   printf("\n     %d lines of text", lines_of_text);
   printf("\n     %d lines of code", lines_of_code);
   return(111);
   
}  /* ends main */



      /***********************************************
      *
      *   is_a_loc(string)
      *   
      *   This function looks at the text string
      *   to see if it contains any of the lines of 
      *   code strings stored in the rule array.
      *
      ***********************************************/

int is_a_loc(string)
   char string[];
{
   char *place;
   int  i, result = 0;

   char response[LENGTH];

   for(i=0; i<RULES; i++){
     place = strstr(string, rule[i]);
     if(place != '\0'){
        result = 1;
        i      = RULES+1;
     }
   }

   return(result);

}  /* ends is_a_loc */



      /***********************************************
      *
      *   read_until_finding(target, diff_file)
      *   
      *   This function reads lines from an ASCII
      *   text file until it hits a line containing
      *   the target text string.
      *
      ***********************************************/

read_until_finding(target, diff_file)
   char target[];
   FILE *diff_file;
{
   int  not_found = 1;
   char *place, string[LENGTH];
  
   while(not_found){
      if(fgets(string, LENGTH, diff_file) == '\0')
         not_found = 0;
      place  = strstr(string, target);
      if(place != '\0')
         not_found = 0;
   }  /* ends while not_found */
}  /* ends read_until_finding */



      /***********************************************
      *
      *   convert_to_uppercase(string)
      *   
      *   This function converts all lower case
      *   characters in a string to uppercase.
      *
      ***********************************************/

convert_to_uppercase(string)
   char string[];
{
   char c;
   int  i;

   for(i=0; i<strlen(string); i++){
      c         = toupper(string[i]);
      string[i] = c;
   }  /* ends loop over i */
}  /* ends convert_to_uppercase */
