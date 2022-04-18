#pragma once

#define _USE_MATH_DEFINES
#include <string>
#include <math.h>

class InputController {
public:
	int takeIntervalsAmount();
	int takeEquationId();
	double takeLowerLimit();
	double takeUpperLimit();
private:
	double takeDouble(std::string); //takes message for specifying input
	int takeInteger(std::string); //takes message for specifying input
};

int InputController::takeIntervalsAmount() {
	std::string fineMessage = "Please enter an amount of subintervals (even integer above 0): ";
	std::string errorMessage = "Non-valid input. " + fineMessage;
	std::cout << fineMessage;
	int result = 0;
	result = takeInteger(errorMessage);
	while (result <= 0 || (result % 2 != 0)) {
		std::cout << errorMessage;
		result = takeInteger(errorMessage);
	}
	return result;
}

int InputController::takeEquationId()  {
	std::string fineMessage = "Please enter an equation id (from 1 to 4): ";
	std::string errorMessage = "Non-valid input. " + fineMessage;
	std::cout << fineMessage;
	int result = 0;
	result = takeInteger(errorMessage);
	while (result < 1 || result > 4) {
		std::cout << errorMessage;
		result = takeInteger(errorMessage);
	}
	return result;
}

double InputController::takeLowerLimit() {
	std::string fineMessage = "Please enter lower limit of the integral (double number): ";
	std::string errorMessage = "Non-valid input. " + fineMessage;
	std::cout << fineMessage;
	return takeDouble(errorMessage);
}

double InputController::takeUpperLimit() {
	std::string fineMessage = "Please enter upper limit of the integral (double number): ";
	std::string errorMessage = "Non-valid input. " + fineMessage;
	std::cout << fineMessage;
	return takeDouble(errorMessage);
}

double InputController::takeDouble(std::string message) {
	if (message.empty()) {
		// default
		message = "Non-valid input. Please enter a double number: ";
	}

	bool correctInput = false;
	double value;

	while (!correctInput) {
		correctInput = true;
		std::string input;
		std::getline(std::cin, input);

		// hard-coded for usual values
		if (input == "pi/2") {
			value = M_PI / 2;
			break;
		}

		// because there is no actual need to write math expression parsers

		// hard-coded for usual values
		if (input == "pi/4") {
			value = M_PI / 4;
			break;
		}

		for (int i = 0; i < input.size(); i++) {
			if (!correctInput) {
				break;
			}
			switch (input[i]) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
			case '.':
			case '-':
				break;
			case ',':
				input[i] = '.';
				break;
			default:
				correctInput = false;
				break;
			}
		}

		if (correctInput) {
			value = std::stod(input);
		}
		else {
			std::cout << message;
		}
	}

	return value;
}

int InputController::takeInteger(std::string message) {
	if (message.empty()) {
		// default
		message = "Non-valid input. Please enter an integer number: ";
	}

	bool correctInput = false;
	int value;

	while (!correctInput) {
		correctInput = true;
		std::string input;
		std::getline(std::cin, input);

		for (int i = 0; i < input.size(); i++) {
			if (!correctInput) {
				break;
			}
			switch (input[i]) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
			case '-':
				break;
			default:
				correctInput = false;
				break;
			}
		}

		if (input.size() > 10) {
			correctInput = false;
		}

		if (correctInput) {
			value = std::stoi(input);
		}
		if (!correctInput) {
			std::cout << message;
		}
	}

 	return value;
}