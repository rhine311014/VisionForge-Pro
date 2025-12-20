// SysPlatformInfo.h
#pragma once

#include "vsAlignToolDef.h"
#include "vsBaseAlignTool.h"
#include "CommProtocolProcess.h"

const int SEARCHTOOLSIZE = 58;   //定位工具个数，vector空间大小

// 系统配置信息（管理员配置）
typedef struct tagSysPlatformInfo
{
public:
	/**平台类型相关 **********************************************************************/
	/*                                                                     */
	/************************************************************************/
	CString				m_strPlatformName;		// 平台名称
	int					m_nPlatformIndex;		// 平台序号
	PlatformType		m_ePlatformType;		// 平台类型
	PlatformType		m_eMidPlatformType;		// 中间平台类型


	/**平台通信相关 后续可能移植到通信界面**********************************************************************/
	/*                                                                */
	/************************************************************************/
	

	BOOL				m_bShareCommEnable;		// 共享通信是否有效，共享通信目前只支持寄存器通信，且共享同一接口的不同平台，不能同时处理通信指令，只能一个处理完成后另一再处理。
	int					m_nShareCommIndex;		// 共享通信接口的序号、编号或通道号(>=0)
	BOOL				m_bShareCommPlatformParallel;	// 共享同一通信接口的各个平台，是否可以同时通信（近似同时）：FALSE，否，即需要等待一个工位处理完整条指令后，下一平台才可能接收处理指令；TRUE，是，各平台处理指令处理近似是并行的，但真正的数据通信过程仍然串行的；
	// 主动提供共享工位和被动共享到其他工位，都需要配置m_bShareCommEnable=TRUE，同一组内不需要手动指定区分主动和被动
	// m_nShareCommIndex只是共享标识编号，用于不同组共享之间的区分，不需要等于提供共享工位的工位索引，同一组内所有工位的标识编号需相同
	// 同一组共享所有工位的m_bShareCommPlatformParallel必须相同，要么都是并行TRUE，要么都是串行FALSE
	// 在大于2个工位的共享内，如果m_bShareCommPlatformParallel=TRUE并行，当其中一个共享释放了通信连接时，其他所有工位都进行争抢，没有轮候顺序的区别，这是因为并行的原理基于windows自带的临界区功能
	BOOL				m_bEnableKeyenceCode;			// 启用基恩士指令格式码；
	BOOL				m_bFixCode;						// 固定指令码

	BOOL				m_bEnableMulRegTrigger;			// 启用多地址触发：PLC在不同段地址触发相同信号；来区分不同的流程功能；例如D50发送1 触发目标拍照，D150发送1 触发视觉同工位内 对象拍照并对位
	
	BOOL				m_bShowBendPathCalc;			// 启用折弯路径计算 启用该功能后，网口通信界面中显示折弯路径计算功能按钮，并调用MotionDll执行折弯路径计算并可手动发送折弯数据给PLC;


	/**相机位置相关**********************************************************************/
	/*                                                                      */
	/************************************************************************/
	int					m_nPositionNum;				// 相机移动拍摄的位置个数（当相机个数大于1时，本参数被忽略）
	int					m_nCamNum;					// 相机个数
	//int					m_nObjectPositionNum;		// 对象位置个数 用于虚拟
	//int					m_nTargetPositionNum;		// 目标位置个数	用于虚拟
	CamPlatformType		m_eCamPlatformType;			// 相机平台类型
	BOOL				m_bCameraHardTriggerMode;	// 相机硬触发模式
	BOOL                m_bShareCamSystem;          //是否是相机共享系统

	/**对位相关**********************************************************************/
	/*                                                                      */
	/************************************************************************/

	AlignerTargetType		m_eAlignerTargetType;		// 目标Mark类型
	AlignerTargetMoveType	m_eAlignerTargetMoveType;	// 目标Mark移动类型
	BOOL					m_bBenchMarkAmend;			// 是否启用基准目标修正（当目标Mark类型为实时Mark时，本参数被忽略）
	AlignerObjectType		m_eAlignerObjectType;		// 对象Mark类型
	AlignerObjectMoveType	m_eAlignerObjectMoveType;	// 对象Mark移动类型

	BOOL					m_bTargetObjectCamSeparate; //目标相机和对象相机是各自独立的
	BOOL					m_bTargetCalibUseBoard;		//目标相机和对象相机是各自独立时，目标标定是否使用标定板
	BOOL					m_bTarObjCamSepNoAxisCalib;	//目标相机和对象相机是各自独立时，标定目标位置时不读取平台轴位置
	BOOL					m_bTarObjCamSepAutoCalibTar;// 目标对象分离，目标位置自动标定不关联，需配合关联时读轴位置使用（m_bTarObjCamSepNoAxisCalib = FALSE）
//	BOOL					m_bTarCamWithDiffObjCam;	// 一组目标相机对应两组不同的对象相机
	// 拾取应用
	BOOL					m_bPlatformPick;			//是否为拾取系统
	BOOL					m_bTargetPick;				//是否为目标拾取(返回偏差量）（不做示教过程）
	PickCamPlatformType		m_ePickCamPlatformType;		//拾取系统中相机的安装方式（0,固定安装；1，共享平台的XY；）

	BOOL					m_bEnableTargetBench;		// 启用目标基准对位(标定完成以后将标定位置作为对位目标基准)
	BOOL					m_bEnableObjectCalibBench;	//对象标定时是否选择一个位置为基准，其他位置通过图像平台坐标通过轴移动量计算
	int						m_nObjectCalibBenchIndex;	//基准位置序号
	BOOL					m_bEnableMultiCalibExtension;// 启用单产品多标定扩展
	int						m_nMultiCalibExtensionMaxNum;// 扩展数量
	BOOL					m_bUseAlignFAHLex;			// 背光贴片机 高低平面对位的时候是否执行一次对位函数FAHLex；（ture，执行函数；默认是false，不执行）

	/**产品相关**********************************************************************/
	/*                                                                       */
	/************************************************************************/

	CString					m_strPlatformDetailSavePath;				// 平台详细信息保存路径，为相对于EXE路径的相对路径
	/* 单产品载入功能使用说明：

		情况1：只加载单个品种：只需要配置m_bLoadSingleProductSearchToolEnable = TRUE; 
		m_bLoadSingleProductSearchToolEnableFaChd无需配置;
		此时无论是否启用大小品种，均只载入单个品种（当前品种）；

		情况2：加载单个大品种下全部小品种（前提是启用大小品种功能）； 首先需要配置m_bProductFaChd=TRUE；
		然后配置：
		m_bLoadSingleProductSearchToolEnableFaChd = TRUE;
		m_bLoadSingleProductSearchToolEnable = FALSE; 
	 
		情况3：实现加载全部产品：（内存会随品种数目正增长，可能导致内存耗尽出现崩溃，不建议使用该配置）
		不启用大小品种时：只需要配置m_bLoadSingleProductSearchToolEnable = FALSE; 
		启用大小品种时（m_bProductFaChd=TRUE；）：需要配置 m_bLoadSingleProductSearchToolEnableFaChd = FALSE;时

	*/

	BOOL					m_bLoadSingleProductSearchToolEnable;		// 只加载单个产品的搜索工具（加载多了占用内存会变大）	
	BOOL					m_bProductFaChd;							// 是否启用大小品种
	BOOL					m_bLoadSingleProductSearchToolEnableFaChd;	// 只加载单个大产品的搜索工具，与m_bProductFaChd配合使用，优先级低于m_bLoadSingleProductSearchToolEnable
	// 1）打开软件时，加载当前大品种的全部小品种
	// 2）指令切换大小品种时，加载新大品种的全部小品种，释放其他全部大品种的全部小品种
	// 3）界面切换大品种时，加载新大品种的全部小品种，释放其他全部大品种的全部小品种；界面切换小品种时，不做加载和释放；界面复制品种、新建品种、切换平台、不保存退出时，释放其他全部大品种的全部小品种

	/**显示相关*** *******************************************************************/
	/*                                                                     */
	/************************************************************************/
	// int					m_nPositionShowMode;			// 相机移动拍摄时的显示模式（当相机个数大于1时，本参数被忽略）（0,独立显示；1，共享显示；）	//三钻折弯项目
	int					m_nShowMode;                    // 0: 按照位置数显示窗口, 1: 按照相机数显示窗口
	int					m_nDisplayArrayMode[MAXPOSNUM];	// View中8个显示窗口的排列方式	//hk20180130
		//初始值：	0,1,2,3,4,5,6,7（4VS4）
		//	1  2   5  6
		//  3  4   7  8
		//初始值：	0,1,4,5,2,3,6,7（4VS4）
		//	1  2   3  4
		//  5  6   7  8
		//模式1：	1,0,6,2,4,3,7,5（3VS3，拍产品左上，右上，右下三个角）
		//	2  1   5  4
		//  7  3   8  6
		//模式2：	0,1,2,6,3,4,5,7（3VS3，拍产品左上，右上，左下三个角）
		//	1  2   4  5
		//  3  7   6  8
		//模式3：	6,2,1,0,7,5,4,3（3VS3，拍产品左下，右上，右下三个角）
		//	7  3   8  6
		//  2  1   5  4
		//模式4：	1,6,0,2,4,7,3,5（3VS3，拍产品左上，左下，右下三个角）
		//	2  7   5  8
		//  1  3   4  6
	BOOL                m_bTargetInDifferentComputer;	//对象和目标不在同一台控制器时，该软件为目标端
	BOOL                m_bObjectInDifferentComputer;	//对象和目标不在同一台控制器时，该软件为对象端

	/************************************************************************/
	/* 定位工具相关                                                                     */
	/************************************************************************/
	vector<map<pair<int,int>,BOOL>> m_vmapSysSearchToolInfo;     //

	/************************************************************************/
	/* 软件注册                                                                     */
	/************************************************************************/
	BOOL                m_bRegisterSuccess;                // 软件是否注册

	BOOL				m_bPlatformGlue;

	BOOL				m_bPlatformShareShow;				// 是否是共享工位
	int					m_nShareShowPlatformIndex;			// 共享显示工位序号

	tagSysPlatformInfo()
	{
		// 平台类型相关
		m_strPlatformName		= _T("");
		m_nPlatformIndex		= 0;
		m_ePlatformType			= ePlatformXYD;
		m_eMidPlatformType		= ePlatformXYD;
		
		// 通信相关
		//m_eCommType				= eSerial;
		m_bShareCommEnable		= FALSE;		// 共享通信是否有效
		m_nShareCommIndex		= -1;			// 共享通信接口的序号、编号或通道号(>=0)
		m_bShareCommPlatformParallel = FALSE;
		m_bEnableKeyenceCode  = FALSE;
		m_bFixCode = TRUE;
		m_bShowBendPathCalc = FALSE;
		m_bEnableMulRegTrigger = FALSE;


		// 相机位置相关
		m_nCamNum				= 1;
		m_nPositionNum			= 2;
		m_eCamPlatformType		= eCamPlatformSepFix;
		m_bCameraHardTriggerMode = FALSE;
		m_bShareCamSystem=FALSE;                     //是否为相机共享系统

		// 对位相关
		m_eAlignerTargetType	= eTargetBench;
		m_eAlignerTargetMoveType = eTargetNoMove;
		m_bBenchMarkAmend		= FALSE;
		m_eAlignerObjectType	= eObjectOnline;
		m_eAlignerObjectMoveType = eObjectMoveByPlatform;

		m_bTargetObjectCamSeparate = FALSE;
		m_bTargetCalibUseBoard = FALSE;
		m_bTarObjCamSepNoAxisCalib = FALSE;
//		m_bTarCamWithDiffObjCam = FALSE;
		m_bTarObjCamSepAutoCalibTar = FALSE;

		m_bPlatformPick			= FALSE;						//是否为拾取系统
		m_bTargetPick			= FALSE;						//是否为目标拾取(返回偏差量）（不做示教过程）
		m_ePickCamPlatformType  = ePickCamPlatformSepFix;		//拾取系统中相机的安装方式


		m_bEnableTargetBench	= FALSE;
		m_bEnableObjectCalibBench  = FALSE;    //对象标定时是否选择一个位置为基准，其他位置通过图像平台坐标通过轴移动量计算
		m_nObjectCalibBenchIndex   = 0;        //基准位置序号
		m_bEnableMultiCalibExtension = FALSE;	// 启用单产品多标定扩展
		m_nMultiCalibExtensionMaxNum	= 0;		// 扩展数量
		m_bUseAlignFAHLex = FALSE;

		//产品相关
		m_strPlatformDetailSavePath = _T("");
		m_bLoadSingleProductSearchToolEnable = TRUE;
		m_bProductFaChd = FALSE;
		m_bLoadSingleProductSearchToolEnableFaChd = FALSE;

		// 显示相关
		//m_nPositionShowMode		= 0;
		m_nShowMode = 0;						// 0 按照位置显示, 1按照相机数显示
		for (int i=0; i<MAXPOSNUM; i++)	//hk20180130
		{
			m_nDisplayArrayMode[i] = i;
		}
		m_bTargetInDifferentComputer = FALSE;
		m_bObjectInDifferentComputer = FALSE;

		// 软件注册
		m_bRegisterSuccess = FALSE;

		m_bPlatformShareShow = FALSE;				// 是否是共享工位
		m_nShareShowPlatformIndex = 0;			// 共享显示工位序号

		m_bPlatformGlue = FALSE;

		m_vmapSysSearchToolInfo.reserve(SEARCHTOOLSIZE);

		map<pair<int,int>,BOOL> mapTmp;
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_TOOL,eQuickSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_TOOL,ePatternSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_LINE_SEARCH_TOOL,eLineSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_CORNER_SEARCH_TOOL,eCornerSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_CIRCLE_SEARCH_TOOL,eCircleSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_CROSS_SEARCH_TOOL,eCrossSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATCORNER_SEARCH_TOOL,ePatCornerSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PROCIRCLE_SEARCH_TOOL,eProCircleSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_FIXEDPOS_SEARCH_TOOL,eFixedPosSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERNCORNER_SEARCH_TOOL,ePatternCornerSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_COMBINED_QUICK_SEARCH_TOOL,eCombinedQuickSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL,eCombinedPatternSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_BASE_CORNER_SEARCH_TOOL,eBaseCornerSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_CHECK_TOOL,ePatternSearchCheckTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_CHECK_TOOL,eQuickSearchCheckTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_OBROUND_TOOL,eQuickSearchObroundTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL,ePatternSearchObroundTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_MPATTERN_SEARCH_TOOL,eMPatternSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_MQUICK_SEARCH_TOOL,eMQuickSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_MQUICK_CORNER_SEARCH_TOOL,eMQuickCornerSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL,eMQuickPickCornerSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_GENRECT_TOOL,eQuickSearchGenRectTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL,ePatternSearchGenRectTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_LINE_SEARCH_TOOL,ePatternLineSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_2PATTERN_SEARCH_TOOL,e2PatternSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL,eQuickLineCircleSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL,ePatternLineCircleSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL,ePatternCircleSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL,eQuickSearchCheckLineTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL,eQuickCornerCircleSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL,ePatternSearchCheckLineTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_BLOB_SEARCH_TOOL,ePatternBlobSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_CORNER_SEARCH_CHECK_TOOL,eCornerSearchCheckTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_RECT_SEARCH_TOOL,eRectSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_RECT_SEARCH_TOOL,ePatternRectSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_AREA_INSPECT_TOOL,eAreaInspectTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_AREA_INSPECTEX_TOOL,eAreaInspectToolEx),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_CHORD_SEARCH_TOOL,eChordSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL,eTwoPatModelSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_MPATTERNCORNER_SEARCH_TOOL,eMPatternCornerSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL,eMPatternPickCornerSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL,ePatternCornerCircleSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
        mapTmp.clear();
        mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL,ePatternPosLineDistanceTool),TRUE));
        m_vmapSysSearchToolInfo.push_back(mapTmp);
        mapTmp.clear();
        mapTmp.insert(make_pair(make_pair(IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL,eCornerSearchCheckInspectTool),TRUE));
        m_vmapSysSearchToolInfo.push_back(mapTmp);
        mapTmp.clear();
        mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL,ePatternDouleSearchTool),TRUE));
        m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
        mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_CHORD_SEARCH_TOOL,ePatternChordSearchTool),TRUE));
        m_vmapSysSearchToolInfo.push_back(mapTmp);
        mapTmp.clear();
        mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL,eQuickMulCircleSearchTool),TRUE));
        m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL,ePatternCircle4LineSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL,eQuickCircle4LineSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_DMCOLDE_CALIBRATE_TOOL,eDmCodeCalibrateTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_SPECIAL_PICK_SEARCH_TOOL,eSpecialPickSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL,ePatternCornerLineSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL,ePatternMulCircleSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL, ePatternCornerMulCircleSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL,ePatternCornerMulObroundTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL,eQuickCornerMulObroundTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL,eCircleCornerSearchCheckInspectTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_ACFINSPECT_TOOL,eACFInspectTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL,ePatternVirtualRectSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL,eVirtualRectSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();

		mapTmp.insert(make_pair(make_pair(IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL,eTwoQuickModelSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		mapTmp.insert(make_pair(make_pair(IDS_CMB_IDREADER_DMCODE_TOOL,eIdReaderDmCodeTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();

		mapTmp.insert(make_pair(make_pair(IDS_CMB_IDREADER_BARCODE_TOOL,eIdReaderBarCodeTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		
		mapTmp.insert(make_pair(make_pair(IDS_CMB_QUICK_POSLINEDISTANCE_TOOL,eQuickPosLineDistanceTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
		
		mapTmp.insert(make_pair(make_pair(IDS_CMB_COMBINED_CORNER_TOOL,eCombinationCornerSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();

		mapTmp.insert(make_pair(make_pair(IDS_CMB_MQUICK_PICK_SEARCH_TOOL,eMQuickPickSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();	

		mapTmp.insert(make_pair(make_pair(IDS_CMB_MPATTERN_PICK_SEARCH_TOOL,eMPatternPickSearchTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();

		mapTmp.insert(make_pair(make_pair(IDS_CMB_AREAINSPECT_PICK_TOOL,eAreaInspectPickTool),TRUE));
		m_vmapSysSearchToolInfo.push_back(mapTmp);
		mapTmp.clear();
}

} SysPlatformInfo;


//定位工具配置信息
typedef struct tagDlgSearchToolInfo
{
public:
	map<pair<int,CString>,BOOL> m_mapDlgSearchToolInfo;

	tagDlgSearchToolInfo()
	{
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_TOOL,_T("QuickSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_TOOL,_T("PatternSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_LINE_SEARCH_TOOL,_T("LineSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_CORNER_SEARCH_TOOL,_T("CornerSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_CIRCLE_SEARCH_TOOL,_T("CircleSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_CROSS_SEARCH_TOOL,_T("CrossSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATCORNER_SEARCH_TOOL,_T("PatCornerSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PROCIRCLE_SEARCH_TOOL,_T("ProCircleSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_FIXEDPOS_SEARCH_TOOL,_T("FixedPosSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERNCORNER_SEARCH_TOOL,_T("PatternCornerSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_COMBINED_QUICK_SEARCH_TOOL,_T("CombinedQuickSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL,_T("CombinedPatternSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_BASE_CORNER_SEARCH_TOOL,_T("BaseCornerSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_CHECK_TOOL,_T("PatternSearchCheckToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_CHECK_TOOL,_T("QuickSearchCheckToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_OBROUND_TOOL,_T("QuickSearchObroundToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL,_T("PatternSearchObroundToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_MPATTERN_SEARCH_TOOL,_T("MPatternSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_MQUICK_SEARCH_TOOL,_T("MQuickSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_MQUICK_CORNER_SEARCH_TOOL,_T("MQuickCornerSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL,_T("MQuickPickCornerSearchToolEnabled")),TRUE));
		//无 IDS_CMB_QUICK_SEARCH_GENRECT_TOOL IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_GENRECT_TOOL,_T("QuickSearchGenRectToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL,_T("PatternSearchGenRectToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_LINE_SEARCH_TOOL,_T("PatternLineSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_2PATTERN_SEARCH_TOOL,_T("TwoPatternSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL,_T("QuickLineCircleSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL,_T("PatternLineCircleSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL,_T("PatternCircleSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL,_T("QuickSearchCheckLineToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL,_T("PatternSearchCheckLineToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_BLOB_SEARCH_TOOL,_T("PatternBlobSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_CORNER_SEARCH_CHECK_TOOL,_T("CornerSearchCheckToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_RECT_SEARCH_TOOL,_T("RectSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_RECT_SEARCH_TOOL,_T("PatternRectSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_AREA_INSPECT_TOOL,_T("AreaInspectToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_AREA_INSPECTEX_TOOL,_T("AreaInspectExToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_CHORD_SEARCH_TOOL,_T("ChordSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL,_T("TwoPatModelSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_MPATTERNCORNER_SEARCH_TOOL,_T("MPatternCornerSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL,_T("MPatternPickCornerSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL,_T("PatternCornerCircleSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL,_T("QuickCornerCircleSearchToolEnabled")),FALSE));
        m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL,_T("PatternPosLineDistanceSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL,_T("CornerSearchCheckInspectToolEnabled")),FALSE));
        m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL,_T("PatternDoubleSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_CHORD_SEARCH_TOOL,_T("PatternChordSearchToolEnabled")),FALSE));
        m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL,_T("QuickMulCircleSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_DMCOLDE_CALIBRATE_TOOL,_T("DmCodeCalibrateToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL,_T("PatternCircle4LineToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL,_T("QuickCircle4LineToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_SPECIAL_PICK_SEARCH_TOOL,_T("SpecialPickSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL,_T("PatternCornerLineSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL,_T("PatternMulCircleSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL,_T("PatternCornerMulCircleSearchToolEnabled")),TRUE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL,_T("PatternCornerMulObroundToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL,_T("QuickCornerMulObroundToolEnabled")),FALSE));	
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL,_T("CircleCornerSearchCheckInspectToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_ACFINSPECT_TOOL,_T("ACFInspectToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL,_T("VirtualRectSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL,_T("PatternVirtualRectSearchToolEnabled")),FALSE));

		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL,_T("TwoQuickModelSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_IDREADER_DMCODE_TOOL,_T("IdReaderDmCodeToolEnabled")),FALSE));
		
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_IDREADER_BARCODE_TOOL,_T("IdReaderBarCodeToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_QUICK_POSLINEDISTANCE_TOOL,_T("QuickPosLineDistanceEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_COMBINED_CORNER_TOOL,_T("CombinedCornerSearchEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_MQUICK_PICK_SEARCH_TOOL,_T("MQuickPickSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_MPATTERN_PICK_SEARCH_TOOL,_T("MPatternPickSearchToolEnabled")),FALSE));
		m_mapDlgSearchToolInfo.insert(make_pair(make_pair(IDS_CMB_AREAINSPECT_PICK_TOOL,_T("AreaInspectPickTool")),FALSE));


	}


} DlgSearchToolInfo;


//2vs2
typedef struct  tagAligner2Vs2TypeInfo
{
public:
	Aligner2Vs2Type m_alignerType;
	int m_nAlignerTypeID;
	bool m_bOpenAligner;
	CString m_strAlignerName;
	tagAligner2Vs2TypeInfo()
	{
		m_alignerType = eAlignerPos12_Auto;
		m_nAlignerTypeID = IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_AUTO;
		m_bOpenAligner = TRUE;
		m_strAlignerName = _T("eAlignerPos12_Auto");
	}
	tagAligner2Vs2TypeInfo(Aligner2Vs2Type alignerType,int nAlignerTypeID,bool bOpenAligner,CString strAlignerName)
	{
		m_alignerType = alignerType;
		m_nAlignerTypeID = nAlignerTypeID;
		m_bOpenAligner = bOpenAligner;
		m_strAlignerName = strAlignerName;
	}

}Aligner2Vs2TypeInfo;

//3Vs3
typedef struct  tagAligner3Vs3TypeInfo
{
public:
	Aligner3Vs3Type m_alignerType;
	int m_nAlignerTypeID;
	bool m_bOpenAligner;
	CString m_strAlignerName;
	tagAligner3Vs3TypeInfo()
	{
		m_alignerType = eAlignerPos123_Auto;
		m_nAlignerTypeID = IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO;
		m_bOpenAligner = TRUE;
		m_strAlignerName = _T("eAlignerPos123_Auto");
	}
	tagAligner3Vs3TypeInfo(Aligner3Vs3Type alignerType,int nAlignerTypeID,bool bOpenAligner,CString strAlignerName)
	{
		m_alignerType = alignerType;
		m_nAlignerTypeID = nAlignerTypeID;
		m_bOpenAligner = bOpenAligner;
		m_strAlignerName = strAlignerName;
	}

}Aligner3Vs3TypeInfo;
//4Vs4
typedef struct  tagAligner4Vs4TypeInfo
{
public:
	Aligner4Vs4Type m_alignerType;
	int m_nAlignerTypeID;
	bool m_bOpenAligner;
	CString m_strAlignerName;
	tagAligner4Vs4TypeInfo()
	{
		m_alignerType = eAlignerPos1234_Auto;
		m_nAlignerTypeID = IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_AUTO;
		m_bOpenAligner = TRUE;
		m_strAlignerName = _T("eAlignerPos1234_Auto");
	}
	tagAligner4Vs4TypeInfo(Aligner4Vs4Type alignerType,int nAlignerTypeID,bool bOpenAligner,CString strAlignerName)
	{
		m_alignerType = alignerType;
		m_nAlignerTypeID = nAlignerTypeID;
		m_bOpenAligner = bOpenAligner;
		m_strAlignerName = strAlignerName;
	}

}Aligner4Vs4TypeInfo;
//对位方式信息
typedef struct tagAlignerTypeInfo
{
public:
	vector<Aligner2Vs2TypeInfo> m_vAligner2Vs2TypeInfo;
	vector<Aligner3Vs3TypeInfo> m_vAligner3Vs3TypeInfo;
	vector<Aligner4Vs4TypeInfo> m_vAligner4Vs4TypeInfo;

	tagAlignerTypeInfo()
	{
		//2Vs2
		Aligner2Vs2TypeInfo aligner2vs2Type(eAlignerPos12_Auto,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_AUTO,TRUE,_T("eAlignerPos12_Auto"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//0
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_12C1,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C1,TRUE,_T("eAlignerPos12_12C1"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//1
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_12C2,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C2,TRUE,_T("eAlignerPos12_12C2"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//2
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_12C1L2,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C1L2,TRUE,_T("eAlignerPos12_12C1L2"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//3
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_12C2L1,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C2L1,TRUE,_T("eAlignerPos12_12C2L1"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//4
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_12Dot1L2,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12Dot1L2,TRUE,_T("eAlignerPos12_12Dot1L2"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//5
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_12Dot2L1,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12Dot2L1,TRUE,_T("eAlignerPos12_12Dot2L1"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//6
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_L12L1,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_L12L1,TRUE,_T("eAlignerPos12_L12L1"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//7
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_L12L2,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_L12L2,TRUE,_T("eAlignerPos12_L12L2"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//8
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_L12,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_L12,TRUE,_T("eAlignerPos12_L12"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//9
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_CTo3,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_CTO3,TRUE,_T("eAlignerPos12_CTo3"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//10
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_Combine_C12,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_COMBINE_C12,TRUE,_T("eAlignerPos12_Combine_C12"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//11
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_Combine_C12V,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_COMBINE_C12V,TRUE,_T("eAlignerPos12_Combine_C12V"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//12
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_Auto_Weigth,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_AUTO_WEIGHT,TRUE,_T("eAlignerPos12_Auto_Weigth"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//13
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_12C1_Weigth,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C1_WEIGHT,TRUE,_T("eAlignerPos12_12C1_Weigth"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//14
		aligner2vs2Type = Aligner2Vs2TypeInfo(eAlignerPos12_12C2_Weigth,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C2_WEIGHT,TRUE,_T("eAlignerPos12_12C2_Weigth"));
		m_vAligner2Vs2TypeInfo.push_back(aligner2vs2Type);//15


		//3Vs3对位方式
		Aligner3Vs3TypeInfo aligner3vs3Type(eAlignerPos123_Auto,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO,TRUE,_T("eAlignerPos123_Auto"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//0
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_123C1,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C1,TRUE,_T("eAlignerPos123_123C1"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//1
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_123C2,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2,TRUE,_T("eAlignerPos123_123C2"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//2
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_123C3,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C3,FALSE,_T("eAlignerPos123_123C3"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//3
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_123C13L12,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C13L12,TRUE,_T("eAlignerPos123_123C13L12"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//4
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_123C13AL12,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C13AL12,TRUE,_T("eAlignerPos123_123C13AL12"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//5
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_123C2L13,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2L13,TRUE,_T("eAlignerPos123_123C2L13"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//6
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_123C2TL13,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2TL13,TRUE,_T("eAlignerPos123_123C2TL13"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//7
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_Auto_Weigth,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO_WEIGHT,TRUE,_T("eAlignerPos123_Auto_Weigth"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//8
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_123C1_Weigth,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C1_WEIGHT,TRUE,_T("eAlignerPos123_123C1_Weigth"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//9
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_123C2_Weigth,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2_WEIGHT,TRUE,_T("eAlignerPos123_123C2_Weigth"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//10
		aligner3vs3Type = Aligner3Vs3TypeInfo(eAlignerPos123_Auto_L1C_Weigth,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO_L1C_WEIGHT,TRUE,_T("eAlignerPos123_Auto_L1C_Weigth"));
		m_vAligner3Vs3TypeInfo.push_back(aligner3vs3Type);//11

		//4Vs4对位方式
		Aligner4Vs4TypeInfo aligner4vs4Type(eAlignerPos1234_Auto,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_AUTO,TRUE,_T("eAlignerPos1234_Auto"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//0
		aligner4vs4Type = Aligner4Vs4TypeInfo (eAlignerPos1234_123C1,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_123C1,TRUE,_T("eAlignerPos1234_123C1"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//1
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_124C2,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_124C2,TRUE,_T("eAlignerPos1234_124C2"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//2
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_134C3,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_134C3,TRUE,_T("eAlignerPos1234_134C3"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//3
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_234C4,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_234C4,TRUE,_T("eAlignerPos1234_234C4"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//4
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_Combine,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE,TRUE,_T("eAlignerPos1234_Combine"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//5
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_Combine_4Vs4,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_4VS4,TRUE,_T("eAlignerPos1234_Combine_4Vs4"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//6
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_Combine_Target_4Vs4,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_TARGET_4VS4,TRUE,_T("eAlignerPos1234_Combine_Target_4Vs4"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//7
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_Combine_Object_4Vs4,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_OBJECT_4VS4,TRUE,_T("eAlignerPos1234_Combine_Object_4Vs4"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//8
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_1234_Weight,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_1234_WEIGHT,TRUE,_T("eAlignerPos1234_1234_Weight"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//9
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_Combine_Weight,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_WEIGHT,TRUE,_T("eAlignerPos1234_Combine_Weight"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//10
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_56_4V2,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_56_4V2,TRUE,_T("eAlignerPos1234_56_4V2"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//11
		aligner4vs4Type = Aligner4Vs4TypeInfo(eAlignerPos1234_Center,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_CENTER,TRUE,_T("eAlignerPos1234_Center"));
		m_vAligner4Vs4TypeInfo.push_back(aligner4vs4Type);//12
	}
}AlignerTypeInfo;
