#include <iostream>
#include "DefinitiveIntegralCalculator.h"

int main()
{
    std::cout << "Program for approximating definite integral of a function." << std::endl;

    DefinitiveIntegralCalculator defIntegralCalculator;
    defIntegralCalculator.inputCalculateAndPrint();

    std::cout << "\n\n\n\n" << std::endl; // to move away visual studio console info, just for looks
}
