#include "matrix.h"
#include "frac.h"

int main() {
	Matrix<Frac> A = std::vector<std::vector<Frac>>({{4, -2, -5, -6}, {4, -2, -6, -7}, {0, 0, 4, -2}, {0, 0, -4, -2}});
	A = A ^ 4;
	std::cout << A.trans().USV() << '\n';
	return 0;
}