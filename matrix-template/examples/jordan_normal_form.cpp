#include "matrix.h"
#include "frac.h"

#include <iostream>

int main() {
	Matrix<Frac> A = Matrix<Frac>({{5, -7, -4}, {6, -9, -5}, {-6, 8, 4}});
	std::cout << A << '\n';

	auto characteristicalPolynomial = A.charPoly();
	for (auto e : characteristicalPolynomial)
		std::cout << e << '\n';

	std::cout << (A + A.UNO()) * (A + A.UNO()) * (A - A.UNO() * 2) << '\n';

	std::cout << (A + A.UNO()).USV() << '\n';

	std::cout << ((A + A.UNO()) * (A + A.UNO())).USV() << '\n';

	std::cout << (A - A.UNO() * 2).USV() << '\n';

	//test done its faking insane

	return 0;
}