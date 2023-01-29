#include "matrix.h"
#include "frac.h"
#include <iostream>
#include <cassert>

int main() {
	Matrix<Frac> G = Matrix<Frac>({
		{1, -2, 1},
		{-2, 3, -1},
		{1, -1, -1}
	}).Tr();

	Matrix<Frac> A = G * Matrix<Frac>({
		{7, -5, -4},
		{6, -6, 0},
		{3, -4, 2}
	}) * G.inv();

	Matrix<Frac> F = Matrix<Frac>({
		{-1, 2, -1},
		{-2, 3, -1},
		{1, -1, -1}
	}).Tr();

	Matrix<Frac> B = F * Matrix<Frac>({
		{4, 8, -8},
		{-3, -6, 6},
		{-2, -6, 9}
	}) * F.inv();

	std::cout << A << '\n' << B << '\n';

	std::cout << A * Matrix<Frac>({
		{-2, 1, 1}
	}).Tr();

	std::cout << "Composition matrix is:\n" << A * B << '\n';

	std::cout << "FSS for A is:\n";
	A.printFSS();
	std::cout << '\n';

	std::cout << "FSS for B is:\n";
	B.printFSS();
	std::cout << '\n';

	std::vector<Matrix<Frac>> E = {
		Matrix<Frac>({{-2, 1, 1}}).Tr(),
		Matrix<Frac>({{-1, 2, 1}}).Tr(),
		Matrix<Frac>({{0, 0, 1}}).Tr()
	};

	std::cout << "Im A is:\n";
	for (const auto& v : E) {
		std::cout << (A * v).Tr();
	}

	std::cout << '\n';
	std::cout << "Im B is:\n";
	for (const auto& v : E) {
		std::cout << (B * v).Tr();
	}
	return 0;
}