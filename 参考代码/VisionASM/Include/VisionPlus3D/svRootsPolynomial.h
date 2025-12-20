#pragma once

#pragma pack(push,8)

#pragma warning(disable:4244)

#include <vector>
#include <map>

#ifdef SVROOTSPOLYNOMIAL_EXPORTS_3D
#define SVROOTSPOLYNOMIAL_API_3D __declspec(dllexport)
#else
#define SVROOTSPOLYNOMIAL_API_3D __declspec(dllimport)
#endif

#ifndef svStd
#define svStd std::
#endif

#define GTE_ROOTS_LOW_DEGREE_BLOCK(block)

class SVROOTSPOLYNOMIAL_API_3D RootsPolynomial
{
public:
	static void SolveQuadratic(const float& p0, const float& p1,
		const float& p2, svStd map<float, int>& rmMap);

	static void SolveCubic(const float& p0, const float& p1,
		const float& p2, const float& p3, svStd map<float, int>& rmMap);

	static void SolveQuartic(const float& p0, const float& p1,
		const float& p2, const float& p3, 
		const float& p4, svStd map<float, int>& rmMap);

	static void GetRootInfoQuadratic(const float& p0, const float& p1,
		const float& p2, svStd vector<int>& info);

	static void GetRootInfoCubic(const float& p0, const float& p1,
		const float& p2, const float& p3, svStd vector<int>& info);

	static void GetRootInfoQuartic(const float& p0, const float& p1,
		const float& p2, const float& p3, 
		const float& p4, svStd vector<int>& info);

	static int Find(int degree, const float* c, unsigned int maxIterations, float* roots);

	static bool Find(int degree, const float* c, float tmin, float tmax,
		unsigned int maxIterations, float& root);

private:
	static void SolveDepressedQuadratic(const float& c0,
		svStd map<float, int>& rmMap);

	static void SolveDepressedCubic(const float& c0, const float& c1,
		svStd map<float, int>& rmMap);

	static void SolveDepressedQuartic(const float& c0, const float& c1,
		const float& c2, svStd map<float, int>& rmMap);

	static void SolveBiquadratic(const float& c0, const float& c2,
		svStd map<float, int>& rmMap);

	static void GetRootInfoDepressedQuadratic(const float& c0,
		svStd vector<int>& info);

	static void GetRootInfoDepressedCubic(const float& c0,
		const float& c1, svStd vector<int>& info);

	static void GetRootInfoDepressedQuartic(const float& c0,
		const float& c1, const float& c2, svStd vector<int>& info);

	static void GetRootInfoBiquadratic(const float& c0,
		const float& c2, std::vector<int>& info);

	static int FindRecursive(int degree, const float* c, float tmin, float tmax,
		unsigned int maxIterations, float* roots);

	static float Evaluate(int degree, const float* c, float t);
};


#pragma pack(pop)