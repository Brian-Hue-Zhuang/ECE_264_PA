#include <math.h>

#include "answer02.h"

// Given intg_arg, this function performs numerical integration of 
// the function intg_arg.func_to_be_integrated over the range
// intg_arg.lower_limit to intg_arg.upper_limit
// 
// The bounded interval is divided into intg_arg.n_intervals contiguous
// uniform intervals
// 
// The Simpson's rule is used to perform the integration for each interval
//
// The integral is the sum of the integration over all intervals
//
// The caller function has to make sure that intg_arg.n_intervals >= 1
// Therefore, this function may assume that intg_arg.n_intervals >= 1
//
double simpson_numerical_integration(integrand intg_arg)
{
   double integral = 0.0;
   double b;
   double a;
   int n_int;
   double step_size;
   int i;
   b = intg_arg.upper_limit;
   a = intg_arg.lower_limit;
   n_int = intg_arg.n_intervals;
   step_size = (b - a) / n_int;
   for(i = 0; i < n_int; i++)
   {
      double b_temp = a + step_size;
      double m = (b_temp + a) / 2;
      double intg = (intg_arg.func_to_be_integrated(a) + 4 * intg_arg.func_to_be_integrated(m) + intg_arg.func_to_be_integrated(b_temp));
      integral += ((b_temp - a) / 6) * intg;
      a += step_size;
   }
   return integral;
}

// Given intg_arg, this function performs numerical integration over the
// range of intg_arg.lower_limit to intg_arg.upper_limit of f(x):
// 
// f(x) = intg_arg.func_to_be_integrated(x) * cos_sin((n * M_PI * x)/L), 
//
// where 2L = intg_arg.upper_limit - intg_arg.lower_limit = period 
// 
// The bounded interval is divided into intg_arg.n_intervals contiguous
// uniform intervals
// 
// The Simpson's rule is used to perform the integration for each interval
//
// The integral is the sum of the integration over all intervals
//
// The caller function has to make sure that intg_arg.n_intervals >= 1
// Therefore, this function may assume that intg_arg.n_intervals >= 1
//
// The caller function should also pass in n >= 0
//
// The caller function should also pass in cos or sin for the function 
// cos_sin.
//
double simpson_numerical_integration_for_fourier(integrand intg_arg, int n, 
                                                 double (*cos_sin)(double))
{
   double integral = 0.0;
   double b = intg_arg.upper_limit;;
   double a = intg_arg.lower_limit;;
   double L = (b - a);
   int n_int = intg_arg.n_intervals;
   int i;
   double step_size = (b - a) / n_int; 
   
   for(i = 0; i < n_int; i++)
   {
      double b_temp = a + step_size;
      double m = (b_temp + a) / 2;
      double intg = intg_arg.func_to_be_integrated(a) * cos_sin((n * M_PI * a) / L) + 4 * (intg_arg.func_to_be_integrated(m) * cos_sin((n * M_PI * m) / L)) + intg_arg.func_to_be_integrated(b) * cos_sin((n * M_PI * b) / L);
      integral += ((b_temp - a) / 6) * intg;
      a += step_size;
   }
   return integral;
}

// Given fourier_arg, this function computes 
// Fourier coefficients a_0, a_1, ..., a_{fourier_arg.n_terms-1} and stores
// them as fourier_arg.a_i[0], fourier_arg.a_i[1], and so on, and 
// b_1, ..., b_{fourier_arg.n_terms-1} and stores them as fourier_arg.b_i[1],
// fourier_arg.b_i[2], and so on.
//
// The period is defined to be 
//    fourier_arg.intg.upper_limit - fourier_arg.intg.lower_limit
// 
// The function simpson_numerical_integration is used in the process of 
// computing a_0.  fourier_arg.intg should be passed to the function.
//  
// The function simpson_numerical_integration_for_fourier is used in the 
// process of computing a_1, ... and b_1, ...
// fourier_arg.intg should be passed, appropriate n >= 0, and either sin or
// cos function should also be passed.
//
// The caller function should pass into this function fourier_arg.n_terms >= 1
//
// The caller function should also allocate space to store the coefficients
// a_0, a_1, ..., b_1, ...
// 
// The caller function should ensure that the period is not 0.
//
void compute_fourier_coefficients(fourier fourier_arg) 
{
   int n = fourier_arg.n_terms;
   int i;
   int period;
   if (fourier_arg.intg.upper_limit > fourier_arg.intg.lower_limit)
   {
      period = fourier_arg.intg.upper_limit - fourier_arg.intg.lower_limit;
   }
   else
   {
      period = fourier_arg.intg.lower_limit - fourier_arg.intg.upper_limit;
   }
   double L = period / 2;
   fourier_arg.a_i[0] = (1 / L) * simpson_numerical_integration(fourier_arg.intg);
   
   for(i = 1; i <= n; i++)
   {
      fourier_arg.a_i[i] = (1 / L) * simpson_numerical_integration_for_fourier(fourier_arg.intg, i, cos);
      fourier_arg.b_i[i] = (1 / L) * simpson_numerical_integration_for_fourier(fourier_arg.intg, i, sin);
   }
   return;
}