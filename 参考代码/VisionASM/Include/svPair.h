#pragma once

#include "svDef.h"
#include <utility>

#pragma pack(push,8)

template<class T>
class scPair : public std::pair<T,T>
{
public:
	scPair() : std::pair<T,T>() {}
	scPair(T x, T y) : std::pair<T,T>(x, y) {}

	const T& x() const {return first;}
	T& x() {return first;}

	const T& y() const {return second;}
	T& y() {return second;}

	scPair<T> operator+ (const scPair<T>& r) const;
	scPair<T>& operator+= (const scPair<T>& r);

	scPair<T> operator- () const;
	scPair<T> operator- (const scPair<T>& r) const;
	scPair<T>& operator-= (const scPair<T>& r);

	scPair<T> operator* (const scPair<T>& r) const;
	scPair<T>& operator*= (const scPair<T>& r);

	scPair<T> operator/ (const scPair<T>& r) const;
	scPair<T>& operator/= (const scPair<T>& r);

	bool operator== (const scPair<T>& r) const;
	bool operator!= (const scPair<T>& r) const { return !(*this == r); }
};

typedef scPair<short>	scSPair;
typedef scPair<int>		scIPair;
typedef scPair<float>	scFPair;
typedef scPair<double>	scDPair;

#pragma pack(pop)
