// vsBaseAlignTool.h: interface for the scAlignerTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVALIGNERTOOL_H__3C92A5F4_E7B2_4983_A287_F579AA3F5837__INCLUDED_)
#define AFX_SVALIGNERTOOL_H__3C92A5F4_E7B2_4983_A287_F579AA3F5837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "vsAlignToolParam.h"

#define  MAXTARGETPOS		8
#define CALIBMOVEPOS_XYNUM	10
#define CALIBMOVEPOS_DNUM	4
#define CALIBPOS			8

typedef struct TagscDMCodeParam
{

	FindCornerPara m_FindCornerPara;//棋盘格角点提取参数

	FindDMCodePara m_FindDMCodePara;// DM解码参数
	bool	m_bEnableReDecode;		// 重新检测二维码

	double m_dShutter;
	scRect  m_DmCodeSearchRect;
	scRect  m_CornersSearchRect;

	double m_dImageWidth;
	double m_dImageHeight;


	TagscDMCodeParam()
	{
		m_bEnableReDecode  = false;

		m_dShutter = 0.0;
		m_DmCodeSearchRect.Set(25,25,702,430);
		m_CornersSearchRect.Set(0,0,752,480);

		m_dImageWidth = 752;
		m_dImageHeight = 480;


	}

	TagscDMCodeParam& operator= (const TagscDMCodeParam &alignerProcessInfo)
	{
		if (this == &alignerProcessInfo)
		{
			return *this;
		}

		m_FindCornerPara = alignerProcessInfo.m_FindCornerPara;
		m_FindDMCodePara = alignerProcessInfo.m_FindDMCodePara;
		m_bEnableReDecode = alignerProcessInfo.m_bEnableReDecode;

		m_dShutter = alignerProcessInfo.m_dShutter;
		m_DmCodeSearchRect = alignerProcessInfo.m_DmCodeSearchRect;
		m_CornersSearchRect = alignerProcessInfo.m_CornersSearchRect;

		m_dImageWidth = alignerProcessInfo.m_dImageWidth;
		m_dImageHeight = alignerProcessInfo.m_dImageHeight;

		return *this;
	}

}scDMCodeParam;



// 定位结果状态iyo --只可能有一个定位失败的情况，所以下列是全集
// typedef enum enumAlignerResultStateType
// {
// 	eAlignerResultNG = -1,			// 超过一个位置定位失败
// 	eAlignerResultOK = 0,			// 定位成功
// 	eAlignerResultNG1,			    // 位置1定位失败
// 	eAlignerResultNG2,				// 位置2定位失败
// 	eAlignerResultNG3,				// 位置3定位失败
// 	eAlignerResultNG4,				// 位置4定位失败
// } AlignerResultStateType;

// 定位结果状态iyo --只可能有一个定位失败的情况，所以下列是全集
// typedef enum enumtoAlignerPointMethodType
// {
// 	eAlignerErr = -1,
// 	eAligner1234_1234 = 0,			// 定位成功
// 	eAligner234_234 = 1,			// 位置1定位失败
// 	eAligner134_134 = 2,			// 位置2定位失败
// 	eAligner124_124,				// 位置3定位失败
// 	eAligner123_123,				// 位置4定位失败
// } toAlignerPointMethodType;


// 对位目标移动类型
typedef enum enumAlignerTargetMoveType
{
	eTargetNoMove = 0,			// 不随平台移动
	eTargetMoveByPlatform		// 随平台移动
}AlignerTargetMoveType;


// 对位对象移动类型
typedef enum enumAlignerObjectMoveType
{
	eObjectNoMove = 0,			// 不随平台移动
	eObjectMoveByPlatform		// 随平台移动
}AlignerObjectMoveType;


typedef enum enumTargetAutoCalibAlignMode
{
	eAlignOnObject = 0,//产品在对象端对准再由PLC移动固定距离到目标端
	eAlignToTarget = 1,//产品直接从对象端对准到目标端
}TargetAutoCalibAlignMode;







// 多相机图像搜索结果（一次定位信号后，所有相机的定位结果）
class VS_ALIGN_API CMarkImagePos
{
public:
	CMarkImagePos(){};
	virtual ~CMarkImagePos(){m_vPos.clear();m_vbOK.clear();};
	CMarkImagePos& operator = (const CMarkImagePos& lSR)
	{
		if (this == &lSR)
		{
			return *this;
		}

		m_vPos = lSR.m_vPos;
		m_vbOK = lSR.m_vbOK;

		return *this;
	}

public:
	void SetMarkImagePosNum(int nNum);							// 设置Mark图像位置的个数
	int  GetMarkImagePosNum();									// 获取Mark图像位置的个数

	void Reset();												// 重置所有Mark图像位置

	BOOL SetMarkImagePos(int nIndex, BOOL bOK, CCoordPos pos);	// 设置第nIndex个Mark图像位置
	BOOL GetIsMarkImagePosOK(int nIndex);						// 获取第nIndex个Mark图像位置是否有效
	CCoordPos GetMarkImagePos(int nIndex);						// 获取第nIndex个Mark图像位置

	BOOL IsAllMarkImagePosOK();									// 获取所有Mark图像位置是否有效
	BOOL IsAllMarkImagePosOK(std::vector<int> nPosIndex);		// 获取对应Mark图像位置是否有效	

public:
	std::vector<CCoordPos>	m_vPos;
	std::vector<BOOL>		m_vbOK;
};

// 多相机Mark的平台坐标值
class VS_ALIGN_API CMarkPlatformPos
{
public:
	CMarkPlatformPos(){};
	virtual ~CMarkPlatformPos(){m_vPos.clear();m_vbOK.clear();};
	CMarkPlatformPos& operator = (const CMarkPlatformPos& lSR);

public:
	
	void SetMarkPlatformPosNum(int nNum);								// 设置Mark图像位置的个数
	int  GetMarkPlatformPosNum();										// 获取Mark图像位置的个数

	void Reset();														// 重置所有Mark图像位置

	BOOL SetMarkPlatformPos(int nIndex, BOOL bOK, CCoordPos pos);		// 设置第nIndex个Mark图像位置
	BOOL GetIsMarkPlatformPosOK(int nIndex);							// 获取第nIndex个Mark图像位置是否有效
	CCoordPos GetMarkPlatformPos(int nIndex);							

	BOOL IsAllMarkPlatformPosOK();

public:
	std::vector<CCoordPos>	m_vPos;
	std::vector<BOOL>		m_vbOK;
};

// 多相机、多Mark坐标值（主要针对二维码应用）
class VS_ALIGN_API CMulMarkPos
{
public:
	CMulMarkPos(){};
	virtual ~CMulMarkPos(){m_vvPos.clear();};

	CMulMarkPos& operator = (const CMulMarkPos& lSR)
	{
		if (this == &lSR)
		{
			return *this;
		}

		m_vvPos = lSR.m_vvPos;

		return *this;
	}

public:

	void SetMarkImagePosNum(int nCamPosNum);
	int  GetMarkImagePosNum();

	BOOL SetMarkImagePos(int nCamPosIndex, std::vector<sc2Vector> vpos);
	std::vector<sc2Vector> GetMarkImagePos(int nCamPosIndex);	

public:

	std::vector<std::vector<sc2Vector>> m_vvPos;

};




class vcBaseVisionAlign;


class VS_ALIGN_API vcBaseAlignTool
{
public:
	vcBaseAlignTool();
	virtual ~vcBaseAlignTool();

	vcBaseVisionAlign* m_pVisionAlignerTool;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
public:


	// 1. 系统配置
	virtual void SetPlatformIndex(int nPlatformIndex);									// 设置平台序号
	virtual int	 GetPlatformIndex();

	virtual void SetMarkNum(int iNum);													// 设置标定及对位时mark点的个数

	virtual void SetCamNum(int nCamNum);												// 设置相机的数量
	virtual int	GetCamNum();

	virtual void SetMidPlatformType(PlatformType ePlatformType);						// 设置中间平台类型
	virtual PlatformType GetMidPlatformType();
	
	virtual void SetAlignerTargetMoveType(AlignerTargetMoveType eMoveType);				// 设置目标移动类型
	virtual AlignerTargetMoveType GetAlignerTargetMoveType();
	
	virtual void SetAlignerObjectMoveType(AlignerObjectMoveType eMoveType);				// 设置对象移动类型
	virtual AlignerObjectMoveType GetAlignerObjectMoveType();

	
	virtual void SetTargetObjectCamSeparate(BOOL bTargetObjectCamSeparate);				// 设置目标相机和对象相机是否是独立的
	virtual BOOL GetTargetObjectCamSeparate();

	virtual void SetTarObjCamSepNoAxisCalib(BOOL bTarObjCamSepNoAxisCalib);				// 设置目标相机和对象相机是各自独立时，标定目标位置时不读取平台轴位置
	virtual BOOL GetTarObjCamSepNoAxisCalib();

	virtual void SetTarObjCamSepAutoCalibTar(BOOL bTarObjCamSepAutoCalibTar);			// 设置目标相机和对象相机是各自独立时，目标位置自动标定不关联
	virtual BOOL GetTarObjCamSepAutoCalibTar();

	virtual void SetTargetCalibUseBoard(BOOL bTargetCalibUseBoard);						// 设置是否启用标定板关联
	virtual BOOL GetTargetCalibUseBoard();
	
// 	virtual void SetPlatformAlignTarCamWithDiffObjCam(BOOL bTarCamWithDifObjCam);		// 一组目标相机对应两组对象相机
// 	virtual BOOL GetPlatformAlignTarCamWithDiffObjCam();

// 	virtual void SetTargetGuanLianUse4Points(BOOL bTargetGuanLianUse4Points);			// 设置目标相机的关联是否使用4点
// 	virtual BOOL GetTargetGuanLianUse4Points();

	void SetEnablePositionVirtualMode(BOOL bEnablePositionVirtualMode);					// 设置产品虚拟位置信息函数
	BOOL GetEnablePositionVirtualMode();
		
	//virtual void SetCalibSendAbsPos(BOOL bCalibSendAbsPos);								// 设置标定时使用相对或绝对位置
	//virtual BOOL GetCalibSendAbsPos();

	virtual void SetCalibSendRelativePos(BOOL bCalibSendRelativePos);
	virtual BOOL GetCalibSendRelativePos();

	virtual void SetCalibrateChangeXYDDirection(BOOL bCalibrateChangeXDirection,BOOL bCalibrateChangeYDirection,BOOL bCalibrateChangeDDirection);				// 设置标定过程中是否设置反置轴的方向
	virtual BOOL GetCalibrateChangeXYDDirection(BOOL &bCalibrateChangeXDirection,BOOL &bCalibrateChangeYDirection,BOOL &bCalibrateChangeDDirection);

	virtual void SetUseAlignFAHLex(BOOL bUseAlignFAHLex);
	virtual BOOL GetUseAlignFAHLex();

	void SetTargetUseFixedVirtualCoordinates(BOOL bTargetUseFixedVirtualCoordinates);
	BOOL GetTargetUseFixedVirtualCoordinates();

	virtual void SetEnableMultiCalibExtension(BOOL bEnableMultiCalibExtension);			// 启用单产品多标定扩展	
	virtual BOOL GetEnableMultiCalibExtension();

	virtual void SetMultiCalibExtensionMaxNum(int nMultiCalibExtensionMaxNum);						
	virtual int GetMultiCalibExtensionMaxNum();

	virtual void SetPlatformPick(BOOL bPlatformPick);									// 设置是否为拾取系统
	virtual BOOL GetPlatformPick();

	virtual void SetPickCamPlatformType(PickCamPlatformType ePickCamPlatformType);		// 设置拾取系统中相机的安装方式
	virtual PickCamPlatformType GetPickCamPlatformType();

	


	// 2. 选项
	virtual void vcBaseAlignTool::SetTarAutoCalibAlignMode(TargetAutoCalibAlignMode eTarAutoCalibAlignMode);
	virtual TargetAutoCalibAlignMode vcBaseAlignTool::GetTarAutoCalibAlignMode();

	virtual void SetIsReadTargetPlatformPos(BOOL bEnable);								// 设置是否从PLC中读取的目标平台坐标（对象目标不在一台工控机时使用）

	virtual void SetEnablePickPosUpdate(BOOL bEnablePickPosUpdate);						// 拾取时旋转中心位置修正
	virtual BOOL GetEnablePickPosUpdate();


	// 4. 标定界面（说明：该选项可移动到标定参数中）
	BOOL SetHomographyDOF(HomographyDOF dof);											// 设置标定算法
	HomographyDOF GetHomographyDOF();

	virtual void SetCalibParam(CCalibrateParam* sysCalibParam);							// 设置标定参数
	virtual CCalibrateParam* GetCalibParam();

	virtual BOOL SetCalibParams(std::vector<CCalibrateParam*> vpCalibParam);			// 设置标定参数
	virtual std::vector<CCalibrateParam*> GetCalibParams();

	
	// 5. 平台选项
	virtual void SetPlatformParam(CPlatformInfo* pPlatformInfo);						// 设置对位平台信息
	virtual CPlatformInfo* GetPlatformParam();

	virtual void SetCamPlatformParam(CCamPlatformInfo* pCamPlatformInfo);				// 设置相机平台信息
	virtual CCamPlatformInfo* GetCamPlatformParam();

	void SetLogRecordEnable(BOOL bEnable);												// 设置、获取是否记录日志
	BOOL GetLogRecordEnalbe();


	// 6. 对位参数
	virtual void SetProductAlignerParam(CAlignerParam& alignerParam);					// 设置产品对位信息
	virtual CAlignerParam* GetProductAlignerParam();


	

public:

	

	// 1. 系统配置
	int								m_nPlatformIndex;								// 平台序号
	int								m_iMarkNum;	
	int								m_nCamNum;										// 相机数量
	PlatformType					m_eMidPlatformType;								// 中间平台类型
	AlignerTargetMoveType			m_eAlignerTargetMoveType;						// 目标Mark移动类型
	AlignerObjectMoveType			m_eAlignerObjectMoveType;						// 对象Mark移动类型

	BOOL							m_bTargetObjectCamSeparate;						// 目标相机和对象相机是否是独立的
	BOOL							m_bTarObjCamSepNoAxisCalib;						// 目标相机和对象相机是各自独立时，标定目标位置时不读取平台轴位置
	BOOL							m_bTarObjCamSepAutoCalibTar;					// 目标对象分离，目标位置自动标定不关联
	BOOL							m_bTargetCalibUseBoard;							// 是否启用标定板关联
//	BOOL							m_bPlatformTarCamWithDiffObjCam;
	BOOL							m_bEnableTargetAndObjectPositionVirtualMode;
	
	
	//BOOL							m_bCalibSendAbsPos;								// 标定发送绝对位置
	BOOL							m_bCalibSendRelativePos;						// 标定发送相对位置
	BOOL							m_bCalibrateChangeXDirection;					// 标定过程中反置轴的X方向
	BOOL							m_bCalibrateChangeYDirection;					// 标定过程中反置轴的Y方向
	BOOL							m_bCalibrateChangeDDirection;					// 标定过程中反置轴的D方向

	BOOL							m_bUseAlignFAHLex;  
	BOOL							m_bTargetUseFixedVirtualCoordinates;			//目标对象分离，目标使用对象关联的方式建立固定坐标系，不做目标修正。

	BOOL							m_bEnableMultiCalibExtension;					// 启用单产品多标定扩展
	int								m_nMultiCalibExtensionMaxNum;					// 扩展数量

	BOOL							m_bPlatformPick;								// 是否为拾取系统
	PickCamPlatformType				m_ePickCamPlatformType;							// 拾取系统中相机的安装方式（0,固定安装；1，共享平台的XY；）

	


	// 2. 选项
	TargetAutoCalibAlignMode        m_eTarAutoCalibAlignMode;						// 目标自动标定（9宫格标定）时的对位模式 0：在对象端对位再移动固定距离到目标 1：直接对到目标
	BOOL                            m_bReadTargetPlatformPos;   					// 是否为从PLC中读取的目标平台坐标（在对象目标不在同一控制器时使用）
	BOOL							m_bEnablePickPosUpdate;							// 拾取时旋转中心位置修正

	
	// 4. 标定界面（说明：该选项可移动到标定参数中）
	HomographyDOF					m_HomographyDOF;
	CCalibrateParam* 				m_CalibrateParam;								// 标定参数
	std::vector<CCalibrateParam*>	m_vpCalibrateParam;								// 产品的标定信息


	// 5. 平台选项
	CPlatformInfo*					m_pPlatformParam;								// 平台信息
	CCamPlatformInfo*				m_pCamPlatformInfo;								// 相机平台信息

protected:

	BOOL							m_bLogRecordEnable;
	CAlignerParam					m_AlignermentParam;								// 系统对位参数
	PlatformCalibType				m_PlatformCalibType;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

	// 平台标定
public:

	// 平台各轴所在的基准位置
	virtual void SetCalibPlatformPos(CPlatformAxisPos* pPlatformAxisPos/*, int nIndex = 0, BOOL bTarCamWithDiffObjCam = FALSE*/);
	virtual CPlatformAxisPos* GetCalibPlatformPos(/*int nIndex = 0*/);

	// 平台标定时图像搜索结果
	virtual void SetCalibPlatformMoveSearchResult(std::vector<CMarkImagePos*> ImageResults,int nCalibPos);						// XYD/XD/YD/XY/X平台标定时图像搜索结果
	virtual void SetCalibPlatformMoveSearchResult(std::vector<CMarkImagePos*> ImageResults,int nMarkType,int nCaliPos);			// XYPD平台标定时图像搜索结果
	virtual void GetCalibPlatformMoveSearchResult(int nCalibPos,int& nNum,std::vector<CCoordPos>& ImageResults);

	// XYD/XD/YD平台全局二维码标定，二维码图像、二维码靶标搜索结果
	virtual void SetWholeDmCodeCalibImageResult(std::vector<CMulMarkPos*> vDmCodePlatformPos,int nCalibPos);
	virtual void SetWholeDmCodeCalibBoardResult(std::vector<CMulMarkPos*> vDmCodePlatformPos,int nCalibPos);

	// XYPD平台全局二维码标定，二维码图像、二维码靶标搜索结果
	virtual void SetWholeDmCodeCalibImageResult(std::vector<CMulMarkPos*> vDmCodePlatformPos,int nMarkType,int nCalibPos);
	virtual void SetWholeDmCodeCalibBoardResult(std::vector<CMulMarkPos*> vDmCodePlatformPos,int nMarkType,int nCalibPos);

	// 获取平移标定和旋转标定时平台各轴的移动位置数量
	virtual int GetCalibPlatformMovePosNum(int nMarkType);			// 包含XYD平台拾取、XY平台拾取
	virtual int GetCalibPlatformXYDMovePosNum();
	virtual int GetCalibPlatformXYMovePosNum();
	virtual int GetCalibPlatformXDMovePosNum();
	virtual int GetCalibPlatformYDMovePosNum();
	virtual int GetCalibPlatformXDPYMovePosNum();
	virtual int GetCalibPlatformXDPY1MovePosNum();
	virtual int GetCalibPlatformXYPD_XYMovePosNum();
	virtual int GetCalibPlatformXYPD_DMovePosNum();
	virtual int GetCalibPlatformXMovePosNum();
	virtual int GetCalibPlatformYMovePosNum();

	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformMovePos(int nMarkType);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformMovePos(int nPosIndex,int nMarkType);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformXYDMovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformXYMovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformXDMovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformYDMovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformXDPYMovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformXDPY1MovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformXYPD_XYMovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformXYPD_DMovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformXMovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformYMovePos(int nPosIndex);

	// 标定计算
	virtual BOOL ExecuteCalibrate(void);								// 视觉流程中没有使用
	virtual BOOL ExecuteCalibrate(int nCalibPos);						// 机器人应用中使用
	virtual BOOL ExecuteCalibrate(int nCalibPos, int nMarkType);		// 视觉流程中使用
	virtual BOOL ExecuteCalibrateFCBS(int nCalibPos, int nMarkType);	// XY+D平台使用

	// 目标、对象不分离标定计算
	virtual BOOL ExecuteCalibrateSingleCamEX(int nCamIndex);							// XYD平台,  用9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateSingleCam(int nCamIndex);								// XYD平台,  用5点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateXYSingleCamEX(int nCamIndex);							// XY平台,   用9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateXYSingleCam(int nCamIndex);							// XY平台,   用5点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateXDSingleCamEX(int nCamIndex);							// XD平台,   用5点、9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateYDSingleCamEX(int nCamIndex);							// YD平台,   用5点、9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateXDPYSingleCamEX(int nCamIndex);						// XD+Y平台, 用5点、9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateXDPY1SingleCamEX(int nCamIndex);						// XD+Y1平台,用5点、9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateSingleCamTarget(int nCamIndex,int nMarkType);			// XY+D平台, 用9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateSingleCam(int nCamIndex,int nMarkType);				// XY+D平台, 用5点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateXSingleCamEX(int nCamIndex);							// X平台,    用9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateXSingleCam(int nCamIndex);								// X平台,    用5点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateYSingleCamEX(int nCamIndex);							// Y平台,    用9点H矩阵方式进行标定

	virtual BOOL ExecuteCalibrateSingleCamTarget(int nCamIndex);						// 高低平面，标定目标平面，主要应用为叠摸

	virtual BOOL ExecuteCalibrateBoardSingleCamEX(int nCamIndex);						// 使用棋盘格靶标进行对象标定（不分离）
	virtual BOOL ExecuteCalibrateDmCodeSingleCamEX(int nCalibPos, sc2Vector scImageCenter,
		std::vector<sc2Vector> &vImagePos,std::vector<sc2Vector> &vBoardPos);			// 使用二维码靶标进行对象标定（不分离）


	// 目标、对象分离标定计算
	virtual BOOL ExecuteCalibrateSingleCamEX(int nCamIndex, int nBaseCamIndex);			// XYD平台
	virtual BOOL ExecuteFCBSCalibrateSingleCamEX(int nCamIndex, int nBaseCamIndex);		// XY+D平台

	// 使用二维码靶标进行关联计算
	virtual BOOL ExecuteCalibrateDmCode(int nCalibPos,std::vector<sc2Vector>& tarImgPos,std::vector<sc2Vector>& objImgPos);		// 使用二维码靶标进行关联计算
	virtual BOOL ExecuteCalibrateSingleCamDmCodeTarget(int nCamIndex, int nBaseCamIndex,std::vector<sc2Vector>& tarImgPos,std::vector<sc2Vector>& objImgPos);		// XYD平台， 二维码靶标进行关联计算
	virtual BOOL ExecuteCalibrateSingleCamDmCodeObject(int nCamIndex, int nBaseCamIndex,std::vector<sc2Vector>& tarImgPos,std::vector<sc2Vector>& objImgPos);		// XY+D平台，二维码靶标进行关联计算


	// 使用棋盘格靶标关联计算
	virtual void SetCalibBoardSize(sc2Vector vBoardSize);
	virtual void SetCalibBoardResult(std::vector<CMarkImagePos*> ImageResults,int nCalibPos);
	virtual BOOL ExecuteCalibrateBoardSingleCamEX(int nCamIndex, int nBaseCamIndex);																// 使用棋盘格靶标进行标定计算
	virtual BOOL ExecuteCalibrateDmCodeBoard(int nCalibPos,std::vector<sc2Vector> &vImagePos,std::vector<sc2Vector> &vBoardPos,int nMarkType);		// 二维码靶标替换棋盘格靶标
	virtual BOOL ExecuteCalibrateDmCodeBoardSingleCamEX(int nCalibPos,std::vector<sc2Vector> &vImagePos,std::vector<sc2Vector> &vBoardPos);	


	// 全局二维码标定
//	virtual BOOL ExecuteCalibrateWholeDmCodeBoardImgToBoard(int nCalibPos,std::vector<sc2Vector> &vImagePos,std::vector<sc2Vector> &vBoardPos);
	virtual BOOL ExecuteCalibrateWholeDmCodeBoardImgToBoard(int nExProductIndex);								// 执行全局二维码标定：图像坐标到靶标坐标间的映射关系

	virtual BOOL ExecuteCalibrateWholeDmCodeBoardBoardToPlat(int nExProductIndex, int nMarkType);				// 执行全局二维码标定：靶标坐标到平台坐标的旋转缩放关系

	virtual BOOL ExecuteCalibrateSingleCamWholeDmCodeBoardBoardToPlat(int nExProductIndex);						// 全局二维码标定时XYD平台靶标到平台坐标映射
//	virtual BOOL ExecuteCalibrateSingleCamXYWholeDmCodeBoardBoardToPlat(int nCalibPos);							// 全局二维码标定时XY平台靶标到平台坐标映射
	virtual BOOL ExecuteCalibrateSingleCamXDWholeDmCodeBoardBoardToPlat(int nExProductIndex);					// 全局二维码标定时XD平台靶标到平台坐标映射
	virtual BOOL ExecuteCalibrateSingleCamYDWholeDmCodeBoardBoardToPlat(int nExProductIndex);					// 全局二维码标定时YD平台靶标到平台坐标映射
//	virtual BOOL ExecuteCalibrateSingleCamXDPYWholeDmCodeBoardBoardToPlat(int nCalibPos);						// 全局二维码标定时XDPY平台靶标到平台坐标映射
//	virtual BOOL ExecuteCalibrateSingleCamXDPY1WholeDmCodeBoardBoardToPlat(int nCalibPos);						// 全局二维码标定时XDPY1平台靶标到平台坐标映射
	virtual BOOL ExecuteCalibrateSingleCamXYPDWholeDmCodeBoardBoardToPlat(int nCalibPos);						// 全局二维码标定时XYPD平台靶标到平台坐标映射
//	virtual BOOL ExecuteCalibrateSingleCamXWholeDmCodeBoardBoardToPlat(int nCalibPos);							// 全局二维码标定时X平台靶标到平台坐标映射

	virtual BOOL ExecuteCalibWholeDmCodeBoard(int nExProductIdx = 0,BOOL bPickDiffPosSameCalib = FALSE);		// 执行全局二维码标定求取旋转中心

	//功能：利用该位置标定信息把源坐标转化为映射坐标
	//参数：位置序号，源坐标（输入），映射坐标（输出）
	virtual BOOL ConvertSrcPosToDstPos(int nCalibPos,sc2Vector srcPos,sc2Vector& dstPos);

	//功能：利用若干源坐标点和目的坐标点建立映射关系，并把一个源坐标点转化为映射坐标点
	//参数：位置序号，若干源坐标（输入），若干目的坐标（输入），要求取的一个源坐标（输入），求取得到的映射坐标（输出）
//	virtual BOOL ConvertSrcPosToDstPos(std::vector<sc2Vector> &srcvPos,std::vector<sc2Vector> &dstvPos,sc2Vector srcPos,sc2Vector &dstPos);

	// 启用畸变矫正功能，如下参数有实际意义
	// bImageDisCor =  TURE：srcvPos传入为图像坐标；
	// bImageDisCor = FALSE：dstvPos传入为图像坐标；
	virtual BOOL ConvertSrcPosToDstPos(std::vector<sc2Vector> &srcvPos,std::vector<sc2Vector> &dstvPos,sc2Vector srcPos,sc2Vector &dstPos, BOOL bImageDisCor = TRUE, int nCalibPos = 0);

	//功能：利用若干源坐标点和目的坐标点建立映射关系，并把若干源坐标点转化为映射坐标点
	//参数：位置序号，若干源坐标（输入），若干目的坐标（输入），要求取的若干源坐标（输入），求取得到的映射坐标（输出）
	virtual BOOL ConvertSrcPosToDstPos(const std::vector<sc2Vector> &srcvPos,const std::vector<sc2Vector> &dstvPos,std::vector<sc2Vector> &vSrcTranseferPos,std::vector<sc2Vector> &vDstTranseferPos);


	// 关联标定，输入两个MARK的平台坐标，与两个MARK的靶标坐标一起，计算靶标坐标系到平台坐标系转换关系
	virtual BOOL ExecuteCalibrateFCBR(sc2Vector platformPos1, sc2Vector platformPos2, sc2Vector imagePos1, sc2Vector imagePos2);
	// 从图像坐标转换靶标坐标，不考虑轴位置变化
	virtual BOOL ConvertImagePosToBoardPos(sc2Vector& boardPos, int nIndex, sc2Vector imagePos);


	// 获取标定结果信息
	virtual std::vector<CCalibratedInfo*> GetCalibratedInfo();									// 所有相机标定结果
	virtual CCalibratedInfo* GetSingleCalibratedInfo(int nCamIndex);							// 单个相机标定结果
	virtual BOOL SetSameCalibrateInfo(int nSrcPos,std::vector<int>vnPos);						// 将nSrc位置的标定信息拷贝到其他位置中

	// 设置产品标定信息
	virtual bool InitCalibratedInfos(std::vector<CCalibratedInfo*> vCalibratedInfos);
	virtual bool SetProductCalibratedInfo(std::vector<CCalibratedInfo*> vCalibratedInfos);
	virtual bool SetProductCalibratedInfoSingleCam( int nCamIndex, CCalibratedInfo* vCalibratedInfos);




protected:

	std::vector<std::vector<sc2Vector>>			m_vvWholeDmCodeCalibMoveImageResults[CALIBPOS];			// 全局二维码标定-二维码图像坐标
	std::vector<std::vector<sc2Vector>>			m_vvWholeDmCodeCalibMoveBoardResults[CALIBPOS];			// 全局二维码标定-二维码平台坐标

	CPlatformAxisPos* 				m_PlatformAxisPos;								// 平台轴初始位置
//	std::vector<CPlatformAxisPos*>	m_vSpecialPlatformAxisPos;	
	std::vector<CPlatformAxisPos*>	m_vPlatformAxisPos;								// 平台轴移动旋转的绝对位置
	std::vector<CCoordPos>			m_vCalibPlatformMoveResults[CALIBPOS];			// 平台标定时图像搜索结果



	std::vector<CCalibratedInfo*>	m_vCalibratedInfos;								// 标定的结果数据信息
	std::vector<CCalibratedInfo*>	m_vAlignerCalibInfos;							// 相机对位时的标定参数信息

	sc2Vector						m_vCalibBoardSize;								// 标定靶标矩形尺寸
	std::vector<CCoordPos>			m_vCalibBoardResults[CALIBPOS];					// 靶标图像坐标

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

	// 标定求精
public:	

	// 如下求精接口函数没有使用
	virtual BOOL IsExecuteCalibrateRefine(void);														// 判断是否需要求精
	virtual BOOL IsRefineFinished(int nCamIndex);														// 计算求精过程是否结束（该相机求精结束，返回ture）
	virtual CPlatformAxisPos* GetCalibRefineMovePos(int nCamIndex);										// 获取求精时各轴的移动位置(如果该相机求精结束，就返回基准位置)	
	virtual void SetRefineImgCoordPos(CCoordPos pos);													// 设置求精时的图像搜索结果
	virtual BOOL ExecuteCalibrateRefine(int nCamIndex);													// 执行标定结果求精

	// 平台标定求精(使用)
	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformRefineMovePos(int nPosIndex);				// 获取标定求精时各轴的位置（共3个，正转D度，反转D度，基准位置）	
	virtual void SetCalibPlatformRefineMoveSearchResult(std::vector<CMarkImagePos*> ImageResults);		// 设置平台标定求精时图像搜索结果（共3个，正转D度，反转D度，基准位置）
	virtual BOOL ExecuteCalibratePlatformRefine(int nPosIndex);											// 执行标定结果求精（计算新的更为准确的基准Mark平台坐标系坐标）
	virtual CMarkPlatformPos GetCalibRefineMarkPlatformPos(int nPosIndex);								// 获取平台标定求精时图像搜索结果的平台坐标位置	

protected:
	// 将标定求精是的图像位置转为平台坐标位置
	bool ConvertCalibRefineImagePosToCurPlatformPos(CCoordPos& platformCoordPos, int nPosIndex, CCoordPos imageCoordPos);



protected:

	BOOL							m_bIsRefineSatisfied;							// 求精是否满足精度要求

	std::vector<CPlatformAxisPos*>	m_vPlatformRefineAxisPos;						// 标定求精时平台轴移动的绝对位置(3个位置，正转角度、反转角度、基准位置)
	std::vector<CCoordPos>			m_vCalibPlatformRefineMoveResults;				// 平台标定求精时图像搜索结果
	std::vector<CCoordPos>			m_vCalibRefinePlatformCoord;					// 平台标定求精时基准mark的平台坐标值
	std::vector<CCoordPos>			m_vCalibRefineImageCoord;						// 平台标定求精时图像搜索结果	
	CPlatformAxisPos* 				m_PlatformRefineAxisPos;						// 标定求精时平台轴移动的绝对位置
	

	CMarkPlatformPos				m_RefineMarkPlatformPos;
	CMarkImagePos					m_RefineSearchResults;	

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

	// 相机标定
public:

	// 相机标定
	virtual void SetCalibCamPlatformPos(int nIndex, CPlatformAxisPos* vPlatformAxisPos);				// 设置/获取标定时，相机各轴所在的基准位置
	virtual CPlatformAxisPos* GetCalibCamPlatformPos(int nIndex);

	virtual void SetCalibCamPlatformPos( std::vector<CPlatformAxisPos*> vPlatformAxisPos);				// 所有相机轴位置
	virtual std::vector<CPlatformAxisPos*> GetCalibCamPlatformPos();

	virtual void SetCalibCamAxisPlatformAxisPos(int nIndex, CPlatformAxisPos* vPlatformAxisPos);		//单个相机轴所在平台的轴位置
	virtual CPlatformAxisPos* GetCalibCamAxisPlatformAxisPos(int nIndex);

	virtual std::vector<CPlatformAxisPos*> GetCalibCameraMovePos(int nIndex);							// 获取相机平移标定时各轴的移动位置	
	virtual void SetCalibCameraMoveSearchResult(std::vector<CMarkImagePos*> ImageResults);				// 设置相机平台标定时图像搜索结果

	virtual BOOL ExecuteCamCalibrate(void);																//执行相机平台标定	
	virtual BOOL ExecuteCamCalibrate( int nCalibPos );
	virtual BOOL ExecuteCamCalibrateSingleCam(int nCamIndex);	


protected:

	std::vector<CPlatformAxisPos*> 					m_vCamPlatformInitAxisPos;						// 相机轴初始位置（所有相机的初始位置）
	std::vector<CPlatformAxisPos*>					m_vCamAxisPlatformInitAxisPos;					// 相机轴所在平台轴初始位置（所有相机的初始位置）

	std::vector< std::vector<CPlatformAxisPos*> >	m_vCamPlatformAxisPos;							// 平台轴移动旋转的绝对位置	(单个相机)
	std::vector<CCoordPos>							m_vCalibCameraMoveResults;						// 相机平移标定时图像搜索结果（所有相机）	


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	// 实时目标
public:

	virtual void SetTargetPlatformPos(std::vector<CPlatformAxisPos*> pPlatformAxisPos);						// 定位实时目标时，对位平台位置
	virtual std::vector<CPlatformAxisPos*> GetTargetPlatformPos();

	virtual void SetTargetPlatformPos(CPlatformAxisPos* pPlatformAxisPos,int nIndex);						// 定位实时目标时，对位平台位置
	virtual CPlatformAxisPos* GetTargetPlatformPos(int nIndex);

	virtual void SetTargetCameraPos(std::vector<CPlatformAxisPos*> pCamPlatformAxisPos);					// 定位实时目标时，相机平台位置
	virtual void SetTargetCameraPos(std::vector<CPlatformAxisPos*> pCamPlatformAxisPos,int nCamPos);
	virtual void SetTargetCameraPos(CPlatformAxisPos* pCamPlatformAxisPos, int nIndex);
	virtual std::vector<CPlatformAxisPos*>  GetTargetCameraPos();


	virtual void SetTargetCamAxisPlatformAxisPos(CPlatformAxisPos* pCamAxisPlatformAxisPos,int nIndex);		// 定位实时目标时，相机所在平台轴位置
	virtual void SetTargetCamAxisPlatformAxisPos(std::vector<CPlatformAxisPos*> pCamPlatformAxisPos);
	std::vector<CPlatformAxisPos*> GetTargetCamAxisPlatformAxisPos();
	
	virtual void SetTargetCamAxisPos_MultiEx(CPlatformAxisPos* pCamPlatformAxisPos,int nIndex);						// 设置扩展各产品的目标相机实时小轴位置
	virtual void SetTargetCamAxisPos_MultiEx(std::vector<std::vector<CPlatformAxisPos*>> vvpCamPlatformAxisPos);
	
	virtual void SetTargetCamAxisPlatformAxisPos_MultiEx(CPlatformAxisPos* pCamAxisPlatformAxisPos,int nIndex);		// 设置扩展各产品的目标相机实时大轴位置
	virtual void SetTargetCamAxisPlatformAxisPos_MultiEx(std::vector<std::vector<CPlatformAxisPos*>> vvpCamAxisPlatformAxisPos);

	virtual void SetTargetMarkSearchResults(CMarkImagePos ImageResults);											// 设置目标Mark的图像搜索结果
	virtual CMarkImagePos GetTargetMarkSearchResults();

	virtual BOOL SetTargetMarkPlatformPos(CMarkPlatformPos &tarPlatPos);
	virtual CMarkPlatformPos GetTargetMarkPlatformPos();									// 获取目标Mark的全局平台坐标值	
	virtual CMarkPlatformPos GetTargetMarkPlatformPosForCheck();							// 目标距离检查（流程中使用）	
//	virtual CMarkPlatformPos GetSpecialTargetMarkPlatformPos();								// 一组目标相机对应两组对象相机
	virtual BOOL GetTargetMarkPlatformDistance(double &dDistance);							// 获取目标平台坐标距离，八字对位使用

	// XY+D平台，目标角度旋转后的平台坐标
	virtual CCoordPos GetImageCoordPosAfterRotate(int nPosIndex);							

	// 根据目标图像到对象图像映射结果，将目标图像坐标映射到对象图像中，（目标、对象分离的情况）
	virtual BOOL ConvertTarImgPosToObjImgPosUsingImageHomographyProject(CCoordPos& ObjImgPos, int nIndex, CCoordPos TarImgPos);

	// 将当前平台坐标转化为图像坐标
	virtual bool ConvertCurPlatformPosToImagePos( CCoordPos &imageCoordPos, int nIndex, CCoordPos platformCoordPos,CPlatformAxisPos* pPlatformAxisPos);

	// 目标是否重构	
	virtual BOOL IsTargetReConstruct();	

	// 对位计算，获取目标平台坐标
	virtual CMarkPlatformPos GetTargetMarkPlatformPosForAlign();

	// 目标综合对位点重构
	virtual BOOL ExecuteTargetPlatformPosRestruct(std::vector<CCoordPos>& vcpTargetPos);	




public:

	CMarkImagePos									m_TargetSearchResults;							// 目标Mark搜索时，目标Mark的图像坐标值
	CMarkPlatformPos								m_TargetMarkPlatformPos;						// 目标Mark搜索时，目标Mark的平台坐标值
	std::vector<CPlatformAxisPos*>					m_pTargetPlatformAxisPos;						// 目标Mark搜索时，平台轴绝对位置
	std::vector<CPlatformAxisPos*>					m_vTargetCamPlatformAxisPos;					// 目标Mark搜索时，相机轴绝对位置
	std::vector<CPlatformAxisPos*>					m_vTargetCamAxisPlatformAxisPos;				// 目标Mark搜索时，相机所在平台轴绝对位置
	std::vector<std::vector<CPlatformAxisPos*>>		m_vvpTargetCamPlatformAxisPosMultiEx;			// 目标Mark搜索扩展产品时，相机轴绝对位置
	std::vector<std::vector<CPlatformAxisPos*>>		m_vvpTargetCamAxisPlatformAxisPosMultiEx;		// 目标Mark搜索扩展产品时，相机所在平台轴绝对位置
	CCoordPos										m_ImageCoordPos[MAXTARGETPOS];					// 存储旋转后的图像坐标


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	// 实时对象
public:
	virtual void SetObjectPlatformPos(std::vector<CPlatformAxisPos*> pPlatformAxisPos);		// 定位实时对象时，对位平台位置
	virtual std::vector<CPlatformAxisPos*> GetObjectPlatformPos();

	virtual void SetObjectPlatformPos(CPlatformAxisPos* pPlatformAxisPos,int nIndex);		// 定位实时对象时，对位平台位置
	virtual CPlatformAxisPos* GetObjectPlatformPos(int nIndex);

	virtual void SetObjectCameraPos(std::vector<CPlatformAxisPos*> pCamPlatformAxisPos);	// 定位实时对象时，相机平台位置
	virtual void SetObjectCameraPos(CPlatformAxisPos* pCamPlatformAxisPos,int nIndex);
	virtual std::vector<CPlatformAxisPos*> GetObjectCameraPos();

	virtual void SetObjectCamAxisPlatformAxisPos(CPlatformAxisPos* pCamAxisPlatformAxisPos,int nIndex);
	virtual void SetObjectCamAxisPlatformAxisPos(std::vector<CPlatformAxisPos*> pCamPlatformAxisPos);
	virtual std::vector<CPlatformAxisPos*> GetObjectCamAxisPlatformAxisPos();
	
	//设置扩展各产品的对象相机实时小轴位置
	virtual void SetObjectCamAxisPos_MultiEx(CPlatformAxisPos* pCamPlatformAxisPos,int nIndex);	
	virtual void SetObjectCamAxisPos_MultiEx(std::vector<std::vector<CPlatformAxisPos*>> vvpCamPlatformAxisPos);
	
	//设置扩展各产品的对象相机实时大轴位置
	virtual void SetObjectCamAxisPlatformAxisPos_MultiEx(CPlatformAxisPos* pCamAxisPlatformAxisPos,int nIndex);	
	virtual void SetObjectCamAxisPlatformAxisPos_MultiEx(std::vector<std::vector<CPlatformAxisPos*>> vvpCamAxisPlatformAxisPos);

	
	virtual void SetObjectMarkSearchResults(CMarkImagePos ImageResults);			// 对象Mark的图像搜索结果
	virtual CMarkImagePos GetObjectMarkSearchResults();
	
	virtual CMarkPlatformPos GetObjectMarkPlatformPos();							// 获取对象Mark的全局平台坐标值
	virtual CMarkPlatformPos GetObjectMarkPlatformPosForCheck();
//	virtual CMarkPlatformPos GetSpecialObjectMarkPlatformPos();
	virtual CMarkPlatformPos GetObjectMarkPlatformPos(BOOL bReConstruct);			// 重构前后	
	virtual BOOL GetObjectMarkPlatformDistance(double &dDistance);					// 获取对象平台坐标距离，八字对位使用
	


	BOOL IsObjectReConstruct();														//对象是否重构


	// 对位计算,获取对象平台坐标
	virtual CMarkPlatformPos GetObjectMarkPlatformPosForAlign();

	// 对象综合对位点重构
	virtual BOOL ExecuteObjectPlatformPosRestruct(std::vector<CCoordPos>& vcpObjectPos);


public:

	BOOL											m_bEnableMultiCombinationCode;					// 激活单产品扩展的对象拍照指令组合码
	int												m_nCombinationCode;								// 对象拍照指令组合码（即一个扩展产品拍照时，把拍照结果也赋给其它（由组合码决定）扩展产品）

	CMarkImagePos									m_ObjectSearchResults;							// 对象Mark搜索时，对象Mark的图像坐标值
	CMarkPlatformPos								m_ObjectMarkPlatformPos;						// 对象Mark搜索时，对象Mark的平台坐标值
	std::vector<CPlatformAxisPos*>					m_pObjectPlatformAxisPos;						// 对象Mark搜索时，平台和相机各轴的绝对位置
	std::vector<CPlatformAxisPos*>					m_vObjectCamPlatformAxisPos;					// 对象Mark搜索时，平台和相机各轴的绝对位置
	std::vector<CPlatformAxisPos*>					m_vObjectCamAxisPlatformAxisPos;				// 对象Mark搜索时，相机各轴所在平台的绝对位置
	std::vector<std::vector<CPlatformAxisPos*>>		m_vvpObjectCamPlatformAxisPosMultiEx;			// 对象Mark搜索扩展产品时，相机各轴的绝对位置
	std::vector<std::vector<CPlatformAxisPos*>>		m_vvpObjectCamAxisPlatformAxisPosMultiEx;		// 对象Mark搜索扩展产品时，相机平台各轴的绝对位置

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	// 虚拟对象
public:

	virtual void SetVirtualPlatformPos(CPlatformAxisPos* pPlatformAxisPos);						// 实时虚拟Mark搜索时平台和相机各轴所在的绝对位置
	virtual void SetVirtualPlatformPos(std::vector<CPlatformAxisPos*> pPlatformAxisPos);		// 虚拟对象时，对位平台位置
	virtual void SetVirtualPlatformPos(CPlatformAxisPos* pPlatformAxisPos,int nIndex);			// 虚拟对象时，对位平台位置

	virtual void SetVirtualMarkSearchResults(CMarkImagePos ImageResults);			// 虚拟Mark的图像搜索结果
	virtual CMarkImagePos GetVirtualMarkSearchResults();

	virtual CPlatformAxisPos* GetVirtualPlatformPos();
	virtual CPlatformAxisPos* GetVirtualPlatformPos(int nIndex);
	
	virtual CMarkPlatformPos GetObjectMarkPlatformPosEX();							// 基于标定角度情况下的对象Mark的全局平台坐标值（虚拟Mark）
	virtual CMarkPlatformPos GetVirtualMarkPltfmPos();								// 获取虚拟Mark的全局平台坐标值
	virtual CMarkPlatformPos GetVirtualMarkPltfmPosEX();

	virtual void SetVirtualCameraPos(std::vector<CPlatformAxisPos*> pCamPlatformAxisPos);
	virtual std::vector<CPlatformAxisPos*> GetVirtualCameraPos();

	// 计算对象Mark到虚拟Mark之间的位置关系
	virtual BOOL CalculateObjectToVirtualMarkOffset(std::vector<CCoordPos>& vObjectToVirtualOffsets);

	//	设置对象Mark到虚拟Mark之间的位置关系
	virtual void SetObjectMarkToVirtualMarkOffset(std::vector<CCoordPos> vObjectToVirtualOffsets);

	virtual BOOL ConvertObjectPltfmPtToVirtualPltfmPt(std::vector<CCoordPos> vObjectPlatformPos, std::vector<CCoordPos>& vVirtualPlatformPos);

	//  参数1：平台坐标；参数2：索引；参数3：图像坐标； 参数4：平台轴位置；参数5：标定轴位置
	virtual bool ConvertImagePosToCurPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos, BOOL bBaseOnCaliAxisbAngle);

public:

	CMarkImagePos					m_VirtualSearchResults;					// 虚拟Mark搜索时，虚拟Mark的图像坐标值
	CMarkPlatformPos				m_VirtualMarkPlatformPos;				// 虚拟Mark搜索时，虚拟Mark的平台坐标值
	CPlatformAxisPos*				m_pVirtualPlatformAxisPos;				// 虚拟Mark搜索时，平台和相机各轴的绝对位置
	std::vector<CPlatformAxisPos*>  m_vpVirtualPlatformAxisPos;
	std::vector<CPlatformAxisPos*>  m_vVirtualCamPlatformAxisPos;			// 虚拟Mark搜索时，平台和相机各轴的绝对位置


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	// 对位计算过程
public:

	// 设置是否使用内部添加的图像十字中心基准对位模式
	virtual void SetInnerAlignBench(BOOL bInnerAlignBench);								
	virtual BOOL GetInnerAlignBench();

	// 设置Mark所在图像的大小
	virtual void SetImageSize(int nIndex, BOOL bOK, CCoordPos cpImageSize);				
	virtual CCoordPos GetImageSize(int nIndex);

	// 保留序号
	virtual void SetAlignReservedIndex(int nAlignReservedIndex);
	virtual int GetAlignReservedIndex();

	// 基准居中对位补偿序号
	virtual void SetAlignBenchOffsetIndex(int nAlignBenchOffsetIndex);
	virtual int GetAlignBenchOffsetIndex();

	// 扩展产品序号
	virtual void SetAlignExProductIndex(int nAlignExProductIndex);
	virtual int GetAlignExProductIndex();

	//功能：目标硬补时采用默认方式获取目标固定补偿（即对象第一个相机和目标第一个相机的图像中心平台坐标差）
	//参数：默认目标固定补偿值（输出）
	virtual bool GetDefaultTargetFixOffset(CCoordPos &offset);

	//功能：计算目标固定补偿（目标采用靶标标定时使用此函数来计算目标固定补偿）
	//参数：目标固定补偿值（输出）
	virtual BOOL CaculateTargetFixOffset(CCoordPos &offset);
	virtual BOOL CaculateTargetFixOffsetForProduct(CCoordPos &offset);		// 使用二维码作为中介，建立目标对象坐标系位置关系
	virtual BOOL CaculateTargetFixOffsetForDmCode(CCoordPos &offset);		// 使用产品作为中介，建立目标对象坐标系位置关系

	// 图像定位结果进行畸变校正
	virtual BOOL ExecuteDisCorPoint(int nIndex, CCoordPos inputPoint, CCoordPos& outputPoint);
	virtual BOOL ExecuteDisCorPoint(int nIndex, std::vector<sc2Vector> inputPoint, std::vector<sc2Vector>& outputPoint);


	// 判断对位过程是否结束，（根据上步计算得到的平台偏差判断）
	virtual BOOL IsAlignFinished(int nTimes, BOOL bMulAligner = FALSE);

	// 判断整个对位过程是否成功
	virtual BOOL IsAlignSucceed(void);

//	virtual BOOL IsPCBPrecisionInspectFinished(int nInsTpye, std::vector<BOOL>& vbPosIndex,std::vector<double>&vdOffsetX,std::vector<double>&vdOffsetY);




	// 对位偏差计算：成功，返回TRUE；否则，返回FALSE（补偿相机及旋转中心变化量，计算各轴移动偏移量）
	virtual BOOL ExecuteAlign(int nAlnIndex = 0, BOOL bInspect = TRUE);

	// 当目标和对象不在同一个平面时，对位偏差计算：成功，返回TRUE；否则，返回FALSE（补偿相机及旋转中心变化量，计算各轴移动偏移量）
	virtual BOOL ExecuteAlignExHLLayer(int nAlnIndex, std::vector<CCoordPos> vcpCurCrossPos);

	// 目标、对象不分离
	virtual BOOL ExecuteAlign_Case1(int nAlnIndex = 0, BOOL bInspect = TRUE);

	// 目标、对象分离
	virtual BOOL ExecuteAlign_Case2(int nAlnIndex = 0, BOOL bInspect = TRUE);

	// 目标、对象分离，目标或者对象通过产品尺寸进行虚拟
	virtual BOOL ExecuteAlign_Case3(int nAlnIndex = 0, BOOL bInspect = TRUE);

	// 目标、对象分离，一组目标相机对应两组对象相机
//	virtual BOOL ExecuteAlign_Case4(int nAlnIndex = 0, BOOL bInspect = TRUE);

	// 高低平面对位
	virtual BOOL ExecuteAlign_Case5(int nAlnIndex = 0, BOOL bInspect = TRUE);

	// 八字对位
	virtual BOOL ExecuteAlign_Case6(int nAlnIndex = 0, BOOL bInspect = TRUE);


	// 将图像坐标值转换为当前平台坐标值
	// 1. 参数1：平台坐标；参数2：索引；参数3：图像坐标；
	virtual bool ConvertImagePosToCurPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos);

	// 2. 参数1：平台坐标；参数2：索引；参数3：图像坐标；
	//	  参数4：实时轴位置；
	// 应用：获取目标、对象平台坐标
	virtual bool ConvertImagePosToCurPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos);

	// 3. 参数1：平台坐标；参数2：索引；参数3：图像坐标；
	//    参数4：实时轴位置；参数5：相机轴位置；
	virtual bool ConvertImagePosToCurPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos);

	// 4. 参数1：平台坐标；参数2：索引；参数3：图像坐标；
	//    参数4：平台轴位置；参数5：相机轴位置；参数6：相机轴位置
	// 应用：获取目标、对象平台坐标，相机轴标定
	virtual bool ConvertImagePosToCurPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos,CPlatformAxisPos* pCamAxisPlatformAxisPos);

// 	//  5. 参数1：平台坐标；参数2：索引；参数3：图像坐标；
// 	//    参数4：平台轴位置；参数5：标定轴位置
// 	virtual bool ConvertImagePosToCurPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
// 		CPlatformAxisPos* pPlatformAxisPos, BOOL bBaseOnCaliAxisbAngle);


	// 6. 将图像坐标值转换为当前平台坐标值
	// (1).将CCoordPos类型图像坐标值(基础点、辅助点)转换为vector<CCoordPos>类型，
	// (2).然后vector<CCoordPos>类型图像坐标值转换为vector<CCoordPos>类型当前平台坐标值,
	// (3).最后将vector<CCoordPos>类型当前平台坐标值转换为CCoordPos类型当前平台坐标值
	// (4).备注，当不需要相机轴、相机轴所在轴时，可不输入当前相机轴位置和相机轴所在轴位置，函数内部根据相机平台类型进行判断。
	virtual bool ConvertImagePosToCurPlatformPosCommon(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos = NULL,CPlatformAxisPos* pCamAxisPlatformAxisPos = NULL);

	// 7. 将图像坐标值转换为当前平台坐标值的实体函数，只针对基础点进行转换
	// 备注，当不需要相机轴、相机轴所在轴时，可不输入当前相机轴位置和相机轴所在轴位置，函数内部根据相机平台类型进行判断。
	virtual bool ConvertImagePosToCurPlatformPosMainPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos = NULL,CPlatformAxisPos* pCamAxisPlatformAxisPos = NULL);

	// 9. 用靶标标定时，将图像坐标值转换为虚拟平台坐标值
	virtual bool ConvertImagePosToCurPlatformPosBoard(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos,BOOL DistortionCorrected = FALSE);




	// 拾取按照上料工位配置, 由于少X轴或Y轴导致旋转中心偏移, 回到基准位置时, 会有xy的偏差
	virtual BOOL GetAlignOffsetRevise(std::vector<CCoordPos> vcpObjectPlatformPos, std::vector<CCoordPos> vcpTargetPlatformPos, const CCoordPos& cpPlatformOffset, CCoordPos& cpPlatformOffsetRevise);

	// 获取用于对位的产品位置和角度（特别说明：该角度可能与补偿时获取产品的角度不一致）
	virtual BOOL GetAlignProductPosAngle(std::vector<CCoordPos> vObjectPlatformCoordPos, sc2Vector& vProductPos, scDegree& degProductAngle, BOOL bAuxiliary = FALSE,BOOL bCurObject =  TRUE);

	// 根据对象平台位置、目标平台位置、Mark数量、对位模式、平台类型，
	// 计算产品偏差、平台偏移量，以及对位调整后临时理论的对象平台位置和目标平台位置
	virtual BOOL GetAlginOffset(
		std::vector<CCoordPos> vcpObjectPlatformPos,
		std::vector<CCoordPos> vcpTargetPlatformPos,
		CCoordPos& cpProductOffset,
		CCoordPos& cpPlatformOffset,
		std::vector<CCoordPos>& vcpAlginedObjectPlatformPos,
		std::vector<CCoordPos>& vcpAlginedTargetPlatformPos);


	// 计算产品偏差、平台偏移量，最优化方法(根据目标、对象的所有相应对位点，对位完成后，对象与目标的所有相应对位点的距离平方和最小)
	virtual BOOL GetAlginOffset_Optimize(
		std::vector<CCoordPos> vcpObjectPlatformPos,
		std::vector<CCoordPos> vcpTargetPlatformPos,
		sc2Vector& vProductOffsetXY, scDegree& degProductOffsetD,
		sc2Vector& vPlatformOffsetXY, scDegree& degPlatformOffsetD);


	// 计算产品偏差、平台偏移量，几何方法(根据目标、对象的位置和角度(姿态)，直接计算对象与目标的产品偏差和平台偏移量)
	virtual BOOL GetAlginOffset_Geometry(
		sc2Vector vProductObjectPos, scDegree degProductObjectAngle,
		sc2Vector vProductTargetPos, scDegree degProductTargetAngle,
		sc2Vector& vProductOffsetXY, scDegree& degProductOffsetD,
		sc2Vector& vPlatformOffsetXY, scDegree& degPlatformOffsetD);


	// 根据平台类型，旋转平移对象平台坐标，得到相对于当前(原，变换前)坐标系的新平台坐标
	BOOL PlatformTransformRotateShiftObject(sc2Vector vProductObjectPos, scDegree degProductObjectAngle, 
		scDegree degPlatformOffsetD, sc2Vector vPlatformOffsetXY,
		sc2Vector& vNewProductObjectPos, scDegree& degNewProductObjectAngle);

	// 根据平台类型，旋转平移目标平台坐标，得到相对于当前(原，变换前)坐标系的新平台坐标
	BOOL PlatformTransformRotateShiftTarget(sc2Vector vProductTargetPos, scDegree degProductTargetAngle, 
		scDegree degPlatformOffsetD, sc2Vector vPlatformOffsetXY,
		sc2Vector& vNewProductTargetPos, scDegree& degNewProductTargetAngle);

	// 根据平台类型，旋转平移对象平台坐标，得到相对于当前(原，变换前)坐标系的新平台坐标
	// 根据平台类型，旋转平移目标平台坐标，得到相对于当前(原，变换前)坐标系的新平台坐标
	BOOL PlatformTransform(sc2Vector vProductObjectPos, scDegree degProductObjectAngle, 
		sc2Vector vProductTargetPos, scDegree degProductTargetAngle, 
		scDegree degPlatformOffsetD, sc2Vector vPlatformOffsetXY,
		sc2Vector& vNewProductObjectPos, scDegree& degNewProductObjectAngle, 
		sc2Vector& vNewProductTargetPos, scDegree& degNewProductTargetAngle);


	virtual BOOL GetAlginOffsetInline(
		std::vector<CCoordPos> vcpObjectPlatformPos,
		std::vector<CCoordPos> vcpTargetPlatformPos,
		CCoordPos& cpProductOffset,
		CCoordPos& cpPlatformOffset,
		std::vector<CCoordPos>& vcpAlginedObjectPlatformPos,
		std::vector<CCoordPos>& vcpAlginedTargetPlatformPos);

	// 是否进行坐标补偿计算 Coordinate Compensation
	virtual BOOL IsExecuteCoordinateCompensation();	

	// 坐标补偿计算 Coordinate Compensation
	virtual BOOL ExecuteCoordinateCompensation(std::vector<CCoordPos> vObjectPlatformPos, std::vector<CCoordPos> vVirtualObjectPlatformPos, std::vector<CCoordPos>& vFixedObjectPlatformPos);

	// Mark坐标系下的补偿
	// 获取用于补偿的产品位置和角度（产品位置即产品旋转中心，与相应的对位模式相关）
	virtual BOOL GetProductPosAngle(std::vector<CCoordPos> vObjectPlatformCoordPos, sc2Vector& vProductPos, scDegree& degProductAngle);

	// 在Mark坐标系下，根据系统对位补偿量，对对象Mark的平台坐标值进行旋转修正
	virtual void MarkCoordOffsetRotationObjectPlatformPos(std::vector<CCoordPos> vObjectPlatformCoordPos, std::vector<CCoordPos>& vFixedObjectPlatformCoordPos);

	// 在Mark坐标系下，根据系统对位补偿量，对对象Mark的平台坐标值进行平移修正
	virtual void MarkCoordOffsetXYObjectPltfmPt(std::vector<CCoordPos> vObjectPlatformCoordPos, std::vector<CCoordPos>& vFixedObjectPlatformCoordPos);

	// 对角Mark坐标系下的补偿	
	// 在对角Mark坐标系下，根据系统对位补偿量，对对象Mark的平台坐标值进行旋转修正
	virtual void MarkCoordDiagonalOffsetRotationObjectPlatformPos(scDegree degRotate, std::vector<CCoordPos> vObjectPlatformCoordPos,std::vector<CCoordPos>& vFixedObjectPlatformCoordPos);

	// 在对角Mark坐标系下，根据系统对位补偿量，对对象Mark的平台坐标值进行平移修正
	virtual void MarkCoordDiagonalOffsetXYObjectPltfmPt(std::vector<CCoordPos> vObjectPlatformCoordPos,std::vector<CCoordPos> vObjectPlatformCoordPosDirection, sc2Vector offsetXY,std::vector<CCoordPos>& vFixedObjectPlatformCoordPos);


	// 平台坐标系下的补偿
	// 在全局平台坐标系下，根据系统对位补偿量，对对象Mark的平台坐标值进行旋转修正
	virtual void PltfmCoordOffsetRotationObjectPltfmPt(std::vector<CCoordPos> vObjectPlatformCoordPos, std::vector<CCoordPos>& vFixedObjectPlatformCoordPos);

	// 在全局平台坐标系下，根据系统对位补偿量，对对象Mark的平台坐标值进行平移修正
	virtual void PltfmCoordOffsetXYObjectPltfmPt(std::vector<CCoordPos> vObjectPlatformCoordPos, std::vector<CCoordPos>& vFixedObjectPlatformCoordPos);

	//八字补偿时， 在Mark坐标系下，指定补偿量，对对象Mark的平台坐标值进行平移修正
	virtual void MarkCoordOffsetXYObjectPltfmPt(std::vector<CCoordPos> vObjectPlatformCoordPos,std::vector<CCoordPos>& vFixedObjectPlatformCoordPos, double dOffsetX, double dOffsetY);




	// 说明：对应获取偏差量的相关接口函数需要重新进行整理

	// 获取对位产品的偏移值，用于界面显示，用XYD形式显示
	virtual CCoordPos GetAlignProductOffset();

	// 获取XYPD平台时对象与上次目标旋转后的偏移值，用于界面显示，用XYD形式显示
	virtual CCoordPos GetAlignProductOffsetXYPD();

	//获取对位各轴偏差量计算结果，用于界面显示，用XYD形式显示
	virtual CCoordPos GetAlignPlatformOffset(void);

	//获取对位各轴偏差量计算结果，用于输出控制轴运动
	virtual CCoordPos GetAlignPlatformPosOffset(void);

	// 获取拾取按照上料对位计算时, 因轴带来的xy偏差
	virtual CCoordPos GetAlignPlatformPosOffsetPickRevise();

	// 对位偏差计算成功后，获取对位移动的绝对位置（）																					
	virtual CPlatformAxisPos* GetAlignPlatformMovementResults(BOOL bUseOffset = FALSE);

	// 对位偏差计算成功后，获取对位移动的绝对位置（）																					
	virtual CPlatformAxisPos* GetAdjustAlignPlatformMovementResults(double dratioX,double dratioY,double dratioD = 1.0);

	// 中间对位过程，当左右相机由独立的XY轴控制时，获取左右相机移动量
	virtual std::vector<CPlatformAxisPos*> GetAlignCameraMovementResults();


	virtual CCoordPos CalculateTempPlatformPos();
	virtual CCoordPos GetCalculateTempPlatformPos();


public:
	
	BOOL							m_bInnerAlignBench;								// 是否使用内部添加的图像十字中心基准对位模式
	CMarkImagePos					m_ImageSizes;									// Mark所在图像的大小

//	BOOL   m_bUsePreTargetPos;					// 连续对位时，标记是否使用上一次目标的位置结果，适用于XY+D平台，目标在D平台上
	double m_dTargetX[MAXTARGETPOS];			// 存储目标mark图像坐标
	double m_dTargetY[MAXTARGETPOS];			// 存储目标mark图像坐标
	double m_dDAngle;							// 存储第一次对位角度偏移量

	double m_dAuxiliaryTargetX0[6];				//存储辅助目标mark0图像坐标
	double m_dAuxiliaryTargetY0[6];				//存储辅助目标mark0图像坐标
	double m_dAuxiliaryTargetX1[6];				//存储辅助目标mark1图像坐标
	double m_dAuxiliaryTargetY1[6];				//存储辅助目标mark1图像坐标


protected:

	int								m_nAlignReservedIndex;					// 保留序号
	int								m_nAlignBenchOffsetIndex;				// 基准居中对位补偿序号
	int								m_nAlignnExProductIndex;				// 扩展产品序号

	int								m_nAlnIndex;							// 对位次数索引序号
	BOOL							m_bIsAlignerSuccess;					//	TRUE对位成功；FALSE超过最大对位次数

	CPlatformAxisPos*				m_pPlatformPos;				        	// 每次对位计算成功后的平台各轴的绝对位置
	CCoordPos						m_PlatformOffset;						// 每次对位计算的偏移量，按照xyd的形式给出
	CCoordPos						m_PlatformOffsetRevise;					// 拾取按照上料对位后, 由于轴的缺失, 旋转中心偏移,导致xy变动	
	CCoordPos						m_TempPlatformPos;

	CCoordPos						m_ProductOffset;						// 每次对位计算的产品偏移量，按照xyd的形式给出
	CCoordPos						m_ProductOffsetXYPD;					// XYPD平台与上一次目标选择后的产品偏移量，按照xyd的形式给出

	std::vector<CPlatformAxisPos*>  m_vCamPlatformMovement;					// 每次对位后，为使左右相机都能准确拍摄到对象Mark位置时，左右相机平台移动量





//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
public:

	// 执行基准mark修正
	virtual CBenchTargetMarkFixInfo* ExacuteBenchMarkFix( std::vector<CCoordPos> imageCoordPoses, 
		std::vector<CPlatformAxisPos*> vPlatformAxisPoses) ;

	virtual CBenchTargetMarkFixInfo* ExacuteBenchMarkFix( std::vector<CCoordPos> imageCoordPoses, 
		CPlatformAxisPos* pPlatformAxisPos, std::vector<CPlatformAxisPos*> vCamPlatformAxisPoses);




	// 拾取
	virtual int GetCalibCenterPlatformMovePosNum(int nMarkType);		// XY+D拾取
	virtual int GetCalibPlatformPickMovePosNum();						// XY+D拾取 - XY
	virtual int GetCalibCenterPlatformPickMovePosNum();					// XY+D拾取 - D

	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformPickMovePos(int nPosIndex);
	virtual std::vector<CPlatformAxisPos*> GetCalibCenterPlatformPickMovePos(int nPosIndex);

	virtual BOOL ExecuteCalibratePickSingleCamEX(int nCamIndex);							// XY+D平台拾取，XY标定，用9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateCenterPickSingleCamEX(int nCamIndex);						// XY+D平台拾取，D标定， 用9点H矩阵方式进行标定

	virtual BOOL ExecuteCalibrateXYSingleCamPickEX(int nCamIndex);							// XY平台拾取,用9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateXYSingleCamPick(int nCamIndex);							// XY平台拾取,用5点H矩阵方式进行标定

	virtual BOOL ExecuteCalibrateSingleCamXYWholeDmCodeBoardBoardToPlatPick(int nCalibPos);	//全局二维码标定时拾取XY平台靶标到平台坐标映射
	virtual BOOL ExecuteCalibrateSingleCamWholeDmCodeBoardBoardToPlatPick(int nCalibPos);	//全局二维码标定时拾取相机shareXY时靶标到平台坐标映射

	virtual CBenchTargetMarkPickInfo* ExacuteBenchMarkPick( std::vector<CCoordPos> imageCoordPoses, 
		std::vector<CPlatformAxisPos*> vPlatformAxisPoses, CPlatformAxisPos* pPickPlatformAxisPoses,int nIndex,CBenchTargetMarkPickInfo* OriginFixInfo,int nExProductIndex=0) ;

	virtual BOOL ExecuteAlignPick(int nAlnIndex/* = 0*/);



	// 折弯
public:

	virtual BOOL ExeBendInsInfoCalcu(std::vector<sc2Vector>& vTargetPlaftormPos, std::vector<sc2Vector>& vObjectPlatformPos, std::vector<double>& vdDistance);
	virtual void SetBendAlignProcess(BOOL bBendAlignProcess);

	BOOL m_bBendAlignProcess;
	
	
	// PCB
//	virtual void GetAlignerResultStateType(enumAlignerResultStateType & _TempenumAlignerResultStateType,CMarkImagePos ImageResults);		// 设置目标Mark的图像搜索结果


//public:

// 		enumAlignerResultStateType	m_enumAlignerResultStateType;					// 位置X处定位状态；
// 		enumAlignerResultStateType	m_targetAlignResultMode;
// 		enumAlignerResultStateType	m_objectAlignResultMode;
//		toAlignerPointMethodType	m_PointMethodType; 

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


	
public:

	virtual BOOL ExecuteCalibrateBenchMarkRgisterPlace(int nCalibPos, int nMarkType);		// X+XYD上料平台执行基准注册标定计算，保存X轴放料轴位置
	virtual BOOL ExecuteCalibrateBenchMarkRgister(int nCalibPos, int nMarkType);			// X+XYD上料平台执行基准注册标定计算，拍照并保存X轴拍照轴位置


	// 日志记录
	void AlignToolLogRecord(CString strlog,int nPlatformIndex);							// 记录详细的对位过程
//	void AlignToolLogRecordFormat(CString strlog,int nPlatformIndex);					// 记录详细的对位过程
	void AlignToolLogRecordProduct(CString strlog,int nPlatformIndex);					// 记录对位过程中产品信息
//	void AlignToolLogRecord(CString strPathFileName,CString strlog,int nPlatformIndex);	

	// TT2020_03_25↓↓↓↓↓↓↓↓↓↓↓↓
//	bool CreateMultipleDirectory(CString& szPath, char split);
//	void AlignToolCalibTargetLogRecord(CString strPathFileName,CString strLog,bool bLogTimeEnable=1);

//	void SaveCalibTargetProjectData(int nCamIndex,int nBaseCamIndex,std::vector<sc2Vector>& tarImgPos,std::vector<sc2Vector>& objImgPos);

	// TT2020_03_25↑↑↑↑↑↑↑↑↑↑↑↑
// 	void SaveCalibTargetProcessData(int nCamIndex,int nBaseCamIndex,
// 		int nOutNum ,int nTotolNum,double* h,std::vector<sc2Vector>& tarImgInputPos,
// 		std::vector<sc2Vector>& objImgInputPos,std::vector<sc2Vector>& tarImgPosDisCor, 
// 		std::vector<sc2Vector>& objImgPosDisCor, std::vector<sc2Vector>& tagToObjError, std::vector<sc2Vector>& objToTagError);
	
//	CString strRecordFileFullPathLog;


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	virtual CString	GetAlignErrInfo();
	int m_nAlignErrNum;

	void CalculateCalibTargetProcessData(int nCamIndex,int nBaseCamIndex,
		std::vector<sc2Vector>& tarImgPos,std::vector<sc2Vector>& objImgPos, 
		std::vector<sc2Vector>& TarImagTransferObjImage,std::vector<sc2Vector>& ObjImagTransferTarImage);

	BOOL m_bEnableKeyenceCode;

};


#endif // !defined(AFX_SVALIGNERTOOL_H__3C92A5F4_E7B2_4983_A287_F579AA3F5837__INCLUDED_)
