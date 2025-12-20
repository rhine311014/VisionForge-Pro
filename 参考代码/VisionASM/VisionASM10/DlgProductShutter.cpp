// DlgProductShutter.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgProductShutter.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "LogFileSave.h"

// CDlgProductShutter 对话框

IMPLEMENT_DYNAMIC(CDlgProductShutter, CDialog)

CDlgProductShutter::CDlgProductShutter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProductShutter::IDD, pParent)
{
	m_pParent = (CDlgSearch*)pParent;

	//m_dProductShutter = 0;
	//m_dEnvironmentShutter = 0;
}

CDlgProductShutter::~CDlgProductShutter()
{
}

void CDlgProductShutter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_PRODUCT_SHUTTER, m_sliderProductShutter);
	DDX_Control(pDX, IDC_SLIDER_GAIN, m_sliderGain);
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_sliderGamma);
}


BEGIN_MESSAGE_MAP(CDlgProductShutter, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_RESET, &CDlgProductShutter::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// CDlgProductShutter 消息处理程序


BOOL CDlgProductShutter::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (m_pParent != NULL)
	{
		// 更新曝光显示		
// 		double dCurrent1 = 0, dMin1 = 0, dMax1 = 0;
// 		m_pParent->m_pVisionASM->GetPosCameraShutter(m_pParent->m_nCurPositionIdx, dCurrent1, dMin1, dMax1);

		CString strVal;
		double dCurrent = 0, dMin = 0, dMax = 0;
		if (m_pParent->m_pVisionASM->GetPosCameraShutter(m_pParent->m_nCurPositionIdx, dCurrent, dMin, dMax))
		{
			m_sliderProductShutter.EnableWindow(m_bEnableSearchShutter.m_bShutterEnable);
			m_sliderProductShutter.ShowWindow(m_bEnableSearchShutter.m_bShutterEnable);
			
			GetDlgItem(IDC_STATIC_SHUTTER)->EnableWindow(m_bEnableSearchShutter.m_bShutterEnable);
			GetDlgItem(IDC_STATIC_SHUTTER)->ShowWindow(m_bEnableSearchShutter.m_bShutterEnable);
			GetDlgItem(IDC_STATIC_SHUTTER_VAL)->EnableWindow(m_bEnableSearchShutter.m_bShutterEnable);
			GetDlgItem(IDC_STATIC_SHUTTER_VAL)->ShowWindow(m_bEnableSearchShutter.m_bShutterEnable);

			m_sliderProductShutter.SetRange(ceil(dMin/0.01), floor(dMax/0.01), TRUE);
			m_sliderProductShutter.SetPos( (int) ((dCurrent+0.005) / 0.01) );
			strVal.Format(_T("%.2fms"), dCurrent);
			GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strVal);
			
			m_dProductShutter.m_dShutter = dCurrent;
		}
		else
		{
			m_sliderProductShutter.EnableWindow(FALSE);

		}

		if (m_pParent->m_pVisionASM->GetPosCameraGain(m_pParent->m_nCurPositionIdx, dCurrent, dMin, dMax))
		{
			m_sliderGain.EnableWindow(m_bEnableSearchShutter.m_bGainEnable);
			m_sliderGain.ShowWindow(m_bEnableSearchShutter.m_bGainEnable);

			GetDlgItem(IDC_STATIC_GAIN)->EnableWindow(m_bEnableSearchShutter.m_bGainEnable);
			GetDlgItem(IDC_STATIC_GAIN)->ShowWindow(m_bEnableSearchShutter.m_bGainEnable);
			GetDlgItem(IDC_STATIC_GAIN_VAL)->EnableWindow(m_bEnableSearchShutter.m_bGainEnable);
			GetDlgItem(IDC_STATIC_GAIN_VAL)->ShowWindow(m_bEnableSearchShutter.m_bGainEnable);

			m_sliderGain.SetRange(ceil(dMin/0.01), floor(dMax/0.01), TRUE);
			m_sliderGain.SetPos( (int) ((dCurrent+0.005) / 0.01) );
			strVal.Format(_T("%.2fms"), dCurrent);
			GetDlgItem(IDC_STATIC_GAIN_VAL)->SetWindowText(strVal);
			
			m_dProductShutter.m_dGain = dCurrent;
		}
		else
		{
			m_sliderGain.EnableWindow(FALSE);
		}

		ImageRoi roi;
		if (m_pParent->m_pVisionASM->GetPosImageROIParam(m_pParent->m_nCurPositionIdx, roi))
		{
			dCurrent = 1.0;
			dMin = 0.0;
			dMax = 10.0;

			dCurrent = roi.m_dGamma;

			m_sliderGamma.EnableWindow(m_bEnableSearchShutter.m_bGammaEnable && roi.m_bGammaCorrectionEnable);
			m_sliderGamma.ShowWindow(m_bEnableSearchShutter.m_bGammaEnable && roi.m_bGammaCorrectionEnable);
			GetDlgItem(IDC_STATIC_GAMMA_VAL)->EnableWindow(m_bEnableSearchShutter.m_bGammaEnable && roi.m_bGammaCorrectionEnable);
			GetDlgItem(IDC_STATIC_GAMMA_VAL)->ShowWindow(m_bEnableSearchShutter.m_bGammaEnable && roi.m_bGammaCorrectionEnable);
			GetDlgItem(IDC_STATIC_GAMMA)->EnableWindow(m_bEnableSearchShutter.m_bGammaEnable && roi.m_bGammaCorrectionEnable);
			GetDlgItem(IDC_STATIC_GAMMA)->ShowWindow(m_bEnableSearchShutter.m_bGammaEnable && roi.m_bGammaCorrectionEnable);

			// 暂时禁用拍照对位切换Gamma
			m_sliderGamma.EnableWindow(FALSE);
			m_sliderGamma.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_GAMMA_VAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_GAMMA_VAL)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_GAMMA)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_GAMMA)->ShowWindow(FALSE);

			m_sliderGamma.SetRange(ceil(dMin/0.1), floor(dMax/0.1), TRUE);
			m_sliderGamma.SetPos( (int) ((dCurrent+0.05) / 0.1) );
			strVal.Format(_T("%.2f"), dCurrent);
			GetDlgItem(IDC_STATIC_GAMMA_VAL)->SetWindowText(strVal);

			m_dProductShutter.m_dGamma = dCurrent;
		}
		else
		{
			m_sliderGamma.EnableWindow(FALSE);
			m_sliderGamma.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_GAMMA_VAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_GAMMA_VAL)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_GAMMA)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_GAMMA)->ShowWindow(FALSE);
		}
		

	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgProductShutter::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString strVal;
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_PRODUCT_SHUTTER:
		{
			if (m_pParent != NULL)
			{
				// 设置相机的曝光时间
				m_dProductShutter.m_dShutter = (double)(m_sliderProductShutter.GetPos())*0.01;
				strVal.Format(_T("%.2fms"), m_dProductShutter.m_dShutter);
				GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strVal);
				m_pParent->m_pVisionASM->SetPosCameraShutter(m_pParent->m_nCurPositionIdx, m_dProductShutter.m_dShutter);
				m_pParent->m_pVisionASM->UpdatePosCameraShutter(m_pParent->m_nCurPositionIdx);
				if (m_dProductShutterOld.m_dShutter != m_dProductShutter.m_dShutter)
				{
					CString strLog;
					strLog.Format(_T("修改位置%d[曝光]由%lf到%lf"),m_pParent->m_nCurPositionIdx,m_dProductShutterOld.m_dShutter,m_dProductShutter.m_dShutter);
					g_opLogFile.SaveLogText(strLog,6);
				}
				m_dProductShutterOld.m_dShutter = m_dProductShutter.m_dShutter;
			}
		}
		break;
	case IDC_SLIDER_GAIN:
		{
			// 设置相机的增益
			m_dProductShutter.m_dGain = (double)(m_sliderGain.GetPos())*0.01;
			strVal.Format(_T("%.2fdb"), m_dProductShutter.m_dGain);
			GetDlgItem(IDC_STATIC_GAIN_VAL)->SetWindowText(strVal);
			m_pParent->m_pVisionASM->SetPosCameraGain(m_pParent->m_nCurPositionIdx, m_dProductShutter.m_dGain);
			m_pParent->m_pVisionASM->UpdatePosCameraGain(m_pParent->m_nCurPositionIdx);
			if (m_dProductShutterOld.m_dGain != m_dProductShutter.m_dGain)
			{
				CString strLog;
				strLog.Format(_T("修改位置%d[增益]由%lf到%lf"),m_pParent->m_nCurPositionIdx,m_dProductShutterOld.m_dGain,m_dProductShutter.m_dGain);
				g_opLogFile.SaveLogText(strLog,3);
			}
			m_dProductShutterOld.m_dGain = m_dProductShutter.m_dGain;
		}
		break;
	case IDC_SLIDER_GAMMA:
		{

			{
				// 设置相机的Gamma
				m_dProductShutter.m_dGamma = (double)(m_sliderGamma.GetPos())*0.1;
				strVal.Format(_T("%.2f"), m_dProductShutter.m_dGamma);
				GetDlgItem(IDC_STATIC_GAMMA_VAL)->SetWindowText(strVal);
				ImageRoi roi;
				m_pParent->m_pVisionASM->GetPosImageROIParam(m_pParent->m_nCurPositionIdx, roi);
				roi.m_dGamma = m_dProductShutter.m_dGamma;
				m_pParent->m_pVisionASM->SetPosImageROIParam(m_pParent->m_nCurPositionIdx, roi);
				//m_pParent->m_pVisionASM->SetImageROIParam(m_pParent->m_nCurPositionIdx, roi);
				if (m_dProductShutterOld.m_dGamma != m_dProductShutter.m_dGamma)
				{
					CString strLog;
					strLog.Format(_T("修改位置%d[Gamma]由%lf到%lf"),m_pParent->m_nCurPositionIdx,m_dProductShutterOld.m_dGamma,m_dProductShutter.m_dGamma);
					g_opLogFile.SaveLogText(strLog,3);
				}
				m_dProductShutterOld.m_dGamma = m_dProductShutter.m_dGamma;
			}
		}
		break;
	default:
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlgProductShutter::OnBnClickedBtnReset()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strVal;
	m_sliderProductShutter.SetPos( (int) ((m_dEnvironmentShutter.m_dShutter+0.005) / 0.01) );
	strVal.Format(_T("%.2fms"), m_dEnvironmentShutter.m_dShutter);
	GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strVal);


	m_pParent->m_pVisionASM->SetPosCameraShutter(m_pParent->m_nCurPositionIdx, m_dEnvironmentShutter.m_dShutter);
	m_pParent->m_pVisionASM->UpdatePosCameraShutter(m_pParent->m_nCurPositionIdx);

	m_pParent->m_pVisionASM->SetPosCameraGain(m_pParent->m_nCurPositionIdx, m_dEnvironmentShutter.m_dGain);
	m_pParent->m_pVisionASM->UpdatePosCameraGain(m_pParent->m_nCurPositionIdx);

	ImageRoi roi;
	m_pParent->m_pVisionASM->GetPosImageROIParam(m_pParent->m_nCurPositionIdx, roi);
	roi.m_dGamma = m_dEnvironmentShutter.m_dGamma;
	m_pParent->m_pVisionASM->SetPosImageROIParam(m_pParent->m_nCurPositionIdx, roi);

	m_dProductShutter = m_dEnvironmentShutter;
}
