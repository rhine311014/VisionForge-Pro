// vsXYDAlignTool.cpp: implementation of the vcXYDAlignTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vsXYDAlignTool.h"
#include "svIntersect.h"
#include "svHomography.h"
#include "vsAlignBase.h"

//#include "vsVisionAlignDef.h"	//20160510增加对角补偿
//#include "vsXYDVisionAlign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vcXYDAlignTool::vcXYDAlignTool()
{

}

vcXYDAlignTool::~vcXYDAlignTool()
{

}



/************************************************************************/
/*								基准目标			                    */
/************************************************************************/

//执行基准修正
//1.首先读取修正时的基准mark的图像坐标
//2.读取基准修正时平台及相机各轴的绝对位置
//3.调用图像坐标到平台坐标转换函数计算基准修正mark的平台坐标
//4.设置基准修正信息参数中的平台坐标

CBenchTargetMarkFixInfo* vcXYDAlignTool::ExacuteBenchMarkFix( std::vector<CCoordPos> imageCoordPoses, 
	CPlatformAxisPos* pPlatformAxisPos, std::vector<CPlatformAxisPos*> vCamPlatformAxisPoses)
{
	CBenchTargetMarkFixInfo* pBenchFixInfo= new CBenchTargetMarkFixInfo;
	pBenchFixInfo->SetMarkNum(m_iMarkNum);


	for (int i=0; i<m_iMarkNum; i++)
	{
		CCoordPos platformCoordPos;
		CCoordPos imageCoordPos = imageCoordPoses.at(i);
		CPlatformXYDAxisPos* pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)pPlatformAxisPos;
		if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
		{
			ConvertImagePosToCurPlatformPos(platformCoordPos, i, imageCoordPos, 
				pPlatformXYDAxisPos);
		}
		else
		{		
			CPlatformXYAxisPos* pCamPlatformAxisPos = (CPlatformXYAxisPos*)vCamPlatformAxisPoses.at(i);
			ConvertImagePosToCurPlatformPos(platformCoordPos, i, imageCoordPos, 
				pPlatformXYDAxisPos, pCamPlatformAxisPos);
			pBenchFixInfo->SetFixCamPlatformPos(i , pCamPlatformAxisPos);
		}

		pBenchFixInfo->SetFixPlatformPos(i,pPlatformXYDAxisPos);
		pBenchFixInfo->SetMarkImageCoordPos(i,imageCoordPos);
		pBenchFixInfo->SetMarkPlatformCoordPos(i,platformCoordPos);
	}
	pBenchFixInfo->Validate(TRUE);
	return pBenchFixInfo;
}

CBenchTargetMarkFixInfo* vcXYDAlignTool::ExacuteBenchMarkFix( std::vector<CCoordPos> imageCoordPoses, 
	std::vector<CPlatformAxisPos*> vPlatformAxisPoses) 
{
	CBenchTargetMarkFixInfo* pBenchFixInfo= new CBenchTargetMarkFixInfo;
	pBenchFixInfo->SetMarkNum(m_iMarkNum);

	for (int i=0; i<m_iMarkNum; i++)
	{
		CCoordPos platformCoordPos;
		CCoordPos imageCoordPos = imageCoordPoses.at(i);
		CPlatformXYDAxisPos* pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)vPlatformAxisPoses.at(i);
		if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
		{
			ConvertImagePosToCurPlatformPos(platformCoordPos, i, imageCoordPos, 
				pPlatformXYDAxisPos);
		}
		else
		{		
			//			CPlatformXYAxisPos* pCamPlatformAxisPos = (CPlatformXYAxisPos*)vCamPlatformAxisPoses.at(i);
			ConvertImagePosToCurPlatformPos(platformCoordPos, i, imageCoordPos, 
				pPlatformXYDAxisPos);
			//			pBenchFixInfo->SetFixCamPlatformPos(i , pCamPlatformAxisPos);
		}

		pBenchFixInfo->SetFixPlatformPos(i,pPlatformXYDAxisPos);
		pBenchFixInfo->SetMarkImageCoordPos(i,imageCoordPos);
		pBenchFixInfo->SetMarkPlatformCoordPos(i,platformCoordPos);
	}
	pBenchFixInfo->Validate(TRUE);
	return pBenchFixInfo;

}




/************************************************************************/
/*								基准拾取			                    */
/************************************************************************/

// 拾取基准对象与旋转中心的偏差信息
CBenchTargetMarkPickInfo* vcXYDAlignTool::ExacuteBenchMarkPick( std::vector<CCoordPos> imageCoordPoses, 
	std::vector<CPlatformAxisPos*> vPlatformAxisPoses, CPlatformAxisPos* pPickPlatformAxisPoses,int nIndex,CBenchTargetMarkPickInfo* OriginFixInfo,int nExProductIndex) 
{
	double dDeltaX = 0;
	double dDeltaY = 0;
	CBenchTargetMarkPickInfo* pBenchFixInfo=NULL;
	if (OriginFixInfo!=NULL)
	{
		pBenchFixInfo = new CBenchTargetMarkPickInfo(*OriginFixInfo);
	}
	else
	{
		pBenchFixInfo= new CBenchTargetMarkPickInfo();

	}


	int nPlatformIndex = m_nPlatformIndex;
	CString str;

	str.Format(_T("第%d个位置修正开始"), nIndex);
	AlignToolLogRecord(str,nPlatformIndex);

	if (m_bEnableMultiCalibExtension && m_nMultiCalibExtensionMaxNum>=0)
	{

		pBenchFixInfo->SetMarkNum(m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1));
	}
	else
	{
		nExProductIndex = 0;
		pBenchFixInfo->SetMarkNum(m_iMarkNum);
	}


	for (int i=0; i<m_iMarkNum; i++)
	{
		CCoordPos platformCoordPos;
		CCoordPos imageCoordPos = imageCoordPoses.at(i);
		CPlatformXYDAxisPos* pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)vPlatformAxisPoses.at(i);
		CPlatformXYDAxisPos* pPickPlatformXYDAxisPos = (CPlatformXYDAxisPos*)pPickPlatformAxisPoses;
		if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
		{
			ConvertImagePosToCurPickPlatformPos(platformCoordPos, i+m_iMarkNum*nExProductIndex, imageCoordPos, pPlatformXYDAxisPos);
		}
		else
		{		
			//			CPlatformXYAxisPos* pCamPlatformAxisPos = (CPlatformXYAxisPos*)vCamPlatformAxisPoses.at(i);
			ConvertImagePosToCurPlatformPos(platformCoordPos, i+m_iMarkNum*nExProductIndex, imageCoordPos, pPlatformXYDAxisPos);
			//			pBenchFixInfo->SetFixCamPlatformPos(i , pCamPlatformAxisPos);
		}

		if (GetMidPlatformType() ==ePlatformXY)
		{
			for (int j=0;j<4;j++)
			{
				pBenchFixInfo->SetFixPlatformPos(i+m_iMarkNum*nExProductIndex,pPlatformXYDAxisPos,j);//nIndex);
				pBenchFixInfo->SetPickPlatformPos(i+m_iMarkNum*nExProductIndex,pPickPlatformXYDAxisPos,j);//nIndex);
				pBenchFixInfo->SetMarkImageCoordPos(i+m_iMarkNum*nExProductIndex,imageCoordPos,j);//nIndex);
				pBenchFixInfo->SetMarkPlatformCoordPos(i+m_iMarkNum*nExProductIndex,platformCoordPos,j);//nIndex);
			}
		}
		else
		{
			pBenchFixInfo->SetFixPlatformPos(i+m_iMarkNum*nExProductIndex,pPlatformXYDAxisPos,nIndex);
			pBenchFixInfo->SetPickPlatformPos(i+m_iMarkNum*nExProductIndex,pPickPlatformXYDAxisPos,nIndex);
			pBenchFixInfo->SetMarkImageCoordPos(i+m_iMarkNum*nExProductIndex,imageCoordPos,nIndex);
			pBenchFixInfo->SetMarkPlatformCoordPos(i+m_iMarkNum*nExProductIndex,platformCoordPos,nIndex);
		}		


	}

	if (GetMidPlatformType() ==ePlatformXY) //XY平台
	{
		double dMarkX = 0.0;
		double dMarkY = 0.0;
		for (int i=0;i<m_iMarkNum;i++)
		{
			dMarkX+=pBenchFixInfo->GetMarkPlatformCoordPos(i+m_iMarkNum*nExProductIndex,nIndex).m_dPosX;
			dMarkY+=pBenchFixInfo->GetMarkPlatformCoordPos(i+m_iMarkNum*nExProductIndex,nIndex).m_dPosY;
		}
		dMarkX = dMarkX / m_iMarkNum;
		dMarkY = dMarkY / m_iMarkNum;

		CPlatformXYDAxisPos* pPickPlatformXYDAxisPos = (CPlatformXYDAxisPos*)pPickPlatformAxisPoses;
		double dPickMarkX = pPickPlatformXYDAxisPos->m_dPosX;
		double dPickMarkY = pPickPlatformXYDAxisPos->m_dPosY;
		double dPickMarkD = 0;

		CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 
		//平台各轴的方向	
		int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
		int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;
		int iPlatformDCoordType = pPlatformParam->m_nPlatformDCoordType;
		if (eDirectNegative == iPlatformXCoordType) dPickMarkX = -1*dPickMarkX;	
		if (eDirectNegative == iPlatformYCoordType) dPickMarkY = -1*dPickMarkY;
		if (eDirectNegative == iPlatformDCoordType) dPickMarkD = -1*dPickMarkD;

		//记录结果
		CCoordPos pos;
		pos.m_dPosX =  dPickMarkX - dMarkX;
		pos.m_dPosY  = dPickMarkY - dMarkY;
		pos.m_dAngle = 0;

		for(int i = 0; i < m_iMarkNum; i++)
		{
			for (int j=0;j<4;j++)
			{
				pBenchFixInfo->SetPickPlatformOffset(i+m_iMarkNum*nExProductIndex, pos,j);//nIndex);
			}

		}

		pBenchFixInfo->Validate(TRUE);
		return pBenchFixInfo;
	}


	double dDegLx;//mark坐标系与平台坐标系的夹角
	if (2 == m_iMarkNum)
	{
		double dCH0X = pBenchFixInfo->GetMarkPlatformCoordPos(0+m_iMarkNum*nExProductIndex,nIndex).m_dPosX;
		double dCH0Y = pBenchFixInfo->GetMarkPlatformCoordPos(0+m_iMarkNum*nExProductIndex,nIndex).m_dPosY;
		double dCH1X = pBenchFixInfo->GetMarkPlatformCoordPos(1+m_iMarkNum*nExProductIndex,nIndex).m_dPosX;
		double dCH1Y = pBenchFixInfo->GetMarkPlatformCoordPos(1+m_iMarkNum*nExProductIndex,nIndex).m_dPosY;
		// 构造实时mark对直线Lx
		scLine Lx;
		scLineSeg lineSeg(sc2Vector(dCH0X, dCH0Y), sc2Vector(dCH1X, dCH1Y));
		Lx.SetFromLineSeg(lineSeg);
		dDegLx = (Lx.GetRotation()).ToDouble()*180.0/skPI;

		dDeltaX = dCH0X - dCH1X;
		dDeltaY = dCH0Y - dCH1Y;
		// 		str.Format("dDegLx=%f",dDegLx);
		// 		AlignToolLogRecord(str);
	}
	else
	{
		return FALSE;
	}

	if (2 == m_iMarkNum)
	{
		double dMarkX = (pBenchFixInfo->GetMarkPlatformCoordPos(0+m_iMarkNum*nExProductIndex,nIndex).m_dPosX + pBenchFixInfo->GetMarkPlatformCoordPos(1+m_iMarkNum*nExProductIndex,nIndex).m_dPosX)/2;
		double dMarkY = (pBenchFixInfo->GetMarkPlatformCoordPos(0+m_iMarkNum*nExProductIndex,nIndex).m_dPosY + pBenchFixInfo->GetMarkPlatformCoordPos(1+m_iMarkNum*nExProductIndex,nIndex).m_dPosY)/2;

		CPlatformXYDAxisPos* pPickPlatformXYDAxisPos = (CPlatformXYDAxisPos*)pPickPlatformAxisPoses;
		double dPickMarkX = pPickPlatformXYDAxisPos->m_dPosX;
		double dPickMarkY = pPickPlatformXYDAxisPos->m_dPosY;
		double dPickMarkD = pPickPlatformXYDAxisPos->m_dAngle;

		CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 
		//平台各轴的方向	
		int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
		int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;
		int iPlatformDCoordType = pPlatformParam->m_nPlatformDCoordType;
		if (eDirectNegative == iPlatformXCoordType) dPickMarkX = -1*dPickMarkX;	
		if (eDirectNegative == iPlatformYCoordType) dPickMarkY = -1*dPickMarkY;
		if (eDirectNegative == iPlatformDCoordType) dPickMarkD = -1*dPickMarkD;

		scLine Lx, Ly;
		Lx.SetPos(sc2Vector(dMarkX, dMarkY));
		Lx.SetRotation(scDegree(dDegLx));
		Ly.SetPos(sc2Vector(dMarkX, dMarkY));
		Ly.SetRotation(scDegree(dDegLx+90));



		//计算虚拟mark点到直线Lx，Ly的投影点坐标和距离
		double dx, dy;
		double dProjectToLxLength = 0, dProjectToLyLength = 0;
		sc2Vector ProjectToLxPt, ProjectToLyPt;
		ProjectToLxPt = Lx.Project(sc2Vector(dPickMarkX, dPickMarkY));
		ProjectToLyPt = Ly.Project(sc2Vector(dPickMarkX, dPickMarkY));
		// 		str.Format("Mark%d: ProjectTOLXPt=%f,ProjectToLyPt=%f",i,ProjectToLxPt,ProjectToLyPt);
		// 		AlignToolLogRecord(str);

		dx = ProjectToLxPt.GetX() - dMarkX;
		dy = ProjectToLxPt.GetY() - dMarkY;
		dProjectToLxLength = sqrt(dx*dx + dy*dy);
		if (dx < 0) 
		{
			if (dDeltaX<0)
			{
				dProjectToLxLength = -1*dProjectToLxLength;
			}
			else if (dDeltaX>0)
			{
				dProjectToLxLength =  dProjectToLxLength;
			}
			else
			{
				return FALSE;
			}

		}
		else if (dx>0)
		{
			if (dDeltaX<0)
			{
				dProjectToLxLength = dProjectToLxLength;
			}
			else if (dDeltaX>0)
			{
				dProjectToLxLength =  -1*dProjectToLxLength;
			}
			else
			{
				return FALSE;
			}

		}
		else
		{
			return FALSE;
		}

		dx = ProjectToLyPt.GetX() - dMarkX;
		dy = ProjectToLyPt.GetY() - dMarkY;
		dProjectToLyLength = sqrt(dx*dx + dy*dy);		
		if (dy < 0) 
		{
			if (dDeltaX<0)
			{
				dProjectToLyLength = -1*dProjectToLyLength;
			}
			else if (dDeltaX>0)
			{
				dProjectToLyLength =  dProjectToLyLength;
			}
			else
			{
				return FALSE;
			}

		}
		else if (dy>0)
		{
			if (dDeltaX<0)
			{
				dProjectToLyLength = dProjectToLyLength;
			}
			else if (dDeltaX>0)
			{
				dProjectToLyLength =  -1*dProjectToLyLength;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
		// 		str.Format("Mark%d: dProjectToLxLength=%f,dProjectToLyLength=%f",i,dProjectToLxLength,dProjectToLyLength);
		// 		AlignToolLogRecord(str);

		//记录结果
		CCoordPos pos;
		pos.m_dPosX = dProjectToLxLength;
		pos.m_dPosY  = dProjectToLyLength;
		pos.m_dAngle = dPickMarkD;

		for(int i = 0; i < m_iMarkNum; i++)
		{
			pBenchFixInfo->SetPickPlatformOffset(i+m_iMarkNum*nExProductIndex, pos,nIndex);
		}

	}
	else
	{
		return FALSE;
	}


	str.Format(_T("第%d个位置修正结束"), nIndex);
	AlignToolLogRecord(str,nPlatformIndex);

	// 拾取时旋转中心位置修正
	//if (((vcXYDVisionAlign*)m_pVisionAlignerTool)->m_pSystempOptionConfig->m_bEnablePickPosUpdate)
	if (m_bEnablePickPosUpdate)
	{
		if (nIndex == 0)
		{
			str.Format(_T("根据第%d个位置修正旋转中心开始"), nIndex);
			AlignToolLogRecord(str,nPlatformIndex);

			// 修正当前标定信息

			// 			for (int i = 0 + nExProductIndex*m_iMarkNum; i < pBenchFixInfo->m_vCalibratedInfo.size() && i< (nExProductIndex+1)*m_iMarkNum; i++)
			// 			{
			// 				if (pBenchFixInfo->m_vCalibratedInfo.at(i) != NULL)
			// 				{
			// 					delete pBenchFixInfo->m_vCalibratedInfo.at(i);
			// 					pBenchFixInfo->m_vCalibratedInfo.at(i) = NULL;
			// 				}
			// 			}

			// 			if (m_bEnableMultiCalibExtension && m_nMultiCalibExtensionMaxNum>=0)
			// 			{
			// 				pBenchFixInfo->m_vCalibratedInfo.resize(m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1));
			// 			}
			// 			else
			// 			{
			// 				pBenchFixInfo->m_vCalibratedInfo.resize(m_iMarkNum);
			// 			}
			// 
			// 			for (int i = 0 + nExProductIndex*m_iMarkNum; i < pBenchFixInfo->m_vCalibratedInfo.size() && i< (nExProductIndex+1)*m_iMarkNum; i++)
			// 			{
			// 				pBenchFixInfo->m_vCalibratedInfo.at(i) = NULL;
			// 			}

			for (int i = 0; i < m_iMarkNum; i++)
			{
				double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(i+m_iMarkNum*nExProductIndex)->GetMarkPlatformCoordPos().GetPosX();
				double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(i+m_iMarkNum*nExProductIndex)->GetMarkPlatformCoordPos().GetPosY();

				CCoordPos cp = pBenchFixInfo->GetPickPlatformOffset(i+m_iMarkNum*nExProductIndex, nIndex);

				CCoordPos cOffset;
				cOffset.m_dPosX = dPlatformBenchMarkX + cp.m_dPosX;
				cOffset.m_dPosY = dPlatformBenchMarkY + cp.m_dPosY;

				m_vAlignerCalibInfos.at(i+m_iMarkNum*nExProductIndex)->SetMarkPlatformCoordPos(cOffset);

				if (i+m_iMarkNum*nExProductIndex < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[i+m_iMarkNum*nExProductIndex] != NULL)
				{
					//					pBenchFixInfo->m_vCalibratedInfo.at(i+m_iMarkNum*nExProductIndex) = new CCalibratedInfo(*m_vAlignerCalibInfos[i+m_iMarkNum*nExProductIndex]);
					//					//pBenchFixInfo->m_vCalibratedInfo.push_back(m_vAlignerCalibInfos[i]);
					//pBenchFixInfo->m_vCalibratedInfo.at(i) = m_vAlignerCalibInfos[i];
				}
			}


			for (int i=0; i<m_iMarkNum; i++)
			{
				CCoordPos platformCoordPos;
				CCoordPos imageCoordPos = imageCoordPoses.at(i);
				CPlatformXYDAxisPos* pPlatformXYDAxisPos = (CPlatformXYDAxisPos*)vPlatformAxisPoses.at(i);
				CPlatformXYDAxisPos* pPickPlatformXYDAxisPos = (CPlatformXYDAxisPos*)pPickPlatformAxisPoses;
				if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
				{
					ConvertImagePosToCurPickPlatformPos(platformCoordPos, i+m_iMarkNum*nExProductIndex, imageCoordPos, pPlatformXYDAxisPos);
				}
				else
				{		
					ConvertImagePosToCurPlatformPos(platformCoordPos, i+m_iMarkNum*nExProductIndex, imageCoordPos, pPlatformXYDAxisPos);
				}

				pBenchFixInfo->SetFixPlatformPos(i+m_iMarkNum*nExProductIndex,pPlatformXYDAxisPos,nIndex);
				pBenchFixInfo->SetPickPlatformPos(i+m_iMarkNum*nExProductIndex,pPickPlatformXYDAxisPos,nIndex);
				pBenchFixInfo->SetMarkImageCoordPos(i+m_iMarkNum*nExProductIndex,imageCoordPos,nIndex);
				pBenchFixInfo->SetMarkPlatformCoordPos(i+m_iMarkNum*nExProductIndex,platformCoordPos,nIndex);
			}

			if (2 == m_iMarkNum)
			{
				double dMarkX = (pBenchFixInfo->GetMarkPlatformCoordPos(0+m_iMarkNum*nExProductIndex,nIndex).m_dPosX + pBenchFixInfo->GetMarkPlatformCoordPos(1+m_iMarkNum*nExProductIndex,nIndex).m_dPosX)/2;
				double dMarkY = (pBenchFixInfo->GetMarkPlatformCoordPos(0+m_iMarkNum*nExProductIndex,nIndex).m_dPosY + pBenchFixInfo->GetMarkPlatformCoordPos(1+m_iMarkNum*nExProductIndex,nIndex).m_dPosY)/2;

				CPlatformXYDAxisPos* pPickPlatformXYDAxisPos = (CPlatformXYDAxisPos*)pPickPlatformAxisPoses;
				double dPickMarkX = pPickPlatformXYDAxisPos->m_dPosX;
				double dPickMarkY = pPickPlatformXYDAxisPos->m_dPosY;
				double dPickMarkD = pPickPlatformXYDAxisPos->m_dAngle;

				CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 
				//平台各轴的方向	
				int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
				int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;
				int iPlatformDCoordType = pPlatformParam->m_nPlatformDCoordType;
				if (eDirectNegative == iPlatformXCoordType) dPickMarkX = -1*dPickMarkX;	
				if (eDirectNegative == iPlatformYCoordType) dPickMarkY = -1*dPickMarkY;
				if (eDirectNegative == iPlatformDCoordType) dPickMarkD = -1*dPickMarkD;

				scLine Lx, Ly;
				Lx.SetPos(sc2Vector(dMarkX, dMarkY));
				Lx.SetRotation(scDegree(dDegLx));
				Ly.SetPos(sc2Vector(dMarkX, dMarkY));
				Ly.SetRotation(scDegree(dDegLx+90));

				//计算虚拟mark点到直线Lx，Ly的投影点坐标和距离
				double dx, dy;
				double dProjectToLxLength = 0, dProjectToLyLength = 0;
				sc2Vector ProjectToLxPt, ProjectToLyPt;
				ProjectToLxPt = Lx.Project(sc2Vector(dPickMarkX, dPickMarkY));
				ProjectToLyPt = Ly.Project(sc2Vector(dPickMarkX, dPickMarkY));

				dx = ProjectToLxPt.GetX() - dMarkX;
				dy = ProjectToLxPt.GetY() - dMarkY;
				dProjectToLxLength = sqrt(dx*dx + dy*dy);
				if (dx < 0) 
				{
					if (dDeltaX<0)
					{
						dProjectToLxLength = -1*dProjectToLxLength;
					}
					else if (dDeltaX>0)
					{
						dProjectToLxLength =  dProjectToLxLength;
					}
					else
					{
						return FALSE;
					}

				}
				else if (dx>0)
				{
					if (dDeltaX<0)
					{
						dProjectToLxLength = dProjectToLxLength;
					}
					else if (dDeltaX>0)
					{
						dProjectToLxLength =  -1*dProjectToLxLength;
					}
					else
					{
						return FALSE;
					}

				}
				else
				{
					return FALSE;
				}

				dx = ProjectToLyPt.GetX() - dMarkX;
				dy = ProjectToLyPt.GetY() - dMarkY;
				dProjectToLyLength = sqrt(dx*dx + dy*dy);		
				if (dy < 0) 
				{
					if (dDeltaX<0)
					{
						dProjectToLyLength = -1*dProjectToLyLength;
					}
					else if (dDeltaX>0)
					{
						dProjectToLyLength =  dProjectToLyLength;
					}
					else
					{
						return FALSE;
					}

				}
				else if (dy>0)
				{
					if (dDeltaX<0)
					{
						dProjectToLyLength = dProjectToLyLength;
					}
					else if (dDeltaX>0)
					{
						dProjectToLyLength =  -1*dProjectToLyLength;
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}

				//记录结果
				CCoordPos pos;
				pos.m_dPosX = dProjectToLxLength;
				pos.m_dPosY  = dProjectToLyLength;
				pos.m_dAngle = dPickMarkD;

				for(int i = 0; i < m_iMarkNum; i++)
				{
					pBenchFixInfo->SetPickPlatformOffset(i+m_iMarkNum*nExProductIndex, pos,nIndex);
				}

				str.Format(_T("根据第%d个位置修正旋转中心结束"), nIndex);
				AlignToolLogRecord(str,nPlatformIndex);

			}
			else
			{
				return FALSE;
			}

		}
	}


	pBenchFixInfo->Validate(TRUE);
	return pBenchFixInfo;
}

bool vcXYDAlignTool::ConvertImagePosToCurPickPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, CPlatformAxisPos* pPlatformAxisPos)
{
	if (nIndex%m_iMarkNum<0 || nIndex%m_iMarkNum >= m_iMarkNum)
		return false;
	if (NULL == m_vAlignerCalibInfos.at(nIndex))
		return false;

	int nRemIndex = nIndex%m_iMarkNum;
	if (NULL == m_vpCalibrateParam.at(nRemIndex))
		return false;

	if(m_vpCalibrateParam.at(nRemIndex)->m_nEnableDisCor)
	{
		CString strOut, strTmp;
		double p[15];
		memset(p, 0.0, sizeof(double)*15);
		scCalibrateResult result = m_vAlignerCalibInfos.at(nIndex)->m_result;
		result.GetNonlinearProjectResult(p);
		double dRms = result.GetRMS();
		strOut.Format("畸变参数: ");
		for (int i=0;i<15; i++)
		{
			strTmp.Format(_T("P%d:%.6f "), i, p[i]);
			strOut += strTmp;
		}
		strTmp.Format(_T("P15:%.6f "),dRms);
		strOut += strTmp;
		AlignToolLogRecord(strOut,m_nPlatformIndex);

		strOut.Format("畸变校正前图像坐标: ");
		strTmp.Format(_T("Pos(%.6f, %.6f, %.6f) "), imageCoordPos.m_dPosX, imageCoordPos.m_dPosY, imageCoordPos.m_dAngle);
		strOut += strTmp;
		for (int i=0; i<imageCoordPos.m_vdAuxiliaryPosX.size(); i++)
		{
			strTmp.Format(_T("AuxiliaryPos%d(%.6f, %.6f, %.6f) "), i, imageCoordPos.m_vdAuxiliaryPosX.at(i), imageCoordPos.m_vdAuxiliaryPosY.at(i), imageCoordPos.m_vdAuxiliaryAngle.at(i));
			strOut += strTmp;
		}
		AlignToolLogRecord(strOut,m_nPlatformIndex);

		CCoordPos imageDisCorPos;
		ExecuteDisCorPoint(nIndex, imageCoordPos, imageDisCorPos);
		imageCoordPos = imageDisCorPos;

		strOut.Format("畸变校正后图像坐标: ");
		strTmp.Format(_T("Pos(%.6f, %.6f, %.6f) "), imageCoordPos.m_dPosX, imageCoordPos.m_dPosY, imageCoordPos.m_dAngle);
		strOut += strTmp;
		for (int i=0; i<imageCoordPos.m_vdAuxiliaryPosX.size(); i++)
		{
			strTmp.Format(_T("AuxiliaryPos%d(%.6f, %.6f, %.6f) "), i, imageCoordPos.m_vdAuxiliaryPosX.at(i), imageCoordPos.m_vdAuxiliaryPosY.at(i), imageCoordPos.m_vdAuxiliaryAngle.at(i));
			strOut += strTmp;
		}
		AlignToolLogRecord(strOut,m_nPlatformIndex);
	}


	if (m_PlatformCalibType == ePlatformCalib9Point)
	{
		if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
		{
			return false;
		}
	}
	int nPlatformIndex = m_nPlatformIndex;
	CString str;
	//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
	//相机平台类型没有基类，不用改；
	CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
	CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
	CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 
	//	CString str;

	if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
	{
		return false;
	}

	str.Format("轴基准坐标：X=%f,Y=%f,D=%f",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
	AlignToolLogRecord(str,nPlatformIndex);
	str.Format("当前轴坐标：X=%f,Y=%f,D=%f",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
	AlignToolLogRecord(str,nPlatformIndex);

	// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
	//标定参数
	double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
	double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
	double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
	double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

	double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
	double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();
	//平台各轴的方向	
	int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
	int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;

	// 实际平台XY轴偏移量
	double dPlatformOffsetX = 0;
	double dPlatformOffsetY = 0;

	if (m_ePickCamPlatformType == ePickCamPlatformSepFix) // 相机固定安装
	{
		dPlatformOffsetX = /*pPlatformXYDAxisPos->m_dPosX - */pPlatformBenchAxisPos->m_dPosX;
		dPlatformOffsetY = /*pPlatformXYDAxisPos->m_dPosY- */pPlatformBenchAxisPos->m_dPosY;
	}
	else if (m_ePickCamPlatformType == ePickCamPlatformShareXY) // 相机安装在吸头XYD平台的XY轴上
	{
		dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX/* - pPlatformBenchAxisPos->m_dPosX*/;
		dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY/*- pPlatformBenchAxisPos->m_dPosY*/;
	}
	else
	{
		return false;
	}


	if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
	if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;


	str.Format("Mark平台基准位置：X=%f,Y=%f",dPlatformBenchMarkX,dPlatformBenchMarkY);
	AlignToolLogRecord(str,nPlatformIndex);
	str.Format("轴平移量：X=%f,Y=%f",dPlatformOffsetX,dPlatformOffsetY);
	AlignToolLogRecord(str,nPlatformIndex);

	//平台XY平移量对基准mark平台坐标的影响方向相同
	dPlatformBenchMarkX += dPlatformOffsetX;
	dPlatformBenchMarkY += dPlatformOffsetY;


	// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
	double dImgOffsetRealDistanceX = 0;
	double dImgOffsetRealDistanceY = 0;
	double dImgOffsetX = imageCoordPos.m_dPosX - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX;
	double dImgOffsetY = imageCoordPos.m_dPosY - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY;
	dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
	dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

	str.Format("基准图像位置：X=%f,Y=%f",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
	AlignToolLogRecord(str,nPlatformIndex);
	str.Format("当前图像位置：X=%f,Y=%f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
	AlignToolLogRecord(str,nPlatformIndex);
	str.Format("图像像素移动量：X=%f,Y=%f",dImgOffsetX,dImgOffsetY);
	AlignToolLogRecord(str,nPlatformIndex);
	{
		if (m_PlatformCalibType == ePlatformCalib9Point)
		{
			sc2Vector vSrc;
			sc2Vector vDst;
			vSrc.SetX(dImgOffsetX);
			vSrc.SetY(dImgOffsetY);
			if (m_vpCalibrateParam.at(nIndex%m_iMarkNum)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
			{
				sc2Vector vBoard;
				vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
				vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
			}
			else
			{
				vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
			}

			dImgOffsetRealDistanceX = vDst.GetX();
			dImgOffsetRealDistanceY = vDst.GetY();
		}
	}

	str.Format("图像像素平台移动量：X=%f,Y=%f",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
	AlignToolLogRecord(str,nPlatformIndex);


	// $4、内容相加，即为相机实时Mark的平台坐标值
	platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
	platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
	//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
	platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

	str.Format("平台坐标值：X=%f,Y=%f,D=%f",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
	AlignToolLogRecord(str,nPlatformIndex);
	return true;
}

bool vcXYDAlignTool::CalcuObjectPlatformPosToPickPlatformPos(std::vector<CCoordPos> vObjectPlatformPos, CCoordPos& cpPickPlatformPos,int nIndex)
{

	double dDegLx;//mark坐标系与平台坐标系的夹角

	if (GetMidPlatformType() == ePlatformXY)
	{
		double dMarkX = 0.0;
		double dMarkY = 0.0;
		for (int i=0;i<m_iMarkNum;i++)
		{
			dMarkX+=vObjectPlatformPos.at(i).m_dPosX;
			dMarkY+=vObjectPlatformPos.at(i).m_dPosY;
		}
		dMarkX = dMarkX / m_iMarkNum;
		dMarkY = dMarkY / m_iMarkNum;
		// 计算拾取时的绝对位置
		CBenchTargetMarkPickInfo *pBenchTargetMarkPickInfo = m_AlignermentParam.GetBenchTargetMarkPickInfo();
		if (pBenchTargetMarkPickInfo == NULL)
		{
			return FALSE;
		}

		CCoordPos pickPlatformOffset = pBenchTargetMarkPickInfo->GetPickPlatformOffset(0+ m_iMarkNum*m_nAlignnExProductIndex,nIndex);


		CCoordPos pos;
		pos.m_dPosX =  dMarkX+pickPlatformOffset.m_dPosX;
		pos.m_dPosY  = dMarkY+pickPlatformOffset.m_dPosY;
		cpPickPlatformPos = pos;
		return TRUE;
	}

	if (2 == m_iMarkNum)
	{
		double dCH0X = vObjectPlatformPos.at(0).m_dPosX;
		double dCH0Y = vObjectPlatformPos.at(0).m_dPosY;
		double dCH1X = vObjectPlatformPos.at(1).m_dPosX;
		double dCH1Y = vObjectPlatformPos.at(1).m_dPosY;
		// 构造实时mark对直线Lx、LyCH0、LyCH1
		scLine Lx;
		scLineSeg lineSeg(sc2Vector(dCH0X, dCH0Y), sc2Vector(dCH1X, dCH1Y));
		Lx.SetFromLineSeg(lineSeg);
		dDegLx = (Lx.GetRotation()).ToDouble()*180.0/skPI;
	}
	else
	{
		return FALSE;
	}


	if (2 == m_iMarkNum)
	{
		// 计算拾取时的绝对位置
		CBenchTargetMarkPickInfo *pBenchTargetMarkPickInfo = m_AlignermentParam.GetBenchTargetMarkPickInfo();
		if (pBenchTargetMarkPickInfo == NULL)
		{
			return FALSE;
		}

		CCoordPos pickPlatformOffset = pBenchTargetMarkPickInfo->GetPickPlatformOffset(0+ m_iMarkNum*m_nAlignnExProductIndex,nIndex);

		double dValue;
		sc2Vector Px, Py;
		sc2Vector PtIntersect;
		dValue = (vObjectPlatformPos.at(0).m_dPosX + vObjectPlatformPos.at(1).m_dPosX)/2
			+ cos(scDegree(dDegLx))*pickPlatformOffset.m_dPosX;
		Px.SetX(dValue);
		dValue = (vObjectPlatformPos.at(0).m_dPosY + vObjectPlatformPos.at(1).m_dPosY)/2
			+ sin(scDegree(dDegLx))*pickPlatformOffset.m_dPosX;
		Px.SetY(dValue);

		dValue = (vObjectPlatformPos.at(0).m_dPosX + vObjectPlatformPos.at(1).m_dPosX)/2
			- sin(scDegree(dDegLx))*pickPlatformOffset.m_dPosY;
		Py.SetX(dValue);
		dValue = (vObjectPlatformPos.at(0).m_dPosY + vObjectPlatformPos.at(1).m_dPosY)/2
			+ cos(scDegree(dDegLx))*pickPlatformOffset.m_dPosY;
		Py.SetY(dValue);
		scLine Lx2;
		Lx2.SetPos(Px);
		Lx2.SetRotation(scDegree(dDegLx+90));
		PtIntersect = Lx2.Project(Py);
		//记录结果
		CCoordPos pos;
		pos.m_dPosX = PtIntersect.GetX();
		pos.m_dPosY  = PtIntersect.GetY();
		cpPickPlatformPos = pos;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

bool vcXYDAlignTool::GetPickPlatformPos(CPlatformAxisPos* pPlatformAxisPos, CCoordPos& cpPickPlatformPos)
{

	int nPlatformIndex = m_nPlatformIndex;
	CString str;
	//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
	//相机平台类型没有基类，不用改；
	CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
	CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 

	if (NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
	{
		return false;
	}

	str.Format("当前轴坐标：X=%f,Y=%f,D=%f",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
	AlignToolLogRecord(str,nPlatformIndex);

	// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
	//平台各轴的方向	
	int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
	int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;
	int iPlatformDCoordType = pPlatformParam->m_nPlatformDCoordType;

	// 实际平台XY轴偏移量
	double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX/* - pPlatformBenchAxisPos->m_dPosX*/;
	double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY/*- pPlatformBenchAxisPos->m_dPosY*/;
	double dPlatformOffsetD = pPlatformXYDAxisPos->m_dAngle;

	if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
	if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;
	if (eDirectNegative == iPlatformDCoordType) dPlatformOffsetD = -1*dPlatformOffsetD;


	//平台XY平移量对基准mark平台坐标的影响方向相同
	str.Format("轴平移量：X=%f,Y=%f,D=%f",dPlatformOffsetX,dPlatformOffsetY,dPlatformOffsetD);
	AlignToolLogRecord(str,nPlatformIndex);

	// $4、内容相加，即为相机实时Mark的平台坐标值
	cpPickPlatformPos.m_dPosX = dPlatformOffsetX ;
	cpPickPlatformPos.m_dPosY = dPlatformOffsetY ;
	cpPickPlatformPos.m_dAngle = dPlatformOffsetD ;

	str.Format("平台坐标值：X=%f,Y=%f,D=%f",cpPickPlatformPos.m_dPosX,cpPickPlatformPos.m_dPosY,cpPickPlatformPos.m_dAngle);
	AlignToolLogRecord(str,nPlatformIndex);
	return true;
}


BOOL vcXYDAlignTool::ExecuteAlignPick(int nAlnIndex/* = 0*/)
{
	m_nAlnIndex = nAlnIndex;
	m_nAlignErrNum = 0;

	int nPlatformIndex = m_nPlatformIndex;
	CString str;
	str.Format("对位计算开始：序号[%d]------------------------------------------------------------",nAlnIndex);
	AlignToolLogRecord(str,nPlatformIndex);
	// 	if (eTargetOnline == m_AlignermentParam.GetTargetMarkType() && FALSE == m_TargetSearchResults.IsAllMarkImagePosOK())
	// 	{
	// 		return FALSE;
	// 	}

	for (int i = 0; i < m_iMarkNum; i++)
	{
		if (NULL == m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nAlignnExProductIndex))
		{
			m_nAlignErrNum = -1;
			return false;
		}

		if (!m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nAlignnExProductIndex)->IsValid())
		{
			m_nAlignErrNum = -2;
			return false;
		}
	}


	if (FALSE == m_ObjectSearchResults.IsAllMarkImagePosOK())
	{
		m_nAlignErrNum = -3;
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//不同的角度采用不同的补偿
	// 	double dangle = abs(m_ObjectSearchResults.GetMarkImagePos(0).m_dAngle);
	// 	double dangeloffset0 = abs(dangle-0);
	// 	double dangeloffset90 = abs(dangle-90);
	// 	double dangeloffset180 = abs(dangle-180);
	// 	double dangeloffset270 = abs(dangle-270);
	// 
	// 	int nMinIndex = 0;
	// 	double dmin = dangeloffset0;
	// 	if (dmin>dangeloffset90)
	// 	{
	// 		dmin=dangeloffset90;
	// 		nMinIndex =1;
	// 	}
	// 
	// 	if (dmin>dangeloffset180)
	// 	{
	// 		dmin=dangeloffset180;
	// 		nMinIndex =2;
	// 	}
	// 
	// 	if (dmin>dangeloffset270)
	// 	{
	// 		dmin=dangeloffset270;
	// 		nMinIndex =3;
	// 	}


	double dangle = abs(scDegree(m_ObjectSearchResults.GetMarkImagePos(0).m_dAngle).Norm().ToDouble());
	int nMinIndex = 0;
	if (dangle <= 45 )
	{
		nMinIndex = 0;
	}
	else if (dangle > 45 && dangle <= 135)
	{
		nMinIndex = 1;
	}
	else if (dangle > 135 && dangle <= 225)
	{
		nMinIndex = 2;
	}
	else if (dangle > 225 && dangle < 315)
	{
		nMinIndex = 3;
	}
	else
	{
		nMinIndex = 0;
	}



	str.Format("图像角度=%f,调用补偿参数%d",m_ObjectSearchResults.GetMarkImagePos(0).m_dAngle,nMinIndex);
	AlignToolLogRecord(str,nPlatformIndex);

	// 	if (TRUE == m_AlignermentParam.GetVirtualObjectMarkEnable()&&FALSE == m_VirtualSearchResults.IsAllMarkImagePosOK())
	// 	{
	// 		return FALSE;
	// 	}

	//目标mark坐标补偿时用到这些参数
	std::vector<CPlatformXYDAxisPos*> pObjectPlatformAxisPos;

	// 读取基准拾取
	CBenchTargetMarkPickInfo *pBenchTargetMarkPickInfo = m_AlignermentParam.GetBenchTargetMarkPickInfo();
	if (pBenchTargetMarkPickInfo == NULL)
	{
		return FALSE;
	}

	int k=0;
	for (k=0;k<m_pObjectPlatformAxisPos.size();k++)
	{
		// pObjectPlatformAxisPos.push_back((CPlatformXYDAxisPos*)(m_pObjectPlatformAxisPos.at(k)));//搜索对象mark轴位置
		pObjectPlatformAxisPos.push_back((CPlatformXYDAxisPos*)pBenchTargetMarkPickInfo->GetPickPlatformPos(0+ m_iMarkNum*m_nAlignnExProductIndex,nMinIndex));
		str.Format("搜索对象mark%d 轴位置 X=%f,Y=%f,D=%f",k,pObjectPlatformAxisPos.at(k)->m_dPosX,pObjectPlatformAxisPos.at(k)->m_dPosY,pObjectPlatformAxisPos.at(k)->m_dAngle);
		AlignToolLogRecord(str,nPlatformIndex);
	}
	// 	std::vector<CPlatformXYDAxisPos*> pTargetPlatformAxisPos;
	// 	for (k=0;k<m_pTargetPlatformAxisPos.size();k++)
	// 	{
	// 		pTargetPlatformAxisPos.push_back((CPlatformXYDAxisPos*)m_pTargetPlatformAxisPos.at(k));//搜索目标mark轴位置
	// 		str.Format("搜索目标mark%d 轴位置 X=%f,Y=%f,D=%f",k,pTargetPlatformAxisPos.at(k)->m_dPosX,pTargetPlatformAxisPos.at(k)->m_dPosY,pTargetPlatformAxisPos.at(k)->m_dAngle);
	// 		AlignToolLogRecord(str,nPlatformIndex);
	// 	}
	// 	if(pTargetPlatformAxisPos.size()<m_iMarkNum)
	// 	{
	// 		pTargetPlatformAxisPos.resize(m_iMarkNum);
	// 
	// 	}

	double dObjectX[6];			//存储对象mark图像坐标
	double dObjectY[6];
	double dObjectD[6];
	double dTargetX[6];			//存储目标mark图像坐标
	double dTargetY[6];
	double dTargetD[6];



	memset(dObjectX, 0.0, sizeof(double)*6);
	memset(dObjectY, 0.0, sizeof(double)*6);
	memset(dObjectD, 0.0, sizeof(double)*6);
	memset(dTargetX, 0.0, sizeof(double)*6);
	memset(dTargetY, 0.0, sizeof(double)*6);
	memset(dTargetD, 0.0, sizeof(double)*6);



	// $1、实时对象Mark的图像坐标值转化为平台坐标值

	// $1.1 将实时对象Mark的图像坐标值转换为实时对象Mark搜索时的全局平台坐标值
	m_ObjectMarkPlatformPos = GetObjectMarkPickPlatformPos();
	if (FALSE == m_ObjectMarkPlatformPos.IsAllMarkPlatformPosOK())
	{
		return FALSE;
	}

	int i=0;
	//读取每个相机对应的对象mark和平台mark的坐标值
	for (i=0; i<m_iMarkNum; i++)
	{
		//记录相机对象mark的坐标
		dObjectX[i] = m_ObjectMarkPlatformPos.m_vPos.at(i).m_dPosX;
		dObjectY[i] = m_ObjectMarkPlatformPos.m_vPos.at(i).m_dPosY;
		dObjectD[i] = m_ObjectMarkPlatformPos.m_vPos.at(i).m_dAngle;

		str.Format("对象图像坐标%d:ObjectImageX=%f,ObjectImageY=%f,ObjectImageD=%f",i,m_ObjectSearchResults.m_vPos.at(i).m_dPosX,m_ObjectSearchResults.m_vPos.at(i).m_dPosY,m_ObjectSearchResults.m_vPos.at(i).m_dAngle);
		AlignToolLogRecord(str,nPlatformIndex);
		str.Format("对象平台坐标%d:ObjectX=%f,ObjectY=%f,ObjectD=%f",i,dObjectX[i],dObjectY[i],dObjectD[i]);
		AlignToolLogRecord(str,nPlatformIndex);
	}

	// $1.2补偿
	{
		// $1.2 对象Mark的全局平台坐标值补偿
		//当不需要基准修正时，考虑要不要对对象mark进行补偿

		CMarkPlatformPos m_ObjectAddRotationOffsetPos;
		m_ObjectAddRotationOffsetPos.m_vPos.resize(m_iMarkNum);
		m_ObjectAddRotationOffsetPos.m_vbOK.resize(m_iMarkNum);
		CMarkPlatformPos m_ObjectAddMoveOffsetPos;
		m_ObjectAddMoveOffsetPos.m_vPos.resize(m_iMarkNum);
		m_ObjectAddMoveOffsetPos.m_vbOK.resize(m_iMarkNum);


		int  nCurOffsetIndex = m_AlignermentParam.GetCurAlignerOffsetIndex();
		AlignOffsetByAera alignOffsetByArea = m_AlignermentParam.GetAlignOffsetByArea();
		if (alignOffsetByArea.m_bEnable)
		{
			int nRow = alignOffsetByArea.m_nAreaRow;
			int nCol = alignOffsetByArea.m_nAreaCol;
			if (m_nAlignnExProductIndex > 0)
			{
			   m_AlignermentParam.SetCurAlignerOffsetIndex(10 + nCurOffsetIndex);
			} 
			else
			{
				m_AlignermentParam.SetCurAlignerOffsetIndex(nRow*nCol*m_nAlignnExProductIndex + nCurOffsetIndex);
			}
				
		}
		else
		{
			switch (nMinIndex)
			{
			case 0:
				m_AlignermentParam.SetCurAlignerOffsetIndex(BENCH_FIX_POSNUM*m_nAlignnExProductIndex + 0);	
				break;
			case 1:
				m_AlignermentParam.SetCurAlignerOffsetIndex(BENCH_FIX_POSNUM*m_nAlignnExProductIndex + 1);	
				break;
			case 2:
				m_AlignermentParam.SetCurAlignerOffsetIndex(BENCH_FIX_POSNUM*m_nAlignnExProductIndex + 2);	
				break;
			case 3:
				m_AlignermentParam.SetCurAlignerOffsetIndex(BENCH_FIX_POSNUM*m_nAlignnExProductIndex + 3);	
				break;
			default:
				m_AlignermentParam.SetCurAlignerOffsetIndex(BENCH_FIX_POSNUM*m_nAlignnExProductIndex + 0);	
				break;
			}
		}
		//if (eMarkCoord == m_AlignermentParam.GetAlignerOffsetCoordType())// Mark坐标系下补偿
		if (m_AlignermentParam.GetAlignerBaseOffset().GetAngle()||m_AlignermentParam.GetAlignerBaseOffset().GetPosX()||m_AlignermentParam.GetAlignerBaseOffset().GetPosY()
			|| m_AlignermentParam.GetAlignerOffset().GetAngle()||m_AlignermentParam.GetAlignerOffset().GetPosX()||m_AlignermentParam.GetAlignerOffset().GetPosY())
		{
			// Mark坐标系下旋转角度进行补偿
			MarkCoordOffsetRotationObjectPlatformPos(m_ObjectMarkPlatformPos.m_vPos, m_ObjectAddRotationOffsetPos.m_vPos);
			// Mark坐标系下XY轴平移量进行补偿
			MarkCoordOffsetXYObjectPltfmPt(m_ObjectAddRotationOffsetPos.m_vPos, m_ObjectAddMoveOffsetPos.m_vPos);
			for (i=0; i<m_iMarkNum; i++)
			{
				//记录相机对象mark的坐标
				dObjectX[i] = m_ObjectAddMoveOffsetPos.m_vPos.at(i).m_dPosX;
				dObjectY[i] = m_ObjectAddMoveOffsetPos.m_vPos.at(i).m_dPosY;
				dObjectD[i] = m_ObjectAddMoveOffsetPos.m_vPos.at(i).m_dAngle;

				str.Format("Mark坐标系补偿后对象平台坐标%d:ObjectX=%f,ObjectY=%f,ObjectD=%f",i,dObjectX[i],dObjectY[i],dObjectD[i]);
				AlignToolLogRecord(str,nPlatformIndex);
			}
		}

		m_AlignermentParam.SetCurAlignerOffsetIndex(nCurOffsetIndex);
		// 		else// 平台坐标系下补偿
		// 		{
		// 			// 全局平台坐标系下旋转角度进行补偿
		// 			if (m_AlignermentParam.GetAlignerOffset().GetAngle()||m_AlignermentParam.GetAlignerOffset().GetPosX()||m_AlignermentParam.GetAlignerOffset().GetPosY())
		// 			{		
		// 				// 全局平台坐标系下旋转角度进行补偿
		// 				PltfmCoordOffsetRotationObjectPltfmPt(m_ObjectMarkPlatformPos.m_vPos, m_ObjectAddRotationOffsetPos.m_vPos);
		// 				// 全局平台坐标系下XY轴平移量进行补偿
		// 				PltfmCoordOffsetXYObjectPltfmPt(m_ObjectAddRotationOffsetPos.m_vPos, m_ObjectAddMoveOffsetPos.m_vPos);
		// 				for (i=0; i<m_iMarkNum; i++)
		// 				{
		// 					//记录相机对象mark的坐标
		// 					dObjectX[i] = m_ObjectAddMoveOffsetPos.m_vPos.at(i).m_dPosX;
		// 					dObjectY[i] = m_ObjectAddMoveOffsetPos.m_vPos.at(i).m_dPosY;
		// 					dObjectD[i] = m_ObjectAddMoveOffsetPos.m_vPos.at(i).m_dAngle;
		// 
		// 					str.Format("平台坐标系对象平台坐标%d:ObjectX=%f,ObjectY=%f,ObjectD=%f",i,dObjectX[i],dObjectY[i],dObjectD[i]);
		// 					AlignToolLogRecord(str,nPlatformIndex);
		// 				}
		// 			}
		//		}
	}


	// $1.3计算实时拾取的全局平台坐标(XY轴平台坐标位置，不包括D轴旋转角度)
	std::vector<CCoordPos> vObjectPlatformPos;
	for (i=0; i<m_iMarkNum; i++)
	{
		CCoordPos pos;
		pos.m_dPosX = dObjectX[i];
		pos.m_dPosY = dObjectY[i];
		pos.m_dAngle = dObjectD[i];

		vObjectPlatformPos.push_back(pos);

	}
	CCoordPos cpObjectPickPlatformPos;  // 实时拾取的全局平台坐标(XY轴平台坐标位置，不包括D轴旋转角度)
	if (!CalcuObjectPlatformPosToPickPlatformPos(vObjectPlatformPos,cpObjectPickPlatformPos,nMinIndex))
	{
		return FALSE;
	}
	str.Format("对象全局坐标X=%f ,Y=%f",cpObjectPickPlatformPos.GetPosX(),cpObjectPickPlatformPos.GetPosY());
	AlignToolLogRecord(str,nPlatformIndex);


	// $2 基准拾取和Mark的位置（全局平台坐标）
	//CBenchTargetMarkPickInfo *pBenchTargetMarkPickInfo = m_AlignermentParam.GetBenchTargetMarkPickInfo();
	//if (pBenchTargetMarkPickInfo == NULL)
	//{
	//	return FALSE;
	//}
	// 基准Mark的全局平台坐标
	for (i=0; i<m_iMarkNum; i++)
	{
		CCoordPos pos;
		pos = pBenchTargetMarkPickInfo->GetMarkPlatformCoordPos(i+ m_iMarkNum*m_nAlignnExProductIndex,nMinIndex);
		dTargetX[i] = pos.m_dPosX;
		dTargetY[i] = pos.m_dPosY;
		dTargetD[i] = pos.m_dAngle;
		str.Format("目标平台坐标%d:TargetX=%f,TargetY=%f,TargetD=%f",i,dTargetX[i],dTargetY[i],dTargetD[i]);
		AlignToolLogRecord(str,nPlatformIndex);
	}
	CCoordPos cpBenchPickPlatformPos; // 基准拾取的全局平台坐标(XYD平台坐标位置)
	if (!GetPickPlatformPos(pBenchTargetMarkPickInfo->GetPickPlatformPos(0+ m_iMarkNum*m_nAlignnExProductIndex,nMinIndex), cpBenchPickPlatformPos))
	{
		return FALSE;
	}


	double dAlignerDegree = 0;
	double dAlignerX = 0;
	double dAlignerY = 0;


	double dProductX= 0;
	double dProductY = 0;
	double dProductDegree = 0;


	if (GetMidPlatformType() == ePlatformXY)
	{
		// $4、目标Mark与对象Mark在平台坐标下进行对位，计算旋转角度及XY轴偏移量
		//if (m_iMarkNum == 2)
		//点到点对位
		//			if (eDotToDotAligner == m_AlignermentParam.GetAlignerMode())
		// 计算相对移动量
		double m_dXD = 0;
		double m_dYD = 0;
		double m_dDD = 0;
		CCoordPos cpCurPickPlatformPos;

		GetPickPlatformPos(pObjectPlatformAxisPos.at(m_iMarkNum-1), cpCurPickPlatformPos);

		m_dDD = 0;
		m_dXD = cpObjectPickPlatformPos.m_dPosX - cpCurPickPlatformPos.m_dPosX;
		m_dYD = cpObjectPickPlatformPos.m_dPosY - cpCurPickPlatformPos.m_dPosY;


		dAlignerDegree = m_dDD;
		dAlignerX = m_dXD;
		dAlignerY = m_dYD;	
		str.Format("XYD方向位置偏差 X=%f,Y=%f,D=%f",dAlignerX,dAlignerY,dAlignerDegree);
		AlignToolLogRecord(str,nPlatformIndex);

		//计算产品偏差量
		dProductX =m_dXD;
		dProductY = m_dYD;
		dProductDegree = dAlignerDegree;

		str.Format("XYD方向产品偏差 X=%f,Y=%f,D=%f",dProductX,dProductY,dProductDegree);
		AlignToolLogRecord(str,nPlatformIndex);

		str.Format("------------------------------------------------------------对位计算结束");
		AlignToolLogRecord(str,nPlatformIndex);
	}
	// 			else
	// 			{
	// 				return FALSE;
	// 			}


	else
	{
		// $4、目标Mark与对象Mark在平台坐标下进行对位，计算旋转角度及XY轴偏移量
		if (m_iMarkNum == 2)
		{
			//点到点对位
			//			if (eDotToDotAligner == m_AlignermentParam.GetAlignerMode())

			// $4.1 构造目标Mark对直线L0和实时对象Mark对直线L1
			scLine L0, L1;
			scLineSeg lineSeg0(sc2Vector(dTargetX[0], dTargetY[0]), 
				sc2Vector(dTargetX[1], dTargetY[1]));
			L0.SetFromLineSeg(lineSeg0);
			scLineSeg lineSeg1(sc2Vector(dObjectX[0], dObjectY[0]), 
				sc2Vector(dObjectX[1], dObjectY[1]));
			L1.SetFromLineSeg(lineSeg1);



			// $4.2 基准mark直线到实时mark直线的角度偏差计算
			scRadian rad;
			bool bIsOK = sfAngleLine2Line(L0, L1, rad);
			double m_dDD = rad.SignedNormMod180().ToDouble()*180.0/skPI;
			str.Format("对象-目标角度=%f",m_dDD);
			AlignToolLogRecord(str,nPlatformIndex);


			// 计算相对移动量
			double m_dXD = 0;
			double m_dYD = 0;
			CCoordPos cpCurPickPlatformPos;

			GetPickPlatformPos(pObjectPlatformAxisPos.at(1), cpCurPickPlatformPos);

			m_dDD = cpBenchPickPlatformPos.m_dAngle + m_dDD - cpCurPickPlatformPos.m_dAngle;
			m_dXD = cpObjectPickPlatformPos.m_dPosX - cpCurPickPlatformPos.m_dPosX;
			m_dYD = cpObjectPickPlatformPos.m_dPosY - cpCurPickPlatformPos.m_dPosY;

			if (eAlignerMove_XY == m_AlignermentParam.GetAlignerMoveType())
			{
				m_dDD = 0;
			}

			dAlignerDegree = m_dDD;
			dAlignerX = m_dXD;
			dAlignerY = m_dYD;	
			str.Format("XYD方向位置偏差 X=%f,Y=%f,D=%f",dAlignerX,dAlignerY,dAlignerDegree);
			AlignToolLogRecord(str,nPlatformIndex);


			//计算产品偏差量
			sc2Vector scTargetMidPoint;
			double dTargetMid;
			dTargetMid = (dTargetX[0] + dTargetX[1])/2.0;
			scTargetMidPoint.SetX(dTargetMid);
			dTargetMid = (dTargetY[0] + dTargetY[1])/2.0;
			scTargetMidPoint.SetY(dTargetMid);

			sc2Vector scObjectMidPoint;
			double dObjectMid;
			dObjectMid = (dObjectX[0] + dObjectX[1])/2.0;
			scObjectMidPoint.SetX(dObjectMid);
			dObjectMid = (dObjectY[0] + dObjectY[1])/2.0;
			scObjectMidPoint.SetY(dObjectMid);

			dProductX = scTargetMidPoint.GetX() - scObjectMidPoint.GetX();
			dProductY = scTargetMidPoint.GetY() - scObjectMidPoint.GetY();
			dProductDegree = dAlignerDegree;

			str.Format("XYD方向产品偏差 X=%f,Y=%f,D=%f",dProductX,dProductY,dProductDegree);
			AlignToolLogRecord(str,nPlatformIndex);

			str.Format("------------------------------------------------------------对位计算结束");
			AlignToolLogRecord(str,nPlatformIndex);
		}
		// 			else
		// 			{
		// 				return FALSE;
		// 			}
		else
		{
			return FALSE;
		}


	}


	//	$5、赋值（不需要考虑轴方向和类型，那些在获取对位结果时计算）
	m_PlatformOffset.m_dPosX  = dAlignerX;		
	m_PlatformOffset.m_dPosY  = dAlignerY;
	m_PlatformOffset.m_dAngle = dAlignerDegree;


	m_ProductOffset.m_dPosX  = dProductX;		
	m_ProductOffset.m_dPosY  = dProductY;
	m_ProductOffset.m_dAngle = dProductDegree;

	m_ProductOffsetXYPD.m_dPosX  = 0;		
	m_ProductOffsetXYPD.m_dPosY  = 0;
	m_ProductOffsetXYPD.m_dAngle = 0;

	return TRUE;
}


// $6、当左右相机单独XY轴控制时，为完成迭代对位，计算左右相机平移量
// 	int CamPlatformType = m_pCamPlatformInfo->GetCamPlatformType();
// 	if ( eCamPlatformSepXY == CamPlatformType)
// 	{	
// 计算全局平台坐标系下，被检测的对象Mark的平台移动量
// 		for (int j=0; j<m_iMarkNum; j++)
// 		{
// 			dObjectXforCam1[j] = cos(scDegree(-1*dAlignerDegree))*dObjectXforCam0[j] 
// 							   - sin(scDegree(-1*dAlignerDegree))*dObjectYforCam0[j];
// 			dObjectYforCam1[j] = sin(scDegree(-1*dAlignerDegree))*dObjectXforCam0[j] 
// 							   + cos(scDegree(-1*dAlignerDegree))*dObjectYforCam0[j];
// 			dPltfmXforCam[j] = dObjectXforCam1[j] - dObjectXforCam0[j] + dAlignerX;
// 			dPltfmYforCam[j] = dObjectYforCam1[j] - dObjectYforCam0[j] + dAlignerY;
// 计算相机变换矩阵的逆矩阵
// 			sc2Matrix TDMatrix, TDMatrixInvert;
// 			TDMatrix = m_vAlignerCalibInfos.at(j+ m_iMarkNum*m_nAlignnExProductIndex)->GetCamPlatformTransferMatrix();
// 			TDMatrix.Inverse(TDMatrixInvert);
// 			dCameraPltfmX[j] = TDMatrix.GetElement(0,0)*dPltfmXforCam[j] + TDMatrix.GetElement(0,1)*dPltfmYforCam[j];
// 			dCameraPltfmY[j] = TDMatrix.GetElement(1,0)*dPltfmXforCam[j] + TDMatrix.GetElement(1,1)*dPltfmYforCam[j];

// 马丽加(防止内存泄漏)
// 			if (m_vCamPlatformMovement.at(j)!=NULL)
// 			{
// 				delete m_vCamPlatformMovement.at(j);
// 				m_vCamPlatformMovement.at(j) = NULL;
// 			}

// 			m_vCamPlatformMovement.at(j) = new CPlatformXYAxisPos;
// 			((CPlatformXYAxisPos*)m_vCamPlatformMovement.at(j))->m_dPosX = dCameraPltfmX[j];
// 			((CPlatformXYAxisPos*)m_vCamPlatformMovement.at(j))->m_dPosY = dCameraPltfmY[j];
// 		}
// 	}
// 	else if (eCamPlatformSepX == CamPlatformType)
// 	{
// 		for (int j=0; j<m_iMarkNum; j++)
// 		{
// 			dObjectXforCam1[j] = cos(scDegree(-1*dAlignerDegree))*dObjectXforCam0[j] 
// 				- sin(scDegree(-1*dAlignerDegree))*dObjectYforCam0[j];
// 			dObjectYforCam1[j] = sin(scDegree(-1*dAlignerDegree))*dObjectXforCam0[j] 
// 				+ cos(scDegree(-1*dAlignerDegree))*dObjectYforCam0[j];
// 			dPltfmXforCam[j] = dObjectXforCam1[j] - dObjectXforCam0[j] + dAlignerX;
// 			dPltfmYforCam[j] = dObjectYforCam1[j] - dObjectYforCam0[j] + dAlignerY;
// 计算相机变换矩阵的AXX和AXY
// 			sc2Matrix TDMatrix;
// 			TDMatrix = m_vAlignerCalibInfos.at(j+ m_iMarkNum*m_nAlignnExProductIndex)->GetCamPlatformTransferMatrix();
// 			dCameraPltfmX[j] = 0.5*(dPltfmXforCam[j]/TDMatrix.GetElement(0,0) + dPltfmYforCam[j]/TDMatrix.GetElement(0,1));
// 			dCameraPltfmY[j] = 0;

// 马丽加(防止内存泄漏)
// 			if (m_vCamPlatformMovement.at(j)!=NULL)
// 			{
// 				delete m_vCamPlatformMovement.at(j);
// 				m_vCamPlatformMovement.at(j) = NULL;
// 			}

// 			m_vCamPlatformMovement.at(j) = new CPlatformXYAxisPos;
// 			((CPlatformXYAxisPos*)m_vCamPlatformMovement.at(j))->m_dPosX = dCameraPltfmX[j];
// 			((CPlatformXYAxisPos*)m_vCamPlatformMovement.at(j))->m_dPosY = dCameraPltfmY[j];	
// 		}
// 	}
// 	else
// 	{
// 		for (int j=0; j<m_iMarkNum; j++)
// 		{
// 马丽加(防止内存泄漏)
// 			if (m_vCamPlatformMovement.at(j)!=NULL)
// 			{
// 				delete m_vCamPlatformMovement.at(j);
// 				m_vCamPlatformMovement.at(j) = NULL;
// 			}

// 			m_vCamPlatformMovement.at(j) = new CPlatformXYAxisPos;
// 			((CPlatformXYAxisPos*)m_vCamPlatformMovement.at(j))->m_dPosX = 0;
// 			((CPlatformXYAxisPos*)m_vCamPlatformMovement.at(j))->m_dPosY = 0;
// 		}
// 	}

//对位偏移量计算成功，对位次数加一
//	m_AlignermentTimes++;
// 	return TRUE;
// }



CMarkPlatformPos vcXYDAlignTool::GetObjectMarkPickPlatformPos()
{
	//针对每个相机的图像mark计算其平台坐标值
	int nPlatformIndex = m_nPlatformIndex;
	CString str;
	str.Format("对象平台坐标计算开始------------------");
	AlignToolLogRecord(str,nPlatformIndex);
	for (int i=0; i<m_iMarkNum; i++)
	{	

		if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
		{	
			str.Format("对象平台坐标%d开始",i);
			AlignToolLogRecord(str,nPlatformIndex);

			ConvertImagePosToCurPickPlatformPos(m_ObjectMarkPlatformPos.m_vPos.at(i),  i+ m_iMarkNum*m_nAlignnExProductIndex, m_ObjectSearchResults.m_vPos.at(i),	m_pObjectPlatformAxisPos.at(i));
			str.Format("对象平台坐标%d结束",i);
			AlignToolLogRecord(str,nPlatformIndex);
		}
		else
		{
			ConvertImagePosToCurPlatformPos(m_ObjectMarkPlatformPos.m_vPos.at(i),  i+ m_iMarkNum*m_nAlignnExProductIndex, m_ObjectSearchResults.m_vPos.at(i), 
				m_pObjectPlatformAxisPos.at(i), m_vObjectCamPlatformAxisPos.at(i));
		}

		m_ObjectMarkPlatformPos.m_vbOK.at(i) = TRUE;
	}
	str.Format("------------------对象平台坐标计算结束");
	AlignToolLogRecord(str,nPlatformIndex);
	return m_ObjectMarkPlatformPos;
}


int vcXYDAlignTool::GetCalibCenterPlatformMovePosNum(int nMarkType)
{
	int nMovePosNun = 0;

	if (m_bPlatformPick == TRUE) 
	{
		if (m_ePickCamPlatformType == ePickCamPlatformSepFix)	// 相机固定安装
		{
			nMovePosNun = GetCalibPlatformXYDMovePosNum();

			if (ePlatformXY == m_eMidPlatformType)
			{
				nMovePosNun = GetCalibPlatformXYMovePosNum();
			}
		}
		else if (m_ePickCamPlatformType == ePickCamPlatformShareXY)	// 相机安装在XYD吸头平台的XY轴上
		{
			if (nMarkType == 0)
			{
				// 拾取并且是相机安装在拾取头平台上的
				nMovePosNun = GetCalibCenterPlatformPickMovePosNum();
			}
			else if (nMarkType == 1)
			{
				// 拾取并且是相机安装在拾取头平台上的
				nMovePosNun = GetCalibPlatformPickMovePosNum();
			}
		}		
	}

	return nMovePosNun;

}

// XY+D拾取 - XY
int vcXYDAlignTool::GetCalibPlatformPickMovePosNum()
{
	int nMovePosNun = 0;

	nMovePosNun = 10;//14;  //只标定矩阵

	return nMovePosNun;
}

// XY+D拾取 - D
int vcXYDAlignTool::GetCalibCenterPlatformPickMovePosNum()
{
	int nMovePosNun = 0;

	nMovePosNun = 4;//14;  //只标定旋转中心

	return nMovePosNun;
}


// XY+D拾取 - XY
std::vector<CPlatformAxisPos*> vcXYDAlignTool::GetCalibPlatformPickMovePos(int nPosIndex)
{

	//将存储标定时的绝对位置的变量清空
	for (int j=0; j<m_vPlatformAxisPos.size(); j++)
	{	
		if (m_vPlatformAxisPos.at(j) != NULL)
		{
			delete m_vPlatformAxisPos.at(j);
			m_vPlatformAxisPos.at(j) = NULL;
		}
	}
	m_vPlatformAxisPos.clear();

	if (nPosIndex<0 || nPosIndex >= m_iMarkNum)
		return m_vPlatformAxisPos;

	m_CalibrateParam = m_vpCalibrateParam[nPosIndex];
	//***********************XYD平台***********************//
	//相机固定安装/或独立X安装/或独立XY轴安装时，可以正常标定
	CPlatformXYDInfo* pPlatformParam = (CPlatformXYDInfo*)m_pPlatformParam;//平台参数

	//相机平台X轴安装时，不能完成标定，直接返回空值
	if (  eCamPlatformShareX != m_pCamPlatformInfo->GetCamPlatformType())
	{

		double dOffsetX,dOffsetY,dOffsetD;
		dOffsetX = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosX;
		dOffsetY = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosY;
		dOffsetD = m_CalibrateParam->m_dPlatformCalibMovement.m_dAngle;
		double dX0,dY0,dD0;
		dX0 = ((CPlatformXYDAxisPos*)m_PlatformAxisPos)->m_dPosX;
		dY0 = ((CPlatformXYDAxisPos*)m_PlatformAxisPos)->m_dPosY;
		dD0 = ((CPlatformXYDAxisPos*)m_PlatformAxisPos)->m_dAngle;

		if (m_bCalibSendRelativePos)
		{
			dX0 = 0.0;
			dY0 = 0.0;
			dD0 = 0.0;
		}
		// 		double dMoveOffsetX,dMoveOffsetY;
		// 		dMoveOffsetX = m_CalibrateParam->m_dCameraCalibMovement.m_dPosX;
		// 		dMoveOffsetY = m_CalibrateParam->m_dCameraCalibMovement.m_dPosY;
		// 
		// 		if (eDirectNegative == pPlatformParam->m_nPlatformXCoordType)//Y轴方向为负
		// 			dMoveOffsetX = -dMoveOffsetX;
		// 		if (eDirectNegative == pPlatformParam->m_nPlatformYCoordType)//x轴方向为负
		// 			dMoveOffsetY = -dMoveOffsetY;

		//将平台坐标移动量转化为实际轴移动量
		if (eDirectNegative == pPlatformParam->m_nPlatformXCoordType)//Y轴方向为负
			dOffsetX = -dOffsetX;
		if (eDirectNegative == pPlatformParam->m_nPlatformYCoordType)//x轴方向为负
			dOffsetY = -dOffsetY;
		if (eDirectNegative == pPlatformParam->m_nPlatformDCoordType)//D轴方向为负
			dOffsetD = -dOffsetD;
		// 		if (eDriveLine == pPlatformParam->m_nPlatformDDriveType)//直线驱动
		// 			dOffsetD = tan(scDegree(dOffsetD))*pPlatformParam->m_dPlatformRotationLength;
		if (eDriveLine == pPlatformParam->m_nPlatformDDriveType)//直线驱动
		{
			double dStartAngle = 0;
			if (pPlatformParam->m_dPlatformRotationLength > DBL_EPSILON)
			{
				if (pPlatformParam->m_nPlatformDDriveLineType == eDriveLineShift)
				{
					dStartAngle = scDegree(scRadian(asin(dD0/pPlatformParam->m_dPlatformRotationLength))).ToDouble();
					dOffsetD = (sin(scDegree(dStartAngle + dOffsetD)) - sin(scDegree(dStartAngle)))*pPlatformParam->m_dPlatformRotationLength;
				}
				else
				{
					dStartAngle = scDegree(scRadian(atan(dD0/pPlatformParam->m_dPlatformRotationLength))).ToDouble();
					dOffsetD = (tan(scDegree(dStartAngle + dOffsetD)) - tan(scDegree(dStartAngle)))*pPlatformParam->m_dPlatformRotationLength;
				}
			}
		}
		//需要设置平台各轴的初始位置，即基准位置
		if (m_PlatformAxisPos != NULL)
		{

			if (m_PlatformCalibType == ePlatformCalib9Point)
			{
				//将12个位置压入，第一个和最后一个是基准位置：指针类型直接压入还是new？
				//0	
				CPlatformXYDAxisPos* AxisMovePos0 = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)m_PlatformAxisPos));
				if (m_bCalibSendRelativePos)
				{
					AxisMovePos0->m_dPosX = 0.0;
					AxisMovePos0->m_dPosY = 0.0;
					AxisMovePos0->m_dAngle = 0.0;
				}
				m_vPlatformAxisPos.push_back(AxisMovePos0);
				//1
				CPlatformXYDAxisPos* AxisMovePos1 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos1->m_dPosX  = dX0 -  dOffsetX;
				AxisMovePos1->m_dPosY  = dY0 -  dOffsetY;	
				AxisMovePos1->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos1);
				//2
				CPlatformXYDAxisPos* AxisMovePos2 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos2->m_dPosX  = dX0 -  dOffsetX;
				AxisMovePos2->m_dPosY  = dY0;	
				AxisMovePos2->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos2);
				//3
				CPlatformXYDAxisPos* AxisMovePos3 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos3->m_dPosX  = dX0 -  dOffsetX;
				AxisMovePos3->m_dPosY  = dY0 +  dOffsetY;	
				AxisMovePos3->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos3);
				//4
				CPlatformXYDAxisPos* AxisMovePos4 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos4->m_dPosX  = dX0;
				AxisMovePos4->m_dPosY  = dY0 +  dOffsetY;	
				AxisMovePos4->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos4);
				//5
				CPlatformXYDAxisPos* AxisMovePos5 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos5->m_dPosX  = dX0 +  dOffsetX;
				AxisMovePos5->m_dPosY  = dY0 +  dOffsetY;	
				AxisMovePos5->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos5);
				//6
				CPlatformXYDAxisPos* AxisMovePos6 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos6->m_dPosX  = dX0 +  dOffsetX;
				AxisMovePos6->m_dPosY  = dY0;	
				AxisMovePos6->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos6);
				//7
				CPlatformXYDAxisPos* AxisMovePos7 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos7->m_dPosX  = dX0 +  dOffsetX;
				AxisMovePos7->m_dPosY  = dY0 -  dOffsetY;	
				AxisMovePos7->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos7);
				//8
				CPlatformXYDAxisPos* AxisMovePos8 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos8->m_dPosX  = dX0;
				AxisMovePos8->m_dPosY  = dY0 -  dOffsetY;	
				AxisMovePos8->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos8);

				// 回到原位
				//9
				CPlatformXYDAxisPos* AxisMovePos9 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos9->m_dPosX  = dX0 ;
				AxisMovePos9->m_dPosY  = dY0 ;	
				AxisMovePos9->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos9);

				// 				// 移动到拾取位
				// 				//10
				// 				CPlatformXYDAxisPos* AxisMovePos10 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos10->m_dPosX  = dX0; //+ dMoveOffsetX;
				// 				AxisMovePos10->m_dPosY  = dY0 ;//+ dMoveOffsetY;	
				// 				AxisMovePos10->m_dAngle  = dD0+ dOffsetD;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos10);
				// 
				// 				// 吸取旋转放置位升起
				// 				//11
				// 				CPlatformXYDAxisPos* AxisMovePos11 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos11->m_dPosX  = dX0 ;//+ dMoveOffsetX;
				// 				AxisMovePos11->m_dPosY  = dY0 ;//+ dMoveOffsetY;	
				// 				AxisMovePos11->m_dAngle  = dD0 - dOffsetD;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos11);
				// 
				// 				// 移动回原位
				// 				//12
				// 				CPlatformXYDAxisPos* AxisMovePos12 = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)m_PlatformAxisPos));
				// 				m_vPlatformAxisPos.push_back(AxisMovePos12);
				if (m_bCalibSendRelativePos)
				{

					for (int i=m_vPlatformAxisPos.size()-1;i>0;i--)
					{
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosX = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosX-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dPosX;
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosY = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosY-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dPosY;
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dAngle = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dAngle-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dAngle;
					}

					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dPosX = 0.0;
					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dPosY = 0.0;
					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dAngle = 0.0;
				}
			}
			else
			{
				//将8个位置压入，第一个和最后一个是基准位置：指针类型直接压入还是new？
				//0	
				CPlatformXYDAxisPos* AxisMovePos0 = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)m_PlatformAxisPos));
				if (m_bCalibSendRelativePos)
				{
					AxisMovePos0->m_dPosX = 0.0;
					AxisMovePos0->m_dPosY = 0.0;
					AxisMovePos0->m_dAngle = 0.0;
				}
				m_vPlatformAxisPos.push_back(AxisMovePos0);
				//1
				CPlatformXYDAxisPos* AxisMovePos1 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos1->m_dPosX  = dX0 +  dOffsetX;
				AxisMovePos1->m_dPosY  = dY0 +  dOffsetY;	
				AxisMovePos1->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos1);
				//2
				CPlatformXYDAxisPos* AxisMovePos2 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos2->m_dPosX  = dX0 -  dOffsetX;
				AxisMovePos2->m_dPosY  = dY0 -  dOffsetY;	
				AxisMovePos2->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos2);
				//3
				CPlatformXYDAxisPos* AxisMovePos3 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos3->m_dPosX  = dX0 -  dOffsetX;
				AxisMovePos3->m_dPosY  = dY0 +  dOffsetY;	
				AxisMovePos3->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos3);
				//4
				CPlatformXYDAxisPos* AxisMovePos4 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos4->m_dPosX  = dX0 +  dOffsetX;
				AxisMovePos4->m_dPosY  = dY0 -  dOffsetY;	
				AxisMovePos4->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos4);
				//5
				CPlatformXYDAxisPos* AxisMovePos5 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos5->m_dPosX  = dX0 ;
				AxisMovePos5->m_dPosY  = dY0 ;	
				AxisMovePos5->m_dAngle  = dD0 + dOffsetD;
				m_vPlatformAxisPos.push_back(AxisMovePos5);
				//6
				CPlatformXYDAxisPos* AxisMovePos6 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos6->m_dPosX  = dX0 ;
				AxisMovePos6->m_dPosY  = dY0 ;	
				AxisMovePos6->m_dAngle  = dD0 - dOffsetD;
				m_vPlatformAxisPos.push_back(AxisMovePos6);
				//7
				CPlatformXYDAxisPos* AxisMovePos7 = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)m_PlatformAxisPos));
				if (m_bCalibSendRelativePos)
				{
					AxisMovePos7->m_dPosX = 0.0;
					AxisMovePos7->m_dPosY = 0.0;
					AxisMovePos7->m_dAngle = 0.0;
				}
				m_vPlatformAxisPos.push_back(AxisMovePos7);

				if (m_bCalibSendRelativePos)
				{

					for (int i=m_vPlatformAxisPos.size()-1;i>0;i--)
					{
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosX = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosX-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dPosX;
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosY = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosY-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dPosY;
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dAngle = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dAngle-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dAngle;
					}

					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dPosX = 0.0;
					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dPosY = 0.0;
					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dAngle = 0.0;
				}
			}
		}
	}

	return m_vPlatformAxisPos;
}

// XY+D拾取 - D
std::vector<CPlatformAxisPos*> vcXYDAlignTool::GetCalibCenterPlatformPickMovePos(int nPosIndex)
{

	//将存储标定时的绝对位置的变量清空
	for (int j=0; j<m_vPlatformAxisPos.size(); j++)
	{	
		if (m_vPlatformAxisPos.at(j) != NULL)
		{
			delete m_vPlatformAxisPos.at(j);
			m_vPlatformAxisPos.at(j) = NULL;
		}
	}
	m_vPlatformAxisPos.clear();

	if (nPosIndex<0 || nPosIndex >= m_iMarkNum)
		return m_vPlatformAxisPos;

	m_CalibrateParam = m_vpCalibrateParam[nPosIndex];
	//***********************XYD平台***********************//
	//相机固定安装/或独立X安装/或独立XY轴安装时，可以正常标定
	CPlatformXYDInfo* pPlatformParam = (CPlatformXYDInfo*)m_pPlatformParam;//平台参数

	//相机平台X轴安装时，不能完成标定，直接返回空值
	if (  eCamPlatformShareX != m_pCamPlatformInfo->GetCamPlatformType())
	{

		double dOffsetX,dOffsetY,dOffsetD;
		dOffsetX = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosX;
		dOffsetY = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosY;
		dOffsetD = m_CalibrateParam->m_dPlatformCalibMovement.m_dAngle;
		double dX0,dY0,dD0;
		dX0 = ((CPlatformXYDAxisPos*)m_PlatformAxisPos)->m_dPosX;
		dY0 = ((CPlatformXYDAxisPos*)m_PlatformAxisPos)->m_dPosY;
		dD0 = ((CPlatformXYDAxisPos*)m_PlatformAxisPos)->m_dAngle;


		if (m_bCalibSendRelativePos)
		{
			dX0 = 0.0;
			dY0 = 0.0;
			dD0 = 0.0;
		}

		// 		double dMoveOffsetX,dMoveOffsetY;
		// 		dMoveOffsetX = m_CalibrateParam->m_dCameraCalibMovement.m_dPosX;
		// 		dMoveOffsetY = m_CalibrateParam->m_dCameraCalibMovement.m_dPosY;
		// 
		// 		if (eDirectNegative == pPlatformParam->m_nPlatformXCoordType)//Y轴方向为负
		// 			dMoveOffsetX = -dMoveOffsetX;
		// 		if (eDirectNegative == pPlatformParam->m_nPlatformYCoordType)//x轴方向为负
		// 			dMoveOffsetY = -dMoveOffsetY;

		//将平台坐标移动量转化为实际轴移动量
		if (eDirectNegative == pPlatformParam->m_nPlatformXCoordType)//Y轴方向为负
			dOffsetX = -dOffsetX;
		if (eDirectNegative == pPlatformParam->m_nPlatformYCoordType)//x轴方向为负
			dOffsetY = -dOffsetY;
		if (eDirectNegative == pPlatformParam->m_nPlatformDCoordType)//D轴方向为负
			dOffsetD = -dOffsetD;
		// 		if (eDriveLine == pPlatformParam->m_nPlatformDDriveType)//直线驱动
		// 			dOffsetD = tan(scDegree(dOffsetD))*pPlatformParam->m_dPlatformRotationLength;
		if (eDriveLine == pPlatformParam->m_nPlatformDDriveType)//直线驱动
		{
			double dStartAngle = 0;
			if (pPlatformParam->m_dPlatformRotationLength > DBL_EPSILON)
			{
				if (pPlatformParam->m_nPlatformDDriveLineType == eDriveLineShift)
				{
					dStartAngle = scDegree(scRadian(asin(dD0/pPlatformParam->m_dPlatformRotationLength))).ToDouble();
					dOffsetD = (sin(scDegree(dStartAngle + dOffsetD)) - sin(scDegree(dStartAngle)))*pPlatformParam->m_dPlatformRotationLength;
				}
				else
				{
					dStartAngle = scDegree(scRadian(atan(dD0/pPlatformParam->m_dPlatformRotationLength))).ToDouble();
					dOffsetD = (tan(scDegree(dStartAngle + dOffsetD)) - tan(scDegree(dStartAngle)))*pPlatformParam->m_dPlatformRotationLength;
				}
			}
		}
		//需要设置平台各轴的初始位置，即基准位置
		if (m_PlatformAxisPos != NULL)
		{
			if (m_PlatformCalibType == ePlatformCalib9Point)
			{
				//将12个位置压入，第一个和最后一个是基准位置：指针类型直接压入还是new？
				//0	
				// 				CPlatformXYDAxisPos* AxisMovePos0 = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)m_PlatformAxisPos));
				// 				m_vPlatformAxisPos.push_back(AxisMovePos0);
				// 				//1
				// 				CPlatformXYDAxisPos* AxisMovePos1 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos1->m_dPosX  = dX0 -  dOffsetX;
				// 				AxisMovePos1->m_dPosY  = dY0 -  dOffsetY;	
				// 				AxisMovePos1->m_dAngle  = dD0;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos1);
				// 				//2
				// 				CPlatformXYDAxisPos* AxisMovePos2 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos2->m_dPosX  = dX0 -  dOffsetX;
				// 				AxisMovePos2->m_dPosY  = dY0;	
				// 				AxisMovePos2->m_dAngle  = dD0;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos2);
				// 				//3
				// 				CPlatformXYDAxisPos* AxisMovePos3 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos3->m_dPosX  = dX0 -  dOffsetX;
				// 				AxisMovePos3->m_dPosY  = dY0 +  dOffsetY;	
				// 				AxisMovePos3->m_dAngle  = dD0;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos3);
				// 				//4
				// 				CPlatformXYDAxisPos* AxisMovePos4 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos4->m_dPosX  = dX0;
				// 				AxisMovePos4->m_dPosY  = dY0 +  dOffsetY;	
				// 				AxisMovePos4->m_dAngle  = dD0;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos4);
				// 				//5
				// 				CPlatformXYDAxisPos* AxisMovePos5 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos5->m_dPosX  = dX0 +  dOffsetX;
				// 				AxisMovePos5->m_dPosY  = dY0 +  dOffsetY;	
				// 				AxisMovePos5->m_dAngle  = dD0;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos5);
				// 				//6
				// 				CPlatformXYDAxisPos* AxisMovePos6 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos6->m_dPosX  = dX0 +  dOffsetX;
				// 				AxisMovePos6->m_dPosY  = dY0;	
				// 				AxisMovePos6->m_dAngle  = dD0;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos6);
				// 				//7
				// 				CPlatformXYDAxisPos* AxisMovePos7 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos7->m_dPosX  = dX0 +  dOffsetX;
				// 				AxisMovePos7->m_dPosY  = dY0 -  dOffsetY;	
				// 				AxisMovePos7->m_dAngle  = dD0;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos7);
				// 				//8
				// 				CPlatformXYDAxisPos* AxisMovePos8 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				// 				AxisMovePos8->m_dPosX  = dX0;
				// 				AxisMovePos8->m_dPosY  = dY0 -  dOffsetY;	
				// 				AxisMovePos8->m_dAngle  = dD0;
				// 				m_vPlatformAxisPos.push_back(AxisMovePos8);

				// 回到原位
				//9
				CPlatformXYDAxisPos* AxisMovePos9 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos9->m_dPosX  = dX0 ;
				AxisMovePos9->m_dPosY  = dY0 ;	
				AxisMovePos9->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos9);

				// 移动到拾取位
				//10
				CPlatformXYDAxisPos* AxisMovePos10 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos10->m_dPosX  = dX0; //+ dMoveOffsetX;
				AxisMovePos10->m_dPosY  = dY0 ;//+ dMoveOffsetY;	
				AxisMovePos10->m_dAngle  = dD0+ dOffsetD;
				m_vPlatformAxisPos.push_back(AxisMovePos10);

				// 吸取旋转放置位升起
				//11
				CPlatformXYDAxisPos* AxisMovePos11 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos11->m_dPosX  = dX0 ;//+ dMoveOffsetX;
				AxisMovePos11->m_dPosY  = dY0 ;//+ dMoveOffsetY;	
				AxisMovePos11->m_dAngle  = dD0 - dOffsetD;
				m_vPlatformAxisPos.push_back(AxisMovePos11);

				// 移动回原位
				//12
				CPlatformXYDAxisPos* AxisMovePos12 = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)m_PlatformAxisPos));
				if (m_bCalibSendRelativePos)
				{
					AxisMovePos12->m_dPosX = 0.0;
					AxisMovePos12->m_dPosY = 0.0;
					AxisMovePos12->m_dAngle = 0.0;
				}
				m_vPlatformAxisPos.push_back(AxisMovePos12);

				if (m_bCalibSendRelativePos)
				{

					for (int i=m_vPlatformAxisPos.size()-1;i>0;i--)
					{
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosX = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosX-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dPosX;
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosY = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosY-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dPosY;
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dAngle = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dAngle-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dAngle;
					}

					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dPosX = 0.0;
					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dPosY = 0.0;
					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dAngle = 0.0;
				}
			}
			else
			{
				//将8个位置压入，第一个和最后一个是基准位置：指针类型直接压入还是new？
				//0	
				CPlatformXYDAxisPos* AxisMovePos0 = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)m_PlatformAxisPos));
				if (m_bCalibSendRelativePos)
				{
					AxisMovePos0->m_dPosX = 0.0;
					AxisMovePos0->m_dPosY = 0.0;
					AxisMovePos0->m_dAngle = 0.0;
				}
				m_vPlatformAxisPos.push_back(AxisMovePos0);
				//1
				CPlatformXYDAxisPos* AxisMovePos1 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos1->m_dPosX  = dX0 +  dOffsetX;
				AxisMovePos1->m_dPosY  = dY0 +  dOffsetY;	
				AxisMovePos1->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos1);
				//2
				CPlatformXYDAxisPos* AxisMovePos2 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos2->m_dPosX  = dX0 -  dOffsetX;
				AxisMovePos2->m_dPosY  = dY0 -  dOffsetY;	
				AxisMovePos2->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos2);
				//3
				CPlatformXYDAxisPos* AxisMovePos3 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos3->m_dPosX  = dX0 -  dOffsetX;
				AxisMovePos3->m_dPosY  = dY0 +  dOffsetY;	
				AxisMovePos3->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos3);
				//4
				CPlatformXYDAxisPos* AxisMovePos4 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos4->m_dPosX  = dX0 +  dOffsetX;
				AxisMovePos4->m_dPosY  = dY0 -  dOffsetY;	
				AxisMovePos4->m_dAngle  = dD0;
				m_vPlatformAxisPos.push_back(AxisMovePos4);
				//5
				CPlatformXYDAxisPos* AxisMovePos5 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos5->m_dPosX  = dX0 ;
				AxisMovePos5->m_dPosY  = dY0 ;	
				AxisMovePos5->m_dAngle  = dD0 + dOffsetD;
				m_vPlatformAxisPos.push_back(AxisMovePos5);
				//6
				CPlatformXYDAxisPos* AxisMovePos6 = new CPlatformXYDAxisPos;//轴移动绝对位置参数
				AxisMovePos6->m_dPosX  = dX0 ;
				AxisMovePos6->m_dPosY  = dY0 ;	
				AxisMovePos6->m_dAngle  = dD0 - dOffsetD;
				m_vPlatformAxisPos.push_back(AxisMovePos6);
				//7
				CPlatformXYDAxisPos* AxisMovePos7 = new CPlatformXYDAxisPos(*((CPlatformXYDAxisPos*)m_PlatformAxisPos));
				if (m_bCalibSendRelativePos)
				{
					AxisMovePos7->m_dPosX = 0.0;
					AxisMovePos7->m_dPosY = 0.0;
					AxisMovePos7->m_dAngle = 0.0;
				}
				m_vPlatformAxisPos.push_back(AxisMovePos7);

				if (m_bCalibSendRelativePos)
				{

					for (int i=m_vPlatformAxisPos.size()-1;i>0;i--)
					{
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosX = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosX-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dPosX;
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosY = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dPosY-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dPosY;
						((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dAngle = ((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i))->m_dAngle-((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(i-1))->m_dAngle;
					}

					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dPosX = 0.0;
					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dPosY = 0.0;
					((CPlatformXYDAxisPos*)m_vPlatformAxisPos.at(0))->m_dAngle = 0.0;
				}
			}
		}
	}

	return m_vPlatformAxisPos;
}


// XY+D平台拾取，XY标定，用9点H矩阵方式进行标定
BOOL vcXYDAlignTool::ExecuteCalibratePickSingleCamEX(int nCamIndex)
{
	if (0 == m_iMarkNum)
	{
		return FALSE;
	}	
	int nRemCamIndex = nCamIndex%m_iMarkNum;
	m_CalibrateParam = m_vpCalibrateParam[nRemCamIndex];

	//$ 1.0 计算转换矩阵

	//参数准备：平台坐标偏移量,图像坐标偏移量，方程矩阵，转换参数
	//	double dX1,dX2,dY1,dY2;//平台坐标偏移量
	//	double dx1,dy1,dx2,dy2;//图像坐标偏移量
	//	double A[4][4];	    // 4维矩阵,图像坐标矩阵
	//	double dA11, dA12, dA21, dA22;//转换矩阵
	//m_vCalibratedInfos.at(nCamIndex) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
	if (m_bEnableMultiCalibExtension == TRUE)
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			for (int j = 0; j < m_nMultiCalibExtensionMaxNum+1; j++)
			{
				if (k + m_iMarkNum*j < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k + m_iMarkNum*j] != NULL)
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(*m_vAlignerCalibInfos[k + m_iMarkNum*j]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}
	else
	{
		int k = 0;
		for(k = 0; k < m_iMarkNum; k++)
		{
			//if (k != nCamIndex)
			{
				if (k < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k] != NULL)
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(*m_vAlignerCalibInfos[k]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}


	//获取该相机对应的图像坐标

	double MarkdX[10],MarkdY[10];
	int i=0;
	for (i=0; i<10; i++)
	{
		MarkdX[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosX;
		MarkdY[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosY;
	}

	//设置基准位置处mark的图像坐标，平台轴位置，相机轴位置
	m_vCalibratedInfos.at(nCamIndex)->SetMarkImgCoordPos(m_vCalibPlatformMoveResults[nRemCamIndex].at(0));
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformAxisPos(m_PlatformAxisPos);
	//	m_vCalibratedInfos.at(nCamIndex)->SetCamPlatformAxisPos(m_vCamPlatformInitAxisPos.at(nCamIndex));

	std::vector<sc2Vector> vImageMark;
	for(i = 0; i < 9; i++)
	{
		vImageMark.push_back(sc2Vector(MarkdX[i], MarkdY[i]) - sc2Vector(MarkdX[0], MarkdY[0]));
	}

	//平台平面坐标 //相机随着轴一起运动，假设相机不动，所拍摄的物体在动，实际运动距离与设置的相反?????
	double dPosX =  m_CalibrateParam->m_dPlatformCalibMovement.m_dPosX;
	double dPosY =  m_CalibrateParam->m_dPlatformCalibMovement.m_dPosY;
	std::vector<sc2Vector> vPlatMove;
	vPlatMove.push_back(sc2Vector(0, 0));
	vPlatMove.push_back(sc2Vector(+dPosX, +dPosY));
	vPlatMove.push_back(sc2Vector(+dPosX, 0));
	vPlatMove.push_back(sc2Vector(+dPosX, -dPosY));
	vPlatMove.push_back(sc2Vector(0, -dPosY));
	vPlatMove.push_back(sc2Vector(-dPosX, -dPosY));
	vPlatMove.push_back(sc2Vector(-dPosX, 0));
	vPlatMove.push_back(sc2Vector(-dPosX, +dPosY));
	vPlatMove.push_back(sc2Vector(0, +dPosY));

	// 计算H变换矩阵
	double h[9];
	memset(h, 0.0, sizeof(double)*9);
	//if (m_bPlatformPick)
	{
		CString strh;
		HomographyDOF dof = GetHomographyDOF();

		//		HomographyDOF dof = m_CalibrateParam->GetHomographyDOF();
		switch(dof)
		{
		case eAuto:
			strh="eAuto";
			break;
		case eScaleRotate:
			strh="eScaleRotate";
			break;
		case eScaleXYRotateXY:
			strh="eScaleXYRotateXY";
			break;
		case eProject:
			strh="eProject";
			break;
		default:
			dof =eAuto;
			strh="WrongParam , turn to eAuto";
			break;
		}
		strh=_T("当前标定H矩阵算法为:")+strh;
		AlignToolLogRecord(strh,m_nPlatformIndex);

		if(!sfGetHomography(vImageMark,vPlatMove,h,dof))
		{
			return FALSE;
		}
	}
	//else
	//{
	//	if(!sfGetHomography(vImageMark,vPlatMove,h))
	//	{
	//		return FALSE;
	//	}
	//}

	//记录变换结果
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformTransferH(h);


	//把9个位置的图像坐标保存
	for (int m=0; m<9; m++)
	{
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosX = m_vCalibPlatformMoveResults[nRemCamIndex].at(m).m_dPosX;
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosY = m_vCalibPlatformMoveResults[nRemCamIndex].at(m).m_dPosY;
	}

	// 单相机拾取时，不同位置拷贝相应标定结果
	int nExProductIdx = nCamIndex/m_iMarkNum;
	/*for(int k = 0; k < m_iMarkNum; k++)*/
	for(int k = nExProductIdx*m_iMarkNum; k < (nExProductIdx+1)*m_iMarkNum; k++)
	{
		if (k != nCamIndex)
		{
			*(m_vCalibratedInfos.at(k)) = *(m_vCalibratedInfos.at(nCamIndex));
		}
	}

	// 
	// 	double tanValue1,tanValue2;
	// 	double dA1 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(0,0);
	// 	double dA2 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(0,1);
	// 	double dA3 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(1,0);
	// 	double dA4 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(1,1);
	// 
	// 	tanValue1 = dA21/dA11;
	// 	tanValue2 = (-1*dA12)/dA22;
	// 	tanValue1 = (arctan(tanValue1).ToDouble())*180.0/skPI;
	// 	tanValue2 = (arctan(tanValue2).ToDouble())*180.0/skPI;	
	// 
	// 	m_vCalibratedInfos.at(nCamIndex)->m_dImageToPlatformAnlge = (tanValue1 + tanValue2)/2.0;

	return  TRUE;
}

// XY+D平台拾取，D标定， 用9点H矩阵方式进行标定
BOOL vcXYDAlignTool::ExecuteCalibrateCenterPickSingleCamEX(int nCamIndex)
{
	CString str;
	if (0 == m_iMarkNum)
	{
		return FALSE;
	}	
	int nRemCamIndex = nCamIndex%m_iMarkNum;
	m_CalibrateParam = m_vpCalibrateParam[nRemCamIndex];

	//$ 1.0 计算转换矩阵

	//参数准备：平台坐标偏移量,图像坐标偏移量，方程矩阵，转换参数
	//	double dX1,dX2,dY1,dY2;//平台坐标偏移量
	//	double dx1,dy1,dx2,dy2;//图像坐标偏移量
	//	double A[4][4];	    // 4维矩阵,图像坐标矩阵
	//	double dA11, dA12, dA21, dA22;//转换矩阵
	//m_vCalibratedInfos.at(nCamIndex) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
	if (m_bEnableMultiCalibExtension == TRUE)
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			for (int j = 0; j < m_nMultiCalibExtensionMaxNum+1; j++)
			{
				if (k + m_iMarkNum*j < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k + m_iMarkNum*j] != NULL)
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(*m_vAlignerCalibInfos[k + m_iMarkNum*j]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}
	else
	{
		int k = 0;
		for(k = 0; k < m_iMarkNum; k++)
		{
			//if (k != nCamIndex)
			{
				if (k < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k] != NULL)
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(*m_vAlignerCalibInfos[k]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}


	//获取该相机对应的图像坐标
	if (m_vCalibPlatformMoveResults[nRemCamIndex].size()!= CALIBMOVEPOS_XYNUM+CALIBMOVEPOS_DNUM)
	{
		return FALSE;
	}

	double MarkdX[14],MarkdY[14];
	int i=0;
	for (i=0; i<14; i++)
	{
		MarkdX[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosX;
		MarkdY[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosY;

	}

	//设置基准位置处mark的图像坐标，平台轴位置，相机轴位置
	m_vCalibratedInfos.at(nCamIndex)->SetMarkImgCoordPos(m_vCalibPlatformMoveResults[nRemCamIndex].at(0));
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformAxisPos(m_PlatformAxisPos);
	// 	//	m_vCalibratedInfos.at(nCamIndex)->SetCamPlatformAxisPos(m_vCamPlatformInitAxisPos.at(nCamIndex));
	// 
	// 	std::vector<sc2Vector> vImageMark;
	// 	for(i = 0; i < 9; i++)
	// 	{
	// 		vImageMark.push_back(sc2Vector(MarkdX[i], MarkdY[i]) - sc2Vector(MarkdX[0], MarkdY[0]));
	// 	}

	//平台平面坐标
	// 	double dPosX = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosX;
	// 	double dPosY = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosY;
	// 	std::vector<sc2Vector> vPlatMove;
	// 	vPlatMove.push_back(sc2Vector(0, 0));
	// 	vPlatMove.push_back(sc2Vector(+dPosX, +dPosY));
	// 	vPlatMove.push_back(sc2Vector(+dPosX, 0));
	// 	vPlatMove.push_back(sc2Vector(+dPosX, -dPosY));
	// 	vPlatMove.push_back(sc2Vector(0, -dPosY));
	// 	vPlatMove.push_back(sc2Vector(-dPosX, -dPosY));
	// 	vPlatMove.push_back(sc2Vector(-dPosX, 0));
	// 	vPlatMove.push_back(sc2Vector(-dPosX, +dPosY));
	// 	vPlatMove.push_back(sc2Vector(0, +dPosY));

	// 计算H变换矩阵
	double h[9];
	memset(h, 0.0, sizeof(double)*9);

	// 	if(!sfGetHomography(vImageMark,vPlatMove,h))
	// 	{
	// 		return FALSE;
	// 	}

	//记录变换结果
	/*	m_vCalibratedInfos.at(nCamIndex)->SetPlatformTransferH(h);*/
	m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferH(h);	 

	//$ 2.0 计算基准mark平台坐标
	scDegree  degRotationDegree;		// 正转旋转角度
	sc2Vector vBenchMarkImagePos;		// 基准点Mark图像坐标
	sc2Vector vStartMarkImagePos;		// 起始点（基准点）Mark图像坐标

	sc2Vector vRotatedMarkImagePos1;	// 正转后Mark图像坐标
	sc2Vector vRotatedMarkImagePos2;	// 反转后Mark图像坐标

	sc2Vector vBenchMarkPlatformPos1;	// 正转计算得到基准点Mark的平台坐标
	sc2Vector vBenchMarkPlatformPos2;   // 反转计算得到基准点Mark的平台坐标

	degRotationDegree = scDegree(m_CalibrateParam->m_dPlatformCalibMovement.m_dAngle);

	vBenchMarkImagePos.SetX(MarkdX[0]);
	vBenchMarkImagePos.SetY(MarkdY[0]);

	vStartMarkImagePos.SetX(MarkdX[10]);
	vStartMarkImagePos.SetY(MarkdY[10]);

	vRotatedMarkImagePos1.SetX(MarkdX[11]);
	vRotatedMarkImagePos1.SetY(MarkdY[11]);

	vRotatedMarkImagePos2.SetX(MarkdX[12]);
	vRotatedMarkImagePos2.SetY(MarkdY[12]);

	// $2.1、正向旋转Δθ角,计算基准mark平台坐标
	if (!vfCalibBenchMarkPlatformPos(vStartMarkImagePos, vRotatedMarkImagePos1, degRotationDegree, h, 
		vBenchMarkImagePos, vBenchMarkPlatformPos1))
	{
		return FALSE;
	}


	// $2.2、反向旋转Δθ角,计算基准mark平台坐标
	if (!vfCalibBenchMarkPlatformPos(vStartMarkImagePos, vRotatedMarkImagePos2, -degRotationDegree, h, 
		vBenchMarkImagePos, vBenchMarkPlatformPos2))
	{
		return FALSE;
	}

	// $2.3、记录基准Mark点的平台坐标值
	CCoordPos pos;
	pos.m_dPosX = (vBenchMarkPlatformPos1.GetX() + vBenchMarkPlatformPos2.GetX())/2.0;
	pos.m_dPosY = (vBenchMarkPlatformPos1.GetY() + vBenchMarkPlatformPos2.GetY())/2.0;
	m_vCalibratedInfos.at(nCamIndex)->SetMarkPlatformCoordPos(pos);	

	str.Format("基准点图像坐标X=%f,Y=%f",vBenchMarkImagePos.GetX(),vBenchMarkImagePos.GetY());
	str.Format("基准点平台坐标X=%f,Y=%f",pos.m_dPosX,pos.m_dPosY);
	AlignToolLogRecord(str,m_nPlatformIndex);


	//$3.0、计算图像坐标与平台坐标之间的夹角
	m_vCalibratedInfos.at(nCamIndex)->m_dImageToPlatformAnlge = 0;
	m_vCalibratedInfos.at(nCamIndex)->SetValid(TRUE);

	//把三个位置（11正转/12反转/13零位）的图像坐标保存到【9】【10】【11】
	for (int m=9; m<CALIBMOVEPOSNUM; m++)
	{
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosX = m_vCalibPlatformMoveResults[nRemCamIndex].at(m+2).m_dPosX;
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosY = m_vCalibPlatformMoveResults[nRemCamIndex].at(m+2).m_dPosY;
	}

	// 单相机拾取时，不同位置拷贝相应标定结果
	int nExProductIdx = nCamIndex/m_iMarkNum;
	/*for(k = 0; k < m_iMarkNum; k++)*/
	for(int k = nExProductIdx*m_iMarkNum; k < (nExProductIdx+1)*m_iMarkNum; k++)
	{
		if (k != nCamIndex)
		{
			*(m_vCalibratedInfos.at(k)) = *(m_vCalibratedInfos.at(nCamIndex));
		}
	}

	// 
	// 	double tanValue1,tanValue2;
	// 	double dA1 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(0,0);
	// 	double dA2 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(0,1);
	// 	double dA3 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(1,0);
	// 	double dA4 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(1,1);
	// 
	// 	tanValue1 = dA21/dA11;
	// 	tanValue2 = (-1*dA12)/dA22;
	// 	tanValue1 = (arctan(tanValue1).ToDouble())*180.0/skPI;
	// 	tanValue2 = (arctan(tanValue2).ToDouble())*180.0/skPI;	
	// 
	// 	m_vCalibratedInfos.at(nCamIndex)->m_dImageToPlatformAnlge = (tanValue1 + tanValue2)/2.0;

	return  TRUE;
}

// XY平台拾取,用9点H矩阵方式进行标定
BOOL vcXYDAlignTool::ExecuteCalibrateXYSingleCamPickEX(int nCamIndex)
{
	if (0 == m_iMarkNum)
	{
		return FALSE;
	}

	// 	if (m_pCamPlatformInfo->GetCamNum()!=1)
	// 	{
	// 		return FALSE;
	// 	}
	int nRemCamIndex = nCamIndex%m_iMarkNum;
	m_CalibrateParam = m_vpCalibrateParam[nRemCamIndex];

	//$ 1.0 计算转换矩阵

	//参数准备：平台坐标偏移量,图像坐标偏移量，方程矩阵，转换参数
	//	double dX1,dX2,dY1,dY2;//平台坐标偏移量
	//	double dx1,dy1,dx2,dy2;//图像坐标偏移量
	//	double A[4][4];	    // 4维矩阵,图像坐标矩阵
	//	double dA11, dA12, dA21, dA22;//转换矩阵
	//	m_vCalibratedInfos.at(nCamIndex) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
	if (m_bEnableMultiCalibExtension == TRUE)
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			for (int j = 0; j < m_nMultiCalibExtensionMaxNum+1; j++)
			{
				if (k + m_iMarkNum*j < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k + m_iMarkNum*j] != NULL)
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(*m_vAlignerCalibInfos[k + m_iMarkNum*j]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}
	else
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			//		if (k != nCamIndex)
			{
				if (k < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k] != NULL)
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(*m_vAlignerCalibInfos[k]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}

	//获取该相机对应的图像坐标
	double MarkdX[10],MarkdY[10];
	int i=0;

	for (i=0; i<10; i++)
	{
		MarkdX[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosX;
		MarkdY[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosY;	


	}

	//设置基准位置处mark的图像坐标，平台轴位置，相机轴位置
	m_vCalibratedInfos.at(nCamIndex)->SetMarkImgCoordPos(m_vCalibPlatformMoveResults[nRemCamIndex].at(0));
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformAxisPos(m_PlatformAxisPos);
	//	m_vCalibratedInfos.at(nCamIndex)->SetCamPlatformAxisPos(m_vCamPlatformInitAxisPos.at(nCamIndex));

	std::vector<sc2Vector> vImageMark;
	for(i = 0; i < 9; i++)
	{
		vImageMark.push_back(sc2Vector(MarkdX[i], MarkdY[i]) - sc2Vector(MarkdX[0], MarkdY[0]));
	}

	//平台平面坐标
	double dPosX = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosX;
	double dPosY = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosY;
	std::vector<sc2Vector> vPlatMove;
	vPlatMove.push_back(sc2Vector(0, 0));
	vPlatMove.push_back(sc2Vector(+dPosX, +dPosY));
	vPlatMove.push_back(sc2Vector(+dPosX, 0));
	vPlatMove.push_back(sc2Vector(+dPosX, -dPosY));
	vPlatMove.push_back(sc2Vector(0, -dPosY));
	vPlatMove.push_back(sc2Vector(-dPosX, -dPosY));
	vPlatMove.push_back(sc2Vector(-dPosX, 0));
	vPlatMove.push_back(sc2Vector(-dPosX, +dPosY));
	vPlatMove.push_back(sc2Vector(0, +dPosY));


	// 计算H变换矩阵
	double h[9];
	memset(h, 0.0, sizeof(double)*9);
	if (m_bPlatformPick)
	{
		CString strh;
		HomographyDOF dof = GetHomographyDOF();
		//		HomographyDOF dof = m_CalibrateParam->GetHomographyDOF();
		switch(dof)
		{
		case eAuto:
			strh="eAuto";
			break;
		case eScaleRotate:
			strh="eScaleRotate";
			break;
		case eScaleXYRotateXY:
			strh="eScaleXYRotateXY";
			break;
		case eProject:
			strh="eProject";
			break;
		default:
			dof =eAuto;
			strh="WrongParam , turn to eAuto";
			break;
		}
		strh=_T("当前标定H矩阵算法为:")+strh;
		AlignToolLogRecord(strh,m_nPlatformIndex);

		if(!sfGetHomography(vImageMark,vPlatMove,h,dof))
		{
			return FALSE;
		}
	}
	else
	{
		if(!sfGetHomography(vImageMark,vPlatMove,h))
		{
			return FALSE;
		}
	}


	//记录变换结果
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformTransferH(h);

	//把九个位置的图像坐标保存
	for (int m=0; m<9; m++)
	{
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosX = m_vCalibPlatformMoveResults[nRemCamIndex].at(m).m_dPosX;
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosY = m_vCalibPlatformMoveResults[nRemCamIndex].at(m).m_dPosY;
	}

	//记录变换结果
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformTransferH(h);


	//$ 2.0 计算基准mark平台坐标

	// $2.3、记录基准Mark点的平台坐标值

	CCoordPos pos;
	//if (nCamIndex==0)
	{
		pos.m_dPosX = 0;
		pos.m_dPosY = 0;
		m_vCalibratedInfos.at(nCamIndex)->SetMarkPlatformCoordPos(pos);	
	}
	// 	else
	// 	{
	// 		CPlatformAxisPos* 				pBasePlatformAxisPos;
	// 		if (m_vCalibratedInfos.at(0)!=NULL)
	// 		{
	// 			pBasePlatformAxisPos = m_vCalibratedInfos.at(0)->GetPlatformAxisPos();
	// 
	// 			pos.m_dPosX = ((CPlatformXYDAxisPos*) m_PlatformAxisPos)->m_dPosX - ((CPlatformXYDAxisPos*)pBasePlatformAxisPos)->m_dPosX;
	// 			pos.m_dPosY = ((CPlatformXYDAxisPos*) m_PlatformAxisPos)->m_dPosY - ((CPlatformXYDAxisPos*)pBasePlatformAxisPos)->m_dPosY;
	// 			m_vCalibratedInfos.at(nCamIndex)->SetMarkPlatformCoordPos(pos);	
	// 		}
	// 
	// 	}

	//$3.0、计算图像坐标与平台坐标之间的夹角
	m_vCalibratedInfos.at(nCamIndex)->m_dImageToPlatformAnlge = 0;
	m_vCalibratedInfos.at(nCamIndex)->SetValid(TRUE);


	// 单相机拾取时，不同位置拷贝相应标定结果
	int nExProductIdx = nCamIndex/m_iMarkNum;
	for(int k = nExProductIdx*m_iMarkNum; k < (nExProductIdx+1)*m_iMarkNum; k++)
	{
		if (k != nCamIndex)
		{
			*(m_vCalibratedInfos.at(k)) = *(m_vCalibratedInfos.at(nCamIndex));
		}
	}
	// 
	// 	double tanValue1,tanValue2;
	// 	double dA1 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(0,0);
	// 	double dA2 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(0,1);
	// 	double dA3 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(1,0);
	// 	double dA4 = m_vCalibratedInfos.at(nCamIndex)->GetPlatformTransferMatrix().GetElement(1,1);
	// 
	// 	tanValue1 = dA21/dA11;
	// 	tanValue2 = (-1*dA12)/dA22;
	// 	tanValue1 = (arctan(tanValue1).ToDouble())*180.0/skPI;
	// 	tanValue2 = (arctan(tanValue2).ToDouble())*180.0/skPI;	
	// 
	// 	m_vCalibratedInfos.at(nCamIndex)->m_dImageToPlatformAnlge = (tanValue1 + tanValue2)/2.0;

	return  TRUE;
}

// XY平台拾取,用5点H矩阵方式进行标定
BOOL vcXYDAlignTool::ExecuteCalibrateXYSingleCamPick( int nCamIndex )
{
	if (0 == m_iMarkNum)
	{
		return FALSE;
	}	
	int nRemCamIndex = nCamIndex%m_iMarkNum;
	m_CalibrateParam = m_vpCalibrateParam[nRemCamIndex];

	//$ 1.0 计算转换矩阵

	//参数准备：平台坐标偏移量,图像坐标偏移量，方程矩阵，转换参数
	double dX1,dX2,dY1,dY2;//平台坐标偏移量
	double dx1,dy1,dx2,dy2;//图像坐标偏移量
	double A[4][4];	    // 4维矩阵,图像坐标矩阵
	double dA11, dA12, dA21, dA22;//转换矩阵
	//m_vCalibratedInfos.at(nCamIndex) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
	if (m_bEnableMultiCalibExtension == TRUE)
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			for (int j = 0; j < m_nMultiCalibExtensionMaxNum+1; j++)
			{
				if (k + m_iMarkNum*j < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k + m_iMarkNum*j] != NULL)
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(*m_vAlignerCalibInfos[k + m_iMarkNum*j]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}
	else
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			//if (k != nCamIndex)
			{
				if (k < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k] != NULL)
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(*m_vAlignerCalibInfos[k]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}


	//获取该相机对应的图像坐标
	double MarkdX[6],MarkdY[6];
	for (int i=0; i<6; i++)
	{
		MarkdX[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosX;
		MarkdY[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosY;
	}

	//设置基准位置处mark的图像坐标，平台轴位置，相机轴位置
	m_vCalibratedInfos.at(nCamIndex)->SetMarkImgCoordPos(m_vCalibPlatformMoveResults[nRemCamIndex].at(0));
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformAxisPos(m_PlatformAxisPos);
	//	m_vCalibratedInfos.at(nCamIndex)->SetCamPlatformAxisPos(m_vCamPlatformInitAxisPos.at(nCamIndex));
	// 平台坐标系变换矩阵计算之相机由点1到点2、由点3到点4：平台坐标偏移量，图像坐标偏移量
	dX1 = -2*m_CalibrateParam->m_dPlatformCalibMovement.m_dPosX;
	dY1 = -2*m_CalibrateParam->m_dPlatformCalibMovement.m_dPosY;
	dX2 = 2*m_CalibrateParam->m_dPlatformCalibMovement.m_dPosX;
	dY2 = -2*m_CalibrateParam->m_dPlatformCalibMovement.m_dPosY;

	dx1 = MarkdX[2]-MarkdX[1];
	dy1 = MarkdY[2]-MarkdY[1];
	dx2 = MarkdX[4]-MarkdX[3];
	dy2 = MarkdY[4]-MarkdY[3];


	A[0][0] = dx1;
	A[0][1] = dy1;
	A[0][2] = 0;
	A[0][3] = 0;

	A[1][0] = 0;
	A[1][1] = 0;
	A[1][2] = dx1;
	A[1][3] = dy1;

	A[2][0] = dx2;
	A[2][1] = dy2;
	A[2][2] = 0;
	A[2][3] = 0;

	A[3][0] = 0;
	A[3][1] = 0;
	A[3][2] = dx2;
	A[3][3] = dy2;

	if (0 == MatrixInvert(A, 4))
	{
		return FALSE;
	}

	dA11 = A[0][0]*dX1 + A[0][1]*dY1 + A[0][2]*dX2 + A[0][3]*dY2;
	dA12 = A[1][0]*dX1 + A[1][1]*dY1 + A[1][2]*dX2 + A[1][3]*dY2;
	dA21 = A[2][0]*dX1 + A[2][1]*dY1 + A[2][2]*dX2 + A[2][3]*dY2;
	dA22 = A[3][0]*dX1 + A[3][1]*dY1 + A[3][2]*dX2 + A[3][3]*dY2;

	//记录变换结果
	sc2Matrix matrix(dA11,dA12,dA21,dA22);

	m_vCalibratedInfos.at(nCamIndex)->SetPlatformTransferMatrix(matrix);


	//$ 2.0 计算基准mark平台坐标

	CCoordPos pos;
	//	if (nCamIndex==0)
	{
		pos.m_dPosX = 0;
		pos.m_dPosY = 0;
		m_vCalibratedInfos.at(nCamIndex)->SetMarkPlatformCoordPos(pos);	
	}
	// 	else
	// 	{
	// 		CPlatformAxisPos* 				pBasePlatformAxisPos;
	// 		if (m_vCalibratedInfos.at(0)!=NULL)
	// 		{
	// 			pBasePlatformAxisPos = m_vCalibratedInfos.at(0)->GetPlatformAxisPos();
	// 
	// 			pos.m_dPosX = ((CPlatformXYDAxisPos*) m_PlatformAxisPos)->m_dPosX - ((CPlatformXYDAxisPos*)pBasePlatformAxisPos)->m_dPosX;
	// 			pos.m_dPosY = ((CPlatformXYDAxisPos*) m_PlatformAxisPos)->m_dPosY - ((CPlatformXYDAxisPos*)pBasePlatformAxisPos)->m_dPosY;
	// 			m_vCalibratedInfos.at(nCamIndex)->SetMarkPlatformCoordPos(pos);	
	// 		}
	// 
	// 	}




	//$3.0、计算图像坐标与平台坐标之间的夹角
	m_vCalibratedInfos.at(nCamIndex)->m_dImageToPlatformAnlge = 0;
	m_vCalibratedInfos.at(nCamIndex)->SetValid(TRUE);

	// 单相机拾取时，不同位置拷贝相应标定结果
	int nExProductIdx = nCamIndex/m_iMarkNum;
	/*for(int k = 0; k < m_iMarkNum; k++)*/
	for(int k = nExProductIdx*m_iMarkNum; k < (nExProductIdx+1)*m_iMarkNum; k++)
	{
		if (k != nCamIndex)
		{
			*(m_vCalibratedInfos.at(k)) = *(m_vCalibratedInfos.at(nCamIndex));
		}
	}

	return  TRUE;
}

BOOL vcXYDAlignTool::ExecuteCalibrateSingleCamXYWholeDmCodeBoardBoardToPlatPick(int nCamIndex)//全局二维码标定时拾取XY平台靶标到平台坐标映射
{
	if (0 == m_iMarkNum)
	{
		return FALSE;
	}	

	CString strInfo;
	strInfo.Format(_T("位置%d:全局二维码靶标标定靶标到平台映射计算开始--------------"),nCamIndex);
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	int nRemCamIndex = nCamIndex%m_iMarkNum;

	m_CalibrateParam = m_vpCalibrateParam[nRemCamIndex];

	//$ 1.0 计算转换矩阵

	//参数准备：平台坐标偏移量,图像坐标偏移量，方程矩阵，转换参数
	//	double dX1,dX2,dY1,dY2;//平台坐标偏移量
	//	double dx1,dy1,dx2,dy2;//图像坐标偏移量
	//	double A[4][4];	    // 4维矩阵,图像坐标矩阵
	//	double dA11, dA12, dA21, dA22;//转换矩阵
	//	m_vCalibratedInfos.at(nCamIndex) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());

	if (m_bEnableMultiCalibExtension == TRUE)
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			for (int j = 0; j < m_nMultiCalibExtensionMaxNum+1; j++)
			{
				if (k + m_iMarkNum*j < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k + m_iMarkNum*j] != NULL)
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(*m_vAlignerCalibInfos[k + m_iMarkNum*j]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}
	else
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			//		if (k != nCamIndex)
			{
				if (k < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k] != NULL)
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(*m_vAlignerCalibInfos[k]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}


	//获取该相机对应的图像坐标

	double MarkdX[10],MarkdY[10];
	double BoardMarkX[10],BoardMarkY[10];
	sc2Vector boardMark;
	sc2Vector imgMark;
	int i=0;
	for (i=0; i<10; i++)
	{
		MarkdX[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosX;
		MarkdY[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosY;
		imgMark.SetX(MarkdX[i]);
		imgMark.SetY(MarkdY[i]);
		boardMark = sfHomographyProject(imgMark,m_vCalibratedInfos.at(nCamIndex)->m_PlatformTranferH1);
		BoardMarkX[i] = boardMark.GetX();
		BoardMarkY[i] = boardMark.GetY();
	}

	strInfo.Format(_T("位置%d图像坐标："),nCamIndex);
	CString strTmpInfo;
	for (i=0;i<10;i++)
	{
		strTmpInfo.Format(_T(" %.3f,%.3f"),MarkdX[i],MarkdY[i]);
		strInfo += strTmpInfo;
	}
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	strInfo.Format(_T("位置%d靶标坐标："),nCamIndex);
	for (i=0;i<10;i++)
	{
		strTmpInfo.Format(_T(" %.3f,%.3f"),BoardMarkX[i],BoardMarkY[i]);
		strInfo += strTmpInfo;
	}
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	//设置基准位置处mark的图像坐标，平台轴位置，相机轴位置
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformAxisPos(m_PlatformAxisPos);
	//	m_vCalibratedInfos.at(nCamIndex)->SetCamPlatformAxisPos(m_vCamPlatformInitAxisPos.at(nCamIndex));

	std::vector<sc2Vector> vBoardMark;
	for(i = 0; i < 9; i++)
	{		
		vBoardMark.push_back(sc2Vector(BoardMarkX[i], BoardMarkY[i])-sc2Vector(BoardMarkX[0], BoardMarkY[0]));
	}

	//平台平面坐标
	double dPosX = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosX;
	double dPosY = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosY;
	std::vector<sc2Vector> vPlatMove;
	vPlatMove.push_back(sc2Vector(0, 0));
	vPlatMove.push_back(sc2Vector(+dPosX, +dPosY));
	vPlatMove.push_back(sc2Vector(+dPosX, 0));
	vPlatMove.push_back(sc2Vector(+dPosX, -dPosY));
	vPlatMove.push_back(sc2Vector(0, -dPosY));
	vPlatMove.push_back(sc2Vector(-dPosX, -dPosY));
	vPlatMove.push_back(sc2Vector(-dPosX, 0));
	vPlatMove.push_back(sc2Vector(-dPosX, +dPosY));
	vPlatMove.push_back(sc2Vector(0, +dPosY));

	// 计算H变换矩阵
	double h2[9];
	memset(h2, 0.0, sizeof(double)*9);
	if (m_bPlatformPick)
	{
		CString strh;
		HomographyDOF dof = GetHomographyDOF();
		//		HomographyDOF dof = m_CalibrateParam->GetHomographyDOF();
		switch(dof)
		{
		case eAuto:
			strh="eAuto";
			break;
		case eScaleRotate:
			strh="eScaleRotate";
			break;
		case eScaleXYRotateXY:
			strh="eScaleXYRotateXY";
			break;
		case eProject:
			strh="eProject";
			break;
		default:
			dof =eAuto;
			strh="WrongParam , turn to eAuto";
			break;
		}
		strh=_T("当前标定H矩阵算法为:")+strh;
		AlignToolLogRecord(strh,m_nPlatformIndex);

		if(!sfGetHomography(vBoardMark,vPlatMove,h2,dof))
		{
			return FALSE;
		}
	}
	else
	{
		if(!sfGetHomography(vBoardMark,vPlatMove,h2))
		{
			return FALSE;
		}
	}

	strInfo.Format(_T("靶标到平台坐标映射矩阵:"));
	for (int i=0;i<9;i++)
	{
		strTmpInfo.Format(_T(" h2[%d]:%.6f"),i,h2[i]);
		strInfo = strInfo + strTmpInfo;
	}
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	//记录变换结果
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformTransferH2(h2);
	double h1[9];
	memset(h1, 0.0, sizeof(double)*9);
	m_vAlignerCalibInfos.at(nCamIndex)->GetPlatformTransferH1(h1);

	double h[9];
	memset(h, 0.0, sizeof(double)*9);
	h[0]=h2[0]*h1[0]+h2[1]*h1[3]+h2[2]*h1[6];
	h[1]=h2[0]*h1[1]+h2[1]*h1[4]+h2[2]*h1[7];
	h[2]=h2[0]*h1[2]+h2[1]*h1[5]+h2[2]*h1[8];

	h[3]=h2[3]*h1[0]+h2[4]*h1[3]+h2[5]*h1[6];
	h[4]=h2[3]*h1[1]+h2[4]*h1[4]+h2[5]*h1[7];
	h[5]=h2[3]*h1[2]+h2[4]*h1[5]+h2[5]*h1[8];

	h[6]=h2[6]*h1[0]+h2[7]*h1[3]+h2[8]*h1[6];
	h[7]=h2[6]*h1[1]+h2[7]*h1[4]+h2[8]*h1[7];
	h[8]=h2[6]*h1[2]+h2[7]*h1[5]+h2[8]*h1[8];

	strInfo.Format(_T("图像到平台坐标映射矩阵:"));
	for (int i=0;i<9;i++)
	{
		strTmpInfo.Format(_T(" h[%d]:%.6f"),i,h[i]);
		strInfo = strInfo + strTmpInfo;
	}
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	double dis = 0 ;

	svStd vector<sc2Vector> vOffset;
	if (!sfGetHomographyMaxErrorInverseProject(vBoardMark,vPlatMove,h2,dis,vOffset))
	{
		return FALSE;
	}
	CCoordPos errorOffset;
	errorOffset.m_dPosX = dis;
	errorOffset.m_dPosY = dis;
	m_vCalibratedInfos.at(nCamIndex)->SetMarkPlatformCoordPosRefineOffsetH2(errorOffset);
	strInfo.Format(_T("h2矩阵映射误差:%.3f"),dis);
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	m_vCalibratedInfos.at(nCamIndex)->SetPlatformTransferH(h);

	m_vCalibratedInfos.at(nCamIndex)->SetMarkPlatformCoordPos(CCoordPos());


	//把九个位置的图像坐标保存
	for (int m=0; m<9; m++)
	{
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosX = m_vCalibPlatformMoveResults[nRemCamIndex].at(m).m_dPosX;
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosY = m_vCalibPlatformMoveResults[nRemCamIndex].at(m).m_dPosY;
	}


	//$3.0、计算图像坐标与平台坐标之间的夹角
	m_vCalibratedInfos.at(nCamIndex)->m_dImageToPlatformAnlge = 0;
	m_vCalibratedInfos.at(nCamIndex)->SetValid(TRUE);

	// 单相机拾取时，不同位置拷贝相应标定结果
	int nExProductIdx = nCamIndex/m_iMarkNum;
	for(int k = nExProductIdx*m_iMarkNum; k < (nExProductIdx+1)*m_iMarkNum; k++)
	{
		if (k != nCamIndex)
		{
			*(m_vCalibratedInfos.at(k)) = *(m_vCalibratedInfos.at(nCamIndex));
		}
	}

	strInfo.Format(_T("位置%d:全局二维码靶标标定靶标到平台映射计算结束--------------"),nCamIndex);
	AlignToolLogRecord(strInfo,m_nPlatformIndex);
	return  TRUE;
}

BOOL vcXYDAlignTool::ExecuteCalibrateSingleCamWholeDmCodeBoardBoardToPlatPick(int nCamIndex)//全局二维码标定时拾取相机shareXY时靶标到平台坐标映射
{
	if (0 == m_iMarkNum)
	{
		return FALSE;
	}	

	CString strInfo;
	strInfo.Format(_T("位置%d:全局二维码靶标标定靶标到平台映射计算开始--------------"),nCamIndex);
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	int nRemCamIndex = nCamIndex%m_iMarkNum;

	m_CalibrateParam = m_vpCalibrateParam[nRemCamIndex];

	//$ 1.0 计算转换矩阵

	//参数准备：平台坐标偏移量,图像坐标偏移量，方程矩阵，转换参数
	//	double dX1,dX2,dY1,dY2;//平台坐标偏移量
	//	double dx1,dy1,dx2,dy2;//图像坐标偏移量
	//	double A[4][4];	    // 4维矩阵,图像坐标矩阵
	//	double dA11, dA12, dA21, dA22;//转换矩阵
	//	m_vCalibratedInfos.at(nCamIndex) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());

	if (m_bEnableMultiCalibExtension == TRUE)
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			for (int j = 0; j < m_nMultiCalibExtensionMaxNum+1; j++)
			{
				if (k + m_iMarkNum*j < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k + m_iMarkNum*j] != NULL)
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(*m_vAlignerCalibInfos[k + m_iMarkNum*j]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k + m_iMarkNum*j) != NULL)
					{
						delete m_vCalibratedInfos.at(k + m_iMarkNum*j);
						m_vCalibratedInfos.at(k + m_iMarkNum*j) = NULL;
					}
					m_vCalibratedInfos.at(k + m_iMarkNum*j) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}
	else
	{
		for(int k = 0; k < m_iMarkNum; k++)
		{
			//		if (k != nCamIndex)
			{
				if (k < m_vAlignerCalibInfos.size() && m_vAlignerCalibInfos[k] != NULL)
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(*m_vAlignerCalibInfos[k]);
				}
				else
				{
					if (m_vCalibratedInfos.at(k) != NULL)
					{
						delete m_vCalibratedInfos.at(k);
						m_vCalibratedInfos.at(k) = NULL;
					}
					m_vCalibratedInfos.at(k) = new CCalibratedInfo(m_pPlatformParam->GetPlatformType(),m_pCamPlatformInfo->GetCamPlatformType());
				}
			}
		}
	}


	//获取该相机对应的图像坐标

	double MarkdX[10],MarkdY[10];
	double BoardMarkX[10],BoardMarkY[10];
	sc2Vector boardMark;
	sc2Vector imgMark;
	int i=0;
	for (i=0; i<10; i++)
	{
		MarkdX[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosX;
		MarkdY[i] = m_vCalibPlatformMoveResults[nRemCamIndex].at(i).m_dPosY;
		imgMark.SetX(MarkdX[i]);
		imgMark.SetY(MarkdY[i]);
		boardMark = sfHomographyProject(imgMark,m_vCalibratedInfos.at(nCamIndex)->m_PlatformTranferH1);
		BoardMarkX[i] = boardMark.GetX();
		BoardMarkY[i] = boardMark.GetY();
	}

	strInfo.Format(_T("位置%d图像坐标："),nCamIndex);
	CString strTmpInfo;
	for (i=0;i<10;i++)
	{
		strTmpInfo.Format(_T(" %.3f,%.3f"),MarkdX[i],MarkdY[i]);
		strInfo += strTmpInfo;
	}
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	strInfo.Format(_T("位置%d靶标坐标："),nCamIndex);
	for (i=0;i<10;i++)
	{
		strTmpInfo.Format(_T(" %.3f,%.3f"),BoardMarkX[i],BoardMarkY[i]);
		strInfo += strTmpInfo;
	}
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	//设置基准位置处mark的图像坐标，平台轴位置，相机轴位置
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformAxisPos(m_PlatformAxisPos);
	//	m_vCalibratedInfos.at(nCamIndex)->SetCamPlatformAxisPos(m_vCamPlatformInitAxisPos.at(nCamIndex));

	std::vector<sc2Vector> vBoardMark;
	for(i = 0; i < 9; i++)
	{		
		vBoardMark.push_back(sc2Vector(BoardMarkX[i], BoardMarkY[i])-sc2Vector(BoardMarkX[0], BoardMarkY[0]));
	}

	//平台平面坐标
	double dPosX = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosX;
	double dPosY = m_CalibrateParam->m_dPlatformCalibMovement.m_dPosY;
	std::vector<sc2Vector> vPlatMove;
	vPlatMove.push_back(sc2Vector(0, 0));
	vPlatMove.push_back(sc2Vector(+dPosX, +dPosY));
	vPlatMove.push_back(sc2Vector(+dPosX, 0));
	vPlatMove.push_back(sc2Vector(+dPosX, -dPosY));
	vPlatMove.push_back(sc2Vector(0, -dPosY));
	vPlatMove.push_back(sc2Vector(-dPosX, -dPosY));
	vPlatMove.push_back(sc2Vector(-dPosX, 0));
	vPlatMove.push_back(sc2Vector(-dPosX, +dPosY));
	vPlatMove.push_back(sc2Vector(0, +dPosY));

	// 计算H变换矩阵
	double h2[9];
	memset(h2, 0.0, sizeof(double)*9);
	if (m_bPlatformPick)
	{
		CString strh;
		HomographyDOF dof = GetHomographyDOF();
		//		HomographyDOF dof = m_CalibrateParam->GetHomographyDOF();
		switch(dof)
		{
		case eAuto:
			strh="eAuto";
			break;
		case eScaleRotate:
			strh="eScaleRotate";
			break;
		case eScaleXYRotateXY:
			strh="eScaleXYRotateXY";
			break;
		case eProject:
			strh="eProject";
			break;
		default:
			dof =eAuto;
			strh="WrongParam , turn to eAuto";
			break;
		}
		strh=_T("当前标定H矩阵算法为:")+strh;
		AlignToolLogRecord(strh,m_nPlatformIndex);

		if(!sfGetHomography(vBoardMark,vPlatMove,h2,dof))
		{
			return FALSE;
		}
	}
	else
	{
		if(!sfGetHomography(vBoardMark,vPlatMove,h2))
		{
			return FALSE;
		}
	}

	strInfo.Format(_T("靶标到平台坐标映射矩阵:"));
	for (int i=0;i<9;i++)
	{
		strTmpInfo.Format(_T(" h2[%d]:%.6f"),i,h2[i]);
		strInfo = strInfo + strTmpInfo;
	}
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	double dis = 0 ;

	svStd vector<sc2Vector> vOffset;
	if (!sfGetHomographyMaxErrorInverseProject(vBoardMark,vPlatMove,h2,dis,vOffset))
	{
		return FALSE;
	}
	CCoordPos errorOffset;
	errorOffset.m_dPosX = dis;
	errorOffset.m_dPosY = dis;
	m_vCalibratedInfos.at(nCamIndex)->SetMarkPlatformCoordPosRefineOffsetH2(errorOffset);
	strInfo.Format(_T("h2矩阵映射误差:%.3f"),dis);
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	//记录变换结果
	m_vCalibratedInfos.at(nCamIndex)->SetPlatformTransferH2(h2);
	double h1[9];
	memset(h1, 0.0, sizeof(double)*9);
	m_vAlignerCalibInfos.at(nCamIndex)->GetPlatformTransferH1(h1);

	double h[9];
	memset(h, 0.0, sizeof(double)*9);
	h[0]=h2[0]*h1[0]+h2[1]*h1[3]+h2[2]*h1[6];
	h[1]=h2[0]*h1[1]+h2[1]*h1[4]+h2[2]*h1[7];
	h[2]=h2[0]*h1[2]+h2[1]*h1[5]+h2[2]*h1[8];

	h[3]=h2[3]*h1[0]+h2[4]*h1[3]+h2[5]*h1[6];
	h[4]=h2[3]*h1[1]+h2[4]*h1[4]+h2[5]*h1[7];
	h[5]=h2[3]*h1[2]+h2[4]*h1[5]+h2[5]*h1[8];

	h[6]=h2[6]*h1[0]+h2[7]*h1[3]+h2[8]*h1[6];
	h[7]=h2[6]*h1[1]+h2[7]*h1[4]+h2[8]*h1[7];
	h[8]=h2[6]*h1[2]+h2[7]*h1[5]+h2[8]*h1[8];

	strInfo.Format(_T("图像到平台坐标映射矩阵:"));
	for (int i=0;i<9;i++)
	{
		strTmpInfo.Format(_T(" h[%d]:%.6f"),i,h[i]);
		strInfo = strInfo + strTmpInfo;
	}
	AlignToolLogRecord(strInfo,m_nPlatformIndex);

	m_vCalibratedInfos.at(nCamIndex)->SetPlatformTransferH(h);

	m_vCalibratedInfos.at(nCamIndex)->SetMarkPlatformCoordPos(CCoordPos());


	//把九个位置的图像坐标保存
	for (int m=0; m<9; m++)
	{
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosX = m_vCalibPlatformMoveResults[nRemCamIndex].at(m).m_dPosX;
		m_vCalibratedInfos.at(nCamIndex)->m_cpObjectMarkImgCoordPos[m].m_dPosY = m_vCalibPlatformMoveResults[nRemCamIndex].at(m).m_dPosY;
	}


	//$3.0、计算图像坐标与平台坐标之间的夹角
	m_vCalibratedInfos.at(nCamIndex)->m_dImageToPlatformAnlge = 0;
	//m_vCalibratedInfos.at(nCamIndex)->SetValid(TRUE);

	// 单相机拾取时，不同位置拷贝相应标定结果
	int nExProductIdx = nCamIndex/m_iMarkNum;
	for(int k = nExProductIdx*m_iMarkNum; k < (nExProductIdx+1)*m_iMarkNum; k++)
	{
		if (k != nCamIndex)
		{
			*(m_vCalibratedInfos.at(k)) = *(m_vCalibratedInfos.at(nCamIndex));
		}
	}

	strInfo.Format(_T("位置%d:全局二维码靶标标定靶标到平台映射计算结束--------------"),nCamIndex);
	AlignToolLogRecord(strInfo,m_nPlatformIndex);
	return  TRUE;
}



CMarkPlatformPos vcXYDAlignTool::GetBendInsTargetPlatformPos()
{
	//针对每个相机的图像mark计算其平台坐标值
	int nPlatformIndex = m_nPlatformIndex;
	CString str;
	str.Format("折弯检测目标平台坐标计算开始------------------");
	AlignToolLogRecord(str,nPlatformIndex);

	int i=0;
	CMarkPlatformPos TargetPlatformPos;
	TargetPlatformPos.m_vPos.resize(m_iMarkNum);
	TargetPlatformPos.m_vbOK.resize(m_iMarkNum);


	str.Format("m_bEnableMultiCalibExtension = %d",m_bEnableMultiCalibExtension);
	AlignToolLogRecord(str,nPlatformIndex);

	str.Format("m_bBendAlignProcess = %d",m_bBendAlignProcess);
	AlignToolLogRecord(str,nPlatformIndex);

	str.Format("m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode = %d",m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode);
	AlignToolLogRecord(str,nPlatformIndex);

	str.Format("m_AlignermentParam.GetBendInfo().m_bBendingInspect = %d",m_AlignermentParam.GetBendInfo().m_bBendingInspect);
	AlignToolLogRecord(str,nPlatformIndex);

	str.Format("m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode = %d",m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode);
	AlignToolLogRecord(str,nPlatformIndex);


	// 产品扩展
	if (m_bEnableMultiCalibExtension)
	{
		// FAAL，对位过程中检测变量：m_bBendAlignProcess == ture
		if (m_bBendAlignProcess)
		{
			// 为了兼容正常的产品扩展功能，启用二维码检测时，使用最后一个（最大）扩展平台序号
			if (m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode)
			{
				for (i=0; i<m_iMarkNum; i++)
				{	
					if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
					{
						str.Format("目标平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nMultiCalibExtensionMaxNum, m_TargetSearchResults.m_vPos.at(i), m_pTargetPlatformAxisPos.at(i));

						str.Format("目标平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}
					else
					{
						str.Format("目标平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						if (m_bEnableMultiCalibExtension)
						{
							*((CPlatformXYAxisPos*)(m_vTargetCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamPlatformAxisPosMultiEx.at(m_nMultiCalibExtensionMaxNum).at(i)));
							*((CPlatformXYAxisPos*)(m_vTargetCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamAxisPlatformAxisPosMultiEx.at(m_nMultiCalibExtensionMaxNum).at(i)));
						}

						ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nMultiCalibExtensionMaxNum, m_TargetSearchResults.m_vPos.at(i), 
							m_pTargetPlatformAxisPos.at(i), m_vTargetCamPlatformAxisPos.at(i), m_vTargetCamAxisPlatformAxisPos.at(i));

						str.Format("目标平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);

					}

				}
			}
			// 不启用二维码检测，使用正常流程中的标定信息，跟通信指令无关，都是通过折弯参数界面选择
			else
			{
				for (i=0; i<m_iMarkNum; i++)
				{	
					if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
					{
						str.Format("目标平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_TargetSearchResults.m_vPos.at(i), m_pTargetPlatformAxisPos.at(i));

						str.Format("目标平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}
					else
					{
						str.Format("目标平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						if (m_bEnableMultiCalibExtension)
						{
							*((CPlatformXYAxisPos*)(m_vTargetCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
							*((CPlatformXYAxisPos*)(m_vTargetCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamAxisPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
						}

						ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_TargetSearchResults.m_vPos.at(i), 
							m_pTargetPlatformAxisPos.at(i), m_vTargetCamPlatformAxisPos.at(i), m_vTargetCamAxisPlatformAxisPos.at(i));

						str.Format("目标平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);

					}

				}
			}
		}
		// FCHB，m_bBendAlignProcess == false
		else
		{
			// 1. 必须启用折弯界面的FCHB指令，才能使用两区域或者两几何定位工具，并返回检测结果；
			// 2. 启用折弯界面的FCHB，并且使用二维码检测
			if (m_AlignermentParam.GetBendInfo().m_bBendingInspect && m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode)
			{
				for (i=0; i<m_iMarkNum; i++)
				{	
					if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
					{
						str.Format("目标平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nMultiCalibExtensionMaxNum, m_TargetSearchResults.m_vPos.at(i), m_pTargetPlatformAxisPos.at(i));

						str.Format("目标平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);

					}
					else
					{
						str.Format("目标平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						if (m_bEnableMultiCalibExtension)
						{
							*((CPlatformXYAxisPos*)(m_vTargetCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamPlatformAxisPosMultiEx.at(m_nMultiCalibExtensionMaxNum).at(i)));
							*((CPlatformXYAxisPos*)(m_vTargetCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamAxisPlatformAxisPosMultiEx.at(m_nMultiCalibExtensionMaxNum).at(i)));
						}

						ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nMultiCalibExtensionMaxNum, m_TargetSearchResults.m_vPos.at(i), 
							m_pTargetPlatformAxisPos.at(i), m_vTargetCamPlatformAxisPos.at(i), m_vTargetCamAxisPlatformAxisPos.at(i));

						str.Format("目标平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);

					}

				}
			}
			else
			{
				for (i=0; i<m_iMarkNum; i++)
				{	
					if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
					{
						str.Format("目标平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_TargetSearchResults.m_vPos.at(i), m_pTargetPlatformAxisPos.at(i));

						str.Format("目标平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}
					else
					{
						str.Format("目标平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						if (m_bEnableMultiCalibExtension)
						{
							*((CPlatformXYAxisPos*)(m_vTargetCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
							*((CPlatformXYAxisPos*)(m_vTargetCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamAxisPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
						}

						ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_TargetSearchResults.m_vPos.at(i), 
							m_pTargetPlatformAxisPos.at(i), m_vTargetCamPlatformAxisPos.at(i), m_vTargetCamAxisPlatformAxisPos.at(i));

						str.Format("目标平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);

					}

				}
			}
		}
	}
	else
	{
		for (i=0; i<m_iMarkNum; i++)
		{	
			if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
			{
				str.Format("目标平台坐标%d开始",i);
				AlignToolLogRecord(str,nPlatformIndex);

				ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_TargetSearchResults.m_vPos.at(i), m_pTargetPlatformAxisPos.at(i));

				str.Format("目标平台坐标%d结束",i);
				AlignToolLogRecord(str,nPlatformIndex);
			}
			else
			{
				str.Format("目标平台坐标%d开始",i);
				AlignToolLogRecord(str,nPlatformIndex);

				if (m_bEnableMultiCalibExtension)
				{
					*((CPlatformXYAxisPos*)(m_vTargetCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
					*((CPlatformXYAxisPos*)(m_vTargetCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpTargetCamAxisPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
				}

				ConvertBendDmCodeImagePosToPlatformPos(TargetPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_TargetSearchResults.m_vPos.at(i), 
					m_pTargetPlatformAxisPos.at(i), m_vTargetCamPlatformAxisPos.at(i), m_vTargetCamAxisPlatformAxisPos.at(i));

				str.Format("目标平台坐标%d结束",i);
				AlignToolLogRecord(str,nPlatformIndex);

			}

		}
	}

	// 把各位置的平台坐标归一化到指定平台位置
	CPlatformXYDInfo* pPlatformParam = (CPlatformXYDInfo*)m_pPlatformParam;
	for (i=0; i<m_iMarkNum; i++)
	{
		// $2、实时目标Mark的图像坐标值转化为平台坐标值
		//目标mark为实时mark
		str.Format("目标图像坐标%d:TargetImageX=%f,TargetImageY=%f,TargetImageD=%f",i,m_TargetSearchResults.m_vPos.at(i).m_dPosX,m_TargetSearchResults.m_vPos.at(i).m_dPosY,m_TargetSearchResults.m_vPos.at(i).m_dAngle);
		AlignToolLogRecord(str,nPlatformIndex);
		str.Format("目标平台坐标%d:TargetX=%f,TargetY=%f,TargetD=%f",i,TargetPlatformPos.m_vPos.at(i).m_dPosX,TargetPlatformPos.m_vPos.at(i).m_dPosY,TargetPlatformPos.m_vPos.at(i).m_dAngle);
		AlignToolLogRecord(str,nPlatformIndex);

		// $3、对目标Mark的平台坐标值进行修正(平台XY轴移动影响影响结果)
		double dPlatformOffsetX = 0;
		double dPlatformOffsetY = 0;
		int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
		int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;

		if (eTargetMoveByPlatform == m_eAlignerTargetMoveType)
		{
			dPlatformOffsetX = ( (CPlatformXYDAxisPos*)m_pObjectPlatformAxisPos.at(i) )->m_dPosX - ((CPlatformXYDAxisPos*)m_pTargetPlatformAxisPos.at(i))->m_dPosX;
			if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;

			dPlatformOffsetY = ((CPlatformXYDAxisPos*)m_pObjectPlatformAxisPos.at(i))->m_dPosY - ((CPlatformXYDAxisPos*)m_pTargetPlatformAxisPos.at(i))->m_dPosY;
			if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;

		}
		else
		{
			if (m_bTargetObjectCamSeparate)
			{
				dPlatformOffsetX = ((CPlatformXYDAxisPos*)m_pObjectPlatformAxisPos.at(m_pObjectPlatformAxisPos.size()/2 - 1))->m_dPosX - ((CPlatformXYDAxisPos*)m_pTargetPlatformAxisPos.at(i))->m_dPosX;
				if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;

				dPlatformOffsetY = ((CPlatformXYDAxisPos*)m_pObjectPlatformAxisPos.at(m_pObjectPlatformAxisPos.size()/2 -1))->m_dPosY - ((CPlatformXYDAxisPos*)m_pTargetPlatformAxisPos.at(i))->m_dPosY;
				if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;
			}
			else
			{
				dPlatformOffsetX = ((CPlatformXYDAxisPos*)m_pObjectPlatformAxisPos.at(m_pObjectPlatformAxisPos.size()-1))->m_dPosX - ((CPlatformXYDAxisPos*)m_pTargetPlatformAxisPos.at(i))->m_dPosX;
				if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;

				dPlatformOffsetY = ((CPlatformXYDAxisPos*)m_pObjectPlatformAxisPos.at(m_pObjectPlatformAxisPos.size()-1))->m_dPosY - ((CPlatformXYDAxisPos*)m_pTargetPlatformAxisPos.at(i))->m_dPosY;
				if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;
			}
		}


		if (m_bEnableMultiCalibExtension)
		{
			// FAAL
			if (m_bBendAlignProcess)
			{
				// 使用二维码检测判断条件
				if ( (m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode) && (eDmCodeCalib == m_AlignermentParam.GetCalibTargetMarkPatternMode()))
				{
					dPlatformOffsetX = 0;
					dPlatformOffsetY = 0;
				}
				else
				{

				}
			}
			// FCHB
			else
			{
				if ( (m_AlignermentParam.GetBendInfo().m_bBendingInspect)  && (m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode) && (eDmCodeCalib == m_AlignermentParam.GetCalibTargetMarkPatternMode()))
				{
					dPlatformOffsetX = 0;
					dPlatformOffsetY = 0;
				}
				else
				{

				}
			}
		}
		else
		{		

		}

		TargetPlatformPos.m_vPos.at(i).m_dPosX -= dPlatformOffsetX;
		TargetPlatformPos.m_vPos.at(i).m_dPosY -= dPlatformOffsetY;

		str.Format("平台轴运动修正后目标平台坐标%d:TargetX=%f,TargetY=%f,TargetD=%f",i,TargetPlatformPos.m_vPos.at(i).m_dPosX,TargetPlatformPos.m_vPos.at(i).m_dPosY,TargetPlatformPos.m_vPos.at(i).m_dAngle);
		AlignToolLogRecord(str,nPlatformIndex);

	}

	str.Format("------------------折弯检测目标平台坐标计算结束");
	AlignToolLogRecord(str,nPlatformIndex);

	return TargetPlatformPos;

}

CMarkPlatformPos vcXYDAlignTool::GetBendInsObjectPlatformPos()
{

	//针对每个相机的图像mark计算其平台坐标值
	int nPlatformIndex = m_nPlatformIndex;
	CString str;
	str.Format("折弯检测对象平台坐标计算开始------------------");
	AlignToolLogRecord(str,nPlatformIndex);

	CMarkPlatformPos ObjectPlatformPos;
	ObjectPlatformPos.m_vPos.resize(m_iMarkNum);
	ObjectPlatformPos.m_vbOK.resize(m_iMarkNum);

	str.Format("m_bEnableMultiCalibExtension = %d",m_bEnableMultiCalibExtension);
	AlignToolLogRecord(str,nPlatformIndex);

	str.Format("m_bBendAlignProcess = %d",m_bBendAlignProcess);
	AlignToolLogRecord(str,nPlatformIndex);

	str.Format("m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode = %d",m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode);
	AlignToolLogRecord(str,nPlatformIndex);

	str.Format("m_AlignermentParam.GetBendInfo().m_bBendingInspect = %d",m_AlignermentParam.GetBendInfo().m_bBendingInspect);
	AlignToolLogRecord(str,nPlatformIndex);

	str.Format("m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode = %d",m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode);
	AlignToolLogRecord(str,nPlatformIndex);

	// 产品扩展
	if (m_bEnableMultiCalibExtension)
	{
		// FAAL，对位过程中检测变量：m_bBendAlignProcess == ture
		if (m_bBendAlignProcess)
		{
			// 为了兼容正常的产品扩展功能，启用二维码检测时，使用最后一个（最大）扩展平台序号
			if (m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode)
			{
				for (int i=0; i<m_iMarkNum; i++)
				{
					if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
					{	
						str.Format("对象平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nMultiCalibExtensionMaxNum, m_ObjectSearchResults.m_vPos.at(i), m_pObjectPlatformAxisPos.at(i));

						str.Format("对象平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}
					else
					{
						str.Format("对象平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						if (m_bEnableMultiCalibExtension)
						{
							*((CPlatformXYAxisPos*)(m_vObjectCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamPlatformAxisPosMultiEx.at(m_nMultiCalibExtensionMaxNum).at(i)));
							*((CPlatformXYAxisPos*)(m_vObjectCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamAxisPlatformAxisPosMultiEx.at(m_nMultiCalibExtensionMaxNum).at(i)));
						}

						ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nMultiCalibExtensionMaxNum, m_ObjectSearchResults.m_vPos.at(i), 
							m_pObjectPlatformAxisPos.at(i), m_vObjectCamPlatformAxisPos.at(i),m_vObjectCamAxisPlatformAxisPos.at(i));

						str.Format("对象平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}

				}
			}
			// 不启用二维码检测，使用正常流程中的标定信息，跟通信指令无关，都是通过折弯参数界面选择
			else
			{
				for (int i=0; i<m_iMarkNum; i++)
				{
					if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
					{	
						str.Format("对象平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_ObjectSearchResults.m_vPos.at(i), m_pObjectPlatformAxisPos.at(i));

						str.Format("对象平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}
					else
					{
						str.Format("对象平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						if (m_bEnableMultiCalibExtension)
						{
							*((CPlatformXYAxisPos*)(m_vObjectCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
							*((CPlatformXYAxisPos*)(m_vObjectCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamAxisPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
						}

						ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_ObjectSearchResults.m_vPos.at(i), 
							m_pObjectPlatformAxisPos.at(i), m_vObjectCamPlatformAxisPos.at(i),m_vObjectCamAxisPlatformAxisPos.at(i));

						str.Format("对象平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}

				}
			}
		}
		// FCHB，m_bBendAlignProcess == false
		else
		{
			// 1. 必须启用折弯界面的FCHB指令，才能使用两区域或者两几何定位工具，并返回检测结果；
			// 2. 启用折弯界面的FCHB，并且使用二维码检测
			if (m_AlignermentParam.GetBendInfo().m_bBendingInspect && m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode)
			{
				for (int i=0; i<m_iMarkNum; i++)
				{
					if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
					{	
						str.Format("对象平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nMultiCalibExtensionMaxNum, m_ObjectSearchResults.m_vPos.at(i), m_pObjectPlatformAxisPos.at(i));

						str.Format("对象平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}
					else
					{
						str.Format("对象平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						if (m_bEnableMultiCalibExtension)
						{
							*((CPlatformXYAxisPos*)(m_vObjectCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamPlatformAxisPosMultiEx.at(m_nMultiCalibExtensionMaxNum).at(i)));
							*((CPlatformXYAxisPos*)(m_vObjectCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamAxisPlatformAxisPosMultiEx.at(m_nMultiCalibExtensionMaxNum).at(i)));
						}

						ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nMultiCalibExtensionMaxNum, m_ObjectSearchResults.m_vPos.at(i), 
							m_pObjectPlatformAxisPos.at(i), m_vObjectCamPlatformAxisPos.at(i),m_vObjectCamAxisPlatformAxisPos.at(i));

						str.Format("对象平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}

				}
			}
			else
			{
				for (int i=0; i<m_iMarkNum; i++)
				{
					if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
					{	
						str.Format("对象平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_ObjectSearchResults.m_vPos.at(i), m_pObjectPlatformAxisPos.at(i));

						str.Format("对象平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}
					else
					{
						str.Format("对象平台坐标%d开始",i);
						AlignToolLogRecord(str,nPlatformIndex);

						if (m_bEnableMultiCalibExtension)
						{
							*((CPlatformXYAxisPos*)(m_vObjectCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
							*((CPlatformXYAxisPos*)(m_vObjectCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamAxisPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
						}

						ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_ObjectSearchResults.m_vPos.at(i), 
							m_pObjectPlatformAxisPos.at(i), m_vObjectCamPlatformAxisPos.at(i),m_vObjectCamAxisPlatformAxisPos.at(i));

						str.Format("对象平台坐标%d结束",i);
						AlignToolLogRecord(str,nPlatformIndex);
					}

				}
			}
		}
	}
	else
	{
		for (int i=0; i<m_iMarkNum; i++)
		{
			if (eCamPlatformSepX != m_pCamPlatformInfo->GetCamPlatformType() && eCamPlatformSepXY != m_pCamPlatformInfo->GetCamPlatformType() )
			{	
				str.Format("对象平台坐标%d开始",i);
				AlignToolLogRecord(str,nPlatformIndex);

				ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_ObjectSearchResults.m_vPos.at(i), m_pObjectPlatformAxisPos.at(i));

				str.Format("对象平台坐标%d结束",i);
				AlignToolLogRecord(str,nPlatformIndex);
			}
			else
			{
				str.Format("对象平台坐标%d开始",i);
				AlignToolLogRecord(str,nPlatformIndex);

				if (m_bEnableMultiCalibExtension)
				{
					*((CPlatformXYAxisPos*)(m_vObjectCamPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
					*((CPlatformXYAxisPos*)(m_vObjectCamAxisPlatformAxisPos.at(i)))=*((CPlatformXYAxisPos*)(m_vvpObjectCamAxisPlatformAxisPosMultiEx.at(m_nAlignnExProductIndex).at(i)));
				}

				ConvertBendDmCodeImagePosToPlatformPos(ObjectPlatformPos.m_vPos.at(i),  i + m_iMarkNum*m_nAlignnExProductIndex, m_ObjectSearchResults.m_vPos.at(i), 
					m_pObjectPlatformAxisPos.at(i), m_vObjectCamPlatformAxisPos.at(i),m_vObjectCamAxisPlatformAxisPos.at(i));

				str.Format("对象平台坐标%d结束",i);
				AlignToolLogRecord(str,nPlatformIndex);
			}

		}
	}


	str.Format("------------------折弯检测对象平台坐标计算结束");
	AlignToolLogRecord(str,nPlatformIndex);
	return ObjectPlatformPos;


}

BOOL vcXYDAlignTool::ExeBendInsInfoCalcu(std::vector<sc2Vector>& vTargetPlaftormPos, std::vector<sc2Vector>& vObjectPlatformPos, std::vector<double>& vdDistance)
{
	// 说明：
	// （1）该接口函数仅适用于两相机实时目标，实时对象或者四相机目标对象分离的方式；
	// （2）该接口函数将两相机和四相机分离的方式统一到一起进行计算；
	// （3）折弯检测使用平台坐标进行计算，统一使用这一个接口函数；
	// （4）折弯检测使用图像坐标进行计算，统一使用流程中获取的图像坐标进行计算和显示，跟对位工具里边的图像坐标没有任何关系；

	CString str;
	int nPlatformIndex = m_nPlatformIndex;
	str.Format(_T("折弯检测开始-----"));
	AlignToolLogRecord(str, nPlatformIndex);

	if (m_bEnableMultiCalibExtension)
	{
		// 产品扩展，对位过程中检测，使用最后一个扩展序号的二维码标定信息进行检测
		if (m_bBendAlignProcess)
		{
			if (m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode)
			{
				for (int i = 0; i < m_iMarkNum; i++)
				{
					if (NULL == m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nMultiCalibExtensionMaxNum))
					{
						m_nAlignErrNum = -1;
						return false;
					}

					if (!m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nMultiCalibExtensionMaxNum)->IsValid())
					{
						m_nAlignErrNum = -2;
						return false;
					}
				}
			}
			else
			{
				for (int i = 0; i < m_iMarkNum; i++)
				{
					if (NULL == m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nAlignnExProductIndex))
					{
						m_nAlignErrNum = -1;
						return false;
					}

					if (!m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nAlignnExProductIndex)->IsValid())
					{
						m_nAlignErrNum = -2;
						return false;
					}
				}
			}
		}
		else
		{
			if (m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode)
			{
				for (int i = 0; i < m_iMarkNum; i++)
				{
					if (NULL == m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nMultiCalibExtensionMaxNum))
					{
						m_nAlignErrNum = -1;
						return false;
					}

					if (!m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nMultiCalibExtensionMaxNum)->IsValid())
					{
						m_nAlignErrNum = -2;
						return false;
					}
				}
			}
			else
			{
				for (int i = 0; i < m_iMarkNum; i++)
				{
					if (NULL == m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nAlignnExProductIndex))
					{
						m_nAlignErrNum = -1;
						return false;
					}

					if (!m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nAlignnExProductIndex)->IsValid())
					{
						m_nAlignErrNum = -2;
						return false;
					}
				}
			}
		}
	}
	else
	{		
		for (int i = 0; i < m_iMarkNum; i++)
		{
			if (NULL == m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nAlignnExProductIndex))
			{
				m_nAlignErrNum = -1;
				return false;
			}

			if (!m_vAlignerCalibInfos.at(i + m_iMarkNum*m_nAlignnExProductIndex)->IsValid())
			{
				m_nAlignErrNum = -2;
				return false;
			}
		}
	}




	// 1. 获取目标平台坐标；
	CMarkPlatformPos TargetMarkPlatformPos = GetBendInsTargetPlatformPos();

	double dTargetPlatfromCH0_X = 0, dTargetPlatfromCH0_Y = 0, dTargetPlatfromCH1_X = 0, dTargetPlatfromCH1_Y = 0;
	if ( (m_bTargetObjectCamSeparate) )
	{
		dTargetPlatfromCH0_X = TargetMarkPlatformPos.m_vPos.at(2).GetPosX();
		dTargetPlatfromCH0_Y = TargetMarkPlatformPos.m_vPos.at(2).GetPosY();

		dTargetPlatfromCH1_X = TargetMarkPlatformPos.m_vPos.at(3).GetPosX();
		dTargetPlatfromCH1_Y = TargetMarkPlatformPos.m_vPos.at(3).GetPosY();

		str.Format(_T("目标CH2平台坐标 X = %f, Y = %f"), dTargetPlatfromCH0_X, dTargetPlatfromCH0_Y);
		AlignToolLogRecord(str,m_nPlatformIndex);

		str.Format(_T("目标CH3平台坐标 X = %f, Y = %f"), dTargetPlatfromCH1_X, dTargetPlatfromCH1_Y);
		AlignToolLogRecord(str, nPlatformIndex);
	}
	else
	{
		dTargetPlatfromCH0_X = TargetMarkPlatformPos.m_vPos.at(0).GetPosX();
		dTargetPlatfromCH0_Y = TargetMarkPlatformPos.m_vPos.at(0).GetPosY();

		dTargetPlatfromCH1_X = TargetMarkPlatformPos.m_vPos.at(1).GetPosX();
		dTargetPlatfromCH1_Y = TargetMarkPlatformPos.m_vPos.at(1).GetPosY();

		str.Format(_T("目标CH0平台坐标 X = %f, Y = %f"), dTargetPlatfromCH0_X, dTargetPlatfromCH0_Y);
		AlignToolLogRecord(str,m_nPlatformIndex);

		str.Format(_T("目标CH1平台坐标 X = %f, Y = %f"), dTargetPlatfromCH1_X, dTargetPlatfromCH1_Y);
		AlignToolLogRecord(str, nPlatformIndex);
	}


	// (1) 目标产品位置
	double dTargetPlatfromCenterX, dTargetPlatfromCenterY;
	dTargetPlatfromCenterX = (dTargetPlatfromCH0_X + dTargetPlatfromCH1_X)/2.0;
	dTargetPlatfromCenterY = (dTargetPlatfromCH0_Y + dTargetPlatfromCH1_Y)/2.0;

	scLine TargetPlatfromLx;
	scLineSeg TargetPlatfromlineSeg(sc2Vector(dTargetPlatfromCH0_X, dTargetPlatfromCH0_Y), sc2Vector(dTargetPlatfromCH1_X, dTargetPlatfromCH1_Y));
	TargetPlatfromLx.SetFromLineSeg(TargetPlatfromlineSeg);

	//	scDegree degTargetPlatfromProductAngle = scDegree(TargetPlatfromLx.GetRotation());
	scRadian radTargetPlatfromProductAngle = TargetPlatfromLx.GetRotation();

	str.Format(_T("目标产品位置 X = %f, Y = %f, D = %f"), dTargetPlatfromCenterX, dTargetPlatfromCenterY, scDegree(radTargetPlatfromProductAngle).ToDouble());
	AlignToolLogRecord(str, nPlatformIndex);

	// (2) 目标距离
	double dTargetPlatfromDis;
	sc2Vector vMark_Target_Dis[2];

	vMark_Target_Dis[0].SetX(dTargetPlatfromCH0_X);
	vMark_Target_Dis[0].SetY(dTargetPlatfromCH0_Y);
	vMark_Target_Dis[1].SetX(dTargetPlatfromCH1_X);
	vMark_Target_Dis[1].SetY(dTargetPlatfromCH1_Y);

	dTargetPlatfromDis = vMark_Target_Dis[0].Distance(vMark_Target_Dis[1]);

	str.Format(_T("目标产品尺寸 Dis = %f"), dTargetPlatfromDis);
	AlignToolLogRecord(str, nPlatformIndex);


	// 2. 获取对象平台坐标；
	CMarkPlatformPos ObjectMarkPlatformPos = GetBendInsObjectPlatformPos();

	double dObjectPlatfromCH0_X = ObjectMarkPlatformPos.m_vPos.at(0).GetPosX();
	double dObjectPlatfromCH0_Y = ObjectMarkPlatformPos.m_vPos.at(0).GetPosY();

	double dObjectPlatfromCH1_X = ObjectMarkPlatformPos.m_vPos.at(1).GetPosX();
	double dObjectPlatfromCH1_Y = ObjectMarkPlatformPos.m_vPos.at(1).GetPosY();

	str.Format(_T("对象CH0平台坐标 X = %f, Y = %f"), dObjectPlatfromCH0_X, dObjectPlatfromCH0_Y);
	AlignToolLogRecord(str,m_nPlatformIndex);

	str.Format(_T("对象CH1平台坐标 X = %f, Y = %f"), dObjectPlatfromCH1_X, dObjectPlatfromCH1_Y);
	AlignToolLogRecord(str, nPlatformIndex);


	// (1) 对象产品位置
	double dObjectPlatfromCenterX, dObjectPlatfromCenterY;
	dObjectPlatfromCenterX = (dObjectPlatfromCH0_X + dObjectPlatfromCH1_X)/2.0;
	dObjectPlatfromCenterY = (dObjectPlatfromCH0_Y + dObjectPlatfromCH1_Y)/2.0;

	scLine ObjectPlatfromLx;
	scLineSeg ObjectPlatfromlineSeg(sc2Vector(dObjectPlatfromCH0_X, dObjectPlatfromCH0_Y), sc2Vector(dObjectPlatfromCH1_X, dObjectPlatfromCH1_Y));
	ObjectPlatfromLx.SetFromLineSeg(ObjectPlatfromlineSeg);

	//	scDegree degObjectProductAngle = scDegree(ObjectPlatfromLx.GetRotation());
	scRadian radObjectPlatformProductAngle = ObjectPlatfromLx.GetRotation();

	str.Format(_T("对象产品位置 X = %f, Y = %f, D = %f"), dObjectPlatfromCenterX, dObjectPlatfromCenterY, scDegree(radObjectPlatformProductAngle).ToDouble());
	AlignToolLogRecord(str, nPlatformIndex);

	// (2) 对象距离
	double dObjectPlatfromDis;
	sc2Vector vMark_Object_Dis[2];

	vMark_Object_Dis[0].SetX(dObjectPlatfromCH0_X);
	vMark_Object_Dis[0].SetY(dObjectPlatfromCH0_Y);
	vMark_Object_Dis[1].SetX(dObjectPlatfromCH1_X);
	vMark_Object_Dis[1].SetY(dObjectPlatfromCH1_Y);
	dObjectPlatfromDis = vMark_Object_Dis[0].Distance(vMark_Object_Dis[1]);

	str.Format(_T("对象产品尺寸 Dis = %f"), dObjectPlatfromDis);
	AlignToolLogRecord(str, nPlatformIndex);


	// 3. 建立目标产品坐标系；
	double dTempTarget_X0 = (- dTargetPlatfromDis / 2.0);
	double dTempTarget_Y0 = 0;

	double dTempTarget_X1 = (dTargetPlatfromDis / 2.0);
	double dTempTarget_Y1 = 0;

	double dTargetCH0_New_X = dTempTarget_X0;
	double dTargetCH0_New_Y = dTempTarget_Y0;

	double dTargetCH1_New_X = dTempTarget_X1;
	double dTargetCH1_New_Y = dTempTarget_Y1;

	scRadian radTempTargetAngle = radTargetPlatfromProductAngle;

	str.Format(_T("目标产品坐标系CH0平台坐标 X = %f, Y = %f"), dTargetCH0_New_X, dTargetCH0_New_Y);
	AlignToolLogRecord(str,m_nPlatformIndex);

	str.Format(_T("目标产品坐标系CH1平台坐标 X = %f, Y = %f"), dTargetCH1_New_X, dTargetCH1_New_Y);
	AlignToolLogRecord(str, nPlatformIndex);

	str.Format(_T("目标产品坐标系角度 D = %f"), scDegree(radTempTargetAngle).ToDouble());
	AlignToolLogRecord(str, nPlatformIndex);

	// 4. 平台坐标系和目标产品坐标系的平移关系(不包括旋转)；
	double dPlatOffsetX = dTargetPlatfromCenterX;
	double dPlatOffsetY = dTargetPlatfromCenterY;

	str.Format(_T("平台坐标系原点和目标产品坐标系原点平移关系 X = %f, Y = %f"), dPlatOffsetX, dPlatOffsetY);
	AlignToolLogRecord(str, nPlatformIndex);


	// 5. 在如上4的情况下，计算对象的平台坐标；
	double dTempObjectCH0_X = dObjectPlatfromCH0_X - dPlatOffsetX;
	double dTempObjectCH0_Y = dObjectPlatfromCH0_Y - dPlatOffsetY;

	double dTempObjectCH1_X = dObjectPlatfromCH1_X - dPlatOffsetX;
	double dTempObjectCH1_Y = dObjectPlatfromCH1_Y - dPlatOffsetY;

	str.Format(_T("平台坐标系平移后对象平台坐标CH0 X = %f, Y = %f"), dTempObjectCH0_X, dTempObjectCH0_Y);
	AlignToolLogRecord(str, nPlatformIndex);

	str.Format(_T("平台坐标系平移后对象平台坐标CH1 X = %f, Y = %f"), dTempObjectCH1_X, dTempObjectCH1_Y);
	AlignToolLogRecord(str, nPlatformIndex);

	// 6. 在如上5的情况下，计算对象在目标产品坐标系下的平台坐标；
	double dObjectCH0_New_X = dTempObjectCH0_Y * sin(radTempTargetAngle) + dTempObjectCH0_X * cos(radTempTargetAngle);
	double dObjectCH0_New_Y = dTempObjectCH0_Y * cos(radTempTargetAngle) - dTempObjectCH0_X * sin(radTempTargetAngle);

	double dObjectCH1_New_X = dTempObjectCH1_Y * sin(radTempTargetAngle) + dTempObjectCH1_X * cos(radTempTargetAngle);
	double dObjectCH1_New_Y = dTempObjectCH1_Y * cos(radTempTargetAngle) - dTempObjectCH1_X * sin(radTempTargetAngle);


	str.Format(_T("目标产品坐标系下对象平台坐标CH0 X = %f, Y = %f"), dObjectCH0_New_X, dObjectCH0_New_Y);
	AlignToolLogRecord(str, nPlatformIndex);

	str.Format(_T("目标产品坐标系下对象平台坐标CH1 X = %f, Y = %f"), dObjectCH1_New_X, dObjectCH1_New_Y);
	AlignToolLogRecord(str, nPlatformIndex);

	// 7. 计算折弯检测结果
	vTargetPlaftormPos.push_back( sc2Vector(dTargetCH0_New_X, dTargetCH0_New_Y) );
	vTargetPlaftormPos.push_back( sc2Vector(dTargetCH1_New_X, dTargetCH1_New_Y) );

	vObjectPlatformPos.push_back( sc2Vector(dObjectCH0_New_X, dObjectCH0_New_Y) );
	vObjectPlatformPos.push_back( sc2Vector(dObjectCH1_New_X, dObjectCH1_New_Y) );

	vdDistance.push_back(dTargetPlatfromDis);
	vdDistance.push_back(dObjectPlatfromDis);


	str.Format(_T("折弯检测结束-----"));
	AlignToolLogRecord(str, nPlatformIndex);

	return TRUE;
}

BOOL vcXYDAlignTool::ConvertBendDmCodeImagePosToPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
	CPlatformAxisPos* pPlatformAxisPos)
{
	return ConvertBendDmCodeImagePosToPlatformPosCommon(platformCoordPos, nIndex, imageCoordPos, pPlatformAxisPos);
}

BOOL vcXYDAlignTool::ConvertBendDmCodeImagePosToPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
	CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos,CPlatformAxisPos* pCamAxisPlatformAxisPos)
{
	return ConvertBendDmCodeImagePosToPlatformPosCommon(platformCoordPos, nIndex, imageCoordPos, pPlatformAxisPos, pCamPlatformAxisPos);
}


BOOL vcXYDAlignTool::ConvertBendDmCodeImagePosToPlatformPosCommon(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
	CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos/* = NULL*/,CPlatformAxisPos* pCamAxisPlatformAxisPos/* = NULL*/)
{
	int nRemIndex = nIndex%m_iMarkNum;
	int nExProductIndex = nIndex/m_iMarkNum;
	if (NULL == m_vpCalibrateParam.at(nRemIndex))
		return false;

	if(m_vpCalibrateParam.at(nRemIndex)->m_nEnableDisCor)
	{
		CString strOut, strTmp;
		strOut.Format(_T("畸变矫正开始："));
		AlignToolLogRecord(strOut,m_nPlatformIndex);

		double p[15];
		memset(p, 0.0, sizeof(double)*15);
		scCalibrateResult result = m_vAlignerCalibInfos.at(nIndex)->m_result;
		result.GetNonlinearProjectResult(p);
		double dRms = result.GetRMS();
		strOut.Format("畸变参数: ");
		for (int i=0;i<15; i++)
		{
			strTmp.Format(_T("P%d:%.6f "), i, p[i]);
			strOut += strTmp;
		}
		strTmp.Format(_T("P15:%.6f "),dRms);
		strOut += strTmp;
		AlignToolLogRecord(strOut,m_nPlatformIndex);

		strOut.Format("畸变校正前图像坐标: ");
		strTmp.Format(_T("Pos(%.6f, %.6f, %.6f) "), imageCoordPos.m_dPosX, imageCoordPos.m_dPosY, imageCoordPos.m_dAngle);
		strOut += strTmp;
		for (int i=0; i<imageCoordPos.m_vdAuxiliaryPosX.size(); i++)
		{
			strTmp.Format(_T("AuxiliaryPos%d(%.6f, %.6f, %.6f) "), i, imageCoordPos.m_vdAuxiliaryPosX.at(i), imageCoordPos.m_vdAuxiliaryPosY.at(i), imageCoordPos.m_vdAuxiliaryAngle.at(i));
			strOut += strTmp;
		}
		AlignToolLogRecord(strOut,m_nPlatformIndex);

		CCoordPos imageDisCorPos;
		ExecuteDisCorPoint(nIndex, imageCoordPos, imageDisCorPos);
		imageCoordPos = imageDisCorPos;

		strOut.Format("畸变校正后图像坐标: ");
		strTmp.Format(_T("Pos(%.6f, %.6f, %.6f) "), imageCoordPos.m_dPosX, imageCoordPos.m_dPosY, imageCoordPos.m_dAngle);
		strOut += strTmp;
		for (int i=0; i<imageCoordPos.m_vdAuxiliaryPosX.size(); i++)
		{
			strTmp.Format(_T("AuxiliaryPos%d(%.6f, %.6f, %.6f) "), i, imageCoordPos.m_vdAuxiliaryPosX.at(i), imageCoordPos.m_vdAuxiliaryPosY.at(i), imageCoordPos.m_vdAuxiliaryAngle.at(i));
			strOut += strTmp;
		}
		AlignToolLogRecord(strOut,m_nPlatformIndex);

		strOut.Format(_T("畸变矫正结束："));
		AlignToolLogRecord(strOut,m_nPlatformIndex);
	}

	CString str;
	str.Format("图像坐标转平台坐标开始, MarkNum: %d, PosIndex：%d", m_iMarkNum, nIndex);
	AlignToolLogRecord(str,m_nPlatformIndex);

	std::vector<CCoordPos> vPlatformCoordPos;	// 平台坐标数组
	std::vector<CCoordPos> vImageCoordPos;		// 图像坐标数组 

	// 图像坐标imageCoordPos主、辅点，转换为图像坐标数组vImageCoordPos 
	vImageCoordPos.push_back(imageCoordPos);
	int nAuxIndex = 0;
	for (nAuxIndex = 0; nAuxIndex < imageCoordPos.m_vdAuxiliaryPosX.size(); nAuxIndex++)
	{
		CCoordPos pos;
		pos.SetPosAngle(imageCoordPos.GetAuxPos(nAuxIndex), imageCoordPos.GetAngle());
		vImageCoordPos.push_back(pos);
	}

	// 根据图像坐标数组vImageCoordPos，获取平台坐标数组vPlatformCoordPos；
	BOOL bSuccess = TRUE;
	int nPtIndex = 0;
	vPlatformCoordPos.resize(vImageCoordPos.size());
	BOOL bLogRecordEnable = m_bLogRecordEnable;			// 临时记录是否存储日志变量
	for(nPtIndex = 0; nPtIndex < vImageCoordPos.size(); nPtIndex++)
	{
		if (nPtIndex > 0)
		{
			// 辅助点，暂时不存储图像坐标转换详细信息，因为太多了
			if (bLogRecordEnable)
			{
				m_bLogRecordEnable = FALSE;
			}
		}

		bSuccess = bSuccess && ConvertBendDmCodeImagePosToPlatformPosMainPos(vPlatformCoordPos.at(nPtIndex), nIndex, vImageCoordPos.at(nPtIndex),
			pPlatformAxisPos, pCamPlatformAxisPos, pCamAxisPlatformAxisPos);

		if (nPtIndex > 0)
		{
			// 辅助点，暂时不存储图像坐标转换详细信息，因为太多了
			if (bLogRecordEnable)
			{
				m_bLogRecordEnable = TRUE;
			}
		}

		CCoordPos imgPos = vImageCoordPos.at(nPtIndex);
		CCoordPos platPos = vPlatformCoordPos.at(nPtIndex);
		if (nPtIndex == 0)
		{
			str.Format("PosIndex：%d, 基础点: %d, 图像坐标: X: %f, Y: %f, D: %f", nIndex, nPtIndex, imgPos.GetPosX(), imgPos.GetPosY(), imgPos.GetAngle());
			AlignToolLogRecord(str,m_nPlatformIndex);

			str.Format("PosIndex：%d, 基础点: %d, 平台坐标: X: %f, Y: %f, D: %f", nIndex, nPtIndex, platPos.GetPosX(), platPos.GetPosY(), platPos.GetAngle());
			AlignToolLogRecord(str,m_nPlatformIndex);
		}
		else
		{		
			str.Format("PosIndex：%d, 辅助点: %d, 图像坐标: X: %f, Y: %f, D: %f", nIndex, nPtIndex-1, imgPos.GetPosX(), imgPos.GetPosY(), imgPos.GetAngle());
			AlignToolLogRecord(str,m_nPlatformIndex);

			str.Format("PosIndex：%d, 辅助点: %d, 平台坐标: X: %f, Y: %f, D: %f", nIndex, nPtIndex-1, platPos.GetPosX(), platPos.GetPosY(), platPos.GetAngle());
			AlignToolLogRecord(str,m_nPlatformIndex);
		}
	}

	m_bLogRecordEnable = bLogRecordEnable;

	if(!bSuccess)
	{
		str.Format("图像坐标转平台坐标完成：NG");
		AlignToolLogRecord(str,m_nPlatformIndex);
		return false;
	}

	// 将平台坐标数组vPlatformCoordPos，转换为平台坐标platformCoordPos主、辅点；
	platformCoordPos = vPlatformCoordPos.at(0);
	for(nAuxIndex = 0; nAuxIndex < (vPlatformCoordPos.size()-1) && nAuxIndex < platformCoordPos.m_vdAuxiliaryPosX.size(); nAuxIndex++)
	{
		platformCoordPos.SetAuxPosAngle(nAuxIndex, vPlatformCoordPos.at(nAuxIndex+1).GetPos(), vPlatformCoordPos.at(nAuxIndex+1).GetAngle());
	}	

	str.Format("图像坐标转平台坐标完成：OK");
	AlignToolLogRecord(str,m_nPlatformIndex);

	return bSuccess;
}


BOOL vcXYDAlignTool::ConvertBendDmCodeImagePosToPlatformPosMainPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
	CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos/* = NULL*/,CPlatformAxisPos* pCamAxisPlatformAxisPos/* = NULL*/)
{
	int nRemIndex = nIndex%m_iMarkNum;
	int nExProductIndex = nIndex/m_iMarkNum;
	if (NULL == m_vpCalibrateParam.at(nRemIndex))
	{
		return false;
	}

	{
		CString str;

		str.Format("坐标转换");
		AlignToolLogRecord(str, m_nPlatformIndex);

		str.Format("m_bEnableMultiCalibExtension = %d",m_bEnableMultiCalibExtension);
		AlignToolLogRecord(str,m_nPlatformIndex);

		str.Format("m_bBendAlignProcess = %d",m_bBendAlignProcess);
		AlignToolLogRecord(str,m_nPlatformIndex);

		str.Format("m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode = %d",m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode);
		AlignToolLogRecord(str,m_nPlatformIndex);

		str.Format("m_AlignermentParam.GetBendInfo().m_bBendingInspect = %d",m_AlignermentParam.GetBendInfo().m_bBendingInspect);
		AlignToolLogRecord(str,m_nPlatformIndex);

		str.Format("m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode = %d",m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode);
		AlignToolLogRecord(str,m_nPlatformIndex);

		str.Format("m_AlignermentParam.GetCalibTargetMarkPatternMode() = %d", m_AlignermentParam.GetCalibTargetMarkPatternMode());
		AlignToolLogRecord(str, m_nPlatformIndex);


	}



	// 产品扩展
	if (m_bEnableMultiCalibExtension)
	{
		// 二维码
		if (eDmCodeCalib == m_AlignermentParam.GetCalibTargetMarkPatternMode())
		{
			// FAAL
			if (m_bBendAlignProcess)
			{
				if (m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode)
				{
					if (m_bEnableMultiCalibExtension == TRUE)
					{
						if (nIndex<0 || nIndex >= m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1))
						{
							return false;
						}
					}
					else
					{
						if (nIndex<0 || nIndex >= m_iMarkNum)
						{
							return false;
						}
					}

					if (nIndex >= m_vAlignerCalibInfos.size())
					{
						return false;
					}

					if (NULL == m_vAlignerCalibInfos.at(nIndex))
					{
						return false;
					}

					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
						{
							return false;
						}
					}

					int nPlatformIndex = m_nPlatformIndex;
					CString str;

					//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
					//相机平台类型没有基类，不用改；
					CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
					CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
					CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 

					if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
					{
						return false;
					}

					str.Format("轴基准坐标： X = %f , Y = %f , D = %f ",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前轴坐标： X = %f , Y = %f , D = %f ",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);


					// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
					//标定参数
					double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
					double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
					double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
					double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

					double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
					double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();

					str.Format("Mark平台基准位置： X = %f , Y = %f ",dPlatformBenchMarkX,dPlatformBenchMarkY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台各轴的方向	
					int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
					int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;


					// 实际平台XY轴偏移量
					double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
					double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY - pPlatformBenchAxisPos->m_dPosY;

					if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
					if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;

					if ( GetMidPlatformType() ==ePlatformXDPY || GetMidPlatformType() ==ePlatformXDPY1)
					{
						dPlatformOffsetY =0 ;
					}
					else if ( GetMidPlatformType() ==ePlatformXYPD && m_bCalibrateChangeXDirection && m_bCalibrateChangeYDirection)
					{

					}
					else if ( GetMidPlatformType() ==ePlatformXYPD || GetMidPlatformType() ==ePlatformXY || GetMidPlatformType()==ePlatformX || GetMidPlatformType() == ePlatformY)
					{
						dPlatformOffsetX =0 ;
						dPlatformOffsetY =0 ;
					}

					str.Format("轴平移量： X = %f , Y = %f ",dPlatformOffsetX,dPlatformOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);


					//平台XY平移量对基准mark平台坐标的影响方向相反
					dPlatformBenchMarkX -= dPlatformOffsetX;
					dPlatformBenchMarkY -= dPlatformOffsetY;


					// 二维码标定，转换矩阵中包含了旋转平移关系，基准mark坐标为（0,0）
					if (m_bTargetCalibUseBoard)
					{
						dPlatformBenchMarkX = 0 ;
						dPlatformBenchMarkY = 0 ;
					}



					// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
					double dImgOffsetRealDistanceX = 0;
					double dImgOffsetRealDistanceY = 0;
					double dImgOffsetX = imageCoordPos.m_dPosX/* - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX*/;
					double dImgOffsetY = imageCoordPos.m_dPosY/* - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY*/;
					dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
					dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

					str.Format("基准图像位置： X = %f , Y = %f ",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前图像位置： X = %f , Y = %f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("图像像素移动量： X = %f , Y = %f ",dImgOffsetX,dImgOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("使用2*2矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

					if (m_bTargetObjectCamSeparate)
					{
						if (GetMidPlatformType() ==ePlatformXYPD)
						{
							// 目标、对象分离，目标平台坐标
							if (nIndex%m_iMarkNum >= m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}						
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，对象平台坐标
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}
						else
						{
							// 目标、对象分离，对象点平台坐标计算
							if (nIndex%m_iMarkNum < m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}		

									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，目标点平台坐标计算
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}

					}
					else
					{
						if (m_PlatformCalibType == ePlatformCalib9Point)
						{
							sc2Vector vSrc;
							sc2Vector vDst;
							vSrc.SetX(dImgOffsetX);
							vSrc.SetY(dImgOffsetY);
							if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
							{
								sc2Vector vBoard;
								vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
								vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
							}
							else
							{
								vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
							}

							dImgOffsetRealDistanceX = vDst.GetX();
							dImgOffsetRealDistanceY = vDst.GetY();
						}
					}

					str.Format("图像像素平台移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);


					// $4、内容相加，即为相机实时Mark的平台坐标值
					platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
					platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
					//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
					platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

					// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响
					if (eCamPlatformSepXY == m_pCamPlatformInfo->GetCamPlatformType() 
						|| eCamPlatformSepX == m_pCamPlatformInfo->GetCamPlatformType()
						|| eCamPlatformShareX == m_pCamPlatformInfo->GetCamPlatformType())
					{
						// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响

						// 相机轴
						CPlatformXYAxisPos*   pCamPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformAxisPos());
						CPlatformXYAxisPos*   pCamPlatformXYAxisPos =	 (CPlatformXYAxisPos*)pCamPlatformAxisPos;

						// 相机轴所在轴
						CPlatformXYAxisPos*	  pCamAxisPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamAxisPlatformAxisPos());
						CPlatformXYAxisPos*   pCamAxisPlatformXYAxisPos = (CPlatformXYAxisPos*)pCamAxisPlatformAxisPos;

						double dCameraMovementX = 0;				// 相机轴运动量X
						double dCameraMovementY = 0;				// 相机轴运动量Y
						double dCameraOffsetRealDistanceX = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响X
						double dCameraOffsetRealDistanceY = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响Y

						double dCamAxisPlatformAxisMovementX = 0;	// 相机轴所在轴运动量X
						double dCamAxisPlatformAxisMovementY = 0;	// 相机轴所在轴运动量Y

						// 相机轴运动量对实时Mark在平台坐标系中的位置的影响
						if (NULL != pCamPlatformBenchAxisPos && NULL != pCamPlatformXYAxisPos)
						{
							str.Format("相机轴基准坐标： X = %f , Y = %f ",pCamPlatformBenchAxisPos->m_dPosX,pCamPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机轴坐标： X = %f , Y = %f ",pCamPlatformXYAxisPos->m_dPosX,pCamPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamPlatformXCoordType = m_pCamPlatformInfo->GetCamDirectX(nIndex%m_iMarkNum);
							int iCamPlatformYCoordType = m_pCamPlatformInfo->GetCamDirectY(nIndex%m_iMarkNum);


							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCameraMovementX = 0;
								dCameraMovementY = 0;

								str.Format("相机移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								dCameraOffsetRealDistanceX = dCameraMovementX ;
								dCameraOffsetRealDistanceY = dCameraMovementY ;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCameraMovementX =pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if (eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -1*dCameraMovementX;

								dCameraOffsetRealDistanceX = dCameraMovementX;
								dCameraOffsetRealDistanceY = dCameraMovementY;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamPlatformYCoordType)
								{
									dCameraMovementY = pCamPlatformXYAxisPos->m_dPosY - pCamPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);


								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;


								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11 + dCameraMovementY*dCamTranferA12;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA21 + dCameraMovementY*dCamTranferA22;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;

								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA12;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
						}

						// 相机轴所在轴运动量对实时Mark在平台坐标系中的位置的影响
						if ( NULL != pCamAxisPlatformBenchAxisPos && NULL != pCamAxisPlatformXYAxisPos)
						{
							str.Format("相机平台所在轴基准坐标： X = %f , Y = %f ",pCamAxisPlatformBenchAxisPos->m_dPosX,pCamAxisPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机平台所在轴坐标： X = %f , Y = %f ",pCamAxisPlatformXYAxisPos->m_dPosX,pCamAxisPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamAxisPlatformAxisXCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectX(nIndex%m_iMarkNum);
							int iCamAxisPlatformAxisYCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectY(nIndex%m_iMarkNum);

							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCamAxisPlatformAxisMovementX = 0;
								dCamAxisPlatformAxisMovementY = 0;

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 

								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								dCamAxisPlatformAxisMovementY = 0;
							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								if(eDirectNone != iCamAxisPlatformAxisXCoordType)
								{
									dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamAxisPlatformAxisYCoordType)
								{
									dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								if(eDirectNegative == iCamAxisPlatformAxisYCoordType) dCamAxisPlatformAxisMovementY = -dCamAxisPlatformAxisMovementY;
							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
							}
						}

						// $4、三部分内容相加，即为相机实时Mark的平台坐标值
						platformCoordPos.m_dPosX += dCameraOffsetRealDistanceX + dCamAxisPlatformAxisMovementX;
						platformCoordPos.m_dPosY += dCameraOffsetRealDistanceY + dCamAxisPlatformAxisMovementY;
					}

					str.Format("平台坐标值： X = %f , Y = %f , D = %f ",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

				}
				else
				{
					if (m_bEnableMultiCalibExtension == TRUE)
					{
						if (nIndex<0 || nIndex >= m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1))
						{
							return false;
						}
					}
					else
					{
						if (nIndex<0 || nIndex >= m_iMarkNum)
						{
							return false;
						}
					}

					if (nIndex >= m_vAlignerCalibInfos.size())
					{
						return false;
					}

					if (NULL == m_vAlignerCalibInfos.at(nIndex))
					{
						return false;
					}

					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
						{
							return false;
						}
					}

					int nPlatformIndex = m_nPlatformIndex;
					CString str;

					//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
					//相机平台类型没有基类，不用改；
					CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
					CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
					CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 

					if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
					{
						return false;
					}

					// 当目标自动标定不关联时，目标标定时轴位置通常是贴合时轴位置，而不是对象拍照时轴位置，
					// 按照目标标定时轴位置计算得到目标平台坐标很大，直接对位计算会超出视野，需要使用目标补偿解决，但是最后一次对位后位置会直接跑到贴合位
					// 使用对象标定时轴位置0/1替换目标标定时轴位置2/3，也可以解决这个问题，而不使用目标补偿
					if (m_bTargetObjectCamSeparate == TRUE && m_bTarObjCamSepNoAxisCalib == FALSE
						&& (GetMidPlatformType() != ePlatformXYPD && nIndex%m_iMarkNum >= m_iMarkNum/2) && m_bTarObjCamSepAutoCalibTar == TRUE && m_eTarAutoCalibAlignMode == eAlignOnObject)
					{
						pPlatformBenchAxisPos = (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex - m_iMarkNum/2)->GetPlatformAxisPos());
					}

					str.Format("轴基准坐标： X = %f , Y = %f , D = %f ",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前轴坐标： X = %f , Y = %f , D = %f ",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);


					// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
					//标定参数
					double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
					double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
					double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
					double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

					double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
					double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();

					str.Format("Mark平台基准位置： X = %f , Y = %f ",dPlatformBenchMarkX,dPlatformBenchMarkY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台各轴的方向	
					int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
					int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;


					// 实际平台XY轴偏移量
					double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
					double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY - pPlatformBenchAxisPos->m_dPosY;

					if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
					if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;

					if ( GetMidPlatformType() ==ePlatformXDPY || GetMidPlatformType() ==ePlatformXDPY1)
					{
						dPlatformOffsetY =0 ;
					}
					else if ( GetMidPlatformType() ==ePlatformXYPD && m_bCalibrateChangeXDirection && m_bCalibrateChangeYDirection)
					{

					}
					else if ( GetMidPlatformType() ==ePlatformXYPD || GetMidPlatformType() ==ePlatformXY || GetMidPlatformType()==ePlatformX || GetMidPlatformType() == ePlatformY)
					{
						dPlatformOffsetX =0 ;
						dPlatformOffsetY =0 ;
					}

					str.Format("轴平移量： X = %f , Y = %f ",dPlatformOffsetX,dPlatformOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);


					//平台XY平移量对基准mark平台坐标的影响方向相反
					dPlatformBenchMarkX -= dPlatformOffsetX;
					dPlatformBenchMarkY -= dPlatformOffsetY;

					// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
					double dImgOffsetRealDistanceX = 0;
					double dImgOffsetRealDistanceY = 0;
					double dImgOffsetX = imageCoordPos.m_dPosX - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX;
					double dImgOffsetY = imageCoordPos.m_dPosY - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY;
					dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
					dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

					str.Format("基准图像位置： X = %f , Y = %f ",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前图像位置： X = %f , Y = %f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("图像像素移动量： X = %f , Y = %f ",dImgOffsetX,dImgOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("使用2*2矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

					if (m_bTargetObjectCamSeparate)
					{
						if (GetMidPlatformType() ==ePlatformXYPD)
						{
							// 目标、对象分离，目标平台坐标
							if (nIndex%m_iMarkNum >= m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}						
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，对象平台坐标
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}
						else
						{
							// 目标、对象分离，对象点平台坐标计算
							if (nIndex%m_iMarkNum < m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}		

									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，目标点平台坐标计算
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}

					}
					else
					{
						if (m_PlatformCalibType == ePlatformCalib9Point)
						{
							sc2Vector vSrc;
							sc2Vector vDst;
							vSrc.SetX(dImgOffsetX);
							vSrc.SetY(dImgOffsetY);
							if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
							{
								sc2Vector vBoard;
								vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
								vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
							}
							else
							{
								vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
							}

							dImgOffsetRealDistanceX = vDst.GetX();
							dImgOffsetRealDistanceY = vDst.GetY();
						}
					}

					str.Format("图像像素平台移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);


					// $4、内容相加，即为相机实时Mark的平台坐标值
					platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
					platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
					//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
					platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

					// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响
					if (eCamPlatformSepXY == m_pCamPlatformInfo->GetCamPlatformType() 
						|| eCamPlatformSepX == m_pCamPlatformInfo->GetCamPlatformType()
						|| eCamPlatformShareX == m_pCamPlatformInfo->GetCamPlatformType())
					{
						// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响

						// 相机轴
						CPlatformXYAxisPos*   pCamPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformAxisPos());
						CPlatformXYAxisPos*   pCamPlatformXYAxisPos =	 (CPlatformXYAxisPos*)pCamPlatformAxisPos;

						// 相机轴所在轴
						CPlatformXYAxisPos*	  pCamAxisPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamAxisPlatformAxisPos());
						CPlatformXYAxisPos*   pCamAxisPlatformXYAxisPos = (CPlatformXYAxisPos*)pCamAxisPlatformAxisPos;

						double dCameraMovementX = 0;				// 相机轴运动量X
						double dCameraMovementY = 0;				// 相机轴运动量Y
						double dCameraOffsetRealDistanceX = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响X
						double dCameraOffsetRealDistanceY = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响Y

						double dCamAxisPlatformAxisMovementX = 0;	// 相机轴所在轴运动量X
						double dCamAxisPlatformAxisMovementY = 0;	// 相机轴所在轴运动量Y

						// 相机轴运动量对实时Mark在平台坐标系中的位置的影响
						if (NULL != pCamPlatformBenchAxisPos && NULL != pCamPlatformXYAxisPos)
						{
							str.Format("相机轴基准坐标： X = %f , Y = %f ",pCamPlatformBenchAxisPos->m_dPosX,pCamPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机轴坐标： X = %f , Y = %f ",pCamPlatformXYAxisPos->m_dPosX,pCamPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamPlatformXCoordType = m_pCamPlatformInfo->GetCamDirectX(nIndex%m_iMarkNum);
							int iCamPlatformYCoordType = m_pCamPlatformInfo->GetCamDirectY(nIndex%m_iMarkNum);


							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCameraMovementX = 0;
								dCameraMovementY = 0;

								str.Format("相机移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								dCameraOffsetRealDistanceX = dCameraMovementX ;
								dCameraOffsetRealDistanceY = dCameraMovementY ;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCameraMovementX =pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if (eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -1*dCameraMovementX;

								dCameraOffsetRealDistanceX = dCameraMovementX;
								dCameraOffsetRealDistanceY = dCameraMovementY;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamPlatformYCoordType)
								{
									dCameraMovementY = pCamPlatformXYAxisPos->m_dPosY - pCamPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);


								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;


								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11 + dCameraMovementY*dCamTranferA12;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA21 + dCameraMovementY*dCamTranferA22;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;

								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA12;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
						}

						// 相机轴所在轴运动量对实时Mark在平台坐标系中的位置的影响
						if ( NULL != pCamAxisPlatformBenchAxisPos && NULL != pCamAxisPlatformXYAxisPos)
						{
							str.Format("相机平台所在轴基准坐标： X = %f , Y = %f ",pCamAxisPlatformBenchAxisPos->m_dPosX,pCamAxisPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机平台所在轴坐标： X = %f , Y = %f ",pCamAxisPlatformXYAxisPos->m_dPosX,pCamAxisPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamAxisPlatformAxisXCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectX(nIndex%m_iMarkNum);
							int iCamAxisPlatformAxisYCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectY(nIndex%m_iMarkNum);

							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCamAxisPlatformAxisMovementX = 0;
								dCamAxisPlatformAxisMovementY = 0;

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 

								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								dCamAxisPlatformAxisMovementY = 0;
							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								if(eDirectNone != iCamAxisPlatformAxisXCoordType)
								{
									dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamAxisPlatformAxisYCoordType)
								{
									dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								if(eDirectNegative == iCamAxisPlatformAxisYCoordType) dCamAxisPlatformAxisMovementY = -dCamAxisPlatformAxisMovementY;
							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
							}
						}

						// $4、三部分内容相加，即为相机实时Mark的平台坐标值
						platformCoordPos.m_dPosX += dCameraOffsetRealDistanceX + dCamAxisPlatformAxisMovementX;
						platformCoordPos.m_dPosY += dCameraOffsetRealDistanceY + dCamAxisPlatformAxisMovementY;
					}

					str.Format("平台坐标值： X = %f , Y = %f , D = %f ",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

				}
			}
			// FCHB
			else
			{
				if (m_AlignermentParam.GetBendInfo().m_bBendingInspect && m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode)
				{
					if (m_bEnableMultiCalibExtension == TRUE)
					{
						if (nIndex<0 || nIndex >= m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1))
						{
							return false;
						}
					}
					else
					{
						if (nIndex<0 || nIndex >= m_iMarkNum)
						{
							return false;
						}
					}

					if (nIndex >= m_vAlignerCalibInfos.size())
					{
						return false;
					}

					if (NULL == m_vAlignerCalibInfos.at(nIndex))
					{
						return false;
					}

					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
						{
							return false;
						}
					}

					int nPlatformIndex = m_nPlatformIndex;
					CString str;

					//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
					//相机平台类型没有基类，不用改；
					CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
					CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
					CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 

					if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
					{
						return false;
					}

					str.Format("轴基准坐标： X = %f , Y = %f , D = %f ",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前轴坐标： X = %f , Y = %f , D = %f ",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);


					// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
					//标定参数
					double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
					double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
					double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
					double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

					double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
					double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();

					str.Format("Mark平台基准位置： X = %f , Y = %f ",dPlatformBenchMarkX,dPlatformBenchMarkY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台各轴的方向	
					int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
					int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;


					// 实际平台XY轴偏移量
					double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
					double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY - pPlatformBenchAxisPos->m_dPosY;

					if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
					if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;

					if ( GetMidPlatformType() ==ePlatformXDPY || GetMidPlatformType() ==ePlatformXDPY1)
					{
						dPlatformOffsetY =0 ;
					}
					else if ( GetMidPlatformType() ==ePlatformXYPD && m_bCalibrateChangeXDirection && m_bCalibrateChangeYDirection)
					{

					}
					else if ( GetMidPlatformType() ==ePlatformXYPD || GetMidPlatformType() ==ePlatformXY || GetMidPlatformType()==ePlatformX || GetMidPlatformType() == ePlatformY)
					{
						dPlatformOffsetX =0 ;
						dPlatformOffsetY =0 ;
					}

					str.Format("轴平移量： X = %f , Y = %f ",dPlatformOffsetX,dPlatformOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);


					//平台XY平移量对基准mark平台坐标的影响方向相反
					dPlatformBenchMarkX -= dPlatformOffsetX;
					dPlatformBenchMarkY -= dPlatformOffsetY;


					// 二维码标定，转换矩阵中包含了旋转平移关系，基准mark坐标为（0,0）
					if (m_bTargetCalibUseBoard)
					{
						dPlatformBenchMarkX = 0 ;
						dPlatformBenchMarkY = 0 ;
					}



					// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
					double dImgOffsetRealDistanceX = 0;
					double dImgOffsetRealDistanceY = 0;
					double dImgOffsetX = imageCoordPos.m_dPosX/* - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX*/;
					double dImgOffsetY = imageCoordPos.m_dPosY/* - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY*/;
					dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
					dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

					str.Format("基准图像位置： X = %f , Y = %f ",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前图像位置： X = %f , Y = %f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("图像像素移动量： X = %f , Y = %f ",dImgOffsetX,dImgOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("使用2*2矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

					if (m_bTargetObjectCamSeparate)
					{
						if (GetMidPlatformType() ==ePlatformXYPD)
						{
							// 目标、对象分离，目标平台坐标
							if (nIndex%m_iMarkNum >= m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}						
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，对象平台坐标
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}
						else
						{
							// 目标、对象分离，对象点平台坐标计算
							if (nIndex%m_iMarkNum < m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}		

									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，目标点平台坐标计算
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}

					}
					else
					{
						if (m_PlatformCalibType == ePlatformCalib9Point)
						{
							sc2Vector vSrc;
							sc2Vector vDst;
							vSrc.SetX(dImgOffsetX);
							vSrc.SetY(dImgOffsetY);
							if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
							{
								sc2Vector vBoard;
								vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
								vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
							}
							else
							{
								vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
							}

							dImgOffsetRealDistanceX = vDst.GetX();
							dImgOffsetRealDistanceY = vDst.GetY();
						}
					}

					str.Format("图像像素平台移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);


					// $4、内容相加，即为相机实时Mark的平台坐标值
					platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
					platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
					//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
					platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

					// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响
					if (eCamPlatformSepXY == m_pCamPlatformInfo->GetCamPlatformType() 
						|| eCamPlatformSepX == m_pCamPlatformInfo->GetCamPlatformType()
						|| eCamPlatformShareX == m_pCamPlatformInfo->GetCamPlatformType())
					{
						// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响

						// 相机轴
						CPlatformXYAxisPos*   pCamPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformAxisPos());
						CPlatformXYAxisPos*   pCamPlatformXYAxisPos =	 (CPlatformXYAxisPos*)pCamPlatformAxisPos;

						// 相机轴所在轴
						CPlatformXYAxisPos*	  pCamAxisPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamAxisPlatformAxisPos());
						CPlatformXYAxisPos*   pCamAxisPlatformXYAxisPos = (CPlatformXYAxisPos*)pCamAxisPlatformAxisPos;

						double dCameraMovementX = 0;				// 相机轴运动量X
						double dCameraMovementY = 0;				// 相机轴运动量Y
						double dCameraOffsetRealDistanceX = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响X
						double dCameraOffsetRealDistanceY = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响Y

						double dCamAxisPlatformAxisMovementX = 0;	// 相机轴所在轴运动量X
						double dCamAxisPlatformAxisMovementY = 0;	// 相机轴所在轴运动量Y

						// 相机轴运动量对实时Mark在平台坐标系中的位置的影响
						if (NULL != pCamPlatformBenchAxisPos && NULL != pCamPlatformXYAxisPos)
						{
							str.Format("相机轴基准坐标： X = %f , Y = %f ",pCamPlatformBenchAxisPos->m_dPosX,pCamPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机轴坐标： X = %f , Y = %f ",pCamPlatformXYAxisPos->m_dPosX,pCamPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamPlatformXCoordType = m_pCamPlatformInfo->GetCamDirectX(nIndex%m_iMarkNum);
							int iCamPlatformYCoordType = m_pCamPlatformInfo->GetCamDirectY(nIndex%m_iMarkNum);


							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCameraMovementX = 0;
								dCameraMovementY = 0;

								str.Format("相机移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								dCameraOffsetRealDistanceX = dCameraMovementX ;
								dCameraOffsetRealDistanceY = dCameraMovementY ;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCameraMovementX =pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if (eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -1*dCameraMovementX;

								dCameraOffsetRealDistanceX = dCameraMovementX;
								dCameraOffsetRealDistanceY = dCameraMovementY;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamPlatformYCoordType)
								{
									dCameraMovementY = pCamPlatformXYAxisPos->m_dPosY - pCamPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);


								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;


								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11 + dCameraMovementY*dCamTranferA12;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA21 + dCameraMovementY*dCamTranferA22;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;

								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA12;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
						}

						// 相机轴所在轴运动量对实时Mark在平台坐标系中的位置的影响
						if ( NULL != pCamAxisPlatformBenchAxisPos && NULL != pCamAxisPlatformXYAxisPos)
						{
							str.Format("相机平台所在轴基准坐标： X = %f , Y = %f ",pCamAxisPlatformBenchAxisPos->m_dPosX,pCamAxisPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机平台所在轴坐标： X = %f , Y = %f ",pCamAxisPlatformXYAxisPos->m_dPosX,pCamAxisPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamAxisPlatformAxisXCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectX(nIndex%m_iMarkNum);
							int iCamAxisPlatformAxisYCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectY(nIndex%m_iMarkNum);

							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCamAxisPlatformAxisMovementX = 0;
								dCamAxisPlatformAxisMovementY = 0;

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 

								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								dCamAxisPlatformAxisMovementY = 0;
							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								if(eDirectNone != iCamAxisPlatformAxisXCoordType)
								{
									dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamAxisPlatformAxisYCoordType)
								{
									dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								if(eDirectNegative == iCamAxisPlatformAxisYCoordType) dCamAxisPlatformAxisMovementY = -dCamAxisPlatformAxisMovementY;
							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
							}
						}

						// $4、三部分内容相加，即为相机实时Mark的平台坐标值
						platformCoordPos.m_dPosX += dCameraOffsetRealDistanceX + dCamAxisPlatformAxisMovementX;
						platformCoordPos.m_dPosY += dCameraOffsetRealDistanceY + dCamAxisPlatformAxisMovementY;
					}

					str.Format("平台坐标值： X = %f , Y = %f , D = %f ",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

				}
				else
				{
					if (m_bEnableMultiCalibExtension == TRUE)
					{
						if (nIndex<0 || nIndex >= m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1))
						{
							return false;
						}
					}
					else
					{
						if (nIndex<0 || nIndex >= m_iMarkNum)
						{
							return false;
						}
					}

					if (nIndex >= m_vAlignerCalibInfos.size())
					{
						return false;
					}

					if (NULL == m_vAlignerCalibInfos.at(nIndex))
					{
						return false;
					}

					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
						{
							return false;
						}
					}

					int nPlatformIndex = m_nPlatformIndex;
					CString str;

					//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
					//相机平台类型没有基类，不用改；
					CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
					CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
					CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 

					if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
					{
						return false;
					}

					// 当目标自动标定不关联时，目标标定时轴位置通常是贴合时轴位置，而不是对象拍照时轴位置，
					// 按照目标标定时轴位置计算得到目标平台坐标很大，直接对位计算会超出视野，需要使用目标补偿解决，但是最后一次对位后位置会直接跑到贴合位
					// 使用对象标定时轴位置0/1替换目标标定时轴位置2/3，也可以解决这个问题，而不使用目标补偿
					if (m_bTargetObjectCamSeparate == TRUE && m_bTarObjCamSepNoAxisCalib == FALSE
						&& (GetMidPlatformType() != ePlatformXYPD && nIndex%m_iMarkNum >= m_iMarkNum/2) && m_bTarObjCamSepAutoCalibTar == TRUE && m_eTarAutoCalibAlignMode == eAlignOnObject)
					{
						pPlatformBenchAxisPos = (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex - m_iMarkNum/2)->GetPlatformAxisPos());
					}

					str.Format("轴基准坐标： X = %f , Y = %f , D = %f ",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前轴坐标： X = %f , Y = %f , D = %f ",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);


					// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
					//标定参数
					double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
					double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
					double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
					double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

					double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
					double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();

					str.Format("Mark平台基准位置： X = %f , Y = %f ",dPlatformBenchMarkX,dPlatformBenchMarkY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台各轴的方向	
					int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
					int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;


					// 实际平台XY轴偏移量
					double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
					double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY - pPlatformBenchAxisPos->m_dPosY;

					if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
					if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;

					if ( GetMidPlatformType() ==ePlatformXDPY || GetMidPlatformType() ==ePlatformXDPY1)
					{
						dPlatformOffsetY =0 ;
					}
					else if ( GetMidPlatformType() ==ePlatformXYPD && m_bCalibrateChangeXDirection && m_bCalibrateChangeYDirection)
					{

					}
					else if ( GetMidPlatformType() ==ePlatformXYPD || GetMidPlatformType() ==ePlatformXY || GetMidPlatformType()==ePlatformX || GetMidPlatformType() == ePlatformY)
					{
						dPlatformOffsetX =0 ;
						dPlatformOffsetY =0 ;
					}

					str.Format("轴平移量： X = %f , Y = %f ",dPlatformOffsetX,dPlatformOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);


					//平台XY平移量对基准mark平台坐标的影响方向相反
					dPlatformBenchMarkX -= dPlatformOffsetX;
					dPlatformBenchMarkY -= dPlatformOffsetY;

					// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
					double dImgOffsetRealDistanceX = 0;
					double dImgOffsetRealDistanceY = 0;
					double dImgOffsetX = imageCoordPos.m_dPosX - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX;
					double dImgOffsetY = imageCoordPos.m_dPosY - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY;
					dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
					dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

					str.Format("基准图像位置： X = %f , Y = %f ",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前图像位置： X = %f , Y = %f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("图像像素移动量： X = %f , Y = %f ",dImgOffsetX,dImgOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("使用2*2矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

					if (m_bTargetObjectCamSeparate)
					{
						if (GetMidPlatformType() ==ePlatformXYPD)
						{
							// 目标、对象分离，目标平台坐标
							if (nIndex%m_iMarkNum >= m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}						
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，对象平台坐标
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}
						else
						{
							// 目标、对象分离，对象点平台坐标计算
							if (nIndex%m_iMarkNum < m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}		

									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，目标点平台坐标计算
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}

					}
					else
					{
						if (m_PlatformCalibType == ePlatformCalib9Point)
						{
							sc2Vector vSrc;
							sc2Vector vDst;
							vSrc.SetX(dImgOffsetX);
							vSrc.SetY(dImgOffsetY);
							if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
							{
								sc2Vector vBoard;
								vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
								vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
							}
							else
							{
								vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
							}

							dImgOffsetRealDistanceX = vDst.GetX();
							dImgOffsetRealDistanceY = vDst.GetY();
						}
					}

					str.Format("图像像素平台移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);


					// $4、内容相加，即为相机实时Mark的平台坐标值
					platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
					platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
					//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
					platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

					// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响
					if (eCamPlatformSepXY == m_pCamPlatformInfo->GetCamPlatformType() 
						|| eCamPlatformSepX == m_pCamPlatformInfo->GetCamPlatformType()
						|| eCamPlatformShareX == m_pCamPlatformInfo->GetCamPlatformType())
					{
						// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响

						// 相机轴
						CPlatformXYAxisPos*   pCamPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformAxisPos());
						CPlatformXYAxisPos*   pCamPlatformXYAxisPos =	 (CPlatformXYAxisPos*)pCamPlatformAxisPos;

						// 相机轴所在轴
						CPlatformXYAxisPos*	  pCamAxisPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamAxisPlatformAxisPos());
						CPlatformXYAxisPos*   pCamAxisPlatformXYAxisPos = (CPlatformXYAxisPos*)pCamAxisPlatformAxisPos;

						double dCameraMovementX = 0;				// 相机轴运动量X
						double dCameraMovementY = 0;				// 相机轴运动量Y
						double dCameraOffsetRealDistanceX = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响X
						double dCameraOffsetRealDistanceY = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响Y

						double dCamAxisPlatformAxisMovementX = 0;	// 相机轴所在轴运动量X
						double dCamAxisPlatformAxisMovementY = 0;	// 相机轴所在轴运动量Y

						// 相机轴运动量对实时Mark在平台坐标系中的位置的影响
						if (NULL != pCamPlatformBenchAxisPos && NULL != pCamPlatformXYAxisPos)
						{
							str.Format("相机轴基准坐标： X = %f , Y = %f ",pCamPlatformBenchAxisPos->m_dPosX,pCamPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机轴坐标： X = %f , Y = %f ",pCamPlatformXYAxisPos->m_dPosX,pCamPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamPlatformXCoordType = m_pCamPlatformInfo->GetCamDirectX(nIndex%m_iMarkNum);
							int iCamPlatformYCoordType = m_pCamPlatformInfo->GetCamDirectY(nIndex%m_iMarkNum);


							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCameraMovementX = 0;
								dCameraMovementY = 0;

								str.Format("相机移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								dCameraOffsetRealDistanceX = dCameraMovementX ;
								dCameraOffsetRealDistanceY = dCameraMovementY ;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCameraMovementX =pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if (eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -1*dCameraMovementX;

								dCameraOffsetRealDistanceX = dCameraMovementX;
								dCameraOffsetRealDistanceY = dCameraMovementY;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamPlatformYCoordType)
								{
									dCameraMovementY = pCamPlatformXYAxisPos->m_dPosY - pCamPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);


								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;


								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11 + dCameraMovementY*dCamTranferA12;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA21 + dCameraMovementY*dCamTranferA22;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;

								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA12;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
						}

						// 相机轴所在轴运动量对实时Mark在平台坐标系中的位置的影响
						if ( NULL != pCamAxisPlatformBenchAxisPos && NULL != pCamAxisPlatformXYAxisPos)
						{
							str.Format("相机平台所在轴基准坐标： X = %f , Y = %f ",pCamAxisPlatformBenchAxisPos->m_dPosX,pCamAxisPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机平台所在轴坐标： X = %f , Y = %f ",pCamAxisPlatformXYAxisPos->m_dPosX,pCamAxisPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamAxisPlatformAxisXCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectX(nIndex%m_iMarkNum);
							int iCamAxisPlatformAxisYCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectY(nIndex%m_iMarkNum);

							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCamAxisPlatformAxisMovementX = 0;
								dCamAxisPlatformAxisMovementY = 0;

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 

								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								dCamAxisPlatformAxisMovementY = 0;
							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								if(eDirectNone != iCamAxisPlatformAxisXCoordType)
								{
									dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamAxisPlatformAxisYCoordType)
								{
									dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								if(eDirectNegative == iCamAxisPlatformAxisYCoordType) dCamAxisPlatformAxisMovementY = -dCamAxisPlatformAxisMovementY;
							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
							}
						}

						// $4、三部分内容相加，即为相机实时Mark的平台坐标值
						platformCoordPos.m_dPosX += dCameraOffsetRealDistanceX + dCamAxisPlatformAxisMovementX;
						platformCoordPos.m_dPosY += dCameraOffsetRealDistanceY + dCamAxisPlatformAxisMovementY;
					}

					str.Format("平台坐标值： X = %f , Y = %f , D = %f ",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

				}
			}

		}
		// 棋盘格
		else
		{
			// FAAL
			if (m_bBendAlignProcess)
			{
				if (m_AlignermentParam.GetBendInfo().m_bBendAlignDmCode)
				{
					// 定制应用
					// 从图像坐标转换平台坐标思路：先从图像转为靶标（2*2矩阵+基准MARK图像/平台坐标），再从靶标转为平台（3*3矩阵）
					// 假设没有进行靶标到平台的关联标定，3*3矩阵默认为单位阵，即认为靶标等于平台，也不影响图像坐标转换和对位计算

					if (nIndex<0 || nIndex >= m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1))
					{
						return false;
					}

					if (nIndex >= m_vAlignerCalibInfos.size())
					{
						return false;
					}

					if (NULL == m_vAlignerCalibInfos.at(nIndex))
					{
						return false;
					}

					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
						{
							return false;
						}
					}

					int nPlatformIndex = m_nPlatformIndex;
					CString str;

					//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
					//相机平台类型没有基类，不用改；
					CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
					CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
					CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 
					//	CString str;

					if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
					{
						return false;
					}

					str.Format("轴基准坐标：X=%f,Y=%f,D=%f",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前轴坐标：X=%f,Y=%f,D=%f",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

					// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
					//标定参数
					double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
					double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
					double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
					double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

					double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
					double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();

					str.Format("Mark平台基准位置：X=%f,Y=%f",dPlatformBenchMarkX,dPlatformBenchMarkY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台各轴的方向	
					int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
					int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;

					// 实际平台XY轴偏移量
					double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
					double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY - pPlatformBenchAxisPos->m_dPosY;

					// 实际在使用时，会要求PLC在每次拍照时均发送轴位置0/0/0，靶标标定时轴位置0/0/0，因此轴位置偏差等于0/0/0

					if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
					if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;
					if ( GetMidPlatformType() ==ePlatformXDPY || GetMidPlatformType() ==ePlatformXDPY1)
					{
						dPlatformOffsetY =0 ;
					}
					else if ( GetMidPlatformType() ==ePlatformXYPD || GetMidPlatformType() ==ePlatformXY || GetMidPlatformType()==ePlatformX || GetMidPlatformType() == ePlatformY)
					{
						dPlatformOffsetX =0 ;
						dPlatformOffsetY =0 ;
					}

					str.Format("轴平移量：X=%f,Y=%f",dPlatformOffsetX,dPlatformOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台XY平移量对基准mark平台坐标的影响方向相反
					dPlatformBenchMarkX -= dPlatformOffsetX;
					dPlatformBenchMarkY -= dPlatformOffsetY;


					// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
					double dImgOffsetRealDistanceX = 0;
					double dImgOffsetRealDistanceY = 0;
					double dImgOffsetX = imageCoordPos.m_dPosX - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX;
					double dImgOffsetY = imageCoordPos.m_dPosY - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY;
					dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
					dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

					str.Format("基准图像位置：X=%f,Y=%f",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前图像位置：X=%f,Y=%f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("图像像素移动量：X=%f,Y=%f",dImgOffsetX,dImgOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("图像像素平台移动量：X=%f,Y=%f",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

					// $4、内容相加，即为相机实时Mark的平台坐标值
					platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
					platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
					//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
					platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

					str.Format("平台坐标值：X=%f,Y=%f,D=%f",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);



					// 上述计算得到的是靶标坐标，下面根据靶标到平台的单应矩阵，将靶标坐标转换成平台坐标
					// 如果有关联标定，则根据3*3单应矩阵转换即可
					// 如果无关联标定，3*3单应矩阵等于单位矩阵，转换后等于原数值

					str.Format("投影前靶标坐标值：X=%f,Y=%f,D=%f",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

					// 生成用于单应矩阵投影的平台坐标
					sc2Vector boardPos;
					boardPos.SetX(platformCoordPos.GetPosX());
					boardPos.SetY(platformCoordPos.GetPosY());

					// 得到投影后平台坐标
					sc2Vector platformPos = sfHomographyProject(boardPos, m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);

					// 将投影后平台坐标输出
					platformCoordPos.m_dPosX = platformPos.GetX();
					platformCoordPos.m_dPosY = platformPos.GetY();
					platformCoordPos.m_dAngle = 0;

					str.Format("投影后平台坐标值：X=%f,Y=%f,D=%f",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("投影单应矩阵：0=%f,1=%f,2=%f",m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[0],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[1],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[2]);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("投影单应矩阵：3=%f,4=%f,5=%f",m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[3],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[4],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[5]);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("投影单应矩阵：6=%f,7=%f,8=%f",m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[6],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[7],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[8]);
					AlignToolLogRecord(str,nPlatformIndex);


					// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响
					if (eCamPlatformSepXY == m_pCamPlatformInfo->GetCamPlatformType() 
						|| eCamPlatformSepX == m_pCamPlatformInfo->GetCamPlatformType()
						|| eCamPlatformShareX == m_pCamPlatformInfo->GetCamPlatformType())
					{
						// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响

						// 相机轴
						CPlatformXYAxisPos*   pCamPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformAxisPos());
						CPlatformXYAxisPos*   pCamPlatformXYAxisPos =	 (CPlatformXYAxisPos*)pCamPlatformAxisPos;

						// 相机轴所在轴
						CPlatformXYAxisPos*	  pCamAxisPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamAxisPlatformAxisPos());
						CPlatformXYAxisPos*   pCamAxisPlatformXYAxisPos = (CPlatformXYAxisPos*)pCamAxisPlatformAxisPos;

						double dCameraMovementX = 0;				// 相机轴运动量X
						double dCameraMovementY = 0;				// 相机轴运动量Y
						double dCameraOffsetRealDistanceX = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响X
						double dCameraOffsetRealDistanceY = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响Y

						double dCamAxisPlatformAxisMovementX = 0;	// 相机轴所在轴运动量X
						double dCamAxisPlatformAxisMovementY = 0;	// 相机轴所在轴运动量Y

						// 相机轴运动量对实时Mark在平台坐标系中的位置的影响
						if (NULL != pCamPlatformBenchAxisPos && NULL != pCamPlatformXYAxisPos)
						{
							str.Format("相机轴基准坐标：X=%f,Y=%f",pCamPlatformBenchAxisPos->m_dPosX,pCamPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机轴坐标：X=%f,Y=%f",pCamPlatformXYAxisPos->m_dPosX,pCamPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamPlatformXCoordType = m_pCamPlatformInfo->GetCamDirectX(nIndex%m_iMarkNum);
							int iCamPlatformYCoordType = m_pCamPlatformInfo->GetCamDirectY(nIndex%m_iMarkNum);


							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCameraMovementX = 0;
								dCameraMovementY = 0;

								str.Format("相机移动量移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								dCameraOffsetRealDistanceX = dCameraMovementX ;
								dCameraOffsetRealDistanceY = dCameraMovementY ;

								str.Format("相机移动带来平台移动值：X=%f,Y=%f",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCameraMovementX =pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
								dCameraMovementY = 0;

								str.Format("相机移动量移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if (eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -1*dCameraMovementX;

								dCameraOffsetRealDistanceX = dCameraMovementX;
								dCameraOffsetRealDistanceY = dCameraMovementY;

								str.Format("相机移动带来平台移动值：X=%f,Y=%f",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamPlatformYCoordType)
								{
									dCameraMovementY = pCamPlatformXYAxisPos->m_dPosY - pCamPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机移动量移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);


								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;


								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11 + dCameraMovementY*dCamTranferA12;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA21 + dCameraMovementY*dCamTranferA22;

								str.Format("相机移动带来平台移动值：X=%f,Y=%f",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								dCameraMovementY = 0;

								str.Format("相机移动量移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;

								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA12;

								str.Format("相机移动带来平台移动值：X=%f,Y=%f",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
						}
						else
						{
							return false;
						}

						// 相机轴所在轴运动量对实时Mark在平台坐标系中的位置的影响
						if ( NULL != pCamAxisPlatformBenchAxisPos && NULL != pCamAxisPlatformXYAxisPos)
						{
							str.Format("相机平台所在轴基准坐标：X=%f,Y=%f",pCamAxisPlatformBenchAxisPos->m_dPosX,pCamAxisPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机平台所在轴坐标：X=%f,Y=%f",pCamAxisPlatformXYAxisPos->m_dPosX,pCamAxisPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamAxisPlatformAxisXCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectX(nIndex%m_iMarkNum);
							int iCamAxisPlatformAxisYCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectY(nIndex%m_iMarkNum);

							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCamAxisPlatformAxisMovementX = 0;
								dCamAxisPlatformAxisMovementY = 0;

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								dCamAxisPlatformAxisMovementY = 0;
							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								if(eDirectNone != iCamAxisPlatformAxisXCoordType)
								{
									dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamAxisPlatformAxisYCoordType)
								{
									dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								if(eDirectNegative == iCamAxisPlatformAxisYCoordType) dCamAxisPlatformAxisMovementY = -dCamAxisPlatformAxisMovementY;
							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
							}
						}

						// $4、三部分内容相加，即为相机实时Mark的平台坐标值
						platformCoordPos.m_dPosX += dCameraOffsetRealDistanceX + dCamAxisPlatformAxisMovementX;
						platformCoordPos.m_dPosY += dCameraOffsetRealDistanceY + dCamAxisPlatformAxisMovementY;
					}


					str.Format("平台坐标值：X=%f,Y=%f,D=%f",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
				}
				else
				{
					if (m_bEnableMultiCalibExtension == TRUE)
					{
						if (nIndex<0 || nIndex >= m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1))
						{
							return false;
						}
					}
					else
					{
						if (nIndex<0 || nIndex >= m_iMarkNum)
						{
							return false;
						}
					}

					if (nIndex >= m_vAlignerCalibInfos.size())
					{
						return false;
					}

					if (NULL == m_vAlignerCalibInfos.at(nIndex))
					{
						return false;
					}

					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
						{
							return false;
						}
					}

					int nPlatformIndex = m_nPlatformIndex;
					CString str;

					//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
					//相机平台类型没有基类，不用改；
					CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
					CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
					CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 

					if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
					{
						return false;
					}

					// 当目标自动标定不关联时，目标标定时轴位置通常是贴合时轴位置，而不是对象拍照时轴位置，
					// 按照目标标定时轴位置计算得到目标平台坐标很大，直接对位计算会超出视野，需要使用目标补偿解决，但是最后一次对位后位置会直接跑到贴合位
					// 使用对象标定时轴位置0/1替换目标标定时轴位置2/3，也可以解决这个问题，而不使用目标补偿
					if (m_bTargetObjectCamSeparate == TRUE && m_bTarObjCamSepNoAxisCalib == FALSE
						&& (GetMidPlatformType() != ePlatformXYPD && nIndex%m_iMarkNum >= m_iMarkNum/2) && m_bTarObjCamSepAutoCalibTar == TRUE && m_eTarAutoCalibAlignMode == eAlignOnObject)
					{
						pPlatformBenchAxisPos = (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex - m_iMarkNum/2)->GetPlatformAxisPos());
					}

					str.Format("轴基准坐标： X = %f , Y = %f , D = %f ",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前轴坐标： X = %f , Y = %f , D = %f ",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);


					// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
					//标定参数
					double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
					double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
					double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
					double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

					double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
					double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();

					str.Format("Mark平台基准位置： X = %f , Y = %f ",dPlatformBenchMarkX,dPlatformBenchMarkY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台各轴的方向	
					int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
					int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;


					// 实际平台XY轴偏移量
					double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
					double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY - pPlatformBenchAxisPos->m_dPosY;

					if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
					if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;

					if ( GetMidPlatformType() ==ePlatformXDPY || GetMidPlatformType() ==ePlatformXDPY1)
					{
						dPlatformOffsetY =0 ;
					}
					else if ( GetMidPlatformType() ==ePlatformXYPD && m_bCalibrateChangeXDirection && m_bCalibrateChangeYDirection)
					{

					}
					else if ( GetMidPlatformType() ==ePlatformXYPD || GetMidPlatformType() ==ePlatformXY || GetMidPlatformType()==ePlatformX || GetMidPlatformType() == ePlatformY)
					{
						dPlatformOffsetX =0 ;
						dPlatformOffsetY =0 ;
					}

					str.Format("轴平移量： X = %f , Y = %f ",dPlatformOffsetX,dPlatformOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);


					//平台XY平移量对基准mark平台坐标的影响方向相反
					dPlatformBenchMarkX -= dPlatformOffsetX;
					dPlatformBenchMarkY -= dPlatformOffsetY;

					// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
					double dImgOffsetRealDistanceX = 0;
					double dImgOffsetRealDistanceY = 0;
					double dImgOffsetX = imageCoordPos.m_dPosX - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX;
					double dImgOffsetY = imageCoordPos.m_dPosY - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY;
					dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
					dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

					str.Format("基准图像位置： X = %f , Y = %f ",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前图像位置： X = %f , Y = %f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("图像像素移动量： X = %f , Y = %f ",dImgOffsetX,dImgOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("使用2*2矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

					if (m_bTargetObjectCamSeparate)
					{
						if (GetMidPlatformType() ==ePlatformXYPD)
						{
							// 目标、对象分离，目标平台坐标
							if (nIndex%m_iMarkNum >= m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}						
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，对象平台坐标
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}
						else
						{
							// 目标、对象分离，对象点平台坐标计算
							if (nIndex%m_iMarkNum < m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}		

									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，目标点平台坐标计算
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}

					}
					else
					{
						if (m_PlatformCalibType == ePlatformCalib9Point)
						{
							sc2Vector vSrc;
							sc2Vector vDst;
							vSrc.SetX(dImgOffsetX);
							vSrc.SetY(dImgOffsetY);
							if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
							{
								sc2Vector vBoard;
								vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
								vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
							}
							else
							{
								vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
							}

							dImgOffsetRealDistanceX = vDst.GetX();
							dImgOffsetRealDistanceY = vDst.GetY();
						}
					}

					str.Format("图像像素平台移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);


					// $4、内容相加，即为相机实时Mark的平台坐标值
					platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
					platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
					//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
					platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

					// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响
					if (eCamPlatformSepXY == m_pCamPlatformInfo->GetCamPlatformType() 
						|| eCamPlatformSepX == m_pCamPlatformInfo->GetCamPlatformType()
						|| eCamPlatformShareX == m_pCamPlatformInfo->GetCamPlatformType())
					{
						// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响

						// 相机轴
						CPlatformXYAxisPos*   pCamPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformAxisPos());
						CPlatformXYAxisPos*   pCamPlatformXYAxisPos =	 (CPlatformXYAxisPos*)pCamPlatformAxisPos;

						// 相机轴所在轴
						CPlatformXYAxisPos*	  pCamAxisPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamAxisPlatformAxisPos());
						CPlatformXYAxisPos*   pCamAxisPlatformXYAxisPos = (CPlatformXYAxisPos*)pCamAxisPlatformAxisPos;

						double dCameraMovementX = 0;				// 相机轴运动量X
						double dCameraMovementY = 0;				// 相机轴运动量Y
						double dCameraOffsetRealDistanceX = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响X
						double dCameraOffsetRealDistanceY = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响Y

						double dCamAxisPlatformAxisMovementX = 0;	// 相机轴所在轴运动量X
						double dCamAxisPlatformAxisMovementY = 0;	// 相机轴所在轴运动量Y

						// 相机轴运动量对实时Mark在平台坐标系中的位置的影响
						if (NULL != pCamPlatformBenchAxisPos && NULL != pCamPlatformXYAxisPos)
						{
							str.Format("相机轴基准坐标： X = %f , Y = %f ",pCamPlatformBenchAxisPos->m_dPosX,pCamPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机轴坐标： X = %f , Y = %f ",pCamPlatformXYAxisPos->m_dPosX,pCamPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamPlatformXCoordType = m_pCamPlatformInfo->GetCamDirectX(nIndex%m_iMarkNum);
							int iCamPlatformYCoordType = m_pCamPlatformInfo->GetCamDirectY(nIndex%m_iMarkNum);


							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCameraMovementX = 0;
								dCameraMovementY = 0;

								str.Format("相机移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								dCameraOffsetRealDistanceX = dCameraMovementX ;
								dCameraOffsetRealDistanceY = dCameraMovementY ;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCameraMovementX =pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if (eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -1*dCameraMovementX;

								dCameraOffsetRealDistanceX = dCameraMovementX;
								dCameraOffsetRealDistanceY = dCameraMovementY;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamPlatformYCoordType)
								{
									dCameraMovementY = pCamPlatformXYAxisPos->m_dPosY - pCamPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);


								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;


								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11 + dCameraMovementY*dCamTranferA12;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA21 + dCameraMovementY*dCamTranferA22;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;

								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA12;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
						}

						// 相机轴所在轴运动量对实时Mark在平台坐标系中的位置的影响
						if ( NULL != pCamAxisPlatformBenchAxisPos && NULL != pCamAxisPlatformXYAxisPos)
						{
							str.Format("相机平台所在轴基准坐标： X = %f , Y = %f ",pCamAxisPlatformBenchAxisPos->m_dPosX,pCamAxisPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机平台所在轴坐标： X = %f , Y = %f ",pCamAxisPlatformXYAxisPos->m_dPosX,pCamAxisPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamAxisPlatformAxisXCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectX(nIndex%m_iMarkNum);
							int iCamAxisPlatformAxisYCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectY(nIndex%m_iMarkNum);

							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCamAxisPlatformAxisMovementX = 0;
								dCamAxisPlatformAxisMovementY = 0;

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 

								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								dCamAxisPlatformAxisMovementY = 0;
							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								if(eDirectNone != iCamAxisPlatformAxisXCoordType)
								{
									dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamAxisPlatformAxisYCoordType)
								{
									dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								if(eDirectNegative == iCamAxisPlatformAxisYCoordType) dCamAxisPlatformAxisMovementY = -dCamAxisPlatformAxisMovementY;
							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
							}
						}

						// $4、三部分内容相加，即为相机实时Mark的平台坐标值
						platformCoordPos.m_dPosX += dCameraOffsetRealDistanceX + dCamAxisPlatformAxisMovementX;
						platformCoordPos.m_dPosY += dCameraOffsetRealDistanceY + dCamAxisPlatformAxisMovementY;
					}

					str.Format("平台坐标值： X = %f , Y = %f , D = %f ",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

				}
			}
			// FCHB
			else
			{				
				if (m_AlignermentParam.GetBendInfo().m_bBendingInspect && m_AlignermentParam.GetBendInfo().m_bBendInspectDmCode)
				{
					// 定制应用
					// 从图像坐标转换平台坐标思路：先从图像转为靶标（2*2矩阵+基准MARK图像/平台坐标），再从靶标转为平台（3*3矩阵）
					// 假设没有进行靶标到平台的关联标定，3*3矩阵默认为单位阵，即认为靶标等于平台，也不影响图像坐标转换和对位计算

					if (nIndex<0 || nIndex >= m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1))
					{
						return false;
					}

					if (nIndex >= m_vAlignerCalibInfos.size())
					{
						return false;
					}

					if (NULL == m_vAlignerCalibInfos.at(nIndex))
					{
						return false;
					}

					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
						{
							return false;
						}
					}

					int nPlatformIndex = m_nPlatformIndex;
					CString str;

					//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
					//相机平台类型没有基类，不用改；
					CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
					CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
					CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 
					//	CString str;

					if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
					{
						return false;
					}

					str.Format("轴基准坐标：X=%f,Y=%f,D=%f",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前轴坐标：X=%f,Y=%f,D=%f",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

					// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
					//标定参数
					double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
					double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
					double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
					double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

					double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
					double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();

					str.Format("Mark平台基准位置：X=%f,Y=%f",dPlatformBenchMarkX,dPlatformBenchMarkY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台各轴的方向	
					int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
					int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;

					// 实际平台XY轴偏移量
					double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
					double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY - pPlatformBenchAxisPos->m_dPosY;

					// 实际在使用时，会要求PLC在每次拍照时均发送轴位置0/0/0，靶标标定时轴位置0/0/0，因此轴位置偏差等于0/0/0

					if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
					if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;
					if ( GetMidPlatformType() ==ePlatformXDPY || GetMidPlatformType() ==ePlatformXDPY1)
					{
						dPlatformOffsetY =0 ;
					}
					else if ( GetMidPlatformType() ==ePlatformXYPD || GetMidPlatformType() ==ePlatformXY || GetMidPlatformType()==ePlatformX || GetMidPlatformType() == ePlatformY)
					{
						dPlatformOffsetX =0 ;
						dPlatformOffsetY =0 ;
					}

					str.Format("轴平移量：X=%f,Y=%f",dPlatformOffsetX,dPlatformOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台XY平移量对基准mark平台坐标的影响方向相反
					dPlatformBenchMarkX -= dPlatformOffsetX;
					dPlatformBenchMarkY -= dPlatformOffsetY;


					// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
					double dImgOffsetRealDistanceX = 0;
					double dImgOffsetRealDistanceY = 0;
					double dImgOffsetX = imageCoordPos.m_dPosX - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX;
					double dImgOffsetY = imageCoordPos.m_dPosY - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY;
					dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
					dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

					str.Format("基准图像位置：X=%f,Y=%f",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前图像位置：X=%f,Y=%f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("图像像素移动量：X=%f,Y=%f",dImgOffsetX,dImgOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("图像像素平台移动量：X=%f,Y=%f",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

					// $4、内容相加，即为相机实时Mark的平台坐标值
					platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
					platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
					//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
					platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

					str.Format("平台坐标值：X=%f,Y=%f,D=%f",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);



					// 上述计算得到的是靶标坐标，下面根据靶标到平台的单应矩阵，将靶标坐标转换成平台坐标
					// 如果有关联标定，则根据3*3单应矩阵转换即可
					// 如果无关联标定，3*3单应矩阵等于单位矩阵，转换后等于原数值

					str.Format("投影前靶标坐标值：X=%f,Y=%f,D=%f",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

					// 生成用于单应矩阵投影的平台坐标
					sc2Vector boardPos;
					boardPos.SetX(platformCoordPos.GetPosX());
					boardPos.SetY(platformCoordPos.GetPosY());

					// 得到投影后平台坐标
					sc2Vector platformPos = sfHomographyProject(boardPos, m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);

					// 将投影后平台坐标输出
					platformCoordPos.m_dPosX = platformPos.GetX();
					platformCoordPos.m_dPosY = platformPos.GetY();
					platformCoordPos.m_dAngle = 0;

					str.Format("投影后平台坐标值：X=%f,Y=%f,D=%f",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("投影单应矩阵：0=%f,1=%f,2=%f",m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[0],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[1],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[2]);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("投影单应矩阵：3=%f,4=%f,5=%f",m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[3],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[4],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[5]);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("投影单应矩阵：6=%f,7=%f,8=%f",m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[6],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[7],m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH[8]);
					AlignToolLogRecord(str,nPlatformIndex);


					// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响
					if (eCamPlatformSepXY == m_pCamPlatformInfo->GetCamPlatformType() 
						|| eCamPlatformSepX == m_pCamPlatformInfo->GetCamPlatformType()
						|| eCamPlatformShareX == m_pCamPlatformInfo->GetCamPlatformType())
					{
						// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响

						// 相机轴
						CPlatformXYAxisPos*   pCamPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformAxisPos());
						CPlatformXYAxisPos*   pCamPlatformXYAxisPos =	 (CPlatformXYAxisPos*)pCamPlatformAxisPos;

						// 相机轴所在轴
						CPlatformXYAxisPos*	  pCamAxisPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamAxisPlatformAxisPos());
						CPlatformXYAxisPos*   pCamAxisPlatformXYAxisPos = (CPlatformXYAxisPos*)pCamAxisPlatformAxisPos;

						double dCameraMovementX = 0;				// 相机轴运动量X
						double dCameraMovementY = 0;				// 相机轴运动量Y
						double dCameraOffsetRealDistanceX = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响X
						double dCameraOffsetRealDistanceY = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响Y

						double dCamAxisPlatformAxisMovementX = 0;	// 相机轴所在轴运动量X
						double dCamAxisPlatformAxisMovementY = 0;	// 相机轴所在轴运动量Y

						// 相机轴运动量对实时Mark在平台坐标系中的位置的影响
						if (NULL != pCamPlatformBenchAxisPos && NULL != pCamPlatformXYAxisPos)
						{
							str.Format("相机轴基准坐标：X=%f,Y=%f",pCamPlatformBenchAxisPos->m_dPosX,pCamPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机轴坐标：X=%f,Y=%f",pCamPlatformXYAxisPos->m_dPosX,pCamPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamPlatformXCoordType = m_pCamPlatformInfo->GetCamDirectX(nIndex%m_iMarkNum);
							int iCamPlatformYCoordType = m_pCamPlatformInfo->GetCamDirectY(nIndex%m_iMarkNum);


							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCameraMovementX = 0;
								dCameraMovementY = 0;

								str.Format("相机移动量移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								dCameraOffsetRealDistanceX = dCameraMovementX ;
								dCameraOffsetRealDistanceY = dCameraMovementY ;

								str.Format("相机移动带来平台移动值：X=%f,Y=%f",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCameraMovementX =pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
								dCameraMovementY = 0;

								str.Format("相机移动量移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if (eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -1*dCameraMovementX;

								dCameraOffsetRealDistanceX = dCameraMovementX;
								dCameraOffsetRealDistanceY = dCameraMovementY;

								str.Format("相机移动带来平台移动值：X=%f,Y=%f",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamPlatformYCoordType)
								{
									dCameraMovementY = pCamPlatformXYAxisPos->m_dPosY - pCamPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机移动量移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);


								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;


								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11 + dCameraMovementY*dCamTranferA12;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA21 + dCameraMovementY*dCamTranferA22;

								str.Format("相机移动带来平台移动值：X=%f,Y=%f",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								dCameraMovementY = 0;

								str.Format("相机移动量移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;

								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA12;

								str.Format("相机移动带来平台移动值：X=%f,Y=%f",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
						}
						else
						{
							return false;
						}

						// 相机轴所在轴运动量对实时Mark在平台坐标系中的位置的影响
						if ( NULL != pCamAxisPlatformBenchAxisPos && NULL != pCamAxisPlatformXYAxisPos)
						{
							str.Format("相机平台所在轴基准坐标：X=%f,Y=%f",pCamAxisPlatformBenchAxisPos->m_dPosX,pCamAxisPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机平台所在轴坐标：X=%f,Y=%f",pCamAxisPlatformXYAxisPos->m_dPosX,pCamAxisPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamAxisPlatformAxisXCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectX(nIndex%m_iMarkNum);
							int iCamAxisPlatformAxisYCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectY(nIndex%m_iMarkNum);

							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCamAxisPlatformAxisMovementX = 0;
								dCamAxisPlatformAxisMovementY = 0;

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								dCamAxisPlatformAxisMovementY = 0;
							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								if(eDirectNone != iCamAxisPlatformAxisXCoordType)
								{
									dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamAxisPlatformAxisYCoordType)
								{
									dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								if(eDirectNegative == iCamAxisPlatformAxisYCoordType) dCamAxisPlatformAxisMovementY = -dCamAxisPlatformAxisMovementY;
							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
							}
						}

						// $4、三部分内容相加，即为相机实时Mark的平台坐标值
						platformCoordPos.m_dPosX += dCameraOffsetRealDistanceX + dCamAxisPlatformAxisMovementX;
						platformCoordPos.m_dPosY += dCameraOffsetRealDistanceY + dCamAxisPlatformAxisMovementY;
					}


					str.Format("平台坐标值：X=%f,Y=%f,D=%f",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
				}
				else
				{
					if (m_bEnableMultiCalibExtension == TRUE)
					{
						if (nIndex<0 || nIndex >= m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1))
						{
							return false;
						}
					}
					else
					{
						if (nIndex<0 || nIndex >= m_iMarkNum)
						{
							return false;
						}
					}

					if (nIndex >= m_vAlignerCalibInfos.size())
					{
						return false;
					}

					if (NULL == m_vAlignerCalibInfos.at(nIndex))
					{
						return false;
					}

					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
						{
							return false;
						}
					}

					int nPlatformIndex = m_nPlatformIndex;
					CString str;

					//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
					//相机平台类型没有基类，不用改；
					CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
					CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
					CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 

					if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
					{
						return false;
					}

					// 当目标自动标定不关联时，目标标定时轴位置通常是贴合时轴位置，而不是对象拍照时轴位置，
					// 按照目标标定时轴位置计算得到目标平台坐标很大，直接对位计算会超出视野，需要使用目标补偿解决，但是最后一次对位后位置会直接跑到贴合位
					// 使用对象标定时轴位置0/1替换目标标定时轴位置2/3，也可以解决这个问题，而不使用目标补偿
					if (m_bTargetObjectCamSeparate == TRUE && m_bTarObjCamSepNoAxisCalib == FALSE
						&& (GetMidPlatformType() != ePlatformXYPD && nIndex%m_iMarkNum >= m_iMarkNum/2) && m_bTarObjCamSepAutoCalibTar == TRUE && m_eTarAutoCalibAlignMode == eAlignOnObject)
					{
						pPlatformBenchAxisPos = (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex - m_iMarkNum/2)->GetPlatformAxisPos());
					}

					str.Format("轴基准坐标： X = %f , Y = %f , D = %f ",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前轴坐标： X = %f , Y = %f , D = %f ",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);


					// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
					//标定参数
					double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
					double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
					double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
					double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

					double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
					double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();

					str.Format("Mark平台基准位置： X = %f , Y = %f ",dPlatformBenchMarkX,dPlatformBenchMarkY);
					AlignToolLogRecord(str,nPlatformIndex);

					//平台各轴的方向	
					int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
					int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;


					// 实际平台XY轴偏移量
					double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
					double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY - pPlatformBenchAxisPos->m_dPosY;

					if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
					if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;

					if ( GetMidPlatformType() ==ePlatformXDPY || GetMidPlatformType() ==ePlatformXDPY1)
					{
						dPlatformOffsetY =0 ;
					}
					else if ( GetMidPlatformType() ==ePlatformXYPD && m_bCalibrateChangeXDirection && m_bCalibrateChangeYDirection)
					{

					}
					else if ( GetMidPlatformType() ==ePlatformXYPD || GetMidPlatformType() ==ePlatformXY || GetMidPlatformType()==ePlatformX || GetMidPlatformType() == ePlatformY)
					{
						dPlatformOffsetX =0 ;
						dPlatformOffsetY =0 ;
					}

					str.Format("轴平移量： X = %f , Y = %f ",dPlatformOffsetX,dPlatformOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);


					//平台XY平移量对基准mark平台坐标的影响方向相反
					dPlatformBenchMarkX -= dPlatformOffsetX;
					dPlatformBenchMarkY -= dPlatformOffsetY;

					// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
					double dImgOffsetRealDistanceX = 0;
					double dImgOffsetRealDistanceY = 0;
					double dImgOffsetX = imageCoordPos.m_dPosX - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX;
					double dImgOffsetY = imageCoordPos.m_dPosY - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY;
					dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
					dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

					str.Format("基准图像位置： X = %f , Y = %f ",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("当前图像位置： X = %f , Y = %f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
					AlignToolLogRecord(str,nPlatformIndex);
					str.Format("图像像素移动量： X = %f , Y = %f ",dImgOffsetX,dImgOffsetY);
					AlignToolLogRecord(str,nPlatformIndex);

					str.Format("使用2*2矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

					if (m_bTargetObjectCamSeparate)
					{
						if (GetMidPlatformType() ==ePlatformXYPD)
						{
							// 目标、对象分离，目标平台坐标
							if (nIndex%m_iMarkNum >= m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}						
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，对象平台坐标
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}
						else
						{
							// 目标、对象分离，对象点平台坐标计算
							if (nIndex%m_iMarkNum < m_iMarkNum/2)
							{
								if (m_PlatformCalibType == ePlatformCalib9Point)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
									{
										sc2Vector vBoard;
										vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
										vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
									}
									else
									{
										vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									}		

									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();
								}
							}
							// 目标、对象分离，目标点平台坐标计算
							else
							{
								if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);
									vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
								else
								{
									sc2Vector vSrc;
									sc2Vector vDst;
									vSrc.SetX(dImgOffsetX);
									vSrc.SetY(dImgOffsetY);

									double h[9];
									memset(h, 0.0, sizeof(double)*9);
									h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

									vDst = sfHomographyProject(vSrc,h);
									dImgOffsetRealDistanceX = vDst.GetX();
									dImgOffsetRealDistanceY = vDst.GetY();

									str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
									AlignToolLogRecord(str,nPlatformIndex);
								}
							}
						}

					}
					else
					{
						if (m_PlatformCalibType == ePlatformCalib9Point)
						{
							sc2Vector vSrc;
							sc2Vector vDst;
							vSrc.SetX(dImgOffsetX);
							vSrc.SetY(dImgOffsetY);
							if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
							{
								sc2Vector vBoard;
								vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
								vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
							}
							else
							{
								vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
							}

							dImgOffsetRealDistanceX = vDst.GetX();
							dImgOffsetRealDistanceY = vDst.GetY();
						}
					}

					str.Format("图像像素平台移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);


					// $4、内容相加，即为相机实时Mark的平台坐标值
					platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
					platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
					//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
					platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

					// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响
					if (eCamPlatformSepXY == m_pCamPlatformInfo->GetCamPlatformType() 
						|| eCamPlatformSepX == m_pCamPlatformInfo->GetCamPlatformType()
						|| eCamPlatformShareX == m_pCamPlatformInfo->GetCamPlatformType())
					{
						// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响

						// 相机轴
						CPlatformXYAxisPos*   pCamPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformAxisPos());
						CPlatformXYAxisPos*   pCamPlatformXYAxisPos =	 (CPlatformXYAxisPos*)pCamPlatformAxisPos;

						// 相机轴所在轴
						CPlatformXYAxisPos*	  pCamAxisPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamAxisPlatformAxisPos());
						CPlatformXYAxisPos*   pCamAxisPlatformXYAxisPos = (CPlatformXYAxisPos*)pCamAxisPlatformAxisPos;

						double dCameraMovementX = 0;				// 相机轴运动量X
						double dCameraMovementY = 0;				// 相机轴运动量Y
						double dCameraOffsetRealDistanceX = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响X
						double dCameraOffsetRealDistanceY = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响Y

						double dCamAxisPlatformAxisMovementX = 0;	// 相机轴所在轴运动量X
						double dCamAxisPlatformAxisMovementY = 0;	// 相机轴所在轴运动量Y

						// 相机轴运动量对实时Mark在平台坐标系中的位置的影响
						if (NULL != pCamPlatformBenchAxisPos && NULL != pCamPlatformXYAxisPos)
						{
							str.Format("相机轴基准坐标： X = %f , Y = %f ",pCamPlatformBenchAxisPos->m_dPosX,pCamPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机轴坐标： X = %f , Y = %f ",pCamPlatformXYAxisPos->m_dPosX,pCamPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamPlatformXCoordType = m_pCamPlatformInfo->GetCamDirectX(nIndex%m_iMarkNum);
							int iCamPlatformYCoordType = m_pCamPlatformInfo->GetCamDirectY(nIndex%m_iMarkNum);


							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCameraMovementX = 0;
								dCameraMovementY = 0;

								str.Format("相机移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								dCameraOffsetRealDistanceX = dCameraMovementX ;
								dCameraOffsetRealDistanceY = dCameraMovementY ;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCameraMovementX =pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if (eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -1*dCameraMovementX;

								dCameraOffsetRealDistanceX = dCameraMovementX;
								dCameraOffsetRealDistanceY = dCameraMovementY;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamPlatformYCoordType)
								{
									dCameraMovementY = pCamPlatformXYAxisPos->m_dPosY - pCamPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);


								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;


								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11 + dCameraMovementY*dCamTranferA12;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA21 + dCameraMovementY*dCamTranferA22;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);

							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
								double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
								double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
								double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
								double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
								if(eDirectNone != iCamPlatformXCoordType)
								{
									dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
								}
								dCameraMovementY = 0;

								str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								//考虑相机平台的方向
								if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
								if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;

								dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11;
								dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA12;

								str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
						}

						// 相机轴所在轴运动量对实时Mark在平台坐标系中的位置的影响
						if ( NULL != pCamAxisPlatformBenchAxisPos && NULL != pCamAxisPlatformXYAxisPos)
						{
							str.Format("相机平台所在轴基准坐标： X = %f , Y = %f ",pCamAxisPlatformBenchAxisPos->m_dPosX,pCamAxisPlatformBenchAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);
							str.Format("当前相机平台所在轴坐标： X = %f , Y = %f ",pCamAxisPlatformXYAxisPos->m_dPosX,pCamAxisPlatformXYAxisPos->m_dPosY);
							AlignToolLogRecord(str,nPlatformIndex);

							int iCamAxisPlatformAxisXCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectX(nIndex%m_iMarkNum);
							int iCamAxisPlatformAxisYCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectY(nIndex%m_iMarkNum);

							// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
							int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
							if (eCamPlatformSepFix == iCameraFixType)
							{
								dCamAxisPlatformAxisMovementX = 0;
								dCamAxisPlatformAxisMovementY = 0;

								str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);
							}
							else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
							{
								dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 

								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								dCamAxisPlatformAxisMovementY = 0;
							}
							else if (eCamPlatformSepXY == iCameraFixType)
							{
								if(eDirectNone != iCamAxisPlatformAxisXCoordType)
								{
									dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
								}
								if(eDirectNone != iCamAxisPlatformAxisYCoordType)
								{
									dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 
								}
								str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
								AlignToolLogRecord(str,nPlatformIndex);

								if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
								if(eDirectNegative == iCamAxisPlatformAxisYCoordType) dCamAxisPlatformAxisMovementY = -dCamAxisPlatformAxisMovementY;
							}
							else if (eCamPlatformSepX == iCameraFixType)
							{
							}
						}

						// $4、三部分内容相加，即为相机实时Mark的平台坐标值
						platformCoordPos.m_dPosX += dCameraOffsetRealDistanceX + dCamAxisPlatformAxisMovementX;
						platformCoordPos.m_dPosY += dCameraOffsetRealDistanceY + dCamAxisPlatformAxisMovementY;
					}

					str.Format("平台坐标值： X = %f , Y = %f , D = %f ",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
					AlignToolLogRecord(str,nPlatformIndex);

				}
			}

		}
	}
	// 不启用产品扩展
	else
	{
		if (m_bEnableMultiCalibExtension == TRUE)
		{
			if (nIndex<0 || nIndex >= m_iMarkNum*(m_nMultiCalibExtensionMaxNum+1))
			{
				return false;
			}
		}
		else
		{
			if (nIndex<0 || nIndex >= m_iMarkNum)
			{
				return false;
			}
		}

		if (nIndex >= m_vAlignerCalibInfos.size())
		{
			return false;
		}

		if (NULL == m_vAlignerCalibInfos.at(nIndex))
		{
			return false;
		}

		if (m_PlatformCalibType == ePlatformCalib9Point)
		{
			if (!m_vAlignerCalibInfos.at(nIndex)->IsValid())
			{
				return false;
			}
		}

		int nPlatformIndex = m_nPlatformIndex;
		CString str;

		//平台、相机平台初始位置；平台、相机平台当前位置；平台参数->转换成相应平台类型
		//相机平台类型没有基类，不用改；
		CPlatformXYDAxisPos*  pPlatformBenchAxisPos =    (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetPlatformAxisPos());
		CPlatformXYDAxisPos*  pPlatformXYDAxisPos =		 (CPlatformXYDAxisPos*)pPlatformAxisPos;
		CPlatformXYDInfo*     pPlatformParam =			 (CPlatformXYDInfo*)m_pPlatformParam; 

		if (NULL == pPlatformBenchAxisPos || NULL == pPlatformXYDAxisPos  || NULL == pPlatformParam)
		{
			return false;
		}

		// 当目标自动标定不关联时，目标标定时轴位置通常是贴合时轴位置，而不是对象拍照时轴位置，
		// 按照目标标定时轴位置计算得到目标平台坐标很大，直接对位计算会超出视野，需要使用目标补偿解决，但是最后一次对位后位置会直接跑到贴合位
		// 使用对象标定时轴位置0/1替换目标标定时轴位置2/3，也可以解决这个问题，而不使用目标补偿
		if (m_bTargetObjectCamSeparate == TRUE && m_bTarObjCamSepNoAxisCalib == FALSE
			&& (GetMidPlatformType() != ePlatformXYPD && nIndex%m_iMarkNum >= m_iMarkNum/2) && m_bTarObjCamSepAutoCalibTar == TRUE && m_eTarAutoCalibAlignMode == eAlignOnObject)
		{
			pPlatformBenchAxisPos = (CPlatformXYDAxisPos*)(m_vAlignerCalibInfos.at(nIndex - m_iMarkNum/2)->GetPlatformAxisPos());
		}

		str.Format("轴基准坐标： X = %f , Y = %f , D = %f ",pPlatformBenchAxisPos->m_dPosX,pPlatformBenchAxisPos->m_dPosY,pPlatformBenchAxisPos->m_dAngle);
		AlignToolLogRecord(str,nPlatformIndex);
		str.Format("当前轴坐标： X = %f , Y = %f , D = %f ",pPlatformXYDAxisPos->m_dPosX,pPlatformXYDAxisPos->m_dPosY,pPlatformXYDAxisPos->m_dAngle);
		AlignToolLogRecord(str,nPlatformIndex);


		// $1、先根据不同的平台类型，修正标定后的基准Mark在平台坐标系中的位置
		//标定参数
		double dTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,0);
		double dTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(0,1);
		double dTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,0);
		double dTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetPlatformTransferMatrix().GetElement(1,1);

		double dPlatformBenchMarkX = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosX();
		double dPlatformBenchMarkY = m_vAlignerCalibInfos.at(nIndex)->GetMarkPlatformCoordPos().GetPosY();

		str.Format("Mark平台基准位置： X = %f , Y = %f ",dPlatformBenchMarkX,dPlatformBenchMarkY);
		AlignToolLogRecord(str,nPlatformIndex);

		//平台各轴的方向	
		int iPlatformXCoordType = pPlatformParam->m_nPlatformXCoordType;
		int iPlatformYCoordType = pPlatformParam->m_nPlatformYCoordType;


		// 实际平台XY轴偏移量
		double dPlatformOffsetX = pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
		double dPlatformOffsetY = pPlatformXYDAxisPos->m_dPosY - pPlatformBenchAxisPos->m_dPosY;

		if (eDirectNegative == iPlatformXCoordType) dPlatformOffsetX = -1*dPlatformOffsetX;	
		if (eDirectNegative == iPlatformYCoordType) dPlatformOffsetY = -1*dPlatformOffsetY;

		if ( GetMidPlatformType() ==ePlatformXDPY || GetMidPlatformType() ==ePlatformXDPY1)
		{
			dPlatformOffsetY =0 ;
		}
		else if ( GetMidPlatformType() ==ePlatformXYPD && m_bCalibrateChangeXDirection && m_bCalibrateChangeYDirection)
		{

		}
		else if ( GetMidPlatformType() ==ePlatformXYPD || GetMidPlatformType() ==ePlatformXY || GetMidPlatformType()==ePlatformX || GetMidPlatformType() == ePlatformY)
		{
			dPlatformOffsetX =0 ;
			dPlatformOffsetY =0 ;
		}

		str.Format("轴平移量： X = %f , Y = %f ",dPlatformOffsetX,dPlatformOffsetY);
		AlignToolLogRecord(str,nPlatformIndex);


		//平台XY平移量对基准mark平台坐标的影响方向相反
		dPlatformBenchMarkX -= dPlatformOffsetX;
		dPlatformBenchMarkY -= dPlatformOffsetY;

		// $2、根据标定后得到的变换矩阵和基准Mark的图像坐标值，计算图像上像素位置偏差对应的实际平台移动量
		double dImgOffsetRealDistanceX = 0;
		double dImgOffsetRealDistanceY = 0;
		double dImgOffsetX = imageCoordPos.m_dPosX - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX;
		double dImgOffsetY = imageCoordPos.m_dPosY - m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY;

		dImgOffsetRealDistanceX = dTranferA11*dImgOffsetX + dTranferA12*dImgOffsetY;
		dImgOffsetRealDistanceY = dTranferA21*dImgOffsetX + dTranferA22*dImgOffsetY;

		str.Format("基准图像位置： X = %f , Y = %f ",m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosX,m_vAlignerCalibInfos.at(nIndex)->GetMarkImgCoordPos().m_dPosY);
		AlignToolLogRecord(str,nPlatformIndex);
		str.Format("当前图像位置： X = %f , Y = %f",imageCoordPos.m_dPosX,imageCoordPos.m_dPosY);
		AlignToolLogRecord(str,nPlatformIndex);
		str.Format("图像像素移动量： X = %f , Y = %f ",dImgOffsetX,dImgOffsetY);
		AlignToolLogRecord(str,nPlatformIndex);

		str.Format("使用2*2矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
		AlignToolLogRecord(str,nPlatformIndex);

		if (m_bTargetObjectCamSeparate)
		{
			if (GetMidPlatformType() ==ePlatformXYPD)
			{
				// 目标、对象分离，目标平台坐标
				if (nIndex%m_iMarkNum >= m_iMarkNum/2)
				{
					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						sc2Vector vSrc;
						sc2Vector vDst;
						vSrc.SetX(dImgOffsetX);
						vSrc.SetY(dImgOffsetY);
						if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
						{
							sc2Vector vBoard;
							vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
							vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
						}
						else
						{
							vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
						}						
						dImgOffsetRealDistanceX = vDst.GetX();
						dImgOffsetRealDistanceY = vDst.GetY();
					}
				}
				// 目标、对象分离，对象平台坐标
				else
				{
					if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
					{
						sc2Vector vSrc;
						sc2Vector vDst;
						vSrc.SetX(dImgOffsetX);
						vSrc.SetY(dImgOffsetY);
						vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
						dImgOffsetRealDistanceX = vDst.GetX();
						dImgOffsetRealDistanceY = vDst.GetY();

						str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
						AlignToolLogRecord(str,nPlatformIndex);
					}
					else
					{
						sc2Vector vSrc;
						sc2Vector vDst;
						vSrc.SetX(dImgOffsetX);
						vSrc.SetY(dImgOffsetY);

						double h[9];
						memset(h, 0.0, sizeof(double)*9);
						h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

						vDst = sfHomographyProject(vSrc,h);
						dImgOffsetRealDistanceX = vDst.GetX();
						dImgOffsetRealDistanceY = vDst.GetY();

						str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
						AlignToolLogRecord(str,nPlatformIndex);
					}
				}
			}
			else
			{
				// 目标、对象分离，对象点平台坐标计算
				if (nIndex%m_iMarkNum < m_iMarkNum/2)
				{
					if (m_PlatformCalibType == ePlatformCalib9Point)
					{
						sc2Vector vSrc;
						sc2Vector vDst;
						vSrc.SetX(dImgOffsetX);
						vSrc.SetY(dImgOffsetY);
						if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
						{
							sc2Vector vBoard;
							vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
							vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
						}
						else
						{
							vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
						}		

						dImgOffsetRealDistanceX = vDst.GetX();
						dImgOffsetRealDistanceY = vDst.GetY();
					}
				}
				// 目标、对象分离，目标点平台坐标计算
				else
				{
					if (m_AlignermentParam.GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point || m_AlignermentParam.GetCalibTargetMarkPatternMode() == eDmCodeCalib || m_bTarObjCamSepAutoCalibTar == TRUE)
					{
						sc2Vector vSrc;
						sc2Vector vDst;
						vSrc.SetX(dImgOffsetX);
						vSrc.SetY(dImgOffsetY);
						vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
						dImgOffsetRealDistanceX = vDst.GetX();
						dImgOffsetRealDistanceY = vDst.GetY();

						str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
						AlignToolLogRecord(str,nPlatformIndex);
					}
					else
					{
						sc2Vector vSrc;
						sc2Vector vDst;
						vSrc.SetX(dImgOffsetX);
						vSrc.SetY(dImgOffsetY);

						double h[9];
						memset(h, 0.0, sizeof(double)*9);
						h[0]=dTranferA11;h[1]=dTranferA12;h[3]=dTranferA21;h[4]=dTranferA22;h[8]=1;

						vDst = sfHomographyProject(vSrc,h);
						dImgOffsetRealDistanceX = vDst.GetX();
						dImgOffsetRealDistanceY = vDst.GetY();

						str.Format("使用3*3矩阵计算图像像素移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
						AlignToolLogRecord(str,nPlatformIndex);
					}
				}
			}

		}
		else
		{
			// 导致折弯检测工位使用二维码靶标标定时，对象目标平台坐标计算错误，H2 H5不为0
			if (m_PlatformCalibType == ePlatformCalib9Point)
			{
				sc2Vector vSrc;
				sc2Vector vDst;
				vSrc.SetX(dImgOffsetX);
				vSrc.SetY(dImgOffsetY);
				if (m_vpCalibrateParam.at(nRemIndex)->GetCalibMarkPatternMode() == eWholeDmcodeBoardCalib)
				{
					sc2Vector vBoard;
					vBoard = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH1);
					vDst = sfHomographyProject(vBoard,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH2);
				}
				else
				{
					vDst = sfHomographyProject(vSrc,m_vAlignerCalibInfos.at(nIndex)->m_PlatformTranferH);
				}

				dImgOffsetRealDistanceX = vDst.GetX();
				dImgOffsetRealDistanceY = vDst.GetY();
			}
		}

		str.Format("图像像素平台移动量： X = %f , Y = %f ",dImgOffsetRealDistanceX,dImgOffsetRealDistanceY);
		AlignToolLogRecord(str,nPlatformIndex);


		// $4、内容相加，即为相机实时Mark的平台坐标值
		platformCoordPos.m_dPosX = dPlatformBenchMarkX + dImgOffsetRealDistanceX ;
		platformCoordPos.m_dPosY = dPlatformBenchMarkY + dImgOffsetRealDistanceY ;
		//mark为线时，平台mark线的角度为图像线的角度+图像平台坐标之间的夹角；
		platformCoordPos.m_dAngle = imageCoordPos.GetAngle() + 	m_vAlignerCalibInfos.at(nIndex)->m_dImageToPlatformAnlge;

		// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响
		if (eCamPlatformSepXY == m_pCamPlatformInfo->GetCamPlatformType() 
			|| eCamPlatformSepX == m_pCamPlatformInfo->GetCamPlatformType()
			|| eCamPlatformShareX == m_pCamPlatformInfo->GetCamPlatformType())
		{
			// $3、根据相机安装方式，计算由相机运动量对实时Mark在平台坐标系中的位置的影响

			// 相机轴
			CPlatformXYAxisPos*   pCamPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformAxisPos());
			CPlatformXYAxisPos*   pCamPlatformXYAxisPos =	 (CPlatformXYAxisPos*)pCamPlatformAxisPos;

			// 相机轴所在轴
			CPlatformXYAxisPos*	  pCamAxisPlatformBenchAxisPos = (CPlatformXYAxisPos*)(m_vAlignerCalibInfos.at(nIndex)->GetCamAxisPlatformAxisPos());
			CPlatformXYAxisPos*   pCamAxisPlatformXYAxisPos = (CPlatformXYAxisPos*)pCamAxisPlatformAxisPos;

			double dCameraMovementX = 0;				// 相机轴运动量X
			double dCameraMovementY = 0;				// 相机轴运动量Y
			double dCameraOffsetRealDistanceX = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响X
			double dCameraOffsetRealDistanceY = 0;		// 相机轴运动量对实时Mark在平台坐标系中的位置的影响Y

			double dCamAxisPlatformAxisMovementX = 0;	// 相机轴所在轴运动量X
			double dCamAxisPlatformAxisMovementY = 0;	// 相机轴所在轴运动量Y

			// 相机轴运动量对实时Mark在平台坐标系中的位置的影响
			if (NULL != pCamPlatformBenchAxisPos && NULL != pCamPlatformXYAxisPos)
			{
				str.Format("相机轴基准坐标： X = %f , Y = %f ",pCamPlatformBenchAxisPos->m_dPosX,pCamPlatformBenchAxisPos->m_dPosY);
				AlignToolLogRecord(str,nPlatformIndex);
				str.Format("当前相机轴坐标： X = %f , Y = %f ",pCamPlatformXYAxisPos->m_dPosX,pCamPlatformXYAxisPos->m_dPosY);
				AlignToolLogRecord(str,nPlatformIndex);

				int iCamPlatformXCoordType = m_pCamPlatformInfo->GetCamDirectX(nIndex%m_iMarkNum);
				int iCamPlatformYCoordType = m_pCamPlatformInfo->GetCamDirectY(nIndex%m_iMarkNum);


				// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
				int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
				if (eCamPlatformSepFix == iCameraFixType)
				{
					dCameraMovementX = 0;
					dCameraMovementY = 0;

					str.Format("相机移动量：X=%f,Y=%f",dCameraMovementX,dCameraMovementY);
					AlignToolLogRecord(str,nPlatformIndex);

					dCameraOffsetRealDistanceX = dCameraMovementX ;
					dCameraOffsetRealDistanceY = dCameraMovementY ;

					str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);
				}
				else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
				{
					dCameraMovementX =pPlatformXYDAxisPos->m_dPosX - pPlatformBenchAxisPos->m_dPosX;
					dCameraMovementY = 0;

					str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
					AlignToolLogRecord(str,nPlatformIndex);

					if (eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -1*dCameraMovementX;

					dCameraOffsetRealDistanceX = dCameraMovementX;
					dCameraOffsetRealDistanceY = dCameraMovementY;

					str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

				}
				else if (eCamPlatformSepXY == iCameraFixType)
				{
					double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
					double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
					double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
					double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
					if(eDirectNone != iCamPlatformXCoordType)
					{
						dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
					}
					if(eDirectNone != iCamPlatformYCoordType)
					{
						dCameraMovementY = pCamPlatformXYAxisPos->m_dPosY - pCamPlatformBenchAxisPos->m_dPosY ; 
					}
					str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
					AlignToolLogRecord(str,nPlatformIndex);


					//考虑相机平台的方向
					if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
					if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;


					dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11 + dCameraMovementY*dCamTranferA12;
					dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA21 + dCameraMovementY*dCamTranferA22;

					str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);

				}
				else if (eCamPlatformSepX == iCameraFixType)
				{
					double dCamTranferA11 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,0);
					double dCamTranferA12 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(0,1);
					double dCamTranferA21 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,0);
					double dCamTranferA22 = m_vAlignerCalibInfos.at(nIndex)->GetCamPlatformTransferMatrix().GetElement(1,1);
					if(eDirectNone != iCamPlatformXCoordType)
					{
						dCameraMovementX = pCamPlatformXYAxisPos->m_dPosX - pCamPlatformBenchAxisPos->m_dPosX ; 
					}
					dCameraMovementY = 0;

					str.Format("相机移动量： X = %f , Y = %f ",dCameraMovementX,dCameraMovementY);
					AlignToolLogRecord(str,nPlatformIndex);

					//考虑相机平台的方向
					if(eDirectNegative == iCamPlatformXCoordType) dCameraMovementX = -dCameraMovementX;
					if(eDirectNegative == iCamPlatformYCoordType) dCameraMovementY = -dCameraMovementY;

					dCameraOffsetRealDistanceX = dCameraMovementX*dCamTranferA11;
					dCameraOffsetRealDistanceY = dCameraMovementX*dCamTranferA12;

					str.Format("相机移动带来平台移动值： X = %f , Y = %f ",dCameraOffsetRealDistanceX,dCameraOffsetRealDistanceY);
					AlignToolLogRecord(str,nPlatformIndex);
				}
			}

			// 相机轴所在轴运动量对实时Mark在平台坐标系中的位置的影响
			if ( NULL != pCamAxisPlatformBenchAxisPos && NULL != pCamAxisPlatformXYAxisPos)
			{
				str.Format("相机平台所在轴基准坐标： X = %f , Y = %f ",pCamAxisPlatformBenchAxisPos->m_dPosX,pCamAxisPlatformBenchAxisPos->m_dPosY);
				AlignToolLogRecord(str,nPlatformIndex);
				str.Format("当前相机平台所在轴坐标： X = %f , Y = %f ",pCamAxisPlatformXYAxisPos->m_dPosX,pCamAxisPlatformXYAxisPos->m_dPosY);
				AlignToolLogRecord(str,nPlatformIndex);

				int iCamAxisPlatformAxisXCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectX(nIndex%m_iMarkNum);
				int iCamAxisPlatformAxisYCoordType = m_pCamPlatformInfo->GetCamPlatformAxisDirectY(nIndex%m_iMarkNum);

				// 0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
				int iCameraFixType = m_pCamPlatformInfo->GetCamPlatformType();
				if (eCamPlatformSepFix == iCameraFixType)
				{
					dCamAxisPlatformAxisMovementX = 0;
					dCamAxisPlatformAxisMovementY = 0;

					str.Format("相机平台所在轴移动量：X=%f,Y=%f",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
					AlignToolLogRecord(str,nPlatformIndex);
				}
				else if (eCamPlatformShareX == iCameraFixType)//平台X轴安装，移动量为平台X轴的移动量
				{
					dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
					dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 

					str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
					AlignToolLogRecord(str,nPlatformIndex);

					if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
					dCamAxisPlatformAxisMovementY = 0;
				}
				else if (eCamPlatformSepXY == iCameraFixType)
				{
					if(eDirectNone != iCamAxisPlatformAxisXCoordType)
					{
						dCamAxisPlatformAxisMovementX = pCamAxisPlatformXYAxisPos->m_dPosX - pCamAxisPlatformBenchAxisPos->m_dPosX ; 
					}
					if(eDirectNone != iCamAxisPlatformAxisYCoordType)
					{
						dCamAxisPlatformAxisMovementY = pCamAxisPlatformXYAxisPos->m_dPosY - pCamAxisPlatformBenchAxisPos->m_dPosY ; 
					}
					str.Format("相机平台所在轴移动量： X = %f , Y = %f ",dCamAxisPlatformAxisMovementX,dCamAxisPlatformAxisMovementY);
					AlignToolLogRecord(str,nPlatformIndex);

					if(eDirectNegative == iCamAxisPlatformAxisXCoordType) dCamAxisPlatformAxisMovementX = -dCamAxisPlatformAxisMovementX;
					if(eDirectNegative == iCamAxisPlatformAxisYCoordType) dCamAxisPlatformAxisMovementY = -dCamAxisPlatformAxisMovementY;
				}
				else if (eCamPlatformSepX == iCameraFixType)
				{
				}
			}

			// $4、三部分内容相加，即为相机实时Mark的平台坐标值
			platformCoordPos.m_dPosX += dCameraOffsetRealDistanceX + dCamAxisPlatformAxisMovementX;
			platformCoordPos.m_dPosY += dCameraOffsetRealDistanceY + dCamAxisPlatformAxisMovementY;
		}

		str.Format("平台坐标值： X = %f , Y = %f , D = %f ",platformCoordPos.m_dPosX,platformCoordPos.m_dPosY,platformCoordPos.m_dAngle);
		AlignToolLogRecord(str,nPlatformIndex);
	}

	return true;
}


void vcXYDAlignTool::SetBendAlignProcess(BOOL bBendAlignProcess)
{
	m_bBendAlignProcess = bBendAlignProcess;
}