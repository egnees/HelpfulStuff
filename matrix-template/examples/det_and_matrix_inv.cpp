#include "matrix.h"
#include "frac.h"
#include <vector>
#include <cassert>

int main() {
	Matrix<Frac> A1_ = std::vector<std::vector<Frac>>({{1, 0, 0}, {1, 0, 0}, {-1, 0, 0}});
	Matrix<Frac> A2_ = std::vector<std::vector<Frac>>({{0, 2, 2}, {0, 1, 2}, {0, 1, -5}});

	Matrix<Frac> C1  = std::vector<std::vector<Frac>>({{-1, 4, 3}, {1, 3, 5}, {1, 5, 1}});
	Matrix<Frac> C2  = std::vector<std::vector<Frac>>({{3, -1, -2}, {2, 1, 1}, {1, 0, 0}}); 

	Matrix<Frac> C1_ = std::vector<std::vector<Frac>>({{-1, 0, 0}, {1, 0, 0}, {1, 0, 0}});
	Matrix<Frac> C2_ = std::vector<std::vector<Frac>>({{0, -1, -2}, {0, 1, 1}, {0, 0, 0}}); 

	assert((C1_ + C2_).det() != Frac(0));

	Matrix A = (C1 * A1_ + C2 * A2_) * (C1_ + C2_).inv();
	std::cout << A << '\n';

	std::cout << C1.inv() * A * C1 << '\n';
	std::cout << C2.inv() * A * C2 << '\n';
	return 0;
}