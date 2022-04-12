#pragma once

#include "InputController.h"
#include "Equations.h"

struct discontinuityFixer {
	bool isFixed;
	double value;
};

class DefinitiveIntegralCalculator {
public:
	void calculate(int);
	void selectEquationThenCalculate();
private:
	bool checkForDiscontinuity(double);
	struct discontinuityFixer fixDiscontinuity(double, double, double, int, int);
	void printResult(bool, double);
	void printDiscontinuityInfo(bool, double, double, double);
	InputController inputController;
};

void DefinitiveIntegralCalculator::selectEquationThenCalculate() {
	int equationId = chooseEquation();
	calculate(equationId);
}

void DefinitiveIntegralCalculator::calculate(int equationId) {
	double lowerLimit = inputController.takeLowerLimit();
	double upperLimit = inputController.takeUpperLimit();
	int intervalsAmount = inputController.takeIntervalsAmount();

	double stepSize = (upperLimit - lowerLimit) / intervalsAmount;
	const double epsilon = stepSize / 2;

	double lowerLimitValue = useEquationById(lowerLimit, equationId);
	double upperLimitValue = useEquationById(upperLimit, equationId);

	bool discontinuityFixed = true;

	if (checkForDiscontinuity(lowerLimitValue)) {
		discontinuityFixer fixed;
		fixed = fixDiscontinuity(lowerLimit, stepSize, epsilon, 0, equationId);
		discontinuityFixed = fixed.isFixed;
		if (discontinuityFixed) {
			lowerLimitValue = fixed.value;
		}
		printDiscontinuityInfo(discontinuityFixed, lowerLimit, lowerLimitValue, epsilon);
	}
	if (checkForDiscontinuity(upperLimitValue) && discontinuityFixed) {
		discontinuityFixer fixed;
		fixed = fixDiscontinuity(lowerLimit, stepSize, epsilon, intervalsAmount, equationId);
		discontinuityFixed = fixed.isFixed;
		if (discontinuityFixed) {
			upperLimitValue = fixed.value;
		}
		printDiscontinuityInfo(discontinuityFixed, upperLimit, upperLimitValue, epsilon);
	}

	double integrationResult = lowerLimitValue + upperLimitValue;
	
	for (int i = 1; i < intervalsAmount; i++) {
		if (!discontinuityFixed) {
			break;
		}

		double equationValueInStep = useEquationById(lowerLimit + i * stepSize, equationId);
		
		if (checkForDiscontinuity(equationValueInStep)) {
			discontinuityFixer fixed;
			fixed = fixDiscontinuity(lowerLimit, stepSize, epsilon, i, equationId);
			if (fixed.isFixed) {
				equationValueInStep = fixed.value;
			}
			else {
				discontinuityFixed = fixed.isFixed; // false
			}

			printDiscontinuityInfo(discontinuityFixed, lowerLimit + i * stepSize, equationValueInStep, epsilon);
		}

		if (i % 2 == 0) {
			integrationResult += 2 * equationValueInStep;
		}
		else {
			integrationResult += 4 * equationValueInStep;
		}
	}

	integrationResult = integrationResult * (stepSize / 3);

	printResult(discontinuityFixed, integrationResult); // if gap is not fixed, result does not matter
}

bool DefinitiveIntegralCalculator::checkForDiscontinuity(double value) {
	if (std::isinf(value) || std::isnan(value)) {
		return true;
	}
	else {
		return false;
	}
}

discontinuityFixer DefinitiveIntegralCalculator::fixDiscontinuity(double lowerLimit, double stepSize, double epsilon, int i, int equationId) {
	double resultValue;
	double left = useEquationById(lowerLimit + (i - epsilon) * stepSize, equationId);
	double right = useEquationById(lowerLimit + (i + epsilon) * stepSize, equationId);
	if (!checkForDiscontinuity(left) && !checkForDiscontinuity(right)) {
		resultValue = (left + right) / 2;
		return { true, resultValue };
	}
	else {
		return { false, 0 };
	}
}

void DefinitiveIntegralCalculator::printResult(bool allDiscontinuityResolved, double result) {
	if (allDiscontinuityResolved) {
		std::cout << "The definitive integral is " << result << std::endl;
	}
	else {
		std::cout << "Definitive integral cannot be found because function is not defined somewhere from lower to upper limit." << std::endl;
	}
}

void DefinitiveIntegralCalculator::printDiscontinuityInfo(bool isFixed, double atX, double newValue, double epsilon) {
	if (isFixed) {
		std::cout << "Discontinuity encountered at x = " << atX << std::endl;
		std::cout << "Fixing it by taking an average between x1 = " << atX - epsilon << " and x2 = " << atX + epsilon << std::endl;
		std::cout << "New value to fix the discontinuity = " << newValue << std::endl;
	}
	else {
		std::cout << "Function value does not exist at x = " << atX << std::endl;

	}
}