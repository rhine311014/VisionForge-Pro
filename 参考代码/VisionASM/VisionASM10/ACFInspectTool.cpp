#include "stdafx.h"
#include "ACFInspectTool.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "Shlwapi.h" 
#include "svTimer.h"
#include "svBlob.h"
#include "svBlobSceneDescription.h"
#include "svHistogram.h"
#include "svImageSharpness.h"
#include "svIntersect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void tagACFInspectTrainDataParam::SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
{
	CString strInfo = _T("");

	strInfo.Format(_T("%d"), GetRoughSearchToolType());

	if(FALSE == cXMLConfigurator.FindElem(_T("RoughSearchToolType")))
	{
		cXMLConfigurator.AddElem(_T("RoughSearchToolType"));
	}
	strInfo.Format(_T("%d"), m_Rough_SearchToolType);
	cXMLConfigurator.SetElemData(strInfo);


	if(FALSE == cXMLConfigurator.FindElem(_T("CoarseScale")))
	{
		cXMLConfigurator.AddElem(_T("CoarseScale"));
	}
	cXMLConfigurator.SetElemData(m_CoarseScale);

	//ContrastThre
	if(FALSE == cXMLConfigurator.FindElem(_T("CoarseScale")))
	{
		cXMLConfigurator.AddElem(_T("CoarseScale"));
	}
	cXMLConfigurator.SetElemData(m_CoarseScale);


	if(FALSE == cXMLConfigurator.FindElem(_T("Accuracy")))
	{
		cXMLConfigurator.AddElem(_T("Accuracy"));
	}
	cXMLConfigurator.SetElemData(m_Accuracy);

	if(FALSE == cXMLConfigurator.FindElem(_T("dAngleLow")))
	{
		cXMLConfigurator.AddElem(_T("dAngleLow"));
	}
	cXMLConfigurator.SetElemData(m_dAngleLow);

	if(FALSE == cXMLConfigurator.FindElem(_T("dAngleHigh")))
	{
		cXMLConfigurator.AddElem(_T("dAngleHigh"));
	}
	cXMLConfigurator.SetElemData(m_dAngleHigh);

	if(FALSE == cXMLConfigurator.FindElem(_T("bMaskEnable")))
	{
		cXMLConfigurator.AddElem(_T("bMaskEnable"));
	}
	cXMLConfigurator.SetElemData(m_bMaskEnable);

	if(FALSE == cXMLConfigurator.FindElem(_T("dGrau")))
	{
		cXMLConfigurator.AddElem(_T("dGrau"));
	}
	cXMLConfigurator.SetElemData(m_dGrau);

	if(FALSE == cXMLConfigurator.FindElem(_T("lNoiseThre")))
	{
		cXMLConfigurator.AddElem(_T("lNoiseThre"));
	}
	cXMLConfigurator.SetElemData(m_lNoiseThre);

	if(FALSE == cXMLConfigurator.FindElem(_T("lTotalMag")))
	{
		cXMLConfigurator.AddElem(_T("lTotalMag"));
	}
	cXMLConfigurator.SetElemData(m_lTotalMag);

	// 找线工具START
	if(FALSE == cXMLConfigurator.FindElem(_T("ContrastThre1")))
	{
		cXMLConfigurator.AddElem(_T("ContrastThre1"));
	}
	cXMLConfigurator.SetElemData(m_dContrastThre[0]);

	if(FALSE == cXMLConfigurator.FindElem(_T("FilterHalfWidth1")))
	{
		cXMLConfigurator.AddElem(_T("FilterHalfWidth1"));
	}
	cXMLConfigurator.SetElemData(m_lFilterHalfWidth[0]);

	if(FALSE == cXMLConfigurator.FindElem(_T("OutLineNum1")))
	{
		cXMLConfigurator.AddElem(_T("OutLineNum1"));
	}
	cXMLConfigurator.SetElemData(m_nOutLineNum[0]);

	if(FALSE == cXMLConfigurator.FindElem(_T("EdgeProperty1")))
	{
		cXMLConfigurator.AddElem(_T("EdgeProperty1"));
	}
	cXMLConfigurator.SetElemData(m_edgeProperty[0]);

	if(FALSE == cXMLConfigurator.FindElem(_T("SortByScore1")))
	{
		cXMLConfigurator.AddElem(_T("SortByScore1"));
	}
	cXMLConfigurator.SetElemData(m_bSortByScore[0]);

	if(FALSE == cXMLConfigurator.FindElem(_T("DualEdge1")))
	{
		cXMLConfigurator.AddElem(_T("DualEdge1"));
	}
	cXMLConfigurator.SetElemData(m_bEnableDualEdge[0]);



	if(FALSE == cXMLConfigurator.FindElem(_T("ContrastThre2")))
	{
		cXMLConfigurator.AddElem(_T("ContrastThre2"));
	}
	cXMLConfigurator.SetElemData(m_dContrastThre[1]);

	if(FALSE == cXMLConfigurator.FindElem(_T("FilterHalfWidth2")))
	{
		cXMLConfigurator.AddElem(_T("FilterHalfWidth2"));
	}
	cXMLConfigurator.SetElemData(m_lFilterHalfWidth[1]);

	if(FALSE == cXMLConfigurator.FindElem(_T("OutLineNum2")))
	{
		cXMLConfigurator.AddElem(_T("OutLineNum2"));
	}
	cXMLConfigurator.SetElemData(m_nOutLineNum[1]);

	if(FALSE == cXMLConfigurator.FindElem(_T("EdgeProperty2")))
	{
		cXMLConfigurator.AddElem(_T("EdgeProperty2"));
	}
	cXMLConfigurator.SetElemData(m_edgeProperty[1]);

	if(FALSE == cXMLConfigurator.FindElem(_T("SortByScore2")))
	{
		cXMLConfigurator.AddElem(_T("SortByScore2"));
	}
	cXMLConfigurator.SetElemData(m_bSortByScore[1]);

	if(FALSE == cXMLConfigurator.FindElem(_T("DualEdge2")))
	{
		cXMLConfigurator.AddElem(_T("DualEdge2"));
	}
	cXMLConfigurator.SetElemData(m_bEnableDualEdge[1]);






	if(FALSE == cXMLConfigurator.FindElem(_T("EnableBaseSearch")))
	{
		cXMLConfigurator.AddElem(_T("EnableBaseSearch"));
	}
	cXMLConfigurator.SetElemData(m_bEnableBaseSearch);

	if(FALSE == cXMLConfigurator.FindElem(_T("nInsAreaNum")))
	{
		cXMLConfigurator.AddElem(_T("nInsAreaNum"));
	}
	cXMLConfigurator.SetElemData(m_nInsAreaNum);

	for (int i=0;i<m_nInsAreaNum;i++)
	{
		strInfo.Format(_T("InsAreaList%d"),i);
		if(FALSE == cXMLConfigurator.FindElem(strInfo))
		{
			cXMLConfigurator.AddElem(strInfo);
		}

		cXMLConfigurator.EnterElem();
		{
			if(FALSE == cXMLConfigurator.FindElem(_T("DefectType")))
			{
				cXMLConfigurator.AddElem(_T("DefectType"));
			}
			cXMLConfigurator.SetElemData(m_nDefectType.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("BinaryThre")))
			{
				cXMLConfigurator.AddElem(_T("BinaryThre"));
			}
			cXMLConfigurator.SetElemData(m_nBinaryThre.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("MinAcceptArea")))
			{
				cXMLConfigurator.AddElem(_T("MinAcceptArea"));
			}
			cXMLConfigurator.SetElemData(m_nMinAcceptArea.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("MaxAcceptArea")))
			{
				cXMLConfigurator.AddElem(_T("MaxAcceptArea"));
			}
			cXMLConfigurator.SetElemData(m_nMaxAcceptArea.at(i));


			if(FALSE == cXMLConfigurator.FindElem(_T("MaxDefectArea")))
			{
				cXMLConfigurator.AddElem(_T("MaxDefectArea"));
			}
			cXMLConfigurator.SetElemData(m_dMaxDefectArea.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("MinMean")))
			{
				cXMLConfigurator.AddElem(_T("MinMean"));
			}
			cXMLConfigurator.SetElemData(m_dMinMean.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("MaxMean")))
			{
				cXMLConfigurator.AddElem(_T("MaxMean"));
			}
			cXMLConfigurator.SetElemData(m_dMaxMean.at(i));


			if(FALSE == cXMLConfigurator.FindElem(_T("MinStdDev")))
			{
				cXMLConfigurator.AddElem(_T("MinStdDev"));
			}
			cXMLConfigurator.SetElemData(m_dMinStdDev.at(i));


			if(FALSE == cXMLConfigurator.FindElem(_T("MaxStdDev")))
			{
				cXMLConfigurator.AddElem(_T("MaxStdDev"));
			}
			cXMLConfigurator.SetElemData(m_dMaxStdDev.at(i));


			if(FALSE == cXMLConfigurator.FindElem(_T("MeanInvalid")))
			{
				cXMLConfigurator.AddElem(_T("MeanInvalid"));
			}
			cXMLConfigurator.SetElemData(m_bMeanInvalid.at(i));


			if(FALSE == cXMLConfigurator.FindElem(_T("StdDevInvalid")))
			{
				cXMLConfigurator.AddElem(_T("StdDevInvalid"));
			}
			cXMLConfigurator.SetElemData(m_bStdDevInvalid.at(i));


			if(FALSE == cXMLConfigurator.FindElem(_T("MeanInvert")))
			{
				cXMLConfigurator.AddElem(_T("MeanInvert"));
			}
			cXMLConfigurator.SetElemData(m_bMeanInvert.at(i));


			if(FALSE == cXMLConfigurator.FindElem(_T("StdDevInvert")))
			{
				cXMLConfigurator.AddElem(_T("StdDevInvert"));
			}
			cXMLConfigurator.SetElemData(m_bStdDevInvert.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("ColNum")))
			{
				cXMLConfigurator.AddElem(_T("ColNum"));
			}
			cXMLConfigurator.SetElemData(m_nColNum.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("RowNum")))
			{
				cXMLConfigurator.AddElem(_T("RowNum"));
			}
			cXMLConfigurator.SetElemData(m_nRowNum.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("NGAllowed")))
			{
				cXMLConfigurator.AddElem(_T("NGAllowed"));
			}
			cXMLConfigurator.SetElemData(m_nNGAllowed.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("EnableHistogram")))
			{
				cXMLConfigurator.AddElem(_T("EnableHistogram"));
			}
			cXMLConfigurator.SetElemData(m_bEnableHistogram.at(i));

			if(FALSE == cXMLConfigurator.FindElem(_T("EnableBlob")))
			{
				cXMLConfigurator.AddElem(_T("EnableBlob"));
			}
			cXMLConfigurator.SetElemData(m_bEnableBlob.at(i));

		}
		cXMLConfigurator.LeaveElem();

	}

}
void tagACFInspectTrainDataParam::LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
{
	bool bRet = false;

	bRet = cXMLConfigurator.FindElem(_T("RoughSearchToolType"));
	if (bRet)
	{
		SetRoughSearchToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
	}



	bRet = cXMLConfigurator.FindElem(_T("CoarseScale"));
	if (bRet)
	{
		m_CoarseScale = (CoarseScale)_ttol(cXMLConfigurator.GetElemData());
		//m_CoarseScale = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("Accuracy"));
	if (bRet)
	{
		m_Accuracy = (Accuracy)_ttol(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("dAngleLow"));
	if (bRet)
	{
		m_dAngleLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("dAngleHigh"));
	if (bRet)
	{
		m_dAngleHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("bMaskEnable"));
	if (bRet)
	{
		m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());
	}


	bRet = cXMLConfigurator.FindElem(_T("dGrau"));
	if (bRet)
	{
		m_dGrau = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// noise thre
	bRet = cXMLConfigurator.FindElem(_T("lNoiseThre"));
	if (bRet)
	{
		m_lNoiseThre = _ttol(cXMLConfigurator.GetElemData());
	}

	// total mag
	bRet = cXMLConfigurator.FindElem(_T("lTotalMag"));
	if (bRet)
	{
		m_lTotalMag = _ttol(cXMLConfigurator.GetElemData());
	}

	// 找线工具START
	// 找线工具
	bRet = cXMLConfigurator.FindElem(_T("ContrastThre1"));
	if (bRet)
	{
		m_dContrastThre[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth1"));
	if (bRet)
	{
		m_lFilterHalfWidth[0] = _ttol(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("OutLineNum1"));
	if (bRet)
	{
		m_nOutLineNum[0] = _ttol(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EdgeProperty1"));
	if (bRet)
	{
		m_edgeProperty[0] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("SortByScore1"));
	if (!bRet)
	{
		m_bSortByScore[0] = true;
	}
	else
	{
		m_bSortByScore[0] = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("DualEdge1"));
	if (!bRet)
	{
		m_bEnableDualEdge[0] = false;
	}
	else
	{
		m_bEnableDualEdge[0] = _ttoi(cXMLConfigurator.GetElemData());
	}



	bRet = cXMLConfigurator.FindElem(_T("ContrastThre2"));
	if (bRet)
	{
		m_dContrastThre[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth2"));
	if (bRet)
	{
		m_lFilterHalfWidth[1] = _ttol(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("OutLineNum2"));
	if (bRet)
	{
		m_nOutLineNum[1] = _ttol(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EdgeProperty2"));
	if (bRet)
	{
		m_edgeProperty[1] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("SortByScore2"));
	if (!bRet)
	{
		m_bSortByScore[1] = true;
	}
	else
	{
		m_bSortByScore[1] = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("DualEdge2"));
	if (!bRet)
	{
		m_bEnableDualEdge[1] = false;
	}
	else
	{
		m_bEnableDualEdge[1] = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("FitPointIndex2"));


	bRet = cXMLConfigurator.FindElem(_T("EnableBaseSearch"));
	if (bRet)
	{
		m_bEnableBaseSearch = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("nInsAreaNum"));
	if (bRet)
	{
		m_nInsAreaNum = _ttoi(cXMLConfigurator.GetElemData());
	}

	if (m_nInsAreaNum<0)
	{
		m_nInsAreaNum = MAXTOOLNUM;
	}

	m_nDefectType.resize(m_nInsAreaNum);
	m_nBinaryThre.resize(m_nInsAreaNum);
	m_nMinAcceptArea.resize(m_nInsAreaNum);
	m_nMaxAcceptArea.resize(m_nInsAreaNum);
	m_dMaxDefectArea.resize(m_nInsAreaNum);

	m_dMinMean.resize(m_nInsAreaNum);
	m_dMaxMean.resize(m_nInsAreaNum);
	m_dMinStdDev.resize(m_nInsAreaNum);
	m_dMaxStdDev.resize(m_nInsAreaNum);

	m_bMeanInvalid.resize(m_nInsAreaNum);
	m_bStdDevInvalid.resize(m_nInsAreaNum);

	m_bMeanInvert.resize(m_nInsAreaNum);
	m_bStdDevInvert.resize(m_nInsAreaNum);

	m_nRowNum.resize(m_nInsAreaNum);
	m_nColNum.resize(m_nInsAreaNum);
	m_nNGAllowed.resize(m_nInsAreaNum);

	m_bEnableHistogram.resize(m_nInsAreaNum);
	m_bEnableBlob.resize(m_nInsAreaNum);

	for (int i=0;i<m_nInsAreaNum;i++)
	{
		CString strInfo;
		strInfo.Format(_T("InsAreaList%d"),i);
		bRet = cXMLConfigurator.FindElem(strInfo);
		if (!bRet)
		{
			return;
		}

		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("DefectType"));
			if (bRet)
			{
				m_nDefectType.at(i) = _ttol(cXMLConfigurator.GetElemData());
			}


			bRet = cXMLConfigurator.FindElem(_T("BinaryThre"));
			if (bRet)
			{
				m_nBinaryThre.at(i) = _ttol(cXMLConfigurator.GetElemData());
			}


			bRet = cXMLConfigurator.FindElem(_T("MinAcceptArea"));
			if (bRet)
			{
				m_nMinAcceptArea.at(i) = _ttol(cXMLConfigurator.GetElemData());
			}


			bRet = cXMLConfigurator.FindElem(_T("MaxAcceptArea"));
			if (bRet)
			{
				m_nMaxAcceptArea.at(i) = _ttol(cXMLConfigurator.GetElemData());
			}


			bRet = cXMLConfigurator.FindElem(_T("MaxDefectArea"));
			if (bRet)
			{
				m_dMaxDefectArea.at(i) = _ttof(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("MinMean"));
			if (bRet)
			{
				m_dMinMean.at(i) = _ttof(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("MaxMean"));
			if (bRet)
			{
				m_dMaxMean.at(i) = _ttof(cXMLConfigurator.GetElemData());
			}


			bRet = cXMLConfigurator.FindElem(_T("MinStdDev"));
			if (bRet)
			{
				m_dMinStdDev.at(i) = _ttof(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("MaxStdDev"));
			if (bRet)
			{
				m_dMaxStdDev.at(i) = _ttof(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("MeanInvalid"));
			if (bRet)
			{
				m_bMeanInvalid.at(i) = _ttoi(cXMLConfigurator.GetElemData());
			}


			bRet = cXMLConfigurator.FindElem(_T("StdDevInvalid"));
			if (bRet)
			{
				m_bStdDevInvalid.at(i) = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("MeanInvert"));
			if (bRet)
			{
				m_bMeanInvert.at(i) = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("StdDevInvert"));
			if (bRet)
			{
				m_bStdDevInvert.at(i) = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("ColNum"));
			if (bRet)
			{
				m_nColNum.at(i) = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("RowNum"));
			if (bRet)
			{
				m_nRowNum.at(i) = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("NGAllowed"));
			if (bRet)
			{
				m_nNGAllowed.at(i) = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("EnableHistogram"));
			if (bRet)
			{
				m_bEnableHistogram.at(i) = _ttoi(cXMLConfigurator.GetElemData());
			}

			bRet = cXMLConfigurator.FindElem(_T("EnableBlob"));
			if (bRet)
			{
				m_bEnableBlob.at(i)  = _ttoi(cXMLConfigurator.GetElemData());
			}

		}
		cXMLConfigurator.LeaveElem();
	}
}

void tagACFInspectTrainGuiParam::SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
{
	// TrainRect
	//if(FALSE == cXMLConfigurator.FindElem(_T("TrainRect")))
	//{
	cXMLConfigurator.AddElem(_T("TrainRect"));
	//}

	cXMLConfigurator.EnterElem();

	// OriginX
	if(FALSE == cXMLConfigurator.FindElem(_T("OriginX")))
	{
		cXMLConfigurator.AddElem(_T("OriginX"));
	}
	cXMLConfigurator.SetElemData(m_TrainRect.GetOrigin().GetX());

	// OriginY
	if(FALSE == cXMLConfigurator.FindElem(_T("OriginY")))
	{
		cXMLConfigurator.AddElem(_T("OriginY"));
	}
	cXMLConfigurator.SetElemData(m_TrainRect.GetOrigin().GetY());

	// SizeX
	if(FALSE == cXMLConfigurator.FindElem(_T("SizeX")))
	{
		cXMLConfigurator.AddElem(_T("SizeX"));
	}
	cXMLConfigurator.SetElemData(m_TrainRect.GetSize().GetX());

	// SizeY
	if(FALSE == cXMLConfigurator.FindElem(_T("SizeY")))
	{
		cXMLConfigurator.AddElem(_T("SizeY"));
	}
	cXMLConfigurator.SetElemData(m_TrainRect.GetSize().GetY());

	cXMLConfigurator.LeaveElem();


	// TrainAffineRect
	//if(FALSE == cXMLConfigurator.FindElem(_T("TrainAffineRect")))
	//{
	cXMLConfigurator.AddElem(_T("TrainAffineRect"));
	//}
	cXMLConfigurator.EnterElem();

	// CenterX
	if(FALSE == cXMLConfigurator.FindElem(_T("CenterX")))
	{
		cXMLConfigurator.AddElem(_T("CenterX"));
	}
	cXMLConfigurator.SetElemData(m_TrainAffineRect.GetCenter().GetX());

	// CenterY
	if(FALSE == cXMLConfigurator.FindElem(_T("CenterY")))
	{
		cXMLConfigurator.AddElem(_T("CenterY"));
	}
	cXMLConfigurator.SetElemData(m_TrainAffineRect.GetCenter().GetY());

	// XLength
	if(FALSE == cXMLConfigurator.FindElem(_T("XLength")))
	{
		cXMLConfigurator.AddElem(_T("XLength"));
	}
	cXMLConfigurator.SetElemData(m_TrainAffineRect.GetXLength());

	// YLength
	if(FALSE == cXMLConfigurator.FindElem(_T("YLength")))
	{
		cXMLConfigurator.AddElem(_T("YLength"));
	}
	cXMLConfigurator.SetElemData(m_TrainAffineRect.GetYLength());

	// XRotation
	if(FALSE == cXMLConfigurator.FindElem(_T("XRotation")))
	{
		cXMLConfigurator.AddElem(_T("XRotation"));
	}
	cXMLConfigurator.SetElemData(m_TrainAffineRect.GetXRotation().ToDouble());

	// Skew
	if(FALSE == cXMLConfigurator.FindElem(_T("Skew")))
	{
		cXMLConfigurator.AddElem(_T("Skew"));
	}
	cXMLConfigurator.SetElemData(m_TrainAffineRect.GetSkew().ToDouble());

	cXMLConfigurator.LeaveElem();


	// search rect
	cXMLConfigurator.AddElem(_T("SearchRect"));
	cXMLConfigurator.EnterElem();

	// OriginX
	if(FALSE == cXMLConfigurator.FindElem(_T("OriginX")))
	{
		cXMLConfigurator.AddElem(_T("OriginX"));
	}
	cXMLConfigurator.SetElemData(m_SearchRect.GetOrigin().GetX());

	// OriginY
	if(FALSE == cXMLConfigurator.FindElem(_T("OriginY")))
	{
		cXMLConfigurator.AddElem(_T("OriginY"));
	}
	cXMLConfigurator.SetElemData(m_SearchRect.GetOrigin().GetY());

	// SizeX
	if(FALSE == cXMLConfigurator.FindElem(_T("SizeX")))
	{
		cXMLConfigurator.AddElem(_T("SizeX"));
	}
	cXMLConfigurator.SetElemData(m_SearchRect.GetSize().GetX());

	// SizeY
	if(FALSE == cXMLConfigurator.FindElem(_T("SizeY")))
	{
		cXMLConfigurator.AddElem(_T("SizeY"));
	}
	cXMLConfigurator.SetElemData(m_SearchRect.GetSize().GetY());

	cXMLConfigurator.LeaveElem();



	cXMLConfigurator.AddElem(_T("ModelPoint"));
	cXMLConfigurator.EnterElem();

	// CenterX
	if(FALSE == cXMLConfigurator.FindElem(_T("CenterX")))
	{
		cXMLConfigurator.AddElem(_T("CenterX"));
	}
	cXMLConfigurator.SetElemData(m_ModelPoint.GetX());

	// CenterY
	if(FALSE == cXMLConfigurator.FindElem(_T("CenterY")))
	{
		cXMLConfigurator.AddElem(_T("CenterY"));
	}
	cXMLConfigurator.SetElemData(m_ModelPoint.GetY());

	cXMLConfigurator.LeaveElem();


	if(FALSE == cXMLConfigurator.FindElem(_T("nGUIInsAreaNum")))
	{
		cXMLConfigurator.AddElem(_T("nGUIInsAreaNum"));
	}

	cXMLConfigurator.SetElemData(m_nInsAreaNum);


	CString strInfo = _T("");

	for (int i=0;i<m_InspectAffineRect.size();i++)
	{
		strInfo.Format(_T("InsAffineRectList%d"),i);
		//if(FALSE == cXMLConfigurator.FindElem(strInfo))
		//{
		cXMLConfigurator.AddElem(strInfo);
		//}

		cXMLConfigurator.EnterElem();
		{
			// CenterX
			if(FALSE == cXMLConfigurator.FindElem(_T("CenterX")))
			{
				cXMLConfigurator.AddElem(_T("CenterX"));
			}
			cXMLConfigurator.SetElemData(m_InspectAffineRect.at(i)->GetCenter().GetX());

			// CenterY
			if(FALSE == cXMLConfigurator.FindElem(_T("CenterY")))
			{
				cXMLConfigurator.AddElem(_T("CenterY"));
			}
			cXMLConfigurator.SetElemData(m_InspectAffineRect.at(i)->GetCenter().GetY());

			// XLength
			if(FALSE == cXMLConfigurator.FindElem(_T("XLength")))
			{
				cXMLConfigurator.AddElem(_T("XLength"));
			}
			cXMLConfigurator.SetElemData(m_InspectAffineRect.at(i)->GetXLength());

			// YLength
			if(FALSE == cXMLConfigurator.FindElem(_T("YLength")))
			{
				cXMLConfigurator.AddElem(_T("YLength"));
			}
			cXMLConfigurator.SetElemData(m_InspectAffineRect.at(i)->GetYLength());

			// XRotation
			if(FALSE == cXMLConfigurator.FindElem(_T("XRotation")))
			{
				cXMLConfigurator.AddElem(_T("XRotation"));
			}
			cXMLConfigurator.SetElemData(m_InspectAffineRect.at(i)->GetXRotation().ToDouble());

			// Skew
			if(FALSE == cXMLConfigurator.FindElem(_T("Skew")))
			{
				cXMLConfigurator.AddElem(_T("Skew"));
			}
			cXMLConfigurator.SetElemData(m_InspectAffineRect.at(i)->GetSkew().ToDouble());
		}
		cXMLConfigurator.LeaveElem();
	}

	// 卡尺参数START
	// Caliper Num1
	if(FALSE == cXMLConfigurator.FindElem(_T("CaliperNum1")))
	{
		cXMLConfigurator.AddElem(_T("CaliperNum1"));
	}

	cXMLConfigurator.SetElemData(m_nCaliperNum[0]);

	// startX1
	if(FALSE == cXMLConfigurator.FindElem(_T("StartX1")))
	{
		cXMLConfigurator.AddElem(_T("StartX1"));
	}

	cXMLConfigurator.SetElemData(m_dStartX[0]);


	// startY1
	if(FALSE == cXMLConfigurator.FindElem(_T("StartY1")))
	{
		cXMLConfigurator.AddElem(_T("StartY1"));
	}

	cXMLConfigurator.SetElemData(m_dStartY[0]);

	// EndX1
	if(FALSE == cXMLConfigurator.FindElem(_T("EndX1")))
	{
		cXMLConfigurator.AddElem(_T("EndX1"));
	}

	cXMLConfigurator.SetElemData(m_dEndX[0]);

	// EndY1
	if(FALSE == cXMLConfigurator.FindElem(_T("EndY1")))
	{
		cXMLConfigurator.AddElem(_T("EndY1"));
	}

	cXMLConfigurator.SetElemData(m_dEndY[0]);

	// Project Len1
	if(FALSE == cXMLConfigurator.FindElem(_T("ProjectLen1")))
	{
		cXMLConfigurator.AddElem(_T("ProjectLen1"));
	}

	cXMLConfigurator.SetElemData(m_dCaliperProjectLen[0]);

	// Search Len1
	if(FALSE == cXMLConfigurator.FindElem(_T("SearchLen1")))
	{
		cXMLConfigurator.AddElem(_T("SearchLen1"));
	}

	cXMLConfigurator.SetElemData(m_dCaliperSearchLen[0]);

	// Search Rotation1
	if(FALSE == cXMLConfigurator.FindElem(_T("SearchRotation1")))
	{
		cXMLConfigurator.AddElem(_T("SearchRotation1"));
	}

	cXMLConfigurator.SetElemData(m_CaliperSearchRotation[0].ToDouble());


	//Caliper Visible1
	if(FALSE == cXMLConfigurator.FindElem(_T("CaliperVisible1")))
	{
		cXMLConfigurator.AddElem(_T("CaliperVisible1"));
	}

	cXMLConfigurator.SetElemData(m_bCaliperVisible[0]);

	if(FALSE == cXMLConfigurator.FindElem(_T("CaliperMaskEnable")))
	{
		cXMLConfigurator.AddElem(_T("CaliperMaskEnable"));
	}

	cXMLConfigurator.SetElemData(m_bEnableLineMask[0]);

	CString strTmp;
	cXMLConfigurator.DeleteElem(_T("MaskParam"));
	cXMLConfigurator.AddElem(_T("MaskParam"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_vdCaliperMaskData[0].size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);

	for (int j=0; j<m_vdCaliperMaskData[0].size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_vdCaliperMaskData[0].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_vbCaliperMask[0].size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

	for (int j=0; j<m_vbCaliperMask[0].size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_vbCaliperMask[0].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();



	// Caliper Num2
	if(FALSE == cXMLConfigurator.FindElem(_T("CaliperNum2")))
	{
		cXMLConfigurator.AddElem(_T("CaliperNum2"));
	}

	cXMLConfigurator.SetElemData(m_nCaliperNum[1]);

	// startX2
	if(FALSE == cXMLConfigurator.FindElem(_T("StartX2")))
	{
		cXMLConfigurator.AddElem(_T("StartX2"));
	}

	cXMLConfigurator.SetElemData(m_dStartX[1]);


	// startY2
	if(FALSE == cXMLConfigurator.FindElem(_T("StartY2")))
	{
		cXMLConfigurator.AddElem(_T("StartY2"));
	}

	cXMLConfigurator.SetElemData(m_dStartY[1]);

	// EndX2
	if(FALSE == cXMLConfigurator.FindElem(_T("EndX2")))
	{
		cXMLConfigurator.AddElem(_T("EndX2"));
	}

	cXMLConfigurator.SetElemData(m_dEndX[1]);

	// EndY2
	if(FALSE == cXMLConfigurator.FindElem(_T("EndY2")))
	{
		cXMLConfigurator.AddElem(_T("EndY2"));
	}

	cXMLConfigurator.SetElemData(m_dEndY[1]);

	// Project Len2
	if(FALSE == cXMLConfigurator.FindElem(_T("ProjectLen2")))
	{
		cXMLConfigurator.AddElem(_T("ProjectLen2"));
	}

	cXMLConfigurator.SetElemData(m_dCaliperProjectLen[1]);

	// Search Len2
	if(FALSE == cXMLConfigurator.FindElem(_T("SearchLen2")))
	{
		cXMLConfigurator.AddElem(_T("SearchLen2"));
	}

	cXMLConfigurator.SetElemData(m_dCaliperSearchLen[1]);

	// Search Rotation2
	if(FALSE == cXMLConfigurator.FindElem(_T("SearchRotation2")))
	{
		cXMLConfigurator.AddElem(_T("SearchRotation2"));
	}

	cXMLConfigurator.SetElemData(m_CaliperSearchRotation[1].ToDouble());


	//Caliper Visible2
	if(FALSE == cXMLConfigurator.FindElem(_T("CaliperVisible2")))
	{
		cXMLConfigurator.AddElem(_T("CaliperVisible2"));
	}

	cXMLConfigurator.SetElemData(m_bCaliperVisible[1]);

	if(FALSE == cXMLConfigurator.FindElem(_T("CaliperMaskEnable1")))
	{
		cXMLConfigurator.AddElem(_T("CaliperMaskEnable1"));
	}

	cXMLConfigurator.SetElemData(m_bEnableLineMask[1]);

	cXMLConfigurator.DeleteElem(_T("MaskParam1"));
	cXMLConfigurator.AddElem(_T("MaskParam1"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_vdCaliperMaskData[1].size());
	cXMLConfigurator.AddElem(_T("MaskDataNum1"), strInfo);

	for (int j=0; j<m_vdCaliperMaskData[1].size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_vdCaliperMaskData[1].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_vbCaliperMask[1].size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum1"), strInfo);

	for (int j=0; j<m_vbCaliperMask[1].size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_vbCaliperMask[1].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();





}

void tagACFInspectTrainGuiParam::LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
{
	sc2Vector vPos;
	bool bRet = false;
	bRet = cXMLConfigurator.FindElem(_T("TrainRect"));
	if (bRet)
	{
		//return;
		cXMLConfigurator.EnterElem();
		bRet = cXMLConfigurator.FindElem(_T("OriginX"));
		if (bRet)
		{
			vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		bRet = cXMLConfigurator.FindElem(_T("OriginY"));
		if (bRet)
		{
			vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}
		m_TrainRect.SetOrigion(vPos);

		bRet = cXMLConfigurator.FindElem(_T("SizeX"));
		if (bRet)
		{
			vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		bRet = cXMLConfigurator.FindElem(_T("SizeY"));
		if (bRet)
		{
			vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		m_TrainRect.SetSize(vPos);
		cXMLConfigurator.LeaveElem();
	}
	////TrainAffineRect
	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
	if (bRet)
	{
		cXMLConfigurator.EnterElem();
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

		m_TrainAffineRect.SetCenter(vPos);

		bRet = cXMLConfigurator.FindElem(_T("XLength"));
		if (bRet)
		{
			m_TrainAffineRect.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		bRet = cXMLConfigurator.FindElem(_T("YLength"));
		if (bRet)
		{
			m_TrainAffineRect.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		bRet = cXMLConfigurator.FindElem(_T("XRotation"));
		if (bRet)
		{
			m_TrainAffineRect.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
		}

		bRet = cXMLConfigurator.FindElem(_T("Skew"));
		if (bRet)
		{
			m_TrainAffineRect.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
		}

		cXMLConfigurator.LeaveElem();
	}

	// search rect
	bRet = cXMLConfigurator.FindElem(_T("SearchRect"));
	if (bRet)
	{
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("OriginX"));
		if (bRet)
		{
			vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		bRet = cXMLConfigurator.FindElem(_T("OriginY"));
		if (bRet)
		{
			vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		m_SearchRect.SetOrigion(vPos);

		bRet = cXMLConfigurator.FindElem(_T("SizeX"));
		if (bRet)
		{
			vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		bRet = cXMLConfigurator.FindElem(_T("SizeY"));
		if (bRet)
		{
			vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		m_SearchRect.SetSize(vPos);
		cXMLConfigurator.LeaveElem();
	}

	// model point
	bRet = cXMLConfigurator.FindElem(_T("ModelPoint"));
	if (!bRet)
	{
		//return;
	}
	else
	{
		cXMLConfigurator.EnterElem();

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

		m_ModelPoint = vPos;
		cXMLConfigurator.LeaveElem();
	}

	// 卡尺参数START
	// CaliperNum1
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum1"));
	if (bRet)
	{
		m_nCaliperNum[0] = _ttoi(cXMLConfigurator.GetElemData());
	}

	// startX
	bRet = cXMLConfigurator.FindElem(_T("StartX1"));
	if (bRet)
	{
		m_dStartX[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// startY
	bRet = cXMLConfigurator.FindElem(_T("StartY1"));
	if (bRet)
	{
		m_dStartY[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// EndX
	bRet = cXMLConfigurator.FindElem(_T("EndX1"));
	if (bRet)
	{
		m_dEndX[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// EndY
	bRet = cXMLConfigurator.FindElem(_T("EndY1"));
	if (bRet)
	{
		m_dEndY[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("ProjectLen1"));
	if (bRet)
	{
		m_dCaliperProjectLen[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("SearchLen1"));
	if (bRet)
	{
		m_dCaliperSearchLen[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("SearchRotation1"));
	if (bRet)
	{
		m_CaliperSearchRotation[0] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}


	// Caliper1 Visible
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible1"));
	if (bRet)
	{
		m_bCaliperVisible[0]=true;
	}
	else
	{
		m_bCaliperVisible[0] = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable"));
	if (bRet)
	{
		m_bEnableLineMask[0] = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		m_bEnableLineMask[0] = false;
	}



	bRet = cXMLConfigurator.FindElem(_T("MaskParam"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
		if (bRet) 
		{
			m_vdCaliperMaskData[0].resize(_ttoi(cXMLConfigurator.GetElemData()));
		}
		for (int j=0; j<m_vdCaliperMaskData[0].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (bRet)
			{
				m_vdCaliperMaskData[0].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
			}
		}


		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum"));
		if (bRet) 
		{
			m_vbCaliperMask[0].resize(_ttoi(cXMLConfigurator.GetElemData()));
		}

		for (int j=0; j<m_vbCaliperMask[0].size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (bRet)
			{
				m_vbCaliperMask[0].at(j) = _ttoi(cXMLConfigurator.GetElemData());
			}			

		}
		cXMLConfigurator.LeaveElem();
	}

	// CaliperNum2
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum2"));
	if (bRet)
	{
		m_nCaliperNum[1] = _ttoi(cXMLConfigurator.GetElemData());
	}

	// startX
	bRet = cXMLConfigurator.FindElem(_T("StartX2"));
	if (bRet)
	{
		m_dStartX[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// startY
	bRet = cXMLConfigurator.FindElem(_T("StartY2"));
	if (bRet)
	{
		m_dStartY[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// EndX
	bRet = cXMLConfigurator.FindElem(_T("EndX2"));
	if (bRet)
	{
		m_dEndX[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// EndY
	bRet = cXMLConfigurator.FindElem(_T("EndY2"));
	if (bRet)
	{
		m_dEndY[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("ProjectLen2"));
	if (bRet)
	{
		m_dCaliperProjectLen[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("SearchLen2"));
	if (bRet)
	{
		m_dCaliperSearchLen[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("SearchRotation2"));
	if (bRet)
	{
		m_CaliperSearchRotation[1] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}


	// Caliper Visible
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible2"));
	if (bRet)
	{
		m_bCaliperVisible[1]=true;
	}
	else
	{
		m_bCaliperVisible[1] = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable1"));
	if (bRet)
	{
		m_bEnableLineMask[1] = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		m_bEnableLineMask[1] = false;
	}


	bRet = cXMLConfigurator.FindElem(_T("MaskParam1"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum1"));
		if (bRet) 
		{
			m_vdCaliperMaskData[1].resize(_ttoi(cXMLConfigurator.GetElemData()));
		}


		for (int j=0; j<m_vdCaliperMaskData[1].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				m_vdCaliperMaskData[1].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
			}				
		}

		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum1"));
		if (bRet)
		{
			m_vbCaliperMask[1].resize(_ttoi(cXMLConfigurator.GetElemData()));
		}

		for (int j=0; j<m_vbCaliperMask[1].size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (bRet)
			{
				m_vbCaliperMask[1].at(j) = _ttoi(cXMLConfigurator.GetElemData());
			}			
		}
		cXMLConfigurator.LeaveElem();
	}




	bRet = cXMLConfigurator.FindElem(_T("nGUIInsAreaNum"));
	if (bRet)
	{
		m_nInsAreaNum = _ttoi(cXMLConfigurator.GetElemData());
	}


	CString strInfo = _T("");

	for (int j=0;j<m_InspectAffineRect.size();j++)
	{
		if (m_InspectAffineRect.at(j)!=NULL)
		{
			delete m_InspectAffineRect.at(j);
			m_InspectAffineRect.at(j) = NULL;

		}
	}
	m_InspectAffineRect.clear();

	for (int i=0;i<m_nInsAreaNum;i++)
	{
		scAffineRect* tempscAffineRect = new scAffineRect;
		m_InspectAffineRect.push_back(tempscAffineRect);

	}

	for (int i=0;i<m_nInsAreaNum;i++)
	{
		CString strInfo;
		strInfo.Format(_T("InsAffineRectList%d"),i);
		bRet = cXMLConfigurator.FindElem(strInfo);
		if (!bRet)
		{
			return;
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

			m_InspectAffineRect.at(i)->SetCenter(vPos);

			bRet = cXMLConfigurator.FindElem(_T("XLength"));
			if (bRet)
			{
				m_InspectAffineRect.at(i)->SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("YLength"));
			if (bRet)
			{
				m_InspectAffineRect.at(i)->SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("XRotation"));
			if (bRet)
			{
				m_InspectAffineRect.at(i)->SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
			}

			bRet = cXMLConfigurator.FindElem(_T("Skew"));
			if (bRet)
			{
				m_InspectAffineRect.at(i)->SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
			}
		}
		cXMLConfigurator.LeaveElem();
	}


}

void tagACFInspectSearchDataParam::SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
{
	CString strInfo = _T("");
	strInfo.Format(_T("%d"), m_nSearchNum);
	cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);

	strInfo.Format(_T("%f"), m_dAcceptThre);
	cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);

	strInfo.Format(_T("%f"), m_dConfusionThre);
	cXMLConfigurator.AddElem(_T("ConfusionThre"), strInfo);

	strInfo.Format(_T("%f"), m_dAngleLow);
	cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
	strInfo.Format(_T("%f"), m_dAngleHigh);
	cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);

	strInfo.Format(_T("%f"), m_dSizeLow);
	cXMLConfigurator.AddElem(_T("SizeLow"), strInfo);
	strInfo.Format(_T("%f"), m_dSizeHigh);
	cXMLConfigurator.AddElem(_T("SizeHigh"), strInfo);

	strInfo.Format(_T("%d"), m_bEnableSharpScore);
	cXMLConfigurator.AddElem(_T("EnableSharpScore"), strInfo);
	strInfo.Format(_T("%f"), m_dSharpThre);
	cXMLConfigurator.AddElem(_T("SharpThre"), strInfo);
}

void tagACFInspectSearchDataParam::LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
{
	bool bRet = false;
	bRet = cXMLConfigurator.FindElem(_T("SearchNum"));
	if (bRet)
	{
		m_nSearchNum = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("AcceptThre"));
	if (bRet)
	{
		m_dAcceptThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("ConfusionThre"));
	if (bRet)
	{
		m_dConfusionThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("AngleLow"));
	if (bRet)
	{
		m_dAngleLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("AngleHigh"));
	if (bRet)
	{
		m_dAngleHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}


	bRet = cXMLConfigurator.FindElem(_T("SizeLow"));
	if (bRet)
	{
		m_dSizeLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	else
	{
		m_dSizeLow = 1.0;
	}

	bRet = cXMLConfigurator.FindElem(_T("SizeHigh"));
	if (bRet)
	{
		m_dSizeHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	else
	{
		m_dSizeHigh = 1.0;
	}

	bRet = cXMLConfigurator.FindElem(_T("EnableSharpScore"));
	if (!bRet)
	{
		m_bEnableSharpScore =  false;
	}
	m_bEnableSharpScore = _ttoi(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("SharpThre"));
	if (!bRet)
	{
		m_dSharpThre = 0.5;
	}
	else
	{
		m_dSharpThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
}

void tagACFInspectTrainResult::SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
{
	//ModelX
	if(FALSE == cXMLConfigurator.FindElem(_T("ModelX")))
	{
		cXMLConfigurator.AddElem(_T("ModelX"));
	}
	cXMLConfigurator.SetElemData(m_dModelX);

	//m_dModelY
	if(FALSE == cXMLConfigurator.FindElem(_T("ModelY")))
	{
		cXMLConfigurator.AddElem(_T("ModelY"));
	}
	cXMLConfigurator.SetElemData(m_dModelY);

	if(FALSE == cXMLConfigurator.FindElem(_T("Sharpness")))
	{
		cXMLConfigurator.AddElem(_T("Sharpness"));
	}
	cXMLConfigurator.SetElemData(m_dSharpness);

}

void tagACFInspectTrainResult::LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
{
	bool bRet = false;
	bRet = cXMLConfigurator.FindElem(_T("ModelX"));
	if (bRet)
	{
		m_dModelX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("ModelY"));
	if (bRet)
	{
		m_dModelY =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("Sharpness"));
	if (bRet)
	{
		m_dSharpness =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

}

CACFInspectTool::CACFInspectTool()
{
	
	m_bInspectFinish						= false;
	m_bInspectOK							= false;
//	m_pParentTool							= NULL;

	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;



	CString str;
	// 区域定位搜索区
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	// 初定位参考点
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 200, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);
	
	// 区域定位训练区
	m_GuiTrainRectRough.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRectRough.SetLabelVisible(TRUE);

	// 几何定位训练区
	m_GuiTrainAffineRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainAffineRect.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	for (int i=0;i<MAXTOOLNUM;i++)
	{
		scGuiAffineRect *tempGuiTrainRect = new scGuiAffineRect;
		tempGuiTrainRect->SetCenterLengthsRotationSkew(250+i*100, 200, 50, 50, 0, 0);
		str.Format(_T("ACF检测%d"),i+1);
		tempGuiTrainRect->SetLabel(str);
		tempGuiTrainRect->SetLabelVisible(TRUE);
		m_GuiTrainRect.push_back(tempGuiTrainRect);

	}
	
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRectRough.SetLabel(_T("Train Area"));
		m_GuiTrainAffineRect.SetLabel(_T("Train Area1"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));

		for (int i=0;i<MAXTOOLNUM;i++)
		{
			str.Format(_T("ACF INSP%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}

		break;
	case 1:
		m_GuiTrainRectRough.SetLabel(_T("训练区域"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		for (int i=0;i<MAXTOOLNUM;i++)
		{
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}

		break;
	default:
		m_GuiTrainRectRough.SetLabel(_T("训练区域"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		for (int i=0;i<MAXTOOLNUM;i++)
		{
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}


		break;
	}
	
	// 找线工具
	m_GuiFindLine[1].SetStartXYEndXY(240, 320, 240, 170);		
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[0].SetStartXYEndXY(270, 130, 420, 130);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	int i=0;
	for (i=0; i<LINE_NUM; i++)
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


	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	//m_tmpTrainDataParam.m_dGrau				= m_AreaInspectTool.GetGranularity();
	//m_tmpTrainDataParam.m_lNoiseThre		= m_AreaInspectTool.GetNoiseThre();
	//m_tmpTrainDataParam.m_lTotalMag			= m_AreaInspectTool.GetTotalGradMagThre();
	//m_tmpTrainDataParam.m_bMaskEnable		= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor			= m_GuiSearchRect.GetLineColor();

	m_tmpTrainGuiParam.m_ModelPoint	= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect =   m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_TrainRect = m_GuiTrainRectRough.GetRect();
	m_tmpTrainGuiParam.m_TrainAffineRect = m_GuiTrainAffineRect.GetAffineRect();

	for (int i=0;i<MAXTOOLNUM;i++)
	{
		*(m_tmpTrainGuiParam.m_InspectAffineRect.at(i)) = m_GuiTrainRect.at(i)->GetAffineRect();
	}
	

	m_tmpTrainDataParam.m_Accuracy				= m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -5.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 5.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_dGrau				= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre		= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag			= m_QuickSearchTool.GetTotalGradMagThre();

	// 找线工具
	for (i=0; i<LINE_NUM; i++)
	{
		m_FindLineTool[i].GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], 
			m_tmpTrainDataParam.m_lFilterHalfWidth[i]);	// 卡尺属性
		m_tmpTrainDataParam.m_nOutLineNum[i]			= 0;// 忽略数
		m_tmpTrainGuiParam.m_nCaliperNum[i]				= m_GuiFindLine[i].GetCaliperNum();	// 卡尺数
		m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
			m_tmpTrainGuiParam.m_dEndY[i]);	// 位置
		m_tmpTrainGuiParam.m_dCaliperProjectLen[i]		= m_GuiFindLine[i].GetCaliperProjectionLength();// 仿射长度
		m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength(); // 搜索长度
		m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection(); // 方向
		m_tmpTrainGuiParam.m_bEnableLineMask[i]			= false;	// 找线工具的掩膜
	}

	m_tmpSearchDataParam.m_nSearchNum		= 1;
	m_tmpSearchDataParam.m_dAcceptThre		= 0.7;
	m_tmpSearchDataParam.m_dConfusionThre   = 0.8;

	m_tmpSearchDataParam.m_dAngleLow		= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh		= 5.0;
	m_tmpSearchDataParam.m_dSizeLow			= 1.0;
	m_tmpSearchDataParam.m_dSizeHigh		= 1.0;
	m_tmpSearchDataParam.m_bEnableSharpScore	= false;
	m_tmpSearchDataParam.m_dSharpThre			= 0.5;

	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();


	m_TrainDataParam						= m_tmpTrainDataParam;
	m_TrainGuiParam							= m_tmpTrainGuiParam;
	m_SearchDataParam						= m_tmpSearchDataParam;
	m_SearchGuiParam						= m_tmpSearchGuiParam;

	m_dCurMean.resize(MAXTOOLNUM,0);
	m_dCurStdDev.resize(MAXTOOLNUM,0);
	m_dCurDefArea.resize(MAXTOOLNUM,0);
	m_dmaxGrayValue.resize(MAXTOOLNUM,0);
// 	m_nMaxGreyVal = 0;
// 	m_dDefectArea = 0.0;
}

CACFInspectTool::CACFInspectTool(CACFInspectTool &CACFInspectTool)
{
	if (CACFInspectTool.GetSearchToolType() != eACFInspectTool)
	{
		return;
	}

	// 清空
//  	ClearAllGui();
//  	ClearResult();

	// 初始化

	m_bInspectFinish						= false;
	m_bInspectOK							= false;
//	m_pParentTool							= NULL;

	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;

	for (int i=0;i<m_GuiTrainRect.size();i++)
	{
		if (m_GuiTrainRect.at(i)!=NULL)
		{
			delete m_GuiTrainRect.at(i);
			m_GuiTrainRect.at(i) = NULL;
		}
	}
	m_GuiTrainRect.clear();

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	// 初定位参考点
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 200, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);
	
	// 区域定位训练区
	m_GuiTrainRectRough.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRectRough.SetLabelVisible(TRUE);

	// 几何定位训练区
	m_GuiTrainAffineRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainAffineRect.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	for (int i=0;i<m_tmpTrainGuiParam.m_InspectAffineRect.size();i++)
	{
		if (m_tmpTrainGuiParam.m_InspectAffineRect.at(i)!=NULL)
		{
			delete m_tmpTrainGuiParam.m_InspectAffineRect.at(i);
			m_tmpTrainGuiParam.m_InspectAffineRect.at(i) = NULL;
		}
	}
	m_tmpTrainGuiParam.m_InspectAffineRect.clear();

	CString str;
	for (int i=0;i<CACFInspectTool.m_TrainDataParam.m_nInsAreaNum;i++)
	{
		scGuiAffineRect* tempGuiTrainRect = new scGuiAffineRect;
		tempGuiTrainRect->SetCenterLengthsRotationSkew(250+i*100, 200, 50, 50, 0, 0);
		str.Format(_T("ACF检测%d"),i+1);
		tempGuiTrainRect->SetLabel(str);
		tempGuiTrainRect->SetLabelVisible(TRUE);
		m_GuiTrainRect.push_back(tempGuiTrainRect);


	}

	m_dCurMean.resize(CACFInspectTool.m_TrainDataParam.m_nInsAreaNum,0);
	m_dCurStdDev.resize(CACFInspectTool.m_TrainDataParam.m_nInsAreaNum,0);
	m_dCurDefArea.resize(CACFInspectTool.m_TrainDataParam.m_nInsAreaNum,0);
	m_dmaxGrayValue.resize(CACFInspectTool.m_TrainDataParam.m_nInsAreaNum,0);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRectRough.SetLabel(_T("Train Area"));
		m_GuiTrainAffineRect.SetLabel(_T("Train Area1"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));

		for (int i=0;i<CACFInspectTool.m_TrainDataParam.m_nInsAreaNum;i++)
		{
			str.Format(_T("ACF INSP%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}

		break;
	case 1:
		m_GuiTrainRectRough.SetLabel(_T("训练区域"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		for (int i=0;i<CACFInspectTool.m_TrainDataParam.m_nInsAreaNum;i++)
		{
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}

		break;
	default:
		m_GuiTrainRectRough.SetLabel(_T("训练区域"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		for (int i=0;i<CACFInspectTool.m_TrainDataParam.m_nInsAreaNum;i++)
		{
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}


		break;
	}

	// 找线工具
	m_GuiFindLine[1].SetStartXYEndXY(240, 320, 240, 170);
	m_GuiFindLine[0].SetStartXYEndXY(270, 130, 420, 130);

	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	for (int i=0; i<LINE_NUM; i++)
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

	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	// 赋值
	m_CamHeight								=  CACFInspectTool.m_CamHeight;
	m_CamWidth								=  CACFInspectTool.m_CamWidth;


	m_TrainDataParam						= CACFInspectTool.m_TrainDataParam;
	m_TrainGuiParam							= CACFInspectTool.m_TrainGuiParam;
	m_SearchDataParam						= CACFInspectTool.m_SearchDataParam;
	m_SearchGuiParam						= CACFInspectTool.m_SearchGuiParam;
	m_TrainResult                           = CACFInspectTool.m_TrainResult;
	m_ImageTrain							= CACFInspectTool.m_ImageTrain;
	m_ImageMask								= CACFInspectTool.m_ImageMask;
	m_bTrainGuiShow							= CACFInspectTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= CACFInspectTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CACFInspectTool.m_bTrainOK;
	m_pGuiDisplay							= CACFInspectTool.m_pGuiDisplay;
	
	

	CACFInspectTool.UpdateSearchGuiData(true);
	CACFInspectTool.UpdateTrainGuiData(true);

	

	SetTrainDataParam(&CACFInspectTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CACFInspectTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CACFInspectTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CACFInspectTool.m_tmpSearchGuiParam);
	SetTrainImage(CACFInspectTool.m_tmpImageTrain);
	SetMaskImage(CACFInspectTool.m_tmpImageMask);
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}


	if (m_bTrainOK && m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		m_bTrainOK = false;
		bool bRet = false;
		if (m_tmpTrainDataParam.m_bEnableBaseSearch && (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool
			|| m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool))
		{
			m_PatternSearchTool = CACFInspectTool.m_PatternSearchTool;
			m_bTrainOK = CACFInspectTool.m_bTrainOK;
		}

		if (m_tmpTrainDataParam.m_bEnableBaseSearch && (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool
			|| m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool))
		{
			// 设置训练参数
			if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
			{
				return;
			}

			if (!m_QuickSearchTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
			{
				return;
			}

			if (!m_QuickSearchTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
			{
				return;
			}

			
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

				tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
				GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);
			}		


			// 设置掩模图像
			if (m_TrainDataParam.m_bMaskEnable)
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				m_bTrainOK = bRet;
			}
			else
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				m_bTrainOK = bRet;
			}
		}

	}	

	UpdateTrainGuiDisplay();
}

CACFInspectTool& CACFInspectTool::operator=(CACFInspectTool& CACFInspectTool)
{
	if (this == &CACFInspectTool)
	{
		return *this;
	}

	if (CACFInspectTool.GetSearchToolType() != eACFInspectTool)
	{
		return *this;
	}
	
	// 清空
	ClearAllGui();
	ClearResult();
	
	// 初始化

	m_bInspectFinish						= false;
	m_bInspectOK							= false;
//	m_pParentTool							= NULL;

	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;



	CString str;
	// 区域定位搜索区
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	// 初定位参考点
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 200, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	// 区域定位训练区
	m_GuiTrainRectRough.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRectRough.SetLabelVisible(TRUE);

	// 几何定位训练区
	m_GuiTrainAffineRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainAffineRect.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	for (int i=0;i<CACFInspectTool.m_TrainDataParam.m_nInsAreaNum;i++)
	{
		scGuiAffineRect* tempGuiTrainRect = new scGuiAffineRect;
		tempGuiTrainRect->SetCenterLengthsRotationSkew(250+i*100, 200, 50, 50, 0, 0);
		str.Format(_T("ACF检测%d"),i+1);
		tempGuiTrainRect->SetLabel(str);
		tempGuiTrainRect->SetLabelVisible(TRUE);
		m_GuiTrainRect.push_back(tempGuiTrainRect);

	}

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRectRough.SetLabel(_T("Train Area"));
		m_GuiTrainAffineRect.SetLabel(_T("Train Area1"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));

		for (int i=0;i<CACFInspectTool.m_TrainDataParam.m_nInsAreaNum;i++)
		{
			str.Format(_T("ACF INSP%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}

		break;
	case 1:
		m_GuiTrainRectRough.SetLabel(_T("训练区域"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		for (int i=0;i<CACFInspectTool.m_TrainDataParam.m_nInsAreaNum;i++)
		{
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}

		break;
	default:
		m_GuiTrainRectRough.SetLabel(_T("训练区域"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		for (int i=0;i<CACFInspectTool.m_TrainDataParam.m_nInsAreaNum;i++)
		{
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}


		break;
	}

	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiFindLine[1].SetStartXYEndXY(240, 320, 240, 170);
	m_GuiFindLine[0].SetStartXYEndXY(270, 130, 420, 130);

	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	for (int i=0; i<LINE_NUM; i++)
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

	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识
	
	m_tmpTrainDataParam = CACFInspectTool.m_tmpTrainDataParam;

	m_tmpTrainGuiParam.m_nLineWidth = m_GuiTrainRectRough.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor =  m_GuiTrainRectRough.GetLineColor();

	m_tmpSearchGuiParam.m_nLineWidth = m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor =  m_GuiSearchRect.GetLineColor();

	m_dCurMean.resize(CACFInspectTool.m_TrainDataParam.m_nInsAreaNum,0);
	m_dCurStdDev.resize(CACFInspectTool.m_TrainDataParam.m_nInsAreaNum,0);
	m_dCurDefArea.resize(CACFInspectTool.m_TrainDataParam.m_nInsAreaNum,0);
	m_dmaxGrayValue.resize(CACFInspectTool.m_TrainDataParam.m_nInsAreaNum,0);
	// 赋值
	m_CamHeight								=  CACFInspectTool.m_CamHeight;
	m_CamWidth								=  CACFInspectTool.m_CamWidth;


	m_TrainDataParam						= CACFInspectTool.m_TrainDataParam;
	m_TrainGuiParam							= CACFInspectTool.m_TrainGuiParam;
	m_SearchDataParam						= CACFInspectTool.m_SearchDataParam;
	m_SearchGuiParam						= CACFInspectTool.m_SearchGuiParam;
	m_TrainResult							= CACFInspectTool.m_TrainResult;
	m_ImageTrain							= CACFInspectTool.m_ImageTrain;
	m_ImageMask								= CACFInspectTool.m_ImageMask;
	m_bTrainGuiShow							= CACFInspectTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= CACFInspectTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CACFInspectTool.m_bTrainOK;
	m_pGuiDisplay							= CACFInspectTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CACFInspectTool.UpdateSearchGuiData(true);
	CACFInspectTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CACFInspectTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CACFInspectTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CACFInspectTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CACFInspectTool.m_tmpSearchGuiParam);
	SetTrainImage(CACFInspectTool.m_tmpImageTrain);
	SetMaskImage(CACFInspectTool.m_tmpImageMask);
	//if (m_bTrainOK)
	//{

	//	//m_bTrainOK = false;

	//	m_PatternSearchTool = CACFInspectTool.m_PatternSearchTool;
	//	m_QuickSearchTool = CACFInspectTool.m_QuickSearchTool;
	//	

	//}

	if (m_bTrainOK && m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		m_bTrainOK = false;
		bool bRet = false;


		if (m_tmpTrainDataParam.m_bEnableBaseSearch && (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool
			|| m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool))
		{
			m_PatternSearchTool = CACFInspectTool.m_PatternSearchTool;
			m_bTrainOK = CACFInspectTool.m_bTrainOK;
		}

		if (m_tmpTrainDataParam.m_bEnableBaseSearch && (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool
			|| m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool))
		{
			// 设置训练参数
			if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
			{
				return *this;
			}

			if (!m_QuickSearchTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
			{
				return *this;
			}

			if (!m_QuickSearchTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
			{
				return *this;
			}

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

				tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
				GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);
			}		

			// 设置掩模图像
			if (m_TrainDataParam.m_bMaskEnable)
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				m_bTrainOK = bRet;
			}
			else
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				m_bTrainOK = bRet;
			}
		}
	}

	return *this;
}

CACFInspectTool::~CACFInspectTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
	ClearTrainVectorGui();
}

// 获取工具类型
CSearchToolType CACFInspectTool::GetSearchToolType()
{
	return eACFInspectTool;
}					



// 设置训练结果显示控件指针
bool CACFInspectTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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


scGuiDisplay* CACFInspectTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}
// 设置显示控件指针		
bool CACFInspectTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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

	// 设置Gui控件
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	
	// 显示控件
	UpdateTrainGuiDisplay();
	UpdateTrainResultGuiDisplay();
	UpdateSearchResultGuiDisplay();

	return true;
}

// 获取显示控件指针										
scGuiDisplay* CACFInspectTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CACFInspectTool::SetGuiKeyText(CString strGuiKeyText)									
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

// 获取Gui图形的关键字
bool CACFInspectTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CACFInspectTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CACFInspectTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CACFInspectTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CACFInspectTool::UpdateTrainGuiDisplay()
{
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		m_GuiTrainRectRough.SetVisible(m_bTrainGuiShow && m_tmpTrainDataParam.m_bEnableBaseSearch);
		m_GuiTrainAffineRect.SetVisible(false);
		m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainDataParam.m_bEnableBaseSearch);

		m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainDataParam.m_bEnableBaseSearch);
		// 找线工具
		for (int i=0; i<LINE_NUM; i++)	
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_GuiFindLine[i].SetVisible(FALSE);

			if (m_tmpTrainGuiParam.m_bEnableLineMask[i])
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
			}
			else
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
			}
		}

	}
	else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		m_GuiTrainRectRough.SetVisible(false);
		m_GuiTrainAffineRect.SetVisible(m_bTrainGuiShow && m_tmpTrainDataParam.m_bEnableBaseSearch);
		m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainDataParam.m_bEnableBaseSearch);
		m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainDataParam.m_bEnableBaseSearch);
		// 找线工具
		for (int i=0; i<LINE_NUM; i++)	
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_GuiFindLine[i].SetVisible(FALSE);

			if (m_tmpTrainGuiParam.m_bEnableLineMask[i])
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
			}
			else
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
			}
		}
		
	}
	else if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool)
	{
		m_GuiTrainRectRough.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible && m_tmpTrainDataParam.m_bEnableBaseSearch);
		m_GuiTrainAffineRect.SetVisible(false);

		m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible && m_tmpTrainDataParam.m_bEnableBaseSearch);	// 参考点
		m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible && m_tmpTrainDataParam.m_bEnableBaseSearch);	// 搜索矩形
		// 找线工具
		for (int i=0; i<LINE_NUM; i++)	
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_GuiFindLine[i].SetVisible(m_bTrainGuiShow && m_tmpTrainDataParam.m_bEnableBaseSearch && m_tmpTrainGuiParam.m_bLineVisible);

			if (m_tmpTrainGuiParam.m_bEnableLineMask[i])
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
			}
			else
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
			}
		}
	}
	else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool)
	{
		m_GuiTrainRectRough.SetVisible(false);
		m_GuiTrainAffineRect.SetVisible(m_bTrainGuiShow /*&& m_tmpTrainGuiParam.m_bTrainRectVisible*/ && m_tmpTrainDataParam.m_bEnableBaseSearch); // 训练矩形
		m_GuiModelPoint.SetVisible(m_bTrainGuiShow /*&& m_tmpTrainGuiParam.m_bModelPointVisible*/ && m_tmpTrainDataParam.m_bEnableBaseSearch);	// 参考点
		m_GuiSearchRect.SetVisible(m_bTrainGuiShow /*&& m_tmpTrainGuiParam.m_bSearchRectVisible*/ && m_tmpTrainDataParam.m_bEnableBaseSearch);	// 搜索矩形

		// 找线工具
		for (int i=0; i<LINE_NUM; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_GuiFindLine[i].SetVisible(m_bTrainGuiShow && m_tmpTrainDataParam.m_bEnableBaseSearch && m_tmpTrainGuiParam.m_bLineVisible);

			if (m_tmpTrainGuiParam.m_bEnableLineMask[i])
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
			}
			else
			{
				m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
			}
		}
	}


	scFeatureletChainSet tmpscFLCS;
	bool bflag=m_QuickSearchTool.GetTrainFeatures(tmpscFLCS);
	if(bflag)
	{
		m_GuiTrainFeaturelet.SetFeaturelet(tmpscFLCS);
		m_GuiTrainFeaturelet.SetVisible(m_bTrainOK);

		if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
		{
			m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();
			m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet,m_strKeyText+_T("_FeatureChain"));

			m_pGuiTrainResultDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
			m_pGuiTrainResultDisplay->MyInvalidate();
		}
	}

	for (int i=0;i<m_tmpTrainDataParam.m_nInsAreaNum;i++)
	{
		m_GuiTrainRect.at(i)->SetCenterCrossVisible(FALSE);
		m_GuiTrainRect.at(i)->SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);

		CString str;
		if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
		{
			m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
			str.Format("_SearchRect%d",i);
			m_pGuiInteractiveContainer->AddItem(m_GuiTrainRect.at(i), m_strKeyText+str);
	
		}

	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		// 粗定位
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRectRough, m_strKeyText+_T("_TrainRect_Rough"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainAffineRect, m_strKeyText+_T("_TrainAffRect_Rough"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint_Rough"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect_Rough"));

		for (int i=0; i<LINE_NUM; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine[i], strKey);
		}

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();
	}
	return true;
}

bool CACFInspectTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[1].SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		// 可以在主界面显示直线
		//m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));
		//m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[1], m_strKeyText+_T("_TrainResultLine2"));

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CACFInspectTool::UpdateSearchResultGuiDisplay()							
{
	int i=0;

//	m_GuiContour.SetVisible(m_bSearchResultGuiShow);
	
	for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
	{
		m_pGuiSearchAffineRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}

	for (i=0; i<m_pGuiSearchResultRectArray.size(); i++)
	{
		m_pGuiSearchResultRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}

	for (i=0;i<m_pGuiSearchBlobContour.size();i++)
	{
		m_pGuiSearchBlobContour[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}

	for (i=0; i<m_pGuiSearchResultRectArray2.size(); i++)
	{
		m_pGuiSearchResultRectArray2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}
	
	for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
	{
		m_pGuiSearchCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}


	for (i=0; i<m_pGuiNGAffineRectArray.size(); i++)
	{
		m_pGuiNGAffineRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}
	
	// 找线工具
	for (i=0; i<m_pGuiSearchRectArray.size(); i++)
	{
		m_pGuiSearchRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bRectVisible);
	}

	for (i=0; i<m_pGuiSearchLineArray.size(); i++)
	{
		m_pGuiSearchLineArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchResultFitPointArray_1.size(); i++)
	{
		m_pGuiSearchResultFitPointArray_1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint);
	}

	for (i=0; i<m_pGuiSearchResultFitPointArray_2.size(); i++)
	{
		m_pGuiSearchResultFitPointArray_2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint1);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
// 		m_pGuiStaticContainer->AddItem(&m_GuiContour,"blob result");
		
		int i=0;

		for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchAffineRectArray[i], m_pGuiSearchAffineRectArray[i]->GetTipText());
		}
		
		for (i=0; i<m_pGuiSearchResultRectArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultRectArray[i], m_pGuiSearchResultRectArray[i]->GetTipText());
		}
		for (i=0;i<m_pGuiSearchBlobContour.size();i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchBlobContour[i], m_pGuiSearchBlobContour[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchResultRectArray2.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultRectArray2[i], m_pGuiSearchResultRectArray2[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray[i], m_pGuiSearchCrossArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiNGAffineRectArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiNGAffineRectArray[i], m_pGuiNGAffineRectArray[i]->GetTipText());
		}
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

			for (i=0; i<m_pGuiSearchLineArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArray[i], m_pGuiSearchLineArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchResultFitPointArray_1.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPointArray_1[i], m_pGuiSearchResultFitPointArray_1[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchResultFitPointArray_2.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPointArray_2[i], m_pGuiSearchResultFitPointArray_2[i]->GetTipText());
			}
		}
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

bool CACFInspectTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CACFInspectTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eACFInspectTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CACFInspectTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CACFInspectTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eACFInspectTool)
	{
		return false;
	}
	
	*((CACFInspectTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CACFInspectTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eACFInspectTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CACFInspectTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CACFInspectTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eACFInspectTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);
	
	*((CACFInspectTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CACFInspectTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CACFInspectTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_ImageTrain;

	return true;
}

// 设置训练原始图像									
bool CACFInspectTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CACFInspectTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CACFInspectTool::Train()
{
	m_bTrainOK = true;
	m_nResultErrNumber=0;
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

	if (!m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		m_ImageTrain     = m_tmpImageTrain;
		m_ImageMask      = m_tmpImageMask;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam  = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_bTrainOK = true;
		return true;
	}
	else
	{
		if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
		{
			if (!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
			{
				m_nResultErrNumber = -99;
				return false;
			}

			if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
			{
				m_nResultErrNumber = -99;
				return false;
			}

			// 必须在训练时设置角度
			if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
			{
				if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
				{
					m_nResultErrNumber = -99;
					return false;
				}
			}
			else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
			{
				m_nResultErrNumber = -99;
				return false;
			}

			bool bRet = false;
			// 设置掩模图像
			if (m_tmpTrainDataParam.m_bMaskEnable)
			{
				// 		bool bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint,
				// 											&m_tmpTrainGuiParam.m_TrainRect);

				if (bIsProcessImage)
				{
					bRet = m_PatternSearchTool.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint,
						&m_tmpTrainGuiParam.m_TrainRect);
				}
				else
				{
					bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint,
						&m_tmpTrainGuiParam.m_TrainRect);
				}
				m_bTrainOK = bRet;
				if (m_bTrainOK)
				{
					sc2Vector cModelOrigin;
					m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
					m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
					m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

					//计算清晰度
					scImageSharpness tool;
					double dScoreSharp = 0;
					//if (tool.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainRect,dScoreSharp))
					bool bScoreSharp = false;			
					if (bIsProcessImage)
					{
						bScoreSharp = tool.Execute(tmpImage,m_tmpTrainGuiParam.m_TrainRect,dScoreSharp);
					}
					else
					{
						bScoreSharp = tool.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainRect,dScoreSharp);
					}
					if (bScoreSharp)
					{
						m_tmpTrainResult.m_dSharpness = dScoreSharp;
					}
					else
					{
						m_tmpTrainResult.m_dSharpness = 1;
					}

					m_ImageTrain     = m_tmpImageTrain;
					m_ImageMask      = m_tmpImageMask;
					m_TrainDataParam = m_tmpTrainDataParam;
					m_TrainGuiParam  = m_tmpTrainGuiParam;
					m_TrainResult    = m_tmpTrainResult;
				}
				else
				{
					m_nResultErrNumber=-99;////区域定位失败
				}

			}
			else
			{

				if (bIsProcessImage)
				{
					bRet = m_PatternSearchTool.Train(tmpImage, m_tmpTrainGuiParam.m_ModelPoint,
						&m_tmpTrainGuiParam.m_TrainRect);
				}
				else
				{
					bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint,
						&m_tmpTrainGuiParam.m_TrainRect);
				}
				m_bTrainOK = bRet;
				if (m_bTrainOK)
				{
					sc2Vector cModelOrigin;
					m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
					m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
					m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

					//计算清晰度
					scImageSharpness tool;
					double dScoreSharp = 0;
					//if (tool.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainRect,dScoreSharp))
					bool bScoreSharp = false;			
					if (bIsProcessImage)
					{
						bScoreSharp = tool.Execute(tmpImage,m_tmpTrainGuiParam.m_TrainRect,dScoreSharp);
					}
					else
					{
						bScoreSharp = tool.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainRect,dScoreSharp);
					}
					if (bScoreSharp)
					{
						m_tmpTrainResult.m_dSharpness = dScoreSharp;
					}
					else
					{
						m_tmpTrainResult.m_dSharpness = 1;
					}

					m_ImageTrain     = m_tmpImageTrain;
					m_TrainDataParam = m_tmpTrainDataParam;
					m_TrainGuiParam  = m_tmpTrainGuiParam;
					m_TrainResult    = m_tmpTrainResult;
				}
				else
				{
					m_nResultErrNumber=-99;////区域定位失败
				}

			}



			return bRet;



		}
		else if(m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
		{
			if (!m_QuickSearchTool.SetGranularity(m_tmpTrainDataParam.m_dGrau))
			{
				m_nResultErrNumber = -99;
				return false;
			}

			if (!m_QuickSearchTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
			{
				m_nResultErrNumber = -99;
				return false;
			}

			if (!m_QuickSearchTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
			{
				m_nResultErrNumber = -99;
				return false;
			}

			bool bRet = false;
			if (m_tmpTrainDataParam.m_bMaskEnable)
			{	
				if (bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint.GetY());	
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint.GetY());	
				}


				m_bTrainOK = bRet;
				if (m_bTrainOK)
				{
					sc2Vector cModelOrigin;
					m_QuickSearchTool.GetModelOrigin(cModelOrigin);
					m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
					m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

					//计算清晰度
					scImageSharpness tool;
					double dScoreSharp = 0;
					//if (tool.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainAffineRect.BoundingBox(),dScoreSharp))
					bool bScoreSharp = false;
					if (bIsProcessImage)
					{
						bScoreSharp = tool.Execute(tmpImage,m_tmpTrainGuiParam.m_TrainAffineRect.BoundingBox(),dScoreSharp);

					}
					else
					{
						bScoreSharp = tool.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainAffineRect.BoundingBox(),dScoreSharp);
					}
					if (bScoreSharp)
					{
						m_tmpTrainResult.m_dSharpness = dScoreSharp;
					}
					else
					{
						m_tmpTrainResult.m_dSharpness = 1;
					}

					m_ImageTrain     = m_tmpImageTrain;
					m_ImageMask      = m_tmpImageMask;
					m_TrainDataParam = m_tmpTrainDataParam;
					m_TrainGuiParam  = m_tmpTrainGuiParam;
					m_TrainResult    = m_tmpTrainResult;
				}
				if(!bRet)
				{
					m_nResultErrNumber = -99;
				}

			}
			else
			{
				if (bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint.GetY());
				}

				m_bTrainOK = bRet;
				if (m_bTrainOK)
				{
					sc2Vector cModelOrigin;
					m_QuickSearchTool.GetModelOrigin(cModelOrigin);
					m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
					m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

					//计算清晰度
					scImageSharpness tool;
					double dScoreSharp = 0;
					//if (tool.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainAffineRect.BoundingBox(),dScoreSharp))
					bool bScoreSharp = false;
					if (bIsProcessImage)
					{
						bScoreSharp = tool.Execute(tmpImage,m_tmpTrainGuiParam.m_TrainAffineRect.BoundingBox(),dScoreSharp);

					}
					else
					{
						bScoreSharp = tool.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainAffineRect.BoundingBox(),dScoreSharp);
					}
					if (bScoreSharp)
					{
						m_tmpTrainResult.m_dSharpness = dScoreSharp;
					}
					else
					{
						m_tmpTrainResult.m_dSharpness = 1;
					}

					m_ImageTrain     = m_tmpImageTrain;
					m_TrainDataParam = m_tmpTrainDataParam;
					m_TrainGuiParam  = m_tmpTrainGuiParam;
					m_TrainResult    = m_tmpTrainResult;
				}
				if (!bRet)
				{
					m_nResultErrNumber=-99;///几何定位失败
				}

			}

			return bRet;
		}
		else if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool)
		{
			// 设置训练参数
			if (!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
			{
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}

			if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
			{
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}

			// 必须在训练时设置角度
			if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
			{
				if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
				{
					m_nResultErrNumber=-1;       //区域定位失败
					return false;
				}
			}
			else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
			{
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}

			// 设置掩模图像
			if (m_tmpTrainDataParam.m_bMaskEnable)
			{
				bool bRet = false;
				if (bIsProcessImage)
				{
					bRet = m_PatternSearchTool.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint,
						&m_tmpTrainGuiParam.m_TrainRect);
				}
				else
				{
					bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint,
						&m_tmpTrainGuiParam.m_TrainRect);
				}

				if (!bRet)
				{
					m_nResultErrNumber=-1;       //区域定位失败
					return false;
				}

				sc2Vector cModelOrigin;
				m_PatternSearchTool.GetPatternOrigin(cModelOrigin);	// 获取标记点坐标
				m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

				// 找线0
				m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
				m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
				m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);

				m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
					m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);

				if (m_tmpTrainDataParam.m_bEnableDualEdge[0])	// 是否启用双极性
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[0] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[0] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				} 
				else
				{
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				}
				m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[0]);

				scLineSeg cLineSeg1;
				double dRMS1;
				if(bIsProcessImage)
				{
					bRet = m_FindLineTool[0].Execute(tmpImage, cLineSeg1, dRMS1);
				}
				else
				{
					bRet = m_FindLineTool[0].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);	// 找线工具1
				}

				if (!bRet)
				{
					m_nResultErrNumber=-2;       //找线1失败
					return false;
				}


				// 找线1
				m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
				m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
				m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
				//m_FindLineTool[1].SetFitPointIndex(m_tmpTrainDataParam.m_nFitPointIndex[1]);
				m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
					m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);

				if (m_tmpTrainDataParam.m_bEnableDualEdge[1])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[1] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[1] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				} 
				else
				{
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[1]);


				scLineSeg cLineSeg2;
				double dRMS2;
				if(bIsProcessImage)
				{
					bRet = m_FindLineTool[1].Execute(tmpImage, cLineSeg2, dRMS2);
				}
				else
				{
					bRet = m_FindLineTool[1].Execute(m_tmpImageTrain, cLineSeg2, dRMS2);	// 找线工具2
				}

				if (!bRet)
				{
					m_nResultErrNumber=-3;       //找线2失败
					return false;
				}

				// 求交点
				bool bInterSect = false;
				std::vector<sc2Vector> vectArray;
				bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
				if (!bRet || !bInterSect || vectArray.size() <= 0)
				{
					m_nResultErrNumber=-5;       //求交点失败
					return false;
				}

				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();

				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_ImageMask      = m_tmpImageMask;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
				m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
				m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());

				return true;
			}
			else
			{
				bool bRet = false;
				if (bIsProcessImage)
				{
					bRet = m_PatternSearchTool.Train(tmpImage, m_tmpTrainGuiParam.m_ModelPoint,
						&m_tmpTrainGuiParam.m_TrainRect);
				}
				else
				{
					bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint,
						&m_tmpTrainGuiParam.m_TrainRect);
				}

				if (!bRet)
				{
					m_nResultErrNumber=-1;       //区域定位失败
					return false;
				}

				sc2Vector cModelOrigin;
				m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
				m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

				// 找线0
				m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
				m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);

				if (m_tmpTrainDataParam.m_bEnableDualEdge[0])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[0] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[0] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				} 
				else
				{
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				}

				m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
				m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
					m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
				m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[0]);
				//m_FindLineTool[0].SetFitPointIndex(m_tmpTrainDataParam.m_nFitPointIndex[0]);

				scLineSeg cLineSeg1;
				double dRMS1;
				if(bIsProcessImage)
				{
					bRet = m_FindLineTool[0].Execute(tmpImage, cLineSeg1, dRMS1);
				}
				else
				{
					bRet = m_FindLineTool[0].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
				}

				if (!bRet)
				{
					m_nResultErrNumber=-2;       //找线1失败
					return false;
				}

				// 找线1
				m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
				m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
				if (m_tmpTrainDataParam.m_bEnableDualEdge[1])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[1] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[1] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				} 
				else
				{
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}

				m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
				m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
					m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
				m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[1]);
				//m_FindLineTool[1].SetFitPointIndex(m_tmpTrainDataParam.m_nFitPointIndex[1]);

				scLineSeg cLineSeg2;
				double dRMS2;
				if(bIsProcessImage)
				{
					bRet = m_FindLineTool[1].Execute(tmpImage, cLineSeg2, dRMS2);
				}
				else
				{
					bRet = m_FindLineTool[1].Execute(m_tmpImageTrain, cLineSeg2, dRMS2);
				}

				if (!bRet)
				{
					m_nResultErrNumber=-3;       //找线2失败
					return false;
				}

				// 求交点
				bool bInterSect = false;
				std::vector<sc2Vector> vectArray;
				bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
				if (!bRet || !bInterSect || vectArray.size() <= 0)
				{
					m_nResultErrNumber=-5;       //求交点失败
					return false;
				}

				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();

				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
				m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
				m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());

				return true;

			}

			return false;
		}
		else if(m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool)
		{
			// 设置训练参数
			if (!m_QuickSearchTool.SetGranularity(m_tmpTrainDataParam.m_dGrau))
			{
				m_nResultErrNumber = -1;		//几何定位失败
				return false;
			}

			if (!m_QuickSearchTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
			{
				m_nResultErrNumber = -1;		//几何定位失败
				return false;
			}

			if (!m_QuickSearchTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
			{
				m_nResultErrNumber = -1;		//几何定位失败
				return false;
			}	

			// 设置掩模图像
			if (m_tmpTrainDataParam.m_bMaskEnable)
			{
				bool bRet = false;
				if (bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint.GetY());	
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint.GetY());	
				}
				if (!bRet)
				{
					m_nResultErrNumber = -1;	//几何定位失败
					return false;
				}
				sc2Vector cModelOrigin;
				m_QuickSearchTool.GetModelOrigin(cModelOrigin);
				m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

				// 找线0
				m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
				m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
				m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
				//m_FindLineTool[0].SetFitPointIndex(m_tmpTrainDataParam.m_nFitPointIndex[0]);
				m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
					m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);


				if (m_tmpTrainDataParam.m_bEnableDualEdge[0])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[0] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[0] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				} 
				else
				{
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				}
				m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[0]);
				scLineSeg cLineSeg1;
				double dRMS1;
				if(bIsProcessImage)
				{
					bRet = m_FindLineTool[0].Execute(tmpImage, cLineSeg1, dRMS1);
				}
				else
				{
					bRet = m_FindLineTool[0].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
				}

				if (!bRet)
				{
					m_nResultErrNumber = -2;        //找线1失败
					return false;
				}



				// 找线1
				m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
				m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
				m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
				//m_FindLineTool[1].SetFitPointIndex(m_tmpTrainDataParam.m_nFitPointIndex[1]);
				m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
					m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);


				if (m_tmpTrainDataParam.m_bEnableDualEdge[1])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[1] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[1] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				} 
				else
				{
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[1]);
				scLineSeg cLineSeg2;
				double dRMS2;
				if(bIsProcessImage)
				{
					bRet = m_FindLineTool[1].Execute(tmpImage, cLineSeg2, dRMS2);
				}
				else
				{
					bRet = m_FindLineTool[1].Execute(m_tmpImageTrain, cLineSeg2, dRMS2);
				}

				if (!bRet)
				{
					m_nResultErrNumber = -3;         //找线2失败
					return false;
				}

				// 求交点
				bool bInterSect = false;
				std::vector<sc2Vector> vectArray;
				bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
				if (!bRet || !bInterSect || vectArray.size() <= 0)
				{
					m_nResultErrNumber = -5;        //求交点失败
					return false;
				}

				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();

				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_ImageMask      = m_tmpImageMask;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
				m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
				m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());

				return true;
			}
			else
			{
				bool bRet = false;
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint.GetY());
				}

				if (!bRet)
				{
					m_nResultErrNumber = -1;         //几何定位失败
					return false;
				}
				sc2Vector cModelOrigin;
				m_QuickSearchTool.GetModelOrigin(cModelOrigin);
				m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

				// 找线0
				m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
				m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);

				if (m_tmpTrainDataParam.m_bEnableDualEdge[0])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[0] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[0] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				} 
				else
				{
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				}

				m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
				m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
					m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
				m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[0]);
				//m_FindLineTool[0].SetFitPointIndex(m_tmpTrainDataParam.m_nFitPointIndex[0]);

				scLineSeg cLineSeg1;
				double dRMS1;
				if(bIsProcessImage)
				{
					bRet = m_FindLineTool[0].Execute(tmpImage, cLineSeg1, dRMS1);
				}
				else
				{
					bRet = m_FindLineTool[0].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
				}

				if (!bRet)
				{
					m_nResultErrNumber = -2;         //找线1失败
					return false;
				}


				// 找线1
				m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
				m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
				if (m_tmpTrainDataParam.m_bEnableDualEdge[1])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[1] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[1] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				} 
				else
				{
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}

				m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
				m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
					m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
				m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[1]);
				//m_FindLineTool[1].SetFitPointIndex(m_tmpTrainDataParam.m_nFitPointIndex[1]);

				scLineSeg cLineSeg2;
				double dRMS2;
				if(bIsProcessImage)
				{
					bRet = m_FindLineTool[1].Execute(tmpImage, cLineSeg2, dRMS2);
				}
				else
				{
					bRet = m_FindLineTool[1].Execute(m_tmpImageTrain, cLineSeg2, dRMS2);
				}

				if (!bRet)
				{
					m_nResultErrNumber = -3;         //找线2失败
					return false;
				}

				// 求交点
				bool bInterSect = false;
				std::vector<sc2Vector> vectArray;
				bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
				if (!bRet || !bInterSect || vectArray.size() <= 0)
				{
					m_nResultErrNumber = -4;         //求交点失败
					return false;
				}

				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();

				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
				m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
				m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());

				return true;
			}

			return false;
		}
	}





	//m_ImageTrain     = m_tmpImageTrain;
	//m_ImageMask      = m_tmpImageMask;
	//m_TrainDataParam = m_tmpTrainDataParam;
	//m_TrainGuiParam  = m_tmpTrainGuiParam;
	//m_TrainResult    = m_tmpTrainResult;

	m_nResultErrNumber=-99;///失败
	return false;

}

// 获取训练结果图像														
bool CACFInspectTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
	{
		bRet = m_QuickSearchTool.GetTrainModelImage(m_ImageTrainResult);
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
	{
		bRet = m_PatternSearchTool.GetPatternImage(m_ImageTrainResult);
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool)
	{
		bRet = m_PatternSearchTool.GetPatternImage(m_ImageTrainResult);
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool)
	{
		bRet = m_QuickSearchTool.GetTrainModelImage(m_ImageTrainResult);
	}


	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}	

// 获取训练结果
bool CACFInspectTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eACFInspectTool)
	{
		return false;
	}
	
	*((CACFInspectTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}						

// 是否训练成功
bool CACFInspectTool::IsTrained()
{
	return m_bTrainOK;
}															
			
// 设置搜索Data参数	
bool CACFInspectTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eACFInspectTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CACFInspectSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CACFInspectTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eACFInspectTool)
	{
		return false;
	}

	*((CACFInspectSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CACFInspectTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eACFInspectTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CACFInspectSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CACFInspectTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eACFInspectTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);
	
	*((CACFInspectSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CACFInspectTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
		//return false;
	}
	m_nResultErrNumber = 0;
	// 清空
	ClearResult();
	m_bInspectOK = true;
	// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
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
	bool bRet_Rough = false;
	
	double dRotation = 0.0;
	double dTransX =0.0;
	double dTransY =0.0;
	if(m_tmpTrainDataParam.m_nInsAreaNum <1) return false;
	// 位置修正
	CSearchResult searchResult;

	// 初定位
	if (m_TrainDataParam.m_bEnableBaseSearch)
	{
		if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
		{
			// 设置搜索个数
			if (!m_PatternSearchTool.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
			{
				m_nResultErrNumber = -1;
				return false;
			}

			// 设置接收阈值
			if (!m_PatternSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
			{
				m_nResultErrNumber = -1;
				return false;
			}

			if (!m_PatternSearchTool.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre))
			{
				m_nResultErrNumber = -1;
				return false;
			}
			// 执行区域初定位搜索
			if(bIsProcessImage)
			{
				bRet_Rough = m_PatternSearchTool.Execute(tmpImage, m_TrainGuiParam.m_SearchRect);	
			}
			else
			{
				bRet_Rough = m_PatternSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);	
			}


		}
		else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
		{
			if (!m_QuickSearchTool.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
			{
				m_nResultErrNumber = -1;
				return false;
			}

			// 设置接收阈值
			if (!m_QuickSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
			{
				m_nResultErrNumber = -1;
				return false;
			}

			// 设置搜索角度范围
			if (!m_QuickSearchTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
			{
				m_nResultErrNumber = -1;
				return false;
			}

			// 设置缩放比例
			if (!m_QuickSearchTool.SetDofNominalSize(1.0))
			{
				m_nResultErrNumber = -1;
				return false;
			}

			// 设置缩放比例
			if (!m_QuickSearchTool.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
			{
				m_nResultErrNumber = -1;
				return false;
			}
			if(bIsProcessImage)
			{
				bRet_Rough = m_QuickSearchTool.Execute(tmpImage, m_TrainGuiParam.m_SearchRect);
			}
			else
			{
				bRet_Rough = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
			}

		}
		else if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool)
		{
			m_nResultErrNumber=0;
			scLineSeg modelLineSeg1; // 训练时的线段1
			scLineSeg modelLineSeg2; // 训练时的线段2
			modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
			modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));
			modelLineSeg2.SetP1(sc2Vector(m_TrainResult.m_dStartX[1], m_TrainResult.m_dStartY[1]));
			modelLineSeg2.SetP2(sc2Vector(m_TrainResult.m_dEndX[1], m_TrainResult.m_dEndY[1]));

			// 设置搜索个数
			if (!m_PatternSearchTool.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
			{
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}

			// 设置接收阈值
			if (!m_PatternSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
			{
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}

			if (!m_PatternSearchTool.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre))
			{
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}

			for (int i=0; i<LINE_NUM; i++)
			{
				m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
				m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
				,m_TrainGuiParam.m_dCaliperProjectLen[i]);
				//m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
				m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i], eRansacRobust);
				m_FindLineTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore[i]);
				//m_FindLineTool[i].SetFitPointIndex(m_TrainDataParam.m_nFitPointIndex[i]);

				// 找线双极性
				if (m_tmpTrainDataParam.m_bEnableDualEdge[i])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[i] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[i] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
				} 
				else
				{
					m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
				}
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


			// 执行搜索
			//bool bRet = m_PatternSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
			/*bool bRet = false;*/
			if (bIsProcessImage)
			{
				bRet_Rough = m_PatternSearchTool.Execute(tmpImage, m_TrainGuiParam.m_SearchRect);		
			}
			else
			{
				bRet_Rough = m_PatternSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
			}
			if (bRet_Rough)
			{

				/*if (m_tmpSearchDataParam.m_bCoordSortEnable)
				{
					m_PatternSearchTool.SortByCoordXY(m_tmpSearchDataParam.m_dCoordSortGap,m_tmpSearchDataParam.m_nCoordSortMethod,
						m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
				}*/


				// 获取结果
				int nResultNum = m_PatternSearchTool.GetResultNum();
				if (nResultNum <= 0)
				{
					m_nResultErrNumber=-1;
					return false;
				}
				for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
				{
					scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool.GetResult(i);

					// 求变换矩阵
					sc2Vector pos = tmpPatternResult.GetPosition();
					sc2Vector tmModelPoint;
					double dRotation;
					double dTransX;
					double dTransY;
					m_PatternSearchTool.GetPatternOrigin(tmModelPoint);
					dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
					dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
					dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

					scLineSeg ResLine[LINE_NUM];
					double dRMS[LINE_NUM];
					bool   bFindLine[LINE_NUM];
					for (int k=0; k<LINE_NUM; k++)
					{
						bFindLine[k] = 0;
					}

					for (int j=0; j<LINE_NUM; j++)
					{
						// 求找线工具变换后的位置,找线
						double dTransStartX, dTransStartY;
						double dTransEndX, dTransEndY;
						dTransStartX = cos(dRotation) * m_TrainGuiParam.m_dStartX[j]  - sin(dRotation) * m_TrainGuiParam.m_dStartY[j] + dTransX;
						dTransStartY = sin(dRotation) * m_TrainGuiParam.m_dStartX[j]  + cos(dRotation) * m_TrainGuiParam.m_dStartY[j] + dTransY;
						dTransEndX = cos(dRotation) * m_TrainGuiParam.m_dEndX[j]  - sin(dRotation) * m_TrainGuiParam.m_dEndY[j] + dTransX;
						dTransEndY = sin(dRotation) * m_TrainGuiParam.m_dEndX[j]  + cos(dRotation) * m_TrainGuiParam.m_dEndY[j] + dTransY;
						m_FindLineTool[j].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[j], /*scRadian(dRotation)+*/m_TrainGuiParam.m_CaliperSearchRotation[j], m_TrainGuiParam.m_dCaliperSearchLen[j]
						,m_TrainGuiParam.m_dCaliperProjectLen[j]);
						m_FindLineTool[j].SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);
						m_FindLineTool[j].SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask[j]);
						if(bIsProcessImage)
						{
							// 执行找线
							bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, ResLine[j], dRMS[j]); // 成功
						}
						else
						{
							// 执行找线
							bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLine[j], dRMS[j]); // 成功
						}
					}
					// 找线工具1没找到线，找线工具2找到线
					if (!bFindLine[0] && bFindLine[1])
					{

						CSearchResult searchResult;
						sc2Vector pointCenter2;
						//计算找线2的中点
						{
							sc2Vector pointS;
							sc2Vector pointE;
							sc2Vector pointCenter;
							svStd vector<sc2Vector> pointResult;	// 探测点
							svStd vector<bool> used;				// 参与拟合标志
							for (int j=1; j<LINE_NUM; j++)
							{
								m_FindLineTool[j].GetFitData(pointResult);
								m_FindLineTool[j].GetUsed(used);

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
								}
								else
								{
									return false;
								}
							}
						}
						searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
						searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();
						searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();
						searchResult.m_bFindLine2 = TRUE;
						searchResult.m_bFindLine1 = FALSE;
						searchResult.m_resultSearchRect.m_searchRect = tmpPatternResult.MatchRegion();
						searchResult.m_resultSearchRect.m_bFindRect = TRUE;
						CResultSearchRect resRect;
						resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
						searchResult.m_vTrainSearchRect.push_back(resRect);
						CResultLine line;
						int nLine = 1;
						line.m_vLineX[0] = ResLine[nLine].GetP1().GetX();
						line.m_vLineY[0] = ResLine[nLine].GetP1().GetY();
						line.m_vLineX[1] = ResLine[nLine].GetP2().GetX();
						line.m_vLineY[1] = ResLine[nLine].GetP2().GetY();
						searchResult.m_vLineSearchResult.push_back(line);
						m_SearchResultArray.push_back(searchResult);

						m_nResultErrNumber=-2;       //找线1失败
						return false;
					}
					// 找线工具1找到线，找线工具2没找到线
					if (bFindLine[0] && !bFindLine[1])
					{
						CSearchResult searchResult;
						sc2Vector pointCenter1;
						//计算找线1的中点
						{
							sc2Vector pointS;
							sc2Vector pointE;

							svStd vector<sc2Vector> pointResult;	// 探测点
							svStd vector<bool> used;				// 参与拟合标志
							for (int j=0; j<1; j++)
							{
								m_FindLineTool[j].GetFitData(pointResult);
								m_FindLineTool[j].GetUsed(used);

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
									pointCenter1 = (pointS+pointE)/2;


								}
								else
								{
									return false;
								}
							}
						}
						searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
						searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();
						searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
						searchResult.m_bFindLine1=TRUE;
						searchResult.m_bFindLine2=FALSE;
						searchResult.m_resultSearchRect.m_searchRect = tmpPatternResult.MatchRegion();
						searchResult.m_resultSearchRect.m_bFindRect = TRUE;
						CResultSearchRect resRect;
						resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
						searchResult.m_vTrainSearchRect.push_back(resRect);

						CResultLine line;
						int nLine = 0;
						line.m_vLineX[0] = ResLine[nLine].GetP1().GetX();
						line.m_vLineY[0] = ResLine[nLine].GetP1().GetY();
						line.m_vLineX[1] = ResLine[nLine].GetP2().GetX();
						line.m_vLineY[1] = ResLine[nLine].GetP2().GetY();
						searchResult.m_vLineSearchResult.push_back(line);

						m_SearchResultArray.push_back(searchResult);
						m_nResultErrNumber=-3;       //找线2失败
						return false;

					}
					// 两个找线工具都没找到线
					if (!bFindLine[0] && !bFindLine[1])
					{
						CSearchResult searchResult;
						searchResult.m_bFindLine1=FALSE;
						searchResult.m_bFindLine2=FALSE;
						searchResult.m_resultSearchRect.m_searchRect = tmpPatternResult.MatchRegion();
						searchResult.m_resultSearchRect.m_bFindRect = TRUE;
						CResultSearchRect resRect;
						resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
						searchResult.m_vTrainSearchRect.push_back(resRect);
						m_SearchResultArray.push_back(searchResult);
						m_nResultErrNumber=-4;       //找线1失败，找线2失败
						return false;
					}
					// 求交点
					if (bFindLine[0] && bFindLine[1])
					{
						sc2Vector pointCenter1;
						sc2Vector pointCenter2;
						//计算找线1的中点
						{
							sc2Vector pointS;
							sc2Vector pointE;

							svStd vector<sc2Vector> pointResult;	// 探测点
							svStd vector<bool> used;				// 参与拟合标志
							for (int j=0; j<1; j++)
							{
								m_FindLineTool[j].GetFitData(pointResult);
								m_FindLineTool[j].GetUsed(used);

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
									pointCenter1 = (pointS+pointE)/2;


								}
								else
								{
									return false;
								}
							}
						}

						//计算找线2的中点
						{
							sc2Vector pointS;
							sc2Vector pointE;
							sc2Vector pointCenter;
							svStd vector<sc2Vector> pointResult;	// 探测点
							svStd vector<bool> used;				// 参与拟合标志
							for (int j=1; j<LINE_NUM; j++)
							{
								m_FindLineTool[j].GetFitData(pointResult);
								m_FindLineTool[j].GetUsed(used);

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
								}
								else
								{
									return false;
								}
							}
						}
						//计算夹角角度,并进行相关处理
						scRadian InAngle;
						sfAngleLine2Line(ResLine[0].GetLine(),ResLine[1].GetLine(),InAngle);

						sc2Vector vLineCrossPoint;
						bool bIntersect;
						std::vector<sc2Vector> crossPointArray; 
						if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[1].GetLine(), bIntersect, crossPointArray)
							&& crossPointArray.size() > 0)
						{


							CSearchResult searchResult;
							searchResult.m_dPosX = crossPointArray[0].GetX();
							searchResult.m_dPosY = crossPointArray[0].GetY();
							// 角度
							//double dRotAngle = scDegree((ResLine[0].GetRotation()-modelLineSeg1.GetRotation()+ResLine[0].GetRotation()-modelLineSeg1.GetRotation())/2.0).ToDouble(); 
							double dRotAngle = scDegree((ResLine[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180()+ResLine[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180())/2.0).ToDouble(); 
							//searchResult.m_dAngle = dRotAngle;
							searchResult.m_dAngle = scDegree(tmpPatternResult.GetRotate()).ToDouble();
							searchResult.m_dScore = tmpPatternResult.GetScore();

							searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
							searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();

							searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
							searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();

							searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
							searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();

							searchResult.m_bFindLine1 = TRUE;
							searchResult.m_bFindLine2 = TRUE;

							searchResult.m_resultSearchRect.m_searchRect = tmpPatternResult.MatchRegion();
							searchResult.m_resultSearchRect.m_bFindRect = TRUE;

							// 设置结果
							CResultSearchRect resRect;
							resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
							searchResult.m_vTrainSearchRect.push_back(resRect);
							// 线
							for (int nLine = 0; nLine < LINE_NUM; nLine++)
							{
								CResultLine line;
								line.m_vLineX[0] = ResLine[nLine].GetP1().GetX();
								line.m_vLineY[0] = ResLine[nLine].GetP1().GetY();
								line.m_vLineX[1] = ResLine[nLine].GetP2().GetX();
								line.m_vLineY[1] = ResLine[nLine].GetP2().GetY();
								searchResult.m_vLineSearchResult.push_back(line);

							}

							m_SearchResultArray.push_back(searchResult);

							CString	strTmpKey;
							strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
							//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
							scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
							pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
							pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
							pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempAffineRect->SetTipTextEnabled(true);
							pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
							m_pGuiSearchRectArray.push_back(pTempAffineRect);

							strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
							//scGuiCross *pTempCross = new scGuiCross;
							scGuiCross *pTempCross = m_GuiSearchCross+i;
							pTempCross->SetCenterExtent(tmpPatternResult.GetPosition(), sc2Vector(40,40));
							pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
							pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempCross->SetTipTextEnabled(true);
							pTempCross->SetTipText(strTmpKey);
							m_pGuiSearchCrossArray.push_back(pTempCross);

							for (int l=0; l<2; l++)
							{
								strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, l, i);
								//scGuiLine *pTemLine = new scGuiLine;
								scGuiLine *pTemLine = m_GuiSearchLine+i*2+l;
								pTemLine->SetLine(ResLine[l].GetLine());
								pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTemLine->SetTipTextEnabled(true);
								pTemLine->SetTipText(strTmpKey);
								m_pGuiSearchLineArray.push_back(pTemLine);
							}

							strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, i);
							//scGuiCross *pTempLineCross = new scGuiCross;
							scGuiCross *pTempLineCross = m_GuiSearchLineCross+i;
							pTempLineCross->SetCenterExtent(crossPointArray[0], sc2Vector(40,40));
							pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
							pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempLineCross->SetTipTextEnabled(true);
							pTempLineCross->SetTipText(strTmpKey);

							strTmpKey.Format(_T("工具1%s_ResultCross%d"), m_strKeyText, i);
							pTempCross = m_GuiSearchCross2+i*2+1;
							pTempCross->SetCenterExtent(pointCenter1, sc2Vector(40,40));
							pTempCross->SetLineColor(RGB(255,0,0));
							pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempCross->SetTipTextEnabled(true);
							pTempCross->SetTipText(strTmpKey);			
							m_pGuiSearchCrossArray.push_back(pTempCross);

							strTmpKey.Format(_T("工具2%s_ResultCross%d"), m_strKeyText, i);
							pTempCross = m_GuiSearchCross2+i*2+2;
							pTempCross->SetCenterExtent(pointCenter2, sc2Vector(40,40));
							pTempCross->SetLineColor(RGB(255,0,0));
							pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempCross->SetTipTextEnabled(true);
							pTempCross->SetTipText(strTmpKey);			
							m_pGuiSearchCrossArray.push_back(pTempCross);

							CString st;
							st.Format(_T("%d X:%.1f;Y:%.1f;IA:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY(),scDegree(InAngle).NormMod180().ToDouble());
							pTempLineCross->SetLabel(st);
							pTempLineCross->SetLabelVisible(TRUE);
							m_pGuiSearchLineCrossArray.push_back(pTempLineCross);

							// 找线0拟合点
							{
								std::vector<bool> bUsed;
								std::vector<sc2Vector> points;
								m_FindLineTool[0].GetUsed(bUsed);
								m_FindLineTool[0].GetFitData(points);
								for (long m=0; m<bUsed.size(); m++)
								{
									bool bUse = bUsed[m];
									sc2Vector pt = points[m];

									int nIndex = m_pGuiSearchResultFitPointArray_1.size();	// 拟合点个数
									strTmpKey.Format(_T("%s_FitPoint1_%d"), m_strKeyText, nIndex);
									scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint_1 + nIndex;
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
									m_pGuiSearchResultFitPointArray_1.push_back(pTempFitPoint1);	
								}
							}

							// 找线1拟合点
							{
								std::vector<bool> bUsed;
								std::vector<sc2Vector> points;
								m_FindLineTool[1].GetUsed(bUsed);
								m_FindLineTool[1].GetFitData(points);
								for (long m=0; m<bUsed.size(); m++)
								{
									bool bUse = bUsed[m];
									sc2Vector pt = points[m];

									int nIndex = m_pGuiSearchResultFitPointArray_2.size();	// 拟合点个数
									strTmpKey.Format(_T("%s_FitPoint1_1_%d"), m_strKeyText, nIndex);
									scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint_2 + nIndex;
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
									m_pGuiSearchResultFitPointArray_2.push_back(pTempFitPoint1);	
								}
							}

						}

					}

				}	

				if (m_SearchResultArray.size() <= 0)
				{
					return false;
				}

				m_SearchDataParam = m_tmpSearchDataParam;
				m_SearchGuiParam  = m_tmpSearchGuiParam;
			}
			else
			{
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}

			/*return bRet;*/
		}
		else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool)
		{
			m_nResultErrNumber=0;
			scLineSeg modelLineSeg1; // 训练时的线段1
			scLineSeg modelLineSeg2; // 训练时的线段2
			modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
			modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));
			modelLineSeg2.SetP1(sc2Vector(m_TrainResult.m_dStartX[1], m_TrainResult.m_dStartY[1]));
			modelLineSeg2.SetP2(sc2Vector(m_TrainResult.m_dEndX[1], m_TrainResult.m_dEndY[1]));

			// 设置搜索个数
			if (!m_QuickSearchTool.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
			{
				m_nResultErrNumber = -1; // 几何定位失败
				return false;
			}

			// 设置接收阈值
			if (!m_QuickSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
			{
				m_nResultErrNumber = -1; // 几何定位失败
				return false;
			}

			// 设置搜索角度范围
			if (!m_QuickSearchTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
			{
				m_nResultErrNumber = -1; // 几何定位失败
				return false;
			}

			// 设置缩放比例
			if (!m_QuickSearchTool.SetDofNominalSize(1.0))
			{
				m_nResultErrNumber = -1; // 几何定位失败
				return false;
			}

			for (int i=0; i<LINE_NUM; i++)
			{
				m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
				m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
				,m_TrainGuiParam.m_dCaliperProjectLen[i]);
				//m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
				m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i], eRansacRobust);
				m_FindLineTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore[i]);
				//m_FindLineTool[i].SetFitPointIndex(m_TrainDataParam.m_nFitPointIndex[i]);


				// 找线双极性
				if (m_tmpTrainDataParam.m_bEnableDualEdge[i])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[i] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[i] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
				} 
				else
				{
					m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
				}
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

			// 执行搜索
			/*bool bRet = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);*/
			//bool bRet = false;
			if (bIsProcessImage)
			{
				bRet_Rough = m_QuickSearchTool.Execute(tmpImage, m_TrainGuiParam.m_SearchRect);		
			}
			else
			{
				bRet_Rough = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
			}

			if (bRet_Rough)
			{

				/*if (m_tmpSearchDataParam.m_bCoordSortEnable)
				{
				m_QuickSearchTool.SortByCoordXY(m_tmpSearchDataParam.m_dCoordSortGap,m_tmpSearchDataParam.m_nCoordSortMethod,
				m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
				}*/


				// 获取结果
				int nResultNum = m_QuickSearchTool.GetResultNum();
				for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
				{
					scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);

					// 求变换矩阵
					sc2Vector pos = tmpQuickResult.GetPosition();
					sc2Vector tmModelPoint;
					double dRotation;
					double dTransX;
					double dTransY;
					m_QuickSearchTool.GetModelOrigin(tmModelPoint);
					dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
					dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
					dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

					scLineSeg ResLine[LINE_NUM];
					double dRMS[LINE_NUM];
					bool   bFindLine[LINE_NUM];
					for (int k=0; k<LINE_NUM; k++)
					{
						bFindLine[k] = 0;
					}

					for (int j=0; j<LINE_NUM; j++)
					{
						// 求找线工具变换后的位置,找线
						double dTransStartX, dTransStartY;
						double dTransEndX, dTransEndY;
						dTransStartX = cos(dRotation) * m_TrainGuiParam.m_dStartX[j]  - sin(dRotation) * m_TrainGuiParam.m_dStartY[j] + dTransX;
						dTransStartY = sin(dRotation) * m_TrainGuiParam.m_dStartX[j]  + cos(dRotation) * m_TrainGuiParam.m_dStartY[j] + dTransY;
						dTransEndX = cos(dRotation) * m_TrainGuiParam.m_dEndX[j]  - sin(dRotation) * m_TrainGuiParam.m_dEndY[j] + dTransX;
						dTransEndY = sin(dRotation) * m_TrainGuiParam.m_dEndX[j]  + cos(dRotation) * m_TrainGuiParam.m_dEndY[j] + dTransY;
						m_FindLineTool[j].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[j], /*scRadian(dRotation)+*/m_TrainGuiParam.m_CaliperSearchRotation[j], m_TrainGuiParam.m_dCaliperSearchLen[j]
						,m_TrainGuiParam.m_dCaliperProjectLen[j]);
						m_FindLineTool[j].SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);
						m_FindLineTool[j].SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask[j]);

						// 执行找线
						if(bIsProcessImage)
						{
							bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, ResLine[j], dRMS[j]); // 成功
						}
						else
						{
							bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLine[j], dRMS[j]); // 成功
						}
					}

					if (!bFindLine[0] && bFindLine[1])
					{
						CSearchResult searchResult;
						sc2Vector pointCenter2;
						//计算找线2的中点
						{
							sc2Vector pointS;
							sc2Vector pointE;
							sc2Vector pointCenter;
							svStd vector<sc2Vector> pointResult;	// 探测点
							svStd vector<bool> used;				// 参与拟合标志
							for (int j=1; j<LINE_NUM; j++)
							{
								m_FindLineTool[j].GetFitData(pointResult);
								m_FindLineTool[j].GetUsed(used);

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
								}
								else
								{
									return false;
								}
							}
						}
						searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
						searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();
						searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();
						searchResult.m_bFindLine2 = TRUE;
						searchResult.m_bFindLine1 = FALSE;
						searchResult.m_resultSearchRect.m_searchRect = tmpQuickResult.MatchRegion();
						searchResult.m_resultSearchRect.m_bFindRect = TRUE;

						// 设置训练矩形
						CResultSearchRect resRect;
						resRect.m_searchRect = tmpQuickResult.MatchRegion();
						searchResult.m_vTrainSearchRect.push_back(resRect);
						CResultLine line;
						line.m_vLineX[0] = ResLine[1].GetP1().GetX();
						line.m_vLineY[0] = ResLine[1].GetP1().GetY();
						line.m_vLineX[1] = ResLine[1].GetP2().GetX();
						line.m_vLineY[1] = ResLine[1].GetP2().GetY();
						searchResult.m_vLineSearchResult.push_back(line);
						m_SearchResultArray.push_back(searchResult);

						m_nResultErrNumber = -2; // 找线1失败
						return false;

					}
					if (bFindLine[0] && !bFindLine[1])
					{
						CSearchResult searchResult;
						sc2Vector pointCenter1;
						//计算找线1的中点
						{
							sc2Vector pointS;
							sc2Vector pointE;

							svStd vector<sc2Vector> pointResult;	// 探测点
							svStd vector<bool> used;				// 参与拟合标志
							for (int j=0; j<1; j++)
							{
								m_FindLineTool[j].GetFitData(pointResult);
								m_FindLineTool[j].GetUsed(used);

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
									pointCenter1 = (pointS+pointE)/2;


								}
								else
								{
									return false;
								}
							}
						}
						searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
						searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();
						searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
						searchResult.m_bFindLine1=TRUE;
						searchResult.m_bFindLine2=FALSE;
						searchResult.m_resultSearchRect.m_searchRect = tmpQuickResult.MatchRegion();
						searchResult.m_resultSearchRect.m_bFindRect = TRUE;

						// 设置训练矩形
						CResultSearchRect resRect;
						resRect.m_searchRect = tmpQuickResult.MatchRegion();
						searchResult.m_vTrainSearchRect.push_back(resRect);


						// 设置线
						CResultLine line;
						line.m_vLineX[0] = ResLine[0].GetP1().GetX();
						line.m_vLineY[0] = ResLine[0].GetP1().GetY();
						line.m_vLineX[1] = ResLine[0].GetP2().GetX();
						line.m_vLineY[1] = ResLine[0].GetP2().GetY();
						searchResult.m_vLineSearchResult.push_back(line);

						m_SearchResultArray.push_back(searchResult);
						m_nResultErrNumber = -3; // 找线2失败
						return false;
					}
					if (!bFindLine[0] && !bFindLine[1])
					{
						CSearchResult searchResult;
						searchResult.m_bFindLine1=FALSE;
						searchResult.m_bFindLine2=FALSE;
						searchResult.m_resultSearchRect.m_searchRect = tmpQuickResult.MatchRegion();
						searchResult.m_resultSearchRect.m_bFindRect = TRUE;
						// 设置训练矩形
						CResultSearchRect resRect;
						resRect.m_searchRect = tmpQuickResult.MatchRegion();
						searchResult.m_vTrainSearchRect.push_back(resRect);

						m_SearchResultArray.push_back(searchResult);
						m_nResultErrNumber = -4; // 找线1失败，找线2失败
						return false;
					}
					// 求交点
					if (bFindLine[0] && bFindLine[1])
					{
						sc2Vector pointCenter1;
						sc2Vector pointCenter2;

						//计算找线1的中点
						{
							sc2Vector pointS;
							sc2Vector pointE;

							svStd vector<sc2Vector> pointResult;	// 探测点
							svStd vector<bool> used;				// 参与拟合标志
							for (int j=0; j<1; j++)
							{
								m_FindLineTool[j].GetFitData(pointResult);
								m_FindLineTool[j].GetUsed(used);

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
									pointCenter1 = (pointS+pointE)/2;


								}
								else
								{
									return false;
								}
							}
						}

						//计算找线2的中点
						{
							sc2Vector pointS;
							sc2Vector pointE;
							sc2Vector pointCenter;
							svStd vector<sc2Vector> pointResult;	// 探测点
							svStd vector<bool> used;				// 参与拟合标志
							for (int j=1; j<LINE_NUM; j++)
							{
								m_FindLineTool[j].GetFitData(pointResult);
								m_FindLineTool[j].GetUsed(used);

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
								}
								else
								{
									return false;
								}
							}
						}

						//计算夹角角度,并进行相关处理
						scRadian InAngle;
						sfAngleLine2Line(ResLine[0].GetLine(),ResLine[1].GetLine(),InAngle);

						sc2Vector vLineCrossPoint;
						bool bIntersect;
						std::vector<sc2Vector> crossPointArray; 
						if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[1].GetLine(), bIntersect, crossPointArray)
							&& crossPointArray.size() > 0)
						{

							CSearchResult searchResult;


							searchResult.m_dPosX = crossPointArray[0].GetX();
							searchResult.m_dPosY = crossPointArray[0].GetY();
							// 角度
							//double dRotAngle = scDegree((ResLine[0].GetRotation()-modelLineSeg1.GetRotation()+ResLine[0].GetRotation()-modelLineSeg1.GetRotation())/2.0).ToDouble(); 
							//double dRotAngle = scDegree((ResLine[0].GetRotation().SignedNormMod180()-modelLineSeg1.GetRotation().SignedNormMod180()+ResLine[0].GetRotation().SignedNormMod180()-modelLineSeg1.GetRotation().SignedNormMod180())/2.0).ToDouble(); 
							searchResult.m_dAngle = scDegree(tmpQuickResult.GetRotate()).ToDouble();
							searchResult.m_dScore = tmpQuickResult.GetScore();

							searchResult.m_resultSearchRect.m_searchRect = tmpQuickResult.MatchRegion();
							searchResult.m_resultSearchRect.m_bFindRect = TRUE;


							searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
							searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();

							searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
							searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();

							searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
							searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();

							searchResult.m_bFindLine1 = TRUE;
							searchResult.m_bFindLine2 = TRUE;


							// 设置训练矩形
							CResultSearchRect resRect;
							resRect.m_searchRect = tmpQuickResult.MatchRegion();
							searchResult.m_vTrainSearchRect.push_back(resRect);


							// 设置线
							CResultLine line;
							line.m_vLineX[0] = ResLine[0].GetP1().GetX();
							line.m_vLineY[0] = ResLine[0].GetP1().GetY();
							line.m_vLineX[1] = ResLine[0].GetP2().GetX();
							line.m_vLineY[1] = ResLine[0].GetP2().GetY();
							searchResult.m_vLineSearchResult.push_back(line);
							line.m_vLineX[0] = ResLine[1].GetP1().GetX();
							line.m_vLineY[0] = ResLine[1].GetP1().GetY();
							line.m_vLineX[1] = ResLine[1].GetP2().GetX();
							line.m_vLineY[1] = ResLine[1].GetP2().GetY();
							searchResult.m_vLineSearchResult.push_back(line);

							m_SearchResultArray.push_back(searchResult);

							CString	strTmpKey;
							strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
							//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
							scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
							pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
							pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
							pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempAffineRect->SetTipTextEnabled(true);
							pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
							m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);

							strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
							//scGuiCross *pTempCross = new scGuiCross;
							scGuiCross *pTempCross = m_GuiSearchCross+i;
							pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
							pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
							pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempCross->SetTipTextEnabled(true);
							pTempCross->SetTipText(strTmpKey);
							m_pGuiSearchCrossArray.push_back(pTempCross);

							for (int l=0; l<2; l++)
							{
								strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, l, i);
								//scGuiLine *pTemLine = new scGuiLine;
								scGuiLine *pTemLine = m_GuiSearchLine+i*2+l;
								pTemLine->SetLine(ResLine[l].GetLine());
								pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTemLine->SetTipTextEnabled(true);
								pTemLine->SetTipText(strTmpKey);
								m_pGuiSearchLineArray.push_back(pTemLine);
							}

							strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, i);
							//scGuiCross *pTempLineCross = new scGuiCross;
							scGuiCross *pTempLineCross = m_GuiSearchLineCross+i;
							pTempLineCross->SetCenterExtent(crossPointArray[0], sc2Vector(40,40));
							//					pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
							pTempLineCross->SetLineColor(RGB(255, 0, 0));
							pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempLineCross->SetTipTextEnabled(true);
							pTempLineCross->SetTipText(strTmpKey);

							strTmpKey.Format(_T("工具1%s_ResultCross%d"), m_strKeyText, i);
							pTempCross = m_GuiSearchCross2+i*2+1;
							pTempCross->SetCenterExtent(pointCenter1, sc2Vector(40,40));
							pTempCross->SetLineColor(RGB(255,0,0));
							pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempCross->SetTipTextEnabled(true);
							pTempCross->SetTipText(strTmpKey);			
							m_pGuiSearchCrossArray.push_back(pTempCross);

							strTmpKey.Format(_T("工具2%s_ResultCross%d"), m_strKeyText, i);
							pTempCross = m_GuiSearchCross2+i*2+2;
							pTempCross->SetCenterExtent(pointCenter2, sc2Vector(40,40));
							pTempCross->SetLineColor(RGB(255,0,0));
							pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempCross->SetTipTextEnabled(true);
							pTempCross->SetTipText(strTmpKey);			
							m_pGuiSearchCrossArray.push_back(pTempCross);

							//CString st;
							//st.Format(_T("%d X:%.1f;Y:%.1f;IA:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY(),scDegree(InAngle).NormMod180().ToDouble());

							double tempAngle;
							tempAngle = scDegree(InAngle).NormMod180().ToDouble();

							CString st;
							st.Format(_T("%d X:%.1f;Y:%.1f;IA:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY(),tempAngle);
							pTempLineCross->SetLabel(st);
							pTempLineCross->SetLabelVisible(TRUE);
							m_pGuiSearchLineCrossArray.push_back(pTempLineCross);	// 交点Label

							// 找线0拟合点
							{
								std::vector<bool> bUsed;
								std::vector<sc2Vector> points;
								m_FindLineTool[0].GetUsed(bUsed);
								m_FindLineTool[0].GetFitData(points);
								for (long m=0; m<bUsed.size(); m++)
								{
									bool bUse = bUsed[m];
									sc2Vector pt = points[m];

									int nIndex = m_pGuiSearchResultFitPointArray_1.size();
									strTmpKey.Format(_T("%s_FitPoint1_%d"), m_strKeyText, nIndex);
									scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint_1 + nIndex;
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
									m_pGuiSearchResultFitPointArray_1.push_back(pTempFitPoint1);	
								}
							}

							// 找线1拟合点
							{
								std::vector<bool> bUsed;
								std::vector<sc2Vector> points;
								m_FindLineTool[1].GetUsed(bUsed);
								m_FindLineTool[1].GetFitData(points);
								for (long m=0; m<bUsed.size(); m++)
								{
									bool bUse = bUsed[m];
									sc2Vector pt = points[m];

									int nIndex = m_pGuiSearchResultFitPointArray_2.size();
									strTmpKey.Format(_T("%s_FitPoint1_1_%d"), m_strKeyText, nIndex);
									scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint_2 + nIndex;
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
									m_pGuiSearchResultFitPointArray_2.push_back(pTempFitPoint1);	
								}
							}
						}

					}	
				}	

				if (m_SearchResultArray.size() <= 0)
				{
					m_nResultErrNumber = -5; // 求交点失败
					return false;
				}

				m_SearchDataParam = m_tmpSearchDataParam;
				m_SearchGuiParam  = m_tmpSearchGuiParam;
			}
			else
			{
				m_nResultErrNumber = -1;  //几何定位失败
				return false;
			}

			//return true;
		}

		if (bRet_Rough)
		{
			int nResultNum =0;
			
			if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
			{
				nResultNum = m_PatternSearchTool.GetResultNum();
			}
			else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
			{
				nResultNum = m_QuickSearchTool.GetResultNum();
			}
			else if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool)
			{
				nResultNum = m_PatternSearchTool.GetResultNum();
			}
			else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool)
			{
				nResultNum = m_QuickSearchTool.GetResultNum();
			}

			if (nResultNum <= 0)
			{
				m_nResultErrNumber = -1;//粗定位失败
				return false;
			}
			// 1个初定位
			for (int i=0; i<nResultNum && i<m_tmpSearchDataParam.m_nSearchNum && i<MAXRESNUM && i<1; i++)
			{
				
				// 粗定位获取转换矩阵
				if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
				{
					scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool.GetResult(i);

					// 显示粗定位矩形和参考点
					ShowResultRect(m_pGuiSearchResultRectArray2,m_GuiSearchAffineRect2,i,&(tmpPatternResult.MatchRegion()),_T("_ResultRect_"),_T(""),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
					ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpPatternResult.GetPosition(),_T("_ResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

					// 求变换矩阵
					dRotation = 0.0;
					dTransX =0.0;
					dTransY =0.0;

					sc2Vector pos = tmpPatternResult.GetPosition();
					sc2Vector tmModelPoint;
					m_PatternSearchTool.GetPatternOrigin(tmModelPoint);

					// 对searchResult 初步赋值
					searchResult.m_dPosX = tmpPatternResult.GetPosition().GetX();
					searchResult.m_dPosY = tmpPatternResult.GetPosition().GetY();
					searchResult.m_dAngle = tmpPatternResult.GetRotate().ToDouble();
					searchResult.m_dScore = tmpPatternResult.GetScore();

					// 搜索矩形
					CResultSearchRect resRect;
					resRect.m_searchRect = tmpPatternResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);


					dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
					dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
					dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

				}
				else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool)
				{
					scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);

					// 显示粗定位
					ShowResultRect(m_pGuiSearchResultRectArray2,m_GuiSearchAffineRect2,i,&(tmpQuickResult.MatchRegion()),_T("_ResultRect_"),_T(""),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
					//显示粗定位参考点
					ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpQuickResult.GetPosition(),_T("_ResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

					// 求变换矩阵
					dRotation = 0;
					dTransX = 0;
					dTransY = 0;

					sc2Vector pos = tmpQuickResult.GetPosition();
					sc2Vector tmModelPoint;
					m_QuickSearchTool.GetModelOrigin(tmModelPoint);

					// 对searchResult 初步赋值
					searchResult.m_dPosX = tmpQuickResult.GetPosition().GetX();
					searchResult.m_dPosY = tmpQuickResult.GetPosition().GetY();
					searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
					searchResult.m_dScore = tmpQuickResult.GetScore();

					CResultSearchRect resRect;
					resRect.m_searchRect = tmpQuickResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);

					dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
					dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
					dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

				}
				//粗定位获取转换矩阵
				else if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool)
				{
					scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool.GetResult(i);

					// 显示粗定位矩形和参考点
					ShowResultRect(m_pGuiSearchResultRectArray2,m_GuiSearchAffineRect2,i,&(tmpPatternResult.MatchRegion()),_T("_ResultRect_"),_T(""),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
					ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpPatternResult.GetPosition(),_T("_ResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

					// 求变换矩阵
					dRotation = 0.0;
					dTransX =0.0;
					dTransY =0.0;

					sc2Vector pos = tmpPatternResult.GetPosition();
					sc2Vector tmModelPoint;
					m_PatternSearchTool.GetPatternOrigin(tmModelPoint);

					// 对searchResult 初步赋值
					searchResult.m_dPosX = tmpPatternResult.GetPosition().GetX();
					searchResult.m_dPosY = tmpPatternResult.GetPosition().GetY();
					searchResult.m_dAngle = tmpPatternResult.GetRotate().ToDouble();
					searchResult.m_dScore = tmpPatternResult.GetScore();

					// 搜索矩形
					CResultSearchRect resRect;
					resRect.m_searchRect = tmpPatternResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);


					dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
					dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
					dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());
				}

				// 粗定位获取转换矩阵
				else if (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool)
				{
					scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);

					// 显示粗定位
					ShowResultRect(m_pGuiSearchResultRectArray2,m_GuiSearchAffineRect2,i,&(tmpQuickResult.MatchRegion()),_T("_ResultRect_"),_T(""),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
					//显示粗定位参考点
					ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpQuickResult.GetPosition(),_T("_ResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

					// 求变换矩阵
					dRotation = 0;
					dTransX = 0;
					dTransY = 0;

					sc2Vector pos = tmpQuickResult.GetPosition();
					sc2Vector tmModelPoint;
					m_QuickSearchTool.GetModelOrigin(tmModelPoint);

					// 对searchResult 初步赋值
					searchResult.m_dPosX = tmpQuickResult.GetPosition().GetX();
					searchResult.m_dPosY = tmpQuickResult.GetPosition().GetY();
					searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
					searchResult.m_dScore = tmpQuickResult.GetScore();

					CResultSearchRect resRect;
					resRect.m_searchRect = tmpQuickResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);

					dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
					dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
					dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());
				}

			}


		}
		else
		{
			m_nResultErrNumber = -1;
			return false;
		}

	}

	searchResult.m_vACFSearchRect.resize(m_tmpTrainDataParam.m_nInsAreaNum);

	for (int i=0;i<m_tmpTrainDataParam.m_nInsAreaNum;i++)
	{
		searchResult.m_vACFSearchRect.at(i).m_searchRect = *(m_tmpTrainGuiParam.m_InspectAffineRect.at(i));
	}

	if (m_TrainDataParam.m_bEnableBaseSearch && bRet_Rough)
	{

		for (int i=0;i<m_tmpTrainDataParam.m_nInsAreaNum;i++)
		{
			scAffineRect rect2 = *(m_tmpTrainGuiParam.m_InspectAffineRect.at(i));
			double dXBefore = rect2.GetCenter().GetX();
			double dYBefore = rect2.GetCenter().GetY();
			double dXAfter = cos(dRotation) * dXBefore  - sin(dRotation) * dYBefore + dTransX;
			double dYAfter = sin(dRotation) * dXBefore  + cos(dRotation) * dYBefore + dTransY;
			sc2Vector scTmp(dXAfter, dYAfter);


			dXBefore = rect2.GetCornerPx().GetX();
			dYBefore = rect2.GetCornerPx().GetY();
			dXAfter = cos(dRotation) * dXBefore  - sin(dRotation) * dYBefore + dTransX;
			dYAfter = sin(dRotation) * dXBefore  + cos(dRotation) * dYBefore + dTransY;
			sc2Vector PxTmp(dXAfter, dYAfter);


			dXBefore = rect2.GetCornerPo().GetX();
			dYBefore = rect2.GetCornerPo().GetY();
			dXAfter = cos(dRotation) * dXBefore  - sin(dRotation) * dYBefore + dTransX;
			dYAfter = sin(dRotation) * dXBefore  + cos(dRotation) * dYBefore + dTransY;
			sc2Vector PoTmp(dXAfter, dYAfter);

			dXBefore = rect2.GetCornerPy().GetX();
			dYBefore = rect2.GetCornerPy().GetY();
			dXAfter = cos(dRotation) * dXBefore  - sin(dRotation) * dYBefore + dTransX;
			dYAfter = sin(dRotation) * dXBefore  + cos(dRotation) * dYBefore + dTransY;
			sc2Vector PyTmp(dXAfter, dYAfter);

			rect2.SetCenter(scTmp);
			rect2.SetCornersPoPxPy(PoTmp,PxTmp,PyTmp);
			searchResult.m_vACFSearchRect.at(i).m_searchRect = rect2;

		}


		
	}

	// 灰度分析
	std::vector<bool> bInspectResult;
	std::vector<bool> bInspectResult2;
	m_dCurMean.clear();
	m_dCurStdDev.clear();
	m_dCurDefArea.clear();
	m_dmaxGrayValue.clear();
	m_dCurMean.resize(m_tmpTrainDataParam.m_nInsAreaNum,-1.0);
	m_dCurStdDev.resize(m_tmpTrainDataParam.m_nInsAreaNum,-1.0);
	m_dCurDefArea.resize(m_tmpTrainDataParam.m_nInsAreaNum,-1.0);
	m_dmaxGrayValue.resize(m_tmpTrainDataParam.m_nInsAreaNum,-1.0);
	bInspectResult.resize(m_tmpTrainDataParam.m_nInsAreaNum,true);
	bInspectResult2.resize(m_tmpTrainDataParam.m_nInsAreaNum,true);
	
	{
		for (int i=0;i<m_tmpTrainDataParam.m_nInsAreaNum;i++)
		{

			// 调整搜索区
			scAffineRect rect2 = searchResult.m_vACFSearchRect.at(i).m_searchRect;

			if (!m_tmpTrainDataParam.m_bEnableHistogram.at(i))
			{
				continue;
			}

			
			int nRowNum =m_tmpTrainDataParam.m_nRowNum.at(i);
			int nColNum =m_tmpTrainDataParam.m_nColNum.at(i);

			if (nRowNum <1 || nRowNum >MAXROWNUM ||  nColNum>MAXCOLNUM || nColNum<1)
			{
				m_nResultErrNumber = -6;
				return false;
			}

			int nNGNums = 0;
			// 等分

			double dAveMean = 0.0;
			double dAvedCurStdDev = 0.0;
			// 记录每个行列的检测状态  默认0   1-成功代表存在产品  2-失败空穴
			vector<vector<int>> vvintRolColStatus;
			vvintRolColStatus.resize(nRowNum);
			for (int iRow=0;iRow<nRowNum;iRow++)
			{
				vvintRolColStatus.at(iRow).resize(nColNum,0);
			}

			for (int iRow=0;iRow<nRowNum;iRow++)
			{
				for (int iCol=0;iCol<nColNum;iCol++)
				{

					scAffineRect rect;
					sc2Vector po = rect2.GetCornerPo();
					sc2Vector px = rect2.GetCornerPx();
					sc2Vector py = rect2.GetCornerPy();
					sc2Vector popp = rect2.GetCornerPopp();

					sc2Vector ox = px - po;
					sc2Vector oy = py - po;

					// 区域过小时不等分 
					if(ox.Length() <= nColNum) nColNum = 1;
					if (oy.Length() <= nRowNum) nRowNum = 1;


					sc2Vector poij = po+ox/nColNum*iCol+oy/nRowNum*iRow;
					sc2Vector pxij = poij+ox/nColNum;
					sc2Vector pyij = poij+oy/nRowNum;
					sc2Vector poppij =pxij+oy/nRowNum;

					sc2Vector pcenterij = (poij + pxij + pyij+ poppij)/4;
					rect.SetCenter(pcenterij);
					rect.SetCornersPoPxPy(poij,pxij,pyij);

					HStatus		nHStatus;
					svStd vector<long> histogramResult(256, 0);
					if(bIsProcessImage)
					{
						nHStatus = sfHistogram(tmpImage,rect,histogramResult);
					}
					else
					{
						nHStatus = sfHistogram(ImageSearch,rect,histogramResult);
					}
					if (nHStatus!=HSuccess)
					{
						vvintRolColStatus.at(iRow).at(iCol) = 2;
						CResultSearchRect resRect;
						resRect.m_searchRect = rect;
						searchResult.m_vTrainSearchRect.push_back(resRect);
						nNGNums++;
					}
					else
					{
						scHistoStats histoStats(histogramResult);

						double dCurMean = histoStats.Mean();
						double dCurStdDev = histoStats.StdDev();

						dAveMean += dCurMean;
						dAvedCurStdDev += dCurStdDev;

						bool m_bmean=true;
						bool m_bstddev=true;
						CString StrlabelShow = _T("");
						if(nRowNum==1 && nColNum==1)
						{
							StrlabelShow = _T("");
						}
						else
						{
							StrlabelShow.Format(_T("R:%d-C:%d-灰度:%.1f-均值:%.1f"),iRow+1,iCol+1,dCurMean,dCurStdDev);
						}
						
						if (m_tmpTrainDataParam.m_bMeanInvalid.at(i))
						{
							if ((dCurMean<m_tmpTrainDataParam.m_dMinMean.at(i)) || (dCurMean-0.001>m_tmpTrainDataParam.m_dMaxMean.at(i)))
							{
								m_bmean=false;

							}
						}

						if (m_tmpTrainDataParam.m_bStdDevInvalid.at(i))
						{
							if ((dCurStdDev<m_tmpTrainDataParam.m_dMinStdDev.at(i))||(dCurStdDev-0.001>m_tmpTrainDataParam.m_dMaxStdDev.at(i)))
							{
								m_bstddev=false;

							}
						}


						if (m_tmpTrainDataParam.m_bMeanInvert.at(i) && m_tmpTrainDataParam.m_bMeanInvalid.at(i))
						{
							m_bmean = !m_bmean;
						}

						if (m_tmpTrainDataParam.m_bStdDevInvert.at(i) && m_tmpTrainDataParam.m_bStdDevInvalid.at(i))
						{
							m_bstddev = !m_bstddev;
						}

						if (!m_bmean || !m_bstddev)
						{
							nNGNums++;
							vvintRolColStatus.at(iRow).at(iCol) = 2;
							CResultSearchRect resRect;
							resRect.m_searchRect = rect;
							searchResult.m_vTrainSearchRect.push_back(resRect);
							
							//显示NG等分区域
							if(nRowNum>1 ||  nColNum>1)
							{
								ShowResultRect(m_pGuiNGAffineRectArray,m_GuiNGAffineRect, i*nColNum*nRowNum +iRow*nColNum+iCol,&rect,_T("ResultRectNGShow"),StrlabelShow,RGB(255,153,18),m_tmpSearchGuiParam.m_nLineWidth);
							}
						}
						else
						{
							vvintRolColStatus.at(iRow).at(iCol) = 1;
							//显示OK等分区域
							if(nRowNum>1 ||  nColNum>1)
							{
								ShowResultRect(m_pGuiNGAffineRectArray,m_GuiNGAffineRect, i*nColNum*nRowNum +iRow*nColNum+iCol,&rect,_T("ResultRectNGShow"),StrlabelShow,RGB(0,255-153,255-18),m_tmpSearchGuiParam.m_nLineWidth+1);
							}
						}

					}


				}
			}

			dAveMean = dAveMean/(nColNum*nRowNum);
			dAvedCurStdDev = dAvedCurStdDev/(nColNum*nRowNum);
			searchResult.m_vACFRowColStatus = vvintRolColStatus;
			m_dCurMean.at(i) = dAveMean;
			m_dCurStdDev.at(i) = dAvedCurStdDev;

			if (nNGNums >m_tmpTrainDataParam.m_nNGAllowed.at(i))
			{
				bInspectResult.at(i) = false;
				m_nResultErrNumber = -6;
			}

		}
	}

	// 执行Blob分析
	{
		for (int i=0;i<m_tmpTrainDataParam.m_nInsAreaNum;i++)
		{
			if (!m_tmpTrainDataParam.m_bEnableBlob.at(i))
			{
				continue;
			}
			scBlobTool blobTool;
			blobTool.SetSegmentationHardThresh(m_tmpTrainDataParam.m_nBinaryThre.at(i),(m_tmpTrainDataParam.m_nDefectType.at(i) == 1));
			blobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
			blobTool.ConnectivityCleanup(scBlobSceneDescription::eFill);
			blobTool.ConnectivityMinPels(m_tmpTrainDataParam.m_nMinAcceptArea.at(i));
			blobTool.EnableComputeInertia(true);
			blobTool.EnableComputeContour(true);
			bool bRet = false;
			scAffineRect rect = searchResult.m_vACFSearchRect.at(i).m_searchRect;
			
			HStatus		nHStatus;
			svStd vector<long> histogramResult(256, 0);
			if(bIsProcessImage)
			{
				nHStatus = sfHistogram(tmpImage,rect,histogramResult);
			}
			else
			{
				nHStatus = sfHistogram(ImageSearch,rect,histogramResult);
			}
			if (nHStatus!=HSuccess)
			{
				m_dmaxGrayValue.at(i) =-1;
			}
			else
			{
				scHistoStats histoStats(histogramResult);
				m_dmaxGrayValue.at(i) = histoStats.HistMax();
			}
			
			if(bIsProcessImage)
			{
				bRet =  blobTool.Execute(tmpImage,&rect);
			}
			else
			{
				bRet = blobTool.Execute(ImageSearch,&rect);
			}

			if (!bRet)
			{
				m_nResultErrNumber=-7;         //
				m_bInspectOK = false;
				bInspectResult2.at(i) = false;
				
			}
	
			if (bRet)
			{
				scBlobSceneDescription* pBSD = blobTool.GetBSD();
				if (pBSD == NULL)
				{
					m_bInspectFinish = false;
					m_bInspectOK = false;
					bInspectResult2.at(i) = false;
					m_nResultErrNumber = -31;
				}	

				//计算Blob个数
				int nResultBlobNum = 0;
				if (bRet == true)
				{
					pBSD->filter(eArea,m_tmpTrainDataParam.m_nMinAcceptArea.at(i),m_tmpTrainDataParam.m_nMaxAcceptArea.at(i),0);
					scBlob * const *pBlobB = pBSD->allBlobs(false);

					//if (NULL==pBlobB)
					//{
					//	m_nResultErrNumber=-32; 
					//	bInspectResult2.at(i) = false;
					//	return false;
					//}

					double  m_dDefectArea = 0.0;
					nResultBlobNum = pBSD->numBlobs(true); 
					if (nResultBlobNum>=1)
					{
						//计算缺陷面积
						pBSD->sort(eArea,eDescending);
						const scBlob* pBlob = pBSD->getBlob(1);

						if (NULL==pBlob)
						{
							m_nResultErrNumber=-32; 
							bInspectResult2.at(i) = false;
							return false;
						}

						m_dDefectArea= pBlob->area;			
						m_dCurDefArea.at(i) = m_dDefectArea;
						if (m_dDefectArea>m_tmpTrainDataParam.m_dMaxDefectArea.at(i) || m_dDefectArea<0.0)
						{
							m_nResultErrNumber=-34;  //超出缺陷面积
							m_bInspectFinish = true;
							m_bInspectOK = false;
							bInspectResult2.at(i) = false;
						}
						else
						{
							bInspectResult2.at(i) = true;
						}

						//// 主界面显示缺陷外接矩形GUI
						//CResultSearchRect blobRect;
						//blobRect.m_searchRect.SetCenterLengthsRotAndSkew(
						//	sc2Vector(pBlob->boundPrincipalCenterX,pBlob->boundPrincipalCenterY), pBlob->boundPrincipalWidth, pBlob->boundPrincipalHeight,scRadian(scDegree(pBlob->angle)), scRadian(0));

						//blobRect.m_bFindRect = bInspectResult2.at(i);
						//searchResult.m_vACFSearchRect.push_back(blobRect);
					}
					else
					{
						m_nResultErrNumber=-32;  //没有满足有效面积的Blob
						//bInspectResult2.at(i) = false;
					}

					CString strTmpKey = _T("");

					strTmpKey.Format(_T("%s_BlobResultBlobContour%d"), m_strKeyText,i);
					scGuiContour *tmpGuiContour = m_GuiSearchBlobContour + i;
					tmpGuiContour->SetContourBoundary(pBlobB,nResultBlobNum,true,true);
					tmpGuiContour->SetTipTextEnabled(TRUE);

					if (m_bInspectOK)
					{
						tmpGuiContour->SetLineColor(RGB(0,255,0));
					}
					else
					{
						tmpGuiContour->SetLineColor(RGB(255,0,0));

					}
					//CString strLabel = _T("");
					//strLabel.Format(_T("缺陷面积%.3f"),m_dDefectArea);
					//tmpGuiContour->SetLabel(strLabel);
					tmpGuiContour->SetTipText(strTmpKey);
					tmpGuiContour->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					m_pGuiSearchBlobContour.push_back(tmpGuiContour);

				}

			}

		}

	}
	
	CString	strTmpKey;
	CString strLable;

	for (int i=0;i<bInspectResult.size() && i<bInspectResult2.size();i++)
	{
		int nRowNum =1;
		int nColNum =1;

		if(i<m_tmpTrainDataParam.m_nRowNum.size())
		{
			nRowNum = m_tmpTrainDataParam.m_nRowNum.at(i);
		}
		if(i<m_tmpTrainDataParam.m_nColNum.size())
		{
			nColNum = m_tmpTrainDataParam.m_nColNum.at(i);
		}
		
		strLable.Format(_T("ACF%d:("),i);
		strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
		scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
		pTempAffineRect->SetAffineRect(searchResult.m_vACFSearchRect.at(i).m_searchRect);
		pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
		if (!bInspectResult.at(i) || !bInspectResult2.at(i))
		{
			pTempAffineRect->SetLineColor(RGB(255,0,0));
		}
		else
		{
			pTempAffineRect->SetLineColor(RGB(0,255,0));
		}
		pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		pTempAffineRect->SetCenterCrossVisible(FALSE);
		pTempAffineRect->SetTipTextEnabled(true);
		pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
		
		CString Str;
		if (m_tmpTrainDataParam.m_bEnableHistogram.at(i) && m_tmpTrainDataParam.m_bMeanInvalid.at(i))
		{
			Str.Format(_T("_灰度%.1f_"),m_dCurMean.at(i));
			strLable += Str;
		}

		if (m_tmpTrainDataParam.m_bEnableHistogram.at(i) && m_tmpTrainDataParam.m_bStdDevInvalid.at(i))
		{
			Str.Format(_T("_均匀度%.1f_"),m_dCurStdDev.at(i));
			strLable += Str;
		}

		if (m_tmpTrainDataParam.m_bEnableBlob.at(i))
		{
			Str.Format(_T("_缺陷面积%.1f_最大灰度%.1f"),m_dCurDefArea.at(i),m_dmaxGrayValue.at(i));
			strLable += Str;
		}
		strLable += _T(")");
		pTempAffineRect->SetLabel(strLable);
		if(nRowNum==1 && nColNum==1)
		{
			pTempAffineRect->SetLabelVisible(TRUE);
		}
		else
		{
			pTempAffineRect->SetLabelVisible(FALSE);
		}

		m_pGuiSearchResultRectArray.push_back(pTempAffineRect);
	}


	m_bInspectOK = TRUE;
	for (int i=0;i<bInspectResult.size();i++)
	{
		m_bInspectOK = m_bInspectOK && bInspectResult.at(i) && bInspectResult2.at(i);
		searchResult.m_vACFSearchRect.at(i).m_bFindRect= bInspectResult.at(i) && bInspectResult2.at(i);
	}
	m_SearchDataParam = m_tmpSearchDataParam;
	m_SearchGuiParam  = m_tmpSearchGuiParam;
	//sc2Vector resultLeftPos;
	//scAffineRect rect = *(m_tmpTrainGuiParam.m_InspectAffineRect.at(0));
	//
	//resultLeftPos = rect.GetUL();
	//searchResult.m_dPosX = resultLeftPos.GetX();
	//searchResult.m_dPosY = resultLeftPos.GetY();
	//searchResult.m_vdAuxiliaryPosX.at(0) = rect.GetWidth();
	//searchResult.m_vdAuxiliaryPosY.at(0) = rect.GetHeight();

	//int nValidResultNum = (m_tmpTrainDataParam.m_nInsAreaNum > searchResult.m_resultAreaInspectToolEx.m_vdHeight.size())? searchResult.m_resultAreaInspectToolEx.m_vdHeight.size(): m_tmpTrainDataParam.m_nInsAreaNum;
	//searchResult.m_resultAreaInspectToolEx.m_nValidResultNum = nValidResultNum;
	//for (int i=0;i<nValidResultNum;i++)
	//{
	//	scRect rect = *(m_tmpTrainGuiParam.m_SearchRect.at(i));
	//	resultLeftPos = rect.GetUL();
	//	searchResult.m_resultAreaInspectToolEx.m_vdLeftUpX.at(i) = resultLeftPos.GetX();
	//	searchResult.m_resultAreaInspectToolEx.m_vdLeftUpY.at(i) = resultLeftPos.GetY();
	//	searchResult.m_resultAreaInspectToolEx.m_vdWidth.at(i) = rect.GetWidth();
	//	searchResult.m_resultAreaInspectToolEx.m_vdHeight.at(i) = rect.GetHeight();
	//	searchResult.m_resultAreaInspectToolEx.m_vnResult.at(i) = (int)bInspectResult.at(i);
	//}
	//
	//m_SearchResultArray.push_back(searchResult);

	m_SearchResultArray.push_back(searchResult);

	return m_bInspectOK;

}



// 获取搜索结果个数										
int CACFInspectTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CACFInspectTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
	
// 加载模板训练图像
bool CACFInspectTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	CACFInspectTrainDataParam tmpTrainDataParam;
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
	if (cSearchToolType != eACFInspectTool)
	{
		return false;
	}

	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();

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
	ImagePattern = tmpImageModel;

	if (tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool)
	{
		if(cpImage::statusSuccess == tmpImageModel.GetSubImage(ImagePattern,rRect))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool)
	{
		if(cpImage::statusSuccess == tmpImageModel.GetSubImage(ImagePattern,rRect2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}


// 加载模板文件
bool CACFInspectTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) AreaInspect模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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
	CACFInspectTrainDataParam tmpTrainDataParam;
	CACFInspectTrainGuiParam tmpTrainGuiParam;
	CACFInspectSearchDataParam tmpSearchDataParam;
	CACFInspectSearchGuiParam tmpSearchGuiParam;
	CACFInspectTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eACFInspectTool)
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


	bRet = cXMLConfigurator.FindElem(_T("TrainDataParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	{
		tmpTrainDataParam.LoadModelFromFile(cXMLConfigurator);
	}
	cXMLConfigurator.LeaveElem();

	
	bRet = cXMLConfigurator.FindElem(_T("TrainGuiParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	{
		tmpTrainGuiParam.LoadModelFromFile(cXMLConfigurator);
	}
	cXMLConfigurator.LeaveElem();
	
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("SearchParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	{
		//m_tmpSearchDataParam.LoadModelFromFile(cXMLConfigurator);
		tmpSearchDataParam.LoadModelFromFile(cXMLConfigurator);
	}
	cXMLConfigurator.LeaveElem();

	/////////////TrainResult//////////////
	bRet = cXMLConfigurator.FindElem(_T("TrainResult"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	{
		tmpTrainResult.LoadModelFromFile(cXMLConfigurator);
		//m_tmpTrainResult.LoadModelFromFile(cXMLConfigurator);
	}
	cXMLConfigurator.LeaveElem();

	////////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	cpImage tmpImageMask;
	HStatus hStatus;
	cpImage tmpIImageModel;
	//scImageGray tmpImageMask;
	hStatus = cDIB.Init(strDir + _T("Model.bmp"));
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	hStatus = cDIB.GetImage(tmpIImageModel);
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	
	/////////////CameraParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("CamParam"));
	if (!bRet)
	{
		m_CamHeight = tmpIImageModel.Height();
		m_CamWidth = tmpIImageModel.Width();
	}
	else
	{
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("CamHeight"));
			if (!bRet)
			{
				m_CamHeight = tmpIImageModel.Height();
			}
			else
			{
				cXMLConfigurator.GetElemData(m_CamHeight);
			}
			bRet = cXMLConfigurator.FindElem(_T("CamWidth"));
			if (!bRet)
			{
				m_CamWidth = tmpIImageModel.Width();
			}
			else
			{
				cXMLConfigurator.GetElemData(m_CamWidth);
			}		
		}
		cXMLConfigurator.LeaveElem();
	}

	//////////////////////////读取掩模图像//////////////////////////
	{
		if (tmpTrainDataParam.m_bMaskEnable)
		{
			hStatus = cDIB.Init(strDir + _T("Mask.bmp"));
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_bMaskEnable = false;
			}
			else
			{
				hStatus = cDIB.GetImage(tmpImageMask);
				if (!IsSuccess(hStatus))
				{
					tmpTrainDataParam.m_bMaskEnable = false;
				}
			}
		}
	}
	
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}

	 m_tmpImageTrain = tmpIImageModel;

	m_tmpTrainDataParam = tmpTrainDataParam;
	m_tmpTrainGuiParam  = tmpTrainGuiParam;
	m_tmpSearchDataParam= tmpSearchDataParam;
	m_tmpSearchGuiParam = tmpSearchGuiParam;
	m_TrainDataParam	= tmpTrainDataParam;
	m_TrainGuiParam		= tmpTrainGuiParam;
	m_SearchDataParam	= tmpSearchDataParam;
	m_SearchGuiParam	= tmpSearchGuiParam;
	m_TrainResult       = tmpTrainResult;
	m_ImageTrain		= tmpIImageModel;
	m_ImageMask			= tmpImageMask;

	m_bTrainOK = true;
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool && m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		bRet = m_PatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model_Rough.pts"));
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool && m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		bRet = m_QuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model_Rough.ptq"));
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool && m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		bRet = m_PatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model_Rough.pts"));
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool && m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		bRet = m_QuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model_Rough.ptq"));
	}
	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	
	return true;
}
	
// 保存模板文件							
bool CACFInspectTool::SaveModelToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) AreaInspect模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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

	////////////CaneraParam//////////
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
	// ImageChannel
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nImageChannel);
	cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);


	cXMLConfigurator.AddElem(_T("TrainDataParam"));
	cXMLConfigurator.EnterElem();
	{
		m_TrainDataParam.SaveModelToFile(cXMLConfigurator);
	}
	cXMLConfigurator.LeaveElem();
	

	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
	cXMLConfigurator.EnterElem();
	{
		m_TrainGuiParam.SaveModelToFile(cXMLConfigurator);
	}
	cXMLConfigurator.LeaveElem();

	

	
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	cXMLConfigurator.AddElem(_T("SearchParam"));
	cXMLConfigurator.EnterElem();
	{
		m_SearchDataParam.SaveModelToFile(cXMLConfigurator);
	}
	cXMLConfigurator.LeaveElem();
	
	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	{
		m_TrainResult.SaveModelToFile(cXMLConfigurator);
	}

	cXMLConfigurator.LeaveElem();

	bool bRet = cXMLConfigurator.Save(strDir+_T("Model.xml"));
	if (!bRet)
	{
		return false;
	}

	////////////////////////////存储模板图像//////////////////////////
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

		if (m_TrainDataParam.m_bMaskEnable)
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

	//////////////////////////存储模板pts文件//////////////////////////
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool && m_TrainDataParam.m_bEnableBaseSearch)
	{
		bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model_Rough.pts"));
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eQuickSearchTool && m_TrainDataParam.m_bEnableBaseSearch)
	{
		bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model_Rough.ptq"));
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool && m_TrainDataParam.m_bEnableBaseSearch)
	{
		bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model_Rough.pts"));
	}
	if (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool && m_TrainDataParam.m_bEnableBaseSearch)
	{
		bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model_Rough.ptq"));
	}


	if (!bRet)
	{
		return false;
	}

	return true;
}	
	
// 更新训练控件数据信息	
bool CACFInspectTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
// 	m_tmpTrainGuiParam.m_SearchRect.resize(m_tmpTrainDataParam.m_nInsAreaNum);
// 	m_GuiTrainRect.resize(m_tmpTrainDataParam.m_nInsAreaNum);





	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint	= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect =   m_GuiSearchRect.GetRect();
		m_tmpTrainGuiParam.m_TrainRect = m_GuiTrainRectRough.GetRect();
		m_tmpTrainGuiParam.m_TrainAffineRect = m_GuiTrainAffineRect.GetAffineRect();

		for (int i=0;i<m_GuiTrainRect.size();i++)
		{
			*m_tmpTrainGuiParam.m_InspectAffineRect.at(i)= m_GuiTrainRect.at(i)->GetAffineRect();
		}

		for (int i=0; i<LINE_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nCaliperNum[i]		= m_GuiFindLine[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_dCaliperProjectLen[i]	= m_GuiFindLine[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
			m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);
			m_GuiFindLine[i].GetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);
			m_GuiFindLine[i].GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);
			m_tmpTrainGuiParam.m_bCaliperVisible[i]=m_GuiFindLine[i].IsCaliperVisible();
		}

	}
	else // 将数据传递到Gui控件上
	{
		ClearTrainGui();

		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());

		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		m_GuiTrainRectRough.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRectRough.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRectRough.SetRect(m_tmpTrainGuiParam.m_TrainRect);

		m_GuiTrainAffineRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainAffineRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainAffineRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);

		m_tmpTrainGuiParam.m_ModelPoint	= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect =   m_GuiSearchRect.GetRect();
		m_tmpTrainGuiParam.m_TrainRect = m_GuiTrainRectRough.GetRect();
		m_tmpTrainGuiParam.m_TrainAffineRect = m_GuiTrainAffineRect.GetAffineRect();

		for (int i=0; i<LINE_NUM; i++)
		{
			m_GuiFindLine[i].SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiFindLine[i].SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
			m_GuiFindLine[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[i]);
			m_GuiFindLine[i].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);
			m_GuiFindLine[i].SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen[i]);
			m_GuiFindLine[i].SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen[i]);
			m_GuiFindLine[i].SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchRotation[i]);

			m_GuiFindLine[i].SetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);

			// 卡尺可见时，卡尺所在矩形框消失，

			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bCaliperVisible[i] && m_tmpTrainDataParam.m_bEnableBaseSearch);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bCaliperVisible[i] && m_tmpTrainDataParam.m_bEnableBaseSearch);

		}

		for (int i=0;i<m_GuiTrainRect.size();i++)
		{
			if (m_GuiTrainRect.at(i)!=NULL)
			{
				delete m_GuiTrainRect.at(i);
				m_GuiTrainRect.at(i)= NULL;
			}
		}
		m_GuiTrainRect.clear();
		
		

		for (int i=0;i<m_tmpTrainGuiParam.m_InspectAffineRect.size();i++)
		{
			scGuiAffineRect * tempGuiRect = new scGuiAffineRect;
			CString str;

			tempGuiRect->SetCenterLengthsRotationSkew(250+i*100, 200, 50, 50, 0, 0);
			str.Format(_T("ACF检测%d"),i+1);
			tempGuiRect->SetLabel(str);
			tempGuiRect->SetLabelVisible(TRUE);
			m_GuiTrainRect.push_back(tempGuiRect);
			m_GuiTrainRect.at(i)->SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiTrainRect.at(i)->SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
			m_GuiTrainRect.at(i)->SetAffineRect(*m_tmpTrainGuiParam.m_InspectAffineRect.at(i));
		}

	}

	return true;
}

bool CACFInspectTool::SaveTrainGuiData()
{
	return UpdateTrainGuiData(true);
}

bool CACFInspectTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
		int j=0;
		for (j=0; j<m_pGuiSearchAffineRectArray.size(); j++)
		{
			m_pGuiSearchAffineRectArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchAffineRectArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchResultRectArray.size(); j++)
		{
			m_pGuiSearchResultRectArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchResultRectArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

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

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiSearchLineArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiSearchLineCrossArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineCrossArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}
	}
	
	return true;
}

// 清除控件及其显示					
bool CACFInspectTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}	

// 清除定位工具在Display上的控件
bool CACFInspectTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CACFInspectTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchResultRectArray);
	ClearGraphicsArray(m_pGuiSearchBlobContour);
	ClearGraphicsArray(m_pGuiSearchResultRectArray2);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiNGAffineRectArray);

	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_1);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_2);
	m_bInspectFinish	= false;
	m_bInspectOK		= false;

	return true;
}

bool CACFInspectTool::SetGuiByCam(double nWidth,double nHeight)
{

	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;
	
	double centerx,centery, xlen,ylen, rot,skew;
	for (int i=0;i<m_GuiTrainRect.size();i++)
	{
		//训练区域
		
		if (i<m_GuiTrainRect.size())
		{
			m_GuiTrainRect[i]->GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
			m_GuiTrainRect[i]->SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);
		}
	}

	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiTrainAffineRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainAffineRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	m_GuiTrainRectRough.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRectRough.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);


	//卡尺
	double sx,sy,ex,ey;
	m_GuiFindLine[0].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[0].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
	m_GuiFindLine[1].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[1].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);


	double plength,slength;
	plength = m_GuiFindLine[0].GetCaliperProjectionLength();
	slength = m_GuiFindLine[0].GetCaliperSearchLength();
	m_GuiFindLine[0].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[0].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	plength = m_GuiFindLine[1].GetCaliperProjectionLength();
	slength = m_GuiFindLine[1].GetCaliperSearchLength();
	m_GuiFindLine[1].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[1].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CACFInspectTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
 		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
 		
 		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect_Rough"));
 		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainAffRect_Rough"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint_Rough"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect_Rough"));
 
 		for (int i=0;i<m_GuiTrainRect.size();i++)
 		{
 			CString str;
 			str.Format(_T("_SearchRect%d"),i);
 			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+str);	
 
 		}
		int j;
		for (j=0; j<LINE_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine2"));
			
	}


	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}

	return true;
}

bool CACFInspectTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		
		for (j=0; j<m_pGuiSearchAffineRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchAffineRectArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultRectArray[j]->GetTipText());
		}

		for (j=0;j<m_pGuiSearchBlobContour.size();j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchBlobContour[j]->GetTipText());
		}


		for (j=0; j<m_pGuiSearchResultRectArray2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultRectArray2[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiNGAffineRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiNGAffineRectArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchRectArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPointArray_1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPointArray_1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPointArray_2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPointArray_2[j]->GetTipText());
		}

	}

	m_SearchResultArray.clear();

	return true;
}


// 清除vector中的Gui
bool CACFInspectTool::ClearTrainVectorGui()
{
	for (int i=0;i<m_GuiTrainRect.size();i++)
	{
		if (m_GuiTrainRect.at(i)!=NULL)
		{
			delete m_GuiTrainRect.at(i);
			m_GuiTrainRect.at(i)= NULL;
		}
	}
	m_GuiTrainRect.clear();

	return true;
}

// 获取训练模板原始图像
bool CACFInspectTool::GetPatternImage(cpImage &ImagePattern)
{
	bool bRet = false;
	


	if (m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		if (m_tmpImageTrain.IsValid() == false)
		{
			return false;
		}
		if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternSearchTool)
		{
			if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool.GetTrainRectRegion()))
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
			if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool.GetTrainRectRegion()))
			{
				bRet = true;
			}
			else
			{
				bRet = false;
			}
		}

		if (m_tmpTrainDataParam.GetRoughSearchToolType() == ePatternCornerSearchTool)
		{
			if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool.GetTrainRectRegion()))
			{
				bRet = true;
			}
			else
			{
				bRet = false;
			}
		}
		if (m_tmpTrainDataParam.GetRoughSearchToolType() == eCornerSearchTool)
		{
			if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool.GetTrainRectRegion()))
			{
				bRet = true;
			}
			else
			{
				bRet = false;
			}
		}
	}
	else
	{
		ImagePattern = m_ImageTrain;
	}
	

	return bRet;

}

// 恢复训练参数
bool CACFInspectTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}
											
// 恢复搜索参数		
bool CACFInspectTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}


bool CACFInspectTool::ApplyStatisticData()
{
	return true;
}

bool CACFInspectTool::ResetStatisticData()
{
	return true;
}

void CACFInspectTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
	CString str = _T("");
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRectRough.SetLabel(_T("Train Area"));
		m_GuiTrainAffineRect.SetLabel(_T("Train Area1"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));

		for (int i=0;i<m_GuiTrainRect.size();i++)
		{
			str.Format(_T("ACF INSP%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}

		break;
	case 1:
		m_GuiTrainRectRough.SetLabel(_T("训练区域"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		for (int i=0;i<m_GuiTrainRect.size();i++)
		{
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}

		break;
	default:
		m_GuiTrainRectRough.SetLabel(_T("训练区域"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		
		for (int i=0;i<m_GuiTrainRect.size();i++)
		{
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}


		break;
	}

	for (int i=0; i<LINE_NUM; i++)
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


}	

void  CACFInspectTool::ResizeToolsAllParam(int nNewToolNum)
{
	ClearResultGui();
	if (nNewToolNum<0 || (nNewToolNum == m_tmpTrainDataParam.m_nInsAreaNum) || nNewToolNum>MAXALLOWTOOLNUM)
	{
		return;
	}
	//trainDataParam
	
	m_tmpTrainDataParam.m_dMinMean.resize(nNewToolNum,0.0);
	m_tmpTrainDataParam.m_dMaxMean.resize(nNewToolNum,255.0);
	m_tmpTrainDataParam.m_dMinStdDev.resize(nNewToolNum,0.0);
	m_tmpTrainDataParam.m_dMaxStdDev.resize(nNewToolNum,255.0);
	m_tmpTrainDataParam.m_bMeanInvalid.resize(nNewToolNum,TRUE);
	m_tmpTrainDataParam.m_bStdDevInvalid.resize(nNewToolNum,TRUE);
	m_tmpTrainDataParam.m_bMeanInvert.resize(nNewToolNum,FALSE);
	m_tmpTrainDataParam.m_bStdDevInvert.resize(nNewToolNum,FALSE);

	m_tmpTrainDataParam.m_nDefectType.resize(nNewToolNum,0);
	m_tmpTrainDataParam.m_nBinaryThre.resize(nNewToolNum,80);
	m_tmpTrainDataParam.m_nMinAcceptArea.resize(nNewToolNum,3);
	m_tmpTrainDataParam.m_nMaxAcceptArea.resize(nNewToolNum,100000);
	m_tmpTrainDataParam.m_dMaxDefectArea.resize(nNewToolNum,50.0);

	m_tmpTrainDataParam.m_nColNum.resize(nNewToolNum,1);
	m_tmpTrainDataParam.m_nRowNum.resize(nNewToolNum,1);
	m_tmpTrainDataParam.m_nNGAllowed.resize(nNewToolNum,0);

	m_tmpTrainDataParam.m_bEnableHistogram.resize(nNewToolNum,true);
	m_tmpTrainDataParam.m_bEnableBlob.resize(nNewToolNum,false);

	m_tmpTrainGuiParam.m_LineColor =RGB(0, 0, 255);
	m_tmpTrainGuiParam.m_nLineWidth =1;
	m_tmpTrainGuiParam.m_bSearchRectVisible =TRUE;

	//m_tmpSearchGuiParam
	m_tmpSearchGuiParam.m_LineColor = RGB(0, 0, 255);
	m_tmpSearchGuiParam.m_nLineWidth = 1;
	m_tmpSearchGuiParam.m_bAffineRectVisible = TRUE;
	m_tmpSearchGuiParam.m_bModelPointVisible = TRUE;

	m_dCurMean.resize(nNewToolNum,0);
	m_dCurStdDev.resize(nNewToolNum,0);
	m_dCurDefArea.resize(nNewToolNum,0);
	m_dmaxGrayValue.resize(nNewToolNum,0);

	if (nNewToolNum< m_tmpTrainDataParam.m_nInsAreaNum)
	{
		for (int i=m_tmpTrainDataParam.m_nInsAreaNum-1;i>=nNewToolNum;i-- )
		{
			if (m_GuiTrainRect.at(i)!=NULL)
			{
				delete m_GuiTrainRect.at(i);
				m_GuiTrainRect.at(i) = NULL;
			}
			m_GuiTrainRect.erase(m_GuiTrainRect.begin()+i);


			if (m_tmpTrainGuiParam.m_InspectAffineRect.at(i)!=NULL)
			{
				delete m_tmpTrainGuiParam.m_InspectAffineRect.at(i);
				m_tmpTrainGuiParam.m_InspectAffineRect.at(i) = NULL;
			}
			m_tmpTrainGuiParam.m_InspectAffineRect.erase(m_tmpTrainGuiParam.m_InspectAffineRect.begin()+i);
		}


	}
	else
	{
		CString str;
		for (int i=0;i<nNewToolNum;i++)
		{
			if (i>=m_tmpTrainDataParam.m_nInsAreaNum)
			{
				scGuiAffineRect *tempGuiTrainRect = new scGuiAffineRect;	
				scAffineRect* tempAffineRect = new scAffineRect;
				tempGuiTrainRect->SetCenterLengthsRotationSkew(250+i*100, 200, 50, 50, 0, 0);
				str.Format(_T("ACF检测%d"),i+1);
				tempGuiTrainRect->SetLabel(str);
				tempGuiTrainRect->SetLabelVisible(TRUE);
				m_GuiTrainRect.push_back(tempGuiTrainRect);
				m_tmpTrainGuiParam.m_InspectAffineRect.push_back(tempAffineRect);
				*(m_tmpTrainGuiParam.m_InspectAffineRect.at(i))= m_GuiTrainRect.at(i)->GetAffineRect();
			}
		
		}
		
	}

	 m_tmpTrainDataParam.m_nInsAreaNum = nNewToolNum;
	 m_tmpTrainGuiParam.m_nInsAreaNum = nNewToolNum;
	 m_TrainDataParam						= m_tmpTrainDataParam;
	 m_TrainGuiParam							= m_tmpTrainGuiParam;
	 m_SearchDataParam						= m_tmpSearchDataParam;
	 m_SearchGuiParam						= m_tmpSearchGuiParam;

	
}

bool CACFInspectTool::ExecutePatternSerach(cpImage &ImageSearch,scPatternSearchRot* m_PatternSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dConfusionThre,double dTransX,double dTransY,double dRotation)
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

bool CACFInspectTool::ExecuteQuickSearch(cpImage &ImageSearch,scQuickSearchTool* m_QuickSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dAngleLow,double m_dAngleHigh,double m_dSizeLow1,double m_dSizeHigh1,double dTransX,double dTransY,double dRotation)
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
	if (!m_QuickSearchTool->SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
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


void CACFInspectTool::ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,COLORREF color,int LineWidth)
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
	pTempAffineRect->SetCenterCrossVisible(FALSE);
	pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
	m_pGuiSearchRectArray.push_back(pTempAffineRect);
}

void CACFInspectTool::ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,const CString strRectLabel,COLORREF color,int LineWidth)
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
	pTempAffineRect->SetCenterCrossVisible(FALSE);
	pTempAffineRect->SetLabel(strRectLabel);
	pTempAffineRect->SetLabelVisible(TRUE);
	m_pGuiSearchRectArray.push_back(pTempAffineRect);
}


void CACFInspectTool::ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM*4],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,COLORREF color,int LineWidth)
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

void CACFInspectTool::ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM*4],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,const CString strCrossLabel,COLORREF color,int LineWidth)
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

CString	CACFInspectTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());

	switch(m_iLanguage)
	{
	case 1:
		{
			switch (m_nResultErrNumber)
			{
			case -99:
				{
					strErrInfo += _T("训练NG");
				}//训练未成功
				break;
			case -1:
				{
					strErrInfo += _T("定位NG");
				}
				break;
			case -2:
				{
					strErrInfo += _T("找线1失败");
				}
				break;
			case -3:
				{
					strErrInfo += _T("找线2失败");
				}
				break;
			case -4:
				{
					strErrInfo += _T("找线1失败，找线2失败");
				}
				break;
			case -5:
				{
					strErrInfo += _T("求交点失败");
				}
				break;
			case -7:
				{
					strErrInfo += _T("灰度执行NG");
				}
				break;
			case -8:
				{
					strErrInfo += _T("Blob执行NG");
				}
				break;
			case -31:
				{
					strErrInfo += _T("Blob场景描述NG");
				}
				break;
			case -32:
				{
					strErrInfo += _T("Blob接受面积范围NG");
				}
				break;
			case -34:
				{
					strErrInfo += _T("缺陷NG");
				}
				break;
			default:
				strErrInfo += _T("");
				break;
			}
		}
		break;
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -99:
				{
					strErrInfo += _T("Train NG");
				}
				break;
			case -1:
				{
					strErrInfo += _T("Seach NG");
				}
				break;
			case -2:							
				strErrInfo += _T("Find line1 failed");
				break;
			case -3:							
				strErrInfo += _T("Find line2 failed");
				break;										
			case -4:							
				strErrInfo += _T("Find line1 failed，Find line2 failed");
				break;
			case -5:							
				strErrInfo += _T("Find the intersection of line1 and line2 failed");
				break;
			case -6:
				strErrInfo += _T("Gray Execute NG");
				break;
			case -7:
				strErrInfo += _T("Blob Execute NG");
				break;
			case -31:
				{
					strErrInfo += _T("Blob Scene NG");
				}
				break;
			case -32:
				{
					strErrInfo += _T("Blob Accept Area NG");
				}
				break;
			case -34:
				{
					strErrInfo += _T("Defect Area NG");
				}
				break;
			default:
				strErrInfo += _T("");
				break;
			}
		}
		break;
	default:
		{
			switch (m_nResultErrNumber)
			{
			case -99:
				{
					strErrInfo += _T("训练NG");
				}//训练未成功
				break;
			case -1:
				{
					strErrInfo += _T("定位NG");
				}
				break;
			case -2:
				{
					strErrInfo += _T("找线1失败");
				}
				break;
			case -3:
				{
					strErrInfo += _T("找线2失败");
				}
				break;
			case -4:
				{
					strErrInfo += _T("找线1失败，找线2失败");
				}
				break;
			case -5:
				{
					strErrInfo += _T("求交点失败");
				}
				break;
			case -7:
				{
					strErrInfo += _T("灰度执行NG");
				}
				break;
			case -8:
				{
					strErrInfo += _T("Blob执行NG");
				}
				break;
			case -31:
				{
					strErrInfo += _T("Blob场景描述NG");
				}
				break;
			case -32:
				{
					strErrInfo += _T("Blob接受面积范围NG");
				}
				break;
			case -34:
				{
					strErrInfo += _T("缺陷NG");
				}
				break;
			default:
				strErrInfo += _T("");
				break;
			}
		}
		break;
	}
	return strErrInfo;

}