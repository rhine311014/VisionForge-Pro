#pragma once

#include "svDef.h"
#include "svAngle.h"
#include "svPolyLine.h"
#include "svImage.h"
#include "vpImage.h"

#pragma pack(push,8)

class SV2DMEASURE_API scCalibrateResult
{
public:
	enum CalibrateMode
	{                                                                  
		eNonlinearProject	= 1,		// 非线性投影模型
		eLinearProject		= 2,		// 线性投影模型
		eEasyDistortion		= 4			// 简单畸变模型
	};

public:
	scCalibrateResult(CalibrateMode mode = eNonlinearProject);
	~scCalibrateResult();

	scCalibrateResult(const scCalibrateResult& result);
	scCalibrateResult& operator=(const scCalibrateResult& result);

	CalibrateMode GetCalibrateMode();

	bool GetNonlinearProjectResult(double para[15]);
	bool GetLinearProjectResult(double para[6]);
	bool GetEasyDistortionResult(double& k);

	bool SetNonlinearProjectResult(const double para[15]);
	bool SetLinearProjectResult(const double para[6]);
	bool SetEasyDistortionResult(double k);

	double GetCoefficientXX();			// PXX
	double GetCoefficientXY();			// PXY
	double GetCoefficientXC();			// PXC
	double GetCoefficientYX();			// PYX
	double GetCoefficientYY();			// PYY
	double GetCoefficientYC();			// PYC
	double GetCoefficientDX();			// PDX
	double GetCoefficientDY();			// PDY
	double GetCoefficientK();			// K
	double GetTranslationX();			// Tx
	double GetTranslationY();			// Ty
	double GetScalingX();				// Sx
	double GetScalingY();				// Sy
	scRadian GetSkew();					// skew(弧度)
	scRadian GetRotate();				// rot(弧度)

	void   SetRMS(double rms);			// 设置RMS误差
	double GetRMS();					// 获取RMS误差

private:
	double	m_dRms;
	double*	m_pdCalibrateResult;
	CalibrateMode m_mode;
};

class SV2DMEASURE_API scFiducialCoord
{
public:
	sc2Vector m_Origin; // 基准标记点坐标
	scDegree  m_DegX;	// X边旋转角度
	scDegree  m_DegY;	// Y边旋转角度
	double	  m_dUnitX; // X方向单元长度
	double	  m_dUnitY; // Y方向单元长度
	sc2Vector m_Po;		// 原点坐标cornerPo
	sc2Vector m_Px;		// X方向角点坐标cornerPx
	sc2Vector m_Py;		// Y方向角点坐标cornerPy
};

enum TargetMode
{                                                                  
	eChessboard	= 1,	// 棋盘格靶标
	eDotTarget	= 2,	// 点阵靶标
	eGridLine	= 4		// 网格线靶标
};

class SV2DMEASURE_API scFiducialParam
{
public:
	scFiducialParam()
	{
		m_bFiducial		= false;
		m_ucNoiseThre	= 16;
		m_lengthlow		= 16;
		m_lengthhigh	= DBL_MAX;
		m_dFitThre		= 0.5;
		m_isAnnulus		= false;
		m_bXAxisLonger	= true;
		m_bFitCircleOrEllipse = true;
		m_Targetmode	= eChessboard;
		m_bCenter		= false;
	};

public:
	bool			m_bFiducial;	// 是否计算基准标记点
	unsigned char	m_ucNoiseThre;	// 噪声阈值
	double			m_lengthlow;	// 轮廓最短长度阈值
	double			m_lengthhigh;	// 轮廓最长长度阈值
	double			m_dFitThre;		// 拟合RMS误差阈值
	bool			m_isAnnulus;	// 是否为圆环（点阵靶标为圆环）
	bool			m_bXAxisLonger;	// X轴是否为标记点长轴:true为长轴;false为短轴
	bool			m_bFitCircleOrEllipse;// 拟合圆或者拟合椭圆:true为圆;false为椭圆
	TargetMode		m_Targetmode;	// 靶标类型

	bool			m_bCenter;		// 基准标记点是否在中心位置
									//(中心位置描述：棋盘格靶标的四个角点的中心;点阵靶标的点中心;网格线靶标的网格线四个交点中心)
	scFiducialCoord m_coord;		// 基准标记点
};

// 二维几何标定
SV2DMEASURE_API bool sfCalibtare(const scImageGray& image, const scPolyline& polyroi, int nCol, int nRow, double dXTitle, double dYTitle, scCalibrateResult& result,
								 bool bBlackCorner = true, bool bMorphProcess = false, int nMorphSize = 1, bool bSubPixel = false, int nWinSize = 7);

SV2DMEASURE_API bool sfCalibtare(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result);

SV2DMEASURE_API bool sfCalibtareQuad(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result, double thre=16, bool bOutputOutPoints=false);
SV2DMEASURE_API bool sfCalibtareQuad(cpImage& image, double dXTitle, double dYTitle, scCalibrateResult& result, double thre=16, bool bOutputOutPoints=false);

SV2DMEASURE_API bool sfCalibtareDotTarget(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result);
SV2DMEASURE_API bool sfCalibtareDotTarget(cpImage& image, double dXTitle, double dYTitle, scCalibrateResult& result);

SV2DMEASURE_API bool sfCalibtareGridLine(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result);
SV2DMEASURE_API bool sfCalibtareGridLine(cpImage& image, double dXTitle, double dYTitle, scCalibrateResult& result);

SV2DMEASURE_API bool sfCalibtareGridLine(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result, unsigned char ucNoiseThre,long lLengthThre);
SV2DMEASURE_API bool sfCalibtareGridLine(cpImage& image, double dXTitle, double dYTitle, scCalibrateResult& result, unsigned char ucNoiseThre,long lLengthThre);

SV2DMEASURE_API bool sfCalibtare(const scImageGray& image, const scPolyline& polyroi, int nCol, int nRow, double dXTitle, double dYTitle, scCalibrateResult& result,
								 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners,
								 bool bBlackCorner = true, bool bMorphProcess = false, int nMorphSize = 1, bool bSubPixel = false, int nWinSize = 7,scFiducialParam &param = scFiducialParam());

SV2DMEASURE_API bool sfCalibtare(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result,
								 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners,scFiducialParam &param = scFiducialParam());

SV2DMEASURE_API bool sfCalibtareQuad(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result,
								 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, double thre=16, bool bOutputOutPoints=false,scFiducialParam &param = scFiducialParam());
SV2DMEASURE_API bool sfCalibtareQuad(cpImage& image, double dXTitle, double dYTitle, scCalibrateResult& result,
								 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, double thre=16, bool bOutputOutPoints=false,scFiducialParam &param = scFiducialParam());

SV2DMEASURE_API bool sfCalibtareDotTarget(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result,
								 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners,scFiducialParam &param = scFiducialParam());
SV2DMEASURE_API bool sfCalibtareDotTarget(cpImage& image, double dXTitle, double dYTitle, scCalibrateResult& result,
	svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners,scFiducialParam &param = scFiducialParam());

SV2DMEASURE_API bool sfCalibtareGridLine(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result,
								 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners,scFiducialParam &param = scFiducialParam());
SV2DMEASURE_API bool sfCalibtareGridLine(cpImage& image, double dXTitle, double dYTitle, scCalibrateResult& result,
	svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners,scFiducialParam &param = scFiducialParam());

SV2DMEASURE_API bool sfCalibtareGridLine(const scImageGray& image, double dXTitle, double dYTitle, scCalibrateResult& result,
										 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, unsigned char ucNoiseThre,long lLengthThre,scFiducialParam &param = scFiducialParam());
SV2DMEASURE_API bool sfCalibtareGridLine(cpImage& image, double dXTitle, double dYTitle, scCalibrateResult& result,
	svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, unsigned char ucNoiseThre,long lLengthThre,scFiducialParam &param = scFiducialParam());

SV2DMEASURE_API bool sfCalibtare(const svStd vector<sc2Vector>& imgcorners, const svStd vector<sc2Vector>& objcorners, scCalibrateResult& result);

SV2DMEASURE_API bool sfFindChessboardCorners(const scImageGray& image, const scPolyline& polyroi, int nCol, int nRow, double dXTitle, double dYTitle,
								svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners,
								bool bBlackCorner = true, bool bMorphProcess = false, int nMorphSize = 1, bool bSubPixel = false, int nWinSize = 7);

SV2DMEASURE_API bool sfFindChessboardCorners(const scImageGray& image, double dXTitle, double dYTitle,
											 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners);

SV2DMEASURE_API bool sfFindChessboardCornersQuad(const scImageGray& image, double dXTitle, double dYTitle,
	svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, scRect& roiRect, double thre=16, bool bOutputOutPoints=false);
SV2DMEASURE_API bool sfFindChessboardCornersQuad(const scImageGray& image, double dXTitle, double dYTitle,
											 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, double thre=16, bool bOutputOutPoints=false);
SV2DMEASURE_API bool sfFindChessboardCornersQuad(cpImage& image, double dXTitle, double dYTitle,
											 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, double thre=16, bool bOutputOutPoints=false);

SV2DMEASURE_API bool sfFindDotTargetCenters(const scImageGray& image, double dXTitle, double dYTitle,
											 svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners);
SV2DMEASURE_API bool sfFindDotTargetCenters(cpImage& image, double dXTitle, double dYTitle,
	svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners);

SV2DMEASURE_API bool sfFindGridLineCorners(const scImageGray& image, double dXTitle, double dYTitle,
										   svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners);
SV2DMEASURE_API bool sfFindGridLineCorners(cpImage& image, double dXTitle, double dYTitle,
	svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners);

SV2DMEASURE_API bool sfFindGridLineCorners(const scImageGray& image, double dXTitle, double dYTitle,
											svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, unsigned char ucNoiseThre,long lLengthThre);
SV2DMEASURE_API bool sfFindGridLineCorners(cpImage& image, double dXTitle, double dYTitle,
	svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, unsigned char ucNoiseThre,long lLengthThre);

SV2DMEASURE_API bool sfFindCornerSubPixel(const scImageGray& image, svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, int nWinSize = 7, bool bGetError = false);
SV2DMEASURE_API bool sfFindCornerSubPixel(cpImage& image, svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, int nWinSize = 7, bool bGetError = false);

SV2DMEASURE_API bool sfCalculateRadio(scCalibrateResult& result, long lWidth, long lHeight, svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, double& dRadio);

// 标记点
SV2DMEASURE_API bool sfGetFiducialMark(const scImageGray& image, scFiducialCoord& coord, unsigned char ucNoiseThre = 16, double lengthlow = 16, double lengthhigh = DBL_MAX, double dFitThre = 0.5, bool isAnnulus = false, bool bXAxisLonger = true, bool bFitCircleOrEllipse = true);

// 标记点(计算特征点,仅支持棋盘格靶标)
SV2DMEASURE_API bool sfFindCornerFiducialMark(const scImageGray& image, scFiducialCoord& coord, unsigned char ucNoiseThre = 16, double lengthlow = 16, double lengthhigh = DBL_MAX, double dFitThre = 0.5, bool isAnnulus = false, bool bXAxisLonger = true, bool bFitCircleOrEllipse = true);

// 二维几何反投影
SV2DMEASURE_API bool sfInverseProject(const svStd vector<sc2Vector>& imgcorners, svStd vector<sc2Vector>& objcorners, scCalibrateResult result);

#pragma pack(pop)
