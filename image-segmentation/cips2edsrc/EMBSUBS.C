

       /***********************************************
       *
       *       file c:\cips\embsubs.c
       *
       *       Functions: This file contains
       *          detect_edges
       *          setup_masks
       *          get_edge_options
       *          perform_convolution
       *          quick_edge
       *          fix_edges
       *
       *       Purpose:
       *          These functions implement several
       *          types of basic edge detection.
       *
       *       External Calls:
       *          wtiff.c - round_off_image_size
       *                    create_file_if_needed
       *                    write_array_into_tiff_image
       *          tiff.c - read_tiff_header
       *          rtiff.c - read_tiff_image
       *          numcvrt.c - get_integer
       *
       *
       *       Modifications:
       *          3 April 1994 - created
       *
       *************************************************/

#include "cips.h"






short emboss_0_mask[3][3] =  {
       {-1,  0,  1},
       {-1,  1,  1},
       {-1,  0,  1} };

short emboss_1_mask[3][3] =  {
       { 0,  1,  1},
       {-1,  1,  1},
       {-1, -1,  0} };

short emboss_2_mask[3][3] =  {
       { 1,  1,  1},
       { 0,  1,  0},
       { 1,  1, -1} };

short emboss_3_mask[3][3] =  {
       { 1,  1,  0},
       { 1,  1, -1},
       { 0, -1, -1} };

short emboss_4_mask[3][3] =  {
       { 1,  0, -1},
       { 1,  1, -1},
       { 1,  0, -1} };

short emboss_5_mask[3][3] =  {
       { 0, -1, -1},
       { 1,  1, -1},
       { 1,  1,  0} };

short emboss_6_mask[3][3] =  {
       {-1, -1, -1},
       { 0,  1,  0},
       { 1,  1,  1} };

short emboss_7_mask[3][3] =  {
       {-1, -1,  0},
       {-1,  1,  1},
       { 0,  1,  1} };



   /***************************
   *
   *   Directions for the masks
   *  3 2 1
   *  4 x 0
   *  5 6 7
   *
   ****************************/



  /**************************************************
  *
  *   emboss_array(...
  *
  *   This function detects edges in an area of one
  *   image and sends the result to another image
  *   on disk.  It reads the input image from disk,
  *   calls a convolution function, and then writes
  *   the result out to disk.  If needed, it
  *   allocates space on disk for the output image.
  *
  ***************************************************/



emboss_array(in_name, out_name, the_image, out_image,
             il, ie, ll, le, threshold,
             high, zero, one, two, three, four,
             five, six, seven)
   char   in_name[], out_name[];
   int    high, il, ie,
          ll, le, threshold;
   int    zero, one, two, three, four, five,
          six, seven;
   short  the_image[ROWS][COLS], out_image[ROWS][COLS];

{
   int    i, j, k, length, width;
   struct tiff_header_struct image_header;


   create_file_if_needed(in_name, out_name, out_image);

   read_tiff_image(in_name, the_image, il, ie, ll, le);

   read_tiff_header(in_name, &image_header);

   emboss_convolution(the_image, out_image,
                      threshold,
                      &image_header, high,
                      zero, one, two, three,
                      four, five, six, seven);

   fix_edges(out_image, 1);

   write_array_into_tiff_image(out_name, out_image,
                               il, ie, ll, le);
}  /* ends emboss_array */







     /**********************************************************
     *
     *   emoboss_convolution(...
     *
     *   This function performs convolution between the input
     *   image and 8 3x3 masks.  The result is placed in
     *   the out_image.
     *
     ********************************************************/

emboss_convolution(image, out_image,
                   threshold, high,
                   zero, one, two, three, four,
                   five, six, seven,
                   rows, cols, bitsperpixel)
   short **image,
         **out_image;
   int   high, threshold;
   int    zero, one, two, three, four, five,
          six, seven;
   long   rows, cols, bitsperpixel;
{

   int a, b, i, is_present, j, sum;

   short  max,
          min,
          new_hi,
          new_low;

   new_hi  = 250;
   new_low = 16;
   if(bitsperpixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   min = 0;
   max = 255;
   if(bitsperpixel == 4)
      max = 16;

     /* clear output image array */
   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = 0;

   printf("\n ");

   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%3d", i);
      for(j=1; j<cols-1; j++){


         /******************************
         *
         *  Look at all 8 directions 
         *  convolve only if the 
         *  direction is set
         *
         ******************************/

         /* 0 direction */
      if(zero){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum + image[i+a][j+b] *
                     emboss_0_mask[a+1][b+1];
            }
         }
            if(sum > max) sum = max;
            if(sum < 0)   sum = 0;
         if(sum > out_image[i][j])
            out_image[i][j]   = sum;
      }  /* ends if zero */


         /* 1 direction */
      if(one){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum + image[i+a][j+b] * 
                     emboss_1_mask[a+1][b+1];
            }
         }
            if(sum > max) sum = max;
            if(sum < 0)   sum = 0;
         if(sum > out_image[i][j])
            out_image[i][j]   = sum;
      }  /* ends if one */


         /* 2 direction */
      if(two){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum + image[i+a][j+b] * 
                     emboss_2_mask[a+1][b+1];
            }
         }
            if(sum > max) sum = max;
            if(sum < 0)   sum = 0;
         if(sum > out_image[i][j])
            out_image[i][j]   = sum;
      }  /* ends if two */


         /* 3 direction */
      if(three){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum + image[i+a][j+b] * 
                     emboss_3_mask[a+1][b+1];
            }
         }
            if(sum > max) sum = max;
            if(sum < 0)   sum = 0;
         if(sum > out_image[i][j])
            out_image[i][j]   = sum;
      }  /* ends if three */


         /* 4 direction */
      if(four){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum + image[i+a][j+b] * 
                     emboss_4_mask[a+1][b+1];
            }
         }
            if(sum > max) sum = max;
            if(sum < 0)   sum = 0;
         if(sum > out_image[i][j])
            out_image[i][j]   = sum;
      }  /* ends if four */


         /* 5 direction */
      if(five){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum + image[i+a][j+b] * 
                     emboss_5_mask[a+1][b+1];
            }
         }
            if(sum > max) sum = max;
            if(sum < 0)   sum = 0;
            /* Correction 12-27-92
               see file header for
               details. */
         if(sum > out_image[i][j])
            out_image[i][j]   = sum;
      }  /* ends if five */


         /* 6 direction */
      if(six){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum + image[i+a][j+b] * 
                     emboss_6_mask[a+1][b+1];
            }
         }
            if(sum > max) sum = max;
            if(sum < 0)   sum = 0;
         if(sum > out_image[i][j])
            out_image[i][j]   = sum;
      }  /* ends if six */


         /* 7 direction */
      if(seven){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum + image[i+a][j+b] * 
                     emboss_7_mask[a+1][b+1];
            }
         }
            if(sum > max) sum = max;
            if(sum < 0)   sum = 0;
         if(sum > out_image[i][j])
            out_image[i][j]   = sum;
      }  /* ends if seven */


      }  /* ends loop over j */
   }  /* ends loop over i */


     /* if desired, threshold the output image */
   if(threshold == 1){
       for(i=0; i<rows; i++){
          for(j=0; j<cols; j++){
             if(out_image[i][j] > high){
                  out_image[i][j] = new_hi;
             }
             else{
                  out_image[i][j] = new_low;
             }
          }
       }
   }  /* ends if threshold == 1 */

}  /* ends emboss_convolution */





#ifdef NEVER

     /*******************************************
     *
     *   quick_edge(...
     *
     *   This function finds edges by using
     *   a single 3x3 mask.
     *
     *******************************************/


quick_emboss(in_name, out_name, the_image, out_image,
           il, ie, ll, le, threshold, high)
   char   in_name[], out_name[];
   int    high, il, ie, ll, le, threshold;
   short  the_image[ROWS][COLS], out_image[ROWS][COLS];

{
   int    a, b, i, j, k,
          length, max, new_hi, new_low,
          sum, width;
   struct tiff_header_struct image_header;


   create_file_if_needed(in_name, out_name, out_image);

   read_tiff_header(in_name, &image_header);
   new_hi  = 250;
   new_low = 16;
   if(image_header.bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   max = 255;
   if(image_header.bits_per_pixel == 4)
      max = 16;


   read_tiff_image(in_name, the_image, il, ie, ll, le);

         /* Do convolution over image array */
   printf("\n");
   for(i=1; i<ROWS-1; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=1; j<COLS-1; j++){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum +
                     the_image[i+a][j+b] *
                     emboss_mask[a+1][b+1];
            }
         }
         if(sum < 0)   sum = 0;
         if(sum > max) sum = max;
         out_image[i][j]   = sum;

      }  /* ends loop over j */
   }  /* ends loop over i */

     /* if desired, threshold the output image */
   if(threshold == 1){
       for(i=0; i<ROWS; i++){
          for(j=0; j<COLS; j++){
             if(out_image[i][j] > high){
                  out_image[i][j] = new_hi;
             }
             else{
                  out_image[i][j] = new_low;
             }
          }
       }
   }  /* ends if threshold == 1 */

   fix_edges(out_image, 1);

   write_array_into_tiff_image(out_name, out_image,
                               il, ie, ll, le);

}  /* ends quick_edge */

#endif




#ifdef NEVER

   /***********************************************
    *
    *    setup_masks(...
    *
    *    This function copies the mask values defined
    *    at the top of this file into the mask
    *    arrays mask_0 through mask_7.
    *
    ***********************************************/



setup_masks(detect_type, mask_0, mask_1, mask_2, mask_3,
            mask_4, mask_5, mask_6, mask_7)
   int    detect_type;
   short  mask_0[3][3],
          mask_1[3][3],
          mask_2[3][3],
          mask_3[3][3],
          mask_4[3][3],
          mask_5[3][3],
          mask_6[3][3],
          mask_7[3][3];
{
   int i, j;

   if(detect_type == KIRSCH){
      for(i=0; i<3; i++){
        for(j=0; j<3; j++){
          mask_0[i][j] = kirsch_mask_0[i][j];
          mask_1[i][j] = kirsch_mask_1[i][j];
          mask_2[i][j] = kirsch_mask_2[i][j];
          mask_3[i][j] = kirsch_mask_3[i][j];
          mask_4[i][j] = kirsch_mask_4[i][j];
          mask_5[i][j] = kirsch_mask_5[i][j];
          mask_6[i][j] = kirsch_mask_6[i][j];
          mask_7[i][j] = kirsch_mask_7[i][j];
        }
      }
   }  /* ends if detect_type == KIRSCH */


   if(detect_type == PREWITT){
      for(i=0; i<3; i++){
        for(j=0; j<3; j++){
          mask_0[i][j] = prewitt_mask_0[i][j];
          mask_1[i][j] = prewitt_mask_1[i][j];
          mask_2[i][j] = prewitt_mask_2[i][j];
          mask_3[i][j] = prewitt_mask_3[i][j];
          mask_4[i][j] = prewitt_mask_4[i][j];
          mask_5[i][j] = prewitt_mask_5[i][j];
          mask_6[i][j] = prewitt_mask_6[i][j];
          mask_7[i][j] = prewitt_mask_7[i][j];
        }
      }
   }  /* ends if detect_type == PREWITT */


   if(detect_type == SOBEL){
      for(i=0; i<3; i++){
        for(j=0; j<3; j++){
          mask_0[i][j] = sobel_mask_0[i][j];
          mask_1[i][j] = sobel_mask_1[i][j];
          mask_2[i][j] = sobel_mask_2[i][j];
          mask_3[i][j] = sobel_mask_3[i][j];
          mask_4[i][j] = sobel_mask_4[i][j];
          mask_5[i][j] = sobel_mask_5[i][j];
          mask_6[i][j] = sobel_mask_6[i][j];
          mask_7[i][j] = sobel_mask_7[i][j];
        }
      }
   }  /* ends if detect_type == SOBEL */



}  /* ends setup_masks */

#endif

   /***********************************************
    *
    *    get_emboss_options(...
    *
    *    This function queries the user for the
    *    parameters need to perform edge
    *    detection.
    *
    ***********************************************/


get_emboss_options(detect_type, threshold, high, size)
    int *detect_type, *high, *size, *threshold;
{
    int not_finished, response;
    not_finished = 1;
    while(not_finished){

      printf("\nThe Edge Detector options are:\n");
      printf("\n\t1.  Type of edge detector is %d", *detect_type);
      printf("\n\t      (recall 1=Prewitt     2=Kirsch");
      printf("\n\t              3=Sobel       4=quick");
      printf("\n\t              5=homogeneity 6=difference");
      printf("\n\t              7=contrast    8=gaussian");
      printf("\n\t              10=range      11=variance");
      printf("\n\t2.  Threshold output is %d (0=off 1=on)", *threshold);
      printf("\n\t3.  High threshold is %d", *high);
      printf("\n\t4.  Size is %d (gaussian only)", *size);
      printf("\n\nEnter choice (0 = no change) _\b");


      get_integer(&response);

      if(response == 0){
        not_finished = 0;
      }


      if(response == 1){
        printf("\n\nEnter type of edge detector");
        printf("\n\t      (recall 1=Prewitt     2=Kirsch");
        printf("\n\t              3=Sobel       4=quick");
        printf("\n\t              5=homogeneity 6=difference");
        printf("\n\t              7=contrast    8=gaussian");
        printf("\n\t              10=range      11=variance");
        printf("\n  _\b");
        get_integer(detect_type);
      }

      if(response == 2){
        printf("\n\nEnter threshold output (0=off 1=on)");
        printf("\n  _\b");
        get_integer(threshold);
      }

      if(response == 3){
        printf("\n\nEnter high threshold");
        printf("\n  _\b");
        get_integer(high);
      }

      if(response == 4){
        printf("\n\nEnter size for gaussian (7 or 9)");
        printf("\n  _\b");
        get_integer(size);
      }
    }  /* ends while not_finished */

}  /* ends get_edge_options */
