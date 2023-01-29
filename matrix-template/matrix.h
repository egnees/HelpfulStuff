//describes m_ x n_ matrix
#include <vector>
#include <iostream>
#include <algorithm>

template<typename T>
class Matrix {
public:
	std::vector<T>& operator[](const size_t& i) {
		return a_[i];
	}

	const std::vector<T>& operator[](const size_t& i) const {
		return a_[i];
	}

	Matrix(const std::vector<std::vector<T>>& a) {
		a_ = a;
		if (a.empty()) {
			m_ = 0;
			n_ = 0;
		} else {
			m_ = a.size();
			n_ = a[0].size();
		}
	}

	friend bool operator==(const Matrix& left, const Matrix& right) {
		return left.a_ == right.a_;
	}

	friend bool operator!=(const Matrix& left, const Matrix& right) {
		return left.a_ != right.a_;
	}	

	Matrix(size_t m, size_t n, const T& x) {
		a_.resize(m, std::vector<T> (n, x));
		m_ = m;
		n_ = n;	
	}

	Matrix(size_t n, const T& x) {
		m_ = n_ = n;
		a_.resize(n, std::vector<T> (n));
		for (size_t i = 0; i < n; ++i) {
			a_[i][i] = x;
		}
	}

	Matrix(const std::vector<T>& v) {
		m_ = v.size();
		n_ = 1;
		a_.resize(m_, std::vector<T> (n_));
		for (size_t i = 0; i < m_; i++)
			a_[i][0] = v[i];
	}

	Matrix operator+(const T& x) const {
		Matrix other(*this);
		for (auto& row : other.a_) {
			for (auto& elem : row) {
				elem += x;
			}
		}
		return other;
	}

	Matrix operator+(const Matrix& other) const {
		Matrix res(*this);
		for (size_t i = 0; i < m_; ++i) {
			for (size_t j = 0; j < n_; ++j) {
				res[i][j] += other[i][j];
			}
		}
		return res;
	}

	Matrix operator-(const Matrix& other) const {
		Matrix res(*this);
		for (size_t i = 0; i < m_; ++i) {
			for (size_t j = 0; j < n_; ++j) {
				res[i][j] -= other[i][j];
			}
		}
		return res;
	}

	Matrix operator-(const T& x) const {
		return *this + (-x);
	}

	Matrix& operator+=(const T& x) {
		return *this = *this + x;
	}

	Matrix& operator+=(const Matrix& other) {
		return *this = *this + other;
	}

	Matrix operator*(const T& x) const {
		Matrix res(*this);
		for (auto& row : res.a_) {
			for (auto& elem : row) {
				elem *= x;
			}
		}
		return res;
	}

	Matrix operator*=(const T& x) {
		return *this = (*this) * x;
	}

	friend Matrix operator*(const Matrix& m, const Matrix& other) {
		Matrix res(m.m_, other.n_, T(0));
		for (size_t i = 0; i < m.m_; ++i) {
			for (size_t j = 0; j < other.n_; ++j) {
				for (size_t k = 0; k < m.n_; ++k) {
					res[i][j] += m[i][k] * other[k][j];
				}
			}
		}

		return res;
	}

	friend Matrix& operator*=(Matrix& m, Matrix& other) {
		return m = m * other;
	}

	friend std::ostream& operator<<(std::ostream& out, const Matrix& m) {
		for (auto& row : m.a_) {
			for (auto& elem : row) {
				out << elem << ' '; 
			}
			out << '\n';
		}
		return out;
	}

	Matrix& swap(const size_t& i, const size_t& j) {
		std::swap(a_[i], a_[j]);
		return *this;
	}

	Matrix& sub(const size_t& i, const size_t& j, T x) {
		for (size_t k = 0; k < n_; ++k) {
			a_[i][k] -= a_[j][k] * x;
		}
		return *this;
	}

	Matrix& mult(const size_t& i, T x) {
		for (size_t k = 0; k < n_; ++k) {
			a_[i][k] *= x;
		}
		return *this;
	}

	Matrix USV() const {
		Matrix res(*this);
		size_t next_row = 0;
		std::vector<std::pair<size_t, size_t>> steps;
		for (size_t it = 0; it < n_; ++it) {
			int first_good = -1;
			for (size_t x = next_row; x < m_; ++x) {
				if (res[x][it] != 0) {
					first_good = x;
					break;
				}
			}
			if (first_good == -1) continue;

			steps.push_back(std::make_pair(next_row, it));

			res.swap(next_row, first_good);
			for (size_t x = next_row + 1; x < m_; ++x) {
				res.sub(x, next_row, res[x][it] / res[next_row][it]);
			}

			++next_row;

			if (next_row == m_) break;
		}

		std::reverse(steps.begin(), steps.end());

		for (const auto& [i, j] : steps) {
			res.mult(i, T(1) / res[i][j]);

			for (size_t q = 0; q < i; ++q) {
				res.sub(q, i, res[q][j]);
			}

		}

		return res;
	}

	T det() const {
		Matrix res(*this);
		size_t next_row = 0;

		T ret = T(1);

		for (size_t it = 0; it < n_; ++it) {
			int first_good = -1;
			for (size_t x = next_row; x < m_; ++x) {
				if (res[x][it] != 0) {
					first_good = x;
					break;
				}
			}
			if (first_good == -1) continue;

			res.swap(next_row, first_good);

			if (abs(next_row - first_good) % 2 == 1) ret *= T(-1);

			for (size_t x = next_row + 1; x < m_; ++x) {
				res.sub(x, next_row, res[x][it] / res[next_row][it]);
			}

			++next_row;

			if (next_row == m_) break;
		}

		for (size_t i = 0; i < n_; i++) ret *= res[i][i];

		return ret;
	}

	std::vector<T> charPoly() const {
		std::vector<T> ret(n_ + 1, T(0));
		for (int msk = 0; msk < (1 << n_); msk++) {
			int k = 0;
			for (size_t i = 0; i < n_; i++) if (msk & (1 << i)) k++;
			Matrix cur = Matrix<T>(n_ - k, n_ - k, T(0));
			size_t ptr_row = 0;
			for (size_t i = 0; i < n_; i++) {
				if (msk & (1 << i)) continue;
				size_t ptr_col = 0;
				for (size_t j = 0; j < n_; j++) {
					if (msk & (1 << j)) continue;
					cur[ptr_row][ptr_col] = a_[i][j];
					ptr_col++;
				}
				ptr_row++;
			}
			ret[k] += cur.det();
		}
		for (size_t i = 0; i < n_; i++) {
			if ((n_ - i) & 1) ret[i] *= T(-1);
		}
		return ret;
	}

	void printCharPoly(const std::string& name = "") {
		auto h = charPoly();
		std::reverse(h.begin(), h.end());
		if (name != "") {
			std::cout << "Char poly for matrix " << name << ":\n";
		}
		for (size_t i = 0; i < h.size(); ++i) {
			if (h[i] != T(0)) {
				if (i && h[i] > T(0)) std::cout << "+";
				if (h[i] != T(1) && h[i] != T(-1)) std::cout << h[i];
				else if (h[i] == T(-1)) std::cout << "-";
				int power = h.size() - i - 1;
				if (power > 0) std::cout << "x";
				if (power > 1) std::cout << "^" << power;  
			}
		}
		std::cout << '\n';
	}

	Matrix UNO() const {
		Matrix res = Matrix(a_.size(), a_.size(), T(0));
		for (size_t i = 0; i < a_.size(); i++)
			res[i][i] = T(1);
		return res;
	}

	std::vector<Matrix> FSS() const {
		auto U = USV();
		size_t ptr = -1;
		std::vector<std::pair<size_t, size_t>> dep;
		std::vector<Matrix> ret;
		for (size_t col = 0; col < n_; ++col) {
			if (ptr + 1 < m_ && U[ptr + 1][col] == T(1)) {
				dep.push_back(std::make_pair(col, ++ptr));
			} else {
				Matrix<T> current = Matrix<T>(n_, 1, T(0));
				current[col][0] = T(1);
				for (auto [j, i] : dep) {
					current[j][0] = T(-1) * U[i][col];
				}
				ret.push_back(current);
			}
		}
		return ret;
	}

	auto printFSS() const {
		auto fss = FSS();
		for (auto e : fss) {
			std::cout << e.Tr();
		}
	}

	Matrix E() const {
		return UNO();
	}

	size_t rk() const {
		auto U = USV();
		size_t ret = 0;

		for (size_t row = 0; row < m_; ++row) {
			bool one = false;
			for (size_t col = 0; col < n_; ++col) {
				if (U[row][col] != T(0)) {
					one = true;
					break;
				}
			}
			if (one) ret += 1;
		}

		return ret;
	}

	Matrix inv() const {
		//it is possible to get inv
		Matrix res = Matrix(a_.size(), a_.size() * 2, T(0));
		for (size_t i = 0; i < a_.size(); i++)
			for (size_t j = 0; j < a_.size(); j++)
				res[i][j] = a_[i][j];
		for (size_t i = 0; i < a_.size(); i++) res[i][i + a_.size()] = T(1);
		res = res.USV();
		Matrix ret = Matrix(a_.size(), a_.size(), T(0));
		for (size_t i = 0; i < a_.size(); i++)
			for (size_t j = 0; j < a_.size(); j++)
				ret[i][j] = res[i][j + a_.size()];
		return ret;
	}

	Matrix Tr() const {
		//Tr is not trace
		std::vector<std::vector<T>> b_(n_, std::vector<T>(m_));
		for (size_t i = 0; i < m_; i++) {
			for (size_t j = 0; j < n_; j++) {
				b_[j][i] = a_[i][j];
			}
		}
		return Matrix(b_);
	}

	Matrix operator ^ (int n) const {
		Matrix cur = *this;
		if (n < 0) {
			n *= -1;
			cur = cur.inv();
		}
		Matrix ret = UNO();
		for (int i = 0; i < n; i++) ret *= cur;
		return ret;
	}
	size_t m_, n_;
	std::vector<std::vector<T>> a_;
};