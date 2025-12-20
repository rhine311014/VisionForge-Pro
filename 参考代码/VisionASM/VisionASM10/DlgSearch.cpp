// DlgSearch.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSearch.h"
#include "DlgProductShutter.h"
#include "svDIB.h"

#include <IO.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSearch dialog

CDlgSearch::CDlgSearch( CWnd* pParent /*= NULL*/, 
						CBaseSearchTool* pDefSearchTool /*= NULL*/ )
: CDialog(CDlgSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSearch)
	m_nDisplayType = 0;
	//}}AFX_DATA_INIT
	m_pSearchTool = NULL;
	m_pTrainDataParam = NULL;
	m_pTrainGuiParam = NULL;
	m_pSearchDataParam = NULL;
	m_pSearchGuiParam = NULL;
	m_nCurSelSearchToolIdx = -1;
	m_bTrainDataUpdated	= FALSE;
	m_bSearchDataUpdated= FALSE;
	m_pDefSearchTool = pDefSearchTool;
	m_pVisionASM = NULL;

	m_psaSysInfoStrings = NULL;
	m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);

	m_pStatusBar = NULL;
	m_bStatusBarFlag = TRUE;
	m_hIconSucceed = NULL;
	m_hIconFailed = NULL;

	m_pVAPrevMainWnd = NULL;
	m_pPrevGuiDisplay = NULL;
	m_nCurPositionIdx = 0;

	m_strTitleName = _T("定位模板设置");
	
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_strCurWorkDir = szPath;
	m_strCurWorkDir = m_strCurWorkDir.Left(m_strCurWorkDir.ReverseFind('\\'));

	m_nTestImage1Index = -1;
	m_nTestImage1total = 0;
	m_nTestImageIndexStatus = 0;

	//m_bEnableSearchShutter = FALSE;
	//m_dProductSearchShutter = 0;
	//m_dEnvironmentShutter = 0;

	m_pDlgQuickSearchTool					= new CDlgQuickSearchTool;
	m_pDlgPatternSearchTool					= new CDlgPatternSearchTool;
	m_pDlgLineSearchTool						= new CDlgLineSearchTool;
	m_pDlgCornerSearchTool					= new CDlgCornerSearchTool;
	m_pDlgCircleSearchTool					= new CDlgCircleSearchTool;
	m_pDlgCrossSearchTool					= new CDlgCrossSearchTool;
	m_pDlgPatCornerSearchTool				= new CDlgPatCornerSearchTool;
	m_pDlgProCircleSearchTool				= new CDlgProCircleSearchTool;
	m_pDlgFixedPosSearchTool					= new CDlgFixedPosSearchTool;
	m_pDlgPatternCornerSearchTool			= new CDlgPatternCornerSearchTool;
	m_pDlgCombinedQuickSearchTool			= new CDlgCombinedQuickSearchTool;
	m_pDlgCombinedPatternSearchTool			= new CDlgCombinedPatternSearchTool;
	m_pDlgBaseCornerSearchTool				= new CDlgBaseCornerSearchTool;
	m_pDlgPatternSearchCheckTool				= new CDlgPatternSearchCheckTool;
	m_pDlgQuickSearchCheckTool				= new CDlgQuickSearchCheckTool;
	m_pDlgQuickSearchObroundTool				= new CDlgQuickSearchObroundTool;
	m_pDlgPatternSearchObroundTool			= new CDlgPatternSearchObroundTool;
	m_pDlgMPatternSearchTool					= new CDlgMPatternSearchTool;
	m_pDlgMQuickSearchTool					= new CDlgMQuickSearchTool;
	m_pDlgMQuickCornerSearchTool				= new CDlgMQuickCornerSearchTool;
	m_pDlgMQuickPickCornerSearchTool		= new CDlgMQuickPickCornerSearchTool;
	m_pDlgQuickSearchGenRectTool				= new CDlgQuickSearchGenRectTool;
	m_pDlgPatternSearchGenRectTool			= new CDlgPatternSearchGenRectTool;
	m_pDlgPatternLineSearchTool				= new CDlgPatternLineSearchTool;
	m_pDlg2PatternSearchTool					= new CDlg2PatternSearchTool;
	m_pDlgQuickLineCircleSearchTool			= new CDlgQuickLineCircleTool;
	m_pDlgPatternLineCircleSearchTool		= new CDlgPatternLineCircleTool;
	m_pDlgPatternCircleSearchTool			= new CDlgPatternCircleSearchTool;
	m_pDlgQuickSearchCheckLineTool			= new CDlgQuickSearchCheckLineTool;
	m_pDlgPatternSearchCheckLineTool			= new CDlgPatternSearchCheckLineTool;
	m_pDlgPatternBlobSearchTool				= new CDlgPatternBlobSearchTool;
	m_pDlgCornerSearchCheckTool				= new CDlgCornerSearchCheckTool;
	m_pDlgRectSearchTool						= new CDlgRectSearchTool;
	m_pDlgPatternRectSearchTool				= new CDlgPatternRectSearchTool;
	m_pDlgAreaInspectTool					= new CDlgAreaInspect;
	m_pDlgAreaInspectExTool					= new CDlgAreaInspectEx;
	m_pDlgChordSearchTool					= new CDlgChordSearchTool;
	m_pDlgTwoPatModelSearchTool				= new CDlgTwoPatModelSearchTool;
	m_pDlgMPatternCornerSearchTool			= new CDlgMPatternCornerSearchTool;
	m_pDlgMPatternPickCornerSearchTool			= new CDlgMPatternPickCornerSearchTool;
	m_pDlgPatternCornerCircleSearchTool		= new CDlgPatternCornerCircleSearchTool;
	m_pDlgQuickCornerCircleSearchTool		= new CDlgQuickCornerCircleSearchTool;
	m_pDlgPatternPosLineDistanceTool			= new CDlgPatternPosLineDistanceTool;
	m_pDlgCornerSearchCheckInspectTool		= new CDlgCornerSearchCheckInspectTool;
	m_pDlgPatternDoubleSearchTool			= new CDlgPatternDouleSearchTool;
	m_pDlgPatternChordSearchTool				= new CDlgPatternChordSearchTool;
	m_pDlgQuickMulCircleSearchTool			= new CDlgQuickMulCircleSearchTool;
	m_pDlgDmCodeCalibrateTool				= new CDlgDmCodeCalibrateTool;
	m_pDlgPatternCircle4LineSearchTool		= new CDlgPatternCircle4LineSearchTool;
	m_pDlgQuickCircle4LineSearchTool			= new CDlgQuickCircle4LineSearchTool;
	m_pDlgSpecialPickSearchTool				= new CDlgSpecialPickSearchTool;
	m_pDlgPatternCornerLineSearchTool		= new CDlgPatternCornerLineSearchTool;
	m_pDlgPatternMulCircleSearchTool			= new CDlgPatternMulCircleSearchTool;
	m_pDlgPatternCornerMulCircleSearchTool	= new CDlgPatternCornerMulCircleSearchTool;
	m_pDlgPatternCornerMulObroundTool		= new CDlgPatternCornerMulObroundTool;
	m_pDlgQuickCornerMulObroundTool			= new CDlgQuickCornerMulObroundTool;
	m_pDlgQuickPosLineDistanceTool			=new CDlgQuickPosLineDistanceTool ;
	m_pDlgCombinedCornerSearchTool=new  CDlgCombinedCornerSearchTool;
	m_pDlgMQuickPickSearchTool  =new CDlgMQuickPickSearchTool ;
	m_pDlgMPatternPickSearchTool  =new CDlgMPatternPickSearchTool ;
	m_pDlgAreaInspectPickTool  =new CDlgAreaInspectPickTool ;

	
	m_pDlgCircleCornerSearchCheckInspectTool = new CDlgCircleCornerSearchCheckInspectTool;
	m_pDlgACFInspectTool = new CDlgACFInspectTool;


	m_pDlgVirtualRectSearchTool = new CDlgVirtualRectSearchTool;
	m_pDlgPatternVirtualRectSearchTool = new CDlgPatternVirtualRectSearchTool;
	m_pDlgTwoQuickModelSearchTool			= new CDlgTwoQuickModelSearchTool;
	
	m_pDlgIdReaderDmCodeTool                = new CDlgIdReaderDmCodeTool;

	m_pDlgIdReaderBarCodeTool                = new CDlgIdReaderBarCodeToolSetting;

	m_bIsPreviewTool = false;
}

CDlgSearch::~CDlgSearch()
{

	if (m_pDlgVirtualRectSearchTool !=NULL)
	{
		delete m_pDlgVirtualRectSearchTool;
		m_pDlgVirtualRectSearchTool = NULL;
	}

	if (m_pDlgPatternVirtualRectSearchTool !=NULL)
	{
		delete m_pDlgPatternVirtualRectSearchTool;
		m_pDlgPatternVirtualRectSearchTool = NULL;
	}


	if (m_pDlgACFInspectTool !=NULL)
	{
		delete m_pDlgACFInspectTool;
		m_pDlgACFInspectTool = NULL;
	}

	if (m_pDlgCircleCornerSearchCheckInspectTool != NULL)
	{
		delete m_pDlgCircleCornerSearchCheckInspectTool;
		m_pDlgCircleCornerSearchCheckInspectTool = NULL;
	}
	
	if (m_pDlgTwoQuickModelSearchTool != NULL)
	{
		delete m_pDlgTwoQuickModelSearchTool;
		m_pDlgTwoQuickModelSearchTool = NULL;
	}
	
	if (m_pDlgQuickSearchTool != NULL)
	{
		delete m_pDlgQuickSearchTool;
		m_pDlgQuickSearchTool = NULL;
	}
	if (m_pDlgPatternSearchTool != NULL)
	{
		delete m_pDlgPatternSearchTool;
		m_pDlgPatternSearchTool = NULL;
	}
	if(m_pDlgLineSearchTool != NULL)
	{
		delete m_pDlgLineSearchTool;
		m_pDlgLineSearchTool = NULL;
	}
	if (m_pDlgCornerSearchTool != NULL)
	{
		delete m_pDlgCornerSearchTool;
		m_pDlgCornerSearchTool = NULL;
	}
	if (m_pDlgCircleSearchTool != NULL)
	{
		delete m_pDlgCircleSearchTool;
		m_pDlgCircleSearchTool = NULL;
	}
	if (m_pDlgCrossSearchTool != NULL)
	{
		delete m_pDlgCrossSearchTool;
		m_pDlgCrossSearchTool = NULL;
	}
	if (m_pDlgPatCornerSearchTool != NULL)
	{
		delete m_pDlgPatCornerSearchTool;
		m_pDlgPatCornerSearchTool = NULL;
	}
	if (m_pDlgProCircleSearchTool != NULL)
	{
		delete m_pDlgProCircleSearchTool;
		m_pDlgProCircleSearchTool = NULL;
	}
	if (m_pDlgFixedPosSearchTool != NULL)
	{
		delete m_pDlgFixedPosSearchTool;
		m_pDlgFixedPosSearchTool = NULL;
	}
	if (m_pDlgPatternCornerSearchTool != NULL)
	{
		delete m_pDlgPatternCornerSearchTool;
		m_pDlgPatternCornerSearchTool = NULL;
	}
	if (m_pDlgCombinedQuickSearchTool != NULL)
	{
		delete m_pDlgCombinedQuickSearchTool;
		m_pDlgCombinedQuickSearchTool = NULL;
	}
	if (m_pDlgCombinedPatternSearchTool != NULL)
	{
		delete m_pDlgCombinedPatternSearchTool;
		m_pDlgCombinedPatternSearchTool = NULL;
	}
	if (m_pDlgBaseCornerSearchTool != NULL)
	{
		delete m_pDlgBaseCornerSearchTool;
		m_pDlgBaseCornerSearchTool = NULL;
	}
	if (m_pDlgPatternSearchCheckTool != NULL)
	{
		delete m_pDlgPatternSearchCheckTool;
		m_pDlgPatternSearchCheckTool = NULL;
	}
	if (m_pDlgQuickSearchCheckTool != NULL)
	{
		delete m_pDlgQuickSearchCheckTool;
		m_pDlgQuickSearchCheckTool = NULL;
	}
	if (m_pDlgQuickSearchObroundTool != NULL)
	{
		delete m_pDlgQuickSearchObroundTool;
		m_pDlgQuickSearchObroundTool = NULL;
	}
	if (m_pDlgPatternSearchObroundTool != NULL)
	{
		delete m_pDlgPatternSearchObroundTool;
		m_pDlgPatternSearchObroundTool = NULL;
	}
	if (m_pDlgMPatternSearchTool != NULL)
	{
		delete m_pDlgMPatternSearchTool;
		m_pDlgMPatternSearchTool = NULL;
	}
	if (m_pDlgMQuickSearchTool != NULL)
	{
		delete m_pDlgMQuickSearchTool;
		m_pDlgMQuickSearchTool = NULL;
	}
	if (m_pDlgMQuickCornerSearchTool != NULL)
	{
		delete m_pDlgMQuickCornerSearchTool;
		m_pDlgMQuickCornerSearchTool = NULL;
	}
	if (m_pDlgMQuickPickCornerSearchTool != NULL)
	{
		delete m_pDlgMQuickPickCornerSearchTool;
		m_pDlgMQuickPickCornerSearchTool = NULL;
	}
	if (m_pDlgQuickSearchGenRectTool != NULL)
	{
		delete m_pDlgQuickSearchGenRectTool;
		m_pDlgQuickSearchGenRectTool = NULL;
	}
	if (m_pDlgPatternSearchGenRectTool != NULL)
	{
		delete m_pDlgPatternSearchGenRectTool;
		m_pDlgPatternSearchGenRectTool = NULL;
	}
	if (m_pDlgPatternLineSearchTool != NULL)
	{
		delete m_pDlgPatternLineSearchTool;
		m_pDlgPatternLineSearchTool = NULL;
	}
	if (m_pDlg2PatternSearchTool != NULL)
	{
		delete m_pDlg2PatternSearchTool;
		m_pDlg2PatternSearchTool = NULL;
	}
	if (m_pDlgQuickLineCircleSearchTool != NULL)
	{
		delete m_pDlgQuickLineCircleSearchTool;
		m_pDlgQuickLineCircleSearchTool = NULL;
	}
	if (m_pDlgPatternLineCircleSearchTool != NULL)
	{
		delete m_pDlgPatternLineCircleSearchTool;
		m_pDlgPatternLineCircleSearchTool = NULL;
	}
	if (m_pDlgPatternCircleSearchTool != NULL)
	{
		delete m_pDlgPatternCircleSearchTool;
		m_pDlgPatternCircleSearchTool = NULL;
	}
	if (m_pDlgQuickSearchCheckLineTool != NULL)
	{
		delete m_pDlgQuickSearchCheckLineTool;
		m_pDlgQuickSearchCheckLineTool = NULL;
	}
	if (m_pDlgPatternSearchCheckLineTool != NULL)
	{
		delete m_pDlgPatternSearchCheckLineTool;
		m_pDlgPatternSearchCheckLineTool = NULL;
	}
	if (m_pDlgPatternBlobSearchTool != NULL)
	{
		delete m_pDlgPatternBlobSearchTool;
		m_pDlgPatternBlobSearchTool = NULL;
	}
	if (m_pDlgCornerSearchCheckTool != NULL)
	{
		delete m_pDlgCornerSearchCheckTool;
		m_pDlgCornerSearchCheckTool = NULL;
	}
	if (m_pDlgRectSearchTool != NULL)
	{
		delete m_pDlgRectSearchTool;
		m_pDlgRectSearchTool = NULL;
	}
	if (m_pDlgPatternRectSearchTool != NULL)
	{
		delete m_pDlgPatternRectSearchTool;
		m_pDlgPatternRectSearchTool = NULL;
	}
	if (m_pDlgAreaInspectTool != NULL)
	{
		delete m_pDlgAreaInspectTool;
		m_pDlgAreaInspectTool = NULL;
	}
	if (m_pDlgAreaInspectExTool != NULL)
	{
		delete m_pDlgAreaInspectExTool;
		m_pDlgAreaInspectExTool = NULL;
	}
	if (m_pDlgChordSearchTool != NULL)
	{
		delete m_pDlgChordSearchTool;
		m_pDlgChordSearchTool = NULL;
	}
	if (m_pDlgTwoPatModelSearchTool != NULL)
	{
		delete m_pDlgTwoPatModelSearchTool;
		m_pDlgTwoPatModelSearchTool = NULL;
	}
	if (m_pDlgMPatternCornerSearchTool != NULL)
	{
		delete m_pDlgMPatternCornerSearchTool;
		m_pDlgMPatternCornerSearchTool = NULL;
	}
	if (m_pDlgMPatternPickCornerSearchTool != NULL)
	{
		delete m_pDlgMPatternPickCornerSearchTool;
		m_pDlgMPatternPickCornerSearchTool = NULL;
	}
	if (m_pDlgPatternCornerCircleSearchTool != NULL)
	{
		delete m_pDlgPatternCornerCircleSearchTool;
		m_pDlgPatternCornerCircleSearchTool = NULL;
	}
	if (m_pDlgQuickCornerCircleSearchTool != NULL)
	{
		delete m_pDlgQuickCornerCircleSearchTool;
		m_pDlgQuickCornerCircleSearchTool = NULL;
	}
	if (m_pDlgPatternPosLineDistanceTool != NULL)
	{
		delete m_pDlgPatternPosLineDistanceTool;
		m_pDlgPatternPosLineDistanceTool = NULL;
	}
	if (m_pDlgCornerSearchCheckInspectTool != NULL)
	{
		delete m_pDlgCornerSearchCheckInspectTool;
		m_pDlgCornerSearchCheckInspectTool = NULL;
	}
	if (m_pDlgPatternDoubleSearchTool != NULL)
	{
		delete m_pDlgPatternDoubleSearchTool;
		m_pDlgPatternDoubleSearchTool = NULL;
	}
	if (m_pDlgPatternChordSearchTool != NULL)
	{
		delete m_pDlgPatternChordSearchTool;
		m_pDlgPatternChordSearchTool = NULL;
	}
	if (m_pDlgQuickMulCircleSearchTool != NULL)
	{
		delete m_pDlgQuickMulCircleSearchTool;
		m_pDlgQuickMulCircleSearchTool = NULL;
	}
	if (m_pDlgDmCodeCalibrateTool != NULL)
	{
		delete m_pDlgDmCodeCalibrateTool;
		m_pDlgDmCodeCalibrateTool = NULL;
	}
	if (m_pDlgPatternCircle4LineSearchTool != NULL)
	{
		delete m_pDlgPatternCircle4LineSearchTool;
		m_pDlgPatternCircle4LineSearchTool = NULL;
	}
	if (m_pDlgQuickCircle4LineSearchTool != NULL)
	{
		delete m_pDlgQuickCircle4LineSearchTool;
		m_pDlgQuickCircle4LineSearchTool = NULL;
	}
	if (m_pDlgSpecialPickSearchTool != NULL)
	{
		delete m_pDlgSpecialPickSearchTool;
		m_pDlgSpecialPickSearchTool = NULL;
	}
	if (m_pDlgPatternCornerLineSearchTool != NULL)
	{
		delete m_pDlgPatternCornerLineSearchTool;
		m_pDlgPatternCornerLineSearchTool = NULL;
	}
	if (m_pDlgPatternMulCircleSearchTool != NULL)
	{
		delete m_pDlgPatternMulCircleSearchTool;
		m_pDlgPatternMulCircleSearchTool = NULL;
	}
	if (m_pDlgPatternCornerMulCircleSearchTool != NULL)
	{
		delete m_pDlgPatternCornerMulCircleSearchTool;
		m_pDlgPatternCornerMulCircleSearchTool = NULL;
	}
	if (m_pDlgPatternCornerMulObroundTool != NULL)
	{
		delete m_pDlgPatternCornerMulObroundTool;
		m_pDlgPatternCornerMulObroundTool = NULL;
	}
	if (m_pDlgQuickCornerMulObroundTool != NULL)
	{
		delete m_pDlgQuickCornerMulObroundTool;
		m_pDlgQuickCornerMulObroundTool = NULL;
	}
	if (m_pDlgIdReaderDmCodeTool != NULL)
	{
		delete m_pDlgIdReaderDmCodeTool;
		m_pDlgIdReaderDmCodeTool = NULL;
	}
	
	if (m_pDlgIdReaderBarCodeTool != NULL)
	{
		delete m_pDlgIdReaderBarCodeTool;
		m_pDlgIdReaderBarCodeTool = NULL;
	}
	if (m_pDlgQuickPosLineDistanceTool != NULL)
	{
		delete m_pDlgQuickPosLineDistanceTool;
		m_pDlgQuickPosLineDistanceTool = NULL;
	}

	if (m_pDlgCombinedCornerSearchTool != NULL)
	{
		delete m_pDlgCombinedCornerSearchTool;
		m_pDlgCombinedCornerSearchTool = NULL;
	}
	if(m_pDlgMQuickPickSearchTool!=NULL)
	{
	delete m_pDlgMQuickPickSearchTool;
	m_pDlgMQuickPickSearchTool=NULL;
	}
	if(m_pDlgMPatternPickSearchTool!=NULL)
	{
		delete m_pDlgMPatternPickSearchTool;
		m_pDlgMPatternPickSearchTool=NULL;
	}

	if(m_pDlgAreaInspectPickTool!=NULL)
	{
		delete m_pDlgAreaInspectPickTool;
		m_pDlgAreaInspectPickTool=NULL;
	}
	DestoryItems();
}

void CDlgSearch::InitDlgItem()
{
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		DlgSearchToolInfo dlgSearchToolInfo = m_pVisionASM->GetDlgSearchToolData();
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		vector<map<pair<int,int>,BOOL>>::iterator iterSys;
		map<pair<int,CString>,BOOL>::iterator iterDlg;
		iterSys = platformInfo.m_vmapSysSearchToolInfo.begin();
		


		for (;iterSys != platformInfo.m_vmapSysSearchToolInfo.end();iterSys++)
		{
			if ((*iterSys).begin()->second)
			{
				for (iterDlg = dlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();iterDlg != dlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterDlg++)
				{
					if (((*iterSys).begin()->first.first == iterDlg->first.first) && iterDlg->second)
					{
						m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,iterDlg->first.first));
						break;
					}
				}
			}
		}
		//if (platformInfo.m_bQuickSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bPatternSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bLineSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bCornerSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bCircleSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bCrossSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CROSS_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bPatCornerSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATCORNER_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bProCircleSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PROCIRCLE_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bFixedPosSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FIXEDPOS_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bPatternCornerSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bCombinedQuickSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_QUICK_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bCombinedPatternSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bBaseCornerSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BASE_CORNER_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bPatternSearchCheckToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_TOOL));
		//}
		//if (platformInfo.m_bQuickSearchCheckToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_TOOL));
		//}
		//if (platformInfo.m_bQuickSearchObroundToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_OBROUND_TOOL));
		//}
		//if (platformInfo.m_bPatternSearchObroundToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL));
		//}
		//if (platformInfo.m_bMPatternSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bMQuickSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bMQuickCornerSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_CORNER_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bQuickSearchGenRectToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_GENRECT_TOOL));
		//}
		//if (platformInfo.m_bPatternSearchGenRectToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL));
		//}
		//if (platformInfo.m_bPatternLineSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_SEARCH_TOOL));
		//}
		//if (platformInfo.m_b2PatternSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_2PATTERN_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bQuickLineCircleSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bPatternLineCircleSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bPatternCircleSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bQuickSearchCheckLineToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL));
		//}
		//if (platformInfo.m_bPatternSearchCheckLineToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL));
		//}
		//if (platformInfo.m_bPatternBlobSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_BLOB_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bCornerSearchCheckToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_CHECK_TOOL));
		//}
		//if (platformInfo.m_bRectSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bPatternRectSearchToolEnabled)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_RECT_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bAreaInspectToolEnable)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECT_TOOL));
		//}
		//if (platformInfo.m_bAreaInspectExToolEnable)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECTEX_TOOL));
		//}
		//if (platformInfo.m_bChordSearchToolEnable)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_CHORD_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bTwoPatModelSearchToolEnable)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bMPatternCornerSearchToolEnable)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNCORNER_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bPatternCornerCircleSearchToolEnable)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL));
		//}
		//if(platformInfo.m_bQuickCornerCircleSearchToolEnable)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL));
		//}
		//if (platformInfo.m_bPatternPosLineDistanceToolEnable)
		//{
		//	m_comboSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL));
		//}
	}
	
	m_comboDisplayType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_INPUT_IMAGE));
	m_comboDisplayType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TRAIN_IMAGE));
	m_comboDisplayType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RESULT_IMAGE));
	
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	

	m_guiDisplay.Create(IDC_STC_DISPLAY, this);
	m_guiDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_guiDisplay.SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_guiDisplay.SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_guiDisplay.SetLanguage(1);
		break;
	}
//	m_guiDisplay.SetBackgroundColor(RGB(0,0,0));
	m_guiDisplay.RegisterWndMsg(this, SV_DISPLAY_MOUSEMOVE|SV_DISPLAY_DBLCLICK);
	
	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	int nPartWidth[4] = {20, 400, 750, -1};
	m_pStatusBar->SetParts(4, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);
	
	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	
	UpdateStatusBar();
	
	// 设置按钮
	m_btnLoadImage.SetThemeHelper(&m_themeHelper);
// 	m_btnLoadImage.SetIcon(IDI_ICON_LOAD_IMAGE_40);
// 	m_btnLoadImage.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnGrabImage.SetThemeHelper(&m_themeHelper);
// 	m_btnGrabImage.SetIcon(IDI_ICON_GRAB_40);
// 	m_btnGrabImage.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnSearch.SetThemeHelper(&m_themeHelper);
// 	m_btnSearch.SetIcon(IDI_ICON_SEARCH_40);
// 	m_btnSearch.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnOK.SetThemeHelper(&m_themeHelper);
// 	m_btnOK.SetIcon(IDI_ICON_OK_48);
// 	m_btnOK.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_btnCancel.SetThemeHelper(&m_themeHelper);
// 	m_btnCancel.SetIcon(IDI_ICON_CANCEL_48);
// 	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);


	m_comboSearchToolType.SetCurSel(0);
	m_comboDisplayType.SetCurSel(0);

	if (m_pVisionASM != NULL)
	{
		// 如果没有相机，禁用“采集图像”按钮
		GetDlgItem(IDC_BTN_GRAB_IMAGE)->EnableWindow(m_pVisionASM->IsPosCameraOnline(m_nCurPositionIdx));
		GetDlgItem(IDC_BTN_GRAB_SEARCH_IMAGE)->EnableWindow(m_pVisionASM->IsPosCameraOnline(m_nCurPositionIdx));

		if (m_pVisionASM->IsPosCameraGrabbing(m_nCurPositionIdx))
		{
			if (m_pVisionASM->FreezePosCamera(m_nCurPositionIdx))
			{
				SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
				SetDlgItemText(IDC_BTN_GRAB_SEARCH_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_SEARCH_IMAGE));	//_T("采集并定位"));
			}
			else
			{
				SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
				SetDlgItemText(IDC_BTN_GRAB_SEARCH_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB_SEARCH_IMAGE));	//_T("停止采集定位"));
			}
		}
		else
		{
			SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
			SetDlgItemText(IDC_BTN_GRAB_SEARCH_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_SEARCH_IMAGE));	//_T("采集并定位"));
		}
	}


	if(m_bIsPreviewTool)
	{
		m_btnOK.EnableWindow(FALSE);
		m_btnGrabImage.EnableWindow(FALSE);
		m_btnLoadImage.EnableWindow(TRUE);
		m_btnGrabImage.EnableWindow(FALSE);
		
		GetDlgItem(IDC_BTN_GRAB_SEARCH_IMAGE)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CMB_SEARCH_TOOL_TYPE))->EnableWindow(FALSE);

		// 如果是预览模板界面,则修改标题名
		CString strWindowTitleName;
		switch (m_lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			strWindowTitleName = _T("SearchTool Model Preview");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			strWindowTitleName = _T("定位工具模板预览");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			strWindowTitleName = _T("定位工具模板预览");
			break;
		}
		SetWindowText(strWindowTitleName);
		
	}
	// 初始状态下，禁用“保存”按钮
//	GetDlgItem(IDOK)->EnableWindow(FALSE);
}

LRESULT CDlgSearch::OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam)
{
	CString strStatus = " ";
	
	if (wParam == m_guiDisplay.GetID())
	{
		if (m_guiDisplay.GetImageCP() != NULL)
		{
			if (m_guiDisplay.GetImageCP()->IsValid())
			{
				long nWidth  = m_guiDisplay.GetImageCP()->Width();
				long nHeight = m_guiDisplay.GetImageCP()->Height();
				
				// 获取鼠标相对于显示控件的坐标,lParam的高16位存储的是Y坐标,低16位存储的是X坐标
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);
				
				// 将相对于显示控件坐标系的坐标,转换到相对于图像坐标系的坐标
				double dImageX, dImageY;
				m_guiDisplay.TransformPos(x,y,dImageX,dImageY);
				
				// 获取对应像素灰度值
				BYTE R,G,B;
				m_guiDisplay.GetImagePixelValue(LONG(dImageX),LONG(dImageY),R,G,B);
				if(dImageX<0||dImageY<0||dImageY>=nHeight||dImageX>=nWidth)
				{
					R = 0;
					G = 0;
					B = 0;
				}
				
				strStatus.Format("(%.0f,%.0f)  (%d,%d,%d)", dImageX, dImageY, R, G, B);
			}
		}	
	}
	
	m_pStatusBar->SetText(strStatus,3,0);
	
	return 0 ;
}

LRESULT CDlgSearch::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{
	if (wParam == m_guiDisplay.GetID())
	{
		GetDlgItem(IDC_BTN_LOAD_IMAGE2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_SEARCH_EXE2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_SEARCH_EXE3)->ShowWindow(SW_SHOW);
	}

	return 0 ;
}

void CDlgSearch::UpdateStatusBar( bool bSucceed, LPCTSTR strInfo /*= NULL*/, LPCTSTR strInfoEx /*= NULL*/)
{
	if (bSucceed)
	{
		m_pStatusBar->SetIcon(0, m_hIconSucceed);
	}
	else
	{
		m_pStatusBar->SetIcon(0, m_hIconFailed);
	}
	if (strInfo == NULL)
	{
		m_pStatusBar->SetText(_T(""), 1, 0);
	}
	else
	{
		m_pStatusBar->SetText(strInfo, 1, 0);
	}
	if (strInfoEx == NULL)
	{
		m_pStatusBar->SetText(_T(""), 2, 0);
	}
	else
	{
		m_pStatusBar->SetText(strInfoEx, 2, 0);
	}
}

void CDlgSearch::UpdateGuiDisplay()
{
	UpdateData(TRUE);
	
	m_guiDisplay.ClearScreen();
	
	switch(m_nDisplayType)
	{
	case 0:
		{
			m_guiDisplay.SetImage(m_imageInput, TRUE);
			m_pSearchTool->ShowTrainGui(false);
 			m_pSearchTool->ShowSearchResultGui(false);
		}
		break;
	case 1:
		{
			m_guiDisplay.SetImage(m_imageTrain, TRUE);
			m_pSearchTool->SetGuiByCam(m_imageInput.Width(),m_imageInput.Height());
			if (m_pSearchTool != NULL)
			{
				m_pSearchTool->ShowTrainGui(m_imageTrain.IsValid());
				m_pSearchTool->ShowSearchResultGui(false);
				m_pSearchTool->UpdateGuiDisplay();
			}

			CString strSelText = _T("");
			int nCurSel;
			nCurSel = m_comboSearchToolType.GetCurSel();
			m_comboSearchToolType.GetLBText(nCurSel,strSelText);
			//更新控件显示
			UpdateToolDlgControlDisplay(strSelText);
		}	
		break;
	case 2:
		{
			m_guiDisplay.SetImage(m_imageResult, TRUE);
			if (m_pSearchTool != NULL)
			{
				m_pSearchTool->ShowTrainGui(false);
				m_pSearchTool->ShowSearchResultGui(m_imageResult.IsValid());
				m_pSearchTool->UpdateGuiDisplay();
			}
		}
		break;
	}
}

void CDlgSearch::UpdateSearchToolDisplay()
{
	if (m_pDefSearchTool == NULL)
	{
		m_comboSearchToolType.SetCurSel(0);
		OnSelchangeCmbSearchToolType();
		return;
	}
	CSearchToolType eType = m_pDefSearchTool->GetSearchToolType();
	if (eType == eSearchToolUnknown)
	{
		return;
	}	

	DlgSearchToolInfo dlgSearchToolInfo = m_pVisionASM->GetDlgSearchToolData();
	map<pair<int,CString>,BOOL>::iterator iterDlg;
	iterDlg = dlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();
	int nSearchTool = 0;


	vector<map<pair<int,int>,BOOL>>::iterator iterPlatform;
	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
	iterPlatform =sysPlatformInfo.m_vmapSysSearchToolInfo.begin();
	for(;iterPlatform != sysPlatformInfo.m_vmapSysSearchToolInfo.end();iterPlatform++)
	{
		if (eType == (*iterPlatform).begin()->first.second)
		{
			nSearchTool = (*iterPlatform).begin()->first.first;
			break;
		}
	}

	for(;iterDlg != dlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterDlg++)
	{
		if (( nSearchTool == iterDlg->first.first)&&!iterDlg->second)       //m_pDefSearchTool工具在"环境设置"--"定位工具"中取消选定，重新设定定位工具
		{
			m_comboSearchToolType.SetCurSel(0);
			OnSelchangeCmbSearchToolType();
			return;
		}
	}


	if ((m_pSearchTool != NULL) && (m_pSearchTool != m_pDefSearchTool))
	{
		delete m_pSearchTool;
	}
	m_pSearchTool = NULL;

	DestroyAllDlg();

	BOOL bEnableSearchNum = TRUE;

	CString strSelText = _T("");

	switch (eType)
	{
	case eQuickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL); break;
	case ePatternSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL); break;
	case eLineSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_SEARCH_TOOL); break;
	case eCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL); break;
	case eCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL); break;
	case eCrossSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CROSS_SEARCH_TOOL); break;
	case ePatCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATCORNER_SEARCH_TOOL); break;
	case eProCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PROCIRCLE_SEARCH_TOOL); break;
	case eFixedPosSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FIXEDPOS_SEARCH_TOOL); break;
	case ePatternCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL); break;
	case eCombinedQuickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_QUICK_SEARCH_TOOL); break;
	case eCombinedPatternSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL); break;
	case eBaseCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BASE_CORNER_SEARCH_TOOL); break;
	case ePatternSearchCheckTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_TOOL); break;
	case eQuickSearchCheckTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_TOOL); break;
	case eQuickSearchObroundTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_OBROUND_TOOL); break;
	case ePatternSearchObroundTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL); break;
	case eMPatternSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_SEARCH_TOOL); break;
	case eMQuickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_SEARCH_TOOL); break;
	case eMQuickCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_CORNER_SEARCH_TOOL); break;
	case eMQuickPickCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL); break;
	case eQuickSearchGenRectTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_GENRECT_TOOL); break;
	case ePatternSearchGenRectTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL); break;
	case ePatternLineSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_SEARCH_TOOL); break;
	case e2PatternSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_2PATTERN_SEARCH_TOOL); break;
	case eQuickLineCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL); break;
	case ePatternLineCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL); break;
	case ePatternCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL); break;
	case eQuickSearchCheckLineTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL); break;
	case ePatternSearchCheckLineTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL); break;
	case ePatternBlobSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_BLOB_SEARCH_TOOL); break;
	case eCornerSearchCheckTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_CHECK_TOOL); break;
	case eRectSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL); break;
	case ePatternRectSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_RECT_SEARCH_TOOL); break;
	case eAreaInspectTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECT_TOOL); break;
	case eAreaInspectToolEx:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECTEX_TOOL); break;
	case eChordSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CHORD_SEARCH_TOOL); break;
	case eTwoPatModelSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL); break;
	case eMPatternCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNCORNER_SEARCH_TOOL); break;
	case eMPatternPickCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL); break;
	case ePatternCornerCircleSearchTool:
		strSelText =GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL); break;
	case eQuickCornerCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL);break;
	case ePatternPosLineDistanceTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL); break;
    case eCornerSearchCheckInspectTool:
        strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL); break;
	case ePatternDouleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL);break;
	case ePatternChordSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CHORD_SEARCH_TOOL);break;
    case eQuickMulCircleSearchTool:
        strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL);break;
	case eDmCodeCalibrateTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DMCOLDE_CALIBRATE_TOOL);break;
	case ePatternCircle4LineSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL); break;
	case eQuickCircle4LineSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL); break;
	case eSpecialPickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL);break;
	case ePatternCornerLineSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL);break;
	case ePatternMulCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL);break;
	case ePatternCornerMulCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL);break;
	case ePatternCornerMulObroundTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL);break;
	case eQuickCornerMulObroundTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL);break;	
	case eCircleCornerSearchCheckInspectTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL);break;
	case eACFInspectTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL);break;
	case ePatternVirtualRectSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL);break;
	case eVirtualRectSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL);break;
	case eTwoQuickModelSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL); break;
	case eIdReaderDmCodeTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_DMCODE_TOOL);break;
	case eIdReaderBarCodeTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_BARCODE_TOOL);break;
	case eQuickPosLineDistanceTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_POSLINEDISTANCE_TOOL);break;
	case eCombinationCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_CORNER_TOOL);break;
	case eMQuickPickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_SEARCH_TOOL);break;
	case eMPatternPickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_PICK_SEARCH_TOOL);break;
	case eAreaInspectPickTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREAINSPECT_PICK_TOOL);break;
	default:
		break;
	}

	//m_nCurSelSearchToolIdx = m_comboSearchToolType.SelectString(0, strSelText);
	int nFInd = m_comboSearchToolType.FindStringExact(-1,strSelText);
	if (nFInd != CB_ERR)
	{
		m_nCurSelSearchToolIdx = m_comboSearchToolType.SelectString(nFInd-1, strSelText);
	}

    //创建对话框
	CreateToolDlg(strSelText);
	CString strInfo;
	strInfo.Format(_T("开始加载：%s\n"),strSelText);
	AlignToolLogRecord(strInfo,0);
	//更新工具参数
	UpdateSearchToolParam(strSelText);
	strInfo.Format(_T("加载完成：%s\n"),strSelText);
	AlignToolLogRecord(strInfo,0);
    m_imageInput = m_imageTrain;
// 	m_pSearchTool->m_CamHeight = m_imageTrain.Height();
// 	m_pSearchTool->m_CamWidth = m_imageTrain.Width();

	m_comboDisplayType.SetCurSel(1);
	UpdateGuiDisplay();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_pSearchTool->SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_pSearchTool->SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_pSearchTool->SetLanguage(1);
		break;
	}
}

void CDlgSearch::UpdateStatusBar()
{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgSearch::SetSysInfoStringsRef( CStringArray *psaSysInfoStrings )
{
	m_psaSysInfoStrings = psaSysInfoStrings;
	if (m_psaSysInfoStrings != NULL)
	{
		m_lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
}

CBaseSearchTool* CDlgSearch::GetSearchTool()
{
	return m_pSearchTool;
}

void CDlgSearch::SetVisionASMRef( vcBaseVisionAlign *pVisionASM, int nPositionIdx)
{
	SysPlatformInfo platformInfo;
	// 恢复旧视觉对位工具的主窗口
	if (m_pVisionASM != NULL)
	{
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
		m_pVisionASM->SetPosDisplay(m_nCurPositionIdx, m_pPrevGuiDisplay);
	}
	m_pVisionASM = pVisionASM;
	// 设置新视觉对位工具的主窗口
	if (m_pVisionASM != NULL)
	{
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		//if (platformInfo.m_nCamNum == 1)
		//{
		//	nPositionIdx = 0;
		//}
		m_pVAPrevMainWnd = m_pVisionASM->SetMainWnd(this);
		m_pPrevGuiDisplay = m_pVisionASM->GetPosDisplay(nPositionIdx);
		m_pVisionASM->SetPosDisplay(nPositionIdx, &m_guiDisplay);
		m_nCurPositionIdx = nPositionIdx;
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);

		BOOL bEnableShutter = optionInfo.m_bEnableSearchShutter;
		if (bEnableShutter)
		{
			// 获取环境设置中的曝光
			double dMin = 0, dMax = 0;
			bool bEnable = false;
			m_pVisionASM->GetPosCameraShutter(m_nCurPositionIdx, m_dEnvironmentShutter.m_dShutter, dMin, dMax);
			m_pVisionASM->GetPosCameraGain(m_nCurPositionIdx, m_dEnvironmentShutter.m_dGain, dMin, dMax);
			//m_pVisionASM->GetPosCameraGamma(m_nCurPositionIdx,bEnable, m_dEnvironmentShutter.m_dGamma, dMin, dMax);
			ImageRoi imageRoi;
			m_pVisionASM->GetPosImageROIParam(m_nCurPositionIdx, imageRoi);
			m_dEnvironmentShutter.m_dGamma = imageRoi.m_dGamma;
		}
	}
}

void CDlgSearch::UpdateDialogLanguage()
{
//	return;
	if (m_psaSysInfoStrings != NULL)
	{
		m_lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	
	switch (m_lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strXMLPath = m_strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strXMLPath = m_strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strXMLPath = m_strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
		break;
	}
	xml.Load(strXMLPath);
	xml.FindElem(_T("ResourceBundle"));    
	xml.EnterElem();
	
	while(xml.FindElem(_T("")))
	{
		CString strDlgID = xml.GetAttribute(_T("Key"));
		
		if(_T("IDD_DLG_SEARCH") == strDlgID)
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

void CDlgSearch::DestoryItems()
{
	if (m_pVisionASM != NULL)
	{
		if (m_pVisionASM->IsPosCameraOnline(m_nCurPositionIdx))
		{
			m_pVisionASM->FreezePosCamera(m_nCurPositionIdx);
		}

		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
		m_pVisionASM->SetPosDisplay(m_nCurPositionIdx, m_pPrevGuiDisplay);
	}

	if (m_pStatusBar != NULL)
	{
		delete m_pStatusBar;
		m_pStatusBar = NULL;
	}
	if (m_pTrainDataParam != NULL)
	{
		delete m_pTrainDataParam;
		m_pTrainDataParam = NULL;
	}
	if (m_pTrainGuiParam != NULL)
	{
		delete m_pTrainGuiParam;
		m_pTrainGuiParam = NULL;
	}
	if (m_pSearchDataParam != NULL)
	{
		delete m_pSearchDataParam;
		m_pSearchDataParam = NULL;
	}
	if (m_pSearchGuiParam != NULL)
	{
		delete m_pSearchGuiParam;
		m_pSearchGuiParam = NULL;
	}
	if (m_pSearchTool != NULL)
	{
		delete m_pSearchTool;
		m_pSearchTool = NULL;
	}
	if (m_hIconSucceed != NULL)
	{
		DestroyIcon(m_hIconSucceed);
		m_hIconSucceed = NULL;
	}
	if (m_hIconFailed != NULL)
	{
		DestroyIcon(m_hIconFailed);
		m_hIconSucceed = NULL;
	}

// 	if (m_pSearchTool != NULL)
// 	{
// 		delete m_pSearchTool;
// 		m_pSearchTool = NULL;
// 	}
}


BOOL CDlgSearch::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (m_pVisionASM != NULL)
	{
		if (m_pVisionASM->IsPosCameraOnline(m_nCurPositionIdx))
		{
			m_pVisionASM->FreezePosCamera(m_nCurPositionIdx);
		}

		// 只有启用产品曝光时，才再销毁时重新设置成初始化时得到的环境曝光
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		BOOL bEnableShutter = optionInfo.m_bEnableSearchShutter;
		if (bEnableShutter && (m_bEnableSearchShutter.m_bShutterEnable || m_bEnableSearchShutter.m_bGainEnable || m_bEnableSearchShutter.m_bGammaEnable))
		{
			if (m_pVisionASM->SetPosCameraShutter(m_nCurPositionIdx, m_dEnvironmentShutter.m_dShutter))
			{
				m_pVisionASM->UpdatePosCameraShutter(m_nCurPositionIdx);
			}

			if (m_pVisionASM->SetPosCameraGain(m_nCurPositionIdx, m_dEnvironmentShutter.m_dGain))
			{
				m_pVisionASM->UpdatePosCameraGain(m_nCurPositionIdx);
			}

			ImageRoi roi;
			m_pVisionASM->GetPosImageROIParam(m_nCurPositionIdx, roi);
			roi.m_dGamma = m_dEnvironmentShutter.m_dGamma;
			if (m_pVisionASM->SetImageROIParam(m_nCurPositionIdx,roi))
			{
				//m_pVisionASM->UpdatePosCameraShutter(m_nCurPositionIdx);
			}

		}

		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
		m_pVisionASM->SetPosDisplay(m_nCurPositionIdx, m_pPrevGuiDisplay);
	}

	return CDialog::DestroyWindow();
}

void CDlgSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSearch)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SEARCH_EXE, m_btnSearch);
	DDX_Control(pDX, IDC_BTN_LOAD_IMAGE, m_btnLoadImage);
	DDX_Control(pDX, IDC_BTN_GRAB_IMAGE, m_btnGrabImage);
	DDX_Control(pDX, IDC_CMB_DISPLAY_TYPE, m_comboDisplayType);
	DDX_Control(pDX, IDC_CMB_SEARCH_TOOL_TYPE, m_comboSearchToolType);
	DDX_Control(pDX, IDC_TAB_SEARCH_TOOL_SETTING, m_ctrlTabControl);
	DDX_CBIndex(pDX, IDC_CMB_DISPLAY_TYPE, m_nDisplayType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSearch, CDialog)
	//{{AFX_MSG_MAP(CDlgSearch)
	ON_CBN_SELCHANGE(IDC_CMB_SEARCH_TOOL_TYPE, OnSelchangeCmbSearchToolType)
	ON_CBN_SELCHANGE(IDC_CMB_DISPLAY_TYPE, OnSelchangeCmbDisplayType)
	ON_BN_CLICKED(IDC_BTN_LOAD_IMAGE, OnBtnLoadImage)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SEARCH_EXE, OnBtnSearchExe)
	ON_BN_CLICKED(IDC_BTN_GRAB_IMAGE, OnBtnGrabImage)
	ON_MESSAGE(WM_SV_DISPLAY_MOUSEMOVE,OnGuiDisplayMouseMove)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_LOAD_IMAGE2, &CDlgSearch::OnBnClickedBtnLoadImage2)
	ON_BN_CLICKED(IDC_BTN_SEARCH_EXE2, &CDlgSearch::OnBnClickedBtnSearchExe2)
//	ON_BN_CLICKED(IDOK, &CDlgSearch::OnBnClickedOk)
ON_BN_CLICKED(IDC_BTN_SEARCH_EXE3, &CDlgSearch::OnBnClickedBtnSearchExe3)
ON_BN_CLICKED(IDC_BTN_GRAB_SEARCH_IMAGE, &CDlgSearch::OnBnClickedBtnGrabSearchImage)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BTN_PRODUCT_SHUTTER_SETTING, &CDlgSearch::OnBnClickedBtnProductShutterSetting)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSearch message handlers

LRESULT CDlgSearch::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			m_pVisionASM->UpdateDisplayImage((int)wParam);
		}
		break;
	case WM_UPDATE_STATUSBAR_INFO:
		{
			m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
			UpdateStatusBar();
		}
		break;
	case WM_CAMERA_ARRIVAL:
		{
			if (m_nCurPositionIdx == (int)wParam)
			{
				GetDlgItem(IDC_BTN_GRAB_IMAGE)->EnableWindow(TRUE);
				
				m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
				UpdateStatusBar();
			}
		}
		break;
	case WM_CAMERA_REMOVAL:
		{
			if (m_nCurPositionIdx == (int)wParam)
			{
				scGuiDisplay* pDisplay = m_pVisionASM->GetCameraDisplay((int)wParam);
				if ((pDisplay != NULL) && ::IsWindow(pDisplay->GetSafeHwnd()))
				{
					pDisplay->ClearScreen();
				}
				GetDlgItem(IDC_BTN_GRAB_IMAGE)->EnableWindow(FALSE);

				m_pVisionASM->GetStatusBarInfo(m_bStatusBarFlag, m_strStatusBarInfo1, m_strStatusBarInfo2);
				UpdateStatusBar();
			}
		}
		break;
	case WM_SETFOCUS:
		{
			TRACE(_T("CDlgSearch WM_SETFOCUS\n"));
		}
		break;
	case WM_KILLFOCUS:
		{
			TRACE(_T("CDlgSearch WM_KILLFOCUS\n"));
		}
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgSearch::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将启用曝光和不启用曝光完全分离出来，避免有些相机切换相机曝光不起作用
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	BOOL bEnableShutter = optionInfo.m_bEnableSearchShutter;

	if (bEnableShutter && (m_bEnableSearchShutter.m_bShutterEnable || m_bEnableSearchShutter.m_bGainEnable || m_bEnableSearchShutter.m_bGammaEnable))
	{
		GetDlgItem(IDC_BTN_PRODUCT_SHUTTER_SETTING)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BTN_PRODUCT_SHUTTER_SETTING)->ShowWindow(SW_HIDE);
	}

	UpdateDialogLanguage();
	InitDlgItem();

	UpdateSearchToolDisplay();

	SetWindowText(m_strTitleName);

	CString strToolIdx,strLog;
	m_comboSearchToolType.GetLBText(m_nCurSelSearchToolIdx,strToolIdx);
	strLog.Format(_T("进入[%s]\n"),strToolIdx);
	g_opLogFile.SaveLogText(strLog,5);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSearch::OnSelchangeCmbDisplayType() 
{
	int nDisplayTypeOld = m_nDisplayType;
	CString strDisplayTypeOld,strDisplayType,strLog;
	m_comboDisplayType.GetLBText(nDisplayTypeOld,strDisplayTypeOld);

	UpdateGuiDisplay();

	m_comboDisplayType.GetLBText(m_nDisplayType,strDisplayType);
	strLog.Format(_T("修改[%s]到[%s]\n"),strDisplayTypeOld,strDisplayType);
	g_opLogFile.SaveLogText(strLog,6);
}

void CDlgSearch::OnBtnLoadImage() 
{
	CString strLog;
	strLog.Format(_T("点击[加载图像]"));
	g_opLogFile.SaveLogText(strLog,6);


	/*CString strFilter = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FILTER_BMP_FILE) + _T("|*.bmp||");*/
	CString strFilter = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FILTER_BMP_AND_JPEG_FILE);
	CFileDialog cFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (IDOK == cFileDlg.DoModal())
	{
		/*scDIB cDib;*/
		CString strPath = cFileDlg.GetPathName();
		if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".bmp"))
		{
			scDIB cDib;
			cDib.Init(strPath);
			cDib.GetImage(m_imageInput);
		}
		else if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".jpg"))
		{
			scJpeg cJpeg;
			cJpeg.Read(strPath,m_imageInput);
		}
		else if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".png"))
		{
			scPng cPng;
			cPng.Read(strPath,m_imageInput);
		}
		//cDib.Init(strPath);
		//cDib.GetImage(m_imageInput);
		
		m_imageTrain.Release();
		m_imageResult.Release();
		m_comboDisplayType.SetCurSel(0);
		UpdateGuiDisplay();

		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_LOAD_IMAGE_SUCCEED);	//_T("图像加载成功");
		m_strStatusBarInfo2 = strPath;
		UpdateStatusBar();
	}
}

void CDlgSearch::OnBtnSearchExe()
{
	if (m_pSearchTool == NULL)
	{
		return;
	}
	CString strLog;
	strLog.Format(_T("点击[搜索]"));
	g_opLogFile.SaveLogText(strLog,6);
	// 更新搜索参数
	int nCurSel = m_comboSearchToolType.GetCurSel();
	CString strSelText = _T("");
	m_comboSearchToolType.GetLBText(nCurSel, strSelText);
	UpdateDlgSearchToolData(strSelText);

	scTimer t;

	BOOL bInspectTool = FALSE;
	bInspectTool = (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECT_TOOL))|| (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECTEX_TOOL));

	if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_DMCOLDE_CALIBRATE_TOOL))
	{
		UpdateStatusBar(true,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_IS_SEARCHING));
	}

	//
	bool bSearchSuccess = false;

	// 二维码工具内会对传入图像进行采样处理
	cpImage imageTmp;
	if (eIdReaderDmCodeTool == m_pSearchTool->GetSearchToolType())
	{
		m_imageInput.Clone(imageTmp);
		bSearchSuccess = (m_pSearchTool->Search(imageTmp)|| bInspectTool);
		//imageTmp.Release();
	}
	else
	{
		bSearchSuccess = (m_pSearchTool->Search(m_imageInput)|| bInspectTool );
	}

	// 搜索
	if (bSearchSuccess)
	{
		CString strInfo;
//		strInfo.Format(_T("%f"),t.GetTimeMilli(TRUE));
//		AfxMessageBox(strInfo);	

		m_imageResult = m_imageInput;
		m_comboDisplayType.SetCurSel(2);
		UpdateGuiDisplay();
//		GetDlgItem(IDOK)->EnableWindow(TRUE);
//		CString strInfo;
		if (m_pSearchTool->GetResultNum())
		{
			CSearchResult result;
			m_pSearchTool->GetResult(0, result);
			strInfo.Format(_T("X:%.1f,Y:%.1f,D:%.3f,S:%.2f,T:%.1fms"), 
				result.m_dPosX, result.m_dPosY, result.m_dAngle,result.m_dScore,t.GetTimeMilli(TRUE));
		}
		
		// 显示结果
		UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_SUCCEED), strInfo);
		m_bSearchDataUpdated = TRUE;

	}
	else
	{
		m_imageResult = m_imageInput;
		m_comboDisplayType.SetCurSel(2);
		UpdateGuiDisplay();
		//错误信息提示
		CString strErrInfo =  m_pSearchTool->GetResultErrInfo();
		UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_FAILED),strErrInfo);
		m_bSearchDataUpdated = FALSE;
	}
	

	if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECT_TOOL))
	{
		m_pDlgAreaInspectTool->UpdateSearchResult();
	}
	if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECTEX_TOOL))
	{
		m_pDlgAreaInspectExTool->UpdateSearchResult();
	}
	if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_ACFINSPECT_TOOL))
	{
		m_pDlgACFInspectTool->UpdateSearchResult();
	}
	if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL))
	{
		m_pDlgPatternPosLineDistanceTool->UpdateSearchResult();
	}
    if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL))
    {
        m_pDlgCornerSearchCheckInspectTool->UpdateSearchData(false);
    }
	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL))
	{
		m_pDlgSpecialPickSearchTool->UpdateSearchResult();
	}
	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL))
	{
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchData(false);
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchResult();
	}
	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL))
	{
		m_pDlgACFInspectTool->UpdateSearchData(false);
		m_pDlgACFInspectTool->UpdateSearchResult();
	}
	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgCircleSearchTool->UpdateSearchResult();
	}
	if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL))
	{
		m_pDlgQuickMulCircleSearchTool->UpdateSearchResult();
	}
	if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_QUICK_POSLINEDISTANCE_TOOL))
	{
		m_pDlgQuickPosLineDistanceTool->UpdateSearchResult();
	}
	if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREAINSPECT_PICK_TOOL))
	{
		m_pDlgAreaInspectPickTool->UpdateSearchResult();
	}
}

void CDlgSearch::OnBtnGrabImage() 
{
	BOOL bSysKeyboardEnabled = m_bSysKeyboardEnabled;
	m_bSysKeyboardEnabled = FALSE;
	
	CString strLog;
	strLog.Format(_T("点击[采集图像]"));
	g_opLogFile.SaveLogText(strLog, 6);

	m_strStatusBarInfo2 = _T("");
	if (m_pVisionASM != NULL)
	{
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		if (m_pVisionASM->IsPosCameraOnline(m_nCurPositionIdx))
		{
			strLog.Format(_T("当前位置[%d]相机处于在线状态"), m_nCurPositionIdx);
			g_opLogFile.SaveLogText(strLog, 7);

			m_comboDisplayType.SetCurSel(0);
			if (m_pVisionASM->IsPosCameraGrabbing(m_nCurPositionIdx))
			{
				strLog.Format(_T("当前位置[%d]相机处于正在采集状态"), m_nCurPositionIdx);
				g_opLogFile.SaveLogText(strLog, 8);

				if (m_pVisionASM->FreezePosCamera(m_nCurPositionIdx))
				{
					strLog.Format(_T("当前位置[%d]相机停止采集成功"), m_nCurPositionIdx);
					g_opLogFile.SaveLogText(strLog, 9);

					m_imageTrain.Release();
					m_imageResult.Release();
					cpImage* pImageTemp = (cpImage*)m_guiDisplay.GetImageCP();
					if (pImageTemp != NULL)
					{
						pImageTemp->Clone(m_imageInput);	
					}
					SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
					GetDlgItem(IDC_BTN_GRAB_SEARCH_IMAGE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_SEARCH_EXE)->EnableWindow(TRUE);
					GetDlgItem(IDOK)->EnableWindow(TRUE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
					m_comboDisplayType.EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_PRODUCT_SHUTTER_SETTING)->EnableWindow(FALSE);

					UpdateGuiDisplay();
					m_bStatusBarFlag = TRUE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRAB_STOPPED);	//_T("采集已停止")
				}
				else
				{
					strLog.Format(_T("当前位置[%d]相机停止采集失败"), m_nCurPositionIdx);
					g_opLogFile.SaveLogText(strLog, 9);

					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_FAIL_TO_STOP_GRAB);	//_T("停止采集失败");
				}
			}
			else
			{

				strLog.Format(_T("当前位置[%d]相机处于停止采集状态"), m_nCurPositionIdx);
				g_opLogFile.SaveLogText(strLog, 8);

				BOOL bEnableShutter = optionInfo.m_bEnableSearchShutter;
				if (bEnableShutter && (m_bEnableSearchShutter.m_bShutterEnable || m_bEnableSearchShutter.m_bGainEnable || m_bEnableSearchShutter.m_bGammaEnable))
				{
					m_pVisionASM->SetPosCameraShutter(m_nCurPositionIdx, m_dProductSearchShutter.m_dShutter);
					m_pVisionASM->UpdatePosCameraShutter(m_nCurPositionIdx);
					m_pVisionASM->SetPosCameraGain(m_nCurPositionIdx, m_dProductSearchShutter.m_dGain);
					m_pVisionASM->UpdatePosCameraGain(m_nCurPositionIdx);

					ImageRoi imageRoi;
					m_pVisionASM->GetPosImageROIParam(m_nCurPositionIdx, imageRoi);
					imageRoi.m_dGamma = m_dProductSearchShutter.m_dGamma;
					m_pVisionASM->SetPosImageROIParam(m_nCurPositionIdx,imageRoi);



				}
				
				if (m_pVisionASM->GrabPosCamera(m_nCurPositionIdx))
				{

					strLog.Format(_T("当前位置[%d]相机开始采集成功"), m_nCurPositionIdx);
					g_opLogFile.SaveLogText(strLog, 9);


					SetDlgItemText(IDC_BTN_GRAB_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
					GetDlgItem(IDC_BTN_GRAB_SEARCH_IMAGE)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_SEARCH_EXE)->EnableWindow(FALSE);
					GetDlgItem(IDOK)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
					m_comboDisplayType.EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_PRODUCT_SHUTTER_SETTING)->EnableWindow(TRUE);

					UpdateGuiDisplay();
					m_bStatusBarFlag = TRUE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRABBING);	//_T("正在连续采集")
					m_strStatusBarInfo2 = _T("");
				}
				else
				{
					strLog.Format(_T("当前位置[%d]相机开始采集失败"), m_nCurPositionIdx);
					g_opLogFile.SaveLogText(strLog, 9);

					m_bStatusBarFlag = FALSE;
					m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GRAB_FAILED);	//_T("采集失败")
				}
			}		
		}
		else
		{
			strLog.Format(_T("当前位置[%d]相机处于离线状态"), m_nCurPositionIdx);
			g_opLogFile.SaveLogText(strLog, 7);

			m_bStatusBarFlag = FALSE;
			m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_CONNECTION_ERROR);	//_T("相机连接错误")
		}
	}
	else
	{
		strLog.Format(_T("当前位置[%d]VISION=NULL"), m_nCurPositionIdx);
		g_opLogFile.SaveLogText(strLog, 7);

		m_bStatusBarFlag = FALSE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_VISION_ALIGN_TOOL);
	}

	CString strSelText = _T("");
	int nCurSel;
	nCurSel = m_comboSearchToolType.GetCurSel();
	m_comboSearchToolType.GetLBText(nCurSel,strSelText);
	//更新控件显示
	UpdateToolDlgControlDisplay(strSelText);


	m_bSysKeyboardEnabled = bSysKeyboardEnabled;

	UpdateStatusBar();
}

void CDlgSearch::OnSelchangeCmbSearchToolType() 
{
	int nCurSel = m_comboSearchToolType.GetCurSel();
	if (nCurSel == m_nCurSelSearchToolIdx)
	{
		return;
	}
	//UpdateSearchToolDisplay();
	int nCurSelSearchToolIdxOld = m_nCurSelSearchToolIdx;
	//CString strOld,strNew,strLog;
	//m_comboSearchToolType.GetLBText(nCurSelSearchToolIdxOld,strOld);
	////m_comboSearchToolType.GetWindowText(strNew);
	

	CString strSelText = _T("");
	m_nCurSelSearchToolIdx = nCurSel;
	m_comboSearchToolType.GetLBText(nCurSel, strSelText);
	if (nCurSelSearchToolIdxOld != -1)
	{
		CString strLog,strOld;
		m_comboSearchToolType.GetLBText(nCurSelSearchToolIdxOld, strOld);
		strLog.Format(_T("修改[定位工具类型]由[%s]到[%s]\n"),strOld,strSelText);
		g_opLogFile.SaveLogText(strLog,5);
	}
		
	
	DestroyAllDlg();
// 	m_imageInput.Release();
	m_imageTrain.Release();


	m_bSearchDataUpdated = FALSE;
	m_bTrainDataUpdated = FALSE;
	
	BOOL bEnableSearchNum = TRUE;
// 	m_pParent->UpdateData(TRUE);
// 	if (m_pParent->m_nMarkAppType == ((int)eMAT_TwoCamera - 1))
// 	{
// 		bEnableSearchNum = FALSE;
// 	}
	

	//创建对话框
	CreateToolDlg(strSelText);
	//工具更新
	InitSearchTooParam(strSelText);


	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_pSearchTool->SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_pSearchTool->SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_pSearchTool->SetLanguage(1);
		break;
	}

	UpdateGuiDisplay();
}

void CDlgSearch::OnOK() 
{

	if (m_pSearchTool == NULL)
	{
		return;
	}
	
	CString strLog;
	strLog.Format(_T("点击[保存]\n"));
	g_opLogFile.SaveLogText(strLog, 5);
	strLog.Format(_T("退出模板设置\n"));
	g_opLogFile.SaveLogText(strLog, 4);

	// 更新搜索参数
	int nCurSel = m_comboSearchToolType.GetCurSel();
	CString strSelText = _T("");
	m_comboSearchToolType.GetLBText(nCurSel, strSelText);
	BOOL bInspectTool= FALSE;
	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL))
	{
		m_pDlgQuickSearchTool->CheckDlgDataValidation();
		m_pDlgQuickSearchTool->UpdateData(TRUE);
		m_pDlgQuickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL))
	{
		m_pDlgPatternSearchTool->CheckDlgDataValidation();
		m_pDlgPatternSearchTool->UpdateData(TRUE);
		m_pDlgPatternSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_SEARCH_TOOL))
	{
		m_pDlgLineSearchTool->CheckDlgDataValidation();
		m_pDlgLineSearchTool->UpdateData(TRUE);
		m_pDlgLineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL))
	{
		m_pDlgCornerSearchTool->CheckDlgDataValidation();
		m_pDlgCornerSearchTool->UpdateData(TRUE);
		m_pDlgCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgCircleSearchTool->CheckDlgDataValidation();
		m_pDlgCircleSearchTool->UpdateData(TRUE);
		m_pDlgCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CROSS_SEARCH_TOOL))
	{
		m_pDlgCrossSearchTool->CheckDlgDataValidation();
		m_pDlgCrossSearchTool->UpdateData(TRUE);
		m_pDlgCrossSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATCORNER_SEARCH_TOOL))
	{
		m_pDlgPatCornerSearchTool->CheckDlgDataValidation();
		m_pDlgPatCornerSearchTool->UpdateData(TRUE);
		m_pDlgPatCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PROCIRCLE_SEARCH_TOOL))
	{
		m_pDlgProCircleSearchTool->CheckDlgDataValidation();
		m_pDlgProCircleSearchTool->UpdateData(TRUE);
		m_pDlgProCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FIXEDPOS_SEARCH_TOOL))
	{
		m_pDlgFixedPosSearchTool->CheckDlgDataValidation();
		m_pDlgFixedPosSearchTool->UpdateData(TRUE);
		m_pDlgFixedPosSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DMCOLDE_CALIBRATE_TOOL))
	{
		m_pDlgDmCodeCalibrateTool->CheckDlgDataValidation();
		m_pDlgDmCodeCalibrateTool->UpdateData(TRUE);
		m_pDlgDmCodeCalibrateTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL))
	{
		m_pDlgPatternCornerSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCornerSearchTool->UpdateData(TRUE);
		m_pDlgPatternCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_QUICK_SEARCH_TOOL))
	{
		m_pDlgCombinedQuickSearchTool->CheckDlgDataValidation();
		m_pDlgCombinedQuickSearchTool->UpdateData(TRUE);
		m_pDlgCombinedQuickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL))
	{
		m_pDlgCombinedPatternSearchTool->CheckDlgDataValidation();
		m_pDlgCombinedPatternSearchTool->UpdateData(TRUE);
		m_pDlgCombinedPatternSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BASE_CORNER_SEARCH_TOOL))
	{
		m_pDlgBaseCornerSearchTool->CheckDlgDataValidation();
		m_pDlgBaseCornerSearchTool->UpdateData(TRUE);
		m_pDlgBaseCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_TOOL))
	{
		m_pDlgPatternSearchCheckTool->CheckDlgDataValidation();
		m_pDlgPatternSearchCheckTool->UpdateData(TRUE);
		m_pDlgPatternSearchCheckTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_TOOL))
	{
		m_pDlgQuickSearchCheckTool->CheckDlgDataValidation();
		m_pDlgQuickSearchCheckTool->UpdateData(TRUE);
		m_pDlgQuickSearchCheckTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_OBROUND_TOOL))
	{
		m_pDlgQuickSearchObroundTool->CheckDlgDataValidation();
		m_pDlgQuickSearchObroundTool->UpdateData(TRUE);
		m_pDlgQuickSearchObroundTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL))
	{
		m_pDlgPatternSearchObroundTool->CheckDlgDataValidation();
		m_pDlgPatternSearchObroundTool->UpdateData(TRUE);
		m_pDlgPatternSearchObroundTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_SEARCH_TOOL))
	{
		m_pDlgMPatternSearchTool->CheckDlgDataValidation();
		m_pDlgMPatternSearchTool->UpdateData(TRUE);
		m_pDlgMPatternSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_SEARCH_TOOL))
	{
		m_pDlgMQuickSearchTool->CheckDlgDataValidation();
		m_pDlgMQuickSearchTool->UpdateData(TRUE);
		m_pDlgMQuickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_CORNER_SEARCH_TOOL))
	{
		m_pDlgMQuickCornerSearchTool->CheckDlgDataValidation();
		m_pDlgMQuickCornerSearchTool->UpdateData(TRUE);
		m_pDlgMQuickCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL))
	{
		m_pDlgMQuickPickCornerSearchTool->CheckDlgDataValidation();
		m_pDlgMQuickPickCornerSearchTool->UpdateData(TRUE);
		m_pDlgMQuickPickCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_GENRECT_TOOL))
	{
		m_pDlgQuickSearchGenRectTool->CheckDlgDataValidation();
		m_pDlgQuickSearchGenRectTool->UpdateData(TRUE);
		m_pDlgQuickSearchGenRectTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL))
	{
		m_pDlgPatternSearchGenRectTool->CheckDlgDataValidation();
		m_pDlgPatternSearchGenRectTool->UpdateData(TRUE);
		m_pDlgPatternSearchGenRectTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_SEARCH_TOOL))
	{
		m_pDlgPatternLineSearchTool->CheckDlgDataValidation();
		m_pDlgPatternLineSearchTool->UpdateData(TRUE);
		m_pDlgPatternLineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_2PATTERN_SEARCH_TOOL))
	{
		m_pDlg2PatternSearchTool->CheckDlgDataValidation();
		m_pDlg2PatternSearchTool->UpdateData(TRUE);
		m_pDlg2PatternSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgQuickLineCircleSearchTool->CheckDlgDataValidation();
		m_pDlgQuickLineCircleSearchTool->UpdateData(TRUE);
		m_pDlgQuickLineCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternLineCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternLineCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternLineCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL))
	{
		m_pDlgQuickSearchCheckLineTool->CheckDlgDataValidation();
		m_pDlgQuickSearchCheckLineTool->UpdateData(TRUE);
		m_pDlgQuickSearchCheckLineTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL))
	{
		m_pDlgPatternSearchCheckLineTool->CheckDlgDataValidation();
		m_pDlgPatternSearchCheckLineTool->UpdateData(TRUE);
		m_pDlgPatternSearchCheckLineTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_BLOB_SEARCH_TOOL))
	{
		m_pDlgPatternBlobSearchTool->CheckDlgDataValidation();
		m_pDlgPatternBlobSearchTool->UpdateData(TRUE);
		m_pDlgPatternBlobSearchTool->UpdateSearchData(true);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_CHECK_TOOL))
	{
		m_pDlgCornerSearchCheckTool->CheckDlgDataValidation();
		m_pDlgCornerSearchCheckTool->UpdateData(TRUE);
		m_pDlgCornerSearchCheckTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL))
	{
		m_pDlgRectSearchTool->CheckDlgDataValidation();
		m_pDlgRectSearchTool->UpdateData(TRUE);
		m_pDlgRectSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_RECT_SEARCH_TOOL))
	{
		m_pDlgPatternRectSearchTool->CheckDlgDataValidation();
		m_pDlgPatternRectSearchTool->UpdateData(TRUE);
		m_pDlgPatternRectSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECT_TOOL))
	{
		bInspectTool = TRUE;
		m_pDlgAreaInspectTool->CheckDlgDataValidation();
		m_pDlgAreaInspectTool->UpdateData(TRUE);
		m_pDlgAreaInspectTool->UpdateSearchData(true);
		m_pSearchTool->SetTrainImage(m_imageInput);
		m_pSearchTool->Train();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECTEX_TOOL))
	{
		bInspectTool = TRUE;
		m_pDlgAreaInspectExTool->CheckDlgDataValidation();
		m_pDlgAreaInspectExTool->UpdateData(TRUE);
		m_pDlgAreaInspectExTool->UpdateSearchData(true);
		m_pSearchTool->SetTrainImage(m_imageInput);
		m_pSearchTool->Train();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERNCORNER_SEARCH_TOOL))
	{
		m_pDlgMPatternCornerSearchTool->CheckDlgDataValidation();
		m_pDlgMPatternCornerSearchTool->UpdateData(TRUE);
		m_pDlgMPatternCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL))
	{
		m_pDlgMPatternPickCornerSearchTool->CheckDlgDataValidation();
		m_pDlgMPatternPickCornerSearchTool->UpdateData(TRUE);
		m_pDlgMPatternPickCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL))
	{
		m_pDlgPatternDoubleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternDoubleSearchTool->UpdateData(TRUE);
		m_pDlgPatternDoubleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL))
	{
		m_pDlgTwoPatModelSearchTool->CheckDlgDataValidation();
		m_pDlgTwoPatModelSearchTool->UpdateData(TRUE);
		m_pDlgTwoPatModelSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CHORD_SEARCH_TOOL))
	{
		m_pDlgChordSearchTool->CheckDlgDataValidation();
		m_pDlgChordSearchTool->UpdateData(TRUE);
		m_pDlgChordSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCornerCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternCornerCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgQuickCornerCircleSearchTool->CheckDlgDataValidation();
		m_pDlgQuickCornerCircleSearchTool->UpdateData(TRUE);
		m_pDlgQuickCornerCircleSearchTool->UpdateSearchData(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL))
	{
		m_pDlgPatternPosLineDistanceTool->CheckDlgDataValidation();
		m_pDlgPatternPosLineDistanceTool->UpdateData(TRUE);
		m_pDlgPatternPosLineDistanceTool->UpdateSearchData(true);
	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL))
    {
		m_pDlgCornerSearchCheckInspectTool->CheckDlgDataValidation();
		m_pDlgCornerSearchCheckInspectTool->UpdateData(TRUE);
		m_pDlgCornerSearchCheckInspectTool->UpdateSearchData(true);
    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CHORD_SEARCH_TOOL))
    {
		m_pDlgPatternChordSearchTool->CheckDlgDataValidation();
		m_pDlgPatternChordSearchTool->UpdateData(TRUE);
		m_pDlgPatternChordSearchTool->UpdateSearchData(true);
    }
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL))
    {
		m_pDlgQuickMulCircleSearchTool->CheckDlgDataValidation();
		m_pDlgQuickMulCircleSearchTool->UpdateData(TRUE);
		m_pDlgQuickMulCircleSearchTool->UpdateSearchData(true);
    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pDlgPatternCircle4LineSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCircle4LineSearchTool->UpdateData(TRUE);
		m_pDlgPatternCircle4LineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pDlgQuickCircle4LineSearchTool->CheckDlgDataValidation();
		m_pDlgQuickCircle4LineSearchTool->UpdateData(TRUE);
		m_pDlgQuickCircle4LineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL))
	{
		m_pDlgSpecialPickSearchTool->CheckDlgDataValidation();
		m_pDlgSpecialPickSearchTool->UpdateData(TRUE);
		m_pDlgSpecialPickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerLineSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCornerLineSearchTool->UpdateData(TRUE);
		m_pDlgPatternCornerLineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternMulCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternMulCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternMulCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerMulCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCornerMulCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternCornerMulCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL))
	{
		m_pDlgPatternCornerMulObroundTool->CheckDlgDataValidation();
		m_pDlgPatternCornerMulObroundTool->UpdateData(TRUE);
		m_pDlgPatternCornerMulObroundTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL))
	{
		m_pDlgQuickCornerMulObroundTool->CheckDlgDataValidation();
		m_pDlgQuickCornerMulObroundTool->UpdateData(TRUE);
		m_pDlgQuickCornerMulObroundTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL))
	{
		m_pDlgCircleCornerSearchCheckInspectTool->CheckDlgDataValidation();
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateData(TRUE);
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchData(true);
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL))
	{
		bInspectTool = TRUE;
		m_pDlgACFInspectTool->CheckDlgDataValidation();
		m_pDlgACFInspectTool->UpdateData(TRUE);
		m_pDlgACFInspectTool->UpdateSearchData(true);
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pDlgVirtualRectSearchTool->CheckDlgDataValidation();
		m_pDlgVirtualRectSearchTool->UpdateData(TRUE);
		m_pDlgVirtualRectSearchTool->UpdateSearchData(true);
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pDlgPatternVirtualRectSearchTool->CheckDlgDataValidation();
		m_pDlgPatternVirtualRectSearchTool->UpdateData(TRUE);
		m_pDlgPatternVirtualRectSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL))
	{
		m_pDlgTwoQuickModelSearchTool->CheckDlgDataValidation();
		m_pDlgTwoQuickModelSearchTool->UpdateData(TRUE);
		m_pDlgTwoQuickModelSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_DMCODE_TOOL))
	{
		m_pDlgIdReaderDmCodeTool->CheckDlgDataValidation();
		m_pDlgIdReaderDmCodeTool->UpdateData(TRUE);
		m_pDlgIdReaderDmCodeTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_BARCODE_TOOL))
	{
		m_pDlgIdReaderBarCodeTool->CheckDlgDataValidation();
		m_pDlgIdReaderBarCodeTool->UpdateData(TRUE);
		m_pDlgIdReaderBarCodeTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_POSLINEDISTANCE_TOOL))
	{
		m_pDlgQuickPosLineDistanceTool->CheckDlgDataValidation();
		m_pDlgQuickPosLineDistanceTool->UpdateData(TRUE);
		m_pDlgQuickPosLineDistanceTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_CORNER_TOOL))
	{
		m_pDlgCombinedCornerSearchTool->CheckDlgDataValidation();
		m_pDlgCombinedCornerSearchTool->UpdateData(TRUE);
		m_pDlgCombinedCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_SEARCH_TOOL))
	{
		m_pDlgMQuickPickSearchTool->CheckDlgDataValidation();
		m_pDlgMQuickPickSearchTool->UpdateData(TRUE);
		m_pDlgMQuickPickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_PICK_SEARCH_TOOL))
	{
		m_pDlgMPatternPickSearchTool->CheckDlgDataValidation();
		m_pDlgMPatternPickSearchTool->UpdateData(TRUE);
		m_pDlgMPatternPickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREAINSPECT_PICK_TOOL))
	{
		bInspectTool = TRUE;
		m_pDlgAreaInspectPickTool->CheckDlgDataValidation();
		m_pDlgAreaInspectPickTool->UpdateData(TRUE);
		m_pDlgAreaInspectPickTool->UpdateSearchData(true);
		m_pSearchTool->SetTrainImage(m_imageInput);
		m_pSearchTool->Train();
	}
	// 搜索
	if (!m_pSearchTool->Search(m_imageInput) && (!bInspectTool))
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_FAILED));
		UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_FAILED));
		m_bSearchDataUpdated = FALSE;
		return;
	}
	else
	{
		if (m_pVisionASM!=NULL)
		{
			if (m_pVisionASM->IsSysMemoryUsedRateWarning())
			{
				CString strWarning ;
				strWarning = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_SAVE_SEARCHTOOL_FAIL_OF_MEM_OUT) + 
							_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
							_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION2);
				MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
				return;
			}
		}
		CDialog::OnOK();
	}	
}

void CDlgSearch::OnCancel() 
{
	if(m_bIsPreviewTool)
	{
		CDialog::OnCancel();
		return ;
	}

	CString strLog;
	strLog.Format(_T("点击[取消]\n"));
	g_opLogFile.SaveLogText(strLog, 5);
	strLog.Format(_T("退出模板设置\n"));
	g_opLogFile.SaveLogText(strLog, 4);

	if (m_pSearchTool != NULL)
	{
		if (m_bTrainDataUpdated || m_bSearchDataUpdated)
		{
			int nRet = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_SEARCH_TOOL_OR_NOT), MB_YESNOCANCEL);	//_T("是否保存对当前定位工具的修改？")
			if (nRet == IDYES)
			{
				OnOK();
				return;
			}
			else if (nRet == IDCANCEL)
			{
				return;
			}
		}
	}
	CDialog::OnCancel();
}

void CDlgSearch::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CString strLog;
	strLog.Format(_T("点击[关闭]\n"));
	g_opLogFile.SaveLogText(strLog, 5);
	strLog.Format(_T("退出模板设置"));
	g_opLogFile.SaveLogText(strLog, 4);

	CDialog::OnClose();
}


void CDlgSearch::OnBnClickedBtnLoadImage2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[加载多]"));
	g_opLogFile.SaveLogText(strLog,6);
	// TODO: Add your control notification handler code here
	//CString strFilter = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FILTER_BMP_FILE) + _T("|*.bmp||");
	CString strFilter = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FILTER_BMP_AND_JPEG_FILE);
	CFileDialog cFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	//	cFileDlg.m_ofn.lpstrInitialDir = "D:\\";
	CString strCurrentFileName = "";
	if (IDOK == cFileDlg.DoModal())
	{
		m_vectorTest1FileNames.clear();
		m_nTestImage1total = 0;
		m_nTestImage1Index = -1;
		m_nTestImageIndexStatus = 0;

		//scDIB cDib;
		CString strPath = cFileDlg.GetPathName();
		if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".bmp"))
		{
			scDIB cDib;
			cDib.Init(strPath);
			cDib.GetImage(m_imageInput);
		}
		else if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".jpg"))
		{
			scJpeg cJpeg;
			cJpeg.Read(strPath,m_imageInput);
		}
		else if (strPath.Mid(strPath.ReverseFind('.'),4)==_T(".png"))
		{
			scPng cPng;
			cPng.Read(strPath,m_imageInput);
		}
		//cDib.Init(strPath);
		//cDib.GetImage(m_imageInput);

		m_imageTrain.Release();
		//	m_imageTrain2.Release();
		//	m_imageTrain3.Release();
		m_imageResult.Release();
		m_comboDisplayType.SetCurSel(0);
		UpdateGuiDisplay();

		strCurrentFileName = cFileDlg.GetPathName();
//		m_vectorTest1FileNames.push_back(strCurrentFileName);

		CString strCurrentPath = strCurrentFileName;
		strCurrentPath = strCurrentPath.Left(strCurrentPath.ReverseFind('\\')) + _T("\\");

		CFileFind FileObj;

		int i = 0;
		BOOL res = FileObj.FindFile(strCurrentFileName);
		res = FileObj.FindNextFile();
		CString	sFile = FileObj.GetRoot();
		sFile 	+= _T("\\*.*");
		res = FileObj.FindFile(sFile);
		while (res)
		{
			res = FileObj.FindNextFile();	

			CString strTempCurrentFileName = FileObj.GetFileName();
			int nlenght = strTempCurrentFileName.GetLength();
			//if(FileObj.IsArchived() && strTempCurrentFileName.Mid(nlenght - 3, 3) == "bmp")
			if(FileObj.IsArchived() && (strTempCurrentFileName.Mid(nlenght - 3, 3) == "bmp" 
									 || strTempCurrentFileName.Mid(nlenght - 3, 3) == "jpg" 
									 || strTempCurrentFileName.Mid(nlenght - 3, 3) == "png"))
			{
				m_nTestImage1total++;
				m_vectorTest1FileNames.push_back(strCurrentPath+strTempCurrentFileName);
				if (strCurrentFileName == strCurrentPath+strTempCurrentFileName)//2018-8-24 [qw]
				{
					m_nTestImage1Index=m_nTestImage1total-1;
				}
			}

		}
		FileObj.Close();

		m_bStatusBarFlag = TRUE;
		m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_LOAD_IMAGE_SUCCEED);	//_T("图像加载成功");
		m_strStatusBarInfo2 = strPath;
		UpdateStatusBar();
	}
}


void CDlgSearch::OnBnClickedBtnSearchExe2()
{
	// TODO: 在此添加控件通知处理程序代码
	//for (int g=0;g<1000;g++)
	CString strLog;
	strLog.Format(_T("点击[向后搜索]"));
	g_opLogFile.SaveLogText(strLog,6);

	{
		if (m_pSearchTool == NULL)
		{
			return;
		}
		// 更新搜索参数
		int nCurSel = m_comboSearchToolType.GetCurSel();
		CString strSelText = _T("");
		m_comboSearchToolType.GetLBText(nCurSel, strSelText);
		UpdateDlgSearchToolData(strSelText);
		
		if (m_nTestImageIndexStatus == 0)
		{
		}
		else if (m_nTestImageIndexStatus == 1)
		{
		}
		else if (m_nTestImageIndexStatus == 2)
		{
			m_nTestImage1Index = m_nTestImage1Index + 1;
		}

		int nNextIndex = m_nTestImage1Index + 1;
		if(nNextIndex > m_nTestImage1total)
		{
			m_nTestImageIndexStatus = 1;
			AfxMessageBox("Search Completed!");
			return;
		}
		if (m_nTestImage1Index < 0)
		{
			return;
		}
		
		CString str = m_vectorTest1FileNames[m_nTestImage1Index];
		if (str.Mid(str.ReverseFind('.'),4)==_T(".bmp"))
		{
			scDIB cDib;
			cDib.Init(str);
			cDib.GetImage(m_imageInput);
		}
		else if (str.Mid(str.ReverseFind('.'),4)==_T(".jpg"))
		{
			scJpeg cJpeg;
			cJpeg.Read(str,m_imageInput);
		}
		else if (str.Mid(str.ReverseFind('.'),4)==_T(".png"))
		{
			scPng cPng;
			cPng.Read(str,m_imageInput);
		}
		//scDIB dib;
		////从BMP格式文件读取数据初始化scDIB对象
		//dib.Init(m_vectorTest1FileNames[m_nTestImage1Index]);
		//// 按照alignModulus像素对齐，将图像数据保存到image结构中
		//dib.GetImage(m_imageInput, 1);

		CString strFile = str;

		strFile = strFile.Right(strFile.GetLength() - strFile.ReverseFind('\\') -1);

		BOOL bInspectTool = FALSE;
		bInspectTool = (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECT_TOOL))|| (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECTEX_TOOL));

		scTimer t;
		// 搜索
		bool bSearchSuccess = false;

		cpImage imageTmp;
		if (eIdReaderDmCodeTool == m_pSearchTool->GetSearchToolType())
		{
			m_imageInput.Clone(imageTmp);
			bSearchSuccess = (m_pSearchTool->Search(imageTmp)|| bInspectTool);
			imageTmp.Release();
		}
		else
		{
			bSearchSuccess = (m_pSearchTool->Search(m_imageInput)|| bInspectTool );
		}

		if (bSearchSuccess)
		{
			CString strInfo;

			m_imageResult = m_imageInput;
			m_comboDisplayType.SetCurSel(2);
			UpdateGuiDisplay();
			//		GetDlgItem(IDOK)->EnableWindow(TRUE);

			if (m_pSearchTool->GetResultNum())
			{
				CSearchResult result;
				m_pSearchTool->GetResult(0, result);
				strInfo.Format(_T("X:%.1f,Y:%.1f,D:%.3f,S:%.2f,T:%.1fms"), 
					result.m_dPosX, result.m_dPosY, result.m_dAngle,result.m_dScore,t.GetTimeMilli(TRUE));
			}

			// 显示结果
			UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_SUCCEED) + ":" + strFile,strInfo);
			m_bSearchDataUpdated = TRUE;

			AlignToolLogRecord(strFile+strInfo,0);
		}
		else
		{
// 			CString strInfo;
// 			strInfo.Format(_T("Err:%d"), m_pSearchTool->GetResultErrNumber());

			m_imageResult = m_imageInput;
			m_comboDisplayType.SetCurSel(2);

			CString strErrInfo =  m_pSearchTool->GetResultErrInfo();
			UpdateGuiDisplay();
			UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_FAILED) + ":" + strFile,strErrInfo);
			m_bSearchDataUpdated = FALSE;  

			AlignToolLogRecord(strFile+strErrInfo,0);
		}


		if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECT_TOOL))
		{
			m_pDlgAreaInspectTool->UpdateSearchResult();
		}
		if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECTEX_TOOL))
		{
			m_pDlgAreaInspectExTool->UpdateSearchResult();
		}
        if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL))
        {
            m_pDlgCornerSearchCheckInspectTool->UpdateSearchData(false);
        }
		if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL))
		{
			m_pDlgSpecialPickSearchTool->UpdateSearchData(false);
		}
		if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL))
		{
			m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchData(false);
			m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchResult();
		}
		if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL))
		{
			m_pDlgACFInspectTool->UpdateSearchData(false);
			m_pDlgACFInspectTool->UpdateSearchResult();
		}
		if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL))
		{
			m_pDlgCircleSearchTool->UpdateSearchResult();
		}
		if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL))
		{
			m_pDlgQuickMulCircleSearchTool->UpdateSearchResult();
		}
		if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREAINSPECT_PICK_TOOL))
		{
			m_pDlgAreaInspectPickTool->UpdateSearchResult();
		}
		m_nTestImage1Index++;
		m_nTestImageIndexStatus = 1;
	}
}


// void CDlgSearch::OnBnClickedOk()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CDialog::OnOK();
// }


void CDlgSearch::OnBnClickedBtnSearchExe3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[向前搜索]"));
	g_opLogFile.SaveLogText(strLog,6);
	{
		if (m_pSearchTool == NULL)
		{
			return;
		}
		// 更新搜索参数
		int nCurSel = m_comboSearchToolType.GetCurSel();
		CString strSelText = _T("");
		m_comboSearchToolType.GetLBText(nCurSel, strSelText);
		UpdateDlgSearchToolData(strSelText);

		if (m_nTestImageIndexStatus == 0)
		{
		}
		else if (m_nTestImageIndexStatus == 1)
		{
			m_nTestImage1Index = m_nTestImage1Index - 2;
		}
		else if (m_nTestImageIndexStatus == 2)
		{

		}

		int nNextIndex = m_nTestImage1Index;
		if(nNextIndex < 0)
		{
			m_nTestImageIndexStatus = 2;
			AfxMessageBox("Search Completed!");
			return;
		}
		if (m_nTestImage1Index < 0)
		{
			return;
		}

		CString str = m_vectorTest1FileNames[m_nTestImage1Index];
		if (str.Mid(str.ReverseFind('.'),4)==_T(".bmp"))
		{
			scDIB cDib;
			cDib.Init(str);
			cDib.GetImage(m_imageInput);
		}
		else if (str.Mid(str.ReverseFind('.'),4)==_T(".jpg"))
		{
			scJpeg cJpeg;
			cJpeg.Read(str,m_imageInput);
		}
		else if (str.Mid(str.ReverseFind('.'),4)==_T(".png"))
		{
			scPng cPng;
			cPng.Read(str,m_imageInput);
		}
		//scDIB dib;
		////从BMP格式文件读取数据初始化scDIB对象
		//dib.Init(m_vectorTest1FileNames[m_nTestImage1Index]);
		//// 按照alignModulus像素对齐，将图像数据保存到image结构中
		//dib.GetImage(m_imageInput, 1);

		CString strFile = str;

		strFile = strFile.Right(strFile.GetLength() - strFile.ReverseFind('\\') -1);

		BOOL bInspectTool = FALSE;
		bInspectTool = (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECT_TOOL))|| (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECTEX_TOOL));

		scTimer t;
		// 搜索
		bool bSearchSuccess = false;

		cpImage imageTmp;
		if (eIdReaderDmCodeTool == m_pSearchTool->GetSearchToolType())
		{
			m_imageInput.Clone(imageTmp);
			bSearchSuccess = (m_pSearchTool->Search(imageTmp)|| bInspectTool);
			imageTmp.Release();
		}
		else
		{
			bSearchSuccess = (m_pSearchTool->Search(m_imageInput)|| bInspectTool );
		}

		if (bSearchSuccess)
		{
			CString strInfo;

			m_imageResult = m_imageInput;
			m_comboDisplayType.SetCurSel(2);
			UpdateGuiDisplay();
			//		GetDlgItem(IDOK)->EnableWindow(TRUE);

			if (m_pSearchTool->GetResultNum())
			{
				CSearchResult result;
				m_pSearchTool->GetResult(0, result);
				strInfo.Format(_T("X:%.1f,Y:%.1f,D:%.3f,S:%.2f,T:%.1fms"), 
					result.m_dPosX, result.m_dPosY, result.m_dAngle,result.m_dScore,t.GetTimeMilli(TRUE));
			}

			// 显示结果
			UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_SUCCEED) + ":" + strFile,strInfo);
			m_bSearchDataUpdated = TRUE;

			AlignToolLogRecord(strFile+strInfo,0);
		}
		else
		{

			m_imageResult = m_imageInput;
			m_comboDisplayType.SetCurSel(2);

			CString strErrInfo =  m_pSearchTool->GetResultErrInfo();

			UpdateGuiDisplay();
			UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_FAILED) + ":" + strFile,strErrInfo);
			m_bSearchDataUpdated = FALSE;

			AlignToolLogRecord(strFile+strErrInfo,0);
		}


		if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECT_TOOL))
		{
			m_pDlgAreaInspectTool->UpdateSearchResult();
		}
		if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREA_INSPECTEX_TOOL))
		{
			m_pDlgAreaInspectExTool->UpdateSearchResult();
		}
		if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL))
		{
			m_pDlgPatternPosLineDistanceTool->UpdateSearchResult();
		}
        if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL))
        {
            m_pDlgCornerSearchCheckInspectTool->UpdateSearchData(false);
        }
		if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL))
		{
			m_pDlgSpecialPickSearchTool->UpdateSearchData(false);
		}
		if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL))
		{
			m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchData(false);
			m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchResult();
		}
		if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL))
		{
			m_pDlgACFInspectTool->UpdateSearchData(false);
			m_pDlgACFInspectTool->UpdateSearchResult();
		}
		if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL))
		{
			m_pDlgCircleSearchTool->UpdateSearchResult();
		}
		if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL))
		{
			m_pDlgQuickMulCircleSearchTool->UpdateSearchResult();
		}
		if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_QUICK_POSLINEDISTANCE_TOOL))
		{
			m_pDlgQuickPosLineDistanceTool->UpdateSearchResult();
		}
		if (strSelText == m_psaSysInfoStrings->GetAt(IDS_CMB_AREAINSPECT_PICK_TOOL))
		{
			m_pDlgAreaInspectPickTool->UpdateSearchResult();
		}
		m_nTestImage1Index--;
		m_nTestImageIndexStatus = 2;
	}
}


void CDlgSearch::OnBnClickedBtnGrabSearchImage()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[连续采集并搜索]"));
	g_opLogFile.SaveLogText(strLog,6);

	BOOL bSysKeyboardEnabled = m_bSysKeyboardEnabled;
	m_bSysKeyboardEnabled = FALSE;

	m_strStatusBarInfo2 = _T("");
	if (m_pVisionASM != NULL)
	{
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		if (m_pVisionASM->IsPosCameraOnline(m_nCurPositionIdx))
		{
			m_comboDisplayType.SetCurSel(0);
			if (m_pVisionASM->IsPosCameraGrabbing(m_nCurPositionIdx))
			{
				// 关闭定时器
				KillTimer(1);

				if (m_pVisionASM->FreezePosCamera(m_nCurPositionIdx))
				{
					m_imageTrain.Release();
					m_imageResult.Release();
					cpImage* pImageTemp = (cpImage*)m_guiDisplay.GetImageCP();
					if (pImageTemp != NULL)
					{
						pImageTemp->Clone(m_imageInput);	
					}
					SetDlgItemText(IDC_BTN_GRAB_SEARCH_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_SEARCH_IMAGE));	//_T("采集并定位"));
					GetDlgItem(IDC_BTN_GRAB_IMAGE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_SEARCH_EXE)->EnableWindow(TRUE);
					GetDlgItem(IDC_CMB_SEARCH_TOOL_TYPE)->EnableWindow(TRUE);	
					GetDlgItem(IDOK)->EnableWindow(TRUE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
					m_comboDisplayType.EnableWindow(TRUE);

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
				BOOL bEnableShutter = optionInfo.m_bEnableSearchShutter;
				if (bEnableShutter && (m_bEnableSearchShutter.m_bShutterEnable || m_bEnableSearchShutter.m_bGainEnable || m_bEnableSearchShutter.m_bGammaEnable))
				{
					m_pVisionASM->SetPosCameraShutter(m_nCurPositionIdx, m_dProductSearchShutter.m_dShutter);
					m_pVisionASM->UpdatePosCameraShutter(m_nCurPositionIdx);
					m_pVisionASM->SetPosCameraGain(m_nCurPositionIdx, m_dProductSearchShutter.m_dGain);
					m_pVisionASM->UpdatePosCameraGain(m_nCurPositionIdx);
					ImageRoi imageRoi;
					m_pVisionASM->GetPosImageROIParam(m_nCurPositionIdx, imageRoi);
					imageRoi.m_dGamma = m_dProductSearchShutter.m_dGamma;
					m_pVisionASM->SetPosImageROIParam(m_nCurPositionIdx,imageRoi);
				}
				if (m_pVisionASM->GrabPosCamera(m_nCurPositionIdx))
				{
					// 设置定时器
					SetTimer(1, 10, NULL);

					SetDlgItemText(IDC_BTN_GRAB_SEARCH_IMAGE, GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB_SEARCH_IMAGE));	//_T("停止采集定位"));
					GetDlgItem(IDC_BTN_GRAB_IMAGE)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_LOAD_IMAGE)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_SEARCH_EXE)->EnableWindow(FALSE);
					GetDlgItem(IDC_CMB_SEARCH_TOOL_TYPE)->EnableWindow(FALSE);					
					GetDlgItem(IDOK)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
					m_comboDisplayType.EnableWindow(FALSE);

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

	CString strSelText = _T("");
	int nCurSel;
	nCurSel = m_comboSearchToolType.GetCurSel();
	m_comboSearchToolType.GetLBText(nCurSel,strSelText);
	//更新控件显示
	UpdateToolDlgControlDisplay(strSelText);

	m_bSysKeyboardEnabled = bSysKeyboardEnabled;

	UpdateStatusBar();
}


void CDlgSearch::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		if (m_pVisionASM != NULL)
		{
			if (m_pVisionASM->IsPosCameraOnline(m_nCurPositionIdx))
			{
				if (m_pVisionASM->IsPosCameraGrabbing(m_nCurPositionIdx))
				{
					// 获取输入图像
					cpImage* pImageTemp = (cpImage*)m_guiDisplay.GetImageCP();
					if (pImageTemp != NULL)
					{
						pImageTemp->Clone(m_imageInput);	
					}

					// 执行搜索
					OnBtnSearchExe();
				}
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

// void CDlgSearch::SetEnableSearchShutter(BOOL bEnableSearchShutter)
// {
// 	m_bEnableSearchShutter = bEnableSearchShutter;
// }
// 
// BOOL CDlgSearch::GetEnableSearchShutter()
// {
// 	return m_bEnableSearchShutter;
// }

// double CDlgSearch::GetProductSearchShutter()
// {
// 	return m_dProductSearchShutter;	
// }
// 
// void CDlgSearch::SetProductSearchShutter( double dShutter )
// {
// 	m_dProductSearchShutter = dShutter;
// }


CameraParamSet CDlgSearch::GetProductSearchShutterInfoShutter()
{
	if(m_dProductSearchShutter.m_dShutter < 0.001)
	{
		return m_dEnvironmentShutter;
	}
	else
	{
		return m_dProductSearchShutter;
	}
}

CameraParamEnable CDlgSearch::GetProductSearchShutterInfoEnable()
{
	return m_bEnableSearchShutter;
}

void CDlgSearch::SetProductSearchShutterInfo(CameraParamEnable bEnable, CameraParamSet dShutter)
{
	m_bEnableSearchShutter = bEnable;

	if(dShutter.m_dShutter < 0.0011)
	{
		m_dProductSearchShutter.m_dShutter = m_dEnvironmentShutter.m_dShutter;
	}
	else
	{
		m_dProductSearchShutter.m_dShutter = dShutter.m_dShutter;
	}

	/*if(dShutter.m_dGain < 0.001)
	{
		m_dProductSearchShutter.m_dGain = m_dEnvironmentShutter.m_dGain;
	}
	else
	{
		m_dProductSearchShutter.m_dGain = dShutter.m_dGain;
	}*/
	m_dProductSearchShutter.m_dGain = dShutter.m_dGain;
	m_dProductSearchShutter.m_dGamma = m_dEnvironmentShutter.m_dGamma;

	//if(dShutter.m_dGamma < 1.01)
	//{
	//	m_dProductSearchShutter.m_dGamma = m_dEnvironmentShutter.m_dGamma;
	//}
	//else
	//{
	//	m_dProductSearchShutter.m_dGamma = dShutter.m_dGamma;
	//}
}


void CDlgSearch::OnBnClickedBtnProductShutterSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[曝光设置]"));
	g_opLogFile.SaveLogText(strLog,6);

	CameraParam para;
	ImageRoi imageRoi;

	bool bGetPosCameraParam = m_pVisionASM->GetPosCameraParam(m_nCurPositionIdx, para);
	bool bGetImageROIParam = m_pVisionASM->GetPosImageROIParam(m_nCurPositionIdx, imageRoi);
	if (!bGetPosCameraParam || !bGetImageROIParam)
	{
		AfxMessageBox(_T("获取相机参数错误！"));
		return;
	}


	CDlgProductShutter dlg(this);
	dlg.m_dEnvironmentShutter = m_dEnvironmentShutter;
	dlg.m_bEnableSearchShutter = m_bEnableSearchShutter;

	if (dlg.DoModal() == IDOK)
	{
		m_dProductSearchShutter = dlg.m_dProductShutter;
	}
	else
	{
		m_pVisionASM->SetPosCameraParam(m_nCurPositionIdx, para);
		m_pVisionASM->UpdatePosCameraShutter(m_nCurPositionIdx);
		m_dProductSearchShutter.m_dShutter = para.m_dShutter;

		m_pVisionASM->UpdatePosCameraGain(m_nCurPositionIdx);
		m_dProductSearchShutter.m_dGain = para.m_dGain;

		m_pVisionASM->SetPosImageROIParam(m_nCurPositionIdx,imageRoi);
		m_dProductSearchShutter.m_dGamma =imageRoi.m_dGamma;

	}

}

void CDlgSearch::DestroyAllDlg()
{
	if(m_pDlgQuickSearchTool->GetSafeHwnd())
	{
		m_pDlgQuickSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickSearchTool->DestroyWindow();
	}
	if(m_pDlgPatternSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternSearchTool->DestroyWindow();
	}
	if(m_pDlgLineSearchTool->GetSafeHwnd())
	{
		m_pDlgLineSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgLineSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgLineSearchTool->DestroyWindow();
	}
	if(m_pDlgCornerSearchTool->GetSafeHwnd())
	{
		m_pDlgCornerSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgCornerSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgCornerSearchTool->DestroyWindow();
	}
	if(m_pDlgCircleSearchTool->GetSafeHwnd())
	{
		m_pDlgCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgCircleSearchTool->DestroyWindow();
	}
	if(m_pDlgCrossSearchTool->GetSafeHwnd())
	{
		m_pDlgCrossSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgCrossSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgCrossSearchTool->DestroyWindow();
	}
	if(m_pDlgPatCornerSearchTool->GetSafeHwnd())
	{
		m_pDlgPatCornerSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatCornerSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatCornerSearchTool->m_GuiPatternDisplay2.RemoveImage();
		m_pDlgPatCornerSearchTool->m_GuiPatternDisplay2.ClearScreen();
		m_pDlgPatCornerSearchTool->DestroyWindow();
	}
	if(m_pDlgProCircleSearchTool->GetSafeHwnd())
	{
		m_pDlgProCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgProCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgProCircleSearchTool->DestroyWindow();
	}
	if(m_pDlgFixedPosSearchTool->GetSafeHwnd())
	{
		m_pDlgFixedPosSearchTool->DestroyWindow();
	}						
	if(m_pDlgDmCodeCalibrateTool->GetSafeHwnd())
	{
		m_pDlgDmCodeCalibrateTool->DestroyWindow();
	}
	if(m_pDlgPatternCornerSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternCornerSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCornerSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternCornerSearchTool->DestroyWindow();
	}
	if(m_pDlgCombinedQuickSearchTool->GetSafeHwnd())
	{
		m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay2.RemoveImage();
		m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay2.ClearScreen();
		m_pDlgCombinedQuickSearchTool->DestroyWindow();
	}
	if(m_pDlgCombinedPatternSearchTool->GetSafeHwnd())
	{
		m_pDlgCombinedPatternSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgCombinedPatternSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgCombinedPatternSearchTool->m_GuiPatternDisplay2.RemoveImage();
		m_pDlgCombinedPatternSearchTool->m_GuiPatternDisplay2.ClearScreen();
		m_pDlgCombinedPatternSearchTool->DestroyWindow();
	}
	if(m_pDlgBaseCornerSearchTool->GetSafeHwnd())
	{
		m_pDlgBaseCornerSearchTool->DestroyWindow();
	}
	if(m_pDlgPatternSearchCheckTool->GetSafeHwnd())
	{
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplayCheck1.ClearScreen();
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplayCheck2.ClearScreen();
		m_pDlgPatternSearchCheckTool->DestroyWindow();
	}
	if(m_pDlgQuickSearchCheckTool->GetSafeHwnd())
	{
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck1.ClearScreen();
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck2.ClearScreen();
		m_pDlgQuickSearchCheckTool->DestroyWindow();
	}
	if(m_pDlgQuickSearchObroundTool->GetSafeHwnd())
	{
		m_pDlgQuickSearchObroundTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickSearchObroundTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickSearchObroundTool->DestroyWindow();
	}
	if(m_pDlgPatternSearchObroundTool->GetSafeHwnd())
	{
		m_pDlgPatternSearchObroundTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternSearchObroundTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternSearchObroundTool->DestroyWindow();
	}
	if(m_pDlgMPatternSearchTool->GetSafeHwnd())
	{
		m_pDlgMPatternSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMPatternSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgMPatternSearchTool->DestroyWindow();
	}
	if(m_pDlgMQuickSearchTool->GetSafeHwnd())
	{
		m_pDlgMQuickSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMQuickSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgMQuickSearchTool->DestroyWindow();
	}
	if(m_pDlgMQuickCornerSearchTool->GetSafeHwnd())
	{
		m_pDlgMQuickCornerSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMQuickCornerSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgMQuickCornerSearchTool->DestroyWindow();
	}
	if(m_pDlgMQuickPickCornerSearchTool->GetSafeHwnd())
	{
		m_pDlgMQuickPickCornerSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMQuickPickCornerSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgMQuickPickCornerSearchTool->DestroyWindow();
	}
	if(m_pDlgQuickSearchGenRectTool->GetSafeHwnd())
	{
		m_pDlgQuickSearchGenRectTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickSearchGenRectTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickSearchGenRectTool->DestroyWindow();
	}
	if(m_pDlgPatternSearchGenRectTool->GetSafeHwnd())
	{
		m_pDlgPatternSearchGenRectTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternSearchGenRectTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternSearchGenRectTool->DestroyWindow();
	}
	if(m_pDlgPatternLineSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternLineSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternLineSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternLineSearchTool->DestroyWindow();
	}
	if(m_pDlg2PatternSearchTool->GetSafeHwnd())
	{
		m_pDlg2PatternSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlg2PatternSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlg2PatternSearchTool->m_GuiPatternDisplay2.RemoveImage();
		m_pDlg2PatternSearchTool->m_GuiPatternDisplay2.ClearScreen();
		m_pDlg2PatternSearchTool->DestroyWindow();
	}
	if(m_pDlgQuickLineCircleSearchTool->GetSafeHwnd())
	{
		m_pDlgQuickLineCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickLineCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickLineCircleSearchTool->DestroyWindow();
	}
	if(m_pDlgPatternLineCircleSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternLineCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternLineCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternLineCircleSearchTool->DestroyWindow();
	}
	if(m_pDlgPatternCircleSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternCircleSearchTool->DestroyWindow();
	}
	if(m_pDlgQuickSearchCheckLineTool->GetSafeHwnd())
	{
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck1.ClearScreen();
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck2.ClearScreen();
		m_pDlgQuickSearchCheckLineTool->DestroyWindow();
	}
	if(m_pDlgPatternSearchCheckLineTool->GetSafeHwnd())
	{
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplayCheck1.ClearScreen();
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplayCheck2.ClearScreen();
		m_pDlgPatternSearchCheckLineTool->DestroyWindow();
	}
	if(m_pDlgPatternBlobSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternBlobSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternBlobSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternBlobSearchTool->DestroyWindow();
	}
	if(m_pDlgCornerSearchCheckTool->GetSafeHwnd())
	{
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck1.ClearScreen();
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck2.ClearScreen();
		m_pDlgCornerSearchCheckTool->DestroyWindow();
	}
	if(m_pDlgRectSearchTool->GetSafeHwnd())
	{
		m_pDlgRectSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgRectSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgRectSearchTool->DestroyWindow();
	}
	if(m_pDlgPatternRectSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternRectSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternRectSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternRectSearchTool->DestroyWindow();
	}
	
	if(m_pDlgAreaInspectTool->GetSafeHwnd())
	{
// 		m_dlgAreaInspectTool->m_GuiPatternDisplay.RemoveImage();
// 		m_dlgAreaInspectTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgAreaInspectTool->DestroyWindow();
	}
	
	if(m_pDlgAreaInspectExTool->GetSafeHwnd())
	{
// 		m_dlgAreaInspectExTool->m_GuiPatternDisplay.RemoveImage();
// 		m_dlgAreaInspectExTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgAreaInspectExTool->DestroyWindow();
	}

	if(m_pDlgChordSearchTool->GetSafeHwnd())
	{
		m_pDlgChordSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgChordSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgChordSearchTool->DestroyWindow();
	}

	if(m_pDlgTwoPatModelSearchTool->GetSafeHwnd())
	{
		m_pDlgTwoPatModelSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgTwoPatModelSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgTwoPatModelSearchTool->m_GuiPatternDisplay2.RemoveImage();
		m_pDlgTwoPatModelSearchTool->m_GuiPatternDisplay2.ClearScreen();
		m_pDlgTwoPatModelSearchTool->DestroyWindow();
	}

	if (m_pDlgMPatternCornerSearchTool->GetSafeHwnd())
	{
		m_pDlgMPatternCornerSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMPatternCornerSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgMPatternCornerSearchTool->DestroyWindow();
	}
	if (m_pDlgMPatternPickCornerSearchTool->GetSafeHwnd())
	{
		m_pDlgMPatternPickCornerSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMPatternPickCornerSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgMPatternPickCornerSearchTool->DestroyWindow();
	}
	if (m_pDlgPatternCornerCircleSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternCornerCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCornerCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternCornerCircleSearchTool->DestroyWindow();
	}
	if (m_pDlgPatternDoubleSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternDoubleSearchTool->m_GuiPatternDisplay1.RemoveImage();
		m_pDlgPatternDoubleSearchTool->m_GuiPatternDisplay1.ClearScreen();
		m_pDlgPatternDoubleSearchTool->m_GuiPatternDisplay2.RemoveImage();
		m_pDlgPatternDoubleSearchTool->m_GuiPatternDisplay2.ClearScreen();
		m_pDlgPatternDoubleSearchTool->DestroyWindow();
	}
    if (m_pDlgQuickMulCircleSearchTool->GetSafeHwnd())
    {
        m_pDlgQuickMulCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
        m_pDlgQuickMulCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
        m_pDlgQuickMulCircleSearchTool->DestroyWindow();
    }
	if (m_pDlgPatternChordSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternChordSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternChordSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternChordSearchTool->DestroyWindow();
	}
	if(m_pDlgQuickCornerCircleSearchTool->GetSafeHwnd())
	{
		m_pDlgQuickCornerCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickCornerCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickCornerCircleSearchTool->DestroyWindow();
	}
	if(m_pDlgPatternCornerMulObroundTool->GetSafeHwnd())
	{
		m_pDlgPatternCornerMulObroundTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCornerMulObroundTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternCornerMulObroundTool->DestroyWindow();
	}
	if(m_pDlgQuickCornerMulObroundTool->GetSafeHwnd())
	{
		m_pDlgQuickCornerMulObroundTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickCornerMulObroundTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickCornerMulObroundTool->DestroyWindow();
	}

	if(m_pDlgPatternPosLineDistanceTool->GetSafeHwnd())
	{
		m_pDlgPatternPosLineDistanceTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternPosLineDistanceTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternPosLineDistanceTool->DestroyWindow();
	}
    if(m_pDlgCornerSearchCheckInspectTool->GetSafeHwnd())
    {
        m_pDlgCornerSearchCheckInspectTool->RemoveImage();
        m_pDlgCornerSearchCheckInspectTool->ClearScreen();
        m_pDlgCornerSearchCheckInspectTool->DestroyWindow();
    }
	if(m_pDlgPatternCircle4LineSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternCircle4LineSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCircle4LineSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternCircle4LineSearchTool->DestroyWindow();
	}

	if(m_pDlgQuickCircle4LineSearchTool->GetSafeHwnd())
	{
		m_pDlgQuickCircle4LineSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickCircle4LineSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickCircle4LineSearchTool->DestroyWindow();
	}
	if(m_pDlgSpecialPickSearchTool->GetSafeHwnd())
	{
		m_pDlgSpecialPickSearchTool->RemoveImage();
		m_pDlgSpecialPickSearchTool->ClearScreen();
		m_pDlgSpecialPickSearchTool->DestroyWindow();
	}
	if (m_pDlgPatternCornerLineSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternCornerLineSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCornerLineSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternCornerLineSearchTool->DestroyWindow();
	}
	if (m_pDlgPatternMulCircleSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternMulCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternMulCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternMulCircleSearchTool->DestroyWindow();
	}
	if (m_pDlgPatternCornerMulCircleSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternCornerMulCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCornerMulCircleSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternCornerMulCircleSearchTool->DestroyWindow();
	}
	if(m_pDlgCircleCornerSearchCheckInspectTool->GetSafeHwnd())
	{
		
		m_pDlgCircleCornerSearchCheckInspectTool->RemoveImage();
		m_pDlgCircleCornerSearchCheckInspectTool->ClearScreen();
		m_pDlgCircleCornerSearchCheckInspectTool->DestroyWindow();
	}
	if(m_pDlgACFInspectTool->GetSafeHwnd())
	{
		m_pDlgACFInspectTool->RemoveImage();
		m_pDlgACFInspectTool->ClearScreen();
		m_pDlgACFInspectTool->DestroyWindow();
	}
	if (m_pDlgVirtualRectSearchTool->GetSafeHwnd())
	{
		m_pDlgVirtualRectSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgVirtualRectSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgVirtualRectSearchTool->DestroyWindow();
	}
	if (m_pDlgPatternVirtualRectSearchTool->GetSafeHwnd())
	{
		m_pDlgPatternVirtualRectSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternVirtualRectSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgPatternVirtualRectSearchTool->DestroyWindow();
	}
	if(m_pDlgTwoQuickModelSearchTool->GetSafeHwnd())
	{
		m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay2.RemoveImage();
		m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay2.ClearScreen();
		m_pDlgTwoQuickModelSearchTool->DestroyWindow();
	}
	if(m_pDlgQuickPosLineDistanceTool->GetSafeHwnd())
	{
		m_pDlgQuickPosLineDistanceTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickPosLineDistanceTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgQuickPosLineDistanceTool->DestroyWindow();
	}
	if(m_pDlgCombinedCornerSearchTool->GetSafeHwnd())
    {
        m_pDlgCombinedCornerSearchTool->RemoveImage();
        m_pDlgCombinedCornerSearchTool->ClearScreen();
        m_pDlgCombinedCornerSearchTool->DestroyWindow();
    }
	if(m_pDlgMQuickPickSearchTool->GetSafeHwnd())
	{
		m_pDlgMQuickPickSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMQuickPickSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgMQuickPickSearchTool->DestroyWindow();
	}
	if(m_pDlgMPatternPickSearchTool->GetSafeHwnd())
	{
		m_pDlgMPatternPickSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMPatternPickSearchTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgMPatternPickSearchTool->DestroyWindow();
	}

	if(m_pDlgAreaInspectPickTool->GetSafeHwnd())
	{
		// 		m_dlgAreaInspectExTool->m_GuiPatternDisplay.RemoveImage();
		// 		m_dlgAreaInspectExTool->m_GuiPatternDisplay.ClearScreen();
		m_pDlgAreaInspectPickTool->DestroyWindow();
	}
	if (m_pDlgIdReaderDmCodeTool->GetSafeHwnd())
	{
		m_pDlgIdReaderDmCodeTool->DestroyWindow();
	}
	if (m_pDlgIdReaderBarCodeTool->GetSafeHwnd())
	{
		m_pDlgIdReaderBarCodeTool->DestroyWindow();
	}
}
void CDlgSearch::UpdateSearchToolParam(CString strSelText)
{
	cpImage imagePattern;
	cpImage imagePattern2;
	cpImage imagePattern3;

	cpImage imagePattern_HeadTail;

	// 清空数据
	if (m_pSearchTool != NULL)
	{
		m_pSearchTool->ClearAllGui();
		if (m_pSearchTool != m_pDefSearchTool)
		{
			delete m_pSearchTool;
		}
		m_pSearchTool = NULL;
	}
	if (m_pTrainDataParam != NULL)
	{
		delete m_pTrainDataParam;
		m_pTrainDataParam = NULL;
	}
	if (m_pTrainGuiParam != NULL)
	{
		delete m_pTrainGuiParam;	
		m_pTrainGuiParam	= NULL;
	}
	if (m_pSearchDataParam != NULL)
	{
		delete m_pSearchDataParam;
		m_pSearchDataParam = NULL;
	}
	if (m_pSearchGuiParam != NULL)
	{
		delete m_pSearchGuiParam;
		m_pSearchGuiParam = NULL;
	}
	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL))
	{
		m_pSearchTool		= new CQuickSearchTool(*((CQuickSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickSearchTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgQuickSearchTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchTool->ShowWindow(TRUE);
		m_pDlgQuickSearchTool->UpdateTrainData(false);
		m_pDlgQuickSearchTool->UpdateSearchData(false);
		m_pDlgQuickSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternSearchTool(*((CPatternSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgPatternSearchTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchTool->ShowWindow(TRUE);
		m_pDlgPatternSearchTool->UpdateTrainData(false);
		m_pDlgPatternSearchTool->UpdateSearchData(false);		
		m_pDlgPatternSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CLineSearchTool(*((CLineSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CLineSearchTrainDataParam;
		m_pTrainGuiParam	= new CLineSearchTrainGuiParam;
		m_pSearchDataParam	= new CLineSearchSearchDataParam;
		m_pSearchGuiParam	= new CLineSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgLineSearchTool->m_imageMask);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgLineSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgLineSearchTool->ShowWindow(TRUE);
		m_pDlgLineSearchTool->UpdateTrainData(false);
		m_pDlgLineSearchTool->UpdateSearchData(false);
		m_pDlgLineSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgLineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCornerSearchTool(*((CCornerSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CCornerSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgCornerSearchTool->m_imageMask);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCornerSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCornerSearchTool->ShowWindow(TRUE);
		m_pDlgCornerSearchTool->UpdateTrainData(false);
		m_pDlgCornerSearchTool->UpdateSearchData(false);
		m_pDlgCornerSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
		m_pDlgCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCircleSearchTool(*((CCircleSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CCircleSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgCircleSearchTool->m_imageMask);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCircleSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCircleSearchTool->ShowWindow(TRUE);
		m_pDlgCircleSearchTool->UpdateTrainData(false);
		m_pDlgCircleSearchTool->UpdateSearchData(false);
		m_pDlgCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CROSS_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCrossSearchTool(*((CCrossSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CCrossSearchTrainDataParam;
		m_pTrainGuiParam	= new CCrossSearchTrainGuiParam;
		m_pSearchDataParam	= new CCrossSearchSearchDataParam;
		m_pSearchGuiParam	= new CCrossSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgCrossSearchTool->m_imageMask);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCrossSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCrossSearchTool->ShowWindow(TRUE);
		m_pDlgCrossSearchTool->UpdateTrainData(false);
		m_pDlgCrossSearchTool->UpdateSearchData(false);
		m_pDlgCrossSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgCrossSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATCORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatCornerSearchTool(*((CPatCornerSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatCornerSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgPatCornerSearchTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetMaskImage2(m_pDlgPatCornerSearchTool->m_imageMask2);
		m_pSearchTool->GetTrainResultImage2(imagePattern2);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatCornerSearchTool->ShowWindow(TRUE);
		m_pDlgPatCornerSearchTool->UpdateTrainData(false);
		m_pDlgPatCornerSearchTool->UpdateSearchData(false);
		m_pDlgPatCornerSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatCornerSearchTool->m_GuiPatternDisplay2.SetImage(imagePattern2, TRUE);
		m_pDlgPatCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PROCIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CProCircleSearchTool(*((CProCircleSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CProCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CProCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CProCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CProCircleSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgProCircleSearchTool->m_imageMask);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgProCircleSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgProCircleSearchTool->ShowWindow(TRUE);
		m_pDlgProCircleSearchTool->UpdateTrainData(false);
		m_pDlgProCircleSearchTool->UpdateSearchData(false);
		m_pDlgProCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgProCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FIXEDPOS_SEARCH_TOOL))
	{
		m_pSearchTool		= new CFixedPosSearchTool(*((CFixedPosSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CFixedPosSearchTrainDataParam;
		m_pTrainGuiParam	= new CFixedPosSearchTrainGuiParam;
		m_pSearchDataParam	= new CFixedPosSearchSearchDataParam;
		m_pSearchGuiParam	= new CFixedPosSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgFixedPosSearchTool->ShowWindow(TRUE);
		m_pDlgFixedPosSearchTool->UpdateTrainData(false);
		m_pDlgFixedPosSearchTool->UpdateSearchData(false);
		m_pDlgFixedPosSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DMCOLDE_CALIBRATE_TOOL))
	{
		m_pSearchTool		= new CDmCodeCalibrateTool(*((CDmCodeCalibrateTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CDmCodeCalibrateTrainDataParam;
		m_pTrainGuiParam	= new CDmCodeCalibrateTrainGuiParam;
		m_pSearchDataParam	= new CDmCodeCalibrateSearchDataParam;
		m_pSearchGuiParam	= new CDmCodeCalibrateSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgDmCodeCalibrateTool->ShowWindow(TRUE);
		m_pDlgDmCodeCalibrateTool->UpdateTrainData(false);
		m_pDlgDmCodeCalibrateTool->UpdateSearchData(false);
		m_pDlgDmCodeCalibrateTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCornerSearchTool(*((CPatternCornerSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgPatternCornerSearchTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternCornerSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCornerSearchTool->UpdateTrainData(false);
		m_pDlgPatternCornerSearchTool->UpdateSearchData(false);		
		m_pDlgPatternCornerSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_QUICK_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCombinedQuickSearch(*((CCombinedQuickSearch*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CCombinedQuickSearchTrainDataParam;
		m_pTrainGuiParam	= new CCombinedQuickSearchTrainGuiParam;
		m_pSearchDataParam	= new CCombinedQuickSearchDataParam;
		m_pSearchGuiParam	= new CCombinedQuickSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgCombinedQuickSearchTool->m_imageMask);
		m_pSearchTool->GetMaskImage2(m_pDlgCombinedQuickSearchTool->m_imageMask1);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetPatternImage2(imagePattern2);

		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCombinedQuickSearchTool->ShowWindow(TRUE);
		m_pDlgCombinedQuickSearchTool->UpdateTrainData(false);
		m_pDlgCombinedQuickSearchTool->UpdateSearchData(false);
		m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay2.SetImage(imagePattern2, TRUE);
		m_pDlgCombinedQuickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCombinedPatternSearchTool(*((CCombinedPatternSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CCombinedPatternSearchTrainDataParam;
		m_pTrainGuiParam	= new CCombinedPatternSearchTrainGuiParam;
		m_pSearchDataParam	= new CCombinedPatternSearchSearchDataParam;
		m_pSearchGuiParam	= new CCombinedPatternSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgCombinedPatternSearchTool->m_imageMask);
		m_pSearchTool->GetMaskImage2(m_pDlgCombinedPatternSearchTool->m_imageMask1);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainResultImage2(imagePattern2);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCombinedPatternSearchTool->ShowWindow(TRUE);
		m_pDlgCombinedPatternSearchTool->UpdateTrainData(false);
		m_pDlgCombinedPatternSearchTool->UpdateSearchData(false);		
		m_pDlgCombinedPatternSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgCombinedPatternSearchTool->m_GuiPatternDisplay2.SetImage(imagePattern2, TRUE);
		m_pDlgCombinedPatternSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BASE_CORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CBaseCornerSearchTool(*((CBaseCornerSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CBaseCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CBaseCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CBaseCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CBaseCornerSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgBaseCornerSearchTool->ShowWindow(TRUE);
		m_pDlgBaseCornerSearchTool->UpdateTrainData(false);
		m_pDlgBaseCornerSearchTool->UpdateSearchData(false);		
		m_pDlgBaseCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_TOOL))
	{
		m_pSearchTool		= new CPatternSearchCheckTool(*((CPatternSearchCheckTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternSearchCheckTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchCheckTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchCheckSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchCheckSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternSearchCheckTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainResultImage2(imagePattern2);
		m_pSearchTool->GetTrainResultImage3(imagePattern3);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchCheckTool->ShowWindow(TRUE);
		m_pDlgPatternSearchCheckTool->UpdateTrainData(false);
		m_pDlgPatternSearchCheckTool->UpdateSearchData(false);		
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplayCheck1.SetImage(imagePattern2, TRUE);
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgPatternSearchCheckTool->m_GuiPatternDisplayCheck2.SetImage(imagePattern3, TRUE);
		m_pDlgPatternSearchCheckTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_TOOL))
	{
		m_pSearchTool		= new CQuickSearchCheckTool(*((CQuickSearchCheckTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickSearchCheckTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchCheckTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchCheckSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchCheckSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgQuickSearchCheckTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetPatternImage2(imagePattern2);
		m_pSearchTool->GetPatternImage3(imagePattern3);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchCheckTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck1);
		m_pSearchTool->SetTrainResultDisplay3(&m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchCheckTool->ShowWindow(TRUE);
		m_pDlgQuickSearchCheckTool->UpdateTrainData(false);
		m_pDlgQuickSearchCheckTool->UpdateSearchData(false);		
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck1.SetImage(imagePattern2, TRUE);
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck2.SetImage(imagePattern3, TRUE);
		m_pDlgQuickSearchCheckTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_OBROUND_TOOL))
	{
		m_pSearchTool		= new CQuickSearchObroundTool(*((CQuickSearchObroundTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickSearchObroundTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchObroundTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchObroundSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchObroundSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgQuickSearchObroundTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchObroundTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchObroundTool->ShowWindow(TRUE);
		m_pDlgQuickSearchObroundTool->UpdateTrainData(false);
		m_pDlgQuickSearchObroundTool->UpdateSearchData(false);		
		m_pDlgQuickSearchObroundTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickSearchObroundTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL))
	{
		m_pSearchTool		= new CPatternSearchObroundTool(*((CPatternSearchObroundTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternSearchObroundTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchObroundTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchObroundSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchObroundSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternSearchObroundTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchObroundTool->ShowWindow(TRUE);
		m_pDlgPatternSearchObroundTool->UpdateTrainData(false);
		m_pDlgPatternSearchObroundTool->UpdateSearchData(false);		
		m_pDlgPatternSearchObroundTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternSearchObroundTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMPatternSearchTool(*((CMPatternSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CMPatternSearchTrainDataParam;
		m_pTrainGuiParam	= new CMPatternSearchTrainGuiParam;
		m_pSearchDataParam	= new CMPatternSearchSearchDataParam;
		m_pSearchGuiParam	= new CMPatternSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgMPatternSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMPatternSearchTool->ShowWindow(TRUE);
		m_pDlgMPatternSearchTool->UpdateTrainData(false);
		m_pDlgMPatternSearchTool->UpdateSearchData(false);		
		m_pDlgMPatternSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgMPatternSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMQuickSearchTool(*((CMQuickSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CMQuickSearchTrainDataParam;
		m_pTrainGuiParam	= new CMQuickSearchTrainGuiParam;
		m_pSearchDataParam	= new CMQuickSearchSearchDataParam;
		m_pSearchGuiParam	= new CMQuickSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgMQuickSearchTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgMQuickSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMQuickSearchTool->ShowWindow(TRUE);
		m_pDlgMQuickSearchTool->UpdateTrainData(false);
		m_pDlgMQuickSearchTool->UpdateSearchData(false);		
		m_pDlgMQuickSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgMQuickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_CORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMQuickCornerSearchTool(*((CMQuickCornerSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CMQuickCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CMQuickCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CMQuickCornerSearchDataParam;
		m_pSearchGuiParam	= new CMQuickCornerSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgMQuickCornerSearchTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgMQuickCornerSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMQuickCornerSearchTool->ShowWindow(TRUE);
		m_pDlgMQuickCornerSearchTool->UpdateTrainData(false);
		m_pDlgMQuickCornerSearchTool->UpdateSearchData(false);		
		m_pDlgMQuickCornerSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgMQuickCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMQuickPickCornerSearchTool(*((CMQuickPickCornerSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CMQuickPickCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CMQuickPickCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CMQuickPickCornerSearchDataParam;
		m_pSearchGuiParam	= new CMQuickPickCornerSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgMQuickPickCornerSearchTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgMQuickPickCornerSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMQuickPickCornerSearchTool->ShowWindow(TRUE);
		m_pDlgMQuickPickCornerSearchTool->UpdateTrainData(false);
		m_pDlgMQuickPickCornerSearchTool->UpdateSearchData(false);		
		m_pDlgMQuickPickCornerSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgMQuickPickCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_GENRECT_TOOL))
	{
		m_pSearchTool		= new CQuickSearchGenRectTool(*((CQuickSearchGenRectTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickSearchGenRectTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchGenRectTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchGenRectSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchGenRectSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgQuickSearchGenRectTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchGenRectTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchGenRectTool->ShowWindow(TRUE);
		m_pDlgQuickSearchGenRectTool->UpdateTrainData(false);
		m_pDlgQuickSearchGenRectTool->UpdateSearchData(false);		
		m_pDlgQuickSearchGenRectTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickSearchGenRectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL))
	{
		m_pSearchTool		= new CPatternSearchGenRectTool(*((CPatternSearchGenRectTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternSearchGenRectTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchGenRectTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchGenRectSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchGenRectSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternSearchGenRectTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchGenRectTool->ShowWindow(TRUE);
		m_pDlgPatternSearchGenRectTool->UpdateTrainData(false);
		m_pDlgPatternSearchGenRectTool->UpdateSearchData(false);		
		m_pDlgPatternSearchGenRectTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternSearchGenRectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternLineSearchTool(*((CPatternLineSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternLineSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternLineSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternLineSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternLineSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternLineSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternLineSearchTool->ShowWindow(TRUE);
		m_pDlgPatternLineSearchTool->UpdateTrainData(false);
		m_pDlgPatternLineSearchTool->UpdateSearchData(false);		
		m_pDlgPatternLineSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternLineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_2PATTERN_SEARCH_TOOL))
	{
		m_pSearchTool		= new C2PatternSearchTool(*((C2PatternSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new C2PatternSearchTrainDataParam;
		m_pTrainGuiParam	= new C2PatternSearchTrainGuiParam;
		m_pSearchDataParam	= new C2PatternSearchSearchDataParam;
		m_pSearchGuiParam	= new C2PatternSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlg2PatternSearchTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetMaskImage2(m_pDlg2PatternSearchTool->m_imageMask2);
		m_pSearchTool->GetTrainResultImage2(imagePattern2);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlg2PatternSearchTool->ShowWindow(TRUE);
		m_pDlg2PatternSearchTool->UpdateTrainData(false);
		m_pDlg2PatternSearchTool->UpdateSearchData(false);
		m_pDlg2PatternSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlg2PatternSearchTool->m_GuiPatternDisplay2.SetImage(imagePattern2, TRUE);
		m_pDlg2PatternSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CQuickLineCircleTool(*((CQuickLineCircleTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickLineCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CQuickLineCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CQuickLineCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CQuickLineCircleSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgQuickLineCircleSearchTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickLineCircleSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickLineCircleSearchTool->ShowWindow(TRUE);
		m_pDlgQuickLineCircleSearchTool->UpdateTrainData(false);
		m_pDlgQuickLineCircleSearchTool->UpdateSearchData(false);		
		m_pDlgQuickLineCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickLineCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternLineCircleTool(*((CPatternLineCircleTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternLineCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternLineCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternLineCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternLineCircleSearchSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternLineCircleSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgPatternLineCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternLineCircleSearchTool->UpdateTrainData(false);
		m_pDlgPatternLineCircleSearchTool->UpdateSearchData(false);		
		m_pDlgPatternLineCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternLineCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCircleSearchTool(*((CPatternCircleSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCircleSearchSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternCircleSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgPatternCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCircleSearchTool->UpdateTrainData(false);
		m_pDlgPatternCircleSearchTool->UpdateSearchData(false);
		m_pDlgPatternCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternCircleSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL))
	{
		m_pSearchTool		= new CQuickSearchCheckLineTool(*((CQuickSearchCheckLineTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickSearchCheckLineTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchCheckLineTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchCheckLineSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchCheckLineSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgQuickSearchCheckLineTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetPatternImage2(imagePattern2);
		m_pSearchTool->GetPatternImage3(imagePattern3);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck1);
		m_pSearchTool->SetTrainResultDisplay3(&m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchCheckLineTool->ShowWindow(TRUE);
		m_pDlgQuickSearchCheckLineTool->UpdateTrainData(false);
		m_pDlgQuickSearchCheckLineTool->UpdateSearchData(false);		
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck1.SetImage(imagePattern2, TRUE);
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck2.SetImage(imagePattern3, TRUE);
		m_pDlgQuickSearchCheckLineTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL))
	{
		m_pSearchTool		= new CPatternSearchCheckLineTool(*((CPatternSearchCheckLineTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternSearchCheckLineTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchCheckLineTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchCheckLineSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchCheckLineSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternSearchCheckLineTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainResultImage2(imagePattern2);
		m_pSearchTool->GetTrainResultImage3(imagePattern3);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchCheckLineTool->ShowWindow(TRUE);
		m_pDlgPatternSearchCheckLineTool->UpdateTrainData(false);
		m_pDlgPatternSearchCheckLineTool->UpdateSearchData(false);		
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplayCheck1.SetImage(imagePattern2, TRUE);
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgPatternSearchCheckLineTool->m_GuiPatternDisplayCheck2.SetImage(imagePattern3, TRUE);
		m_pDlgPatternSearchCheckLineTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_BLOB_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternBlobSearchTool(*((CPatternBlobSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternBlobSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternBlobSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternBlobSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternBlobSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternBlobSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternBlobSearchTool->ShowWindow(TRUE);
		m_pDlgPatternBlobSearchTool->UpdateTrainData(false);
		m_pDlgPatternBlobSearchTool->UpdateSearchData(false);		
		m_pDlgPatternBlobSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternBlobSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_CHECK_TOOL))
	{
		m_pSearchTool		= new CCornerSearchCheckTool(*((CCornerSearchCheckTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CCornerSearchCheckTrainDataParam;
		m_pTrainGuiParam	= new CCornerSearchCheckTrainGuiParam;
		m_pSearchDataParam	= new CCornerSearchCheckSearchDataParam;
		m_pSearchGuiParam	= new CCornerSearchCheckSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgCornerSearchCheckTool->m_imageMask);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetPatternImage2(imagePattern2);
		m_pSearchTool->GetPatternImage3(imagePattern3);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCornerSearchCheckTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck1);
		m_pSearchTool->SetTrainResultDisplay3(&m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCornerSearchCheckTool->ShowWindow(TRUE);
		m_pDlgCornerSearchCheckTool->UpdateTrainData(false);
		m_pDlgCornerSearchCheckTool->UpdateSearchData(false);
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);		
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck1.RemoveImage();
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck1.SetImage(imagePattern2, TRUE);
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck2.RemoveImage();
		m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck2.SetImage(imagePattern3, TRUE);
		m_pDlgCornerSearchCheckTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL))
	{
		m_pSearchTool		= new CRectSearchTool(*((CRectSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CRectSearchTrainDataParam;
		m_pTrainGuiParam	= new CRectSearchTrainGuiParam;
		m_pSearchDataParam	= new CRectSearchSearchDataParam;
		m_pSearchGuiParam	= new CRectSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgRectSearchTool->m_imageMask);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgRectSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgRectSearchTool->ShowWindow(TRUE);
		m_pDlgRectSearchTool->UpdateTrainData(false);
		m_pDlgRectSearchTool->UpdateSearchData(false);
		m_pDlgRectSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgRectSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
		m_pDlgRectSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_RECT_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternRectSearchTool(*((CPatternRectSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternRectSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternRectSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternRectSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternRectSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgPatternRectSearchTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternRectSearchTool->ShowWindow(TRUE);
		m_pDlgPatternRectSearchTool->UpdateTrainData(false);
		m_pDlgPatternRectSearchTool->UpdateSearchData(false);
		m_pDlgPatternRectSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternRectSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
		m_pDlgPatternRectSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECT_TOOL))
	{
		m_pSearchTool		= new CAreaInspectTool(*((CAreaInspectTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CAreaInspectTrainDataParam;
		m_pTrainGuiParam	= new CAreaInspectTrainGuiParam;
		m_pSearchDataParam	= new CAreaInspectSearchDataParam;
		m_pSearchGuiParam	= new CAreaInspectSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);
        m_pDlgAreaInspectTool->m_GuiPatternDisplay.RemoveImage();
        if (((CAreaInspectTrainDataParam*)m_pTrainDataParam)->m_bEnableBaseSearch)
        {
            m_pSearchTool->GetMaskImage(m_pDlgAreaInspectTool->m_imageMask); 
            m_pDlgAreaInspectTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);	
        }
		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgAreaInspectTool->ShowWindow(TRUE);
		m_pDlgAreaInspectTool->UpdateTrainData(false);
		m_pDlgAreaInspectTool->UpdateSearchData(false);
 				
		m_pDlgAreaInspectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECTEX_TOOL))
	{
		m_pSearchTool		= new CAreaInspectToolEx(*((CAreaInspectToolEx*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CAreaInspectExTrainDataParam;
		m_pTrainGuiParam	= new CAreaInspectExTrainGuiParam;
		m_pSearchDataParam	= new CAreaInspectExSearchDataParam;
		m_pSearchGuiParam	= new CAreaInspectExSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgAreaInspectExTool->ShowWindow(TRUE);
		m_pDlgAreaInspectExTool->UpdateTrainData(false);
		m_pDlgAreaInspectExTool->UpdateSearchData(false);
		//m_dlgAreaInspectExTool->m_GuiPatternDisplay.RemoveImage();
		//m_dlgAreaInspectExTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
		m_pDlgAreaInspectExTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CHORD_SEARCH_TOOL))
	{
		m_pSearchTool		= new CChordSearchTool(*((CChordSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CChordSearchTrainDataParam;
		m_pTrainGuiParam	= new CChordSearchTrainGuiParam;
		m_pSearchDataParam	= new CChordSearchSearchDataParam;
		m_pSearchGuiParam	= new CChordSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);
        m_pSearchTool->GetMaskImage(m_pDlgChordSearchTool->m_imageMask);

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgChordSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgChordSearchTool->ShowWindow(TRUE);
		m_pDlgChordSearchTool->UpdateTrainData(false);
		m_pDlgChordSearchTool->UpdateSearchData(false);
		m_pDlgChordSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgChordSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
		m_pDlgChordSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL))
	{
		m_pSearchTool		= new CTwoPatModelSearchTool(*((CTwoPatModelSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CTwoPatModelSearchTrainDataParam;
		m_pTrainGuiParam	= new CTwoPatModelSearchTrainGuiParam;
		m_pSearchDataParam	= new CTwoPatModelSearchSearchDataParam;
		m_pSearchGuiParam	= new CTwoPatModelSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);//加载第一训练区域训练结果图
		m_pSearchTool->GetTrainImage(m_imageTrain);
		m_pSearchTool->GetMaskImage(m_pDlgTwoPatModelSearchTool->m_imageMask);
		m_pSearchTool->GetMaskImage2(m_pDlgTwoPatModelSearchTool->m_imageMask2);

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgTwoPatModelSearchTool->ShowWindow(TRUE);
		m_pDlgTwoPatModelSearchTool->UpdateTrainData(false);
		m_pDlgTwoPatModelSearchTool->UpdateSearchData(false);
		m_pDlgTwoPatModelSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgTwoPatModelSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pSearchTool->GetTrainResultImage2(imagePattern);//加载第二训练区域训练结果图
		m_pDlgTwoPatModelSearchTool->m_GuiPatternDisplay2.RemoveImage();
		m_pDlgTwoPatModelSearchTool->m_GuiPatternDisplay2.SetImage(imagePattern, TRUE);				
		m_pDlgTwoPatModelSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNCORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMPatternCornerSearchTool(*((CMPatternCornerSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CMPatternCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CMPatternCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CMPatternCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CMPatternCornerSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);
        m_pSearchTool->GetMaskImage(m_pDlgMPatternCornerSearchTool->m_imageMask);
		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMPatternCornerSearchTool->ShowWindow(TRUE);
		m_pDlgMPatternCornerSearchTool->UpdateTrainData(false);
		m_pDlgMPatternCornerSearchTool->UpdateSearchData(false);
		m_pDlgMPatternCornerSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMPatternCornerSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
		m_pDlgMPatternCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMPatternPickCornerSearchTool(*((CMPatternPickCornerSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CMPatternPickCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CMPatternPickCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CMPatternPickCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CMPatternPickCornerSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);
		m_pSearchTool->GetMaskImage(m_pDlgMPatternPickCornerSearchTool->m_imageMask);
		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMPatternPickCornerSearchTool->ShowWindow(TRUE);
		m_pDlgMPatternPickCornerSearchTool->UpdateTrainData(false);
		m_pDlgMPatternPickCornerSearchTool->UpdateSearchData(false);
		m_pDlgMPatternPickCornerSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgMPatternPickCornerSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
		m_pDlgMPatternPickCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCornerCircleSearchTool(*((CPatternCornerCircleSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternCornerCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerCircleSearchSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternCornerCircleSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgPatternCornerCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCornerCircleSearchTool->UpdateTrainData(false);
		m_pDlgPatternCornerCircleSearchTool->UpdateSearchData(false);
		m_pDlgPatternCornerCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternCornerCircleSearchTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CQuickCornerCircleSearchTool(*((CQuickCornerCircleSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickCornerCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CQuickCornerCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CQuickCornerCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CQuickCornerCircleSearchSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgQuickCornerCircleSearchTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickCornerCircleSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgQuickCornerCircleSearchTool->ShowWindow(TRUE);
		m_pDlgQuickCornerCircleSearchTool->UpdateTrainData(false);
		m_pDlgQuickCornerCircleSearchTool->UpdateSearchData(false);
		m_pDlgQuickCornerCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickCornerCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL))
	{
		m_pSearchTool		= new CPatternPosLineDistanceTool(*((CPatternPosLineDistanceTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternPosLineDistanceTrainDataParam;
		m_pTrainGuiParam	= new CPatternPosLineDistanceTrainGuiParam;
		m_pSearchDataParam	= new CPatternPosLineDistanceSearchDataParam;
		m_pSearchGuiParam	= new CPatternPosLineDistanceSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgPatternPosLineDistanceTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternPosLineDistanceTool->ShowWindow(TRUE);
		m_pDlgPatternPosLineDistanceTool->UpdateTrainData(false);
		m_pDlgPatternPosLineDistanceTool->UpdateSearchData(false);		
		m_pDlgPatternPosLineDistanceTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternPosLineDistanceTool->UpdateControlDisplay();
	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL))
    {
        m_pSearchTool		= new CCornerCheckSearchTool(*((CCornerCheckSearchTool*)m_pDefSearchTool));
        m_pSearchTool->RestoreTrainParam();
        m_pSearchTool->RestoreSearchParam();
        m_pTrainDataParam	= new CCornerCheckSearchTrainDataParam;
        m_pTrainGuiParam	= new CCornerCheckSearchTrainGuiParam;
        m_pSearchDataParam	= new CCornerCheckSearchSearchDataParam;
        m_pSearchGuiParam	= new CCornerCheckSearchSearchGuiParam;
        m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
        m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
        m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
        m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
        m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainResultImage2(imagePattern2);
        if (((CCornerCheckSearchTrainDataParam*)m_pTrainDataParam)->GetSubSearchToolType()==ePatternCornerSearchTool)
        {
            m_pSearchTool->GetMaskImage(m_pDlgCornerSearchCheckInspectTool->m_DlgPatternCornerSearchTool.m_imageMask);
            m_pDlgCornerSearchCheckInspectTool->m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
        }
        else if (((CCornerCheckSearchTrainDataParam*)m_pTrainDataParam)->GetSubSearchToolType()==eCornerSearchTool)
        {
            m_pSearchTool->GetMaskImage(m_pDlgCornerSearchCheckInspectTool->m_DlgCornerSearchTool.m_imageMask);
			m_pSearchTool->GetPatternImage(imagePattern);
            m_pDlgCornerSearchCheckInspectTool->m_DlgCornerSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
			
        }
        if (((CCornerCheckSearchTrainDataParam*)m_pTrainDataParam)->GetInspectToolType()==ePatternSearchTool)
        {
            m_pSearchTool->GetMaskImage2(m_pDlgCornerSearchCheckInspectTool->m_DlgPatternSearchTool.m_imageMask);
            m_pDlgCornerSearchCheckInspectTool->m_DlgPatternSearchTool.m_GuiPatternDisplay.SetImage(imagePattern2, TRUE);
        }
        else if (((CCornerCheckSearchTrainDataParam*)m_pTrainDataParam)->GetInspectToolType()==eQuickSearchTool)
        {
            m_pSearchTool->GetMaskImage2(m_pDlgCornerSearchCheckInspectTool->m_DlgQuickSearchTool.m_imageMask);
			m_pSearchTool->GetPatternImage2(imagePattern2);
            m_pDlgCornerSearchCheckInspectTool->m_DlgQuickSearchTool.m_GuiPatternDisplay.SetImage(imagePattern2, TRUE);
			
        }
        m_pSearchTool->GetTrainImage(m_imageTrain);
        m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgCornerSearchCheckInspectTool->m_DlgQuickSearchTool.m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCornerSearchCheckInspectTool->m_DlgCornerSearchTool.m_GuiPatternDisplay);
        m_pSearchTool->ShowTrainGui(false);
        m_pSearchTool->ShowSearchResultGui(false);
        m_pDlgCornerSearchCheckInspectTool->ShowWindow(TRUE);
        m_pDlgCornerSearchCheckInspectTool->UpdateTrainData(false);
        m_pDlgCornerSearchCheckInspectTool->UpdateSearchData(false);

        m_pDlgCornerSearchCheckInspectTool->UpdateDlgItem();
        m_pDlgCornerSearchCheckInspectTool->UpdateControlDisplay();

    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL))
	{
		
		m_pSearchTool		= new CPatternDoubleSearchTool(*((CPatternDoubleSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternDoubleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternDoubleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternDoubleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternDoubleSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgPatternDoubleSearchTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetMaskImage2(m_pDlgPatternDoubleSearchTool->m_imageMask2);
		m_pSearchTool->GetTrainResultImage2(imagePattern2);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternDoubleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternDoubleSearchTool->UpdateTrainData(false);
		m_pDlgPatternDoubleSearchTool->UpdateSearchData(false);
		m_pDlgPatternDoubleSearchTool->m_GuiPatternDisplay1.RemoveImage();
		m_pDlgPatternDoubleSearchTool->m_GuiPatternDisplay1.SetImage(imagePattern, TRUE);
		m_pDlgPatternDoubleSearchTool->m_GuiPatternDisplay2.RemoveImage();
		m_pDlgPatternDoubleSearchTool->m_GuiPatternDisplay2.SetImage(imagePattern2, TRUE);		
		m_pDlgPatternDoubleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CHORD_SEARCH_TOOL))
	{
		
		m_pSearchTool		= new CPatternChordSearchTool(*((CPatternChordSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternChordSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternChordSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternChordSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternChordSearchSearchGuiParam;
		
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternChordSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgPatternChordSearchTool->ShowWindow(TRUE);
		m_pDlgPatternChordSearchTool->UpdateTrainData(false);
		m_pDlgPatternChordSearchTool->UpdateSearchData(false);
        m_pDlgPatternChordSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternChordSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternChordSearchTool->UpdateControlDisplay();
	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL))
    {

        m_pSearchTool		= new CQuickMulCircleSearchTool(*((CQuickMulCircleSearchTool*)m_pDefSearchTool));
        m_pSearchTool->RestoreTrainParam();
        m_pSearchTool->RestoreSearchParam();
        m_pTrainDataParam	= new CQuickMulCircleSearchTrainDataParam;
        m_pTrainGuiParam	= new CQuickMulCircleSearchTrainGuiParam;
        m_pSearchDataParam	= new CQuickMulCircleSearchSearchDataParam;
        m_pSearchGuiParam	= new CQuickMulCircleSearchSearchGuiParam;

        m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
        m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
        m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
        m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

        m_pSearchTool->GetMaskImage(m_pDlgQuickMulCircleSearchTool->m_imageMask);

        m_pSearchTool->GetPatternImage(imagePattern);
        m_pSearchTool->GetTrainImage(m_imageTrain);

        m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickMulCircleSearchTool->m_GuiPatternDisplay);
        m_pSearchTool->ShowTrainGui(false);
        m_pSearchTool->ShowSearchResultGui(false);

        m_pDlgQuickMulCircleSearchTool->ShowWindow(TRUE);
        m_pDlgQuickMulCircleSearchTool->UpdateTrainData(false);
        m_pDlgQuickMulCircleSearchTool->UpdateSearchData(false);
        m_pDlgQuickMulCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
        m_pDlgQuickMulCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
        m_pDlgQuickMulCircleSearchTool->UpdateControlDisplay();
    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL))
	{

		m_pSearchTool		= new CPatternCircle4LineSearchTool(*((CPatternCircle4LineSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternCircle4LineSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCircle4LineSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCircle4LineSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCircle4LineSearchSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternCircle4LineSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgPatternCircle4LineSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCircle4LineSearchTool->UpdateTrainData(false);
		m_pDlgPatternCircle4LineSearchTool->UpdateSearchData(false);
		m_pDlgPatternCircle4LineSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCircle4LineSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternCircle4LineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL))
	{

		m_pSearchTool		= new CQuickCircle4LineSearchTool(*((CQuickCircle4LineSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickCircle4LineSearchTrainDataParam;
		m_pTrainGuiParam	= new CQuickCircle4LineSearchTrainGuiParam;
		m_pSearchDataParam	= new CQuickCircle4LineSearchSearchDataParam;
		m_pSearchGuiParam	= new CQuickCircle4LineSearchSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgQuickCircle4LineSearchTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickCircle4LineSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgQuickCircle4LineSearchTool->ShowWindow(TRUE);
		m_pDlgQuickCircle4LineSearchTool->UpdateTrainData(false);
		m_pDlgQuickCircle4LineSearchTool->UpdateSearchData(false);
		m_pDlgQuickCircle4LineSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickCircle4LineSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickCircle4LineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL))
	{
		 
		m_pSearchTool		= new CSpecialPickSearchTool(*((CSpecialPickSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CSpecialPickSearchTrainDataParam;
		m_pTrainGuiParam	= new CSpecialPickSearchTrainGuiParam;
		m_pSearchDataParam	= new CSpecialPickSearchSearchDataParam;
		m_pSearchGuiParam	= new CSpecialPickSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		if (((CSpecialPickSearchTrainDataParam*)m_pTrainDataParam)->GetRoughSearchToolType()==ePatternSearchTool)
		{
			m_pSearchTool->GetMaskImage(m_pDlgSpecialPickSearchTool->m_DlgPatternSearchTool_Rough.m_imageMask);
			m_pDlgSpecialPickSearchTool->m_DlgPatternSearchTool_Rough.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		}
		else if (((CSpecialPickSearchTrainDataParam*)m_pTrainDataParam)->GetRoughSearchToolType()==eQuickSearchTool)
		{
			m_pSearchTool->GetMaskImage(m_pDlgSpecialPickSearchTool->m_DlgQuickSearchTool_Rough.m_imageMask);
			m_pSearchTool->GetPatternImage(imagePattern);
			m_pDlgSpecialPickSearchTool->m_DlgQuickSearchTool_Rough.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		}
		m_pSearchTool->GetTrainResultImage_HeadTail(imagePattern_HeadTail);
		if (((CSpecialPickSearchTrainDataParam*)m_pTrainDataParam)->GetHeadTailCheckToolType()==ePatternSearchTool)
		{
			m_pSearchTool->GetMaskImage_HeadTail(m_pDlgSpecialPickSearchTool->m_DlgPatternSearchTool_HeadTail.m_imageMask);
			m_pDlgSpecialPickSearchTool->m_DlgPatternSearchTool_HeadTail.m_GuiPatternDisplay.SetImage(imagePattern_HeadTail, TRUE);
		}
		else if (((CSpecialPickSearchTrainDataParam*)m_pTrainDataParam)->GetHeadTailCheckToolType()==eQuickSearchTool)
		{
			m_pSearchTool->GetMaskImage_HeadTail(m_pDlgSpecialPickSearchTool->m_DlgQuickSearchTool_HeadTail.m_imageMask);
			m_pSearchTool->GetPatternImage2(imagePattern_HeadTail);
			m_pDlgSpecialPickSearchTool->m_DlgQuickSearchTool_HeadTail.m_GuiPatternDisplay.SetImage(imagePattern_HeadTail, TRUE);
		}
		m_pSearchTool->GetTrainImage(m_imageTrain);
		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgSpecialPickSearchTool->m_DlgQuickSearchTool_Rough.m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgSpecialPickSearchTool->m_DlgQuickSearchTool_HeadTail.m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgSpecialPickSearchTool->ShowWindow(TRUE);
		m_pDlgSpecialPickSearchTool->UpdateTrainData(false);
		m_pDlgSpecialPickSearchTool->UpdateSearchData(false);

		m_pDlgSpecialPickSearchTool->UpdateDlgItem();
		m_pDlgSpecialPickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCornerLineSearchTool(*((CPatternCornerLineSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternCornerLineSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerLineSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerLineSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerLineSearchSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternCornerLineSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgPatternCornerLineSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCornerLineSearchTool->UpdateTrainData(false);
		m_pDlgPatternCornerLineSearchTool->UpdateSearchData(false);
		m_pDlgPatternCornerLineSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCornerLineSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternCornerLineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL))
	{

		m_pSearchTool		= new CPatternMulCircleSearchTool(*((CPatternMulCircleSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternMulCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternMulCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternMulCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternMulCircleSearchSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternMulCircleSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgPatternMulCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternMulCircleSearchTool->UpdateTrainData(false);
		m_pDlgPatternMulCircleSearchTool->UpdateSearchData(false);
		m_pDlgPatternMulCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternMulCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternMulCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL))
	{

		m_pSearchTool		= new CPatternCornerMulCircleSearchTool(*((CPatternCornerMulCircleSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternCornerMulCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerMulCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerMulCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerMulCircleSearchSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternCornerMulCircleSearchTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgPatternCornerMulCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCornerMulCircleSearchTool->UpdateTrainData(false);
		m_pDlgPatternCornerMulCircleSearchTool->UpdateSearchData(false);
		m_pDlgPatternCornerMulCircleSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCornerMulCircleSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternCornerMulCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL))
	{

		m_pSearchTool		= new CPatternCornerMulObroundTool(*((CPatternCornerMulObroundTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternCornerMulObroundTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerMulObroundTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerMulObroundSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerMulObroundSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternCornerMulObroundTool->m_imageMask);

		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgPatternCornerMulObroundTool->ShowWindow(TRUE);
		m_pDlgPatternCornerMulObroundTool->UpdateTrainData(false);
		m_pDlgPatternCornerMulObroundTool->UpdateSearchData(false);
		m_pDlgPatternCornerMulObroundTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternCornerMulObroundTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgPatternCornerMulObroundTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL))
	{

		m_pSearchTool		= new CQuickCornerMulObroundTool(*((CQuickCornerMulObroundTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickCornerMulObroundTrainDataParam;
		m_pTrainGuiParam	= new CQuickCornerMulObroundTrainGuiParam;
		m_pSearchDataParam	= new CQuickCornerMulObroundSearchDataParam;
		m_pSearchGuiParam	= new CQuickCornerMulObroundSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgPatternCornerMulObroundTool->m_imageMask);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickCornerMulObroundTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgQuickCornerMulObroundTool->ShowWindow(TRUE);
		m_pDlgQuickCornerMulObroundTool->UpdateTrainData(false);
		m_pDlgQuickCornerMulObroundTool->UpdateSearchData(false);
		m_pDlgQuickCornerMulObroundTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgQuickCornerMulObroundTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickCornerMulObroundTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL))
	{
		m_pSearchTool		= new CCircleCornerCheckSearchTool(*((CCircleCornerCheckSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CCircleCornerCheckSearchTrainDataParam;
		m_pTrainGuiParam	= new CCircleCornerCheckSearchTrainGuiParam;
		m_pSearchDataParam	= new CCircleCornerCheckSearchSearchDataParam;
		m_pSearchGuiParam	= new CCircleCornerCheckSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);

		m_pSearchTool->GetTrainResultImage2(imagePattern2);
		
		m_pSearchTool->GetTrainImage(m_imageTrain);
		
		if (((CCircleCornerCheckSearchTrainDataParam*)m_pTrainDataParam)->GetSubSearchToolType()==ePatternCornerSearchTool)
		{
			m_pSearchTool->GetMaskImage(m_pDlgCircleCornerSearchCheckInspectTool->m_DlgPatternCornerSearchTool.m_imageMask);
			m_pDlgCircleCornerSearchCheckInspectTool->m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		}
		else if (((CCircleCornerCheckSearchTrainDataParam*)m_pTrainDataParam)->GetSubSearchToolType()==eCornerSearchTool)
		{
			m_pSearchTool->GetMaskImage(m_pDlgCircleCornerSearchCheckInspectTool->m_DlgCornerSearchTool.m_imageMask);
			m_pDlgCircleCornerSearchCheckInspectTool->m_DlgCornerSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);

			m_pSearchTool->SetTrainResultDisplay(&m_pDlgCircleCornerSearchCheckInspectTool->m_DlgCornerSearchTool.m_GuiPatternDisplay);							
		}
		
		m_pSearchTool->SetDisplay(&m_guiDisplay);
		
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCircleCornerSearchCheckInspectTool->ShowWindow(TRUE);
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateTrainData(false);
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchData(false);

		m_pDlgCircleCornerSearchCheckInspectTool->UpdateDlgItem();
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL))
	{
		m_pSearchTool		= new CACFInspectTool(*((CACFInspectTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CACFInspectTrainDataParam;
		m_pTrainGuiParam	= new CACFInspectTrainGuiParam;
		m_pSearchDataParam	= new CACFInspectSearchDataParam;
		m_pSearchGuiParam	= new CACFInspectSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		if (((CACFInspectTrainDataParam*)m_pTrainDataParam)->GetRoughSearchToolType()==ePatternSearchTool)
		{
			m_pSearchTool->GetMaskImage(m_pDlgACFInspectTool->m_DlgPatternSearchTool.m_imageMask);
			m_pDlgACFInspectTool->m_DlgPatternSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		}
		else if (((CACFInspectTrainDataParam*)m_pTrainDataParam)->GetRoughSearchToolType()==eQuickSearchTool)
		{
			m_pSearchTool->GetMaskImage(m_pDlgACFInspectTool->m_DlgQuickSearchTool.m_imageMask);
			m_pDlgACFInspectTool->m_DlgQuickSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
			
		}
		else if (((CACFInspectTrainDataParam*)m_pTrainDataParam)->GetRoughSearchToolType()==ePatternCornerSearchTool)
		{
			m_pSearchTool->GetMaskImage(m_pDlgACFInspectTool->m_DlgPatternCornerSearchTool.m_imageMask);
			m_pDlgACFInspectTool->m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		}
		else if (((CACFInspectTrainDataParam*)m_pTrainDataParam)->GetRoughSearchToolType()==eCornerSearchTool)
		{
			m_pSearchTool->GetMaskImage(m_pDlgACFInspectTool->m_DlgCornerSearchTool.m_imageMask);
			m_pDlgACFInspectTool->m_DlgCornerSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
			
		}

		m_pSearchTool->GetTrainImage(m_imageTrain);
		m_pSearchTool->SetDisplay(&m_guiDisplay);

		if (m_pSearchTool->GetSearchToolType() == eQuickSearchTool)
		{
			m_pSearchTool->SetTrainResultDisplay(&m_pDlgACFInspectTool->m_DlgQuickSearchTool.m_GuiPatternDisplay);
		}
		else
		{
			m_pSearchTool->SetTrainResultDisplay(&m_pDlgACFInspectTool->m_DlgCornerSearchTool.m_GuiPatternDisplay);
		}
		//m_pSearchTool->SetTrainResultDisplay(&m_pDlgACFInspectTool->m_DlgCornerSearchTool.m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgACFInspectTool->ShowWindow(TRUE);
		m_pDlgACFInspectTool->UpdateTrainData(false);
		m_pDlgACFInspectTool->UpdateSearchData(false);

		m_pDlgACFInspectTool->UpdateDlgItem();
		m_pDlgACFInspectTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pSearchTool		= new CVirtualRectSearchTool(*((CVirtualRectSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CVirtualRectSearchTrainDataParam;
		m_pTrainGuiParam	= new CVirtualRectSearchTrainGuiParam;
		m_pSearchDataParam	= new CVirtualRectSearchSearchDataParam;
		m_pSearchGuiParam	= new CVirtualRectSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgVirtualRectSearchTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgVirtualRectSearchTool->ShowWindow(TRUE);
		m_pDlgVirtualRectSearchTool->UpdateTrainData(false);
		m_pDlgVirtualRectSearchTool->UpdateSearchData(false);
		m_pDlgVirtualRectSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgVirtualRectSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
		m_pDlgVirtualRectSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternVirtualRectSearchTool(*((CPatternVirtualRectSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CPatternVirtualRectSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternVirtualRectSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternVirtualRectSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternVirtualRectSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgPatternVirtualRectSearchTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternVirtualRectSearchTool->ShowWindow(TRUE);
		m_pDlgPatternVirtualRectSearchTool->UpdateTrainData(false);
		m_pDlgPatternVirtualRectSearchTool->UpdateSearchData(false);
		m_pDlgPatternVirtualRectSearchTool->m_GuiPatternDisplay.RemoveImage();
		m_pDlgPatternVirtualRectSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
		m_pDlgPatternVirtualRectSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL))
	{
		m_pSearchTool		= new CTwoQuickModelSearchTool(*((CTwoQuickModelSearchTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CTwoQuickModelSearchTrainDataParam;
		m_pTrainGuiParam	= new CTwoQuickModelSearchTrainGuiParam;
		m_pSearchDataParam	= new CTwoQuickModelSearchSearchDataParam;
		m_pSearchGuiParam	= new CTwoQuickModelSearchSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetMaskImage(m_pDlgTwoQuickModelSearchTool->m_imageMask);
		m_pSearchTool->GetMaskImage2(m_pDlgTwoQuickModelSearchTool->m_imageMask1);
		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetPatternImage2(imagePattern2);

		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgTwoQuickModelSearchTool->ShowWindow(TRUE);
		m_pDlgTwoQuickModelSearchTool->UpdateTrainData(false);
		m_pDlgTwoQuickModelSearchTool->UpdateSearchData(false);
		m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay2.SetImage(imagePattern2, TRUE);
		m_pDlgTwoQuickModelSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_DMCODE_TOOL))
	{

		m_pSearchTool		= new CIdReaderDmCodeTool(*((CIdReaderDmCodeTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CIdReaderDmCodeTrainDataParam;
		m_pTrainGuiParam	= new CIdReaderDmCodeTrainGuiParam;
		m_pSearchDataParam	= new CIdReaderDmCodeSearchDataParam;
		m_pSearchGuiParam	= new CIdReaderDmCodeSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgIdReaderDmCodeTool->ShowWindow(TRUE);
		m_pDlgIdReaderDmCodeTool->UpdateTrainData(false);
		m_pDlgIdReaderDmCodeTool->UpdateSearchData(false);
		m_pDlgIdReaderDmCodeTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_BARCODE_TOOL))
	{

		m_pSearchTool		= new CIdReaderBarCodeTool(*((CIdReaderBarCodeTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CIdReaderBarCodeTrainDataParam;
		m_pTrainGuiParam	= new CIdReaderBarCodeTrainGuiParam;
		m_pSearchDataParam	= new CIdReaderBarCodeSearchDataParam;
		m_pSearchGuiParam	= new CIdReaderBarCodeSearchGuiParam;

		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

		m_pSearchTool->GetPatternImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);

		m_pDlgIdReaderBarCodeTool->ShowWindow(TRUE);
		m_pDlgIdReaderBarCodeTool->UpdateTrainData(false);
		m_pDlgIdReaderBarCodeTool->UpdateSearchData(false);
		m_pDlgIdReaderBarCodeTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_POSLINEDISTANCE_TOOL))
	{
		m_pSearchTool		= new CQuickPosLineDistanceTool(*((CQuickPosLineDistanceTool*)m_pDefSearchTool));
		m_pSearchTool->RestoreTrainParam();
		m_pSearchTool->RestoreSearchParam();
		m_pTrainDataParam	= new CQuickPosLineDistanceTrainDataParam;
		m_pTrainGuiParam	= new CQuickPosLineDistanceTrainGuiParam;
		m_pSearchDataParam	= new CQuickPosLineDistanceSearchDataParam;
		m_pSearchGuiParam	= new CQuickPosLineDistanceSearchGuiParam;
		m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
		m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
		m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
		m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
		m_pSearchTool->GetMaskImage(m_pDlgQuickPosLineDistanceTool->m_imageMask);
		m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainImage(m_imageTrain);

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickPosLineDistanceTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickPosLineDistanceTool->ShowWindow(TRUE);
		m_pDlgQuickPosLineDistanceTool->UpdateTrainData(false);
		m_pDlgQuickPosLineDistanceTool->UpdateSearchData(false);		
		m_pDlgQuickPosLineDistanceTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
		m_pDlgQuickPosLineDistanceTool->UpdateControlDisplay();
	}
	    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_CORNER_TOOL))
    {
        m_pSearchTool		= new CCombinationCornerSearchTool(*((CCombinationCornerSearchTool*)m_pDefSearchTool));
        m_pSearchTool->RestoreTrainParam();
        m_pSearchTool->RestoreSearchParam();
        m_pTrainDataParam	= new CCombinationCornerSearchTrainDataParam;
        m_pTrainGuiParam	= new CCombinationCornerSearchTrainGuiParam;
        m_pSearchDataParam	= new CCombinationCornerSearchSearchDataParam;
        m_pSearchGuiParam	= new CCombinationCornerSearchSearchGuiParam;
        m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
        m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
        m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
        m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
        m_pSearchTool->GetTrainResultImage(imagePattern);
		m_pSearchTool->GetTrainResultImage2(imagePattern2);
        if (((CCombinationCornerSearchTrainDataParam*)m_pTrainDataParam)->GetSubSearchToolType()==ePatternCornerSearchTool)
        {
            m_pSearchTool->GetMaskImage(m_pDlgCombinedCornerSearchTool->m_DlgPatternCornerSearchTool.m_imageMask);
            m_pDlgCombinedCornerSearchTool->m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
        }
        else if (((CCombinationCornerSearchTrainDataParam*)m_pTrainDataParam)->GetSubSearchToolType()==eCornerSearchTool)
        {
            m_pSearchTool->GetMaskImage(m_pDlgCombinedCornerSearchTool->m_DlgCornerSearchTool.m_imageMask);
			m_pSearchTool->GetPatternImage(imagePattern);
            m_pDlgCombinedCornerSearchTool->m_DlgCornerSearchTool.m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
			
        }
        if (((CCombinationCornerSearchTrainDataParam*)m_pTrainDataParam)->GetInspectToolType()==ePatternSearchTool)
        {
            m_pSearchTool->GetMaskImage2(m_pDlgCombinedCornerSearchTool->m_DlgPatternSearchTool.m_imageMask);
            m_pDlgCombinedCornerSearchTool->m_DlgPatternSearchTool.m_GuiPatternDisplay.SetImage(imagePattern2, TRUE);
        }
        else if (((CCombinationCornerSearchTrainDataParam*)m_pTrainDataParam)->GetInspectToolType()==eQuickSearchTool)
        {
            m_pSearchTool->GetMaskImage2(m_pDlgCombinedCornerSearchTool->m_DlgQuickSearchTool.m_imageMask);
			m_pSearchTool->GetPatternImage2(imagePattern2);
            m_pDlgCombinedCornerSearchTool->m_DlgQuickSearchTool.m_GuiPatternDisplay.SetImage(imagePattern2, TRUE);
			
        }
        m_pSearchTool->GetTrainImage(m_imageTrain);
        m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgCombinedCornerSearchTool->m_DlgQuickSearchTool.m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCombinedCornerSearchTool->m_DlgCornerSearchTool.m_GuiPatternDisplay);
        m_pSearchTool->ShowTrainGui(false);
        m_pSearchTool->ShowSearchResultGui(false);
        m_pDlgCombinedCornerSearchTool->ShowWindow(TRUE);
        m_pDlgCombinedCornerSearchTool->UpdateTrainData(false);
        m_pDlgCombinedCornerSearchTool->UpdateSearchData(false);
        m_pDlgCombinedCornerSearchTool->UpdateDlgItem();
        m_pDlgCombinedCornerSearchTool->UpdateControlDisplay();

    }
		else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_SEARCH_TOOL))
		{
			m_pSearchTool		= new CMQuickPickSearchTool(*((CMQuickPickSearchTool*)m_pDefSearchTool));
			m_pSearchTool->RestoreTrainParam();
			m_pSearchTool->RestoreSearchParam();
			m_pTrainDataParam	= new CMQuickPickSearchTrainDataParam;
			m_pTrainGuiParam	= new CMQuickPickSearchTrainGuiParam;
			m_pSearchDataParam	= new CMQuickPickSearchSearchDataParam;
			m_pSearchGuiParam	= new CMQuickPickSearchSearchGuiParam;
			m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
			m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
			m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
			m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

			m_pSearchTool->GetMaskImage(m_pDlgMQuickPickSearchTool->m_imageMask);

			m_pSearchTool->GetPatternImage(imagePattern);
			m_pSearchTool->GetTrainImage(m_imageTrain);

			m_pSearchTool->SetDisplay(&m_guiDisplay);
			m_pSearchTool->SetTrainResultDisplay(&m_pDlgMQuickPickSearchTool->m_GuiPatternDisplay);
			m_pSearchTool->ShowTrainGui(false);
			m_pSearchTool->ShowSearchResultGui(false);
			m_pDlgMQuickPickSearchTool->ShowWindow(TRUE);
			m_pDlgMQuickPickSearchTool->UpdateTrainData(false);
			m_pDlgMQuickPickSearchTool->UpdateSearchData(false);		
			m_pDlgMQuickPickSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
			m_pDlgMQuickPickSearchTool->UpdateControlDisplay();
		}

		else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_PICK_SEARCH_TOOL))
		{
			m_pSearchTool		= new CMPatternPickSearchTool(*((CMPatternPickSearchTool*)m_pDefSearchTool));
			m_pSearchTool->RestoreTrainParam();
			m_pSearchTool->RestoreSearchParam();
			m_pTrainDataParam	= new CMPatternPickSearchTrainDataParam;
			m_pTrainGuiParam	= new CMPatternPickSearchTrainGuiParam;
			m_pSearchDataParam	= new CMPatternPickSearchSearchDataParam;
			m_pSearchGuiParam	= new CMPatternPickSearchSearchGuiParam;
			m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
			m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
			m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
			m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);

			m_pSearchTool->GetMaskImage(m_pDlgMPatternPickSearchTool->m_imageMask);

			m_pSearchTool->GetTrainResultImage(imagePattern);
			m_pSearchTool->GetTrainImage(m_imageTrain);

			m_pSearchTool->SetDisplay(&m_guiDisplay);
			m_pSearchTool->ShowTrainGui(false);
			m_pSearchTool->ShowSearchResultGui(false);
			m_pDlgMPatternPickSearchTool->ShowWindow(TRUE);
			m_pDlgMPatternPickSearchTool->UpdateTrainData(false);
			m_pDlgMPatternPickSearchTool->UpdateSearchData(false);		
			m_pDlgMPatternPickSearchTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);
			m_pDlgMPatternPickSearchTool->UpdateControlDisplay();
		}
		else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREAINSPECT_PICK_TOOL))
		{
			m_pSearchTool		= new CAreaInspectPickTool(*((CAreaInspectPickTool*)m_pDefSearchTool));
			m_pSearchTool->RestoreTrainParam();
			m_pSearchTool->RestoreSearchParam();
			m_pTrainDataParam	= new CAreaInspectPickTrainDataParam;
			m_pTrainGuiParam	= new CAreaInspectPickTrainGuiParam;
			m_pSearchDataParam	= new CAreaInspectPickSearchDataParam;
			m_pSearchGuiParam	= new CAreaInspectPickSearchGuiParam;
			m_pSearchTool->GetTrainDataParam(m_pTrainDataParam);
			m_pSearchTool->GetTrainGuiParam(m_pTrainGuiParam);
			m_pSearchTool->GetSearchDataParam(m_pSearchDataParam);
			m_pSearchTool->GetSearchGuiParam(m_pSearchGuiParam);
			m_pSearchTool->GetTrainResultImage(imagePattern);
			m_pSearchTool->GetTrainImage(m_imageTrain);

			m_pSearchTool->SetDisplay(&m_guiDisplay);	
			m_pSearchTool->ShowTrainGui(false);
			m_pSearchTool->ShowSearchResultGui(false);
			m_pDlgAreaInspectPickTool->ShowWindow(TRUE);
			m_pDlgAreaInspectPickTool->UpdateTrainData(false);
			m_pDlgAreaInspectPickTool->UpdateSearchData(false);
			//m_dlgAreaInspectPickTool->m_GuiPatternDisplay.RemoveImage();
			//m_dlgAreaInspectPickTool->m_GuiPatternDisplay.SetImage(imagePattern, TRUE);			
			m_pDlgAreaInspectPickTool->UpdateControlDisplay();
		}
}
void CDlgSearch::CreateToolDlg(CString strSelText)
{
	CRect rs;
	m_ctrlTabControl.GetClientRect(&rs);
	//调整子对话框在父窗口中的位置
	rs.top+= 0; 
	rs.left+=0; 
	rs.right-=0; 



	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL))
	{
		m_pDlgQuickSearchTool->m_pDlgParent = this;
		m_pDlgQuickSearchTool->Create(IDD_DLG_QUICKSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgQuickSearchTool->MoveWindow(&rs);
		m_pDlgQuickSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL))
	{
		m_pDlgPatternSearchTool->m_pDlgParent = this;
		m_pDlgPatternSearchTool->Create(IDD_DLG_PATTERNSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternSearchTool->MoveWindow(&rs);
		m_pDlgPatternSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_SEARCH_TOOL))
	{
		m_pDlgLineSearchTool->m_pDlgParent = this;
		m_pDlgLineSearchTool->Create(IDD_DLG_LINESEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgLineSearchTool->MoveWindow(&rs);
		m_pDlgLineSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL))
	{
		m_pDlgCornerSearchTool->m_pDlgParent = this;
		m_pDlgCornerSearchTool->Create(IDD_DLG_CORNERSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgCornerSearchTool->MoveWindow(&rs);
		m_pDlgCornerSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgCircleSearchTool->m_pDlgParent = this;
		m_pDlgCircleSearchTool->Create(IDD_DLG_CIRCLESEARCH_TOOL,&m_ctrlTabControl); 
		m_pDlgCircleSearchTool->MoveWindow(&rs);
		m_pDlgCircleSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CROSS_SEARCH_TOOL))
	{
		m_pDlgCrossSearchTool->m_pDlgParent = this;
		m_pDlgCrossSearchTool->Create(IDD_DLG_CROSSSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgCrossSearchTool->MoveWindow(&rs);
		m_pDlgCrossSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATCORNER_SEARCH_TOOL))
	{
		m_pDlgPatCornerSearchTool->m_pDlgParent = this;
		m_pDlgPatCornerSearchTool->Create(IDD_DLG_PATCORNERSEARCH_TOOL,&m_ctrlTabControl); 
		m_pDlgPatCornerSearchTool->MoveWindow(&rs);
		m_pDlgPatCornerSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PROCIRCLE_SEARCH_TOOL))
	{
		m_pDlgProCircleSearchTool->m_pDlgParent = this;
		m_pDlgProCircleSearchTool->Create(IDD_DLG_PROCIRCLESEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgProCircleSearchTool->MoveWindow(&rs);
		m_pDlgProCircleSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FIXEDPOS_SEARCH_TOOL))
	{
		m_pDlgFixedPosSearchTool->m_pDlgParent = this;
		m_pDlgFixedPosSearchTool->Create(IDD_DLG_FIXEDPOSSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgFixedPosSearchTool->MoveWindow(&rs);
		m_pDlgFixedPosSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DMCOLDE_CALIBRATE_TOOL))
	{
		m_pDlgDmCodeCalibrateTool->m_pDlgParent = this;
		m_pDlgDmCodeCalibrateTool->Create(IDD_DLG_DMCODE_CALIBRATE_TOOL,&m_ctrlTabControl);
		m_pDlgDmCodeCalibrateTool->MoveWindow(&rs);
		m_pDlgDmCodeCalibrateTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL))
	{
		m_pDlgPatternCornerSearchTool->m_pDlgParent = this;
		m_pDlgPatternCornerSearchTool->Create(IDD_DLG_PATTERNCORNERSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternCornerSearchTool->MoveWindow(&rs);
		m_pDlgPatternCornerSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_QUICK_SEARCH_TOOL))
	{
		m_pDlgCombinedQuickSearchTool->m_pDlgParent = this;
		m_pDlgCombinedQuickSearchTool->Create(IDD_DLG_COMBINED_QUICKSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgCombinedQuickSearchTool->MoveWindow(&rs);
		m_pDlgCombinedQuickSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL))
	{
		m_pDlgCombinedPatternSearchTool->m_pDlgParent = this;
		m_pDlgCombinedPatternSearchTool->Create(IDD_DLG_COMBINED_PATTERNSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgCombinedPatternSearchTool->MoveWindow(&rs);
		m_pDlgCombinedPatternSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BASE_CORNER_SEARCH_TOOL))
	{
		m_pDlgBaseCornerSearchTool->m_pDlgParent = this;
		m_pDlgBaseCornerSearchTool->Create(IDD_DLG_BASECORNERSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgBaseCornerSearchTool->MoveWindow(&rs);
		m_pDlgBaseCornerSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_TOOL))
	{
		m_pDlgPatternSearchCheckTool->m_pDlgParent = this;
		m_pDlgPatternSearchCheckTool->Create(IDD_DLG_PATTERNSEARCH_CHECK_TOOL,&m_ctrlTabControl);
		m_pDlgPatternSearchCheckTool->MoveWindow(&rs);
		m_pDlgPatternSearchCheckTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_TOOL))
	{
		m_pDlgQuickSearchCheckTool->m_pDlgParent = this;
		m_pDlgQuickSearchCheckTool->Create(IDD_DLG_QUICKSEARCH_CHECK_TOOL,&m_ctrlTabControl);
		m_pDlgQuickSearchCheckTool->MoveWindow(&rs);
		m_pDlgQuickSearchCheckTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_OBROUND_TOOL))
	{
		m_pDlgQuickSearchObroundTool->m_pDlgParent = this;
		m_pDlgQuickSearchObroundTool->Create(IDD_DLG_QUICKSEARCH_OBROUND_TOOL,&m_ctrlTabControl);
		m_pDlgQuickSearchObroundTool->MoveWindow(&rs);
		m_pDlgQuickSearchObroundTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL))
	{
		m_pDlgPatternSearchObroundTool->m_pDlgParent = this;
		m_pDlgPatternSearchObroundTool->Create(IDD_DLG_PATTERNSEARCH_OBROUND_TOOL,&m_ctrlTabControl);
		m_pDlgPatternSearchObroundTool->MoveWindow(&rs);
		m_pDlgPatternSearchObroundTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_SEARCH_TOOL))
	{
		m_pDlgMPatternSearchTool->m_pDlgParent = this;
		m_pDlgMPatternSearchTool->Create(IDD_DLG_MPATTERNSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgMPatternSearchTool->MoveWindow(&rs);
		m_pDlgMPatternSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_SEARCH_TOOL))
	{
		m_pDlgMQuickSearchTool->m_pDlgParent = this;
		m_pDlgMQuickSearchTool->Create(IDD_DLG_MQUICKSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgMQuickSearchTool->MoveWindow(&rs);
		m_pDlgMQuickSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_CORNER_SEARCH_TOOL))
	{
		m_pDlgMQuickCornerSearchTool->m_pDlgParent = this;
		m_pDlgMQuickCornerSearchTool->Create(IDD_DLG_MQUICKCORNERSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgMQuickCornerSearchTool->MoveWindow(&rs);
		m_pDlgMQuickCornerSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL))
	{
		m_pDlgMQuickPickCornerSearchTool->m_pDlgParent = this;
		m_pDlgMQuickPickCornerSearchTool->Create(IDD_DLG_MQUICKPICKCORNERSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgMQuickPickCornerSearchTool->MoveWindow(&rs);
		m_pDlgMQuickPickCornerSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_GENRECT_TOOL))
	{
		m_pDlgQuickSearchGenRectTool->m_pDlgParent = this;
		m_pDlgQuickSearchGenRectTool->Create(IDD_DLG_QUICKSEARCH_GENRECT_TOOL,&m_ctrlTabControl);
		m_pDlgQuickSearchGenRectTool->MoveWindow(&rs);
		m_pDlgQuickSearchGenRectTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL))
	{
		m_pDlgPatternSearchGenRectTool->m_pDlgParent = this;
		m_pDlgPatternSearchGenRectTool->Create(IDD_DLG_PATTERNSEARCH_GENRECT_TOOL,&m_ctrlTabControl);
		m_pDlgPatternSearchGenRectTool->MoveWindow(&rs);
		m_pDlgPatternSearchGenRectTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_SEARCH_TOOL))
	{
		m_pDlgPatternLineSearchTool->m_pDlgParent = this;
		m_pDlgPatternLineSearchTool->Create(IDD_DLG_PATTERNLINESEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternLineSearchTool->MoveWindow(&rs);
		m_pDlgPatternLineSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_2PATTERN_SEARCH_TOOL))
	{
		m_pDlg2PatternSearchTool->m_pDlgParent = this;
		m_pDlg2PatternSearchTool->Create(IDD_DLG_2PATTERNSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlg2PatternSearchTool->MoveWindow(&rs);
		m_pDlg2PatternSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgQuickLineCircleSearchTool->m_pDlgParent = this;
		m_pDlgQuickLineCircleSearchTool->Create(IDD_DLG_QUICK_LINE_CIRCLE_TOOL,&m_ctrlTabControl);
		m_pDlgQuickLineCircleSearchTool->MoveWindow(&rs);
		m_pDlgQuickLineCircleSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternLineCircleSearchTool->m_pDlgParent = this;
		m_pDlgPatternLineCircleSearchTool->Create(IDD_DLG_PATTERN_LINE_CIRCLE_TOOL,&m_ctrlTabControl);
		m_pDlgPatternLineCircleSearchTool->MoveWindow(&rs);
		m_pDlgPatternLineCircleSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCircleSearchTool->m_pDlgParent = this;
		m_pDlgPatternCircleSearchTool->Create(IDD_DLG_PATTERN_CIRCLESEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternCircleSearchTool->MoveWindow(&rs);
		m_pDlgPatternCircleSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL))
	{
		m_pDlgQuickSearchCheckLineTool->m_pDlgParent = this;
		m_pDlgQuickSearchCheckLineTool->Create(IDD_DLG_QUICKSEARCH_CHECK_LINE_TOOL,&m_ctrlTabControl);
		m_pDlgQuickSearchCheckLineTool->MoveWindow(&rs);
		m_pDlgQuickSearchCheckLineTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL))
	{
		m_pDlgPatternSearchCheckLineTool->m_pDlgParent = this;
		m_pDlgPatternSearchCheckLineTool->Create(IDD_DLG_PATTERNSEARCH_CHECK_LINE_TOOL,&m_ctrlTabControl);
		m_pDlgPatternSearchCheckLineTool->MoveWindow(&rs);
		m_pDlgPatternSearchCheckLineTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_BLOB_SEARCH_TOOL))
	{
		m_pDlgPatternBlobSearchTool->m_pDlgParent = this;
		m_pDlgPatternBlobSearchTool->Create(IDD_DLG_PATTERNBLOBSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternBlobSearchTool->MoveWindow(&rs);
		m_pDlgPatternBlobSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_CHECK_TOOL))
	{
		m_pDlgCornerSearchCheckTool->m_pDlgParent = this;
		m_pDlgCornerSearchCheckTool->Create(IDD_DLG_CORNERSEARCH_CHECK_TOOL,&m_ctrlTabControl);
		m_pDlgCornerSearchCheckTool->MoveWindow(&rs);
		m_pDlgCornerSearchCheckTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL))
	{
		m_pDlgRectSearchTool->m_pDlgParent = this;
		m_pDlgRectSearchTool->Create(IDD_DLG_RECTSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgRectSearchTool->MoveWindow(&rs);
		m_pDlgRectSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_RECT_SEARCH_TOOL))
	{
		m_pDlgPatternRectSearchTool->m_pDlgParent = this;
		m_pDlgPatternRectSearchTool->Create(IDD_DLG_PATTERNRECTSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternRectSearchTool->MoveWindow(&rs);
		m_pDlgPatternRectSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECT_TOOL))
	{
		m_pDlgAreaInspectTool->m_pDlgParent = this;
		m_pDlgAreaInspectTool->Create(IDD_DLG_AREAINSPECT_TOOL,&m_ctrlTabControl);
		m_pDlgAreaInspectTool->MoveWindow(&rs);
		m_pDlgAreaInspectTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECTEX_TOOL))
	{
		m_pDlgAreaInspectExTool->m_pDlgParent = this;
		m_pDlgAreaInspectExTool->Create(IDD_DLG_AREAINSPECT_TOOLEX,&m_ctrlTabControl);
		m_pDlgAreaInspectExTool->MoveWindow(&rs);
		m_pDlgAreaInspectExTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CHORD_SEARCH_TOOL))
	{
		m_pDlgChordSearchTool->m_pDlgParent = this;
		m_pDlgChordSearchTool->Create(IDD_DLG_CHORDSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgChordSearchTool->MoveWindow(&rs);
		m_pDlgChordSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL))
	{
		m_pDlgTwoPatModelSearchTool->m_pDlgParent = this;
		m_pDlgTwoPatModelSearchTool->Create(IDD_DLG_TWO_PATCORNERSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgTwoPatModelSearchTool->MoveWindow(&rs);
		m_pDlgTwoPatModelSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNCORNER_SEARCH_TOOL))
	{
		m_pDlgMPatternCornerSearchTool->m_pDlgParent = this;
		m_pDlgMPatternCornerSearchTool->Create(IDD_DLG_MPATTERNCORNERSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgMPatternCornerSearchTool->MoveWindow(&rs);
		m_pDlgMPatternCornerSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL))
	{
		m_pDlgMPatternPickCornerSearchTool->m_pDlgParent = this;
		m_pDlgMPatternPickCornerSearchTool->Create(IDD_DLG_MPATTERNPICKCORNERSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgMPatternPickCornerSearchTool->MoveWindow(&rs);
		m_pDlgMPatternPickCornerSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerCircleSearchTool->m_pDlgParent = this;
		m_pDlgPatternCornerCircleSearchTool->Create(IDD_DLG_PATTERNCORNERCIRCLESEARCH_TOOL,&m_ctrlTabControl);												
		m_pDlgPatternCornerCircleSearchTool->MoveWindow(&rs);
		m_pDlgPatternCornerCircleSearchTool->ShowWindow(TRUE);
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgQuickCornerCircleSearchTool->m_pDlgParent = this;
		m_pDlgQuickCornerCircleSearchTool->Create(IDD_DLG_QUICK_CORNER_CIRCLESEARCH_TOOL,&m_ctrlTabControl);												
		m_pDlgQuickCornerCircleSearchTool->MoveWindow(&rs);
		m_pDlgQuickCornerCircleSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL))
	{
		m_pDlgPatternPosLineDistanceTool->m_pDlgParent = this;
		m_pDlgPatternPosLineDistanceTool->Create(IDD_DLG_PATTERN_POSLINEDISTANCE_TOOL,&m_ctrlTabControl);
		m_pDlgPatternPosLineDistanceTool->MoveWindow(&rs);
		m_pDlgPatternPosLineDistanceTool->ShowWindow(TRUE);

	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL))
    {
        m_pDlgCornerSearchCheckInspectTool->m_pDlgParent = this;
        m_pDlgCornerSearchCheckInspectTool->Create(IDD_DLG_CORNERSEARCH_CHECK_INSPECT_TOOL,&m_ctrlTabControl);
        m_pDlgCornerSearchCheckInspectTool->MoveWindow(&rs);
        m_pDlgCornerSearchCheckInspectTool->ShowWindow(TRUE);

    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL))
	{
		m_pDlgPatternDoubleSearchTool->m_pDlgParent = this;
		m_pDlgPatternDoubleSearchTool->Create(IDD_DLG_PATTERNDOUBLESEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternDoubleSearchTool->MoveWindow(&rs);
		m_pDlgPatternDoubleSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CHORD_SEARCH_TOOL))
	{
		m_pDlgPatternChordSearchTool->m_pDlgParent = this;
		m_pDlgPatternChordSearchTool->Create(IDD_DLG_PATTERNCHORD_SEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternChordSearchTool->MoveWindow(&rs);
		m_pDlgPatternChordSearchTool->ShowWindow(TRUE);
	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL))
    {
        m_pDlgQuickMulCircleSearchTool->m_pDlgParent = this;
        m_pDlgQuickMulCircleSearchTool->Create(IDD_DLG_QUICKMULCIRCLESEARCH_TOOL,&m_ctrlTabControl);
        m_pDlgQuickMulCircleSearchTool->MoveWindow(&rs);
        m_pDlgQuickMulCircleSearchTool->ShowWindow(TRUE);
    }

	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pDlgPatternCircle4LineSearchTool->m_pDlgParent = this;
		m_pDlgPatternCircle4LineSearchTool->Create(IDD_DLG_PATTERN_CIRCLE_4LINESEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternCircle4LineSearchTool->MoveWindow(&rs);
		m_pDlgPatternCircle4LineSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pDlgQuickCircle4LineSearchTool->m_pDlgParent = this;
		m_pDlgQuickCircle4LineSearchTool->Create(IDD_DLG_QUICK_CIRCLE_4LINESEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgQuickCircle4LineSearchTool->MoveWindow(&rs);
		m_pDlgQuickCircle4LineSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL))
	{
		m_pDlgSpecialPickSearchTool->m_pDlgParent = this;
		m_pDlgSpecialPickSearchTool->Create(IDD_DLG_SPECIAL_PICK_SEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgSpecialPickSearchTool->MoveWindow(&rs);
		m_pDlgSpecialPickSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerLineSearchTool->m_pDlgParent = this;
		m_pDlgPatternCornerLineSearchTool->Create(IDD_DLG_PATTERN_CORNER_LINE_SEARCH_TOOL, &m_ctrlTabControl);
		m_pDlgPatternCornerLineSearchTool->MoveWindow(&rs);
		m_pDlgPatternCornerLineSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternMulCircleSearchTool->m_pDlgParent = this;
		m_pDlgPatternMulCircleSearchTool->Create(IDD_DLG_PATTERNMULCIRCLESEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternMulCircleSearchTool->MoveWindow(&rs);
		m_pDlgPatternMulCircleSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerMulCircleSearchTool->m_pDlgParent = this;
		m_pDlgPatternCornerMulCircleSearchTool->Create(IDD_DLG_PATTERNCORNERMULCIRCLESEARCHTOOL,&m_ctrlTabControl);
		m_pDlgPatternCornerMulCircleSearchTool->MoveWindow(&rs);
		m_pDlgPatternCornerMulCircleSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL))
	{
		m_pDlgPatternCornerMulObroundTool->m_pDlgParent = this;
		m_pDlgPatternCornerMulObroundTool->Create(IDD_DLG_PATTERN_CORNER_MUIOBROUND_TOOL,&m_ctrlTabControl);
		m_pDlgPatternCornerMulObroundTool->MoveWindow(&rs);
		m_pDlgPatternCornerMulObroundTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL))
	{
		m_pDlgQuickCornerMulObroundTool->m_pDlgParent = this;
		m_pDlgQuickCornerMulObroundTool->Create(IDD_DLG_QUICK_CORNER_MUIOBROUND_TOOL,&m_ctrlTabControl);
		m_pDlgQuickCornerMulObroundTool->MoveWindow(&rs);
		m_pDlgQuickCornerMulObroundTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL))
	{
		m_pDlgCircleCornerSearchCheckInspectTool->m_pDlgParent = this;
		m_pDlgCircleCornerSearchCheckInspectTool->Create(IDD_DLG_CIRCLECORNERSEARCH_CHECK_INSPECT_TOOL,&m_ctrlTabControl);
		m_pDlgCircleCornerSearchCheckInspectTool->MoveWindow(&rs);
		m_pDlgCircleCornerSearchCheckInspectTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL))
	{
		m_pDlgACFInspectTool->m_pDlgParent = this;
		m_pDlgACFInspectTool->Create(IDD_DLG_ACF_INSPECT_TOOL,&m_ctrlTabControl);
		m_pDlgACFInspectTool->MoveWindow(&rs);
		m_pDlgACFInspectTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pDlgVirtualRectSearchTool->m_pDlgParent = this;
		m_pDlgVirtualRectSearchTool->Create(IDD_DLG_VIRTUALRECTSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgVirtualRectSearchTool->MoveWindow(&rs);
		m_pDlgVirtualRectSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pDlgPatternVirtualRectSearchTool->m_pDlgParent = this;
		m_pDlgPatternVirtualRectSearchTool->Create(IDD_DLG_PATTERNVIRTUALRECTSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgPatternVirtualRectSearchTool->MoveWindow(&rs);
		m_pDlgPatternVirtualRectSearchTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL))
	{
		m_pDlgTwoQuickModelSearchTool->m_pDlgParent = this;
		m_pDlgTwoQuickModelSearchTool->Create(IDD_DLG_TWO_QUICKMODELSEARCH_TOOL,&m_ctrlTabControl);
		m_pDlgTwoQuickModelSearchTool->MoveWindow(&rs);
		m_pDlgTwoQuickModelSearchTool->ShowWindow(TRUE);

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_DMCODE_TOOL))
	{
		m_pDlgIdReaderDmCodeTool->m_pDlgParent = this;
		m_pDlgIdReaderDmCodeTool->Create(IDD_TOOL_ID_READER_DM_CODE_TOOL,&m_ctrlTabControl);
		m_pDlgIdReaderDmCodeTool->MoveWindow(&rs);
		m_pDlgIdReaderDmCodeTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_BARCODE_TOOL))
	{
		m_pDlgIdReaderBarCodeTool->m_pDlgParent = this;
		m_pDlgIdReaderBarCodeTool->Create(IDD_TOOL_ID_READER_BAR_CODE_TOOL,&m_ctrlTabControl);
		m_pDlgIdReaderBarCodeTool->MoveWindow(&rs);
		m_pDlgIdReaderBarCodeTool->ShowWindow(TRUE);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_POSLINEDISTANCE_TOOL))
	{
		m_pDlgQuickPosLineDistanceTool->m_pDlgParent = this;
		m_pDlgQuickPosLineDistanceTool->Create(IDD_DLG_QUICK_POSLINEDISTANCE_TOOL,&m_ctrlTabControl);
		m_pDlgQuickPosLineDistanceTool->MoveWindow(&rs);
		m_pDlgQuickPosLineDistanceTool->ShowWindow(TRUE);

	}
	    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_CORNER_TOOL))
    {
        m_pDlgCombinedCornerSearchTool->m_pDlgParent = this;
        m_pDlgCombinedCornerSearchTool->Create(IDD_DLG_COMBINED_CORNERSEARCH_TOOL,&m_ctrlTabControl);
        m_pDlgCombinedCornerSearchTool->MoveWindow(&rs);
        m_pDlgCombinedCornerSearchTool->ShowWindow(TRUE);

    }
		else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_SEARCH_TOOL))
		{
			m_pDlgMQuickPickSearchTool->m_pDlgParent = this;
			m_pDlgMQuickPickSearchTool->Create(IDD_DLG_MQUICKPICKSEARCH_TOOL,&m_ctrlTabControl);
			m_pDlgMQuickPickSearchTool->MoveWindow(&rs);
			m_pDlgMQuickPickSearchTool->ShowWindow(TRUE);

		}
		else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_PICK_SEARCH_TOOL))
		{
			m_pDlgMPatternPickSearchTool->m_pDlgParent = this;
			m_pDlgMPatternPickSearchTool->Create(IDD_DLG_MPATTERNPICKSEARCH_TOOL,&m_ctrlTabControl);
			m_pDlgMPatternPickSearchTool->MoveWindow(&rs);
			m_pDlgMPatternPickSearchTool->ShowWindow(TRUE);

		}
		else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREAINSPECT_PICK_TOOL))
		{
			m_pDlgAreaInspectPickTool->m_pDlgParent = this;
			m_pDlgAreaInspectPickTool->Create(IDD_DLG_AREAINSPECT_PICK_TOOL,&m_ctrlTabControl);
			m_pDlgAreaInspectPickTool->MoveWindow(&rs);
			m_pDlgAreaInspectPickTool->ShowWindow(TRUE);

		}
}
void CDlgSearch::UpdateToolDlgControlDisplay(CString strSelText)
{

	CString strInfo;
	if(m_pVisionASM->IsPosCameraGrabbing(m_nCurPositionIdx))
	{
		 strInfo.Format(_T("当前位置[%d][%d]相机处于正在采集状态"), m_nCurPositionIdx, m_pVisionASM->GetPosCameraIndex(m_nCurPositionIdx));
		 g_opLogFile.SaveLogText(strInfo, 7);
	}
	else
	{
		strInfo.Format(_T("当前位置[%d][%d]相机处于停止采集状态"), m_nCurPositionIdx, m_pVisionASM->GetPosCameraIndex(m_nCurPositionIdx));
		g_opLogFile.SaveLogText(strInfo, 7);
	}

	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL))
	{
		m_pDlgQuickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL))
	{
		m_pDlgPatternSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_SEARCH_TOOL))
	{
		m_pDlgLineSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL))
	{
		m_pDlgCornerSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgCircleSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CROSS_SEARCH_TOOL))
	{
		m_pDlgCrossSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATCORNER_SEARCH_TOOL))
	{
		m_pDlgPatCornerSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PROCIRCLE_SEARCH_TOOL))
	{
		m_pDlgProCircleSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FIXEDPOS_SEARCH_TOOL))
	{
		m_pDlgFixedPosSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DMCOLDE_CALIBRATE_TOOL))
	{
		m_pDlgDmCodeCalibrateTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL))
	{
		m_pDlgPatternCornerSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_QUICK_SEARCH_TOOL))
	{
		m_pDlgCombinedQuickSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL))
	{
		m_pDlgCombinedPatternSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BASE_CORNER_SEARCH_TOOL))
	{
		m_pDlgBaseCornerSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_TOOL))
	{
		m_pDlgPatternSearchCheckTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_TOOL))
	{
		m_pDlgQuickSearchCheckTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_OBROUND_TOOL))
	{
		m_pDlgQuickSearchObroundTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL))
	{
		m_pDlgPatternSearchObroundTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_SEARCH_TOOL))
	{
		m_pDlgMPatternSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_SEARCH_TOOL))
	{
		m_pDlgMQuickSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_CORNER_SEARCH_TOOL))
	{
		m_pDlgMQuickCornerSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL))
	{
		m_pDlgMQuickPickCornerSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_GENRECT_TOOL))
	{
		m_pDlgQuickSearchGenRectTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL))
	{
		m_pDlgPatternSearchGenRectTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_SEARCH_TOOL))
	{
		m_pDlgPatternLineSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_2PATTERN_SEARCH_TOOL))
	{
		m_pDlg2PatternSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgQuickLineCircleSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternLineCircleSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCircleSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL))
	{
		m_pDlgQuickSearchCheckLineTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL))
	{
		m_pDlgPatternSearchCheckLineTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_BLOB_SEARCH_TOOL))
	{
		m_pDlgPatternBlobSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_CHECK_TOOL))
	{
		m_pDlgCornerSearchCheckTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL))
	{
		m_pDlgRectSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_RECT_SEARCH_TOOL))
	{
		m_pDlgPatternRectSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECT_TOOL))
	{
		m_pDlgAreaInspectTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECTEX_TOOL))
	{
		m_pDlgAreaInspectExTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CHORD_SEARCH_TOOL))
	{
		m_pDlgChordSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL))
	{
		m_pDlgTwoPatModelSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNCORNER_SEARCH_TOOL))
	{
		m_pDlgMPatternCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL))
	{
		m_pDlgMPatternPickCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerCircleSearchTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgQuickCornerCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL))
	{
		m_pDlgPatternPosLineDistanceTool->UpdateControlDisplay();

	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL))
    {
        m_pDlgCornerSearchCheckInspectTool->UpdateControlDisplay();

    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL))
	{
		m_pDlgPatternDoubleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CHORD_SEARCH_TOOL))
	{
		m_pDlgPatternChordSearchTool->UpdateControlDisplay();
	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL))
    {
        m_pDlgQuickMulCircleSearchTool->UpdateControlDisplay();
    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pDlgPatternCircle4LineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pDlgQuickCircle4LineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL))
	{
		m_pDlgSpecialPickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerLineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternMulCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerMulCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL))
	{
		m_pDlgPatternCornerMulObroundTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL))
	{
		m_pDlgQuickCornerMulObroundTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL))
	{
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL))
	{
		m_pDlgACFInspectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pDlgVirtualRectSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pDlgPatternVirtualRectSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL))
	{
		m_pDlgTwoQuickModelSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_DMCODE_TOOL))
	{
		m_pDlgIdReaderDmCodeTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_BARCODE_TOOL))
	{
		m_pDlgIdReaderBarCodeTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_CORNER_TOOL))
    {
        m_pDlgCombinedCornerSearchTool->UpdateControlDisplay();

    }
		else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_POSLINEDISTANCE_TOOL))
	{
		m_pDlgQuickPosLineDistanceTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_SEARCH_TOOL))
	{
		m_pDlgMQuickPickSearchTool->UpdateControlDisplay();

	}else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_PICK_SEARCH_TOOL))
	{
		m_pDlgMPatternPickSearchTool->UpdateControlDisplay();

	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREAINSPECT_PICK_TOOL))
	{
		m_pDlgAreaInspectPickTool->UpdateControlDisplay();

	}
}
void CDlgSearch::InitSearchTooParam(CString strSelText)
{
	// 清空数据
	if (m_pSearchTool != NULL)
	{
		m_pSearchTool->ClearAllGui();
		if (m_pSearchTool != m_pDefSearchTool)
		{
			delete m_pSearchTool;
		}
		m_pSearchTool = NULL;
	}
	if (m_pTrainDataParam != NULL)
	{
		delete m_pTrainDataParam;
		m_pTrainDataParam = NULL;
	}
	if (m_pTrainGuiParam != NULL)
	{
		delete m_pTrainGuiParam;	
		m_pTrainGuiParam	= NULL;
	}
	if (m_pSearchDataParam != NULL)
	{
		delete m_pSearchDataParam;
		m_pSearchDataParam = NULL;
	}
	if (m_pSearchGuiParam != NULL)
	{
		delete m_pSearchGuiParam;
		m_pSearchGuiParam = NULL;
	}
	
	bool bValidateDlgTrainData = true;
	bool bValidateDlgSearchData = true;
	// 创建定位工具
	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL))
	{
		m_pSearchTool		= new CQuickSearchTool;
		m_pTrainDataParam	= new CQuickSearchTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchTool->ShowWindow(TRUE);
		m_pDlgQuickSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternSearchTool;
		m_pTrainDataParam	= new CPatternSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchTool->ShowWindow(TRUE);
		m_pDlgPatternSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CLineSearchTool;
		m_pTrainDataParam	= new CLineSearchTrainDataParam;
		m_pTrainGuiParam	= new CLineSearchTrainGuiParam;
		m_pSearchDataParam	= new CLineSearchSearchDataParam;
		m_pSearchGuiParam	= new CLineSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgLineSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgLineSearchTool->ShowWindow(TRUE);
		m_pDlgLineSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgLineSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgLineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCornerSearchTool;
		m_pTrainDataParam	= new CCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CCornerSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCornerSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCornerSearchTool->ShowWindow(TRUE);
		m_pDlgCornerSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgCornerSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCircleSearchTool;
		m_pTrainDataParam	= new CCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CCircleSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCircleSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCircleSearchTool->ShowWindow(TRUE);
		m_pDlgCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CROSS_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCrossSearchTool;
		m_pTrainDataParam	= new CCrossSearchTrainDataParam;
		m_pTrainGuiParam	= new CCrossSearchTrainGuiParam;
		m_pSearchDataParam	= new CCrossSearchSearchDataParam;
		m_pSearchGuiParam	= new CCrossSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCrossSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCrossSearchTool->ShowWindow(TRUE);
		m_pDlgCrossSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgCrossSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgCrossSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATCORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatCornerSearchTool;
		m_pTrainDataParam	= new CPatCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatCornerSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatCornerSearchTool->ShowWindow(TRUE);
		m_pDlgPatCornerSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatCornerSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PROCIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CProCircleSearchTool;
		m_pTrainDataParam	= new CProCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CProCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CProCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CProCircleSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgProCircleSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgProCircleSearchTool->ShowWindow(TRUE);
		m_pDlgProCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgProCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgProCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FIXEDPOS_SEARCH_TOOL))
	{
		m_pSearchTool		= new CFixedPosSearchTool;
		m_pTrainDataParam	= new CFixedPosSearchTrainDataParam;
		m_pTrainGuiParam	= new CFixedPosSearchTrainGuiParam;
		m_pSearchDataParam	= new CFixedPosSearchSearchDataParam;
		m_pSearchGuiParam	= new CFixedPosSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgFixedPosSearchTool->ShowWindow(TRUE);
		m_pDlgFixedPosSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgFixedPosSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgFixedPosSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DMCOLDE_CALIBRATE_TOOL))
	{
		m_pSearchTool		= new CDmCodeCalibrateTool;
		m_pTrainDataParam	= new CDmCodeCalibrateTrainDataParam;
		m_pTrainGuiParam	= new CDmCodeCalibrateTrainGuiParam;
		m_pSearchDataParam	= new CDmCodeCalibrateSearchDataParam;
		m_pSearchGuiParam	= new CDmCodeCalibrateSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgDmCodeCalibrateTool->ShowWindow(TRUE);
		m_pDlgDmCodeCalibrateTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgDmCodeCalibrateTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgDmCodeCalibrateTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCornerSearchTool;
		m_pTrainDataParam	= new CPatternCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternCornerSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCornerSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternCornerSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_QUICK_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCombinedQuickSearch;
		m_pTrainDataParam	= new CCombinedQuickSearchTrainDataParam;
		m_pTrainGuiParam	= new CCombinedQuickSearchTrainGuiParam;
		m_pSearchDataParam	= new CCombinedQuickSearchDataParam;
		m_pSearchGuiParam	= new CCombinedQuickSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgCombinedQuickSearchTool->m_GuiPatternDisplay2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCombinedQuickSearchTool->ShowWindow(TRUE);
		m_pDlgCombinedQuickSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgCombinedQuickSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgCombinedQuickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL))
	{
		m_pSearchTool		= new CCombinedPatternSearchTool;
		m_pTrainDataParam	= new CCombinedPatternSearchTrainDataParam;
		m_pTrainGuiParam	= new CCombinedPatternSearchTrainGuiParam;
		m_pSearchDataParam	= new CCombinedPatternSearchSearchDataParam;
		m_pSearchGuiParam	= new CCombinedPatternSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCombinedPatternSearchTool->ShowWindow(TRUE);
		m_pDlgCombinedPatternSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgCombinedPatternSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgCombinedPatternSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BASE_CORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CBaseCornerSearchTool;
		m_pTrainDataParam	= new CBaseCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CBaseCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CBaseCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CBaseCornerSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgBaseCornerSearchTool->ShowWindow(TRUE);
		m_pDlgBaseCornerSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgBaseCornerSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgBaseCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_TOOL))
	{
		m_pSearchTool		= new CPatternSearchCheckTool;
		m_pTrainDataParam	= new CPatternSearchCheckTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchCheckTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchCheckSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchCheckSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchCheckTool->ShowWindow(TRUE);
		m_pDlgPatternSearchCheckTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternSearchCheckTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternSearchCheckTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_TOOL))
	{
		m_pSearchTool		= new CQuickSearchCheckTool;
		m_pTrainDataParam	= new CQuickSearchCheckTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchCheckTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchCheckSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchCheckSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchCheckTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck1);
		m_pSearchTool->SetTrainResultDisplay3(&m_pDlgQuickSearchCheckTool->m_GuiPatternDisplayCheck2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchCheckTool->ShowWindow(TRUE);
		m_pDlgQuickSearchCheckTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickSearchCheckTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickSearchCheckTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_OBROUND_TOOL))
	{
		m_pSearchTool		= new CQuickSearchObroundTool;
		m_pTrainDataParam	= new CQuickSearchObroundTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchObroundTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchObroundSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchObroundSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchObroundTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchObroundTool->ShowWindow(TRUE);
		m_pDlgQuickSearchObroundTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickSearchObroundTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickSearchObroundTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL))
	{
		m_pSearchTool		= new CPatternSearchObroundTool;
		m_pTrainDataParam	= new CPatternSearchObroundTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchObroundTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchObroundSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchObroundSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchObroundTool->ShowWindow(TRUE);
		m_pDlgPatternSearchObroundTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternSearchObroundTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternSearchObroundTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMPatternSearchTool;
		m_pTrainDataParam	= new CMPatternSearchTrainDataParam;
		m_pTrainGuiParam	= new CMPatternSearchTrainGuiParam;
		m_pSearchDataParam	= new CMPatternSearchSearchDataParam;
		m_pSearchGuiParam	= new CMPatternSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMPatternSearchTool->ShowWindow(TRUE);
		m_pDlgMPatternSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgMPatternSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgMPatternSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMQuickSearchTool;
		m_pTrainDataParam	= new CMQuickSearchTrainDataParam;
		m_pTrainGuiParam	= new CMQuickSearchTrainGuiParam;
		m_pSearchDataParam	= new CMQuickSearchSearchDataParam;
		m_pSearchGuiParam	= new CMQuickSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgMQuickSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMQuickSearchTool->ShowWindow(TRUE);
		m_pDlgMQuickSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgMQuickSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgMQuickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_CORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMQuickCornerSearchTool;
		m_pTrainDataParam	= new CMQuickCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CMQuickCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CMQuickCornerSearchDataParam;
		m_pSearchGuiParam	= new CMQuickCornerSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgMQuickCornerSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMQuickCornerSearchTool->ShowWindow(TRUE);
		m_pDlgMQuickCornerSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgMQuickCornerSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgMQuickCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMQuickPickCornerSearchTool;
		m_pTrainDataParam	= new CMQuickPickCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CMQuickPickCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CMQuickPickCornerSearchDataParam;
		m_pSearchGuiParam	= new CMQuickPickCornerSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgMQuickPickCornerSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMQuickPickCornerSearchTool->ShowWindow(TRUE);
		m_pDlgMQuickPickCornerSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgMQuickPickCornerSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgMQuickPickCornerSearchTool->UpdateControlDisplay();
	}

	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_GENRECT_TOOL))
	{
		m_pSearchTool		= new CQuickSearchGenRectTool;
		m_pTrainDataParam	= new CQuickSearchGenRectTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchGenRectTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchGenRectSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchGenRectSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchGenRectTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchGenRectTool->ShowWindow(TRUE);
		m_pDlgQuickSearchGenRectTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickSearchGenRectTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickSearchGenRectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL))
	{
		m_pSearchTool		= new CPatternSearchGenRectTool;
		m_pTrainDataParam	= new CPatternSearchGenRectTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchGenRectTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchGenRectSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchGenRectSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchGenRectTool->ShowWindow(TRUE);
		m_pDlgPatternSearchGenRectTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternSearchGenRectTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternSearchGenRectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternLineSearchTool;
		m_pTrainDataParam	= new CPatternLineSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternLineSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternLineSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternLineSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternLineSearchTool->ShowWindow(TRUE);
		m_pDlgPatternLineSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternLineSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternLineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_2PATTERN_SEARCH_TOOL))
	{
		m_pSearchTool		= new C2PatternSearchTool;
		m_pTrainDataParam	= new C2PatternSearchTrainDataParam;
		m_pTrainGuiParam	= new C2PatternSearchTrainGuiParam;
		m_pSearchDataParam	= new C2PatternSearchSearchDataParam;
		m_pSearchGuiParam	= new C2PatternSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlg2PatternSearchTool->ShowWindow(TRUE);
		m_pDlg2PatternSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlg2PatternSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlg2PatternSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CQuickLineCircleTool;
		m_pTrainDataParam	= new CQuickLineCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CQuickLineCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CQuickLineCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CQuickLineCircleSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickLineCircleSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickLineCircleSearchTool->ShowWindow(TRUE);
		m_pDlgQuickLineCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickLineCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickLineCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternLineCircleTool;
		m_pTrainDataParam	= new CPatternLineCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternLineCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternLineCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternLineCircleSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternLineCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternLineCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternLineCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternLineCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCircleSearchTool;
		m_pTrainDataParam	= new CPatternCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCircleSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL))
	{
		m_pSearchTool		= new CQuickSearchCheckLineTool;
		m_pTrainDataParam	= new CQuickSearchCheckLineTrainDataParam;
		m_pTrainGuiParam	= new CQuickSearchCheckLineTrainGuiParam;
		m_pSearchDataParam	= new CQuickSearchCheckLineSearchDataParam;
		m_pSearchGuiParam	= new CQuickSearchCheckLineSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck1);
		m_pSearchTool->SetTrainResultDisplay3(&m_pDlgQuickSearchCheckLineTool->m_GuiPatternDisplayCheck2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickSearchCheckLineTool->ShowWindow(TRUE);
		m_pDlgQuickSearchCheckLineTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickSearchCheckLineTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickSearchCheckLineTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL))
	{
		m_pSearchTool		= new CPatternSearchCheckLineTool;
		m_pTrainDataParam	= new CPatternSearchCheckLineTrainDataParam;
		m_pTrainGuiParam	= new CPatternSearchCheckLineTrainGuiParam;
		m_pSearchDataParam	= new CPatternSearchCheckLineSearchDataParam;
		m_pSearchGuiParam	= new CPatternSearchCheckLineSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternSearchCheckLineTool->ShowWindow(TRUE);
		m_pDlgPatternSearchCheckLineTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternSearchCheckLineTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternSearchCheckLineTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_BLOB_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternBlobSearchTool;
		m_pTrainDataParam	= new CPatternBlobSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternBlobSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternBlobSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternBlobSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternBlobSearchTool->ShowWindow(TRUE);
		m_pDlgPatternBlobSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternBlobSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternBlobSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_CHECK_TOOL))
	{
		m_pSearchTool		= new CCornerSearchCheckTool;
		m_pTrainDataParam	= new CCornerSearchCheckTrainDataParam;
		m_pTrainGuiParam	= new CCornerSearchCheckTrainGuiParam;
		m_pSearchDataParam	= new CCornerSearchCheckSearchDataParam;
		m_pSearchGuiParam	= new CCornerSearchCheckSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCornerSearchCheckTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck1);
		m_pSearchTool->SetTrainResultDisplay3(&m_pDlgCornerSearchCheckTool->m_GuiPatternDisplayCheck2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCornerSearchCheckTool->ShowWindow(TRUE);
		m_pDlgCornerSearchCheckTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgCornerSearchCheckTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgCornerSearchCheckTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL))
	{
		m_pSearchTool		= new CRectSearchTool;
		m_pTrainDataParam	= new CRectSearchTrainDataParam;
		m_pTrainGuiParam	= new CRectSearchTrainGuiParam;
		m_pSearchDataParam	= new CRectSearchSearchDataParam;
		m_pSearchGuiParam	= new CRectSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgRectSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgRectSearchTool->ShowWindow(TRUE);
		m_pDlgRectSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgRectSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgRectSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_RECT_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternRectSearchTool;
		m_pTrainDataParam	= new CPatternRectSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternRectSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternRectSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternRectSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternRectSearchTool->ShowWindow(TRUE);
		m_pDlgPatternRectSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternRectSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternRectSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECT_TOOL))
	{
		m_pSearchTool		= new CAreaInspectTool;
		m_pTrainDataParam	= new CAreaInspectTrainDataParam;
		m_pTrainGuiParam	= new CAreaInspectTrainGuiParam;
		m_pSearchDataParam	= new CAreaInspectSearchDataParam;
		m_pSearchGuiParam	= new CAreaInspectSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgAreaInspectTool->ShowWindow(TRUE);
		m_pDlgAreaInspectTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgAreaInspectTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgAreaInspectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECTEX_TOOL))
	{
		m_pSearchTool		= new CAreaInspectToolEx;
		m_pTrainDataParam	= new CAreaInspectExTrainDataParam;
		m_pTrainGuiParam	= new CAreaInspectExTrainGuiParam;
		m_pSearchDataParam	= new CAreaInspectExSearchDataParam;
		m_pSearchGuiParam	= new CAreaInspectExSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgAreaInspectExTool->ShowWindow(TRUE);
		m_pDlgAreaInspectExTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgAreaInspectExTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgAreaInspectExTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_CHORD_SEARCH_TOOL))
	{
		m_pSearchTool		= new CChordSearchTool;
		m_pTrainDataParam	= new CChordSearchTrainDataParam;
		m_pTrainGuiParam	= new CChordSearchTrainGuiParam;
		m_pSearchDataParam	= new CChordSearchSearchDataParam;
		m_pSearchGuiParam	= new CChordSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgChordSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgChordSearchTool->ShowWindow(TRUE);
		m_pDlgChordSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgChordSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgChordSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL))
	{
		m_pSearchTool		= new CTwoPatModelSearchTool;
		m_pTrainDataParam	= new CTwoPatModelSearchTrainDataParam;
		m_pTrainGuiParam	= new CTwoPatModelSearchTrainGuiParam;
		m_pSearchDataParam	= new CTwoPatModelSearchSearchDataParam;
		m_pSearchGuiParam	= new CTwoPatModelSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgTwoPatModelSearchTool->ShowWindow(TRUE);
		m_pDlgTwoPatModelSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgTwoPatModelSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgTwoPatModelSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNCORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMPatternCornerSearchTool;
		m_pTrainDataParam	= new CMPatternCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CMPatternCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CMPatternCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CMPatternCornerSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMPatternCornerSearchTool->ShowWindow(TRUE);
		m_pDlgMPatternCornerSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgMPatternCornerSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgMPatternCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMPatternPickCornerSearchTool;
		m_pTrainDataParam	= new CMPatternPickCornerSearchTrainDataParam;
		m_pTrainGuiParam	= new CMPatternPickCornerSearchTrainGuiParam;
		m_pSearchDataParam	= new CMPatternPickCornerSearchSearchDataParam;
		m_pSearchGuiParam	= new CMPatternPickCornerSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMPatternPickCornerSearchTool->ShowWindow(TRUE);
		m_pDlgMPatternPickCornerSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgMPatternPickCornerSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgMPatternPickCornerSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCornerCircleSearchTool;
		m_pTrainDataParam	= new CPatternCornerCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerCircleSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternCornerCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCornerCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternCornerCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternCornerCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CQuickCornerCircleSearchTool;
		m_pTrainDataParam	= new CQuickCornerCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CQuickCornerCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CQuickCornerCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CQuickCornerCircleSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickCornerCircleSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickCornerCircleSearchTool->ShowWindow(TRUE);
		m_pDlgQuickCornerCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickCornerCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickCornerCircleSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL))
	{
		m_pSearchTool		= new CPatternPosLineDistanceTool;
		m_pTrainDataParam	= new CPatternPosLineDistanceTrainDataParam;
		m_pTrainGuiParam	= new CPatternPosLineDistanceTrainGuiParam;
		m_pSearchDataParam	= new CPatternPosLineDistanceSearchDataParam;
		m_pSearchGuiParam	= new CPatternPosLineDistanceSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternPosLineDistanceTool->ShowWindow(TRUE);
		m_pDlgPatternPosLineDistanceTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternPosLineDistanceTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternPosLineDistanceTool->UpdateControlDisplay();
	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL))
    {
        m_pSearchTool		= new CCornerCheckSearchTool;
        m_pTrainDataParam	= new CCornerCheckSearchTrainDataParam;
        m_pTrainGuiParam	= new CCornerCheckSearchTrainGuiParam;
        m_pSearchDataParam	= new CCornerCheckSearchSearchDataParam;
        m_pSearchGuiParam	= new CCornerCheckSearchSearchGuiParam;

        m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgCornerSearchCheckInspectTool->m_DlgQuickSearchTool.m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCornerSearchCheckInspectTool->m_DlgCornerSearchTool.m_GuiPatternDisplay);
        m_pSearchTool->ShowTrainGui(false);
        m_pSearchTool->ShowSearchResultGui(false);
        m_pDlgCornerSearchCheckInspectTool->ShowWindow(TRUE);
        m_pDlgCornerSearchCheckInspectTool->UpdateTrainData(bValidateDlgTrainData);
        m_pDlgCornerSearchCheckInspectTool->UpdateSearchData(bValidateDlgSearchData);
        m_pDlgCornerSearchCheckInspectTool->UpdateDlgItem();
        m_pDlgCornerSearchCheckInspectTool->UpdateControlDisplay();
    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL))
	{
		
		m_pSearchTool		= new CPatternDoubleSearchTool;
		
		m_pTrainDataParam	= new CPatternDoubleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternDoubleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternDoubleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternDoubleSearchSearchGuiParam;
		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternDoubleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternDoubleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternDoubleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternDoubleSearchTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CHORD_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternChordSearchTool;
		m_pTrainDataParam	= new CPatternChordSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternChordSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternChordSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternChordSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternChordSearchTool->ShowWindow(TRUE);
		m_pDlgPatternChordSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternChordSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternChordSearchTool->UpdateControlDisplay();
	}
    else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL))
    {
        m_pSearchTool		= new CQuickMulCircleSearchTool;
        m_pTrainDataParam	= new CQuickMulCircleSearchTrainDataParam;
        m_pTrainGuiParam	= new CQuickMulCircleSearchTrainGuiParam;
        m_pSearchDataParam	= new CQuickMulCircleSearchSearchDataParam;
        m_pSearchGuiParam	= new CQuickMulCircleSearchSearchGuiParam;

        m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickMulCircleSearchTool->m_GuiPatternDisplay);
        m_pSearchTool->ShowTrainGui(false);
        m_pSearchTool->ShowSearchResultGui(false);
        m_pDlgQuickMulCircleSearchTool->ShowWindow(TRUE);
        m_pDlgQuickMulCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
        m_pDlgQuickMulCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
        m_pDlgQuickMulCircleSearchTool->UpdateControlDisplay();
    }

	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCircle4LineSearchTool;
		m_pTrainDataParam	= new CPatternCircle4LineSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCircle4LineSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCircle4LineSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCircle4LineSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternCircle4LineSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCircle4LineSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternCircle4LineSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternCircle4LineSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pSearchTool		= new CQuickCircle4LineSearchTool;
		m_pTrainDataParam	= new CQuickCircle4LineSearchTrainDataParam;
		m_pTrainGuiParam	= new CQuickCircle4LineSearchTrainGuiParam;
		m_pSearchDataParam	= new CQuickCircle4LineSearchSearchDataParam;
		m_pSearchGuiParam	= new CQuickCircle4LineSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickCircle4LineSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickCircle4LineSearchTool->ShowWindow(TRUE);
		m_pDlgQuickCircle4LineSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickCircle4LineSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickCircle4LineSearchTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL))
	{
		m_pSearchTool		= new CSpecialPickSearchTool;
		m_pTrainDataParam	= new CSpecialPickSearchTrainDataParam;
		m_pTrainGuiParam	= new CSpecialPickSearchTrainGuiParam;
		m_pSearchDataParam	= new CSpecialPickSearchSearchDataParam;
		m_pSearchGuiParam	= new CSpecialPickSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgSpecialPickSearchTool->m_DlgQuickSearchTool_Rough.m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgSpecialPickSearchTool->m_DlgQuickSearchTool_HeadTail.m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgSpecialPickSearchTool->ShowWindow(TRUE);
		m_pDlgSpecialPickSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgSpecialPickSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgSpecialPickSearchTool->UpdateDlgItem();
		m_pDlgSpecialPickSearchTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCornerLineSearchTool;

		m_pTrainDataParam	= new CPatternCornerLineSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerLineSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerLineSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerLineSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternCornerLineSearchTool->ShowWindow(TRUE);
 		m_pDlgPatternCornerLineSearchTool->UpdateTrainData(bValidateDlgTrainData);
 		m_pDlgPatternCornerLineSearchTool->UpdateSearchData(bValidateDlgSearchData);
 		m_pDlgPatternCornerLineSearchTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternMulCircleSearchTool;
		m_pTrainDataParam	= new CPatternMulCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternMulCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternMulCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternMulCircleSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternMulCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternMulCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternMulCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternMulCircleSearchTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternCornerMulCircleSearchTool;
		m_pTrainDataParam	= new CPatternCornerMulCircleSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerMulCircleSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerMulCircleSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerMulCircleSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternCornerMulCircleSearchTool->ShowWindow(TRUE);
		m_pDlgPatternCornerMulCircleSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternCornerMulCircleSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternCornerMulCircleSearchTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL))
	{
		m_pSearchTool		= new CPatternCornerMulObroundTool;
		m_pTrainDataParam	= new CPatternCornerMulObroundTrainDataParam;
		m_pTrainGuiParam	= new CPatternCornerMulObroundTrainGuiParam;
		m_pSearchDataParam	= new CPatternCornerMulObroundSearchDataParam;
		m_pSearchGuiParam	= new CPatternCornerMulObroundSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternCornerMulObroundTool->ShowWindow(TRUE);
		m_pDlgPatternCornerMulObroundTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternCornerMulObroundTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternCornerMulObroundTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL))
	{
		m_pSearchTool		= new CQuickCornerMulObroundTool;
		m_pTrainDataParam	= new CQuickCornerMulObroundTrainDataParam;
		m_pTrainGuiParam	= new CQuickCornerMulObroundTrainGuiParam;
		m_pSearchDataParam	= new CQuickCornerMulObroundSearchDataParam;
		m_pSearchGuiParam	= new CQuickCornerMulObroundSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickCornerMulObroundTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickCornerMulObroundTool->ShowWindow(TRUE);
		m_pDlgQuickCornerMulObroundTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickCornerMulObroundTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickCornerMulObroundTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL))
	{
		m_pSearchTool		= new CCircleCornerCheckSearchTool;
		m_pTrainDataParam	= new CCircleCornerCheckSearchTrainDataParam;
		m_pTrainGuiParam	= new CCircleCornerCheckSearchTrainGuiParam;
		m_pSearchDataParam	= new CCircleCornerCheckSearchSearchDataParam;
		m_pSearchGuiParam	= new CCircleCornerCheckSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCircleCornerSearchCheckInspectTool->m_DlgCornerSearchTool.m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgCircleCornerSearchCheckInspectTool->ShowWindow(TRUE);
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateDlgItem();
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL))
	{
		m_pSearchTool		= new CACFInspectTool;
		m_pTrainDataParam	= new CACFInspectTrainDataParam;
		m_pTrainGuiParam	= new CACFInspectTrainGuiParam;
		m_pSearchDataParam	= new CACFInspectSearchDataParam;
		m_pSearchGuiParam	= new CACFInspectSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgACFInspectTool->m_DlgQuickSearchTool.m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgACFInspectTool->ShowWindow(TRUE);
		m_pDlgACFInspectTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgACFInspectTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgACFInspectTool->UpdateDlgItem();
		m_pDlgACFInspectTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pSearchTool		= new CVirtualRectSearchTool;
		m_pTrainDataParam	= new CVirtualRectSearchTrainDataParam;
		m_pTrainGuiParam	= new CVirtualRectSearchTrainGuiParam;
		m_pSearchDataParam	= new CVirtualRectSearchSearchDataParam;
		m_pSearchGuiParam	= new CVirtualRectSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgVirtualRectSearchTool->ShowWindow(TRUE);
		m_pDlgVirtualRectSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgVirtualRectSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgVirtualRectSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pSearchTool		= new CPatternVirtualRectSearchTool;
		m_pTrainDataParam	= new CPatternVirtualRectSearchTrainDataParam;
		m_pTrainGuiParam	= new CPatternVirtualRectSearchTrainGuiParam;
		m_pSearchDataParam	= new CPatternVirtualRectSearchSearchDataParam;
		m_pSearchGuiParam	= new CPatternVirtualRectSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgPatternVirtualRectSearchTool->ShowWindow(TRUE);
		m_pDlgPatternVirtualRectSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgPatternVirtualRectSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgPatternVirtualRectSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL))
	{
		m_pSearchTool		= new CTwoQuickModelSearchTool;
		m_pTrainDataParam	= new CTwoQuickModelSearchTrainDataParam;
		m_pTrainGuiParam	= new CTwoQuickModelSearchTrainGuiParam;
		m_pSearchDataParam	= new CTwoQuickModelSearchSearchDataParam;
		m_pSearchGuiParam	= new CTwoQuickModelSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgTwoQuickModelSearchTool->m_GuiPatternDisplay2);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgTwoQuickModelSearchTool->ShowWindow(TRUE);
		m_pDlgTwoQuickModelSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgTwoQuickModelSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgTwoQuickModelSearchTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_DMCODE_TOOL))
	{
		m_pSearchTool		= new CIdReaderDmCodeTool;
		m_pTrainDataParam	= new CIdReaderDmCodeTrainDataParam;
		m_pTrainGuiParam	= new CIdReaderDmCodeTrainGuiParam;
		m_pSearchDataParam	= new CIdReaderDmCodeSearchDataParam;
		m_pSearchGuiParam	= new CIdReaderDmCodeSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgIdReaderDmCodeTool->ShowWindow(TRUE);
		m_pDlgIdReaderDmCodeTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgIdReaderDmCodeTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgIdReaderDmCodeTool->UpdateControlDisplay();
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_BARCODE_TOOL))
	{
		m_pSearchTool		= new CIdReaderBarCodeTool;
		m_pTrainDataParam	= new CIdReaderBarCodeTrainDataParam;
		m_pTrainGuiParam	= new CIdReaderBarCodeTrainGuiParam;
		m_pSearchDataParam	= new CIdReaderBarCodeSearchDataParam;
		m_pSearchGuiParam	= new CIdReaderBarCodeSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgIdReaderBarCodeTool->ShowWindow(TRUE);
		m_pDlgIdReaderBarCodeTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgIdReaderBarCodeTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgIdReaderBarCodeTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_POSLINEDISTANCE_TOOL))
	{
		m_pSearchTool		= new CQuickPosLineDistanceTool;
		m_pTrainDataParam	= new CQuickPosLineDistanceTrainDataParam;
		m_pTrainGuiParam	= new CQuickPosLineDistanceTrainGuiParam;
		m_pSearchDataParam	= new CQuickPosLineDistanceSearchDataParam;
		m_pSearchGuiParam	= new CQuickPosLineDistanceSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgQuickPosLineDistanceTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgQuickPosLineDistanceTool->ShowWindow(TRUE);
		m_pDlgQuickPosLineDistanceTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgQuickPosLineDistanceTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgQuickPosLineDistanceTool->UpdateControlDisplay();


	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_CORNER_TOOL))
    {
        m_pSearchTool		= new CCombinationCornerSearchTool;
        m_pTrainDataParam	= new CCombinationCornerSearchTrainDataParam;
        m_pTrainGuiParam	= new CCombinationCornerSearchTrainGuiParam;
        m_pSearchDataParam	= new CCombinationCornerSearchSearchDataParam;
        m_pSearchGuiParam	= new CCombinationCornerSearchSearchGuiParam;

        m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay2(&m_pDlgCombinedCornerSearchTool->m_DlgQuickSearchTool.m_GuiPatternDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgCombinedCornerSearchTool->m_DlgCornerSearchTool.m_GuiPatternDisplay);
        m_pSearchTool->ShowTrainGui(false);
        m_pSearchTool->ShowSearchResultGui(false);
        m_pDlgCombinedCornerSearchTool->ShowWindow(TRUE);
        m_pDlgCombinedCornerSearchTool->UpdateTrainData(bValidateDlgTrainData);
        m_pDlgCombinedCornerSearchTool->UpdateSearchData(bValidateDlgSearchData);
        m_pDlgCombinedCornerSearchTool->UpdateDlgItem();
        m_pDlgCombinedCornerSearchTool->UpdateControlDisplay();
    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMQuickPickSearchTool;
		m_pTrainDataParam	= new CMQuickPickSearchTrainDataParam;
		m_pTrainGuiParam	= new CMQuickPickSearchTrainGuiParam;
		m_pSearchDataParam	= new CMQuickPickSearchSearchDataParam;
		m_pSearchGuiParam	= new CMQuickPickSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->SetTrainResultDisplay(&m_pDlgMQuickPickSearchTool->m_GuiPatternDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMQuickPickSearchTool->ShowWindow(TRUE);
		m_pDlgMQuickPickSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgMQuickPickSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgMQuickPickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_PICK_SEARCH_TOOL))
	{
		m_pSearchTool		= new CMPatternPickSearchTool;
		m_pTrainDataParam	= new CMPatternPickSearchTrainDataParam;
		m_pTrainGuiParam	= new CMPatternPickSearchTrainGuiParam;
		m_pSearchDataParam	= new CMPatternPickSearchSearchDataParam;
		m_pSearchGuiParam	= new CMPatternPickSearchSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgMPatternPickSearchTool->ShowWindow(TRUE);
		m_pDlgMPatternPickSearchTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgMPatternPickSearchTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgMPatternPickSearchTool->UpdateControlDisplay();
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREAINSPECT_PICK_TOOL))
	{
		m_pSearchTool		= new CAreaInspectPickTool;
		m_pTrainDataParam	= new CAreaInspectPickTrainDataParam;
		m_pTrainGuiParam	= new CAreaInspectPickTrainGuiParam;
		m_pSearchDataParam	= new CAreaInspectPickSearchDataParam;
		m_pSearchGuiParam	= new CAreaInspectPickSearchGuiParam;

		m_pSearchTool->SetDisplay(&m_guiDisplay);	
		m_pSearchTool->ShowTrainGui(false);
		m_pSearchTool->ShowSearchResultGui(false);
		m_pDlgAreaInspectPickTool->ShowWindow(TRUE);
		m_pDlgAreaInspectPickTool->UpdateTrainData(bValidateDlgTrainData);
		m_pDlgAreaInspectPickTool->UpdateSearchData(bValidateDlgSearchData);
		m_pDlgAreaInspectPickTool->UpdateControlDisplay();
	}

}
void CDlgSearch::UpdateDlgSearchToolData(CString strSelText)
{
	if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL))
	{
		m_pDlgQuickSearchTool->CheckDlgDataValidation();
		m_pDlgQuickSearchTool->UpdateData(TRUE);
		m_pDlgQuickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL))
	{
		m_pDlgPatternSearchTool->CheckDlgDataValidation();
		m_pDlgPatternSearchTool->UpdateData(TRUE);
		m_pDlgPatternSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_SEARCH_TOOL))
	{
		m_pDlgLineSearchTool->CheckDlgDataValidation();
		m_pDlgLineSearchTool->UpdateData(TRUE);
		m_pDlgLineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL))
	{
		m_pDlgCornerSearchTool->CheckDlgDataValidation();
		m_pDlgCornerSearchTool->UpdateData(TRUE);
		m_pDlgCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgCircleSearchTool->CheckDlgDataValidation();
		m_pDlgCircleSearchTool->UpdateData(TRUE);
		m_pDlgCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CROSS_SEARCH_TOOL))
	{
		m_pDlgCrossSearchTool->CheckDlgDataValidation();
		m_pDlgCrossSearchTool->UpdateData(TRUE);
		m_pDlgCrossSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATCORNER_SEARCH_TOOL))
	{
		m_pDlgPatCornerSearchTool->CheckDlgDataValidation();
		m_pDlgPatCornerSearchTool->UpdateData(TRUE);
		m_pDlgPatCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PROCIRCLE_SEARCH_TOOL))
	{
		m_pDlgProCircleSearchTool->CheckDlgDataValidation();
		m_pDlgProCircleSearchTool->UpdateData(TRUE);
		m_pDlgProCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FIXEDPOS_SEARCH_TOOL))
	{
		m_pDlgFixedPosSearchTool->CheckDlgDataValidation();
		m_pDlgFixedPosSearchTool->UpdateData(TRUE);
		m_pDlgFixedPosSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DMCOLDE_CALIBRATE_TOOL))
	{
		m_pDlgDmCodeCalibrateTool->CheckDlgDataValidation();
		m_pDlgDmCodeCalibrateTool->UpdateData(TRUE);
		m_pDlgDmCodeCalibrateTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL))
	{
		m_pDlgPatternCornerSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCornerSearchTool->UpdateData(TRUE);
		m_pDlgPatternCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_QUICK_SEARCH_TOOL))
	{
		m_pDlgCombinedQuickSearchTool->CheckDlgDataValidation();
		m_pDlgCombinedQuickSearchTool->UpdateData(TRUE);
		m_pDlgCombinedQuickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL))
	{
		m_pDlgCombinedPatternSearchTool->CheckDlgDataValidation();
		m_pDlgCombinedPatternSearchTool->UpdateData(TRUE);
		m_pDlgCombinedPatternSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BASE_CORNER_SEARCH_TOOL))
	{
		m_pDlgBaseCornerSearchTool->CheckDlgDataValidation();
		m_pDlgBaseCornerSearchTool->UpdateData(TRUE);
		m_pDlgBaseCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_TOOL))
	{
		m_pDlgPatternSearchCheckTool->CheckDlgDataValidation();
		m_pDlgPatternSearchCheckTool->UpdateData(TRUE);
		m_pDlgPatternSearchCheckTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_TOOL))
	{
		m_pDlgQuickSearchCheckTool->CheckDlgDataValidation();
		m_pDlgQuickSearchCheckTool->UpdateData(TRUE);
		m_pDlgQuickSearchCheckTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_OBROUND_TOOL))
	{
		m_pDlgQuickSearchObroundTool->CheckDlgDataValidation();
		m_pDlgQuickSearchObroundTool->UpdateData(TRUE);
		m_pDlgQuickSearchObroundTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL))
	{
		m_pDlgPatternSearchObroundTool->CheckDlgDataValidation();
		m_pDlgPatternSearchObroundTool->UpdateData(TRUE);
		m_pDlgPatternSearchObroundTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_SEARCH_TOOL))
	{
		m_pDlgMPatternSearchTool->CheckDlgDataValidation();
		m_pDlgMPatternSearchTool->UpdateData(TRUE);
		m_pDlgMPatternSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_SEARCH_TOOL))
	{
		m_pDlgMQuickSearchTool->CheckDlgDataValidation();
		m_pDlgMQuickSearchTool->UpdateData(TRUE);
		m_pDlgMQuickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_CORNER_SEARCH_TOOL))
	{
		m_pDlgMQuickCornerSearchTool->CheckDlgDataValidation();
		m_pDlgMQuickCornerSearchTool->UpdateData(TRUE);
		m_pDlgMQuickCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL))
	{
		m_pDlgMQuickPickCornerSearchTool->CheckDlgDataValidation();
		m_pDlgMQuickPickCornerSearchTool->UpdateData(TRUE);
		m_pDlgMQuickPickCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_GENRECT_TOOL))
	{
		m_pDlgQuickSearchGenRectTool->CheckDlgDataValidation();
		m_pDlgQuickSearchGenRectTool->UpdateData(TRUE);
		m_pDlgQuickSearchGenRectTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL))
	{
		m_pDlgPatternSearchGenRectTool->CheckDlgDataValidation();
		m_pDlgPatternSearchGenRectTool->UpdateData(TRUE);
		m_pDlgPatternSearchGenRectTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_SEARCH_TOOL))
	{
		m_pDlgPatternLineSearchTool->CheckDlgDataValidation();
		m_pDlgPatternLineSearchTool->UpdateData(TRUE);
		m_pDlgPatternLineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_2PATTERN_SEARCH_TOOL))
	{
		m_pDlg2PatternSearchTool->CheckDlgDataValidation();
		m_pDlg2PatternSearchTool->UpdateData(TRUE);
		m_pDlg2PatternSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgQuickLineCircleSearchTool->CheckDlgDataValidation();
		m_pDlgQuickLineCircleSearchTool->UpdateData(TRUE);
		m_pDlgQuickLineCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternLineCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternLineCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternLineCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL))
	{
		m_pDlgQuickSearchCheckLineTool->CheckDlgDataValidation();
		m_pDlgQuickSearchCheckLineTool->UpdateData(TRUE);
		m_pDlgQuickSearchCheckLineTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL))
	{
		m_pDlgPatternSearchCheckLineTool->CheckDlgDataValidation();
		m_pDlgPatternSearchCheckLineTool->UpdateData(TRUE);
		m_pDlgPatternSearchCheckLineTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_BLOB_SEARCH_TOOL))
	{
		m_pDlgPatternBlobSearchTool->CheckDlgDataValidation();
		m_pDlgPatternBlobSearchTool->UpdateData(TRUE);
		m_pDlgPatternBlobSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_CHECK_TOOL))
	{
		m_pDlgCornerSearchCheckTool->CheckDlgDataValidation();
		m_pDlgCornerSearchCheckTool->UpdateData(TRUE);
		m_pDlgCornerSearchCheckTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL))
	{
		m_pDlgRectSearchTool->CheckDlgDataValidation();
		m_pDlgRectSearchTool->UpdateData(TRUE);
		m_pDlgRectSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_RECT_SEARCH_TOOL))
	{
		m_pDlgPatternRectSearchTool->CheckDlgDataValidation();
		m_pDlgPatternRectSearchTool->UpdateData(TRUE);
		m_pDlgPatternRectSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECT_TOOL))
	{
		m_pDlgAreaInspectTool->CheckDlgDataValidation();
		m_pDlgAreaInspectTool->UpdateData(TRUE);
		m_pDlgAreaInspectTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECTEX_TOOL))
	{
		m_pDlgAreaInspectExTool->CheckDlgDataValidation();
		m_pDlgAreaInspectExTool->UpdateData(TRUE);
		m_pDlgAreaInspectExTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_CHORD_SEARCH_TOOL))
	{
		m_pDlgChordSearchTool->CheckDlgDataValidation();
		m_pDlgChordSearchTool->UpdateData(TRUE);
		m_pDlgChordSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL))
	{
		m_pDlgTwoPatModelSearchTool->CheckDlgDataValidation();
		m_pDlgTwoPatModelSearchTool->UpdateData(TRUE);
		m_pDlgTwoPatModelSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNCORNER_SEARCH_TOOL))
	{
		m_pDlgMPatternCornerSearchTool->CheckDlgDataValidation();
		m_pDlgMPatternCornerSearchTool->UpdateData(TRUE);
		m_pDlgMPatternCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL))
	{
		m_pDlgMPatternPickCornerSearchTool->CheckDlgDataValidation();
		m_pDlgMPatternPickCornerSearchTool->UpdateData(TRUE);
		m_pDlgMPatternPickCornerSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCornerCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternCornerCircleSearchTool->UpdateSearchData(true);
	}
	else if(strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL))
	{
		m_pDlgQuickCornerCircleSearchTool->CheckDlgDataValidation();
		m_pDlgQuickCornerCircleSearchTool->UpdateData(TRUE);
		m_pDlgQuickCornerCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL))
	{
		m_pDlgPatternPosLineDistanceTool->CheckDlgDataValidation();
		m_pDlgPatternPosLineDistanceTool->UpdateData(TRUE);
		m_pDlgPatternPosLineDistanceTool->UpdateSearchData(true);
	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL))
    {
        m_pDlgCornerSearchCheckInspectTool->CheckDlgDataValidation();
        m_pDlgCornerSearchCheckInspectTool->UpdateData(TRUE);
        m_pDlgCornerSearchCheckInspectTool->UpdateSearchData(true);
    }
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL))
	{
		m_pDlgPatternDoubleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternDoubleSearchTool->UpdateData(TRUE);
		m_pDlgPatternDoubleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CHORD_SEARCH_TOOL))
	{
		m_pDlgPatternChordSearchTool->CheckDlgDataValidation();
		m_pDlgPatternChordSearchTool->UpdateData(TRUE);
		m_pDlgPatternChordSearchTool->UpdateSearchData(true);
	}
    else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL))
    {
        m_pDlgQuickMulCircleSearchTool->CheckDlgDataValidation();
        m_pDlgQuickMulCircleSearchTool->UpdateData(TRUE);
        m_pDlgQuickMulCircleSearchTool->UpdateSearchData(true);
    }

	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pDlgPatternCircle4LineSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCircle4LineSearchTool->UpdateData(TRUE);
		m_pDlgPatternCircle4LineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL))
	{
		m_pDlgQuickCircle4LineSearchTool->CheckDlgDataValidation();
		m_pDlgQuickCircle4LineSearchTool->UpdateData(TRUE);
		m_pDlgQuickCircle4LineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL))
	{
		m_pDlgSpecialPickSearchTool->CheckDlgDataValidation();
		m_pDlgSpecialPickSearchTool->UpdateData(TRUE);
		m_pDlgSpecialPickSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerLineSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCornerLineSearchTool->UpdateData(TRUE);
		m_pDlgPatternCornerLineSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternMulCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternMulCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternMulCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL))
	{
		m_pDlgPatternCornerMulCircleSearchTool->CheckDlgDataValidation();
		m_pDlgPatternCornerMulCircleSearchTool->UpdateData(TRUE);
		m_pDlgPatternCornerMulCircleSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL))
	{
		m_pDlgPatternCornerMulObroundTool->CheckDlgDataValidation();
		m_pDlgPatternCornerMulObroundTool->UpdateData(TRUE);
		m_pDlgPatternCornerMulObroundTool->UpdateSearchData(true);
	
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL))
	{
		m_pDlgQuickCornerMulObroundTool->CheckDlgDataValidation();
		m_pDlgQuickCornerMulObroundTool->UpdateData(TRUE);
		m_pDlgQuickCornerMulObroundTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL))
	{
		m_pDlgCircleCornerSearchCheckInspectTool->CheckDlgDataValidation();
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateData(TRUE);
		m_pDlgCircleCornerSearchCheckInspectTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL))
	{
		m_pDlgACFInspectTool->CheckDlgDataValidation();
		m_pDlgACFInspectTool->UpdateData(TRUE);
		m_pDlgACFInspectTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pDlgVirtualRectSearchTool->CheckDlgDataValidation();
		m_pDlgVirtualRectSearchTool->UpdateData(TRUE);
		m_pDlgVirtualRectSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL))
	{
		m_pDlgPatternVirtualRectSearchTool->CheckDlgDataValidation();
		m_pDlgPatternVirtualRectSearchTool->UpdateData(TRUE);
		m_pDlgPatternVirtualRectSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL))
	{
		m_pDlgTwoQuickModelSearchTool->CheckDlgDataValidation();
		m_pDlgTwoQuickModelSearchTool->UpdateData(TRUE);
		m_pDlgTwoQuickModelSearchTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_DMCODE_TOOL))
	{
		m_pDlgIdReaderDmCodeTool->CheckDlgDataValidation();
		m_pDlgIdReaderDmCodeTool->UpdateData(TRUE);
		m_pDlgIdReaderDmCodeTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_BARCODE_TOOL))
	{
		m_pDlgIdReaderBarCodeTool->CheckDlgDataValidation();
		m_pDlgIdReaderBarCodeTool->UpdateData(TRUE);
		m_pDlgIdReaderBarCodeTool->UpdateSearchData(true);
	}
	else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_POSLINEDISTANCE_TOOL))
	{
		m_pDlgQuickPosLineDistanceTool->CheckDlgDataValidation();
		m_pDlgQuickPosLineDistanceTool->UpdateData(TRUE);
		m_pDlgQuickPosLineDistanceTool->UpdateSearchData(true);
	}
	 else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_CORNER_TOOL))
    {
        m_pDlgCombinedCornerSearchTool->CheckDlgDataValidation();
        m_pDlgCombinedCornerSearchTool->UpdateData(TRUE);
        m_pDlgCombinedCornerSearchTool->UpdateSearchData(true);
    }
	 else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_SEARCH_TOOL))
	 {
		 m_pDlgMQuickPickSearchTool->CheckDlgDataValidation();
		 m_pDlgMQuickPickSearchTool->UpdateData(TRUE);
		 m_pDlgMQuickPickSearchTool->UpdateSearchData(true);
	 }
	 else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_PICK_SEARCH_TOOL))
	 {
		 m_pDlgMPatternPickSearchTool->CheckDlgDataValidation();
		 m_pDlgMPatternPickSearchTool->UpdateData(TRUE);
		 m_pDlgMPatternPickSearchTool->UpdateSearchData(true);
	 }
	 else if (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREAINSPECT_PICK_TOOL))
	 {
		 m_pDlgAreaInspectPickTool->CheckDlgDataValidation();
		 m_pDlgAreaInspectPickTool->UpdateData(TRUE);
		 m_pDlgAreaInspectPickTool->UpdateSearchData(true);
	 }
}