#include <iostream>
#include <tuple>
#include <limits>

//https://ja.wikipedia.org/wiki/‰ÁŽZŠí

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
}

bool Nor(const bool& A, const bool& B) {
	return !(A | B);
}
bool Equal(const bool& A, const bool& B) {
	return !(A ^ B);
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
T Sub(const T& A, const T& B) {//have bug??

	std::size_t L = std::numeric_limits<T>::digits;
	L += L % 2;

	T V(0);
	
	std::tuple<bool, bool> X{ false,true };

	for (std::size_t i = 0; i < L; i++) {
		X = FullAdder(((A & (1 << i)) ? true : false), Not((B & (1 << i)) ? true : false), std::get<1>(X));
		V |= (std::get<0>(X) ? 1 : 0) << i;
	}

	return V;
}


int main() {
	int A = 10;
	int B = -10;

	int X1 = Add(A, A);		std::cout << "Add:" << A << '+' << A <<"=="<<X1<< std::endl;
	int X2 = Add(A, B);		std::cout << "Add:" << A << '+' << B <<"=="<<X2<<std::endl;
	int X3 = Add(A, B*2);	std::cout << "Add:" << A << '+' << B*2 <<"=="<<X3<< std::endl;	

	int Y1 = Sub(A, A);		std::cout << "Sub:" << A << '-' << A <<"=="<<Y1<< std::endl;
	int Y2 = Sub(A, B);		std::cout << "Sub:" << A << '-' << B <<"=="<<Y2<<std::endl;
	int Y3 = Sub(A, B * 2); std::cout << "Sub:" << A << '-' << B * 2 << "==" << Y3 << std::endl;
	int Y4 = Sub(A/2, A);	std::cout << "Sub:" << A/2 << '-' << A << "==" << Y4 << std::endl;

	return 0;

}