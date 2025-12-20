#pragma once

#include <vector>
#include <afxtempl.h>
#include "svVector.h"
#include "svMatrix.h"
#include "svFind.h"
#include "svGuiLine.h"
#include "svMeasure.h"
#include "svCalibrate.h"
#include "svImageWarp.h"
#include "svCodeCalibrate.h"
#include "svHomography.h"

#include "vsAlignToolDef.h"
#include "svXMLConfigurator.h"


// 标定模板类型
typedef enum enumCalibMarkPatternMode
{ 
	eTraditionalCalib		= 0,		// 传统模板标定
	eCalibUseDmCode			= 1,        // 二维码标定
	eWholeDmcodeBoardCalib	= 2,		// 全局二维码靶标标定

} CalibMarkPatternMode;

// 标定参数：主要是与平台、相机个数、相机平台类型有关
class VS_ALIGN_API CCalibrateParam
{
public:
	CCalibrateParam();
	virtual ~CCalibrateParam()/*{m_vstrCalibPatternIDS.clear();}*/;
	CCalibrateParam(const CCalibrateParam& lCP);
	CCalibrateParam& operator = (const CCalibrateParam& lCP);
	BOOL operator==(const CCalibrateParam& lCP);
	BOOL operator!=(const CCalibrateParam& lCP);

	void SetCalibPatternNum(int nNum);									// 设置标定模板的个数
	int GetCalibPatternNum();											// 获得标定模板的个数

	BOOL SetCalibPatternID(int nIndex, CString strID);					// 设置标定模板的ID
	CString GetCalibPatternID(int nIndex);								// 获取标定模板的ID

	void SetPlatformCalibMovement(CCoordPos	movement);					// 设置标定时的平台运动偏移量
	CCoordPos GetPlatformCalibMovement();								// 获取标定时的平台运动偏移量

	void SetCameraCalibMovement(CCoordPos	movement);					// 设置标定时的相机平台运动偏移量
	CCoordPos GetCameraCalibMovement();									// 设置标定时的相机平台运动偏移量

	void SetPlatformCalibrateRefine(BOOL bRefine);                      // 设置平台标定是否需要求精（Ture表求精，False表不求精）
	BOOL GetPlatformCalibrateRefine();                                  // 获取平台标定是否需要求精（Ture表求精，False表不求精）

	void SetPlatformCalibRefineMoveD(double dMoveD);					// 平台标定求精的D方向移动量(度)
	double GetPlatformCalibRefineMoveD();								// 获取标定求精的D方向移动量（度）

	void SetPlatformCalibRefineTime(int dRefineTime);					// 平台标定求精次数
	int GetPlatformCalibRefineTime();									// 获取标定标定求精次数(不是移动次数，移动次数需要根据算法确定)

	void SetSearchDelayTime(int nSearchDelayTime);						// 搜索延时（ms）
	int  GetSearchDelayTime();											// 获取搜索延时（ms）

	// 设置、获取标定类型
	void SetCalibMarkPatternMode(CalibMarkPatternMode mode);
	CalibMarkPatternMode GetCalibMarkPatternMode();


public:

	int				m_nCalibPatternNum;									// 标定模板ID的数量，与拍照的位置数量相关
	std::vector<CString> m_vstrCalibPatternIDS;							// 标定模板ID
	CCoordPos		m_dPlatformCalibMovement;							// 平台坐标系标定时各轴运动控制量
	CCoordPos		m_dCameraCalibMovement;								// 相机单独控制时，XY轴运动控制量
	double			m_dPlatformCalibRefineMoveD;						// 平台标定求精的D方向移动量
	int				m_nPlatformCalibRefineTime;							// 平台标定求精次数(不是移动次数，移动次数需要根据算法确定)
	int				m_nSearchDelayTime;									// 搜索延时（ms）

	BOOL            m_bPlatformCalibrateRefine;                         // 平台标定时是否需要求精

	int				m_nEnableDisCor;									// 畸形校正是否启用
	double			m_dTileX;											// 畸形校正棋盘格X长度
	double			m_dTileY;											// 畸形校正棋盘格Y长度
	bool			m_bOutPoints;										// 畸形校正是否启用外围点
	int				m_iThre;											// 畸形校正强度

	CalibMarkPatternMode m_eCalibMarkPatternMode;                      //标定模式（模板模式、二维码模式）

};


#define CALIBMOVEPOSNUM			12
#define	CALIBBOARDMARKPOSNUM	2	// 用靶标关联映射时Mark点位置数
// 单个相机的标定结果信息
class VS_ALIGN_API CCalibratedInfo
{
public:
	CCalibratedInfo();
	CCalibratedInfo(PlatformType ePlatformType, CamPlatformType eCamPlatformType);
	CCalibratedInfo(const CCalibratedInfo& lCInfo);
	virtual ~CCalibratedInfo();
	CCalibratedInfo& operator = (const CCalibratedInfo& lCInfo);
	BOOL operator==(const CCalibratedInfo& calibInfo);
	BOOL operator!=(const CCalibratedInfo& calibInfo);

	void	SetPlatformTransferMatrix(const sc2Matrix matrix);			// 设置、获取图像坐标系到平台坐标系之间的变换矩阵
	sc2Matrix	GetPlatformTransferMatrix() const;

	void SetPlatformTransferH(const double h[9]);// 设置、获取图像坐标系到平台坐标系之间的H变换矩阵
	bool GetPlatformTransferH(double h[9]) const;

	void SetPlatformTransferH1(const double h[9]);// 设置、获取图像坐标系到靶标系之间的H变换矩阵
	bool GetPlatformTransferH1(double h[9]) const;

	void SetPlatformTransferH2(const double h[9]);// 设置、获取靶标坐标系到平台坐标系之间的H变换矩阵
	bool GetPlatformTransferH2(double h[9]) const;

	void SetMarkPlatformCoordPosRefineOffset(const CCoordPos posOffset);	//设置、获取求精前后平台坐标的差值
	CCoordPos GetMarkPlatformCoordPosRefineOffset() const;
	void SetMarkPlatformCoordPosRefineOffsetH1(const CCoordPos posOffset);	//全局二维码标定H1矩阵误差
	CCoordPos GetMarkPlatformCoordPosRefineOffsetH1() const;
	void SetMarkPlatformCoordPosRefineOffsetH2(const CCoordPos posOffset);	//全局二维码标定H2矩阵误差
	CCoordPos GetMarkPlatformCoordPosRefineOffsetH2() const;

	void	SetMarkPlatformCoordPos(const CCoordPos pos);					// 设置、获取基准Mark在平台坐标系的坐标值和姿态
	CCoordPos	GetMarkPlatformCoordPos() const;

	void	SetMarkImgCoordPos(const CCoordPos pos);						// 设置、获取基准Mark在图像坐标系中的位置
	CCoordPos	GetMarkImgCoordPos() const;

	void	SetMarkPlatformCoordPosBat(const CCoordPos pos);				// 设置、获取基准Mark在平台坐标系的坐标值和姿态
	CCoordPos	GetMarkPlatformCoordPosBat() const;

	void	SetMarkImgCoordPosBat(const CCoordPos pos);						// 设置、获取基准Mark在图像坐标系中的位置
	CCoordPos	GetMarkImgCoordPosBat() const;


	void	SetTargetMarkImgCoordPos(const CCoordPos pos[CALIBMOVEPOSNUM]);	// 设置、获取目标Mark在图像坐标系中的位置
	bool	GetTargetMarkImgCoordPos(CCoordPos pos[CALIBMOVEPOSNUM]) const;

	void	SetObjectMarkImgCoordPos(const CCoordPos pos[CALIBMOVEPOSNUM]);	// 设置、获取对象Mark在图像坐标系中的位置
	bool	GetObjectMarkImgCoordPos(CCoordPos pos[CALIBMOVEPOSNUM]) const;



	void	SetPlatformAxisPos(const CPlatformAxisPos* pAxisPos);			// 设置、获取基准位置处平台各个轴的位置（XYD、UVW、DXY）
	CPlatformAxisPos*	GetPlatformAxisPos() const;


	void	SetCamPlatformTransferMatrix(const sc2Matrix matrix);			// 设置、获取相机坐标系到平台坐标系之间的变换矩阵
	sc2Matrix	GetCamPlatformTransferMatrix() const;

	void	SetCamPlatformAxisPos(CPlatformAxisPos* pAxisPos);	            // 设置、获基准位置处相机平台各个轴的位置（XY）
	CPlatformAxisPos* GetCamPlatformAxisPos();

	void	SetCamAxisPlatformAxisPos(CPlatformAxisPos* pAxisPos);			// 设置、获取相机轴所在平台的轴位置
	CPlatformAxisPos* GetCamAxisPlatformAxisPos();							

	void	SetCalibBoardMarkImgCoordPos(const CCoordPos pos[CALIBBOARDMARKPOSNUM]);	// 设置、获取用靶标关联映射时，标定板上的Mark位置图像坐标
	bool	GetCalibBoardMarkImgCoordPos(CCoordPos pos[CALIBBOARDMARKPOSNUM]) const;

	void	SetCalibBoardMarkBoardCoordPos(const CCoordPos pos[CALIBBOARDMARKPOSNUM]);	// 设置、获取用靶标关联映射时，标定板上的Mark位置物理坐标
	bool	GetCalibBoardMarkBoardCoordPos(CCoordPos pos[CALIBBOARDMARKPOSNUM]) const;

	void	SetCalibBoardSize(sc2Vector vBoardSize);	// 设置、获取用靶标矩形尺寸
	bool	GetCalibBoardSize(sc2Vector& vBoardSize);	// 设置、获取用靶标矩形尺寸


	void	SetValid(BOOL bValid);						// 设置标定信息是否有效	
	BOOL	IsValid();									// 获取标定信息是否有效	

	void ToString(CString & Str); //用于打印数据


public:
	double			m_PlatformTranferH[9];				// 标定的H矩阵
	double			m_PlatformTranferH1[9];				// 全局二维码靶标标定的H矩阵1(图像坐标到靶标坐标的映射矩阵)
	double			m_PlatformTranferH2[9];				// 全局二维码靶标标定的H矩阵2（靶标坐标到平台坐标的映射矩阵）
	sc2Matrix		m_PlatformTranferMatrix;			// 标定时，图像坐标系到平台坐标系之间的变换矩阵
	CCoordPos		m_cpMarkPlatformCoordPos;			// 标定时，基准Mark在平台坐标系的坐标值和姿态
	CCoordPos		m_cpMarkImgCoordPos;				// 标定时，基准Mark在图像坐标系中的位置
	CCoordPos		m_cpMarkPlatformCoordPosBat;			// 标定时，基准Mark在平台坐标系的坐标值和姿态(备份)
	CCoordPos		m_cpMarkImgCoordPosBat;				// 标定时，基准Mark在图像坐标系中的位置(备份)
	CCoordPos		m_cpObjectMarkImgCoordPos[CALIBMOVEPOSNUM];		// 标定时，对象(膜平面)Mark在图像坐标系中的位置
	CCoordPos		m_cpTargetMarkImgCoordPos[CALIBMOVEPOSNUM];		// 标定时，目标(孔、胶框平面)Mark在图像坐标系中的位置
	CPlatformAxisPos*	m_pPlatformAxisPos;				// 标定时，基准位置处平台各个轴的位置（XYD、UVW）
	double			m_dImageToPlatformAnlge;			// 标定时，图像坐标系与平台坐标系之间的角度

	sc2Matrix		m_CameraTranferMatrix;				// 标定时，相机坐标系到平台坐标系之间的变换矩阵
	CPlatformAxisPos*	m_pCamPlatformAxisPos;			// 标定时，基准位置处相机平台各个轴的位置（XY）
	CPlatformAxisPos*	m_pCamAxisPlatformAxisPos;		// 标定时，相机轴所在平台的轴位置

	CCoordPos		m_cpCalibBoardMarkImgCoordPos[CALIBBOARDMARKPOSNUM]; // 用靶标关联映射时，标定板上的Mark位置图像坐标
	CCoordPos		m_cpCalibBoardMarkBoardCoordPos[CALIBBOARDMARKPOSNUM]; // 用靶标关联映射时，标定板上的Mark位置物理坐标
	sc2Vector		m_vCalibBoardSize;					// 靶标矩形框尺寸

	CCoordPos		m_cpRefineOffset;					//求精前后平台坐标的差值
	CCoordPos		m_cpRefineOffsetH1;					//全局二维码靶标标定H1矩阵误差
	CCoordPos		m_cpRefineOffsetH2;					//全局二维码靶标标定H2矩阵误差
	BOOL			m_bValid;
	scCalibrateResult m_result;                         //畸变校正标定结果
};
 
typedef struct tagCameraParamSet
{
	double			m_dShutter;			// 曝光时间
	double			m_dGain;			// 增益
	double			m_dGamma;			// Gamma

	tagCameraParamSet()
	{
		m_dShutter      = 0.001;			// 曝光时间
		m_dGain         = 0;			// 增益
		m_dGamma		= 1;			// Gamma
	}

	tagCameraParamSet& operator= (const tagCameraParamSet &cameraParamSet)
	{
		if (this == &cameraParamSet)
		{
			return *this;
		}

		m_dShutter = cameraParamSet.m_dShutter;
		m_dGain = cameraParamSet.m_dGain;
		m_dGamma = cameraParamSet.m_dGamma;

		return *this;
	}
	
	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		// m_dShutter
		if(FALSE == cXMLConfigurator.FindElem(_T("dShutter")))
		{
			cXMLConfigurator.AddElem(_T("dShutter"));
		}
		cXMLConfigurator.SetElemData(m_dShutter);

		// m_dGain
		if(FALSE == cXMLConfigurator.FindElem(_T("dGain")))
		{
			cXMLConfigurator.AddElem(_T("dGain"));
		}
		cXMLConfigurator.SetElemData(m_dGain);

		// m_dGamma
		if(FALSE == cXMLConfigurator.FindElem(_T("dGamma")))
		{
			cXMLConfigurator.AddElem(_T("dGamma"));
		}
		cXMLConfigurator.SetElemData(m_dGamma);


	}
	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		// m_dShutter
		if (cXMLConfigurator.FindElem(_T("dShutter")))// Level 4
		{
			cXMLConfigurator.GetElemData(m_dShutter);	
		}

		// m_dGain
		if (cXMLConfigurator.FindElem(_T("dGain")))// Level 4
		{
			cXMLConfigurator.GetElemData(m_dGain);	
		}

		// m_dGamma
		if (cXMLConfigurator.FindElem(_T("dGamma")))// Level 4
		{
			cXMLConfigurator.GetElemData(m_dGamma);	
		}

	}
}CameraParamSet;

// 定位时切换曝光、增益、Gamma 是否启用
typedef struct tagCameraParamEnable
{
	BOOL			m_bShutterEnable;		// 启用曝光
	BOOL			m_bGainEnable;			// 启用增益
	BOOL			m_bGammaEnable;			// 启用Gamma

	tagCameraParamEnable()
	{
		m_bShutterEnable		= FALSE;			// 启用曝光
		m_bGainEnable			= FALSE;			// 启用增益
		m_bGammaEnable			= FALSE;			// 启用Gamma
	}

	tagCameraParamEnable& operator= (const tagCameraParamEnable &cameraParamEnable)
	{
		if (this == &cameraParamEnable)
		{
			return *this;
		}

		m_bShutterEnable = cameraParamEnable.m_bShutterEnable;
		m_bGainEnable = cameraParamEnable.m_bGainEnable;
		m_bGammaEnable = cameraParamEnable.m_bGammaEnable;

		return *this;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		// m_bShutterEnable
		if(FALSE == cXMLConfigurator.FindElem(_T("bShutterEnable")))
		{
			cXMLConfigurator.AddElem(_T("bShutterEnable"));
		}
		cXMLConfigurator.SetElemData(m_bShutterEnable);

		// m_bGainEnable
		if(FALSE == cXMLConfigurator.FindElem(_T("bGainEnable")))
		{
			cXMLConfigurator.AddElem(_T("bGainEnable"));
		}
		cXMLConfigurator.SetElemData(m_bGainEnable);

		// m_bGammaEnable
		if(FALSE == cXMLConfigurator.FindElem(_T("bGammaEnable")))
		{
			cXMLConfigurator.AddElem(_T("bGammaEnable"));
		}
		cXMLConfigurator.SetElemData(m_bGammaEnable);

	}
	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		// m_bShutterEnable
		if (cXMLConfigurator.FindElem(_T("bShutterEnable")))// Level 4
		{
			cXMLConfigurator.GetElemData(m_bShutterEnable);	
		}

		// m_bGainEnable
		if (cXMLConfigurator.FindElem(_T("bGainEnable")))// Level 4
		{
			cXMLConfigurator.GetElemData(m_bGainEnable);	
		}

		// m_bGammaEnable
		if (cXMLConfigurator.FindElem(_T("bGammaEnable")))// Level 4
		{
			cXMLConfigurator.GetElemData(m_bGammaEnable);	
		}
	}
}CameraParamEnable;



// 对位目标类型
typedef enum enumAlignerTargetType
{
	eTargetOnline = 0,
	eTargetBench
} AlignerTargetType;


// 对位对象类型
typedef enum enumAlignerObjectType
{
	eObjectOnline = 0,
	eObjectVirtual =1,
	eObjectBench,
} AlignerObjectType;


// 对位模式：对象到目标，点到点、点到边、边到边(*对象到目标*)
typedef enum enumAlignerMode
{ 
	eDotToDotAligner  = 0,
	eDotToLineAligner = 1,
	eLineToDotAligner = 2,
	eLineToLineAligner = 3,
	eDotToDotAligner_Optimize
} AlignerMode;


//  对位移动类型
typedef enum enumAlignerMoveType
{ 
	eAlignerMove_XYD	= 0,			// 对位时移动XYD
	eAlignerMove_XY		= 1,			// 对位时只移动XY，即只对位XY
	eAlignerMove_XD		= 2,			// 对位时只移动XD，即只对位XD
	eAlignerMove_YD		= 3,			// 对位时只移动YD，即只对位YD
	eAlignerMove_X		= 4,			// 对位时只移动X，即只对位X
	eAlignerMove_Y		= 5,			// 对位时只移动Y，即只对位Y
	eAlignerMove_D		= 6,			// 对位时只移动D，即只对位D
} AlignerMoveType;



// 关联定位模板类型
typedef enum enumCalibTargetMarkPatternMode
{ 
	eSinglePatternSingleMark		= 0,		// 单模板单Mark
	eDoublePatternSingleMark		= 1,		// 双模板单Mark
	eSinglePatternMultiMark			= 2,		// 单模板多Mark
	eSinglePatternGuanlian4Point	= 3,
	eDmCodeCalib					= 4,        // 二维码标定
//	eBoardCalib						= 5,        // 棋盘格标定
} CalibTargetMarkPatternMode;


// 对位补偿坐标系类型
typedef enum enumAlignerOffsetCoordType
{
	eMarkCoord = 0,			// Mark坐标系补偿
	ePlatformCoord,			// 平台坐标系补偿
	eMarkPlatformCoord,		// Mark平台混合坐标系补偿，角度用Mark坐标系补偿，XY用平台坐标系补偿
	eMarkCoordDiagonal,		// 对角Mark坐标系补偿(对角对位)		//20160510增加对角补偿
	eMarkCoordDiagonal2,	// 对角Mark坐标系补偿2(对角对位)	//20160510增加对角补偿
} AlignerOffsetCoordType;


//20160510增加对角补偿
class VS_ALIGN_API CProductSizeInfo
{
public:
	CProductSizeInfo();

	double  m_dObjectL1;				// 对象水平长度
	double  m_dObjectW1;				// 对象垂直长度
	double  m_dTargetL2;				// 目标水平长度
	double  m_dTargetW2;				// 目标垂直长度

	bool   m_bOffsetXYEnable;			// 平移设置开启			
	double m_dF1;						// 对象目标水平偏移
	double m_dF2;						// 对象目标垂直偏移
};


// 补偿设置（补偿的方向、X和Y补偿值对调）
class VS_ALIGN_API CProductOffsetInfo
{
public:
	CProductOffsetInfo();

	BOOL m_bEnableProductOffsetXDirection;
	BOOL m_bEnableProductOffsetYDirection;
	BOOL m_bEnableProductOffsetDDirection;

	BOOL m_bEnableProductOffsetXSetting;
	BOOL m_bEnableProductOffsetYSetting;
	BOOL m_bEnableProductOffsetDSetting;


};


//lzk 0405 对象目位置虚拟增加变量
class VS_ALIGN_API CProductPositionVirtualInfo
{
public:
	CProductPositionVirtualInfo();
	bool m_bEnableObjectPositionVirtualMode;	//是否启用对象位置虚拟
	bool m_bEnableTargetPositionVirtualMode;	//是否启用目标位置虚拟
	int m_nObjectPositionVirtualMode;			//对象位置虚拟类型
	int m_nTargetPositionVirtualMode;			//对位位置虚拟类型
};

class VS_ALIGN_API CAlignCheckInfo
{
public:
	CAlignCheckInfo();

	BOOL	m_bCheckTargetAndObjectDis; // 是否检测目标与对象之间的最大距离
	int		m_nCheckMode;				// 0：输入模式；1：检测模式

	BOOL	m_bCheckTargetDis;			// 是否检测目标的距离
	double  m_dTargetDisThreshold;
	double  m_dTargetDis0;				// 目标的距离0
	double  m_dTargetDis1;				// 目标的距离1
	double  m_dTargetDis2;				// 目标的距离2
	double  m_dTargetDis3;				// 目标的距离3
	double  m_dTargetDis4;				// 目标的距离4
	double  m_dTargetDis5;				// 目标的距离5

	BOOL	m_bCheckObjectDis;			// 是否检测对象的距离
	double  m_dObjectDisThreshold;
	double  m_dObjectDis0;				// 对象的距离0
	double  m_dObjectDis1;				// 对象的距离1
	double  m_dObjectDis2;				// 对象的距离2
	double  m_dObjectDis3;				// 对象的距离3
	double  m_dObjectDis4;				// 对象的距离4
	double  m_dObjectDis5;				// 对象的距离5

	BOOL	m_bCheckTargetAngle;		// 是否检测目标的角度
	double  m_dTargetAngleThreshold;
	double  m_dTargetAngle0;
	double  m_dTargetAngle1;
	double  m_dTargetAngle2;
	double  m_dTargetAngle3;
	double  m_dTargetAngle4;
	double  m_dTargetAngle5;
	

	BOOL	m_bCheckObjectAngle;		// 是否检测对象的角度
	double  m_dObjectAngleThreshold;
	double  m_dObjectAngle0;
	double  m_dObjectAngle1;
	double  m_dObjectAngle2;
	double  m_dObjectAngle3;
	double  m_dObjectAngle4;
	double  m_dObjectAngle5;
	

//	double  m_dMaxObjectAngle;			// 对象的最大角度(单位度)
	double  m_dMaxTargetAngle;			// 目标的最大角度(单位度)
//	double  m_dMaxTargetAndObjectDis;	// 目标与对象之间的最大距离；目标对象独立检查时为目标最大检查距离

//	BOOL    m_bEnableTarObjSeparateCheck;//是否允许目标和对象独立设置检查距离进行独立检查
//	double  m_dMaxObjectDis;             //对象最大距离

//	BOOL	m_bCheckPixelDis;			// 是否检测像素距离
//	double  m_dPixelDisX1;				// 像素距离
//	double  m_dPixelDisY1;
//	double  m_dPixelDisX2;
//	double  m_dPixelDisY2;

};


//  1---2     3---4
// 2/4相机时的2vs2对位模式类型
typedef enum enumAligner2Vs2Type
{ 
	eAlignerPos12_Auto				= 0,			// 位置1、2对位置3、4，两点中心+角度
	eAlignerPos12_12C1				= 1,			// 位置1、2对位置3、4，以角点位置1为基准+位置1和2角度
	eAlignerPos12_12C2				= 2,			// 位置1、2对位置3、4，以角点位置2为基准+位置1和2角度
	eAlignerPos12_12C1L2			= 3,			// 位置1、2对位置3、4，以角点位置1为基准+边2角度
	eAlignerPos12_12C2L1			= 4,			// 位置1、2对位置3、4，以角点位置2为基准+边1角度
	eAlignerPos12_12Dot1L2			= 5,			// 位置1、2对位置3、4，以任意点位置1为基准+边2角度
	eAlignerPos12_12Dot2L1			= 6,			// 位置1、2对位置3、4，以任意点位置2为基准+边1角度
	eAlignerPos12_L12L1				= 7,			// 位置1、2对位置3、4，以边1和边2交点为基准+边1角度
	eAlignerPos12_L12L2				= 8,			// 位置1、2对位置3、4，以边1和边2交点为基准+边2角度
	eAlignerPos12_L12				= 9,			// 位置1、2对位置3、4，以边1和边2交点为基准+边1和边2综合角度
	eAlignerPos12_CTo3				= 10,			// 位置1、2对位置3、4，角度通过1、2连线与3、4连线确定，位移通过1、2中点与3点确定
	eAlignerPos12_Combine_C12		= 11,			// 位置1、2对位置3、4，公共边上的点组合构成直线，垂直边上的点与角点连线，与公共直线计算交点，两点中心+角度
	eAlignerPos12_Combine_C12V		= 12,			// 位置1、2对位置3、4，公共边上的点组合构成直线，垂直边上的点向公共直线做垂线得到交点，两点中心+角度
	eAlignerPos12_Auto_Weigth		= 13,			// 位置1、2对位置3、4，两点中心+圆+角度;位置12基准中心对位_权重；√
	eAlignerPos12_12C1_Weigth		= 14,			// 位置1、2对位置3、4，以角点位置1+圆为基准+位置1和2角度;位置12基准角点1_权重；
	eAlignerPos12_12C2_Weigth		= 15,			// 位置1、2对位置3、4，以角点位置2+圆为基准+位置1和2角度;位置12基准角点2_权重；

} Aligner2Vs2Type;


//  1---2     4---5
//  |	|     |   |
//  3	3A	  6  6A
// 3/6相机时的4vs4对位模式类型
typedef enum enumAligner3Vs3Type
{ 
	eAlignerPos123_Auto				= 0,		// 位置1、2、3对位置4、5、6，居中
	eAlignerPos123_123C1			= 1,		// 位置1、2、3对位置4、5、6，以角点位置1为基准
	eAlignerPos123_123C2			= 2,		// 位置1、2、3对位置4、5、6，以角点位置2为基准
	eAlignerPos123_123C3			= 3,		// 位置1、2、3对位置4、5、6，以角点位置3为基准
	eAlignerPos123_123C13L12		= 4,		// 位置1、2、3对位置4、5、6，以角点位置1和3的中心为偏移，以位置1到位置2的边为角度
	eAlignerPos123_123C13AL12		= 5,		// 位置1、2、3对位置4、5、6，以角点位置1和3A的中心为偏移，以位置1到位置2的边为角度
	eAlignerPos123_123C2L13			= 6,		// 位置1、2、3对位置4、5、6，以角点位置2为基准，以位置1到位置3的边为角度
	eAlignerPos123_123C2TL13		= 7,		// 位置1、2、3对位置4、5、6，以角点位置2到Line（位置1到位置3建立的直线）做投影为基准，以位置1到位置3的边为角度

	//jjh0611  三个点 但不确定圆心的位置，圆心位置可能为位置1，位置2，位置3
	// 3v3视觉对位系统：对位方式使用 位置123_基准中心对位_权重 ，视觉补偿使用 对角Mark坐标D0 ；
	// 3A---1    6A---4
	//  |	|     |   |
	//  2	3 	  5   6
	eAlignerPos123_Auto_Weigth		= 8,			// 两角点中心对位+圆心 角度为两角点连线;位置123_基准中心对位_权重；√
	eAlignerPos123_123C1_Weigth		= 9,			// 点1对位+圆心  角度为两角点连线;位置123_基准角点1_权重；
	eAlignerPos123_123C2_Weigth		= 10,			// 点2对位+圆心  角度为两角点连线;位置123_基准角点2_权重 功能；
	eAlignerPos123_Auto_L1C_Weigth	= 11,			// 两角点中心对位+圆心,角度为第一个点与圆心连接的直线  // jjh0619位置123_基准中心边圆_权重；
} Aligner3Vs3Type;


//  1---2     5---6
//  |   |     |   |
//  3---4     7---8
// 4/8相机时的4vs4对位模式类型
typedef enum enumAligner4Vs4Type
{ 
	eAlignerPos1234_Auto				= 0,		// 位置1、2、3、4对位置5、6、7、8，居中
	eAlignerPos1234_123C1				= 1,		// 位置1、2、3对位置5、6、7，以角点位置1为基准
	eAlignerPos1234_124C2				= 2,		// 位置1、2、4对位置5、6、8，以角点位置2为基准
	eAlignerPos1234_134C3				= 3,		// 位置1、3、4对位置5、7、8，以角点位置3为基准
	eAlignerPos1234_234C4				= 4,		// 位置2、3、4对位置6、7、8，以角点位置4为基准
	eAlignerPos1234_Combine				= 5,		// 四个位置组合构成直线，两两相交重新构造对位点，长边对位角度，四边中点对位位移
	eAlignerPos1234_Combine_4Vs4		= 6,		// 四个位置组合构成直线，两两相交重新构造对位点，4点对4点
	eAlignerPos1234_Combine_Target_4Vs4 = 7,		// 目标四个位置组合构成直线，两两相交重新构造对位点，4点对4点
	eAlignerPos1234_Combine_Object_4Vs4 = 8,		// 对象四个位置组合构成直线，两两相交重新构造对位点，4点对4点
	eAlignerPos1234_1234_Weight			= 9,		// 均方误差最小对位角度，均方误差最小（X1,Y1）+ 圆（X2,Y2）,根据权重系数计算（X,Y）
	eAlignerPos1234_Combine_Weight		= 10,		// 长边对位角度，四个位置组合构成直线，四边中点（X1,Y1）+ 圆（X2,Y2），根据权重系数计算（X,Y）
	eAlignerPos1234_56_4V2				= 11,		// 位置1、2、3、4中心对位置5、6中心，位置1、2对位置5、6角度
	eAlignerPos1234_Center				= 12,		// 位置1、2、3、4中心对位5、6、7、8中心，长边对位角度（不重新构造点）
} Aligner4Vs4Type;

// 5/10相机时的5vs5对位模式类型
typedef enum enumAligner5Vs5Type
{ 
	eAlignerPos12345_Auto = 0,		// 位置1、2、3、4、5对位置6、7、8、9、10，居中
} Aligner5Vs5Type;

// 6/12相机时的6vs6对位模式类型
typedef enum enumAligner6Vs6Type
{ 
	eAlignerPos123456_Auto = 0,		// 位置1、2、3、4、5、6对位置7、8、9、10、11、12，居中
} Aligner6Vs6Type;


// 焊接参数
class VS_ALIGN_API CWeldInfo
{
public:
	CWeldInfo();

	// 专用焊接参数变量
	BOOL					m_bEnableWeld;						// 0.是否启用焊接参数

	CCoordPos				m_WeldMovementRatio;				// 1. 运动量系数
	CCoordPos				m_WeldAmmendMovementRatio;			// 2. 补偿系数

	int						m_nWeldTargetSearchTimes;			// 3. 目标多次定位/目标一次定位/目标零次定位
	int						m_nWeldSearchBench;					// 4. 对象基准/目标基准/无基准
	int						m_nWeldAngleBench;					// 5. 角度以对象为基准/角度以目标为基准/角度无基准
};

// 目标基准Mark修正信息
class VS_ALIGN_API CBenchTargetMarkFixInfo
{
public:
	CBenchTargetMarkFixInfo();	
	CBenchTargetMarkFixInfo(int nMarkNum, PlatformType ePlatformType);
	CBenchTargetMarkFixInfo(const CBenchTargetMarkFixInfo& lBTMFInfo);							
	virtual ~CBenchTargetMarkFixInfo();
	CBenchTargetMarkFixInfo& operator = (const CBenchTargetMarkFixInfo& lBTMFInfo);

public:
	void SetMarkNum(int nNum);										// 基准线Mark的数量
	int  GetMarkNum();

	BOOL SetMarkImageCoordPos(int nIndex, CCoordPos CoorPos);		// 基准线修复时，基准修正点在四个相机图像中的点的位置信息		
	CCoordPos GetMarkImageCoordPos(int nIndex);

	BOOL SetMarkPlatformCoordPos(int nIndex, CCoordPos CoorPos);	// 基准线修复时，基准修正点在全局平台坐标系中的点的位置信息
	CCoordPos GetMarkPlatformCoordPos(int nIndex);

	virtual BOOL SetFixPlatformPos(std::vector<CPlatformAxisPos*> pAxisPos);		// 基准线修复时，平台各轴所在的绝对位置信息
	virtual std::vector<CPlatformAxisPos*> GetFixPlatformPos();
	virtual CPlatformAxisPos* GetFixPlatformPos(int nIndex);
	virtual BOOL SetFixPlatformPos(int nIndex,CPlatformAxisPos* pAxisPos);

	virtual BOOL SetFixCamPlatformPos(int nIndex, CPlatformAxisPos* vAxisPos);		// 基准线修复时，相机各轴所在的绝对位置信息
	virtual CPlatformAxisPos* GetFixCamPlatformPos(int nIndex);

	BOOL IsValid() { return m_bIsValid; }
	void Validate(BOOL bIsValid) { m_bIsValid = bIsValid; }			// 基准修正完成后请调用此函数，标记此基准Mark修正信息为有效信息

protected:
	int						m_nBenchTargetMarkNum;					// 基准线Mark的数量
	std::vector<CCoordPos>	m_vBenchTargetMarkImageCoordPoses;		// 基准线修复时，基准修正点在四个相机图像中的点的位置信息
	std::vector<CCoordPos>	m_vBenchTargetMarkPlatformCoordPoses;	// 基准线修复时，基准修正点在全局平台坐标系中的点的位置信息
	std::vector<CPlatformAxisPos*>		m_pBenchFixPlatformPos;					// 基准线修复时，平台各轴所在的绝对位置信息
	std::vector<CPlatformAxisPos*>	m_vpBenchFixCamPlatformPos;		// 基准线修复时，相机平台各轴所在的绝对位置信息

	BOOL					m_bIsValid;								// 是否已经进行过基准修正，为有效的目标基准Mark修正信息
};


// 目标基准Mark拾取信息
#define	 BENCH_FIX_POSNUM 4
class VS_ALIGN_API CBenchTargetMarkPickInfo
{
public:
	CBenchTargetMarkPickInfo();	
	CBenchTargetMarkPickInfo(int nMarkNum, PlatformType ePlatformType);
	CBenchTargetMarkPickInfo(const CBenchTargetMarkPickInfo& lBTMFInfo);							
	virtual ~CBenchTargetMarkPickInfo();
	CBenchTargetMarkPickInfo& operator = (const CBenchTargetMarkPickInfo& lBTMFInfo);

public:
	void SetMarkNum(int nNum);										// 基准线Mark的数量
	int  GetMarkNum();

	BOOL SetMarkImageCoordPos(int nIndex, CCoordPos CoorPos,int nPos);		// 基准线修复时，基准修正点在四个相机图像中的点的位置信息		
	CCoordPos GetMarkImageCoordPos(int nIndex,int nPos);

	BOOL SetMarkPlatformCoordPos(int nIndex, CCoordPos CoorPos,int nPos);	// 基准线修复时，基准修正点在全局平台坐标系中的点的位置信息
	CCoordPos GetMarkPlatformCoordPos(int nIndex,int nPos);

	virtual BOOL SetFixPlatformPos(int nIndex,CPlatformAxisPos* pAxisPos,int nPos);				// 实时定位时，平台当前轴位置(搜索基准对象位置)
	virtual CPlatformAxisPos* GetFixPlatformPos(int nIndex,int nPos);
	virtual BOOL SetPickPlatformPos(int nIndex,CPlatformAxisPos* pAxisPos,int nPos);			// 基准拾取轴位置（获取拾取平台位置）
	virtual CPlatformAxisPos* GetPickPlatformPos(int nIndex,int nPos);
	BOOL SetPickPlatformOffset(int nIndex, CCoordPos CoorPos,int nPos);		// 	标准的对象Mark与拾取旋转中心之间的偏差
	CCoordPos GetPickPlatformOffset(int nIndex,int nPos);


	virtual BOOL SetFixPlatformPos(std::vector<CPlatformAxisPos*> pAxisPos,int nPos);		// 基准线修复时，平台各轴所在的绝对位置信息
	virtual std::vector<CPlatformAxisPos*> GetFixPlatformPos(int nPos);


	virtual BOOL SetPickPlatformPos(std::vector<CPlatformAxisPos*> pAxisPos,int nPos);		// 拾取时，平台各轴所在的绝对位置信息
	virtual std::vector<CPlatformAxisPos*> GetPickPlatformPos(int nPos);

	virtual BOOL SetFixCamPlatformPos(int nIndex, CPlatformAxisPos* vAxisPos);		// 基准线修复时，相机各轴所在的绝对位置信息
	virtual CPlatformAxisPos* GetFixCamPlatformPos(int nIndex);

	BOOL IsValid() { return m_bIsValid; }
	void Validate(BOOL bIsValid) { m_bIsValid = bIsValid; }			// 基准修正完成后请调用此函数，标记此基准Mark修正信息为有效信息

	std::vector<CCalibratedInfo*> m_vCalibratedInfo;

protected:
	int						m_nBenchTargetMarkNum;					// 基准线Mark的数量
	std::vector<CCoordPos>	m_vBenchTargetMarkImageCoordPoses[BENCH_FIX_POSNUM];		// 基准线修复时，基准修正点在四个相机图像中的点的位置信息
	std::vector<CCoordPos>	m_vBenchTargetMarkPlatformCoordPoses[BENCH_FIX_POSNUM];	// 基准线修复时，基准修正点在全局平台坐标系中的点的位置信息
	std::vector<CPlatformAxisPos*>		m_pBenchFixPlatformPos[BENCH_FIX_POSNUM];					// 基准线修复时，平台各轴所在的绝对位置信息
	std::vector<CPlatformAxisPos*>	m_vpBenchFixCamPlatformPos;		 // 基准线修复时，相机平台各轴所在的绝对位置信息


	// 拾取相关
	std::vector<CPlatformAxisPos*>		m_pBenchPickPlatformPos[BENCH_FIX_POSNUM];	// 拾取时，平台各轴所在的绝对位置信息
	// 	标准的对象Mark与拾取旋转中心之间的偏差
	std::vector<CCoordPos>	m_vBenchPickPlatformOffset[BENCH_FIX_POSNUM];							// 	标准的对象Mark与拾取旋转中心之间的偏差


	BOOL					m_bIsValid;								// 是否已经进行过基准修正，为有效的目标基准Mark修正信息
};

typedef struct TagBendInspectRangeInfo
{
	double m_dRangeLX[3];	// 0:基准值, 1:下限, 2:上限
	double m_dRangeLY[3];
	double m_dRangeLDis[3];

	double m_dRangeRX[3];	// 0:基准值, 1:下限, 2:上限
	double m_dRangeRY[3];
	double m_dRangeRDis[3];

	double m_dRangeTargetDis[3]; 
	double m_dRangeObjectDis[3];
	double m_dRangeSubtractX[3];
	double m_dRangeSubtractY[3];
	
	double m_dRangeCircleRadius[3];
	double m_dRangeCircleRadiusX[3];
	double m_dRangeCircleRadiusY[3];

	double m_dDeltaY[3];

	TagBendInspectRangeInfo()
	{
		double dValue[3] = {0.0,0.1,0.1};
		for(int i = 0; i < 3; i++)
		{
			m_dRangeLX[i]			= dValue[i];
			m_dRangeLY[i]			= dValue[i];
			m_dRangeLDis[i]			= dValue[i];

			m_dRangeRX[i]			= dValue[i];
			m_dRangeRY[i]			= dValue[i];
			m_dRangeRDis[i]			= dValue[i];

			m_dRangeTargetDis[i]	= dValue[i];
			m_dRangeObjectDis[i]	= dValue[i];
			m_dRangeSubtractY[i]	= dValue[i];
			m_dRangeSubtractX[i]	= dValue[i];

			m_dRangeCircleRadius[i] = dValue[i];
			m_dRangeCircleRadiusX[i] = dValue[i];
			m_dRangeCircleRadiusY[i] = dValue[i];

			m_dDeltaY[i] = dValue[i];
		}
	}

}BendInspectRangeInfo;

typedef struct _tagAlignOffsetByArea
{
	BOOL m_bEnable;			// 是否启用
	int m_nAreaRow;
	int m_nAreaCol;
	int m_nSearchType;       // 0 - 对象搜索定位;  1 - 目标搜索定位
	double m_dSearchStartX;
	double m_dSearchStartY;
	double m_dSearchWidth;
	double m_dSearchHeight;

	_tagAlignOffsetByArea()
	{
		m_bEnable = FALSE;
		m_nAreaCol = 1;
		m_nAreaRow = 1;
		m_nSearchType = 0;
		m_dSearchStartX = 10;
		m_dSearchStartY = 10;
		m_dSearchWidth = 300;
		m_dSearchHeight = 300;
	}

}AlignOffsetByAera;



class VS_ALIGN_API CBendInfo
{
public:
	CBendInfo();

	// 专用折弯参数
	BOOL			m_bBendAlignInspect;	// 对位过程中检测（FAAL）

	BOOL			m_bBendType;			// 图像坐标检测还是平台坐标检测；
	double			m_dBendImageRadio;		// 图像检测时的像素当量；
	BOOL			m_bBendAlignDmCode;		// 对位过程中使用二维码标定信息进行检测，需开启产品扩展、二维码标定功能；

	// 目标、对象距离补偿
	BOOL            m_bBendDisOffset;				// 是否启用距离补偿功能；
	double			m_dBendTargetDisOffset;			// 目标距离补偿值；
	double			m_dBendObjectDisOffset;			// 对象距离补偿值；

	// Lx、Ly、Rx、Ry距离补偿：目的是使Lx = Rx,Ly = Ry；
	BOOL			m_bBendResultOffset;
	sc2Vector		m_scBendLeftDisOffset;
	sc2Vector		m_scBendRightDisOffset;
	double			m_dLeftDisOffset;
	double			m_dRightDisOffset;
	BOOL			m_bEnableUseMathCaluLRDis;

	// 实时计算Lx、Ly、Rx、Ry：目的是让计算的四个值和标准阈值进行比对判定
	BOOL			m_bBendThreOffset;
	sc2Vector		m_scBendStanderThre;
	// 取消Y向绝对值使用
	BOOL m_bEnableFbsLRY;

	BOOL			m_bBendPressPara;
	BOOL			m_bBendPressParaWrite;
	long			m_lBendPressParaWriteAddre;
	double			m_dBendPressOKYmin;
	double			m_dBendPressOKYmax;			// Lx、Ly、Rx、Ry在阈值范围之内，不对位，直接压合；
	double			m_dBendPressNGYmin;
	double			m_dBendPressNGYmax;			// Lx、Ly、Rx、Ry在阈值范围之外，不对位，直接报警提示；

	BOOL			m_bBendLoadThre;
	sc2Vector		m_scBendLoadThre;

	BOOL			m_bBendingInspect;		// 对位过程中检测（FCHB）
	BOOL			m_bBendingProductID;	// 产品ID
	BOOL			m_bBendInspectDmCode;	// 检测过程中使用二维码标定信息进行检测，需开启产品扩展、二维码标定功能；
	BOOL			m_bEnableReadWsInfo;    // 折弯平台信息
	BOOL			m_bEnableReadInspectWsInfo;    // 本压平台信息
	BOOL			m_bEnableReadStartTimeInfo;    // 生产开始时间
	BOOL			m_bEnableReadEndTimeInfo;    // 生产结束时间
	BOOL			m_bEnableReadProsessingTimeInfo;    // 生产总时间

	BOOL			m_bEnableMoveSnap;		// 启用移动拍照 移动2个位置
	int				m_nPosIndexFisrt;		// 移动拍照起始位置

	BendInspectRangeInfo m_BendInspectRangeInfo;
	
	BOOL	m_bEnableAxisMoveLimit;			// 启用轴移动相对偏差量限制： 
											// 规则：多次对位时设置一个较小的限制，视觉给出的偏差量一旦超过限制，
											// 则视觉给出偏差以防止拉扯产品最大移动量发送，PLC移动该偏差量后再次触发拍照对位，多次以较小的移动量执行对位
											// 没有超过限制时，则发送正常的相对偏差量

	double  m_dAxisAllowMoveMaxX;			// 防止拉扯产品最大移动量+X  偏差量的绝对值;
	double  m_dAxisAllowMoveMaxY;			// 防止拉扯产品最大移动量+Y  偏差量的绝对值;
	double  m_dAxisAllowMoveMaxD;			// 防止拉扯产品最大移动量+D  偏差量的绝对值;

	BOOL m_bBendInspectSnCodeInvert;//高低位取反
	long m_bBendInspectSnCodeLength;//SN码长度
	BOOL m_bBendInspectGetSnOnce;//多次对位只获取一次SN码

	CString m_strPlatformName1;
	CString m_strPlatformName2;
	BOOL m_bEnableFbisTarObc;

	CString m_dBendSendszGetName;
	double m_dBendSendAdsorptionSite;//吸附位置
	double m_dBendSendRAngle;//R角
	double m_dBendSendCompensateY1;//补偿Y1
	double m_dBendSendCompensateY2;//补偿Y2
	double m_dBendSendCompensateY3;//补偿Y3
	double m_dBendSendCompensateZ1;//补偿Z1
	double m_dBendSendCompensateZ2;//补偿Z2
	double m_dBendSendCompensateZ3;//补偿Z3
};

//单个指令的控制情况
typedef struct _tagCurCMDPhotoLightCtrlSet
{
	std::vector<BOOL>     m_vCmdLightCtrlEnable;  //当前命令启用控制的控制器
	std::vector<std::vector<BOOL>>      m_vbChannelCtrlIndex;  //选中的控制通道
	std::vector<std::vector<BOOL>>      m_vbChannelOnorOff; //选中的控制通道亮灭状态

	_tagCurCMDPhotoLightCtrlSet()
	{
		for (int i =0; i < 8;++i)  //每个指令可对应8个控制器
		{
			m_vCmdLightCtrlEnable.push_back(FALSE);

			std::vector<BOOL> vChannelCtrlIndex;
			std::vector<BOOL> vChannelOnorOff;
			for (int i =0;i<8;++i)  //每个控制器8个通道的控制情况
			{
				vChannelCtrlIndex.push_back(FALSE);
				vChannelOnorOff.push_back(TRUE);
			}
			m_vbChannelCtrlIndex.push_back(vChannelCtrlIndex);
			m_vbChannelOnorOff.push_back(vChannelOnorOff);
		}
	}

}CurCMDPhotoLightCtrlSet;


typedef struct _tagPhotoLightCtrlSet
{
	int                                   m_nCmdSum;  //支持指令数量
	std::vector<CString>                  m_vCmd;  //支持的指令
	std::vector<CurCMDPhotoLightCtrlSet>  m_vCmdLightCtrlInfo;  //每一个指令对应的控制信息

	_tagPhotoLightCtrlSet()
	{
		m_nCmdSum = 3;
		m_vCmd.push_back(_T("对象拍照"));
		m_vCmd.push_back(_T("目标拍照"));
		m_vCmd.push_back(_T("检测拍照"));

		for (int i =0; i< m_nCmdSum;++i)
		{
			CurCMDPhotoLightCtrlSet Object;
			m_vCmdLightCtrlInfo.push_back(Object);
		}
	}

}PhotoLightCtrlSet;


// 单个产品对位信息
class VS_ALIGN_API CAlignerParam
{
public:
	CAlignerParam();
	virtual ~CAlignerParam();
	CAlignerParam(const CAlignerParam& lPTInfo);
	CAlignerParam& operator = (const CAlignerParam& lPTInfo);


public:

	void SetProductIndex(int nIndex);							// 产品索引号：1、2、3、4... ...		
	int	 GetProductIndex();										// 

	void SetProductUserName(CString strUserName);				// 产品用户自定义名称
	CString GetProductUserName();


	// 对位信息
	void SetAlignerPrecision(CCoordPos precision);					// 设置、获取对位精度
	CCoordPos GetAlignerPrecision();

	void SetAlignerMaxTime(int nTime);								// 设置、获取对位最大次数
	int GetAlignerMaxTime();

	void SetSearchDelayTime(int nMS);						// 设置获取搜索延时（ms）
	int  GetSearchDelayTime();

	void SetObjectSearchDelayTime(int nMS);						// 设置获取搜索延时（ms）
	int  GetObjectSearchDelayTime();

	void SetObjectSearchDelayTime2(int nMS);						// 设置获取搜索延时2（ms）
	int  GetObjectSearchDelayTime2();

	void SetEnableObjectSearchDelayTime2(BOOL bEnableObjectSearchDelayTime2);	
	BOOL  GetEnableObjectSearchDelayTime2();

	void SetAligner2Vs2Type(Aligner2Vs2Type type);			// 设置2Vs2的对位模式类型
	Aligner2Vs2Type GetAligner2Vs2Type();					// 获取2Vs2的对位模式类型

	void SetAligner3Vs3Type(Aligner3Vs3Type type);			// 设置3Vs3的对位模式类型
	Aligner3Vs3Type GetAligner3Vs3Type();					// 获取3Vs3的对位模式类型

	void SetAligner4Vs4Type(Aligner4Vs4Type type);			// 设置4Vs4的对位模式类型
	Aligner4Vs4Type GetAligner4Vs4Type();					// 获取4Vs4的对位模式类型

	void SetAligner5Vs5Type(Aligner5Vs5Type type);			// 设置5Vs5的对位模式类型
	Aligner5Vs5Type GetAligner5Vs5Type();					// 获取5Vs5的对位模式类型

	void SetAligner6Vs6Type(Aligner6Vs6Type type);			// 设置6Vs6的对位模式类型
	Aligner6Vs6Type GetAligner6Vs6Type();					// 获取6Vs6的对位模式类型

	void SetAlignerWeight(double dWeight);
	double GetAlignerWeight();

	void SetAlignerCirclePos(int nPos);
	int  GetAlignerCirclePos();


	void SetAlignerMoveType(AlignerMoveType type);			// 设置对位移动类型
	AlignerMoveType GetAlignerMoveType();					// 获取对位移动类型


	void SetEnableAlignAngleVectical(BOOL bAlignAngleVectical);	// 设置对位角度是否垂直（目标、对象角度相互垂直）
	BOOL GetEnableAlignAngleVectical();							// 获取对位角度是否垂直（目标、对象角度相互垂直）

	void SetAlignAngleVecticalMode(int nAlignAngleVecticalMode);	// 设置对位角度垂直模式
	int GetAlignAngleVecticalMode();							// 获取对位角度垂直模式


	void SetAlignerOffsetCoordType(AlignerOffsetCoordType type);	// 设置对位补偿类型
	AlignerOffsetCoordType GetAlignerOffsetCoordType();				// 获取对位补偿类型

	void SetAlignerOffset(CCoordPos Offset);						// 设置、获取对位补偿值
	CCoordPos GetAlignerOffset();

	void SetAlignerBaseOffset(CCoordPos Offset);					// 设置、获取对位基础补偿值
	CCoordPos GetAlignerBaseOffset();

	bool SetAlignerOffsetNum(int nNum);								// 设置、获取对位补偿值的数量
	int  GetAlignerOffsetNum();

	bool SetAlignerOffset(int nIndex, CCoordPos Offset);			// 设置、获取对位补偿值
	CCoordPos GetAlignerOffset(int nIndex);

	bool SetAlignerOffsetTip(int nIndex, CString strTip);			// 设置、获取对位补偿的备注
	CString GetAlignerOffsetTip(int nIndex);

	bool SetCurAlignerOffsetIndex(int nIndex);						// 设置、获得当前使用的对位补偿值
	int	 GetCurAlignerOffsetIndex();

	bool SetAlignerBenchOffsetNum(int nNum);						// 设置、获取基准中心对位补偿值的数量
	int  GetAlignerBenchOffsetNum();

	bool SetAlignerBenchOffset(int nIndex, CCoordPos BenchOffset);	// 设置、获取基准中心对位补偿值
	CCoordPos GetAlignerBenchOffset(int nIndex);

	bool SetCurAlignerBenchOffsetIndex(int nIndex);					// 设置、获得当前使用的基准中心对位补偿值
	int	 GetCurAlignerBenchOffsetIndex();

	CCoordPos GetAlignerBenchOffset();


	//八字对位参数设置
	void SetEightShapeAlignMode(int nMode);                       //设置八字对位模式 0 顺八字模式， 1 逆八字模式
	int  GetEightShapeAlignMode();                                //获取八字对位模式

	void EnableEightShapeAlign(BOOL bEnable);                     //设置八字对位是否有效
	BOOL GetEnableEightShapeAlign();                              //读取八字对位是否有效

	void   SetEightShapeAngle(double dAngle);                      // 设置八字角度
	double GetEightShapeAngle();                                   // 获取八字角度

	void EnableMulMarkMode(BOOL bEnable);                     //设置多mark点模式是否有效
	BOOL GetMulMarkMode();                                  //读取多Mark点模式是否有效


	//多次对位目标补偿
	void EnableAlnTargetOffset(BOOL bEnable);                     //设置多次对位目标补偿是否有效
	BOOL GetEnableAlnTargetOffset();                              //读取多次对位目标补偿是否有效

	void   SetAlnTargetOffsetX(double offsetX);                      // 设置多次对位X方向偏移量
	double GetAlnTargetOffsetX();                               // 获取多次对位X方向偏移量

	void   SetAlnTargetOffsetY(double dAngle);                      // 设置多次对位Y方向偏移量
	double GetAlnTargetOffsetY();                                   // 获取多次对位Y方向偏移量

	void EnableAlnSendTargetOffsetToPLC(BOOL bEnable);             //设置发送多次对位目标补偿给PLC是否有效，选中时不通过MMVA指令驱动平台运动到绝对位置，而是把绝对位置在对位成功后发送发给PLC备用轴位置
	BOOL GetEnableAlnSendTargetOffsetToPLC();                      //读取发送多次对位目标补偿给PLC是否有效，选中时不通过MMVA指令驱动平台运动到绝对位置，而是把绝对位置在对位成功后发送发给PLC备用轴位置
	//是否允许对位偏差方向取反
	void EnableAlnOffsetReverseX(BOOL bEnable);
	void EnableAlnOffsetReverseY(BOOL bEnable);
	void EnableAlnOffsetReverseD(BOOL bEnable);

	//获取是否允许对位偏差方向取反
	BOOL GetEnableAlnOffsetReverseX();
	BOOL GetEnableAlnOffsetReverseY();
	BOOL GetEnableAlnOffsetReverseD();

	//是否允许目标固定补偿
	void EnableAlnTargetFixOffset(BOOL bEnable);
	//获取是否允许目标固定补偿
	BOOL GetEnableAlnTargetFixOffset();

	void SetTargetOffsetType(int nOffsetType);		// 目标固定补偿方式，0：产品；1：二维码
	int GetTargetOffsetType();   

	bool SetCurAlignerTarFixOffsetIndex(int nIndex);						// 设置、获得当前使用的目标固定补偿序号
	int	 GetCurAlignerTarFixOffsetIndex();

	bool SetAlnTargetFixOffset(int nIndex, CCoordPos Offset);			// 设置目标固定补偿值
	CCoordPos GetAlnTargetFixOffset(int nIndex);						// 获取目标固定补偿值
	CCoordPos GetCurTargetFixOffset();									// 获取当前目标固定补偿值

	// 对位模式
	void SetAlignerMode(AlignerMode mode);							// 设置、获取对位模式
	AlignerMode GetAlignerMode();

	// 目标Mark 
	void SetTargetMarkType(AlignerTargetType type);					// 设置、获取目标类型
	AlignerTargetType GetTargetMarkType();

	// 对象Mark 
	void SetObjectMarkType(AlignerObjectType type);					// 设置、获取对象类型
	AlignerObjectType GetObjectMarkType();

	void SetTargetMarkPatternNum(int nNum);							// 设置、获取目标模板的数量
	int  GetTargetMarkPatternNum();

	BOOL SetTargetMarkPatternID(int nIndex, CString strID);			// 设置、获取目标模板的ID
	CString GetTargetMarkPatternID(int nIndex);

	// 候补目标Mark
	void SetTargetCandidateEnable(BOOL bEnable);					// 设置是否开启候补目标Mark搜索
	BOOL GetTargetCandidateEnable();

	void SetTargetCandidateSyncEnable(BOOL bEnable);				// 设置是否开启候补目标Mark成对搜索
	BOOL GetTargetCandidateSyncEnable();

	BOOL SetTargetCandidateOffsetNum(int nNum);								// 设置、获取目标候补补偿值的数量
	int  GetTargetCandidateOffsetNum();

	BOOL SetTargetCandidateOffset(int nIndex, CCoordPos Offset);			// 设置、获取目标候补补偿值
	CCoordPos GetTargetCandidateOffset(int nIndex);

	BOOL SetTargetCandidateOffsetEnable(int nIndex, BOOL bEnable);			// 设置、获取目标候补补偿值是否有效
	BOOL GetTargetCandidateOffsetEnable(int nIndex);


	BOOL SetTargetPosPatInfo(int nPosIndex, CPosPatInfo info);					// 设置、获取目标各位置模板信息
	CPosPatInfo GetTargetPosPatInfo(int nPosIndex);


	BOOL SetObjectPosPatInfo(int nPosIndex, CPosPatInfo info);					// 设置、获取对象各位置模板信息
	CPosPatInfo GetObjectPosPatInfo(int nPosIndex);


	// 基准Mark修正
	void SetBenchTargetMarkFixEnable(BOOL bEnable);					// 设置、获取是否支持基准修正
	BOOL GetBenchTargetMarkFixEnable();

	virtual BOOL SetBenchTargetMarkFixInfo(const CBenchTargetMarkFixInfo* pInfo);	// 设置、获取基准修正信息
	virtual CBenchTargetMarkFixInfo* GetBenchTargetMarkFixInfo();;

	// 基准Mark拾取
	void SetBenchTargetMarkPickEnable(BOOL bEnable);					// 设置、获取是否支持基准拾取
	BOOL GetBenchTargetMarkPickEnable();

	virtual BOOL SetBenchTargetMarkPickInfo(const CBenchTargetMarkPickInfo* pInfo);	// 设置、获取基准拾取信息
	virtual CBenchTargetMarkPickInfo* GetBenchTargetMarkPickInfo();

	

	// 对象Mark
	void SetObjectMarkPatternNum(int nNum);							// 设置、获取对象模板的数量
	int  GetObjectMarkPatternNum();

	BOOL SetObjectMarkPatternID(int nIndex, CString strID);			// 设置、获取对象模板的ID
	CString GetObjectMarkPatternID(int nIndex);

	// 候补对象Mark
	void SetObjectCandidateEnable(BOOL bEnable);					// 设置是否开启候补对象Mark搜索
	BOOL GetObjectCandidateEnable();

	void SetObjectCandidateSyncEnable(BOOL bEnable);				// 设置是否开启候补对象Mark成对搜索
	BOOL GetObjectCandidateSyncEnable();

	BOOL SetObjectCandidateOffsetNum(int nNum);								// 设置、获取对象候补补偿值的数量
	int  GetObjectCandidateOffsetNum();

	BOOL SetObjectCandidateOffset(int nIndex, CCoordPos Offset);			// 设置、获取对象候补补偿值
	CCoordPos GetObjectCandidateOffset(int nIndex);

	BOOL SetObjectCandidateOffsetEnable(int nIndex, BOOL bEnable);			// 设置、获取对象候补补偿值是否有效
	BOOL GetObjectCandidateOffsetEnable(int nIndex);


	// 对象虚拟Mark
	void SetVirtualObjectMarkEnable(BOOL bEnable);					// 设置、获取是否支持虚拟Mark
	BOOL GetVirtualObjectMarkEnable();

	void SetVirtualObjectMarkPatternNum(int nNum);					// 设置、获取虚拟Mark模板的数量
	int  GetVirtualObjectMarkPatternNum();								

	BOOL SetVirtualObjectMarkPatternID(int nIndex, CString strID);	// 设置、获取虚拟Mark模板的ID
	CString GetVirtualObjectMarkPatternID(int nIndex);

	BOOL SetVirtualObjectMarkOffset(int nIndex, CCoordPos offset);	// 设置、获取虚拟Mark模板的偏移量
	CCoordPos GetVirtualObjectMarkOffset(int nIndex);


	// 关联标定Mark
	void SetTargetObjectCamSeparateEnable(BOOL bEnable);			// 设置、获取目标相机和对象相机是否各自独立的
	BOOL GetTargetObjectCamSeparateEnable();

	// 设置是否启用标定板关联
	virtual void SetTargetCalibUseBoardEnable(BOOL bEnable);
	virtual BOOL GetTargetCalibUseBoardEnable();

	void SetTargetUseFixedVirtualCoordinates(BOOL bTargetUseFixedVirtualCoordinates);
	BOOL GetTargetUseFixedVirtualCoordinates();

	void SetCalibTargetMarkEnable(BOOL bEnable);					// 设置、获取是否使用关联标定Mark
	BOOL GetCalibTargetMarkEnable();

	void SetCalibTargetMarkPatternMode(CalibTargetMarkPatternMode mode);	// 设置、获取关联标定模板类型
	CalibTargetMarkPatternMode GetCalibTargetMarkPatternMode();

	void SetCalibTargetMarkPatternNum(int nNum);					// 设置、获取关联标定Mark模板的数量
	int  GetCalibTargetMarkPatternNum();								

	BOOL SetCalibTargetMarkPatternID(int nIndex, CString strID);	// 设置、获取关联标定Mark模板的ID
	CString GetCalibTargetMarkPatternID(int nIndex);

	void SetCalibTargetDMCodeOutLineEnable(BOOL bEnable); // 设置、获取二维码关联时是否忽略点
	BOOL GetCalibTargetDMCodeOutLineEnable()const{return m_bEnableCalibTargetDMCodeOutLine;}

	void SetCalibTargetDMCodeOutLinePercent(double dVal);// 设置、获取二维码关联时是否忽略点百分比
	double GetCalibTargetDMCodeOutLinePercent()const{return m_dCalibTargetDMCodeOutLinePercent;}


	// 检测模板设置
	void SetSysPlatformInspectEnable(BOOL bEnable);					// 设置、获取检测模板是否启用(系统平台信息配置选项)
	BOOL GetSysPlatformInspectEnable();

	void SetInspectCandidateEnable(BOOL bEnable);					// 设置是否开启候补检测Mark搜索
	BOOL GetInspectCandidateEnable();

	void SetInspectCandidateSyncEnable(BOOL bEnable);				// 设置是否开启候补检测Mark成对搜索
	BOOL GetInspectCandidateSyncEnable();

	void SetInspectSingleSearchEnable(BOOL bEnable);				// 设置是否开启候补检测Mark单独搜索
	BOOL GetInspectSingleSearchEnable();

	void SetInspectMarkPatternNum(int nNum);						// 设置、获取检测Mark模板的数量
	int  GetInspectMarkPatternNum();	

	BOOL SetInspectMarkPatternID(int nIndex, CString strID);		// 设置、获取检测Mark模板的ID
	CString GetInspectMarkPatternID(int nIndex);

	BOOL SetInspectCandidateOffsetNum(int nNum);					// 设置、获取检测候补补偿值的数量
	int  GetInspectCandidateOffsetNum();

	BOOL SetInspectCandidateOffsetEnable(int nIndex, BOOL bEnable);	// 设置、获取检测候补补偿值是否有效
	BOOL GetInspectCandidateOffsetEnable(int nIndex);

	void SetProductSizeInfo(CProductSizeInfo productSizeInfo);		// 设置、获取产品尺寸信息	//20160510增加对角补偿
	CProductSizeInfo GetProductSizeInfo();


	void SetPositionModeInfo(int nPosMode);							// 设置、获取产品拍照位置信息	//20160510增加对角补偿
	int GetPositionModeInfo();										// 0:左上、右下；１：右上、左下

	void SetProductOffsetInfo(CProductOffsetInfo productOffsetInfo);// 补偿设置（补偿的方向、X和Y补偿值对调）
	CProductOffsetInfo GetProductOffsetInfo();

	// 模板曝光
	BOOL SetObjectSearchShutter(int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter);			// 设置、获取对象模板曝光时间
	CameraParamSet GetObjectSearchShutter(int nPatternIndex, int nCandidateIndex);

	BOOL SetTargetSearchShutter(int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter);			// 设置、获取目标模板曝光时间
	CameraParamSet GetTargetSearchShutter(int nPatternIndex, int nCandidateIndex);

	BOOL SetCalibTargetSearchShutter(int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter);		// 设置、获取标定目标模板曝光时间
	CameraParamSet GetCalibTargetSearchShutter(int nPatternIndex, int nCandidateIndex);

	BOOL SetVirtualObjectSearchShutter(int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter);	// 设置、获取虚拟对象模板曝光时间
	CameraParamSet GetVirtualObjectSearchShutter(int nPatternIndex, int nCandidateIndex);

	BOOL SetInspectSearchShutter(int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter);			// 设置、获取检测模板曝光时间
	CameraParamSet GetInspectSearchShutter(int nPatternIndex, int nCandidateIndex);


	BOOL SetEnableObjectSearchShutter(int nIndex, CameraParamEnable bEnable);			// 设置、获取对象候补补偿值是否有效
	CameraParamEnable GetEnableObjectSearchShutter(int nIndex);

	BOOL SetEnableTargetSearchShutter(int nIndex, CameraParamEnable bEnable);			
	CameraParamEnable GetEnableTargetSearchShutter(int nIndex);

	BOOL SetEnableCalibTargetSearchShutter(int nIndex, CameraParamEnable bEnable);			
	CameraParamEnable GetEnableCalibTargetSearchShutter(int nIndex);

	BOOL SetEnableVirtualObjectSearchShutter(int nIndex, CameraParamEnable bEnable);			// 设置、获取对象候补补偿值是否有效
	CameraParamEnable GetEnableVirtualObjectSearchShutter(int nIndex);

	BOOL SetEnableInspectSearchShutter(int nIndex, CameraParamEnable bEnable);			// 设置、获取检测候补补偿值是否有效
	CameraParamEnable GetEnableInspectSearchShutter(int nIndex);

	void SetEnableMutiTarget(BOOL bEnable);                // 设置、获取是否启用多目标对位
	BOOL GetEnableMutiTarget();

	void SetAlignAccordingToPickPlatformEnable(BOOL bEnable);
	BOOL GetAlignAccordingToPickPlatformEnable();

	void SetWeldInfo(CWeldInfo WeldInfo);	// 设置、获取焊接参数
	CWeldInfo GetWeldInfo();

	void SetProductPositionVirtualInfo(CProductPositionVirtualInfo productPositionVirtualInfo);		// 设置、获取产品尺寸信息
	CProductPositionVirtualInfo GetProductPositionVirtualInfo();

	void SetBendInfo(CBendInfo BendInfo);															// 设置、获取折弯参数
	CBendInfo GetBendInfo();

	void SetAlignOffsetByArea(AlignOffsetByAera& alignOffsetByArea);
	AlignOffsetByAera GetAlignOffsetByArea();

	void SetLightCtrlInfo(PhotoLightCtrlSet& cObj);															// 设置、获取流程光源控制参数
	PhotoLightCtrlSet GetLightCtrlInfo();

public:

	// 产品信息
	int					m_nAlignerIndex;				// 产品索引号：1、2、3、4... ...
	CString				m_strAlignerCustomName;			// 产品用户自定义名称
	AlignerMode			m_eAlignerMode;					// 对位模式，点到点，点到线，线到线
	// 对位精度
	CCoordPos			m_AlignerPrecision;				// 对位精度参数设置
	int					m_nAlignerMaxTime;			    // 最大对位次数

	int					m_nSearchDelayTime;				// 搜索延时(ms)
	int					m_nObjectSearchDelayTime;		// 对象搜索延时(ms)
	int					m_nObjectSearchDelayTime2;		// 对象搜索延时2(ms)
	BOOL				m_bEnableObjectSearchDelayTime2;

	Aligner2Vs2Type		m_eAligner2Vs2Type;				// 2/4相机时的2vs2对位模式类型
	Aligner3Vs3Type		m_eAligner3Vs3Type;				// 3/6相机时的3vs3对位模式类型
	Aligner4Vs4Type		m_eAligner4Vs4Type;				// 4/8相机时的4vs4对位模式类型
	Aligner5Vs5Type		m_eAligner5Vs5Type;				// 5/10相机时的5vs5对位模式类型
	Aligner6Vs6Type		m_eAligner6Vs6Type;				// 6/12相机时的6vs6对位模式类型

	double m_dAlignerWeight;
	int    m_nAlignerCirclePos;

	AlignerMoveType		m_eAlignerMoveType;				// 对位移动类型


	// 对位角度设置（目标、对象角度相互垂直）
	BOOL				m_bAlignAngleVertical;
	int                 m_nAlignAngleVerticalMode;			// 0:对象到目标Mark角度（+90°），即目标Mark角度-对象Mark角度=90°；1：对象到目标Mark角度（-90°），即目标Mark角度-对象Mark角度=-90°

	// 对位补偿
	AlignerOffsetCoordType	m_eAlignerOffsetCoordType;	// 对位补偿坐标系类型：0为Mark坐标系，1为平台坐标系
	CCoordPos			m_AlignerOffset;				// 对位补偿参数设置

	CCoordPos			m_AlignerBaseOffset;			// 对位基础补偿参数设置

	//八字对位补偿设置
	BOOL                m_bCheckEightShapeAlign;
	int                 m_nEightShapeAlignMode;
	double              m_dEightShapeAngle;

	//多Mark点模式
	BOOL               m_bDiffPosWithSameSearchResultEnabled;

	//多次对位目标补偿
	BOOL  m_bCheckAlnTargetOffsetEnable;
	double m_dAlnTargetOffsetX;
	double m_dAlnTargetOffsetY;
	BOOL  m_bCheckAlnSendTargetOffsetToPLCEnable;	// 选中时不通过MMVA指令驱动平台运动到绝对位置，而是把绝对位置在对位成功后发送发给PLC备用轴位置

	//是否允许目标固定补偿
	BOOL m_bEnableAlnTargetFixOffset;
	std::vector<CCoordPos> m_vAlnTargetFixOffset;//目标固定补偿值
	int m_nTargetFixOffsetType;

	//是否允许平台移动量XYD反置
	BOOL m_bAlignOffsetReverseX; 
	BOOL m_bAlignOffsetReverseY;
	BOOL m_bAlignOffsetReverseD;

	int						m_nAlignerOffsetNum;		// 对位补偿数量
	std::vector<CCoordPos>  m_vAlignerOffset;			// 对位补偿（对象）
	std::vector<CString>	m_vAlignerOffsetTips;			// 对位补偿备注

	int						m_nCurAlignerOffsetIndex;		// 对位补偿数量
	int                     m_nCurAlignerTarFixOffsetIndex;  // 当前目标固定补偿序号
	int						m_nAlignerBenchOffsetNum;		// 基准中心对位补偿数量
	std::vector<CCoordPos>  m_vAlignerBenchOffset;			// 基准中心对位补偿（对象）
	int						m_nCurAlignerBenchOffsetIndex;	// 基准中心对位补偿数量


	//////////////////////////////////////////////////////////////////////////
	// 目标设置
	// 如果为实时Mark，则设置Mark定位模板
	// 如果为基准Mark，则不设置Mark定位模板，但如果需要修正，则需要保存修正的相关信息

	// 目标模板
	AlignerTargetType	m_eTargetMarkType;				// 目标Mark类型：0为实时Mark，1为基准mark

	// 候补目标Mark
	BOOL				m_bTargetCandidateEnable;		// 否开启候补目标Mark搜索
	BOOL				m_bTargetCandidateSync;			// 是否开启候补目标Mark成对搜索

	int						m_nTargetCandidateOffsetNum;	// 候补目标Mark补偿数量（相对于第0个目标模板）
	std::vector<CCoordPos>  m_vTargetCandidateOffset;		// 候补目标Mark补偿（相对于第0个目标模板）
	std::vector<BOOL>  m_vTargetCandidateOffsetEnable;		// 候补目标Mark补偿（相对于第0个目标模板）是否有效

	// 对象模板
	AlignerObjectType	m_eObjectMarkType;				// 对象Mark类型：0为实时Mark，1为虚拟mark，2为基准Mark

	// 候补对象Mark
	BOOL				m_bObjectCandidateEnable;		// 否开启候补对象Mark搜索
	BOOL				m_bObjectCandidateSync;				// 是否开启候补对象Mark成对搜索

	int						m_nObjectCandidateOffsetNum;// 候补对象Mark补偿数量（相对于第0个对象模板）
	std::vector<CCoordPos>  m_vObjectCandidateOffset;// 候补对象Mark补偿（相对于第0个对象模板）
	std::vector<BOOL>  m_vObjectCandidateOffsetEnable;		// 候补对象Mark补偿（相对于第0个对象模板）是否有效

	std::vector<CAlignCheckInfo>	m_vAlignCheckInfo;				// 检测信息
	//CAlignCheckInfo		m_AlignCheckInfo;				// 检测信息

	CProductSizeInfo	m_ProductSizeInfo;				// 产品尺寸信息		//20160510增加对角补偿
	int					m_nPosMode;						// 拍照位置模式		//20160510增加对角补偿

	CProductOffsetInfo  m_ProductOffsetInfo;			// 补偿设置（补偿的方向、X和Y补偿值对调）

	
	int					m_nTargetMarkPatternNum;		// 实时目标Mark模板的个数
	std::vector<CString> m_vstrTargetMarkPatternIDs;		// 实时目标Mark模板所对应的唯一ID号


	// 针对于YX+D平台
	// 基准Mark修正，实际上就是重新设置一下基准目标Mark位置，原来是直接用标定时的Mark位置就可以了	
	// 基准Mark修正
	BOOL					m_bBenchTargetMarkFixEnable;			// 基准目标Mark是否进行修正
	CBenchTargetMarkFixInfo* m_pBenchTargetMarkFixInfo;


	BOOL					m_bBenchTargetMarkPickEnable;	// 拾取信息是否有效
	CBenchTargetMarkPickInfo* m_pBenchTargetMarkPickInfo;	// 拾取信息

	

	//////////////////////////////////////////////////////////////////////////
	// 对象设置
	// 如果只支持实时Mark，则设置Mark定位模板
	// 如果还支持虚拟Mark，则需要设置实时Mark定位模板和虚拟Mark定位模板，还需要保存实时Mark定位模板和虚拟Mark定位模板之间的偏移量

	// 对象模板
	int						m_nObjectMarkPatternNum;			// 对象模板的数量
	std::vector<CString>	m_vstrObjectMarkPatternIDs;			// 对象模板所对应的ID号


	BOOL					m_bVirtualObjectMarkEnable;			// 是否支持虚拟Mark

	// 针对于XYD、UVW平台
	// 虚拟Mark模板:用来确定虚拟Mark的位置，获得虚拟Mark和实时Mark之间的偏差
	int						m_nVirtualMarkPatternNum;			// 对象模板的数量
	std::vector<CString>	m_vstrVirtualMarkPatternIDs;			// 对象模板所对应的ID号

	/////////////////////////////////////////////////////////////////////////
	// 	标准的对象Mark与虚拟Mark之间的XY偏差：在标定完平台，设置好对位参数后，但是在进行实时对位前要做好
	std::vector<CCoordPos>	m_vObjectVirtualMarkOffset;			// 实时Mark与虚拟Mark之间的XY偏差


	//////////////////////////////////////////////////////////////////////////
	// 关联标定设置
	// 当对象和目标相机分离时，比如2对2、4对4等，需要设置关联标定模板及其他相关设置
	// 关联标定模板
	BOOL						m_bTargetObjectCamSeparate;			//目标相机和对象相机是各自独立的
	BOOL						m_bTargetCalibUseBoard;				// 是否启用标定板关联
	BOOL						m_bCalibTargetMarkEnable;			// 是否支持关联标定Mark
	CalibTargetMarkPatternMode  m_eCalibTargetMarkPatternMode;		// 关联模板类型
	int							m_nCalibTargetMarkPatternNum;		// 关联标定模板的数量
	std::vector<CString>		m_vstrCalibTargetMarkPatternIDs;	// 关联标定模板所对应的ID号
	BOOL                        m_bEnableCalibTargetDMCodeOutLine;  // 允许二维码关联时忽略点
	double                      m_dCalibTargetDMCodeOutLinePercent; // 二维码关联时忽略点百分比

	//////////////////////////////////////////////////////////////////////////
	// 检测模板设置
	BOOL						m_bSysPlatformInspectEnable;			// 检测模板是否启用(系统平台信息配置选项)
	//	BOOL						m_bAlignInspectEnable;					// 检测模板是否启用(软件界面选项)
	BOOL						m_bInspectCandidateEnable;				// 是否开启候补检测Mark搜索
	BOOL						m_bInspectCandidateSync;				// 是否开启候补检测Mark成对搜索
	int							m_nInspectMarkPatternNum;			// 检测Mark模板的数量
	std::vector<CString>		m_vstrInspectMarkPatternIDs;		// 检测Mark模板的ID

	BOOL						m_bInspectSingleSearch;				// 是否开启候补检测Mark单独搜索

	int							m_nInspectCandidateOffsetNum;			// 候补检测Mark补偿数量（相对于第0个检测模板）
	std::vector<BOOL>			m_vInspectCandidateOffsetEnable;		// 候补检测Mark补偿（相对于第0个检测模板）是否有效

	std::vector<CPosPatInfo>	m_vTargetPosPatInfo;		    // 各位置目标模板信息
	std::vector<CPosPatInfo>	m_vObjectPosPatInfo;			// 各位置对象模板信息

	BOOL  m_bEnableMutiTarget;									// 多目标对位是否有效
	std::vector<std::vector<CameraParamSet>>	m_vvdTargetSearchShutter;
	std::vector<std::vector<CameraParamSet>>	m_vvdObjectSearchShutter;
	std::vector<std::vector<CameraParamSet>>	m_vvdCalibTargetSearchShutter;
	std::vector<std::vector<CameraParamSet>>	m_vvdVirtualObjectSearchShutter;
	std::vector<std::vector<CameraParamSet>>	m_vvdInspectSearchShutter;

	std::vector<CameraParamEnable>					m_vbTargetSearchShutter;
	std::vector<CameraParamEnable>					m_vbObjectSearchShutter;
	std::vector<CameraParamEnable>					m_vbCalibTargetSearchShutter;
	std::vector<CameraParamEnable>					m_vbVirtualObjectSearchShutter;
	std::vector<CameraParamEnable>					m_vbInspectSearchShutter;

	BOOL m_bAccordingToPickPlatformEnable;


	CWeldInfo					m_WeldInfo;						// 焊接参数
	CProductPositionVirtualInfo m_ProductPositionVirtualInfo;
	CBendInfo					m_BendInfo;						// 折弯参数

	AlignOffsetByAera			m_AlignOffsetByArea;			// 根据区域补偿参数

	PhotoLightCtrlSet           m_cPhotoLightCtrl;              //流程光源控制参数

public:
	// 单产品扩展
	int		m_nMultiCalibExtensionMaxNum;	// 扩展数量

	// 扩展数量
	void SetMultiCalibExtensionMaxNum(int nMultiCalibExtensionMaxNum);
	int  GetMultiCalibExtensionMaxNum() {return m_nMultiCalibExtensionMaxNum;};

	// 设置、获取检测信息
	void SetAlignCheckInfo(const std::vector<CAlignCheckInfo> checkInfo);				
	std::vector<CAlignCheckInfo> GetAlignCheckInfo();

	void SetAlignCheckInfo(const CAlignCheckInfo checkInfo,const int Index);
	CAlignCheckInfo GetAlignCheckInfo(const int Index);

};

