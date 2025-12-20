#pragma once

#include "svDef.h"
#include <vector>
#include "svFeature.h"
#include "vpImage.h"

#pragma pack(push,8)

class	scBlob;
class	scBlobSceneDescription;

class	svBlobTool;			//内部数据结构
class	svRLEBuffer;		//内部数据结构
struct	svVertex;			//内部数据结构

class SVBLOB_API scBlob  
{
public:
	scBlob();
	virtual ~scBlob();

	scBlob& operator= (scBlob* pBlob);
	bool	isFiltered();

	scBlob* pParent;
	scBlob* pPrevSibling;
	scBlob* pNextSibling;
	scBlob* pFirstChild;
	scBlob* pLastChild;
	long	lChildNum;

	//基本参数
	long	lID;
	BYTE	label;
	bool	isClipped;

	double	area;
	double	centerMassX;
	double	centerMassY;

	double	imageBoundCenterX;
	double	imageBoundCenterY;
	long	imageBoundMinX;
	long	imageBoundMaxX;
	long	imageBoundMinY;
	long	imageBoundMaxY;
	long	imageBoundWidth;
	long	imageBoundHeight;
	double	imageBoundAspect;

	//矩相关参数
	double	inertiaX;
	double	inertiaY;
	double	inertiaMin;
	double	inertiaMax;
	double	angle;
	double	elongation;

	double	boundPrincipalCenterX;
	double	boundPrincipalCenterY;
	double	boundPrincipalMinX;
	double	boundPrincipalMaxX;
	double	boundPrincipalMinY;
	double	boundPrincipalMaxY;
	double	boundPrincipalWidth;
	double	boundPrincipalHeight;
	double	boundPrincipalAspect;

	//轮廓参数
	long	boundaryPixelLength;
	double	perimeter;
	double	acircularity;
	double	acircularityRms;
	double	acircularityNormal;
	std::vector<POINT> contour;
	bool GetBlobFeaturelet(scFeatureletVector& chain);
	bool GetAngleBoundRect(double degAngle, sc2Vector& center, sc2Vector &size) const;	//degAngle为角度

	//区域
	bool GetBlobMaskImage(scImageGray& maskImageBlob, long& nOffsetX, long& nOffsetY) const;
	bool GetBlobMaskImage(cpImage& maskImageBlob, long& nOffsetX, long& nOffsetY) const;
	bool GetBlobGrayImage(const scImageGray inputImage, scImageGray& grayImageBlob, long& nOffsetX, long& nOffsetY) const;
	bool GetBlobGrayImage(const cpImage inputImage, cpImage& grayImageBlob, long& nOffsetX, long& nOffsetY) const;

	friend scBlobSceneDescription;

private:
	svVertex* pContour;					//内部参数

	long	horizontalSteps;			//内部参数
	long	verticalSteps;				//内部参数
	long	convexCorners;				//内部参数

	void AddChild(scBlob* pNewChild);	//内部调用函数

	long	lOriginID;					//内部参数
	bool	bSign;						//内部参数
	bool	bReComputeCt;				//内部参数
	double	inertiaXY;					//内部参数

	scBlobSceneDescription* scene_;		//内部参数

	std::vector<scBlob*> encloseList;					//内部参数
	void addEncloseObject(scBlob* pObjectEnclose);		//内部调用函数
	void deleteEncloseObject(scBlob* pObjectEnclose);	//内部调用函数
	void reflashEnclose(scBlob* pObject);				//内部调用函数
	scBlob* GetMaxEnclose(scBlob* pBlobRoot);			//内部调用函数
};

class SVBLOB_API scBlobSceneDescription  
{
public:
	scBlobSceneDescription();
	virtual ~scBlobSceneDescription();

	friend svBlobTool;
	
	enum Analysis
	{
		eWholeImageGreyScale,
		eGreyScale,
		eLabelled
	};
	
	enum ConnectCleanup
	{
		eNone,
		ePrune,
		eFill
	};

	Analysis connectivityKind() const { return _connectivityKind; };
	
	BYTE scaleVal() const { return _scaleVal; };
	
	ConnectCleanup cleanupKind() const { return _cleanupKind; };

	int minPels() const { return _minPels; };
	
	const scBlob* firstTop() const;

	const scBlob* lastTop() const;

	const scBlob* getBlob(int index, bool filtered = true) const;

	int numBlobs(bool filtered = true) const;

	scBlob * const * allBlobs(bool filtered = true) const;

	scFeatureletChainSet allBlobsContour(bool filtered = true, long lSmooth = 0) const;

	void filter(Measure measure, double loLimit, double hiLimit, bool invert=false);

	void clearFilters();

	void sort(Measure measure, SortOrder order);

	void clearSort();

	//区域
	bool GetRoiSegmentImage(scImageGray& maskImageBlob, int& nOffsetX, int& nOffsetY) const;
	bool GetRoiSegmentImage(cpImage& maskImageBlob, int& nOffsetX, int& nOffsetY) const;
	bool GetFullSegmentImage(const scImageGray inputImage, scImageGray& maskImageBlob, int& nOffsetX, int& nOffsetY) const;
	bool GetFullSegmentImage(const cpImage inputImage, cpImage& maskImageBlob, int& nOffsetX, int& nOffsetY) const;

protected:
	bool connectComponentsWholeImageGreyScale(const svRLEBuffer& rleBuffer, BYTE scaleVal);
	
	bool connectComponentsGreyScale(const svRLEBuffer& rleBuffer, BYTE scaleVal, ConnectCleanup cleanupType = eNone, int minPels = 4);
	
	bool connectComponentsLabelled(const svRLEBuffer& rleBuffer, ConnectCleanup cleanupType = eNone, int minPels = 4);

private:
	friend scBlob;

	bool _isDegenerate;

	bool	  _bSort;
	Measure	  _measure;
	SortOrder _order;
	
	bool _bComputeInertia;

	bool _bComputeContour;

	bool	_bTransform;
	double	_dOriginX;
	double	_dOriginY;
	double	_dTranX;
	double	_dTranY;
	double	_dAngle;
	double	_dSinA;	
	double	_dCosA;

	const svRLEBuffer* _rleBuffer;

	Analysis _connectivityKind;
	BYTE _scaleVal;
	ConnectCleanup _cleanupKind;
	int _minPels;

	scBlob* pBlobRoot;
	long lObjectNum;

	scBlob** pBlobUnFilter;
	long lUnFilterObjectNum;

	scBlob** pBlobFilter;
	long lFilterObjectNum;

	bool isDegenerate() const { return _isDegenerate; }
	
	void enableComputeInertia(bool bEnable = true) { _bComputeInertia = bEnable; }
	
	void enableComputeContour(bool bEnable = true) { _bComputeContour = bEnable; }
	
	void enableClientTransform(bool bTransform = false, double dOriginX = 0, double dOriginY = 0, double dTranX = 0, double dTranY = 0, double dAngle = 0);

	void cleanupNone();
	void cleanupPrune();
	void cleanupFill_GreyScale();
	void cleanupFill_Label();

	void createFillStack(std::vector<scBlob*> &svFillBlob, scBlob* pBlob);
	void fillBlob(scBlob* pBlobSmall, scBlob* pBlobBig);
};

#pragma pack(pop)
