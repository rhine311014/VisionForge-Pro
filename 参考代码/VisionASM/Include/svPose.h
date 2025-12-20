#pragma once

#include "svAngle.h"
#include "svVector.h"
#include <vector>

#pragma pack(push,8)

class SVCORE_API scPose
{
public:
	inline sc2Vector	GetPosition(); 

	inline scDegree		GetRotate();

	inline double		GetSize();

public:
	sc2Vector m_Position;
	scDegree  m_Rotate;
	double	  m_dSize;
};

// 按行按列排序
SVCORE_API bool sfSortByCoordXY(svStd vector<sc2Vector>& vResultArray, double dGap = 64, CoordSortMethod method = eCoordYFirst, SortOrder orderX = eAscending, SortOrder orderY = eAscending, BOOL bImageCoord = TRUE);
SVCORE_API bool sfSortByCoordXY(svStd vector<sc2Vector>& vResultArray, svStd vector<int>& vIndexArray, double dGap = 64, CoordSortMethod method = eCoordYFirst, SortOrder orderX = eAscending, SortOrder orderY = eAscending, BOOL bImageCoord = TRUE);

#pragma pack(pop)
