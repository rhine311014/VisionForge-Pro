#pragma once

#include "svDef.h"
#include "svError.h"
#include "svAngle.h"
#include "svMath.h"
#include "svImage.h"
#include "vpImage.h"
#include "svVector.h"
#include "svAnnulusSection.h"
#include "svEllipseAnnulusSection.h"

#pragma pack(push,8)

class SVCORE_API scCircleSpreadParams
{
public:
	scCircleSpreadParams();

	scCircleSpreadParams(const scAnnulusSection &section, int xNumSamples, int yNumSamples);

	scCircleSpreadParams(const scCircleSpreadParams& params);

	scCircleSpreadParams& operator=(const scCircleSpreadParams& params);

	const scAnnulusSection &GetSection() const { return m_Section;}
	void SetSection(const scAnnulusSection &section) { m_Section = section; };

	int GetxNumSamples () const { return m_xNumSamples;}
	HStatus SetxNumSamples (int xNum);

	int GetyNumSamples () const { return m_yNumSamples;}
	HStatus SetyNumSamples (int yNum);

	bool WillClip(const scImageGray& srcImg) const;

	sc2Vector MapPolarPosition(const sc2Vector& polarPosition) const;

	sc2Vector MapImagePosition(const sc2Vector& imagePosition) const;

private:
	scAnnulusSection m_Section;
	int m_xNumSamples;
	int m_yNumSamples;
};

HStatus SVCORE_API sfCircleSpread(const scImageGray& srcimage, scImageGray& dstimage, const scCircleSpreadParams& params);
HStatus SVCORE_API sfCircleSpread(cpImage& srcimage, cpImage& dstimage, const scCircleSpreadParams& params);

class SVCORE_API scEllipseSpreadParams
{
public:
	scEllipseSpreadParams();

	scEllipseSpreadParams(const scEllipseAnnulusSection &section, int xNumSamples, int yNumSamples);

	scEllipseSpreadParams(const scEllipseSpreadParams& params);

	scEllipseSpreadParams& operator=(const scEllipseSpreadParams& params);

	const scEllipseAnnulusSection &GetSection() const { return m_Section;}
	void SetSection(const scEllipseAnnulusSection &section) { m_Section = section; };

	int GetxNumSamples () const { return m_xNumSamples;}
	HStatus SetxNumSamples (int xNum);

	int GetyNumSamples () const { return m_yNumSamples;}
	HStatus SetyNumSamples (int yNum);

	bool WillClip(const scImageGray& srcImg) const;

	sc2Vector MapPolarPosition(const sc2Vector& polarPosition) const;

	sc2Vector MapImagePosition(const sc2Vector& imagePosition) const;

private:
	scEllipseAnnulusSection m_Section;
	int m_xNumSamples;
	int m_yNumSamples;
};

HStatus SVCORE_API sfEllipseSpread(const scImageGray& srcimage, scImageGray& dstimage, const scEllipseSpreadParams& params);
HStatus SVCORE_API sfEllipseSpread(cpImage& srcimage, cpImage& dstimage, const scEllipseSpreadParams& params);

#pragma pack(pop)
