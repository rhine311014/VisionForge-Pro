#pragma once

#include "svDef.h"
#include "svImage.h"
#include "svVector.h"
#include "svRect.h"
#include "svPose.h"

#pragma pack(push,8)

class SVPATTERNINSPECT_API scPatternInspect
{
public:
	enum Method					// 检测区域类型
	{
		eGreyRegion				= 0,
		eBlankRegion			= 1
	};

	enum DifferenceImageTypes	// 输出差异图像类型
	{
		eDifferenceImageAll			= -1,
		eDifferenceImageNone		=  0,
		eDifferenceImageAbsolute	=  1,
		eDifferenceImageBrighter	=  2,
		eDifferenceImageDarker		=  4
	};

public:
	scPatternInspect(Method method);
	~scPatternInspect();

public:
	void	SetSobelScale(double dSobelScale);
	double	GetSobelScale();

	void	SetSobelOffset(double dSobelOffset);
	double	GetSobelOffset();

	void	SetThresholdScale(double dThresholdScale);
	double	GetThresholdScale();

	void	SetThresholdOffset(double dThresholdOffset);
	double	GetThresholdOffset();

	bool	SetMaskImage(const scImageGray& mask, sc2Vector offset = sc2Vector(0,0));
	bool	ClearMaskImage();

	bool	Train(const scImageGray& image, scRect rect, const sc2Vector& refPoint);

	long	GetInspectRegionWidth();
	long	GetInspectRegionHeight();

	bool	SetReferencePoint(sc2Vector point);
	sc2Vector GetReferencePoint();
	sc2Vector GetOriginPoint();

	bool	StatisticalTrain(const scImageGray& image, scPose* pose);

	bool	UnTrain();

	bool	SaveTrainModelToFile(const char* strFilePath);
	bool	LoadTrainModelFromFile(const char* strFilePath);

	bool	SaveTrainModelToBinaryFile(const char* strFilePath);
	bool	LoadTrainModelFromBinaryFile(const char* strFilePath);

	long	GetTrainImageNum();

	bool	GetMaskImage(scImageGray& image);

	bool	GetPatternImage(scImageGray& image);

	bool	GetStandardDevImage(scImageGray& image);

	bool	GetThresholdImage(scImageGray& image);

public:
	void	EnableCorrelationScoreCompute(bool bComputeCorrelationScore);
	bool	IsComputeCorrelationScore();

	void	SetDifferenceImageType(DifferenceImageTypes nConsDifferenceImage);
	DifferenceImageTypes GetDifferenceImageType();

	void	SetNormalizationMode(NormalizationMode nConsNormalizationMode);
	NormalizationMode GetNormalizationMode();

	bool	SetTailHigh(long lTailHigh);
	long	GetTailHigh();

	bool	SetTailLow(long lTailLow);
	long	GetTailLow();

	bool Execute(const scImageGray& image, scPose* pose);

	double	GetCorrelationScore();

	bool	IsClipped();

	bool	GetClippedMaskImage(scImageGray& image);

	bool	GetMatchImage(scImageGray& image);

	bool	GetMatchImageNormalized(scImageGray& image);

	bool	GetDifferenceImageRawAbsolute(scImageGray& image);

	bool	GetDifferenceImageRawBrighter(scImageGray& image);

	bool	GetDifferenceImageRawDarker(scImageGray& image);

	bool	GetDifferenceImageAbsolute(scImageGray& image);

	bool	GetDifferenceImageBrighter(scImageGray& image);

	bool	GetDifferenceImageDarker(scImageGray& image);

private:
	Method	m_nMethod;
	void*	m_hPatInspect;
};

#pragma pack(pop)
