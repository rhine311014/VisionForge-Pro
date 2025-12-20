// CDlgCalibTargetDmcodeParam.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibTargetDmcodeParam.h"
#include "afxdialogex.h"
#include "LogFileSave.h"
#include "DlgDmCodeShutterSetting.h"
// CDlgCalibTargetDmcodeParam 对话框

IMPLEMENT_DYNAMIC(CDlgCalibTargetDmcodeParam, CDialog)


CDlgCalibTargetDmcodeParam::CDlgCalibTargetDmcodeParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibTargetDmcodeParam::IDD, pParent)
	, m_strCalibDMCodeMaxNum(_T("1"))
	, m_strCalibDMCodeMaxTime(_T("1000"))
	, m_strCalibDMCodeGridSize(_T("1"))
	, m_strCalibDMCodeGridThre(_T("16"))
    , m_nPosIndex(0)
	, m_nDecodeMethod(0)
	, m_nDmCodeMirror(2)
	, m_strSampleLevel(_T("1"))
	, m_strMinBarSize(_T("20"))
	, m_strMaxBarSize(_T("500"))
{
    m_nPosNum = 0;
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = TRUE;
	m_bKillFocusEnabled = TRUE;
    m_nPrePosIndex = m_nPosIndex;
	m_pVisionASM = NULL;
	m_LogLevel = 1;
	m_strKeyText = _T("");
	m_StatusBar = NULL;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;
	m_bStatusBarFlag = TRUE;
	m_pVAPrevMainWnd = NULL;

	m_pInteractiveContainer = NULL;
	m_pStaticContainer = NULL;

	m_DmCodeCalibTool.SetGuiGraphics(&m_GuiCalibrateGraphics);
	CreateGUID(m_strKeyText);

	m_vcsResource.resize(MAXDMCODENUM);
	m_vImageInput.resize(MAXDMCODENUM);
	for (int i=0;i<MAXDMCODENUM;i++)
	{
		m_dEnvironmentShutter[i] = 0.0;
		InitializeCriticalSection(&m_vcsResource.at(i));	
	}

	m_bIsBoardCalib = false;
	
}

CDlgCalibTargetDmcodeParam::~CDlgCalibTargetDmcodeParam()
{
	for (int i=0; i<m_vcsResource.size(); i++)
	{
		DeleteCriticalSection(&m_vcsResource.at(i));
	}
	if (m_StatusBar != NULL)
	{
		delete m_StatusBar;
		m_StatusBar = NULL;
	}
	if (m_hIconSucceed != NULL)
	{
		DestroyIcon(m_hIconSucceed);
		m_hIconSucceed = NULL;
	}
	if (m_hIconFailed != NULL)
	{
		DestroyIcon(m_hIconFailed);
		m_hIconFailed = NULL;
	}

	m_pInteractiveContainer = m_pGuiDisplay.GetInteractiveContainer();
	m_pStaticContainer = m_pGuiDisplay.GetStaticContainer();
	m_pInteractiveContainer->Clear();
	m_pStaticContainer->Clear();
}

void CDlgCalibTargetDmcodeParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VAL_DMCODEMAXNUM, m_strCalibDMCodeMaxNum);
	DDX_Text(pDX, IDC_EDIT_VAL_DMCODEMAXTIME, m_strCalibDMCodeMaxTime);
	DDX_Text(pDX, IDC_EDIT_VAL_DMCODEGRIDSIZE, m_strCalibDMCodeGridSize);
	DDX_Text(pDX, IDC_EDIT_VAL_DMCODEGRIDTHRE, m_strCalibDMCodeGridThre);
	DDX_Check(pDX, IDC_CHECK_DMCODEOUTPOINT, m_bDmcodeOutputPoint);
	DDX_Check(pDX, IDC_CHECK_DMCODEREDECODE, m_bDmcodeRecode);
	DDX_Radio(pDX, IDC_RDO_POS1, m_nPosIndex);
	DDX_Control(pDX, IDC_COMBO_IMAGE_TYPE, m_comboDisplayImage);
	DDX_Radio(pDX, IDC_RDO_DMCODE_METHOD_DEFAULT, m_nDecodeMethod);
	DDX_Radio(pDX, IDC_RDO_MIRROR_MODE_CLOSE, m_nDmCodeMirror);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_LEVEL, m_strSampleLevel);
	DDX_Text(pDX, IDC_EDIT_MIN_BAR_SIZE, m_strMinBarSize);
	DDX_Text(pDX, IDC_EDIT_MAX_BAR_SIZE, m_strMaxBarSize);
	DDX_Control(pDX, IDC_COMBO_COPY_SRC, m_cmbCopySrc);
	DDX_Control(pDX, IDC_CHECK_COPY_DST_POS1, m_chkDstCopyPos1);
	DDX_Control(pDX, IDC_CHECK_COPY_DST_POS2, m_chkDstCopyPos2);
	DDX_Control(pDX, IDC_CHECK_COPY_DST_POS3, m_chkDstCopyPos3);
	DDX_Control(pDX, IDC_CHECK_COPY_DST_POS4, m_chkDstCopyPos4);
	DDX_Control(pDX, IDC_CHECK_COPY_DST_POS5, m_chkDstCopyPos5);
	DDX_Control(pDX, IDC_CHECK_COPY_DST_POS6, m_chkDstCopyPos6);
	DDX_Control(pDX, IDC_CHECK_COPY_DST_POS7, m_chkDstCopyPos7);
	DDX_Control(pDX, IDC_CHECK_COPY_DST_POS8, m_chkDstCopyPos8);
}


BEGIN_MESSAGE_MAP(CDlgCalibTargetDmcodeParam, CDialog)
	ON_WM_DESTROY()
	
	ON_EN_SETFOCUS(IDC_EDIT_VAL_DMCODEMAXNUM, &CDlgCalibTargetDmcodeParam::OnEnSetfocusEditValDMCodeMaxNum)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_DMCODEMAXNUM, &CDlgCalibTargetDmcodeParam::OnEnKillfocusEditValDMCodeMaxNum)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_DMCODEMAXTIME, &CDlgCalibTargetDmcodeParam::OnEnSetfocusEditValDMCodeMaxTime)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_DMCODEMAXTIME, &CDlgCalibTargetDmcodeParam::OnEnKillfocusEditValDMCodeMaxTime)
	
	ON_EN_SETFOCUS(IDC_EDIT_VAL_DMCODEGRIDSIZE, &CDlgCalibTargetDmcodeParam::OnEnSetfocusEditValDMCodeGridSize)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_DMCODEGRIDSIZE, &CDlgCalibTargetDmcodeParam::OnEnKillfocusEditValCodeGridSize)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_DMCODEGRIDTHRE, &CDlgCalibTargetDmcodeParam::OnEnSetfocusEditValDMCodeGridThre)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_DMCODEGRIDTHRE, &CDlgCalibTargetDmcodeParam::OnEnKillfocusEditValCodeGridThre)
	
	ON_BN_CLICKED(IDOK, &CDlgCalibTargetDmcodeParam::OnBnClickedOk)
    ON_BN_CLICKED(IDC_RDO_POS1, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos1)
    ON_BN_CLICKED(IDC_CHECK_DMCODEREDECODE, &CDlgCalibTargetDmcodeParam::OnBnClickedCheckDmcoderedecode)
    ON_BN_CLICKED(IDC_CHECK_DMCODEOUTPOINT, &CDlgCalibTargetDmcodeParam::OnBnClickedCheckDmcodeoutpoint)
    ON_BN_CLICKED(IDC_RDO_POS2, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos2)
    ON_BN_CLICKED(IDC_RDO_POS3, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos3)
    ON_BN_CLICKED(IDC_RDO_POS4, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos4)
    ON_BN_CLICKED(IDC_RDO_POS5, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos5)
    ON_BN_CLICKED(IDC_RDO_POS6, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos6)
    ON_BN_CLICKED(IDC_RDO_POS7, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos7)
    ON_BN_CLICKED(IDC_RDO_POS8, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos8)
	ON_BN_CLICKED(IDC_BTN_GRAB_IMAGE, &CDlgCalibTargetDmcodeParam::OnBnClickedBtnGrabImage)
	ON_BN_CLICKED(IDC_BTN_LOAD_IMAGE, &CDlgCalibTargetDmcodeParam::OnBnClickedBtnLoadImage)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE_TYPE, &CDlgCalibTargetDmcodeParam::OnCbnSelchangeComboImageType)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_TEST, &CDlgCalibTargetDmcodeParam::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_DMCODE_FULL_IMAGE, &CDlgCalibTargetDmcodeParam::OnBnClickedBtnDmcodeFullImage)
	ON_BN_CLICKED(IDC_BTN_CORNERS_FULL_IMAGE, &CDlgCalibTargetDmcodeParam::OnBnClickedBtnCornersFullImage)
	ON_BN_CLICKED(IDC_BTN_SHUTTER_SETTING, &CDlgCalibTargetDmcodeParam::OnBnClickedBtnShutterSetting)
	ON_BN_CLICKED(IDC_RDO_DMCODE_METHOD_DEFAULT, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoDmcodeMethodDefault)
	ON_BN_CLICKED(IDC_RDO_DECODE_METHOD_VSB, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoDecodeMethodVsb)
	ON_BN_CLICKED(IDC_RDO_MIRROR_MODE_CLOSE, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoMirrorModeClose)
	ON_BN_CLICKED(IDC_RDO_MIRROR_MODE_OPEN, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoMirrorModeOpen)
	ON_BN_CLICKED(IDC_RDO_MIRROR_MODE_ANY, &CDlgCalibTargetDmcodeParam::OnBnClickedRdoMirrorModeAny)
	ON_EN_SETFOCUS(IDC_EDIT_SAMPLE_LEVEL, &CDlgCalibTargetDmcodeParam::OnEnSetfocusEditSampleLevel)
	ON_EN_KILLFOCUS(IDC_EDIT_SAMPLE_LEVEL, &CDlgCalibTargetDmcodeParam::OnEnKillfocusEditSampleLevel)
	ON_EN_SETFOCUS(IDC_EDIT_MIN_BAR_SIZE, &CDlgCalibTargetDmcodeParam::OnEnSetfocusEditMinBarSize)
	ON_EN_KILLFOCUS(IDC_EDIT_MIN_BAR_SIZE, &CDlgCalibTargetDmcodeParam::OnEnKillfocusEditMinBarSize)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_BAR_SIZE, &CDlgCalibTargetDmcodeParam::OnEnSetfocusEditMaxBarSize)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX_BAR_SIZE, &CDlgCalibTargetDmcodeParam::OnEnKillfocusEditMaxBarSize)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CDlgCalibTargetDmcodeParam::OnBnClickedButtonCopy)
END_MESSAGE_MAP()


// CDlgCalibTargetDmcodeParam 消息处理程序


BOOL CDlgCalibTargetDmcodeParam::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	UpdateDialogLanguage();
	InitDlgItem();
	UpdateDlgData(false);
	OnBnClickedBtnGrabImage();
	return TRUE;  
}
void CDlgCalibTargetDmcodeParam::UpdateStatusBar()
{
	m_StatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_StatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_StatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgCalibTargetDmcodeParam::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

}

void CDlgCalibTargetDmcodeParam::OnEnSetfocusEditValDMCodeMaxNum()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_DMCODEMAXNUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_DMCODEMAXNUM, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEMAXNUM, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[识别个数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,m_LogLevel);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValDMCodeMaxNum();
	}	
}


void CDlgCalibTargetDmcodeParam::OnEnKillfocusEditValDMCodeMaxNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_DMCODEMAXNUM, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEMAXNUM, m_strCalibDMCodeMaxNum);
		}
		else
		{
			m_strCalibDMCodeMaxNum.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_VAL_DMCODEMAXNUM, m_strCalibDMCodeMaxNum);
		}
        
	}	
}

void CDlgCalibTargetDmcodeParam::OnEnSetfocusEditValDMCodeMaxTime()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_DMCODEMAXTIME)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_DMCODEMAXTIME, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEMAXTIME, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[解码时间]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,m_LogLevel);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValDMCodeMaxTime();
	}	
}


void CDlgCalibTargetDmcodeParam::OnEnKillfocusEditValDMCodeMaxTime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_DMCODEMAXTIME, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEMAXTIME, m_strCalibDMCodeMaxTime);
		}
		else
		{
			m_strCalibDMCodeMaxTime.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_VAL_DMCODEMAXTIME, m_strCalibDMCodeMaxTime);
		}
        
	}	
}


void CDlgCalibTargetDmcodeParam::OnEnSetfocusEditValDMCodeGridSize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_DMCODEGRIDSIZE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDSIZE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDSIZE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[单元尺寸]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,m_LogLevel);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCodeGridSize();
	}	
}


void CDlgCalibTargetDmcodeParam::OnEnKillfocusEditValCodeGridSize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDSIZE, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDSIZE, m_strCalibDMCodeGridSize);
		}
		else
		{
			m_strCalibDMCodeGridSize.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDSIZE, m_strCalibDMCodeGridSize);
		}
        
	}
}


void CDlgCalibTargetDmcodeParam::OnEnSetfocusEditValDMCodeGridThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_DMCODEGRIDTHRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDTHRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDTHRE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[强度阈值]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,m_LogLevel);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCodeGridThre();
	}	
}


void CDlgCalibTargetDmcodeParam::OnEnKillfocusEditValCodeGridThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDTHRE, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDTHRE, m_strCalibDMCodeGridThre);
		}
		else
		{
			m_strCalibDMCodeGridThre.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_DMCODEGRIDTHRE, m_strCalibDMCodeGridThre);
		}
        
	}
}


BOOL CDlgCalibTargetDmcodeParam::IsStringNumerical( LPCTSTR lpszVal )
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

BOOL CDlgCalibTargetDmcodeParam::IsStringPositiveNumerical(LPCTSTR lpszVal)
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
			if ((i == 0) && (ch == '+'))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgCalibTargetDmcodeParam::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateDlgData(TRUE);

	CTotalProductData *pTotalProductData;
	pTotalProductData = m_pVisionASM->GetTotalProuductData();
	int nCurProductIdx = pTotalProductData->GetCurProductIndex();
	// 获取当前产品对应的标定数据
	CProductData* pProductData;
	pProductData = pTotalProductData->GetItemPtr(nCurProductIdx);

	for (int i=0;i<m_pVisionASM->GetPosNum();i++)
	{
		m_vDmCodeParam[i]=m_vtmpDmCodeParam[i];
		pProductData->m_vDMcodeCalib.at(i) = m_vDmCodeParam[i];
		
	}
	m_pVisionASM->SaveProductDataInfo(nCurProductIdx);
	

	CString strLog;
	strLog.Format(_T("[保存退出]二维码关联参数设置\n"));
	g_opLogFile.SaveLogText(strLog,m_LogLevel-1);

	SetVisionASMRef(NULL);
	CDialog::OnOK();
}

void CDlgCalibTargetDmcodeParam::OnPosChange()
{

	UpdateDlgData(TRUE);
    if (m_nPosIndex == m_nPrePosIndex)
    {
        return;
    }
	

	CString strLog;
	strLog.Format(_T("修改[位置%d]为[位置%d]\n"),m_nPrePosIndex,m_nPosIndex);
	g_opLogFile.SaveLogText(strLog,m_LogLevel);

	m_pVisionASM->FreezePosCamera(m_nPrePosIndex);
	m_pVisionASM->SetPosDisplay(m_nPrePosIndex, NULL);
	m_pGuiDisplay.RemoveImage();  
	m_comboDisplayImage.SetCurSel(0);
	UpdateGuiDisplay();
	UpdateDlgData(FALSE);
	OnBnClickedBtnGrabImage();
	
    m_nPrePosIndex = m_nPosIndex;
}
void CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos1()
{
    // TODO: 在此添加控件通知处理程序代码
    OnPosChange();

}



void CDlgCalibTargetDmcodeParam::OnDmcodemirrorChange()
{
    // TODO: 在此添加控件通知处理程序代码
    //UpdateData();
	int nMirrorOld = m_nDmCodeMirror;
	UpdateData(TRUE);
	if (nMirrorOld != m_nDmCodeMirror)
	{
		CString strLog;
		CString strInfo[3]={"关","开","任意"};
		CString strValue =strInfo[m_nDmCodeMirror];
		strLog.Format(_T("修改[图像镜像]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,m_LogLevel);
	}
}

void CDlgCalibTargetDmcodeParam::OnDeCodeMethodChange()
{
	int nMethodOld = m_nDecodeMethod;
	UpdateData(TRUE);
	if (nMethodOld != m_nDecodeMethod)
	{
		CString strLog;
		CString strInfo[2]={"方法一","方法二"};
		CString strValue =strInfo[m_nDecodeMethod];
		strLog.Format(_T("修改[解码方法]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,m_LogLevel);
	}
	/*if (m_nDecodeMethod == 0)
	{
		GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(TRUE);
	}*/
}

void CDlgCalibTargetDmcodeParam::OnBnClickedCheckDmcoderedecode()
{
    // TODO: 在此添加控件通知处理程序代码
    //UpdateData();
	BOOL bCheckOld = m_bDmcodeRecode;

	UpdateData(TRUE);
	if (bCheckOld != m_bDmcodeRecode)
	{
		CString strLog;
		CString strValue = m_bDmcodeRecode ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[重新检测]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,m_LogLevel);
	}
}


void CDlgCalibTargetDmcodeParam::OnBnClickedCheckDmcodeoutpoint()
{
    // TODO: 在此添加控件通知处理程序代码
    //UpdateData();
	BOOL bCheckOld = m_bDmcodeOutputPoint;

	UpdateData(TRUE);
	if (bCheckOld != m_bDmcodeOutputPoint)
	{
		CString strLog;
		CString strValue = m_bDmcodeOutputPoint ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[显示外围点]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,m_LogLevel);
	}
}


void CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos2()
{
    // TODO: 在此添加控件通知处理程序代码
    OnPosChange();
}
void CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos3()
{
    // TODO: 在此添加控件通知处理程序代码
    OnPosChange();
}
void CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos4()
{
    // TODO: 在此添加控件通知处理程序代码
    OnPosChange();
}
void CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos5()
{
    // TODO: 在此添加控件通知处理程序代码
    OnPosChange();
}
void CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos6()
{
    // TODO: 在此添加控件通知处理程序代码
    OnPosChange();
}
void CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos7()
{
    // TODO: 在此添加控件通知处理程序代码
    OnPosChange();
}
void CDlgCalibTargetDmcodeParam::OnBnClickedRdoPos8()
{
    // TODO: 在此添加控件通知处理程序代码
    OnPosChange();
}
void CDlgCalibTargetDmcodeParam::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir;
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
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

		if(_T("IDD_DLG_CALIB_TARGET_DMCODE_PARAM") == strDlgID)
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

void CDlgCalibTargetDmcodeParam::SetLogLevel(int level)
{
	m_LogLevel = level;
}

void CDlgCalibTargetDmcodeParam::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strLog;
	strLog.Format(_T("[不保存退出]二维码关联参数设置\n"));
	g_opLogFile.SaveLogText(strLog,m_LogLevel-1);

	if (m_pVisionASM != NULL)
	{
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		BOOL bEnableShutter = optionInfo.m_bEnableSearchShutter;
		if (bEnableShutter)
		{
			// 获取环境设置中的曝光
			for (int i=0;i<m_pVisionASM->GetPosNum();i++)
			{
				if (m_vDmCodeParam[i].m_dShutter < 0.001)
				{
					m_pVisionASM->SwitchToSearchShutter(i, m_dEnvironmentShutter[i]);
				}
				else
				{
					m_pVisionASM->SwitchToSearchShutter(i, m_vDmCodeParam[i].m_dShutter);
				}
				
			}

		}
	}
	SetVisionASMRef(NULL);
	CDialog::OnCancel();
}


BOOL CDlgCalibTargetDmcodeParam::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_pVisionASM != NULL)
	{
		m_pVisionASM->FreezeAllCameras();
		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplay.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);

	}
	return CDialog::DestroyWindow();
}


LRESULT CDlgCalibTargetDmcodeParam::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			if (m_nPosIndex == (int)wParam)
			{
				// 设置图像
				m_pVisionASM->UpdateDisplayImage((int)wParam);
				cpImage* pImageTemp = (cpImage*)m_pGuiDisplay.GetImageCP();
				if (pImageTemp != NULL)
				{
					pImageTemp->Clone(m_vImageInput[m_nPosIndex]);	
				}
				UpdateGuiDisplay();
			}
		}
		break;
	case WM_UPDATE_CALIBRATE_TARGET_GRAPHIC:
		{
			//m_pVisionASM->UpdateAlnObjectGraphics((int)wParam, (BOOL)lParam);
			m_pGuiDisplay.Invalidate();
		}
		break;
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgCalibTargetDmcodeParam::UpdateBtnDisplay()
{
	if (m_pVisionASM != NULL)
	{
		// 如果没有相机，禁用“采集图像”按钮
		GetDlgItem(IDC_BTN_GRAB_IMAGE)->EnableWindow(m_pVisionASM->IsPosCameraOnline(m_nPosIndex));

		if (m_pVisionASM->IsPosCameraGrabbing(m_nPosIndex))
		{

			SetDlgItemText(IDC_BTN_GRAB_IMAGE,GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
			GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
			CPlatformOptionInfo optionInfo;
			m_pVisionASM->GetPlatformOptionInfo(optionInfo);
			GetDlgItem(IDC_BTN_SHUTTER_SETTING)->EnableWindow(TRUE && optionInfo.m_bEnableSearchShutter);
			m_comboDisplayImage.EnableWindow(FALSE);
			

		}
		else
		{
			SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
			GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SHUTTER_SETTING)->EnableWindow(FALSE);
			m_comboDisplayImage.EnableWindow(TRUE);
		}
	}
	else
	{
		SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
		GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SHUTTER_SETTING)->EnableWindow(FALSE);
		m_comboDisplayImage.EnableWindow(TRUE);
	}
}
void CDlgCalibTargetDmcodeParam::OnBnClickedBtnGrabImage()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pVisionASM != NULL)
	{
		if (m_pVisionASM->IsPosCameraOnline(m_nPosIndex))
		{
			m_pVisionASM->SetPosDisplay(m_nPosIndex, &m_pGuiDisplay);
			if (m_pVisionASM->IsPosCameraGrabbing(m_nPosIndex))
			{
				if (m_pVisionASM->FreezePosCamera(m_nPosIndex))
				{
					m_comboDisplayImage.SetCurSel(1);
					UpdateGuiDisplay();

					m_bStatusBarFlag = TRUE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRAB_STOPPED);	//_T("采集已停止")
				}
				else
				{
					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB);	//_T("停止采集失败");
				}
			}
			else
			{
				if (m_pVisionASM->GrabPosCamera(m_nPosIndex))
				{
					
					m_comboDisplayImage.SetCurSel(0);
					UpdateGuiDisplay();

					m_bStatusBarFlag = TRUE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRABBING);	//_T("正在连续采集")
					m_strStatusBarInfo2 = _T("");
				}
				else
				{
					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRAB_FAILED);	//_T("采集失败")
				}
				
			}		
		}
		else
		{
			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_CONNECTION_ERROR);	//_T("相机连接错误")
		}
	}
	else
	{
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_VISION_ALIGN_TOOL);
	}
	UpdateBtnDisplay();
	UpdateStatusBar();
}


void CDlgCalibTargetDmcodeParam::OnBnClickedBtnLoadImage()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString strFilter = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FILTER_BMP_FILE) + _T("|*.bmp||");
	CString strFilter = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FILTER_BMP_AND_JPEG_FILE);
	CFileDialog cFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (IDOK == cFileDlg.DoModal())
	{
		//scDIB cDib;
		CString strPath = cFileDlg.GetPathName();
		if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".bmp"))
		{
			scDIB cDib;
			cDib.Init(strPath);
			cDib.GetImage(m_vImageInput.at(m_nPosIndex));
		}
		else if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".jpg"))
		{
			scJpeg cJpeg;
			cJpeg.Read(strPath,m_vImageInput.at(m_nPosIndex));
		}
		else if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".png"))
		{
			scPng cPng;
			cPng.Read(strPath,m_vImageInput.at(m_nPosIndex));
		}
		//cDib.Init(strPath);
		//cDib.GetImage(m_vImageInput.at(m_nPosIndex));
		m_comboDisplayImage.SetCurSel(1);
		UpdateGuiDisplay();

		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LOAD_IMAGE));	
		m_strStatusBarInfo2 = strPath;
		UpdateStatusBar();
	}
}


void CDlgCalibTargetDmcodeParam::OnCbnSelchangeComboImageType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateGuiDisplay();
}
void CDlgCalibTargetDmcodeParam::SetVisionASMRef(vcBaseVisionAlign *pVisionASM)
{
	if (m_pVisionASM != NULL)
	{
		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplay.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);

	}

	m_pVisionASM = pVisionASM;
	if (m_pVisionASM != NULL)
	{
		// 保存旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		m_vpPrevGuiDisplay.resize(nNum);
		for (int i=0; i<nNum; i++)
		{
			m_vpPrevGuiDisplay.at(i) = m_pVisionASM->GetPosDisplay(i);
			m_pVisionASM->SetPosDisplay(i, NULL);
		}
		m_pVAPrevMainWnd = m_pVisionASM->SetMainWnd(this);
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		BOOL bEnableShutter = optionInfo.m_bEnableSearchShutter;
		if (bEnableShutter)
		{
			// 获取环境设置中的曝光
			CameraParam para;
			for (int i=0;i<nNum;i++)
			{
				m_pVisionASM->GetPosCameraParam(i, para);
				m_dEnvironmentShutter[i] = para.m_dShutter;
			}
			
		}
	}
}

void CDlgCalibTargetDmcodeParam::InitDlgItem()
{
	ASSERT(m_pVisionASM != NULL);
	m_nPosNum = m_pVisionASM->GetPosNum();
	for (int i=0;i<MAXDMCODENUM;i++)
	{
		if (i<m_nPosNum)
		{
			GetDlgItem(IDC_RDO_POS1+i)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_RDO_POS1+i)->ShowWindow(SW_HIDE);
		}
	}
	SysPlatformInfo platInfo;
	m_pVisionASM->GetSysPlatformInfo(platInfo);
	//目标对象不在同一个控制器、目标采用靶标关联且本机为目标端时，禁用对象端的位置选择
	if (m_bIsBoardCalib && platInfo.m_bTargetInDifferentComputer)
	{
		for (int i=0;i<m_nPosNum/2;i++)
		{
			GetDlgItem(IDC_RDO_POS1+i)->EnableWindow(FALSE);
		}
		m_nPosIndex = m_nPosNum/2;
		m_nPrePosIndex = m_nPosIndex;
	}
	//目标对象不在同一个控制器、目标采用靶标关联且本机为对象端时，禁用目标端的位置选择
	else if (m_bIsBoardCalib && platInfo.m_bObjectInDifferentComputer)
	{
		for (int i=0;i<m_nPosNum/2;i++)
		{
			GetDlgItem(IDC_RDO_POS1+i+m_nPosNum/2)->EnableWindow(FALSE);
		}
	}

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	m_pGuiDisplay.Create(IDC_STC_DISPLAY, this);
	m_pGuiDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_pGuiDisplay.SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_pGuiDisplay.SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_pGuiDisplay.SetLanguage(1);
		break;
	}
	m_pGuiDisplay.RegisterWndMsg(this, SV_DISPLAY_MOUSEMOVE|SV_DISPLAY_DBLCLICK);

	m_GuiDmCodeSearchRect.SetCenterWidthHeight(376,240,702,430);
	m_GuiDmCodeSearchRect.SetLabelVisible(TRUE);
	m_GuiCornersSearchRect.SetCenterWidthHeight(376,240,752,480);
	m_GuiCornersSearchRect.SetLabelVisible(TRUE);
	switch (lgidLanguage)
	{
	case 0:
		m_GuiDmCodeSearchRect.SetLabel(_T("DmCode Search Area"));
		m_GuiCornersSearchRect.SetLabel(_T("Corners Search Area"));
		break;
	case 1:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	default:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	}
	AddGuiTrainGraphicsToDisplay(&m_pGuiDisplay);
	AddGuiResultGraphicsToDisplay(&m_pGuiDisplay);

	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	int nPartWidth[3] = {20, 200,  -1};
	m_StatusBar->SetParts(3, nPartWidth);
	m_StatusBar->ShowWindow(SW_SHOW);
	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	m_comboDisplayImage.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ORIGINAL_IMAGE));
	m_comboDisplayImage.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_INPUT_IMAGE));
	m_comboDisplayImage.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RESULT_IMAGE));
	m_comboDisplayImage.SetCurSel(0);

	CTotalProductData *pTotalProductData;
	pTotalProductData = m_pVisionASM->GetTotalProuductData();
	int nCurProductIdx = pTotalProductData->GetCurProductIndex();
	// 获取当前产品对应的标定数据
	CProductData* pProductData;
	pProductData = pTotalProductData->GetItemPtr(nCurProductIdx);

	scDMCodeParam DmCodeParam;

	for (int i=0;i<m_pVisionASM->GetPosNum();i++)
	{
		DmCodeParam = pProductData->m_vDMcodeCalib.at(i);
		m_vDmCodeParam[i]= DmCodeParam;
		m_vtmpDmCodeParam[i] = DmCodeParam;
	}

	m_vpCheckPos.push_back(&m_chkDstCopyPos1);
	m_vpCheckPos.push_back(&m_chkDstCopyPos2);
	m_vpCheckPos.push_back(&m_chkDstCopyPos3);
	m_vpCheckPos.push_back(&m_chkDstCopyPos4);
	m_vpCheckPos.push_back(&m_chkDstCopyPos5);
	m_vpCheckPos.push_back(&m_chkDstCopyPos6);
	m_vpCheckPos.push_back(&m_chkDstCopyPos7);
	m_vpCheckPos.push_back(&m_chkDstCopyPos8);
	CString strInfo;
	for (int i=0;i<MAXPOSNUM;i++)
	{
		if (i<m_pVisionASM->GetPosNum())
		{
			strInfo.Format(_T("位置%d"),i+1);
			m_cmbCopySrc.InsertString(i,strInfo);	
			m_vpCheckPos.at(i)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_vpCheckPos.at(i)->ShowWindow(SW_HIDE);
		}
	}
	m_cmbCopySrc.SetCurSel(0);
	if (m_pVisionASM != NULL)
	{
		// 如果没有相机，禁用“采集图像”按钮
		GetDlgItem(IDC_BTN_GRAB_IMAGE)->EnableWindow(m_pVisionASM->IsPosCameraOnline(m_nPosIndex));
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		if (m_pVisionASM->IsPosCameraGrabbing(m_nPosIndex))
		{
			if (m_pVisionASM->FreezePosCamera(m_nPosIndex))
			{
				SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
				GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_SHUTTER_SETTING)->EnableWindow(FALSE);
			}
			else
			{
				SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
				GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_SHUTTER_SETTING)->EnableWindow(TRUE && optionInfo.m_bEnableSearchShutter);
			}
		}
		else
		{
			SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
			GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SHUTTER_SETTING)->EnableWindow(FALSE);
		}
	}
}

void CDlgCalibTargetDmcodeParam::CopyParameter(int nSrcPosIndex,int nDstPosIndex)
{
	m_vtmpDmCodeParam[nDstPosIndex].m_FindCornerPara = m_vtmpDmCodeParam[nSrcPosIndex].m_FindCornerPara;

	m_vtmpDmCodeParam[nDstPosIndex].m_FindDMCodePara = m_vtmpDmCodeParam[nSrcPosIndex].m_FindDMCodePara;

	m_vtmpDmCodeParam[nDstPosIndex].m_bEnableReDecode = m_vtmpDmCodeParam[nSrcPosIndex].m_bEnableReDecode;		
	m_vtmpDmCodeParam[nDstPosIndex].m_DmCodeSearchRect = m_vtmpDmCodeParam[nSrcPosIndex].m_DmCodeSearchRect;
	m_vtmpDmCodeParam[nDstPosIndex].m_CornersSearchRect = m_vtmpDmCodeParam[nSrcPosIndex].m_CornersSearchRect;
	m_vtmpDmCodeParam[nDstPosIndex].m_dImageHeight = m_vtmpDmCodeParam[nDstPosIndex].m_dImageHeight;
	m_vtmpDmCodeParam[nDstPosIndex].m_dImageWidth = m_vtmpDmCodeParam[nDstPosIndex].m_dImageWidth;
}

void CDlgCalibTargetDmcodeParam::UpdateDlgData(bool bSave)
{
	if (bSave)
	{
		UpdateData(TRUE);
		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindCornerPara.m_dGridSize = atof(m_strCalibDMCodeGridSize);
		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindCornerPara.m_dGridThre = atof(m_strCalibDMCodeGridThre);
		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindCornerPara.m_bOutputOutPoints=m_bDmcodeOutputPoint;

		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindDMCodePara.m_nMaxCodeNum = atoi(m_strCalibDMCodeMaxNum);
		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindDMCodePara.m_nWaitingTime = atoi(m_strCalibDMCodeMaxTime);		
		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindDMCodePara.m_modeMirror=(MirrorMode)m_nDmCodeMirror;
		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindDMCodePara.m_methodDecode=(DecodeMethod)m_nDecodeMethod;
		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindDMCodePara.m_nMinBarSize = atoi(m_strMinBarSize);
		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindDMCodePara.m_nMaxBarSize = atoi(m_strMaxBarSize);
		m_vtmpDmCodeParam[m_nPrePosIndex].m_FindDMCodePara.m_nSampleLevel = atoi(m_strSampleLevel);

		m_vtmpDmCodeParam[m_nPrePosIndex].m_bEnableReDecode=m_bDmcodeRecode;		
		m_vtmpDmCodeParam[m_nPrePosIndex].m_DmCodeSearchRect = m_GuiDmCodeSearchRect.GetRect();
		m_vtmpDmCodeParam[m_nPrePosIndex].m_CornersSearchRect = m_GuiCornersSearchRect.GetRect();
		
	}
	else
	{
		m_strCalibDMCodeGridThre.Format("%0.3f",m_vtmpDmCodeParam[m_nPosIndex].m_FindCornerPara.m_dGridThre);
		m_strCalibDMCodeGridSize.Format("%0.3f",m_vtmpDmCodeParam[m_nPosIndex].m_FindCornerPara.m_dGridSize);
		m_bDmcodeOutputPoint = m_vtmpDmCodeParam[m_nPosIndex].m_FindCornerPara.m_bOutputOutPoints;

		m_strCalibDMCodeMaxNum.Format("%d",m_vtmpDmCodeParam[m_nPosIndex].m_FindDMCodePara.m_nMaxCodeNum);
		m_strCalibDMCodeMaxTime.Format("%d",m_vtmpDmCodeParam[m_nPosIndex].m_FindDMCodePara.m_nWaitingTime);
		m_nDmCodeMirror = (int)m_vtmpDmCodeParam[m_nPosIndex].m_FindDMCodePara.m_modeMirror;
		m_nDecodeMethod = (int)m_vtmpDmCodeParam[m_nPosIndex].m_FindDMCodePara.m_methodDecode;
		m_strMinBarSize.Format("%d",m_vtmpDmCodeParam[m_nPosIndex].m_FindDMCodePara.m_nMinBarSize);
		m_strMaxBarSize.Format("%d",m_vtmpDmCodeParam[m_nPosIndex].m_FindDMCodePara.m_nMaxBarSize);
		m_strSampleLevel.Format("%d",m_vtmpDmCodeParam[m_nPosIndex].m_FindDMCodePara.m_nSampleLevel);
		m_bDmcodeRecode = m_vtmpDmCodeParam[m_nPosIndex].m_bEnableReDecode;
		
		m_GuiDmCodeSearchRect.SetRect(m_vtmpDmCodeParam[m_nPosIndex].m_DmCodeSearchRect);
		m_GuiCornersSearchRect.SetRect(m_vtmpDmCodeParam[m_nPosIndex].m_CornersSearchRect);

		/*if (m_nDecodeMethod == 0)
		{
			GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_SAMPLE_LEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_MIN_BAR_SIZE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_MAX_BAR_SIZE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->EnableWindow(TRUE);
		}*/

		UpdateData(FALSE);
	}
}

void CDlgCalibTargetDmcodeParam::UpdateTrainGui(bool bShow)
{
	if (m_vImageInput[m_nPosIndex].IsValid() && bShow)
	{
		SetGuiByImage(m_vImageInput[m_nPosIndex].Width(),m_vImageInput[m_nPosIndex].Height());
		m_GuiDmCodeSearchRect.SetVisible(bShow);
		m_GuiCornersSearchRect.SetVisible(bShow);
		
	}
	else
	{
		m_GuiDmCodeSearchRect.SetVisible(FALSE);
		m_GuiCornersSearchRect.SetVisible(FALSE);
	}
	PostMessageUpdateGuiCalibrateTargetGraphics(bShow);
}
void CDlgCalibTargetDmcodeParam::AddGuiTrainGraphicsToDisplay(scGuiDisplay* pGuiDisplay)
{
	if (pGuiDisplay == NULL && ::IsWindow(pGuiDisplay->GetSafeHwnd()))
	{
		return;
	}

	m_pInteractiveContainer = m_pGuiDisplay.GetInteractiveContainer();
	m_pInteractiveContainer->AddItem(&m_GuiCornersSearchRect, m_strKeyText+_T("_CornersSearchRect"));
	m_pInteractiveContainer->AddItem(&m_GuiDmCodeSearchRect, m_strKeyText+_T("_DmCodeSearchRect"));	
	m_pGuiDisplay.SetInteractiveContainer(m_pInteractiveContainer);

}
void CDlgCalibTargetDmcodeParam::UpdateResultGui(bool bShow)
{

	CGuiGraphics* tempCalibrateGraphics = &m_GuiCalibrateGraphics;
	if (bShow)
	{
		for (int j=0;j<tempCalibrateGraphics->m_vGuiObjectCross.size();j++)
		{
			tempCalibrateGraphics->m_vGuiObjectCross.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowObjectCross.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiTargetCross.size();j++)
		{
			tempCalibrateGraphics->m_vGuiTargetCross.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowTargetCross.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiObjectCoordCross.size();j++)
		{
			tempCalibrateGraphics->m_vGuiObjectCoordCross.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowObjectCoordCross.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiTargetCoordCross.size();j++)
		{
			tempCalibrateGraphics->m_vGuiTargetCoordCross.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowTargetCoordCross.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiObjectPolyline.size();j++)
		{
			tempCalibrateGraphics->m_vGuiObjectPolyline.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowObjectPolyline.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiTargetPolyline.size();j++)
		{
			tempCalibrateGraphics->m_vGuiTargetPolyline.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowTargetPolyline.at(j));
		}
	}
	else
	{
		for (int j=0;j<tempCalibrateGraphics->m_vGuiObjectCross.size();j++)
		{
			tempCalibrateGraphics->m_vbShowObjectCross.at(j) = FALSE;
			tempCalibrateGraphics->m_vGuiObjectCross.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowObjectCross.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiTargetCross.size();j++)
		{
			tempCalibrateGraphics->m_vbShowTargetCross.at(j) = FALSE;
			tempCalibrateGraphics->m_vGuiTargetCross.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowTargetCross.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiObjectCoordCross.size();j++)
		{
			tempCalibrateGraphics->m_vbShowObjectCoordCross.at(j) = FALSE;
			tempCalibrateGraphics->m_vGuiObjectCoordCross.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowObjectCoordCross.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiTargetCoordCross.size();j++)
		{
			tempCalibrateGraphics->m_vbShowTargetCoordCross.at(j) = FALSE;
			tempCalibrateGraphics->m_vGuiTargetCoordCross.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowTargetCoordCross.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiObjectPolyline.size();j++)
		{
			tempCalibrateGraphics->m_vbShowObjectPolyline.at(j) = FALSE;
			tempCalibrateGraphics->m_vGuiObjectPolyline.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowObjectPolyline.at(j));
		}
		for (int j=0;j<tempCalibrateGraphics->m_vGuiTargetPolyline.size();j++)
		{
			tempCalibrateGraphics->m_vbShowTargetPolyline.at(j) = FALSE;
			tempCalibrateGraphics->m_vGuiTargetPolyline.at(j)->SetVisible(bShow && tempCalibrateGraphics->m_vbShowTargetPolyline.at(j));
		}
	}
	PostMessageUpdateGuiCalibrateTargetGraphics(bShow);
}

void CDlgCalibTargetDmcodeParam::CreateGUID(CString &strGUID)
{	
	GUID guid;
	CoCreateGuid(&guid);
	WCHAR   wstr[100]; 
	StringFromGUID2(guid,wstr,100); 
	CString   str   =   wstr; 	
	strGUID = wstr;
}
void CDlgCalibTargetDmcodeParam::PostMessageUpdateGuiCalibrateTargetGraphics(BOOL bShow)
{
	::PostMessage(this->GetSafeHwnd(), WM_UPDATE_CALIBRATE_TARGET_GRAPHIC, 0, bShow);
}

void CDlgCalibTargetDmcodeParam::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strInfo;
	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_IS_SEARCHING);
	m_strStatusBarInfo2 = strInfo;
	UpdateStatusBar();
	UpdateDlgData(TRUE);
	UpdateResultGui(false);
	scTimer t;
	m_DmCodeCalibTool.SetSearchImage(m_vImageInput[m_nPosIndex]);
	m_DmCodeCalibTool.SetDmCodeParam(m_vtmpDmCodeParam[m_nPosIndex]);
	if (!m_DmCodeCalibTool.SearchDmCode())
	{
		strInfo.Format(_T("%s"),m_DmCodeCalibTool.GetErrorString());
		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 =  GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DM_CODE_SEARCH_FAIL);
		m_strStatusBarInfo2 = strInfo;
		UpdateStatusBar();
		return ;
	}
	else
	{
		Code2DResult tmpResult;
		m_DmCodeCalibTool.GetDmCodeResult(tmpResult);
		strInfo.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DM_CODE_CORNERS),m_DmCodeCalibTool.GetCornersNum()
			,tmpResult.m_CodeString);
		CString strTime;
		strTime.Format(_T("，T：%.1fms"),t.GetTimeMilli(TRUE));
		strInfo = strInfo + strTime;
		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DM_CODE_SEARCH_SUCCESS);
		m_strStatusBarInfo2 = strInfo;
		UpdateStatusBar();

	}
	m_comboDisplayImage.SetCurSel(2);
	UpdateGuiDisplay();
}

void CDlgCalibTargetDmcodeParam::AddGuiResultGraphicsToDisplay(scGuiDisplay* pGuiDisplay)
{
	if (pGuiDisplay == NULL && ::IsWindow(pGuiDisplay->GetSafeHwnd()))
	{
		return;
	}

	CString strInfo1;
	scGuiStaticContainer* pGuiStaticContainer = NULL;
	pGuiStaticContainer = pGuiDisplay->GetStaticContainer();
	CGuiGraphics* tempCalibrateGraphics = &m_GuiCalibrateGraphics;
	for (int j=0;j<tempCalibrateGraphics->m_vGuiObjectCross.size();j++)
	{
		strInfo1.Format(_T("Gui_CalibrateObjectCross%d"), j);
		pGuiStaticContainer->RemoveItem(strInfo1);	
		pGuiStaticContainer->AddItem(tempCalibrateGraphics->m_vGuiObjectCross.at(j), strInfo1);
	}
	for (int j=0;j<tempCalibrateGraphics->m_vGuiTargetCross.size();j++)
	{
		strInfo1.Format(_T("Gui_CalibrateTargetCross%d"), j);
		pGuiStaticContainer->RemoveItem(strInfo1);	
		pGuiStaticContainer->AddItem(tempCalibrateGraphics->m_vGuiTargetCross.at(j), strInfo1);
	}
	for (int j=0;j<tempCalibrateGraphics->m_vGuiObjectCoordCross.size();j++)
	{
		strInfo1.Format(_T("Gui_CalibrateObjectCoordCross%d"), j);
		pGuiStaticContainer->RemoveItem(strInfo1);	
		pGuiStaticContainer->AddItem(tempCalibrateGraphics->m_vGuiObjectCoordCross.at(j), strInfo1);
	}
	for (int j=0;j<tempCalibrateGraphics->m_vGuiTargetCoordCross.size();j++)
	{
		strInfo1.Format(_T("Gui_CalibrateTargetCoordCross%d"), j);
		pGuiStaticContainer->RemoveItem(strInfo1);	
		pGuiStaticContainer->AddItem(tempCalibrateGraphics->m_vGuiTargetCoordCross.at(j), strInfo1);
	}
	for (int j=0;j<tempCalibrateGraphics->m_vGuiObjectPolyline.size();j++)
	{
		strInfo1.Format(_T("Gui_CalibrateObjectPolyline%d"), j);
		pGuiStaticContainer->RemoveItem(strInfo1);	
		pGuiStaticContainer->AddItem(tempCalibrateGraphics->m_vGuiObjectPolyline.at(j), strInfo1);
	}
	for (int j=0;j<tempCalibrateGraphics->m_vGuiTargetPolyline.size();j++)
	{
		strInfo1.Format(_T("Gui_CalibrateTargetPolyline%d"), j);
		pGuiStaticContainer->RemoveItem(strInfo1);	
		pGuiStaticContainer->AddItem(tempCalibrateGraphics->m_vGuiTargetPolyline.at(j), strInfo1);
	}
	pGuiDisplay->SetStaticContainer(pGuiStaticContainer);
}

void CDlgCalibTargetDmcodeParam::UpdateGuiDisplay()
{
	UpdateData(TRUE);
	m_pGuiDisplay.RemoveImage(FALSE);
	switch(m_comboDisplayImage.GetCurSel())
	{
	case 0:
		{		
			m_pGuiDisplay.SetImage(m_vImageInput.at(m_nPosIndex));
			UpdateTrainGui(false);
			UpdateResultGui(false);
		}
		break;
	case 1:
		{
			m_pGuiDisplay.SetImage(m_vImageInput.at(m_nPosIndex));
			UpdateTrainGui(true);
			UpdateResultGui(false);
		}
		break;
	case 2:
		{
			m_pGuiDisplay.SetImage(m_vImageInput.at(m_nPosIndex));
			UpdateTrainGui(false);
			UpdateResultGui(true);
		}
		break;
	}
}



void CDlgCalibTargetDmcodeParam::OnBnClickedBtnDmcodeFullImage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_comboDisplayImage.GetCurSel() == 1 && m_vImageInput[m_nPosIndex].IsValid())
	{
		m_GuiDmCodeSearchRect.SetCenterWidthHeight(m_vImageInput[m_nPosIndex].Width()/2,m_vImageInput[m_nPosIndex].Height()/2
			,m_vImageInput[m_nPosIndex].Width()*98/100,m_vImageInput[m_nPosIndex].Height()*98/100);
		UpdateGuiDisplay();
	}
}


void CDlgCalibTargetDmcodeParam::OnBnClickedBtnCornersFullImage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_comboDisplayImage.GetCurSel() == 1 && m_vImageInput[m_nPosIndex].IsValid())
	{
		m_GuiCornersSearchRect.SetCenterWidthHeight(m_vImageInput[m_nPosIndex].Width()/2,m_vImageInput[m_nPosIndex].Height()/2
			,m_vImageInput[m_nPosIndex].Width(),m_vImageInput[m_nPosIndex].Height());
		UpdateGuiDisplay();
	}
}


void CDlgCalibTargetDmcodeParam::OnBnClickedBtnShutterSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[曝光设置]"));
	g_opLogFile.SaveLogText(strLog,6);

	CameraParam para;
	double dMin = 0, dMax = 0,dShutter = 0.0;
	if (!m_pVisionASM->GetPosCameraShutter(m_nPosIndex, dShutter,dMin,dMax))
	{
		AfxMessageBox(_T("获取相机曝光错误！"));
		return;
	}

	CDlgDmCodeShutterSetting dlg(this);
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.m_dEnvironmentShutter = m_dEnvironmentShutter[m_nPosIndex];

	if (dlg.DoModal() == IDOK)
	{
		m_vtmpDmCodeParam[m_nPosIndex].m_dShutter = dlg.m_dDmCodeShutter;
	}
	else
	{
		m_pVisionASM->SwitchToSearchShutter(m_nPosIndex, dShutter);
	}
}

bool CDlgCalibTargetDmcodeParam::SetGuiByImage(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_vtmpDmCodeParam[m_nPosIndex].m_dImageWidth) < 10 && abs(nHeight - m_vtmpDmCodeParam[m_nPosIndex].m_dImageHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen;
	m_GuiDmCodeSearchRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiDmCodeSearchRect.SetCenterWidthHeight((centerx/m_vtmpDmCodeParam[m_nPosIndex].m_dImageWidth)*nWidth, (centery/m_vtmpDmCodeParam[m_nPosIndex].m_dImageHeight)*nHeight, (xlen/m_vtmpDmCodeParam[m_nPosIndex].m_dImageWidth)*nWidth, (ylen/m_vtmpDmCodeParam[m_nPosIndex].m_dImageHeight)*nHeight);

	//搜索区域

	m_GuiCornersSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiCornersSearchRect.SetCenterWidthHeight((centerx/m_vtmpDmCodeParam[m_nPosIndex].m_dImageWidth)*nWidth, (centery/m_vtmpDmCodeParam[m_nPosIndex].m_dImageHeight)*nHeight, (xlen/m_vtmpDmCodeParam[m_nPosIndex].m_dImageWidth)*nWidth, (ylen/m_vtmpDmCodeParam[m_nPosIndex].m_dImageHeight)*nHeight);

	m_vtmpDmCodeParam[m_nPosIndex].m_dImageHeight = nHeight;
	m_vtmpDmCodeParam[m_nPosIndex].m_dImageWidth = nWidth;
	return true;
}

void CDlgCalibTargetDmcodeParam::SetIsBoardCalibrate(bool bBoardCalib)
{
	m_bIsBoardCalib = bBoardCalib;
}


void CDlgCalibTargetDmcodeParam::OnBnClickedRdoDmcodeMethodDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	OnDeCodeMethodChange();
}


void CDlgCalibTargetDmcodeParam::OnBnClickedRdoDecodeMethodVsb()
{
	// TODO: 在此添加控件通知处理程序代码
	OnDeCodeMethodChange();
}


void CDlgCalibTargetDmcodeParam::OnBnClickedRdoMirrorModeClose()
{
	// TODO: 在此添加控件通知处理程序代码
	OnDmcodemirrorChange();
}


void CDlgCalibTargetDmcodeParam::OnBnClickedRdoMirrorModeOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	OnDmcodemirrorChange();
}


void CDlgCalibTargetDmcodeParam::OnBnClickedRdoMirrorModeAny()
{
	// TODO: 在此添加控件通知处理程序代码
	OnDmcodemirrorChange();
}


void CDlgCalibTargetDmcodeParam::OnEnSetfocusEditSampleLevel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SAMPLE_LEVEL)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SAMPLE_LEVEL, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SAMPLE_LEVEL, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[采样水平]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,m_LogLevel);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValDMCodeMaxNum();
	}		
}


void CDlgCalibTargetDmcodeParam::OnEnKillfocusEditSampleLevel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SAMPLE_LEVEL, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_SAMPLE_LEVEL, m_strSampleLevel);
		}
		else
		{
			m_strSampleLevel.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_SAMPLE_LEVEL, m_strSampleLevel);
		}

	}
}


void CDlgCalibTargetDmcodeParam::OnEnSetfocusEditMinBarSize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_MIN_BAR_SIZE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_MIN_BAR_SIZE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_MIN_BAR_SIZE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[采样水平]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,m_LogLevel);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValDMCodeMaxNum();
	}	
}


void CDlgCalibTargetDmcodeParam::OnEnKillfocusEditMinBarSize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_MIN_BAR_SIZE, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_MIN_BAR_SIZE, m_strMinBarSize);
		}
		else
		{
			m_strMinBarSize.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_MIN_BAR_SIZE, m_strMinBarSize);
		}

	}
}


void CDlgCalibTargetDmcodeParam::OnEnSetfocusEditMaxBarSize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_MAX_BAR_SIZE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_MAX_BAR_SIZE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_MAX_BAR_SIZE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大尺寸]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,m_LogLevel);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValDMCodeMaxNum();
	}	
}


void CDlgCalibTargetDmcodeParam::OnEnKillfocusEditMaxBarSize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_MAX_BAR_SIZE, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_MAX_BAR_SIZE, m_strMaxBarSize);
		}
		else
		{
			m_strMaxBarSize.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_MAX_BAR_SIZE, m_strMaxBarSize);
		}

	}
}


void CDlgCalibTargetDmcodeParam::OnBnClickedButtonCopy()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_cmbCopySrc.GetCurSel() == m_nPosIndex)
	{
		UpdateDlgData(TRUE);
	}
	for(int i = 0; i < m_pVisionASM->GetPosNum(); i++)
	{
		if (m_vpCheckPos.at(i)->GetCheck() == BST_CHECKED && i != m_cmbCopySrc.GetCurSel())
		{
			CopyParameter(m_cmbCopySrc.GetCurSel(),i);
			
		}
	}
	UpdateDlgData(FALSE);
	m_comboDisplayImage.SetCurSel(1);
	UpdateGuiDisplay();
}
