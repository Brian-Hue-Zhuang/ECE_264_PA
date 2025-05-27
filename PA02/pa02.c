#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <errno.h>

#include "answer02.h"
#include "aux02.h"
#include "utility02.h"

int main(int argc, char **argv)
{
   // check for correct number of arguments.  If not, exit and return 
   // exit_failure
   if(argc != 7)
   {
      return EXIT_FAILURE;
   }
   // now, try to parse the arguments and call the correct function
   char *endptr;
   errno = 0;
   double (*func_to_be_integrated) (double) = NULL;
   double up = strtod(argv[2], &endptr);
   if(isinf(up) || isnan(up) || *endptr != '\0' || errno != 0)
   {
      return EXIT_FAILURE;
   }
   double low = strtod(argv[3], &endptr);
   if( isinf(low) || isnan(low) || *endptr != '\0' || errno != 0)
   {
      return EXIT_FAILURE;
   }
   int n_int = strtod(argv[4], &endptr);
   if(*endptr != '\0' || errno != 0 || low == up)
   {
      return EXIT_FAILURE;
   }
   // fill in the correct statements to complete the main function
   // we expect six arguments:
   //
   // the first argument is 1 character from the sets {"1", "2", "3"}
   // it specifies the unknown function for which we are computing
   // the associated Fourier series
   // 1:  unknown_function_1
   // 2:  unknown_function_2
   // 3:  unknown_function_3
   // otherwise: return EXIT_FAILURE
   if(argv[1][1] == '\0')
   {
      switch(*argv[1])
      {
         case '1':
         func_to_be_integrated = unknown_function_1;
         break;  

         case '2':
         func_to_be_integrated = unknown_function_2;
         break;

         case '3':
         func_to_be_integrated = unknown_function_3;
         break;

         default:
         return EXIT_FAILURE;
         break;
      }
   }
   else
   {
      return EXIT_FAILURE;
   }
   if(n_int < 1 || n_int > INT_MAX)
   {
      return EXIT_FAILURE;
   }

   fourier fourier_arg;
   fourier_arg.intg.func_to_be_integrated = func_to_be_integrated;
   fourier_arg.intg.upper_limit = up;
   fourier_arg.intg.lower_limit = low;
   fourier_arg.intg.n_intervals = n_int;
   fourier_arg.n_terms = (int)strtod(argv[5], &endptr);  // you should change this, this is initialized
                             // so that the following statements are fine

   // keep the code below
   //
   // based on fourier_arg.nterms
   // allocate space for fourier_arg.a_i and fourier_arg.b_i;

   fourier_arg.a_i = malloc(sizeof(*(fourier_arg.a_i))*fourier_arg.n_terms);
   if (fourier_arg.a_i == NULL) {
      return EXIT_FAILURE;
   }
   fourier_arg.b_i = malloc(sizeof(*(fourier_arg.b_i))*fourier_arg.n_terms);
   if (fourier_arg.b_i == NULL) {
      free(fourier_arg.a_i);
      return EXIT_FAILURE;
   }

   // keep the code above

   // add code to compute Fourier coefficients and save them in a file
   compute_fourier_coefficients(fourier_arg);
   save_fourier_coefficients(fourier_arg.a_i, fourier_arg.b_i, fourier_arg.n_terms, argv[6]);

   // keep the code below

   free(fourier_arg.a_i);
   free(fourier_arg.b_i);

   // keep the code above

   return EXIT_SUCCESS;
} 