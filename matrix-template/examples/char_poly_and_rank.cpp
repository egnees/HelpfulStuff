#include "matrix.h"
#include "frac.h"
#include <iostream>
#include <algorithm>
#include <string>

bool check(const Matrix<Frac> &A, const Matrix<Frac> &B, Frac lambda) {
	for (int i = 1; i <= (int)A.m_; ++i) {
		if (((A - A.E() * lambda)^i).rk() != ((B - B.E() * lambda)^i).rk()) {
			return false;
		}
	}
	return true;
}

int main() {
	Matrix<Frac> A = Matrix<Frac>({
			{-1, -3, 1, 3},
			{7, 8, -1, -4}, 
			{7, 4, 3, -4}, 
			{-3, -2, 1, 6}
		});

	Matrix<Frac> B = Matrix<Frac>({
			{7, 5, -1, -3}, 
			{-3, -2, 1, 4}, 
			{3, 4, 3, -2},
			{-3, -6, 1, 8}
		});

	Matrix<Frac> C = Matrix<Frac>({
			{1, -2, -4, -3}, 
			{-3, 2, -3, -2}, 
			{1, 2, 6, 1},
			{9, 2, 6, 9}
		});
	
	A.printCharPoly("A");
	std::cout << '\n';
	B.printCharPoly("B");
	std::cout << '\n';
	C.printCharPoly("C");
	std::cout << '\n';

	std::cout << (check(A, B, Frac(4)) ? "YES\n" : "NO\n");
	return 0;
}