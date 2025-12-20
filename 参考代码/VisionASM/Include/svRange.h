#pragma once

#include "svDef.h"
#include "svAngle.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            范围scRange
// ---------------------------------------------------------------------
// 范围scRange用来表示一个取值范围，提供判断一个取值是否在范围内的判断，
// 还提供范围的膨胀和腐蚀的操作。
// scRange(2,3)包含2、3及其中间的任何取值，即2 <= x <= 3。
// 膨胀定义为MinkowskiSum(A,B) = { a + b | a in A, b in B}。
// 腐蚀定义为MinkowskiDifference(A,B) = { a - b | a in A, b in B}。

class SVCORE_API scRange
{
public:
	enum RangeType  
	{
		ePartial=0,			// 有限的区间
		eEmpty=1,			// 空区间
		eFull=2				// 充满整个空间的区间
	};

	static scRange FullRange();
	static scRange EmptyRange();

	scRange () : m_RangeType(eEmpty), m_dStart(0.), m_dEnd(0.) {};

	scRange (double start, double end)
	{
		if (start > end)
		{
			m_RangeType = eEmpty;
			m_dStart = 0.;
			m_dEnd = 0.;
		}
		else
		{
			m_RangeType = ePartial;
			m_dStart = start;
			m_dEnd = end;
		}
	};

	double Middle() const;

	double Length() const;

	bool IsWithin(double x) const ;

	scRange Intersect(const scRange &r) const;

	bool SomeOverlap(const scRange &r, double threshold=0) const;

	scRange Translate(double x) const;

	scRange Scale(double x) const;

	scRange Dilate(const scRange &r) const;

	scRange Erode(const scRange &r) const;

	RangeType Range() const { return m_RangeType;};

	bool CheckPartialRange() const 
	{ 
		return (m_RangeType == ePartial);
	}

	double Start() const { return m_dStart; };

	double End() const { return m_dEnd; };

	bool operator== (const scRange& that) const
	{
		bool b1 = (m_RangeType == that.Range());
		bool b2 = sfRealEqual(m_dStart, that.Start());
		bool b3 = sfRealEqual(m_dEnd, that.End());

		return (b1 && b2 && b3);
	}

	bool operator!= (const scRange& that) const
	{
		return !(*this == that);
	}

private:
	scRange(RangeType range) : m_RangeType(range), m_dStart(0.), m_dEnd(0.) {}

	RangeType	m_RangeType;
	double		m_dStart;
	double		m_dEnd;
};

// ---------------------------------------------------------------------
//                            角度范围scAngleRange
// ---------------------------------------------------------------------
// 角度范围scAngleRange用来表示一个角度的取值范围，提供判断一个角度取值
// 是否在范围内的判断，还提供角度范围的膨胀操作。
// scAngleRange(6.0,7.0)包含角度0.0，因为角度的周期为2PI，即0 == 2PI。

class SVCORE_API scAngleRange
{
public:
	enum RangeType  
	{
		ePartial=0,			// 有限的区间
		eEmpty=1,			// 空区间
		eFull=2				// 充满整个空间的区间
	};

	static scAngleRange FullAngleRange();
	static scAngleRange EmptyAngleRange();

	scAngleRange () : m_RangeType(eEmpty), m_AngleStart(0.), m_AngleEnd(0.) {}

	scAngleRange (const scRadian &start, const scRadian &end, bool bAllowStartEqualsEnd = false);

	scRadian Middle() const;

	scRadian Length() const;

	bool IsWithin(const scRadian &x) const;

	scAngleRange Dilate(const scAngleRange &x) const;

	RangeType Range() const { return m_RangeType;};

	bool CheckPartialRange() const 
	{ 
		return (m_RangeType == ePartial);
	}

	scRadian Start() const { return m_AngleStart; };

	scRadian End() const { return m_AngleEnd; };

	bool operator== (const scAngleRange& that) const
	{
		bool b1 = (m_RangeType == that.Range());
		bool b2 = sfRealEqual(m_AngleStart, that.Start());
		bool b3 = sfRealEqual(m_AngleEnd, that.End());

		return (b1 && b2 && b3);
	}

	bool operator!= (const scAngleRange& that) const
	{
		return !(*this == that);
	}

private:
	scAngleRange(RangeType range) : m_RangeType(range), m_AngleStart(0.), m_AngleEnd(0.) {}

	RangeType	m_RangeType;
	scRadian	m_AngleStart;
	scRadian	m_AngleEnd;
};

#pragma pack(pop)
