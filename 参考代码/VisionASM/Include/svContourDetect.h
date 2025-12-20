#pragma once

#include "svRect.h"
#include "svImage.h"
#include "vpImage.h"
#include "svFeature.h"

#pragma pack(push,8)

#define     OCTLUT_LEN      8
#define     DIRLUT_LEN      16

struct sviInterpPoint;
struct sviContourPoint;
struct sviContourObject;

class SVCORE_API scContourDetect
{
public:
	scContourDetect();
	scContourDetect(unsigned char ucNoiseThre, long lContourlength);
	~scContourDetect();

	void SetNoiseThre(unsigned char ucNoiseThre);
	unsigned char GetNoiseThre();

	void SetLengthThre(long lLengthThre);
	long GetLengthThre();

	bool Execute(const scImageGray& image, scRect roi);
	bool Execute(const scImageGray& image, const scShape* roi = NULL);
	bool Execute(const scImageGray& image, scImageGray imageMask, const scShape* roi = NULL);
	bool Execute(cpImage& image, scRect roi);
	bool Execute(cpImage& image, const scShape* roi = NULL);
	bool Execute(cpImage& image, cpImage imageMask, const scShape* roi = NULL);

	bool GetContour(scFeatureletChainSet& contour);

	long GetContourCount();

	bool GetContourImage(scImageGray& image);
	bool GetContourImage(cpImage& image);

	void EnableXGradientMagThre(bool enable);
	bool IsEnableXGradientMagThre();
	void SetXGradientMagThre(int threuplimit , int thredownlimit);
	void GetXGradientMagThre(int & threuplimit, int & thredownlimit);

	void EnableYGradientMagThre(bool enable);
	bool IsEnableYGradientMagThre();
	void SetYGradientMagThre(int threuplimit,  int thredownlimit);
	void GetYGradientMagThre(int & threuplimit, int & thredownlimit);

	void EnableGradientAngleThre(bool enable);
	bool IsEnableGradientAngleThre();
	void SetGradientAngleThre(int threuplimit,  int thredownlimit);
	void GetGradientAngleThre(int & threuplimit, int & thredownlimit);

private:
	unsigned char	m_ucNoiseThre;			//噪声阈值
	long			m_lLengthThre;			//长度阈值
	long			m_lContourCount;

	static unsigned char m_ucOctantLUT[OCTLUT_LEN][2];
	static unsigned char m_ucDirctionLUT[DIRLUT_LEN];

	sviContourPoint*  m_pContourPoints;
	sviContourObject* m_pContourList;
	scImageGray		  m_ContourImage;

	bool            m_bEnableXGradientMagThre;
	int             m_nXGradientMagThre_UpLimit;
	int             m_nXGradientMagThre_DownLimit;

	bool            m_bEnableYGradientMagThre;
	int             m_nYGradientMagThre_DownLimit;
	int             m_nYGradientMagThre_UpLimit;

	bool            m_bEnableAgreeThre;
	int             m_nAgreeThreUpLimit;
	int             m_nAgreeThreDownLimit;

	void Release();

	void SobelEdegDetect(unsigned char* pbyImgIn, long lWidth, long lHeight, unsigned char*& pbyMagAngle);
	void SobelEdegDetect(unsigned char* pbyImgIn, long lWidth, long lHeight, unsigned char*& pbyMagAngle , short * & pbyMag_X,short * & pbyMag_Y);

	void PeakDetect(unsigned char* pbyMagAngle, long lWidth, long lHeight, sviInterpPoint** ppPeakPoints, long* plCount);
	void PeakDetect(unsigned char* pbyMagAngle, long lWidth, long lHeight, unsigned char* pbyImgMask, sviInterpPoint** ppPeakPoints, long* plCount);

	void PeakDetect(unsigned char* pbyMagAngle, short * pbyMag_X, short * pbyMag_Y , long lWidth, long lHeight, sviInterpPoint** ppPeakPoints, long* plCount);
	void PeakDetect(unsigned char* pbyMagAngle, short * pbyMag_X, short * pbyMag_Y, long lWidth, long lHeight, unsigned char* pbyImgMask, sviInterpPoint** ppPeakPoints, long* plCount);

	void SubPixelInterp(sviInterpPoint* pPeakPoints, long lCount, long lWidth, long lHeight, sviContourPoint** ppReturnPoints, long*& pnIndexImg);
	bool CreateContourList(sviContourPoint* pReturnPoints, long lCount, long* pnIndexImg, int nWidth, int nHeight, sviContourObject** ppContourList);

	bool ExecuteMask(const scImageGray& image, scImageGray imageMask);
	bool ExecuteMask(const scImageGray& image, scImageGray imageMask, scRect roi);

	void ReMapAgreeUpDownLimit(int uplimit,int downlimit,byte & mapeduplimit,byte & mappeddownlimit);
	bool JudgeIsInLimit(byte target,byte downlimit,byte uplimit);
};

#pragma pack(pop)
