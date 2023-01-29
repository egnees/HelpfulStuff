#include "matrix.h"
#include "frac.h"
#include <iostream>

int main() {
	Matrix<Frac> A = std::vector<std::vector<Frac>>({{-2, 2, 1, 1}, {-3, 1, 3, -1}, {5, -5, -7, 2}, {3, 7, 0, 2}});
	auto chi = A.charPoly();
	for (auto e : chi) {
		std::cout << e << ' ';
	}
	std::cout << "\n\n";

	std::cout << ((A + A.UNO() * 3) ^ 2).USV() << '\n';

	std::cout << (A ^ 2).USV() << '\n';

	Matrix<Frac> A_ = std::vector<std::vector<Frac>>({{1, -2, -3, 0}, {-1, -1, 0, 0}, {1, 0, -1, 0}, {0, 1, 1, 0}});
	std::cout << A_ << '\n';
	std::cout << A_.USV() << '\n';
	return 0;
}