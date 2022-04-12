#include <iostream>
#include "DefinitiveIntegralCalculator.h"

int main()
{
    std::cout << "Program for approximating definite integral of a function." << std::endl;

    DefinitiveIntegralCalculator defIntegralCalculator;
    defIntegralCalculator.selectEquationThenCalculate();
}
