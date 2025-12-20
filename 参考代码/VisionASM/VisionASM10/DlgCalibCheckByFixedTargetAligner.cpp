// DlgCalibCheckByFixedTargetAligner.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibCheckByFixedTargetAligner.h"
#include "afxdialogex.h"
#include <vector>
#include<algorithm>
#include "DlgVisionRunAlignInfo.h"
#include "vsXYDVisionAlign.h"
// CDlgCalibCheckByFixedTargetAligner 对话框

IMPLEMENT_DYNAMIC(CDlgCalibCheckByFixedTargetAligner, CDialogEx)

CDlgCalibCheckByFixedTargetAligner::CDlgCalibCheckByFixedTargetAligner(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCalibCheckByFixedTargetAligner::IDD, pParent)
{
	m_pVisionASM = NULL;
	m_pParent = NULL;
	m_nGetAxisOption = 1;
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = FALSE;
	m_nSeachNums = 4;

	m_nCurTargetSearchIndex = 0;
	m_vbSearchObjectSuss.resize(m_nSeachNums,FALSE);
	m_vbSearchTargetSuss.resize(m_nSeachNums,FALSE);
	m_nCurObjectSearchIndex = 0;
	m_bCandidateSearch = FALSE;
	m_bIsReadOnly = TRUE;
	m_vbIsUpdateAllPos.resize(m_nSeachNums,FALSE);
	m_nExProductIndex = 0; 
}

CDlgCalibCheckByFixedTargetAligner::~CDlgCalibCheckByFixedTargetAligner()
{
	m_pVisionASM = NULL;
	m_pParent = NULL;
}

void CDlgCalibCheckByFixedTargetAligner::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_GET_AXIS_BY_COMM, m_nGetAxisOption);
	DDX_Control(pDX, IDC_LIST_OBJECT_SEARCH_IMAGEPOS, m_ListImagePos);
	DDX_Control(pDX, IDC_LIST_STATISTICS_OBJECT_SEARCH_IMAGEPOS, m_ListImagePosStatistics);
	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE, m_CComboBox_EX_Product);
	DDX_CBIndex(pDX, IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE, m_nExProductIndex);
}


BEGIN_MESSAGE_MAP(CDlgCalibCheckByFixedTargetAligner, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_GET_AXIS_BY_COMM, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedRadioGetAxisByComm)
	ON_BN_CLICKED(IDC_RADIO_GET_AXIS_BY_INPUT, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedRadioGetAxisByInput)
	ON_BN_CLICKED(IDC_BTN_TARGET_SEARCH_POS1, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnTargetSearchPos1)
	ON_BN_CLICKED(IDC_BTN_TARGET_SEARCH_POS2, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnTargetSearchPos2)
	ON_BN_CLICKED(IDC_BTN_TARGET_SEARCH_POS3, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnTargetSearchPos3)
	ON_BN_CLICKED(IDC_BTN_TARGET_SEARCH_POS4, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnTargetSearchPos4)
	ON_BN_CLICKED(IDC_BTN_OBJECT2_SEARCH_POS1, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObject2SearchPos1)
	ON_BN_CLICKED(IDC_BTN_OBJECT2_SEARCH_POS2, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObject2SearchPos2)
	ON_BN_CLICKED(IDC_BTN_OBJECT2_SEARCH_POS3, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObject2SearchPos3)
	ON_BN_CLICKED(IDC_BTN_OBJECT2_SEARCH_POS4, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObject2SearchPos4)
	ON_BN_CLICKED(IDC_BTN_OBJECT_ALIGNER_CALC, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObjectAlignerCalc)
	ON_BN_CLICKED(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnCandidate5ObjectSearchPos1)
	ON_BN_CLICKED(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS2, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnCandidate5ObjectSearchPos2)
	ON_BN_CLICKED(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS3, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnCandidate5ObjectSearchPos3)
	ON_BN_CLICKED(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS4, &CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnCandidate5ObjectSearchPos4)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_OBJECT_SEARCH_IMAGEPOS, &CDlgCalibCheckByFixedTargetAligner::OnNMRClickListObjectSearchImagepos)
	ON_COMMAND(ID_LIST_DEL_ITEM2, &CDlgCalibCheckByFixedTargetAligner::OnListDelItem2)
	ON_COMMAND(ID_LIST_CLEAR_ALL_ITEM2, &CDlgCalibCheckByFixedTargetAligner::OnListClearAllItem2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_X1, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisX1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_Y1, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisY1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_D1, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisD1)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_X2, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisX2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_Y2, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisY2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_D2, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisD2)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_X3, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisX3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_Y3, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisY3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_D3, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisD3)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_X4, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisX4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_Y4, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisY4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_D4, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisD4)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_X1, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisX1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y1, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisY1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_D1, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisD1)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_X2, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisX2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y2, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisY2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_D2, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisD2)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_X3, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisX3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y3, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisY3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_D3, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisD3)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_X4, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisX4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y4, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisY4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_D4, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisD4)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X1, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisX1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y1, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisY1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D1, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisD1)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X2, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisX2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y2, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisY2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D2, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisD2)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X3, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisX3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y3, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisY3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D3, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisD3)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X4, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisX4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y4, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisY4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D4, &CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisD4)
	ON_CBN_SELCHANGE(IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE, &CDlgCalibCheckByFixedTargetAligner::OnCbnSelchangeComboExproductCheckBySize)

	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_X1, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisX1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_Y1, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisY1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_D1, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisD1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_X2, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisX2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_Y2, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisY2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_D2, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisD2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_X3, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisX3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_Y3, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisY3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_D3, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisD3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_X4, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisX4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_Y4, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisY4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_TARGET_AXIS_D4, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisD4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_X1, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisX1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y1, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisY1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_D1, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisD1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_X2, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisX2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y2, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisY2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_D2, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisD2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_X3, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisX3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y3, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisY3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_D3, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisD3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_X4, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisX4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y4, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisY4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_OBJECT_AXIS_D4, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisD4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X1, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisX1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y1, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisY1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D1, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisD1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X2, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisX2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y2, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisY2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D2, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisD2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X3, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisX3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y3, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisY3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D3, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisD3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X4, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisX4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y4, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisY4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D4, &CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisD4)
END_MESSAGE_MAP()


// CDlgCalibCheckByFixedTargetAligner 消息处理程序


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedRadioGetAxisByComm()
{
	UpdateData(TRUE);
	if (m_nGetAxisOption==0)
	{
		for (int i=0;i<59;i++)
		{
			if (NULL !=GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))
			{
				if (  ((CEdit*)GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))!=NULL)
				{
					((CEdit*)GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))->SetReadOnly(TRUE);
				}

			}
		}
		m_bIsReadOnly = TRUE;
	}
	else
	{
		for (int i=0;i<40;i++)
		{
			if (NULL !=GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))
			{
				if (  ((CEdit*)GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))!=NULL)
				{
					((CEdit*)GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))->SetReadOnly(FALSE);
				}

			}
		}
		m_bIsReadOnly = FALSE;
	}
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedRadioGetAxisByInput()
{

	UpdateData(TRUE);
	if (m_nGetAxisOption==0)
	{
		for (int i=0;i<59;i++)
		{
			if (NULL !=GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))
			{
				if (  ((CEdit*)GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))!=NULL)
				{
					((CEdit*)GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))->SetReadOnly(TRUE);
				}

			}
		}
		m_bIsReadOnly = TRUE;
	}
	else
	{
		for (int i=0;i<40;i++)
		{
			if (NULL !=GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))
			{
				if (  ((CEdit*)GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))!=NULL)
				{
					((CEdit*)GetDlgItem(IDC_STATIC_POS_TARGET_AXIS1+i))->SetReadOnly(FALSE);
				}

			}
		}
		m_bIsReadOnly = FALSE;
	}

}

void CDlgCalibCheckByFixedTargetAligner::UpdateStatisticsList(CListCtrl * m_pList,CListCtrl * m_pListStatistics)
{
	// 空指针返回
	if (NULL == m_pList || NULL == m_pListStatistics )
	{
		return;
	}
	// 无表头 返回
	if (NULL == m_pList->GetHeaderCtrl() || NULL == m_pListStatistics->GetHeaderCtrl() )
	{
		ClearListCtrl(m_pListStatistics);
		return;
	}

	int nRowCount = m_pList->GetItemCount();
	int nColsCount = m_pList->GetHeaderCtrl()->GetItemCount();
	int nColsCountStatistics = m_pListStatistics->GetHeaderCtrl()->GetItemCount();

	// 行数不对返回
	if(nRowCount<1)
	{
		ClearListCtrl(m_pListStatistics);
		return;
	}
	// 列数不对应返回
	if (nColsCountStatistics != nColsCount)
	{

		return;
	}

	// 数字检查 返回
	{
		for (int i=0;i<nColsCount-1;i++)
		{
			for (int j=0;j<nRowCount;j++)
			{
				CString str = m_pList->GetItemText(j,i+1);
				CString tmp2= str.SpanIncluding(_T("-.0123456789"));
				if (str !=tmp2)
				{
					return;
				}
			}
		}
	}

	std::vector<std::vector<double>>  vData;
	vData.resize(nColsCount-1);

	for (unsigned  int it = 0;it<vData.size();it++)
	{
		vData.at(it).resize(nRowCount,0.00);
		for (int j=0;j<nRowCount;j++)
		{
			vData.at(it).at(j) = _ttof(m_pList->GetItemText(j,it+1));
		}

	}
	CString strTmp = _T("");
	for (unsigned int it = 0;it<vData.size();it++)
	{
		// 最大值
		std::vector<double>::iterator Tmpiter = max_element(vData.at(it).begin(),vData.at(it).end());
		double dMax =*Tmpiter;
		strTmp.Format(_T("%.3f"),dMax);
		m_pListStatistics->SetItemText(0,it+1,strTmp);

		// 最小值
		Tmpiter = min_element(vData.at(it).begin(),vData.at(it).end());
		double dMin =*Tmpiter;
		strTmp.Format(_T("%.3f"),dMin);
		m_pListStatistics->SetItemText(1,it+1,strTmp);

		// 极差
		strTmp.Format(_T("%.3f"),dMax-dMin);
		m_pListStatistics->SetItemText(2,it+1,strTmp);
	}

}

// // 更新list序号列
void CDlgCalibCheckByFixedTargetAligner::UpdateListColsOrder(CListCtrl * m_pList)
{
	// 空指针返回
	if (NULL == m_pList)
	{
		return;
	}


	for (int i = 0;i<m_pList->GetItemCount();i++)
	{
		CString str = _T("");
		str.Format(_T("%03d"),i+1);
		m_pList->SetItemText(i,0,str);
	}

}

void CDlgCalibCheckByFixedTargetAligner::InitDialogBtnAndEditControl()
{

	//根据搜索位置数隐藏控件
	switch(m_nSeachNums)
	{
	case 2:
		{
			// 隐藏搜索按钮
			for (int i=0;i<10;i++)
			{
				if (NULL !=GetDlgItem(IDC_BTN_TARGET_SEARCH_POS3+i))
				{
					GetDlgItem(IDC_BTN_TARGET_SEARCH_POS3+i)->ShowWindow(SW_HIDE);
				}

				if (NULL !=GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS3+i))
				{
					GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS3+i)->ShowWindow(SW_HIDE);
				}

				if (NULL !=GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS3+i))
				{
					GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS3+i)->ShowWindow(SW_HIDE);
				}

			}

		};
		break;
	case 3:
		{
			// 隐藏搜索按钮
			for (int i=0;i<5;i++)
			{
				if (NULL !=GetDlgItem(IDC_BTN_TARGET_SEARCH_POS4+i))
				{
					GetDlgItem(IDC_BTN_TARGET_SEARCH_POS4+i)->ShowWindow(SW_HIDE);
				}

				if (NULL !=GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS4+i))
				{
					GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS4+i)->ShowWindow(SW_HIDE);
				}

				if (NULL !=GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS4+i))
				{
					GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS4+i)->ShowWindow(SW_HIDE);
				}
			}
		};
		break;
	case 4:
		{

		};
		break;
	default:
		{
			// 默认
		};
		break;
	}
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnTargetSearchPos1()
{
	m_nCurTargetSearchIndex = 0;
	ExeTargetSearch(0);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnTargetSearchPos2()
{
	m_nCurTargetSearchIndex = 1;
	ExeTargetSearch(1);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnTargetSearchPos3()
{
	m_nCurTargetSearchIndex = 2;
	ExeTargetSearch(2);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnTargetSearchPos4()
{
	m_nCurTargetSearchIndex = 3;
	ExeTargetSearch(3);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObject2SearchPos1()
{
	m_bCandidateSearch  =FALSE;
	m_nCurObjectSearchIndex = 0;
	ExeObjectSearch(0);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObject2SearchPos2()
{
	m_bCandidateSearch  =FALSE;
	m_nCurObjectSearchIndex = 1;
	ExeObjectSearch(1);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObject2SearchPos3()
{
	m_bCandidateSearch  =FALSE;
	m_nCurObjectSearchIndex = 2;
	ExeObjectSearch(2);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObject2SearchPos4()
{
	m_bCandidateSearch  =FALSE;
	m_nCurObjectSearchIndex = 3;
	ExeObjectSearch(3);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnObjectAlignerCalc()
{
	// 执行对位

	for (int it =0;it<m_vbSearchObjectSuss.size();it++)
	{
		if(!m_vbSearchObjectSuss.at(it))
		{
			CString strTmp = _T("");
			strTmp.Format(_T("对象位置%d未完成搜索"),it+1);
			AfxMessageBox(strTmp);
			return;
		}
	}

	for (int it =0;it<m_vbSearchTargetSuss.size();it++)
	{
		if(!m_vbSearchTargetSuss.at(it))
		{
			CString strTmp = _T("");
			strTmp.Format(_T("目标位置%d未完成搜索"),it+1);
			AfxMessageBox(strTmp);
			return;
		}
	}

	BOOL bAlignSucess =  ((vcXYDVisionAlign*)m_pVisionASM)->OnlineAlignExFACO(m_nExProductIndex, 0, m_nExProductIndex,0);
	// 刷新结果
	if (bAlignSucess)
	{
		::PostMessageA(m_pParent->GetSafeHwnd(),WM_CALCSIZE,2,NULL);
	}
	
	
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnCandidate5ObjectSearchPos1()
{
	m_bCandidateSearch = TRUE;
	m_nCurObjectSearchIndex = 0;
	ExeObjectSearch(0);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnCandidate5ObjectSearchPos2()
{
	m_bCandidateSearch = TRUE;
	m_nCurObjectSearchIndex = 1;
	ExeObjectSearch(1);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnCandidate5ObjectSearchPos3()
{
	m_bCandidateSearch = TRUE;
	m_nCurObjectSearchIndex = 2;
	ExeObjectSearch(2);
}


void CDlgCalibCheckByFixedTargetAligner::OnBnClickedBtnCandidate5ObjectSearchPos4()
{
	m_bCandidateSearch = TRUE;
	m_nCurObjectSearchIndex =3;
	ExeObjectSearch(3);
}


void CDlgCalibCheckByFixedTargetAligner::OnNMRClickListObjectSearchImagepos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos(); //返回表示屏幕坐标下光标位置的长整数值
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
		CMenu menu;
		VERIFY( menu.LoadMenu( IDR_LIST_POPUPMENU2) );
		CMenu* popup = menu.GetSubMenu(0); //取得被指定菜单激活的下拉式菜单或子菜单的句柄
		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x, point.y, this ); //在指定位置显示快捷菜单，并跟踪菜单项的选择
	}
	*pResult = 0;
}


void CDlgCalibCheckByFixedTargetAligner::OnListDelItem2()
{
	// 删除一行数据
	int CurSel = m_ListImagePos.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(CurSel >= 0)
	{    
		if (CurSel == m_ListImagePos.GetItemCount()-1)
		{
			m_vbIsUpdateAllPos.resize(m_nSeachNums,FALSE);
		}
		m_ListImagePos.DeleteItem(CurSel);
		if (m_pVisionASM!=NULL)
		{
			m_pVisionASM->m_nExecuteCheckTimes = m_ListImagePos.GetItemCount();
		}
		UpdateListColsOrder(&m_ListImagePos);
		UpdateStatisticsList(&m_ListImagePos,&m_ListImagePosStatistics);
	}
}


void CDlgCalibCheckByFixedTargetAligner::OnListClearAllItem2()
{
	m_ListImagePos.DeleteAllItems();
	if (m_pVisionASM!=NULL)
	{
		m_pVisionASM->m_nExecuteCheckTimes = m_ListImagePos.GetItemCount();
	}
	m_vbIsUpdateAllPos.resize(m_nSeachNums,FALSE);
	UpdateListColsOrder(&m_ListImagePos);
	UpdateStatisticsList(&m_ListImagePos,&m_ListImagePosStatistics);
}

void CDlgCalibCheckByFixedTargetAligner::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
{
	m_pVisionASM = pVisionAligner;

}

BOOL CDlgCalibCheckByFixedTargetAligner::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgCalibCheckByFixedTargetAligner::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateDialogLanguage();
	OnBnClickedRadioGetAxisByComm();
	if (m_pVisionASM !=NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		int nPos =  m_pVisionASM->GetPosNum();
		if (nPos == 2)
		{
			m_nSeachNums =2;
		}
		else if (nPos == 4 && platformInfo.m_bTargetObjectCamSeparate == TRUE)
		{
			m_nSeachNums =2;
		}
		else if (nPos == 4 && platformInfo.m_bTargetObjectCamSeparate == FALSE)
		{
			m_nSeachNums =4;
		}
		else if (nPos == 3 && platformInfo.m_bTargetObjectCamSeparate == FALSE)
		{
			m_nSeachNums =3;
		}
		else if (nPos == 6 && platformInfo.m_bTargetObjectCamSeparate == TRUE)
		{
			m_nSeachNums =3;
		}
		else if (nPos == 8 && platformInfo.m_bTargetObjectCamSeparate == TRUE)
		{
			m_nSeachNums =4;
		}

		// 目标按钮显示

		CString strTmp = _T("");
		for (int it = 0;it<4;it++)
		{
			int k = it;
			if (platformInfo.m_bTargetObjectCamSeparate)
			{
				k = it+ m_pVisionASM->GetPosNum()/2;
			}
			strTmp.Format(_T("目标%d拍照"),it+1);
			GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1 + it*5)->SetWindowText(strTmp);
		}

		// 单平台扩展
		if (platformInfo.m_bEnableMultiCalibExtension)
		{
			GetDlgItem(IDC_STATIC_EXPRODUCT_CHECK_BY_SIZE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE)->ShowWindow(SW_SHOW);

		}
		else
		{
			GetDlgItem(IDC_STATIC_EXPRODUCT_CHECK_BY_SIZE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE)->ShowWindow(SW_HIDE);
		}

		// 修复无协议时 获取轴卡顿问题
		if (!m_pVisionASM->m_XYDCommProtocol.IsPlcProtocolType())
		{
			GetDlgItem(IDC_RADIO_GET_AXIS_BY_COMM)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_GET_AXIS_BY_COMM)->ShowWindow(SW_HIDE);
		}

		m_CComboBox_EX_Product.ResetContent();
		CString strNum = _T("1");
		for (int it = 0;it < platformInfo.m_nMultiCalibExtensionMaxNum + 1;it++)
		{
			strNum.Format(_T("%d"),it+1);
			m_CComboBox_EX_Product.InsertString(it,(LPCTSTR)strNum);
		}
		m_CComboBox_EX_Product.SetCurSel(0);
		m_nExProductIndex = 0;

	}
	m_vbSearchObjectSuss.resize(m_nSeachNums,FALSE);
	m_vbSearchTargetSuss.resize(m_nSeachNums,FALSE);
	m_vbIsUpdateAllPos.resize(m_nSeachNums,FALSE); //zzc根据位置数量更新大小
	InitDialogBtnAndEditControl();
	InitDlgListControl(m_nSeachNums);
	InitAxisData();
	InitAxisTextInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	
}


void CDlgCalibCheckByFixedTargetAligner::OnOK()
{
	::PostMessageA(m_pParent->GetSafeHwnd(),WM_DLGDESTROY,2,NULL);
	CDialogEx::OnOK();
}


void CDlgCalibCheckByFixedTargetAligner::OnCancel()
{
	::PostMessageA(m_pParent->GetSafeHwnd(),WM_DLGDESTROY,2,NULL);
	CDialogEx::OnCancel();
}


void CDlgCalibCheckByFixedTargetAligner::ClearListCtrl(CListCtrl * m_pList)
{
	if (NULL == m_pList)
	{
		return;
	}

	for (int i = 0;i<m_pList->GetItemCount();i++)
	{
		for (int j=1;j<m_pList->GetHeaderCtrl()->GetItemCount();j++)
		{
			m_pList->SetItemText(i,j,_T(""));
		}
	}

}


// 尺寸列表初始化
void CDlgCalibCheckByFixedTargetAligner::InitDlgListControl(int nCols)
{
	if (nCols<1)
	{
		return;
	}


	CRect rcListRect;
	GetDlgItem(IDC_LIST_OBJECT_SEARCH_IMAGEPOS)->GetWindowRect(rcListRect);
	ScreenToClient(rcListRect);		  

	m_ListImagePos.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListImagePosStatistics.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_ListImagePos.DeleteAllItems();
	m_ListImagePosStatistics.DeleteAllItems();


	// 初始化图像坐标列表
	int nRow =0;
	int k = 2*nCols+1;

	// 初始化列表

	m_ListImagePos.InsertColumn(0,_T("拍照次数"), LVCFMT_CENTER, rcListRect.Width()/k);
	m_ListImagePosStatistics.InsertColumn(0,_T("统计"), LVCFMT_CENTER, rcListRect.Width()/k);
	
	for (int i=0;i<nCols;i++)
	{
		CString StrTmp = _T("");
		StrTmp.Format(_T("位置%d X"),i+1);
		m_ListImagePos.InsertColumn(2*i+1, StrTmp, LVCFMT_CENTER,  rcListRect.Width()/k);
		m_ListImagePosStatistics.InsertColumn(2*i+1, StrTmp, LVCFMT_CENTER,  rcListRect.Width()/k);
		StrTmp.Format(_T("位置%d Y"),i+1);
		m_ListImagePos.InsertColumn(2*i+2,StrTmp, LVCFMT_CENTER,  rcListRect.Width()/k);
		m_ListImagePosStatistics.InsertColumn(2*i+2,StrTmp, LVCFMT_CENTER,  rcListRect.Width()/k);

	}

	nRow = m_ListImagePosStatistics.InsertItem(0, _T("最大值"));
	nRow = m_ListImagePosStatistics.InsertItem(1, _T("最小值"));
	nRow = m_ListImagePosStatistics.InsertItem(2, _T("极差"));
}



void CDlgCalibCheckByFixedTargetAligner::InitAxisData()
{
	double dOffsetInit = 0.0;
	CString str;
	str.Format(_T("%.4f"), dOffsetInit);
	if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_X)) !=NULL)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_X))->SetWindowText(str);
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_Y)) !=NULL)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_Y))->SetWindowText(str);
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_D)) !=NULL)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_D))->SetWindowText(str);
	}	

	if (NULL!=m_pVisionASM)
	{
		CTotalCalibData *pTotalCalibData;
		pTotalCalibData = m_pVisionASM->GetTotalCalibData();
		if (pTotalCalibData == NULL)
		{
			return;
		}
		int nCurProductIdx = pTotalCalibData->GetCurCalibIndex();

		// 获取当前产品对应的标定数据
		CCalibData* pCalibData;
		pCalibData = pTotalCalibData->GetItemPtr(nCurProductIdx);
		if (pCalibData == NULL)
		{
			return;
		}

		// 获取当前位置对应的标定结果
		CCalibratedInfo* pCalibratedInfo;
		//
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		int nEnd = m_pVisionASM->GetPosNum();
		int nStart = 0;

		if (platformInfo.m_bTargetObjectCamSeparate)
		{
			nEnd = m_pVisionASM->GetPosNum()/2;
		}
		else
		{
			nEnd = m_pVisionASM->GetPosNum();
		}
		// 考虑单平台扩展；
		int nExProBase = 0;
		if (platformInfo.m_bEnableMultiCalibExtension)
		{
			nExProBase = m_nExProductIndex*m_pVisionASM->GetPosNum();
		}

		double dUVW_1 = 0.0;
		double dUVW_2 = 0.0;
		double dUVW_3 = 0.0;

		//对象轴
		for (int i=nExProBase + nStart;i<nExProBase+nEnd;i++)
		{
			pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(i);
			if (pCalibratedInfo != NULL)
			{
				// 轴位置
				CPlatformXYDAxisPos* pCalibPlatformAxisPos = (CPlatformXYDAxisPos*)pCalibratedInfo->GetPlatformAxisPos();
				int k=(i-nStart-nExProBase)*5;
				
				CString str;
				str.Format(_T("%.4f"), dUVW_1);
				if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X1+k)) !=NULL)
				{
					((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X1+k))->SetWindowText(str);
				}
				if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y1+k)) !=NULL)
				{
					((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y1+k))->SetWindowText(str);
				}
				if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D1+k)) !=NULL)
				{
					((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D1+k))->SetWindowText(str);
				}
				if (platformInfo.m_eMidPlatformType == ePlatformXY1Y2)
				{
					CPlatformXY1Y2Info platformXY1Y2Info;
					platformXY1Y2Info.ConvertXYDPltfmPosToXY1Y2PltfmPos(pCalibPlatformAxisPos->m_dPosX, pCalibPlatformAxisPos->m_dPosY, pCalibPlatformAxisPos->m_dAngle,
						dUVW_2, dUVW_3, dUVW_1);//Y1、Y2、X

					str.Format(_T("%.4f"), dUVW_1);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2))->SetWindowText(str);
					}

					str.Format(_T("%.4f"), dUVW_2);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3))->SetWindowText(str);
					}
					str.Format(_T("%.4f"), dUVW_3);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4))->SetWindowText(str);
					}
				}
				else if (platformInfo.m_eMidPlatformType == ePlatformX1X2Y)
				{
					CPlatformX1X2YInfo platformX1X2YInfo;
					platformX1X2YInfo.ConvertXYDPltfmPosToX1X2YPltfmPos(pCalibPlatformAxisPos->m_dPosX, pCalibPlatformAxisPos->m_dPosY, pCalibPlatformAxisPos->m_dAngle,
						dUVW_1, dUVW_2, dUVW_3);//X1、X2、Y

					str.Format(_T("%.4f"), dUVW_1);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2))->SetWindowText(str);
					}

					str.Format(_T("%.4f"), dUVW_2);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3))->SetWindowText(str);
					}
					str.Format(_T("%.4f"), dUVW_3);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4))->SetWindowText(str);
					}
				}
				else
				{
					str.Format(_T("%.4f"),pCalibPlatformAxisPos->m_dPosX);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2))->SetWindowText(str);
					}

					str.Format(_T("%.4f"),pCalibPlatformAxisPos->m_dPosY);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3))->SetWindowText(str);
					}
					str.Format(_T("%.4f"),pCalibPlatformAxisPos->m_dAngle);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4))->SetWindowText(str);
					}
				}
			}
		}

		// 目标轴
		if (platformInfo.m_bTargetObjectCamSeparate)
		{
			nStart = m_pVisionASM->GetPosNum()/2;
			nEnd = m_pVisionASM->GetPosNum();
		}

		for (int i=nExProBase +nStart;i<nExProBase +nEnd;i++)
		{
			pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(i);
			if (pCalibratedInfo != NULL)
			{
				// 轴位置
				CPlatformXYDAxisPos* pCalibPlatformAxisPos = (CPlatformXYDAxisPos*)pCalibratedInfo->GetPlatformAxisPos();
				int k=(i-nStart-nExProBase )*5;
				CString str;
				if (platformInfo.m_eMidPlatformType == ePlatformXY1Y2)
				{
					CPlatformXY1Y2Info platformXY1Y2Info;
					platformXY1Y2Info.ConvertXYDPltfmPosToXY1Y2PltfmPos(pCalibPlatformAxisPos->m_dPosX, pCalibPlatformAxisPos->m_dPosY, pCalibPlatformAxisPos->m_dAngle,
						dUVW_2, dUVW_3, dUVW_1);//Y1、Y2、X

					str.Format(_T("%.4f"), dUVW_1);
					if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2))->SetWindowText(str);
					}

					str.Format(_T("%.4f"), dUVW_2);
					if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3))->SetWindowText(str);
					}
					str.Format(_T("%.4f"), dUVW_3);
					if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4))->SetWindowText(str);
					}
				}
				else if (platformInfo.m_eMidPlatformType == ePlatformX1X2Y)
				{
					CPlatformX1X2YInfo platformX1X2YInfo;
					platformX1X2YInfo.ConvertXYDPltfmPosToX1X2YPltfmPos(pCalibPlatformAxisPos->m_dPosX, pCalibPlatformAxisPos->m_dPosY, pCalibPlatformAxisPos->m_dAngle,
						dUVW_1, dUVW_2, dUVW_3);//X1、X2、Y

					str.Format(_T("%.4f"), dUVW_1);
					if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2))->SetWindowText(str);
					}

					str.Format(_T("%.4f"), dUVW_2);
					if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3))->SetWindowText(str);
					}
					str.Format(_T("%.4f"), dUVW_3);
					if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4))->SetWindowText(str);
					}
				}
				else
				{
					str.Format(_T("%.4f"),pCalibPlatformAxisPos->m_dPosX);
					if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2))->SetWindowText(str);
					}

					str.Format(_T("%.4f"),pCalibPlatformAxisPos->m_dPosY);
					if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3))->SetWindowText(str);
					}
					str.Format(_T("%.4f"),pCalibPlatformAxisPos->m_dAngle);
					if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4))->SetWindowText(str);
					}
				}

			}
		}

	}
}

void CDlgCalibCheckByFixedTargetAligner::InitAxisTextInfo()
{
	if (NULL!=m_pVisionASM)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		if (platformInfo.m_eMidPlatformType == ePlatformX1X2Y)
		{
			SetDlgItemText(IDC_STATIC_AXIS_X_DATA, _T("X1(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_Y_DATA, _T("X2(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_D_DATA, _T("Y(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_X_DATA2, _T("X1(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_Y_DATA2, _T("X2(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_D_DATA2, _T("Y(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_X_DATA3, _T("X1(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_Y_DATA3, _T("X2(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_D_DATA3, _T("Y(mm)"));
		}
		else if (platformInfo.m_eMidPlatformType == ePlatformXY1Y2)
		{
			SetDlgItemText(IDC_STATIC_AXIS_X_DATA, _T("X(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_Y_DATA, _T("Y1(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_D_DATA, _T("Y2(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_X_DATA2, _T("X(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_Y_DATA2, _T("Y1(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_D_DATA2, _T("Y2(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_X_DATA3, _T("X(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_Y_DATA3, _T("Y1(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_D_DATA3, _T("Y2(mm)"));
		}
	}
}

void CDlgCalibCheckByFixedTargetAligner::UpdateTargetAxisInfo()
{
	int k = m_nCurTargetSearchIndex*5;
	// X
	if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2)) !=NULL)
	{
		CString str = _T("");
		str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(0));
		((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2))->SetWindowText(str);
	}

	// Y
	if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3)) !=NULL)
	{
		CString str = _T("");
		str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(1));
		((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3))->SetWindowText(str);
	}
	// D
	if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4)) !=NULL)
	{
		CString str = _T("");
		str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(2));
		((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4))->SetWindowText(str);
	}

}

void CDlgCalibCheckByFixedTargetAligner::UpdateObjectAxisInfo()
{
	

	int k = m_nCurObjectSearchIndex*5;
	if(!m_bCandidateSearch)
	{
		// X
		if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2)) !=NULL)
		{
			CString str = _T("");
			str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(0));
			((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2))->SetWindowText(str);
		}
		// Y
		if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3)) !=NULL)
		{
			CString str = _T("");
			str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(1));
			((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3))->SetWindowText(str);
		}
		// D
		if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4)) !=NULL)
		{
			CString str = _T("");
			str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(2));
			((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4))->SetWindowText(str);
		}
	}
	else
	{
		
		// X
		if (((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+2)) !=NULL)
		{
			CString str = _T("");
			str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(0));
			((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+2))->SetWindowText(str);
		}
		// Y
		if (((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+3)) !=NULL)
		{
			CString str = _T("");
			str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(1));
			((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+3))->SetWindowText(str);
		}
		// D
		if (((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+4)) !=NULL)
		{
			CString str = _T("");
			str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(2));
			((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+4))->SetWindowText(str);
		}
	}

}

void CDlgCalibCheckByFixedTargetAligner::ExeObjectSearch(int nPos)
{

	if (NULL !=m_pVisionASM)
	{
		if (nPos<0 || nPos>=m_pVisionASM->GetPosNum())
		{
			return;
		}

		//
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		// 对轴进行处理  手动输入轴时，暂时只有UVW平台类型进行转换XYD处理； 
		if (m_nGetAxisOption == 1)
		{
			int k=nPos*5;
			CString strTmp = _T("");
			((vcXYDVisionAlign*)m_pVisionASM)->m_bCmdParamAxisPosEanble = TRUE;

			// UVW 平台
			std::vector<CString> vstrAxisPos;


			if (!m_bCandidateSearch)
			{
				// UVW-1
				if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2)) !=NULL)
				{
					((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+2))->GetWindowText(strTmp);
				}
				vstrAxisPos.push_back(strTmp);

				// UVW-2
				if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3)) !=NULL)
				{
					((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+3))->GetWindowText(strTmp);
				}
				vstrAxisPos.push_back(strTmp);

				// UVW-3
				if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4)) !=NULL)
				{
					((CEdit*)GetDlgItem(IDC_BTN_OBJECT2_SEARCH_POS1+k+4))->GetWindowText(strTmp);
				}
				vstrAxisPos.push_back(strTmp);
			} 
			else
			{
				// X
				if (((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+2)) !=NULL)
				{
					((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+2))->GetWindowText(strTmp);
				}
				vstrAxisPos.push_back(strTmp);

				// Y
				if (((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+3)) !=NULL)
				{
					((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+3))->GetWindowText(strTmp);
				}
				vstrAxisPos.push_back(strTmp);

				// D
				if (((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+4)) !=NULL)
				{
					((CEdit*)GetDlgItem(IDC_BTN_CANDIDATE5_OBJECT_SEARCH_POS1+k+4))->GetWindowText(strTmp);
				}
				vstrAxisPos.push_back(strTmp);
			}

			if (platformInfo.m_eMidPlatformType == ePlatformX1X2Y || platformInfo.m_eMidPlatformType == ePlatformXY1Y2)
			{	
				m_pVisionASM->m_XYDCommProtocol.ConvertToPlatformXYDAxisPos(vstrAxisPos, &(m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos));
			}
			else
			{
				m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos.m_dPosX = atof(vstrAxisPos.at(0));
				m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos.m_dPosY = atof(vstrAxisPos.at(1));
				m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos.m_dAngle = atof(vstrAxisPos.at(2));
			}

		}
		else
		{
			CString strAttention;
			strAttention = m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings->GetAt(IDS_MB_PLEASE_PLC_SEND_CUR_PLATFORM_POS);
			AfxMessageBox(strAttention);
		}


		UpdateData(TRUE);
		if (platformInfo.m_bEnableMultiCalibExtension)
		{

			m_pVisionASM->SetAlignExProductIndex(m_nExProductIndex);
			vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
			if (tmpAlignTool !=NULL)
			{
				tmpAlignTool->m_bEnableMultiCombinationCode = FALSE;
			}
			
		}

		std::vector<int> vnPosIndex;
		int nStartCandidate = 0;
		vnPosIndex.push_back(nPos);
		if (m_bCandidateSearch)
		{
			nStartCandidate= 4;
		}
		m_vbSearchObjectSuss.at(nPos) =  ((vcXYDVisionAlign*)m_pVisionASM)->OnSerialCameraAlignerObjectSearchCandidateExFOBC(vnPosIndex, nStartCandidate);


		((vcXYDVisionAlign*)m_pVisionASM)->m_bCmdParamAxisPosEanble = FALSE;
	}
}

void CDlgCalibCheckByFixedTargetAligner::ExeTargetSearch(int nPos)
{

	if (NULL !=m_pVisionASM)
	{
		if (nPos<0 || nPos>=m_pVisionASM->GetPosNum())
		{
			return;
		}

		//
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		// 对轴进行处理  手动输入轴时，暂时只有UVW平台类型进行转换XYD处理； 
		if (m_nGetAxisOption == 1)
		{
			int k=nPos*5;
			CString strTmp = _T("");
			((vcXYDVisionAlign*)m_pVisionASM)->m_bCmdParamAxisPosEanble = TRUE;

			// UVW 平台
			std::vector<CString> vstrAxisPos;

			// UVW-1
			if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2)) !=NULL)
			{
				((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+2))->GetWindowText(strTmp);
			}
			vstrAxisPos.push_back(strTmp);

			// UVW-2
			if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3)) !=NULL)
			{
				((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+3))->GetWindowText(strTmp);
			}
			vstrAxisPos.push_back(strTmp);

			// UVW-3
			if (((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4)) !=NULL)
			{
				((CEdit*)GetDlgItem(IDC_BTN_TARGET_SEARCH_POS1+k+4))->GetWindowText(strTmp);
			}
			vstrAxisPos.push_back(strTmp);

			if (platformInfo.m_eMidPlatformType == ePlatformX1X2Y || platformInfo.m_eMidPlatformType == ePlatformXY1Y2)
			{	
				m_pVisionASM->m_XYDCommProtocol.ConvertToPlatformXYDAxisPos(vstrAxisPos, &(m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos));
			}
			else
			{
				m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos.m_dPosX = atof(vstrAxisPos.at(0));
				m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos.m_dPosY = atof(vstrAxisPos.at(1));
				m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos.m_dAngle = atof(vstrAxisPos.at(2));
			}
		}
		else
		{
			CString strAttention;
			strAttention = m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings->GetAt(IDS_MB_PLEASE_PLC_SEND_CUR_PLATFORM_POS);
			AfxMessageBox(strAttention);
		}
		UpdateData(TRUE);
		if (platformInfo.m_bEnableMultiCalibExtension)
		{

			vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
			if (tmpAlignTool != NULL)
			{
				m_pVisionASM->SetAlignExProductIndex(m_nExProductIndex);
				tmpAlignTool->m_bEnableMultiCombinationCode = FALSE;
			}

		}

		std::vector<int> vnPosIndex;
		int nStartCandidate = 0;

		if (platformInfo.m_bTargetObjectCamSeparate)
		{
			vnPosIndex.push_back(nPos+m_pVisionASM->GetPosNum()/2);
		}
		else
		{
			vnPosIndex.push_back(nPos);
		}

		m_vbSearchTargetSuss.at(nPos)  = ((vcXYDVisionAlign*)m_pVisionASM)->OnSerialCameraAlignerTargetSearchCandidateExFTGC(vnPosIndex, 0);

		((vcXYDVisionAlign*)m_pVisionASM)->m_bCmdParamAxisPosEanble = FALSE;
	}
}

void CDlgCalibCheckByFixedTargetAligner::UpdateProcessInfo()
{
	double dX0 = 0.0;
	double dY0 = 0.0;
	double dD0 = 0.0;
	double dUVW_1 = 0.0;
	double dUVW_2 = 0.0;
	double dUVW_3 = 0.0;
	double dM1 = 0.0;
	double dM2 = 0.0;
	double dM3 = 0.0;

	if (m_pVisionASM !=NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		if (platformInfo.m_eMidPlatformType == ePlatformX1X2Y)
		{
			CPlatformX1X2YInfo platformX1X2YInfo;
			platformX1X2YInfo.ConvertXYDPltfmPosToX1X2YPltfmPos(m_AlignerProcessInfo.dXMove, m_AlignerProcessInfo.dYMove, m_AlignerProcessInfo.dDMove,
				dUVW_1, dUVW_2, dUVW_3);
			// dUVW_1 Move
			if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_X)) !=NULL)
			{
				CString str = _T("");
				str.Format(_T("%.4f"),dUVW_1);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_X))->SetWindowText(str);
			}
			// dUVW_2 Move
			if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_Y)) !=NULL)
			{
				CString str = _T("");
				str.Format(_T("%.4f"),dUVW_2);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_Y))->SetWindowText(str);
			}
			// dUVW_3 Move
			if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_D)) !=NULL)
			{
				CString str = _T("");
				str.Format(_T("%.4f"),dUVW_3);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_D))->SetWindowText(str);
			}
			dM1 = dUVW_1;
			dM2 = dUVW_2;
			dM3 = dUVW_3;
		}
		else if (platformInfo.m_eMidPlatformType == ePlatformXY1Y2)
		{
			CPlatformXY1Y2Info platformXY1Y2Info;
			platformXY1Y2Info.ConvertXYDPltfmPosToXY1Y2PltfmPos(m_AlignerProcessInfo.dXMove, m_AlignerProcessInfo.dYMove, m_AlignerProcessInfo.dDMove,
				dUVW_2, dUVW_3, dUVW_1);
			// dUVW_1 Move
			if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_X)) !=NULL)
			{
				CString str = _T("");
				str.Format(_T("%.4f"),dUVW_1);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_X))->SetWindowText(str);
			}
			// dUVW_2 Move
			if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_Y)) !=NULL)
			{
				CString str = _T("");
				str.Format(_T("%.4f"),dUVW_2);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_Y))->SetWindowText(str);
			}
			// dUVW_3 Move
			if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_D)) !=NULL)
			{
				CString str = _T("");
				str.Format(_T("%.4f"),dUVW_3);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_D))->SetWindowText(str);
			}
			dM1 = dUVW_1;
			dM2 = dUVW_2;
			dM3 = dUVW_3;
		}
		else
		{
			// X Move
			if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_X)) !=NULL)
			{
				CString str = _T("");
				str.Format(_T("%.4f"),m_AlignerProcessInfo.dXMove);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_X))->SetWindowText(str);
			}

			// Y Move
			if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_Y)) !=NULL)
			{
				CString str = _T("");
				str.Format(_T("%.4f"),m_AlignerProcessInfo.dYMove);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_Y))->SetWindowText(str);
			}

			// D Move
			if (((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_D)) !=NULL)
			{
				CString str = _T("");
				str.Format(_T("%.4f"),m_AlignerProcessInfo.dDMove);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_PLATFORM_OFFSET_D))->SetWindowText(str);
			}
			dM1 = m_AlignerProcessInfo.dXMove;
			dM2 = m_AlignerProcessInfo.dYMove;
			dM3 = m_AlignerProcessInfo.dDMove;
		}

		if (m_nGetAxisOption == 1 /* && platformInfo.m_eMidPlatformType != ePlatformX1X2Y && platformInfo.m_eMidPlatformType != ePlatformXY1Y2*/) // 手动输入轴时 更新候补5的轴输入值 暂时不支持UVW刷新
		{
			for (int i=0;i<m_nSeachNums;i++)
			{
				int k= 5*i;
				CString strTmp = _T("");
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_OBJECT_AXIS_X1+k))->GetWindowText(strTmp);
				CString tmp2= strTmp.SpanIncluding(_T("-.0123456789"));
				if (strTmp !=tmp2)
				{
					return;
				}
				double dAxis_X = atof(strTmp);
				dX0 = dAxis_X + dM1;	//当前值+移动量，XYD坐标

				((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y1+k))->GetWindowText(strTmp);
				tmp2= strTmp.SpanIncluding(_T("-.0123456789"));
				if (strTmp !=tmp2)
				{
					return;
				}
				dAxis_X = atof(strTmp);
				dY0 = dAxis_X + dM2;

				((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_OBJECT_AXIS_D1+k))->GetWindowText(strTmp);
				tmp2= strTmp.SpanIncluding(_T("-.0123456789"));
				if (strTmp !=tmp2)
				{
					return;
				}
				dAxis_X = atof(strTmp);
				dD0 = dAxis_X + dM3;

				strTmp.Format(_T("%4f"), dX0);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X1+k))->SetWindowText(strTmp);
				strTmp.Format(_T("%.4f"), dY0);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y1+k))->SetWindowText(strTmp);
				strTmp.Format(_T("%.4f"), dD0);
				((CEdit*)GetDlgItem(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D1+k))->SetWindowText(strTmp);
			}
		}
	}


}

void CDlgCalibCheckByFixedTargetAligner::UpdateImagePosDataList(const std::vector<double> vData)
{
	if (vData.size()!=2)
	{
		return;
	}

	BOOL bIsAllUpdateAll = TRUE;
	for (int it=0;it<m_vbIsUpdateAllPos.size();it++)
	{
		bIsAllUpdateAll = bIsAllUpdateAll && m_vbIsUpdateAllPos[it];
	}
	CString tmpStr=_T("");
	int nRow = m_ListImagePos.GetItemCount();
	if (bIsAllUpdateAll)  // 已经全部刷新过 新起行
	{
		tmpStr.Format(_T("%03d"),nRow+1);
		m_ListImagePos.InsertItem(nRow,tmpStr);
		for (int it=0;it<2;it++)
		{
			int nCols = 2*m_nCurObjectSearchIndex+1+it;
			tmpStr.Format(_T("%.3f"),vData[it]);
			m_ListImagePos.SetItemText(nRow,nCols,tmpStr);
		}

		for (int it=0;it<m_vbIsUpdateAllPos.size();it++)
		{
			m_vbIsUpdateAllPos[it] = FALSE;
		}

		m_vbIsUpdateAllPos[m_nCurObjectSearchIndex]=TRUE;
	}
	else
	{
		if (nRow==0) //第一次刷新数据，没有行先插入
		{
			tmpStr.Format(_T("%03d"),nRow+1);
			m_ListImagePos.InsertItem(nRow,tmpStr);
			for (int it=0;it<2;it++)
			{
				int nCols = 2*m_nCurObjectSearchIndex+1+it;
				tmpStr.Format(_T("%.3f"),vData[it]);
				m_ListImagePos.SetItemText(nRow,nCols,tmpStr);
			}
			m_vbIsUpdateAllPos[m_nCurObjectSearchIndex]=TRUE;
		}
		else // 最后一行未全部刷新结果
		{
			for (int it=0;it<2;it++)
			{
				int nCols = 2*m_nCurObjectSearchIndex+1+it;
				tmpStr.Format(_T("%.3f"),vData[it]);
				m_ListImagePos.SetItemText(nRow-1,nCols,tmpStr);
			}
			m_vbIsUpdateAllPos[m_nCurObjectSearchIndex]=TRUE;
		}
	}

	UpdateStatisticsList(&m_ListImagePos,&m_ListImagePosStatistics);
}

void CDlgCalibCheckByFixedTargetAligner::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,0));
	}

	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

	vcXMLConfigurator xml;
	CString strXMLPath;

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
		break;
	}
	xml.Load(strXMLPath);
	xml.FindElem(_T("ResourceBundle")); 
	xml.EnterElem();

	while(xml.FindElem(_T("")))
	{
		CString strDlgID = xml.GetAttribute(_T("Key"));

		if(_T("IDD_DLG_CALIB_CHECK_BY_FIXED_TARGET") == strDlgID)
		{
			int nID;
			CString strValue = xml.GetAttribute(_T("Value"));
			SetWindowText(strValue);

			xml.EnterElem();	   
			while(xml.FindElem())
			{
				xml.GetAttribute(_T("Id"),  nID);
				strValue = xml.GetAttribute(_T("Value"));

				if(GetDlgItem(nID) != NULL)
				{
					GetDlgItem(nID)->SetWindowText(strValue);
				}	  
			}
			xml.LeaveElem();
		}  
	}
	xml.LeaveElem();	
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEdit_Keyboard(int nID)
{
	if (  ((CEdit*)GetDlgItem(nID))==NULL)
	{
		return;
	}
	if (m_bIsReadOnly) // 文本框为不可输入状态时
	{
		return;
	}
	GetDlgItemText(nID, m_strInfoOld);
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(nID)->GetWindowRect(&rcWnd);
		GetDlgItemText(nID, strInput);
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(nID, strOutput);
		}
	}

}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEdit(int nID)
{
	CString strVal;
	GetDlgItemText(nID, strVal);
	if (!IsStringNumerical(strVal))
	{
		SetDlgItemText(nID, m_strInfoOld);
	}
	else
	{
		SetDlgItemText(nID, strVal);
	}
}

BOOL CDlgCalibCheckByFixedTargetAligner::IsStringNumerical(LPCTSTR lpszVal)
{
	CString strVal = lpszVal;
	if (strVal.IsEmpty())
	{
		return FALSE;
	}
	int nLen = strVal.GetLength();
	TCHAR ch = 0;
	for (int i=0; i<nLen; i++)
	{
		ch = strVal.GetAt(i);
		if ((ch > '9') || ((ch < '0') && (ch != '.')))
		{
			if ((i == 0) && ((ch == '-') || (ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisX1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_X1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisY1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_Y1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisD1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_D1);
}


void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisX2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_X2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisY2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_Y2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisD2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_D2);
}



void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisX3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_X3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisY3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_Y3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisD3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_D3);
}


void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisX4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_X4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisY4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_Y4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosTargetAxisD4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_TARGET_AXIS_D4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisX1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_X1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisY1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisD1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_D1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisX2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_X2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisY2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisD2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_D2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisX3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_X3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisY3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisD3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_D3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisX4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_X4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisY4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosObjectAxisD4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_OBJECT_AXIS_D4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisX1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisY1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisD1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisX2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisY2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisD2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisX3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisY3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisD3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisX4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisY4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnSetfocusEditValPosCandidateObjectAxisD4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D4);
}

void CDlgCalibCheckByFixedTargetAligner::OnCbnSelchangeComboExproductCheckBySize()
{
	// 切换扩展平台序号后，需要重新初始化轴，清空图像列表；之前搜索成功的对象目标位置标识
	UpdateData(TRUE);

	InitAxisData();
	OnListClearAllItem2();

	for (int it =0;it<m_vbSearchObjectSuss.size();it++)
	{
		m_vbSearchObjectSuss[it] = FALSE;
	}

	for (int it =0;it<m_vbSearchTargetSuss.size();it++)
	{
		m_vbSearchTargetSuss[it] = FALSE;
	}

}


void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisX1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_X1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisY1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_Y1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisD1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_D1);
}


void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisX2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_X2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisY2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_Y2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisD2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_D2);
}



void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisX3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_X3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisY3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_Y3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisD3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_D3);
}


void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisX4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_X4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisY4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_Y4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosTargetAxisD4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_TARGET_AXIS_D4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisX1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_X1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisY1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisD1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_D1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisX2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_X2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisY2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisD2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_D2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisX3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_X3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisY3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisD3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_D3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisX4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_X4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisY4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_Y4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosObjectAxisD4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_OBJECT_AXIS_D4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisX1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisY1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisD1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D1);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisX2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisY2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisD2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D2);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisX3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisY3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisD3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D3);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisX4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_X4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisY4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_Y4);
}

void CDlgCalibCheckByFixedTargetAligner::OnEnKillfocusEditValPosCandidateObjectAxisD4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_CANDIDATE_OBJECT_AXIS_D4);
}