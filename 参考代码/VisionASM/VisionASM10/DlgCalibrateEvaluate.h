#pragma once
#include "afxcmn.h"
#include "vsBaseVisionAlign.h"

// CDlgCalibrateEvaluate 对话框

class CDlgCalibrateEvaluate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalibrateEvaluate)

public:
	CDlgCalibrateEvaluate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibrateEvaluate();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_EVALUATE };
	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	vcBaseVisionAlign *m_pVisionASM;
	
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCalibrateEvaluate;
	CImageList    m_ImageList;
	int m_nCurProductIdx;
	virtual BOOL OnInitDialog();
	void ShowCalibrateResult(vcBaseVisionAlign *pVisionASM,CListCtrl * m_pList,int nPosIndex);
	void FixCalibrateResultImageAnaysisData(CCoordPos cpObjectMarkImgCoordPos[CALIBMOVEPOSNUM],CListCtrl * m_pList,int nPosIndex);
};
