#pragma once

#include "InputController.h"
#include "Equations.h"

struct discontinuityFixer {
	bool isFixed;
	double value;
};

struct calculateResult {
	bool allDiscontinuitiesFixed;
	double value;
};

class DefinitiveIntegralCalculator {
public:
	calculateResult calculate(double, double, int, int, bool);
	void inputCalculateAndPrint();
	std::vector<double> getMarginsOfError(double, double, int, int);
private:
	void printMarginsOfError(double, double, int, int);
	bool checkForDiscontinuity(double);
	struct discontinuityFixer fixDiscontinuity(double, double, double, int, int);
	void printResult(bool, double);
	void printDiscontinuityInfo(bool, double, double, double);
	InputController inputController;
};

void DefinitiveIntegralCalculator::inputCalculateAndPrint() {
	int equationId = chooseEquation();
	double lowerLimit = inputController.takeLowerLimit();
	double upperLimit = inputController.takeUpperLimit();
	int intervalsAmount = inputController.takeIntervalsAmount();

	calculateResult result = calculate(lowerLimit, upperLimit, intervalsAmount, equationId, false);
	
	printResult(result.allDiscontinuitiesFixed, result.value); // if gap is not fixed, result does not matter

	if (result.allDiscontinuitiesFixed) {
		printMarginsOfError(lowerLimit, upperLimit, intervalsAmount, equationId);
	}
}

calculateResult DefinitiveIntegralCalculator::calculate(double lowerLimit, double upperLimit, int intervalsAmount, int equationId, bool onlyResultNeeded) {
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
		if (!onlyResultNeeded) {
			printDiscontinuityInfo(discontinuityFixed, lowerLimit, lowerLimitValue, epsilon);
		}
	}
	if (checkForDiscontinuity(upperLimitValue) && discontinuityFixed) {
		discontinuityFixer fixed;
		fixed = fixDiscontinuity(lowerLimit, stepSize, epsilon, intervalsAmount, equationId);
		discontinuityFixed = fixed.isFixed;
		if (discontinuityFixed) {
			upperLimitValue = fixed.value;
		}
		if (!onlyResultNeeded) {
			printDiscontinuityInfo(discontinuityFixed, upperLimit, upperLimitValue, epsilon);
		}
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
			
			if (!onlyResultNeeded) {
				printDiscontinuityInfo(discontinuityFixed, lowerLimit + i * stepSize, equationValueInStep, epsilon);
			}
		}

		if (i % 2 == 0) {
			integrationResult += 2 * equationValueInStep;
		}
		else {
			integrationResult += 4 * equationValueInStep;
		}
	}

	integrationResult = integrationResult * (stepSize / 3);

	return { discontinuityFixed, integrationResult };
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

std::vector<double> DefinitiveIntegralCalculator::getMarginsOfError(double lowerLimit, double upperLimit, int intervalsAmount, int equationId) {
	double k = use4thDerivativeOfEquationById(upperLimit, equationId);
	double error = (k * (upperLimit - lowerLimit)) / (180 * std::pow(intervalsAmount, 4));
	error = std::abs(error);

	const double constForSympsonsMethod = (double)1 / (double)15;
	double integral2N = calculate(lowerLimit, upperLimit, intervalsAmount, equationId, true).value;
	intervalsAmount = intervalsAmount / 2;
	if (intervalsAmount % 2 != 0) {
		intervalsAmount++;
	}
	double integralN = calculate(lowerLimit, upperLimit, intervalsAmount, equationId, true).value;

	double rungeError = constForSympsonsMethod * std::abs(integral2N - integralN);

	return { error, rungeError };
}

void DefinitiveIntegralCalculator::printMarginsOfError(double lowerLimit, double upperLimit, int intervalsAmount, int equationId) {
	std::vector<double> errors = getMarginsOfError(lowerLimit, upperLimit, intervalsAmount, equationId);

	std::cout << "Methods margin of error by a specific formula: " << errors.at(0) << std::endl;

	
	std::cout << "Methods margin of error by Runge rule: " << errors.at(1) << std::endl;
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