
// PatternCornerSearchTool.cpp: implementation of the CPatternCornerSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpecialPickSearchTool.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"
#include "svIntersect.h"
#include "svHistogram.h"
void CSpecialPickSearchTool::Initialize_Member_Variables()
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;


	// 粗定位GUI 初始化
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_GuiTrainAffineRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainAffineRect.SetLabelVisible(TRUE);

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiModelPoint2.SetCenterXYWidthHeight(450, 240, 40, 40);
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	//精定位GUI 初始化

	m_GuiTrainBlobAffineRect.SetCenterLengthsRotationSkew(200, 200, 50, 50, 0, 0);
	m_GuiTrainBlobAffineRect.SetLabelVisible(TRUE);

	for (int it =0;it<RECT_NUMS;it++)
	{
		m_GuiTrainRect_Accurate[it].SetCenterWidthHeight(500 + it*100,600,100,100);
		m_GuiTrainRect_Accurate[it].SetLabelVisible(TRUE);

		m_GuiTrainAffineRect_Accurate[it].SetCenterLengthsRotationSkew(500 + it*100,600,100,100,0,0);
		m_GuiTrainAffineRect_Accurate[it].SetLabelVisible(TRUE);


		m_GuiSearchRect_Accurate[it].SetCenterWidthHeight(400 + it*100,300,500,400);
		m_GuiSearchRect_Accurate[it].SetLabelVisible(TRUE);

	}


	// 头尾检查GUI 矩形
	m_GuiTrainRect_HeadTail.SetCenterWidthHeight(100, 100, 50, 50);
	m_GuiTrainRect_HeadTail.SetLabelVisible(TRUE);
	m_GuiTrainAffineRect_HeadTail.SetCenterLengthsRotationSkew(100, 100, 50, 50, 0, 0);
	m_GuiTrainAffineRect_HeadTail.SetLabelVisible(TRUE);
	m_GuiSearchRect_HeadTail.SetCenterWidthHeight(100, 100, 100, 100);
	m_GuiSearchRect_HeadTail.SetLabelVisible(TRUE);
	m_GuiModelPoint_HeadTail.SetCenterXYWidthHeight(100, 100, 20, 20);
	m_GuiModelPoint_HeadTail.SetLabelVisible(TRUE);

	// 正反检查GUI 矩形
	m_GuiTrainRect_PN.SetCenterWidthHeight(100, 100, 100, 100);
	m_GuiTrainRect_PN.SetLabelVisible(TRUE);
	m_GuiTrainAffineRect_PN.SetCenterLengthsRotationSkew(1000, 1000, 100, 100, 0, 0);
	m_GuiTrainAffineRect_PN.SetLabelVisible(TRUE);
	m_GuiSearchRect_PN.SetCenterWidthHeight(90, 90, 300, 300);
	m_GuiSearchRect_PN.SetLabelVisible(TRUE);
	m_GuiModelPoint_PN.SetCenterXYWidthHeight(100, 100, 20, 20);
	m_GuiModelPoint_PN.SetLabelVisible(TRUE);

	// 混料检查GUI 矩形
	m_GuiTrainRect_Mixing.SetCenterWidthHeight(120, 120, 100, 100);
	m_GuiTrainRect_Mixing.SetLabelVisible(TRUE);
	m_GuiTrainAffineRect_Mixing.SetCenterLengthsRotationSkew(1200, 1200, 100, 100, 0, 0);
	m_GuiTrainAffineRect_Mixing.SetLabelVisible(TRUE);
	m_GuiSearchRect_Mixing.SetCenterWidthHeight(100, 100, 300, 300);
	m_GuiSearchRect_Mixing.SetLabelVisible(TRUE);
	m_GuiModelPoint_PN.SetCenterXYWidthHeight(100, 100, 20, 20);
	m_GuiModelPoint_PN.SetLabelVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Rough Train Area"));
		m_GuiSearchRect.SetLabel(_T("Rough Search Area"));
		m_GuiModelPoint.SetLabel(_T("Rough Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		m_GuiTrainAffineRect.SetLabel(_T("Rough Train Area"));


		m_GuiTrainRect_HeadTail.SetLabel(_T("HeadTail Train Area"));
		m_GuiTrainAffineRect_HeadTail.SetLabel(_T("HeadTail Train Area"));
		m_GuiModelPoint_HeadTail.SetLabel(_T("HeadTail Reference Point"));
		m_GuiSearchRect_HeadTail.SetLabel(_T("HeadTail Search Area"));

		m_GuiTrainRect_PN.SetLabel(_T("PN Train Area"));
		m_GuiTrainAffineRect_PN.SetLabel(_T("PN Train Area"));
		m_GuiModelPoint_PN.SetLabel(_T("PN Reference Point"));
		m_GuiSearchRect_PN.SetLabel(_T("PN Search Area"));

		m_GuiTrainRect_Mixing.SetLabel(_T("Mixing Train Area"));
		m_GuiTrainAffineRect_Mixing.SetLabel(_T("Mixing Train Area"));
		m_GuiModelPoint_Mixing.SetLabel(_T("Mixing Reference Point"));
		m_GuiSearchRect_Mixing.SetLabel(_T("Mixing Search Area"));

		m_GuiTrainBlobAffineRect.SetLabel("Blob Tool");

		for (int it =0;it<RECT_NUMS;it++)
		{
			CString strTmp = _T("");
			strTmp.Format(_T("Accurate Train Area%d"),it);
			m_GuiTrainRect_Accurate[it].SetLabel(strTmp);
			m_GuiTrainAffineRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("Accurate Search Area%d"),it);
			m_GuiSearchRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("Accurate Reference Point%d"),it);
			m_GuiModelPoint_Accurate[it].SetLabel(strTmp);
		}
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("粗定位训练区"));
		m_GuiSearchRect.SetLabel(_T("粗定位搜索区"));
		m_GuiModelPoint.SetLabel(_T("粗定位参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiTrainAffineRect.SetLabel(_T("粗定位训练区"));


		m_GuiTrainRect_HeadTail.SetLabel(_T("头尾检查训练区"));
		m_GuiTrainAffineRect_HeadTail.SetLabel(_T("头尾检查训练区"));
		m_GuiModelPoint_HeadTail.SetLabel(_T("头尾检查参考点"));
		m_GuiSearchRect_HeadTail.SetLabel(_T("头尾搜索搜索区"));

		m_GuiTrainRect_PN.SetLabel(_T("正反检查训练区"));
		m_GuiTrainAffineRect_PN.SetLabel(_T("正反检查训练区"));
		m_GuiModelPoint_PN.SetLabel(_T("正反检查参考点"));
		m_GuiSearchRect_PN.SetLabel(_T("正反检查搜索区"));

		m_GuiTrainRect_Mixing.SetLabel(_T("混料检查训练区"));
		m_GuiTrainAffineRect_Mixing.SetLabel(_T("混料检查训练区"));
		m_GuiModelPoint_Mixing.SetLabel(_T("混料检查参考点"));
		m_GuiSearchRect_Mixing.SetLabel(_T("混料检查搜索区"));
		m_GuiTrainBlobAffineRect.SetLabel("Blob工具");
		for (int it =0;it<RECT_NUMS;it++)
		{
			CString strTmp = _T("");
			strTmp.Format(_T("精定位训练区%d"),it);
			m_GuiTrainRect_Accurate[it].SetLabel(strTmp);
			m_GuiTrainAffineRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("精定位搜索区%d"),it);
			m_GuiSearchRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("精定位参考点%d"),it);
			m_GuiModelPoint_Accurate[it].SetLabel(strTmp);
		}

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点1"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域"));

		m_GuiTrainRect_HeadTail.SetLabel(_T("头尾检查训练区"));
		m_GuiTrainAffineRect_HeadTail.SetLabel(_T("头尾检查训练区"));
		m_GuiModelPoint_HeadTail.SetLabel(_T("头尾检查参考点"));
		m_GuiSearchRect_HeadTail.SetLabel(_T("头尾搜索搜索区"));

		m_GuiTrainRect_PN.SetLabel(_T("正反检查训练区"));
		m_GuiTrainAffineRect_PN.SetLabel(_T("正反检查训练区"));
		m_GuiModelPoint_PN.SetLabel(_T("正反检查参考点"));
		m_GuiSearchRect_PN.SetLabel(_T("正反检查搜索区"));

		m_GuiTrainRect_Mixing.SetLabel(_T("混料检查训练区"));
		m_GuiTrainAffineRect_Mixing.SetLabel(_T("混料检查训练区"));
		m_GuiModelPoint_Mixing.SetLabel(_T("混料检查参考点"));
		m_GuiSearchRect_Mixing.SetLabel(_T("混料检查搜索区"));
		m_GuiTrainBlobAffineRect.SetLabel("Blob工具");
		for (int it =0;it<RECT_NUMS;it++)
		{
			CString strTmp = _T("");
			strTmp.Format(_T("精定位训练区%d"),it);
			m_GuiTrainRect_Accurate[it].SetLabel(strTmp);
			m_GuiTrainAffineRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("精定位搜索区%d"),it);
			m_GuiSearchRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("精定位参考点%d"),it);
			m_GuiModelPoint_Accurate[it].SetLabel(strTmp);
		}

		break;
	}

	int Sx=100,Sy=100,Ex=200,Ey=200;
	m_GuiFindLine[0].SetStartXYEndXY(Sx, Sy, Ex, Sy);
	m_GuiFindLine[0].SetCaliperParams(20,scRadian(skPI/2.0), 50, 5, TRUE);
	m_GuiFindLine[1].SetStartXYEndXY(Sx, Ey, Ex, Ey);
	m_GuiFindLine[1].SetCaliperParams(20,scRadian(skPI/2.0), 50, 5, TRUE);

	m_GuiFindLine[2].SetStartXYEndXY(Sx, Sy, Sx, Ey);
	m_GuiFindLine[2].SetCaliperParams(20,scRadian(skPI/2.0), 50, 5, TRUE);
	m_GuiFindLine[3].SetStartXYEndXY(Ex, Ey, Ex, Sy);
	m_GuiFindLine[3].SetCaliperParams(20,scRadian(skPI/2.0), 50, 5, TRUE);

	//m_GuiFindLine[0].SetStartXYEndXY(270, 130, 420, 130);
	//m_GuiFindLine[0].SetCaliperParams(20,scRadian(skPI/2.0), 50, 5, TRUE);
	//m_GuiFindLine[1].SetStartXYEndXY(270, 130, 270, 420);
	//m_GuiFindLine[1].SetCaliperParams(20,scRadian(skPI/2.0), 50, 5, TRUE);

	//m_GuiFindLine[2].SetStartXYEndXY(270, 420, 420, 420);
	//m_GuiFindLine[2].SetCaliperParams(20,scRadian(skPI/2.0), 50, 5, TRUE);
	//m_GuiFindLine[3].SetStartXYEndXY(420, 420, 420, 130);
	//m_GuiFindLine[3].SetCaliperParams(20,scRadian(skPI/2.0), 50, 5, TRUE);
	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[2].SetCaliperVisible(FALSE);
	m_GuiFindLine[2].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[3].SetCaliperVisible(FALSE);
	m_GuiFindLine[3].SetAffineRectFindLineVisible(TRUE);
	int i=0;
	for( i =0;i<LINE_NUMS;i++)
	{
		if (m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bEnableCornerLineMask)
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
		}
		else
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
		}
	}


	CString strDirZh[4] = {"上","下","左","右"};
	CString strDirEn[4] = {"Up","Down","Left","Right"};

	for (i=0; i<LINE_NUMS&&i<4; i++)
	{

		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d_%s"), i+1, strDirEn[i]);
			break;
		case 1:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		default:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		}

		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);

	}
	for (int it=0;it<CIRCLE_NUMS;it++)
	{
		m_GuiFindCircle[it].SetCenterRadiusAngleStartAngleSpan(350+i*100, 240+i*100, 100, 0, sk2PI);
		m_GuiFindCircle[it].SetCaliperParams(20, eInward, 50, 20, true);
		m_GuiFindCircle[it].SetLabelVisible(TRUE);
	}
	for (i=0; i<CIRCLE_NUMS; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindCircle%d"), i+1);
			break;
		case 1:
			str.Format(_T("找圆工具%d"), i+1);
			break;
		default:
			str.Format(_T("找圆工具%d"), i+1);
			break;
		}

		m_GuiFindCircle[i].SetLabel(str);
		m_GuiFindCircle[i].SetLabelVisible(TRUE);
	}

	m_GuiFindCircle[0].SetCaliperVisible(false);
	m_GuiFindCircle[0].SetAnnulusFindCircleVisible(TRUE);
	m_GuiFindCircle[1].SetCaliperVisible(false);
	m_GuiFindCircle[1].SetAnnulusFindCircleVisible(TRUE);

	CreateGUID(m_strKeyText);
}

CSpecialPickSearchTool::CSpecialPickSearchTool(void)
{

	Initialize_Member_Variables();

	//训练参数与GUI一致

	// 粗定位训练参数
	m_tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_Accuracy = m_PatternSearchTool_Rough.GetAccuracy();
	m_tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_CoarseScale			= m_PatternSearchTool_Rough.GetCoarsestScale();
	m_tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_dAngleLow			= -2.0;
	m_tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_dAngleHigh			= 2.0;
	m_tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData.m_dGrau	= m_QuickSearchTool_Rough.GetGranularity();
	m_tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData.m_lNoiseThre = m_QuickSearchTool_Rough.GetNoiseThre();
	m_tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData.m_lTotalMag = m_QuickSearchTool_Rough.GetTotalGradMagThre();


	// 头尾训练参数
	m_tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_Accuracy = m_PatternSearchTool_HeadTail.GetAccuracy();
	m_tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_CoarseScale			= m_PatternSearchTool_HeadTail.GetCoarsestScale();
	m_tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_dAngleLow			= -2.0;
	m_tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_dAngleHigh			= 2.0;
	m_tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_dGrau	= m_QuickSearchTool_HeadTail.GetGranularity();
	m_tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_lNoiseThre = m_QuickSearchTool_HeadTail.GetNoiseThre();
	m_tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_lTotalMag = m_QuickSearchTool_HeadTail.GetTotalGradMagThre();

	// 正反训练参数
	m_tmpTrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.m_Accuracy = m_PatternSearchTool_PN.GetAccuracy();
	m_tmpTrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.m_CoarseScale			= m_PatternSearchTool_PN.GetCoarsestScale();
	m_tmpTrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.m_dAngleLow			= -2.0;
	m_tmpTrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.m_dAngleHigh			= 2.0;
	m_tmpTrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.m_dGrau	= m_QuickSearchTool_PN.GetGranularity();
	m_tmpTrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.m_lNoiseThre = m_QuickSearchTool_PN.GetNoiseThre();
	m_tmpTrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.m_lTotalMag = m_QuickSearchTool_PN.GetTotalGradMagThre();


	// 混料训练参数
	m_tmpTrainDataParam.m_Struct_Mixing_PatternSearchTrainData.m_Accuracy = m_PatternSearchTool_Mixing.GetAccuracy();
	m_tmpTrainDataParam.m_Struct_Mixing_PatternSearchTrainData.m_CoarseScale			= m_PatternSearchTool_Mixing.GetCoarsestScale();
	m_tmpTrainDataParam.m_Struct_Mixing_PatternSearchTrainData.m_dAngleLow			= -2.0;
	m_tmpTrainDataParam.m_Struct_Mixing_PatternSearchTrainData.m_dAngleHigh			= 2.0;
	m_tmpTrainDataParam.m_Struct_Mixing_PatternSearchTrainData.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_Struct_Mixing_QuickSearchTrainData.m_dGrau	= m_QuickSearchTool_Mixing.GetGranularity();
	m_tmpTrainDataParam.m_Struct_Mixing_QuickSearchTrainData.m_lNoiseThre = m_QuickSearchTool_Mixing.GetNoiseThre();
	m_tmpTrainDataParam.m_Struct_Mixing_QuickSearchTrainData.m_lTotalMag = m_QuickSearchTool_Mixing.GetTotalGradMagThre();


	// 精定位训练参数

	for (int it = 0;it<RECT_NUMS;it++)
	{
		m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[it].m_Accuracy = m_PatternSearchTool_Accurate[it].GetAccuracy();
		m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[it].m_CoarseScale = m_PatternSearchTool_Accurate[it].GetCoarsestScale();
		m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[it].m_dAngleLow			= -2.0;
		m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[it].m_dAngleHigh			= 2.0;
		m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[it].m_bMaskEnable			= false; // 默认不采用掩模图像

		m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[it].m_dGrau	= m_QuickSearchTool_Accurate[it].GetGranularity();
		m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[it].m_lNoiseThre = m_QuickSearchTool_Accurate[it].GetNoiseThre();
		m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[it].m_lTotalMag = m_QuickSearchTool_Accurate[it].GetTotalGradMagThre();
	}

	m_tmpTrainGuiParam.m_BlobAffineRect			= m_GuiTrainBlobAffineRect.GetAffineRect();


	//训练区GUI参数
	// 粗定位
	m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainRect = m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint	= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint1	= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint2	= m_GuiModelPoint2.GetCenter();

	m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_SearchRect	= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_nLineWidth	= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_LineColor = m_GuiTrainRect.GetLineColor();
	m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainAffineRect = m_GuiTrainAffineRect.GetAffineRect();




	// 头尾检查
	m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_TrainRect = m_GuiTrainRect_HeadTail.GetRect();
	m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_ModelPoint	= m_GuiModelPoint_HeadTail.GetCenter();
	m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect	= m_GuiSearchRect_HeadTail.GetRect();
	m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_nLineWidth	= m_GuiTrainRect_HeadTail.GetLineWidth();
	m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_LineColor = m_GuiTrainRect_HeadTail.GetLineColor();
	m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_TrainAffineRect = m_GuiTrainAffineRect_HeadTail.GetAffineRect();

	//正反检查
	m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_TrainRect = m_GuiTrainRect_PN.GetRect();
	m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_ModelPoint	= m_GuiModelPoint_PN.GetCenter();
	m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_SearchRect	= m_GuiSearchRect_PN.GetRect();
	m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_nLineWidth	= m_GuiTrainRect_PN.GetLineWidth();
	m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_LineColor = m_GuiTrainRect_PN.GetLineColor();
	m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_TrainAffineRect = m_GuiTrainAffineRect_PN.GetAffineRect();

	// 混料检查
	m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_TrainRect = m_GuiTrainRect_Mixing.GetRect();
	m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_ModelPoint	= m_GuiModelPoint_Mixing.GetCenter();
	m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_SearchRect	= m_GuiSearchRect_Mixing.GetRect();
	m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_nLineWidth	= m_GuiTrainRect_Mixing.GetLineWidth();
	m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_LineColor = m_GuiTrainRect_Mixing.GetLineColor();
	m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_TrainAffineRect = m_GuiTrainAffineRect_Mixing.GetAffineRect();


	for (int it=0;it<RECT_NUMS;it++)
	{
		m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_TrainRect = m_GuiTrainRect_Accurate[it].GetRect();
		m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_ModelPoint = m_GuiModelPoint_Accurate[it].GetCenter();
		m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_SearchRect	= m_GuiSearchRect_Accurate[it].GetRect();
		m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_nLineWidth	= m_GuiTrainRect_Accurate[it].GetLineWidth();
		m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_LineColor = m_GuiTrainRect_Accurate[it].GetLineColor();

		m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_TrainAffineRect = m_GuiTrainAffineRect_Accurate[it].GetAffineRect();

	}

	int i = 0;
	for (i=0; i<LINE_NUMS; i++)
	{
		m_FindLineTool_Accurate[i].GetCaliperProperty(m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[i].m_dContrastThre, m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[i].m_edgeProperty, 
			m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[i].m_lFilterHalfWidth);
		m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[i].m_nOutLineNum		= 20;
		m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[i].m_bEnableDualEdge		= false;

		m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_nCaliperNum			= m_GuiFindLine[i].GetCaliperNum();
		m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dStartX, m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dStartY, m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dEndX,
			m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dEndY);
		m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dCaliperProjectLen		= m_GuiFindLine[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dCaliperSearchLen		= m_GuiFindLine[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_CaliperSearchRotation	= m_GuiFindLine[i].GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bCaliperVisible			= m_GuiFindLine[i].IsCaliperVisible();
	}

	for (i=0; i<CIRCLE_NUMS; i++)
	{
		m_FindCicleTool_Accurate[i].GetCaliperProperty(m_tmpTrainDataParam.m_FindCircleCaliperTrainDataParam[i].m_dContrastThre, m_tmpTrainDataParam.m_FindCircleCaliperTrainDataParam[i].m_edgeProperty, 
			m_tmpTrainDataParam.m_FindCircleCaliperTrainDataParam[i].m_lFilterHalfWidth);
		m_tmpTrainDataParam.m_FindCircleCaliperTrainDataParam[i].m_nOutLineNum			= 10;


		m_FindCicleTool_Accurate[i].GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_FindCircleTrainGuiParam[i].m_CircleCenter, m_tmpTrainGuiParam.m_FindCircleTrainGuiParam[i].m_dRadius,
			m_tmpTrainGuiParam.m_FindCircleTrainGuiParam[i].m_ArcStart, m_tmpTrainGuiParam.m_FindCircleTrainGuiParam[i].m_ArcSpan);
		m_tmpTrainGuiParam.m_FindCircleTrainGuiParam[i].m_dCaliperProjectLen		= m_FindCicleTool_Accurate[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_FindCircleTrainGuiParam[i].m_dCaliperSearchLen		= m_FindCicleTool_Accurate[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_FindCircleTrainGuiParam[i].m_CaliperSearchDirection = (CaliperSearchDirection)m_FindCicleTool_Accurate[i].GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_FindCircleTrainGuiParam[i].m_bCircleCaliperVisible  = m_GuiFindCircle[i].IsCaliperVisible();

	}

	m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dAcceptThre			= INI_ACCEPTTHRE;
	m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dConfusionThre		= INI_CONTRASTTHRE;

	m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleLow			= -INI_SEARCH_ANGLE;
	m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleHigh			= INI_SEARCH_ANGLE;

	m_tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dAcceptThre			= INI_ACCEPTTHRE;
	m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dConfusionThre		= INI_CONTRASTTHRE;

	m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleLow			= -INI_SEARCH_ANGLE;
	m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleHigh			= INI_SEARCH_ANGLE;

	m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dAcceptThre			= INI_ACCEPTTHRE;
	m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dConfusionThre		= INI_CONTRASTTHRE;

	m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleLow			= -INI_SEARCH_ANGLE;
	m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleHigh			= INI_SEARCH_ANGLE;

	m_tmpSearchGuiParam.m_nLineWidth	= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor	= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;



}


bool CSpecialPickSearchTool::SetPatternSearchToolParam(scPatternSearchRot *m_PatternSearchTool_Rough,CPatternSearchTrainDataParam Rough_PatternSearchTrainData)
{
	// 设置训练参数
	if (!m_PatternSearchTool_Rough->SetAccuracy(Rough_PatternSearchTrainData.m_Accuracy))
	{
		return false;
	}

	if (!m_PatternSearchTool_Rough->SetCoarsestScale(Rough_PatternSearchTrainData.m_CoarseScale))
	{
		return false;
	}

	// 必须在训练时设置角度
	if (fabs(Rough_PatternSearchTrainData.m_dAngleLow - Rough_PatternSearchTrainData.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool_Rough->SetDofNominalRotate(scDegree(Rough_PatternSearchTrainData.m_dAngleLow)))
		{
			return false;
		}
	}
	else if (!m_PatternSearchTool_Rough->SetDofZoneRotate(scDegree(Rough_PatternSearchTrainData.m_dAngleLow), scDegree(Rough_PatternSearchTrainData.m_dAngleHigh), scDegree(360)))
	{
		return false;
	}
	return true;
}

void CSpecialPickSearchTool::SetPatternSearchToolImageMask(scPatternSearchRot *m_PatternSearchTool_Rough,CPatternSearchTrainDataParam Rough_PatternSearchTrainData,CRoughSearchTrainGuiParam Rough_PatternSearchTrainGui,cpImage &m_ImageTrain, cpImage &m_ImageMask)
{
	// 设置掩模图像
	bool bRet=false;
	// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
	BOOL bIsProcessImage = FALSE;
	if (1 != m_ImageTrain.PixelSize())
	{
		bIsProcessImage = TRUE;
	}

	cpImage tmpImage;
	cpImage tmpImageMask;
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_ImageTrain,tmpImage);

		tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);
	}		
	if (Rough_PatternSearchTrainData.m_bMaskEnable)
	{
		if(bIsProcessImage)
		{
			bRet = m_PatternSearchTool_Rough->Train(tmpImage, tmpImageMask,Rough_PatternSearchTrainGui.m_ModelPoint,
				&Rough_PatternSearchTrainGui.m_TrainRect);
		}
		else
		{
			bRet = m_PatternSearchTool_Rough->Train(m_ImageTrain, m_ImageMask,Rough_PatternSearchTrainGui.m_ModelPoint,
				&Rough_PatternSearchTrainGui.m_TrainRect);
		}
		
		m_bTrainOK = bRet;
	}
	else
	{
		if(bIsProcessImage)
		{
			bRet = m_PatternSearchTool_Rough->Train(tmpImage,Rough_PatternSearchTrainGui.m_ModelPoint,
				&Rough_PatternSearchTrainGui.m_TrainRect);
		}
		else
		{
			bRet = m_PatternSearchTool_Rough->Train(m_ImageTrain,Rough_PatternSearchTrainGui.m_ModelPoint,
				&Rough_PatternSearchTrainGui.m_TrainRect);
		}
		m_bTrainOK = bRet;			
	}
}

bool CSpecialPickSearchTool::SetQuickSearchToolParam(scQuickSearchTool *m_QuickSearchTool_Rough,CQuickSearchTrainDataParam Rough_QuickSearchTrainData)
{
	// 设置训练参数
	if (!m_QuickSearchTool_Rough->SetGranularity(Rough_QuickSearchTrainData.m_dGrau))
	{
		return false;
	}

	if (!m_QuickSearchTool_Rough->SetNoiseThre(Rough_QuickSearchTrainData.m_lNoiseThre))
	{
		return false;
	}

	if (!m_QuickSearchTool_Rough->SetTotalGradMagThre(Rough_QuickSearchTrainData.m_lTotalMag))
	{
		return false;
	}

	return true;
}


void CSpecialPickSearchTool::SetQuickSearchToolImageMask(scQuickSearchTool *m_QuickSearchTool_Rough,CQuickSearchTrainDataParam Rough_QuickSearchTrainData,CRoughSearchTrainGuiParam Rough_QuickSearchTrainGui, cpImage &m_ImageTrain, cpImage &m_ImageMask)
{
	// 设置掩模图像
	bool bRet = false;
	/*if (Rough_QuickSearchTrainData.m_bMaskEnable)
	{
		bRet = m_QuickSearchTool_Rough->Train(m_ImageTrain, m_ImageMask, Rough_QuickSearchTrainGui.m_TrainAffineRect, Rough_QuickSearchTrainGui.m_ModelPoint.GetX(),
			Rough_QuickSearchTrainGui.m_ModelPoint.GetY());
		m_bTrainOK = bRet;
	}
	else
	{
		bRet = m_QuickSearchTool_Rough->Train(m_ImageTrain, Rough_QuickSearchTrainGui.m_TrainAffineRect, Rough_QuickSearchTrainGui.m_ModelPoint.GetX(),
			Rough_QuickSearchTrainGui.m_ModelPoint.GetY());
		m_bTrainOK = bRet;
	}*/

	// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
	BOOL bIsProcessImage = FALSE;
	if (1 != m_ImageTrain.PixelSize())
	{
		bIsProcessImage = TRUE;
	}

	cpImage tmpImage;
	cpImage tmpImageMask;
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_ImageTrain,tmpImage);

		tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);
	}		

	// 设置掩模图像
	if (Rough_QuickSearchTrainData.m_bMaskEnable)
	{
		if(bIsProcessImage)
		{
			bRet = m_QuickSearchTool_Rough->Train(tmpImage, tmpImageMask, Rough_QuickSearchTrainGui.m_TrainAffineRect, Rough_QuickSearchTrainGui.m_ModelPoint.GetX(),
				Rough_QuickSearchTrainGui.m_ModelPoint.GetY());
		}
		else
		{
			bRet = m_QuickSearchTool_Rough->Train(m_ImageTrain, m_ImageMask, Rough_QuickSearchTrainGui.m_TrainAffineRect, Rough_QuickSearchTrainGui.m_ModelPoint.GetX(),
				Rough_QuickSearchTrainGui.m_ModelPoint.GetY());
		}
		m_bTrainOK = bRet;
	}
	else
	{
		if(bIsProcessImage)
		{
			bRet = m_QuickSearchTool_Rough->Train(tmpImage, Rough_QuickSearchTrainGui.m_TrainAffineRect, Rough_QuickSearchTrainGui.m_ModelPoint.GetX(),
				Rough_QuickSearchTrainGui.m_ModelPoint.GetY());
		}
		else
		{
			bRet = m_QuickSearchTool_Rough->Train(m_ImageTrain, Rough_QuickSearchTrainGui.m_TrainAffineRect, Rough_QuickSearchTrainGui.m_ModelPoint.GetX(),
				Rough_QuickSearchTrainGui.m_ModelPoint.GetY());
		}
		m_bTrainOK = bRet;
	}
}

void  CSpecialPickSearchTool::SetFindLineSearchToolParam(scFindLine *mFindLineTool,CCaliperTrainDataParam mFindLineTrainDataParam,CFindLineTrainGuiParam mFindLineTrainGuiParam)
{
	if(mFindLineTool == NULL) return;
	// 找线1
	mFindLineTool->SetCaliperNum(mFindLineTrainGuiParam.m_nCaliperNum);
	mFindLineTool->SetCaliperParams(mFindLineTrainGuiParam.m_nCaliperNum, mFindLineTrainGuiParam.m_CaliperSearchRotation, mFindLineTrainGuiParam.m_dCaliperSearchLen
		,mFindLineTrainGuiParam.m_dCaliperProjectLen);
	mFindLineTool->SetCaliperProperty(mFindLineTrainDataParam.m_dContrastThre, mFindLineTrainDataParam.m_edgeProperty, mFindLineTrainDataParam.m_lFilterHalfWidth);
	mFindLineTool->SetOutLineNum(mFindLineTrainDataParam.m_nOutLineNum, eRansacRobust);
	mFindLineTool->SetStartXYEndXY(mFindLineTrainGuiParam.m_dStartX, mFindLineTrainGuiParam.m_dStartY,mFindLineTrainGuiParam.m_dEndX, mFindLineTrainGuiParam.m_dEndY);
	mFindLineTool->SetCaliperMasked(mFindLineTrainGuiParam.m_vbCaliperMask_FindLine);
	if (mFindLineTrainDataParam.m_bEnableDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (mFindLineTrainDataParam.m_edgeProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (mFindLineTrainDataParam.m_edgeProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		mFindLineTool->SetCaliperProperty(mFindLineTrainDataParam.m_dContrastThre, mFindLineTrainDataParam.m_edgeProperty,
			tmpEdgePolarity,mFindLineTrainDataParam.m_lFilterHalfWidth);
	} 
	else
	{
		mFindLineTool->SetCaliperProperty(mFindLineTrainDataParam.m_dContrastThre,
			mFindLineTrainDataParam.m_edgeProperty, mFindLineTrainDataParam.m_lFilterHalfWidth);
	}
}





CSpecialPickSearchTool::CSpecialPickSearchTool(CSpecialPickSearchTool &cSpecialPickSearchTool)
{
	if (cSpecialPickSearchTool.GetSearchToolType() != eSpecialPickSearchTool)
	{
		return;
	}

	Initialize_Member_Variables();

	int i=0;
	for(int i =0;i<LINE_NUMS;i++)
	{
		if (cSpecialPickSearchTool.m_TrainGuiParam.m_FindLineTrainGuiParam[i].m_bEnableCornerLineMask)
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
		}
		else
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
		}
	}

	// 赋值
	m_CamHeight								= cSpecialPickSearchTool.m_CamHeight;
	m_CamWidth								= cSpecialPickSearchTool.m_CamWidth;

	m_TrainDataParam						= cSpecialPickSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cSpecialPickSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cSpecialPickSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cSpecialPickSearchTool.m_SearchGuiParam;
	m_TrainResult							= cSpecialPickSearchTool.m_TrainResult;

	m_ImageTrain							= cSpecialPickSearchTool.m_ImageTrain;
	m_ImageMask								= cSpecialPickSearchTool.m_ImageMask;
	m_ImageMask_Accurate1					= cSpecialPickSearchTool.m_ImageMask_Accurate1;
	m_ImageMask_Accurate2					= cSpecialPickSearchTool.m_ImageMask_Accurate2;
	m_ImageMask_Accurate3					= cSpecialPickSearchTool.m_ImageMask_Accurate3;
	m_ImageMask_Accurate4					= cSpecialPickSearchTool.m_ImageMask_Accurate4;


	m_ImageMask_HeadTail					= cSpecialPickSearchTool.m_ImageMask_HeadTail;
	m_ImageMask_PN							= cSpecialPickSearchTool.m_ImageMask_PN;
	m_ImageMask_Mixing						= cSpecialPickSearchTool.m_ImageMask_Mixing	;



	m_bTrainGuiShow							= cSpecialPickSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cSpecialPickSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cSpecialPickSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cSpecialPickSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cSpecialPickSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cSpecialPickSearchTool.UpdateSearchGuiData(true);
	cSpecialPickSearchTool.UpdateTrainGuiData(true);


	SetTrainDataParam(&cSpecialPickSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cSpecialPickSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cSpecialPickSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cSpecialPickSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cSpecialPickSearchTool.m_tmpImageTrain);
	SetMaskImage(cSpecialPickSearchTool.m_tmpImageMask);
	SetMaskImage1(cSpecialPickSearchTool.m_tmpImageMask_Accurate1);
	SetMaskImage2(cSpecialPickSearchTool.m_tmpImageMask_Accurate2);
	SetMaskImage3(cSpecialPickSearchTool.m_tmpImageMask_Accurate3);
	SetMaskImage4(cSpecialPickSearchTool.m_tmpImageMask_Accurate4);
	SetMaskImage_HeadTail(cSpecialPickSearchTool.m_tmpImageMask_HeadTail);
	SetMaskImage_PN(cSpecialPickSearchTool.m_tmpImageMask_PN);
	SetMaskImage_Mixing(cSpecialPickSearchTool.m_tmpImageMask_Mixing);

	if (m_bTrainOK)
	{
		bool bRet=false;
		m_bTrainOK = false;
		if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_Rough,m_TrainDataParam.m_Struct_Rough_PatternSearchTrainData);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_Rough,m_TrainDataParam.m_Struct_Rough_PatternSearchTrainData,m_TrainGuiParam.m_RoughSearchTrainGuiParam,m_ImageTrain,m_ImageMask);
		}

		if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_Rough,m_TrainDataParam.m_Struct_Rough_QuickSearchTrainData);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_Rough,m_TrainDataParam.m_Struct_Rough_QuickSearchTrainData,m_TrainGuiParam.m_RoughSearchTrainGuiParam,m_ImageTrain,m_ImageMask);
		}
		if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool)
		{
			m_bTrainOK = true;
		}
		// 精定位
		if (m_tmpTrainDataParam.m_bEnableAccurateLocation&& m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[0],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0]);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[0],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[0],m_ImageTrain,m_ImageMask_Accurate1);

			if (m_tmpTrainDataParam.m_bEnableFourPoint)
			{
				SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[1],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[1]);
				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[1],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[1],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[1],m_ImageTrain,m_ImageMask_Accurate2);

				SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[2],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[2]);
				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[2],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[2],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[2],m_ImageTrain,m_ImageMask_Accurate3);

				SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[3],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[3]);
				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[3],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[3],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[3],m_ImageTrain,m_ImageMask_Accurate4);

			}
		}

		if (m_tmpTrainDataParam.m_bEnableAccurateLocation&& m_tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[0],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0]);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[0],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[0],m_ImageTrain,m_ImageMask_Accurate1);

			if (m_tmpTrainDataParam.m_bEnableFourPoint)
			{
				SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[1],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[1]);
				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[1],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[1],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[1],m_ImageTrain,m_ImageMask_Accurate2);

				SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[2],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[2]);
				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[2],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[2],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[2],m_ImageTrain,m_ImageMask_Accurate3);

				SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[3],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[3]);
				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[3],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[3],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[3],m_ImageTrain,m_ImageMask_Accurate4);

			}
		}


		// 头尾
		if (m_tmpTrainDataParam.m_bEnableHeadTail && m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_HeadTail,m_TrainDataParam.m_Struct_HeadTail_PatternSearchTrainData);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_HeadTail,m_TrainDataParam.m_Struct_HeadTail_PatternSearchTrainData,m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_HeadTail);
		}
		if (m_tmpTrainDataParam.m_bEnableHeadTail && m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_HeadTail,m_TrainDataParam.m_Struct_HeadTail_QuickSearchTrainData);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_HeadTail,m_TrainDataParam.m_Struct_HeadTail_QuickSearchTrainData,m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_HeadTail);
		}


		// 正反
		if (m_tmpTrainDataParam.m_bEnablePositiveAndNegative && m_tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_PN,m_TrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_PN,m_TrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData,m_TrainGuiParam.m_PN_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_PN);
		}
		if (m_tmpTrainDataParam.m_bEnablePositiveAndNegative && m_tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_PN,m_TrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_PN,m_TrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData,m_TrainGuiParam.m_PN_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_PN);
		}

		// 混料
		if (m_tmpTrainDataParam.m_bEnableMixing&& m_tmpTrainDataParam.GetMixingCheckToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_Mixing,m_TrainDataParam.m_Struct_Mixing_PatternSearchTrainData);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_Mixing,m_TrainDataParam.m_Struct_Mixing_PatternSearchTrainData,m_TrainGuiParam.m_Mixing_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_Mixing);
		}
		if (m_tmpTrainDataParam.m_bEnableMixing && m_tmpTrainDataParam.GetMixingCheckToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_Mixing,m_TrainDataParam.m_Struct_Mixing_QuickSearchTrainData);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_Mixing,m_TrainDataParam.m_Struct_Mixing_QuickSearchTrainData,m_TrainGuiParam.m_Mixing_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_Mixing);
		}
	}
}





CSpecialPickSearchTool& CSpecialPickSearchTool::operator=(CSpecialPickSearchTool &cSpecialPickSearchTool)
{
	if (this == &cSpecialPickSearchTool)
	{
		return *this;	
	}

	if (cSpecialPickSearchTool.GetSearchToolType() != eSpecialPickSearchTool)
	{
		return *this;
	}

	// 清空
	ClearAllGui();
	ClearResult();

	Initialize_Member_Variables();

	int i=0;
	for(int i =0;i<LINE_NUMS;i++)
	{
		if (cSpecialPickSearchTool.m_TrainGuiParam.m_FindLineTrainGuiParam[i].m_bEnableCornerLineMask)
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
		}
		else
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
		}
	}

	// 赋值
	m_CamHeight								= cSpecialPickSearchTool.m_CamHeight;
	m_CamWidth								= cSpecialPickSearchTool.m_CamWidth;
	m_TrainDataParam						= cSpecialPickSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cSpecialPickSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cSpecialPickSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cSpecialPickSearchTool.m_SearchGuiParam;
	m_TrainResult							= cSpecialPickSearchTool.m_TrainResult;


	m_ImageTrain							= cSpecialPickSearchTool.m_ImageTrain;
	m_ImageMask								= cSpecialPickSearchTool.m_ImageMask;
	m_ImageMask_Accurate1					= cSpecialPickSearchTool.m_ImageMask_Accurate1;
	m_ImageMask_Accurate2					= cSpecialPickSearchTool.m_ImageMask_Accurate2;
	m_ImageMask_Accurate3					= cSpecialPickSearchTool.m_ImageMask_Accurate3;
	m_ImageMask_Accurate4					= cSpecialPickSearchTool.m_ImageMask_Accurate4;


	m_ImageMask_HeadTail					= cSpecialPickSearchTool.m_ImageMask_HeadTail;
	m_ImageMask_PN							= cSpecialPickSearchTool.m_ImageMask_PN;
	m_ImageMask_Mixing						= cSpecialPickSearchTool.m_ImageMask_Mixing	;




	m_bTrainGuiShow							= cSpecialPickSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cSpecialPickSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cSpecialPickSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cSpecialPickSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cSpecialPickSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cSpecialPickSearchTool.UpdateSearchGuiData(true);
	cSpecialPickSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cSpecialPickSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cSpecialPickSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cSpecialPickSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cSpecialPickSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cSpecialPickSearchTool.m_tmpImageTrain);

	SetMaskImage(cSpecialPickSearchTool.m_tmpImageMask);
	SetMaskImage1(cSpecialPickSearchTool.m_tmpImageMask_Accurate1);
	SetMaskImage2(cSpecialPickSearchTool.m_tmpImageMask_Accurate2);
	SetMaskImage3(cSpecialPickSearchTool.m_tmpImageMask_Accurate3);
	SetMaskImage4(cSpecialPickSearchTool.m_tmpImageMask_Accurate4);
	SetMaskImage_HeadTail(cSpecialPickSearchTool.m_tmpImageMask_HeadTail);
	SetMaskImage_PN(cSpecialPickSearchTool.m_tmpImageMask_PN);
	SetMaskImage_Mixing(cSpecialPickSearchTool.m_tmpImageMask_Mixing);

	if (m_bTrainOK)
	{
		bool bRet=false;
		m_bTrainOK = false;
		if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_Rough,m_TrainDataParam.m_Struct_Rough_PatternSearchTrainData);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_Rough,m_TrainDataParam.m_Struct_Rough_PatternSearchTrainData,m_TrainGuiParam.m_RoughSearchTrainGuiParam,m_ImageTrain,m_ImageMask);
		}

		if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_Rough,m_TrainDataParam.m_Struct_Rough_QuickSearchTrainData);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_Rough,m_TrainDataParam.m_Struct_Rough_QuickSearchTrainData,m_TrainGuiParam.m_RoughSearchTrainGuiParam,m_ImageTrain,m_ImageMask);
		}

		// 精定位
		if (m_tmpTrainDataParam.m_bEnableAccurateLocation&& m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[0],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0]);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[0],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[0],m_ImageTrain,m_ImageMask_Accurate1);

			if (m_tmpTrainDataParam.m_bEnableFourPoint)
			{
				SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[1],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[1]);
				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[1],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[1],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[1],m_ImageTrain,m_ImageMask_Accurate2);

				SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[2],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[2]);
				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[2],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[2],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[2],m_ImageTrain,m_ImageMask_Accurate3);

				SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[3],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[3]);
				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[3],m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[3],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[3],m_ImageTrain,m_ImageMask_Accurate4);

			}
		}

		if (m_tmpTrainDataParam.m_bEnableAccurateLocation&& m_tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[0],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0]);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[0],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[0],m_ImageTrain,m_ImageMask_Accurate1);

			if (m_tmpTrainDataParam.m_bEnableFourPoint)
			{
				SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[1],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[1]);
				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[1],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[1],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[1],m_ImageTrain,m_ImageMask_Accurate2);

				SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[2],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[2]);
				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[2],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[2],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[2],m_ImageTrain,m_ImageMask_Accurate3);

				SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[3],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[3]);
				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[3],m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[3],m_TrainGuiParam.m_AccurateSearchTrainGuiParam[3],m_ImageTrain,m_ImageMask_Accurate4);

			}
		}


		// 头尾
		if (m_tmpTrainDataParam.m_bEnableHeadTail && m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_HeadTail,m_TrainDataParam.m_Struct_HeadTail_PatternSearchTrainData);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_HeadTail,m_TrainDataParam.m_Struct_HeadTail_PatternSearchTrainData,m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_HeadTail);
		}
		if (m_tmpTrainDataParam.m_bEnableHeadTail && m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_HeadTail,m_TrainDataParam.m_Struct_HeadTail_QuickSearchTrainData);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_HeadTail,m_TrainDataParam.m_Struct_HeadTail_QuickSearchTrainData,m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_HeadTail);
		}


		// 正反
		if (m_tmpTrainDataParam.m_bEnablePositiveAndNegative && m_tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_PN,m_TrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_PN,m_TrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData,m_TrainGuiParam.m_PN_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_PN);
		}
		if (m_tmpTrainDataParam.m_bEnablePositiveAndNegative && m_tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_PN,m_TrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_PN,m_TrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData,m_TrainGuiParam.m_PN_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_PN);
		}

		// 混料
		if (m_tmpTrainDataParam.m_bEnableMixing&& m_tmpTrainDataParam.GetMixingCheckToolType() == ePatternSearchTool)
		{
			SetPatternSearchToolParam(&m_PatternSearchTool_Mixing,m_TrainDataParam.m_Struct_Mixing_PatternSearchTrainData);
			SetPatternSearchToolImageMask(&m_PatternSearchTool_Mixing,m_TrainDataParam.m_Struct_Mixing_PatternSearchTrainData,m_TrainGuiParam.m_Mixing_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_Mixing);
		}
		if (m_tmpTrainDataParam.m_bEnableMixing && m_tmpTrainDataParam.GetMixingCheckToolType() == eQuickSearchTool)
		{
			SetQuickSearchToolParam(&m_QuickSearchTool_Mixing,m_TrainDataParam.m_Struct_Mixing_QuickSearchTrainData);
			SetQuickSearchToolImageMask(&m_QuickSearchTool_Mixing,m_TrainDataParam.m_Struct_Mixing_QuickSearchTrainData,m_TrainGuiParam.m_Mixing_Check_TrainGuiParam,m_ImageTrain,m_ImageMask_Mixing);
		}
	}

	return *this;	
}

CSpecialPickSearchTool::~CSpecialPickSearchTool(void)
{
	// 清空
	ClearAllGui();
	ClearResult();
}

CSearchToolType CSpecialPickSearchTool::GetSearchToolType()
{
	return eSpecialPickSearchTool;
}

// 设置显示控件指针
bool CSpecialPickSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
{
	ClearAllGui();

	m_pGuiDisplay = pGuiDisplay;
	m_hDispWnd = NULL;
	if (m_pGuiDisplay)
	{
		m_hDispWnd = m_pGuiDisplay->GetSafeHwnd();
	}
	m_bSearchResultGuiShow = FALSE;
	m_bTrainGuiShow        = FALSE;
	m_bTrainResultGuiShow  = FALSE;

	// 设置Gui控件
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	// 显示控件
	UpdateTrainGuiDisplay();
	UpdateTrainResultGuiDisplay();
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练结果显示控件指针
bool CSpecialPickSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
{
	ClearAllGui();

	m_pGuiTrainResultDisplay = pGuiDisplay;
	m_hTrainResultDispWnd = NULL;
	if(m_pGuiTrainResultDisplay)
	{
		m_hTrainResultDispWnd = m_pGuiTrainResultDisplay->GetSafeHwnd();
	}

	UpdateTrainGuiDisplay();
	return true;
}

// 设置训练结果显示控件指针
bool CSpecialPickSearchTool::SetTrainResultDisplay2(scGuiDisplay *pGuiDisplay)
{
	ClearAllGui();

	m_pGuiTrainResultDisplay2= pGuiDisplay;
	m_hTrainResultDispWnd2 = NULL;
	if(m_pGuiTrainResultDisplay2)
	{
		m_hTrainResultDispWnd2 = m_pGuiTrainResultDisplay2->GetSafeHwnd();
	}

	UpdateTrainGuiDisplay();
	return true;
}

// 获取显示控件指针										
scGuiDisplay* CSpecialPickSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CSpecialPickSearchTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CSpecialPickSearchTool::GetTrainResultDisplay2()
{
	return m_pGuiTrainResultDisplay2;
}

// 设置Gui图形的关键字														
bool CSpecialPickSearchTool::SetGuiKeyText(CString strGuiKeyText)									
{
	if ((strGuiKeyText == _T("")) || (strGuiKeyText == m_strKeyText))
	{
		return false;
	}

	ClearAllGui();

	m_strKeyText = strGuiKeyText;

	UpdateTrainGuiDisplay();
	UpdateTrainResultGuiDisplay();
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置Gui图形的关键字

// 获取Gui图形的关键字
bool CSpecialPickSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CSpecialPickSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CSpecialPickSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CSpecialPickSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CSpecialPickSearchTool::UpdateTrainGuiDisplay()
{	
	// 粗定位
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_bTrainRectVisible);
		m_GuiTrainAffineRect.SetVisible(false);
		m_GuiSearchRect.SetVisible(m_bTrainGuiShow);
	}
	else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		m_GuiTrainRect.SetVisible(false);
		m_GuiTrainAffineRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_bTrainRectVisible);
		m_GuiSearchRect.SetVisible(m_bTrainGuiShow);
	}
	else if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool)
	{
		m_GuiTrainRect.SetVisible(false);
		m_GuiTrainAffineRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_bTrainRectVisible);
		m_GuiSearchRect.SetVisible(false);
	}


	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_bModelPointVisible);
	//	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_bSearchRectVisible);

	// 精定位
	if ( m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
	{
		m_GuiTrainRect_Accurate[0].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[0].m_bTrainRectVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);
		m_GuiModelPoint_Accurate[0].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[0].m_bModelPointVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);
		m_GuiSearchRect_Accurate[0].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[0].m_bSearchRectVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);

		for (int it =0;it<RECT_NUMS;it++)
		{
			if (it>0)
			{
				// 启用四点
				m_GuiTrainRect_Accurate[it].SetVisible(m_tmpTrainDataParam.m_bEnableFourPoint && m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_bTrainRectVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);
				m_GuiModelPoint_Accurate[it].SetVisible(m_tmpTrainDataParam.m_bEnableFourPoint && m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_bModelPointVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);
				m_GuiSearchRect_Accurate[it].SetVisible(m_tmpTrainDataParam.m_bEnableFourPoint &&  m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_bSearchRectVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);
			}

			m_GuiTrainAffineRect_Accurate[it].SetVisible(FALSE);

		}


	} 
	else if ( m_tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
	{
		m_GuiTrainAffineRect_Accurate[0].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[0].m_bTrainRectVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);
		m_GuiModelPoint_Accurate[0].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[0].m_bModelPointVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);
		m_GuiSearchRect_Accurate[0].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[0].m_bSearchRectVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);



		for (int it =0;it<RECT_NUMS;it++)
		{
			if (it>0)
			{
				// 启用四点
				m_GuiTrainAffineRect_Accurate[it].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_bTrainRectVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);
				m_GuiModelPoint_Accurate[it].SetVisible(m_tmpTrainDataParam.m_bEnableFourPoint && m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_bModelPointVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);
				m_GuiSearchRect_Accurate[it].SetVisible(m_tmpTrainDataParam.m_bEnableFourPoint &&  m_bTrainGuiShow && m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_bSearchRectVisible && m_tmpTrainDataParam.m_bEnableAccurateLocation);

			}
			m_GuiTrainRect_Accurate[it].SetVisible(FALSE);
		}

	}
	else if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternRectSearchTool || m_tmpTrainDataParam.GetAccurateSearchToolType() ==eRectSearchTool) 
	{
		//显示找线
		for (int i=0; i<LINE_NUMS; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_GuiFindLine[i].SetVisible( m_tmpTrainDataParam.m_bEnableAccurateLocation && m_bTrainGuiShow && m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bLineVisible);
			if (m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bEnableCornerLineMask)
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
			}
			else
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
			}
		}
		// 隐藏其他
		m_GuiTrainBlobAffineRect.SetVisible(FALSE);
		m_GuiModelPoint2.SetVisible(FALSE);
	}
	else if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eCornerSearchTool || m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternCornerSearchTool)
	{
		for (int i=0; i<LINE_NUMS; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			if (i<2)
			{
				m_GuiFindLine[i].SetVisible(m_tmpTrainDataParam.m_bEnableAccurateLocation && m_bTrainGuiShow && m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bLineVisible);
			}
			else
			{
				m_GuiFindLine[i].SetVisible(FALSE);
			}

			if (m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bEnableCornerLineMask)
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
			}
			else
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
			}


		}
	}
	else if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternBlobSearchTool)
	{
		// 隐藏其他
		for (int i=0; i<LINE_NUMS; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_GuiFindLine[i].SetVisible(FALSE);
			if (m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bEnableCornerLineMask)
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
			}
			else
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
			}
		}
		m_GuiModelPoint2.SetVisible(FALSE);
		//显示
		m_GuiTrainBlobAffineRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bBlobAffineRectVisible);
	}
	else if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eMPatternSearchTool)
	{
		// 隐藏其他
		for (int i=0; i<LINE_NUMS; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_GuiFindLine[i].SetVisible(FALSE);
			if (m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bEnableCornerLineMask)
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
			}
			else
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
			}
		}
		m_GuiTrainBlobAffineRect.SetVisible(FALSE);
		// 多标记点
		m_GuiModelPoint2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_bModelPointVisible);
	}

	// 头尾检查
	if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
	{
		m_GuiTrainRect_HeadTail.SetVisible(m_tmpTrainDataParam.m_bEnableHeadTail && m_bTrainGuiShow && m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_bTrainRectVisible);
		m_GuiTrainAffineRect_HeadTail.SetVisible(false);
		m_GuiModelPoint_HeadTail.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_bModelPointVisible && m_tmpTrainDataParam.m_bEnableHeadTail);
		m_GuiSearchRect_HeadTail.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_bSearchRectVisible && m_tmpTrainDataParam.m_bEnableHeadTail);

	}
	else if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
	{
		m_GuiTrainRect_HeadTail.SetVisible(false);
		m_GuiTrainAffineRect_HeadTail.SetVisible(m_tmpTrainDataParam.m_bEnableHeadTail && m_bTrainGuiShow && m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_bTrainRectVisible);
		m_GuiModelPoint_HeadTail.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_bModelPointVisible && m_tmpTrainDataParam.m_bEnableHeadTail);
		m_GuiSearchRect_HeadTail.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_bSearchRectVisible && m_tmpTrainDataParam.m_bEnableHeadTail);

	}
	else if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == eAreaInspectToolEx)
	{
		m_GuiTrainRect_HeadTail.SetVisible(false);
		m_GuiTrainAffineRect_HeadTail.SetVisible(false);
		m_GuiModelPoint_HeadTail.SetVisible(false);
		m_GuiSearchRect_HeadTail.SetVisible(false);
	}
	//m_GuiModelPoint_HeadTail.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_bModelPointVisible && m_tmpTrainDataParam.m_bEnableHeadTail);
	//m_GuiSearchRect_HeadTail.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_bSearchRectVisible && m_tmpTrainDataParam.m_bEnableHeadTail);


	// 正反检查
	if (m_tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == ePatternSearchTool)
	{
		m_GuiTrainRect_PN.SetVisible(m_tmpTrainDataParam.m_bEnablePositiveAndNegative && m_bTrainGuiShow && m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_bTrainRectVisible);
		m_GuiTrainAffineRect_PN.SetVisible(false);
	}
	else if (m_tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == eQuickSearchTool)
	{
		m_GuiTrainRect_PN.SetVisible(false);
		m_GuiTrainAffineRect_PN.SetVisible(m_tmpTrainDataParam.m_bEnablePositiveAndNegative && m_bTrainGuiShow && m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_bTrainRectVisible);
	}
	// 混料检查
	if (m_tmpTrainDataParam.GetMixingCheckToolType() == ePatternSearchTool)
	{
		m_GuiTrainRect_Mixing.SetVisible(m_tmpTrainDataParam.m_bEnableMixing && m_bTrainGuiShow && m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_bTrainRectVisible);
		m_GuiTrainAffineRect_Mixing.SetVisible(false);
	}
	else if (m_tmpTrainDataParam.GetMixingCheckToolType() == eQuickSearchTool)
	{
		m_GuiTrainRect_Mixing.SetVisible(false);
		m_GuiTrainAffineRect_Mixing.SetVisible(m_tmpTrainDataParam.m_bEnableMixing && m_bTrainGuiShow && m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_bTrainRectVisible);
	}



	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		// 粗定位
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect_Rough"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainAffineRect, m_strKeyText+_T("_TrainAffRect_Rough"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint_Rough"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint2, m_strKeyText+_T("_ModelPoint_Rough2"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect_Rough"));

		// 精定位
		for (int it = 0;it<RECT_NUMS;it++)
		{
			CString str_Tmp =_T("");
			str_Tmp.Format(_T("_TrainRect_Accurate%d"),it+1);
			m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect_Accurate[it], m_strKeyText+ str_Tmp);
			str_Tmp.Format(_T("_TrainAffRect_Accurate%d"),it+1);
			m_pGuiInteractiveContainer->AddItem(&m_GuiTrainAffineRect_Accurate[it], m_strKeyText+str_Tmp);
			str_Tmp.Format(_T("_ModelPoint_Accurate%d"),it+1);
			m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint_Accurate[it], m_strKeyText+str_Tmp);
			str_Tmp.Format(_T("_SearchRect_Accurate%d"),it+1);
			m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect_Accurate[it], m_strKeyText+str_Tmp);
		}

		for (int i=0; i<LINE_NUMS; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine[i], strKey);
		}

		m_pGuiInteractiveContainer->AddItem(&(m_GuiTrainBlobAffineRect), m_strKeyText + _T("_TrainBlobAffineRect"));


		// 头尾检查
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect_HeadTail, m_strKeyText+_T("_TrainRect_HeadTail"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainAffineRect_HeadTail, m_strKeyText+_T("_TrainAffRect_HeadTail"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint_HeadTail, m_strKeyText+_T("_ModelPoint_HeadTail"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect_HeadTail, m_strKeyText+_T("_SearchRect_HeadTail"));

		// 正反检查
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect_PN, m_strKeyText+_T("_TrainRect_PN"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainAffineRect_PN, m_strKeyText+_T("_TrainAffRect_PN"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint_PN, m_strKeyText+_T("_ModelPoint_PN"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect_PN, m_strKeyText+_T("_SearchRect_PN"));

		// 混料检查
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect_Mixing, m_strKeyText+_T("_TrainRect_Mixing"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainAffineRect_Mixing, m_strKeyText+_T("_TrainAffRect_Mixing"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint_Mixing, m_strKeyText+_T("_ModelPoint_Mixing"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect_Mixing, m_strKeyText+_T("_SearchRect_Mixing"));




		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	{
		scFeatureletChainSet tmpscGLCS;
		bool bFalg = m_QuickSearchTool_Rough.GetTrainFeaturesFine(tmpscGLCS);
		if(bFalg)
		{
			m_GuiTrainFeaturelet.SetFeaturelet(tmpscGLCS);
			m_GuiTrainFeaturelet.SetVisible(m_bTrainOK && (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool));
			if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
			{
				m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();
				m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet ,m_strKeyText +_T("_FeatureChain"));

				m_pGuiTrainResultDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
				m_pGuiTrainResultDisplay->MyInvalidate();
			}
		}
	}
	{
		scFeatureletChainSet tmpscGLCS;
		bool bFalg = m_QuickSearchTool_HeadTail.GetTrainFeaturesFine(tmpscGLCS);
		if(bFalg)
		{
			m_GuiTrainFeaturelet2.SetFeaturelet(tmpscGLCS);
			m_GuiTrainFeaturelet2.SetVisible(m_bTrainOK && (m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool));
			if(m_pGuiTrainResultDisplay2 && ::IsWindow(m_hTrainResultDispWnd2))
			{
				m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay2->GetInteractiveContainer();
				m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet2 ,m_strKeyText +_T("_FeatureChain2"));

				m_pGuiTrainResultDisplay2->SetInteractiveContainer(m_pGuiInteractiveContainer);
				m_pGuiTrainResultDisplay2->MyInvalidate();
			}
		}
	}
	return true;
}

bool CSpecialPickSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eMQuickCornerSearchTool || m_tmpTrainDataParam.GetAccurateSearchToolType() ==eMPatternCornerSearchTool) //多标记点几何/区域角定位工具
	{
		for (int it = 0;it<LINE_NUMS;it++)
		{
			m_GuiTrainResultLine[it].SetVisible(m_bTrainResultGuiShow && m_tmpTrainDataParam.m_bEnableAccurateLocation);
		}
	}
	else if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eCornerSearchTool || m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternCornerSearchTool)
	{
		for (int it = 0;it<LINE_NUMS;it++)
		{
			if (it<2)
			{
				m_GuiTrainResultLine[it].SetVisible(m_bTrainResultGuiShow && m_tmpTrainDataParam.m_bEnableAccurateLocation);
			}
			else
			{
				m_GuiTrainResultLine[it].SetVisible(FALSE);
			}
		}
	}



	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();

		for (int it = 0;it<LINE_NUMS;it++)
		{
			CString str_tmp = _T("");
			str_tmp.Format(_T("%s_TrainResultLine_%d"),m_strKeyText,it+1);
			m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[it], str_tmp);
		}


		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CSpecialPickSearchTool::UpdateSearchResultGuiDisplay()							
{
	int i=0;

	for (i=0; i<m_pGuiSearchRectArray.size(); i++)
	{
		m_pGuiSearchRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bRectVisible);
	}

	for (i=0; i<m_pGuiSearchRectArray_HeadTail.size(); i++)
	{
		m_pGuiSearchRectArray_HeadTail[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bRectVisible && m_tmpTrainDataParam.m_bEnableHeadTail);
	}

	//for (i=0; i<m_pGuiSearchRectArray_Accurate.size(); i++)
	//{
	//	m_pGuiSearchRectArray_Accurate[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bRectVisible );
	//}


	for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
	{
		m_pGuiSearchCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bPointVisible);
	}
	for (i=0; i<m_pGuiSearchCrossArray_HeadTail.size(); i++)
	{
		m_pGuiSearchCrossArray_HeadTail[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bPointVisible && m_tmpTrainDataParam.m_bEnableHeadTail);
	}

	for (i=0; i<m_pGuiSearchLineArray.size(); i++)
	{
		m_pGuiSearchLineArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchLineTextArray.size(); i++)
	{
		m_pGuiSearchLineTextArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray2.size(); i++)
	{
		m_pGuiSearchLineCrossArray2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray3.size(); i++)
	{
		m_pGuiSearchLineCrossArray3[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	// 拟合点
	for (i=0; i<m_pGuiSearchResult_Line_FitPointArray1.size(); i++)
	{
		m_pGuiSearchResult_Line_FitPointArray1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bFindLineVisibleFitPoint[0]);
	}
	for (i=0; i<m_pGuiSearchResult_Line_FitPointArray2.size(); i++)
	{
		m_pGuiSearchResult_Line_FitPointArray2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bFindLineVisibleFitPoint[1]);
	}
	for (i=0; i<m_pGuiSearchResult_Line_FitPointArray3.size(); i++)
	{
		m_pGuiSearchResult_Line_FitPointArray3[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bFindLineVisibleFitPoint[2]);
	}
	for (i=0; i<m_pGuiSearchResult_Line_FitPointArray4.size(); i++)
	{
		m_pGuiSearchResult_Line_FitPointArray4[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bFindLineVisibleFitPoint[3]);
	}

	for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
	{
		m_pGuiSearchAffineRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArray_Blob.size(); i++)
	{
		m_pGuiSearchCrossArray_Blob[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}

	for (i=0; i<m_pGuiBlobBoundCenterArray.size(); i++)
	{
		m_pGuiBlobBoundCenterArray[i]->SetVisible(m_bSearchResultGuiShow);
	}

	for (i=0;i<m_pGuiSearchBlobArray.size();i++)
	{
		m_pGuiSearchBlobArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bBlobVisible);
	}

	for (i=0;i<m_pGuiSearchBlobContour.size();i++)
	{
		m_pGuiSearchBlobContour[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bBlobVisible_Contour);
	}

	for (i=0;i<m_pGuiSearchBlobContour_Rough.size();i++)
	{
		m_pGuiSearchBlobContour_Rough[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bBlobVisible_Contour_Rough);
	}

	//m_GuiContour.SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bBlobVisible_Contour);



	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		if (m_bSearchResultGuiShow) // 立即更新
		{
			int i=0;

			for (i=0; i<m_pGuiSearchRectArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchRectArray[i], m_pGuiSearchRectArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray[i], m_pGuiSearchCrossArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchRectArray_HeadTail.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchRectArray_HeadTail[i], m_pGuiSearchRectArray_HeadTail[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchCrossArray_HeadTail.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray_HeadTail[i], m_pGuiSearchCrossArray_HeadTail[i]->GetTipText());
			}


			for (i=0; i<m_pGuiSearchLineArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArray[i], m_pGuiSearchLineArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchLineTextArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineTextArray[i], m_pGuiSearchLineTextArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
			}


			for (i=0; i<m_pGuiSearchLineCrossArray2.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray2[i], m_pGuiSearchLineCrossArray2[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchLineCrossArray3.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray3[i], m_pGuiSearchLineCrossArray3[i]->GetTipText());
			}


			for (i=0; i<m_pGuiSearchResult_Line_FitPointArray1.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResult_Line_FitPointArray1[i], m_pGuiSearchResult_Line_FitPointArray1[i]->GetTipText());
			}
			for (i=0; i<m_pGuiSearchResult_Line_FitPointArray2.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResult_Line_FitPointArray2[i], m_pGuiSearchResult_Line_FitPointArray2[i]->GetTipText());
			}
			for (i=0; i<m_pGuiSearchResult_Line_FitPointArray3.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResult_Line_FitPointArray3[i], m_pGuiSearchResult_Line_FitPointArray3[i]->GetTipText());
			}
			for (i=0; i<m_pGuiSearchResult_Line_FitPointArray4.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResult_Line_FitPointArray4[i], m_pGuiSearchResult_Line_FitPointArray4[i]->GetTipText());
			}


			for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchAffineRectArray[i], m_pGuiSearchAffineRectArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchCrossArray_Blob.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray_Blob[i], m_pGuiSearchCrossArray_Blob[i]->GetTipText());
			}

			for (i=0; i<m_pGuiBlobBoundCenterArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiBlobBoundCenterArray[i], m_pGuiBlobBoundCenterArray[i]->GetTipText());
			}


			for (i=0;i<m_pGuiSearchBlobArray.size();i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchBlobArray[i], m_pGuiSearchBlobArray[i]->GetTipText());
			}


			//m_pGuiStaticContainer->AddItem(&m_GuiContour,"blob result");

			for (i=0;i<m_pGuiSearchBlobContour.size();i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchBlobContour[i], m_pGuiSearchBlobContour[i]->GetTipText());
			}

			for (i=0;i<m_pGuiSearchBlobContour_Rough.size();i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchBlobContour_Rough[i], m_pGuiSearchBlobContour_Rough[i]->GetTipText());
			}
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新界面显示
bool CSpecialPickSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CSpecialPickSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CSpecialPickSearchTrainDataParam*)pTrainDataParam);

	return true;
}

bool CSpecialPickSearchTool::SetTrainDataParamForce(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	m_TrainDataParam = *((CSpecialPickSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CSpecialPickSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	*((CSpecialPickSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CSpecialPickSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CSpecialPickSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CSpecialPickSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CSpecialPickSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CSpecialPickSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CSpecialPickSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CSpecialPickSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}


// 获取训练原始图像								
bool CSpecialPickSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

bool CSpecialPickSearchTool::SetMaskImage1(const cpImage &ImageMask)
{
	m_tmpImageMask_Accurate1 = ImageMask;

	return true;
}
bool CSpecialPickSearchTool::GetMaskImage1(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask_Accurate1;

	return true;
}


bool CSpecialPickSearchTool::SetMaskImage2(const cpImage &ImageMask)
{
	m_tmpImageMask_Accurate2 = ImageMask;

	return true;
}
bool CSpecialPickSearchTool::GetMaskImage2(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask_Accurate2;

	return true;
}

bool CSpecialPickSearchTool::SetMaskImage3(const cpImage &ImageMask)
{
	m_tmpImageMask_Accurate3 = ImageMask;

	return true;
}
bool CSpecialPickSearchTool::GetMaskImage3(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask_Accurate3;

	return true;
}

bool CSpecialPickSearchTool::SetMaskImage4(const cpImage &ImageMask)
{
	m_tmpImageMask_Accurate4 = ImageMask;

	return true;
}
bool CSpecialPickSearchTool::GetMaskImage4(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask_Accurate4;

	return true;
}


bool CSpecialPickSearchTool::SetMaskImage_HeadTail(const cpImage &ImageMask)
{
	m_tmpImageMask_HeadTail = ImageMask;

	return true;
}


// 获取训练原始图像								
bool CSpecialPickSearchTool::GetMaskImage_HeadTail(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask_HeadTail;

	return true;
}


bool CSpecialPickSearchTool::SetMaskImage_PN(const cpImage &ImageMask)
{
	m_tmpImageMask_PN = ImageMask;

	return true;
}


// 获取训练原始图像								
bool CSpecialPickSearchTool::GetMaskImage_PN(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask_PN;

	return true;
}

bool CSpecialPickSearchTool::SetMaskImage_Mixing(const cpImage &ImageMask)
{
	m_tmpImageMask_Mixing = ImageMask;

	return true;
}


// 获取训练原始图像								
bool CSpecialPickSearchTool::GetMaskImage_Mixing(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask_Mixing;

	return true;
}




// 执行训练											
bool CSpecialPickSearchTool::Train()
{

	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
	BOOL bIsProcessImage = FALSE;
	if (1 != m_tmpImageTrain.PixelSize())
	{
		bIsProcessImage = TRUE;
	}

	cpImage tmpImage;
	cpImage tmpImageMask;
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_tmpImageTrain.Width(),m_tmpImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageTrain,tmpImage);

		tmpImageMask.CreateImageBuffer(m_tmpImageMask.Width(),m_tmpImageMask.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask,tmpImageMask);
	}		
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		// 设置训练参数

		if(!SetPatternSearchToolParam(&m_PatternSearchTool_Rough,m_tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData))
		{
			m_nResultErrNumber = -1;
			return false;
		}

		SetPatternSearchToolImageMask(&m_PatternSearchTool_Rough,m_tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData,m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam,m_tmpImageTrain,m_tmpImageMask);

		if (!m_bTrainOK)
		{
			m_nResultErrNumber = -1;
			return false;
		}
		if(m_tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_bMaskEnable)
		{
			m_ImageMask  = m_tmpImageMask;
		}

		sc2Vector cModelOrigin;
		m_PatternSearchTool_Rough.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelY = cModelOrigin.GetY();
	}
	else if(m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		// 设置参数
		if(!SetQuickSearchToolParam(&m_QuickSearchTool_Rough,m_tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData))
		{
			m_nResultErrNumber = -1;
			return false;
		}

		SetQuickSearchToolImageMask(&m_QuickSearchTool_Rough,m_tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData,m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam,m_tmpImageTrain,m_tmpImageMask);
		if (!m_bTrainOK)
		{
			m_nResultErrNumber = -1;
			return false;
		}
		if(m_tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData.m_bMaskEnable)
		{
			m_ImageMask  = m_tmpImageMask;
		}

		sc2Vector cModelOrigin;
		m_QuickSearchTool_Rough.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelY = cModelOrigin.GetY();
	}
	else if(m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool) // Blob粗定位
	{
		m_BlobTool_Rough.SetSegmentationHardThresh(m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_lBlobGrayThre,m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_bBlobMode);
		m_BlobTool_Rough.ConnectivityType(scBlobSceneDescription::eGreyScale);
		m_BlobTool_Rough.ConnectivityCleanup(scBlobSceneDescription::ePrune);
		m_BlobTool_Rough.ConnectivityMinPels(m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_lBlobAreaMinThre);
		//m_BlobTool_Rough.EnableComputeInertia(true);
		//m_BlobTool_Rough.EnableComputeContour(true);
		bool bSuccess = false;
		if (bIsProcessImage)
		{
			 bSuccess = m_BlobTool_Rough.Execute(tmpImage,&(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainAffineRect));
		}
		else
		{
			 bSuccess = m_BlobTool_Rough.Execute(m_tmpImageTrain,&(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainAffineRect));
		}


		if (!bSuccess)
		{
			m_nResultErrNumber = -2;
			return false;
		}
		scBlobSceneDescription* pBSD = m_BlobTool_Rough.GetBSD();
		if (pBSD == NULL)
		{
			m_nResultErrNumber = -3;
			return false;
		}

		pBSD->filter(eArea,m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_lBlobAreaMinThre,m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_lBlobAreaMaxThre,0);

		int nResultBlobNum = pBSD->numBlobs(true); 
		if(nResultBlobNum<1)
		{
			m_nResultErrNumber = -22;
			return false;
		}
		const scBlob* pChildBlob = pBSD->getBlob(1); 
		if(pChildBlob == NULL)
		{
			m_nResultErrNumber = -4;
			return false;
		}

		sc2Vector cModelOrigin;
		cModelOrigin.SetX(pChildBlob->imageBoundCenterX);
		cModelOrigin.SetY(pChildBlob->imageBoundCenterY);

		m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelY = cModelOrigin.GetY();

		m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelX1 = m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint.GetX();
		m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelY1 = m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint.GetY();

	}


	// 启用精定位
	if (m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
		{

			if (!m_tmpTrainDataParam.m_bEnableFourPoint)
			{
				// 设置训练参数
				if(!SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[0],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0]))
				{
					m_nResultErrNumber = -1;
					return false;
				}

				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[0],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[0],m_tmpImageTrain,m_tmpImageMask_Accurate1);

				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0].m_bMaskEnable)
				{
					m_ImageMask_Accurate1  = m_tmpImageMask_Accurate1;
				}
				sc2Vector cModelOrigin;
				m_PatternSearchTool_Accurate[0].GetPatternOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[0].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[0].m_dModelY = cModelOrigin.GetY();

			}
			else
			{
				// 设置训练参数
				if(!SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[0],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0]))
				{
					m_nResultErrNumber = -1;
					return false;
				}

				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[0],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[0],m_tmpImageTrain,m_tmpImageMask_Accurate1);

				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0].m_bMaskEnable)
				{
					m_ImageMask_Accurate1  = m_tmpImageMask_Accurate1;
				}
				sc2Vector cModelOrigin;
				m_PatternSearchTool_Accurate[1].GetPatternOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[1].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[1].m_dModelY = cModelOrigin.GetY();

				if(!SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[1],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[1])) 
				{
					m_nResultErrNumber = -1;
					return false;
				}
				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[1],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[1],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[1],m_tmpImageTrain,m_tmpImageMask_Accurate2);
				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[1].m_bMaskEnable)
				{
					m_ImageMask_Accurate2  = m_tmpImageMask_Accurate2;
				}
				m_PatternSearchTool_Accurate[1].GetPatternOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[1].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[1].m_dModelY = cModelOrigin.GetY();

				if(!SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[2],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[2])) return false;
				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[2],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[2],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[2],m_tmpImageTrain,m_tmpImageMask_Accurate3);
				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[2].m_bMaskEnable)
				{
					m_ImageMask_Accurate3  = m_tmpImageMask_Accurate3;
				}


				m_PatternSearchTool_Accurate[2].GetPatternOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[2].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[2].m_dModelY = cModelOrigin.GetY();

				if(!SetPatternSearchToolParam(&m_PatternSearchTool_Accurate[3],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[3])) return false;
				SetPatternSearchToolImageMask(&m_PatternSearchTool_Accurate[3],m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[3],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[3],m_tmpImageTrain,m_tmpImageMask_Accurate4);
				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[3].m_bMaskEnable)
				{
					m_ImageMask_Accurate4  = m_tmpImageMask_Accurate4;
				}

				m_PatternSearchTool_Accurate[3].GetPatternOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[3].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[3].m_dModelY = cModelOrigin.GetY();
			}

		}
		else if(m_tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
		{

			if (!m_tmpTrainDataParam.m_bEnableFourPoint)
			{
				// 设置参数
				if(!SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[0],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0])) 
				{
					m_nResultErrNumber = -1;
					return false;
				}
				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[0],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[0],m_tmpImageTrain,m_tmpImageMask_Accurate1);
				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0].m_bMaskEnable)
				{
					m_ImageMask_Accurate1  = m_tmpImageMask_Accurate1;
				}
				sc2Vector cModelOrigin;
				m_QuickSearchTool_Accurate[0].GetModelOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[0].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[0].m_dModelY = cModelOrigin.GetY();
			}
			else
			{
				// 设置参数
				if(!SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[0],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0])) return false;

				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[0],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[1],m_tmpImageTrain,m_tmpImageMask_Accurate1);
				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0].m_bMaskEnable)
				{
					m_ImageMask_Accurate1  = m_tmpImageMask_Accurate1;
				}

				sc2Vector cModelOrigin;
				m_QuickSearchTool_Accurate[0].GetModelOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[0].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[0].m_dModelY = cModelOrigin.GetY();


				// 设置参数
				if(!SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[1],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[1])) 
				{
					m_nResultErrNumber = -1;
					return false;
				}

				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[1],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[1],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[1],m_tmpImageTrain,m_tmpImageMask_Accurate2);
				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[1].m_bMaskEnable)
				{
					m_ImageMask_Accurate2  = m_tmpImageMask_Accurate2;
				}

				//sc2Vector cModelOrigin;
				m_QuickSearchTool_Accurate[1].GetModelOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[1].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[1].m_dModelY = cModelOrigin.GetY();

				// 设置参数
				if(!SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[2],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[2])) 
				{
					m_nResultErrNumber = -1;
					return false;
				}

				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[2],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[2],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[2],m_tmpImageTrain,m_tmpImageMask_Accurate3);
				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[2].m_bMaskEnable)
				{
					m_ImageMask_Accurate3  = m_tmpImageMask_Accurate3;
				}
				//sc2Vector cModelOrigin;
				m_QuickSearchTool_Accurate[2].GetModelOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[2].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[2].m_dModelY = cModelOrigin.GetY();

				// 设置参数
				if(!SetQuickSearchToolParam(&m_QuickSearchTool_Accurate[3],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[3]))
				{
					m_nResultErrNumber = -1;
					return false;
				}

				SetQuickSearchToolImageMask(&m_QuickSearchTool_Accurate[3],m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[3],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[3],m_tmpImageTrain,m_tmpImageMask_Accurate3);
				if (!m_bTrainOK)
				{
					m_nResultErrNumber = -1;
					return false;
				}
				if(m_tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[3].m_bMaskEnable)
				{
					m_ImageMask_Accurate4  = m_tmpImageMask_Accurate4;
				}

				//sc2Vector cModelOrigin;
				m_QuickSearchTool_Accurate[3].GetModelOrigin(cModelOrigin);
				m_tmpTrainResult.m_Accurate_RectTrainResult[3].m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_Accurate_RectTrainResult[3].m_dModelY = cModelOrigin.GetY();

			}


		}
		else if(m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternRectSearchTool || m_tmpTrainDataParam.GetAccurateSearchToolType() == eRectSearchTool)
		{
			scLineSeg cLineSeg_Array[LINE_NUMS];
			double dRMS = 0;
			for (int it=0;it<LINE_NUMS;it++)
			{
				SetFindLineSearchToolParam(&m_FindLineTool_Accurate[it],m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[it],m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it]);
				bool bFindLine = false;
				if(bIsProcessImage)
				{
					bFindLine = m_FindLineTool_Accurate[it].Execute(tmpImage,cLineSeg_Array[it],dRMS);
				}
				else
				{
					bFindLine = m_FindLineTool_Accurate[it].Execute(m_tmpImageTrain,cLineSeg_Array[it],dRMS);
				}
				if(!bFindLine)
				{
					m_nResultErrNumber = -11-it;
					return false;
				}
			}

			// 求交点
			for (int i=0;i<LINE_NUMS-1;i++)
			{
				for (int j=i+1;j<LINE_NUMS;j++)
				{
					bool bInterSect = false;
					std::vector<sc2Vector> vectArray;
					bool bRet = false;
					bRet = sfInterSectLineLine(cLineSeg_Array[i].GetLine(), cLineSeg_Array[j].GetLine(), bInterSect, vectArray);
					if (!bRet || !bInterSect || vectArray.size() <= 0)
					{
						m_nResultErrNumber = -15;
						return false;
					}

				}
			}

			// 赋值到训练结果中
			for (int it= 0; it<RECT_NUMS;it++)
			{
				m_tmpTrainResult.m_FindLineTrainResult[it].m_dStartX = cLineSeg_Array[it].GetP1().GetX();
				m_tmpTrainResult.m_FindLineTrainResult[it].m_dStartY = cLineSeg_Array[it].GetP1().GetY();
				m_tmpTrainResult.m_FindLineTrainResult[it].m_dEndX = cLineSeg_Array[it].GetP2().GetX();
				m_tmpTrainResult.m_FindLineTrainResult[it].m_dEndY = cLineSeg_Array[it].GetP2().GetY();

				m_GuiTrainResultLine[it].SetLine(cLineSeg_Array[it].GetLine());
			}



		}
		else if(m_tmpTrainDataParam.GetAccurateSearchToolType() == eCornerSearchTool || m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternCornerSearchTool)
		{
			scLineSeg cLineSeg_Array[LINE_NUMS/2];
			double dRMS = 0;
			for (int it=0;it<LINE_NUMS/2;it++)
			{
				SetFindLineSearchToolParam(&m_FindLineTool_Accurate[it],m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[it],m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it]);
				if(!m_FindLineTool_Accurate[it].Execute(tmpImage,cLineSeg_Array[it].GetLine(),dRMS)) 
				{
					m_nResultErrNumber = -11-it;
					return false;
				}

			}

			// 求交点
			for (int i=0;i<LINE_NUMS/2-1;i++)
			{
				for (int j=i+1;j<LINE_NUMS/2;j++)
				{
					bool bInterSect = false;
					bool bRet = false;
					std::vector<sc2Vector> vectArray;
					bRet = sfInterSectLineLine(cLineSeg_Array[i].GetLine(), cLineSeg_Array[j].GetLine(), bInterSect, vectArray);
					if (!bRet || !bInterSect || vectArray.size() <= 0)
					{
						m_nResultErrNumber = -15;
						return false;
					}

				}
			}

			// 赋值到训练结果中
			for (int it= 0; it<RECT_NUMS;it++)
			{
				m_tmpTrainResult.m_FindLineTrainResult[it].m_dStartX = cLineSeg_Array[it].GetP1().GetX();
				m_tmpTrainResult.m_FindLineTrainResult[it].m_dStartY = cLineSeg_Array[it].GetP1().GetY();
				m_tmpTrainResult.m_FindLineTrainResult[it].m_dEndX = cLineSeg_Array[it].GetP2().GetX();
				m_tmpTrainResult.m_FindLineTrainResult[it].m_dEndY = cLineSeg_Array[it].GetP2().GetY();

				m_GuiTrainResultLine[it].SetLine(cLineSeg_Array[it].GetLine());
			}
		}
		else if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternBlobSearchTool)
		{
			//m_BlobTool.SetSegmentationHardThresh(m_tmpSearchDataParam.m_lBlobGrayThre,m_tmpSearchDataParam.m_bBlobMode);
			//m_BlobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
			//m_BlobTool.ConnectivityCleanup(scBlobSceneDescription::ePrune);
			//m_BlobTool.ConnectivityMinPels(m_tmpSearchDataParam.m_lConnectivityMinPels);
			//m_BlobTool.EnableComputeInertia(true);

			//bool bSuccess = m_BlobTool.Execute(m_tmpImageTrain,&(m_tmpTrainGuiParam.m_BlobAffineRect));
			//if (!bSuccess)
			//{
			//	return false;
			//}

			//scBlobSceneDescription* pBSD = m_BlobTool.GetBSD();
			//if (pBSD == NULL)
			//{
			//	return false;
			//}

			////			pBSD->filter(eArea,m_tmpSearchDataParam.m_lBlobAreaMinThre,m_tmpSearchDataParam.m_lBlobAreaMaxThre,0);
			//scBlob * const *pBlobB = pBSD->allBlobs(false);
			//int nResultBlobNum = pBSD->numBlobs(true); 


			//if (nResultBlobNum < 1)
			//{					
			//	return false;
			//}
		}
		else if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eMPatternSearchTool)
		{
			m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelX1 = m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint1.GetX();
			m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelY1 = m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint1.GetY();
			m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelX2 = m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint2.GetX();
			m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelY2 = m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint2.GetY();
		}

	}

	// 
	if (m_tmpTrainDataParam.m_bEnableHeadTail)
	{
		if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
		{
			// 设置训练参数

			if(!SetPatternSearchToolParam(&m_PatternSearchTool_HeadTail,m_tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData))
			{
				m_nResultErrNumber = -40;
				return false;
			}

			SetPatternSearchToolImageMask(&m_PatternSearchTool_HeadTail,m_tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData,m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam,m_tmpImageTrain,m_tmpImageMask_HeadTail);

			if (!m_bTrainOK)
			{
				m_nResultErrNumber = -40;
				return false;
			}
			if(m_tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable)
			{
				m_ImageMask_HeadTail  = m_tmpImageMask_HeadTail;
			}

			//sc2Vector cModelOrigin_HeadTail;
			//m_PatternSearchTool_HeadTail.GetPatternOrigin(cModelOrigin_HeadTail);
			//m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelX = cModelOrigin_HeadTail.GetX();
			//m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelY = cModelOrigin_HeadTail.GetY();

		}
		else if(m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
		{
			// 设置参数
			if(!SetQuickSearchToolParam(&m_QuickSearchTool_HeadTail,m_tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData))
			{
				m_nResultErrNumber = -40;
				return false;
			}

			SetQuickSearchToolImageMask(&m_QuickSearchTool_HeadTail,m_tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData,m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam,m_tmpImageTrain,m_tmpImageMask_HeadTail);
			if (!m_bTrainOK)
			{
				m_nResultErrNumber = -40;
				return false;
			}
			if(m_tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_bMaskEnable)
			{
				m_ImageMask_HeadTail  = m_tmpImageMask_HeadTail;
			}

			//sc2Vector cModelOrigin_HeadTail;
			//m_QuickSearchTool_Rough.GetModelOrigin(cModelOrigin_HeadTail);
			//m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelX = cModelOrigin_HeadTail.GetX();
			//m_tmpTrainResult.m_Rough_RectTrainResult.m_dModelY = cModelOrigin_HeadTail.GetY();
		}

	}


	m_bTrainOK       = true;
	m_ImageTrain     = m_tmpImageTrain;
	m_ImageMask      = m_tmpImageMask;
	m_ImageMask_Accurate1 = m_tmpImageMask_Accurate1;
	m_ImageMask_Accurate2 = m_tmpImageMask_Accurate2;
	m_ImageMask_Accurate3 = m_tmpImageMask_Accurate3;
	m_ImageMask_Accurate4 = m_tmpImageMask_Accurate4;

	m_ImageMask_HeadTail      = m_tmpImageMask_HeadTail;

	m_TrainDataParam = m_tmpTrainDataParam;
	m_TrainGuiParam	 = m_tmpTrainGuiParam;
	m_TrainResult    = m_tmpTrainResult;

	return true;
}



// 获取训练结果图像														
bool CSpecialPickSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		bRet = m_QuickSearchTool_Rough.GetTrainModelImage(m_ImageTrainResult);
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		bRet = m_PatternSearchTool_Rough.GetPatternImage(m_ImageTrainResult);
	}



	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}

//头尾检查获取训练结果图像
bool CSpecialPickSearchTool::GetTrainResultImage_HeadTail(cpImage &ImageTrainResult)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
	{
		bRet = m_QuickSearchTool_HeadTail.GetTrainModelImage(m_ImageTrainResult_HeadTail);
	}
	if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
	{
		bRet = m_PatternSearchTool_HeadTail.GetPatternImage(m_ImageTrainResult_HeadTail);
	}


	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult_HeadTail;

	return true;
}

bool CSpecialPickSearchTool::GetTrainResultImage2(cpImage &ImageTrainResult,const int index)
{
	bool bRet = false;
	if(index >= RECT_NUMS || index<0) return false;
	if (!m_tmpTrainDataParam.m_bEnableFourPoint && index!=0) return false;

	if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool && m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		switch(index)
		{
		case 0:
			{
				bRet = m_QuickSearchTool_Accurate[0].GetTrainModelImage(m_ImageTrainResult_Accurate1);
			}
			break;
		case 1:
			{
				bRet = m_QuickSearchTool_Accurate[1].GetTrainModelImage(m_ImageTrainResult_Accurate2);
			}
			break;
		case 2:
			{
				bRet = m_QuickSearchTool_Accurate[2].GetTrainModelImage(m_ImageTrainResult_Accurate3);
			}
			break;
		case 3:
			{
				bRet = m_QuickSearchTool_Accurate[3].GetTrainModelImage(m_ImageTrainResult_Accurate4);
			}
			break;
		default:
			{
				bRet = m_QuickSearchTool_Accurate[0].GetTrainModelImage(m_ImageTrainResult_Accurate1);
			}
			break;
		}

	}
	if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool && m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		switch(index)
		{
		case 0:
			{
				bRet = m_PatternSearchTool_Accurate[0].GetPatternImage(m_ImageTrainResult_Accurate1);
			}
			break;
		case 1:
			{
				bRet = m_PatternSearchTool_Accurate[1].GetPatternImage(m_ImageTrainResult_Accurate2);
			}
			break;
		case 2:
			{
				bRet = m_PatternSearchTool_Accurate[2].GetPatternImage(m_ImageTrainResult_Accurate3);
			}
			break;
		case 3:
			{
				bRet = m_PatternSearchTool_Accurate[3].GetPatternImage(m_ImageTrainResult_Accurate4);
			}
			break;
		default:
			{
				bRet = m_PatternSearchTool_Accurate[0].GetPatternImage(m_ImageTrainResult_Accurate1);
			}
			break;
		}

	}


	if (!bRet)
	{
		return false;
	}


	switch(index)
	{
	case 0:
		{
			ImageTrainResult = m_ImageTrainResult_Accurate1;
		}
		break;
	case 1:
		{
			ImageTrainResult = m_ImageTrainResult_Accurate2;
		}
		break;
	case 2:
		{
			ImageTrainResult = m_ImageTrainResult_Accurate3;
		}
		break;
	case 3:
		{
			ImageTrainResult = m_ImageTrainResult_Accurate4;
		}
		break;
	default:
		{
			ImageTrainResult = m_ImageTrainResult_Accurate1;
		}
		break;
	}

	return true;
}

// 获取训练结果
bool CSpecialPickSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	*((CSpecialPickSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CSpecialPickSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CSpecialPickSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CSpecialPickSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CSpecialPickSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	*((CSpecialPickSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CSpecialPickSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CSpecialPickSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}


// 获取搜索Gui参数			
bool CSpecialPickSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eSpecialPickSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CSpecialPickSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}





// 执行搜索 					
bool CSpecialPickSearchTool::Search(cpImage &ImageSearch)
{
	/************************************************************************/
	/* 流程：
	（1）搜素区内进行粗定位（区域、几何、Blob）
	（2）粗定位调整精定位卡尺，进行定位，可以得到2个或4个点
	（3）如果是精定位为4点结果，则在精定位的长边方向的两侧（长边两侧定义为头尾），构建仿射矩形区域，在区域内进行Blob分析或灰度直方图分析，做头尾检测；判定头尾：
	（4）根据头尾判定结果，和处理策略，对精定位的输出4点做相应处理
	（5）输出点为2 时，头尾检查没有意义，与简单多标记点区域或几何工具等同*/

	/************************************************************************/

	if (!m_bTrainOK)
	{
		m_nResultErrNumber = -99; //训练失败
		return false;
	}	
	bool bRet = false;
	// 清空
	ClearResult();
	m_nResultErrNumber=0;

	// 训练结果找线数据
	scLineSeg modelLineSeg[LINE_NUMS];
	for (int it=0; it<LINE_NUMS;it++)
	{
		modelLineSeg[it].SetP1(sc2Vector(m_TrainResult.m_FindLineTrainResult[it].m_dStartX,m_TrainResult.m_FindLineTrainResult[it].m_dStartX));
		modelLineSeg[it].SetP2(sc2Vector(m_TrainResult.m_FindLineTrainResult[it].m_dEndX,m_TrainResult.m_FindLineTrainResult[it].m_dEndY));
	}
	cpImage tmpImage;
	BOOL bIsProcessImage = FALSE;
	if (1 != ImageSearch.PixelSize())
	{
		bIsProcessImage = TRUE;
	}

	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(ImageSearch.Width(),ImageSearch.Height(),epGray8);
		if (!GetProcessImage(&m_tmpTrainDataParam,ImageSearch,tmpImage))
		{
			m_nResultErrNumber = -1;
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//粗定位是否成功
	bool bRet_Rough = false;
	int nResultBlobNum = 0;
	scBlobSceneDescription* pBSD = NULL;
	std::vector<sc2Vector> vvResultPoint;
	std::vector<sc2Vector> vResultArray;
	int nBlobSortIndex[MAXRESNUM];
	// 粗定位
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		// 执行区域初定位搜索
		if(bIsProcessImage)
		{
			bRet_Rough = ExecutePatternSerach(tmpImage,&m_PatternSearchTool_Rough,&(m_TrainGuiParam.m_RoughSearchTrainGuiParam.m_SearchRect),
				m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_nSearchNum,
				m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dAcceptThre,
				m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dConfusionThre,0,0,0);
		}
		else
		{
			bRet_Rough = ExecutePatternSerach(ImageSearch,&m_PatternSearchTool_Rough,&(m_TrainGuiParam.m_RoughSearchTrainGuiParam.m_SearchRect),
				m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_nSearchNum,
				m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dAcceptThre,
				m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_dConfusionThre,0,0,0);
		}
		// 启用粗定位-按坐标排序
		if (bRet_Rough && m_tmpSearchDataParam.m_bCoordSortEnable)
		{
			m_PatternSearchTool_Rough.SortByCoordXY(m_tmpSearchDataParam.m_dCoordSortGap,	m_tmpSearchDataParam.m_nCoordSortMethod,
				m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
		}

	}
	else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		if(bIsProcessImage)
		{
			bRet_Rough = ExecuteQuickSearch(tmpImage,&m_QuickSearchTool_Rough,&(m_TrainGuiParam.m_RoughSearchTrainGuiParam.m_SearchRect),
				m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_nSearchNum,
				m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAcceptThre,
				m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleLow,
				m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleHigh,1,1,0,0,0);
		}
		else
		{
			bRet_Rough = ExecuteQuickSearch(ImageSearch,&m_QuickSearchTool_Rough,&(m_TrainGuiParam.m_RoughSearchTrainGuiParam.m_SearchRect),
				m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_nSearchNum,
				m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAcceptThre,
				m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleLow,
				m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dAngleHigh,1,1,0,0,0);
		}
		

		// 启用粗定位-按坐标排序
		if (bRet_Rough && m_tmpSearchDataParam.m_bCoordSortEnable)
		{
			m_QuickSearchTool_Rough.SortByCoordXY(m_tmpSearchDataParam.m_dCoordSortGap,	m_tmpSearchDataParam.m_nCoordSortMethod,
				m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
		}

	}
	else if(m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool)
	{
		m_BlobTool_Rough.SetSegmentationHardThresh(m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_lBlobGrayThre,m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_bBlobMode);
		m_BlobTool_Rough.ConnectivityType(scBlobSceneDescription::eGreyScale);
		m_BlobTool_Rough.ConnectivityCleanup(scBlobSceneDescription::ePrune);
		m_BlobTool_Rough.ConnectivityMinPels(m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_lBlobAreaMinThre);
		m_tmpSearchGuiParam.m_bBlobVisible_Contour_Rough = false;
		m_BlobTool_Rough.EnableComputeInertia(true);
		//m_BlobTool_Rough.EnableComputeContour(true);
		if(bIsProcessImage)
		{
			bRet_Rough = m_BlobTool_Rough.Execute(tmpImage,&(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainAffineRect));
		}
		else
		{
			bRet_Rough = m_BlobTool_Rough.Execute(ImageSearch,&(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainAffineRect));
		}
		if (!bRet_Rough)
		{
			m_tmpSearchGuiParam.m_bBlobVisible_Contour_Rough = false;
			m_nResultErrNumber = -2;
			return false;
		}

		pBSD = m_BlobTool_Rough.GetBSD();
		if (pBSD == NULL)
		{
			m_tmpSearchGuiParam.m_bBlobVisible_Contour_Rough = false;
			m_nResultErrNumber = -3;
			return false;
		}

		pBSD->filter(eArea,m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_lBlobAreaMinThre,m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_lBlobAreaMaxThre,0);
		pBSD->sort(eArea,eDescending);
		scBlob * const *pBlobB = pBSD->allBlobs(false);

		nResultBlobNum = pBSD->numBlobs(true); 


		for (int it = 0;it<nResultBlobNum && it<MAXRESNUM;it++)
		{
			const scBlob* pChildBlob1 = pBSD->getBlob(it+1); 
			if(pChildBlob1 == NULL)
			{
				m_tmpSearchGuiParam.m_bBlobVisible_Contour_Rough = false;
				m_nResultErrNumber = -4;
				return false;
			}
			vvResultPoint.push_back(sc2Vector(pChildBlob1->imageBoundCenterX,pChildBlob1->imageBoundCenterY));
			vResultArray.push_back(sc2Vector(pChildBlob1->imageBoundCenterX,pChildBlob1->imageBoundCenterY));
			nBlobSortIndex[it] = it;
		}

		// 对粗定位进行排序
		if(bRet_Rough && m_tmpSearchDataParam.m_bCoordSortEnable)
		{

			sfSortByCoordXY(vvResultPoint,m_tmpSearchDataParam.m_dCoordSortGap,m_tmpSearchDataParam.m_nCoordSortMethod,
				m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);

			for(int k = 0 ; k < vResultArray.size() ; k++)
			{
				for(int j = 0; j < vvResultPoint.size() ; j++)
				{
					if(vvResultPoint.at(j).GetX() == vResultArray.at(k).GetX() && vvResultPoint.at(j).GetY() == vResultArray.at(k).GetY())
					{
						nBlobSortIndex[k] = j;
						break;
					}
				}
			}

		}


	}

	// 粗定位成功后
	if (bRet_Rough)
	{
		// 获取结果
		int nResultNum =0;
		int nSearchNumsLimit= 0;

		//获取粗定位搜索结果个数
		if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
		{
			nResultNum = m_PatternSearchTool_Rough.GetResultNum();
			nSearchNumsLimit = m_tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.m_nSearchNum;
		}
		else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
		{
			nResultNum = m_QuickSearchTool_Rough.GetResultNum();
			nSearchNumsLimit = m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_nSearchNum;
		}
		else if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool)
		{
			nResultNum = nResultBlobNum;
			nSearchNumsLimit = m_tmpSearchDataParam.m_Rough_BlobSearchTrainData.m_nBlobSearchNums;
		}

		if (nResultNum <= 0)
		{
			m_nResultErrNumber = -1;//粗定位失败
			return false;
		}

		double dRotation = 0.0;
		double dTransX =0.0;
		double dTransY =0.0;

		// 不进行旋转
		double dTransX2 =0.0;
		double dTransY2 =0.0;



		// 逐个结果进行遍历
		for (int i=0; i<nResultNum && i<nSearchNumsLimit && i<MAXRESNUM; i++)
		{
			CSearchResult searchResult;
			// 粗定位获取转换矩阵
			if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
			{
				scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool_Rough.GetResult(i);

				// 显示粗定位矩形和参考点
				ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect,i,&(tmpPatternResult.MatchRegion()),_T("_ResultRect_"),_T(""),RGB(255,153,18),m_tmpSearchGuiParam.m_nLineWidth);
				ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpPatternResult.GetPosition(),_T("_ResultCross_"),RGB(255,153,18),m_tmpSearchGuiParam.m_nLineWidth);

				// 求变换矩阵
				dRotation = 0.0;
				dTransX =0.0;
				dTransY =0.0;

				sc2Vector pos = tmpPatternResult.GetPosition();
				sc2Vector tmModelPoint;
				m_PatternSearchTool_Rough.GetPatternOrigin(tmModelPoint);

				// 对searchResult 初步赋值
				searchResult.m_dPosX = tmpPatternResult.GetPosition().GetX();
				searchResult.m_dPosY = tmpPatternResult.GetPosition().GetY();
				searchResult.m_dAngle = tmpPatternResult.GetRotate().ToDouble();
				searchResult.m_dScore = tmpPatternResult.GetScore();

				CResultSearchRect resRect;
				resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);

				dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
				dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
				dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());
				// 对搜索区进行调整
				dTransX2 = pos.GetX() - tmModelPoint.GetX();
				dTransY2 = pos.GetY() - tmModelPoint.GetY();
			}
			else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
			{
				scQuickSearchResult tmpQuickResult = m_QuickSearchTool_Rough.GetResult(i);

				// 显示粗定位
				ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect,i,&(tmpQuickResult.MatchRegion()),_T("_ResultRect_"),_T(""),RGB(255,153,18),m_tmpSearchGuiParam.m_nLineWidth);
				//显示粗定位参考点
				ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpQuickResult.GetPosition(),_T("_ResultCross_"),RGB(255,153,18),m_tmpSearchGuiParam.m_nLineWidth);

				// 求变换矩阵
				dRotation = 0;
				dTransX = 0;
				dTransY = 0;
				dTransX2 =0.0;
				dTransY2 =0.0;
				sc2Vector pos = tmpQuickResult.GetPosition();
				sc2Vector tmModelPoint;
				m_QuickSearchTool_Rough.GetModelOrigin(tmModelPoint);

				// 对searchResult 初步赋值
				searchResult.m_dPosX = tmpQuickResult.GetPosition().GetX();
				searchResult.m_dPosY = tmpQuickResult.GetPosition().GetY();
				searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
				searchResult.m_dScore = tmpQuickResult.GetScore();

				CResultSearchRect resRect;
				resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);

				dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
				dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
				dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());
				// 对搜索区进行调整
				dTransX2 = pos.GetX() - tmModelPoint.GetX();
				dTransY2 = pos.GetY() - tmModelPoint.GetY();

			}
			else if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool)
			{
				//pBSD->sort(eArea,eDescending);
				scBlob * const *pBlobB = pBSD->allBlobs(false);
				const scBlob* pChildBlob = pBSD->getBlob(nBlobSortIndex[i] +1); 
				if(pChildBlob == NULL)
				{
					m_tmpSearchGuiParam.m_bBlobVisible_Contour_Rough = false;
					m_nResultErrNumber = -4;
					return false;
				}
				scAffineRect affBlobRect;

				//affBlobRect.SetCenter(sc2Vector(pChildBlob->imageBoundCenterX,pChildBlob->imageBoundCenterY));
				//affBlobRect.SetXLength(pChildBlob->imageBoundWidth);
				//affBlobRect.SetYLength(pChildBlob->imageBoundHeight);

				affBlobRect.SetCenterLengthsRotAndSkew(sc2Vector(pChildBlob->imageBoundCenterX,pChildBlob->imageBoundCenterY),pChildBlob->boundPrincipalWidth,pChildBlob->boundPrincipalHeight,
					scRadian(scDegree(pChildBlob->angle)),scRadian(0));

				//affBlobRect.SetCenter(sc2Vector(pChildBlob->imageBoundCenterX, pChildBlob->imageBoundCenterY));
				//affBlobRect.SetXLength(pChildBlob->imageBoundWidth);
				//affBlobRect.SetYLength(pChildBlob->imageBoundHeight);
				//scRadian tmpsh= scRadian(scDegree(pChildBlob->angle).SignedNormMod180());
				//affBlobRect.SetXRotation(tmpsh);



				//显示轮廓
				CString strTmpKey = _T("");
				strTmpKey.Format(_T("%s_BlobResultBlobContour%d_Rough"), m_strKeyText,i);
				m_tmpSearchGuiParam.m_bBlobVisible_Contour_Rough = true;
				scGuiContour *tmpGuiContour = m_GuiSearchBlobContour_Rough + i;
				tmpGuiContour->SetContourBoundary(pBlobB,nResultBlobNum,true,true);
				//tmpGuiContour->SetTipTextEnabled(TRUE);
				tmpGuiContour->SetLineColor(RGB(0,255,255));
				//tmpGuiContour->SetTipText(strTmpKey);
				tmpGuiContour->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				//tmpGuiContour->SetLabel(strTmpKey);
				m_pGuiSearchBlobContour_Rough.push_back(tmpGuiContour);

				//显示外接矩形
				strTmpKey.Format(_T("Blob面积%.1f"),pChildBlob->area);
				ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect,i,&affBlobRect,_T("_ResultRect_"),strTmpKey,RGB(255,153,18),m_tmpSearchGuiParam.m_nLineWidth);

				CResultSearchRect resRect;
				resRect.m_searchRect = 	affBlobRect;
				searchResult.m_vTrainSearchRect.push_back(resRect);

				searchResult.m_dPosX = pChildBlob->imageBoundCenterX;
				searchResult.m_dPosY = pChildBlob->imageBoundCenterY;
				searchResult.m_dAngle =scDegree(pChildBlob->angle).ToDouble();
				// 求变换矩阵
				dRotation = 0;
				dTransX = 0;
				dTransY = 0;
				dTransX2 =0.0;
				dTransY2 =0.0;
				sc2Vector pos = affBlobRect.GetCenter();
				sc2Vector tmModelPoint;
				tmModelPoint.SetX(m_TrainResult.m_Rough_RectTrainResult.m_dModelX);
				tmModelPoint.SetY(m_TrainResult.m_Rough_RectTrainResult.m_dModelY);

				dRotation =  scRadian(scDegree(pChildBlob->angle)).ToDouble();
				//dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
				//dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());
				//// 对搜索区进行调整
				//dTransX2 = pos.GetX() - tmModelPoint.GetX();
				//dTransY2 = pos.GetY() - tmModelPoint.GetY();

				//double dTransModelX1, dTransModelY1;
				//dTransModelX1 = cos(dRotation) * m_TrainResult.m_Rough_RectTrainResult.m_dModelX1  - sin(dRotation) * m_TrainResult.m_Rough_RectTrainResult.m_dModelY1 + dTransX;
				//dTransModelY1 = sin(dRotation) * m_TrainResult.m_Rough_RectTrainResult.m_dModelX1  + cos(dRotation) * m_TrainResult.m_Rough_RectTrainResult.m_dModelY1 + dTransY;
				//searchResult.m_dPosX = dTransModelX1;
				//searchResult.m_dPosY = dTransModelY1;
		
				dTransX = m_TrainResult.m_Rough_RectTrainResult.m_dModelX1 -vvResultPoint[i].GetX();
				dTransY = m_TrainResult.m_Rough_RectTrainResult.m_dModelY1 -vvResultPoint[i].GetY();
				searchResult.m_dPosX = vvResultPoint[i].GetX();
				searchResult.m_dPosY = vvResultPoint[i].GetY();
			}



			// 是否启用精定位
			if (m_tmpTrainDataParam.m_bEnableAccurateLocation)
			{
				// 精定位采用矩形定位工具(区域矩形或几何矩形)
				if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternRectSearchTool  || m_tmpTrainDataParam.GetAccurateSearchToolType() ==eRectSearchTool)
				{

					// 找线工具参数赋值
					for (int it=0;it<LINE_NUMS;it++)
					{
						SetFindLineSearchToolParam(&m_FindLineTool_Accurate[it],m_TrainDataParam.m_FindLineCaliperTrainDataParam[it],m_TrainGuiParam.m_FindLineTrainGuiParam[it]);

					}

					// 找线工具结果数据定义
					scLineSeg ResLine[LINE_NUMS];
					double dRMS[LINE_NUMS];
					bool   bFindLine[LINE_NUMS];
					for (int k=0; k<LINE_NUMS; k++)
					{
						bFindLine[k] = 0;
					}

					// 求找线工具变换后的位置,找线
					for (int j=0; j<LINE_NUMS; j++)
					{

						double dTransStartX, dTransStartY;
						double dTransEndX, dTransEndY;
						dTransStartX = cos(dRotation) * m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dStartX  - sin(dRotation) * m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dStartY + dTransX;
						dTransStartY = sin(dRotation) * m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dStartX  + cos(dRotation) * m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dStartY + dTransY;
						dTransEndX = cos(dRotation) * m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dEndX  - sin(dRotation) * m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dEndY + dTransX;
						dTransEndY = sin(dRotation) * m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dEndX  + cos(dRotation) * m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dEndY + dTransY;
						m_FindLineTool_Accurate[j].SetCaliperParams(m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_nCaliperNum, 
							m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_CaliperSearchRotation, 
							m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dCaliperSearchLen,
							m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_dCaliperProjectLen);
						m_FindLineTool_Accurate[j].SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);
						m_FindLineTool_Accurate[j].SetCaliperMasked(m_TrainGuiParam.m_FindLineTrainGuiParam[j].m_vbCaliperMask_FindLine);

						if (m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[j].m_bEnableDualEdge)
						{
							EdgePolarity tmpEdgePolarity = eDontCare;
							if (m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[j].m_edgeProperty == eDarkToLight)
							{
								tmpEdgePolarity = eLightToDark;
							} 
							else if (m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[j].m_edgeProperty == eLightToDark)
							{
								tmpEdgePolarity = eDarkToLight;
							}
							m_FindLineTool_Accurate[j].SetCaliperProperty(m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[j].m_dContrastThre,
								m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[j].m_edgeProperty,tmpEdgePolarity,
								m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[j].m_lFilterHalfWidth);
						} 
						else
						{
							m_FindLineTool_Accurate[j].SetCaliperProperty(m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[j].m_dContrastThre,
								m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[j].m_edgeProperty,
								m_tmpTrainDataParam.m_FindLineCaliperTrainDataParam[j].m_lFilterHalfWidth);
						}
						// 执行找线
						if(bIsProcessImage)
						{
							bFindLine[j] = m_FindLineTool_Accurate[j].Execute(tmpImage, ResLine[j], dRMS[j]); // 成功
						}
						else
						{
							bFindLine[j] = m_FindLineTool_Accurate[j].Execute(ImageSearch, ResLine[j], dRMS[j]); // 成功
						}
						// 找线结果显示
						if (bFindLine[j])
						{
							// 显示找线
							ShowResultLine(m_pGuiSearchLineArray,m_GuiSearchLine,i*LINE_NUMS+j,&(ResLine[j].GetLine()),_T("_ResultLine_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

							//显示拟合点
							CString str = _T("");
							switch(j)
							{
							case 0:
								{
									str.Format(_T("%d_FitPoint_%d"),1,1);
									ShowFitPoint(m_pGuiSearchResult_Line_FitPointArray1,m_GuiSearchResult_Line_FitPoint1,&m_FindLineTool_Accurate[0],str);
								}
								break;
							case 1:
								{
									str.Format(_T("%d_FitPoint_%d"),2,2);
									ShowFitPoint(m_pGuiSearchResult_Line_FitPointArray2,m_GuiSearchResult_Line_FitPoint2,&m_FindLineTool_Accurate[1],str);
								}
								break;
							case 2:
								{
									str.Format(_T("%d_FitPoint_%d"),3,3);
									ShowFitPoint(m_pGuiSearchResult_Line_FitPointArray3,m_GuiSearchResult_Line_FitPoint3,&m_FindLineTool_Accurate[2],str);
								}
								break;
							case 3:
								{
									str.Format(_T("%d_FitPoint_%d"),4,4);
									ShowFitPoint(m_pGuiSearchResult_Line_FitPointArray4,m_GuiSearchResult_Line_FitPoint4,&m_FindLineTool_Accurate[3],str);
								}
								break;
							}
						}
						else
						{
							// 显示找线卡尺位置
							scLineSeg tmpScLineSeg;
							tmpScLineSeg.SetP1(sc2Vector(dTransStartX, dTransStartY));
							tmpScLineSeg.SetP2(sc2Vector(dTransEndX, dTransEndY));

							ShowResultLine(m_pGuiSearchLineArray,m_GuiSearchLine,i*LINE_NUMS+j,&(tmpScLineSeg.GetLine()),_T("_ResultLine_"),RGB(255,255,0),m_tmpSearchGuiParam.m_nLineWidth);


							m_nResultErrNumber = -11-j; // -j找线j失败;
							return false;
						}
					}

					bool bIntersect[4] = {false,false,false,false};
					sc2Vector vLineCrossPoint[4];

					// 求交点左上
					if (bFindLine[0] && bFindLine[2])
					{
						int nIntersectIndex = 0;
						std::vector<sc2Vector> crossPointArray; 
						if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[2].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
							&& crossPointArray.size() > 0)
						{
							vLineCrossPoint[nIntersectIndex] = crossPointArray[0];
							ShowResultCross(m_pGuiSearchLineCrossArray,m_GuiSearchLineCross,0+LINE_NUMS*i,crossPointArray[0],_T("_ResultLineCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
						}
					}

					// 求交点右上
					if (bFindLine[0] && bFindLine[3])
					{
						int nIntersectIndex = 1;
						std::vector<sc2Vector> crossPointArray; 
						if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[3].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
							&& crossPointArray.size() > 0)
						{
							vLineCrossPoint[nIntersectIndex] = crossPointArray[0];
							ShowResultCross(m_pGuiSearchLineCrossArray,m_GuiSearchLineCross,1+LINE_NUMS*i,crossPointArray[0],_T("_ResultLineCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
						}
					}


					// 求交点左下
					if (bFindLine[1] && bFindLine[2])
					{
						int nIntersectIndex = 2;
						std::vector<sc2Vector> crossPointArray; 
						if (sfInterSectLineLine(ResLine[1].GetLine(), ResLine[2].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
							&& crossPointArray.size() > 0)
						{
							vLineCrossPoint[nIntersectIndex] = crossPointArray[0];
							ShowResultCross(m_pGuiSearchLineCrossArray,m_GuiSearchLineCross,2+LINE_NUMS*i,crossPointArray[0],_T("_ResultLineCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
						}
					}

					// 求交点右下
					if (bFindLine[1] && bFindLine[3])
					{
						int nIntersectIndex = 3;
						std::vector<sc2Vector> crossPointArray; 
						if (sfInterSectLineLine(ResLine[1].GetLine(), ResLine[3].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
							&& crossPointArray.size() > 0)
						{
							vLineCrossPoint[nIntersectIndex] = crossPointArray[0];
							ShowResultCross(m_pGuiSearchLineCrossArray,m_GuiSearchLineCross,3+LINE_NUMS*i,crossPointArray[0],_T("_ResultLineCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
						}

					}

					// 如果4个交点都找到，将交点存入searchreasult辅助点中
					if (bIntersect[0] && bIntersect[1] && bIntersect[2] && bIntersect[3])
					{
						for (long k=0; k<LINE_NUMS; k++)
						{
							searchResult.m_vdAuxiliaryPosX[k] = vLineCrossPoint[k].GetX();
							searchResult.m_vdAuxiliaryPosY[k] = vLineCrossPoint[k].GetY();
							//searchResult.m_vdAuxiliaryAngle[k] = 0;
						}

						//重新构造点：过中心点，做长边角度的直线与短边相交的点；
						double dPosX = 0.0,dPosY = 0.0;
						double dCenterX = 0.0,dCenterY = 0.0/*, dAngel = 0.0, dAngelAuxiliary = 0.0*/;
						scRadian dAngel,dAngelAuxiliary;
						for (long m=0; m<RECT_LINE_NUM; m++)
						{
							dPosX = dPosX + vLineCrossPoint[m].GetX();
							dPosY = dPosY + vLineCrossPoint[m].GetY();
						}

						// 求矩形中点
						dCenterX = dPosX/4;
						dCenterY = dPosY/4;

						//求长边的角度
						double d1,d2;
						if(false == sfDistancePoint2Point(vLineCrossPoint[0],vLineCrossPoint[1],d1))
							return FALSE;
						if(false == sfDistancePoint2Point(vLineCrossPoint[0],vLineCrossPoint[2],d2))
							return FALSE;

						if(d1>d2)
						{
							scLineSeg lineSeg01(vLineCrossPoint[0],vLineCrossPoint[1]);
							scLineSeg lineSeg23(vLineCrossPoint[2],vLineCrossPoint[3]);

							scLine line01,line23;

							line01.SetFromLineSeg(lineSeg01);
							line23.SetFromLineSeg(lineSeg23);

							scLineSeg lineSeg13(vLineCrossPoint[1],vLineCrossPoint[3]);
							scLineSeg lineSeg02(vLineCrossPoint[0],vLineCrossPoint[2]);

							scLine line13,line02;

							line13.SetFromLineSeg(lineSeg13);
							line02.SetFromLineSeg(lineSeg02);

							dAngel = (line01.GetRotation().Norm() + line23.GetRotation().Norm()).Norm()/2;
							dAngelAuxiliary = (line13.GetRotation().Norm() + line02.GetRotation().Norm()).Norm()/2;
						}
						else
						{
							scLineSeg lineSeg13(vLineCrossPoint[1],vLineCrossPoint[3]);
							scLineSeg lineSeg02(vLineCrossPoint[0],vLineCrossPoint[2]);

							scLine line13,line02;

							line13.SetFromLineSeg(lineSeg13);
							line02.SetFromLineSeg(lineSeg02);

							scLineSeg lineSeg01(vLineCrossPoint[0],vLineCrossPoint[1]);
							scLineSeg lineSeg23(vLineCrossPoint[2],vLineCrossPoint[3]);

							scLine line01,line23;

							line01.SetFromLineSeg(lineSeg01);
							line23.SetFromLineSeg(lineSeg23);

							dAngel = (line13.GetRotation().Norm() + line02.GetRotation().Norm())/2;
							dAngelAuxiliary = (line01.GetRotation().Norm() + line23.GetRotation().Norm())/2;
						}

						// 根据长边角度及中心点构造新的直线
						scLine AcrossLine,AcrossLineAuxiliary;
						AcrossLine.SetPosRotation(sc2Vector(dCenterX,dCenterY),dAngel);
						AcrossLineAuxiliary.SetPosRotation(sc2Vector(dCenterX,dCenterY),dAngelAuxiliary);

						std::vector<sc2Vector> NewcrossPointArray;
						bool bIntersect = false;

						double RectX1,RectY1, RectX2,RectY2;
						RectX1 = vLineCrossPoint[0].GetX();
						RectY1 = vLineCrossPoint[0].GetY();
						RectX2 = vLineCrossPoint[3].GetX();
						RectY2 = vLineCrossPoint[3].GetY();

						// 存储矩形8个点
						sc2Vector Point_Cross[8];
						for (int itp=0;itp<8;itp++)
						{
							Point_Cross[itp].SetX(dCenterX);
							Point_Cross[itp].SetY(dCenterY);
						}
						///////////////////0----------1----------2//////////////////////////////////////
						///////////////////3					               4////////////////////////////////////
						///////////////////5----------6----------7/////////////////////////////////////
						// 上交点 1
						if (sfInterSectLineLine(ResLine[0].GetLine(), AcrossLine, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
						{
							if ((NewcrossPointArray[0].GetX() - RectX1)*(NewcrossPointArray[0].GetX() - RectX2)>0)
							{
								if (sfInterSectLineLine(ResLine[0].GetLine(), AcrossLineAuxiliary, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
								{
									Point_Cross[1] = NewcrossPointArray[0];
								}
							}
							else
							{
								Point_Cross[1] = NewcrossPointArray[0];

							}
						}		

						// 下交点 6
						if (sfInterSectLineLine(ResLine[1].GetLine(), AcrossLine, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
						{
							if ((NewcrossPointArray[0].GetX() - RectX1) * (NewcrossPointArray[0].GetX() - RectX2)>0)
							{
								if (sfInterSectLineLine(ResLine[1].GetLine(), AcrossLineAuxiliary, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
								{
									Point_Cross[6] = NewcrossPointArray[0];
								}
							}
							else
							{
								Point_Cross[6] = NewcrossPointArray[0];
							}
						}

						// 左交点3
						if (sfInterSectLineLine(ResLine[2].GetLine(), AcrossLine, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
						{
							if ((NewcrossPointArray[0].GetY() - vLineCrossPoint[0].GetY())*( NewcrossPointArray[0].GetY()- vLineCrossPoint[2].GetY())>0)
							{
								if (sfInterSectLineLine(ResLine[2].GetLine(), AcrossLineAuxiliary, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
								{
									Point_Cross[3] = NewcrossPointArray[0];

								}
							}
							else
							{
								Point_Cross[3] = NewcrossPointArray[0];

							}
						}
						// 右交点4
						if (sfInterSectLineLine(ResLine[3].GetLine(), AcrossLine, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
						{
							if ((NewcrossPointArray[0].GetY() -  vLineCrossPoint[1].GetY())* (NewcrossPointArray[0].GetY() -  vLineCrossPoint[3].GetY())>0)
							{
								if (sfInterSectLineLine(ResLine[3].GetLine(), AcrossLineAuxiliary, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
								{
									Point_Cross[4] = NewcrossPointArray[0];

								}
							}
							else
							{
								Point_Cross[4] = NewcrossPointArray[0];

							}
						}

						Point_Cross[0] = vLineCrossPoint[0];
						Point_Cross[2] = vLineCrossPoint[1];
						Point_Cross[5] = vLineCrossPoint[2];
						Point_Cross[7] = vLineCrossPoint[3];

						//searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
						//参考点1；
						switch(m_tmpSearchDataParam.m_nResultOutMode1)
						{
						case 8:	//输出中心点
							searchResult.m_dPosX	= dCenterX;
							searchResult.m_dPosY	= dCenterY;
							break;
						case 0: //输出上交点
							searchResult.m_dPosX	= Point_Cross[1].GetX();
							searchResult.m_dPosY	= Point_Cross[1].GetY();
							break;
						case 1: //输出下交点
							searchResult.m_dPosX	= Point_Cross[6].GetX();
							searchResult.m_dPosY	= Point_Cross[6].GetY();
							break;
						case 2: //输出左交点
							searchResult.m_dPosX	= Point_Cross[3].GetX();
							searchResult.m_dPosY	= Point_Cross[3].GetY();
							break;
						case 3: //输出右交点
							searchResult.m_dPosX	= Point_Cross[4].GetX();
							searchResult.m_dPosY	= Point_Cross[4].GetY();
							break;
						case 4://左上方
							searchResult.m_dPosX	= Point_Cross[0].GetX();
							searchResult.m_dPosY	= Point_Cross[0].GetY();
							break;
						case 5://右上方
							searchResult.m_dPosX	= Point_Cross[2].GetX();
							searchResult.m_dPosY	= Point_Cross[2].GetY();
							break;
						case 6://左下方
							searchResult.m_dPosX	= Point_Cross[5].GetX();
							searchResult.m_dPosY	= Point_Cross[5].GetY();
							break;
						case 7://右下方
							searchResult.m_dPosX	= Point_Cross[7].GetX();
							searchResult.m_dPosY	= Point_Cross[7].GetY();
							break;
						default:
							searchResult.m_dPosX	= dCenterX;
							searchResult.m_dPosY	= dCenterY;
							break;
						}
						// 参考点2
						searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
						switch(m_tmpSearchDataParam.m_nResultOutMode2)
						{
						case 8:	//输出中心点
							searchResult.m_vdAuxiliaryPosX.at(0) 	= dCenterX;
							searchResult.m_vdAuxiliaryPosY.at(0) 	= dCenterY;
							break;
						case 0: //输出上交点
							searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[1].GetX();
							searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[1].GetY();
							break;
						case 1: //输出下交点
							searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[6].GetX();
							searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[6].GetY();
							break;
						case 2: //输出左交点
							searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[3].GetX();
							searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[3].GetY();
							break;
						case 3: //输出右交点
							searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[4].GetX();
							searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[4].GetY();
							break;
						case 4://左上方
							searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[0].GetX();
							searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[0].GetY();
							break;
						case 5://右上方
							searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[2].GetX();
							searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[2].GetY();
							break;
						case 6://左下方
							searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[5].GetX();
							searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[5].GetY();
							break;
						case 7://右下方
							searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[7].GetX();
							searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[7].GetY();
							break;
						default:
							searchResult.m_vdAuxiliaryPosX.at(0)	= dCenterX;
							searchResult.m_vdAuxiliaryPosY.at(0)	= dCenterY;
							break;
						}

						for (int nLine = 0; nLine < RECT_LINE_NUM; nLine++)
						{
							CResultLine line;
							line.m_vLineX[0] = ResLine[nLine].GetP1().GetX();
							line.m_vLineY[0] = ResLine[nLine].GetP1().GetY();
							line.m_vLineX[1] = ResLine[nLine].GetP2().GetX();
							line.m_vLineY[1] = ResLine[nLine].GetP2().GetY();
							searchResult.m_vLineSearchResult.push_back(line);
						}

						//启用头尾检查

						bool bRet_HeadTail_Check = false;//头尾检查结果
						searchResult.m_resultSpecialPickHeadTail.bEnable = m_tmpTrainDataParam.m_bEnableHeadTail;
						if (m_tmpTrainDataParam.m_bEnableHeadTail)
						{
							if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
							{
								if(bIsProcessImage)
								{
									bRet_HeadTail_Check = ExecutePatternSerach(tmpImage,&m_PatternSearchTool_HeadTail,&(m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect),
										1,
										m_tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.m_dAcceptThre,
										m_tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.m_dConfusionThre,dTransX2,dTransY2,0);
								}
								else
								{
									bRet_HeadTail_Check = ExecutePatternSerach(ImageSearch,&m_PatternSearchTool_HeadTail,&(m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect),
										1,
										m_tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.m_dAcceptThre,
										m_tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.m_dConfusionThre,dTransX2,dTransY2,0);
								}
							}
							else if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
							{
								if(bIsProcessImage)
								{
									bRet_HeadTail_Check = ExecuteQuickSearch(tmpImage,&m_QuickSearchTool_HeadTail,&(m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect),
										1,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAcceptThre,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAngleLow,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAngleHigh,1,1,dTransX2,dTransY2,0);
								}
								else
								{
									bRet_HeadTail_Check = ExecuteQuickSearch(ImageSearch,&m_QuickSearchTool_HeadTail,&(m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect),
										1,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAcceptThre,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAngleLow,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAngleHigh,1,1,dTransX2,dTransY2,0);
								}
							}
							else if(m_tmpTrainDataParam.GetHeadTailCheckToolType() == eAreaInspectToolEx)
							{
								// 对矩形长边两侧的一定区域进行灰度分析；
								//double dAngel_tmp = (scDegree(searchResult.m_dAngle).SignedNormMod180()).ToDouble();
								double dAngel_tmp = (scDegree(searchResult.m_dAngle).SignedNorm()).ToDouble();
								//if(d1>d2)
								//{
								//	ExeHistogramHeadTail(Point_Cross[1].GetX(),Point_Cross[1].GetY(),dAngel_tmp,Point_Cross[6].GetX(),Point_Cross[6].GetY(),i,bRet_HeadTail_Check,ImageSearch);
								//}
								//else
								//{
								if(bIsProcessImage)
								{
									ExeHistogramHeadTail(Point_Cross[3].GetX(),Point_Cross[3].GetY(),dAngel_tmp,Point_Cross[4].GetX(),Point_Cross[4].GetY(),i,bRet_HeadTail_Check,tmpImage);
								}
								else
								{
									ExeHistogramHeadTail(Point_Cross[3].GetX(),Point_Cross[3].GetY(),dAngel_tmp,Point_Cross[4].GetX(),Point_Cross[4].GetY(),i,bRet_HeadTail_Check,ImageSearch);
								}
									
								//}
								
							}

							if (bRet_HeadTail_Check)
							{
								int n_HeadTailResultNum = 0;
								// 获取结果
								if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
								{
									n_HeadTailResultNum = m_PatternSearchTool_HeadTail.GetResultNum();
								}
								if(m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
								{
									n_HeadTailResultNum = m_QuickSearchTool_HeadTail.GetResultNum();
								}
								if(m_tmpTrainDataParam.GetHeadTailCheckToolType() == eAreaInspectToolEx)
								{
									n_HeadTailResultNum = 1;
								}

								if (n_HeadTailResultNum <= 0)
								{
									bRet_HeadTail_Check = false;
								}
								if(bRet_HeadTail_Check)
								{
									// 显示搜索GUI
									if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
									{
										scPatternSearchRotResult tmpPatternResult_HeadTail = m_PatternSearchTool_HeadTail.GetResult(0);
										ShowResultRect(m_pGuiSearchRectArray_HeadTail,m_GuiSearchAffineRect_HeadTail,i,&(tmpPatternResult_HeadTail.MatchRegion()),_T("_ResultRect_HeadTail_"),_T(""),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
										ShowResultCross(m_pGuiSearchCrossArray_HeadTail,m_GuiSearchCross_HeadTail,i,tmpPatternResult_HeadTail.GetPosition(),_T("_ResultCross_HeadTail_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

									}
									if(m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
									{
										scQuickSearchResult tmpQuickResult_HeadTail = m_QuickSearchTool_HeadTail.GetResult(0);

										// 显示粗定位
										ShowResultRect(m_pGuiSearchRectArray_HeadTail,m_GuiSearchAffineRect_HeadTail,i,&(tmpQuickResult_HeadTail.MatchRegion()),_T("_ResultRect_HeadTail_"),_T(""),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

										//ShowResultRect(m_pGuiSearchRectArray_HeadTail,m_GuiSearchAffineRect_HeadTail,i,&(tmpQuickResult_HeadTail.MatchRegion()),_T("_ResultRect_HeadTail_2"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
										//显示粗定位参考点
										ShowResultCross(m_pGuiSearchCrossArray_HeadTail,m_GuiSearchCross_HeadTail,i,tmpQuickResult_HeadTail.GetPosition(),_T("_ResultRect_HeadTail_2"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

									}
								}
							}
							else
							{
								if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool || m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
								{


									// 显示调整位置后的搜索区

									scAffineRect tmpshow;

									double dTransCenterX, dTransCenterY;
									double dOriCenterX, dOriCenterY;
									dOriCenterX = m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect.GetOrigin().GetX();
									dOriCenterY = m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect.GetOrigin().GetY();
									dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
									dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;

									scRect tmpSearch = m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect;
									tmpSearch.SetOrigion(sc2Vector(dTransCenterX,dTransCenterY));
									tmpshow.SetCenterLengthsRotAndSkew(tmpSearch.GetOrigin(),tmpSearch.GetWidth(),tmpSearch.GetHeight(),scRadian(0),scRadian(0));
									ShowResultRect(m_pGuiSearchRectArray_HeadTail,m_GuiSearchAffineRect_HeadTail,i,&tmpshow,_T("_ResultRect_HeadTail_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);


									m_nResultErrNumber = -40; // 头尾检查失败
								}
							}
							// 头尾检查失败时处理
							if(false == bRet_HeadTail_Check)
							{
								if(m_tmpSearchDataParam.m_nHeadTail_FailType==0)  // 直接失败
								{
									searchResult.m_resultSpecialPickHeadTail.nResultCode = -1;

									return false;
								}
								else if (m_tmpSearchDataParam.m_nHeadTail_FailType==1) //
								{
									// 定位成功PLC拾取后旋转180度
									searchResult.m_resultSpecialPickHeadTail.nResultCode = 1;
								}
								else if (m_tmpSearchDataParam.m_nHeadTail_FailType==2)
								{
									//定位成功PLC先旋转180度再拾取
									searchResult.m_resultSpecialPickHeadTail.nResultCode  = 2; // 标识，头尾检查结果

									// 确保选择不同角度基准时准确；
									searchResult.m_dAngle +=180;

									// 处理方式，将返回点的180度对应点进行返回 仅当返回点为 上下左右的点时 中心点不起左右
									//参考点1
									switch(m_tmpSearchDataParam.m_nResultOutMode1)
									{
									case 0: //输出上交点
										searchResult.m_dPosX	= Point_Cross[6].GetX();
										searchResult.m_dPosY	= Point_Cross[6].GetY();
										break;
									case 1: //输出下交点
										searchResult.m_dPosX	= Point_Cross[1].GetX();
										searchResult.m_dPosY	= Point_Cross[1].GetY();
										break;
									case 2: //输出左交点
										searchResult.m_dPosX	= Point_Cross[4].GetX();
										searchResult.m_dPosY	= Point_Cross[4].GetY();
										break;
									case 3: //输出右交点
										searchResult.m_dPosX	= Point_Cross[3].GetX();
										searchResult.m_dPosY	= Point_Cross[3].GetY();
										break;
									case 4://左上方
										searchResult.m_dPosX	= Point_Cross[7].GetX();
										searchResult.m_dPosY	= Point_Cross[7].GetY();
										break;
									case 5://右上方
										searchResult.m_dPosX	= Point_Cross[5].GetX();
										searchResult.m_dPosY	= Point_Cross[5].GetY();
										break;
									case 6://左下方
										searchResult.m_dPosX	= Point_Cross[2].GetX();
										searchResult.m_dPosY	= Point_Cross[2].GetY();
										break;
									case 7://右下方
										searchResult.m_dPosX	= Point_Cross[0].GetX();
										searchResult.m_dPosY	= Point_Cross[0].GetY();
										break;
									default:
										searchResult.m_dPosX	= dCenterX;
										searchResult.m_dPosY	= dCenterY;

										break;
									}

									//参考点2
									switch(m_tmpSearchDataParam.m_nResultOutMode2)
									{
									case 0: //输出上交点
										searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[6].GetX();
										searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[6].GetY();
										break;
									case 1: //输出下交点
										searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[1].GetX();
										searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[1].GetY();
										break;
									case 2: //输出左交点
										searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[4].GetX();
										searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[4].GetY();
										break;
									case 3: //输出右交点
										searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[3].GetX();
										searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[3].GetY();
										break;
									case 4://左上方
										searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[7].GetX();
										searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[7].GetY();
										break;
									case 5://右上方
										searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[5].GetX();
										searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[5].GetY();
										break;
									case 6://左下方
										searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[2].GetX();
										searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[2].GetY();
										break;
									case 7://右下方
										searchResult.m_vdAuxiliaryPosX.at(0)	= Point_Cross[0].GetX();
										searchResult.m_vdAuxiliaryPosY.at(0)	= Point_Cross[0].GetY();
										break;
									default:
										searchResult.m_vdAuxiliaryPosX.at(0)	= dCenterX;
										searchResult.m_vdAuxiliaryPosY.at(0)	= dCenterY;

										break;
									}

								}
							}

						}
						else
						{
							bRet_HeadTail_Check = true;
						}



						//m_SearchResultArray.push_back(searchResult);

					}
					else
					{
						// 找线交点失败
						m_nResultErrNumber = -15;
						return false;
					}
				}
				else if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternBlobSearchTool)
				{
					scAffineRect affBlobRect;
					double dTransCenterX, dTransCenterY;
					double dOriCenterX, dOriCenterY;
					dOriCenterX = m_TrainGuiParam.m_BlobAffineRect.GetCenter().GetX();
					dOriCenterY = m_TrainGuiParam.m_BlobAffineRect.GetCenter().GetY();
					dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
					dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;

					scAffineRect affRect;
					affRect.SetCenter(sc2Vector(dTransCenterX,dTransCenterY));
					affRect.SetXLength(m_TrainGuiParam.m_BlobAffineRect.GetXLength());
					affRect.SetYLength(m_TrainGuiParam.m_BlobAffineRect.GetYLength());
					affRect.SetXRotation(m_TrainGuiParam.m_BlobAffineRect.GetXRotation() + scRadian(dRotation));
					affRect.SetSkew(m_TrainGuiParam.m_BlobAffineRect.GetSkew());

					m_BlobTool.SetSegmentationHardThresh(m_tmpSearchDataParam.m_lBlobGrayThre,m_tmpSearchDataParam.m_bBlobMode);
					m_BlobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
					m_BlobTool.ConnectivityCleanup(scBlobSceneDescription::ePrune);
					m_BlobTool.ConnectivityMinPels(m_tmpSearchDataParam.m_lConnectivityMinPels);
					m_BlobTool.EnableComputeInertia(true);
					bool bBlobExecute = false;
					if(bIsProcessImage)
					{
						bBlobExecute = m_BlobTool.Execute(tmpImage, &affRect);
					}
					else
					{
						bBlobExecute = m_BlobTool.Execute(ImageSearch, &affRect);
					}
					if (bBlobExecute)
					{                       

						scBlobSceneDescription* pBSD = m_BlobTool.GetBSD();
						if (pBSD == NULL)
						{
							m_nResultErrNumber = -21; // Blob分析计算失败
							return false;
						}

						pBSD->filter(eArea,m_tmpSearchDataParam.m_lBlobAreaMinThre,m_tmpSearchDataParam.m_lBlobAreaMaxThre,0);
						pBSD->sort(eArea,eDescending);
						//scBlob * const *pBlobB = pBSD->allBlobs(false);
						scBlob * const *pBlobB = pBSD->allBlobs(true);
						int nResultBlobNum = pBSD->numBlobs(true); 

						if (pBSD->numBlobs() < 1)
						{                   
							m_nResultErrNumber = -22; // Blob分析个数失败
							m_tmpSearchGuiParam.m_bBlobVisible_Contour = false;
							return false;
						}
						CString strTmpKey = _T("");

						//m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
						//m_GuiContour.SetTipTextEnabled(TRUE);
						strTmpKey.Format(_T("%s_BlobResultBlobContour%d"), m_strKeyText,i);

						m_tmpSearchGuiParam.m_bBlobVisible_Contour = true;
						scGuiContour *tmpGuiContour = m_GuiSearchBlobContour + i;
						tmpGuiContour->SetContourBoundary(pBlobB,nResultBlobNum,true,true);
						tmpGuiContour->SetTipTextEnabled(TRUE);
						tmpGuiContour->SetLineColor(RGB(0,255,255));
						tmpGuiContour->SetTipText(strTmpKey);
						tmpGuiContour->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						m_pGuiSearchBlobContour.push_back(tmpGuiContour);

						// 主轴最小外接矩形
						const scBlob* pBlob = pBSD->getBlob(1);


						//affBlobRect.SetCenter(sc2Vector(pBlob->boundPrincipalCenterX, pBlob->boundPrincipalCenterY));
						//affBlobRect.SetXLength(pBlob->boundPrincipalWidth);
						//affBlobRect.SetYLength(pBlob->boundPrincipalHeight);
						//affBlobRect.SetXRotation(scRadian(scDegree(pBlob->angle)));
						//affBlobRect.SetSkew(m_TrainGuiParam.m_BlobAffineRect.GetSkew());
						//affBlobRect.SetSkew(scRadian(0));

						affBlobRect.SetCenterLengthsRotAndSkew(sc2Vector(pBlob->imageBoundCenterX,pBlob->imageBoundCenterY),pBlob->boundPrincipalWidth,pBlob->boundPrincipalHeight,
							scRadian(scDegree(pBlob->angle)),scRadian(0));

						double dCx = pBlob->boundPrincipalCenterX;
						double dCy = pBlob->boundPrincipalCenterY;
						double dH = pBlob->boundPrincipalHeight;
						double dW = pBlob->boundPrincipalWidth;
						double dA = (scRadian(scDegree(pBlob->angle)) -m_TrainGuiParam.m_BlobAffineRect.GetSkew()).ToDouble();
						scLineSeg lineSegAngle(affBlobRect.GetCornerPo(),affBlobRect.GetCornerPx());
						searchResult.m_dAngle = scDegree(lineSegAngle.GetRotation()).ToDouble();
						// 参考点1
						switch (m_tmpSearchDataParam.m_nBlobOutMode1)
						{
						case 0:
							searchResult.m_dPosX = pBlob->boundPrincipalCenterX;
							searchResult.m_dPosY = pBlob->boundPrincipalCenterY;
							break;
						case 1:
							searchResult.m_dPosX = pBlob->centerMassX;
							searchResult.m_dPosY = pBlob->centerMassY;
							break;
						case 2:// 左上角
							{

								// 外接矩形点
								//searchResult.m_dPosX = dCx -dW/2*cos(dA)-dH/2*sin(dA);
								//searchResult.m_dPosY = dCy +dW/2*sin(dA)-dH/2*cos(dA);
								searchResult.m_dPosX = affBlobRect.GetCornerPo().GetX();
								searchResult.m_dPosY = affBlobRect.GetCornerPo().GetY();
							}
							break;
						case 3:// 右上角
							{
								//searchResult.m_dPosX = dCx +dW/2*cos(dA)-dH/2*sin(dA);
								//searchResult.m_dPosY = dCy -dW/2*sin(dA)-dH/2*cos(dA);
								searchResult.m_dPosX = affBlobRect.GetCornerPx().GetX();
								searchResult.m_dPosY = affBlobRect.GetCornerPx().GetY();
							}
							break;
						case 4:// 左下角
							{
								//searchResult.m_dPosX = dCx -dW/2*cos(dA)+dH/2*sin(dA);
								//searchResult.m_dPosY = dCy +dW/2*sin(dA)+dH/2*cos(dA);
								searchResult.m_dPosX = affBlobRect.GetCornerPy().GetX();
								searchResult.m_dPosY = affBlobRect.GetCornerPy().GetY();
							}
							break;
						case 5:// 右下角
							{
								//searchResult.m_dPosX = dCx +dW/2*cos(dA)+dH/2*sin(dA);
								//searchResult.m_dPosY = dCy -dW/2*sin(dA)+dH/2*cos(dA);
								searchResult.m_dPosX = affBlobRect.GetCornerPopp().GetX();
								searchResult.m_dPosY = affBlobRect.GetCornerPopp().GetY();
							}
							break;
						default:
							searchResult.m_dPosX = pBlob->boundPrincipalCenterX;
							searchResult.m_dPosY = pBlob->boundPrincipalCenterY;
							break;
						}

						// 参考点2
						searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
						switch (m_tmpSearchDataParam.m_nBlobOutMode2)
						{
						case 0:
							searchResult.m_vdAuxiliaryPosX.at(0)= pBlob->boundPrincipalCenterX;
							searchResult.m_vdAuxiliaryPosY.at(0)= pBlob->boundPrincipalCenterY;
							break;
						case 1:
							searchResult.m_vdAuxiliaryPosX.at(0) = pBlob->centerMassX;
							searchResult.m_vdAuxiliaryPosY.at(0) = pBlob->centerMassY;
							break;
						case 2:// 左上角
							{

								// 外接矩形点
								//searchResult.m_dPosX = dCx -dW/2*cos(dA)-dH/2*sin(dA);
								//searchResult.m_dPosY = dCy +dW/2*sin(dA)-dH/2*cos(dA);
								searchResult.m_vdAuxiliaryPosX.at(0) = affBlobRect.GetCornerPo().GetX();
								searchResult.m_vdAuxiliaryPosY.at(0) = affBlobRect.GetCornerPo().GetY();
							}
							break;
						case 3:// 右上角
							{
								//searchResult.m_dPosX = dCx +dW/2*cos(dA)-dH/2*sin(dA);
								//searchResult.m_dPosY = dCy -dW/2*sin(dA)-dH/2*cos(dA);
								searchResult.m_vdAuxiliaryPosX.at(0) = affBlobRect.GetCornerPx().GetX();
								searchResult.m_vdAuxiliaryPosY.at(0) = affBlobRect.GetCornerPx().GetY();
							}
							break;
						case 4:// 左下角
							{
								//searchResult.m_dPosX = dCx -dW/2*cos(dA)+dH/2*sin(dA);
								//searchResult.m_dPosY = dCy +dW/2*sin(dA)+dH/2*cos(dA);
								searchResult.m_vdAuxiliaryPosX.at(0) = affBlobRect.GetCornerPy().GetX();
								searchResult.m_vdAuxiliaryPosY.at(0) = affBlobRect.GetCornerPy().GetY();
							}
							break;
						case 5:// 右下角
							{
								//searchResult.m_dPosX = dCx +dW/2*cos(dA)+dH/2*sin(dA);
								//searchResult.m_dPosY = dCy -dW/2*sin(dA)+dH/2*cos(dA);
								searchResult.m_vdAuxiliaryPosX.at(0) = affBlobRect.GetCornerPopp().GetX();
								searchResult.m_vdAuxiliaryPosY.at(0) = affBlobRect.GetCornerPopp().GetY();
							}
							break;
						default:
							searchResult.m_vdAuxiliaryPosX.at(0) = pBlob->boundPrincipalCenterX;
							searchResult.m_vdAuxiliaryPosY.at(0) = pBlob->boundPrincipalCenterY;
							break;
						}

						CResultSearchRect resRect;
						resRect.m_bFindRect = TRUE;
						resRect.m_searchRect = affBlobRect;
						searchResult.m_vACFSearchRect.push_back(resRect);

						// 2.Blob仿射矩形
						strTmpKey.Format(_T("%s_BlobResultAffineRect%d"), m_strKeyText,i);
						scGuiAffineRect *pTemAffineRect = m_GuiSearchAffineRectBlob+i;
						pTemAffineRect->SetAffineRect(affBlobRect);
						pTemAffineRect->SetLineColor(RGB(0,0,255));
						pTemAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTemAffineRect->SetTipTextEnabled(true);
						pTemAffineRect->SetTipText(strTmpKey);
						m_pGuiSearchBlobArray.push_back(pTemAffineRect);


						// 头尾检查处理
						//启用头尾检查

						bool bRet_HeadTail_Check = false;//头尾检查结果
						searchResult.m_resultSpecialPickHeadTail.bEnable = m_tmpTrainDataParam.m_bEnableHeadTail;
						if (m_tmpTrainDataParam.m_bEnableHeadTail)
						{
							if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
							{
								if(bIsProcessImage)
								{
									bRet_HeadTail_Check = ExecutePatternSerach(tmpImage,&m_PatternSearchTool_HeadTail,&(m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect),
										1,
										m_tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.m_dAcceptThre,
										m_tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.m_dConfusionThre,dTransX,dTransY,dRotation);
								}
								else
								{
									bRet_HeadTail_Check = ExecutePatternSerach(ImageSearch,&m_PatternSearchTool_HeadTail,&(m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect),
										1,
										m_tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.m_dAcceptThre,
										m_tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.m_dConfusionThre,dTransX,dTransY,dRotation);
								}
							}
							else if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
							{
								if(bIsProcessImage)
								{
									bRet_HeadTail_Check = ExecuteQuickSearch(tmpImage,&m_QuickSearchTool_HeadTail,&(m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect),
										1,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAcceptThre,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAngleLow,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAngleHigh,1,1,dTransX,dTransY,dRotation);
								}
								else
								{
									bRet_HeadTail_Check = ExecuteQuickSearch(ImageSearch,&m_QuickSearchTool_HeadTail,&(m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect),
										1,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAcceptThre,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAngleLow,
										m_tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.m_dAngleHigh,1,1,dTransX,dTransY,dRotation);
								}
							}
							else if(m_tmpTrainDataParam.GetHeadTailCheckToolType() == eAreaInspectToolEx)
							{
								// 对矩形长边两侧的一定区域进行灰度分析
								double dAngel_tmp = (scDegree(searchResult.m_dAngle).SignedNorm()).ToDouble();
								if(bIsProcessImage)
								{
									ExeHistogramHeadTail(affBlobRect.GetCornerPo().GetX(),affBlobRect.GetCornerPo().GetY(),dAngel_tmp,affBlobRect.GetCornerPx().GetX(),affBlobRect.GetCornerPx().GetY(),i,bRet_HeadTail_Check,tmpImage);
								}
								else
								{
									ExeHistogramHeadTail(affBlobRect.GetCornerPo().GetX(),affBlobRect.GetCornerPo().GetY(),dAngel_tmp,affBlobRect.GetCornerPx().GetX(),affBlobRect.GetCornerPx().GetY(),i,bRet_HeadTail_Check,ImageSearch);
								}
							}

							if (bRet_HeadTail_Check)
							{
								int n_HeadTailResultNum = 0;
								// 获取结果
								if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
								{
									n_HeadTailResultNum = m_PatternSearchTool_HeadTail.GetResultNum();
								}
								if(m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
								{
									n_HeadTailResultNum = m_QuickSearchTool_HeadTail.GetResultNum();
								}
								else if(m_tmpTrainDataParam.GetHeadTailCheckToolType() == eAreaInspectToolEx)
								{
									n_HeadTailResultNum = 1;
								}

								if (n_HeadTailResultNum <= 0)
								{
									bRet_HeadTail_Check = false;
								}

								if(bRet_HeadTail_Check)
								{
									if (m_tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool)
									{
										scPatternSearchRotResult tmpPatternResult_HeadTail = m_PatternSearchTool_HeadTail.GetResult(0);
										ShowResultRect(m_pGuiSearchRectArray_HeadTail,m_GuiSearchAffineRect_HeadTail,i,&(tmpPatternResult_HeadTail.MatchRegion()),_T("_ResultRect_HeadTail_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
										ShowResultCross(m_pGuiSearchCrossArray_HeadTail,m_GuiSearchCross_HeadTail,i,tmpPatternResult_HeadTail.GetPosition(),_T("_ResultCross_HeadTail_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

									}
									if(m_tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool)
									{
										scQuickSearchResult tmpQuickResult_HeadTail = m_QuickSearchTool_HeadTail.GetResult(0);

										// 显示粗定位
										ShowResultRect(m_pGuiSearchRectArray_HeadTail,m_GuiSearchAffineRect_HeadTail,i,&(tmpQuickResult_HeadTail.MatchRegion()),_T("_ResultRect_HeadTail_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
										//显示粗定位参考点
										ShowResultCross(m_pGuiSearchCrossArray_HeadTail,m_GuiSearchCross_HeadTail,i,tmpQuickResult_HeadTail.GetPosition(),_T("_ResultRect_HeadTail_2"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

									}
								}
							}
							else
							{
								m_nResultErrNumber = -40;
							}

							// 头尾检查失败时处理
							if(false == bRet_HeadTail_Check)
							{
								if(m_tmpSearchDataParam.m_nHeadTail_FailType==0)  // 直接失败
								{
									searchResult.m_resultSpecialPickHeadTail.nResultCode = -1;
									return false;
								}
								else if (m_tmpSearchDataParam.m_nHeadTail_FailType==1) //
								{
									// 定位成功PLC拾取后旋转180度
									searchResult.m_resultSpecialPickHeadTail.nResultCode = 1;
								}
								else if (m_tmpSearchDataParam.m_nHeadTail_FailType==2)
								{
									//定位成功PLC先旋转180度再拾取
									searchResult.m_resultSpecialPickHeadTail.nResultCode  = 2; // 标识，头尾检查结果
									searchResult.m_dAngle +=180;
									// 处理方式，将返回点的180度对应点进行返回 仅当返回点为 上下左右的点时 中心点不起左右
									switch (m_tmpSearchDataParam.m_nBlobOutMode1)
									{
									case 0:
										searchResult.m_dPosX = pBlob->boundPrincipalCenterX;
										searchResult.m_dPosY = pBlob->boundPrincipalCenterY;
										break;
									case 1:
										searchResult.m_dPosX = pBlob->centerMassX;
										searchResult.m_dPosY = pBlob->centerMassY;
										break;
									case 2:// 左上角
										{
											// 头尾处理 范围右下角
											searchResult.m_dPosX = affBlobRect.GetCornerPopp().GetX();
											searchResult.m_dPosY = affBlobRect.GetCornerPopp().GetY();

											//searchResult.m_dPosX = affBlobRect.GetCornerPo().GetX();
											//searchResult.m_dPosY = affBlobRect.GetCornerPo().GetY();
										}
										break;
									case 3:// 右上角
										{
											// 头尾处理 范围左下角
											searchResult.m_dPosX = affBlobRect.GetCornerPy().GetX();
											searchResult.m_dPosY = affBlobRect.GetCornerPy().GetY();

											/*									searchResult.m_dPosX = affBlobRect.GetCornerPx().GetX();
											searchResult.m_dPosY = affBlobRect.GetCornerPx().GetY();*/
										}
										break;
									case 4:// 左下角
										{
											// 头尾处理 范围右上角
											searchResult.m_dPosX = affBlobRect.GetCornerPx().GetX();
											searchResult.m_dPosY = affBlobRect.GetCornerPx().GetY();

											//searchResult.m_dPosX = affBlobRect.GetCornerPy().GetX();
											//searchResult.m_dPosY = affBlobRect.GetCornerPy().GetY();

										}
										break;
									case 5:// 右下角
										{
											// 头尾处理 范围左上角
											searchResult.m_dPosX = affBlobRect.GetCornerPo().GetX();
											searchResult.m_dPosY = affBlobRect.GetCornerPo().GetY();
											//searchResult.m_dPosX = affBlobRect.GetCornerPopp().GetX();
											//searchResult.m_dPosY = affBlobRect.GetCornerPopp().GetY();
										}
										break;
									default:
										searchResult.m_dPosX = pBlob->boundPrincipalCenterX;
										searchResult.m_dPosY = pBlob->boundPrincipalCenterY;
										break;
									}

									switch (m_tmpSearchDataParam.m_nBlobOutMode2)
									{
									case 0:
										searchResult.m_vdAuxiliaryPosX.at(0) = pBlob->boundPrincipalCenterX;
										searchResult.m_vdAuxiliaryPosY.at(0) = pBlob->boundPrincipalCenterY;
										break;
									case 1:
										searchResult.m_vdAuxiliaryPosX.at(0) = pBlob->centerMassX;
										searchResult.m_vdAuxiliaryPosY.at(0) = pBlob->centerMassY;
										break;
									case 2:// 左上角
										{
											// 头尾处理 范围右下角
											searchResult.m_vdAuxiliaryPosX.at(0) = affBlobRect.GetCornerPopp().GetX();
											searchResult.m_vdAuxiliaryPosY.at(0) = affBlobRect.GetCornerPopp().GetY();
										}
										break;
									case 3:// 右上角
										{
											// 头尾处理 范围左下角
											searchResult.m_vdAuxiliaryPosX.at(0) = affBlobRect.GetCornerPy().GetX();
											searchResult.m_vdAuxiliaryPosY.at(0) = affBlobRect.GetCornerPy().GetY();
										}
										break;
									case 4:// 左下角
										{
											// 头尾处理 范围右上角
											searchResult.m_vdAuxiliaryPosX.at(0) = affBlobRect.GetCornerPx().GetX();
											searchResult.m_vdAuxiliaryPosY.at(0) = affBlobRect.GetCornerPx().GetY();
										}
										break;
									case 5:// 右下角
										{
											// 头尾处理 范围左上角
											searchResult.m_vdAuxiliaryPosX.at(0) = affBlobRect.GetCornerPo().GetX();
											searchResult.m_vdAuxiliaryPosY.at(0) = affBlobRect.GetCornerPo().GetY();
										}
										break;
									default:
										searchResult.m_vdAuxiliaryPosX.at(0) = pBlob->boundPrincipalCenterX;
										searchResult.m_vdAuxiliaryPosY.at(0) = pBlob->boundPrincipalCenterY;
										break;
									}
								}
							}


							// 显示最后输出点
							/*strTmpKey.Format(_T("%s_BlobResultCenterMass%d"), m_strKeyText, i);
							scGuiCross *pTempCross = m_GuiSearchCross_Blob+i;
							pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX, searchResult.m_dPosY), sc2Vector(40,40));
							pTempCross->SetLineColor(RGB(255, 0, 0));
							pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempCross->SetTipTextEnabled(true);
							pTempCross->SetTipText(strTmpKey);
							CString st;
							st.Format(_T("返回点%d X:%.1f;Y:%.1f"), i, searchResult.m_dPosX, searchResult.m_dPosY);
							pTempCross->SetLabel(st);
							pTempCross->SetLabelVisible(TRUE);
							m_pGuiSearchCrossArray_Blob.push_back(pTempCross);*/
						}
						else
						{
							bRet_HeadTail_Check = true;
						}


					}
					else
					{
						m_tmpSearchGuiParam.m_bBlobVisible_Contour = false;
						m_nResultErrNumber = -20;
						return false;
					}
				}
				else if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eMPatternSearchTool)
				{
					// 多标记点
					double dTransModelX, dTransModelY;
					if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool)
					{
						dTransModelX = cos(dRotation) *(m_TrainResult.m_Rough_RectTrainResult.m_dModelX2 -dTransX - searchResult.m_dPosX)-sin(dRotation) * (m_TrainResult.m_Rough_RectTrainResult.m_dModelY2 -dTransY - searchResult.m_dPosY) +searchResult.m_dPosX;
						dTransModelY = sin(dRotation) *(m_TrainResult.m_Rough_RectTrainResult.m_dModelX2 -dTransX - searchResult.m_dPosX)+cos(dRotation) * (m_TrainResult.m_Rough_RectTrainResult.m_dModelY2 -dTransY - searchResult.m_dPosY) +searchResult.m_dPosY;
					}
					else
					{
						dTransModelX = cos(dRotation) * m_TrainResult.m_Rough_RectTrainResult.m_dModelX2  - sin(dRotation) * m_TrainResult.m_Rough_RectTrainResult.m_dModelY2 + dTransX;
						dTransModelY = sin(dRotation) * m_TrainResult.m_Rough_RectTrainResult.m_dModelX2  + cos(dRotation) * m_TrainResult.m_Rough_RectTrainResult.m_dModelY2 + dTransY;
					}
					
					searchResult.m_vdAuxiliaryPosX.at(0) = dTransModelX;
					searchResult.m_vdAuxiliaryPosY.at(0) = dTransModelY;
					searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
				}

			}
			else // 不使用精定位
			{


			}

			//显示最终输出 参考点1
			sc2Vector Result_Point(searchResult.m_dPosX,searchResult.m_dPosY);
			CString StrResult_Point = _T("");

			int ntmpShow = i;

			//// 如果采用Blob作为粗定位
			//if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool && m_tmpSearchDataParam.m_bCoordSortEnable)
			//{
			//	ntmpShow = nBlobSortIndex[i];
			//}


			StrResult_Point.Format(_T("Mark%d_1;X:%1.f;Y:%1.f;"),ntmpShow,searchResult.m_dPosX,searchResult.m_dPosY);
			ShowResultCross(m_pGuiSearchLineCrossArray2,m_GuiSearchLineCross2,i,Result_Point,_T("_ResultPointCross_"),StrResult_Point,RGB(255,0,0),m_tmpSearchGuiParam.m_nLineWidth);

			// 参考点2
			sc2Vector Result_Point2(searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0));
			StrResult_Point.Format(_T("Mark%d_2;X:%1.f;Y:%1.f;"),ntmpShow,searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0));
			ShowResultCross(m_pGuiSearchLineCrossArray3,m_GuiSearchLineCross3,i,Result_Point2,_T("_ResultPointCross2_"),StrResult_Point,RGB(0,0,255),m_tmpSearchGuiParam.m_nLineWidth);

			m_SearchResultArray.push_back(searchResult);

		} // for (int i=0; i<nResultNum && i<MAXRESNUM; i++)---End



		if (m_SearchResultArray.size() <= 0)
		{
			return false;
		}
		m_TrainDataParam = m_tmpTrainDataParam;
		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
		return true;
	}
	else
	{
		m_nResultErrNumber = -1;  //粗定位失败
		return false;
	}
}
// 获取搜索结果个数										
int CSpecialPickSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CSpecialPickSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	


// 加载模板训练图像
bool CSpecialPickSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
{
	// strDir为文件夹路径
	// 仅配置ModelImage.bmp
	if (strDir == _T(""))
	{
		return false;
	}
	if (strDir.GetAt(strDir.GetLength()-1) == _T('\\'))
	{
		strDir.Delete(strDir.GetLength()-1);
	}
	DWORD dwRet = GetFileAttributes(strDir);
	if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
	{
		return false;
	}
	strDir += _T('\\');

	//////////////////////////读xml文件//////////////////////////
	CString strXmlPath = strDir + _T("Model.xml");
	vcXMLConfigurator cXMLConfigurator;
	sc2Vector vPos;
	scRect rRect;
	scRect rRect2;
	bool bRet;
	CSpecialPickSearchTrainDataParam tmpTrainDataParam;
	if (!cXMLConfigurator.Load(strXmlPath))
	{
		return false;
	}

	bRet = cXMLConfigurator.FindElem(_T("Config"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

	/////////////工具类型/////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("ToolType"));
	if (!bRet)
	{
		return false;
	}
	CSearchToolType cSearchToolType = (CSearchToolType)_ttoi(cXMLConfigurator.GetElemData());
	if (cSearchToolType != eSpecialPickSearchTool)
	{
		return false;
	}

	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("TrainParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	// 粗定位训练参数
	bRet = cXMLConfigurator.FindElem(_T("RoughSearchToolType"));
	if (bRet)
	{
		tmpTrainDataParam.SetRoughSearchToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
	}
	cXMLConfigurator.LeaveElem();

	bRet = cXMLConfigurator.FindElem(_T("TrainGuiParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("Rough_TrainGuiParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	// Train rect
	bRet = cXMLConfigurator.FindElem(_T("TrainRect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("OriginX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("OriginY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	rRect.SetOrigion(vPos);
	bRet = cXMLConfigurator.FindElem(_T("SizeX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("SizeY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	rRect.SetSize(vPos);
	cXMLConfigurator.LeaveElem();
	// train affine rect
	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (!bRet)
	{
		return false;
	}
	rRect2.SetWidth(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (!bRet)
	{
		return false;
	}
	rRect2.SetHeight(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("CenterX"));
	if (!bRet)
	{
		return false;
	}	
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("CenterY"));
	if (!bRet)
	{
		return false;
	}	
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	rRect2.SetOrigion(sc2Vector(vPos.GetX()-rRect.GetSize().GetX()/2,vPos.GetY()-rRect.GetSize().GetY()/2));



	cXMLConfigurator.LeaveElem();
	cXMLConfigurator.LeaveElem();
	cXMLConfigurator.LeaveElem();
	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpImageModel;
	hStatus = cDIB.Init(strDir + _T("Model.bmp"));
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	hStatus = cDIB.GetImage(tmpImageModel);
	if(tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool )
	{
		if (!IsSuccess(hStatus))
		{
			return false;
		}
		if(cpImage::statusSuccess == tmpImageModel.GetSubImage(ImagePattern,rRect))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if(tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool )
	{
		if (!IsSuccess(hStatus))
		{
			return false;
		}
		if(cpImage::statusSuccess == tmpImageModel.GetSubImage(ImagePattern,rRect2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		ImagePattern = tmpImageModel;
	}
	return true;
}


// 加载模板文件
bool CSpecialPickSearchTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) QuickSearch模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
	//    (4) 掩模图像MaskImage.bmp(视定位工具是否采用掩模训练而定)

	if (strDir == _T(""))
	{
		return false;
	}
	if (strDir.GetAt(strDir.GetLength()-1) == _T('\\'))
	{
		strDir.Delete(strDir.GetLength()-1);
	}
	DWORD dwRet = GetFileAttributes(strDir);
	if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
	{
		return false;
	}
	strDir += _T('\\');

	//////////////////////////读xml文件//////////////////////////
	CString strXmlPath = strDir + _T("Model.xml");
	vcXMLConfigurator cXMLConfigurator;
	CString strInfo;
	bool bRet;
	sc2Vector vPos;
	//	BYTE byR, byG, byB;
	CSpecialPickSearchTrainDataParam tmpTrainDataParam;
	CSpecialPickSearchTrainGuiParam tmpTrainGuiParam;
	CSpecialPickSearchSearchDataParam tmpSearchDataParam;
	CSpecialPickSearchSearchGuiParam tmpSearchGuiParam;	
	CSpecialPickSearchTrainResult    tmpTrainResult;

	if (!cXMLConfigurator.Load(strXmlPath))
	{
		return false;
	}

	bRet = cXMLConfigurator.FindElem(_T("Config"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

	/////////////工具类型/////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("ToolType"));
	if (!bRet)
	{
		return false;
	}
	CSearchToolType cSearchToolType = (CSearchToolType)_ttoi(cXMLConfigurator.GetElemData());
	if (cSearchToolType != eSpecialPickSearchTool)
	{
		return false;
	}


	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("TrainParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	{
		// ImageChannel
		bRet = cXMLConfigurator.FindElem(_T("ImageChannel"));				
		if (!bRet)
		{
			//return false;
			tmpTrainDataParam.m_nImageChannel = 0;
		}
		else
		{
			tmpTrainDataParam.m_nImageChannel = _ttol(cXMLConfigurator.GetElemData());
		}
		// 粗定位训练参数
		bRet = cXMLConfigurator.FindElem(_T("RoughSearchToolType"));
		if (bRet)
		{
			tmpTrainDataParam.SetRoughSearchToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
		}

		bRet = cXMLConfigurator.FindElem(_T("RoughSearchToolTrainData"));
		if (!bRet)
		{
			return false;
		}
		cXMLConfigurator.EnterElem();
		{
			tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.LoadModelFromFile(cXMLConfigurator);
			tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData.LoadModelFromFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

		// 精定位工具训练参数
		bRet = cXMLConfigurator.FindElem(_T("EnableAccurateLocation"));
		if (bRet)
		{
			tmpTrainDataParam.m_bEnableAccurateLocation = _ttoi(cXMLConfigurator.GetElemData());
		}
		bRet = cXMLConfigurator.FindElem(_T("EnableFourPoint"));
		if (bRet)
		{
			tmpTrainDataParam.m_bEnableFourPoint = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("AccurateSearchToolType"));
		if (bRet)
		{
			tmpTrainDataParam.SetAccurateSearchToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
		}

		for (int it = 0;it<RECT_NUMS;it++)
		{
			strInfo.Format(_T("AccurateSearchToolTrainData%d"),it+1);
			bRet = cXMLConfigurator.FindElem(strInfo);
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			{
				tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[it].LoadModelFromFile(cXMLConfigurator);
				tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[it].LoadModelFromFile(cXMLConfigurator);
			}
			cXMLConfigurator.LeaveElem();
		}

		for (int it = 0;it<LINE_NUMS;it++)
		{
			strInfo.Format(_T("AccurateFindLineToolTrainData%d"),it+1);
			bRet = cXMLConfigurator.FindElem(strInfo);
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			{
				tmpTrainDataParam.m_FindLineCaliperTrainDataParam[it].LoadModelFromFile(cXMLConfigurator);
			}
			cXMLConfigurator.LeaveElem();
		}

		// 头尾
		bRet = cXMLConfigurator.FindElem(_T("HeadTail_TrainData"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("EnableHeadTail"));
			if (bRet)
			{
				tmpTrainDataParam.m_bEnableHeadTail = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("HeadTailCheckToolType"));
			if (bRet)
			{
				tmpTrainDataParam.SetHeadTailCheckToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
			}

			tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.LoadModelFromFile(cXMLConfigurator);
			tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.LoadModelFromFile(cXMLConfigurator);
			tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.LoadModelFromFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();


		// 正反
		bRet = cXMLConfigurator.FindElem(_T("PositiveAndNegative_TrainData"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("EnablePositiveAndNegative"));
			if (bRet)
			{
				tmpTrainDataParam.m_bEnablePositiveAndNegative = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("PositiveAndNegativeCheckToolType"));
			if (bRet)
			{
				tmpTrainDataParam.SetPositiveAndNegativeCheckToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
			}

			tmpTrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.LoadModelFromFile(cXMLConfigurator);
			tmpTrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.LoadModelFromFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();


		// 混料
		bRet = cXMLConfigurator.FindElem(_T("Mixing_TrainData"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("EnableMixing"));
			if (bRet)
			{
				tmpTrainDataParam.m_bEnableMixing = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("MixingCheckToolType"));
			if (bRet)
			{
				tmpTrainDataParam.SetMixingCheckToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
			}

			tmpTrainDataParam.m_Struct_Mixing_PatternSearchTrainData.LoadModelFromFile(cXMLConfigurator);
			tmpTrainDataParam.m_Struct_Mixing_QuickSearchTrainData.LoadModelFromFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

	}
	cXMLConfigurator.LeaveElem();

	//   for (int i=0;i<2;i++)
	//   {
	//       CString tmpStr = _T("");
	//       tmpStr.Format(_T("CornerLineMaskEnable%d"),i);
	//       bRet = cXMLConfigurator.FindElem(tmpStr);
	//       if (bRet)
	//       {
	//           tmpTrainGuiParam.m_bEnableCornerLineMask[i] = _ttoi(cXMLConfigurator.GetElemData());
	//       }
	//   }
	//for (int i=0;i<2;i++)
	//{
	//	CString strTmp;
	//	strTmp.Format(_T("MaskParam%d"),i);
	//	bRet = cXMLConfigurator.FindElem(strTmp);
	//	if (bRet)
	//	{
	//		cXMLConfigurator.EnterElem();
	//		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
	//		if (!bRet) return false;
	//		tmpTrainGuiParam.m_vdMaskData_FindLine[i].resize(_ttoi(cXMLConfigurator.GetElemData()));
	//		for (int j=0; j<tmpTrainGuiParam.m_vdMaskData_FindLine[i].size(); j++)
	//		{
	//			strTmp.Format(_T("MaskData%d"), j);
	//			bRet = cXMLConfigurator.FindElem(strTmp);
	//			if (!bRet) return false;
	//			tmpTrainGuiParam.m_vdMaskData_FindLine[i].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	//		}
	//		bRet = cXMLConfigurator.FindElem(_T("CaliperMaskDataNum"));
	//		if (!bRet) return false;
	//		tmpTrainGuiParam.m_vbCaliperMask_FindLine[i].resize(_ttoi(cXMLConfigurator.GetElemData()));
	//		for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask_FindLine[i].size(); j++)
	//		{
	//			strTmp.Format(_T("CaliperMaskData%d"), j);
	//			bRet = cXMLConfigurator.FindElem(strTmp);
	//			if (!bRet) return false;
	//			tmpTrainGuiParam.m_vbCaliperMask_FindLine[i].at(j) = _ttoi(cXMLConfigurator.GetElemData());
	//		}
	//		cXMLConfigurator.LeaveElem();
	//	}

	//}

	//bRet = cXMLConfigurator.FindElem(_T("InspectTrainGuiParam"));
	//if (bRet)
	//{

	//	cXMLConfigurator.EnterElem();
	//	{
	//		for (int i=0;i<INSPCET_TOOL_LINE_NUMS;i++)
	//		{
	//			CString tmpStr = _T("");
	//			tmpStr.Format(_T("LineMaskEnable%d"),i);
	//			bRet = cXMLConfigurator.FindElem(tmpStr);
	//			if (bRet)
	//			{
	//				tmpTrainGuiParam.m_bEnableLineMask[i] = _ttoi(cXMLConfigurator.GetElemData());
	//			}
	//		}

	//		for (int i=0;i<INSPCET_TOOL_LINE_NUMS;i++)
	//		{
	//			CString strTmp;
	//			strTmp.Format(_T("MaskParam%d"),i);
	//			bRet = cXMLConfigurator.FindElem(strTmp);
	//			if (bRet)
	//			{
	//				cXMLConfigurator.EnterElem();
	//				bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
	//				if (!bRet) return false;
	//				tmpTrainGuiParam.m_vdMaskData_FindLine1[i].resize(_ttoi(cXMLConfigurator.GetElemData()));
	//				for (int j=0; j<tmpTrainGuiParam.m_vdMaskData_FindLine1[i].size(); j++)
	//				{
	//					strTmp.Format(_T("MaskData%d"), j);
	//					bRet = cXMLConfigurator.FindElem(strTmp);
	//					if (!bRet) return false;
	//					tmpTrainGuiParam.m_vdMaskData_FindLine1[i].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	//				}
	//				bRet = cXMLConfigurator.FindElem(_T("CaliperMaskDataNum"));
	//				if (!bRet) return false;
	//				tmpTrainGuiParam.m_vbCaliperMask_FindLine1[i].resize(_ttoi(cXMLConfigurator.GetElemData()));
	//				for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask_FindLine1[i].size(); j++)
	//				{
	//					strTmp.Format(_T("CaliperMaskData%d"), j);
	//					bRet = cXMLConfigurator.FindElem(strTmp);
	//					if (!bRet) return false;
	//					tmpTrainGuiParam.m_vbCaliperMask_FindLine1[i].at(j) = _ttoi(cXMLConfigurator.GetElemData());
	//				}
	//				cXMLConfigurator.LeaveElem();
	//			}
	//		}

	//	}
	//	cXMLConfigurator.LeaveElem();
	//}



	//////////////TrainGuiParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("TrainGuiParam"));
	if (!bRet) return false;
	cXMLConfigurator.EnterElem();
	{
		bRet = cXMLConfigurator.FindElem(_T("Rough_TrainGuiParam"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			tmpTrainGuiParam.m_RoughSearchTrainGuiParam.LoadModelFromFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

		//精定位训练GUI
		// --精定位--二次区域
		for (int it=0;it<RECT_NUMS;it++)
		{
			strInfo.Format(_T("Accurate_TrainGuiParam_Rect%d"),it+1);
			bRet = cXMLConfigurator.FindElem(strInfo);
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			{
				tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].LoadModelFromFile(cXMLConfigurator);
			}
			cXMLConfigurator.LeaveElem();
		}

		// --精定位--找线
		for (int it=0;it<LINE_NUMS;it++)
		{
			strInfo.Format(_T("Line_TrainGuiParam_%d"),it+1);
			bRet = cXMLConfigurator.FindElem(strInfo);
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			{
				tmpTrainGuiParam.m_FindLineTrainGuiParam[it].LoadModelFromFile(cXMLConfigurator);
			}
			cXMLConfigurator.LeaveElem();
		}

		// --精定位--找圆;
		{
			// to be continue……
		}


		// 精定位 -Blob
		{
			CString str;

			str.Format(_T("BlobAffineRect"));
			bRet = cXMLConfigurator.FindElem(str);
			if (!bRet)
			{
				return false;
			}
			cXMLConfigurator.EnterElem();
			{
				bRet = cXMLConfigurator.FindElem(_T("CenterX"));
				if (bRet)
				{
					vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
				}	

				bRet = cXMLConfigurator.FindElem(_T("CenterY"));
				if (bRet)
				{
					vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
				}

				tmpTrainGuiParam.m_BlobAffineRect.SetCenter(vPos);
				bRet = cXMLConfigurator.FindElem(_T("XLength"));
				if (bRet)
				{
					tmpTrainGuiParam.m_BlobAffineRect.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
				}

				bRet = cXMLConfigurator.FindElem(_T("YLength"));
				if (bRet)
				{
					tmpTrainGuiParam.m_BlobAffineRect.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
				}

				bRet = cXMLConfigurator.FindElem(_T("XRotation"));
				if (bRet)
				{
					tmpTrainGuiParam.m_BlobAffineRect.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
				}

				bRet = cXMLConfigurator.FindElem(_T("Skew"));
				if (bRet)
				{
					tmpTrainGuiParam.m_BlobAffineRect.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
				}


			}

			cXMLConfigurator.LeaveElem();
		}

		bRet = cXMLConfigurator.FindElem(_T("HeadTail_TrainGuiParam"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.LoadModelFromFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

		// 正反检查
		bRet = cXMLConfigurator.FindElem(_T("PN_TrainGuiParam"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			tmpTrainGuiParam.m_PN_Check_TrainGuiParam.LoadModelFromFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

		// 混料检查
		bRet = cXMLConfigurator.FindElem(_T("Mixing_TrainGuiParam"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.LoadModelFromFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

	}
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("SearchParam"));
	if (!bRet) return false;
	cXMLConfigurator.EnterElem();
	{
		bRet = cXMLConfigurator.FindElem(_T("Rough_SearchParam"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("Pattern"));
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpSearchDataParam.m_Struct_Rough_PatternSearchSearchData.LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			bRet = cXMLConfigurator.FindElem(_T("Quick"));
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			bRet = cXMLConfigurator.FindElem(_T("Blob_Rough"));
			if (!bRet) goto NOBlobRough;
			cXMLConfigurator.EnterElem();
			tmpSearchDataParam.m_Rough_BlobSearchTrainData.LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();
NOBlobRough:;
		}
		cXMLConfigurator.LeaveElem();

		bRet = cXMLConfigurator.FindElem(_T("CoordSortEnable"));
		if (bRet)
		{
			tmpSearchDataParam.m_bCoordSortEnable = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("CoordSortMethod"));
		if (bRet)
		{
			tmpSearchDataParam.m_nCoordSortMethod =(CoordSortMethod) _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("CoordSortOrderX"));
		if (bRet)
		{
			tmpSearchDataParam.m_nCoordSortOrderX =(SortOrder) _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("CoordSortOrderY"));
		if (bRet)
		{
			tmpSearchDataParam.m_nCoordSortOrderY =(SortOrder) _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("CoordSortGap"));
		if (bRet)
		{
			tmpSearchDataParam.m_dCoordSortGap = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}


		// Blob
		{
			// BlobMode
			bRet = cXMLConfigurator.FindElem(_T("BlobMode"));
			if (bRet)
			{
				tmpSearchDataParam.m_bBlobMode = _ttol(cXMLConfigurator.GetElemData());
			}

			// BlobGrayThrehold
			bRet = cXMLConfigurator.FindElem(_T("BlobGrayThre"));
			if (bRet)
			{
				tmpSearchDataParam.m_lBlobGrayThre = _ttol(cXMLConfigurator.GetElemData());
			}

			// ConnectivityMinPels
			bRet = cXMLConfigurator.FindElem(_T("ConnectivityMinPels"));
			if (bRet)
			{
				tmpSearchDataParam.m_lConnectivityMinPels = _ttol(cXMLConfigurator.GetElemData());
			}

			// BlobAreaMinThre
			bRet = cXMLConfigurator.FindElem(_T("BlobAreaMinThre"));
			if (bRet)
			{
				tmpSearchDataParam.m_lBlobAreaMinThre = _ttol(cXMLConfigurator.GetElemData());
			}

			// BlobAreaMaxThre
			bRet = cXMLConfigurator.FindElem(_T("BlobAreaMaxThre"));
			if (bRet)
			{	
				tmpSearchDataParam.m_lBlobAreaMaxThre = _ttol(cXMLConfigurator.GetElemData());
			}

			// BlobOutMode
			bRet = cXMLConfigurator.FindElem(_T("BlobOutMode1"));
			if (bRet)
			{
				tmpSearchDataParam.m_nBlobOutMode1 = _ttoi(cXMLConfigurator.GetElemData());
			}
			bRet = cXMLConfigurator.FindElem(_T("BlobOutMode2"));
			if (bRet)
			{
				tmpSearchDataParam.m_nBlobOutMode2 = _ttoi(cXMLConfigurator.GetElemData());
			}

		}

		// 找线
		for (int it=0;it<LINE_NUMS;it++)
		{
			strInfo.Format(_T("LineShowFitPoint%d"), it);
			bRet = cXMLConfigurator.FindElem(strInfo);
			if(bRet) tmpSearchDataParam.m_bVisibleFitPoint_Line[it] = _ttoi(cXMLConfigurator.GetElemData());
		}

		// 找圆
		for (int it=0;it<CIRCLE_NUMS;it++)
		{
			strInfo.Format(_T("CircleShowFitPoint%d"), it);
			bRet = cXMLConfigurator.FindElem(strInfo);
			if(bRet) tmpSearchDataParam.m_bVisibleFitPoint_Circle[it] = _ttoi(cXMLConfigurator.GetElemData());
		}

		//头尾检查  区域或几何搜索参数 
		bRet = cXMLConfigurator.FindElem(_T("HeadTail_SearchParam"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("Pattern"));
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpSearchDataParam.m_HeadTail_PatternSearchSearchData.LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			bRet = cXMLConfigurator.FindElem(_T("Quick"));
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpSearchDataParam.m_HeadTail_QuickSearchSearchData.LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();
		}
		cXMLConfigurator.LeaveElem();

		//正反检查  区域或几何搜索参数 
		bRet = cXMLConfigurator.FindElem(_T("PN_CHECK_SearchParam"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("Pattern"));
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpSearchDataParam.m_PN_PatternSearchSearchData.LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			bRet = cXMLConfigurator.FindElem(_T("Quick"));
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpSearchDataParam.m_PN_QuickSearchSearchData.LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();
		}
		cXMLConfigurator.LeaveElem();


		//混料检查  区域或几何搜索参数 
		bRet = cXMLConfigurator.FindElem(_T("Mixing_CHECK_SearchParam"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("Pattern"));
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpSearchDataParam.m_Mixing_PatternSearchSearchData.LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			bRet = cXMLConfigurator.FindElem(_T("Quick"));
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpSearchDataParam.m_Mixing_QuickSearchSearchData.LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();
		}
		cXMLConfigurator.LeaveElem();

		// 头尾检查失败时，输出类型
		bRet = cXMLConfigurator.FindElem(_T("HeadTail_FailType"));
		if (bRet)
		{
			tmpSearchDataParam.m_nHeadTail_FailType =  _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("nRectResultOutMode1"));
		if (bRet)
		{
			tmpSearchDataParam.m_nResultOutMode1 = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("nRectResultOutMode2"));
		if (bRet)
		{
			tmpSearchDataParam.m_nResultOutMode2 = _ttoi(cXMLConfigurator.GetElemData());
		}
	}
	cXMLConfigurator.LeaveElem();


	/////////////TrainResult//////////////
	bRet = cXMLConfigurator.FindElem(_T("TrainResult"));
	if (!bRet) return false;
	cXMLConfigurator.EnterElem();
	{
		bRet = cXMLConfigurator.FindElem(_T("Rough_TrainResult"));
		if (!bRet) return false;
		cXMLConfigurator.EnterElem();
		{
			tmpTrainResult.m_Rough_RectTrainResult.LoadModelFromFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

		for (int it=0;it<RECT_NUMS;it++)
		{
			strInfo.Format(_T("Accurate_TrainResult%d"),it+1);
			bRet = cXMLConfigurator.FindElem(strInfo);
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpTrainResult.m_Accurate_RectTrainResult[it].LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();
		}

		for (int it=0;it<LINE_NUMS;it++)
		{
			strInfo.Format(_T("Line_TrainResult%d"),it+1);
			bRet = cXMLConfigurator.FindElem(strInfo);
			if (!bRet) return false;
			cXMLConfigurator.EnterElem();
			tmpTrainResult.m_FindLineTrainResult[it].LoadModelFromFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();
		}

	}
	cXMLConfigurator.LeaveElem();

	cXMLConfigurator.LeaveElem();

	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpImageModel;
	cpImage tmpImageMask;
	cpImage tmpImageMask1;
	cpImage tmpImageMask2;
	cpImage tmpImageMask3;
	cpImage tmpImageMask4;

	cpImage tmpImageMask_HeadTail;
	cpImage tmpImageMask_PN;
	cpImage tmpImageMask_Mixing;

	hStatus = cDIB.Init(strDir + _T("Model.bmp"));
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	hStatus = cDIB.GetImage(tmpImageModel);
	if (!IsSuccess(hStatus))
	{
		return false;
	}

	/////////////CameraParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("CamParam"));
	if (!bRet)
	{
		m_CamHeight = tmpImageModel.Height();
		m_CamWidth = tmpImageModel.Width();
	}
	else
	{
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("CamHeight"));
			if (!bRet)
			{
				m_CamHeight = tmpImageModel.Height();
			}
			else
			{
				cXMLConfigurator.GetElemData(m_CamHeight);
			}
			bRet = cXMLConfigurator.FindElem(_T("CamWidth"));
			if (!bRet)
			{
				m_CamWidth = tmpImageModel.Width();
			}
			else
			{
				cXMLConfigurator.GetElemData(m_CamWidth);
			}		
		}
		cXMLConfigurator.LeaveElem();
	}

	//////////////////////////读取掩模图像//////////////////////////
	if(tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(strDir + _T("Mask.bmp"));
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_bMaskEnable = false;
			}
			else
			{
				hStatus = cDIB.GetImage(tmpImageMask);
				if (!IsSuccess(hStatus))
				{
					tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_bMaskEnable = false;
				}
			}
		}
	}
	if(tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(strDir + _T("Mask.bmp"));
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData.m_bMaskEnable = false;
			}
			else
			{
				hStatus = cDIB.GetImage(tmpImageMask);
				if (!IsSuccess(hStatus))
				{
					tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData.m_bMaskEnable = false;
				}
			}
		}
	}


	if (tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		hStatus = cDIB.Init(strDir + _T("Mask_Accurate1.bmp"));
		if (!IsSuccess(hStatus))
		{
			if(tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
			{
				tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0].m_bMaskEnable = false;
			}
			else if(tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
			{
				tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0].m_bMaskEnable = false;
			}

		}
		else
		{
			hStatus = cDIB.GetImage(tmpImageMask1);
			if (!IsSuccess(hStatus))
			{
				if(tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
				{
					tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0].m_bMaskEnable = false;
				}
				else if(tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
				{
					tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0].m_bMaskEnable = false;
				}
			}
		}

		hStatus = cDIB.Init(strDir + _T("Mask_Accurate2.bmp"));
		if (!IsSuccess(hStatus))
		{
			if(tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
			{
				tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[1].m_bMaskEnable = false;
			}
			else if(tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
			{
				tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[1].m_bMaskEnable = false;
			}
		}
		else
		{
			hStatus = cDIB.GetImage(tmpImageMask2);
			if (!IsSuccess(hStatus))
			{
				if(tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
				{
					tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[1].m_bMaskEnable = false;
				}
				else if(tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
				{
					tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[1].m_bMaskEnable = false;
				}
			}
		}


		hStatus = cDIB.Init(strDir + _T("Mask_Accurate3.bmp"));
		if (!IsSuccess(hStatus))
		{
			if(tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
			{
				tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[2].m_bMaskEnable = false;
			}
			else if(tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
			{
				tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[2].m_bMaskEnable = false;
			}
		}
		else
		{
			hStatus = cDIB.GetImage(tmpImageMask3);
			if (!IsSuccess(hStatus))
			{
				if(tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
				{
					tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[2].m_bMaskEnable = false;
				}
				else if(tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
				{
					tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[2].m_bMaskEnable = false;
				}
			}
		}


		hStatus = cDIB.Init(strDir + _T("Mask_Accurate4.bmp"));
		if (!IsSuccess(hStatus))
		{
			if(tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
			{
				tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[3].m_bMaskEnable = false;
			}
			else if(tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
			{
				tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[3].m_bMaskEnable = false;
			}
		}
		else
		{
			hStatus = cDIB.GetImage(tmpImageMask4);
			if (!IsSuccess(hStatus))
			{
				if(tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool)
				{
					tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[3].m_bMaskEnable = false;
				}
				else if(tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool)
				{
					tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[3].m_bMaskEnable = false;
				}
			}
		}
	}


	if(tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(strDir + _T("Mask_HeadTail.bmp"));
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable = false;
			}
			else
			{
				hStatus = cDIB.GetImage(tmpImageMask_HeadTail);
				if (!IsSuccess(hStatus))
				{
					tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable = false;
				}
			}
		}
	}
	if(tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(strDir + _T("Mask_HeadTail.bmp"));
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_bMaskEnable = false;
			}
			else
			{
				hStatus = cDIB.GetImage(tmpImageMask_HeadTail);
				if (!IsSuccess(hStatus))
				{
					tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_bMaskEnable = false;
				}
			}
		}
	}
	if(tmpTrainDataParam.GetHeadTailCheckToolType() == ePatternSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable)
		{
			m_tmpImageMask_HeadTail = tmpImageMask_HeadTail;
		}
	}
	if(tmpTrainDataParam.GetHeadTailCheckToolType() == eQuickSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_bMaskEnable)
		{
			m_tmpImageMask_HeadTail = tmpImageMask_HeadTail;
		}
	}


	if(tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == ePatternSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(strDir + _T("Mask_PN.bmp"));
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.m_bMaskEnable = false;
			}
			else
			{
				hStatus = cDIB.GetImage(tmpImageMask_PN);
				if (!IsSuccess(hStatus))
				{
					tmpTrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.m_bMaskEnable = false;
				}
			}
		}
	}
	if(tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == eQuickSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(strDir + _T("Mask_PN.bmp"));
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.m_bMaskEnable = false;
			}
			else
			{
				hStatus = cDIB.GetImage(tmpImageMask_PN);
				if (!IsSuccess(hStatus))
				{
					tmpTrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.m_bMaskEnable = false;
				}
			}
		}
	}
	if(tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == ePatternSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.m_bMaskEnable)
		{
			m_tmpImageMask_PN = tmpImageMask_PN;
		}
	}
	if(tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == eQuickSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.m_bMaskEnable)
		{
			m_tmpImageMask_PN = tmpImageMask_PN;
		}
	}


	if(tmpTrainDataParam.GetMixingCheckToolType() == ePatternSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_Mixing_PatternSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(strDir + _T("Mask_Mixing.bmp"));
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_Struct_Mixing_PatternSearchTrainData.m_bMaskEnable = false;
			}
			else
			{
				hStatus = cDIB.GetImage(tmpImageMask_PN);
				if (!IsSuccess(hStatus))
				{
					tmpTrainDataParam.m_Struct_Mixing_PatternSearchTrainData.m_bMaskEnable = false;
				}
			}
		}
	}
	if(tmpTrainDataParam.GetMixingCheckToolType() == eQuickSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_Mixing_QuickSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(strDir + _T("Mask_Mixing.bmp"));
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_Struct_Mixing_QuickSearchTrainData.m_bMaskEnable = false;
			}
			else
			{
				hStatus = cDIB.GetImage(tmpImageMask_Mixing);
				if (!IsSuccess(hStatus))
				{
					tmpTrainDataParam.m_Struct_Mixing_QuickSearchTrainData.m_bMaskEnable = false;
				}
			}
		}
	}
	if(tmpTrainDataParam.GetMixingCheckToolType() == ePatternSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable)
		{
			m_tmpImageMask_Mixing = tmpImageMask_Mixing;
		}
	}
	if(tmpTrainDataParam.GetMixingCheckToolType() == eQuickSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_bMaskEnable)
		{
			m_tmpImageMask_Mixing = tmpImageMask_Mixing;
		}
	}


	m_tmpImageTrain = tmpImageModel;

	if(tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_bMaskEnable)
		{
			m_tmpImageMask = tmpImageMask;
		}
	}

	if(tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool )
	{
		if (tmpTrainDataParam.m_Struct_Rough_QuickSearchTrainData.m_bMaskEnable)
		{
			m_tmpImageMask = tmpImageMask;
		}
	}



	if(tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool )
	{

		if(tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0].m_bMaskEnable)
		{
			m_tmpImageMask_Accurate1 = tmpImageMask1;
		}

		if(tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0].m_bMaskEnable)
		{
			m_tmpImageMask_Accurate2 = tmpImageMask2;
		}

		if(tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0].m_bMaskEnable)
		{
			m_tmpImageMask_Accurate3 = tmpImageMask3;
		}

		if(tmpTrainDataParam.m_Struct_Accurate_PatternSearchTrainData[0].m_bMaskEnable)
		{
			m_tmpImageMask_Accurate4 = tmpImageMask4;
		}
	}

	if(tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool )
	{

		if(tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0].m_bMaskEnable)
		{
			m_tmpImageMask_Accurate1 = tmpImageMask1;
		}

		if(tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0].m_bMaskEnable)
		{
			m_tmpImageMask_Accurate2 = tmpImageMask2;
		}

		if(tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0].m_bMaskEnable)
		{
			m_tmpImageMask_Accurate3 = tmpImageMask3;
		}

		if(tmpTrainDataParam.m_Struct_Accurate_QuickSearchTrainData[0].m_bMaskEnable)
		{
			m_tmpImageMask_Accurate4 = tmpImageMask4;
		}
	}

	//m_PatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	m_tmpTrainDataParam = tmpTrainDataParam;
	m_tmpTrainGuiParam  = tmpTrainGuiParam;
	m_tmpSearchDataParam= tmpSearchDataParam;
	m_tmpSearchGuiParam = tmpSearchGuiParam;
	m_TrainDataParam	= tmpTrainDataParam;
	m_TrainGuiParam		= tmpTrainGuiParam;
	m_SearchDataParam	= tmpSearchDataParam;
	m_SearchGuiParam	= tmpSearchGuiParam;
	m_TrainResult       = tmpTrainResult;
	m_ImageTrain		= tmpImageModel;
	m_ImageMask			= tmpImageMask;

	m_ImageMask_Accurate1=tmpImageMask1;
	m_ImageMask_Accurate2=tmpImageMask2;
	m_ImageMask_Accurate3=tmpImageMask3;
	m_ImageMask_Accurate4=tmpImageMask4;

	m_ImageMask_HeadTail = tmpImageMask_HeadTail;
	m_ImageMask_PN = m_tmpImageMask_PN;
	m_ImageMask_Mixing = tmpImageMask_Mixing;

	m_bTrainOK = true;
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		bRet = m_PatternSearchTool_Rough.LoadTrainModelFromBinaryFile(strDir + _T("Model_Rough.pts"));
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		bRet = m_QuickSearchTool_Rough.LoadTrainModelFromBinaryFile(strDir + _T("Model_Rough.ptq"));
	}
	if(m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool)
	{
		bRet = true;
	}

	if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool && m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		//bRet = m_PatternSearchTool1.LoadTrainModelFromBinaryFile(strDir + _T("Model1.pts"));
		CString strFileName = _T("");
		for (int it=0;it<RECT_NUMS;it++)
		{
			strFileName.Format(_T("Model_Accurate%d.pts"),it+1);
			bRet = m_PatternSearchTool_Accurate[it].LoadTrainModelFromBinaryFile(strDir + strFileName);
		}
	}
	if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool && m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		//bRet = m_QuickSearchTool1.LoadTrainModelFromBinaryFile(strDir + _T("Model1.ptq"));
		CString strFileName = _T("");
		for (int it=0;it<RECT_NUMS;it++)
		{
			strFileName.Format(_T("Model_Accurate%d.ptq"),it+1);
			bRet = m_QuickSearchTool_Accurate[it].LoadTrainModelFromBinaryFile(strDir + strFileName);
		}
	}
	if (!bRet)
	{
		return false;
	}
	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}

// 保存模板文件
bool CSpecialPickSearchTool::SaveModelToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) QuickSearch模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
	//    (4) 掩模图像MaskImage.bmp(视定位工具是否采用掩模训练而定)

	if (strDir == _T(""))
	{
		return false;
	}
	if (strDir.GetAt(strDir.GetLength()-1) == _T('\\'))
	{
		strDir.Delete(strDir.GetLength()-1);
	}
	DWORD dwRet = GetFileAttributes(strDir);
	if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
	{
		if (!CreateDirectory(strDir, NULL))
		{	
			return false;
		}
	}
	strDir += _T('\\');

	//////////////////////////写xml文件//////////////////////////
	vcXMLConfigurator cXMLConfigurator;
	CString strInfo;
	cXMLConfigurator.SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n")); 
	cXMLConfigurator.AddElem(_T("Config"));
	cXMLConfigurator.EnterElem();

	/////////////工具类型/////////////
	strInfo.Format(_T("%d"), GetSearchToolType());
	cXMLConfigurator.AddElem(_T("ToolType"), strInfo);

	////////////CameraParam//////////
	cXMLConfigurator.AddElem(_T("CamParam"));
	cXMLConfigurator.EnterElem();
	{
		cXMLConfigurator.AddElem(_T("CamHeight"), m_CamHeight);
		cXMLConfigurator.AddElem(_T("CamWidth"), m_CamWidth);
	}
	cXMLConfigurator.LeaveElem();
	/////////////TrainDataParam//////////////
	cXMLConfigurator.AddElem(_T("TrainParam"));
	cXMLConfigurator.EnterElem();
	{
		// ImageChannel
		strInfo.Format(_T("%d"), m_TrainDataParam.m_nImageChannel);
		cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);
		// 粗定位训练参数
		// 粗定位工具类型
		strInfo.Format(_T("%d"), m_TrainDataParam.GetRoughSearchToolType());
		cXMLConfigurator.AddElem(_T("RoughSearchToolType"), strInfo);

		cXMLConfigurator.AddElem(_T("RoughSearchToolTrainData"));
		cXMLConfigurator.EnterElem();
		{
			m_TrainDataParam.m_Struct_Rough_PatternSearchTrainData.SaveModelToFile(cXMLConfigurator);
			m_TrainDataParam.m_Struct_Rough_QuickSearchTrainData.SaveModelToFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

		// 精定位工具训练参数
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableAccurateLocation);
		cXMLConfigurator.AddElem(_T("EnableAccurateLocation"), strInfo);

		strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableFourPoint);
		cXMLConfigurator.AddElem(_T("EnableFourPoint"), strInfo);
		// --精定位工具类型
		strInfo.Format(_T("%d"), m_TrainDataParam.GetAccurateSearchToolType());
		cXMLConfigurator.AddElem(_T("AccurateSearchToolType"), strInfo);

		// ----精定位--二次区域几何定位
		for (int it = 0;it<RECT_NUMS;it++)
		{
			strInfo.Format(_T("AccurateSearchToolTrainData%d"),it+1);
			cXMLConfigurator.AddElem(strInfo);
			cXMLConfigurator.EnterElem();
			{
				m_TrainDataParam.m_Struct_Accurate_PatternSearchTrainData[it].SaveModelToFile(cXMLConfigurator);
				m_TrainDataParam.m_Struct_Accurate_QuickSearchTrainData[it].SaveModelToFile(cXMLConfigurator);
			}
			cXMLConfigurator.LeaveElem();
		}

		// -----精定位--找线工具训练参数保存
		for (int it = 0;it<LINE_NUMS;it++)
		{
			strInfo.Format(_T("AccurateFindLineToolTrainData%d"),it+1);
			cXMLConfigurator.AddElem(strInfo);
			cXMLConfigurator.EnterElem();
			{
				m_TrainDataParam.m_FindLineCaliperTrainDataParam[it].SaveModelToFile(cXMLConfigurator);
			}
			cXMLConfigurator.LeaveElem();
		}

		// -----精定位--找圆工具训练参数保存   Blob 等待
		{
			// to be continue……
		}

		//--头尾检查训练参数
		cXMLConfigurator.AddElem(_T("HeadTail_TrainData"));
		cXMLConfigurator.EnterElem();
		{
			strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableHeadTail);
			cXMLConfigurator.AddElem(_T("EnableHeadTail"), strInfo);

			strInfo.Format(_T("%d"), m_TrainDataParam.GetHeadTailCheckToolType());
			cXMLConfigurator.AddElem(_T("HeadTailCheckToolType"), strInfo);
			m_TrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.SaveModelToFile(cXMLConfigurator);
			m_TrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.SaveModelToFile(cXMLConfigurator);
			m_TrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.SaveModelToFile(cXMLConfigurator);

		}
		cXMLConfigurator.LeaveElem();

		//--正反检查训练参数
		cXMLConfigurator.AddElem(_T("PositiveAndNegative_TrainData"));
		cXMLConfigurator.EnterElem();
		{
			strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnablePositiveAndNegative);
			cXMLConfigurator.AddElem(_T("EnablePositiveAndNegative"), strInfo);

			strInfo.Format(_T("%d"), m_TrainDataParam.GetPositiveAndNegativeCheckToolType());
			cXMLConfigurator.AddElem(_T("PositiveAndNegativeCheckToolType"), strInfo);
			m_TrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.SaveModelToFile(cXMLConfigurator);
			m_TrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.SaveModelToFile(cXMLConfigurator);

		}
		cXMLConfigurator.LeaveElem();


		//--混料检查训练参数
		cXMLConfigurator.AddElem(_T("Mixing_TrainData"));
		cXMLConfigurator.EnterElem();
		{
			strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableMixing);
			cXMLConfigurator.AddElem(_T("EnableMixing"), strInfo);

			strInfo.Format(_T("%d"), m_TrainDataParam.GetMixingCheckToolType());
			cXMLConfigurator.AddElem(_T("MixingCheckToolType"), strInfo);
			m_TrainDataParam.m_Struct_Mixing_PatternSearchTrainData.SaveModelToFile(cXMLConfigurator);
			m_TrainDataParam.m_Struct_Mixing_QuickSearchTrainData.SaveModelToFile(cXMLConfigurator);

		}
		cXMLConfigurator.LeaveElem();
	}
	cXMLConfigurator.LeaveElem();


	////////////////TrainGuiParam//////////////


	//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
	// 	cXMLConfigurator.LeaveElem();
	// 
	// 	//////////////TrainGuiParam//////////////
	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
	cXMLConfigurator.EnterElem();
	{
		// 粗定位训练GUI
		cXMLConfigurator.AddElem(_T("Rough_TrainGuiParam"));
		cXMLConfigurator.EnterElem();
		{
			m_TrainGuiParam.m_RoughSearchTrainGuiParam.SaveModelToFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

		//精定位训练GUI
		// --精定位--二次区域
		for (int it=0;it<RECT_NUMS;it++)
		{
			strInfo.Format(_T("Accurate_TrainGuiParam_Rect%d"),it+1);
			cXMLConfigurator.AddElem(strInfo);
			cXMLConfigurator.EnterElem();
			{
				m_TrainGuiParam.m_AccurateSearchTrainGuiParam[it].SaveModelToFile(cXMLConfigurator);
			}
			cXMLConfigurator.LeaveElem();
		}

		// --精定位--找线
		for (int it=0;it<LINE_NUMS;it++)
		{
			strInfo.Format(_T("Line_TrainGuiParam_%d"),it+1);
			cXMLConfigurator.AddElem(strInfo);
			cXMLConfigurator.EnterElem();
			{
				m_TrainGuiParam.m_FindLineTrainGuiParam[it].SaveModelToFile(cXMLConfigurator);
			}
			cXMLConfigurator.LeaveElem();
		}

		// --精定位--找圆;
		{
			// to be continue……
		}

		// 精定位 Blob
		CString str;
		str.Format(_T("BlobAffineRect"));
		cXMLConfigurator.AddElem(str);
		cXMLConfigurator.EnterElem();
		{
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetCenter().GetX());
			cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetCenter().GetY());
			cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetXLength());
			cXMLConfigurator.AddElem(_T("XLength"), strInfo);
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetYLength());
			cXMLConfigurator.AddElem(_T("YLength"), strInfo);
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetXRotation().ToDouble());
			cXMLConfigurator.AddElem(_T("XRotation"), strInfo);
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetSkew().ToDouble());
			cXMLConfigurator.AddElem(_T("Skew"), strInfo);
		}
		cXMLConfigurator.LeaveElem();



		// 头尾检查
		cXMLConfigurator.AddElem(_T("HeadTail_TrainGuiParam"));
		cXMLConfigurator.EnterElem();
		{
			m_TrainGuiParam.m_HeadTail_Check_TrainGuiParam.SaveModelToFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

		// 正反检查
		cXMLConfigurator.AddElem(_T("PN_TrainGuiParam"));
		cXMLConfigurator.EnterElem();
		{
			m_TrainGuiParam.m_PN_Check_TrainGuiParam.SaveModelToFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();

		// 混料检查
		cXMLConfigurator.AddElem(_T("Mixing_TrainGuiParam"));
		cXMLConfigurator.EnterElem();
		{
			m_TrainGuiParam.m_Mixing_Check_TrainGuiParam.SaveModelToFile(cXMLConfigurator);
		}
		cXMLConfigurator.LeaveElem();


	}
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	cXMLConfigurator.AddElem(_T("SearchParam"));
	cXMLConfigurator.EnterElem();
	{
		//粗定位 搜索参数
		cXMLConfigurator.AddElem(_T("Rough_SearchParam"));
		cXMLConfigurator.EnterElem();
		{
			cXMLConfigurator.AddElem(_T("Pattern"));
			cXMLConfigurator.EnterElem();
			m_SearchDataParam.m_Struct_Rough_PatternSearchSearchData.SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			cXMLConfigurator.AddElem(_T("Quick"));
			cXMLConfigurator.EnterElem();
			m_SearchDataParam.m_Struct_Rough_QuickSearchSearchData.SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			cXMLConfigurator.AddElem(_T("Blob_Rough"));
			cXMLConfigurator.EnterElem();
			m_SearchDataParam.m_Rough_BlobSearchTrainData.SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

		}
		cXMLConfigurator.LeaveElem();

		strInfo.Format(_T("%d"), m_SearchDataParam.m_bCoordSortEnable);
		cXMLConfigurator.AddElem(_T("CoordSortEnable"), strInfo);
		strInfo.Format(_T("%d"), m_SearchDataParam.m_nCoordSortMethod);
		cXMLConfigurator.AddElem(_T("CoordSortMethod"), strInfo);
		strInfo.Format(_T("%d"), m_SearchDataParam.m_nCoordSortOrderX);
		cXMLConfigurator.AddElem(_T("CoordSortOrderX"), strInfo);
		strInfo.Format(_T("%d"), m_SearchDataParam.m_nCoordSortOrderY);
		cXMLConfigurator.AddElem(_T("CoordSortOrderY"), strInfo);
		strInfo.Format(_T("%f"), m_SearchDataParam.m_dCoordSortGap);
		cXMLConfigurator.AddElem(_T("CoordSortGap"), strInfo);

		strInfo.Format(_T("%d"), m_SearchDataParam.m_bBlobMode);
		cXMLConfigurator.AddElem(_T("BlobMode"), strInfo);

		strInfo.Format(_T("%d"), m_SearchDataParam.m_lBlobGrayThre);
		cXMLConfigurator.AddElem(_T("BlobGrayThre"), strInfo);

		strInfo.Format(_T("%d"), m_SearchDataParam.m_lConnectivityMinPels);
		cXMLConfigurator.AddElem(_T("ConnectivityMinPels"), strInfo);

		strInfo.Format(_T("%d"), m_SearchDataParam.m_lBlobAreaMinThre);
		cXMLConfigurator.AddElem(_T("BlobAreaMinThre"), strInfo);

		strInfo.Format(_T("%d"), m_SearchDataParam.m_lBlobAreaMaxThre);
		cXMLConfigurator.AddElem(_T("BlobAreaMaxThre"), strInfo);

		strInfo.Format(_T("%d"), m_SearchDataParam.m_nBlobOutMode1);
		cXMLConfigurator.AddElem(_T("BlobOutMode1"), strInfo);

		strInfo.Format(_T("%d"), m_SearchDataParam.m_nBlobOutMode2);
		cXMLConfigurator.AddElem(_T("BlobOutMode2"), strInfo);

		for (int it=0;it<LINE_NUMS;it++)
		{
			CString strTmp = _T("");
			strInfo.Format(_T("LineShowFitPoint%d"), it);
			strTmp.Format(_T("%d"), m_SearchDataParam.m_bVisibleFitPoint_Line[it]);
			cXMLConfigurator.AddElem(strInfo, strTmp);
		}

		for (int it=0;it<CIRCLE_NUMS;it++)
		{
			CString strTmp = _T("");
			strInfo.Format(_T("CircleShowFitPoint%d"), it);
			strTmp.Format(_T("%d"), m_SearchDataParam.m_bVisibleFitPoint_Circle[it]);
			cXMLConfigurator.AddElem(strInfo, strTmp);
		}


		//头尾检查  区域或几何搜索参数
		cXMLConfigurator.AddElem(_T("HeadTail_SearchParam"));
		cXMLConfigurator.EnterElem();
		{
			cXMLConfigurator.AddElem(_T("Pattern"));
			cXMLConfigurator.EnterElem();
			m_SearchDataParam.m_HeadTail_PatternSearchSearchData.SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			cXMLConfigurator.AddElem(_T("Quick"));
			cXMLConfigurator.EnterElem();
			m_SearchDataParam.m_HeadTail_QuickSearchSearchData.SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

		}
		cXMLConfigurator.LeaveElem();

		//正反检查  区域或几何搜索参数
		cXMLConfigurator.AddElem(_T("PN_CHECK_SearchParam"));
		cXMLConfigurator.EnterElem();
		{
			cXMLConfigurator.AddElem(_T("Pattern"));
			cXMLConfigurator.EnterElem();
			m_SearchDataParam.m_PN_PatternSearchSearchData.SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			cXMLConfigurator.AddElem(_T("Quick"));
			cXMLConfigurator.EnterElem();
			m_SearchDataParam.m_PN_PatternSearchSearchData.SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

		}
		cXMLConfigurator.LeaveElem();

		//混料检查  区域或几何搜索参数
		cXMLConfigurator.AddElem(_T("Mixing_CHECK_SearchParam"));
		cXMLConfigurator.EnterElem();
		{
			cXMLConfigurator.AddElem(_T("Pattern"));
			cXMLConfigurator.EnterElem();
			m_SearchDataParam.m_Mixing_PatternSearchSearchData.SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

			cXMLConfigurator.AddElem(_T("Quick"));
			cXMLConfigurator.EnterElem();
			m_SearchDataParam.m_Mixing_QuickSearchSearchData.SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();

		}
		cXMLConfigurator.LeaveElem();

		strInfo.Format(_T("%d"), m_SearchDataParam.m_nHeadTail_FailType);
		cXMLConfigurator.AddElem(_T("HeadTail_FailType"), strInfo);

		strInfo.Format(_T("%d"), m_SearchDataParam.m_nResultOutMode1);
		cXMLConfigurator.AddElem(_T("nRectResultOutMode1"), strInfo);

		strInfo.Format(_T("%d"), m_SearchDataParam.m_nResultOutMode2);
		cXMLConfigurator.AddElem(_T("nRectResultOutMode2"), strInfo);

	}
	cXMLConfigurator.LeaveElem();


	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	{
		cXMLConfigurator.AddElem(_T("Rough_TrainResult"));
		cXMLConfigurator.EnterElem();
		m_TrainResult.m_Rough_RectTrainResult.SaveModelToFile(cXMLConfigurator);
		cXMLConfigurator.LeaveElem();

		for (int it=0;it<RECT_NUMS;it++)
		{
			strInfo.Format(_T("Accurate_TrainResult%d"),it+1);
			cXMLConfigurator.AddElem(strInfo);
			cXMLConfigurator.EnterElem();
			m_TrainResult.m_Accurate_RectTrainResult[it].SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();
		}

		for (int it=0;it<LINE_NUMS;it++)
		{
			strInfo.Format(_T("Line_TrainResult%d"),it+1);
			cXMLConfigurator.AddElem(strInfo);
			cXMLConfigurator.EnterElem();
			m_TrainResult.m_FindLineTrainResult[it].SaveModelToFile(cXMLConfigurator);
			cXMLConfigurator.LeaveElem();
		}

	}
	cXMLConfigurator.LeaveElem();




	cXMLConfigurator.LeaveElem();


	bool bRet = cXMLConfigurator.Save(strDir+_T("Model.xml"));
	if (!bRet)
	{
		return false;
	}

	//////////////////////////存储模板图像文//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	hStatus = cDIB.Init(m_ImageTrain);
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	hStatus = cDIB.Write(strDir + _T("Model.bmp"));
	if (!IsSuccess(hStatus))
	{
		return false;
	}

	//////////////////////////存储掩模图像//////////////////////////
	if (m_ImageMask.IsValid())
	{

		if (m_TrainDataParam.m_Struct_Rough_PatternSearchTrainData.m_bMaskEnable || m_TrainDataParam.m_Struct_Rough_QuickSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(m_ImageMask);
			if (!IsSuccess(hStatus))
			{
				return false;
			}
			hStatus = cDIB.Write(strDir + _T("Mask.bmp"));
			if (!IsSuccess(hStatus))
			{
				return false;
			}
		}


	}

	if (m_TrainDataParam.m_bEnableAccurateLocation   && m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{

		if (m_ImageMask_Accurate1.IsValid())
		{
			hStatus = cDIB.Init(m_ImageMask_Accurate1);
			if (!IsSuccess(hStatus))
			{
				return false;
			}
			hStatus = cDIB.Write(strDir + _T("Mask_Accurate1.bmp"));
			if (!IsSuccess(hStatus))
			{
				return false;
			}
		}

		if (m_ImageMask_Accurate2.IsValid())
		{
			hStatus = cDIB.Init(m_ImageMask_Accurate2);
			if (!IsSuccess(hStatus))
			{
				return false;
			}
			hStatus = cDIB.Write(strDir + _T("Mask_Accurate2.bmp"));
			if (!IsSuccess(hStatus))
			{
				return false;
			}
		}


		if (m_ImageMask_Accurate3.IsValid())
		{
			hStatus = cDIB.Init(m_ImageMask_Accurate3);
			if (!IsSuccess(hStatus))
			{
				return false;
			}
			hStatus = cDIB.Write(strDir + _T("Mask_Accurate3.bmp"));
			if (!IsSuccess(hStatus))
			{
				return false;
			}
		}

		if (m_ImageMask_Accurate4.IsValid())
		{
			hStatus = cDIB.Init(m_ImageMask_Accurate4);
			if (!IsSuccess(hStatus))
			{
				return false;
			}
			hStatus = cDIB.Write(strDir + _T("Mask_Accurate4.bmp"));
			if (!IsSuccess(hStatus))
			{
				return false;
			}
		}

	}


	if (m_ImageMask_HeadTail.IsValid())
	{

		if (m_TrainDataParam.m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable || m_TrainDataParam.m_Struct_HeadTail_QuickSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(m_ImageMask_HeadTail);
			if (!IsSuccess(hStatus))
			{
				return false;
			}
			hStatus = cDIB.Write(strDir + _T("Mask_HeadTail.bmp"));
			if (!IsSuccess(hStatus))
			{
				return false;
			}
		}


	}

	if (m_ImageMask_PN.IsValid())
	{

		if (m_TrainDataParam.m_Struct_PositiveAndNegative_PatternSearchTrainData.m_bMaskEnable || m_TrainDataParam.m_Struct_PositiveAndNegative_QuickSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(m_ImageMask_PN);
			if (!IsSuccess(hStatus))
			{
				return false;
			}
			hStatus = cDIB.Write(strDir + _T("Mask_PN.bmp"));
			if (!IsSuccess(hStatus))
			{
				return false;
			}
		}


	}


	if (m_ImageMask_Mixing.IsValid())
	{

		if (m_TrainDataParam.m_Struct_Mixing_PatternSearchTrainData.m_bMaskEnable || m_TrainDataParam.m_Struct_Mixing_QuickSearchTrainData.m_bMaskEnable)
		{
			hStatus = cDIB.Init(m_ImageMask_Mixing);
			if (!IsSuccess(hStatus))
			{
				return false;
			}
			hStatus = cDIB.Write(strDir + _T("Mask_Mixing.bmp"));
			if (!IsSuccess(hStatus))
			{
				return false;
			}
		}


	}

	//////////////////////////存储模板pts文件//////////////////////////
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		bRet = m_PatternSearchTool_Rough.SaveTrainModelToBinaryFile(strDir + _T("Model_Rough.pts"));
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		bRet = m_QuickSearchTool_Rough.SaveTrainModelToBinaryFile(strDir + _T("Model_Rough.ptq"));
	}

	if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool && m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		CString strFileName = _T("");
		for (int it=0;it<RECT_NUMS;it++)
		{
			strFileName.Format(_T("Model_Accurate%d.pts"),it+1);
			bRet = m_PatternSearchTool_Accurate[it].SaveTrainModelToBinaryFile(strDir + strFileName);
		}
	}
	if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool && m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		CString strFileName = _T("");
		for (int it=0;it<RECT_NUMS;it++)
		{
			strFileName.Format(_T("Model_Accurate%d.ptq"),it+1);
			bRet = m_QuickSearchTool_Accurate[it].SaveTrainModelToBinaryFile(strDir + strFileName);
		}
	}

	if (!bRet)
	{
		return false;
	}

	return true;
}	

bool CSpecialPickSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen, rot,skew;

	m_GuiTrainRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiTrainAffineRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainAffineRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_GuiModelPoint2.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint2.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);


	for (int it=0;it<RECT_NUMS;it++)
	{

		m_GuiTrainRect_Accurate[it].GetCenterWidthHeight(centerx,centery,xlen,ylen);
		m_GuiTrainRect_Accurate[it].SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

		m_GuiTrainAffineRect_Accurate[it].GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
		m_GuiTrainAffineRect_Accurate[it].SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

		m_GuiSearchRect_Accurate[it].GetCenterWidthHeight(centerx,centery,xlen,ylen);
		m_GuiSearchRect_Accurate[it].SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

		m_GuiModelPoint_Accurate[it].GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
		m_GuiModelPoint_Accurate[it].SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	}


	// 头尾
	m_GuiTrainRect_HeadTail.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiTrainRect_HeadTail.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiTrainAffineRect_HeadTail.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainAffineRect_HeadTail.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	m_GuiSearchRect_HeadTail.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect_HeadTail.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiModelPoint_HeadTail.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint_HeadTail.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	// 正反
	m_GuiTrainRect_PN.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiTrainRect_PN.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiTrainAffineRect_PN.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainAffineRect_PN.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	m_GuiSearchRect_PN.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect_PN.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiModelPoint_PN.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint_PN.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);


	// 混料
	m_GuiTrainRect_Mixing.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiTrainRect_Mixing.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiTrainAffineRect_Mixing.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainAffineRect_Mixing.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	m_GuiSearchRect_Mixing.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect_Mixing.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiModelPoint_Mixing.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint_Mixing.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);


	//卡尺
	double sx,sy,ex,ey;
	double plength,slength;
	for (int it=0;it<LINE_NUMS;it++)
	{
		m_GuiFindLine[it].GetStartXYEndXY(sx,sy,ex,ey);
		m_GuiFindLine[it].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);

		plength = m_GuiFindLine[it].GetCaliperProjectionLength();
		slength = m_GuiFindLine[it].GetCaliperSearchLength();
		m_GuiFindLine[it].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
		m_GuiFindLine[it].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	}

	/*  m_GuiFindLine[0].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[0].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
	m_GuiFindLine[1].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[1].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
	*/

	//Blob区域
	m_GuiTrainBlobAffineRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainBlobAffineRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);



	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

void CSpecialPickSearchTool::SetTrainGuiParam_For_Rect(scGuiCross *p_GuiModelPoint,scGuiRect *p_GuiTrainRect,scGuiAffineRect *p_GuiTrainAffineRect,scGuiRect *p_GuiSearchRect,CRoughSearchTrainGuiParam mp_RoughSearchTrainGuiParam)
{
	// 将训练GUI参数设置到scGui
	if (p_GuiModelPoint ==NULL) return;
	if (p_GuiTrainRect ==NULL) return;
	if (p_GuiTrainAffineRect ==NULL) return;
	if (p_GuiSearchRect ==NULL) return;

	p_GuiModelPoint->SetLineColor(mp_RoughSearchTrainGuiParam.m_LineColor);
	p_GuiModelPoint->SetLineWidth(mp_RoughSearchTrainGuiParam.m_nLineWidth);
	p_GuiModelPoint->SetCenterExtent(mp_RoughSearchTrainGuiParam.m_ModelPoint,p_GuiModelPoint->GetExtent());

	p_GuiTrainRect->SetLineColor(mp_RoughSearchTrainGuiParam.m_LineColor);
	p_GuiTrainRect->SetLineWidth(mp_RoughSearchTrainGuiParam.m_nLineWidth);
	p_GuiTrainRect->SetRect(mp_RoughSearchTrainGuiParam.m_TrainRect);

	p_GuiTrainAffineRect->SetLineColor(mp_RoughSearchTrainGuiParam.m_LineColor);
	p_GuiTrainAffineRect->SetLineWidth(mp_RoughSearchTrainGuiParam.m_nLineWidth);
	p_GuiTrainAffineRect->SetAffineRect(mp_RoughSearchTrainGuiParam.m_TrainAffineRect);

	p_GuiSearchRect->SetLineColor(mp_RoughSearchTrainGuiParam.m_LineColor);
	p_GuiSearchRect->SetLineWidth(mp_RoughSearchTrainGuiParam.m_nLineWidth);
	p_GuiSearchRect->SetRect(mp_RoughSearchTrainGuiParam.m_SearchRect);


}



// 更新训练控件数据信息	
bool CSpecialPickSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainRect = m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint = m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint1 = m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint2 = m_GuiModelPoint2.GetCenter();
		m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_SearchRect = m_GuiSearchRect.GetRect();
		m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainAffineRect	= m_GuiTrainAffineRect.GetAffineRect();

		scRadian tmp = m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainAffineRect.GetXRotation();
		scRadian tmp2 = m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_TrainAffineRect.GetSkew();

		for (int it=0;it<RECT_NUMS;it++)
		{
			m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_TrainRect = m_GuiTrainRect_Accurate[it].GetRect();
			m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_ModelPoint = m_GuiModelPoint_Accurate[it].GetCenter();
			m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_SearchRect = m_GuiSearchRect_Accurate[it].GetRect();
			m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it].m_TrainAffineRect	= m_GuiTrainAffineRect_Accurate[it].GetAffineRect();
		}
		// 头尾
		m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_TrainRect = m_GuiTrainRect_HeadTail.GetRect();
		m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_ModelPoint = m_GuiModelPoint_HeadTail.GetCenter();
		m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_SearchRect = m_GuiSearchRect_HeadTail.GetRect();
		m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_TrainAffineRect	= m_GuiTrainAffineRect_HeadTail.GetAffineRect();

		// 正反
		m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_TrainRect = m_GuiTrainRect_PN.GetRect();
		m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_ModelPoint = m_GuiModelPoint_PN.GetCenter();
		m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_SearchRect = m_GuiSearchRect_PN.GetRect();
		m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam.m_TrainAffineRect	= m_GuiTrainAffineRect_PN.GetAffineRect();

		// 混料
		m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_TrainRect = m_GuiTrainRect_Mixing.GetRect();
		m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_ModelPoint = m_GuiModelPoint_Mixing.GetCenter();
		m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_SearchRect = m_GuiSearchRect_Mixing.GetRect();
		m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam.m_TrainAffineRect	= m_GuiTrainAffineRect_Mixing.GetAffineRect();


		for (int it=0; it<LINE_NUMS; it++)
		{
			m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_nCaliperNum		= m_GuiFindLine  [it].GetCaliperNum();
			m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_dCaliperProjectLen	= m_GuiFindLine[it].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_dCaliperSearchLen		= m_GuiFindLine[it].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_CaliperSearchRotation	= m_GuiFindLine[it].GetCaliperSearchDirection();
			m_GuiFindLine[it].GetStartXYEndXY(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_dStartX, m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_dStartY, m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_dEndX,
				m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_dEndY);

			m_GuiFindLine[it].GetMaskData(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_vdMaskData_FindLine);
			m_GuiFindLine[it].GetCaliperMasked(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_vbCaliperMask_FindLine);
			m_tmpTrainGuiParam.m_FindLineTrainGuiParam[it].m_bCaliperVisible		=m_GuiFindLine[it].IsCaliperVisible();
		}


		m_tmpTrainGuiParam.m_BlobAffineRect = m_GuiTrainBlobAffineRect.GetAffineRect();

	}
	else // 将数据传递到Gui控件上
	{	

		m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_LineColor = RGB(0,0,255);
		m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_nLineWidth = 1;


		m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_LineColor = RGB(0,0,255);
		m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam.m_nLineWidth = 1;

		// 粗定位scGui 设置
		SetTrainGuiParam_For_Rect(&m_GuiModelPoint,&m_GuiTrainRect,&m_GuiTrainAffineRect,&m_GuiSearchRect,m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam);
		m_GuiModelPoint2.SetLineColor(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_LineColor);
		m_GuiModelPoint2.SetLineWidth(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint2.SetCenterExtent(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint2,m_GuiModelPoint2.GetExtent());

		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_RoughSearchTrainGuiParam.m_ModelPoint1,m_GuiModelPoint.GetExtent());

		for (int it=0;it<RECT_NUMS;it++)
		{
			SetTrainGuiParam_For_Rect(&m_GuiModelPoint_Accurate[it],&m_GuiTrainRect_Accurate[it],&m_GuiTrainAffineRect_Accurate[it],&m_GuiSearchRect_Accurate[it],m_tmpTrainGuiParam.m_AccurateSearchTrainGuiParam[it]);
		}


		// 头尾
		SetTrainGuiParam_For_Rect(&m_GuiModelPoint_HeadTail,&m_GuiTrainRect_HeadTail,&m_GuiTrainAffineRect_HeadTail,&m_GuiSearchRect_HeadTail,m_tmpTrainGuiParam.m_HeadTail_Check_TrainGuiParam);

		// 正反
		SetTrainGuiParam_For_Rect(&m_GuiModelPoint_PN,&m_GuiTrainRect_PN,&m_GuiTrainAffineRect_PN,&m_GuiSearchRect_PN,m_tmpTrainGuiParam.m_PN_Check_TrainGuiParam);


		// 混料
		SetTrainGuiParam_For_Rect(&m_GuiModelPoint_Mixing,&m_GuiTrainRect_Mixing,&m_GuiTrainAffineRect_Mixing,&m_GuiSearchRect_Mixing,m_tmpTrainGuiParam.m_Mixing_Check_TrainGuiParam);
		for (int i=0; i<LINE_NUMS; i++)
		{
			m_GuiFindLine[i].SetLineColor(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_LineColor);
			m_GuiFindLine[i].SetLineWidth(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_nLineWidth);
			m_GuiFindLine[i].SetCaliperNum(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_nCaliperNum);
			m_GuiFindLine[i].SetStartXYEndXY(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dStartX, m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dStartY, m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dEndX,
				m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dEndY);
			m_GuiFindLine[i].SetCaliperProjectionLength(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dCaliperProjectLen);
			m_GuiFindLine[i].SetCaliperSearchLength(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_dCaliperSearchLen);
			m_GuiFindLine[i].SetCaliperSearchDirection(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_CaliperSearchRotation);

			m_GuiFindLine[i].SetMaskData(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_vdMaskData_FindLine);
			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bCaliperVisible);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_FindLineTrainGuiParam[i].m_bCaliperVisible);
		}


		m_GuiTrainBlobAffineRect.SetLineColor(m_tmpSearchGuiParam.m_LineColor);
		m_GuiTrainBlobAffineRect.SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		m_GuiTrainBlobAffineRect.SetAffineRect(m_tmpTrainGuiParam.m_BlobAffineRect);

	}

	return true;
}

bool CSpecialPickSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
		int j=0;

		for (j=0; j<m_pGuiSearchRectArray.size(); j++)
		{
			m_pGuiSearchRectArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchRectArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCrossArray.size(); j++)
		{
			m_pGuiSearchCrossArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchRectArray_HeadTail.size(); j++)
		{
			m_pGuiSearchRectArray_HeadTail[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchRectArray_HeadTail[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCrossArray_HeadTail.size(); j++)
		{
			m_pGuiSearchCrossArray_HeadTail[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArray_HeadTail[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			//m_pGuiSearchLineArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			//m_pGuiSearchLineArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiSearchLineCrossArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineCrossArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchLineCrossArray2.size(); j++)
		{
			m_pGuiSearchLineCrossArray2[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineCrossArray2[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}

	return true;
}

// 清除控件及其显示					
bool CSpecialPickSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CSpecialPickSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CSpecialPickSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);

	ClearGraphicsArray(m_pGuiSearchRectArray_HeadTail);
	ClearGraphicsArray(m_pGuiSearchCrossArray_HeadTail);

	ClearGraphicsArray(m_pGuiSearchBlobContour);
	ClearGraphicsArray(m_pGuiSearchBlobContour_Rough);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray2);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray3);
	ClearGraphicsArray(m_pGuiSearchLineTextArray);

	ClearGraphicsArray(m_pGuiSearchResult_Line_FitPointArray1);
	ClearGraphicsArray(m_pGuiSearchResult_Line_FitPointArray2);
	ClearGraphicsArray(m_pGuiSearchResult_Line_FitPointArray3);
	ClearGraphicsArray(m_pGuiSearchResult_Line_FitPointArray4);

	ClearGraphicsArray(m_pGuiSearchResult_Circle_FitPointArray1);
	ClearGraphicsArray(m_pGuiSearchResult_Circle_FitPointArray2);
	ClearGraphicsArray(m_pGuiSearchResult_Circle_FitPointArray3);
	ClearGraphicsArray(m_pGuiSearchResult_Circle_FitPointArray4);

	ClearGraphicsArray(m_pGuiSearchRectArray_Accurate);

	ClearGraphicsArray(m_pGuiSearchBlobArray);
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiBlobBoundCenterArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray_Blob);

	m_nResultErrNumber = 0;
	return true;
}

bool CSpecialPickSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect_Rough"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainAffRect_Rough"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint_Rough"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint_Rough2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect_Rough"));		

		for (int it = 0;it<RECT_NUMS;it++)
		{
			CString str_Tmp =_T("");
			str_Tmp.Format(_T("_TrainRect_Accurate%d"),it+1);
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+ str_Tmp);

			str_Tmp.Format(_T("_TrainAffRect_Accurate%d"),it+1);
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+ str_Tmp);

			str_Tmp.Format(_T("_ModelPoint_Accurate%d"),it+1);
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+ str_Tmp);

			str_Tmp.Format(_T("_SearchRect_Accurate%d"),it+1);
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+ str_Tmp);

		}

		for (int i=0; i<LINE_NUMS; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}

		// 头尾检查
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect_HeadTail"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainAffRect_HeadTail"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint_HeadTail"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect_HeadTail"));

		// 正反检查
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect_PN"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainAffRect_PN"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint_PN"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect_PN"));

		// 混料检查
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect_Mixing"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainAffRect_Mixing"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint_Mixing"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect_Mixing"));

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText + _T("_TrainBlobAffineRect"));


		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		for (int it = 0;it<LINE_NUMS;it++)
		{
			CString str_tmp = _T("");
			str_tmp.Format(_T("%s_TrainResultLine_%d"),m_strKeyText,it+1);
			m_pGuiStaticContainer->RemoveItem(str_tmp);
		}



	}

	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}

	if(m_pGuiTrainResultDisplay2 && ::IsWindow(m_hTrainResultDispWnd2))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay2->GetInteractiveContainer();
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain2"));
	}
	return true;
}

bool CSpecialPickSearchTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		for (j=0; j<m_pGuiSearchRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchRectArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArray[j]->GetTipText());
		}


		for (j=0; j<m_pGuiSearchRectArray_HeadTail.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchRectArray_HeadTail[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArray_HeadTail.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArray_HeadTail[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineTextArray.size();j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineTextArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineCrossArray2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray2[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineCrossArray3.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray3[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchRectArray_Accurate.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchRectArray_Accurate[j]->GetTipText());
		}


		// 找线
		for (j=0; j<m_pGuiSearchResult_Line_FitPointArray1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResult_Line_FitPointArray1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResult_Line_FitPointArray2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResult_Line_FitPointArray2[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResult_Line_FitPointArray3.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResult_Line_FitPointArray3[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResult_Line_FitPointArray4.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResult_Line_FitPointArray4[j]->GetTipText());
		}


		// 找圆
		for (j=0; j<m_pGuiSearchResult_Circle_FitPointArray1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResult_Circle_FitPointArray1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResult_Circle_FitPointArray2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResult_Circle_FitPointArray2[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResult_Circle_FitPointArray3.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResult_Circle_FitPointArray3[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResult_Circle_FitPointArray4.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResult_Circle_FitPointArray4[j]->GetTipText());
		}


		for (j=0; j<m_pGuiSearchAffineRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchAffineRectArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArray_Blob.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArray_Blob[j]->GetTipText());
		}


		for (j=0; j<m_pGuiBlobBoundCenterArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiBlobBoundCenterArray[j]->GetTipText());
		}


		for (j=0; j<m_pGuiSearchBlobArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchBlobArray[j]->GetTipText());
		}

		for (j=0;j<m_pGuiSearchBlobContour.size();j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchBlobContour[j]->GetTipText());
		}

		for (j=0;j<m_pGuiSearchBlobContour_Rough.size();j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchBlobContour_Rough[j]->GetTipText());
		}
		//m_pGuiStaticContainer->RemoveItem("blob result");

	}

	return true;
}

// 获取训练模板原始图像
bool CSpecialPickSearchTool::GetPatternImage(cpImage &ImagePattern)
{
	bool bRet = false;
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		/*bRet=m_PatternSearchTool_Rough.GetPatternImage(ImagePattern);*/
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool_Rough.GetTrainRectRegion()))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		/*bRet=m_QuickSearchTool_Rough.GetTrainModelImage(ImagePattern);*/
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool_Rough.GetTrainRectRegion()))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool)
	{
		bRet = true;
		ImagePattern = m_ImageTrain;
	}

	if (!bRet)
	{
		return false;
	}

	return true;
}

bool CSpecialPickSearchTool::GetPatternImage2(cpImage &ImagePattern)
{
	bool bRet = false;
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		/*bRet=m_PatternSearchTool_HeadTail.GetPatternImage(ImagePattern);*/
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool_HeadTail.GetTrainRectRegion()))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		/*bRet=m_QuickSearchTool_HeadTail.GetTrainModelImage(ImagePattern);*/
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool_HeadTail.GetTrainRectRegion()))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternBlobSearchTool)
	{
		bRet = true;
		ImagePattern = m_ImageTrain;
	}

	if (!bRet)
	{
		return false;
	}
	return false;
}
bool CSpecialPickSearchTool::GetPatternImage2(cpImage &ImagePattern,const int index)
{
	bool bRet = false;
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}
	if(index >= RECT_NUMS || index<0) return false;
	if (!m_tmpTrainDataParam.m_bEnableFourPoint && index!=0) return false;

	if (m_tmpTrainDataParam.GetAccurateSearchToolType() == ePatternSearchTool && m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		/*bRet=m_PatternSearchTool_Accurate[index].GetPatternImage(ImagePattern);*/
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool_Accurate[index].GetTrainRectRegion()))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}
	if (m_tmpTrainDataParam.GetAccurateSearchToolType() == eQuickSearchTool  && m_tmpTrainDataParam.m_bEnableAccurateLocation)
	{
		/*bRet=m_QuickSearchTool_Accurate[index].GetTrainModelImage(ImagePattern);*/
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool_Accurate[index].GetTrainRectRegion()))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}


	if (!bRet)
	{
		return false;
	}

	return true;
}	

// 恢复训练参数
bool CSpecialPickSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CSpecialPickSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CSpecialPickSearchTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Rough Train Area"));
		m_GuiSearchRect.SetLabel(_T("Rough Search Area"));
		m_GuiModelPoint.SetLabel(_T("Rough Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Rough Reference Point2"));
		m_GuiTrainAffineRect.SetLabel(_T("Rough Train Area"));


		m_GuiTrainRect_HeadTail.SetLabel(_T("HeadTail Train Area"));
		m_GuiTrainAffineRect_HeadTail.SetLabel(_T("HeadTail Train Area"));
		m_GuiSearchRect_HeadTail.SetLabel(_T("HeadTail Search Area"));

		m_GuiTrainRect_PN.SetLabel(_T("PN Train Area"));
		m_GuiTrainAffineRect_PN.SetLabel(_T("PN Train Area"));
		m_GuiSearchRect_PN.SetLabel(_T("PN Search Area"));

		m_GuiTrainRect_Mixing.SetLabel(_T("Mixing Train Area"));
		m_GuiTrainAffineRect_Mixing.SetLabel(_T("Mixing Train Area"));
		m_GuiSearchRect_Mixing.SetLabel(_T("Mixing Search Area"));


		for (int it =0;it<RECT_NUMS;it++)
		{
			CString strTmp = _T("");
			strTmp.Format(_T("Accurate Train Area%d"),it);
			m_GuiTrainRect_Accurate[it].SetLabel(strTmp);
			m_GuiTrainAffineRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("Accurate Search Area%d"),it);
			m_GuiSearchRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("Accurate Reference Point%d"),it);
			m_GuiModelPoint_Accurate[it].SetLabel(strTmp);
		}
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("粗定位训练区"));
		m_GuiSearchRect.SetLabel(_T("粗定位搜索区"));
		m_GuiModelPoint.SetLabel(_T("粗定位参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiTrainAffineRect.SetLabel(_T("粗定位训练区"));


		m_GuiTrainRect_HeadTail.SetLabel(_T("头尾检查训练区"));
		m_GuiTrainAffineRect_HeadTail.SetLabel(_T("头尾检查训练区"));
		m_GuiSearchRect_HeadTail.SetLabel(_T("头尾搜索搜索区"));

		m_GuiTrainRect_PN.SetLabel(_T("正反检查训练区"));
		m_GuiTrainAffineRect_PN.SetLabel(_T("正反检查训练区"));
		m_GuiSearchRect_PN.SetLabel(_T("正反检查搜索区"));

		m_GuiTrainRect_Mixing.SetLabel(_T("混料检查训练区"));
		m_GuiTrainAffineRect_Mixing.SetLabel(_T("混料检查训练区"));
		m_GuiSearchRect_Mixing.SetLabel(_T("混料检查搜索区"));

		for (int it =0;it<RECT_NUMS;it++)
		{
			CString strTmp = _T("");
			strTmp.Format(_T("精定位训练区%d"),it);
			m_GuiTrainRect_Accurate[it].SetLabel(strTmp);
			m_GuiTrainAffineRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("精定位搜索区%d"),it);
			m_GuiSearchRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("精定位参考点%d"),it);
			m_GuiModelPoint_Accurate[it].SetLabel(strTmp);
		}

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域"));

		m_GuiTrainRect_HeadTail.SetLabel(_T("头尾检查训练区"));
		m_GuiTrainAffineRect_HeadTail.SetLabel(_T("头尾检查训练区"));
		m_GuiSearchRect_HeadTail.SetLabel(_T("头尾搜索搜索区"));

		m_GuiTrainRect_PN.SetLabel(_T("正反检查训练区"));
		m_GuiTrainAffineRect_PN.SetLabel(_T("正反检查训练区"));
		m_GuiSearchRect_PN.SetLabel(_T("正反检查搜索区"));

		m_GuiTrainRect_Mixing.SetLabel(_T("混料检查训练区"));
		m_GuiTrainAffineRect_Mixing.SetLabel(_T("混料检查训练区"));
		m_GuiSearchRect_Mixing.SetLabel(_T("混料检查搜索区"));

		for (int it =0;it<RECT_NUMS;it++)
		{
			CString strTmp = _T("");
			strTmp.Format(_T("精定位训练区%d"),it);
			m_GuiTrainRect_Accurate[it].SetLabel(strTmp);
			m_GuiTrainAffineRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("精定位搜索区%d"),it);
			m_GuiSearchRect_Accurate[it].SetLabel(strTmp);
			strTmp.Format(_T("精定位参考点%d"),it);
			m_GuiModelPoint_Accurate[it].SetLabel(strTmp);
		}

		break;
	}

	for (int i=0; i<LINE_NUMS; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d"), i+1);
			break;
		case 1:
			str.Format(_T("找线工具%d"), i+1);
			break;
		default:
			str.Format(_T("找线工具%d"), i+1);
			break;
		}

		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);
	}

	for (int i=0; i<CIRCLE_NUMS; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindCircle%d"), i+1);
			break;
		case 1:
			str.Format(_T("找圆工具%d"), i+1);
			break;
		default:
			str.Format(_T("找圆工具%d"), i+1);
			break;
		}

		m_GuiFindCircle[i].SetLabel(str);
		m_GuiFindCircle[i].SetLabelVisible(TRUE);
	}
}

void CSpecialPickSearchTool::ShowFitPoint(std::vector<scGuiGraphic*> & m_pGuiInspectFitPointArray,scGuiCoordCross m_GuiInspectFitPoint[MAXPTSNUM],scFindLine* m_FindLineTool,const CString strFitPoint)
{
	std::vector<bool> bUsed;
	std::vector<sc2Vector> points;
	m_FindLineTool->GetUsed(bUsed);
	m_FindLineTool->GetFitData(points);

	for (long m=0; m<bUsed.size(); m++)
	{
		bool bUse = bUsed[m];
		sc2Vector pt = points[m];
		CString strTmpKey = _T("");
		int nIndex = m_pGuiInspectFitPointArray.size();
		strTmpKey = m_strKeyText + strFitPoint;
		CString tmpIndex = _T("");
		tmpIndex.Format(_T("_%d"), nIndex);
		strTmpKey += tmpIndex;

		scGuiCoordCross *pTempFitPoint1 = m_GuiInspectFitPoint + nIndex;
		pTempFitPoint1->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pTempFitPoint1->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pTempFitPoint1->SetRulerVisible(false);
		pTempFitPoint1->SetCenterRotationLengths(pt,scRadian(scDegree(45)),13,13);
		if (bUse)
		{
			pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint);
		}
		else
		{
			pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint);
		}
		pTempFitPoint1->SetTipTextEnabled(true);
		pTempFitPoint1->SetTipText(strTmpKey);
		m_pGuiInspectFitPointArray.push_back(pTempFitPoint1);	
	}
}

CString CSpecialPickSearchTool::GetResultErrInfo()
{
	CString strInfo = _T("");
	switch(m_iLanguage)
	{
	case 0:
		{
			switch(m_nResultErrNumber)
			{
			case -1:
				{
					strInfo.Format(_T("Err:%d,Rough search failed"), m_nResultErrNumber);
				}
				break;
			case -2:
				{
					strInfo.Format(_T("Err:%d,Blob execute failed"), m_nResultErrNumber);
				}
				break;
			case -3:
				{
					strInfo.Format(_T("Err:%d,Blob analysis failed"), m_nResultErrNumber);
				}
				break;
			case -11:
				{
					strInfo.Format(_T("Err:%d,Find line1 failed"),m_nResultErrNumber);
				}
				break;
			case -12:
				{
					strInfo.Format(_T("Err:%d,Find line2 failed"),m_nResultErrNumber);
				}
				break;
			case -13:
				{
					strInfo.Format(_T("Err:%d,Find line3 failed"), m_nResultErrNumber);
				}
				break;
			case -14:
				{
					strInfo.Format(_T("Err:%d,Find line3 failed"),m_nResultErrNumber);
				}
				break;
			case -15:
				{
					strInfo.Format(_T("Err:%d,Find intersect points of 4 lines failed"),m_nResultErrNumber);
				}
				break;
			case -20:
				{
					strInfo.Format(_T("Err:%d,Blob search failed"), m_nResultErrNumber);
				}
				break;
			case -21:
				{
					strInfo.Format(_T("Err:%d,Blob analysis failed"), m_nResultErrNumber);
				}
				break;
			case -22:
				{
					strInfo.Format(_T("Err:%d,Blob num failed"), m_nResultErrNumber);
				}
				break;
			case -40:
				{
					strInfo.Format(_T("Err:%d, Head Tail check error"), m_nResultErrNumber);
				}
				break;
			default:
				strInfo.Format(_T("Err:%d"), m_nResultErrNumber);
				break;
			}
		}
		break;
	case 1:
		{
			switch(m_nResultErrNumber)
			{
			case -1:
				{
					strInfo.Format(_T("Err:%d,粗定位失败"), m_nResultErrNumber);
				}
				break;
			case -2:
				{
					strInfo.Format(_T("Err:%d,Blob执行失败"), m_nResultErrNumber);
				}
				break;
			case -3:
				{
					strInfo.Format(_T("Err:%d,Blob分析失败"), m_nResultErrNumber);
				}
				break;
			case -11:
				{
					strInfo.Format(_T("Err:%d,找线1失败"),m_nResultErrNumber);
				}
				break;
			case -12:
				{
					strInfo.Format(_T("Err:%d,找线2失败"),m_nResultErrNumber);
				}
				break;
			case -13:
				{
					strInfo.Format(_T("Err:%d,找线3失败"), m_nResultErrNumber);
				}
				break;
			case -14:
				{
					strInfo.Format(_T("Err:%d,找线4失败"),m_nResultErrNumber);
				}
				break;
			case -15:
				{
					strInfo.Format(_T("Err:%d,四线交点失败"),m_nResultErrNumber);
				}
				break;
			case -20:
				{
					strInfo.Format(_T("Err:%d,Blob搜索失败"), m_nResultErrNumber);
				}
				break;
			case -21:
				{
					strInfo.Format(_T("Err:%d,Blob分析失败"), m_nResultErrNumber);
				}
				break;
			case -22:
				{
					strInfo.Format(_T("Err:%d,Blob个数失败"), m_nResultErrNumber);
				}
				break;
			case -40:
				{
					strInfo.Format(_T("Err:%d,头尾检查"), m_nResultErrNumber);
				}
				break;
			case  -99:
				{
					strInfo.Format(_T("Err:%d,训练失败"),m_nResultErrNumber);
				}
				break;
			default:
				strInfo.Format(_T("Err:%d"), m_nResultErrNumber);
				break;
			}
		}
		break;
	default:
		{
			switch(m_nResultErrNumber)
			{
			case -1:
				{
					strInfo.Format(_T("Err:%d,粗定位失败"), m_nResultErrNumber);
				}
				break;
			case -2:
				{
					strInfo.Format(_T("Err:%d,Blob执行失败"), m_nResultErrNumber);
				}
				break;
			case -3:
				{
					strInfo.Format(_T("Err:%d,Blob分析失败"), m_nResultErrNumber);
				}
				break;
			case -11:
				{
					strInfo.Format(_T("Err:%d,找线1失败"),m_nResultErrNumber);
				}
				break;
			case -12:
				{
					strInfo.Format(_T("Err:%d,找线2失败"),m_nResultErrNumber);
				}
				break;
			case -13:
				{
					strInfo.Format(_T("Err:%d,找线3失败"), m_nResultErrNumber);
				}
				break;
			case -14:
				{
					strInfo.Format(_T("Err:%d,找线4失败"),m_nResultErrNumber);
				}
				break;
			case -15:
				{
					strInfo.Format(_T("Err:%d,四线交点失败"),m_nResultErrNumber);
				}
				break;
			case -20:
				{
					strInfo.Format(_T("Err:%d,Blob搜索失败"), m_nResultErrNumber);
				}
				break;
			case -21:
				{
					strInfo.Format(_T("Err:%d,Blob分析失败"), m_nResultErrNumber);
				}
				break;
			case -22:
				{
					strInfo.Format(_T("Err:%d,Blob个数失败"), m_nResultErrNumber);
				}
				break;
			case -40:
				{
					strInfo.Format(_T("Err:%d,头尾检查"), m_nResultErrNumber);
				}
				break;
			case  -99:
				{
					strInfo.Format(_T("Err:%d,训练失败"),m_nResultErrNumber);
				}
				break;
			default:
				strInfo.Format(_T("Err:%d"), m_nResultErrNumber);
				break;
			}
		}
		break;
	}

	return strInfo;
}

bool CSpecialPickSearchTool::ExecutePatternSerach(cpImage &ImageSearch,scPatternSearchRot* m_PatternSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dConfusionThre,double dTransX,double dTransY,double dRotation)
{
	bool bRet = false;
	if(NULL == m_PatternSearchTool || NULL == m_SearchRect)
	{
		return bRet;
	}

	if (!m_PatternSearchTool->SetAcceptNum(m_nSearchNum))
	{
		return bRet;
	}

	// 设置接收阈值
	if (!m_PatternSearchTool->SetAcceptThreshold(m_dAcceptThre))
	{
		return bRet;
	}

	if (!m_PatternSearchTool->SetConfusionThreshold(m_dConfusionThre>1.0 ?1.0:m_dConfusionThre))
	{
		return bRet;
	}

	double dTransCenterX, dTransCenterY;
	double dOriCenterX, dOriCenterY;
	dOriCenterX = m_SearchRect->GetOrigin().GetX();
	dOriCenterY = m_SearchRect->GetOrigin().GetY();

	dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
	dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;
	scRect tmpSearch = *m_SearchRect;
	tmpSearch.SetOrigion(sc2Vector(dTransCenterX,dTransCenterY));
	bRet = m_PatternSearchTool->Execute(ImageSearch,tmpSearch);
	return bRet;
}

bool CSpecialPickSearchTool::ExecuteQuickSearch(cpImage &ImageSearch,scQuickSearchTool* m_QuickSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dAngleLow,double m_dAngleHigh,double m_dSizeLow1,double m_dSizeHigh1,double dTransX,double dTransY,double dRotation)
{
	bool bRet = false;
	if(NULL == m_QuickSearchTool || NULL == m_SearchRect)
	{
		return bRet;
	}
	// 设置搜索个数
	if (!m_QuickSearchTool->SetNumToFind(m_nSearchNum))
	{
		return bRet;
	}

	// 设置接收阈值
	if (!m_QuickSearchTool->SetAcceptThreshold(m_dAcceptThre))
	{
		return bRet;
	}

	// 设置搜索角度范围
	if (!m_QuickSearchTool->SetDofZoneRotate(scDegree(m_dAngleLow), scDegree(m_dAngleHigh), scDegree(360)))
	{
		return bRet;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool->SetDofNominalSize(1.0))
	{
		return bRet;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool->SetDofZoneSize(m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dSizeLow,m_tmpSearchDataParam.m_Struct_Rough_QuickSearchSearchData.m_dSizeHigh,5))
	{
		return bRet;
	}


	double dTransCenterX, dTransCenterY;
	double dOriCenterX, dOriCenterY;
	dOriCenterX = m_SearchRect->GetOrigin().GetX();
	dOriCenterY = m_SearchRect->GetOrigin().GetY();


	dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
	dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;
	scRect tmpSearch = *m_SearchRect;
	tmpSearch.SetOrigion(sc2Vector(dTransCenterX,dTransCenterY));
	// 执行搜索
	bRet = m_QuickSearchTool->Execute(ImageSearch, tmpSearch);
	return bRet;
}

bool CSpecialPickSearchTool::ExecuteFindLine(cpImage &ImageSearch,scFindLine* m_FindLineTool,StructFindLineTrainData2 m_sFindLineTrainData,double dTransX,double dTransY,double dRotation,scLineSeg &ResultLine)
{
	bool bRet = false;
	if(NULL == m_FindLineTool)
	{
		return bRet;
	}

	m_FindLineTool->SetCaliperNum(m_sFindLineTrainData.m_nCaliperNum);
	/*m_FindLineTool->SetCaliperParams(m_sFindLineTrainData.m_nCaliperNum, m_sFindLineTrainData.m_CaliperSearchRotation, m_sFindLineTrainData.m_dCaliperSearchLen
	,m_sFindLineTrainData.m_dCaliperProjectLen);*/

	if (m_sFindLineTrainData.m_bEnableDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_sFindLineTrainData.m_edgeProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_sFindLineTrainData.m_edgeProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindLineTool->SetCaliperProperty(m_sFindLineTrainData.m_dContrastThre, m_sFindLineTrainData.m_edgeProperty,tmpEdgePolarity, m_sFindLineTrainData.m_lFilterHalfWidth);
	}
	else
	{
		m_FindLineTool->SetCaliperProperty(m_sFindLineTrainData.m_dContrastThre, m_sFindLineTrainData.m_edgeProperty, m_sFindLineTrainData.m_lFilterHalfWidth);
	}

	m_FindLineTool->SetOutLineNum(m_sFindLineTrainData.m_nOutLineNum, eRansacRobust);
	m_FindLineTool->EnableCaliperSortByScore(m_sFindLineTrainData.m_bSortByScore);
	m_FindLineTool->SetCaliperMasked(m_sFindLineTrainData.m_vbCaliperMask_FindLine);

	double dTransStartX, dTransStartY;
	double dTransEndX, dTransEndY;

	dTransStartX = cos(dRotation) * m_sFindLineTrainData.m_dStartX  - sin(dRotation) * m_sFindLineTrainData.m_dStartY + dTransX;
	dTransStartY = sin(dRotation) * m_sFindLineTrainData.m_dStartX + cos(dRotation) * m_sFindLineTrainData.m_dStartY + dTransY;
	dTransEndX = cos(dRotation) * m_sFindLineTrainData.m_dEndX  - sin(dRotation) * m_sFindLineTrainData.m_dEndY + dTransX;
	dTransEndY = sin(dRotation) * m_sFindLineTrainData.m_dEndX  + cos(dRotation) * m_sFindLineTrainData.m_dEndY + dTransY;

	m_FindLineTool->SetCaliperParams(m_sFindLineTrainData.m_nCaliperNum,m_sFindLineTrainData.m_CaliperSearchRotation, m_sFindLineTrainData.m_dCaliperSearchLen
		,m_sFindLineTrainData.m_dCaliperProjectLen);
	m_FindLineTool->SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);

	double dRMS;
	bRet = m_FindLineTool->Execute(ImageSearch, ResultLine, dRMS);
	return bRet;
}

bool CSpecialPickSearchTool::GetFindLineCenterPoint(scFindLine* m_FindLineTool,sc2Vector &pointCenter2)
{
	bool bRet = false;
	if(NULL == m_FindLineTool)
	{
		return bRet;
	}

	sc2Vector pointS;
	sc2Vector pointE;

	svStd vector<sc2Vector> pointResult;	// 探测点
	svStd vector<bool> used;				// 参与拟合标志

	m_FindLineTool->GetFitData(pointResult);
	m_FindLineTool->GetUsed(used);

	int nNum = pointResult.size();
	if (nNum > 0)
	{
		int i = 0;
		svStd vector<sc2Vector>::iterator iter;
		for( iter = pointResult.begin(), i = 0; iter != pointResult.end(); iter++, i++)
		{
			if (used.at(i) == true)
			{
				pointS = pointResult.at(i);
				break;
			}

		}
		for( iter = pointResult.begin(), i = 0; iter != pointResult.end(); iter++, i++)
		{
			if (used.at(i) == true)
			{
				pointE = pointResult.at(i);
			}

		}
		pointCenter2 = (pointS+pointE)/2;
		bRet = true;
	}
	else
	{
		return bRet;
	}
	return bRet;
}

void CSpecialPickSearchTool::ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,COLORREF color,int LineWidth)
{
	if(NULL == m_AffineRect) return;
	//显示矩形
	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strRect;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+nOffset;
	pTempAffineRect->SetAffineRect(*m_AffineRect);
	pTempAffineRect->SetLineColor(color);
	pTempAffineRect->SetLineWidth(LineWidth);
	pTempAffineRect->SetTipTextEnabled(true);
	pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
	m_pGuiSearchRectArray.push_back(pTempAffineRect);
}

void CSpecialPickSearchTool::ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,const CString strRectLabel,COLORREF color,int LineWidth)
{
	if(NULL == m_AffineRect) return;
	//显示矩形
	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strRect;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+nOffset;
	pTempAffineRect->SetAffineRect(*m_AffineRect);
	pTempAffineRect->SetLineColor(color);
	pTempAffineRect->SetLineWidth(LineWidth);
	pTempAffineRect->SetTipTextEnabled(true);
	pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);

	pTempAffineRect->SetLabel(strRectLabel);
	pTempAffineRect->SetLabelVisible(TRUE);
	m_pGuiSearchRectArray.push_back(pTempAffineRect);
}


void CSpecialPickSearchTool::ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM*4],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,COLORREF color,int LineWidth)
{
	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strCross;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiCross *pTempCross = m_GuiSearchCross+nOffset;
	pTempCross->SetCenterExtent(m_CrossVector, sc2Vector(40,40));
	pTempCross->SetLineColor(color);
	pTempCross->SetLineWidth(LineWidth);
	pTempCross->SetTipTextEnabled(true);
	pTempCross->SetTipText(strTmpKey);
	m_pGuiSearchCrossArray.push_back(pTempCross);
}

void CSpecialPickSearchTool::ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM*4],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,const CString strCrossLabel,COLORREF color,int LineWidth)
{
	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strCross;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiCross *pTempCross = m_GuiSearchCross+nOffset;
	pTempCross->SetCenterExtent(m_CrossVector, sc2Vector(40,40));
	pTempCross->SetLineColor(color);
	pTempCross->SetLineWidth(LineWidth);
	pTempCross->SetTipTextEnabled(true);
	pTempCross->SetTipText(strTmpKey);

	pTempCross->SetLabel(strCrossLabel);
	pTempCross->SetLabelVisible(TRUE);

	m_pGuiSearchCrossArray.push_back(pTempCross);
}
void CSpecialPickSearchTool::ShowResultLine(std::vector<scGuiGraphic*>& m_pGuiSearchLineArray,scGuiLine m_GuiSearchLine[MAXRESNUM],const int nOffset,scLine *m_scLine,const CString strLine,COLORREF color,int LineWidth)
{
	if(NULL == m_scLine) return;

	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strLine;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiLine *pTemLine = m_GuiSearchLine + nOffset;
	pTemLine->SetLine(*m_scLine);
	pTemLine->SetLineColor(color);
	pTemLine->SetLineWidth(LineWidth);
	pTemLine->SetTipTextEnabled(true);
	pTemLine->SetTipText(strTmpKey);
	m_pGuiSearchLineArray.push_back(pTemLine);

}



bool CSpecialPickSearchTool::GetTrainResultImage_PN(cpImage &ImageTrainResult)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == eQuickSearchTool)
	{
		bRet = m_QuickSearchTool_PN.GetTrainResultImage(m_ImageTrainResult_PN);
	}
	if (m_tmpTrainDataParam.GetPositiveAndNegativeCheckToolType() == ePatternSearchTool)
	{
		bRet = m_PatternSearchTool_PN.GetPatternImage(m_ImageTrainResult_PN);
	}


	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult_PN;

	return true;
}

bool CSpecialPickSearchTool::GetTrainResultImage_Mixing(cpImage &ImageTrainResult)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetMixingCheckToolType() == eQuickSearchTool)
	{
		bRet = m_QuickSearchTool_Mixing.GetTrainResultImage(m_ImageTrainResult_Mixing);
	}
	if (m_tmpTrainDataParam.GetMixingCheckToolType() == ePatternSearchTool)
	{
		bRet = m_PatternSearchTool_Mixing.GetPatternImage(m_ImageTrainResult_Mixing);
	}


	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult_Mixing;

	return true;
}

BOOL CSpecialPickSearchTool::ExeHistogramHeadTail(double X1,double Y1,double D,double X2,double Y2,int i,bool &IsHead, cpImage &ImageSearch)
{
	// 对矩形长边两侧的一定区域进行灰度分析；
	//构造两个小矩形，沿着blob角度方向，长度为50个像素，宽度为100个像素，计算两个小矩形的平均灰度值，灰度值小的为圆孔边

	double dXLenHead=(double) (m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadLength);
	double dYLenHead =(double) (m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadWidth);

	double dXOffSetHead = m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadXoffset;
	double dYOffSetHead = m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadYoffset;

	double dXLenTail=(double) (m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nTailLength);
	double dYLenTail =(double) (m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nTailWidth);

	double dXOffSetTail = m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_dTailXoffset;
	double dYOffSetTail = m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_dTailYoffset;

	scAffineRect rectHead;
	scAffineRect rectTail;

	double dMeanHead = 0.0;
	double dMeanTail = 0.0;

	double dCosa = cos((scRadian(scDegree(D))).ToDouble());
	double dSina = sin((scRadian(scDegree(D))).ToDouble());

	{

		sc2Vector p22;

		double tmpX = (X1-dXOffSetHead*dCosa-dYOffSetHead*dSina>0)?(X1-dXOffSetHead*dCosa-dYOffSetHead*dSina):0;
		double tmpY = (Y1- dXOffSetHead*dSina+dYOffSetHead*dCosa>0)?(Y1- dXOffSetHead*dSina+dYOffSetHead*dCosa):0;
		p22.SetX(tmpX);
		p22.SetY(tmpY);


		rectHead.SetCenterLengthsRotAndSkew(p22,dXLenHead,dYLenHead,scRadian(scDegree(D)),scRadian(0));


		sc2Vector p33;
		p33.SetX(X2-dXOffSetTail*dCosa-dYOffSetTail*dSina);
		p33.SetY(Y2-dXOffSetTail*dSina+dYOffSetTail*dCosa);

		rectTail.SetCenterLengthsRotAndSkew(p33,dXLenTail,dYLenTail,scRadian(scDegree(D)),scRadian(0));



		svStd vector<long> HistogramResult1(256, 0);
		svStd vector<long> HistogramResult2(256, 0);

		HStatus nHStatus = sfHistogram(ImageSearch,rectHead, HistogramResult1);
		if(nHStatus	!= HSuccess)
		{
			return FALSE;
		}

		nHStatus = sfHistogram(ImageSearch,rectTail, HistogramResult2);
		if(nHStatus	!= HSuccess)
		{
			return FALSE;
		}

		scHistoStats* pHistoStats1 = new scHistoStats(HistogramResult1);
		if(pHistoStats1->IsValid())
		{
			dMeanHead	= pHistoStats1->Mean();
		}
		else
		{
			return FALSE;
		}

		scHistoStats* pHistoStats2 = new scHistoStats(HistogramResult2);
		if(pHistoStats2->IsValid())
		{
			dMeanTail	= pHistoStats2->Mean();
		}
		else
		{
			return FALSE;
		}

		if(pHistoStats1!=NULL)
		{
			delete pHistoStats1;
			pHistoStats1 = NULL;
		}
		if(pHistoStats2!=NULL)
		{
			delete pHistoStats2;
			pHistoStats2 = NULL;
		}


		CString  strTmpKey=_T("");
		strTmpKey.Format(_T("灰度值%.1f"),dMeanHead);
		ShowResultRect(m_pGuiSearchRectArray_HeadTail,m_GuiSearchAffineRect_HeadTail,2*i,&rectHead,_T("_ResultRect_HeadTail_His"),strTmpKey,RGB(255,0,255),m_tmpSearchGuiParam.m_nLineWidth);
		strTmpKey.Format(_T("灰度值%.1f"),dMeanTail);
		ShowResultRect(m_pGuiSearchRectArray_HeadTail,m_GuiSearchAffineRect_HeadTail,2*i+1,&rectTail,_T("_ResultRect_HeadTail_His"),strTmpKey,RGB(0,255,255),m_tmpSearchGuiParam.m_nLineWidth);



		if (m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_MeanHighHead ==0)
		{
			if (dMeanHead>dMeanTail)
			{
				m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadGreyValue = dMeanHead;
				m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nTailGreyValue = dMeanTail;
				IsHead = true;
			}
			else
			{
				m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadGreyValue = dMeanTail;
				m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nTailGreyValue = dMeanHead;
				IsHead = false;
			}
		}
		else if (m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_MeanHighHead ==1)
		{
			if (dMeanHead<dMeanTail)
			{
				m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadGreyValue = dMeanHead;
				m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nTailGreyValue = dMeanTail;
				IsHead = true;
			}
			else
			{
				m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadGreyValue = dMeanTail;
				m_tmpTrainDataParam.m_Struct_HeadTail_HistogramSearchTrainData.m_nTailGreyValue = dMeanHead;
				IsHead = false;
			}
		}
	}
	return TRUE;
}

//CString CSpecialPickSearchTool::GetResultErrInfo()
//{
//	CString strErrInfo;
//	strErrInfo.Format(_T("Err:%d"), GetResultErrNumber());
//
//	switch(m_iLanguage)
//	{
//	case 0:
//		{
//			switch (m_nResultErrNumber)
//			{
//			case -1:							
//				strErrInfo += _T("ToughSearch：Pattern Search Fail");
//				break;
//			case -2:							
//				strErrInfo += _T("ToughSearch：Quick Search Fail");
//				break;
//			case -3:							
//				strErrInfo += _T("ToughSearch：Blob Search Fail");
//				break;
//			case -4:							
//				strErrInfo += _T("AccurateSearch：Find Line1 Fail");
//				break;
//			case -5:							
//				strErrInfo += _T("AccurateSearch：Find Line2 Fail");
//				break;
//			case -6:							
//				strErrInfo += _T("AccurateSearch：Find Line3 Fail");
//				break;
//			case -7:							
//				strErrInfo += _T("AccurateSearch：Find Line4 Fail");
//				break;
//			case -8:							
//				strErrInfo += _T("AccurateSearch：Bolb Search Fail");
//				break;
//			case -9:							
//				strErrInfo += _T("HeadTailInspect：Pattern Search Fail");
//				break;
//			case -10:							
//				strErrInfo += _T("HeadTailInspect：Quick Search Fail");
//				break;
//			case -11:							
//				strErrInfo += _T("HeadTailInspect：GrayLevelHistogram Search Fail");
//				break;
//			case -99:			//	训练未成功			
//				strErrInfo += _T("The Train is NG");
//				break;
//			default:
//				strErrInfo += _T(" ");
//				break;
//			}
//		}
//		break;
//	case 1:
//		{
//			switch (m_nResultErrNumber)
//			{
//			case -1:							
//				strErrInfo += _T("粗定位：区域定位失败");
//				break;
//			case -2:							
//				strErrInfo += _T("粗定位：几何定位失败");
//				break;
//			case -3:							
//				strErrInfo += _T("粗定位：Blob定位失败");
//				break;
//			case -4:							
//				strErrInfo += _T("精定位：找线1失败");
//				break;
//			case -5:							
//				strErrInfo += _T("精定位：找线2失败");
//				break;
//			case -6:							
//				strErrInfo += _T("精定位：找线3失败");
//				break;
//			case -7:							
//				strErrInfo += _T("精定位：找线4失败");
//				break;
//			case -8:							
//				strErrInfo += _T("精定位：Blob定位失败");
//				break;
//			case -9:							
//				strErrInfo += _T("头尾检测：区域定位失败");
//				break;
//			case -10:							
//				strErrInfo += _T("头尾检测：几何定位失败");
//				break;
//			case -11:							
//				strErrInfo += _T("头尾检测：灰度直方图定位失败");
//				break;
//			case -99:							
//				strErrInfo += _T("训练未成功");
//				break;
//			default:
//				strErrInfo += _T(" ");
//				break;
//			}
//		}
//		break;
//	default:
//		{
//			switch (m_nResultErrNumber)
//			{
//			case -1:							
//				strErrInfo += _T("粗定位：区域定位失败");
//				break;
//			case -2:							
//				strErrInfo += _T("粗定位：几何定位失败");
//				break;
//			case -3:							
//				strErrInfo += _T("粗定位：Blob定位失败");
//				break;
//			case -4:							
//				strErrInfo += _T("精定位：找线1失败");
//				break;
//			case -5:							
//				strErrInfo += _T("精定位：找线2失败");
//				break;
//			case -6:							
//				strErrInfo += _T("精定位：找线3失败");
//				break;
//			case -7:							
//				strErrInfo += _T("精定位：找线4失败");
//				break;
//			case -8:							
//				strErrInfo += _T("精定位：Blob定位失败");
//				break;
//			case -9:							
//				strErrInfo += _T("头尾检测：区域定位失败");
//				break;
//			case -10:							
//				strErrInfo += _T("头尾检测：几何定位失败");
//				break;
//			case -11:							
//				strErrInfo += _T("头尾检测：灰度直方图定位失败");
//				break;
//			case -99:							
//				strErrInfo += _T("训练未成功");
//				break;
//			default:
//				strErrInfo += _T(" ");
//				break;
//			}
//		}
//		break;
//	}
//	return strErrInfo;
//}