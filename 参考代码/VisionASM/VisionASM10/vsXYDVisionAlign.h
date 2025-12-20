// svXYDVisionAilgner.h: interface for the vcXYDVisionAlign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVXYDVISIONAILGNER_H__B97ED5A8_953A_4190_90BC_73B019D5D2AD__INCLUDED_)
#define AFX_SVXYDVISIONAILGNER_H__B97ED5A8_953A_4190_90BC_73B019D5D2AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vsBaseVisionAlign.h"
#include "vsXYDAlignTool.h"


// 视觉对位处理:基本可以流程可以处理XYD
class vcXYDVisionAlign : public vcBaseVisionAlign  
{
public:
	vcXYDVisionAlign();
	virtual ~vcXYDVisionAlign();
	virtual void DestroyWorkThread();

public:

	//   设置系统的选项信息
	void SetSystemOptionConfig(CSystemOptionConfig *pSystemOptionConfig);
	virtual BOOL SetProductCameraShutter(int nIndex);
	virtual	void AddCommCommandInfo(BOOL bSend, CString strCmd, BOOL bTimeExist = FALSE,BOOL bAddDetailInfo = TRUE);

	
public:
	
	// S7：初始化相机：根据相机相机信息(个数、序列号、帧数、曝光时间、增益等），初始化相机，设置相机回调函数，准备开始采集图像；
	// S7.1：初始化图像对象：根据相机个数初始化用于接收数据的图像类对象；
	// S8：初始化通信接口：根据通信接口信息（接口类型及相应的配置信息），初始化通信接口，准备开始接收通信指令信息；
	// S9：初始化标定用的定位工具：；
	// S10：初始化对位用的定位工具：；
	// S11：初始化对位工具的标定功能：；
	// S12：初始化对位工具的对位功能：；
	// S13：创建平台的对位处理线程：；
	// S13.1：

	// method
public:

	// 总体信息：对位平台信息、相机平台信息	
// 	virtual void SetPlatformInfo(CPlatformInfo*	pPlatformInfo);			// 设置平台信息（与主界面交互）
// 	virtual CPlatformInfo* GetPlatformInfo();							// 获取平台信息	（与主界面交互）
	
// 	virtual void SetCamPlatformInfo(CCamPlatformInfo* camPlatformInfo);	// 设置相机平台信息（与主界面交互）
// 	virtual CCamPlatformInfo* GetCamPlatformInfo();						// 获取相机平台信息（与主界面交互）


	//////////////////////////////////////////////////////////////////////////
	// 对位工具：

	// 初始化对位工具（定义new AlignerTool+初始化标定时的nIndex个定位工具对象+下面三个参数设置+初始化对位用的nIndex个定位工具对象+下面两个参数设置）
	virtual BOOL InitAlignTool();	
	virtual BOOL InitAlignTool(int nProductIndex);

public:
	// 对位工具：标定					
//	virtual void SetCalibrateToolParam(CPlatformInfo* pPlatformInfo, CCamPlatformInfo* pCamPlatformInfo, CCalibrateParam* sysCalibParam); // 将这三个参数设置到m_pVisionASMConfig中对位的对应成员中（与主界面交互）										
//	virtual void GetCalibrateToolParam(CPlatformInfo** ppPlatformInfo, CCamPlatformInfo** ppCamPlatformInfo, CCalibrateParam** ppCalibParam); // 获取m_pVisionASMConfig中对位的对应成员（与主界面交互）										
//	virtual void SetCalibrateToolParam(CPlatformInfo* pPlatformInfo, CCamPlatformInfo* pCamPlatformInfo); // 将这三个参数设置到m_pVisionASMConfig中对位的对应成员中（与主界面交互）,并将设置好的数据传送给对位工具m_pAlignerTool										
//	virtual void GetCalibrateToolParam(CPlatformInfo** ppPlatformInfo, CCamPlatformInfo** ppCamPlatformInfo); // 获取m_pVisionASMConfig中对位的对应成员（与主界面交互）										


	virtual BOOL ExecuteBenchMarkFixed(std::vector<CCoordPos> vPtMarkImageBench);     // 执行基准目标Mark修正 
    BOOL         ExecuteTargetBenchMarkFixed();
protected:
	virtual BOOL ExcutePlatformCalibrateRefine(int nCalibPos);                           // 平台标定求精（nIndex为相机索引值）  


	//////////////////////////////////////////////////////////////////////////
	// 对位工具：对位

	//////////////////////////////////////////////////////////////////////////
	// 平台工作线程
protected:
	virtual BOOL CreateWorkThread();			// 创建平台工作线程

	// 工作线程：提取通信接口的命令信息，解析命令信息，执行相应的处理（对象搜索、目标搜索、标定、对位、错误命令）
	// 在处理过程中，还会有其他一些命令（命令包括命令及命令响应，指定平台运动的命令、获取平台位置的命令、指定相机平台运动的命令、获取相机平台位置的命令）
	static UINT WorkThread(LPVOID pParam);      // 工作线程


	// 手动搜索线程
	static UINT ManualSearchThread(LPVOID pParam);      // 手动搜索线程
	UINT ManualSearchThreadFunc();						// 手动搜索线程函数

	BOOL ManualTargetSearchEx(int nPosIndex);		// 手动目标搜索
	BOOL ManualObjectSearchEx(int nPosIndex);		// 手动对象搜索


public:	
	virtual BOOL SuspendWorkThread();           // 挂起工作线程(一个流程走完，再进入等待状态)（与主界面交互）	
   	virtual BOOL ResumeWorkThread();            // 恢复工作线程（与主界面交互）


private:
	//通信获取目标图像坐标和相应轴位置
	BOOL  CommuciateGetTargetImagePos(std::vector<int> &vnPosIndex,std::vector<CCoordPos>& vImagePos,std::vector<CPlatformXYDAxisPos> &vPlatformAxis);
	//通信获取目标平台坐标和相应轴位置
	BOOL  CommuciateGetTargetPlatformPos(std::vector<int> &vnPosIndex,std::vector<CCoordPos>& vCoordPos,std::vector<CPlatformXYDAxisPos> &vPlatformAxis);
	//通信获取靶标坐标（采用二维码计算目标固定补偿时使用）
	BOOL  CommuciateGetBoardPosForTarFixOffset(std::vector<int> &vnPosIndex,std::vector<CCoordPos>& vBoardPos);

	BOOL  CommParamGetPlatformAxisAbsPosBackup(CPlatformXYDAxisPos* pPlatformXYDAxisPos);				 // 获取平台各轴的绝对位置（备用）
	BOOL  CommuciateGetPlatformAxisAbsPosBackup(CPlatformXYDAxisPos* pPlatformXYDAxisPos);				 // 获取平台各轴的绝对位置（备用）
	BOOL  CommParamGetPlatformAxisAbsPos(CPlatformXYDAxisPos* pPlatformXYDAxisPos);                      // 获取平台各轴的绝对位置
	BOOL  CommuciateGetPlatformAxisAbsPos(CPlatformXYDAxisPos* pPlatformXYDAxisPos,BOOL bForceReadPlcReg = FALSE);// 获取平台各轴的绝对位置
	BOOL  CommuciateDrivePlatformAxisMove(CPlatformXYDAxisPos* pPlatformXYDAxisPos);                     // 驱动平台各轴到指定位置
	
	BOOL  CommuciateDrivePlatformAxisMove2(CPlatformXYDAxisPos* pPlatformXYDAxisPos);                     // 或者说，把仅仅把偏差量写入寄存器；驱动平台各轴到指定位置，

	BOOL  CommuciateGetCamPlatformAxisAbsPos(std::vector<CPlatformXYAxisPos*>& vpCamPlatformXYAxisPos);  // 获取所有相机平台各轴的绝对位置
    BOOL  CommuciateDriveCamPlatformAxisMove(std::vector<CPlatformXYAxisPos*> vpCamPlatformXYAxisPos);   // 驱动所有相机平台各轴到指定位置
	
	BOOL  CommuciateGetProductSize(std::vector<CPlatformXYAxisPos*> vpProductSize);			 // 获取产品尺寸
	
	BOOL  CommuciateWaitCamMove();                                                                       // 等待相机移动到位（仅单相机用）
//	BOOL  CommParamGetPlatformAxisOffset(CPlatformXYDAxisPos* pPlatformXYDAxisPos);
public:
	//功能：通信发送二维码靶标坐标（对象目标不在相同工控机时对象目标采用关联方法时目标端使用）
	//参数：位置序号数组、各位置图像中心靶标坐标
	BOOL  CommuciateSendDmCodeBoardAbsPos(std::vector<int> vnPosIndex,std::vector<CCoordPos> &vBoardPos);
	//功能：通信获取二维码靶标坐标和目标图像尺寸（对象目标不在相同工控机时对象目标采用关联方法时对象端使用）
	//参数：位置序号数组、各位置图像尺寸、各位置图像中心靶标坐标
	BOOL  CommuciateGetDmCodeBoardAbsPos(std::vector<int> &vnPosIndex,std::vector<CCoordPos> &vImagePos,std::vector<CCoordPos> &vBoardPos);

public:
	BOOL ReadRegData( CString strRegAddr, int nReadNum, std::vector<int> &vnReceiveData);				// 读寄存器
	BOOL ReadRegData( CString strRegAddr, int nReadNum, std::vector<double> &vdReceiveData);			// 读寄存器
	BOOL WriteRegData( CString strRegAddr, int nWriteNum, std::vector<double> vdWriteData);				// 写寄存器
	BOOL WriteRegData( CString strRegAddr, int nWriteNum, std::vector<int> vnWriteData);				// 写寄存器
	BOOL ReceiveComm(std::vector<int>& vnReceivedData);	

	BOOL ReceiveCommPlcReg(SysWorkCmdType& sysWorkCmdType);											// 接受指令
	BOOL ReceiveCommRepeat(CString& strInfo, CommStatus& nCommStatus, BOOL bLog = TRUE);									// 循环读取通信接口数据									

	// 发送读PLC寄存器字符串，并接收响应字符串
	BOOL CommunicateSendReadRegStr(CString strSend, CString& strInfo, CommStatus& nCommStatus, BOOL bLog = TRUE);

	// 发送写PLC寄存器字符串，并接收解析PLC的响应字符串是否正确
	BOOL CommunicateSendWriteRegStr(CString strSend, CString& strInfo, CommStatus& nCommStatus);
	//将一组整数写到PLC指定寄存器，数据量较大时将自动分多次写入
	BOOL CommunicateWriteReg(std::vector<int> vnSend, CString& strInfo, CommStatus& nCommStatus
		,int nRegAddr,bool bRegType,bool bOffsetAddr);


	BOOL ClearPlcPlatformTrigger();																	// 将对象平台触发寄存器置0
	BOOL SetVisionPlatformTrigger();																// 将对象平台触发寄存器置1
	
	BOOL SetPlcManualSearch(BOOL bShow);

	BOOL SetPlcManualSearch_EX(BOOL bShow);	 //通知PLC手动弹框,腾盛协议定制

	//目标采用靶标关联时，回复PLC结果
	BOOL ReportCalTargetFixOffsetFAFO(BOOL bResult);
	BOOL ReportCalTargetFixOffsetFAFO(BOOL bResult,std::vector<int> vnPosIndex,std::vector<CCoordPos> &vCoordPos,std::vector<CPlatformXYDAxisPos>& vPlatformAixs,std::vector<CCoordPos> &vBoardPos);



	BOOL ReportCamCalibResult(BOOL bResult);

	BOOL ReportCalibResult(BOOL bResult);
	BOOL ReportCalibResultFCBS(BOOL bResult);
	BOOL ReportCalibResultFCBP(BOOL bResult);
	BOOL ReportObjectSearchResult(BOOL bResult);
	BOOL ReportObjectSearchResultFOBC(BOOL bResult, int nSuccessPatIndex = 0);
	BOOL ReportTargetSearchResult(BOOL bResult, int nSuccessPatIndex = 0);
	BOOL ReportTargetSearchResultFTGT(BOOL bResult);
	BOOL ReportTargetSearchResultFTGS(BOOL bResult);

	BOOL ReportSearchResultFGAP(BOOL bResult,double dImagePosX, double dImagePosY,double dImagePosD);
	BOOL ReportObjectSearchResultFCNT(BOOL bResult);
	BOOL ReportSearchResultFGTC(BOOL bResult,double dImagePosX, double dImagePosY,double dImagePosD);


	BOOL ReportTargetSearchResultFCNO(BOOL bResult);

	BOOL ReportAlignResultFACO(CCoordPos platOffset,BOOL bResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL, CCoordPos platBenchOffset = CCoordPos(0,0,0), CPlatformXYDAxisPos* pBenchPlatformXYDAxisPos = NULL);

	BOOL ReportAlignMulResultFACO(std::vector<CCoordPos> vplatOffset,BOOL bResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL, CCoordPos platBenchOffset = CCoordPos(0,0,0), CPlatformXYDAxisPos* pBenchPlatformXYDAxisPos = NULL);

	//拾取按照上料对位, 临时寄存器中返回xy偏差量
	BOOL ReportAlignResultFACO(CCoordPos platOffset, CCoordPos platOffsetPickResive,BOOL bResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL, CCoordPos platBenchOffset = CCoordPos(0,0,0), CPlatformXYDAxisPos* pBenchPlatformXYDAxisPos = NULL);


	BOOL ReportAlignResultFAAO(CCoordPos platOffset,BOOL bResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL);
	BOOL ReportAlignResultFOLC(CCoordPos platOffset,BOOL bResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL);
	BOOL ReportAlignResultFABO(CCoordPos platOffset,BOOL bResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL);
    BOOL ReportAlignResultFAAL(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0), CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL, CPlatformXYDAxisPos* pTargetOffsetPlatformXYDAxisPos = NULL);
	BOOL ReportAlignResultFXYA(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0), CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL);
	BOOL ReportAlignResultFAAB(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0), CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL);
	BOOL ReportAlignResultFAAC(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0), CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL, CPlatformXYDAxisPos* pTargetOffsetPlatformXYDAxisPos = NULL);
	BOOL ReportAlignResultFACB(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0), CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL);
    BOOL ReportAlignResultFALG(CPlatformXYDAxisPos* pPlatformXYDAxisPos,BOOL bResult, CCoordPos platBenchOffset = CCoordPos(0,0,0), CPlatformXYDAxisPos* pBenchPlatformXYDAxisPos = NULL);
	BOOL ReportAlignResultFAHL(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0));
	BOOL ReportMulAlignPosResultFMCO(std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset,BOOL bResult);	// 对位拾取响应

	BOOL ReportAlignResultFALP(CPlatformXYDAxisPos* pPlatformXYDAxisPos,BOOL bResult);	// 对位拾取响应
	BOOL ReportAlignResultFALP_int(CPlatformXYDAxisPos* pPlatformXYDAxisPos,int nResult);	// 对位拾取响应
	BOOL ReportMulAlignPosResultFALP(std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos,BOOL bResult);	// 对位拾取响应
	BOOL ReportAlignResultNumberFALP(CPlatformXYDAxisPos* pPlatformXYDAxisPos,int nResultNumber);	// 对位拾取响应
 
	BOOL ReportAlignResultFAOT(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0));
	BOOL ReportAlignResultFALX(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0));
	BOOL ReportObjectSearchResultFOBM(BOOL bResult);
	BOOL ReportTargetSearchResultFTGM(BOOL bResult);
	
	BOOL ReportACFInspectResultFCHB(BOOL bResult, int nSuccessPatIndex = 0);
	BOOL ReportACFInspectResultFCHBD(BOOL bResult, int nSuccessPatIndex, std::vector<double> vdDistance, std::vector<BOOL> vbAreaInspectSubResult);
	BOOL ReportACFInspectResultFCHC(BOOL bResult, int nSuccessPatIndex = 0);
	BOOL ReportResultFDRP( BOOL bResult);

	BOOL ReportSetProductExposureFSPE(BOOL bResult);	// 设置产品曝光
	BOOL ReportAlignResultFWEL(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0));	// 专用焊接指令,小FPC:XY,大FPC:XYD
	BOOL ReportAlignResultFSOL(BOOL bResult, BOOL bAlignBench = FALSE, CCoordPos platOffset = CCoordPos(0,0,0));	// 专用焊接指令,小FPC:XYD,大FPC:XY

	
	BOOL ReportObjTarSearchResultFPCB(BOOL bResult,std::vector<CCoordPos> markPos);		// 对象、目标图像定位
	BOOL ReportPCBPrecisionFPIS(BOOL bResult);				// PCB 精度检测
	BOOL ReportPCBPrecisionFCPI(BOOL bResult);
	
	
	// 检测产品有、无
	BOOL ReportProductInspectFINS(BOOL bResult);
	
	// 1、 FVAS：
	BOOL ReportSetProductVirtualAlnFVAS(BOOL bResult);

	// 清除报警
	BOOL ReportResetResult(BOOL bResult);
    BOOL ReportCommResultFCHK(BOOL bResult);

	//lzk 0407 易天覆膜机-位置虚拟-指令交互返回
	BOOL ReportSetPositionVirtualFVPC(BOOL bResult);	

	// 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
	BOOL ReportSetProductIndexFSPC(BOOL bResult);	
	// 2、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
	BOOL ReportSaveProductIndexFSSV(BOOL bResult);
	// 3、 FGSP：产品No.的取得，取得目前正在执行的产品No.
	BOOL ReportGetProductIndexFGSP(BOOL bResult, int nCurProductIndex, int nAssitIndex = 1);


	// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
	BOOL ReportGetAlignPrecisionFGAL(BOOL bResult, CCoordPos cpAlignPrecision);	
	// 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
	BOOL ReportSetAlignPrecisionFALL(BOOL bResult);	
	// 3、FGAC:对准最多次数的取得，取得设定对准最多次数
	BOOL ReportGetAlignTimeFGAC(BOOL bResult, int nAlignTime);		
	// 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
	BOOL ReportSetAlignTimeFALC(BOOL bResult);		


	// 2、FGOF:补偿量值的取得，取得设定的补偿量值
	BOOL ReportGetAlignOffsetFGOF(BOOL bResult, CCoordPos cpAlignOffset);		
	// 3、FOFS:补偿量值的设定，设定补偿量值
	BOOL ReportSetAlignOffsetFOFS(BOOL bResult);

	BOOL ReportGetPlatformOffsetFGPO(BOOL bResult, CCoordPos cpAlignOffset);

	// FCCI：复制标定信息
	BOOL ReportCopyCalibrateInfoFCCI(BOOL bResult);

//	BOOL OnSingleCameraAlignerCalibratePick(int nCalibPos);			// 执行拾取的标定(像素当量、旋转方向)
//	BOOL OnSingleCameraAlignerCalibrateCenterPick(int nCalibPos);   // 执行拾取的标定(旋转中心)
	
	
	// 对象拍照或者目标拍照之后执行对位计算
	BOOL OnlineAlignExFACOAfterSearch(int nOffsetIndex,int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0);

	BOOL OnlineAlignFACO(int nOffsetIndex, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0 );
	BOOL OnlineAlignMulExFACO(int nOffsetIndex, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0 );

	BOOL OnlineAlignExFACO(int nOffsetIndex, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0 );
	BOOL OnlineAlignExFMCO(int nOffsetCount);
	BOOL OnlineAlignExFOLC(int nOffsetIndex);
	BOOL OnlineAlignExFABO(int nOffsetIndex);
//	BOOL OnlineAlignExFAAL(int nOffsetIndex, int nReservedIndex = 10, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0);	
	BOOL OnlineAlignExFAAB(int nOffsetIndex);			// 包含图像十字中心基准对位过程的自动迭代对位过程
	BOOL OnlineAlignExFXYA(int nOffsetIndex,int nStartPatIndex);	

	BOOL OnlineAlignExFAAL_Base(int nOffsetIndex, int nReservedIndex = 10, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0);
	BOOL OnlineAlignExFAAL(int nOffsetIndex, int nReservedIndex = 10, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0);	
	BOOL OnlineAlignExFAAL_BendAlign(int nOffsetIndex, int nReservedIndex = 10, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0);	


	BOOL OnlineAlignExFAAC(int nOffsetIndex, int nStartPatIndex, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0);
	BOOL OnlineAlignExFAAO(int nOffsetIndex);
	BOOL OnlineAlignExFACB(int nOffsetIndex, int nStartPatIndex);// 包含图像十字中心基准对位过程的自动迭代对位过程
	BOOL OnlineAlignExFAOT(int nOffsetIndex, int nStartPatIndex); //目标对象候补搜索，迭代对位，图像十字中心基准对位
	BOOL OnlineAlignExFALX(int nOffsetIndex, int nStartPatIndex); //对象候补搜索，迭代对位，图像十字中心基准对位
	BOOL OnlineAlignExFALG(int nOffsetIndex, int nReservedIndex = 10, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0);

	// 拾取
	BOOL OnlineAlignPickExFALP(int nOffsetIndex);
	BOOL OnlineAlignExFALP(int nOffsetIndex);
	BOOL OnlineMulAlignPickExFALP(int nOffsetIndex);

	BOOL SendDrivePlatformAxisMoveEx(CPlatformXYDAxisPos* pPlatformXYDAxisPos);


	// FTGT
	BOOL OnlineAlnTargetSearchExFTGT(int nPosIndex);
	BOOL OnSingleCameraAlignerTargetSearchExFTGT(int nPosIndex); 
	BOOL OnSerialCameraAlignerTargetSearchExFTGT();
	BOOL OnSerialCameraAlignerTargetSearchExFTGT(std::vector<int> vnPosIndex);

	// FTGM
	BOOL OnlineAlnTargetSearchExFTGM(int nPosIndex);

	// FGAP
	BOOL OnlineAlnSearchExFGAP(int nPosIndex, int nPatIndex);
	BOOL OnSingleCameraAlignerSearchExFGAP(int nPosIndex, int nPatIndex); 

	// FGTC
	BOOL OnlineAlnSearchExFGTC(int nPosIndex, int nPatIndex);
	BOOL OnSingleCameraAlignerSearchExFGTC(int nPosIndex, int nPatIndex);

	// FTGS
	BOOL OnlineAlnTargetSearchExFTGS(int nPosIndex);
	BOOL OnSingleCameraAlignerTargetSearchExFTGS(int nPosIndex); 
	BOOL OnSerialCameraAlignerTargetSearchExFTGS();
	BOOL OnSerialCameraAlignerTargetSearchExFTGS(std::vector<int> vnPosIndex);

	// FTGC
	BOOL OnlineAlnTargetSearchCandidateExFTGC(int nPosIndex, int nStartPatIndex);
	BOOL OnSingleCameraAlignerTargetSearchCandidateExFTGC(int nPosIndex, int nStartPatIndex); 	
	BOOL OnSerialCameraAlignerTargetSearchCandidateExFTGC(int nStartPatIndex);
	BOOL OnSerialCameraAlignerTargetSearchCandidateExFTGC(std::vector<int> vnPosIndex, int nStartPatIndex);

	// FOBJ
	BOOL OnlineAlnObjectSearchExFOBJ(int nPosIndex);
	BOOL OnSingleCameraAlignerObjectSearchExFOBJ(int nPosIndex);
	BOOL OnSerialCameraAlignerObjectSearchExFOBJ();
	BOOL OnSerialCameraAlignerObjectSearchExFOBJ(std::vector<int> vnPosIndex);

	// FCNT
	BOOL OnlineAlnObjectSearchExFCNT(int nPosIndex);
	BOOL OnSingleCameraAlignerObjectSearchExFCNT(int nPosIndex);
	BOOL OnSerialCameraAlignerObjectSearchExFCNT();
	BOOL OnSerialCameraAlignerObjectSearchExFCNT(std::vector<int> vnPosIndex);

	// FCNO
	BOOL OnlineAlnTargetSearchExFCNO(int nPosIndex);
	BOOL OnSingleCameraAlignerTargetSearchExFCNO(int nPosIndex);
	BOOL OnSerialCameraAlignerTargetSearchExFCNO();
	BOOL OnSerialCameraAlignerTargetSearchExFCNO(std::vector<int> vnPosIndex);

	// FOBM
	BOOL OnlineAlnObjectSearchExFOBM(int nPosIndex);


public:

	// FOBC
	BOOL OnlineAlnObjectSearchCandidateExFOBC(int nPosIndex, int nStartPatIndex);
	BOOL OnSingleCameraAlignerObjectSearchCandidateExFOBC(int nPosIndex, int nStartPatIndex);
	BOOL OnSerialCameraAlignerObjectSearchCandidateExFOBC(int nStartPatIndex);
	BOOL OnSerialCameraAlignerObjectSearchCandidateExFOBC(std::vector<int> vnPosIndex, int nStartPatIndex);
	// 一次发送多个位置
	BOOL OnSingleCameraMulAlignerPosObjectSearchCandidateExFOBC(int nPosIndex, int nStartPatIndex);
	BOOL OnSerialCameraMulAlignerPosObjectSearchCandidateExFOBC(int nStartPatIndex);

// public:
// 	int OnlineTargetSearch(std::vector<int> vnPosIndex, int nStartPatIndex, int nEndPatIndex, bool bSnapImage = true);

public:


	/////////////////////////////////////////bai
	BOOL OnSerialCameraAlignerTargetAndObjectSearchFOBC(std::vector<int> vnTargetPosIndex,std::vector<int> vnObjectPosIndex,int nSameType);	//目标对象搜索
	//0代表1234相机共同使用 1 代表可能只用12相机或者34相机
	//int OnSerialCameraAlignerTargetAndObjectSearchExFOBJ(std::vector<int> vnTargetPosIndex,std::vector<int> vnObjectPosIndex);	//目标对象搜索
	////////////////////////////////////////////
	// 当目标相机和对象相机是独立时，标定目标相机时，进行目标和对象的Mark定位搜索
	BOOL SingleCameraCalibAlignerTargetSearchExFTGT(int nPosIndex, int nMoveIndex);
    BOOL SingleCameraCalibAlignerTargetSearchExDmCode(int nCalibPos);//二维码关联搜索
	BOOL SerialCameraCalibAlignerTargetSearchExDmCode(std::vector<int> vnCalibPos);//二维码关联搜索
	BOOL SingleCameraCalibAlignerTargetSearchEx(int nCalibPos);//两点或四点自动关联搜索
	BOOL SerialCameraCalibAlignerTargetSearchEx(std::vector<int> vnCalibPos);//两点或四点自动关联搜索
	BOOL SerialCameraCalibAlignerTargetSearch(std::vector<int> vnCalibPos); //自动关联搜索
	// 当目标相机和对象相机是独立时2对4方案中，通过靶标标定目标位置4、位置5相机时，进行目标Mark定位搜索
	BOOL SingleCameraCalibBoardAlignerTargetSearchExFTGT(int nPosIndex, int nMoveIndex);
	std::vector<CMarkImagePos*> GetCalibMarkImagPos();	// 获取标定所有的移动定位结果

	BOOL GetCalibAlignerTargetPlatformAxisPos(int nIndex = 0);
	//计算目标固定补偿时目标二维码搜索
	BOOL TargetSearchForTarFixOffset(std::vector<int> vnPosIndex);
	//计算目标固定补偿时对象二维码搜索
	BOOL ObjectSearchForTarFixOffset(std::vector<int> vnPosIndex);
	//计算目标固定补偿，参数：目标固定补偿序号
	BOOL CaculateTarFixOffset(int nOffsetIndex);
//	BOOL ExecuteCalibWholeDmCodeBoardImgToBoard(int nPosIndex);//执行全局二维码图像到靶标的映射
//	BOOL ExecuteCalibWholeDmCodeBoard(int nCalibPos);//执行全局二维码标定计算（求旋转中心）
	BOOL ExecuteCalibWholeDmCodeBoardPick(int nCalibPos,int nMarkType);//执行全局二维码标定拾取的计算（求旋转中心），相机在随XY轴运动时使用
	// FCHB ACF检测
	BOOL OnlineACFInspectCandidateExFCHB(int nPosIndex, int nStartPatIndex);
	BOOL OnSingleCameraACFInspectCandidateExFCHB(int nPosIndex, int nStartPatIndex);
	BOOL OnSerialCameraACFInspectCandidateExFCHB(int nStartPatIndex);
	BOOL OnSerialCameraACFInspectCandidateExFCHB(std::vector<int> vnPosIndex, int nStartPatIndex);
	BOOL OnSerialCameraACFInspectCandidateExFCHB(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex);
	
	// FCHC 候补ACF检测
	BOOL OnlineACFInspectCandidateExFCHC(int nPosIndex, int nStartPatIndex, int nEndIndex);
	BOOL OnSingleCameraACFInspectCandidateExFCHC(int nPosIndex, int nStartPatIndex, int nEndIndex);
	BOOL OnSerialCameraACFInspectCandidateExFCHC(int nStartPatIndex, int nEndIndex);
	BOOL OnSerialCameraACFInspectCandidateExFCHC(std::vector<int> vnPosIndex, int nStartPatIndex, int nEndIndex);
	
	// FDRP 计算基准拾取位置
	BOOL OnlineExFDRP(int nAngleIndex,int nExProductIndex = 0);


	// FSPE 设置产品曝光
	BOOL OnlineSetProductExposureFSPE(int nMarkTpye, int nPosIndex, int nCandIndex = 0);
	BOOL OnSerialCameraSetProductExposureExFSPE(int nMarkTpye, std::vector<int> vnPosIndex, int nCandIndex = 0);

	BOOL OnSerialObjTarSearchCandidateExFPCB(int nMarkType);													// 对象、目标图像定位
//	BOOL OnlineSetPCBPrecisionFPIS(int nOffsetIndex);															// PCB 精度检测
	BOOL OnlineSetPCBPrecisionFCPI(int nIndex);

	// 专用焊接指令
	BOOL OnlineAlignExFWEL(int nOffsetIndex, int nStartPatIndex); //目标对象候补搜索，迭代对位，图像十字中心基准对位
	BOOL OnlineAlignExFSOL(int nOffsetIndex, int nStartPatIndex); //目标对象候补搜索，迭代对位，图像十字中心基准对位


		// 检测产品有、无
	BOOL OnlineProductInspectFINS(int nMarkTpye, int nPosIndex, int nCandIndex = 0);
	BOOL OnSingleCameraProductInspectExFINS(int nMarkTpye, int nPosIndex, int nCandIndex = 0);

	// 虚拟对象偏移量相关
	// FOBJ
	BOOL SingleCameraAlignerObjectSearchExFOBJ(int nPosIndex);
	// FVOJ
	BOOL SingleCameraAlignerVirtualObjectSearchExFVOJ(int nPosIndex);
	CMarkImagePos GetObjectMarkImagePos();
	CMarkImagePos GetTargetMarkImagePos();
	CMarkImagePos GetVirtualObjectMarkImagePos();

	// 设置保存虚拟对象的偏移量
	BOOL SetAndSaveVirtualMarkOffset(std::vector<CCoordPos>& vObjectToVirtualOffsets, int nExProductIndex = 0);


	// 主要用在目标和对象的相机分离情况下
	void SetDefaultTargetSearchMarkPos(BOOL bOK, int nIndex = 0);				// 设置默认的目标模板Mark的平台轴位置和图像
	void SetDefaultObjectSearchMarkPos(BOOL bOK, int nIndex = 0);				// 设置默认的对象模板Mark的平台轴位置和图像
	void SetDefaultVirtualObjectSearchMarkPos(BOOL bOK);		// 设置默认的虚拟模板Mark的平台轴位置和图像
	void SetDefaultCrossBenchTargetSearchMarkPos(BOOL bOK,BOOL bZeroAngle = 0);		// 设置默认的十字中心模板Mark的平台轴位置和图像

	BOOL IsObjectMarksZeroAngle();
	BOOL IsTargetMarksZeroAngle();


	// 基准拾取相关
	BOOL GetCalibAlignerBenchPickPlatformAxisPos();				// 通信获取基准拾取轴位置
	virtual BOOL ExecuteBenchMarkPick(int nIndex,int nExProductIndex = 0);			// 执行基准拾取Mark 0.初始位置  1.90度位置   2.180度位置  3. 270度位置
	virtual BOOL SaveBenchMarkPick();							// 保存基准拾取Mark 


	BOOL SendResetResult();
    BOOL SendCommResultFCHK();
	BOOL SendCommConnectStatus();//写寄存器通知PLC通信状态，限寄存器通信使用

	std::vector<CPlatformXYDAxisPos> m_CalibPlatformAxisAbsPos;			// 上料标准位置标定时,平台轴的位置	

	CPlatformXYDAxisPos				 m_CurPlatformAxisPos;

	CPlatformXYDAxisPos				 m_BenchPickPlatformAxisPos;		// 基准拾取轴位置


//	virtual void	InitTargetPlatformAxisPos();
//	virtual void	InitCrossBenchTargetPlatformAxisPos();
//	virtual void	InitObjectPlatformAxisPos();
//	virtual void	InitObjectCamAxisPos();
//	virtual void	InitTargetCamAxisPos();
//	virtual void	InitObjectCamAxisPlatformAxisPos();
//	virtual void	InitTargetCamAxisPlatformAxisPos();

//	virtual void	SetTempTargetPlatformAxisPos(int nPosIndex, CPlatformAxisPos* pPlatformAxisPos);
//	virtual void	SetTempObjectPlatformAxisPos(int nPosIndex, CPlatformAxisPos* pPlatformAxisPos);
//	virtual void	SetTempCrossBenchTargetPlatformAxisPos(int nPosIndex, CPlatformAxisPos* pPlatformAxisPos);
//	virtual void    SetTempTargetCamAxisPos(int nPosIndex,CPlatformXYAxisPos* pCamAxisPos);
//	virtual void    SetTempObjectCamAxisPos(int nPosIndex,CPlatformXYAxisPos* pCamAxisPos);

//	virtual void	SetTempObjectCamAxisPlatformAxisPos(int nPosIndex,CPlatformXYAxisPos* pCamAxisPos);
//	virtual void	SetTempTargetCamAxisPlatformAxisPos(int nPosIndex,CPlatformXYAxisPos* pCamAxisPos);

	virtual BOOL	CheckObjectDistance();			// 检测对象距离
	virtual	BOOL	CheckTargetDistance();			// 检测目标距离
	virtual BOOL	CheckTargetAngle();				// 检测目标角度(仅支持4VS4)
	virtual BOOL	CheckObjectAngle();				// 检测目标角度(仅支持4VS4)

	virtual BOOL	SETFPMI();			   // 权限开通
	
	
//	virtual BOOL    CheckObjectDistanceByObjMaxDis();//对象目标距离最大值分离时对象距离检查

//	virtual BOOL	CalculateTargetDistance();		// 计算目标距离
//	virtual BOOL	CalculateObjectDistance();		// 计算对象距离
//	virtual BOOL    CalculateObjectDistanceByObjMaxDis();//对象目标距离最大值分离时计算对象距离

//	virtual BOOL	RecordProductSizeIndfo();		// 记录目标、对象尺寸信息
//	virtual BOOL	RecordTargetSizeIndfo(BOOL bSearchOK, BOOL bCheckOK);		// 记录目标、对象尺寸信息
//	virtual BOOL	RecordObjectSizeIndfo(BOOL bSearchOK, BOOL bCheckOK);		// 记录目标、对象尺寸信息


	virtual BOOL    UpdateTargetPosAfterRoate();    // XY+D平台，旋转后刷新目标位置
    virtual BOOL    AddLastTargetOffsetForAln(CPlatformXYDAxisPos* platformAxisPos);
	
public:
	// 切换产品
	virtual BOOL	OnlineSetProductName(CString strCmdCurProductName);		// 根据名称指令设置当前产品
	virtual BOOL	OnlineSetProductIndex(CString strCmdCurProductName);	// 根据索引指令设置当前产品
	
	//lzk 0407 易天覆膜机-位置虚拟-指令交互函数
	virtual BOOL	OnlineSetPositionVirtualIndex(int nObjectVirtualIndex,int nTargetVirtualIndex);

	virtual BOOL	OnlineSetProductIndex(int nCurProductIndex);			// 根据索引指令设置当前产品
	virtual BOOL	OnlineSaveProductIndex(int nProductIndex);				// 根据指令保存当前产品
	virtual BOOL	OnlineGetProductIndex();								// 根据指令获取当前产品
	virtual BOOL	OnlineSetProductVirtualAln(BOOL bVAlnOn);		        // 根据索引指令设置当前产品

	virtual BOOL    OnlineSaveQRCode();										//读取二维码
	BOOL SaveCSVToDisk(CString strFileName,CString strContent,BOOL bCreateNew);
	BOOL ReportSaveQRCodeFQRC(BOOL bResult);

	virtual BOOL OnlineInspectQRCode(int nPosIndex, int nStartPatIndex);	//检测二维码
	BOOL ReportInspectQRCodeFQRI(BOOL bResult,std::vector<int> vCode,CString strCode = _T(""));		//回复PLC读码信息

	// 清空产品
	BOOL OnlineProductEmptyFPEP(int nProductIndexFa, int nProductIndexChd);
	BOOL ReportProductEmptyFPEP(BOOL bResult);

	// 复制产品
	BOOL OnlineProductCopyFPCP(int nSrcProductIndexFa, int nSrcProductIndexChd, int nDstProductIndexFa, int nDstProductIndexChd);
	BOOL ReportProductCopyFPCP(BOOL bResult);

	// 产品重命名
	BOOL OnlineProductRenameFPRN(int nProductIndexFa, int nProductIndexChd, int nProductNameLength);
	BOOL ReportProductRenameFPRN(BOOL bResult);
	BOOL ReceiveProductNamePlcReg(int nDataNum, CString& strNewName);

	//目标采用靶标关联时，执行目标固定补偿二维码拍照或计算
	virtual BOOL    OnlineCalTargetFixOffsetFAFO(int nType,int nPosIndex,int nOffsetIndex,int nExProductIndex = 0);		//计算目标固定补偿
	//功能：获取二维码图像和靶标位置（目标对象不在同一台工控机中时使用）
	//参数：类型：0目标，1对象、图像坐标、靶标坐标
	virtual BOOL    GetDmCodeImageBoardPos(int nType,int nIndex,CMarkImagePos &imagePos,CMarkPlatformPos &boardPos);// nType:0目标，1对象
	// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
	virtual BOOL OnlineGetAlignPrecisionFGAL(int nPrecisiontType);	
	// 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
	virtual BOOL OnlineSetAlignPrecisionFALL(int nPrecisiontType);	
	// 3、FGAC:对准最多次数的取得，取得设定对准最多次数
	virtual BOOL OnlineGetAlignTimeFGAC();		
	// 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
	virtual BOOL OnlineSetAlignTimeFALC(int nAlignTime);	

	// 2、FGOF:补偿量值的取得，取得设定的补偿量值
	virtual BOOL OnlineGetAlignOffsetFGOF(int nOffsetIndex);		
	// 3、FOFS:补偿量值的设定，设定补偿量值
//	virtual BOOL OnlineSetAlignOffsetFOFS(int nOffsetIndex,CCoordPos cpAlignOffset,BOOL bReGetOffsetIndex);
	virtual BOOL OnlineSetAlignOffsetFOFS(int nOffsetIndex);

	// 多次连续对位发送最终精度到PLC
	virtual BOOL OnlineGetPlatformOffsetFGPO();
public:
	//lzk
	// 复制标定信息
	virtual BOOL	OnlineCopyCalibrateInfo(int nCopiedCalibrateIndex,int nCopyToCalibrateIndex);


// 当目标和对象不在同一个平面高度时，但都使用的相机拍摄
public:
//	BOOL OnlineCalibrateEx(int nCalibPos, int nMarkType);					// 标定，nMarkType为标定mark的类型（0：目标，1：对象），nPosIndex:位置
//	BOOL OnSingleCameraAlignerCalibrateEx(int nCalibPos, int nMarkType); 	// 执行标定，nMarkType为标定mark的类型（0：目标，1：对象），nPosIndex:位置
	BOOL OnSingleCameraPostionCalibrateEx(int nCalibPos);
	BOOL OnSingleCameraBenchMarkRgister(int nCalibPos, int nMarkType);		// X+XYD平台上料单相机基准注册，nMarkType为标定mark的类型（0：目标，1：对象），nPosIndex:位置

//	BOOL OnlineFCBSCalibrate(int nCalibPos, int nMarkType);					// 分段标定，nMarkType为标定mark的类型（1为压头目标，0 为玻璃对象，2为计算），nPosIndex:位置
//	BOOL OnSingleCameraAlignerFCBSCalibrate(int nCalibPos, int nMarkType); 	//分步 执行标定，nMarkType为标定mark的类型（1为压头目标，0 为玻璃对象，2为计算），nPosIndex:位置

	BOOL OnlineFCBPCalibrate(int nCalibPos, int nMarkType,int nExProductIndex=0);	// 标定，nPosIndex:位置,nType 类型 0：搜索 1：获取基准轴位置 2：关联计算;nExProductIndex:扩展产品序号
	BOOL OnlineFCPRCalibrate(int nCalibPos, int nCalibType);

	BOOL CheckCalibratePreparation(CString strCmd, int nCalibPos, int nMarkType);		// 检查标定准备条件是否OK
	BOOL CheckVisionASMConfig(CString strCmd);											// 检查系统配置是否ok
	BOOL CheckPlatformInfo(CString strCmd);												// 检查平台信息是否ok
	BOOL CheckCommuciation(CString strCmd);												// 检查通信是否ok
	BOOL CheckCommuciationValid(CString strCmd);										// 检查通信工具是否有效
	BOOL CheckAlignerSearchTool(CString strCmd);										// 检查对位工具是否ok
	BOOL CheckObjectSearchTool(CString strCmd);											// 检查对象定位工具是否ok
	BOOL CheckTargetSearchTool(CString strCmd);											// 检查目标定位工具是否ok
	BOOL CheckInspectSearchTool(CString strCmd);										// 检查Inspect工具是否ok
	BOOL OnFailedCameraExposureSetting(CString strCmd);									// 切换曝光失败报警
	BOOL OnFailedCommuciateGetPlatformAxisAbsPos(CString strCmd);						// 通信获得轴位置无效
	BOOL OnFailedGetCalibPlatformAxisMovePos(CString strCmd);							// 获取各轴的移动量失败
	BOOL OnFailedCommuciateDrivePlatformAxisMove(CString strCmd, int nPos);				// 驱动到指定位置失败
	BOOL OnFailedSnapSearchImage(CString strCmd);										// 采集失败
	BOOL OnFailedCalibSearch(CString strCmd, int nMovePos, int nCalibPos);				// 标定时，搜索失败
	BOOL OnFailedCommuciateGetCamPlatformAxisAbsPos(CString strCmd);					// 通信获取相机轴位置失败
	BOOL OnFailedCommuciateDriveCamPlatformAxisMove(CString strCmd, int nPos);			// 驱动相机轴到指定位置失败
	BOOL OnFailedSetSameCalibrateInfo(CString strCmd);									// 标定信息复制失败
	
	BOOL OnlineAlignExFAHL(int nOffsetIndex);// 叠片机对位
//////////////////////////////////////////bai=====//////////////////////////////
	BOOL OnlineAlignExFAHLex(int nOffsetIndex,int nMarkType);// 叠片机对位,一次对位流程，类似与ACO
//////////////////////////////////////////=====bai//////////////////////////////

	// 标定
	BOOL OnlineCalibrateFCLB(int nCalibPos, int nMarkType);	
	BOOL OnlineCalibrateFCBS(int nCalibPos, int nMarkType);

	BOOL OnlineSingleCameraCalibrateFCLB(int nCalibPos, int nMarkType);				// XYD平台传统模板标定 + 二维码标定
	BOOL OnlineSingleCameraCalibrateFCLB_WholeDmCode(int nCalibPos, int nMarkType);	// XYD平台全局二维码标定
	BOOL OnlineSingleCameraCalibrateFCBS(int nCalibPos, int nMarkType);				// XY+D平台传统模板标定 + 二维码标定
	BOOL OnlineSingleCameraCalibrateFCBS_WholeDmCode(int nCalibPos, int nMarkType);	// XY+D平台全局二维码标定

	BOOL OnlineCalibrate_CheckByAxisMove(int nCalibPos, int nMarkType = 1);				// SystemConfig->m_bEnableCalibrateCheckByAxisMove
	BOOL ExecuteCalibrate_ObjectBench(int nCalibPos, int nMarkType = 1);				// SystemConfig->m_bEnableObjectCalibBench
	BOOL ExecuteCalibrate_CopyInfo_SingleCamera(int nCalibPos, int nMarkType = 1);		// SystemConfig->m_bDiffPosWithSameCalibInfoEnabled
	BOOL ExecuteCalibrate_CopyInfo_VirtualMode(int nCalibPos, int nMarkType = 1);		// SystemConfig->m_bEnableTargetAndObjectPositionVirtualMode
	BOOL ExecuteCalibrate_TargetBench(int nCalibPos, int nMarkType = 1);				// SystemConfig->m_bEnableTargetBench



//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// Yamaha 机器人
	// 1）机器人工作模式查询、设定（手动模式manual、自动模式auto等）；
	virtual BOOL CommunicateRobotGetMod(RobotMod& rmRobotMod);
	virtual BOOL CommunicateRobotSetMod(RobotMod rmRobotMod);

	// 2）机器人自动模式下程序执行操作（自动模式下的运行Run、重置Reset、停止stop等）；
	virtual BOOL CommunicateRobotAutoControl(RobotAutoControl racAutoControl);

	// 3）机器人手动移动速度查询、设定（mspeed；
	virtual BOOL CommunicateRobotGetMSpeed(int& nSpeed);
	virtual BOOL CommunicateRobotSetMSpeed(int nSpeed);

	// 4）机器人移动命令（inch X、Y、Z等)；
	virtual BOOL CommunicateRobotInch(RobotInchAxis nAxis, RobotInchType nType);

	// 5）机器人当前位置查询（WHRXY）；
	virtual BOOL CommunicateRobotGetPosWHRXY(RobotPos& rpPos);

	// 6）设置机器人示教点位（TEACH 200）；
	virtual BOOL CommunicateRobotTeachPos( int nPosIndex);

	// 7）机器人点位查询（Read P200）；
	virtual BOOL CommunicateRobotReadPos(int nPosIndex, RobotPos& rpPos);

	// 8）执行机器人语言指令（@“机器人语言”，如@MOVE P, P100, S=20 [cr/lf]）.
	virtual BOOL CommunicateRobotMovePos(int nPosIndex, int nSpeed);

	// 9）执行中断处理（ETX(03)）
	virtual BOOL CommunicateRobotETX();

	// 10）设置某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) = 20 [cr/lf]）
	virtual BOOL CommunicateRobotSetPosAixs(int nPosIndex, RobotInchAxis nAxis, double dVal);


	// 10）设置某个点的位置（@“机器人语言”，如@WRITE P100 = 100.00 ...... [cr/lf]）
	virtual BOOL CommunicateRobotSetPos(int nPosIndex, RobotPos rpPos);

	
	// 11）查询某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) [cr/lf]）
	// 12) 设置某个变量的值
	// 13）设置某个变量的值
	// 14）主机器人轴移动至可进行绝对式原点复位的位置(ABSADJ X-)
	virtual BOOL  CommunicateRobotABSADJ(RobotInchAxis nAxis, RobotInchType nType);

	// 15）主机器人轴的绝对式原点复位(ABSRESET k)
	virtual BOOL  CommunicateRobotABSRESET(RobotInchAxis nAxis);

	// 16) 设置整型常量的值（@“机器人语言”，如@SGI0 = 20[cr/lf]）
	virtual BOOL CommunicateRobotWriteSGI(int nIndex, int nVal);

	// 17) 查询整型常量的值（@“机器人语言”，如@READ SGI0[cr/lf]）
	virtual BOOL CommunicateRobotReadSGI(int nIndex, int& nVal);

	// 18) 设置实数常量的值（@“机器人语言”，如@SGR0 = 20.0[cr/lf]）
	virtual BOOL CommunicateRobotWriteSGR(int nIndex, double dVal);

	// 18) 查询实数常量的值（@“机器人语言”，如@READ SGR0[cr/lf]）
	virtual BOOL CommunicateRobotReadSGR(int nIndex, double& dVal);


	//	加载/保存机器人示教范围信息
	virtual BOOL LoadRobotTeachRangeInfo();
	virtual BOOL SaveRobotTeachRangeInfo();

public:
	// 获取目标/对象指定位置的平台坐标
	BOOL ReportGetSearchWorldPosFWLD(BOOL bResult, double dPlatformPosX = 0, double dPlatformPosY = 0, double dPlatformPosD = 0);

	// 获取目标/对象指定位置的平台坐标
	BOOL OnlineGetSearchWorldPosFWLD(int nPosIndex, int nMarkType);

	// 获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
	BOOL ReportGetSearchWorldRadiusFWDR(BOOL bResult, double dPlatformPosX = 0, double dPlatformPosY = 0, double dPlatformPosD = 0);

	// 获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
	BOOL OnlineGetSearchWorldRadiusFWDR(int nPosIndex, int nMarkType); 

	// FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
	BOOL ReportGetObjectWorldPosFGOW(BOOL bResult, double dPlatformPosX = 0, double dPlatformPosY = 0, double dPlatformPosD = 0);

	// FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
	BOOL OnlineAlnSearchExFGOW(int nPosIndex, int nPatIndex);

	// FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
	BOOL OnSingleCameraAlignerSearchExFGOW(int nPosIndex, int nPatIndex);

	// FGTP:拍照取得目标（目标靶标）的图像坐标上的位置
	BOOL OnlineAlnSearchExFGTP(int nPosIndex, int nPatIndex);
	// FGTW:拍照取得目标（目标靶标）的平台坐标上的位置
	BOOL OnlineAlnSearchExFGTW(int nPosIndex, int nPatIndex);
	// FGTW:拍照取得目标（目标靶标）的平台坐标上的位置
	BOOL OnSerialCameraAlignerSearchExFGTW(std::vector<int> vnPosIndex, int nStartPatIndex);
	// FGTW:拍照取得目标（目标靶标）的平台坐标上的位置
	BOOL ReportGetTargetPlatformPosFGTW(BOOL bResult,std::vector<int> vnPosIndex,std::vector<CCoordPos> &vCoordPos,std::vector<CPlatformXYDAxisPos>& vPlatformAixs);

	// FGTP:拍照取得目标（目标靶标）的图像坐标上的位置
	BOOL OnSerialCameraAlignerSearchExFGTP(std::vector<int> vnPosIndex, int nStartPatIndex);
	// FGTP:拍照取得目标（目标靶标）的图像坐标上的位置
	BOOL ReportGetTargetImagePosFGTP(BOOL bResult,std::vector<int> vnPosIndex,std::vector<CCoordPos> &vImagePos,std::vector<CPlatformXYDAxisPos>& vPlatformAixs);
	// 关联标定返回
	BOOL ReportCalibResultFCBR(BOOL bResult);

	// 读写寄存器时，关联标定通信获取两个MARK的平台坐标
	BOOL CommuciateGetPlatformPosFCBR(double& dPlatformPos1X, double& dPlatformPos1Y, double& dPlatformPos2X, double& dPlatformPos2Y);

	// 关联标定，输入两个MARK的平台坐标，与两个MARK的靶标坐标一起，计算靶标坐标系到平台坐标系转换关系
	BOOL OnlineFCBRCalibrate(double dPlatformPos1X, double dPlatformPos1Y, double dPlatformPos2X, double dPlatformPos2Y);


	// 获取目标指定位置的图像坐标
	BOOL OnlineGetTargetImagePosFTGG(int nPosIndex);
	// 报告目标指定位置的图像坐标
	BOOL ReportGetTargetImagePosFTGG(BOOL bResult, double dImagePosX = 0, double dImagePosY = 0, double dImagePosD = 0);

	// 获取对象指定位置的图像坐标
	BOOL OnlineGetObjectImagePosFOBG(int nPosIndex);
	// 报告对象指定位置的图像坐标
	BOOL ReportGetObjectImagePosFOBG(BOOL bResult, double dImagePosX = 0, double dImagePosY = 0, double dImagePosD = 0);

	//功能：对象拍照或者目标拍照之后，执行对位计算
	//参数：vnPosIndex:成功模板序号，nObject：1是对象，2是目标
	BOOL ExecuteAlignAfterFOBCAndFTGC(std::vector<int> &vnPosIndex,int nObject = 1);

	//功能：对象拍照或者目标拍照之后，执行对位计算
	//参数：nObject：1是对象，2是目标
	BOOL ExecuteAlignAfterFOBJAndFTGT(int nObject);
	// 对象拍照或者目标拍照之后执行对位计算，后答复PLC的数据
	BOOL ReportObjectSearchResultFOBCALGOrACO( BOOL bResult, int nSuccessPatIndex,CPlatformAxisPos* vpPlatAxisPos);
	BOOL ReportObjectSearchResultFOBJALGOrACO( BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	// 目标拍照之后执行对位计算，后答复PLC的数据
	BOOL ReportTargetSearchResultFTGCALGOrACO( BOOL bResult, int nSuccessPatIndex,CPlatformAxisPos* vpPlatAxisPos);
	BOOL ReportTargetSearchResultFTGTALGOrACO( BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);

	// 通信握手
	virtual BOOL  CommShakeHands();
	// 发送写PLC寄存器握手字符串，并接收解析PLC的响应字符串是否正确
	BOOL CommunicateSendShakeHandsStr(CString strSend, CString& strInfo, CommStatus& nCommStatus);

//	virtual BOOL BendingProductID(CString& strProductID);	//读取二维码
	BOOL CommunicateGetAlignProcessProductID(CString& strProductID);

	BOOL CommunicateGetAlignProcessProductSTATION(CString& strProductSTATION,CString& strStartIime,CString& strEndIime,CString& strProsessingIime,CString& strInspectStation); //读取对应的工位位置

protected:
	// 启用基恩士指令码时的工作线程
	static UINT WorkThreadEx(LPVOID pParam);      // 工作线程
public:
	// 接受并解析指令
	BOOL ReceiveCommPlcRegEx(SysWorkCmdType& sysWorkCmdType); // 接受指令

	// 多段触发
	BOOL ReceiveCommPlcRegEx2(SysWorkCmdType& sysWorkCmdType); // 接受指令

	// 将PLC数据地址位+nOffSet 位置数据置0； 用于指令触发位置置0和标定到位信号置0 
	BOOL ClearPlcPlatformTriggerEx(int nOffSet=0);																	// 将对象平台触发寄存器置0
	// 多位置同时标定具体实现函数：逻辑为PLC只发一次标定指令，中间多次发送到位信号：与视觉现有逻辑一致																// 将对象平台触发寄存器置1
	BOOL MulPosCameraCalibrateFCLB(std::vector<int> vnPosIndex, int nMarkType);				// 多位置同时标定函数
	// 标定函数
	BOOL OnlineCalibrateFCLB_Ex(int nMarkType);
	
	// 回复PLC视觉过程的结果（拍照OK/NG/继续、标定OK/NG/继续等待） 向D2 +nOffset中写nResult多种状态 
	BOOL ReportPLCFunc(int nResult,int nOffset=0); 
	BOOL ReportPLCWithDetectionResultFunc(int nResult,vector<double> DetectionResult,int nOffset=0);
	BOOL ReportPLCWithDetectionResultFunc2(int nResult,vector<int> DetectionResult,int nOffset=0);

	// 获取平台各轴的绝对位置 读取D60-64;
	BOOL CommuciateGetPlatformAxisAbsPosEx(CPlatformXYDAxisPos* pPlatformXYDAxisPos);
	// 驱动轴到位，并等待到位信号 将驱动轴相对移动量写入D10-14
	BOOL CommuciateDrivePlatformAxisMoveEx( CPlatformXYDAxisPos* pPlatformXYDAxisPos,CPlatformXYDAxisPos* pPlatformXYDAxisPosAbsBase);

	// 标定函数2
	BOOL OnlineCalibrateFCLB_Ex2(int nMarkType,int nExProductIndex=0);
	// 多位置同时标定具体实现函数2：按照流程图实现，需要多次标定指令；优点是一次失败，再次发送标定指令可从失败位置继续标定；
	BOOL MulPosCameraCalibrateFCLB_Ex(std::vector<int> vnPosIndex, int nMarkType,int nExProductIndex=0);	// 多位置同时标定函数 根据基恩士指令码逻辑
	// 当前标定进行到的位置；例如九宫格+旋转的12个位置，当前进行到第2个位置，m_nCurCalibrateIndex=2
	int m_nCurCalibrateIndex;	
	
	// 执行一次对位过程并给出相对偏差量
	BOOL OnlineAlignExFACO_Ex(int nOffsetIndex,int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0 );

	// 循环定位过程
	BOOL OnlineAlignExFACO_WithSnap_Ex(int nOffsetIndex);
	// 回复PLC相对偏差量   输入参数：platOffset：相对偏差量；nResult：对位状态OK/NG/继续；pPlatformXYDAxisPos：绝对轴位置；nOffset：写入定位状态地址偏差量
	BOOL ReportPLC_Offset(CCoordPos platOffset,int nResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL,int nOffset = 0);

	// 回复PLC相对偏差量   输入参数：platOffset：相对偏差量；nResult：对位状态OK/NG/继续；pPlatformXYDAxisPos：绝对轴位置；nOffset：写入定位状态地址偏差量
	BOOL ReportPLC_OffsetAndBenchCenter(CCoordPos platOffset,int nResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos = NULL,CPlatformAxisPos* vpPlatAxisPos = NULL,int nOffset = 0);

	// 回复PLC检测结果   输入参数：nResult：对位次数；BendInspectResult：检测数据结构体
	BOOL ReportPLC_Inspect(int nResult,BendInspectResultInfo* BendInspectResult = NULL);
	// 将 轴数据写入寄存器D10 +nOffSet
	BOOL ReportPLC_WriteAbsPosEx(CPlatformAxisPos* vpPlatAxisPos,int nOffSet=0);

	BOOL OnObjectSearchCandidateExFOBC_Ex(std::vector<int> vnPosIndex, int nStartPatIndex,BOOL* pbIsSizeNG = NULL);
	BOOL OnTargetSearchCandidateExFTGC_Ex(std::vector<int> vnPosIndex, int nStartPatIndex);
	int m_nCurAlignerTimesIndex;
	CString m_bALLBendRecord;

	// 发送通信连接状态函数； 向D0中循环3s每次写入0和1；
	BOOL SendCommConnectStatusEx(KeyenceCodeType eCode);//写寄存器 通信心跳，限寄存器通信使用

	// 额外增加;多位置对象候补拍照 需要发送额外参数
	BOOL OnlineAlnObjectSearchCandidateExFOBC_Ex(int nPosIndex, int nStartPatIndex);
	BOOL OnSerialCameraAlignerObjectSearchCandidateExFOBC_Ex2(std::vector<int> vnPosIndex, int nStartPatIndex);

	// 额外增加;多位置对象候补拍照 需要发送额外参数
	BOOL OnlineAlnTargetSearchCandidateExFTGC_Ex(int nPosIndex, int nStartPatIndex);
	BOOL OnSerialCameraAlignerTargetSearchCandidateExFTGC_Ex2(std::vector<int> vnPosIndex, int nStartPatIndex);

	// 额外增加;开通系统设置功能
	BOOL OnlineAlnTargetSearchCandidateExFPMI_Ex(int nPosIndex);

	
	// 额外增加;切换产品 需要发送额外参数
	virtual BOOL	OnlineSetProductIndexEx(int nCurProductIndex);			// 根据索引指令设置当前产品

	// 额外增加;检测 需要发送额外参数
	BOOL OnlineACFInspectCandidateExFCHB_Ex(int nPosIndex, int nStartPatIndex);
	BOOL OnSerialCameraACFInspectCandidateExFCHB_Ex(std::vector<int> vnPosIndex, int nStartPatIndex);

	// 额外增加;检测 需要发送额外参数
	BOOL OnlineACFInspectCandidateExFBIS_Ex(int nPosIndex, int nStartPatIndex,int nTarFixOffsetIndex  = 0);

	BOOL ReportPLC_FALP(int nResult, CPlatformXYDAxisPos* pPlatformXYDAxisPos,int nOffset = 0);
	BOOL OnlineAlignExFALP_Ex(int nOffsetIndex);

	BOOL OnlineAlignExFAAC_Ex(int nOffsetIndex, int nStartPatIndex,int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex  = 0);
	double m_dTotalAlignTime;

	BOOL ExecuteAlignAfterFOBCAndFTGC_Ex(std::vector<int>& vnPosIndex,int nObject);

	BOOL ReportMulAlignPosResult_Ex(std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset,int nResult);
	BOOL OnlineAlignExFMCO_Ex(int nOffsetCount);

	BOOL ReportMulAlignPosResultFALP_Ex(std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos,int nResult);

	BOOL OnlineMulAlignPickExFALP_Ex(int nOffsetIndex);

	BOOL OnlineFCBPCalibrate_Ex(int nCalibPos, int nMarkType,int nExProductIndex=0);

	BOOL OnlineCalibrateFCBS_Ex(int nMarkType);
	BOOL MulCameraCalibrateFCBS_Ex(std::vector<int> vnPosIndex, int nMarkType);
	int m_nCurFCBS_XY_Index;
	int m_nCurFCBS_Angle_Index;

	

	// 相同指令触发-指令参数调整索引
	int m_nCurFOBC_Index; //当前对象拍照指令的流程索引
	int m_nCurFTGC_Index; //当前对象拍照指令的流程索引
	int m_nCurFCBP_Index; //当前关联指令的流程索引
	int m_nCurFCBS_Index; //当前关联指令的流程索引
	int m_nCurFCHB_Index; //当前检测拍照指令的流程索引
	int m_nCurFCLB_Code_Index;//当前标定指令的流程索引
	int m_nCurFBIS_Index; //当前分步检测指令的流程索引

	int m_FPMI;
	int m_FPMIMIN;
	//scTimer stGlobalSearchTimer;	// 全局搜索时间
	
	

	BOOL OnlineAlignExFAAL_Base_Ex(int nOffsetIndex, int nReservedIndex= 10, int nBenchOffsetIndex = -1, int nExProductIndex= 0,int nTarFixOffsetIndex = 0);
	BOOL OnlineAlignExFAAL_Ex(int nOffsetIndex, int nReservedIndex = 10, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0);
	BOOL OnlineAlignExFAAL_BendAlign_Ex(int nOffsetIndex, int nReservedIndex = 10, int nBenchOffsetIndex = -1, int nExProductIndex = 0,int nTarFixOffsetIndex = 0);

	BOOL OnSerialCameraACFInspectCandidateExFCHB_Ex(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex);
	BOOL OnSerialCameraACFInspectCandidateExFCHB_Ex2(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex);
	BOOL OnSerialCameraACFInspectCandidateExFCHB_Ex3(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex);  //允许候补功能

	BOOL OnSerialCameraACFInspectCandidateExFBIS_Ex2(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex,int nTarFixOffsetIndex = 0);
	BOOL OnSerialCameraACFInspectCandidateExFBIS_Ex3(std::vector<int> vnPosIndex, std::vector<int> vnPatIndex,int nTarFixOffsetIndex = 0); //允许候补功能

	BOOL ReportPLC_RowCols(int nResult,int nSize,std::vector<int> vnRows,std::vector<int> vnCols,int nOffset = 0);

	BOOL ResetAllIndex();
	BOOL ReportMulAlignPosResultFALP_Ex2(std::vector<double> vdDate,int nResult,int nSearchNumStatus);

	// CSV保存m_strProductSTATION
	BOOL SaveResultToCsv(CString strSNCode, BOOL bSearchSucceed, vector< double> vdDetectionResult, BOOL bIsBending);
	BOOL SaveResultToCsv(CString strSNCode,CString strSTATIONCode,CString strInspectStation,BOOL bSearchSucceed, vector< double> vdDetectionResult, BOOL bIsBending);
	//所有数据文件存储到同一路径，用于远程传输
	BOOL SaveResultToCsvFixed(CString strSNCode,CString strSTATIONCode,CString strInspectStation,CString strStartIime,CString strEndIime,CString strProsessingIime, BOOL bSearchSucceed, vector< double> vdDetectionResult, BOOL bIsBending);

	//流程控制光源亮灭
	BOOL OnLightOnOff(int nCurCmd);

	//折弯压合参数实时写入
	BOOL OnBendPressParaWrite();
};


#endif // !defined(AFX_SVXYDVISIONAILGNER_H__B97ED5A8_953A_4190_90BC_73B019D5D2AD__INCLUDED_)
