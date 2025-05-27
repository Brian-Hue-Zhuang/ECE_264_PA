#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "answer01.h"

// a sample test function to call test_my_strtol
//
static void test_my_strtol()
{
   // these are for my_strtol
   char *my_endptr;  
   int my_errno;

   // these are for c strtol
   char *c_endptr;   
   int c_errno;

   int base; 

   // test for a range of base, including an invalid bases 1 and 37
   // here are some suggestions for additional test, exceeding LONG_MAX, or 
   // going below LONG_MIN, and many others

   for (base = 2; base < 3; base++) {

      // before you call the function str_to_long_int
      // set errno to zero

      my_endptr = NULL;
      errno = 0;
      long int test = my_strtol("1111111222", &my_endptr, base);
      my_errno = errno;

      // print errno
      // typically, you want to check errno when the returned value
      // is LONG_MAX or LONG_MIN in a more useful program

      fprintf(stderr, "%ld:%p,%s,%d\n", test, my_endptr, my_endptr, my_errno);

      // call strtol to cross check the results 
      // note that the results should not match when base == 16.
      // before you call the function strtol
      // set errno to zero

      c_endptr = NULL;
      errno = 0;
      test = strtol("1111111222", &c_endptr, base);
      c_errno = errno;

      fprintf(stderr, "%ld:%p,%s,%d\n", test, c_endptr, c_endptr, c_errno);
   }
}

int main(int argc, char **argv)
{
   fprintf(stderr, "Welcome to ECE264, we are working on PA01.\n\n");

   // testing the test_my_strtol function
   // question:  how to do a comprehensive test

   test_my_strtol();

   return EXIT_SUCCESS;
}
