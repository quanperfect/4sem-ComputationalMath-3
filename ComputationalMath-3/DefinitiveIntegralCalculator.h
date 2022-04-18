#pragma once

#include "InputController.h"
#include "Equations.h"

#include <string>

struct discontinuity {
	double x;
	double xleft;
	double xright;
	double newValue;
	std::string typeName;
	int type;
	bool isFixable;
};

struct calculationResult {
	bool allDiscontinuitiesFixed;
	double value;
	std::vector<discontinuity> discontinuities;
};

class DefinitiveIntegralCalculator {
public:
	calculationResult calculate(double, double, int, int);
	void inputCalculateAndPrint();
	double getMarginOfError(double, double, int, int);
private:
	bool checkForNonRemovableDiscontinuities(double, double, int, std::vector<discontinuity>*);
	void printMarginsOfError(double, double, int, int);
	bool checkForDiscontinuity(double);
	struct discontinuity fixDiscontinuity(double, double, double, int, int);
	void printResult(calculationResult);
	void printDiscontinuityInfo(std::vector<discontinuity>);
	InputController inputController;
};

void DefinitiveIntegralCalculator::inputCalculateAndPrint() {
	int equationId = chooseEquation();
	double lowerLimit = inputController.takeLowerLimit();
	double upperLimit = inputController.takeUpperLimit();
	while (lowerLimit >= upperLimit) {
		std::cout << "Non-valid input. Lower limit must be < upper limit." << std::endl;
		lowerLimit = inputController.takeLowerLimit();
		upperLimit = inputController.takeUpperLimit();
	}
	int intervalsAmount = inputController.takeIntervalsAmount();

	calculationResult result = calculate(lowerLimit, upperLimit, intervalsAmount, equationId);
	std::cout << "--------------------------------------" << std::endl;
	if (!result.discontinuities.empty()) {
		printDiscontinuityInfo(result.discontinuities);
		std::cout << std::endl;
	}

	printResult(result);

	if (result.allDiscontinuitiesFixed) {
		printMarginsOfError(lowerLimit, upperLimit, intervalsAmount, equationId);
	}
}

calculationResult DefinitiveIntegralCalculator::calculate(double lowerLimit, double upperLimit, int intervalsAmount, int equationId) {
	calculationResult returnResult = { true, 0, std::vector<discontinuity>() };

	std::vector<discontinuity> discontinuities;
	if (checkForNonRemovableDiscontinuities(lowerLimit, upperLimit, equationId, &discontinuities)) {
		returnResult.allDiscontinuitiesFixed = false;
		returnResult.discontinuities = discontinuities;
		return returnResult;
	}
	
	double stepSize = (upperLimit - lowerLimit) / intervalsAmount;
	const double epsilon = 1 * std::pow(10, -2);

	double lowerLimitValue = useEquationById(lowerLimit, equationId);
	double upperLimitValue = useEquationById(upperLimit, equationId);

	bool discontinuityFixed = true;

	if (checkForDiscontinuity(lowerLimitValue)) {
		discontinuity newDiscontinuity;
		newDiscontinuity = fixDiscontinuity(lowerLimit, stepSize, epsilon, 0, equationId);
		discontinuityFixed = newDiscontinuity.isFixable;
		if (discontinuityFixed) {
			lowerLimitValue = newDiscontinuity.newValue;
		}

		returnResult.discontinuities.push_back(newDiscontinuity);
	}
	if (checkForDiscontinuity(upperLimitValue) && discontinuityFixed) {
		discontinuity newDiscontinuity;
		newDiscontinuity = fixDiscontinuity(lowerLimit, stepSize, epsilon, intervalsAmount, equationId);
		discontinuityFixed = newDiscontinuity.isFixable;
		if (discontinuityFixed) {
			upperLimitValue = newDiscontinuity.newValue;
		}

		returnResult.discontinuities.push_back(newDiscontinuity);
	}

	double integrationResult = lowerLimitValue + upperLimitValue;
	
	for (int i = 1; i < intervalsAmount; i++) {
		if (!discontinuityFixed) {
			break;
		}

		double equationValueInStep = useEquationById(lowerLimit + i * stepSize, equationId);
		
		if (checkForDiscontinuity(equationValueInStep)) {
			discontinuity newDiscontinuity = fixDiscontinuity(lowerLimit, stepSize, epsilon, i, equationId);
			if (newDiscontinuity.isFixable) {
				equationValueInStep = newDiscontinuity.newValue;
			}
			else {
				discontinuityFixed = newDiscontinuity.isFixable; // false
			}
			
			returnResult.discontinuities.push_back(newDiscontinuity);
		}

		if (i % 2 == 0) {
			integrationResult += 2 * equationValueInStep;
		}
		else {
			integrationResult += 4 * equationValueInStep;
		}
	}

	integrationResult = integrationResult * (stepSize / 3);

	returnResult.value = integrationResult;
	returnResult.allDiscontinuitiesFixed = discontinuityFixed;

	return returnResult;
}

bool DefinitiveIntegralCalculator::checkForDiscontinuity(double value) {
	if (std::isinf(value) || std::isnan(value)) {
		return true;
	}
	else {
		return false;
	}
}

bool DefinitiveIntegralCalculator::checkForNonRemovableDiscontinuities(double lowerLimit, double upperLimit, int equationId, std::vector<discontinuity> *discontinuities) {
	// hard coded because I spent way too many time on trying to do it other way
	// and I'm starting to think it is not possible
	switch (equationId) {
		case 1:
			return 0;
			break;
		case 2:
			if ((lowerLimit <= 0 && upperLimit >= 0) || lowerLimit == 0 || upperLimit == 0) {
				discontinuity newDiscontinuity;
				newDiscontinuity.isFixable = false;
				newDiscontinuity.newValue = 0;
				newDiscontinuity.type = 2;
				newDiscontinuity.typeName = "asymptotic";
				newDiscontinuity.x = 0;
				discontinuities->push_back(newDiscontinuity);
				return 1;
			}
			return 0;
			break;
		case 3:
			return 0;
			break;
		case 4:
			if ((lowerLimit <= -2 && upperLimit >= 2) || lowerLimit == -2 || upperLimit == -2) {
				discontinuity newDiscontinuity;
				newDiscontinuity.isFixable = false;
				newDiscontinuity.newValue = 0;
				newDiscontinuity.type = 2;
				newDiscontinuity.typeName = "asymptotic";
				newDiscontinuity.x = -2;
				discontinuities->push_back(newDiscontinuity);
				return 1;
			}
			return 0;
			break;
	}
	return 0;
}

discontinuity DefinitiveIntegralCalculator::fixDiscontinuity(double lowerLimit, double stepSize, double epsilon, int i, int equationId) {
	discontinuity result;
	result.x = lowerLimit + i * stepSize;
	double resultValue;
	result.xleft = result.x - epsilon;
	result.xright = result.x + epsilon;
	double left = useEquationById(result.xleft, equationId);
	double right = useEquationById(result.xright, equationId);
	if (!checkForDiscontinuity(left) && !checkForDiscontinuity(right)) {
		result.isFixable = true;
		result.newValue = (left + right) / 2;
		result.type = 1;
		result.typeName = "removable";
		return result;
	}
	else {
		result.isFixable = false;
		result.type = 2;
		result.typeName = "non-removable";
		return result;
	}
}

double DefinitiveIntegralCalculator::getMarginOfError(double lowerLimit, double upperLimit, int intervalsAmount, int equationId) {
	/*double k = use4thDerivativeOfEquationById(upperLimit, equationId);
	double error = (k * (upperLimit - lowerLimit)) / (180 * std::pow(intervalsAmount, 4));
	error = std::abs(error);*/

	const double constForSympsonsMethod = (double)1 / (double)15;
	double integral2N = calculate(lowerLimit, upperLimit, intervalsAmount, equationId).value;
	intervalsAmount = intervalsAmount / 2;
	if (intervalsAmount % 2 != 0) {
		intervalsAmount++;
	}
	double integralN = calculate(lowerLimit, upperLimit, intervalsAmount, equationId).value;

	double rungeError = constForSympsonsMethod * std::abs(integral2N - integralN);

	return rungeError;
}

void DefinitiveIntegralCalculator::printMarginsOfError(double lowerLimit, double upperLimit, int intervalsAmount, int equationId) {
	double error = getMarginOfError(lowerLimit, upperLimit, intervalsAmount, equationId);
	
	std::cout << "Methods margin of error by Runge rule: " << error << std::endl;
}

void DefinitiveIntegralCalculator::printResult(calculationResult result) {
	if (result.allDiscontinuitiesFixed) {
		std::cout << "The definitive integral is " << result.value << std::endl;
	}
	else {
		if (result.discontinuities.back().isFixable == false) {
			std::cout << "Definitive integral cannot be found because of discontinuity." << std::endl;
		}
		else {
			std::cout << "Definitive integral cannot be found because function is not defined somewhere from lower to upper limit." << std::endl;
		}
	}
}

void DefinitiveIntegralCalculator::printDiscontinuityInfo(std::vector<discontinuity> discontinuities) {
	for (int i = 0; i < discontinuities.size(); i++) {
		if (discontinuities.at(i).isFixable) {
			std::cout << "Discontinuity encountered at x = " << discontinuities.at(i).x << std::endl;
			std::cout << "Fixing it by taking an average between x1 = " << discontinuities.at(i).xleft << " and x2 = " << discontinuities.at(i).xright << std::endl;
			std::cout << "New value to fix the discontinuity = " << discontinuities.at(i).newValue << std::endl;
		}
		else {
			std::cout << "Function has a non-removable " << discontinuities.at(i).typeName << " discontinuity type " << discontinuities.at(i).type << " at x = " << discontinuities.at(i).x << std::endl;

		}
	}
}