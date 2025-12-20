// DlgIdReaderBarCodeToolSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgIdReaderBarCodeToolSetting.h"
#include "afxdialogex.h"
#include "DlgSearch.h"

// CDlgIdReaderBarCodeToolSetting 对话框



IMPLEMENT_DYNAMIC(CDlgIdReaderBarCodeToolSetting, CDialogEx)
CDlgIdReaderBarCodeToolSetting::CDlgIdReaderBarCodeToolSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgIdReaderBarCodeToolSetting::IDD, pParent)
	, m_nDecodeMethod(0)
	, m_nAppMode(0)
	, m_bCodaBarFlag(TRUE)
	, m_bCode128Flag(TRUE)
	, m_bCode39Flag(TRUE)
	, m_bCode93Flag(TRUE)
	, m_bCodeEANFlag(TRUE)
	, m_bCodeITF25Flag(TRUE)
	, m_nMinSizeWidth(30)
	, m_nMaxSizeWidth(2400)
	, m_nRegionNum(1)
	, m_nSampleLevel(1)
	, m_nMinSizeHeight(10)
	, m_nMaxSizeHeight(2000)
	, m_nStaticAreaWidth(30)
	, m_nWindowSize(4)
	, m_nElemMinWidth(1)
	, m_nElemMaxWidth(1000)
	, m_nWaitingTime(500)
	, m_nBarcodeBalanceLevel(5)
	, m_bErrorDetectFlag(1)
	, m_bFrameCheckOut(0)
	, m_nImageMorph(0)
	, m_nNumScanlines(20)
	, m_nPromodeDistortion(0)
	, m_nPromodeSpot(0)
	, m_nPromodeWhiteGap(0)
	, m_nSavePicLevel(1)
	, m_nSegmentationMethod(1)
	, m_nBoxDisplayMode(1)
	, m_nImageChannel(0)
{
	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}

CDlgIdReaderBarCodeToolSetting::~CDlgIdReaderBarCodeToolSetting()
{
}

void CDlgIdReaderBarCodeToolSetting::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_VSB, m_nDecodeMethod);
	DDX_Radio(pDX, IDC_RADIO_APP_MODE_NORMAL, m_nAppMode);
	DDX_Check(pDX, IDC_CHECK_CODE_BAR, m_bCodaBarFlag);
	DDX_Check(pDX, IDC_CHECK_CODE128, m_bCode128Flag);
	DDX_Check(pDX, IDC_CHECK_CODE39, m_bCode39Flag);
	DDX_Check(pDX, IDC_CHECK_CODE93, m_bCode93Flag);
	DDX_Check(pDX, IDC_CHECK_CODE_EAN, m_bCodeEANFlag);
	DDX_Check(pDX, IDC_CHECK_CODE_ITF25, m_bCodeITF25Flag);
	DDX_Text(pDX, IDC_EDIT_SIZEMINWIDTH, m_nMinSizeWidth);
	
	DDX_Text(pDX, IDC_EDIT_SIZEMAXWIDTH, m_nMaxSizeWidth);
	
	DDX_Text(pDX, IDC_EDIT_MAXCODENUM, m_nRegionNum);
	
	DDX_Text(pDX, IDC_EDIT_SIZEMINHEIGHT, m_nMinSizeHeight);
	
	DDX_Text(pDX, IDC_EDIT_SIZEMAXHEIGHT, m_nMaxSizeHeight);
	
	DDX_Text(pDX, IDC_EDIT_SAMPLELEVEL, m_nSampleLevel);
		
	DDX_Text(pDX, IDC_EDIT_STATICAREA_WIDTH, m_nStaticAreaWidth);
	
	DDX_Text(pDX, IDC_EDIT_WINDOWSIZE, m_nWindowSize);
	
	DDX_Text(pDX, IDC_EDIT_ELEMMINWIDTH, m_nElemMinWidth);
	
	DDX_Text(pDX, IDC_EDIT_ELEMMAXWIDTH, m_nElemMaxWidth);
	
	DDX_Text(pDX, IDC_EDIT_WAITTIME, m_nWaitingTime);
	
	DDX_Text(pDX, IDC_EDIT_BARCODEBALANCELEVEL, m_nBarcodeBalanceLevel);
	
	DDX_Text(pDX, IDC_EDIT_ERRORDETECTFLAG, m_bErrorDetectFlag);
	
	DDX_Text(pDX, IDC_EDIT_FRAMECHECKOUT, m_bFrameCheckOut);
	
	DDX_Text(pDX, IDC_EDIT_IMAGEMORPH, m_nImageMorph);
	
	DDX_Text(pDX, IDC_EDIT_NUMSCANLINES, m_nNumScanlines);
	
	DDX_Text(pDX, IDC_EDIT_PROMODEDISTORTION, m_nPromodeDistortion);
	
	DDX_Text(pDX, IDC_EDIT_PROMODESPOT, m_nPromodeSpot);
	
	DDX_Text(pDX, IDC_EDIT_PROMODEWHITEGAP, m_nPromodeWhiteGap);
	
	DDX_Text(pDX, IDC_EDIT_SAVEPICLEVEL, m_nSavePicLevel);
	
	DDX_Radio(pDX, IDC_RADIO_SEGMENTATIONMETHOD_LOCAL, m_nSegmentationMethod);
	DDX_Radio(pDX, IDC_RADIO_RAW, m_nBoxDisplayMode);

	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgIdReaderBarCodeToolSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	ON_BN_CLICKED(IDC_RADIO_VSB, &CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioVsb)
	ON_BN_CLICKED(IDC_RADIO_VSC, &CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioVsb)
	ON_BN_CLICKED(IDC_RADIO_APP_MODE_NORMAL, &CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioAppModeNormal)
	ON_BN_CLICKED(IDC_RADIO_APP_MODE_PRO, &CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioAppModeNormal)
	ON_BN_CLICKED(IDC_CHECK_CODE128, &CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCode128)
	ON_BN_CLICKED(IDC_CHECK_CODE39, &CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCode39)
	ON_BN_CLICKED(IDC_CHECK_CODE93, &CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCode93)
	ON_BN_CLICKED(IDC_CHECK_CODE_BAR, &CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCodeBar)
	ON_BN_CLICKED(IDC_CHECK_CODE_EAN, &CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCodeEan)
	ON_BN_CLICKED(IDC_CHECK_CODE_ITF25, &CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCodeItf25)
	ON_EN_SETFOCUS(IDC_EDIT_SIZEMINWIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSizeminwidth)
	ON_EN_KILLFOCUS(IDC_EDIT_SIZEMINWIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSizeminwidth)
	ON_EN_SETFOCUS(IDC_EDIT_SIZEMAXWIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSizemaxwidth)
	ON_EN_KILLFOCUS(IDC_EDIT_SIZEMAXWIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSizemaxwidth)
	ON_EN_SETFOCUS(IDC_EDIT_MAXCODENUM, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditMaxcodenum)
	ON_EN_KILLFOCUS(IDC_EDIT_MAXCODENUM, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditMaxcodenum)
	ON_EN_SETFOCUS(IDC_EDIT_NUMSCANLINES, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditNumscanlines)
	ON_EN_KILLFOCUS(IDC_EDIT_NUMSCANLINES, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditNumscanlines)
	ON_EN_SETFOCUS(IDC_EDIT_SAMPLELEVEL, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSamplelevel)
	ON_EN_KILLFOCUS(IDC_EDIT_SAMPLELEVEL, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSamplelevel)
	ON_EN_SETFOCUS(IDC_EDIT_SIZEMINHEIGHT, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSizeminheight)
	ON_EN_KILLFOCUS(IDC_EDIT_SIZEMINHEIGHT, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSizeminheight)
	ON_EN_SETFOCUS(IDC_EDIT_SIZEMAXHEIGHT, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSizemaxheight)
	ON_EN_KILLFOCUS(IDC_EDIT_SIZEMAXHEIGHT, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSizemaxheight)
	ON_EN_SETFOCUS(IDC_EDIT_STATICAREA_WIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditStaticareaWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_STATICAREA_WIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditStaticareaWidth)
	ON_EN_SETFOCUS(IDC_EDIT_WINDOWSIZE, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditWindowsize)
	ON_EN_KILLFOCUS(IDC_EDIT_WINDOWSIZE, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditWindowsize)
	ON_EN_SETFOCUS(IDC_EDIT_WAITTIME, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditWaittime)
	ON_EN_KILLFOCUS(IDC_EDIT_WAITTIME, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditWaittime)
	ON_EN_SETFOCUS(IDC_EDIT_ELEMMINWIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditElemminwidth)
	ON_EN_KILLFOCUS(IDC_EDIT_ELEMMINWIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditElemminwidth)
	ON_EN_SETFOCUS(IDC_EDIT_ELEMMAXWIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditElemmaxwidth)
	ON_EN_KILLFOCUS(IDC_EDIT_ELEMMAXWIDTH, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditElemmaxwidth)
	ON_EN_SETFOCUS(IDC_EDIT_ERRORDETECTFLAG, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditErrordetectflag)
	ON_EN_KILLFOCUS(IDC_EDIT_ERRORDETECTFLAG, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditErrordetectflag)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODEBALANCELEVEL, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditBarcodebalancelevel)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODEBALANCELEVEL, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditBarcodebalancelevel)
	ON_EN_SETFOCUS(IDC_EDIT_IMAGEMORPH, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditImagemorph)
	ON_EN_KILLFOCUS(IDC_EDIT_IMAGEMORPH, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditImagemorph)
	ON_EN_SETFOCUS(IDC_EDIT_FRAMECHECKOUT, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditFramecheckout)
	ON_EN_KILLFOCUS(IDC_EDIT_FRAMECHECKOUT, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditFramecheckout)
	ON_EN_SETFOCUS(IDC_EDIT_PROMODEDISTORTION, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditPromodedistortion)
	ON_EN_KILLFOCUS(IDC_EDIT_PROMODEDISTORTION, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditPromodedistortion)
	ON_EN_SETFOCUS(IDC_EDIT_PROMODESPOT, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditPromodespot)
	ON_EN_KILLFOCUS(IDC_EDIT_PROMODESPOT, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditPromodespot)
	ON_EN_SETFOCUS(IDC_EDIT_PROMODEWHITEGAP, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditPromodewhitegap)
	ON_EN_KILLFOCUS(IDC_EDIT_PROMODEWHITEGAP, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditPromodewhitegap)
	ON_EN_SETFOCUS(IDC_EDIT_SAVEPICLEVEL, &CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSavepiclevel)
	ON_EN_KILLFOCUS(IDC_EDIT_SAVEPICLEVEL, &CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSavepiclevel)
	ON_BN_CLICKED(IDC_RADIO_SEGMENTATIONMETHOD_LOCAL, &CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioSegmentationmethodLocal)
	ON_BN_CLICKED(IDC_RADIO_SEGMENTATIONMETHOD_GLOBAL, &CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioSegmentationmethodLocal)
	ON_BN_CLICKED(IDC_RADIO_RAW, &CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioRaw)
	ON_BN_CLICKED(IDC_RADIO_ACCURATE, &CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioRaw)
END_MESSAGE_MAP()


// CDlgIdReaderBarCodeToolSetting 消息处理程序





BOOL CDlgIdReaderBarCodeToolSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	ControEnaleWindow();
	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgIdReaderBarCodeToolSetting::UpdateControlDisplay()
{	
	UpdateData(TRUE);
	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH)->EnableWindow(TRUE);
	}

	if (m_pDlgParent->m_pSearchTool != NULL)
	{
		if (m_pDlgParent->m_imageTrain.IsValid())
		{

			if(m_pDlgParent->m_pSearchTool->GetSearchToolType()== eIdReaderBarCodeTool)
			{
				if (m_pDlgParent->m_imageTrain.IsRGB24() || m_pDlgParent->m_imageTrain.IsRGB32())
				{
					GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_SHOW);
				}
				else
				{
					GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
				}
			}
			else
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
			}

		}
		else
		{
			if(m_pDlgParent->m_pSearchTool->GetSearchToolType()== eIdReaderBarCodeTool)
			{
				cpImage imageTrain;
				m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
				if (imageTrain.IsValid() && 
					(imageTrain.IsRGB24() || imageTrain.IsRGB32()))
				{
					GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_SHOW);
				}
				else
				{
					GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
				}
			}
			else
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
	}

}

void CDlgIdReaderBarCodeToolSetting::UpdateTrainData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		((CIdReaderBarCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;

		Find1DCodePara &find1DCodePara = ((CIdReaderBarCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Find1DCodePara;

		find1DCodePara.m_methodDecode = (DecodeMethod)(m_nDecodeMethod+1);
		find1DCodePara.m_modeApp = (AppMode)m_nAppMode;
		find1DCodePara.m_bCodaBarFlag = m_bCodaBarFlag;
		find1DCodePara.m_bCode128Flag = m_bCode128Flag;
		find1DCodePara.m_bCode39Flag = m_bCode39Flag;
		find1DCodePara.m_bCode93Flag = m_bCode93Flag;
		find1DCodePara.m_bCodeEANFlag = m_bCodeEANFlag;
		find1DCodePara.m_bCodeITF25Flag = m_bCodeITF25Flag;
		find1DCodePara.m_bErrorDetectFlag = m_bErrorDetectFlag > 0;
		find1DCodePara.m_bFrameCheckOut = m_bFrameCheckOut > 0;
		find1DCodePara.m_methodSegmentation = (SegmentationMethod)m_nSegmentationMethod;
		find1DCodePara.m_modeBoxDisplay = (BoxDisplayMode)m_nBoxDisplayMode;
		find1DCodePara.m_nBarcodeBalanceLevel = m_nBarcodeBalanceLevel;
		find1DCodePara.m_nElemMaxWidth = m_nElemMaxWidth;
		find1DCodePara.m_nElemMinWidth = m_nElemMinWidth;
		find1DCodePara.m_nImageMorph = m_nImageMorph > 0;
		find1DCodePara.m_nMaxSizeHeight = m_nMaxSizeHeight;
		find1DCodePara.m_nMinSizeHeight = m_nMinSizeHeight;
		find1DCodePara.m_nMaxSizeWidth = m_nMaxSizeWidth;
		find1DCodePara.m_nMinSizeWidth = m_nMinSizeWidth;
		find1DCodePara.m_nNumScanlines = m_nNumScanlines;
		find1DCodePara.m_nPromodeDistortion = m_nPromodeDistortion;
		find1DCodePara.m_nPromodeSpot = m_nPromodeSpot;
		find1DCodePara.m_nPromodeWhiteGap = m_nPromodeWhiteGap;
		find1DCodePara.m_nRegionNum = m_nRegionNum;
		find1DCodePara.m_nSampleLevel = m_nSampleLevel;
		find1DCodePara.m_nSavePicLevel = m_nSavePicLevel;
		find1DCodePara.m_nStaticAreaWidth = m_nStaticAreaWidth;
		find1DCodePara.m_nWaitingTime = m_nWaitingTime;
		find1DCodePara.m_nWindowSize = m_nWindowSize;
		
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);		
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nImageChannel = ((CIdReaderBarCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;

		Find1DCodePara &find1DCodePara = (((CIdReaderBarCodeTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Find1DCodePara);

		m_nDecodeMethod = ((int)find1DCodePara.m_methodDecode)-1;
		m_nAppMode = (int)find1DCodePara.m_modeApp;
		m_bCodaBarFlag = find1DCodePara.m_bCodaBarFlag;
		m_bCode128Flag = find1DCodePara.m_bCode128Flag;
		m_bCode39Flag = find1DCodePara.m_bCode39Flag;
		m_bCode93Flag = find1DCodePara.m_bCode93Flag;
		m_bCodeEANFlag = find1DCodePara.m_bCodeEANFlag;
		m_bCodeITF25Flag = find1DCodePara.m_bCodeITF25Flag;
		m_bErrorDetectFlag = (int)find1DCodePara.m_bErrorDetectFlag;
		m_bFrameCheckOut = (int)find1DCodePara.m_bFrameCheckOut;
		m_nSegmentationMethod = (int)find1DCodePara.m_methodSegmentation;
		m_nBoxDisplayMode = (int)find1DCodePara.m_modeBoxDisplay;
		m_nBarcodeBalanceLevel = find1DCodePara.m_nBarcodeBalanceLevel;
		m_nElemMaxWidth = find1DCodePara.m_nElemMaxWidth;
		m_nElemMinWidth = find1DCodePara.m_nElemMinWidth;
		m_nImageMorph = (int)find1DCodePara.m_nImageMorph;
		m_nMaxSizeHeight = find1DCodePara.m_nMaxSizeHeight;
		m_nMinSizeHeight = find1DCodePara.m_nMinSizeHeight;
		m_nMaxSizeWidth = find1DCodePara.m_nMaxSizeWidth;
		m_nMinSizeWidth = find1DCodePara.m_nMinSizeWidth;
		m_nNumScanlines = find1DCodePara.m_nNumScanlines;
		m_nPromodeDistortion = find1DCodePara.m_nPromodeDistortion;
		m_nPromodeSpot = find1DCodePara.m_nPromodeSpot;
		m_nPromodeWhiteGap = find1DCodePara.m_nPromodeWhiteGap;
		m_nRegionNum = find1DCodePara.m_nRegionNum;
		m_nSampleLevel = find1DCodePara.m_nSampleLevel;
		m_nSavePicLevel = find1DCodePara.m_nSavePicLevel;
		m_nStaticAreaWidth = find1DCodePara.m_nStaticAreaWidth;
		m_nWaitingTime = find1DCodePara.m_nWaitingTime;
		m_nWindowSize = find1DCodePara.m_nWindowSize;

		UpdateData(FALSE);
	}

}

void CDlgIdReaderBarCodeToolSetting::UpdateSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

	if (bValidate) // 从界面获取搜索参数
	{
		UpdateData(TRUE);
		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
		UpdateData(FALSE);
	}
}

void CDlgIdReaderBarCodeToolSetting::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir = m_pDlgParent->m_strCurWorkDir;

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

		if(_T("IDD_DLG_CORNERSEARCH_TOOL") == strDlgID)
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

void CDlgIdReaderBarCodeToolSetting::OnBtnGetTrainImage() 
{
	if (!m_pDlgParent->m_imageInput.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_TRAIN_IMAGE_FAILED);	//_T("获取训练图像失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_INPUT_IMAGE);		//_T("输入图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}

	UpdateData(TRUE);
	m_pDlgParent->m_imageTrain = m_pDlgParent->m_imageInput;

	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();
	CString strLog;
	strLog.Format(_T("点击[获取训练图像]\n"));
	g_opLogFile.SaveLogText(strLog,6);
	UpdateControlDisplay();
}

void CDlgIdReaderBarCodeToolSetting::OnBtnTrain() 
{
	// TODO: Add your control notification handler code here
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}
	if (!m_pDlgParent->m_imageTrain.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED);		//_T("训练失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_TRAIN_IMAGE);	//_T("训练图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}

	CheckDlgDataValidation();
	UpdateTrainData(true);

	m_pDlgParent->m_pSearchTool->SetTrainImage(m_pDlgParent->m_imageTrain);


	if (m_pDlgParent->m_pSearchTool->Train())
	{
		cpImage trainImage;
		m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else
	{
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED));
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
	CString strLog;
	strLog.Format(_T("点击[训练]\n"));
	g_opLogFile.SaveLogText(strLog,6);
}

void CDlgIdReaderBarCodeToolSetting::ControEnaleWindow()
{
	UpdateData(TRUE);
	if (m_nDecodeMethod==0)
	{
		GetDlgItem(IDC_STATIC_SAMPLELEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SAMPLELEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_STATICAREA_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STATICAREA_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_WINDOWSIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WINDOWSIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_WAITTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WAITTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ERRORDETECTFLAG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ERRORDETECTFLAG)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_IMAGEMORPH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IMAGEMORPH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PROMODEDISTORTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROMODEDISTORTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PROMODESPOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROMODESPOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PROMODEWHITEGAP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROMODEWHITEGAP)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SAVEPICLEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SAVEPICLEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MS)->EnableWindow(TRUE);

		GetDlgItem(IDC_STATIC_NUMSCANLINES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUMSCANLINES)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SIZEMINHEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SIZEMINHEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SIZEMAXHEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SIZEMAXHEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ELEMMINWIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ELEMMINWIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ELEMMAXWIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ELEMMAXWIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_BARCODEBALANCELEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BARCODEBALANCELEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_FRAMECHECKOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FRAMECHECKOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SEGMEATIONGROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SEGMENTATIONMETHOD_LOCAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SEGMENTATIONMETHOD_GLOBAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_BOXDISPLAYGROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RAW)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ACCURATE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_SAMPLELEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SAMPLELEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STATICAREA_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STATICAREA_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_WINDOWSIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WINDOWSIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_WAITTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WAITTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ERRORDETECTFLAG)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ERRORDETECTFLAG)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_IMAGEMORPH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IMAGEMORPH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PROMODEDISTORTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROMODEDISTORTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PROMODESPOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROMODESPOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PROMODEWHITEGAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROMODEWHITEGAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SAVEPICLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SAVEPICLEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MS)->EnableWindow(FALSE);

		GetDlgItem(IDC_STATIC_NUMSCANLINES)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_NUMSCANLINES)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SIZEMINHEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SIZEMINHEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SIZEMAXHEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SIZEMAXHEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ELEMMINWIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ELEMMINWIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ELEMMAXWIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ELEMMAXWIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_BARCODEBALANCELEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BARCODEBALANCELEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_FRAMECHECKOUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FRAMECHECKOUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SEGMEATIONGROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SEGMENTATIONMETHOD_LOCAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SEGMENTATIONMETHOD_GLOBAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_BOXDISPLAYGROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RAW)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_ACCURATE)->EnableWindow(TRUE);
	}
}


void CDlgIdReaderBarCodeToolSetting::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	if(m_nMinSizeWidth<1 || m_nMinSizeWidth>4000)
	{
		m_nMinSizeWidth = 30;
	}

	if(m_nMaxSizeWidth<1 || m_nMaxSizeWidth>4000)
	{
		m_nMaxSizeWidth = 2400;
	}
	
	if(m_nRegionNum<1 || m_nRegionNum>100)
	{
		m_nRegionNum = 1;
	}
	
	if(m_nMinSizeHeight<1 || m_nMinSizeHeight>4000)
	{
		m_nMinSizeHeight = 10;
	}
	if(m_nMaxSizeHeight<1 || m_nMaxSizeHeight>4000)
	{
		m_nMaxSizeHeight = 2000;
	}

	if(m_nSampleLevel<1 || m_nSampleLevel>8)
	{
		m_nSampleLevel = 2000;
	}
	
	if(m_nStaticAreaWidth<0 || m_nStaticAreaWidth>300)
	{
		m_nStaticAreaWidth = 30;
	}
	
	if(m_nWindowSize<4 || m_nWindowSize>65)
	{
		m_nWindowSize = 4;
	}
	
	if(m_nElemMaxWidth<1 || m_nElemMaxWidth>1000)
	{
		m_nElemMaxWidth = 1000;
	}

	if(m_nElemMinWidth<1 || m_nElemMinWidth>1000)
	{
		m_nElemMinWidth = 1;
	}

	if(m_nWaitingTime<0 || m_nWaitingTime>5000)
	{
		m_nWaitingTime = 500;
	}
	
	if(m_nBarcodeBalanceLevel<1 || m_nBarcodeBalanceLevel>10)
	{
		m_nBarcodeBalanceLevel = 5;
	}
	
	if(m_bErrorDetectFlag<0 || m_bErrorDetectFlag>1)
	{
		m_bErrorDetectFlag = 1;
	}
	
	if(m_bFrameCheckOut<0 || m_bFrameCheckOut>1)
	{
		m_bFrameCheckOut = 0;
	}
	if(m_nImageMorph<0 || m_nImageMorph>2)
	{
		m_nImageMorph = 0;
	}
	
	if(m_nNumScanlines<3 || m_nNumScanlines>50)
	{
		m_nNumScanlines = 20;
	}

	if(m_nPromodeDistortion<0 || m_nPromodeDistortion>1)
	{
		m_nPromodeDistortion = 0;
	}
	
	if(m_nPromodeSpot<0 || m_nPromodeSpot>1)
	{
		m_nPromodeSpot = 0;
	}

	if(m_nPromodeWhiteGap<0 || m_nPromodeWhiteGap>1)
	{
		m_nPromodeWhiteGap = 0;
	}
	if(m_nSavePicLevel<1 || m_nSavePicLevel>3)
	{
		m_nSavePicLevel = 1;
	}
	

	UpdateData(FALSE);
}

void CDlgIdReaderBarCodeToolSetting::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		if (m_pDlgParent->m_pSearchTool->GetSearchToolType()==eIdReaderDmCodeTool)
		{

			CIdReaderBarCodeTrainGuiParam trainGuiParam;
			m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
			trainGuiParam.m_TrainRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
			m_pDlgParent->UpdateGuiDisplay();
		}
	}
}

BOOL CDlgIdReaderBarCodeToolSetting::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioVsb()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nDecodeMethod;

	ControEnaleWindow();
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioAppModeNormal()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nAppMode;
	UpdateData();
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCode128()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCode128Flag = !m_bCode128Flag;
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCode39()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCode39Flag = !m_bCode39Flag;
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCode93()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCode93Flag = !m_bCode93Flag;
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCodeBar()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCodaBarFlag = !m_bCodaBarFlag;
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCodeEan()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCodeEANFlag = !m_bCodeEANFlag;
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedCheckCodeItf25()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCodeITF25Flag = !m_bCodeITF25Flag;
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSizeminwidth()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SIZEMINWIDTH, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_SIZEMINWIDTH)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_SIZEMINWIDTH, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSizeminwidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_SIZEMINWIDTH, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSizemaxwidth()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SIZEMAXWIDTH, m_strInput);

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_SIZEMAXWIDTH)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_SIZEMAXWIDTH, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSizemaxwidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_SIZEMAXWIDTH, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditMaxcodenum()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_MAXCODENUM, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_MAXCODENUM)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_MAXCODENUM, m_strOutput);
		}
	}

}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditMaxcodenum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_SIZEMINWIDTH, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditNumscanlines()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_NUMSCANLINES, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_NUMSCANLINES)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_NUMSCANLINES, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditNumscanlines()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_NUMSCANLINES, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSamplelevel()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItemText(IDC_EDIT_SAMPLELEVEL, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_SAMPLELEVEL)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_SAMPLELEVEL, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSamplelevel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_SAMPLELEVEL, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSizeminheight()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SIZEMINHEIGHT, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_SIZEMINHEIGHT)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_SIZEMINHEIGHT, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSizeminheight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_SIZEMINHEIGHT, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSizemaxheight()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SIZEMAXHEIGHT, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_SIZEMAXHEIGHT)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_SIZEMAXHEIGHT, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSizemaxheight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_SIZEMINWIDTH, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditStaticareaWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_STATICAREA_WIDTH, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_STATICAREA_WIDTH)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_STATICAREA_WIDTH, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditStaticareaWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_STATICAREA_WIDTH, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditWindowsize()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_WINDOWSIZE, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_WINDOWSIZE)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_WINDOWSIZE, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditWindowsize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_WINDOWSIZE, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditWaittime()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_WAITTIME, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_WAITTIME)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_WAITTIME, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditWaittime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_WAITTIME, m_strOutput);
	}
}

void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditElemminwidth()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItemText(IDC_EDIT_ELEMMINWIDTH, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_ELEMMINWIDTH)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_ELEMMINWIDTH, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditElemminwidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_ELEMMINWIDTH, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditElemmaxwidth()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_ELEMMAXWIDTH, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_ELEMMAXWIDTH)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_ELEMMAXWIDTH, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditElemmaxwidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_ELEMMINWIDTH, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditErrordetectflag()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_ERRORDETECTFLAG, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_ERRORDETECTFLAG)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_ERRORDETECTFLAG, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditErrordetectflag()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_ERRORDETECTFLAG, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditBarcodebalancelevel()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_BARCODEBALANCELEVEL, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_BARCODEBALANCELEVEL)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_BARCODEBALANCELEVEL, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditBarcodebalancelevel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_BARCODEBALANCELEVEL, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditImagemorph()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_IMAGEMORPH, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_IMAGEMORPH)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_IMAGEMORPH, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditImagemorph()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_IMAGEMORPH, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditFramecheckout()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_FRAMECHECKOUT, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_FRAMECHECKOUT)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_FRAMECHECKOUT, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditFramecheckout()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_FRAMECHECKOUT, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditPromodedistortion()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_PROMODEDISTORTION, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_PROMODEDISTORTION)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_PROMODEDISTORTION, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditPromodedistortion()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_PROMODEDISTORTION, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditPromodespot()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_PROMODESPOT, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_PROMODESPOT)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_PROMODESPOT, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditPromodespot()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_PROMODESPOT, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditPromodewhitegap()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_PROMODEWHITEGAP, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_PROMODEWHITEGAP)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_PROMODEWHITEGAP, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditPromodewhitegap()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_PROMODEWHITEGAP, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnSetfocusEditSavepiclevel()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SAVEPICLEVEL, m_strInput);
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		GetDlgItem(IDC_EDIT_SAVEPICLEVEL)->GetWindowRect(&rcWnd);
		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, m_strInput, m_strOutput))
		{
			SetDlgItemText(IDC_EDIT_SAVEPICLEVEL, m_strOutput);
		}
	}
}


void CDlgIdReaderBarCodeToolSetting::OnEnKillfocusEditSavepiclevel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{
		GetDlgItemText(IDC_EDIT_SAVEPICLEVEL, m_strOutput);
	}
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioSegmentationmethodLocal()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nSegmentationMethod;
	UpdateData();
}


void CDlgIdReaderBarCodeToolSetting::OnBnClickedRadioRaw()
{
	// TODO: 在此添加控件通知处理程序代码
	int nOld = m_nBoxDisplayMode;
	UpdateData();
}
