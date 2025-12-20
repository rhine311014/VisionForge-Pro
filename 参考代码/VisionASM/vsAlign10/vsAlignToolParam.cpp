#include "StdAfx.h"

#include "vsAlignToolParam.h"


/************************************************************************/
/*								其它								    */
/************************************************************************/
CProductSizeInfo::CProductSizeInfo()
{
	m_dObjectL1 = 0;
	m_dObjectW1 = 0;
	m_dTargetL2 = 0;
	m_dTargetW2 = 0;

	m_bOffsetXYEnable = false;
	m_dF1 = 0;
	m_dF2 = 0;
}


// 补偿设置（补偿的方向、X和Y补偿值对调）
CProductOffsetInfo::CProductOffsetInfo()
{
	m_bEnableProductOffsetXDirection = FALSE;
	m_bEnableProductOffsetYDirection = FALSE;
	m_bEnableProductOffsetDDirection = FALSE;

	m_bEnableProductOffsetXSetting = FALSE;
	m_bEnableProductOffsetYSetting = FALSE;
	m_bEnableProductOffsetDSetting = FALSE;
}

//lzk 0405 位置虚拟
CProductPositionVirtualInfo::CProductPositionVirtualInfo()
{
	m_bEnableObjectPositionVirtualMode = false;
	m_bEnableTargetPositionVirtualMode = false;
	m_nObjectPositionVirtualMode = 0;
	m_nTargetPositionVirtualMode = 0;		 
}

CAlignCheckInfo::CAlignCheckInfo()
{
	m_bCheckTargetAndObjectDis  = FALSE; // 是否检测目标与对象之间的最大距离
	m_nCheckMode = 0;				// 0：输入模式；1：检测模式

	m_bCheckTargetDis  = FALSE;			// 是否检测目标的距离
	m_dTargetDisThreshold = 0.01;
	m_dTargetDis0 = 0.01;				// 目标的距离0
	m_dTargetDis1 = 0.01;				// 目标的距离1
	m_dTargetDis2 = 0.01;
	m_dTargetDis3 = 0.01;
	m_dTargetDis4 = 0.01;
	m_dTargetDis5 = 0.01;

	m_bCheckObjectDis  = FALSE;			// 是否检测对象的距离
	m_dObjectDisThreshold = 0.01;
	m_dObjectDis0 = 0.01;				// 对象的距离0
	m_dObjectDis1 = 0.01;				// 对象的距离1
	m_dObjectDis2 = 0.01;
	m_dObjectDis3 = 0.01;
	m_dObjectDis4 = 0.01;
	m_dObjectDis5 = 0.01;

	m_bCheckTargetAngle  = FALSE;		// 是否检测目标的角度
	m_dTargetAngleThreshold = 0.01;
	m_dTargetAngle0 = 0.01;
	m_dTargetAngle1 = 0.01;
	m_dTargetAngle2 = 0.01;
	m_dTargetAngle3 = 0.01;
	m_dTargetAngle4 = 0.01;
	m_dTargetAngle5 = 0.01;

	m_bCheckObjectAngle  = FALSE;		// 是否检测对象的角度
	m_dObjectAngleThreshold = 0.01;
	m_dObjectAngle0 = 0.01;
	m_dObjectAngle1 = 0.01;
	m_dObjectAngle2 = 0.01;
	m_dObjectAngle3 = 0.01;
	m_dObjectAngle4 = 0.01;
	m_dObjectAngle5 = 0.01;




	m_dMaxTargetAngle = 0.01;			// 目标的最大角度(单位度)
//	m_dMaxObjectAngle = 0.01;			// 对象的最大角度(单位度)
	
//	m_dMaxTargetAndObjectDis = 0.01;	// 目标与对象之间的最大距离;目标对象独立检查时为目标最大距离

//	m_bEnableTarObjSeparateCheck = FALSE;//是否允许目标和对独立设置检查距离进行独立检查
//	m_dMaxObjectDis = 0.01;             //对象最大距离

//	m_bCheckPixelDis  = FALSE;			// 是否检测像素距离
//	m_dPixelDisX1 = 2;					// 像素距离
//	m_dPixelDisY1 = 2;
//	m_dPixelDisX2 = 2;
//	m_dPixelDisY2 = 2;

}

/************************************************************************/
/*					 CCalibrateParam implementations				    */
/************************************************************************/

CCalibrateParam::CCalibrateParam()
{
	m_nCalibPatternNum = 0; 
	m_bPlatformCalibrateRefine = FALSE;

	m_dPlatformCalibMovement.m_dPosX  = 0.2;
	m_dPlatformCalibMovement.m_dPosY  = 0.2;
	m_dPlatformCalibMovement.m_dAngle = 0.5;

	m_dCameraCalibMovement.m_dPosX = 0.2;
	m_dCameraCalibMovement.m_dPosY = 0.2;

	m_dPlatformCalibRefineMoveD = 0.5;						// 平台标定求精的D方向移动量
	m_nPlatformCalibRefineTime = 3;						// 平台标定求精次数
	m_nSearchDelayTime = 100;									// 搜索延时

	m_nEnableDisCor = 0;								// 畸变校正是否启用
	m_dTileX = 1;										// 畸形校正X长度
	m_dTileY = 1;										// 畸形校正Y长度
	m_bOutPoints = false;								// 畸形校正是否启用外围点
	m_iThre = 16;										// 畸形校正强度

	m_eCalibMarkPatternMode = eTraditionalCalib;

};

CCalibrateParam::~CCalibrateParam()
{
	m_vstrCalibPatternIDS.clear();
};

CCalibrateParam::CCalibrateParam(const CCalibrateParam& lCP)
{
	m_nCalibPatternNum				= lCP.m_nCalibPatternNum; 
	m_vstrCalibPatternIDS			= lCP.m_vstrCalibPatternIDS;
	m_bPlatformCalibrateRefine		= lCP.m_bPlatformCalibrateRefine;

	m_dPlatformCalibMovement		= lCP.m_dPlatformCalibMovement;
	m_dCameraCalibMovement			= lCP.m_dCameraCalibMovement;
	m_dPlatformCalibRefineMoveD		= lCP.m_dPlatformCalibRefineMoveD;						// 平台标定求精的D方向移动量
	m_nPlatformCalibRefineTime		= lCP.m_nPlatformCalibRefineTime;						// 平台标定求精次数
	m_nSearchDelayTime				= lCP.m_nSearchDelayTime;								// 搜索延时

	m_nEnableDisCor					= lCP.m_nEnableDisCor;
	m_dTileX						= lCP.m_dTileX;
	m_dTileY						= lCP.m_dTileY;
	m_bOutPoints					= lCP.m_bOutPoints;
	m_iThre							= lCP.m_iThre;

	m_eCalibMarkPatternMode			= lCP.m_eCalibMarkPatternMode;

}

CCalibrateParam& CCalibrateParam::operator = (const CCalibrateParam& lCP)
{
	if (this == &lCP)
	{
		return *this;
	}

	m_nCalibPatternNum				= lCP.m_nCalibPatternNum; 
	m_vstrCalibPatternIDS			= lCP.m_vstrCalibPatternIDS;
	m_bPlatformCalibrateRefine		= lCP.m_bPlatformCalibrateRefine;

	m_dPlatformCalibMovement		= lCP.m_dPlatformCalibMovement;
	m_dCameraCalibMovement			= lCP.m_dCameraCalibMovement;
	m_dPlatformCalibRefineMoveD		= lCP.m_dPlatformCalibRefineMoveD;						// 平台标定求精的D方向移动量
	m_nPlatformCalibRefineTime		= lCP.m_nPlatformCalibRefineTime;						// 平台标定求精次数
	m_nSearchDelayTime				= lCP.m_nSearchDelayTime;								// 搜索延时

	m_nEnableDisCor					= lCP.m_nEnableDisCor;
	m_dTileX						= lCP.m_dTileX;
	m_dTileY						= lCP.m_dTileY;
	m_bOutPoints					= lCP.m_bOutPoints;
	m_iThre							= lCP.m_iThre;

	m_eCalibMarkPatternMode			= lCP.m_eCalibMarkPatternMode;

	return *this;
}

BOOL CCalibrateParam::operator==(const CCalibrateParam& lCP)
{
	if (   m_nCalibPatternNum					!= lCP.m_nCalibPatternNum 
		|| m_vstrCalibPatternIDS				!= lCP.m_vstrCalibPatternIDS 

		|| m_dPlatformCalibMovement.m_dPosX		!= lCP.m_dPlatformCalibMovement.m_dPosX
		|| m_dPlatformCalibMovement.m_dPosY		!= lCP.m_dPlatformCalibMovement.m_dPosY
		|| m_dPlatformCalibMovement.m_dAngle	!= lCP.m_dPlatformCalibMovement.m_dAngle

		|| m_dCameraCalibMovement.m_dPosX		!= lCP.m_dCameraCalibMovement.m_dPosX
		|| m_dCameraCalibMovement.m_dPosY		!= lCP.m_dCameraCalibMovement.m_dPosY
		|| m_dCameraCalibMovement.m_dAngle		!= lCP.m_dCameraCalibMovement.m_dAngle

		|| m_bPlatformCalibrateRefine			!= lCP.m_bPlatformCalibrateRefine
		|| m_dPlatformCalibRefineMoveD			!= lCP.m_dPlatformCalibRefineMoveD
		|| m_nPlatformCalibRefineTime			!= lCP.m_nPlatformCalibRefineTime
		|| m_nSearchDelayTime					!= lCP.m_nSearchDelayTime

		|| m_nEnableDisCor						!= lCP.m_nEnableDisCor
		|| m_dTileX								!= lCP.m_dTileX
		|| m_dTileY								!= lCP.m_dTileY
		|| m_bOutPoints							!= lCP.m_bOutPoints
		|| m_iThre								!= lCP.m_iThre

		|| m_eCalibMarkPatternMode				!= lCP.m_eCalibMarkPatternMode)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCalibrateParam::operator!=(const CCalibrateParam& lCP)
{
	return !(*this == lCP);
}

void CCalibrateParam::SetCalibPatternNum( int nNum )
{
	if (nNum < 0)
	{
		return;
	}
	m_vstrCalibPatternIDS.resize(nNum);
	m_nCalibPatternNum = nNum;
}

int CCalibrateParam::GetCalibPatternNum()
{
	m_nCalibPatternNum = m_vstrCalibPatternIDS.size();
	return m_nCalibPatternNum;
}

BOOL CCalibrateParam::SetCalibPatternID( int nIndex, CString strID )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nCalibPatternNum));
	if ((nIndex < 0) || (nIndex >= m_nCalibPatternNum))
	{
		return FALSE;
	}
	else
	{
		m_vstrCalibPatternIDS.at(nIndex) = strID;
		return TRUE;
	}
}

CString CCalibrateParam::GetCalibPatternID( int nIndex )
{
	ASSERT((nIndex > 0) && (nIndex <= m_nCalibPatternNum));
	if ((nIndex < 1) || (nIndex > m_nCalibPatternNum))
	{
		return _T("");
	}
	else
	{
		return m_vstrCalibPatternIDS.at(nIndex);
	}

}

void CCalibrateParam::SetPlatformCalibMovement( CCoordPos movement )
{
	m_dPlatformCalibMovement = movement;
}

CCoordPos CCalibrateParam::GetPlatformCalibMovement()
{
	return m_dPlatformCalibMovement;
}

void CCalibrateParam::SetCameraCalibMovement( CCoordPos movement )
{
	m_dCameraCalibMovement = movement;
}

CCoordPos CCalibrateParam::GetCameraCalibMovement()
{
	return m_dCameraCalibMovement;
}

void CCalibrateParam::SetPlatformCalibrateRefine( BOOL bRefine )
{
	m_bPlatformCalibrateRefine = bRefine;

}

BOOL CCalibrateParam::GetPlatformCalibrateRefine()
{
	return m_bPlatformCalibrateRefine;
}

void CCalibrateParam::SetPlatformCalibRefineMoveD(double dMoveD)
{
	m_dPlatformCalibRefineMoveD = dMoveD;
}

double CCalibrateParam::GetPlatformCalibRefineMoveD()
{
	return m_dPlatformCalibRefineMoveD;
}

void CCalibrateParam::SetPlatformCalibRefineTime(int dRefineTime)
{
	m_nPlatformCalibRefineTime = dRefineTime;
}

int CCalibrateParam::GetPlatformCalibRefineTime()
{
	return m_nPlatformCalibRefineTime;
}

void CCalibrateParam::SetSearchDelayTime(int nSearchDelayTime)
{
	m_nSearchDelayTime = nSearchDelayTime;
}

int  CCalibrateParam::GetSearchDelayTime()
{
	return m_nSearchDelayTime;
}

void CCalibrateParam::SetCalibMarkPatternMode(CalibMarkPatternMode mode)
{
	m_eCalibMarkPatternMode = mode;
}

CalibMarkPatternMode CCalibrateParam::GetCalibMarkPatternMode()
{
	return m_eCalibMarkPatternMode;
}


/************************************************************************/
/*			  	    CCalibratedInfo implementations		                */
/************************************************************************/
CCalibratedInfo::CCalibratedInfo()
{
	m_pPlatformAxisPos = NULL;//new CPlatformXYDAxisPos();
	m_pCamPlatformAxisPos = NULL;//new CPlatformXYAxisPos();
	m_pCamAxisPlatformAxisPos = NULL;
	m_dImageToPlatformAnlge = 0.0;
	m_bValid = FALSE;

	int i = 0;
	for(i = 0; i < 9; i++)
	{
		m_PlatformTranferH[i] = 0.0;
		m_PlatformTranferH1[i] = 0.0;
		m_PlatformTranferH2[i] = 0.0;
	}
	m_PlatformTranferH[0] = 1.0;
	m_PlatformTranferH1[0] = 1.0;
	m_PlatformTranferH2[0] = 1.0;
	m_PlatformTranferH[4] = 1.0;
	m_PlatformTranferH1[4] = 1.0;
	m_PlatformTranferH2[4] = 1.0;
	m_PlatformTranferH[8] = 1.0;
	m_PlatformTranferH1[8] = 1.0;
	m_PlatformTranferH2[8] = 1.0;
	m_cpRefineOffset = CCoordPos(0.0,0.0,0.0);
	m_cpRefineOffsetH1 = CCoordPos(0.0,0.0,0.0);
	m_cpRefineOffsetH2 = CCoordPos(0.0,0.0,0.0);
	m_vCalibBoardSize = sc2Vector(1.0, 1.0);
}

CCalibratedInfo::CCalibratedInfo( const CCalibratedInfo& lCInfo )
{
	m_bValid						= lCInfo.m_bValid;
	m_PlatformTranferMatrix			= lCInfo.m_PlatformTranferMatrix;
	m_cpMarkPlatformCoordPos		= lCInfo.m_cpMarkPlatformCoordPos;
	m_cpMarkImgCoordPos				= lCInfo.m_cpMarkImgCoordPos;
	m_cpMarkPlatformCoordPosBat		= lCInfo.m_cpMarkPlatformCoordPosBat;
	m_cpMarkImgCoordPosBat			= lCInfo.m_cpMarkImgCoordPosBat;
	m_dImageToPlatformAnlge			= lCInfo.m_dImageToPlatformAnlge;
	m_CameraTranferMatrix			= lCInfo.m_CameraTranferMatrix;
	m_pPlatformAxisPos				= NULL;
	m_pCamPlatformAxisPos			= NULL;
	m_pCamAxisPlatformAxisPos		= NULL;
	m_cpRefineOffset				= lCInfo.m_cpRefineOffset;
	m_cpRefineOffsetH1				= lCInfo.m_cpRefineOffsetH1;
	m_cpRefineOffsetH2				= lCInfo.m_cpRefineOffsetH2;
	m_result						= lCInfo.m_result;

	int i = 0;
	for(i = 0; i < 9; i++)
	{
		m_PlatformTranferH[i] = lCInfo.m_PlatformTranferH[i];
		m_PlatformTranferH1[i] = lCInfo.m_PlatformTranferH1[i];
		m_PlatformTranferH2[i] = lCInfo.m_PlatformTranferH2[i];
	}

	for(i = 0; i < CALIBMOVEPOSNUM; i++)
	{
		m_cpObjectMarkImgCoordPos[i] = lCInfo.m_cpObjectMarkImgCoordPos[i];
	}

	for(i = 0; i < CALIBMOVEPOSNUM; i++)
	{
		m_cpTargetMarkImgCoordPos[i] = lCInfo.m_cpTargetMarkImgCoordPos[i];
	}

	if (lCInfo.m_pPlatformAxisPos != NULL)
	{
		PlatformType platformType = lCInfo.m_pPlatformAxisPos->GetPlatformType();
		switch (platformType)
		{
		case ePlatformXYD:
			m_pPlatformAxisPos = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXY:
			m_pPlatformAxisPos = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformX:
			m_pPlatformAxisPos = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformY:
			m_pPlatformAxisPos = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXYPD:
			m_pPlatformAxisPos = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformX1X2Y:
			m_pPlatformAxisPos = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXY1Y2:
			m_pPlatformAxisPos = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformYX1X2PD:
			m_pPlatformAxisPos = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformYDPX1X2:
			m_pPlatformAxisPos = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformDXY:
			m_pPlatformAxisPos = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXDPY:
			m_pPlatformAxisPos = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXDPY1:
			m_pPlatformAxisPos = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		default:
			break;
		}
	}

	if (lCInfo.m_pCamPlatformAxisPos != NULL)
	{
		PlatformType camPlatformType = lCInfo.m_pCamPlatformAxisPos->GetPlatformType();
		switch (camPlatformType)
		{
		case ePlatformXY:
			m_pCamPlatformAxisPos = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformX:
			m_pCamPlatformAxisPos = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformY:
			m_pCamPlatformAxisPos = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		default:
			break;
		}
	}

	if (lCInfo.m_pCamAxisPlatformAxisPos != NULL)
	{
		PlatformType camPlatformType = lCInfo.m_pCamAxisPlatformAxisPos->GetPlatformType();
		switch (camPlatformType)
		{
		case ePlatformXY:
			m_pCamAxisPlatformAxisPos = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformX:
			m_pCamAxisPlatformAxisPos = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformY:
			m_pCamAxisPlatformAxisPos = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		default:
			break;
		}
	}

	for(i = 0; i < CALIBBOARDMARKPOSNUM; i++)
	{
		m_cpCalibBoardMarkImgCoordPos[i] = lCInfo.m_cpCalibBoardMarkImgCoordPos[i];
	}

	for(i = 0; i < CALIBBOARDMARKPOSNUM; i++)
	{
		m_cpCalibBoardMarkBoardCoordPos[i] = lCInfo.m_cpCalibBoardMarkBoardCoordPos[i];
	}

	m_vCalibBoardSize = lCInfo.m_vCalibBoardSize;
}

CCalibratedInfo::CCalibratedInfo( PlatformType ePlatformType, CamPlatformType eCamPlatformType )
{
	switch (ePlatformType)
	{
	case ePlatformXYD:
		m_pPlatformAxisPos = new CPlatformXYDAxisPos();
		break;
	case ePlatformXYPD:
		m_pPlatformAxisPos = new CPlatformXYPDAxisPos();
		break;
	case ePlatformX:
		m_pPlatformAxisPos = new CPlatformXAxisPos();
		break;
	case ePlatformY:
		m_pPlatformAxisPos = new CPlatformYAxisPos();
		break;
	case ePlatformX1X2Y:
		m_pPlatformAxisPos = new CPlatformX1X2YAxisPos();
		break;
	case ePlatformXY1Y2:
		m_pPlatformAxisPos = new CPlatformXY1Y2AxisPos();
		break;
	case ePlatformXY:
		m_pPlatformAxisPos = new CPlatformXYAxisPos();
		break;
	case ePlatformYDPX1X2:
		m_pPlatformAxisPos = new CPlatformYDPX1X2AxisPos();
		break;
	case ePlatformYX1X2PD:
		m_pPlatformAxisPos = new CPlatformYX1X2PDAxisPos();
		break;
	case ePlatformDXY:
		m_pPlatformAxisPos = new CPlatformDXYAxisPos();
		break;
	case ePlatformXDPY:
		m_pPlatformAxisPos = new CPlatformXDPYAxisPos();
		break;
	case ePlatformXDPY1:
		m_pPlatformAxisPos = new CPlatformXDPY1AxisPos();
		break;
	default:
		m_pPlatformAxisPos = NULL;
		break;
	}

	m_pCamPlatformAxisPos = new CPlatformXYAxisPos();
	m_pCamAxisPlatformAxisPos = new CPlatformXYAxisPos();
	int i = 0;
	for(i = 0; i < 9; i++)
	{
		m_PlatformTranferH[i] = 0.0;
		m_PlatformTranferH1[i] = 0.0;
		m_PlatformTranferH2[i] = 0.0;
	}
	m_PlatformTranferH[0] = 1.0;
	m_PlatformTranferH1[0] = 1.0;
	m_PlatformTranferH2[0] = 1.0;
	m_PlatformTranferH[4] = 1.0;
	m_PlatformTranferH1[4] = 1.0;
	m_PlatformTranferH2[4] = 1.0;
	m_PlatformTranferH[8] = 1.0;
	m_PlatformTranferH1[8] = 1.0;
	m_PlatformTranferH2[8] = 1.0;

	m_dImageToPlatformAnlge = 0.0;
	m_bValid = FALSE;
	m_cpRefineOffset = CCoordPos(0.0,0.0,0.0);
	m_cpRefineOffsetH1 = CCoordPos(0.0,0.0,0.0);
	m_cpRefineOffsetH2 = CCoordPos(0.0,0.0,0.0);
	m_vCalibBoardSize = sc2Vector(1.0, 1.0);
}

CCalibratedInfo::~CCalibratedInfo()
{
	if (m_pPlatformAxisPos != NULL)
	{
		delete m_pPlatformAxisPos;
		m_pPlatformAxisPos = NULL;
	}
	if (m_pCamPlatformAxisPos != NULL)
	{
		delete m_pCamPlatformAxisPos;
		m_pCamPlatformAxisPos = NULL;
	}
	if(m_pCamAxisPlatformAxisPos != NULL)
	{
		delete m_pCamAxisPlatformAxisPos;
		m_pCamAxisPlatformAxisPos = NULL;
	}
	m_bValid = FALSE;
}

CCalibratedInfo& CCalibratedInfo::operator=( const CCalibratedInfo& lCInfo )
{
	if (this == &lCInfo)
	{
		return *this;
	}
	m_bValid = lCInfo.m_bValid;
	m_PlatformTranferMatrix = lCInfo.m_PlatformTranferMatrix;
	m_cpMarkPlatformCoordPos = lCInfo.m_cpMarkPlatformCoordPos;
	m_cpMarkImgCoordPos = lCInfo.m_cpMarkImgCoordPos;
	m_cpMarkPlatformCoordPosBat = lCInfo.m_cpMarkPlatformCoordPosBat;
	m_cpMarkImgCoordPosBat = lCInfo.m_cpMarkImgCoordPosBat;
	m_dImageToPlatformAnlge = lCInfo.m_dImageToPlatformAnlge;
	m_CameraTranferMatrix = lCInfo.m_CameraTranferMatrix;
	m_cpRefineOffset		 = lCInfo.m_cpRefineOffset;
	m_cpRefineOffsetH1		 = lCInfo.m_cpRefineOffsetH1;
	m_cpRefineOffsetH2		 = lCInfo.m_cpRefineOffsetH2;
	m_result = lCInfo.m_result;
	int i = 0;
	for(i = 0; i < 9; i++)
	{
		m_PlatformTranferH[i] = lCInfo.m_PlatformTranferH[i];
		m_PlatformTranferH1[i] = lCInfo.m_PlatformTranferH1[i];
		m_PlatformTranferH2[i] = lCInfo.m_PlatformTranferH2[i];
	}

	for(i = 0; i < CALIBMOVEPOSNUM; i++)
	{
		m_cpObjectMarkImgCoordPos[i] = lCInfo.m_cpObjectMarkImgCoordPos[i];
	}

	for(i = 0; i < CALIBMOVEPOSNUM; i++)
	{
		m_cpTargetMarkImgCoordPos[i] = lCInfo.m_cpTargetMarkImgCoordPos[i];
	}

	if (m_pPlatformAxisPos != NULL)
	{
		delete m_pPlatformAxisPos;
		m_pPlatformAxisPos = NULL;
	}
	if (m_pCamPlatformAxisPos != NULL)
	{
		delete m_pCamPlatformAxisPos;
		m_pCamPlatformAxisPos = NULL;
	}
	if (m_pCamAxisPlatformAxisPos != NULL)
	{
		delete m_pCamAxisPlatformAxisPos;
		m_pCamAxisPlatformAxisPos = NULL;
	}

	if (lCInfo.m_pPlatformAxisPos != NULL)
	{
		PlatformType platformType = lCInfo.m_pPlatformAxisPos->GetPlatformType();
		switch (platformType)
		{
		case ePlatformXYD:
			m_pPlatformAxisPos = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXYPD:
			m_pPlatformAxisPos = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformX1X2Y:
			m_pPlatformAxisPos = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXY1Y2:
			m_pPlatformAxisPos = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXY:
			m_pPlatformAxisPos = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformX:
			m_pPlatformAxisPos = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformY:
			m_pPlatformAxisPos = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformYX1X2PD:
			m_pPlatformAxisPos = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformYDPX1X2:
			m_pPlatformAxisPos = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformDXY:
			m_pPlatformAxisPos = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXDPY:
			m_pPlatformAxisPos = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		case ePlatformXDPY1:
			m_pPlatformAxisPos = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)lCInfo.m_pPlatformAxisPos));
			break;
		default:
			break;
		}
	}
	if (lCInfo.m_pCamPlatformAxisPos != NULL)
	{
		PlatformType camPlatformType = lCInfo.m_pCamPlatformAxisPos->GetPlatformType();
		switch (camPlatformType)
		{
		case ePlatformXYD:
			m_pCamPlatformAxisPos = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformXYPD:
			m_pCamPlatformAxisPos = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformXY:
			m_pCamPlatformAxisPos = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformX:
			m_pCamPlatformAxisPos = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformY:
			m_pCamPlatformAxisPos = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformYX1X2PD:
			m_pCamPlatformAxisPos = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformYDPX1X2:
			m_pCamPlatformAxisPos = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformDXY:
			m_pCamPlatformAxisPos = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformXDPY:
			m_pCamPlatformAxisPos = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		case ePlatformXDPY1:
			m_pCamPlatformAxisPos = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)lCInfo.m_pCamPlatformAxisPos));
			break;
		default:
			break;
		}
	}

	if (lCInfo.m_pCamAxisPlatformAxisPos != NULL)
	{
		PlatformType camPlatformType = lCInfo.m_pCamAxisPlatformAxisPos->GetPlatformType();
		switch (camPlatformType)
		{
		case ePlatformXYD:
			m_pCamAxisPlatformAxisPos = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformXYPD:
			m_pCamAxisPlatformAxisPos = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformXY:
			m_pCamAxisPlatformAxisPos = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformX:
			m_pCamAxisPlatformAxisPos = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformY:
			m_pCamAxisPlatformAxisPos = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformYX1X2PD:
			m_pCamAxisPlatformAxisPos = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformYDPX1X2:
			m_pCamAxisPlatformAxisPos = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformDXY:
			m_pCamAxisPlatformAxisPos = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformXDPY:
			m_pCamAxisPlatformAxisPos = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		case ePlatformXDPY1:
			m_pCamAxisPlatformAxisPos = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)lCInfo.m_pCamAxisPlatformAxisPos));
			break;
		default:
			break;
		}
	}

	for(i = 0; i < CALIBBOARDMARKPOSNUM; i++)
	{
		m_cpCalibBoardMarkImgCoordPos[i] = lCInfo.m_cpCalibBoardMarkImgCoordPos[i];
	}

	for(i = 0; i < CALIBBOARDMARKPOSNUM; i++)
	{
		m_cpCalibBoardMarkBoardCoordPos[i] = lCInfo.m_cpCalibBoardMarkBoardCoordPos[i];
	}

	m_vCalibBoardSize = lCInfo.m_vCalibBoardSize;

	return *this;
}

void CCalibratedInfo::SetValid(BOOL bValid)
{
	m_bValid = bValid;
}

void CCalibratedInfo::ToString(CString & Str)
{
	Str.Format(_T("……h0-h8:(%.6f,%.6f,%.6f;%.6f,%.6f,%.6f; %.6f,%.6f,%.6f\n"),m_PlatformTranferH[0],m_PlatformTranferH[1],m_PlatformTranferH[2],m_PlatformTranferH[3],m_PlatformTranferH[4],m_PlatformTranferH[5],m_PlatformTranferH[6],m_PlatformTranferH[7],m_PlatformTranferH[8]);
	Str.AppendFormat(_T("……标定基准平台坐标：（X：%.6f,Y：%.6f）\n"),m_cpMarkPlatformCoordPos.GetPosX(),m_cpMarkPlatformCoordPos.GetPosY());
}
BOOL CCalibratedInfo::IsValid()
{
	// 	BOOL bOK = FALSE;
	// 	for(int i = 0; i < 9; i++)
	// 	{
	// 		if (m_PlatformTranferH[i] != 0)
	// 		{
	// 			bOK = TRUE;
	// 		}
	// 	}
	// 	m_bValid = bOK;
	return m_bValid;
}

void CCalibratedInfo::SetPlatformTransferMatrix(sc2Matrix matrix)
{
	m_PlatformTranferMatrix = matrix;
}

sc2Matrix CCalibratedInfo::GetPlatformTransferMatrix() const
{
	return m_PlatformTranferMatrix;
}

void CCalibratedInfo::SetPlatformTransferH(const double h[9])
{
	int i = 0;
	for( i = 0; i < 9; i++)
	{
		m_PlatformTranferH[i] = h[i];
	}
}

void CCalibratedInfo::SetPlatformTransferH1(const double h[9])
{
	int i = 0;
	for( i = 0; i < 9; i++)
	{
		m_PlatformTranferH1[i] = h[i];
	}
}

void CCalibratedInfo::SetPlatformTransferH2(const double h[9])
{
	int i = 0;
	for( i = 0; i < 9; i++)
	{
		m_PlatformTranferH2[i] = h[i];
	}
}

bool CCalibratedInfo::GetPlatformTransferH(double h[9]) const
{
	if (h == NULL)
	{
		return false;
	}
	int i = 0;
	for( i = 0; i < 9; i++)
	{
		h[i] = m_PlatformTranferH[i];
	}

	return true;
}

bool CCalibratedInfo::GetPlatformTransferH1(double h[9]) const
{
	if (h == NULL)
	{
		return false;
	}
	int i = 0;
	for( i = 0; i < 9; i++)
	{
		h[i] = m_PlatformTranferH1[i];
	}

	return true;
}

bool CCalibratedInfo::GetPlatformTransferH2(double h[9]) const
{
	if (h == NULL)
	{
		return false;
	}
	int i = 0;
	for( i = 0; i < 9; i++)
	{
		h[i] = m_PlatformTranferH2[i];
	}

	return true;
}

void CCalibratedInfo::SetMarkPlatformCoordPos(CCoordPos pos)
{
	m_cpMarkPlatformCoordPos = pos;
}

CCoordPos CCalibratedInfo::GetMarkPlatformCoordPos() const
{
	return m_cpMarkPlatformCoordPos;
}

void CCalibratedInfo::SetMarkPlatformCoordPosRefineOffset(CCoordPos pos)
{
	m_cpRefineOffset = pos;
}

CCoordPos CCalibratedInfo::GetMarkPlatformCoordPosRefineOffset() const
{
	return m_cpRefineOffset;
}

void CCalibratedInfo::SetMarkPlatformCoordPosRefineOffsetH1(CCoordPos pos)	//全局二维码标定H1矩阵误差
{
	m_cpRefineOffsetH1 = pos;
}

CCoordPos CCalibratedInfo::GetMarkPlatformCoordPosRefineOffsetH1() const
{
	return m_cpRefineOffsetH1;
}

void CCalibratedInfo::SetMarkPlatformCoordPosRefineOffsetH2(CCoordPos pos)	//全局二维码标定H2矩阵误差
{
	m_cpRefineOffsetH2 = pos;
}

CCoordPos CCalibratedInfo::GetMarkPlatformCoordPosRefineOffsetH2() const
{
	return m_cpRefineOffsetH2;
}

void	CCalibratedInfo::SetMarkImgCoordPos(CCoordPos pos)
{
	m_cpMarkImgCoordPos = pos;
}

CCoordPos CCalibratedInfo::GetMarkImgCoordPos() const
{
	return m_cpMarkImgCoordPos;
}

void CCalibratedInfo::SetMarkPlatformCoordPosBat(CCoordPos pos)
{
	m_cpMarkPlatformCoordPosBat = pos;
}

CCoordPos CCalibratedInfo::GetMarkPlatformCoordPosBat() const
{
	return m_cpMarkPlatformCoordPosBat;
}

void CCalibratedInfo::SetMarkImgCoordPosBat(CCoordPos pos)
{
	m_cpMarkImgCoordPosBat = pos;
}

CCoordPos CCalibratedInfo::GetMarkImgCoordPosBat() const
{
	return m_cpMarkImgCoordPosBat;
}

CPlatformAxisPos* CCalibratedInfo::GetCamPlatformAxisPos()
{
	return m_pCamPlatformAxisPos;
}

CPlatformAxisPos* CCalibratedInfo::GetCamAxisPlatformAxisPos()
{
	return m_pCamAxisPlatformAxisPos;
}

void CCalibratedInfo::SetTargetMarkImgCoordPos(const CCoordPos pos[CALIBMOVEPOSNUM])
{
	int i = 0;
	for( i = 0; i < CALIBMOVEPOSNUM; i++)
	{
		m_cpTargetMarkImgCoordPos[i] = pos[i];
	}
}

bool CCalibratedInfo::GetTargetMarkImgCoordPos(CCoordPos pos[CALIBMOVEPOSNUM]) const
{
	if (pos == NULL)
	{
		return false;
	}

	int i = 0;
	for( i = 0; i < CALIBMOVEPOSNUM; i++)
	{
		pos[i] = m_cpTargetMarkImgCoordPos[i];
	}

	return true;
}

void CCalibratedInfo::SetObjectMarkImgCoordPos(const CCoordPos pos[CALIBMOVEPOSNUM])
{
	int i = 0;
	for( i = 0; i < CALIBMOVEPOSNUM; i++)
	{
		m_cpObjectMarkImgCoordPos[i] = pos[i];
	}
}

bool CCalibratedInfo::GetObjectMarkImgCoordPos(CCoordPos pos[CALIBMOVEPOSNUM]) const
{
	if (pos == NULL)
	{
		return false;
	}

	int i = 0;
	for( i = 0; i < CALIBMOVEPOSNUM; i++)
	{
		pos[i] = m_cpObjectMarkImgCoordPos[i];
	}

	return true;
}

void CCalibratedInfo::SetCalibBoardMarkImgCoordPos(const CCoordPos pos[CALIBBOARDMARKPOSNUM])
{
	int i = 0;
	for( i = 0; i < CALIBBOARDMARKPOSNUM; i++)
	{
		m_cpCalibBoardMarkImgCoordPos[i] = pos[i];
	}
}

bool CCalibratedInfo::GetCalibBoardMarkImgCoordPos(CCoordPos pos[CALIBBOARDMARKPOSNUM]) const
{
	if (pos == NULL)
	{
		return false;
	}

	int i = 0;
	for( i = 0; i < CALIBBOARDMARKPOSNUM; i++)
	{
		pos[i] = m_cpCalibBoardMarkImgCoordPos[i];
	}

	return true;
}

void CCalibratedInfo::SetCalibBoardMarkBoardCoordPos(const CCoordPos pos[CALIBBOARDMARKPOSNUM])
{
	int i = 0;
	for( i = 0; i < CALIBBOARDMARKPOSNUM; i++)
	{
		m_cpCalibBoardMarkBoardCoordPos[i] = pos[i];
	}
}

bool CCalibratedInfo::GetCalibBoardMarkBoardCoordPos(CCoordPos pos[CALIBBOARDMARKPOSNUM]) const
{
	if (pos == NULL)
	{
		return false;
	}

	int i = 0;
	for( i = 0; i < CALIBBOARDMARKPOSNUM; i++)
	{
		pos[i] = m_cpCalibBoardMarkBoardCoordPos[i];
	}

	return true;
}

void CCalibratedInfo::SetCalibBoardSize(sc2Vector vBoardSize)
{
	m_vCalibBoardSize = vBoardSize;
}

bool CCalibratedInfo::GetCalibBoardSize(sc2Vector& vBoardSize)
{
	vBoardSize = m_vCalibBoardSize;
	return true;
}

void CCalibratedInfo::SetPlatformAxisPos( const CPlatformAxisPos* pAxisPos )
{
	// 释放原有数据的内存并重新创建(new)
	if (m_pPlatformAxisPos != NULL)
	{
		delete m_pPlatformAxisPos;
		m_pPlatformAxisPos = NULL;
	}
	if (pAxisPos == NULL)
	{
		return;
	}
	//判断平台类型并根据类型动态分配内存
	PlatformType platformType = pAxisPos->GetPlatformType();

	switch (platformType)
	{
	case ePlatformXYD:
		m_pPlatformAxisPos = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)pAxisPos));
		break;
	case ePlatformXYPD:
		m_pPlatformAxisPos = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)pAxisPos));
		break;
	case ePlatformX1X2Y:
		m_pPlatformAxisPos = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)pAxisPos));
		break;
	case ePlatformXY1Y2:
		m_pPlatformAxisPos = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)pAxisPos));
		break;
	case ePlatformXY:
		m_pPlatformAxisPos = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
		break;
	case ePlatformX:
		m_pPlatformAxisPos = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos));
		break;
	case ePlatformY:
		m_pPlatformAxisPos = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos));
		break;
	case ePlatformYX1X2PD:
		m_pPlatformAxisPos = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)pAxisPos));
		break;
	case ePlatformYDPX1X2:
		m_pPlatformAxisPos = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)pAxisPos));
		break;
	case ePlatformDXY:
		m_pPlatformAxisPos = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)pAxisPos));
		break;
	case ePlatformXDPY:
		m_pPlatformAxisPos = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)pAxisPos));
		break;
	case ePlatformXDPY1:
		m_pPlatformAxisPos = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)pAxisPos));
		break;
	default:
		break;
	}
}

CPlatformAxisPos* CCalibratedInfo::GetPlatformAxisPos() const 
{
	return m_pPlatformAxisPos;
}

void CCalibratedInfo::SetCamPlatformAxisPos( CPlatformAxisPos* pAxisPos )
{
	// 释放原有数据的内存并重新创建(new)
	if (m_pCamPlatformAxisPos != NULL)
	{
		delete m_pCamPlatformAxisPos;
		m_pCamPlatformAxisPos = NULL;
	}
	if (pAxisPos == NULL)
	{
		return;
	}

	//判断平台类型并根据类型动态分配内存
	PlatformType platformType = pAxisPos->GetPlatformType();

	switch (platformType)
	{
	case ePlatformXYD:
		m_pCamPlatformAxisPos = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)pAxisPos));
		break;
	case ePlatformXYPD:
		m_pCamPlatformAxisPos = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)pAxisPos));
		break;
	case ePlatformXY:
		m_pCamPlatformAxisPos = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
		break;
	case ePlatformX:
		m_pCamPlatformAxisPos = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos));
		break;
	case ePlatformY:
		m_pCamPlatformAxisPos = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos));
		break;
	case ePlatformYX1X2PD:
		m_pCamPlatformAxisPos = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)pAxisPos));
		break;
	case ePlatformYDPX1X2:
		m_pCamPlatformAxisPos = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)pAxisPos));
		break;
	case ePlatformDXY:
		m_pCamPlatformAxisPos = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)pAxisPos));
		break;
	case ePlatformXDPY:
		m_pCamPlatformAxisPos = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)pAxisPos));
		break;
	case ePlatformXDPY1:
		m_pCamPlatformAxisPos = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)pAxisPos));
		break;
	default:
		break;
	}
}

void CCalibratedInfo::SetCamAxisPlatformAxisPos(CPlatformAxisPos* pAxisPos)
{
	// 释放原有数据的内存并重新创建(new)
	if (m_pCamAxisPlatformAxisPos != NULL)
	{
		delete m_pCamAxisPlatformAxisPos;
		m_pCamAxisPlatformAxisPos = NULL;
	}
	if (pAxisPos == NULL)
	{
		return;
	}

	//判断平台类型并根据类型动态分配内存
	PlatformType platformType = pAxisPos->GetPlatformType();

	switch (platformType)
	{
	case ePlatformXYD:
		m_pCamAxisPlatformAxisPos = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)pAxisPos));
		break;
	case ePlatformXYPD:
		m_pCamAxisPlatformAxisPos = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)pAxisPos));
		break;
	case ePlatformXY:
		m_pCamAxisPlatformAxisPos = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
		break;
	case ePlatformX:
		m_pCamAxisPlatformAxisPos = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos));
		break;
	case ePlatformY:
		m_pCamAxisPlatformAxisPos = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos));
		break;
	case ePlatformYX1X2PD:
		m_pCamAxisPlatformAxisPos = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)pAxisPos));
		break;
	case ePlatformYDPX1X2:
		m_pCamAxisPlatformAxisPos = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)pAxisPos));
		break;
	case ePlatformDXY:
		m_pCamAxisPlatformAxisPos = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)pAxisPos));
		break;
	case ePlatformXDPY:
		m_pCamAxisPlatformAxisPos = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)pAxisPos));
		break;
	case ePlatformXDPY1:
		m_pCamAxisPlatformAxisPos = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)pAxisPos));
		break;
	default:
		break;
	}
}

void CCalibratedInfo::SetCamPlatformTransferMatrix(sc2Matrix matrix)
{
	m_CameraTranferMatrix = matrix;
}

sc2Matrix CCalibratedInfo::GetCamPlatformTransferMatrix() const
{
	return m_CameraTranferMatrix;
}

BOOL CCalibratedInfo::operator==( const CCalibratedInfo& calibInfo )
{
	if (m_bValid != calibInfo.m_bValid) return FALSE;
	if (m_PlatformTranferMatrix != calibInfo.m_PlatformTranferMatrix) return FALSE;
	if (m_CameraTranferMatrix != calibInfo.m_CameraTranferMatrix) return FALSE;
	if (m_cpMarkImgCoordPos != calibInfo.m_cpMarkImgCoordPos) return FALSE;
	if (m_cpMarkPlatformCoordPos != calibInfo.m_cpMarkPlatformCoordPos) return FALSE;
	if (m_dImageToPlatformAnlge != calibInfo.m_dImageToPlatformAnlge) return FALSE;
	if (m_cpRefineOffset != calibInfo.m_cpRefineOffset) return FALSE;
	if (m_cpRefineOffsetH1 != calibInfo.m_cpRefineOffsetH1) return FALSE;
	if (m_cpRefineOffsetH2 != calibInfo.m_cpRefineOffsetH2) return FALSE;
	int i = 0;
	for(i = 0; i < 9; i++)
	{
		if (m_PlatformTranferH[i] != calibInfo.m_PlatformTranferH[i]
		|| m_PlatformTranferH1[i] != calibInfo.m_PlatformTranferH1[i]
		|| m_PlatformTranferH2[i] != calibInfo.m_PlatformTranferH2[i])
		{
			return FALSE;
		}
	}
	// 比较基准位置处平台各个轴的位置
	if (m_pPlatformAxisPos->GetPlatformType() != calibInfo.m_pPlatformAxisPos->GetPlatformType())
	{
		return FALSE;
	}
	else
	{
		BOOL bEqual = TRUE;
		switch (m_pPlatformAxisPos->GetPlatformType())
		{
		case ePlatformXYD:
			bEqual = *((CPlatformXYDAxisPos*)m_pPlatformAxisPos) == *((CPlatformXYDAxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformXYPD:
			bEqual = *((CPlatformXYPDAxisPos*)m_pPlatformAxisPos) == *((CPlatformXYPDAxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformX1X2Y:
			bEqual = *((CPlatformX1X2YAxisPos*)m_pPlatformAxisPos) == *((CPlatformX1X2YAxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformXY1Y2:
			bEqual = *((CPlatformXY1Y2AxisPos*)m_pPlatformAxisPos) == *((CPlatformXY1Y2AxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformXY:
			bEqual = *((CPlatformXYAxisPos*)m_pPlatformAxisPos) == *((CPlatformXYAxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformX:
			bEqual = *((CPlatformXAxisPos*)m_pPlatformAxisPos) == *((CPlatformXAxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformY:
			bEqual = *((CPlatformYAxisPos*)m_pPlatformAxisPos) == *((CPlatformYAxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformYX1X2PD:
			bEqual = *((CPlatformYX1X2PDAxisPos*)m_pPlatformAxisPos) == *((CPlatformYX1X2PDAxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformYDPX1X2:
			bEqual = *((CPlatformYDPX1X2AxisPos*)m_pPlatformAxisPos) == *((CPlatformYDPX1X2AxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformDXY:
			bEqual = *((CPlatformDXYAxisPos*)m_pPlatformAxisPos) == *((CPlatformDXYAxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformXDPY:
			bEqual = *((CPlatformXDPYAxisPos*)m_pPlatformAxisPos) == *((CPlatformXDPYAxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformXDPY1:
			bEqual = *((CPlatformXDPY1AxisPos*)m_pPlatformAxisPos) == *((CPlatformXDPY1AxisPos*)calibInfo.m_pPlatformAxisPos);
			break;
		case ePlatformUnknown:
			break;
		default:
			bEqual = FALSE;
			break;
		}
		if (!bEqual) return FALSE;
	}

	// 基准位置处相机平台各个轴的位置
	if (m_pCamPlatformAxisPos->GetPlatformType() != calibInfo.m_pCamPlatformAxisPos->GetPlatformType())
	{
		return FALSE;
	}
	else
	{
		BOOL bEqual = TRUE;
		switch (m_pCamPlatformAxisPos->GetPlatformType())
		{
		case ePlatformXYD:
			bEqual = *((CPlatformXYDAxisPos*)m_pCamPlatformAxisPos) == *((CPlatformXYDAxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformXYPD:
			bEqual = *((CPlatformXYPDAxisPos*)m_pCamPlatformAxisPos) == *((CPlatformXYPDAxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformX1X2Y:
			bEqual = *((CPlatformX1X2YAxisPos*)m_pCamPlatformAxisPos) == *((CPlatformX1X2YAxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformXY1Y2:
			bEqual = *((CPlatformXY1Y2AxisPos*)m_pCamPlatformAxisPos) == *((CPlatformXY1Y2AxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformXY:
			bEqual = *((CPlatformXYAxisPos*)m_pCamPlatformAxisPos) == *((CPlatformXYAxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformX:
			bEqual = *((CPlatformXAxisPos*)m_pCamPlatformAxisPos) == *((CPlatformXAxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformY:
			bEqual = *((CPlatformYAxisPos*)m_pCamPlatformAxisPos) == *((CPlatformYAxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformYX1X2PD:
			bEqual = *((CPlatformYX1X2PDAxisPos*)m_pCamPlatformAxisPos) == *((CPlatformYX1X2PDAxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformYDPX1X2:
			bEqual = *((CPlatformYDPX1X2AxisPos*)m_pCamPlatformAxisPos) == *((CPlatformYDPX1X2AxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformDXY:
			bEqual = *((CPlatformDXYAxisPos*)m_pCamPlatformAxisPos) == *((CPlatformDXYAxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformXDPY:
			bEqual = *((CPlatformXDPYAxisPos*)m_pCamPlatformAxisPos) == *((CPlatformXDPYAxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformXDPY1:
			bEqual = *((CPlatformXDPY1AxisPos*)m_pCamPlatformAxisPos) == *((CPlatformXDPY1AxisPos*)calibInfo.m_pCamPlatformAxisPos);
			break;
		case ePlatformUnknown:
			break;
		default:
			bEqual = FALSE;
			break;
		}
		if (!bEqual) return FALSE;
	}


	if (m_pCamAxisPlatformAxisPos->GetPlatformType() != calibInfo.m_pCamAxisPlatformAxisPos->GetPlatformType())
	{
		return FALSE;
	}
	else
	{
		BOOL bEqual = TRUE;
		switch (m_pCamAxisPlatformAxisPos->GetPlatformType())
		{
		case ePlatformXYD:
			bEqual = *((CPlatformXYDAxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformXYDAxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformXYPD:
			bEqual = *((CPlatformXYPDAxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformXYPDAxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformX1X2Y:
			bEqual = *((CPlatformX1X2YAxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformX1X2YAxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformXY1Y2:
			bEqual = *((CPlatformXY1Y2AxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformXY1Y2AxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformXY:
			bEqual = *((CPlatformXYAxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformXYAxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformX:
			bEqual = *((CPlatformXAxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformXAxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformY:
			bEqual = *((CPlatformYAxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformYAxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformYX1X2PD:
			bEqual = *((CPlatformYX1X2PDAxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformYX1X2PDAxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformYDPX1X2:
			bEqual = *((CPlatformYDPX1X2AxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformYDPX1X2AxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformDXY:
			bEqual = *((CPlatformDXYAxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformDXYAxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformXDPY:
			bEqual = *((CPlatformXDPYAxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformXDPYAxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformXDPY1:
			bEqual = *((CPlatformXDPY1AxisPos*)m_pCamAxisPlatformAxisPos) == *((CPlatformXDPY1AxisPos*)calibInfo.m_pCamAxisPlatformAxisPos);
			break;
		case ePlatformUnknown:
			break;
		default:
			bEqual = FALSE;
			break;
		}
		if (!bEqual) return FALSE;
	}
	return TRUE;	
}

BOOL CCalibratedInfo::operator!=( const CCalibratedInfo& calibInfo )
{
	return !(*this == calibInfo);

	//	if (m_bValid != calibInfo.m_bValid) return TRUE;
	// 	if (m_PlatformTranferMatrix != calibInfo.m_PlatformTranferMatrix) return TRUE;
	// 	if (m_CameraTranferMatrix != calibInfo.m_CameraTranferMatrix) return TRUE;
	// 	if (m_cpMarkImgCoordPos != calibInfo.m_cpMarkImgCoordPos) return TRUE;
	// 	if (m_cpMarkPlatformCoordPos != calibInfo.m_cpMarkPlatformCoordPos) return TRUE;
	// 	if (m_dImageToPlatformAnlge != calibInfo.m_dImageToPlatformAnlge) return TRUE;
	// 	// 比较基准位置处平台各个轴的位置
	// 	if (m_pPlatformAxisPos->GetPlatformType() != calibInfo.m_pPlatformAxisPos->GetPlatformType())
	// 	{
	// 		return TRUE;
	// 	}
	// 	else
	// 	{
	// 		BOOL bNotEqual = TRUE;
	// 		switch (m_pPlatformAxisPos->GetPlatformType())
	// 		{
	// 		case ePlatformXYD:
	// 			bNotEqual = *((CPlatformXYDAxisPos*)m_pPlatformAxisPos) != *((CPlatformXYDAxisPos*)calibInfo.m_pPlatformAxisPos);
	// 			break;
	// 		case ePlatformX1X2Y:
	// 			bNotEqual = *((CPlatformX1X2YAxisPos*)m_pPlatformAxisPos) != *((CPlatformX1X2YAxisPos*)calibInfo.m_pPlatformAxisPos);
	// 			break;
	// 		case ePlatformXY1Y2:
	// 			bNotEqual = *((CPlatformXY1Y2AxisPos*)m_pPlatformAxisPos) != *((CPlatformXY1Y2AxisPos*)calibInfo.m_pPlatformAxisPos);
	// 			break;
	// 		case ePlatformXY:
	// 			bNotEqual = *((CPlatformXYAxisPos*)m_pPlatformAxisPos) != *((CPlatformXYAxisPos*)calibInfo.m_pPlatformAxisPos);
	// 			break;
	// 		case ePlatformYX1X2PD:
	// 			bNotEqual = *((CPlatformYX1X2PDAxisPos*)m_pPlatformAxisPos) != *((CPlatformYX1X2PDAxisPos*)calibInfo.m_pPlatformAxisPos);
	// 			break;
	// 		case ePlatformYDPX1X2:
	// 			bNotEqual = *((CPlatformYDPX1X2AxisPos*)m_pPlatformAxisPos) != *((CPlatformYDPX1X2AxisPos*)calibInfo.m_pPlatformAxisPos);
	// 			break;
	// 		case ePlatformDXY:
	// 			bNotEqual = *((CPlatformDXYAxisPos*)m_pPlatformAxisPos) != *((CPlatformDXYAxisPos*)calibInfo.m_pPlatformAxisPos);
	// 			break;
	// 		case ePlatformUnknown:
	// 			break;
	// 		default:
	// 			bNotEqual = TRUE;
	// 			break;
	// 		}
	// 		if (bNotEqual) return TRUE;
	// 	}
	// 	
	// 	// 基准位置处相机平台各个轴的位置
	// 	if (m_pCamPlatformAxisPos->GetPlatformType() != calibInfo.m_pCamPlatformAxisPos->GetPlatformType())
	// 	{
	// 		return TRUE;
	// 	}
	// 	else
	// 	{
	// 		BOOL bNotEqual = TRUE;
	// 		switch (m_pCamPlatformAxisPos->GetPlatformType())
	// 		{
	// 		case ePlatformXYD:
	// 			bNotEqual = *((CPlatformXYDAxisPos*)m_pCamPlatformAxisPos) != *((CPlatformXYDAxisPos*)calibInfo.m_pCamPlatformAxisPos);
	// 			break;
	// 		case ePlatformX1X2Y:
	// 			bNotEqual = *((CPlatformX1X2YAxisPos*)m_pCamPlatformAxisPos) != *((CPlatformX1X2YAxisPos*)calibInfo.m_pCamPlatformAxisPos);
	// 			break;
	// 		case ePlatformXY1Y2:
	// 			bNotEqual = *((CPlatformXY1Y2AxisPos*)m_pCamPlatformAxisPos) != *((CPlatformXY1Y2AxisPos*)calibInfo.m_pCamPlatformAxisPos);
	// 			break;
	// 		case ePlatformXY:
	// 			bNotEqual = *((CPlatformXYAxisPos*)m_pCamPlatformAxisPos) != *((CPlatformXYAxisPos*)calibInfo.m_pCamPlatformAxisPos);
	// 			break;
	// 		case ePlatformYX1X2PD:
	// 			bNotEqual = *((CPlatformYX1X2PDAxisPos*)m_pCamPlatformAxisPos) != *((CPlatformYX1X2PDAxisPos*)calibInfo.m_pCamPlatformAxisPos);
	// 			break;
	// 		case ePlatformYDPX1X2:
	// 			bNotEqual = *((CPlatformYDPX1X2AxisPos*)m_pCamPlatformAxisPos) != *((CPlatformYDPX1X2AxisPos*)calibInfo.m_pCamPlatformAxisPos);
	// 			break;
	// 		case ePlatformDXY:
	// 			bNotEqual = *((CPlatformDXYAxisPos*)m_pCamPlatformAxisPos) != *((CPlatformDXYAxisPos*)calibInfo.m_pCamPlatformAxisPos);
	// 			break;
	// 		case ePlatformUnknown:
	// 			break;
	// 		default:
	// 			bNotEqual = TRUE;
	// 			break;
	// 		}
	// 		if (bNotEqual) return TRUE;
	// 	}
	// 	
	// 	return FALSE;	
}


/************************************************************************/
/*						 CPCBInfo implementations	                   */
/************************************************************************/

// CPCBInfo::CPCBInfo()
// {
// 	m_PCBInsPre = CCoordPos(0.010000, 0.010000, 0.010000);
// 	m_nPCBInsPreType = 0;
// 	m_bPCBShowInspectResult = FALSE;
// 	m_bShowPCB4To3Align = FALSE;
// }


/************************************************************************/
/*						 CWeldInfo implementations	                   */
/************************************************************************/
CWeldInfo::CWeldInfo()
{
	// 专用焊接参数
	m_bEnableWeld	  = FALSE;

	m_WeldMovementRatio			= CCoordPos(1.0, 1.0, 1.0);
	m_WeldAmmendMovementRatio	= CCoordPos(1.0, 1.0, 0.0);

	m_nWeldTargetSearchTimes	= 0;
	m_nWeldSearchBench			= 0;
	m_nWeldAngleBench			= 0;

}

/************************************************************************/
/*            CBenchTargetMarkFixInfo implementations                   */
/************************************************************************/
CBenchTargetMarkFixInfo::CBenchTargetMarkFixInfo()
{
	m_bIsValid = FALSE;
	m_nBenchTargetMarkNum = 0;
	//	m_pBenchFixPlatformPos = NULL;
}

CBenchTargetMarkFixInfo::CBenchTargetMarkFixInfo( const CBenchTargetMarkFixInfo& lBTMFInfo )
{
	m_nBenchTargetMarkNum = lBTMFInfo.m_nBenchTargetMarkNum;
	m_vBenchTargetMarkImageCoordPoses = lBTMFInfo.m_vBenchTargetMarkImageCoordPoses;
	m_vBenchTargetMarkPlatformCoordPoses = lBTMFInfo.m_vBenchTargetMarkPlatformCoordPoses;
	// 复制运动平台信息
	int nplatCnt = lBTMFInfo.m_pBenchFixPlatformPos.size();
	m_pBenchFixPlatformPos.resize(nplatCnt, NULL);

	for (int k=0;k<nplatCnt;k++)
	{
		if (lBTMFInfo.m_pBenchFixPlatformPos.at(k) != NULL)
		{
			PlatformType platformType = lBTMFInfo.m_pBenchFixPlatformPos.at(k)->GetPlatformType();
			switch (platformType)
			{
			case ePlatformXYD:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXYPD:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformX1X2Y:
				m_pBenchFixPlatformPos.at(k) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXY1Y2:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXY:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformX:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformY:
				m_pBenchFixPlatformPos.at(k) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformYX1X2PD:
				m_pBenchFixPlatformPos.at(k) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformYDPX1X2:
				m_pBenchFixPlatformPos.at(k) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformDXY:
				m_pBenchFixPlatformPos.at(k) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXDPY:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXDPY1:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			default:
				break;
			}
		}
	}


	// 复制相机运动平台信息
	int nCnt = lBTMFInfo.m_vpBenchFixCamPlatformPos.size();
	m_vpBenchFixCamPlatformPos.resize(nCnt, NULL);
	for (int i=0; i<nCnt; i++)
	{
		if (lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i) != NULL)
		{
			PlatformType platformType = lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)->GetPlatformType();
			switch (platformType)
			{
			case ePlatformXYD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXYPD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformX1X2Y:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXY1Y2:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformX:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformYX1X2PD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformYDPX1X2:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformDXY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXDPY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXDPY1:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			default:
				break;
			}
		}
	}
	m_bIsValid = lBTMFInfo.m_bIsValid;
}

CBenchTargetMarkFixInfo::CBenchTargetMarkFixInfo( int nMarkNum, PlatformType ePlatformType )
{
	m_nBenchTargetMarkNum = nMarkNum;
	m_vBenchTargetMarkImageCoordPoses.resize(nMarkNum);
	m_vBenchTargetMarkPlatformCoordPoses.resize(nMarkNum);

	m_pBenchFixPlatformPos.resize(nMarkNum,NULL);

	for (int k=0;k<nMarkNum;k++)
	{
		switch (ePlatformType)
		{
		case ePlatformXYD:
			m_pBenchFixPlatformPos.at(k) = new CPlatformXYDAxisPos();
			break;
		case ePlatformXYPD:
			m_pBenchFixPlatformPos.at(k) = new CPlatformXYPDAxisPos();
			break;
		case ePlatformX1X2Y:
			m_pBenchFixPlatformPos.at(k) = new CPlatformX1X2YAxisPos();
			break;
		case ePlatformXY1Y2:
			m_pBenchFixPlatformPos.at(k) = new CPlatformXY1Y2AxisPos();
			break;
		case ePlatformXY:
			m_pBenchFixPlatformPos.at(k) = new CPlatformXYAxisPos();
			break;
		case ePlatformX:
			m_pBenchFixPlatformPos.at(k) = new CPlatformXAxisPos();
			break;
		case ePlatformY:
			m_pBenchFixPlatformPos.at(k) = new CPlatformYAxisPos();
			break;
		case ePlatformYX1X2PD:
			m_pBenchFixPlatformPos.at(k) = new CPlatformYX1X2PDAxisPos();
			break;
		case ePlatformYDPX1X2:
			m_pBenchFixPlatformPos.at(k) = new CPlatformYDPX1X2AxisPos();
			break;
		case ePlatformDXY:
			m_pBenchFixPlatformPos.at(k) = new CPlatformDXYAxisPos();
			break;
		case ePlatformXDPY:
			m_pBenchFixPlatformPos.at(k) = new CPlatformXDPYAxisPos();
			break;
		case ePlatformXDPY1:
			m_pBenchFixPlatformPos.at(k) = new CPlatformXDPY1AxisPos();
			break;
		default:
			break;
		}


	}

	m_vpBenchFixCamPlatformPos.resize(nMarkNum, NULL);
	for (int i=0; i<nMarkNum; i++)
	{
		m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYAxisPos;
	}

	m_bIsValid = FALSE;
}

CBenchTargetMarkFixInfo::~CBenchTargetMarkFixInfo()
{
	m_vBenchTargetMarkImageCoordPoses.clear();
	m_vBenchTargetMarkPlatformCoordPoses.clear();

	for (int k=0;k<m_pBenchFixPlatformPos.size();k++)
	{
		if (m_pBenchFixPlatformPos.at(k) != NULL)
		{
			delete m_pBenchFixPlatformPos.at(k);
			m_pBenchFixPlatformPos.at(k) = NULL;
		}
	}

	for (int i=0;i<m_vpBenchFixCamPlatformPos.size();i++)
	{
		if (m_vpBenchFixCamPlatformPos.at(i) != NULL)
		{
			delete m_vpBenchFixCamPlatformPos.at(i);
			m_vpBenchFixCamPlatformPos.at(i) = NULL;
		}
	}
	m_vpBenchFixCamPlatformPos.clear();
	m_bIsValid = FALSE;
}

CBenchTargetMarkFixInfo& CBenchTargetMarkFixInfo::operator=( const CBenchTargetMarkFixInfo& lBTMFInfo )
{
	if (this == &lBTMFInfo)
	{
		return *this;
	}

	m_nBenchTargetMarkNum = lBTMFInfo.m_nBenchTargetMarkNum;
	m_vBenchTargetMarkImageCoordPoses = lBTMFInfo.m_vBenchTargetMarkImageCoordPoses;
	m_vBenchTargetMarkPlatformCoordPoses = lBTMFInfo.m_vBenchTargetMarkPlatformCoordPoses;

	int nCntplatOld = m_pBenchFixPlatformPos.size();
	int k=0;
	for (k=0;k<nCntplatOld;k++)
	{
		if (m_pBenchFixPlatformPos.at(k) != NULL)
		{
			delete m_pBenchFixPlatformPos.at(k);
			m_pBenchFixPlatformPos.at(k) = NULL;
		}

	}

	int nCntplatNew=lBTMFInfo.m_pBenchFixPlatformPos.size();
	m_pBenchFixPlatformPos.resize(nCntplatNew);
	for (k=0;k<nCntplatNew;k++)
	{
		if (lBTMFInfo.m_pBenchFixPlatformPos.at(k) != NULL)
		{
			PlatformType platformType = lBTMFInfo.m_pBenchFixPlatformPos.at(k)->GetPlatformType();
			switch (platformType)
			{
			case ePlatformXYD:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXYPD:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformX1X2Y:
				m_pBenchFixPlatformPos.at(k) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXY1Y2:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXY:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformX:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformY:
				m_pBenchFixPlatformPos.at(k) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformYX1X2PD:
				m_pBenchFixPlatformPos.at(k) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformYDPX1X2:
				m_pBenchFixPlatformPos.at(k) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformDXY:
				m_pBenchFixPlatformPos.at(k) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXDPY:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			case ePlatformXDPY1:
				m_pBenchFixPlatformPos.at(k) = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos.at(k)));
				break;
			default:
				break;
			}
		}
	}


	// 复制相机运动平台信息
	int nCntOld = m_vpBenchFixCamPlatformPos.size();
	int i = 0;
	for (i=0; i<nCntOld; i++)
	{
		if (m_vpBenchFixCamPlatformPos.at(i) != NULL)
		{
			delete m_vpBenchFixCamPlatformPos.at(i);
			m_vpBenchFixCamPlatformPos.at(i) = NULL;
		}
	}
	int nCntNew = lBTMFInfo.m_vpBenchFixCamPlatformPos.size();
	m_vpBenchFixCamPlatformPos.resize(nCntNew, NULL);
	for (i=0; i<nCntNew; i++)
	{
		if (lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i) != NULL)
		{
			PlatformType platformType = lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)->GetPlatformType();
			switch (platformType)
			{
			case ePlatformXYD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXYPD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformX1X2Y:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXY1Y2:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformX:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformYX1X2PD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformYDPX1X2:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformDXY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXDPY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXDPY1:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			default:
				break;
			}
		}
	}
	m_bIsValid = lBTMFInfo.m_bIsValid;

	return *this;
}

void CBenchTargetMarkFixInfo::SetMarkNum( int nNum )
{
	if (nNum < 0)
	{
		return;
	}
	m_nBenchTargetMarkNum = nNum;
	m_vBenchTargetMarkImageCoordPoses.resize(nNum);
	m_vBenchTargetMarkPlatformCoordPoses.resize(nNum);

	if (m_vpBenchFixCamPlatformPos.size() > nNum)
	{
		for (int i=nNum; i<m_vpBenchFixCamPlatformPos.size(); i++)
		{
			delete m_vpBenchFixCamPlatformPos.at(i);
			m_vpBenchFixCamPlatformPos.at(i) = NULL;
		}
	}

	m_vpBenchFixCamPlatformPos.resize(nNum, NULL);
	if (m_vpBenchFixCamPlatformPos.size() < nNum)
	{
		for (int i=nNum; i<m_vpBenchFixCamPlatformPos.size(); i++)
		{
			m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYAxisPos;
		}
	}
}

int CBenchTargetMarkFixInfo::GetMarkNum()
{
	return m_nBenchTargetMarkNum;
}


BOOL CBenchTargetMarkFixInfo::SetMarkImageCoordPos( int nIndex, CCoordPos CoorPos )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum))
	{
		return FALSE;
	}
	else
	{
		m_vBenchTargetMarkImageCoordPoses.at(nIndex) = CoorPos;
		return TRUE;
	}
}

CCoordPos CBenchTargetMarkFixInfo::GetMarkImageCoordPos( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum))
	{
		CCoordPos pos;
		return pos;
	}
	else
	{
		return m_vBenchTargetMarkImageCoordPoses.at(nIndex);
	}
}

BOOL CBenchTargetMarkFixInfo::SetMarkPlatformCoordPos( int nIndex, CCoordPos CoorPos )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum))
	{
		return FALSE;
	}
	else
	{
		m_vBenchTargetMarkPlatformCoordPoses.at(nIndex) = CoorPos;
		return TRUE;
	}
}

//计算基准修正的平台坐标
//
CCoordPos CBenchTargetMarkFixInfo::GetMarkPlatformCoordPos( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum))
	{
		CCoordPos pos;
		return pos;
	}
	else
	{
		return m_vBenchTargetMarkPlatformCoordPoses.at(nIndex);
	}
}

BOOL CBenchTargetMarkFixInfo::SetFixPlatformPos(std::vector< CPlatformAxisPos*> pAxisPos )
{
	// 释放原有数据的内存并重新创建(new)
	int i=0;
	for (i=0;i<pAxisPos.size();i++)
	{
		if (pAxisPos.at(i) == NULL)
		{
			return FALSE;
		}
	}


	int nCnt=pAxisPos.size();
	for (i=0;i<m_pBenchFixPlatformPos.size();i++)
	{
		if (m_pBenchFixPlatformPos.at(i) != NULL)
		{
			delete m_pBenchFixPlatformPos.at(i);
			m_pBenchFixPlatformPos.at(i) = NULL;
		}

	}

	m_pBenchFixPlatformPos.resize(nCnt);

	for (i=0;i<nCnt;i++)
	{
		//判断平台类型并根据类型动态分配内存
		PlatformType platformType = pAxisPos.at(i)->GetPlatformType();

		switch (platformType)
		{
		case ePlatformXYD:
			m_pBenchFixPlatformPos.at(i) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformXYPD:
			m_pBenchFixPlatformPos.at(i) = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformX1X2Y:
			m_pBenchFixPlatformPos.at(i) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformXY1Y2:
			m_pBenchFixPlatformPos.at(i) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformXY:
			m_pBenchFixPlatformPos.at(i) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformX:
			m_pBenchFixPlatformPos.at(i) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformY:
			m_pBenchFixPlatformPos.at(i) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformYX1X2PD:
			m_pBenchFixPlatformPos.at(i) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformYDPX1X2:
			m_pBenchFixPlatformPos.at(i) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformDXY:
			m_pBenchFixPlatformPos.at(i) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformXDPY:
			m_pBenchFixPlatformPos.at(i) = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformXDPY1:
			m_pBenchFixPlatformPos.at(i) = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)pAxisPos.at(i)));
			break;
		default:
			break;
		}
	}

	return TRUE;	
}

BOOL CBenchTargetMarkFixInfo::SetFixPlatformPos( int nIndex, CPlatformAxisPos* pAxisPos )
{
	//	ASSERT((nIndex >= 0) && (nIndex < m_pBenchFixPlatformPos.size()));
	if ((nIndex < 0))
	{
		return FALSE;
	}
	if (nIndex+1>m_pBenchFixPlatformPos.size())
	{
		m_pBenchFixPlatformPos.resize(nIndex+1);
		m_pBenchFixPlatformPos.at(nIndex)=NULL;
	}
	// 释放原有数据的内存并重新创建(new)
	if (pAxisPos == NULL)
	{
		return FALSE;
	}

	if (m_pBenchFixPlatformPos.at(nIndex) != NULL)
	{
		delete m_pBenchFixPlatformPos.at(nIndex);
		m_pBenchFixPlatformPos.at(nIndex) = NULL;
	}	

	//判断平台类型并根据类型动态分配内存
	PlatformType platformType = pAxisPos->GetPlatformType();

	switch (platformType)
	{
	case ePlatformXYD:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)pAxisPos));
		break;
	case ePlatformXYPD:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformXYPDAxisPos(*((CPlatformXYPDAxisPos*)pAxisPos));
		break;
	case ePlatformX1X2Y:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)pAxisPos));
		break;
	case ePlatformXY1Y2:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)pAxisPos));
		break;
	case ePlatformXY:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
		break;
	case ePlatformX:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos));
		break;
	case ePlatformY:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos));
		break;
	case ePlatformYX1X2PD:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)pAxisPos));
		break;
	case ePlatformYDPX1X2:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)pAxisPos));
		break;
	case ePlatformDXY:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)pAxisPos));
		break;
	case ePlatformXDPY:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformXDPYAxisPos(*((CPlatformXDPYAxisPos*)pAxisPos));
		break;
	case ePlatformXDPY1:
		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformXDPY1AxisPos(*((CPlatformXDPY1AxisPos*)pAxisPos));
		break;
	default:
		break;
	}

	return TRUE;
	// 	if (ePlatformXY == platformType)
	// 	{
	// 		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
	// 		return TRUE;
	// 	}
	// 	else
	// 		return FALSE;
}



std::vector<CPlatformAxisPos*> CBenchTargetMarkFixInfo::GetFixPlatformPos()
{
	return m_pBenchFixPlatformPos;
}
CPlatformAxisPos* CBenchTargetMarkFixInfo::GetFixPlatformPos(int nIndex)
{
	if (nIndex<0 || nIndex>=m_pBenchFixPlatformPos.size())
	{
		return NULL;
	}
	return m_pBenchFixPlatformPos.at(nIndex);
}
//设置基准mark修正时相机各轴的位置，单个相机设置
BOOL CBenchTargetMarkFixInfo::SetFixCamPlatformPos( int nIndex, CPlatformAxisPos* pAxisPos )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum))
	{
		return FALSE;
	}
	// 释放原有数据的内存并重新创建(new)
	if (pAxisPos == NULL)
	{
		return FALSE;
	}

	if (m_vpBenchFixCamPlatformPos.at(nIndex) != NULL)
	{
		delete m_vpBenchFixCamPlatformPos.at(nIndex);
		m_vpBenchFixCamPlatformPos.at(nIndex) = NULL;
	}	

	//判断平台类型并根据类型动态分配内存
	PlatformType platformType = pAxisPos->GetPlatformType();

	if (ePlatformXY == platformType)
	{
		m_vpBenchFixCamPlatformPos.at(nIndex) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
		return TRUE;
	}
	else if (ePlatformX == platformType)
	{
		m_vpBenchFixCamPlatformPos.at(nIndex) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos));
		return TRUE;
	}
	else if (ePlatformY == platformType)
	{
		m_vpBenchFixCamPlatformPos.at(nIndex) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//设置基准mark修正时相机各轴的位置，单个相机
CPlatformAxisPos* CBenchTargetMarkFixInfo::GetFixCamPlatformPos(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum))
	{
		return NULL;
	}
	else
		return m_vpBenchFixCamPlatformPos.at(nIndex);
}

/************************************************************************/
/*            CBenchTargetMarkPickInfo implementations                   */
/************************************************************************/
CBenchTargetMarkPickInfo::CBenchTargetMarkPickInfo()
{
	m_bIsValid = FALSE;
	m_nBenchTargetMarkNum = 0;
	//	m_pBenchFixPlatformPos = NULL;
}

CBenchTargetMarkPickInfo::CBenchTargetMarkPickInfo( const CBenchTargetMarkPickInfo& lBTMFInfo )
{
	m_nBenchTargetMarkNum = lBTMFInfo.m_nBenchTargetMarkNum;
	for (int i=0;i<BENCH_FIX_POSNUM;i++)
	{
		m_vBenchTargetMarkImageCoordPoses[i] = lBTMFInfo.m_vBenchTargetMarkImageCoordPoses[i];
		m_vBenchTargetMarkPlatformCoordPoses[i] = lBTMFInfo.m_vBenchTargetMarkPlatformCoordPoses[i];
		m_vBenchPickPlatformOffset[i] = lBTMFInfo.m_vBenchPickPlatformOffset[i];
	}

	int nplatCnt =0;
	// 复制运动平台信息
	for (int i=0;i<BENCH_FIX_POSNUM;i++)
	{
		nplatCnt = lBTMFInfo.m_pBenchFixPlatformPos[i].size();
		m_pBenchFixPlatformPos[i].resize(nplatCnt, NULL);
		int k=0;
		for (k=0;k<nplatCnt;k++)
		{
			if (lBTMFInfo.m_pBenchFixPlatformPos[i].at(k) != NULL)
			{
				PlatformType platformType = lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)->GetPlatformType();
				switch (platformType)
				{
				case ePlatformXYD:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformX1X2Y:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformXY1Y2:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformXY:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformX:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformY:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformYX1X2PD:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformYDPX1X2:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformDXY:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				default:
					break;
				}
			}
		}

		nplatCnt = lBTMFInfo.m_pBenchPickPlatformPos[i].size();
		m_pBenchPickPlatformPos[i].resize(nplatCnt, NULL);

		for (k=0;k<nplatCnt;k++)
		{
			if (lBTMFInfo.m_pBenchPickPlatformPos[i].at(k) != NULL)
			{
				PlatformType platformType = lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)->GetPlatformType();
				switch (platformType)
				{
				case ePlatformXYD:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformX1X2Y:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformXY1Y2:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformXY:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformX:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformY:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformYX1X2PD:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformYDPX1X2:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformDXY:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				default:
					break;
				}
			}
		}

	}




	// 复制相机运动平台信息
	int nCnt = lBTMFInfo.m_vpBenchFixCamPlatformPos.size();
	m_vpBenchFixCamPlatformPos.resize(nCnt, NULL);
	for (int i=0; i<nCnt; i++)
	{
		if (lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i) != NULL)
		{
			PlatformType platformType = lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)->GetPlatformType();
			switch (platformType)
			{
			case ePlatformXYD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformX1X2Y:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXY1Y2:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformX:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformYX1X2PD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformYDPX1X2:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformDXY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			default:
				break;
			}
		}
	}
	m_bIsValid = lBTMFInfo.m_bIsValid;
}

CBenchTargetMarkPickInfo::CBenchTargetMarkPickInfo( int nMarkNum, PlatformType ePlatformType )
{
	m_nBenchTargetMarkNum = nMarkNum;
	for (int i=0;i<BENCH_FIX_POSNUM;i++)
	{
		m_vBenchTargetMarkImageCoordPoses[i].resize(nMarkNum);
		m_vBenchTargetMarkPlatformCoordPoses[i].resize(nMarkNum);
		m_vBenchPickPlatformOffset[i].resize(nMarkNum);

		m_pBenchFixPlatformPos[i].resize(nMarkNum,NULL);
	}


	int k=0;
	for (int i=0;i<BENCH_FIX_POSNUM;i++)
	{
		for (k=0;k<nMarkNum;k++)
		{
			switch (ePlatformType)
			{
			case ePlatformXYD:
				m_pBenchFixPlatformPos[i].at(k) = new CPlatformXYDAxisPos();
				break;
			case ePlatformX1X2Y:
				m_pBenchFixPlatformPos[i].at(k) = new CPlatformX1X2YAxisPos();
				break;
			case ePlatformXY1Y2:
				m_pBenchFixPlatformPos[i].at(k) = new CPlatformXY1Y2AxisPos();
				break;
			case ePlatformXY:
				m_pBenchFixPlatformPos[i].at(k) = new CPlatformXYAxisPos();
				break;
			case ePlatformX:
				m_pBenchFixPlatformPos[i].at(k) = new CPlatformXAxisPos();
				break;
			case ePlatformY:
				m_pBenchFixPlatformPos[i].at(k) = new CPlatformYAxisPos();
				break;
			case ePlatformYX1X2PD:
				m_pBenchFixPlatformPos[i].at(k) = new CPlatformYX1X2PDAxisPos();
				break;
			case ePlatformYDPX1X2:
				m_pBenchFixPlatformPos[i].at(k) = new CPlatformYDPX1X2AxisPos();
				break;
			case ePlatformDXY:
				m_pBenchFixPlatformPos[i].at(k) = new CPlatformDXYAxisPos();
				break;
			default:
				break;
			}


		}


		m_pBenchPickPlatformPos[i].resize(nMarkNum,NULL);

		for (k=0;k<nMarkNum;k++)
		{
			switch (ePlatformType)
			{
			case ePlatformXYD:
				m_pBenchPickPlatformPos[i].at(k) = new CPlatformXYDAxisPos();
				break;
			case ePlatformX1X2Y:
				m_pBenchPickPlatformPos[i].at(k) = new CPlatformX1X2YAxisPos();
				break;
			case ePlatformXY1Y2:
				m_pBenchPickPlatformPos[i].at(k) = new CPlatformXY1Y2AxisPos();
				break;
			case ePlatformXY:
				m_pBenchPickPlatformPos[i].at(k) = new CPlatformXYAxisPos();
				break;
			case ePlatformX:
				m_pBenchPickPlatformPos[i].at(k) = new CPlatformXAxisPos();
				break;
			case ePlatformY:
				m_pBenchPickPlatformPos[i].at(k) = new CPlatformYAxisPos();
				break;
			case ePlatformYX1X2PD:
				m_pBenchPickPlatformPos[i].at(k) = new CPlatformYX1X2PDAxisPos();
				break;
			case ePlatformYDPX1X2:
				m_pBenchPickPlatformPos[i].at(k) = new CPlatformYDPX1X2AxisPos();
				break;
			case ePlatformDXY:
				m_pBenchPickPlatformPos[i].at(k) = new CPlatformDXYAxisPos();
				break;
			default:
				break;
			}
		}

	}


	m_vpBenchFixCamPlatformPos.resize(nMarkNum, NULL);
	for (int i=0; i<nMarkNum; i++)
	{
		m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYAxisPos;
	}

	m_bIsValid = FALSE;
}

CBenchTargetMarkPickInfo::~CBenchTargetMarkPickInfo()
{
	int k=0;
	for (int i=0;i<BENCH_FIX_POSNUM;i++)
	{	
		m_vBenchTargetMarkImageCoordPoses[i].clear();
		m_vBenchTargetMarkPlatformCoordPoses[i].clear();
		m_vBenchPickPlatformOffset[i].clear();


		for (k=0;k<m_pBenchFixPlatformPos[i].size();k++)
		{
			if (m_pBenchFixPlatformPos[i].at(k) != NULL)
			{
				delete m_pBenchFixPlatformPos[i].at(k);
				m_pBenchFixPlatformPos[i].at(k) = NULL;
			}
		}

		for (k=0;k<m_pBenchPickPlatformPos[i].size();k++)
		{
			if (m_pBenchPickPlatformPos[i].at(k) != NULL)
			{
				delete m_pBenchPickPlatformPos[i].at(k);
				m_pBenchPickPlatformPos[i].at(k) = NULL;
			}
		}
	}



	for (int i=0;i<m_vpBenchFixCamPlatformPos.size();i++)
	{
		if (m_vpBenchFixCamPlatformPos.at(i) != NULL)
		{
			delete m_vpBenchFixCamPlatformPos.at(i);
			m_vpBenchFixCamPlatformPos.at(i) = NULL;
		}
	}
	m_vpBenchFixCamPlatformPos.clear();

	// 	for (int i = 0; i < m_vCalibratedInfo.size(); i++)
	// 	{
	// 		if (m_vCalibratedInfo.at(i) != NULL)
	// 		{
	// 			delete m_vCalibratedInfo.at(i);
	// 			m_vCalibratedInfo.at(i) = NULL;
	// 		}
	// 	}
	// 	m_vCalibratedInfo.clear();

	m_bIsValid = FALSE;
}

CBenchTargetMarkPickInfo& CBenchTargetMarkPickInfo::operator=( const CBenchTargetMarkPickInfo& lBTMFInfo )
{
	if (this == &lBTMFInfo)
	{
		return *this;
	}

	m_nBenchTargetMarkNum = lBTMFInfo.m_nBenchTargetMarkNum;
	for (int i=0;i<BENCH_FIX_POSNUM;i++)
	{
		m_vBenchTargetMarkImageCoordPoses[i] = lBTMFInfo.m_vBenchTargetMarkImageCoordPoses[i];
		m_vBenchTargetMarkPlatformCoordPoses[i] = lBTMFInfo.m_vBenchTargetMarkPlatformCoordPoses[i];
		m_vBenchPickPlatformOffset[i] = lBTMFInfo.m_vBenchPickPlatformOffset[i];

		int nCntplatOld = m_pBenchFixPlatformPos[i].size();
		int k=0;
		for (k=0;k<nCntplatOld;k++)
		{
			if (m_pBenchFixPlatformPos[i].at(k) != NULL)
			{
				delete m_pBenchFixPlatformPos[i].at(k);
				m_pBenchFixPlatformPos[i].at(k) = NULL;
			}

		}

		int nCntplatNew=lBTMFInfo.m_pBenchFixPlatformPos[i].size();
		m_pBenchFixPlatformPos[i].resize(nCntplatNew);
		for (k=0;k<nCntplatNew;k++)
		{
			if (lBTMFInfo.m_pBenchFixPlatformPos[i].at(k) != NULL)
			{
				PlatformType platformType = lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)->GetPlatformType();
				switch (platformType)
				{
				case ePlatformXYD:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformX1X2Y:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformXY1Y2:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformXY:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformX:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformY:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformYX1X2PD:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformYDPX1X2:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				case ePlatformDXY:
					m_pBenchFixPlatformPos[i].at(k) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_pBenchFixPlatformPos[i].at(k)));
					break;
				default:
					break;
				}
			}
		}




		nCntplatOld = m_pBenchPickPlatformPos[i].size();
		k=0;
		for (k=0;k<nCntplatOld;k++)
		{
			if (m_pBenchPickPlatformPos[i].at(k) != NULL)
			{
				delete m_pBenchPickPlatformPos[i].at(k);
				m_pBenchPickPlatformPos[i].at(k) = NULL;
			}

		}

		nCntplatNew=lBTMFInfo.m_pBenchPickPlatformPos[i].size();
		m_pBenchPickPlatformPos[i].resize(nCntplatNew);
		for (k=0;k<nCntplatNew;k++)
		{
			if (lBTMFInfo.m_pBenchPickPlatformPos[i].at(k) != NULL)
			{
				PlatformType platformType = lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)->GetPlatformType();
				switch (platformType)
				{
				case ePlatformXYD:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformX1X2Y:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformXY1Y2:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformXY:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformX:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformY:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformYX1X2PD:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformYDPX1X2:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				case ePlatformDXY:
					m_pBenchPickPlatformPos[i].at(k) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_pBenchPickPlatformPos[i].at(k)));
					break;
				default:
					break;
				}
			}
		}
	}





	// 复制相机运动平台信息
	int nCntOld = m_vpBenchFixCamPlatformPos.size();
	int i = 0;
	for (i=0; i<nCntOld; i++)
	{
		if (m_vpBenchFixCamPlatformPos.at(i) != NULL)
		{
			delete m_vpBenchFixCamPlatformPos.at(i);
			m_vpBenchFixCamPlatformPos.at(i) = NULL;
		}
	}
	int nCntNew = lBTMFInfo.m_vpBenchFixCamPlatformPos.size();
	m_vpBenchFixCamPlatformPos.resize(nCntNew, NULL);
	for (i=0; i<nCntNew; i++)
	{
		if (lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i) != NULL)
		{
			PlatformType platformType = lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)->GetPlatformType();
			switch (platformType)
			{
			case ePlatformXYD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformX1X2Y:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXY1Y2:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformXY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformX:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformYX1X2PD:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformYDPX1X2:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			case ePlatformDXY:
				m_vpBenchFixCamPlatformPos.at(i) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)lBTMFInfo.m_vpBenchFixCamPlatformPos.at(i)));
				break;
			default:
				break;
			}
		}
	}
	m_bIsValid = lBTMFInfo.m_bIsValid;

	return *this;
}

void CBenchTargetMarkPickInfo::SetMarkNum( int nNum )
{
	if (nNum < 0)
	{
		return;
	}
	m_nBenchTargetMarkNum = nNum;
	for (int i=0;i<BENCH_FIX_POSNUM;i++)
	{
		m_vBenchTargetMarkImageCoordPoses[i].resize(nNum);
		m_vBenchTargetMarkPlatformCoordPoses[i].resize(nNum);
		m_vBenchPickPlatformOffset[i].resize(nNum);

	}


	if (m_vpBenchFixCamPlatformPos.size() > nNum)
	{
		for (int i=nNum; i<m_vpBenchFixCamPlatformPos.size(); i++)
		{
			delete m_vpBenchFixCamPlatformPos.at(i);
			m_vpBenchFixCamPlatformPos.at(i) = NULL;
		}
	}

	m_vpBenchFixCamPlatformPos.resize(nNum, NULL);
	if (m_vpBenchFixCamPlatformPos.size() < nNum)
	{
		for (int i=nNum; i<m_vpBenchFixCamPlatformPos.size(); i++)
		{
			m_vpBenchFixCamPlatformPos.at(i) = new CPlatformXYAxisPos;
		}
	}
}

int CBenchTargetMarkPickInfo::GetMarkNum()
{
	return m_nBenchTargetMarkNum;
}


BOOL CBenchTargetMarkPickInfo::SetMarkImageCoordPos( int nIndex, CCoordPos CoorPos ,int nPos)
{

	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum) ||(nPos<0) ||(nPos>=BENCH_FIX_POSNUM))
	{
		return FALSE;
	}
	else
	{
		m_vBenchTargetMarkImageCoordPoses[nPos].at(nIndex) = CoorPos;
		return TRUE;
	}
}

CCoordPos CBenchTargetMarkPickInfo::GetMarkImageCoordPos( int nIndex,int nPos )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum)||(nPos<0) ||(nPos>=BENCH_FIX_POSNUM))
	{
		CCoordPos pos;
		return pos;
	}
	else
	{
		return m_vBenchTargetMarkImageCoordPoses[nPos].at(nIndex);
	}
}

BOOL CBenchTargetMarkPickInfo::SetMarkPlatformCoordPos( int nIndex, CCoordPos CoorPos,int nPos )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum)||(nPos<0) ||(nPos>=BENCH_FIX_POSNUM))
	{
		return FALSE;
	}
	else
	{
		m_vBenchTargetMarkPlatformCoordPoses[nPos].at(nIndex) = CoorPos;
		return TRUE;
	}
}

//计算基准修正的平台坐标
//
CCoordPos CBenchTargetMarkPickInfo::GetMarkPlatformCoordPos( int nIndex,int nPos )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum)||(nPos<0) ||(nPos>=BENCH_FIX_POSNUM))
	{
		CCoordPos pos;
		return pos;
	}
	else
	{
		return m_vBenchTargetMarkPlatformCoordPoses[nPos].at(nIndex);
	}
}


// 	设置标准的对象Mark与拾取旋转中心之间的偏差
BOOL CBenchTargetMarkPickInfo::SetPickPlatformOffset( int nIndex, CCoordPos CoorPos,int nPos )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum)||(nPos<0) ||(nPos>=BENCH_FIX_POSNUM))
	{
		return FALSE;
	}
	else
	{
		m_vBenchPickPlatformOffset[nPos].at(nIndex) = CoorPos;
		return TRUE;
	}
}

// 	获取标准的对象Mark与拾取旋转中心之间的偏差
CCoordPos CBenchTargetMarkPickInfo::GetPickPlatformOffset( int nIndex,int nPos )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum)||(nPos<0) ||(nPos>=BENCH_FIX_POSNUM))
	{
		CCoordPos pos;
		return pos;
	}
	else
	{
		return m_vBenchPickPlatformOffset[nPos].at(nIndex);
	}
}

BOOL CBenchTargetMarkPickInfo::SetFixPlatformPos(std::vector< CPlatformAxisPos*> pAxisPos,int nPos )
{
	// 释放原有数据的内存并重新创建(new)
	int i=0;
	for (i=0;i<pAxisPos.size();i++)
	{
		if (pAxisPos.at(i) == NULL)
		{
			return FALSE;
		}
	}


	int nCnt=pAxisPos.size();
	for (i=0;i<m_pBenchFixPlatformPos[nPos].size();i++)
	{
		if (m_pBenchFixPlatformPos[nPos].at(i) != NULL)
		{
			delete m_pBenchFixPlatformPos[nPos].at(i);
			m_pBenchFixPlatformPos[nPos].at(i) = NULL;
		}

	}

	m_pBenchFixPlatformPos[nPos].resize(nCnt);

	for (i=0;i<nCnt;i++)
	{
		//判断平台类型并根据类型动态分配内存
		PlatformType platformType = pAxisPos.at(i)->GetPlatformType();

		switch (platformType)
		{
		case ePlatformXYD:
			m_pBenchFixPlatformPos[nPos].at(i) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformX1X2Y:
			m_pBenchFixPlatformPos[nPos].at(i) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformXY1Y2:
			m_pBenchFixPlatformPos[nPos].at(i) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformXY:
			m_pBenchFixPlatformPos[nPos].at(i) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformX:
			m_pBenchFixPlatformPos[nPos].at(i) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformY:
			m_pBenchFixPlatformPos[nPos].at(i) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformYX1X2PD:
			m_pBenchFixPlatformPos[nPos].at(i) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformYDPX1X2:
			m_pBenchFixPlatformPos[nPos].at(i) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformDXY:
			m_pBenchFixPlatformPos[nPos].at(i) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)pAxisPos.at(i)));
			break;
		default:
			break;
		}
	}

	return TRUE;	
}

BOOL CBenchTargetMarkPickInfo::SetFixPlatformPos( int nIndex, CPlatformAxisPos* pAxisPos ,int nPos)
{
	//	ASSERT((nIndex >= 0) && (nIndex < m_pBenchFixPlatformPos.size()));
	if ((nIndex < 0))
	{
		return FALSE;
	}
	if (nPos<0 || nPos>=BENCH_FIX_POSNUM)
	{
		return FALSE;
	}
	if (nIndex+1>m_pBenchFixPlatformPos[nPos].size())
	{
		m_pBenchFixPlatformPos[nPos].resize(nIndex+1);
		m_pBenchFixPlatformPos[nPos].at(nIndex)=NULL;
	}
	// 释放原有数据的内存并重新创建(new)
	if (pAxisPos == NULL)
	{
		return FALSE;
	}

	if (m_pBenchFixPlatformPos[nPos].at(nIndex) != NULL)
	{
		delete m_pBenchFixPlatformPos[nPos].at(nIndex);
		m_pBenchFixPlatformPos[nPos].at(nIndex) = NULL;
	}	

	//判断平台类型并根据类型动态分配内存
	PlatformType platformType = pAxisPos->GetPlatformType();

	switch (platformType)
	{
	case ePlatformXYD:
		m_pBenchFixPlatformPos[nPos].at(nIndex) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)pAxisPos));
		break;
	case ePlatformX1X2Y:
		m_pBenchFixPlatformPos[nPos].at(nIndex) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)pAxisPos));
		break;
	case ePlatformXY1Y2:
		m_pBenchFixPlatformPos[nPos].at(nIndex) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)pAxisPos));
		break;
	case ePlatformXY:
		m_pBenchFixPlatformPos[nPos].at(nIndex) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
		break;
	case ePlatformX:
		m_pBenchFixPlatformPos[nPos].at(nIndex) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos));
		break;
	case ePlatformY:
		m_pBenchFixPlatformPos[nPos].at(nIndex) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos));
		break;
	case ePlatformYX1X2PD:
		m_pBenchFixPlatformPos[nPos].at(nIndex) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)pAxisPos));
		break;
	case ePlatformYDPX1X2:
		m_pBenchFixPlatformPos[nPos].at(nIndex) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)pAxisPos));
		break;
	case ePlatformDXY:
		m_pBenchFixPlatformPos[nPos].at(nIndex) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)pAxisPos));
		break;
	default:
		break;
	}

	return TRUE;
	// 	if (ePlatformXY == platformType)
	// 	{
	// 		m_pBenchFixPlatformPos.at(nIndex) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
	// 		return TRUE;
	// 	}
	// 	else
	// 		return FALSE;
}



std::vector<CPlatformAxisPos*> CBenchTargetMarkPickInfo::GetFixPlatformPos(int nPos)
{
	return m_pBenchFixPlatformPos[nPos];
}
CPlatformAxisPos* CBenchTargetMarkPickInfo::GetFixPlatformPos(int nIndex,int nPos)
{
	if (nIndex<0 || nIndex>=m_pBenchFixPlatformPos[nPos].size())
	{
		return NULL;
	}
	return m_pBenchFixPlatformPos[nPos].at(nIndex);
}


BOOL CBenchTargetMarkPickInfo::SetPickPlatformPos(std::vector< CPlatformAxisPos*> pAxisPos ,int nPos)
{
	// 释放原有数据的内存并重新创建(new)
	int i=0;
	for (i=0;i<pAxisPos.size();i++)
	{
		if (pAxisPos.at(i) == NULL)
		{
			return FALSE;
		}
	}


	int nCnt=pAxisPos.size();
	for (i=0;i<m_pBenchPickPlatformPos[nPos].size();i++)
	{
		if (m_pBenchPickPlatformPos[nPos].at(i) != NULL)
		{
			delete m_pBenchPickPlatformPos[nPos].at(i);
			m_pBenchPickPlatformPos[nPos].at(i) = NULL;
		}

	}

	m_pBenchPickPlatformPos[nPos].resize(nCnt);

	for (i=0;i<nCnt;i++)
	{
		//判断平台类型并根据类型动态分配内存
		PlatformType platformType = pAxisPos.at(i)->GetPlatformType();

		switch (platformType)
		{
		case ePlatformXYD:
			m_pBenchPickPlatformPos[nPos].at(i) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformX1X2Y:
			m_pBenchPickPlatformPos[nPos].at(i) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformXY1Y2:
			m_pBenchPickPlatformPos[nPos].at(i) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformXY:
			m_pBenchPickPlatformPos[nPos].at(i) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformX:
			m_pBenchPickPlatformPos[nPos].at(i) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformY:
			m_pBenchPickPlatformPos[nPos].at(i) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformYX1X2PD:
			m_pBenchPickPlatformPos[nPos].at(i) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformYDPX1X2:
			m_pBenchPickPlatformPos[nPos].at(i) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)pAxisPos.at(i)));
			break;
		case ePlatformDXY:
			m_pBenchPickPlatformPos[nPos].at(i) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)pAxisPos.at(i)));
			break;
		default:
			break;
		}
	}

	return TRUE;	
}

BOOL CBenchTargetMarkPickInfo::SetPickPlatformPos( int nIndex, CPlatformAxisPos* pAxisPos,int nPos )
{
	//	ASSERT((nIndex >= 0) && (nIndex < m_pBenchPickPlatformPos.size()));
	if ((nIndex < 0))
	{
		return FALSE;
	}
	if (nPos<0 ||nPos >BENCH_FIX_POSNUM)
	{
		return FALSE;
	}
	if (nIndex+1>m_pBenchPickPlatformPos[nPos].size())
	{
		m_pBenchPickPlatformPos[nPos].resize(nIndex+1);
		m_pBenchPickPlatformPos[nPos].at(nIndex)=NULL;
	}
	// 释放原有数据的内存并重新创建(new)
	if (pAxisPos == NULL)
	{
		return FALSE;
	}

	if (m_pBenchPickPlatformPos[nPos].at(nIndex) != NULL)
	{
		delete m_pBenchPickPlatformPos[nPos].at(nIndex);
		m_pBenchPickPlatformPos[nPos].at(nIndex) = NULL;
	}	

	//判断平台类型并根据类型动态分配内存
	PlatformType platformType = pAxisPos->GetPlatformType();

	switch (platformType)
	{
	case ePlatformXYD:
		m_pBenchPickPlatformPos[nPos].at(nIndex) = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)pAxisPos));
		break;
	case ePlatformX1X2Y:
		m_pBenchPickPlatformPos[nPos].at(nIndex) = new CPlatformX1X2YAxisPos(*((CPlatformX1X2YAxisPos*)pAxisPos));
		break;
	case ePlatformXY1Y2:
		m_pBenchPickPlatformPos[nPos].at(nIndex) = new CPlatformXY1Y2AxisPos(*((CPlatformXY1Y2AxisPos*)pAxisPos));
		break;
	case ePlatformXY:
		m_pBenchPickPlatformPos[nPos].at(nIndex) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
		break;
	case ePlatformX:
		m_pBenchPickPlatformPos[nPos].at(nIndex) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos));
		break;
	case ePlatformY:
		m_pBenchPickPlatformPos[nPos].at(nIndex) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos));
		break;
	case ePlatformYX1X2PD:
		m_pBenchPickPlatformPos[nPos].at(nIndex) = new CPlatformYX1X2PDAxisPos(*((CPlatformYX1X2PDAxisPos*)pAxisPos));
		break;
	case ePlatformYDPX1X2:
		m_pBenchPickPlatformPos[nPos].at(nIndex) = new CPlatformYDPX1X2AxisPos(*((CPlatformYDPX1X2AxisPos*)pAxisPos));
		break;
	case ePlatformDXY:
		m_pBenchPickPlatformPos[nPos].at(nIndex) = new CPlatformDXYAxisPos(*((CPlatformDXYAxisPos*)pAxisPos));
		break;
	default:
		break;
	}

	return TRUE;
	// 	if (ePlatformXY == platformType)
	// 	{
	// 		m_pBenchPickPlatformPos.at(nIndex) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
	// 		return TRUE;
	// 	}
	// 	else
	// 		return FALSE;
}



std::vector<CPlatformAxisPos*> CBenchTargetMarkPickInfo::GetPickPlatformPos(int nPos)
{
	if (nPos<0 || nPos>=BENCH_FIX_POSNUM)
	{
		std::vector<CPlatformAxisPos*> nullvector;
		return  nullvector;
	}
	return m_pBenchPickPlatformPos[nPos];
}
CPlatformAxisPos* CBenchTargetMarkPickInfo::GetPickPlatformPos(int nIndex,int nPos)
{
	if (nPos<0 || nPos>=BENCH_FIX_POSNUM)
	{
		return NULL;
	}
	if (nIndex<0 || nIndex>=m_pBenchPickPlatformPos[nPos].size())
	{
		return NULL;
	}
	return m_pBenchPickPlatformPos[nPos].at(nIndex);
}



//设置基准mark修正时相机各轴的位置，单个相机设置
BOOL CBenchTargetMarkPickInfo::SetFixCamPlatformPos( int nIndex, CPlatformAxisPos* pAxisPos )
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum))
	{
		return FALSE;
	}
	// 释放原有数据的内存并重新创建(new)
	if (pAxisPos == NULL)
	{
		return FALSE;
	}

	if (m_vpBenchFixCamPlatformPos.at(nIndex) != NULL)
	{
		delete m_vpBenchFixCamPlatformPos.at(nIndex);
		m_vpBenchFixCamPlatformPos.at(nIndex) = NULL;
	}	

	//判断平台类型并根据类型动态分配内存
	PlatformType platformType = pAxisPos->GetPlatformType();

	if (ePlatformXY == platformType)
	{
		m_vpBenchFixCamPlatformPos.at(nIndex) = new CPlatformXYAxisPos(*((CPlatformXYAxisPos*)pAxisPos));
		return TRUE;
	}
	else if (ePlatformX == platformType)
	{
		m_vpBenchFixCamPlatformPos.at(nIndex) = new CPlatformXAxisPos(*((CPlatformXAxisPos*)pAxisPos));
		return TRUE;
	}
	else if (ePlatformY == platformType)
	{
		m_vpBenchFixCamPlatformPos.at(nIndex) = new CPlatformYAxisPos(*((CPlatformYAxisPos*)pAxisPos));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//设置基准mark修正时相机各轴的位置，单个相机
CPlatformAxisPos* CBenchTargetMarkPickInfo::GetFixCamPlatformPos(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < m_nBenchTargetMarkNum));
	if ((nIndex < 0) || (nIndex >= m_nBenchTargetMarkNum))
	{
		return NULL;
	}
	else
		return m_vpBenchFixCamPlatformPos.at(nIndex);
}


/************************************************************************/
/*						 CBendInfo implementations	                   */
/************************************************************************/
// CBendInspectRangeInfo::CBendInspectRangeInfo()
// {
// 	for(int i = 0; i < 1; i++)
// 	{
// 		m_dRangeX[i]			= 1.0;
// 		m_dRangeY[i]			= 1.0;
// 
// 		m_dRangeDis[i]			= 1.0;
// 
// 		m_dRangeTargetDis[i]	= 1.0;
// 		m_dRangeObjectDis[i]	= 1.0;
// 
// 		m_dRangeSubtractX[i]	= 1.0;
// 		m_dRangeSubtractY[i]	= 1.0;
// 
// 		m_dRangeCircleRadius[i] = 0.0;
// 	}
// 
// 	for(int i = 1; i < 3; i++)
// 	{
// 		m_dRangeX[i]			= 0.1;
// 		m_dRangeY[i]			= 0.1;
// 		m_dRangeDis[i]			= 0.1;
// 
// 		m_dRangeTargetDis[i]	= 0.1;
// 		m_dRangeObjectDis[i]	= 0.1;
// 
// 		m_dRangeSubtractY[i]	= 0.1;
// 		m_dRangeSubtractX[i]	= 0.1;
// 
// 		m_dRangeCircleRadius[i] = 0.1;
// 	}
// }

CBendInfo::CBendInfo()
{
	// 专用折弯参数
	m_bBendAlignInspect = FALSE;


	m_bBendType = 0;
	m_dBendImageRadio = 1.0;
	m_bBendAlignDmCode = FALSE;

	// 目标、对象距离补偿
	m_bBendDisOffset = FALSE;
	m_dBendTargetDisOffset = 0.0;
	m_dBendObjectDisOffset = 0.0;

	// Lx、Ly、Rx、Ry距离补偿：目的是使Lx = Rx,Ly = Ry；
	m_bBendResultOffset = FALSE;
	m_scBendLeftDisOffset = sc2Vector(0.0, 0.0);
	m_scBendRightDisOffset = sc2Vector(0.0, 0.0);
	m_dLeftDisOffset = 0.0;
	m_dRightDisOffset = 0.0;

	// 实时计算Lx、Ly、Rx、Ry：目的是让计算的四个值和标准阈值进行比对判定
	m_bBendThreOffset = FALSE;
	m_scBendStanderThre = sc2Vector(0.0, 0.0);

	// 取消Y向绝对值使用
	m_bEnableFbsLRY = FALSE;

	m_bBendPressPara = FALSE;
	m_bBendPressParaWrite = FALSE;
	m_lBendPressParaWriteAddre = 10000;
	m_dBendPressOKYmin = 0.0;
	m_dBendPressOKYmax = 0.0;

	m_dBendPressNGYmin = 0.0;
	m_dBendPressNGYmax = 0.0;

	m_bBendLoadThre = FALSE;
	m_scBendLoadThre = sc2Vector(0.0, 0.0);
	m_bBendInspectDmCode = FALSE;

	m_bEnableReadWsInfo = FALSE;
	m_bBendingInspect = FALSE;
	m_bBendingProductID = FALSE;

	m_bEnableReadInspectWsInfo = FALSE;    // 本压平台信息
	m_bEnableReadStartTimeInfo = FALSE;    // 生产开始时间
	m_bEnableReadEndTimeInfo = FALSE;    // 生产结束时间
	m_bEnableReadProsessingTimeInfo = FALSE;    // 生产总时间

	m_bEnableUseMathCaluLRDis = FALSE;

	m_bEnableMoveSnap= FALSE;
	m_nPosIndexFisrt = 0;
	
	m_bEnableAxisMoveLimit = FALSE;
	m_dAxisAllowMoveMaxX = 1;
	m_dAxisAllowMoveMaxY = 1;
	m_dAxisAllowMoveMaxD = 0.5;

	 m_bBendInspectSnCodeLength=10;//码长度
	 m_bBendInspectSnCodeInvert=FALSE;//码取反
	 m_bBendInspectGetSnOnce=TRUE;

	 m_strPlatformName1 = _T("平台号");
	 m_strPlatformName2 = _T("平台号2");
	 m_bEnableFbisTarObc = FALSE;

	  m_dBendSendszGetName = _T("");
	  m_dBendSendAdsorptionSite=1;//吸附位置
	  m_dBendSendRAngle=1;//R角
	  m_dBendSendCompensateY1=0;//补偿Y1
	  m_dBendSendCompensateY2=0;//补偿Y2
	  m_dBendSendCompensateY3=0;//补偿Y3
	  m_dBendSendCompensateZ1=0;//补偿Z1
	  m_dBendSendCompensateZ2=0;//补偿Z2
	  m_dBendSendCompensateZ3=0;//补偿Z3
}




/************************************************************************/
/*					 CAlignerParam implementations				        */
/************************************************************************/

CAlignerParam::CAlignerParam()
{
	m_nAlignerIndex				= 0;
	m_strAlignerCustomName		= _T("");
	
	m_AlignerPrecision.m_dPosX	= 0.01;
	m_AlignerPrecision.m_dPosY	= 0.01;
	m_AlignerPrecision.m_dAngle	= 0.01;
	m_nAlignerMaxTime			= 1;

	m_nSearchDelayTime = 0;
	m_nObjectSearchDelayTime = 0;
	m_nObjectSearchDelayTime2 = 0;
	m_bEnableObjectSearchDelayTime2 = FALSE;


	m_eAligner2Vs2Type			= eAlignerPos12_Auto;
	m_eAligner3Vs3Type			= eAlignerPos123_Auto;
	m_eAligner4Vs4Type			= eAlignerPos1234_Auto;
	m_eAligner5Vs5Type			= eAlignerPos12345_Auto;
	m_eAligner6Vs6Type			= eAlignerPos123456_Auto;
	m_dAlignerWeight			= 0.8;
	m_nAlignerCirclePos         = 0;

	m_eAlignerMoveType			= eAlignerMove_XYD;
	m_bAlignAngleVertical		= FALSE;
	m_nAlignAngleVerticalMode	= 0;

	// SystemConfig
	m_nMultiCalibExtensionMaxNum = 0;	

	// 补偿
	m_eAlignerOffsetCoordType	= eMarkCoord;
	m_AlignerOffset.m_dPosX		= 0.0;
	m_AlignerOffset.m_dPosY		= 0.0;
	m_AlignerOffset.m_dAngle	= 0.0;
	m_nAlignerOffsetNum = ALIGNOFFSETNUM;
	m_vAlignerOffset.resize(m_nAlignerOffsetNum);
	m_vAlignerOffsetTips.resize(m_nAlignerOffsetNum,"------");
	m_nCurAlignerOffsetIndex = 0;

	m_nAlignerBenchOffsetNum = ALIGNBENCHOFFSETNUM;
	m_vAlignerBenchOffset.resize(m_nAlignerBenchOffsetNum);
	m_nCurAlignerBenchOffsetIndex = 0;

	m_bCheckAlnTargetOffsetEnable = FALSE;
	m_dAlnTargetOffsetX = 0.0;
	m_dAlnTargetOffsetY = 0.0;

	m_bEnableMutiTarget    = FALSE;
	m_bDiffPosWithSameSearchResultEnabled = FALSE;
	m_vAlignCheckInfo.resize(m_nMultiCalibExtensionMaxNum + 1);



	// 目标mark
	m_nTargetMarkPatternNum		= 0;
	m_eTargetMarkType			= eTargetBench;
	m_bTargetCandidateEnable = false;
	m_bTargetCandidateSync = false;
	m_nTargetCandidateOffsetNum = TARGETCANDIDATENUM;
	m_vTargetCandidateOffset.resize(m_nTargetCandidateOffsetNum);
	m_vTargetCandidateOffsetEnable.resize(m_nTargetCandidateOffsetNum, false);

	// 对象mark
	m_nObjectMarkPatternNum		= 0;
	m_eObjectMarkType			= eObjectOnline;
	m_bObjectCandidateEnable = false;
	m_bObjectCandidateSync = false;
	m_nObjectCandidateOffsetNum = OBJECTCANDIDATENUM;
	m_vObjectCandidateOffset.resize(m_nObjectCandidateOffsetNum);
	m_vObjectCandidateOffsetEnable.resize(m_nObjectCandidateOffsetNum, false);

	// 虚拟mark
	m_bVirtualObjectMarkEnable	= FALSE;
	m_nVirtualMarkPatternNum	= 0;


	// 关联mark
	m_bTargetObjectCamSeparate  = FALSE;
	m_bTargetCalibUseBoard  = FALSE;
	m_eCalibTargetMarkPatternMode	= eSinglePatternSingleMark;
	m_bCalibTargetMarkEnable	= FALSE;
	m_nCalibTargetMarkPatternNum	= 0;
	m_bEnableCalibTargetDMCodeOutLine = FALSE;
	m_dCalibTargetDMCodeOutLinePercent = 0.05;

	// 检测mark
	m_bSysPlatformInspectEnable = FALSE;
	m_bInspectCandidateEnable = FALSE;
	m_bInspectCandidateSync = FALSE;
	m_nInspectMarkPatternNum = 0;
	m_nInspectCandidateOffsetNum = INSPECTCANDIDATENUM;
	m_vInspectCandidateOffsetEnable.resize(m_nInspectCandidateOffsetNum, false);
	m_bInspectSingleSearch = FALSE;

	// 目标相机、对象相机在不同控制器
	m_bCheckAlnSendTargetOffsetToPLCEnable = FALSE;
	m_bEnableAlnTargetFixOffset = FALSE;
	m_nTargetFixOffsetType = 0;
	m_vAlnTargetFixOffset.resize(ALIGNOFFSETNUM);
	m_nCurAlignerTarFixOffsetIndex = 0;

	m_bAlignOffsetReverseX = FALSE;
	m_bAlignOffsetReverseY = FALSE;
	m_bAlignOffsetReverseD = FALSE;


	// 模板曝光
	m_vvdTargetSearchShutter.resize(MAXPOSNUM);
	for (int i = 0; i < m_vvdTargetSearchShutter.size(); i++)
	{
		m_vvdTargetSearchShutter[i].resize(TARGETCANDIDATENUM);
	}

	m_vvdObjectSearchShutter.resize(MAXPOSNUM);
	for (int i = 0; i < m_vvdObjectSearchShutter.size(); i++)
	{
		m_vvdObjectSearchShutter[i].resize(OBJECTCANDIDATENUM);
	}


	m_vvdCalibTargetSearchShutter.resize(MAXPOSNUM);
	for (int i = 0; i < m_vvdCalibTargetSearchShutter.size(); i++)
	{
		m_vvdCalibTargetSearchShutter[i].resize(OBJECTCANDIDATENUM);
	}

	m_vvdVirtualObjectSearchShutter.resize(MAXPOSNUM);
	for (int i = 0; i < m_vvdVirtualObjectSearchShutter.size(); i++)
	{
		m_vvdVirtualObjectSearchShutter[i].resize(OBJECTCANDIDATENUM);
	}

	m_vvdInspectSearchShutter.resize(MAXPOSNUM);
	for (int i = 0; i < m_vvdInspectSearchShutter.size(); i++)
	{
		m_vvdInspectSearchShutter[i].resize(INSPECTCANDIDATENUM);
	}

	CameraParamEnable TmpcameraParamEnable;
	m_vbTargetSearchShutter.resize(TARGETCANDIDATENUM, TmpcameraParamEnable);
	m_vbObjectSearchShutter.resize(OBJECTCANDIDATENUM, TmpcameraParamEnable);
	m_vbCalibTargetSearchShutter.resize(OBJECTCANDIDATENUM, TmpcameraParamEnable);
	m_vbVirtualObjectSearchShutter.resize(OBJECTCANDIDATENUM, TmpcameraParamEnable);
	m_vbInspectSearchShutter.resize(OBJECTCANDIDATENUM, TmpcameraParamEnable);

	// 基准目标
	m_bBenchTargetMarkFixEnable = FALSE;
	m_pBenchTargetMarkFixInfo	= NULL;

	// 拾取
	m_bBenchTargetMarkPickEnable = FALSE;
	m_pBenchTargetMarkPickInfo	= NULL;



	// 八字对位
	m_bCheckEightShapeAlign = FALSE;
	m_nEightShapeAlignMode = 0;
	m_dEightShapeAngle = 0.0;
	m_bAccordingToPickPlatformEnable = FALSE;
	m_nPosMode = 0;
}

CAlignerParam::CAlignerParam( const CAlignerParam& lPTInfo )
{
	m_nAlignerIndex				= lPTInfo.m_nAlignerIndex;
	m_strAlignerCustomName		= lPTInfo.m_strAlignerCustomName;
	//	m_eAlignerMode				= lPTInfo.m_eAlignerMode;
	m_AlignerPrecision			= lPTInfo.m_AlignerPrecision;
	m_nAlignerMaxTime			= lPTInfo.m_nAlignerMaxTime;
	m_nSearchDelayTime			= lPTInfo.m_nSearchDelayTime;
	m_nObjectSearchDelayTime	= lPTInfo.m_nObjectSearchDelayTime;
	m_nObjectSearchDelayTime2	= lPTInfo.m_nObjectSearchDelayTime2;
	m_bEnableObjectSearchDelayTime2 = lPTInfo.m_bEnableObjectSearchDelayTime2;
	m_eAligner2Vs2Type			= lPTInfo.m_eAligner2Vs2Type;
	m_eAligner3Vs3Type			= lPTInfo.m_eAligner3Vs3Type;
	m_eAligner4Vs4Type			= lPTInfo.m_eAligner4Vs4Type;
	m_eAligner5Vs5Type			= lPTInfo.m_eAligner5Vs5Type;
	m_eAligner6Vs6Type			= lPTInfo.m_eAligner6Vs6Type;
	m_dAlignerWeight			= lPTInfo.m_dAlignerWeight;
	m_nAlignerCirclePos         = lPTInfo.m_nAlignerCirclePos;

	m_eAlignerMoveType			= lPTInfo.m_eAlignerMoveType;
	m_bAlignAngleVertical		= lPTInfo.m_bAlignAngleVertical;
	m_nAlignAngleVerticalMode	= lPTInfo.m_nAlignAngleVerticalMode;
	m_eAlignerOffsetCoordType	= lPTInfo.m_eAlignerOffsetCoordType;
	m_AlignerOffset				= lPTInfo.m_AlignerOffset;
	m_AlignerBaseOffset			= lPTInfo.m_AlignerBaseOffset;
	m_nAlignerOffsetNum			= lPTInfo.m_nAlignerOffsetNum;
	m_vAlignerOffset			= lPTInfo.m_vAlignerOffset;
	m_vAlignerOffsetTips		= lPTInfo.m_vAlignerOffsetTips;
	m_nCurAlignerOffsetIndex	= lPTInfo.m_nCurAlignerOffsetIndex;
	m_nAlignerBenchOffsetNum	= lPTInfo.m_nAlignerBenchOffsetNum;
	m_vAlignerBenchOffset			= lPTInfo.m_vAlignerBenchOffset;
	m_nCurAlignerBenchOffsetIndex	= lPTInfo.m_nCurAlignerBenchOffsetIndex;
	m_eTargetMarkType			= lPTInfo.m_eTargetMarkType;
	m_eObjectMarkType			= lPTInfo.m_eObjectMarkType;
	m_nTargetMarkPatternNum		= lPTInfo.m_nTargetMarkPatternNum;
	m_vstrTargetMarkPatternIDs	= lPTInfo.m_vstrTargetMarkPatternIDs;
	m_bBenchTargetMarkFixEnable = lPTInfo.m_bBenchTargetMarkFixEnable;
	m_pBenchTargetMarkFixInfo	= NULL;
	m_pBenchTargetMarkPickInfo	= NULL;
	m_nObjectMarkPatternNum		= lPTInfo.m_nObjectMarkPatternNum;
	m_vstrObjectMarkPatternIDs	= lPTInfo.m_vstrObjectMarkPatternIDs;
	m_bVirtualObjectMarkEnable	= lPTInfo.m_bVirtualObjectMarkEnable;
	m_nVirtualMarkPatternNum	= lPTInfo.m_nVirtualMarkPatternNum;
	m_vstrVirtualMarkPatternIDs = lPTInfo.m_vstrVirtualMarkPatternIDs;
	m_vObjectVirtualMarkOffset	= lPTInfo.m_vObjectVirtualMarkOffset;
	m_bTargetObjectCamSeparate  = lPTInfo.m_bTargetObjectCamSeparate;
	m_bTargetCalibUseBoard		= lPTInfo.m_bTargetCalibUseBoard;
	//	m_bTargetUseFixedVirtualCoordinates		= lPTInfo.m_bTargetUseFixedVirtualCoordinates;
	m_eCalibTargetMarkPatternMode	= lPTInfo.m_eCalibTargetMarkPatternMode;
	m_bCalibTargetMarkEnable	= lPTInfo.m_bCalibTargetMarkEnable;
	m_bEnableCalibTargetDMCodeOutLine	= lPTInfo.m_bEnableCalibTargetDMCodeOutLine;
	m_dCalibTargetDMCodeOutLinePercent	= lPTInfo.m_dCalibTargetDMCodeOutLinePercent;



	m_nCalibTargetMarkPatternNum	= lPTInfo.m_nCalibTargetMarkPatternNum;
	m_vstrCalibTargetMarkPatternIDs = lPTInfo.m_vstrCalibTargetMarkPatternIDs;
	m_nCurAlignerTarFixOffsetIndex  = lPTInfo.m_nCurAlignerTarFixOffsetIndex;
	// 检测设置信息
	m_bSysPlatformInspectEnable = lPTInfo.m_bSysPlatformInspectEnable;
	//	m_bAlignInspectEnable = lPTInfo.m_bAlignInspectEnable;
	m_bInspectCandidateEnable = lPTInfo.m_bInspectCandidateEnable;
	m_bInspectCandidateSync = lPTInfo.m_bInspectCandidateSync;
	m_nInspectMarkPatternNum = lPTInfo.m_nInspectMarkPatternNum;
	m_vstrInspectMarkPatternIDs = lPTInfo.m_vstrInspectMarkPatternIDs;
	m_nInspectCandidateOffsetNum = lPTInfo.m_nInspectCandidateOffsetNum;
	m_vInspectCandidateOffsetEnable = lPTInfo.m_vInspectCandidateOffsetEnable;

	m_bInspectSingleSearch = lPTInfo.m_bInspectSingleSearch;

	m_bCheckEightShapeAlign = lPTInfo.m_bCheckEightShapeAlign;
	m_nEightShapeAlignMode  = lPTInfo.m_nEightShapeAlignMode;
	m_dEightShapeAngle      = lPTInfo.m_dEightShapeAngle;

	m_bCheckAlnTargetOffsetEnable= lPTInfo.m_bCheckAlnTargetOffsetEnable;
	m_dAlnTargetOffsetX		     = lPTInfo.m_dAlnTargetOffsetX;
	m_dAlnTargetOffsetY		     = lPTInfo.m_dAlnTargetOffsetY;

	m_bCheckAlnSendTargetOffsetToPLCEnable	= lPTInfo.m_bCheckAlnSendTargetOffsetToPLCEnable;
	m_bEnableAlnTargetFixOffset				= lPTInfo.m_bEnableAlnTargetFixOffset;
	m_nTargetFixOffsetType					= lPTInfo.m_nTargetFixOffsetType;
	m_vAlnTargetFixOffset					= lPTInfo.m_vAlnTargetFixOffset;
	m_nCurAlignerTarFixOffsetIndex			= lPTInfo.m_nCurAlignerTarFixOffsetIndex;


	m_bAlignOffsetReverseX = lPTInfo.m_bAlignOffsetReverseX;
	m_bAlignOffsetReverseY = lPTInfo.m_bAlignOffsetReverseY;
	m_bAlignOffsetReverseD = lPTInfo.m_bAlignOffsetReverseD;

	m_bDiffPosWithSameSearchResultEnabled=lPTInfo.m_bDiffPosWithSameSearchResultEnabled;

	m_bTargetCandidateEnable = lPTInfo.m_bTargetCandidateEnable;
	m_bTargetCandidateSync = lPTInfo.m_bTargetCandidateSync;
	m_nTargetCandidateOffsetNum = lPTInfo.m_nTargetCandidateOffsetNum;
	m_vTargetCandidateOffset = lPTInfo.m_vTargetCandidateOffset;
	m_vTargetCandidateOffsetEnable = lPTInfo.m_vTargetCandidateOffsetEnable;

	m_bObjectCandidateEnable = lPTInfo.m_bObjectCandidateEnable;
	m_bObjectCandidateSync = lPTInfo.m_bObjectCandidateSync;
	m_nObjectCandidateOffsetNum = lPTInfo.m_nObjectCandidateOffsetNum;
	m_vObjectCandidateOffset = lPTInfo.m_vObjectCandidateOffset;
	m_vObjectCandidateOffsetEnable = lPTInfo.m_vObjectCandidateOffsetEnable;

	//	m_AlignCheckInfo	= lPTInfo.m_AlignCheckInfo;
	m_vAlignCheckInfo	= lPTInfo.m_vAlignCheckInfo;
	m_vTargetPosPatInfo = lPTInfo.m_vTargetPosPatInfo;
	m_vObjectPosPatInfo = lPTInfo.m_vObjectPosPatInfo;

	m_ProductSizeInfo = lPTInfo.m_ProductSizeInfo;	//20160510增加对角补偿
	m_nPosMode = lPTInfo.m_nPosMode;				//20160510增加对角补偿


	m_ProductOffsetInfo = lPTInfo.m_ProductOffsetInfo;// 补偿设置（补偿的方向、X和Y补偿值对调）

	if (lPTInfo.m_pBenchTargetMarkFixInfo != NULL)
	{
		m_pBenchTargetMarkFixInfo = new CBenchTargetMarkFixInfo( *(lPTInfo.m_pBenchTargetMarkFixInfo) );
	}


	m_bBenchTargetMarkPickEnable = lPTInfo.m_bBenchTargetMarkPickEnable;
	if (lPTInfo.m_pBenchTargetMarkPickInfo != NULL)
	{
		m_pBenchTargetMarkPickInfo = new CBenchTargetMarkPickInfo( *(lPTInfo.m_pBenchTargetMarkPickInfo) );
	}

	// 	m_vdTargetSearchShutter = lPTInfo.m_vdTargetSearchShutter;
	// 	m_vdObjectSearchShutter = lPTInfo.m_vdObjectSearchShutter;
	// 	m_vdTarObjSepCalibSearchShutter = lPTInfo.m_vdTarObjSepCalibSearchShutter;
	//	m_vdInspectSearchShutter = lPTInfo.m_vdInspectSearchShutter;

	// 	m_bObjectSearchShutter= lPTInfo.m_bObjectSearchShutter;
	// 	m_bTargetSearchShutter= lPTInfo.m_bTargetSearchShutter;
	//	m_bInspectSearchShutter= lPTInfo.m_bInspectSearchShutter;

	m_vvdTargetSearchShutter = lPTInfo.m_vvdTargetSearchShutter;
	m_vvdObjectSearchShutter = lPTInfo.m_vvdObjectSearchShutter;
	m_vvdCalibTargetSearchShutter = lPTInfo.m_vvdCalibTargetSearchShutter;
	m_vvdVirtualObjectSearchShutter = lPTInfo.m_vvdVirtualObjectSearchShutter;
	m_vvdInspectSearchShutter = lPTInfo.m_vvdInspectSearchShutter;

	m_vbTargetSearchShutter = lPTInfo.m_vbTargetSearchShutter;
	m_vbObjectSearchShutter = lPTInfo.m_vbObjectSearchShutter;
	m_vbCalibTargetSearchShutter = lPTInfo.m_vbCalibTargetSearchShutter;
	m_vbVirtualObjectSearchShutter = lPTInfo.m_vbVirtualObjectSearchShutter;
	m_vbInspectSearchShutter = lPTInfo.m_vbInspectSearchShutter;

	m_bEnableMutiTarget=lPTInfo.m_bEnableMutiTarget;
	m_bAccordingToPickPlatformEnable = lPTInfo.m_bAccordingToPickPlatformEnable;

	m_nMultiCalibExtensionMaxNum = lPTInfo.m_nMultiCalibExtensionMaxNum;


	m_WeldInfo						= lPTInfo.m_WeldInfo;					// 焊接参数
//	m_PCBInfo						= lPTInfo.m_PCBInfo;					// 设置PCB参数
	m_ProductPositionVirtualInfo	= lPTInfo.m_ProductPositionVirtualInfo;
//	m_BendInspectRangeInfo			= lPTInfo.m_BendInspectRangeInfo;		// 设置检测折弯参数
	m_BendInfo						= lPTInfo.m_BendInfo;
	m_AlignOffsetByArea				= lPTInfo.m_AlignOffsetByArea;

	m_cPhotoLightCtrl =  lPTInfo.m_cPhotoLightCtrl;
}

CAlignerParam::~CAlignerParam()
{
	m_vstrTargetMarkPatternIDs.clear();
	m_vstrObjectMarkPatternIDs.clear();
	m_vstrVirtualMarkPatternIDs.clear();
	m_vObjectVirtualMarkOffset.clear();
	if (m_pBenchTargetMarkFixInfo != NULL)
	{
		delete m_pBenchTargetMarkFixInfo;
		m_pBenchTargetMarkFixInfo = NULL;
	}


	if (m_pBenchTargetMarkPickInfo != NULL)
	{
		delete m_pBenchTargetMarkPickInfo;
		m_pBenchTargetMarkPickInfo = NULL;
	}

	
}

CAlignerParam& CAlignerParam::operator=( const CAlignerParam& lPTInfo )
{
	if (this == &lPTInfo)
	{
		return *this;
	}
	m_nAlignerIndex				= lPTInfo.m_nAlignerIndex;
	m_strAlignerCustomName		= lPTInfo.m_strAlignerCustomName;
	//	m_eAlignerMode				= lPTInfo.m_eAlignerMode;
	m_AlignerPrecision			= lPTInfo.m_AlignerPrecision;
	m_nAlignerMaxTime			= lPTInfo.m_nAlignerMaxTime;
	m_nSearchDelayTime			= lPTInfo.m_nSearchDelayTime;
	m_nObjectSearchDelayTime	= lPTInfo.m_nObjectSearchDelayTime;
	m_nObjectSearchDelayTime2	= lPTInfo.m_nObjectSearchDelayTime2;
	m_bEnableObjectSearchDelayTime2 = lPTInfo.m_bEnableObjectSearchDelayTime2;
	m_eAligner2Vs2Type			= lPTInfo.m_eAligner2Vs2Type;
	m_eAligner3Vs3Type			= lPTInfo.m_eAligner3Vs3Type;
	m_eAligner4Vs4Type			= lPTInfo.m_eAligner4Vs4Type;
	m_eAligner5Vs5Type			= lPTInfo.m_eAligner5Vs5Type;
	m_eAligner6Vs6Type			= lPTInfo.m_eAligner6Vs6Type;
	m_eAlignerMoveType			= lPTInfo.m_eAlignerMoveType;
	m_dAlignerWeight			= lPTInfo.m_dAlignerWeight;
	m_nAlignerCirclePos         = lPTInfo.m_nAlignerCirclePos;

	m_bAlignAngleVertical		= lPTInfo.m_bAlignAngleVertical;
	m_nAlignAngleVerticalMode	= lPTInfo.m_nAlignAngleVerticalMode;
	m_eAlignerOffsetCoordType	= lPTInfo.m_eAlignerOffsetCoordType;
	m_AlignerOffset				= lPTInfo.m_AlignerOffset;
	m_AlignerBaseOffset			= lPTInfo.m_AlignerBaseOffset;
	m_nAlignerOffsetNum			= lPTInfo.m_nAlignerOffsetNum;
	m_vAlignerOffset			= lPTInfo.m_vAlignerOffset;
	m_vAlignerOffsetTips		= lPTInfo.m_vAlignerOffsetTips;
	m_nCurAlignerOffsetIndex	= lPTInfo.m_nCurAlignerOffsetIndex;
	m_nAlignerBenchOffsetNum	= lPTInfo.m_nAlignerBenchOffsetNum;
	m_vAlignerBenchOffset			= lPTInfo.m_vAlignerBenchOffset;
	m_nCurAlignerBenchOffsetIndex	= lPTInfo.m_nCurAlignerBenchOffsetIndex;
	m_eTargetMarkType			= lPTInfo.m_eTargetMarkType;
	m_eObjectMarkType			= lPTInfo.m_eObjectMarkType;
	m_nTargetMarkPatternNum		= lPTInfo.m_nTargetMarkPatternNum;
	m_vstrTargetMarkPatternIDs	= lPTInfo.m_vstrTargetMarkPatternIDs;
	m_bBenchTargetMarkFixEnable = lPTInfo.m_bBenchTargetMarkFixEnable;
	m_nObjectMarkPatternNum		= lPTInfo.m_nObjectMarkPatternNum;
	m_vstrObjectMarkPatternIDs	= lPTInfo.m_vstrObjectMarkPatternIDs;
	m_bVirtualObjectMarkEnable	= lPTInfo.m_bVirtualObjectMarkEnable;
	m_nVirtualMarkPatternNum	= lPTInfo.m_nVirtualMarkPatternNum;
	m_vstrVirtualMarkPatternIDs = lPTInfo.m_vstrVirtualMarkPatternIDs;
	m_vObjectVirtualMarkOffset	= lPTInfo.m_vObjectVirtualMarkOffset;
	m_bTargetObjectCamSeparate  = lPTInfo.m_bTargetObjectCamSeparate;
	m_bTargetCalibUseBoard		= lPTInfo.m_bTargetCalibUseBoard;
	//	m_bTargetUseFixedVirtualCoordinates		= lPTInfo.m_bTargetUseFixedVirtualCoordinates;
	m_eCalibTargetMarkPatternMode	= lPTInfo.m_eCalibTargetMarkPatternMode;
	m_bCalibTargetMarkEnable	= lPTInfo.m_bCalibTargetMarkEnable;
	m_bEnableCalibTargetDMCodeOutLine	= lPTInfo.m_bEnableCalibTargetDMCodeOutLine;
	m_dCalibTargetDMCodeOutLinePercent	= lPTInfo.m_dCalibTargetDMCodeOutLinePercent;
	m_nCalibTargetMarkPatternNum	= lPTInfo.m_nCalibTargetMarkPatternNum;
	m_vstrCalibTargetMarkPatternIDs = lPTInfo.m_vstrCalibTargetMarkPatternIDs;
	
	// 检测设置信息
	m_bSysPlatformInspectEnable = lPTInfo.m_bSysPlatformInspectEnable;
	//	m_bAlignInspectEnable = lPTInfo.m_bAlignInspectEnable;
	m_bInspectCandidateEnable = lPTInfo.m_bInspectCandidateEnable;
	m_bInspectCandidateSync = lPTInfo.m_bInspectCandidateSync;
	m_nInspectMarkPatternNum = lPTInfo.m_nInspectMarkPatternNum;
	m_vstrInspectMarkPatternIDs = lPTInfo.m_vstrInspectMarkPatternIDs;
	m_nInspectCandidateOffsetNum = lPTInfo.m_nInspectCandidateOffsetNum;
	m_vInspectCandidateOffsetEnable = lPTInfo.m_vInspectCandidateOffsetEnable;

	m_bInspectSingleSearch = lPTInfo.m_bInspectSingleSearch;

	//八字对位
	m_bCheckEightShapeAlign = lPTInfo.m_bCheckEightShapeAlign;
	m_nEightShapeAlignMode  = lPTInfo.m_nEightShapeAlignMode;
	m_dEightShapeAngle      = lPTInfo.m_dEightShapeAngle;

	m_bCheckAlnTargetOffsetEnable= lPTInfo.m_bCheckAlnTargetOffsetEnable;
	m_dAlnTargetOffsetX		     = lPTInfo.m_dAlnTargetOffsetX;
	m_dAlnTargetOffsetY		     = lPTInfo.m_dAlnTargetOffsetY;
	m_bCheckAlnSendTargetOffsetToPLCEnable = lPTInfo.m_bCheckAlnSendTargetOffsetToPLCEnable;

	m_bEnableAlnTargetFixOffset		= lPTInfo.m_bEnableAlnTargetFixOffset;
	m_nTargetFixOffsetType			= lPTInfo.m_nTargetFixOffsetType;
	m_vAlnTargetFixOffset			= lPTInfo.m_vAlnTargetFixOffset;
	//m_dAlnTargetFixOffsetX = lPTInfo.m_dAlnTargetFixOffsetX;
	//m_dAlnTargetFixOffsetY = lPTInfo.m_dAlnTargetFixOffsetY;
	//m_dAlnTargetFixOffsetD = lPTInfo.m_dAlnTargetFixOffsetD;

	m_bAlignOffsetReverseX = lPTInfo.m_bAlignOffsetReverseX;
	m_bAlignOffsetReverseY = lPTInfo.m_bAlignOffsetReverseY;
	m_bAlignOffsetReverseD = lPTInfo.m_bAlignOffsetReverseD;

	m_bDiffPosWithSameSearchResultEnabled = lPTInfo.m_bDiffPosWithSameSearchResultEnabled;

	m_bTargetCandidateEnable = lPTInfo.m_bTargetCandidateEnable;
	m_bTargetCandidateSync = lPTInfo.m_bTargetCandidateSync;
	m_nTargetCandidateOffsetNum = lPTInfo.m_nTargetCandidateOffsetNum;
	m_vTargetCandidateOffset = lPTInfo.m_vTargetCandidateOffset;
	m_vTargetCandidateOffsetEnable = lPTInfo.m_vTargetCandidateOffsetEnable;

	m_bObjectCandidateEnable = lPTInfo.m_bObjectCandidateEnable;
	m_bObjectCandidateSync = lPTInfo.m_bObjectCandidateSync;
	m_nObjectCandidateOffsetNum = lPTInfo.m_nObjectCandidateOffsetNum;
	m_vObjectCandidateOffset = lPTInfo.m_vObjectCandidateOffset;
	m_vObjectCandidateOffsetEnable = lPTInfo.m_vObjectCandidateOffsetEnable;

	//m_AlignCheckInfo	= lPTInfo.m_AlignCheckInfo;
	m_vAlignCheckInfo	= lPTInfo.m_vAlignCheckInfo;
	m_vTargetPosPatInfo = lPTInfo.m_vTargetPosPatInfo;
	m_vObjectPosPatInfo = lPTInfo.m_vObjectPosPatInfo;

	m_ProductSizeInfo = lPTInfo.m_ProductSizeInfo;	//20160510增加对角补偿
	m_nPosMode = lPTInfo.m_nPosMode;				//20160510增加对角补偿

	m_ProductOffsetInfo = lPTInfo.m_ProductOffsetInfo;  // 补偿设置（补偿的方向、X和Y补偿值对调）

	

	if (m_pBenchTargetMarkFixInfo != NULL)
	{
		delete m_pBenchTargetMarkFixInfo;
		m_pBenchTargetMarkFixInfo = NULL;
	}
	if (lPTInfo.m_pBenchTargetMarkFixInfo != NULL)
	{
		m_pBenchTargetMarkFixInfo = new CBenchTargetMarkFixInfo( *(lPTInfo.m_pBenchTargetMarkFixInfo) );
	}


	m_bBenchTargetMarkPickEnable = lPTInfo.m_bBenchTargetMarkPickEnable;
	if (m_pBenchTargetMarkPickInfo != NULL)
	{
		delete m_pBenchTargetMarkPickInfo;
		m_pBenchTargetMarkPickInfo = NULL;
	}
	if (lPTInfo.m_pBenchTargetMarkPickInfo != NULL)
	{
		m_pBenchTargetMarkPickInfo = new CBenchTargetMarkPickInfo( *(lPTInfo.m_pBenchTargetMarkPickInfo) );
	}

	// 	m_vdTargetSearchShutter = lPTInfo.m_vdTargetSearchShutter;
	// 	m_vdObjectSearchShutter = lPTInfo.m_vdObjectSearchShutter;
	// 	m_vdTarObjSepCalibSearchShutter = lPTInfo.m_vdTarObjSepCalibSearchShutter;
	//	m_vdInspectSearchShutter = lPTInfo.m_vdInspectSearchShutter;

	// 	m_bObjectSearchShutter= lPTInfo.m_bObjectSearchShutter;
	// 	m_bTargetSearchShutter= lPTInfo.m_bTargetSearchShutter;
	//	m_bInspectSearchShutter= lPTInfo.m_bInspectSearchShutter;

	m_vvdTargetSearchShutter = lPTInfo.m_vvdTargetSearchShutter;
	m_vvdObjectSearchShutter = lPTInfo.m_vvdObjectSearchShutter;
	m_vvdCalibTargetSearchShutter = lPTInfo.m_vvdCalibTargetSearchShutter;
	m_vvdVirtualObjectSearchShutter = lPTInfo.m_vvdVirtualObjectSearchShutter;
	m_vvdInspectSearchShutter = lPTInfo.m_vvdInspectSearchShutter;

	m_vbTargetSearchShutter = lPTInfo.m_vbTargetSearchShutter;
	m_vbObjectSearchShutter = lPTInfo.m_vbObjectSearchShutter;
	m_vbCalibTargetSearchShutter = lPTInfo.m_vbCalibTargetSearchShutter;
	m_vbVirtualObjectSearchShutter = lPTInfo.m_vbVirtualObjectSearchShutter;
	m_vbInspectSearchShutter = lPTInfo.m_vbInspectSearchShutter;

	m_bEnableMutiTarget=lPTInfo.m_bEnableMutiTarget;
	m_bAccordingToPickPlatformEnable = lPTInfo.m_bAccordingToPickPlatformEnable;

	m_nMultiCalibExtensionMaxNum = lPTInfo.m_nMultiCalibExtensionMaxNum;

//	m_PCBInfo						= lPTInfo.m_PCBInfo;						// 设置PCB参数
	m_WeldInfo						= lPTInfo.m_WeldInfo;						// 焊接参数
	m_ProductPositionVirtualInfo	= lPTInfo.m_ProductPositionVirtualInfo;
//	m_BendInspectRangeInfo			= lPTInfo.m_BendInspectRangeInfo;
	m_BendInfo						= lPTInfo.m_BendInfo;
	m_AlignOffsetByArea				= lPTInfo.m_AlignOffsetByArea;
	m_cPhotoLightCtrl				=  lPTInfo.m_cPhotoLightCtrl;
	return *this;
}

void CAlignerParam::SetProductIndex( int nIndex )
{
	m_nAlignerIndex = nIndex;
}

int CAlignerParam::GetProductIndex()
{
	return m_nAlignerIndex;
}

void CAlignerParam::SetProductUserName( CString strUserName )
{
	m_strAlignerCustomName = strUserName;
}

// void CAlignerParam::SetProductUserName( BSTR bstrUserName )
// {
// 	m_strAlignerCustomName = (CString)bstrUserName;
// }


CString CAlignerParam::GetProductUserName()
{
	return m_strAlignerCustomName;
}

// BSTR CAlignerParam::GetProductUserName()
// {
// 	BSTR bstrName = m_strAlignerCustomName.AllocSysString();
// 	return bstrName;
// }


void CAlignerParam::SetAlignerPrecision( CCoordPos precision )
{
	m_AlignerPrecision = precision;
}

CCoordPos CAlignerParam::GetAlignerPrecision()
{
	return m_AlignerPrecision;
}

void CAlignerParam::SetAlignerMaxTime( int nTime )
{
	m_nAlignerMaxTime = nTime;
}

int CAlignerParam::GetAlignerMaxTime()
{
	return m_nAlignerMaxTime;
}

// 设置获取搜索延时（ms）
void CAlignerParam::SetSearchDelayTime(int nMS)
{
	m_nSearchDelayTime = nMS;
}

int  CAlignerParam::GetSearchDelayTime()
{
	return m_nSearchDelayTime;
}


// 设置获取搜索延时（ms）
void CAlignerParam::SetObjectSearchDelayTime(int nMS)
{
	m_nObjectSearchDelayTime = nMS;
}

int  CAlignerParam::GetObjectSearchDelayTime()
{
	return m_nObjectSearchDelayTime;
}

// 设置获取搜索延时2（ms）
void CAlignerParam::SetObjectSearchDelayTime2(int nMS)
{
	m_nObjectSearchDelayTime2 = nMS;
}

int  CAlignerParam::GetObjectSearchDelayTime2()
{
	return m_nObjectSearchDelayTime2;
}

void CAlignerParam::SetEnableObjectSearchDelayTime2(BOOL bEnableObjectSearchDelayTime2)
{
	m_bEnableObjectSearchDelayTime2 = bEnableObjectSearchDelayTime2;
}

BOOL  CAlignerParam::GetEnableObjectSearchDelayTime2()
{
	return m_bEnableObjectSearchDelayTime2;
}

// 设置2Vs2的对位模式类型
void CAlignerParam::SetAligner2Vs2Type(Aligner2Vs2Type type)
{
	m_eAligner2Vs2Type = type;
}
// 获取2Vs2的对位模式类型
Aligner2Vs2Type CAlignerParam::GetAligner2Vs2Type()
{
	return m_eAligner2Vs2Type;
}


// 设置3Vs3的对位模式类型
void CAlignerParam::SetAligner3Vs3Type(Aligner3Vs3Type type)
{
	m_eAligner3Vs3Type = type;
}
// 获取3Vs3的对位模式类型
Aligner3Vs3Type CAlignerParam::GetAligner3Vs3Type()
{
	return m_eAligner3Vs3Type;
}

// 设置4Vs4的对位模式类型
void CAlignerParam::SetAligner4Vs4Type(Aligner4Vs4Type type)
{
	m_eAligner4Vs4Type = type;
}
// 获取4Vs4的对位模式类型
Aligner4Vs4Type CAlignerParam::GetAligner4Vs4Type()
{
	return m_eAligner4Vs4Type;
}

// 设置5Vs5的对位模式类型
void CAlignerParam::SetAligner5Vs5Type(Aligner5Vs5Type type)	
{
	m_eAligner5Vs5Type = type;
}
// 获取5Vs5的对位模式类型
Aligner5Vs5Type CAlignerParam::GetAligner5Vs5Type()	
{
	return m_eAligner5Vs5Type;
}

// 设置6Vs6的对位模式类型
void CAlignerParam::SetAligner6Vs6Type(Aligner6Vs6Type type)
{
	m_eAligner6Vs6Type = type;
}
// 获取6Vs6的对位模式类型
Aligner6Vs6Type CAlignerParam::GetAligner6Vs6Type()		
{
	return m_eAligner6Vs6Type;
}
// 设置对位移动类型
void CAlignerParam::SetAlignerMoveType(AlignerMoveType type)
{
	m_eAlignerMoveType = type;
}

// 设置对位比例权重系数
void CAlignerParam::SetAlignerWeight(double dWeight)
{
	m_dAlignerWeight = dWeight;
}
double CAlignerParam::GetAlignerWeight()
{
	return m_dAlignerWeight;
}

void CAlignerParam::SetAlignerCirclePos(int nPos)
{
	m_nAlignerCirclePos = nPos;
}

int CAlignerParam::GetAlignerCirclePos()
{
	return m_nAlignerCirclePos;
}

// 获取对位移动类型
AlignerMoveType CAlignerParam::GetAlignerMoveType()
{
	return m_eAlignerMoveType;
}

// 对位角度设置（目标、对象角度相互垂直）
void CAlignerParam::SetEnableAlignAngleVectical(BOOL bAlignAngleVectical)
{
	m_bAlignAngleVertical = bAlignAngleVectical;
}

BOOL  CAlignerParam::GetEnableAlignAngleVectical()
{
	return m_bAlignAngleVertical;
}

// 设置对位角度垂直模式
void CAlignerParam::SetAlignAngleVecticalMode(int nAlignAngleVecticalMode)
{
	m_nAlignAngleVerticalMode = nAlignAngleVecticalMode;
}

// 获取对位角度垂直模式
int CAlignerParam::GetAlignAngleVecticalMode()
{
	return m_nAlignAngleVerticalMode;
}

//八字对位模式设置
void CAlignerParam::EnableEightShapeAlign(BOOL bEnable)
{
	m_bCheckEightShapeAlign = bEnable;
}
BOOL CAlignerParam::GetEnableEightShapeAlign()
{
	return m_bCheckEightShapeAlign;
}
//多Mark点模式设置
void CAlignerParam::EnableMulMarkMode(BOOL bEnable)
{
	m_bDiffPosWithSameSearchResultEnabled = bEnable;
}
BOOL CAlignerParam::GetMulMarkMode()
{
	return m_bDiffPosWithSameSearchResultEnabled;
}
void CAlignerParam::SetEightShapeAlignMode(int nMode)
{
	m_nEightShapeAlignMode = nMode;
}
int CAlignerParam::GetEightShapeAlignMode()
{
	return m_nEightShapeAlignMode;
}

void CAlignerParam::SetEightShapeAngle(double dAngle)
{
	m_dEightShapeAngle = dAngle;
}
double CAlignerParam::GetEightShapeAngle()
{
	return m_dEightShapeAngle;
}

void CAlignerParam::EnableAlnOffsetReverseX(BOOL bEnable)
{
	m_bAlignOffsetReverseX = bEnable;
}
void CAlignerParam::EnableAlnOffsetReverseY(BOOL bEnable)
{
	m_bAlignOffsetReverseY = bEnable;
}
void CAlignerParam::EnableAlnOffsetReverseD(BOOL bEnable)
{
	m_bAlignOffsetReverseD = bEnable;
}

BOOL CAlignerParam::GetEnableAlnOffsetReverseX()
{
	return m_bAlignOffsetReverseX;
}
BOOL CAlignerParam::GetEnableAlnOffsetReverseY()
{
	return m_bAlignOffsetReverseY;
}
BOOL CAlignerParam::GetEnableAlnOffsetReverseD()
{
	return m_bAlignOffsetReverseD;
}

void CAlignerParam::EnableAlnTargetFixOffset(BOOL bEnable)
{
	m_bEnableAlnTargetFixOffset = bEnable;
}
BOOL CAlignerParam::GetEnableAlnTargetFixOffset()
{
	return m_bEnableAlnTargetFixOffset;
}

void CAlignerParam::SetTargetOffsetType(int nOffsetType)
{
	m_nTargetFixOffsetType = nOffsetType;
}

int CAlignerParam::GetTargetOffsetType()
{
	return m_nTargetFixOffsetType;
}


bool CAlignerParam::SetAlnTargetFixOffset(int nIndex, CCoordPos Offset)
{
	if (nIndex < 0 || nIndex > m_vAlnTargetFixOffset.size() - 1)
	{
		return false;
	}
	m_vAlnTargetFixOffset.at(nIndex) = Offset;

	return true;
}

// 设置、获取对位补偿值
CCoordPos CAlignerParam::GetAlnTargetFixOffset(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vAlnTargetFixOffset.size() - 1)
	{
		CCoordPos offset;
		return offset;
	}

	return m_vAlnTargetFixOffset.at(nIndex);
}
CCoordPos CAlignerParam::GetCurTargetFixOffset()
{
	//return m_AlignerOffset;
	return GetAlnTargetFixOffset(GetCurAlignerTarFixOffsetIndex());
}

//多次对位目标补偿
void CAlignerParam::EnableAlnTargetOffset(BOOL bEnable)
{
	m_bCheckAlnTargetOffsetEnable = bEnable;
}
BOOL CAlignerParam::GetEnableAlnTargetOffset()
{
	return m_bCheckAlnTargetOffsetEnable;
}
double CAlignerParam::GetAlnTargetOffsetX()
{
	return m_dAlnTargetOffsetX;
}
void CAlignerParam::SetAlnTargetOffsetX(double doffsetX)
{
	m_dAlnTargetOffsetX = doffsetX;
}
double CAlignerParam::GetAlnTargetOffsetY()
{
	return m_dAlnTargetOffsetY;
}
void CAlignerParam::SetAlnTargetOffsetY(double doffsetY)
{
	m_dAlnTargetOffsetY = doffsetY;
}
void CAlignerParam::EnableAlnSendTargetOffsetToPLC(BOOL bEnable)
{
	m_bCheckAlnSendTargetOffsetToPLCEnable = bEnable;
}
BOOL CAlignerParam::GetEnableAlnSendTargetOffsetToPLC()
{
	return m_bCheckAlnSendTargetOffsetToPLCEnable;
}

void CAlignerParam::SetAlignerOffsetCoordType( AlignerOffsetCoordType type )
{
	m_eAlignerOffsetCoordType = type;
}

AlignerOffsetCoordType CAlignerParam::GetAlignerOffsetCoordType()
{
	return m_eAlignerOffsetCoordType;
}

void CAlignerParam::SetAlignerOffset( CCoordPos Offset )
{
	//m_AlignerOffset = Offset;
	SetAlignerOffset(GetCurAlignerOffsetIndex(), Offset);
}

CCoordPos CAlignerParam::GetAlignerOffset()
{
	//return m_AlignerOffset;
	return GetAlignerOffset(GetCurAlignerOffsetIndex());
}


bool CAlignerParam::SetAlignerOffsetTip(int nIndex, CString str)
{
	if (nIndex < 0 || nIndex > m_vAlignerOffsetTips.size() - 1)
	{
		return false;
	}
	m_vAlignerOffsetTips.at(nIndex) = str;

	return true;
}

CString CAlignerParam::GetAlignerOffsetTip(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vAlignerOffsetTips.size() - 1)
	{
		return _T("------");
	}
	return m_vAlignerOffsetTips.at(nIndex);

}

// 设置、获取对位基础补偿值
void CAlignerParam::SetAlignerBaseOffset( CCoordPos Offset )
{
	m_AlignerBaseOffset = Offset;
}

CCoordPos CAlignerParam::GetAlignerBaseOffset()
{
	return m_AlignerBaseOffset;
}


// 设置、获取对位补偿值的数量
bool CAlignerParam::SetAlignerOffsetNum(int nNum)
{
	if (nNum <= 0)
	{
		return false;
	}

	m_nAlignerOffsetNum = nNum;
	m_vAlignerOffset.resize(nNum);

	return true;
}
int CAlignerParam::GetAlignerOffsetNum()
{
	return m_nAlignerOffsetNum;
}
// 设置、获取对位补偿值
bool CAlignerParam::SetAlignerOffset(int nIndex, CCoordPos Offset)
{
	if (nIndex < 0 || nIndex > m_vAlignerOffset.size() - 1)
	{
		return false;
	}
	m_vAlignerOffset.at(nIndex) = Offset;

	return true;
}

// 设置、获取对位补偿值
CCoordPos CAlignerParam::GetAlignerOffset(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vAlignerOffset.size() - 1)
	{
		CCoordPos offset;
		return offset;
	}

	return m_vAlignerOffset.at(nIndex);
}

// 设置、获得当前使用的对位补偿值
bool CAlignerParam::SetCurAlignerOffsetIndex(int nIndex)
{
	if (nIndex < -1 || nIndex > m_vAlignerOffset.size() - 1)
	{
		return false;
	}
	m_nCurAlignerOffsetIndex = nIndex;

	return true;
}

int	 CAlignerParam::GetCurAlignerOffsetIndex()
{
	return m_nCurAlignerOffsetIndex;
}

bool CAlignerParam::SetCurAlignerTarFixOffsetIndex(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vAlnTargetFixOffset.size() - 1)
	{
		return false;
	}
	m_nCurAlignerTarFixOffsetIndex = nIndex;

	return true;
}
int	 CAlignerParam::GetCurAlignerTarFixOffsetIndex()
{
	return m_nCurAlignerTarFixOffsetIndex;
}

// 设置、获取对位补偿值的数量
bool CAlignerParam::SetAlignerBenchOffsetNum(int nNum)
{
	if (nNum <= 0)
	{
		return false;
	}

	m_nAlignerBenchOffsetNum = nNum;
	m_vAlignerBenchOffset.resize(nNum);

	return true;
}
int CAlignerParam::GetAlignerBenchOffsetNum()
{
	return m_nAlignerBenchOffsetNum;
}
// 设置、获取对位补偿值
bool CAlignerParam::SetAlignerBenchOffset(int nIndex, CCoordPos BenchOffset)
{
	if (nIndex < 0 || nIndex > m_vAlignerBenchOffset.size() - 1)
	{
		return false;
	}
	m_vAlignerBenchOffset.at(nIndex) = BenchOffset;

	return true;
}

// 设置、获取对位补偿值
CCoordPos CAlignerParam::GetAlignerBenchOffset(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vAlignerBenchOffset.size() - 1)
	{
		CCoordPos BenchOffset;
		return BenchOffset;
	}

	return m_vAlignerBenchOffset.at(nIndex);
}

// 设置、获得当前使用的对位补偿值
bool CAlignerParam::SetCurAlignerBenchOffsetIndex(int nIndex)
{
	if (nIndex < -1 || nIndex > m_vAlignerBenchOffset.size() - 1)
	{
		return false;
	}
	m_nCurAlignerBenchOffsetIndex = nIndex;

	return true;
}

int	 CAlignerParam::GetCurAlignerBenchOffsetIndex()
{
	return m_nCurAlignerBenchOffsetIndex;
}

CCoordPos  CAlignerParam::GetAlignerBenchOffset()
{
	return GetAlignerBenchOffset(GetCurAlignerBenchOffsetIndex());
}


// void CAlignerParam::SetAlignerMode( AlignerMode mode )
// {
// 	m_eAlignerMode = mode;
// }
// 
// AlignerMode CAlignerParam::GetAlignerMode()
// {
// 	return m_eAlignerMode;
// }

void CAlignerParam::SetTargetMarkType(AlignerTargetType type)
{
	m_eTargetMarkType = type;
}

AlignerTargetType CAlignerParam::GetTargetMarkType()
{
	return m_eTargetMarkType;
}

// 对象Mark 
// 设置、获取对象类型
void CAlignerParam::SetObjectMarkType(AlignerObjectType type)
{
	m_eObjectMarkType = type;
}		

AlignerObjectType CAlignerParam::GetObjectMarkType()
{
	return m_eObjectMarkType;
}

void CAlignerParam::SetTargetMarkPatternNum( int nNum )
{
	if (nNum < 0)
	{
		return;
	}
	m_vstrTargetMarkPatternIDs.resize(nNum);
	m_nTargetMarkPatternNum = nNum;

	CPosPatInfo info;
	info.m_nPatNum = TARGETCANDIDATENUM;
	info.m_vdShutter.resize(info.m_nPatNum, 0.01);
	m_vTargetPosPatInfo.resize(nNum, info);
}

int CAlignerParam::GetTargetMarkPatternNum()
{
	m_nTargetMarkPatternNum = m_vstrTargetMarkPatternIDs.size();
	return m_nTargetMarkPatternNum;
}

BOOL CAlignerParam::SetTargetMarkPatternID( int nIndex, CString strID )
{
	ASSERT((nIndex > 0) && (nIndex <= m_vstrTargetMarkPatternIDs.size()));
	if ((nIndex < 1) || (nIndex > m_vstrTargetMarkPatternIDs.size()))
	{
		return FALSE;
	}
	else
	{
		m_vstrTargetMarkPatternIDs.at(nIndex) = strID;
		return TRUE;
	}
}

CString CAlignerParam::GetTargetMarkPatternID( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vstrTargetMarkPatternIDs.size()));
	if ((nIndex < 0) || (nIndex >= m_vstrTargetMarkPatternIDs.size()))
	{
		return _T("");
	}
	else
	{
		return m_vstrTargetMarkPatternIDs.at(nIndex);
	}
}

// 候补目标Mark

// 设置是否开启候补目标Mark搜索
void CAlignerParam::SetTargetCandidateEnable(BOOL bEnable)
{
	m_bTargetCandidateEnable = bEnable;
}

BOOL CAlignerParam::GetTargetCandidateEnable()
{
	return m_bTargetCandidateEnable;
}

// 设置是否开启候补目标Mark成对搜索
void CAlignerParam::SetTargetCandidateSyncEnable(BOOL bEnable)
{
	m_bTargetCandidateSync = bEnable;
}

BOOL CAlignerParam::GetTargetCandidateSyncEnable()
{
	return m_bTargetCandidateSync;
}


// 设置、获取目标候补补偿值的数量
BOOL CAlignerParam::SetTargetCandidateOffsetNum(int nNum)
{
	if (nNum <= 0)
	{
		return false;
	}

	m_nTargetCandidateOffsetNum = nNum;
	m_vTargetCandidateOffset.resize(nNum);
	m_vTargetCandidateOffsetEnable.resize(nNum,FALSE);

	return true;
}

int  CAlignerParam::GetTargetCandidateOffsetNum()
{
	return m_nTargetCandidateOffsetNum;
}


// 设置、获取目标候补补偿值
BOOL CAlignerParam::SetTargetCandidateOffset(int nIndex, CCoordPos Offset)
{
	if (nIndex < 0 || nIndex > m_vTargetCandidateOffset.size() - 1)
	{
		return false;
	}
	m_vTargetCandidateOffset.at(nIndex) = Offset;

	return true;

}

CCoordPos CAlignerParam::GetTargetCandidateOffset(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vTargetCandidateOffset.size() - 1)
	{
		CCoordPos offset;
		return offset;
	}

	return m_vTargetCandidateOffset.at(nIndex);
}

// 设置、获取目标候补补偿值是否有效
BOOL CAlignerParam::SetTargetCandidateOffsetEnable(int nIndex, BOOL bEnable)
{
	if (nIndex < 0 || nIndex > m_vTargetCandidateOffsetEnable.size() - 1)
	{
		return false;
	}
	m_vTargetCandidateOffsetEnable.at(nIndex) = bEnable;

	return true;
}

BOOL CAlignerParam::GetTargetCandidateOffsetEnable(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vTargetCandidateOffsetEnable.size() - 1)
	{
		return false;
	}

	return m_vTargetCandidateOffsetEnable.at(nIndex);
}

// 设置、获取目标各位置模板信息
BOOL CAlignerParam::SetTargetPosPatInfo(int nPosIndex, CPosPatInfo info)
{
	if (nPosIndex < 0 || nPosIndex > m_vTargetPosPatInfo.size() - 1)
	{
		return false;
	}
	m_vTargetPosPatInfo.at(nPosIndex) = info;

	return true;
}

CPosPatInfo CAlignerParam::GetTargetPosPatInfo(int nPosIndex)
{
	if (nPosIndex < 0 || nPosIndex > m_vTargetPosPatInfo.size() - 1)
	{
		CPosPatInfo info;
		return info;
	}

	return m_vTargetPosPatInfo.at(nPosIndex);
}


// 设置、获取对象各位置模板信息
BOOL CAlignerParam::SetObjectPosPatInfo(int nPosIndex, CPosPatInfo info)
{
	if (nPosIndex < 0 || nPosIndex > m_vObjectPosPatInfo.size() - 1)
	{
		return false;
	}
	m_vObjectPosPatInfo.at(nPosIndex) = info;

	return true;
}

CPosPatInfo CAlignerParam::GetObjectPosPatInfo(int nPosIndex)
{
	if (nPosIndex < 0 || nPosIndex > m_vObjectPosPatInfo.size() - 1)
	{
		CPosPatInfo info;
		return info;
	}

	return m_vObjectPosPatInfo.at(nPosIndex);
}


// 候补对象Mark

// 设置是否开启候补对象Mark搜索
void CAlignerParam::SetObjectCandidateEnable(BOOL bEnable)
{
	m_bObjectCandidateEnable = bEnable;
}

BOOL CAlignerParam::GetObjectCandidateEnable()
{
	return m_bObjectCandidateEnable;
}

// 设置是否开启候补对象Mark成对搜索
void CAlignerParam::SetObjectCandidateSyncEnable(BOOL bEnable)
{
	m_bObjectCandidateSync = bEnable;
}

BOOL CAlignerParam::GetObjectCandidateSyncEnable()
{
	return m_bObjectCandidateSync;
}


// 设置、获取对象候补补偿值的数量
BOOL CAlignerParam::SetObjectCandidateOffsetNum(int nNum)
{
	if (nNum <= 0)
	{
		return false;
	}

	m_nObjectCandidateOffsetNum = nNum;
	m_vObjectCandidateOffset.resize(nNum);
	m_vObjectCandidateOffsetEnable.resize(nNum,FALSE);

	return true;
}

int  CAlignerParam::GetObjectCandidateOffsetNum()
{
	return m_nObjectCandidateOffsetNum;
}


// 设置、获取对象候补补偿值
BOOL CAlignerParam::SetObjectCandidateOffset(int nIndex, CCoordPos Offset)
{
	if (nIndex < 0 || nIndex > m_vObjectCandidateOffset.size() - 1)
	{
		return false;
	}
	m_vObjectCandidateOffset.at(nIndex) = Offset;

	return true;
}

CCoordPos CAlignerParam::GetObjectCandidateOffset(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vObjectCandidateOffset.size() - 1)
	{
		CCoordPos offset;
		return offset;
	}

	return m_vObjectCandidateOffset.at(nIndex);
}



// 设置、获取对象候补补偿值是否有效
BOOL CAlignerParam::SetObjectCandidateOffsetEnable(int nIndex, BOOL bEnable)
{
	if (nIndex < 0 || nIndex > m_vObjectCandidateOffsetEnable.size() - 1)
	{
		return false;
	}
	m_vObjectCandidateOffsetEnable.at(nIndex) = bEnable;

	return true;
}

BOOL CAlignerParam::GetObjectCandidateOffsetEnable(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vObjectCandidateOffsetEnable.size() - 1)
	{
		return false;
	}

	return m_vObjectCandidateOffsetEnable.at(nIndex);
}



void CAlignerParam::SetBenchTargetMarkFixEnable( BOOL bEnable )
{
	m_bBenchTargetMarkFixEnable = bEnable;
}

BOOL CAlignerParam::GetBenchTargetMarkFixEnable()
{
	return m_bBenchTargetMarkFixEnable;
}

BOOL CAlignerParam::SetBenchTargetMarkFixInfo( const CBenchTargetMarkFixInfo* pInfo )
{
	if (m_pBenchTargetMarkFixInfo != NULL)
	{
		delete m_pBenchTargetMarkFixInfo;
		m_pBenchTargetMarkFixInfo = NULL;
	}

	if (pInfo != NULL)
	{
		m_pBenchTargetMarkFixInfo = new CBenchTargetMarkFixInfo(*pInfo);
	}

	return TRUE;
}

CBenchTargetMarkFixInfo* CAlignerParam::GetBenchTargetMarkFixInfo()
{
	return m_pBenchTargetMarkFixInfo;
}



void CAlignerParam::SetBenchTargetMarkPickEnable( BOOL bEnable )
{
	m_bBenchTargetMarkPickEnable = bEnable;
}

BOOL CAlignerParam::GetBenchTargetMarkPickEnable()
{
	return m_bBenchTargetMarkPickEnable;
}

BOOL CAlignerParam::SetBenchTargetMarkPickInfo( const CBenchTargetMarkPickInfo* pInfo )
{
	if (m_pBenchTargetMarkPickInfo != NULL)
	{
		delete m_pBenchTargetMarkPickInfo;
		m_pBenchTargetMarkPickInfo = NULL;
	}

	if (pInfo != NULL)
	{
		m_pBenchTargetMarkPickInfo = new CBenchTargetMarkPickInfo(*pInfo);
	}

	return TRUE;
}

CBenchTargetMarkPickInfo* CAlignerParam::GetBenchTargetMarkPickInfo()
{
	return m_pBenchTargetMarkPickInfo;
}

void CAlignerParam::SetObjectMarkPatternNum( int nNum )
{
	if (nNum < 0)
	{
		return;
	}
	m_vstrObjectMarkPatternIDs.resize(nNum);
	m_nObjectMarkPatternNum = nNum;

	CPosPatInfo info;
	info.m_nPatNum = OBJECTCANDIDATENUM;
	info.m_vdShutter.resize(info.m_nPatNum, 0.01);
	m_vObjectPosPatInfo.resize(nNum, info);
}

int CAlignerParam::GetObjectMarkPatternNum()
{
	m_nObjectMarkPatternNum = m_vstrObjectMarkPatternIDs.size();
	return m_nObjectMarkPatternNum;
}

BOOL CAlignerParam::SetObjectMarkPatternID( int nIndex, CString strID )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vstrObjectMarkPatternIDs.size()));
	if ((nIndex < 0) || (nIndex >= m_vstrObjectMarkPatternIDs.size()))
	{
		return FALSE;
	}
	else
	{
		m_vstrObjectMarkPatternIDs.at(nIndex) = strID;
		return TRUE;
	}
}

CString CAlignerParam::GetObjectMarkPatternID( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vstrObjectMarkPatternIDs.size()));
	if ((nIndex < 0) || (nIndex >= m_vstrObjectMarkPatternIDs.size()))
	{
		return _T("");
	}
	else
	{
		return m_vstrObjectMarkPatternIDs.at(nIndex);
	}
}

void CAlignerParam::SetVirtualObjectMarkEnable( BOOL bEnable )
{
	m_bVirtualObjectMarkEnable = bEnable;
}

BOOL CAlignerParam::GetVirtualObjectMarkEnable()
{
	return m_bVirtualObjectMarkEnable;
}

void CAlignerParam::SetVirtualObjectMarkPatternNum( int nNum )
{
	if (nNum < 0)
	{
		return;
	}
	m_vstrVirtualMarkPatternIDs.resize(nNum);
	m_vObjectVirtualMarkOffset.resize(nNum);
	m_nVirtualMarkPatternNum = nNum;
}

int CAlignerParam::GetVirtualObjectMarkPatternNum()
{
	m_nVirtualMarkPatternNum = m_vObjectVirtualMarkOffset.size();
	return m_nVirtualMarkPatternNum;
}

BOOL CAlignerParam::SetVirtualObjectMarkPatternID( int nIndex, CString strID )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vstrVirtualMarkPatternIDs.size()));
	if ((nIndex < 0) || (nIndex >= m_vstrVirtualMarkPatternIDs.size()))
	{
		return FALSE;
	}
	else
	{
		m_vstrVirtualMarkPatternIDs.at(nIndex) = strID;
		return TRUE;
	}
}

CString CAlignerParam::GetVirtualObjectMarkPatternID( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vstrVirtualMarkPatternIDs.size()));
	if ((nIndex < 0) || (nIndex >= m_vstrVirtualMarkPatternIDs.size()))
	{
		return _T("");
	}
	else
	{
		return m_vstrVirtualMarkPatternIDs.at(nIndex);
	}
}

BOOL CAlignerParam::SetVirtualObjectMarkOffset( int nIndex, CCoordPos offset )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vObjectVirtualMarkOffset.size()));
	if ((nIndex < 0) || (nIndex >= m_vObjectVirtualMarkOffset.size()))
	{
		return FALSE;
	}
	else
	{
		m_vObjectVirtualMarkOffset.at(nIndex) = offset;
		return TRUE;
	}
}

CCoordPos CAlignerParam::GetVirtualObjectMarkOffset( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vObjectVirtualMarkOffset.size()));
	if ((nIndex < 0) || (nIndex >= m_vObjectVirtualMarkOffset.size()))
	{
		CCoordPos pos;
		return pos;
	}
	else
	{
		return m_vObjectVirtualMarkOffset.at(nIndex);
	}
}

// 设置、获取目标相机和对象相机是否各自独立的
void CAlignerParam::SetTargetObjectCamSeparateEnable(BOOL bEnable)
{
	m_bTargetObjectCamSeparate = bEnable;
}
BOOL CAlignerParam::GetTargetObjectCamSeparateEnable()
{
	return m_bTargetObjectCamSeparate;
}

// 设置是否启用标定板关联
void CAlignerParam::SetTargetCalibUseBoardEnable(BOOL bEnable)
{
	m_bTargetCalibUseBoard = bEnable;
}
BOOL CAlignerParam::GetTargetCalibUseBoardEnable()
{
	return m_bTargetCalibUseBoard;
}

// void CAlignerParam::SetTargetUseFixedVirtualCoordinates(BOOL bEnable)
// {
// 	m_bTargetUseFixedVirtualCoordinates = bEnable;
// }
// BOOL CAlignerParam::GetTargetUseFixedVirtualCoordinates()
// {
// 	return m_bTargetUseFixedVirtualCoordinates;
// }

// 设置、获取是否使用关联标定Mark
void CAlignerParam::SetCalibTargetMarkEnable( BOOL bEnable )
{
	m_bCalibTargetMarkEnable = bEnable;
}

BOOL CAlignerParam::GetCalibTargetMarkEnable()
{
	return m_bCalibTargetMarkEnable;
}


// 设置、获取关联标定模板类型
void CAlignerParam::SetCalibTargetMarkPatternMode(CalibTargetMarkPatternMode mode)
{
	m_eCalibTargetMarkPatternMode = mode;
}
CalibTargetMarkPatternMode CAlignerParam::GetCalibTargetMarkPatternMode()
{
	return m_eCalibTargetMarkPatternMode;
}

void CAlignerParam::SetCalibTargetDMCodeOutLineEnable(BOOL bEnable) // 设置、获取二维码关联时是否忽略点
{
	m_bEnableCalibTargetDMCodeOutLine = bEnable;
}

void CAlignerParam::SetCalibTargetDMCodeOutLinePercent(double dVal)// 设置、获取二维码关联时是否忽略点百分比
{
	m_dCalibTargetDMCodeOutLinePercent = dVal;
}

void CAlignerParam::SetCalibTargetMarkPatternNum( int nNum )
{
	if (nNum < 0)
	{
		return;
	}
	m_vstrCalibTargetMarkPatternIDs.resize(nNum);
	m_nCalibTargetMarkPatternNum = nNum;
}

int CAlignerParam::GetCalibTargetMarkPatternNum()
{
	m_nCalibTargetMarkPatternNum = m_vstrCalibTargetMarkPatternIDs.size();
	return m_nCalibTargetMarkPatternNum;
}

BOOL CAlignerParam::SetCalibTargetMarkPatternID( int nIndex, CString strID )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vstrCalibTargetMarkPatternIDs.size()));
	if ((nIndex < 0) || (nIndex >= m_vstrCalibTargetMarkPatternIDs.size()))
	{
		return FALSE;
	}
	else
	{
		m_vstrCalibTargetMarkPatternIDs.at(nIndex) = strID;
		return TRUE;
	}
}

CString CAlignerParam::GetCalibTargetMarkPatternID( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vstrCalibTargetMarkPatternIDs.size()));
	if ((nIndex < 0) || (nIndex >= m_vstrCalibTargetMarkPatternIDs.size()))
	{
		return _T("");
	}
	else
	{
		return m_vstrCalibTargetMarkPatternIDs.at(nIndex);
	}
}

// 检测模板设置
// 设置、获取检测模板是否启用(系统平台信息配置选项)
void CAlignerParam::SetSysPlatformInspectEnable(BOOL bEnable)
{
	m_bSysPlatformInspectEnable = bEnable;
}
BOOL CAlignerParam::GetSysPlatformInspectEnable()
{
	return m_bSysPlatformInspectEnable;
}
// 
// // 设置、获取检测模板是否启用(软件界面选项)
// void CAlignerParam::SetAlignInspectEnable( BOOL bEnable )
// {
// 	m_bAlignInspectEnable = bEnable;
// }
// 
// BOOL CAlignerParam::GetAlignInspectEnable()
// {
// 	return m_bAlignInspectEnable;
// }

// 设置是否开启候补检测Mark搜索
void CAlignerParam::SetInspectCandidateEnable(BOOL bEnable)
{
	m_bInspectCandidateEnable = bEnable;
}

BOOL CAlignerParam::GetInspectCandidateEnable()
{
	return m_bInspectCandidateEnable;
}

// 设置是否开启候补检测Mark成对搜索
void CAlignerParam::SetInspectCandidateSyncEnable(BOOL bEnable)
{
	m_bInspectCandidateSync = bEnable;
}

BOOL CAlignerParam::GetInspectCandidateSyncEnable()
{
	return m_bInspectCandidateSync;
}

// 设置是否开启候补检测Mark单独搜索
void CAlignerParam::SetInspectSingleSearchEnable(BOOL bEnable)
{
	m_bInspectSingleSearch = bEnable;
}

BOOL CAlignerParam::GetInspectSingleSearchEnable()
{
	return m_bInspectSingleSearch;
}

// 设置、获取检测Mark模板的数量
void CAlignerParam::SetInspectMarkPatternNum( int nNum )
{
	if (nNum < 0)
	{
		return;
	}
	m_vstrInspectMarkPatternIDs.resize(nNum);
	m_nInspectMarkPatternNum = nNum;
}

int CAlignerParam::GetInspectMarkPatternNum()
{
	m_nInspectMarkPatternNum = m_vstrInspectMarkPatternIDs.size();
	return m_nInspectMarkPatternNum;
}


// 设置、获取检测Mark模板的ID
BOOL CAlignerParam::SetInspectMarkPatternID( int nIndex, CString strID )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vstrInspectMarkPatternIDs.size()));

	if ((nIndex < 0) || (nIndex >= m_vstrInspectMarkPatternIDs.size()))
	{
		return FALSE;
	}
	else
	{
		m_vstrInspectMarkPatternIDs.at(nIndex) = strID;
		return TRUE;
	}
}

CString CAlignerParam::GetInspectMarkPatternID( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vstrInspectMarkPatternIDs.size()));

	if ((nIndex < 0) || (nIndex >= m_vstrInspectMarkPatternIDs.size()))
	{
		return _T("");
	}
	else
	{
		return m_vstrInspectMarkPatternIDs.at(nIndex);
	}
}


// 设置、获取检测候补补偿值的数量
BOOL CAlignerParam::SetInspectCandidateOffsetNum(int nNum)
{
	if (nNum <= 0)
	{
		return false;
	}

	m_nInspectCandidateOffsetNum = nNum;
	m_vInspectCandidateOffsetEnable.resize(nNum,FALSE);

	return true;
}

int  CAlignerParam::GetInspectCandidateOffsetNum()
{
	return m_nInspectCandidateOffsetNum;
}


// 设置、获取检测候补补偿值是否有效
BOOL CAlignerParam::SetInspectCandidateOffsetEnable(int nIndex, BOOL bEnable)
{
	if (nIndex < 0 || nIndex > m_vInspectCandidateOffsetEnable.size() - 1)
	{
		return false;
	}
	m_vInspectCandidateOffsetEnable.at(nIndex) = bEnable;

	return true;
}

BOOL CAlignerParam::GetInspectCandidateOffsetEnable(int nIndex)
{
	if (nIndex < 0 || nIndex > m_vInspectCandidateOffsetEnable.size() - 1)
	{
		return false;
	}

	return m_vInspectCandidateOffsetEnable.at(nIndex);
}


//// 设置、获取检测信息
//void CAlignerParam::SetAlignCheckInfo(CAlignCheckInfo checkInfo)
//{
//	m_AlignCheckInfo = checkInfo;
//}
//CAlignCheckInfo CAlignerParam::GetAlignCheckInfo()
//{
//	return m_AlignCheckInfo;
//}

//20160510增加对角补偿
// 设置、获取产品尺寸信息
void CAlignerParam::SetProductSizeInfo(CProductSizeInfo productSizeInfo)
{
	m_ProductSizeInfo = productSizeInfo;
}

CProductSizeInfo CAlignerParam::GetProductSizeInfo()
{
	return m_ProductSizeInfo;
}

//20160510增加对角补偿
// 设置、获取产品拍照位置信息
void CAlignerParam::SetPositionModeInfo(int nPosMode)
{
	m_nPosMode = nPosMode;
}

int CAlignerParam::GetPositionModeInfo()
{
	return m_nPosMode;
}

// 补偿设置（补偿的方向、X和Y补偿值对调）
void CAlignerParam::SetProductOffsetInfo(CProductOffsetInfo productOffsetInfo)
{
	m_ProductOffsetInfo = productOffsetInfo;
}

CProductOffsetInfo CAlignerParam::GetProductOffsetInfo()
{
	return m_ProductOffsetInfo;
}


// 设置PCB参数
// void CAlignerParam::SetPCBInfo(CPCBInfo PCBInfo)
// {
// 	m_PCBInfo = PCBInfo;
// }
// 
// CPCBInfo CAlignerParam::GetPCBInfo()
// {
// 	return m_PCBInfo;
// }


// BOOL CAlignerParam::SetObjectSearchShutter( int nIndex, double dShutter )
// {
// 	if (nIndex >= m_vdObjectSearchShutter.size() || nIndex < 0)
// 	{
// 		return FALSE;
// 	}
// 
// 	m_vdObjectSearchShutter.at(nIndex) = dShutter;
// 
// 	return TRUE;
// }
// 
// double CAlignerParam::GetObjectSearchShutter( int nIndex )
// {
// 	if (nIndex >= m_vdObjectSearchShutter.size() || nIndex < 0)
// 	{
// 		return 0;
// 	}
// 
// 	return m_vdObjectSearchShutter.at(nIndex) ;
// }
// 
// BOOL CAlignerParam::SetTargetSearchShutter( int nIndex, double dShutter )
// {
// 	if (nIndex >= m_vdTargetSearchShutter.size() || nIndex < 0)
// 	{
// 		return FALSE;
// 	}
// 
// 	m_vdTargetSearchShutter.at(nIndex) = dShutter;
// 
// 	return TRUE;
// }
// 
// double CAlignerParam::GetTargetSearchShutter( int nIndex )
// {
// 	if (nIndex >= m_vdTargetSearchShutter.size() || nIndex < 0)
// 	{
// 		return 0;
// 	}
// 
// 	return m_vdTargetSearchShutter.at(nIndex) ;
// }
// 
// BOOL CAlignerParam::SetTarObjSepCalibSearchShutter( int nIndex, double dShutter )
// {
// 	if (nIndex >= m_vdTarObjSepCalibSearchShutter.size() || nIndex < 0)
// 	{
// 		return FALSE;
// 	}
// 
// 	m_vdTarObjSepCalibSearchShutter.at(nIndex) = dShutter;
// 
// 	return TRUE;
// }
// 
// double CAlignerParam::GetTarObjSepCalibSearchShutter( int nIndex )
// {
// 	if (nIndex >= m_vdTarObjSepCalibSearchShutter.size() || nIndex < 0)
// 	{
// 		return 0;
// 	}
// 
// 	return m_vdTarObjSepCalibSearchShutter.at(nIndex) ;
// }


// void CAlignerParam::SetEnableObjectSearchShutter(BOOL bEnable)
// {
// 	m_bObjectSearchShutter = bEnable;
// }
// 
// BOOL CAlignerParam::GetEnableObjectSearchShutter()
// {
// 	return m_bObjectSearchShutter;
// }
// 
// void CAlignerParam::SetEnableTargetSearchShutter(BOOL bEnable)
// {
// 	m_bTargetSearchShutter = bEnable;
// }
// 
// BOOL CAlignerParam::GetEnableTargetSearchShutter()
// {
// 	return m_bTargetSearchShutter;
// }




BOOL CAlignerParam::SetObjectSearchShutter( int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter )
{
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return FALSE;
	}

	if (nPatternIndex >= m_vvdObjectSearchShutter.size() || nCandidateIndex >= m_vvdObjectSearchShutter[nPatternIndex].size())
	{
		return FALSE;
	}

	m_vvdObjectSearchShutter[nPatternIndex][nCandidateIndex] = dShutter;

	return TRUE;
}

CameraParamSet CAlignerParam::GetObjectSearchShutter( int nPatternIndex, int nCandidateIndex )
{
	CameraParamSet tmpcameraParamSet;
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return tmpcameraParamSet;
	}

	if (nPatternIndex >= m_vvdObjectSearchShutter.size() || nCandidateIndex >= m_vvdObjectSearchShutter[nPatternIndex].size())
	{
		return tmpcameraParamSet;
	}

	return m_vvdObjectSearchShutter[nPatternIndex][nCandidateIndex] ;
}

BOOL CAlignerParam::SetTargetSearchShutter( int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter )
{
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return FALSE;
	}

	if (nPatternIndex >= m_vvdTargetSearchShutter.size() || nCandidateIndex >= m_vvdTargetSearchShutter[nPatternIndex].size())
	{
		return FALSE;
	}

	m_vvdTargetSearchShutter[nPatternIndex][nCandidateIndex] = dShutter;

	return TRUE;
}

CameraParamSet CAlignerParam::GetTargetSearchShutter( int nPatternIndex, int nCandidateIndex )
{
	CameraParamSet tmpcameraParamSet;
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return tmpcameraParamSet;
	}

	if (nPatternIndex >= m_vvdTargetSearchShutter.size() || nCandidateIndex >= m_vvdTargetSearchShutter[nPatternIndex].size())
	{
		return tmpcameraParamSet;
	}

	return m_vvdTargetSearchShutter[nPatternIndex][nCandidateIndex] ;
}


BOOL CAlignerParam::SetCalibTargetSearchShutter( int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter )
{
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return FALSE;
	}

	if (nPatternIndex >= m_vvdCalibTargetSearchShutter.size() || nCandidateIndex >= m_vvdCalibTargetSearchShutter[nPatternIndex].size())
	{
		return FALSE;
	}

	m_vvdCalibTargetSearchShutter[nPatternIndex][nCandidateIndex] = dShutter;

	return TRUE;
}

CameraParamSet CAlignerParam::GetCalibTargetSearchShutter( int nPatternIndex, int nCandidateIndex )
{
	CameraParamSet tmpcameraParamSet;
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return tmpcameraParamSet;
	}

	if (nPatternIndex >= m_vvdCalibTargetSearchShutter.size() || nCandidateIndex >= m_vvdCalibTargetSearchShutter[nPatternIndex].size())
	{
		return tmpcameraParamSet;
	}

	return m_vvdCalibTargetSearchShutter[nPatternIndex][nCandidateIndex] ;
}


BOOL CAlignerParam::SetVirtualObjectSearchShutter( int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter )
{
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return FALSE;
	}

	if (nPatternIndex >= m_vvdVirtualObjectSearchShutter.size() || nCandidateIndex >= m_vvdVirtualObjectSearchShutter[nPatternIndex].size())
	{
		return FALSE;
	}

	m_vvdVirtualObjectSearchShutter[nPatternIndex][nCandidateIndex] = dShutter;

	return TRUE;
}

CameraParamSet CAlignerParam::GetVirtualObjectSearchShutter( int nPatternIndex, int nCandidateIndex )
{
	CameraParamSet tmpcameraParamSet;
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return tmpcameraParamSet;
	}

	if (nPatternIndex >= m_vvdVirtualObjectSearchShutter.size() || nCandidateIndex >= m_vvdVirtualObjectSearchShutter[nPatternIndex].size())
	{
		return tmpcameraParamSet;
	}

	return m_vvdVirtualObjectSearchShutter[nPatternIndex][nCandidateIndex] ;
}

// 设置、获取检测模板曝光时间
BOOL CAlignerParam::SetInspectSearchShutter(int nPatternIndex, int nCandidateIndex, CameraParamSet dShutter)
{
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return FALSE;
	}

	if (nPatternIndex >= m_vvdInspectSearchShutter.size() || nCandidateIndex >= m_vvdInspectSearchShutter[nPatternIndex].size())
	{
		return FALSE;
	}

	m_vvdInspectSearchShutter[nPatternIndex][nCandidateIndex] = dShutter;

	return TRUE;
}

CameraParamSet CAlignerParam::GetInspectSearchShutter(int nPatternIndex, int nCandidateIndex)
{
	CameraParamSet tmpcameraParamSet;
	if (nPatternIndex < 0 || nCandidateIndex < 0)
	{
		return tmpcameraParamSet;
	}

	if (nPatternIndex >= m_vvdInspectSearchShutter.size() || nCandidateIndex >= m_vvdInspectSearchShutter[nPatternIndex].size())
	{
		return tmpcameraParamSet;
	}

	return m_vvdInspectSearchShutter[nPatternIndex][nCandidateIndex] ;
}



// 设置、获取对象候补曝光是否有效
BOOL CAlignerParam::SetEnableObjectSearchShutter(int nIndex, CameraParamEnable bEnable)
{
	if (nIndex < 0 || nIndex > m_vbObjectSearchShutter.size())
	{
		return false;
	}

	m_vbObjectSearchShutter.at(nIndex) = bEnable;

	return true;
}

CameraParamEnable CAlignerParam::GetEnableObjectSearchShutter(int nIndex)
{
	CameraParamEnable cameraParamEnable;
	if (nIndex < 0 || nIndex > m_vbObjectSearchShutter.size())
	{
		return cameraParamEnable;
	}

	return m_vbObjectSearchShutter.at(nIndex);

}


// 设置、获取目标候补曝光是否有效
BOOL CAlignerParam::SetEnableTargetSearchShutter(int nIndex, CameraParamEnable bEnable)
{
	if (nIndex < 0 || nIndex > m_vbTargetSearchShutter.size())
	{
		return false;
	}

	m_vbTargetSearchShutter.at(nIndex) = bEnable;

	return true;
}

CameraParamEnable CAlignerParam::GetEnableTargetSearchShutter(int nIndex)
{
	CameraParamEnable cameraParamEnable;
	if (nIndex < 0 || nIndex > m_vbTargetSearchShutter.size())
	{
		return cameraParamEnable;
	}

	return m_vbTargetSearchShutter.at(nIndex);

}


// 设置、获取目标候补曝光是否有效
BOOL CAlignerParam::SetEnableCalibTargetSearchShutter(int nIndex, CameraParamEnable bEnable)
{
	if (nIndex < 0 || nIndex > m_vbCalibTargetSearchShutter.size())
	{
		return false;
	}

	m_vbCalibTargetSearchShutter.at(nIndex) = bEnable;

	return true;
}

CameraParamEnable CAlignerParam::GetEnableCalibTargetSearchShutter(int nIndex)
{
	CameraParamEnable cameraParamEnable;
	if (nIndex < 0 || nIndex > m_vbCalibTargetSearchShutter.size())
	{
		return cameraParamEnable;
	}

	return m_vbCalibTargetSearchShutter.at(nIndex);

}


// 设置、获取曝光是否有效
BOOL CAlignerParam::SetEnableVirtualObjectSearchShutter(int nIndex, CameraParamEnable bEnable)
{
	if (nIndex < 0 || nIndex > m_vbVirtualObjectSearchShutter.size())
	{
		return false;
	}

	m_vbVirtualObjectSearchShutter.at(nIndex) = bEnable;

	return true;
}

CameraParamEnable CAlignerParam::GetEnableVirtualObjectSearchShutter(int nIndex)
{
	CameraParamEnable cameraParamEnable;
	if (nIndex < 0 || nIndex > m_vbVirtualObjectSearchShutter.size())
	{
		return cameraParamEnable;
	}

	return m_vbVirtualObjectSearchShutter.at(nIndex);

}

// 设置、获取检测候补补偿值是否有效
BOOL CAlignerParam::SetEnableInspectSearchShutter(int nIndex, CameraParamEnable bEnable)
{
	if (nIndex < 0 || nIndex > m_vbInspectSearchShutter.size())
	{
		return false;
	}

	m_vbInspectSearchShutter.at(nIndex) = bEnable;

	return true;
}

CameraParamEnable CAlignerParam::GetEnableInspectSearchShutter(int nIndex)
{
	CameraParamEnable cameraParamEnable;
	if (nIndex < 0 || nIndex > m_vbInspectSearchShutter.size())
	{
		return cameraParamEnable;
	}

	return m_vbInspectSearchShutter.at(nIndex);
} 

void CAlignerParam::SetEnableMutiTarget(BOOL bEnable)
{
	m_bEnableMutiTarget = bEnable;
}
BOOL CAlignerParam::GetEnableMutiTarget()
{
	return m_bEnableMutiTarget;
}

void CAlignerParam::SetAlignAccordingToPickPlatformEnable(BOOL bEnable)
{
	m_bAccordingToPickPlatformEnable = bEnable;
}


BOOL CAlignerParam::GetAlignAccordingToPickPlatformEnable()
{
	return m_bAccordingToPickPlatformEnable;
}

// 设置、获取检测信息
void CAlignerParam::SetAlignCheckInfo(const std::vector<CAlignCheckInfo> checkInfo)
{
	m_vAlignCheckInfo.assign(checkInfo.begin(),checkInfo.end());
}

std::vector<CAlignCheckInfo> CAlignerParam::GetAlignCheckInfo()
{
	return m_vAlignCheckInfo;
}

void CAlignerParam::SetAlignCheckInfo(const CAlignCheckInfo checkInfo,const int Index)
{
	if (Index < 0 || Index >= m_vAlignCheckInfo.size())
	{
		return;
	}

	m_vAlignCheckInfo.at(Index) = checkInfo;
}

CAlignCheckInfo CAlignerParam::GetAlignCheckInfo(const int Index)
{
	//异常的Index 默认返回第一个
	if (Index < 0 || Index >= m_vAlignCheckInfo.size())
	{
		return m_vAlignCheckInfo.at(0);
	}

	return m_vAlignCheckInfo.at(Index);

}

void CAlignerParam::SetMultiCalibExtensionMaxNum(int nMultiCalibExtensionMaxNum)
{
	if(nMultiCalibExtensionMaxNum < 0) return;
	m_nMultiCalibExtensionMaxNum = nMultiCalibExtensionMaxNum;
	m_vAlignCheckInfo.resize(m_nMultiCalibExtensionMaxNum + 1);
}

// 设置PCB参数
// void CAlignerParam::SetAlignerPrecision2( CCoordPos precision )
// {
// 	m_PCBInfo.m_PCBInsPre = precision;
// }
// 
// CCoordPos CAlignerParam::GetAlignerPrecision2()
// {
// 	return m_PCBInfo.m_PCBInsPre;
// }

// 设置焊接参数
void CAlignerParam::SetWeldInfo(CWeldInfo WeldInfo)
{
	m_WeldInfo = WeldInfo;
}

CWeldInfo CAlignerParam::GetWeldInfo()
{
	return m_WeldInfo;
}

// 产品虚拟位置信息
void CAlignerParam::SetProductPositionVirtualInfo(CProductPositionVirtualInfo productPositionVirtualInfo)
{
	m_ProductPositionVirtualInfo = productPositionVirtualInfo;
}

CProductPositionVirtualInfo CAlignerParam::GetProductPositionVirtualInfo()
{
	return m_ProductPositionVirtualInfo;
}

// 设置折弯检测参数
// void CAlignerParam::SetBendInspectRangeInfo(CBendInspectRangeInfo BendInspectRangeInfo)
// {
// 	m_BendInspectRangeInfo = BendInspectRangeInfo;
// }
// 
// CBendInspectRangeInfo CAlignerParam::GetBendInspectRangeInfo()
// {
// 	return m_BendInspectRangeInfo;
// }

// 设置折弯参数
void CAlignerParam::SetBendInfo(CBendInfo BendInfo)
{
	m_BendInfo = BendInfo;
}

CBendInfo CAlignerParam::GetBendInfo()
{
	return m_BendInfo;
}


void CAlignerParam::SetAlignOffsetByArea(AlignOffsetByAera& alignOffsetByArea)
{
	m_AlignOffsetByArea = alignOffsetByArea;
}

AlignOffsetByAera CAlignerParam::GetAlignOffsetByArea()
{
	return m_AlignOffsetByArea;
}


void CAlignerParam::SetLightCtrlInfo(PhotoLightCtrlSet& cObj)
{
	m_cPhotoLightCtrl = cObj;
}


PhotoLightCtrlSet CAlignerParam::GetLightCtrlInfo()
{
	return m_cPhotoLightCtrl;
}