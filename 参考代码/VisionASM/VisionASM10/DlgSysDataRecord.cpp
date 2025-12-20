// DlgSysDataRecord.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysDataRecord.h"
#include "svXMLConfigurator.h"
#include "VisionASMDlg.h"
#include "DlgSysEnvironSetting.h"
#include "SystemConfigure.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysDataRecord dialog


CDlgSysDataRecord::CDlgSysDataRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysDataRecord::IDD, pParent)
	, m_nPlatformIndex(0)
//	, m_bUseThreadSaveImageMode(FALSE)
, m_bSaveProcessSnapVDBFile(FALSE)
, m_GuitextWeight(0)
, m_GuiLineWidth(0)
, m_bSaveInsepectResultVDBFile(FALSE)
{
	m_pParent = NULL;
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;

	m_bShowContent = true;
	m_bShowSaveImg = true;
	m_bShowSaveVdb = true;
	m_bShowPathSet = true;
	m_bShowSaveByHour = true;

	m_bEnableContent = true;
	m_bEnableSaveImg = true;
	m_bEnablePathSet = true;
	m_bEnableSaveByHour = true;
	m_bEnableSaveVdb = true;


	m_bAlignerSearchPltfmInfo = FALSE;
	m_bCommunication = FALSE;
	m_bAlignToolLog = FALSE;

	m_bImageName = FALSE;
	m_bSaveNGImage = FALSE;
	m_nSaveFailedImageNum = 0;

	m_bSaveAllImage = FALSE;
	m_nSaveAllImageNum = 0;
	m_bSaveAllImageType = 0;
	m_strJpgQuality = _T("");

	m_bSaveVDBFile = FALSE;
	m_nVBDImageType = 0;
	m_nSaveVDBMaxNums = 0;
	m_strVDBJpgQuality = _T("50");


	m_bCustomLogPath = FALSE;
	m_strDataRecordPath = _T("");
	m_bDataRecordTime = FALSE;
	m_nDataRecordTimeNum = 48;
	m_dDiskSpaceMaxUsePercent = 95;


	m_bAlignerPrecision = FALSE;
	m_bAlignerTime = FALSE;
	m_bTempStartUpShowCamSN = FALSE;
	m_bTempSysKeyboard = FALSE;

}

void CDlgSysDataRecord::UpdateDialogLanguage()
{
//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
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
		
		if(_T("IDD_SYS_DATA_RECORD") == strDlgID)
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

void CDlgSysDataRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysDataRecord)
	DDX_Check(pDX, IDC_CHECK_ALIGNER_SEARCH_PLTFM_INFO, m_bAlignerSearchPltfmInfo);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_PRECISION, m_bAlignerPrecision);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TIME, m_bAlignerTime);
	DDX_Check(pDX, IDC_CHECK_COMMUNICATION, m_bCommunication);
	DDX_Check(pDX, IDC_CHECK_IMAGE_SAVE, m_bSaveNGImage);
	// 	DDX_Check(pDX, IDC_CHECK_CAMERA_SN_SETTING, m_bStartUpShowCamSNSetting);
	// 	DDX_Check(pDX, IDC_CHECK_SYS_KEYBOARD, m_bSysKeyboardEnabled);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SAVE_FAILED_IMAGE_NUM, m_nSaveFailedImageNum);
	DDX_Check(pDX, IDC_CHECK_DATA_RECORD_TIME, m_bDataRecordTime);
	DDX_Text(pDX, IDC_EDIT_DATA_RECORD_TIME_NUM, m_nDataRecordTimeNum);
	DDX_Text(pDX, IDC_EDIT_DISK_SPACE_MAX_USE_PERCENT, m_dDiskSpaceMaxUsePercent);
	DDX_Text(pDX, IDC_EDIT_LOG_PATH, m_strDataRecordPath);
	DDX_Check(pDX, IDC_CHK_CUSTOM_LOG_PATH, m_bCustomLogPath);
	DDX_Control(pDX, IDC_CMB_IMAGE_FORMAT, m_CmbLogImageSaveFormat);
	DDX_Text(pDX, IDC_EDIT_QUALITY, m_strJpgQuality);
	DDX_Check(pDX, IDC_CHECK_SAVE_VDB_FILE, m_bSaveVDBFile);
	//	DDX_Check(pDX, IDC_CHECK_USE_THREAD_SAVE_IMAGE_MODE, m_bUseThreadSaveImageMode);
	DDX_Radio(pDX, IDC_RADIO_VDB_IMAGE_TYPE, m_nVBDImageType);
	DDX_Text(pDX, IDC_EDIT_SAVE_VDB_NUM2, m_nSaveVDBMaxNums);
	DDX_Check(pDX, IDC_CHECK_ALIGN_TOOL, m_bAlignToolLog);
	DDX_Check(pDX, IDC_CHECK_IMAGE_ID_NAME, m_bImageName);
	DDX_Check(pDX, IDC_CHECK_ALL_IMAGE_SAVE, m_bSaveAllImage);
	DDX_Text(pDX, IDC_EDIT_SAVE_ALL_IMAGE_NUM, m_nSaveAllImageNum);
	DDX_Radio(pDX, IDC_RADIO_SEARCH_SAVE_IMAGE, m_bSaveAllImageType);
	DDX_Check(pDX, IDC_CHECK_VDB_ID_NAME, m_bVDBNameByID);
	DDX_Text(pDX, IDC_EDIT_QUALITY_VDB, m_strVDBJpgQuality);

	DDX_Check(pDX, IDC_CHECK_SAVE_VDB_FILE_OBJ, m_bSaveObjectSnapVDBFile);
	DDX_Check(pDX, IDC_CHECK_SAVE_VDB_FILE_TARGET, m_bSaveTargetSnapVDBFile);

	DDX_Check(pDX, IDC_CHECK_SAVE_VDB_FILE_PROCESS, m_bSaveProcessSnapVDBFile);
	DDX_Text(pDX, IDC_EDIT_GUI_TEXTWEIGHT, m_GuitextWeight);
	DDX_Text(pDX, IDC_EDIT_GUI_LINEWIDTH, m_GuiLineWidth);
	DDX_Check(pDX, IDC_CHECK_SVAE_INSPECT_RESULT_VDB, m_bSaveInsepectResultVDBFile);
}


BEGIN_MESSAGE_MAP(CDlgSysDataRecord, CDialog)
	//{{AFX_MSG_MAP(CDlgSysDataRecord)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_EDIT_SAVE_FAILED_IMAGE_NUM, &CDlgSysDataRecord::OnEnSetfocusEditSaveFailedImageNum)
	ON_EN_KILLFOCUS(IDC_EDIT_SAVE_FAILED_IMAGE_NUM, &CDlgSysDataRecord::OnEnKillfocusEditSaveFailedImageNum)
	ON_BN_CLICKED(IDC_CHECK_IMAGE_SAVE, &CDlgSysDataRecord::OnBnClickedCheckImageSave)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_SEARCH_PLTFM_INFO, &CDlgSysDataRecord::OnBnClickedCheckAlignerSearchPltfmInfo)
	ON_BN_CLICKED(IDC_CHECK_COMMUNICATION, &CDlgSysDataRecord::OnBnClickedCheckCommunication)
	ON_EN_SETFOCUS(IDC_EDIT_DATA_RECORD_TIME_NUM, &CDlgSysDataRecord::OnEnSetfocusEditDataRecordTimeNum)
	ON_EN_KILLFOCUS(IDC_EDIT_DATA_RECORD_TIME_NUM, &CDlgSysDataRecord::OnEnKillfocusEditDataRecordTimeNum)
	ON_BN_CLICKED(IDC_CHECK_DATA_RECORD_TIME, &CDlgSysDataRecord::OnBnClickedCheckDataRecordTime)
	ON_EN_SETFOCUS(IDC_EDIT_DISK_SPACE_MAX_USE_PERCENT, &CDlgSysDataRecord::OnEnSetfocusEditDiskSpaceMaxUsePercent)
	ON_EN_KILLFOCUS(IDC_EDIT_DISK_SPACE_MAX_USE_PERCENT, &CDlgSysDataRecord::OnEnKillfocusEditDiskSpaceMaxUsePercent)
	ON_BN_CLICKED(IDC_BTN_SET_LOG_PATH, &CDlgSysDataRecord::OnBnClickedBtnSetLogPath)
	ON_BN_CLICKED(IDC_CHK_CUSTOM_LOG_PATH, &CDlgSysDataRecord::OnBnClickedChkCustomLogPath)
	ON_CBN_SELCHANGE(IDC_CMB_IMAGE_FORMAT, &CDlgSysDataRecord::OnCbnSelchangeCmbImageFormat)
	ON_EN_SETFOCUS(IDC_EDIT_QUALITY, &CDlgSysDataRecord::OnEnSetfocusEditQuality)
	ON_EN_KILLFOCUS(IDC_EDIT_QUALITY, &CDlgSysDataRecord::OnEnKillfocusEditQuality)
	ON_BN_CLICKED(IDC_CHECK_SAVE_VDB_FILE, &CDlgSysDataRecord::OnBnClickedCheckSaveVdbFile)
	ON_EN_SETFOCUS(IDC_EDIT_SAVE_VDB_NUM2, &CDlgSysDataRecord::OnEnSetfocusEditSaveVdbNum2)
	ON_EN_KILLFOCUS(IDC_EDIT_SAVE_VDB_NUM2, &CDlgSysDataRecord::OnEnKillfocusEditSaveVdbNum2)
	ON_EN_SETFOCUS(IDC_EDIT_SAVE_ALL_IMAGE_NUM, &CDlgSysDataRecord::OnSetfocusEditSaveAllImageNum)
	ON_EN_KILLFOCUS(IDC_EDIT_SAVE_ALL_IMAGE_NUM, &CDlgSysDataRecord::OnKillfocusEditSaveAllImageNum)
	ON_BN_CLICKED(IDC_CHECK_ALL_IMAGE_SAVE, &CDlgSysDataRecord::OnBnClickedCheckAllImageSave)
	ON_EN_KILLFOCUS(IDC_EDIT_QUALITY_VDB, &CDlgSysDataRecord::OnEnKillfocusEditQualityVdb)
	ON_EN_SETFOCUS(IDC_EDIT_QUALITY_VDB, &CDlgSysDataRecord::OnEnSetfocusEditQualityVdb)
	ON_BN_CLICKED(IDC_RADIO_VDB_IMAGE_TYPE, &CDlgSysDataRecord::OnBnClickedRadioVdbImageType3)
	ON_BN_CLICKED(IDC_RADIO_VDB_IMAGE_TYPE2, &CDlgSysDataRecord::OnBnClickedRadioVdbImageType3)
	ON_BN_CLICKED(IDC_RADIO_VDB_IMAGE_TYPE3, &CDlgSysDataRecord::OnBnClickedRadioVdbImageType3)

	ON_BN_CLICKED(IDC_CHECK_VDB_ID_NAME, &CDlgSysDataRecord::OnBnClickedCheckVdbIdName)
	ON_BN_CLICKED(IDC_CHECK_SAVE_VDB_FILE_OBJ, &CDlgSysDataRecord::OnBnClickedCheckSaveVdbFileObj)
	ON_BN_CLICKED(IDC_CHECK_SAVE_VDB_FILE_TARGET, &CDlgSysDataRecord::OnBnClickedCheckSaveVdbFileTarget)
	ON_BN_CLICKED(IDC_CHECK_SAVE_VDB_FILE_PROCESS, &CDlgSysDataRecord::OnBnClickedCheckSaveVdbFileProcess)
	ON_EN_SETFOCUS(IDC_EDIT_GUI_TEXTWEIGHT, &CDlgSysDataRecord::OnEnSetfocusEditSaveGuitextWeight)
	ON_EN_KILLFOCUS(IDC_EDIT_GUI_TEXTWEIGHT, &CDlgSysDataRecord::OnEnKillfocusEditSaveGuitextWeight)
	ON_EN_SETFOCUS(IDC_EDIT_GUI_LINEWIDTH, &CDlgSysDataRecord::OnEnSetfocusEditSaveGuiLineWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_GUI_LINEWIDTH, &CDlgSysDataRecord::OnEnKillfocusEditSaveGuiLineWidth)
	ON_BN_CLICKED(IDC_CHECK_SVAE_INSPECT_RESULT_VDB, &CDlgSysDataRecord::OnBnClickedCheckSvaeInspectResultVdb)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysDataRecord message handlers

BOOL CDlgSysDataRecord::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_CmbLogImageSaveFormat.AddString(_T("BMP"));
	m_CmbLogImageSaveFormat.AddString(_T("JPEG"));
	m_CmbLogImageSaveFormat.AddString(_T("PNG"));
	m_CmbLogImageSaveFormat.SetCurSel(0);

	UpdateDlgByAuthorize();

// 	SetGroupBoxStatus(IDC_STATIC_VDB_IMAGE_TYPE,m_bEnableSaveImg && m_bSaveVDBFile,m_bShowSaveImg && m_bSaveVDBFile);
// 
// 	int nShow = SW_SHOW;
// 	if (!m_bEnableSaveImg)
// 	{
// 		nShow = SW_HIDE;
// 	}
// 	
// 
// 	if (m_CmbLogImageSaveFormat.GetCurSel() == 1)
// 	{
// 		GetDlgItem(IDC_STATIC_QUALITY)->ShowWindow(nShow);
// 		GetDlgItem(IDC_EDIT_QUALITY)->ShowWindow(nShow);
// 		GetDlgItem(IDC_STATIC_QUALITY_PERCENT)->ShowWindow(nShow);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_QUALITY)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_EDIT_QUALITY)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_QUALITY_PERCENT)->ShowWindow(SW_HIDE);
// 	}

	UpdateDialogLanguage();
	UpdateDlgData(FALSE);



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysDataRecord::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);

		m_sysDataRecord.m_bAlignerSearchPltfmInfo = m_bAlignerSearchPltfmInfo;
		m_sysDataRecord.m_bCommunication = m_bCommunication;
		m_sysDataRecord.m_bAlignToolLog = m_bAlignToolLog;

		m_sysDataRecord.m_bImageName = m_bImageName;
		m_sysDataRecord.m_bSaveNGImage = m_bSaveNGImage;
		m_sysDataRecord.m_nSaveFailedImageNum = m_nSaveFailedImageNum;
		m_sysDataRecord.m_bSaveAllImage = m_bSaveAllImage;
		m_sysDataRecord.m_nSaveAllImageNum = m_nSaveAllImageNum;
		m_sysDataRecord.m_bSaveAllImageType = m_bSaveAllImageType;
		m_sysDataRecord.m_eImageFormat = (CImageType)(m_CmbLogImageSaveFormat.GetCurSel());
		m_sysDataRecord.m_nImageQuility = atoi(m_strJpgQuality);

		m_sysDataRecord.m_bSaveVDBFile = m_bSaveVDBFile;
		m_sysDataRecord.m_nSaveVDBMaxNums = m_nSaveVDBMaxNums;
		m_sysDataRecord.m_bVDBNameByID=m_bVDBNameByID;
		m_sysDataRecord.m_nVDBImageQuility = atoi(m_strVDBJpgQuality);

		m_sysDataRecord.m_bSaveTargetSnapVDBFile=m_bSaveTargetSnapVDBFile;
		m_sysDataRecord.m_bSaveObjectSnapVDBFile=m_bSaveObjectSnapVDBFile;
		m_sysDataRecord.m_bSaveProcessSnapVDBFile=m_bSaveProcessSnapVDBFile;
		m_sysDataRecord.m_bSaveInsepectResultVDBFile = m_bSaveInsepectResultVDBFile; 


		m_sysDataRecord.m_nVBDImageType = m_nVBDImageType;
		m_sysDataRecord.m_GuitextWeight = m_GuitextWeight;
		m_sysDataRecord.m_GuiLineWidth = m_GuiLineWidth;


		m_sysDataRecord.m_bCustomLogPath = m_bCustomLogPath;
		m_sysDataRecord.m_bDataRecordTime = m_bDataRecordTime;
		m_sysDataRecord.m_nDataRecordTimeNum = m_nDataRecordTimeNum;
		m_sysDataRecord.m_dDiskSpaceMaxUsePercent = m_dDiskSpaceMaxUsePercent;
		m_sysDataRecord.m_strDataRecordPath = m_strDataRecordPath;
		//m_sysDataRecord.m_nStartHourRecordByDay=m_nStartHourRecordByDay;

//		m_sysDataRecord.m_bThreadSaveImageMode = m_bUseThreadSaveImageMode;

		m_sysDataRecord.m_bAlignerPrecision = m_bAlignerPrecision;
		m_sysDataRecord.m_bAlignerTime = m_bAlignerTime;
		
// 		m_bTempStartUpShowCamSN = m_bStartUpShowCamSNSetting;
// 		m_bTempSysKeyboard = m_bSysKeyboardEnabled;
	}
	else
	{
		
		
		m_bAlignerSearchPltfmInfo = m_sysDataRecord.m_bAlignerSearchPltfmInfo;
		m_bCommunication = m_sysDataRecord.m_bCommunication;
		m_bAlignToolLog = m_sysDataRecord.m_bAlignToolLog;

		m_bImageName = m_sysDataRecord.m_bImageName;
		m_bSaveNGImage = m_sysDataRecord.m_bSaveNGImage;
		m_nSaveFailedImageNum = m_sysDataRecord.m_nSaveFailedImageNum;
		m_bSaveAllImage = m_sysDataRecord.m_bSaveAllImage;
		m_nSaveAllImageNum = m_sysDataRecord.m_nSaveAllImageNum;
		m_bSaveAllImageType = m_sysDataRecord.m_bSaveAllImageType;
		m_CmbLogImageSaveFormat.SetCurSel((int)(m_sysDataRecord.m_eImageFormat));
		m_strJpgQuality.Format(_T("%d"),m_sysDataRecord.m_nImageQuility);

		m_bSaveVDBFile = m_sysDataRecord.m_bSaveVDBFile;
		m_bVDBNameByID = m_sysDataRecord.m_bVDBNameByID;
		m_strVDBJpgQuality.Format(_T("%d"),m_sysDataRecord.m_nVDBImageQuility);
		m_nSaveVDBMaxNums = m_sysDataRecord.m_nSaveVDBMaxNums;
		m_nVBDImageType = m_sysDataRecord.m_nVBDImageType;

		m_GuitextWeight = m_sysDataRecord.m_GuitextWeight;
		m_GuiLineWidth = m_sysDataRecord.m_GuiLineWidth;

		m_bSaveTargetSnapVDBFile=m_sysDataRecord.m_bSaveTargetSnapVDBFile;
		m_bSaveObjectSnapVDBFile=m_sysDataRecord.m_bSaveObjectSnapVDBFile;
		m_bSaveProcessSnapVDBFile=m_sysDataRecord.m_bSaveProcessSnapVDBFile;
		m_bSaveInsepectResultVDBFile = m_sysDataRecord.m_bSaveInsepectResultVDBFile; 

		m_bCustomLogPath = m_sysDataRecord.m_bCustomLogPath;
		m_bDataRecordTime = m_sysDataRecord.m_bDataRecordTime;		
		m_nDataRecordTimeNum = m_sysDataRecord.m_nDataRecordTimeNum;
		m_dDiskSpaceMaxUsePercent = m_sysDataRecord.m_dDiskSpaceMaxUsePercent;
		m_strDataRecordPath = m_sysDataRecord.m_strDataRecordPath;
			
//		m_bUseThreadSaveImageMode = m_sysDataRecord.m_bThreadSaveImageMode;

		m_bAlignerPrecision = m_sysDataRecord.m_bAlignerPrecision;
		m_bAlignerTime = m_sysDataRecord.m_bAlignerTime;


		GetDlgItem(IDC_EDIT_SAVE_FAILED_IMAGE_NUM)->EnableWindow(m_bEnableSaveImg && m_bSaveNGImage);
		GetDlgItem(IDC_EDIT_SAVE_ALL_IMAGE_NUM)->EnableWindow(m_bEnableSaveImg && m_bSaveAllImage);

		GetDlgItem(IDC_EDIT_SAVE_VDB_NUM2)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
		GetDlgItem(IDC_RADIO_VDB_IMAGE_TYPE)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
		GetDlgItem(IDC_RADIO_VDB_IMAGE_TYPE2)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
		GetDlgItem(IDC_CHECK_VDB_ID_NAME)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
		GetDlgItem(IDC_RADIO_VDB_IMAGE_TYPE3)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
		GetDlgItem(IDC_STATIC_QUALITY2)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile&&(m_nVBDImageType==2));
		GetDlgItem(IDC_EDIT_QUALITY_VDB)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile&&(m_nVBDImageType==2));
		GetDlgItem(IDC_CHECK_SAVE_VDB_FILE_TARGET)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
		GetDlgItem(IDC_CHECK_SAVE_VDB_FILE_OBJ)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
		GetDlgItem(IDC_CHECK_SAVE_VDB_FILE_PROCESS)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	    
		GetDlgItem(IDC_EDIT_GUI_TEXTWEIGHT)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
		GetDlgItem(IDC_EDIT_GUI_LINEWIDTH)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);



		GetDlgItem(IDC_EDIT_DATA_RECORD_TIME_NUM)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);
		GetDlgItem(IDC_EDIT_DISK_SPACE_MAX_USE_PERCENT)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);
 
		if (m_bCustomLogPath)
		{
			GetDlgItem(IDC_BTN_SET_LOG_PATH)->EnableWindow(m_bEnablePathSet);
			GetDlgItem(IDC_EDIT_LOG_PATH)->EnableWindow(m_bEnablePathSet);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_LOG_PATH)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_LOG_PATH)->EnableWindow(FALSE);
		}

		int nShowQuality = SW_HIDE;
		if (m_CmbLogImageSaveFormat.GetCurSel() == 1 && m_bShowSaveImg)
		{
			nShowQuality = SW_SHOW;
		}

		GetDlgItem(IDC_STATIC_QUALITY)->ShowWindow(nShowQuality);
		GetDlgItem(IDC_EDIT_QUALITY)->ShowWindow(nShowQuality);
		GetDlgItem(IDC_STATIC_QUALITY_PERCENT)->ShowWindow(nShowQuality);


		UpdateData(FALSE);
	}
	UpdateDlgByAuthorize();

}

SysDataRecord CDlgSysDataRecord::GetSysDataRecord() const
{
	return m_sysDataRecord;
}

void CDlgSysDataRecord::SetSysDataRecord(const SysDataRecord& dataRecord)
{
	m_sysDataRecord = dataRecord;
}

BOOL CDlgSysDataRecord::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSysDataRecord::OnEnSetfocusEditSaveFailedImageNum()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SAVE_FAILED_IMAGE_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SAVE_FAILED_IMAGE_NUM, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SAVE_FAILED_IMAGE_NUM, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				if (_ttoi(strOutput) > 500)
				{
					strLog.Format(_T("修改[存储失败图像数量]由%s到%d\n"),strInput,500);
					g_opLogFile.SaveLogText(strLog,3);
				}
				else{
					strLog.Format(_T("修改[存储失败图像数量]由%s到%s\n"),strInput,strOutput);
					g_opLogFile.SaveLogText(strLog,3);
				}			
			}
			
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSaveFailedImageNum();
	}	
}


void CDlgSysDataRecord::OnEnKillfocusEditSaveFailedImageNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nSaveFailedImageNum = (m_nSaveFailedImageNum<0) ? 0 : ((m_nSaveFailedImageNum>500) ? 500 : m_nSaveFailedImageNum);
		UpdateData(FALSE);
	}
}


void CDlgSysDataRecord::OnBnClickedCheckImageSave()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bImgDataOld = m_bSaveNGImage;
	UpdateData(TRUE);
	CString strLog;
	if (bImgDataOld != m_bSaveNGImage)
	{
		CString strVal = m_bSaveNGImage ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[存储搜索失败图像]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}

	GetDlgItem(IDC_EDIT_SAVE_FAILED_IMAGE_NUM)->EnableWindow(m_bEnableSaveImg && m_bSaveNGImage);
}


void CDlgSysDataRecord::OnBnClickedCheckAlignerSearchPltfmInfo()
{
	// TODO: 在此添加控件通知处理程序代码m_bAlignerSearchPltfmInfo
	BOOL bAlignerSearchPltfmInfoOld = m_bAlignerSearchPltfmInfo;
	UpdateData(TRUE);
	CString strLog;
	if (bAlignerSearchPltfmInfoOld != m_bAlignerSearchPltfmInfo)
	{
		CString strVal = m_bAlignerSearchPltfmInfo ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[存储搜索信息]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysDataRecord::OnBnClickedCheckCommunication()
{
	// TODO: 在此添加控件通知处理程序代码m_bCommunication
	BOOL bCommunicationOld = m_bCommunication;
	UpdateData(TRUE);
	CString strLog;
	if (bCommunicationOld != m_bCommunication)
	{
		CString strVal = m_bCommunication ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[存储通信指令]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysDataRecord::OnEnSetfocusEditDataRecordTimeNum()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DATA_RECORD_TIME_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DATA_RECORD_TIME_NUM, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_DATA_RECORD_TIME_NUM, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDataRecordTimeNum();
	}	
}

void CDlgSysDataRecord::OnEnKillfocusEditDataRecordTimeNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nDataRecordTimeNum = (m_nDataRecordTimeNum<1) ? 1 : m_nDataRecordTimeNum;
		UpdateData(FALSE);
	}
}

void CDlgSysDataRecord::OnBnClickedCheckDataRecordTime()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT_DATA_RECORD_TIME_NUM)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);
	GetDlgItem(IDC_STATIC_DATA_RECORD_TIME_NUM)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);

	GetDlgItem(IDC_EDIT_DISK_SPACE_MAX_USE_PERCENT)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);
	GetDlgItem(IDC_STATIC_DISK_SPACE_MAX_USE_PERCENT)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);
}

void CDlgSysDataRecord::OnEnSetfocusEditDiskSpaceMaxUsePercent()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_DISK_SPACE_MAX_USE_PERCENT)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_DISK_SPACE_MAX_USE_PERCENT, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_DISK_SPACE_MAX_USE_PERCENT, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDiskSpaceMaxUsePercent();
	}	
}

void CDlgSysDataRecord::OnEnKillfocusEditDiskSpaceMaxUsePercent()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_dDiskSpaceMaxUsePercent = (m_dDiskSpaceMaxUsePercent>99) ? 99 : m_dDiskSpaceMaxUsePercent;

		if (m_pParent && m_pParent->m_pParent)
		{
			double dtmp = m_pParent->m_pParent->m_SysOptionConfig.m_dMaxDiskUseThre*100;
			m_dDiskSpaceMaxUsePercent = (m_dDiskSpaceMaxUsePercent>dtmp)? dtmp : m_dDiskSpaceMaxUsePercent;
		}


		m_dDiskSpaceMaxUsePercent = (m_dDiskSpaceMaxUsePercent<1) ? 1 : m_dDiskSpaceMaxUsePercent;
		UpdateData(FALSE);
	}
}

void CDlgSysDataRecord::SetPlatformIndex(int nPlatformIndex)
{
	m_nPlatformIndex = nPlatformIndex;
}



void CDlgSysDataRecord::OnBnClickedBtnSetLogPath()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi; 
	ZeroMemory(&bi,sizeof(bi));
	TCHAR dispName[MAX_PATH]; 
	CString str_title = _T("浏览"); 
	ITEMIDLIST *targetLocation;  
	bi.hwndOwner = m_hWnd; 
	bi.pidlRoot = 0; 
	bi.pszDisplayName = dispName; 
	bi.lpszTitle = str_title; 
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_DONTGOBELOWDOMAIN; 
	bi.lpfn = NULL; 
	bi.lParam = 0; 
	bi.iImage = 0; 

	targetLocation=SHBrowseForFolder(&bi);
	
	if (targetLocation != NULL)
	{
		TCHAR szPath[MAX_PATH];
		SHGetPathFromIDList(targetLocation,szPath);

		CString strDrive = szPath[0];

		if(strDrive== _T(""))
		{
			AfxMessageBox(_T("选择路径无效!"));

			if(m_strDataRecordPath == _T(""))
			{
				m_strDataRecordPath = _T("D:\\");
				CString strTmp;
				if (m_pParent->m_SysOptionConfig.m_bPlatformCustomNameEnable)
				{
					strTmp.Format(_T("\\VSLog\\%s"),m_pParent->m_SysOptionConfig.m_strPlatformCustomName[m_nPlatformIndex]); //WS1开始
				}
				else
				{
					strTmp.Format(_T("\\VSLog\\WS%d"),m_nPlatformIndex+1); //WS1开始
				}
				m_strDataRecordPath = m_strDataRecordPath + strTmp;
				UpdateData(FALSE);
			}
			
			return;
		}

		if(strDrive== _T("C"))
		{
			AfxMessageBox(_T("禁止自定义日志路径设置为C盘"));
			return;
		}

		m_strDataRecordPath = szPath;
		CString strTmp;
		if (m_pParent->m_SysOptionConfig.m_bPlatformCustomNameEnable)
		{
			strTmp.Format(_T("\\VSLog\\%s"),m_pParent->m_SysOptionConfig.m_strPlatformCustomName[m_nPlatformIndex]); //WS1开始
		}
		else
		{
			strTmp.Format(_T("\\VSLog\\WS%d"),m_nPlatformIndex+1); //WS1开始
		}
		m_strDataRecordPath = m_strDataRecordPath + strTmp;
		UpdateData(FALSE);
	}
}


void CDlgSysDataRecord::OnBnClickedChkCustomLogPath()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCustomLogPath)
	{
		GetDlgItem(IDC_BTN_SET_LOG_PATH)->EnableWindow(m_bEnablePathSet);
		GetDlgItem(IDC_EDIT_LOG_PATH)->EnableWindow(m_bEnablePathSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_LOG_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOG_PATH)->EnableWindow(FALSE);
	}
}


void CDlgSysDataRecord::OnCbnSelchangeCmbImageFormat()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_CmbLogImageSaveFormat.GetCurSel() == 1)
	{
		GetDlgItem(IDC_STATIC_QUALITY)->ShowWindow(m_bShowSaveImg);
		GetDlgItem(IDC_EDIT_QUALITY)->ShowWindow(m_bShowSaveImg);
		GetDlgItem(IDC_STATIC_QUALITY_PERCENT)->ShowWindow(m_bShowSaveImg);
		GetDlgItem(IDC_STATIC_QUALITY)->EnableWindow(m_bEnableSaveImg);
		GetDlgItem(IDC_EDIT_QUALITY)->EnableWindow(m_bEnableSaveImg);
		GetDlgItem(IDC_STATIC_QUALITY_PERCENT)->EnableWindow(m_bEnableSaveImg);
	}
	else
	{
		GetDlgItem(IDC_STATIC_QUALITY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_QUALITY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_QUALITY_PERCENT)->ShowWindow(SW_HIDE);
	}
}


void CDlgSysDataRecord::OnEnSetfocusEditQuality()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_QUALITY)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_QUALITY, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_QUALITY, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditQuality();
	}
}


void CDlgSysDataRecord::OnEnKillfocusEditQuality()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		int nQuility = atoi(m_strJpgQuality);
		if (nQuility >100)
		{
			m_strJpgQuality.Format(_T("%d"),100);
		}
		else if(nQuility <=0)
		{
			m_strJpgQuality.Format(_T("%d"),1);
		}
		UpdateData(FALSE);
	}
}


void CDlgSysDataRecord::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
{
	CRect CRectGroupBox;
	// 检查是否为Group控件
	if(NULL== GetDlgItem(nID)) return;
	if (NULL== (CStatic*) GetDlgItem(nID)) return;
	GetDlgItem(nID)->GetWindowRect(&CRectGroupBox);
	ScreenToClient(&CRectGroupBox);

	int nShow = SW_SHOW;
	if (bShow)
	{
		nShow = SW_SHOW;
	}
	else
	{
		nShow = SW_HIDE;
	}

	// Group 控件本身也进行显示或隐藏
	GetDlgItem(nID)->EnableWindow(bEnable);
	GetDlgItem(nID)->ShowWindow(nShow);

	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		CRect CRect2;
		pWnd->GetWindowRect(&CRect2);
		ScreenToClient(&CRect2);
		if (CRectGroupBox.PtInRect(CRect2.TopLeft()) || CRectGroupBox.PtInRect(CRect2.BottomRight()))
		{
			pWnd->EnableWindow(bEnable);
			pWnd->ShowWindow(nShow);
		}
		pWnd = pWnd->GetNextWindow();
	}
}

void CDlgSysDataRecord::UpdateDlgByAuthorize()
{
	if (m_pParent && m_pParent->m_pParent && (m_pParent->m_pParent->m_vpVisionASM.size()>0) &&  (m_pParent->m_nPlatformIndex <  m_pParent->m_pParent->m_vpVisionASM.size()) && m_pParent->m_pParent->m_vpVisionASM.at(m_pParent->m_nPlatformIndex))
	{
		CVisionASMConfig *vsConfig;
		m_pParent->m_pParent->m_vpVisionASM.at(m_pParent->m_nPlatformIndex)->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			CString strNodeName = _T("");
			// 日志保存内容
			bool bFind = vsConfig->GetCheckedByItem(eLogAuthorize_Content,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowContent,m_bEnableContent);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_GRP_LOG_CONTENT,m_bEnableContent,m_bShowContent);

			}

			// 存图格式
			bFind = vsConfig->GetCheckedByItem(eLogAuthorize_SaveImg,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowSaveImg,m_bEnableSaveImg);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_IMAGE_FILE,m_bEnableSaveImg,m_bShowSaveImg);

				GetDlgItem(IDC_EDIT_SAVE_FAILED_IMAGE_NUM)->EnableWindow(m_bEnableSaveImg && m_bSaveNGImage);
				GetDlgItem(IDC_EDIT_SAVE_ALL_IMAGE_NUM)->EnableWindow(m_bEnableSaveImg && m_bSaveAllImage);
				GetDlgItem(IDC_RADIO_SEARCH_SAVE_IMAGE)->EnableWindow(m_bEnableSaveImg && m_bSaveAllImage);
				GetDlgItem(IDC_RADIO_THREAD_SAVE_IMAGE)->EnableWindow(m_bEnableSaveImg && m_bSaveAllImage);

				GetDlgItem(IDC_STATIC_QUALITY)->ShowWindow(m_bEnableSaveImg && m_CmbLogImageSaveFormat.GetCurSel() == 1);
				GetDlgItem(IDC_STATIC_QUALITY_PERCENT)->ShowWindow(m_bEnableSaveImg && m_CmbLogImageSaveFormat.GetCurSel() == 1);
				GetDlgItem(IDC_EDIT_QUALITY)->ShowWindow(m_bEnableSaveImg && m_CmbLogImageSaveFormat.GetCurSel() == 1);

			}

			// 存图vdb
			bFind = vsConfig->GetCheckedByItem(eLogAuthorize_SaveVDB,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowSaveVdb,m_bEnableSaveVdb);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_VDB_IMAGE_TYPE,m_bEnableSaveVdb,m_bShowSaveVdb);

				GetDlgItem(IDC_RADIO_VDB_IMAGE_TYPE)->EnableWindow(m_bEnableSaveImg && m_bSaveVDBFile);
				GetDlgItem(IDC_RADIO_VDB_IMAGE_TYPE2)->EnableWindow(m_bEnableSaveImg && m_bSaveVDBFile);
				GetDlgItem(IDC_EDIT_SAVE_VDB_NUM2)->EnableWindow(m_bEnableSaveImg && m_bSaveVDBFile);
				GetDlgItem(IDC_RADIO_VDB_IMAGE_TYPE3)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
				GetDlgItem(IDC_CHECK_VDB_ID_NAME)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
				GetDlgItem(IDC_STATIC_QUALITY2)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile&&(m_nVBDImageType==2));
				GetDlgItem(IDC_EDIT_QUALITY_VDB)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile&&(m_nVBDImageType==2));
				GetDlgItem(IDC_CHECK_SAVE_VDB_FILE_TARGET)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
				GetDlgItem(IDC_CHECK_SAVE_VDB_FILE_OBJ)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
				GetDlgItem(IDC_CHECK_SAVE_VDB_FILE_PROCESS)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
				GetDlgItem(IDC_EDIT_GUI_TEXTWEIGHT)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
				GetDlgItem(IDC_EDIT_GUI_LINEWIDTH)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);

			}

			
			// 路径设置
			bFind = vsConfig->GetCheckedByItem(eLogAuthorize_PathSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowPathSet,m_bEnablePathSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_GRP_CUSTOM_LOG_PATH,m_bEnablePathSet,m_bShowPathSet);

				GetDlgItem(IDC_BTN_SET_LOG_PATH)->EnableWindow(m_bEnablePathSet && m_bCustomLogPath);
				GetDlgItem(IDC_EDIT_LOG_PATH)->EnableWindow(m_bEnablePathSet && m_bCustomLogPath);

			}

			// 按小时存图
			bFind = vsConfig->GetCheckedByItem(eLogAuthorize_SaveByHour,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowSaveByHour,m_bEnableSaveByHour);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_GRP_LOG_MODE,m_bEnableSaveByHour,m_bShowSaveByHour);
				GetDlgItem(IDC_EDIT_DATA_RECORD_TIME_NUM)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);
				GetDlgItem(IDC_STATIC_DATA_RECORD_TIME_NUM)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);

				GetDlgItem(IDC_EDIT_DISK_SPACE_MAX_USE_PERCENT)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);
				GetDlgItem(IDC_STATIC_DISK_SPACE_MAX_USE_PERCENT)->EnableWindow(m_bDataRecordTime && m_bEnableSaveByHour);
			}

			GetDlgItem(IDC_STATIC_GRP_SYS_DATA_RECORD)->EnableWindow(m_bEnableContent || m_bEnableSaveImg || m_bEnablePathSet || m_bEnableSaveByHour || m_bEnableSaveVdb);
			GetDlgItem(IDC_STATIC_GRP_SYS_DATA_RECORD)->ShowWindow(m_bShowContent || m_bShowSaveImg || m_bShowPathSet || m_bShowSaveByHour || m_bShowSaveVdb);

		}
	}
}


void CDlgSysDataRecord::OnBnClickedCheckSaveVdbFile()
{
	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 	SetGroupBoxStatus(IDC_STATIC_VDB_IMAGE_TYPE,m_bEnableSaveImg && m_bSaveVDBFile,m_bShowSaveImg && m_bSaveVDBFile);

	BOOL bSaveVDBFile = m_bSaveVDBFile;
	UpdateData(TRUE);
	CString strLog;
	if (bSaveVDBFile != m_bSaveVDBFile)
	{
		CString strVal = m_bSaveVDBFile ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[存储VDB文件]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}

	GetDlgItem(IDC_EDIT_SAVE_VDB_NUM2)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	GetDlgItem(IDC_RADIO_VDB_IMAGE_TYPE)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	GetDlgItem(IDC_RADIO_VDB_IMAGE_TYPE2)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	GetDlgItem(IDC_RADIO_VDB_IMAGE_TYPE3)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	GetDlgItem(IDC_CHECK_VDB_ID_NAME)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	GetDlgItem(IDC_STATIC_QUALITY2)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile&&(m_nVBDImageType==2));
	GetDlgItem(IDC_EDIT_QUALITY_VDB)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile&&(m_nVBDImageType==2));
	GetDlgItem(IDC_CHECK_SAVE_VDB_FILE_TARGET)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	GetDlgItem(IDC_CHECK_SAVE_VDB_FILE_OBJ)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	GetDlgItem(IDC_CHECK_SAVE_VDB_FILE_PROCESS)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	GetDlgItem(IDC_EDIT_GUI_TEXTWEIGHT)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);
	GetDlgItem(IDC_EDIT_GUI_LINEWIDTH)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile);

}


void CDlgSysDataRecord::OnEnSetfocusEditSaveVdbNum2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SAVE_VDB_NUM2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SAVE_VDB_NUM2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SAVE_VDB_NUM2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				if (_ttoi(strOutput) > 500)
				{
					strLog.Format(_T("修改[最大存VDB数目]由%s到%d\n"),strInput,500);
					g_opLogFile.SaveLogText(strLog,3);
				}
				else
				{
					strLog.Format(_T("修改[最大存VDB数目]由%s到%s\n"),strInput,strOutput);
					g_opLogFile.SaveLogText(strLog,3);
				}			
			}

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSaveVdbNum2();
	}	
}


void CDlgSysDataRecord::OnEnKillfocusEditSaveVdbNum2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nSaveVDBMaxNums = (m_nSaveVDBMaxNums<0) ? 0 : ((m_nSaveVDBMaxNums>50000) ? 50000 : m_nSaveVDBMaxNums);
		UpdateData(FALSE);
	}
}


void CDlgSysDataRecord::OnSetfocusEditSaveAllImageNum()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SAVE_ALL_IMAGE_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SAVE_ALL_IMAGE_NUM, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SAVE_ALL_IMAGE_NUM, strOutput);

// 			if (strInput != strOutput)
// 			{
// 					strLog.Format(_T("修改[存储失败图像数量]由%s到%s\n"),strInput,strOutput);
// 					g_opLogFile.SaveLogText(strLog,3);
// 				}			
// 			}

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditSaveAllImageNum();
	}	

}


void CDlgSysDataRecord::OnKillfocusEditSaveAllImageNum()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nSaveAllImageNum = (m_nSaveAllImageNum<0) ? 0 : ((m_nSaveAllImageNum>500) ? 500 : m_nSaveAllImageNum);
		UpdateData(FALSE);
	}
}


void CDlgSysDataRecord::OnBnClickedCheckAllImageSave()
{
	// TODO: 在此添加控件通知处理程序代码

	BOOL bSaveAllImage = m_bSaveAllImage;
	UpdateData(TRUE);
	CString strLog;
	if (bSaveAllImage != m_bSaveAllImage)
	{
		CString strVal = m_bSaveAllImage ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[存储所有图像]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}

	GetDlgItem(IDC_EDIT_SAVE_ALL_IMAGE_NUM)->EnableWindow(m_bEnableSaveImg && m_bSaveAllImage);
	GetDlgItem(IDC_RADIO_SEARCH_SAVE_IMAGE)->EnableWindow(m_bEnableSaveImg && m_bSaveAllImage);
	GetDlgItem(IDC_RADIO_THREAD_SAVE_IMAGE)->EnableWindow(m_bEnableSaveImg && m_bSaveAllImage);
}


void CDlgSysDataRecord::OnEnKillfocusEditQualityVdb()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		int nQuility = atoi(m_strVDBJpgQuality);
		if (nQuility >100)
		{
			m_strVDBJpgQuality.Format(_T("%d"),100);
		}
		else if(nQuility <=0)
		{
			m_strVDBJpgQuality.Format(_T("%d"),1);
		}
		UpdateData(FALSE);
	}
}


void CDlgSysDataRecord::OnEnSetfocusEditQualityVdb()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_QUALITY_VDB)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_QUALITY_VDB, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_QUALITY_VDB, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditQualityVdb();
	}
}


void CDlgSysDataRecord::OnBnClickedRadioVdbImageType3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	GetDlgItem(IDC_STATIC_QUALITY2)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile&&(m_nVBDImageType==2));
	GetDlgItem(IDC_EDIT_QUALITY_VDB)->EnableWindow(m_bEnableSaveVdb && m_bSaveVDBFile&&(m_nVBDImageType==2));

}


void CDlgSysDataRecord::OnBnClickedCheckVdbIdName()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bImgDataOld = m_bVDBNameByID;
	UpdateData(TRUE);
	CString strLog;
	if (bImgDataOld != m_bVDBNameByID)
	{
		CString strVal = m_bVDBNameByID ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[以ID命名VDB]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}

	
}


void CDlgSysDataRecord::OnBnClickedCheckSaveVdbFileObj()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bImgDataOld = m_bSaveObjectSnapVDBFile;
	UpdateData(TRUE);
	CString strLog;
	if (bImgDataOld != m_bSaveObjectSnapVDBFile)
	{
		CString strVal = m_bSaveObjectSnapVDBFile ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[存储对象VDB文件]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysDataRecord::OnBnClickedCheckSaveVdbFileTarget()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bImgDataOld = m_bSaveTargetSnapVDBFile;
	UpdateData(TRUE);
	CString strLog;
	if (bImgDataOld != m_bSaveTargetSnapVDBFile)
	{
		CString strVal = m_bSaveTargetSnapVDBFile ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[存储目标VDB文件]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysDataRecord::OnBnClickedCheckSaveVdbFileProcess()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bImgDataOld = m_bSaveProcessSnapVDBFile;
	UpdateData(TRUE);
	CString strLog;
	if (bImgDataOld != m_bSaveProcessSnapVDBFile)
	{
		CString strVal = m_bSaveProcessSnapVDBFile ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[存储流程VDB文件]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysDataRecord::OnEnSetfocusEditSaveGuitextWeight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_GUI_TEXTWEIGHT)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_GUI_TEXTWEIGHT, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_GUI_TEXTWEIGHT, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				if (_ttoi(strOutput) > 20)
				{
					strLog.Format(_T("修改[VDB字体大小]由%s到%d\n"),strInput,20);
					g_opLogFile.SaveLogText(strLog,3);
				}
				else
				{
					strLog.Format(_T("修改[VDB字体大小]由%s到%s\n"),strInput,strOutput);
					g_opLogFile.SaveLogText(strLog,3);
				}			
			}

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSaveGuitextWeight();
	}



}


void CDlgSysDataRecord::OnEnKillfocusEditSaveGuitextWeight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_GuitextWeight = (m_GuitextWeight<0) ? 1 : ((m_GuitextWeight>20) ? 20 : m_GuitextWeight);
		UpdateData(FALSE);
	}
}


void CDlgSysDataRecord::OnEnSetfocusEditSaveGuiLineWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_GUI_LINEWIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_GUI_LINEWIDTH, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_GUI_LINEWIDTH, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				if (_ttoi(strOutput) > 50)
				{
					strLog.Format(_T("修改[VDB线宽]由%s到%d\n"),strInput,50);
					g_opLogFile.SaveLogText(strLog,3);
				}
				else
				{
					strLog.Format(_T("修改[VDB线宽]由%s到%s\n"),strInput,strOutput);
					g_opLogFile.SaveLogText(strLog,3);
				}			
			}

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSaveGuiLineWidth();
	}
}


void CDlgSysDataRecord::OnEnKillfocusEditSaveGuiLineWidth()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_GuiLineWidth = (m_GuiLineWidth<0) ? 1 : ((m_GuiLineWidth>50) ? 50 : m_GuiLineWidth);
		UpdateData(FALSE);
	}

}


void CDlgSysDataRecord::OnBnClickedCheckSvaeInspectResultVdb()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bImgDataOld = m_bSaveInsepectResultVDBFile;
	UpdateData(TRUE);
	CString strLog;
	if (bImgDataOld != m_bSaveInsepectResultVDBFile)
	{
		CString strVal = m_bSaveInsepectResultVDBFile ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[按照检测结果存储]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}
