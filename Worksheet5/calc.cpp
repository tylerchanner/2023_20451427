#include <sstream>
#include <iostream>
#include "adder.h"

/**
 * @file calc.cpp
 *
 * @brief Simple calculator program using the mathslib library.
 *
 * This program uses the mathslib library to perform addition. It takes two command line arguments,
 * converts them to integers, adds them using the mathslib's add function, and outputs the result.
 */

 /**
  * Main function of the calculator program.
  * Takes two command line arguments, converts them to integers, adds them, and prints the result.
  *
  * @param argc Number of command line arguments.
  * @param argv Array of command line arguments.
  * @return Returns 0 if successful, 1 if the incorrect number of arguments are provided.
  */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <number1> <number2>" << std::endl;
        return 1;
    }

    int a, b, sum;
    std::stringstream sa(argv[1]);
    std::stringstream sb(argv[2]);

    sa >> a;
    sb >> b;

    sum = add(a, b);
    std::cout << "The sum is: " << sum << std::endl;
    return 0;
}
