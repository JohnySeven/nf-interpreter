//
// Copyright (c) 2017 The nanoFramework project contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "stdafx.h"
#include "nanoPAL_NativeDouble.h"

#include <math.h>

#ifdef WIN32
#include <float.h>

#define __isnand        _isnan
#define __isinfd(x)    (!_finite(x))

inline int __signbitd(double x)
{
  unsigned char *ptr = (unsigned char*) &x;
  return (*(ptr + (sizeof(double) - 1)) & 0x80) != 0;
}

#define rint(x)         floor((x) + 0.5) 
#define remainder(x,y) ((x) - ((y) * rint((x) / (y))))

#define isgreater(param0,param1) (param0 > param1)
#define isless(param0,param1)    (param0 < param1)

#elif __GNUC__ && !defined( isgreater ) // newer versions include this macro already in math.h

#define isgreater(param0,param1) (param0 > param1)
#define isless(param0,param1)    (param0 < param1)

#elif defined(arm) || defined(__arm)

#define __isnand   __ARM_isnan
#define __isinfd   __ARM_isinf
#define __signbitd __ARM_signbit

#endif



using namespace System;
// Summary:
//     Compares this instance to a specified double-precision floating-point number
//     and returns an integer that indicates whether the value of this instance
//     is less than, equal to, or greater than the value of the specified double-precision
//     floating-point number.
//
// Parameters:
//   value:
//     A double-precision floating-point number to compare.
//
// Returns:
//     A signed number indicating the relative values of this instance and value.
//     Return Value Description 
//     Less than zero
//         This instance is less than value. -or- 
//         This instance is not a number (System.Double.NaN) and value is a number. 
//     Zero
//         This instance is equal to value. -or- 
//         Both this instance and value are not a number (System.Double.NaN), 
//         System.Double.PositiveInfinity, or System.Double.NegativeInfinity.
//     Greater than zero 
//         This instance is greater than value. -or- 
//         This instance is a number and value is not a number (System.Double.NaN).
int32_t Double::CompareTo( double d, double val )
{

    if (__isnand(d))
    {
       // Instance is not a number
       if (__isnand(val)) 
          return 0; // value is not a number
       else 
          return -1; //value is a number
    }
    else
    {
      // Instance is a number
      if (__isnand(val)) 
        // value is not a number
        return 1; 
    }
    if ((__signbitd(d) == __signbitd(val)) && (__isinfd(d) == __isinfd(val)))
      // either both are Positive or both are Negative Infinity
      return 0;
    if (isgreater(d,val))
      // this instance is greater than value
      return 1;
    if (isless(d,val))
      // this instance is less than value
      return -1;
      
    // assume values are equal as all other options have been checked.
    return 0;
}

//
// Summary:
//     Returns a value indicating whether the specified number evaluates to negative
//     or positive infinity
//
// Parameters:
//   d:
//     A double-precision floating-point number.
//
// Returns:
//     true if d evaluates to System.Double.PositiveInfinity or System.Double.NegativeInfinity;
//     otherwise, false.
bool Double::IsInfinity( double d )
{
    if(__isnand(d)) 
        return false;

    /* Return 1 if __x is infinite, 0 otherwise */
    /* Used by isinf macro */  
    bool retVal = __isinfd(d); 
    return retVal;
}

//
// Summary:
//     Returns a value indicating whether the specified number evaluates to a value
//     that is not a number (System.Double.NaN).
//
// Parameters:
//   d:
//     A double-precision floating-point number.
//
// Returns:
//     true if d evaluates to System.Double.NaN; otherwise, false.
bool Double::IsNaN( double d )
{
    /* Return 1 if __x is a NaN, 0 otherwise */
    /* Used by isnan macro */
    bool retVal = __isnand(d) != 0; 
    return retVal;
}

//
// Summary:
//     Returns a value indicating whether the specified number evaluates to negative
//     infinity.
//
// Parameters:
//   d:
//     A double-precision floating-point number.
//
// Returns:
//     true if d evaluates to System.Double.NegativeInfinity; otherwise, false.
bool Double::IsNegativeInfinity( double d )
{
    if(__isnand(d) != 0) 
        return false;

    /* Return signbit of __x */
    /* Used by signbit macro */
    int8_t signBit = __signbitd(d);
    
    /* Return 1 if __x is infinite, 0 otherwise */
    /* Used by isinf macro */
    int8_t infiniteBit = __isinfd(d) != 0;
    
    bool retVal = (signBit != 0) && (infiniteBit != 0); 
    return retVal;
}
//
// Summary:
//     Returns a value indicating whether the specified number evaluates to positive
//     infinity.
//
// Parameters:
//   d:
//     A double-precision floating-point number.
//
// Returns:
//     true if d evaluates to System.Double.PositiveInfinity; otherwise, false.
bool Double::IsPositiveInfinity( double d )
{
    if(__isnand(d) != 0) 
        return false;

    /* Return signbit of __x */
    /* Used by signbit macro */
    int8_t signBit = __signbitd(d);
    
    /* Return 1 if __x is infinite, 0 otherwise */
    /* Used by isinf macro */
    int8_t infiniteBit = __isinfd(d);
    
    bool retVal = (signBit == 0) && (infiniteBit != 0);  
    return retVal;
}
