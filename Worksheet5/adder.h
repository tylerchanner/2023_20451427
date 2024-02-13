// adder.h

#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

#if defined(WIN32) | defined(_WIN32)
#ifdef maths_STATIC
#define MATHSLIB_API
#else
#ifdef maths_EXPORTS
#define MATHSLIB_API __declspec(dllexport)
#else
#define MATHSLIB_API __declspec(dllimport)
#endif	
#endif
#else
#define MATHSLIB_API
#endif

/** @file
 * This file contains the declarations of all exported functions in the mathslib library.
 */

 /**
  * @brief Adds two integers.
  *
  * This function takes two integers as parameters and returns their sum. It is part of the mathslib library
  * and can be compiled into a static or dynamic library.
  *
  * @param a The first integer to add.
  * @param b The second integer to add.
  * @return The sum of a and b.
  */
MATHSLIB_API int add(int a, int b);

#endif // MATHSLIB_ADDER_H
