using ll = long long;

#include <cmath>
#include <iostream>
#include <algorithm>

class Frac {
public:
	ll p, q; 

	//invariant gcd(p, q) == 1
	//p \in Z, q \in N
	//p == 0 => q == 1


	Frac(ll x = 0) : p(x), q(1) {};
	Frac(ll a, ll b) {
		if (a == 0) {
			p = 0;
			q = 1;
		} else {
			ll g = std::__gcd(llabs(a), llabs(b));
			a /= g;
			b /= g;

			if (b < 0) {
				b *= -1;
				a *= -1;	
			}

			p = a;
			q = b;
		}
	}

	bool operator==(const Frac& other) const {
		return p == other.p && q == other.q;
	}

	bool operator!=(const Frac& other) const {
		return !(*this == other);
	}

	bool operator<(const Frac& other) const {
		return *this != other && (long double)p/q<(long double)other.p/other.q;
	}

	bool operator>(const Frac& other) const {
		return *this != other && (long double)p/q>(long double)other.p/other.q;
	}

	Frac operator-() const {
		return Frac(-p, q);
	}

	Frac operator+(const Frac& other) const {
		return Frac(p * other.q + other.p * q, q * other.q);
	}

	Frac operator-(const Frac& other) const {
		return *this + (-other);
	}

	Frac operator*(const Frac& other) const {
		return Frac(p * other.p, q * other.q);
	}

	Frac operator/(const Frac& other) const {
		return Frac(p * other.q, other.p * q);
	}

	Frac& operator+=(const Frac& other) {
		*this = *this + other;
		return *this;
	}

	Frac& operator-=(const Frac& other) {
		*this = *this - other;
		return *this;
	}

	Frac& operator*=(const Frac& other) {
		*this = (*this) * (other);
		return *this;
	}

	Frac& operator/=(const Frac& other) {
		*this = *this / other;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& out, const Frac& f) {
		out << f.p;
		if (f.q != 1) out << '/' << f.q;
		return out;
	}
};