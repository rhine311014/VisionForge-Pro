#pragma once

#pragma pack(push,8)

#include <vector>
#include <array>

#ifdef SVLCPSOLVER_EXPORTS_3D
#define SVLCPSOLVER_API_3D __declspec(dllexport)
#else
#define SVLCPSOLVER_API_3D __declspec(dllimport)
#endif

class SVLCPSOLVER_API_3D LCPSolver
{
public:
	LCPSolver(int n);
	inline void SetMaxIterations(int maxIteration) {m_nMaxIterations = maxIteration>0 ? maxIteration : m_nDimension*m_nDimension;}
	inline int GetMaxIterations()const {return m_nMaxIterations;}
	inline int GetNumIterations()const {return m_nNumIterations;}
	inline float const& Augmented(int row, int col)const {return m_pAugmented[col +m_nNumCols *row];}
	inline float& Augmented(int row, int col) {return m_pAugmented[col +m_nNumCols *row];}

	enum Result
	{
		HAS_TRIVAL_SOLUTION, 
		HAS_NONTRIVAL_SOLUTION, 
		NO_SOLUTION,
		FAILED_TO_CONVERGE, 
		INVALID_INPUT
	};

	void MakeZero(float* poly);
	void Copy(const float* poly0, float*poly1);
	bool LessThan(const float* poly0, const float* poly1);
	bool LessThanZero(const float* poly);
	void Multiply(const float* poly, float scalar, float* product);

	bool Solve(const float* q, const float* M, float* w, float* z, Result* result);
	bool SolveVector(std::vector<float> const& q, std::vector<float> const& M, std::vector<float>& w, std::vector<float>& z, Result* result = nullptr);
private:

	struct Variable
	{
		char name;
		int index;
		int complementary;
		float* tuple;
	};

	std::vector<float*> m_VectorPoly;
	std::vector<float> m_VectorRatio;
	std::vector<float> m_VectorMinRatio;
	std::vector<float> m_VectorQMin;
	std::vector<float> m_VectorAugmented;

	std::vector<Variable> m_VectorVarBasic;
	std::vector<Variable> m_VectorVarNonBasic;

	int	m_nDimension;
	int m_nMaxIterations;
	int m_nNumIterations;
	int m_nNumCols;
	Variable*	m_pVarBasic;
	Variable*	m_pVarNonBasic;
	float*		m_pAugmented;
	float*		m_pQMin;
	float*		m_pMinRatio;
	float*		m_pRatio;
	float**		m_pPoly;
};


#pragma pack(pop)