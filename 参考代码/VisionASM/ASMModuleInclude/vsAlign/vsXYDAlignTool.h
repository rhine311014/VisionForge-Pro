// vsXYDAlignTool.h: interface for the vcXYDAlignTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVXYDALIGNERTOOL_H__D24C9C68_D5CE_4E68_B06F_39AA3E7CEBC0__INCLUDED_)
#define AFX_SVXYDALIGNERTOOL_H__D24C9C68_D5CE_4E68_B06F_39AA3E7CEBC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "svMatrix.h"
#include "svFind.h"
#include "svGuiLine.h"
#include "svMeasure.h"
#include "vsBaseAlignTool.h"

// XYD对位工具类
class VS_ALIGN_API vcXYDAlignTool : public vcBaseAlignTool  
{
public:
	vcXYDAlignTool();
	virtual ~vcXYDAlignTool();

	/********************************标定*************************************/

public:
//	CPlatformInfo* m_pPlatformXYDInfo;



public:


	// 1. 基准目标
	virtual CBenchTargetMarkFixInfo* ExacuteBenchMarkFix( std::vector<CCoordPos> imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos, std::vector<CPlatformAxisPos*> vCamPlatformAxisPos);

	virtual CBenchTargetMarkFixInfo* ExacuteBenchMarkFix( std::vector<CCoordPos> imageCoordPoses, 
		std::vector<CPlatformAxisPos*> vPlatformAxisPoses) ;


	// 2. 拾取
	virtual int GetCalibCenterPlatformMovePosNum(int nMarkType);									// XY+D拾取
	virtual int GetCalibPlatformPickMovePosNum();													// XY+D拾取 - XY
	virtual int GetCalibCenterPlatformPickMovePosNum();												// XY+D拾取 - D

	virtual std::vector<CPlatformAxisPos*> GetCalibPlatformPickMovePos(int nPosIndex);				// XY+D拾取 - XY
	virtual std::vector<CPlatformAxisPos*> GetCalibCenterPlatformPickMovePos(int nPosIndex);		// XY+D拾取 - D

	virtual BOOL ExecuteCalibratePickSingleCamEX(int nCamIndex);			// XY+D平台拾取，XY标定，用9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateCenterPickSingleCamEX(int nCamIndex);		// XY+D平台拾取，D标定， 用9点H矩阵方式进行标定

	virtual BOOL ExecuteCalibrateXYSingleCamPickEX(int nCamIndex);			// XY平台拾取,用9点H矩阵方式进行标定
	virtual BOOL ExecuteCalibrateXYSingleCamPick(int nCamIndex);			// XY平台拾取,用5点H矩阵方式进行标定

	virtual BOOL ExecuteCalibrateSingleCamXYWholeDmCodeBoardBoardToPlatPick(int nCalibPos);	//全局二维码标定时拾取XY平台靶标到平台坐标映射
	virtual BOOL ExecuteCalibrateSingleCamWholeDmCodeBoardBoardToPlatPick(int nCalibPos);	//全局二维码标定时拾取相机shareXY时靶标到平台坐标映射


	// 拾取基准对象与旋转中心的偏差信息 nIndex 代表0、90、180、270四个不同的角度
	virtual CBenchTargetMarkPickInfo* ExacuteBenchMarkPick( std::vector<CCoordPos> imageCoordPoses, 
		std::vector<CPlatformAxisPos*> vPlatformAxisPoses, CPlatformAxisPos* pPickPlatformAxisPoses,int nIndex,CBenchTargetMarkPickInfo* OriginFixInfo,int nExProductIndex=0) ;



	// 根据对象的拾取平台坐标，计算吸头的拾取平台坐标
	virtual bool CalcuObjectPlatformPosToPickPlatformPos(std::vector<CCoordPos> vObjectPlatformPos, CCoordPos& cpPickPlatformPos,int nIndex);

	// 根据吸头的平台轴位置，计算吸头的拾取平台坐标
	virtual bool GetPickPlatformPos(CPlatformAxisPos* pPlatformAxisPos, CCoordPos& cpPickPlatformPos);

	// 获取对象Mark的拾取全局平台坐标值
	virtual CMarkPlatformPos GetObjectMarkPickPlatformPos();

	virtual bool ConvertImagePosToCurPickPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos);

	virtual BOOL ExecuteAlignPick(int nAlnIndex/* = 0*/);

	// 折弯
	

	// 不支持辅助点模式
	CMarkPlatformPos GetBendInsTargetPlatformPos();
	CMarkPlatformPos GetBendInsObjectPlatformPos();

	BOOL ConvertBendDmCodeImagePosToPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, CPlatformAxisPos* pPlatformAxisPos);
	BOOL ConvertBendDmCodeImagePosToPlatformPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos,CPlatformAxisPos* pCamAxisPlatformAxisPos);
 
	BOOL ConvertBendDmCodeImagePosToPlatformPosCommon(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos = NULL,CPlatformAxisPos* pCamAxisPlatformAxisPos = NULL);

	BOOL ConvertBendDmCodeImagePosToPlatformPosMainPos(CCoordPos& platformCoordPos, int nIndex, CCoordPos imageCoordPos, 
		CPlatformAxisPos* pPlatformAxisPos, CPlatformAxisPos* pCamPlatformAxisPos = NULL,CPlatformAxisPos* pCamAxisPlatformAxisPos = NULL);

	virtual void SetBendAlignProcess(BOOL bBendAlignProcess);
	virtual BOOL ExeBendInsInfoCalcu(std::vector<sc2Vector>& vTargetPlaftormPos, std::vector<sc2Vector>& vObjectPlatformPos, std::vector<double>& vdDistance);

};

#endif // !defined(AFX_SVXYDALIGNERTOOL_H__D24C9C68_D5CE_4E68_B06F_39AA3E7CEBC0__INCLUDED_)
