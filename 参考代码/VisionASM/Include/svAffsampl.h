#pragma once

#include "svDef.h"
#include "svError.h"
#include "svAngle.h"
#include "svMath.h"
#include "svAffineRect.h"
#include "svImage.h"
#include "vpImage.h"

#pragma pack(push,8)

class SVCORE_API scAffineSamplingParams
{
public:
	scAffineSamplingParams();

	scAffineSamplingParams(const scAffineRect& affRect, int xNumSamples, int yNumSamples);

	scAffineSamplingParams(const scAffineSamplingParams& params);

	scAffineSamplingParams& operator=(const scAffineSamplingParams& params);

	const scAffineRect& GetAffineRectangle () const { return m_AffRect; };
	void SetAffineRectangle (const scAffineRect& affRect) { m_AffRect = affRect; };

	int GetxNumSamples () const { return m_xNumSamples; };
	HStatus SetxNumSamples (int xNum);
	int GetyNumSamples () const { return m_yNumSamples; };
	HStatus SetyNumSamples (int yNum);

	bool WillClip(const scImageGray& srcImg) const;

	void SetClipValue(unsigned char clipvalue);
	unsigned char GetClipValue() const;

	void SetInterpolationType(InterpolationType interpType);
	InterpolationType GetInterpolationType() const;

private:
	scAffineRect m_AffRect;
	int	m_xNumSamples;
	int	m_yNumSamples;

	unsigned char m_ucClipValue;
	InterpolationType m_interpType;
};

HStatus SVCORE_API sfAffineSampling(const scImageGray& srcimage, scImageGray& dstimage, const scAffineSamplingParams& params);
HStatus SVCORE_API sfAffineSampling(cpImage& srcimage, cpImage& dstimage, const scAffineSamplingParams& params);

HStatus SVCORE_API sfAffineSampling(const scImageGray& srcimage, scImageGray& dstimage, const scAffineSamplingParams& params, scImageGray& weights, bool& clipped);
HStatus SVCORE_API sfAffineSampling(cpImage& srcimage, cpImage& dstimage, const scAffineSamplingParams& params, cpImage& weights, bool& clipped);

#pragma pack(pop)
