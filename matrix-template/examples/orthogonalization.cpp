#include "matrix.h"
#include "frac.h"
#include <iostream>
#include <vector>

int main() {
	Matrix<Frac> B = Matrix<Frac>({
		{0, 0, 1, 1},
		{0, 0, -3, -2},
		{1, -3, 3, 1},
		{1, -2, 1, 2}
	});
	Matrix<Frac> v = Matrix<Frac>({{4, 1, 1, -2}}).Tr();
	std::cout << "Beta(v,v) is: " << v.Tr() * B * v << '\n';

	Matrix<Frac> C = Matrix<Frac>({
		{4, 1, 1, -2},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}).Tr();

	Matrix<Frac> B1 = C.Tr() * B * C;
	std::cout << "B1 is:\n" << B1 << '\n';

	auto beta = [&](const Matrix<Frac>& x, const Matrix<Frac>& y) {
		return (x.Tr() * B * y)[0][0];
	};

	std::vector<Matrix<Frac>> current_basis = {
		Matrix<Frac>({{4, 1, 1, -2}}).Tr(),
		Matrix<Frac>({{0, 1, 0, 0}}).Tr(),
		Matrix<Frac>({{0, 0, 1, 0}}).Tr(),
		Matrix<Frac>({{0, 0, 0, 1}}).Tr()
	};

	std::vector<Matrix<Frac>> new_basis = {current_basis[0]};
	for (size_t k = 1; k < current_basis.size(); ++k) {
		Matrix<Frac> e = current_basis[k];
		for (size_t i = 0; i < k; ++i) {
			Frac coef = beta(current_basis[k], new_basis[i]) / beta(new_basis[i], new_basis[i]);
			e += new_basis[i] * Frac(-1) * coef;
		}
		new_basis.push_back(e);
	}

	std::cout << "New basis is:\n";
	for (auto e : new_basis) {
		std::cout << e.Tr();
	}
	std::cout << '\n';

	Matrix<Frac> C1 = Matrix<Frac>({
		{4, 1, 1, -2},
		{-4, 0, -1, 2},
		{12, -2, 4, -6},
		{4, Frac(5,3), Frac(2,3), -1}
	}).Tr();

	std::cout << "A diagonal form of B is:\n" << C1.Tr() * B * C1;
	return 0;
}