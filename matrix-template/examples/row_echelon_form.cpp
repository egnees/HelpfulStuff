#include "matrix.h"
#include "frac.h"

int main() {
	Matrix<Frac> A = std::vector<std::vector<Frac>>({{-1, 2, 1, -1}, {-1, 2, 1, -1}, {2, -3, -1, 1}, {-3, 1, -2, 2}});
	std::cout << (A * A).USV() << '\n';
	Matrix<Frac> B = std::vector<std::vector<Frac>>({{-1, 1, -1}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}});
	std::cout << (A * B).trans().USV();

	std::cout << '\n';
	std::cout << A.trans().USV();	
	std::cout << '\n';
	std::cout << A.USV() << '\n';
	return 0;
}