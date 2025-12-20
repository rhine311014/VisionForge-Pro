#pragma once

#pragma pack(push, 8)

#include <vector>
#include <initializer_list>

#ifdef SVPOLYNOMIAL_EXPORTS_3D
#define SVPOLYNOMIAL_API_3D __declspec(dllexport)
#else
#define SVPOLYNOMIAL_API_3D __declspec(dllimport)
#endif

#ifndef svStd
#define svStd std::
#endif

class SVPOLYNOMIAL_API_3D Polynomial
{
public:
	Polynomial(unsigned int degree = 0);
	Polynomial(float values[], size_t size);
	void SetDegree(unsigned int degree);
	void SetCoefficients(float value);
	inline unsigned int GetDegree() const	{ return static_cast<unsigned int>(m_Coefficient.size() - 1); };
	inline const float& operator[](unsigned int i) const { return m_Coefficient[i]; };
	inline float& operator[](unsigned int i) { return m_Coefficient[i]; };
	
	inline bool operator==(const Polynomial& p) const { return m_Coefficient == p.m_Coefficient; };
	inline bool operator!=(const Polynomial& p) const { return m_Coefficient != p.m_Coefficient; };
	inline bool operator<(const Polynomial& p) const { return m_Coefficient < p.m_Coefficient; };
	inline bool operator<=(const Polynomial& p) const { return m_Coefficient <= p.m_Coefficient; };
	inline bool operator>(const Polynomial& p) const { return m_Coefficient > p.m_Coefficient; };
	inline bool operator>=(const Polynomial& p) const { return m_Coefficient >= p.m_Coefficient; };

	float operator()(float t) const;
	Polynomial GetDerivative() const;
	Polynomial GetInversion() const;
	Polynomial GetTranslation(float t0) const;
	
	void EliminateLeadingZeros();

	void Divide(const Polynomial& divisor, Polynomial& quotient, Polynomial& remainder) const;

	void MakeMonic();
protected:
	svStd vector<float> m_Coefficient;
};



SVPOLYNOMIAL_API_3D Polynomial GreatestCommonDivisor(const Polynomial& p0, const Polynomial& p1);

SVPOLYNOMIAL_API_3D void SquareFreeFactorization(const Polynomial& f, svStd vector<Polynomial>& factors);

// 一元操作符
SVPOLYNOMIAL_API_3D Polynomial operator+(const Polynomial& p);

SVPOLYNOMIAL_API_3D Polynomial operator-(const Polynomial& p);

// 多元操作符
SVPOLYNOMIAL_API_3D Polynomial operator+(const Polynomial& p0, const Polynomial& p1);

SVPOLYNOMIAL_API_3D Polynomial operator-(const Polynomial& p0, const Polynomial& p1);

SVPOLYNOMIAL_API_3D Polynomial operator*(const Polynomial& p0, const Polynomial& p1);

SVPOLYNOMIAL_API_3D Polynomial operator+(const Polynomial& p, float scalar);

SVPOLYNOMIAL_API_3D Polynomial operator+(float scalar, const Polynomial& p);

SVPOLYNOMIAL_API_3D Polynomial operator-(const Polynomial& p, float scalar);

SVPOLYNOMIAL_API_3D Polynomial operator-(float scalar, const Polynomial& p);

SVPOLYNOMIAL_API_3D Polynomial operator*(const Polynomial& p, float scalar);

SVPOLYNOMIAL_API_3D Polynomial operator*(float scalar, const Polynomial& p);

SVPOLYNOMIAL_API_3D Polynomial operator/(const Polynomial& p, float scalar);

SVPOLYNOMIAL_API_3D Polynomial& operator+=(Polynomial& p0, const Polynomial& p1);

SVPOLYNOMIAL_API_3D Polynomial& operator-=(Polynomial& p0, const Polynomial& p1);

SVPOLYNOMIAL_API_3D Polynomial& operator*=(Polynomial& p0, const Polynomial& p1);

SVPOLYNOMIAL_API_3D Polynomial& operator+=(Polynomial& p, float scalar);

SVPOLYNOMIAL_API_3D Polynomial& operator-=(Polynomial& p, float scalar);

SVPOLYNOMIAL_API_3D Polynomial& operator*=(Polynomial& p, float scalar);

SVPOLYNOMIAL_API_3D Polynomial& operator/=(Polynomial& p, float scalar);
#pragma pack(pop)