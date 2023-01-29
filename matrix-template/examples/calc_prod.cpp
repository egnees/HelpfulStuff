#include "matrix.h"

#include <iostream>
#include <cmath>

int main() {
	double n;
	std::cin >> n;
	double n_sqrt = sqrt(n);
	// double n_1_sqrt = sqrt(n + 1);
	Matrix<long double> S({
		{1, 1},
		{1.0 / n_sqrt, -1.0 / n_sqrt}
	});
	Matrix<long double> S_({
		{1.0 / 2, n_sqrt / 2},
		{1.0 / 2, -n_sqrt / 2}
	});
	std::cout << (S * S_);
	return 0;
}