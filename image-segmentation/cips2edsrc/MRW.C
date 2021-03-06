
       /*****************************************************
       *
       *       file c:\lsu\mrw.c
       *
       *       Functions: This file contains
       *           my_read
       *           my_write
       *
       *       Purpose: These two functions call the Turbo C
       *           functions _read and _write. All software
       *           will use my_read and my_write so that if
       *           the software is ported to another system
       *           that uses read and write changing the
       *           two functions in this file will take care
       *           of the move.
       *
       *       External Call:
       *           read
       *           _write
       *
       *       Modifications:
       *           10 June 1987 - created
       *
       *
       ************************************************************/

#include "cips.h"

my_read(file_descriptor, buffer, number_of_bytes)
   int  file_descriptor, number_of_bytes;
   char *buffer;
{
   int bytes_read;
   int read();
   bytes_read = read(file_descriptor, buffer, number_of_bytes);
   return(bytes_read);
}


my_write(file_descriptor, buffer, number_of_bytes)
   int  file_descriptor, number_of_bytes;
   char *buffer;
{
   int bytes_written;
   int write();
   bytes_written = write(file_descriptor, buffer, number_of_bytes);
   return(bytes_written);
}

   /*************************************************
   *
   *   my_clear_text_screen()
   *
   *   This calls Microsoft C functions to clear
   *   the text screen and set a blue background
   *   with gray text.
   *
   **************************************************/


my_clear_text_screen()
{
   _setvideomode(_TEXTC80);/* MSC 6.0 statements */
   _setbkcolor(1);
   _settextcolor(7);
   _clearscreen(_GCLEARSCREEN);
}  /* ends clear_text_screen */
