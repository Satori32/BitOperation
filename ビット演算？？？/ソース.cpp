#include <iostream>
#include <tuple>
#include <limits>
#include <algorithm>
#include <cmath>
#include <random>
#include <cstdint>
//https://ja.wikipedia.org/wiki/â¡éZäÌ

bool And(const bool& A, const bool& B) {
	return A & B;
}

bool OR(const bool& A, const bool& B) {
	return A | B;
}
bool Not(const bool& A) {
	return !A;
//	return ~A;//why not work this line...
}

bool Xor(const bool& A, const bool& B) {
	return A ^ B;
}
bool Nand(const bool& A, const bool& B) {
	return !(A & B);
//	return ~(A & B);	
}

bool Nor(const bool& A, const bool& B) {
	return !(A | B);
//	return ~(A | B);
}
bool Equal(const bool& A, const bool& B) {
	return !(A ^ B);
//	return ~(A ^ B);
}

std::tuple<bool, bool> HarfAdder(const bool& A, const bool& B) {//std::get<0>() => S. std::get<1>() => C.
	bool S = Xor(A, B);
	bool C = And(A, B);

	return { S,C };
}


std::tuple<bool, bool> FullAdder(const bool& A, const bool& B,const bool& X) {//std::get<0>() => S. std::get<1>() => C.
	auto AA = HarfAdder(A, B);
	auto BB = HarfAdder(std::get<0>(AA), X);

	bool S = std::get<0>(BB);
	bool C = OR(std::get<1>(AA), std::get<1>(BB));

	return { S,C };
}

template<class T>
T Add(const T& A, const T& B) {

	std::size_t L = std::numeric_limits<T>::digits;
	std::tuple<bool, bool> X{ false,false };
	T V(0);

	L += L % 2;

	for (std::size_t i = 0; i < L; i++) {
		X = FullAdder(((A & (1 << i)) ? true : false), ((B & (1 << i)) ? true : false), std::get<1>(X));
		V |= (std::get<0>(X) ? 1 : 0) << i;
	}

	return V;
}
template<class T>
T Sub(const T& A, const T& B) {

	std::size_t L = std::numeric_limits<T>::digits;

	std::tuple<bool, bool> X{ false,true };
	T V(0);

	L += L % 2;

	for (std::size_t i = 0; i < L; i++) {
		X = FullAdder(((A & (1 << i)) ? true : false), Not((B & (1 << i)) ? true : false), std::get<1>(X));
		V |= (std::get<0>(X) ? 1 : 0) << i;
	}

	return V;
}

template<class T>
T  Mul(const T& A,const T& B) {//ïMéZñ@Ç≈Ç∑ÅB
	T C(0);

	for (std::size_t X = 0; X < std::numeric_limits<T>::digits; X++) {
		if (B & (1 << X)) {
			C += (A << X);
		}
	}
	return C;
}

template<class T>
int Sign(const T& A) {
	return A >= 0 ? 1 : -1;
}

template<class T>
T Abs(const T& In) {
	return In >= 0 ? In : -In;
}

template<class T>
std::tuple<T, T> DivMod(const T& A, const T& B) {//ïMéZñ@ÉfÉXÅB
	T Y(0);
	T Z(Abs(A));
	std::size_t i = 0;
	if (B == 0) { return { 1,A }; }
	if (Abs(A) <Abs(B)) { return { 0,A }; }
	for (std::size_t j = 0; j < std::numeric_limits<T>::digits; j++) {
		for (i = 0; i < std::numeric_limits<T>::digits; i++) {
			if ((Abs(Z)>>i) < Abs(B)) { break; }
		}
		Y |= 1 << (i-1);
		Z -= Abs(B) << (i-1);
		if (Abs(Z) < Abs(B)) { break; }
	}

	return { Y * Sign(Sign(A) * Sign(B)),Abs(Z) * Sign(A) };
}

int main() {
	int A = 10;
	int B = -10;

	int X1 = Add(A, A);		std::cout << "Add:" << A << '+' << A << "==" << X1 << std::endl;
	int X2 = Add(A, B);		std::cout << "Add:" << A << '+' << B << "==" << X2 << std::endl;
	int X3 = Add(A, B * 2);	std::cout << "Add:" << A << '+' << B * 2 << "==" << X3 << std::endl;

	int Y1 = Sub(A, A);		std::cout << "Sub:" << A << '-' << A << "==" << Y1 << std::endl;
	int Y2 = Sub(A, B);		std::cout << "Sub:" << A << '-' << B << "==" << Y2 << std::endl;
	int Y3 = Sub(A, B * 2); std::cout << "Sub:" << A << '-' << B * 2 << "==" << Y3 << std::endl;
	int Y4 = Sub(A / 2, A);	std::cout << "Sub:" << A / 2 << '-' << A << "==" << Y4 << std::endl;

	int Z1 = Mul(A, A);		std::cout << "Mul:" << A << '*' << A << "==" << Z1 << std::endl;
	int Z2 = Mul(A, B);		std::cout << "Mul:" << A << '*' << B << "==" << Z2 << std::endl;
	int Z3 = Mul(A, B * 2); std::cout << "Mul:" << A << '*' << B * 2 << "==" << Z3 << std::endl;
	int Z4 = Mul(A / 2, A);	std::cout << "Mul:" << A / 2 << '*' << A << "==" << Z4 << std::endl;
	int Z5 = Mul(-A, -A);		std::cout << "Mul:" << -A << '*' << -A << "==" << Z5 << std::endl;
	int Z6 = Mul(-A, A);		std::cout << "Mul:" << -A << '*' << A << "==" << Z6 << std::endl;



	auto W1 = DivMod(A, A);		std::cout << "Div:" << A << '/' << A << "==" << std::get<0>(W1) << std::endl;
	auto W2 = DivMod(A, B);		std::cout << "Div:" << A << '/' << B << "==" << std::get<0>(W2) << std::endl;
	auto W3 = DivMod(A, B / 2);	std::cout << "Div:" << A << '/' << B / 2 << "==" << std::get<0>(W3) << std::endl;

	std::size_t L = 32;

	for (int i = 0; i < L; i++) {
		for (int j = 1; j <= i; j++) {
			auto X = DivMod(i, j);
			std::cout << "Div:" << i << '/' << j << "==" << std::get<0>(X) << ',' << std::get<1>(X) << std::endl;
		}
	}

	std::cout << "Random DivMod Test." << std::endl;
	std::size_t LL = 10240;

	std::mt19937 mt(0);
	std::uniform_int_distribution<int> ui(-1024, 1024);
	for (std::size_t i = 0; i < LL; i++) {
		auto X = ui(mt);
		auto Y = ui(mt);
		if (Y == 0) { continue; }
		auto W = DivMod(X, Y);
			std::cout << X << ',' << Y << "\r";		
		if ((X / Y) != std::get<0>(W)) { std::cout << X << '/' << Y << ':' << ((X / Y) == std::get<0>(W)) << std::endl; }
		if ((X % Y) != std::get<1>(W)) { std::cout << X << '%' << Y << ':' << ((X % Y) == std::get<1>(W)) << std::endl; }
	}

	/** /
	for (std::int64_t i = -0xbeaf; i < 0xbeef; i++) {
		for (std::int64_t j = -0xbeaf; j < i; j++) {
			std::cout << i << ',' << j << "\r";
			auto V = DivMod(i, j);
			if (std::get<0>(V) != i / j) {
				std::cout << i << '/' << j << ' ' << std::get<0>(V) << "!=" << i / j << std::endl;
			}
			if (std::get<1>(V) != i % j) {
				std::cout << i << '%' << j << ' ' << std::get<1>(V) << "!=" << i % j << std::endl;
			}
		}
	}
	/**/
	return 0;

}