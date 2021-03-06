/*
 * GeneratorPSF.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: peter
 */

#include "PointSpreadFunc.h"

namespace kbmod {

PointSpreadFunc::PointSpreadFunc(float stdev) {

	width = stdev;
	float simpleGauss[MAX_KERNEL_RADIUS];
	double psfCoverage = 0.0;
	double normFactor = stdev * sqrt(2.0);
	int i = 0;

	// Create 1D gaussian array
	while (psfCoverage < 0.98 && i < MAX_KERNEL_RADIUS) {
		float currentBin = 0.5
				* (std::erf((float(i) + 0.5) / normFactor)
						- std::erf((float(i) - 0.5) / normFactor));
		simpleGauss[i] = currentBin;
		if (i == 0) {
			psfCoverage += currentBin;
		} else {
			psfCoverage += 2.0 * currentBin;
		}
		i++;
	}

	radius = i;
	dim = 2 * radius - 1;
	sum = 0.0;

	// Create 2D gaussain by multiplying with itself
	kernel = std::vector<float>();
	for (int ii = 0; ii < dim; ++ii) {
		for (int jj = 0; jj < dim; ++jj) {
			float current = simpleGauss[abs(radius - ii - 1)]
					* simpleGauss[abs(radius - jj - 1)];
			kernel.push_back(current);
			sum += current;
		}
	}

}

void PointSpreadFunc::squarePSF() {
	sum = 0.0;
	for (float& i : kernel)
	{
		i = i * i;
		sum += i;
	}
}

void PointSpreadFunc::printPSF() {
	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout.precision(3);
	for (int row = 0; row < dim; ++row) {
		std::cout << "| ";
		for (int col = 0; col < dim; ++col) {
			std::cout << kernel[row * dim + col] << " | ";
		}
		std::cout << "\n ";
		for (int space = 0; space < dim * 8 - 1; ++space)
			std::cout << "-";
		std::cout << "\n";
	}
	std::cout << 100.0 * sum << "% of PSF contained within kernel\n";
}

} /* namespace kbmod */
