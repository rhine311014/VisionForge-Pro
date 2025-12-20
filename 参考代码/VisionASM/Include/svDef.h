#pragma once

#pragma pack(push,8)

#ifdef SVCORE_EXPORTS
#define SVCORE_API __declspec(dllexport)
#else
#define SVCORE_API __declspec(dllimport)
#endif

#ifdef SVCORE_EXPORTS
#define SVPATTERNSEARCH_API __declspec(dllexport)
#else
#define SVPATTERNSEARCH_API __declspec(dllimport)
#endif

#ifdef SVCORE_EXPORTS
#define SVQUICKSEARCH_API __declspec(dllexport)
#else
#define SVQUICKSEARCH_API __declspec(dllimport)
#endif

#ifdef SVCORE_EXPORTS
#define SVFINESEARCH_API __declspec(dllexport)
#else
#define SVFINESEARCH_API __declspec(dllimport)
#endif

#ifdef SVCORE_EXPORTS
#define SVGEOMSEARCH_API __declspec(dllexport)
#else
#define SVGEOMSEARCH_API __declspec(dllimport)
#endif

#ifdef SVCORE_EXPORTS
#define SVBLOB_API __declspec(dllexport)
#else
#define SVBLOB_API __declspec(dllimport)
#endif

#ifdef SVCORE_EXPORTS
#define SV2DMEASURE_API __declspec(dllexport)
#else
#define SV2DMEASURE_API __declspec(dllimport)
#endif

#ifdef SVCORE_EXPORTS
#define SVPATTERNINSPECT_API __declspec(dllexport)
#else
#define SVPATTERNINSPECT_API __declspec(dllimport)
#endif

#ifdef SVCORE_EXPORTS
#define SVCOLOR_API __declspec(dllexport)
#else
#define SVCOLOR_API __declspec(dllimport)
#endif

#ifndef svStd
#define svStd std::
#endif

enum Segmentation
{
	eNone=0,
	eMap,
	eHardThresh,
	eSoftThresh,
	eThreshImage
};

enum MorphOp
{
	eDilateH=0,
	eDilateV,
	eDilateS,
	eErodeH,
	eErodeV,
	eErodeS,
	eOpenH,
	eOpenV,
	eOpenS,
	eCloseH,
	eCloseV,
	eCloseS
};

enum Measure
{
	//basic
	eLabel=0,	
	eArea,	
	eNumChildren,	
	eCenterMassX,	
	eCenterMassY,

	//Inertia
	eInertiaX,	
	eInertiaY,	
	eInertiaMin,	
	eInertiaMax,	
	eElongation,	
	eAngle,		

	// image-coordinate bounding-box information
	eImageBoundCenterX, 
	eImageBoundCenterY,
	eImageBoundMinX,
	eImageBoundMaxX,
	eImageBoundMinY,
	eImageBoundMaxY,
	eImageBoundWidth,
	eImageBoundHeight,
	eImageBoundAspect,

	// principal axis aligned extrema information
	eBoundPrincipalCenterX,
	eBoundPrincipalCenterY,
	eBoundPrincipalMinX,
	eBoundPrincipalMaxX,
	eBoundPrincipalMinY,
	eBoundPrincipalMaxY,
	eBoundPrincipalWidth,
	eBoundPrincipalHeight,
	eBoundPrincipalAspect,

	//Perimeter
	eBoundaryPixelLength,	
	ePerimeter,	
	eAcircularity,	
	eAcircularityRms,	
	eAcircularityNormal
};

//排序
enum SortOrder
{
	eDescending=0,          // largest first, smallest last
	eAscending
};

//排序
enum CoordSortMethod
{
	eCoordXOnly=0,			// 按X坐标排序
	eCoordYOnly,			// 按Y坐标排序
	eCoordXFirst,			// 先按X坐标排序,再按Y坐标排序
	eCoordYFirst			// 先按Y坐标排序,再按X坐标排序
};

//排序
enum MultiFitSortMethod
{
	eFitPointNum=0,			// 按拟合点数量由多到少排序
	eFitRmsError=1,			// 按拟合rms误差由小到大排序
};

// 边缘极性
enum EdgePolarity
{
	eDarkToLight=0,			// 由黑到白过渡的边缘类型
	eLightToDark,			// 由白到黑过渡的边缘类型
	eDontCare,				// 任意过渡的边缘类型
};

enum FitMethod
{
	eAlgebraicDistance = 0,	// 基于代数距离的拟合
	eGeometricDIstance		// 基于几何距离的拟合
};

enum OutLineMethod
{
	eCombination = 0,		//全组合
	eRansac,				//随机抽样
	eRobust,				//权重
	eRansacRobust,			//随机抽样权重
};

enum NormalizationMode		// 归一化操作类型
{
	eIdentity				= 0,
	eHistogramEqualization	= 1,
	eMeanAndStdDev			= 2,
	eMatchTails				= 3
};

// 图形的类型
enum ShapeType{
	eShape = 0,				// 基础图形
	ePoint = 1,				// 二维点
	eLine,					// 直线
	eLineSeg,				// 线段
	eRect,					// 矩形
	eAffineRect,			// 仿射矩形
	eCircle,				// 圆形
	eCircleArc,				// 圆弧
	eEllipse,				// 椭圆
	eEllipseArc,			// 椭圆弧
	eAnnulus,				// 圆环
	eAnnulusSection,		// 圆环段
	eEllipseAnnulus,		// 椭圆环
	eEllipseAnnulusSection, // 椭圆环段
	ePolyLine,				// 多边形
	eShapeTree,				// 图形树基类
	eGenShapeTree,			// 一般图形树
	eRegionTree,			// 区域图形树
	eContourTree,			// 轮廓图形树
	eObround,				// 长圆形
	eGenRect,				// 一般圆角矩形
	eCrossPoint,			// 十字交叉二维点
	eText,					// 文本
	eMaskRect,				// 带掩模区域的矩形
	eMaskAffineRect,		// 带掩模区域的仿射矩形
	eLineRay,				// 射线
	eSplineCurve,			// 样条曲线
	eShapeGroup,			// 图形组
	ePolylineArc,			// 多段线
};

enum CaliperSearchDirection
{
	eInward = 0,			// 卡尺的搜索方向向里，指向圆的中心
	eOutward				// 卡尺的搜索方向向外，由圆心指向外边
};

enum MorphStructType
{
	eHorizontalLine=0,		//1X3横线结构
	eVerticalLine,			//1X3竖线结构
	eDiagonalLine45deg,		//1X3对角线（45度方向）结构
	eDiagonalLine135deg,	//1X3对角线（135度方向）结构
	eDiamond,				//3X3菱形结构
	eSquare,				//3X3正方形结构
};

//镜像类型
enum MirrorType
{
	eNoMirror = 0,			//无镜像
	eHorMirror,				//水平镜像
	eVerMirror,		        //垂直镜像
	eHorVerMirror,          //水平垂直镜像
};

//特殊角度旋转类型
enum RotateType
{
	eRotate0 = 0,	        //旋转0度
	eRotate90,				//旋转90度
	eRotate180,		        //旋转180度
	eRotate270,				//旋转270度
};

//精度
enum Accuracy
{                                                                  
	AccuracyVeryFine	= 1,		// 高精
	AccuracyFine		= 2,		// 精细
	AccuracyCoarse		= 4			// 粗略
};

//最粗层
enum CoarseScale
{
	CoarseScale1  = 1,				// 原始分辨率
	CoarseScale2  = 2,				// 1/2采样
	CoarseScale4  = 3,				// 1/4采样
	CoarseScale8  = 4,				// 1/8采样
	CoarseScale16 = 5,				// 1/16采样
	CoarseScale32 = 6				// 1/32采样
};

enum InterpolationType{
	Nearest = 1,					// 最近邻
	Linear = 2,						// 线性插值
	Cubic = 3,						// 立方插值
};

#ifndef MAX3E
#define MAX3E(e1, e2, e3)           max((e1), max((e2), (e3)))
#endif

#ifndef MAX4E
#define MAX4E(e1, e2, e3, e4)       max((e1), (MAX3E(e2, e3, e4)))
#endif

#ifndef MAX5E
#define MAX5E(e1, e2, e3, e4, e5)   max((e1), (MAX4E(e2, e3, e4, e5)))
#endif

#ifndef MIN3E
#define MIN3E(e1, e2, e3)           min((e1), min((e2), (e3)))
#endif

#ifndef MIN4E
#define MIN4E(e1, e2, e3, e4)       min((e1), (MIN3E(e2, e3, e4)))
#endif

#ifndef MIN5E
#define MIN5E(e1, e2, e3, e4, e5)   min((e1), (MIN4E(e2, e3, e4, e5)))
#endif

#pragma warning(disable: 4018)
#pragma warning(disable: 4244)
#pragma warning(disable: 4251)
#pragma warning(disable: 4267)
#pragma warning(disable: 4541)
#pragma warning(disable: 4786)
#pragma warning(disable: 4800)
#pragma warning(disable: 4996)

#define JPEG_PRO

#pragma pack(pop)
