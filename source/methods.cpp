#include "libraries/tinyexpr/tinyexpr.h" // To evaluate locally math expressions.
#include "util.hpp"

// NOTE: Method Math.
// Return the value of an expression, it will run locally and remotely at the same time and returns the first value it encounters.
double math( const char * & expression )
{

  #define NaN (0.0/0.0)

  // Try to calculate locally first the given expression.
  {
    const double value = te_interp(expression, 0);

    // If it's not NaN, return it.
    if (value != NaN) return value;

  }

  // Try to calculate remotely.
  // {
  //
  // }

  return NaN;

}
