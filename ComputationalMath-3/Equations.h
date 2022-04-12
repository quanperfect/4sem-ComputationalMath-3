#pragma once

#include <math.h>
#include <vector>
#include "InputController.h"

#ifndef EQUATIONS_H

#define EQUATIONS_H

int chooseEquation() {
	InputController inputController;
	int choice;
	std::cout << "Equations: " << std::endl;
	std::cout << "[1] x * sin(x)" << std::endl;
	std::cout << "[2] log2(x)" << std::endl;
	std::cout << "[3] sin(x) / x" << std::endl;
	choice = inputController.takeEquationId();
	std::cout << "-------------------" << std::endl;

	return choice;
}

double f1(double x) {
	return (x * sin(x));
}

double f2(double x) {
	return log2(x);
}

double f3(double x) {
	return (sin(x)/x);
}

double useEquationById(double x, int id) {
	if (id == 1) {
		return f1(x);
	}
	if (id == 2) {
		return f2(x);
	}
	if (id == 3) {
		return f3(x);
	}

	throw std::invalid_argument("Definitive Integral Calculator used equationId that does not exist.");
}

double g1(double x) {
	return (x * sin(x) - 4 * cos(x));
}

double g2(double x) {
	return (-6 / (log(2) * std::pow(x, 4)));
}

double g3(double x) {
	double firstSum = (std::pow(x, 4) - 12 * std::pow(x, 2) + 24) * sin(x);
	double secondSum = (4 * std::pow(x, 3) - 24 * x) * cos(x);
	return ((firstSum + secondSum) / (std::pow(x, 5)));
}

double use4thDerivativeOfEquationById(double x, int id) {
	if (id == 1) {
		return g1(x);
	}
	if (id == 2) {
		return g2(x);
	}
	if (id == 3) {
		return g3(x);
	}

	throw std::invalid_argument("Definitive Integral Calculator used equationId that does not exist.");
}

//#define f1(x) ( cos(x)-x*exp(x) )

#endif // !
#pragma once
