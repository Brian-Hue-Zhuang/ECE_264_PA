#include <stddef.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include "answer01.h"

#define INV_SYMBOL 36
// you should try to re-use a majority of the algorithms in PE02
// for this programming assignment.
// However, you have to take care of endptr, base 0, and base 16
// these are the three additional things you have to deal with in
// this assignment, as compared to the str_to_long_int function
// that you wrote for PE02.
//
// it is really IMPORTANT that you extensively try out the function
// strtol on your own.
//

int char_to_int(char c)
{
   if(isalnum(c))
   {
      int ret_Num = 0;
      // what if c is '0'--'9'
      if(isdigit(c))
      {
         ret_Num = (int)c - 48;
      }
      // what if c is 'a'--'z'
      else if(islower(c))
      {
         ret_Num = (int)c - 87;
      }
      // what if c is 'A'--'Z'
      else if(isupper(c))
      {
         ret_Num = (int)c - 55;
      } 
      return ret_Num;
   }
   else
   {  // otherwise
      return INV_SYMBOL;
   }

}

long int str_to_long_int(char const *nptr, int base, char **endptr)
{
   //no longer accounts for spaces or initial +/- that originates from pe02
   long int ret_value = 0;  // return value
                            // if no conversion happens, return 0 
      while(*nptr)
      {
         long int digValue = char_to_int(*nptr);
         if(digValue != INV_SYMBOL && digValue < base)
         {
            ret_value = digValue + base * ret_value;
         }
         else
         {
            *endptr = (char*) nptr;
            return ret_value;
         }
         nptr++;
      }
   return ret_value;
}

long int my_strtol(char const *nptr, char **endptr, int base)
{
   long int ret_value = 0;  // return value
   int sym = 1;
   // if invalid base, set errno and return immediately
   if(base < 0 || base == 1 || base > 36)
   { 
      errno = EINVAL;
      if(endptr != NULL)
      {
         *endptr = NULL;
      }
      return ret_value;
   }
   // valid base, skip over white space
   while(isspace(*nptr))
   {
      nptr++;
   }
   // if encounter a sign, expect a number after that
   // if a negative sign is encountered, change the sign
   if(*nptr == '-' || *nptr == '+')
   {
      if(*nptr == '-')
      {
         sym = -1;
      }
      nptr++;
   }
   
   switch(base)
   {
      case 0:
      if(*nptr == '0')
      {
         if(*(nptr + 1) == 'x' || *(nptr + 1) == 'X')
         {
            base = 16;
            nptr += 2;
         }
         else
         {
            base = 8;
            nptr++;
         }
      }
      else
      {
         base = 10;
      }
      break;

      case 16:
      if((*(nptr + 1) == 'x' || *(nptr + 1) == 'X'))
      {
         nptr += 2;
      }
      break;

      default:
      break;
   }

   ret_value = sym * str_to_long_int(nptr, base,endptr);
   // clean up, set ret_value properly if the conversion went out of range
   // set errno if necessary
   // set *endptr properly if the endptr is not NULL

   return ret_value;
}
