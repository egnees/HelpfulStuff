#include "matrix.h"
#include "frac.h"
#include <iostream>

int main() {
	Matrix<Frac> A = Matrix<Frac>({
		{3, -5, 3, 0, 0, 0},
		{0, 5, -1, 0, 0, 0},
		{0, 2, 2, 0, 0, 0},
		{-1, -6, 4, 5, -1, 0},
		{-2, -7, 5, 4, 1, 0},
		{1, 3, -2, -1, 1, 4}
	});
	A.printCharPoly("A"); //(x-4)^2*(x-3)^4

	Matrix<Frac> A1 = Matrix<Frac>({
		{3, -5, 3},
		{0, 5, -1},
		{0, 2, 2}
	});
	Matrix<Frac> A2 = Matrix<Frac>({
		{5, -1, 0},
		{4, 1, 0},
		{-1, 1, 4}
	});
	A1.printCharPoly("A1"); //(x-4)(x-3)^2
	A2.printCharPoly("A2"); //(x-4)(x-3)^2
	std::cout << '\n';

	std::vector<std::vector<Frac>> S;

	{
		auto A4 = A - A.E() * Frac(4);
		std::cout << "dim ker(A-4E) = " << A4.FSS().size() << "\n";

		auto ker2 = (A4^2).FSS();

		auto v = ker2[0];
		auto u = A4 * v;
		// u neq 0 => v \in ker (A-4E)^2\ker(A-4E)
		//			  u \in ker (A-4E)	
		std::cout << "The Jordan basis of (A-4E) is:\n";
		std::cout << u.Tr() << v.Tr() << '\n';

		S.push_back(u.Tr()[0]);
		S.push_back(v.Tr()[0]);
	}

	{
		auto A3 = A - A.E() * Frac(3);
		std::cout << "dim ker(A-3E) = " << A3.FSS().size() << '\n';
		std::cout << "dim ker(A-3E)^2 = " << (A3^2).FSS().size() << "\n";

		auto ker2 = (A3^2).FSS();

		auto v = ker2[0];
		auto u = A3 * v;
		// u neq 0 => v \in ker (A-4E)^2\ker(A-4),
		// 			  u \in ker (A-4E)

		auto v1 = ker2[1];
		auto u1 = A3 * v1;
		//u1 neq 0

		std::cout << "The Jordan basis of (A-3E) is:\n";
		std::cout << u.Tr() << v.Tr() << u1.Tr() << v1.Tr() << '\n';

		S.push_back(u.Tr()[0]);
		S.push_back(v.Tr()[0]);
		S.push_back(u1.Tr()[0]);
		S.push_back(v1.Tr()[0]);
	}

	auto B = Matrix<Frac>(S).Tr();
	std::cout << "The Jordan normal form of A is:\n";
	std::cout << B.inv() * A * B;

	return 0;
}