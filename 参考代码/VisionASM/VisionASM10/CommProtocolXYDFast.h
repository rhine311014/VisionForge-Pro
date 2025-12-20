// CommProtocolXYDFast.h: interface for the CCommProtocolXYD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMPROTOCOLXYDFAST_H__55A666F4_9BA1_4092_8104_9169103F7805__INCLUDED_)
#define AFX_COMMPROTOCOLXYDFAST_H__55A666F4_9BA1_4092_8104_9169103F7805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommProtocolProcess.h"

#include "RegisterAddressDef.h"


//////////////////////////////////////////////////////////////////////////
// 产品
// 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
// 2、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
// 3、 FGSP：产品No.的取得，取得目前正在执行的产品No.


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Mark：标定
// 1、FCLB:Calibration 的执行 执行Calibration

//////////////////////////////////////////////////////////////////////////
// Mark:目标Mark
// 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
// 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
// 3、FTGR:目标 Mark 位置的登录，将目前使用的产品数据保存在指定的产品No.中并保存在CF 记忆卡
// 4、FTGG:目标 Mark 画素的取得，取得执行“ＦＴＧＴ”结果的画素
// 5、FTGD:目标 Mark 指示线的全画面显示，在指定目标 Mark No.的目标 Mark 位置上显示全画面的指示线及刻度

//////////////////////////////////////////////////////////////////////////
// Mark:物体Mark
// 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
// 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
// 3、FOBG:物体 Mark 画素的取得，取得执行“ＦＯＢＪ”结果的画素
// 4、FMSC:手动搜索位置的取得，在自动对准时，当目标 Mark 或物体 Mark 搜索失败时，可利用手动方法（鼠标）指定Mark 位置
// 5、FWLD：取得目标（目标靶标）或对象（对象靶标）的平台坐标上的位置
// 

//////////////////////////////////////////////////////////////////////////
// 对准
// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
// 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
// 3、FGAC:对准最多次数的取得，取得设定对准最多次数
// 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
// 5、FAAL:自动对准的执行，执行自动对准（连续对准）
// 5、FAAB:自动对准的执行，执行自动对准（连续对准），增加图像十字中心基准对位过程
// 5.1、FAAC:自动对准的执行，执行自动对准（连续对准）,对象候补搜索
// 5.2、FAHL:Align high vs low layer背光贴片机自动对准的执行，执行自动对准（连续对准）
// 6、FANS:对准后的偏移量的取得，取得对准后平台坐标系的偏移量
// 7、FALG:１次对准移动量的取得，取得１次对准移动量
// 8、FALJ:1:次对准精度判定与对准移动量的取得，对准移动量是否在对准精度内的判定与对准移动量的取得
// 9、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
// 10、FABO:偏移量的取得，取得目标图像十字中心及物体 Mark 之间中心的平台坐标系偏移量

// 
//////////////////////////////////////////////////////////////////////////
// 补偿量・移动
// 1、FAOF:自动补偿量值的取得，对准偏移量以补偿量值取得
// 2、FGOF:补偿量值的取得，取得设定的补偿量值
// 3、FOFS:补偿量值的设定，设定补偿量值
// 4、FSFT:平台Shift 移动动作，选择内部转移量的设定值，并依照设定值将目前平台的位置做相对移动
// 5、FGSF:对准Shift 移动量的取得，取得对准转移量
// 6、MNPS:平台现在位置的取得，取得平台现在位置
// 7、MMVA:平台絶对准置移动量的设定，设定平台絶对准置移动量
// 8、MMVR:平台相对准置移动量的设定，设定平台相对准置移动量

//////////////////////////////////////////////////////////////////////////
// Alarm
// 1、FALM:Alarm资料的取得，取得错误发生的数据代码
// 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
// 3、FSAC：保存校正、定位时的数据。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
// 4、FSCO：保存指令收发的历史纪录。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）




//////////////////////////////////////////////////////////////////////////
// 产品
// 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
// 参　数	
// 		Ｐ1	１～２５０：类型编号
// 		Ｐ2	１：辅助编号：通常设定为“1”。
// 回　复
// 	   Ｒ１	－１：异常停止 ０：正常停止

// 发送：（[STX]FSPC P1 P2[ETX]）
// 回复：（[STX]FSPC R1[ETX]）
typedef struct tagFSPCParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// １～２５０：类型编号
	int P2;			// １：辅助编号：通常设定为“1”
	tagFSPCParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFSPC;
		m_nCmdVal = eCmdValSetProductIndexFSPC;
//		m_nVCmdValue = 4;
		m_nVCmdValue = eVCmdFSPC;
		m_nPNum = 2;
		P1=1;
		P2=1;
	}
}FSPCParam;


//lzk 0408
typedef struct tagFVPCParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FVPC读写寄存器
	int m_nPNum;
	int P1;			// 0~4： 对象虚拟位置模式
	int P2;			// 0~4： 目标虚拟位置模式
	tagFVPCParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFVPC;
		m_nCmdVal = eCmdValFVPC;
		m_nVCmdValue = eVCmdFVPC;
		m_nPNum = 2;
		P1=1;
		P2=1;
	}
}FVPCParam;


// 2、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
// 参　数	
// 		Ｐ1	０：当前使用中的类型	１～２５０：类型编号
// 		Ｐ2	１：辅助编号：通常设定为“1”。
// 回　复
// 		Ｒ１	－１：异常停止０：正常停止

// 发送：（[STX]FSSV P1 P2[ETX]）
// 回复：（[STX]FSSV R1[ETX]）
typedef struct tagFSSVParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０：当前使用中的类型	１～２５０：类型编号
	int P2;			// １：辅助编号：通常设定为“1”
	tagFSSVParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFSSV;
		m_nCmdVal = eCmdValSaveProductIndexFSSV;
		m_nVCmdValue = eVCmdFSSV;
		m_nPNum = 2;
		P1=1;
		P2=1;
	}
}FSSVParam;




// 3、 FGSP：产品No.的取得，取得目前正在执行的产品No.
// 参 数
// 回 复	
// 		 Ｒ１	－１：异常停止　０：正常停止
// 		 Ｒ２	１～２５０：类型编号　
// 		 Ｒ３	１：辅助编号(通常返回“1”）

// 发送：（[STX]FGSP[ETX]）
// 回复：（[STX]FGSP R1 R2 R3[ETX]）
typedef struct tagFGSPParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	// 	int P1;			// ０：当前使用中的类型	１～２５０：类型编号
	// 	int P2;			// １：辅助编号：通常设定为“1”
	tagFGSPParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGSP;
		m_nCmdVal = eCmdValGetProductIndexFGSP;
//		m_nVCmdValue = 20;
		m_nVCmdValue = eVCmdFGSP;
		m_nPNum = 0;
		// 		P1=1;
		// 		P2=1;
	}
}FGSPParam;

// 发送：（[STX]FVAS P1[ETX]）
// 回复：（[STX]FVAS R1[ETX]）
typedef struct tagFVASParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// 0~1：0为关闭，1 为开启

	tagFVASParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFVAS;
		m_nCmdVal = eCmdValFVAS;
		m_nVCmdValue = eVCmdFVAS;
		m_nPNum = 1;
		P1=0;
	}
}FVASParam;


//////////////////////////////////////////////////////////////////////////
// Mark：标定
// 1、FCLB:Calibration的执行，执行Calibration
// 参　数	
// 		Ｐ1	０：目标 靶标 	１：对象　靶标（通常设定为“1”。不支持“0”）
// 		Ｐ2	１～４：靶标№
// 回 复
// 		Ｒ１	－１：异常停止 ０：正常停止

// 发送：（[STX]FCLB P1 P2[ETX]）
// 回复：（[STX]FCLB R1[ETX]）

//标定命令参数
typedef struct tagFCLBParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 0:目标mark 1: 物体mark(通常设置为"1",不支持"0")
	int P2;			// 1~4: Mark No  单相机:在第N个位置做标定 多相机：第N个相机做标定
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置
	tagFCLBParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCLB;
		m_nCmdVal = eCmdValCalibrateFCLB;
//		m_nVCmdValue = 5;
		m_nVCmdValue = eVCmdFCLB;
		m_nPNum = 5;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}
}FCLBParam;

//标定命令参数
typedef struct tagFCBSParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 0:目标mark 1: 物体mark(通常设置为"1",不支持"0")
	int P2;			// 1~4: Mark No  单相机:在第N个位置做标定 多相机：第N个相机做标定
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置
	tagFCBSParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCBS;
		m_nCmdVal = eCmdValCalibrateFCBS;
//		m_nVCmdValue = 21;
		m_nVCmdValue = eVCmdFCBS;
		m_nPNum = 5;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;

	}
}FCBSParam;
// 标定命令参数
typedef struct tagCalibParam
{
	int P1;			// 0:目标mark 1: 物体mark(通常设置为"1",不支持"0")
	int P2;			// 1~4: Mark No  单相机:在第N个位置做标定 多相机：第N个相机做标定

	tagCalibParam()
	{
		P1=1;
		P2=1;
	}

}CalibParam;


// FCBR标定命令参数
typedef struct tagFCBRParam
{
	CString m_strCmd;		// 指令字符串
	int		m_nCmdVal;		// 指令整数值
	int     m_nVCmdValue;	// FAST读写寄存器
	int		m_nPNum;		// 指令参数数量
	int		m_nPType;		// 指令参数类型（即实际参数的个数数量）
	double	P1;				// MARK1平台坐标X
	double	P2;				// MARK1平台坐标Y
	double	P3;				// MARK2平台坐标X
	double	P4;				// MARK2平台坐标Y

	tagFCBRParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCBR;
		m_nCmdVal = eCmdValCalibrateFCBR;
		m_nVCmdValue = eVCmdFCBR;
		m_nPNum = 4;
		P1=0;
		P2=0;
		P3=0;
		P4=0;
	}
}FCBRParam;


//相机轴标定命令参数
typedef struct tagFCPRParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int		m_nPNum;
	int		m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int		P1;			// 1~8位置编号
	int		P2;			// 操作编码，0为记录基准位置；1为标定计算；2为记录当前位置
	double P3;		// X轴位置
	double P4;		// Y轴位置

	tagFCPRParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCPR;
		m_nCmdVal = eCmdValCalibrateFCPR;
		m_nVCmdValue = eVCmdFCPR;
		m_nPNum = 2;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0.0;
		P4=0.0;
	}
}FCPRParam;
//自动关联命令参数
typedef struct tagFCBPParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int		m_nPNum;
	int		m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int		P1;			// 操作编码，0为搜索二维码；1为获取轴基准位置；2为关联计算
	int		P2;			// 1~8位置编号
	int     P3;         // 关联移动位置序号
	int     nP4;         // 产品扩展序号
	double P4;		// 轴1位置
	double P5;		// 轴2位置
	double P6;		// 轴3位置

	tagFCBPParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCBP;
		m_nCmdVal = eCmdValFCBP;
		m_nVCmdValue = eVCmdFCBP;
		m_nPNum = 3;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		nP4=0;
		P4=0.0;
		P5=0.0;
		P6=0.0;
	}
}FCBPParam;
//////////////////////////////////////////////////////////////////////////
// Mark:目标Mark
// 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
// 参　数	
// 			Ｐ1	    １～４  ：靶标№
// 	　　			    ５  ：靶标连续检出
// 		   　		８  ：提取目标靶标位置
// 					９  ：保存目标靶标位置
// 回 复
// 		    Ｒ１	－１：异常停止  ０：正常停止

// 发送：（[STX]FTGT P1[ETX]）
// 回复：（[STX]FTGT R1[ETX]）

typedef struct tagFTGTParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 1~4: Mark no 5：Mark连续搜索
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	int nP2;		// 是否检测或多目标对位序号
	int nP3;		// 扩展产品序号,有效范围1到n+1, n为扩展数目
	int nP4;        // 是否拍照后对位
	tagFTGTParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFTGT;
		m_nCmdVal = eCmdValTargetSearchFTGT;
//		m_nVCmdValue = 6;
		m_nVCmdValue = eVCmdFTGT;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
		nP2 = 0;	
		nP3 = 0;
		nP4 = 0;
	}

}FTGTParam;

//////////////////////////////////////////////////////////////////////////
// Mark:目标Mark
// 1、FTGM:目标 Mark 位置的手动取得，取得目标 Mark（目标）位置
// 参　数	
// 			Ｐ1	    １～４  ：靶标№
// 	　　			    ５  ：靶标连续检出
// 		   　		８  ：提取目标靶标位置
// 					９  ：保存目标靶标位置
// 回 复
// 		    Ｒ１	－１：异常停止  ０：正常停止

// 发送：（[STX]FTGT P1[ETX]）
// 回复：（[STX]FTGT R1[ETX]）

typedef struct tagFTGMParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 1~4: Mark no 5：Mark连续搜索
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	tagFTGMParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFTGM;
		m_nCmdVal = eCmdValTargetSearchFTGM;
		m_nVCmdValue = eVCmdFTGM;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
	}

}FTGMParam;

//////////////////////////////////////////////////////////////////////////
// Mark:目标Mark
// 1、FTGS:目标 Mark 位置的取得，取得目标 Mark（目标）位置，目标平台轴位置一致
// 参　数	
// 			Ｐ1	    １～４  ：靶标№
// 	　　			    ５  ：靶标连续检出
// 		   　		８  ：提取目标靶标位置
// 					９  ：保存目标靶标位置
// 回 复
// 		    Ｒ１	－１：异常停止  ０：正常停止

// 发送：（[STX]FTGS P1[ETX]）
// 回复：（[STX]FTGS R1[ETX]）


typedef struct tagFTGSParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 1~4: Mark no 5：Mark连续搜索
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	tagFTGSParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFTGS;
		m_nCmdVal = eCmdValTargetSearchFTGS;
		m_nVCmdValue = eVCmdFTGS;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
	}

}FTGSParam;


// 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
// 参　数	
// 			Ｐ1	１～４：靶标№
// 　				５：连续搜索靶标
// 			Ｐ２	１～５：候补检索开始的编号
// 				１０：从上次成功的候补编号开始
//  回 复
// 			Ｒ１		＊目标成对进行候补检索
// 						1～５：正常停止后的候补检索编号
//  　					－1：异常停止
// 			Ｒ１		＊独立进行目标的候补检索时
//   　					０：正常停止
// 						－１：异常停止

// 发送：（[STX]FTGC P1 P2[ETX]）
// 回复：（[STX]FTGC R1[ETX]）

typedef struct tagFTGCParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 1~4: Mark no 5：Mark连续搜索
	int P2;			// 1~5: 开始候补搜索号码
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置
	int nP3;		// 是否检测或多目标对位序号
	int nP4;		// 扩展产品序号,有效范围1到n+1, n为扩展数目
	int nP5;        // 是否拍照后对位
	tagFTGCParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFTGC;
		m_nCmdVal = eCmdValTargetSearchFTGC;
//		m_nVCmdValue = 17;
		m_nVCmdValue = eVCmdFTGC;
		m_nPNum = 5;
		m_nPType = -1;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
		nP3 = 0;
		nP4 = 0;
		nP5 = 0;
	}
}FTGCParam;


typedef struct tagTargetSearchParam
{
	int P1;			// 1~4: Mark no 5：Mark连续搜索
	int P2;			// 1~5: 开始候补搜索号码

	tagTargetSearchParam()
	{
		P1=1;
		P2=1;
	}

}TargetSearchParam;



// 3、FTGR:目标 Mark 位置的登录，将目前使用的产品数据保存在指定的产品No.中并保存在CF 记忆卡
// 参数
// 		Ｐ１ １～４：Mark No.
// 		Ｐ２ ０～５１１００：Ｘ的画素（１／１００画素单位）
// 		Ｐ３ ０～４７９００：Ｙ的画素（１／１００画素单位）
// 回复 
// 		Ｒ１  －１：异常结束 ０：正常结束

// 发送：（[STX]FTGR P1 P2 P3[ETX]）
// 回复：（[STX]FTGR R1[ETX]）
typedef struct tagFTGRParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// 1~4: Mark no
	double P2;			// ０～５１１００：Ｘ的画素（１／１００画素单位）
	double P3;			// ０～４７９００：Ｙ的画素（１／１００画素单位）
	tagFTGRParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFTGR;
		m_nCmdVal = eCmdValSaveTargetPosFTGR;
		m_nVCmdValue = eVCmdFTGR;
		m_nPNum = 3;
		P1=1;
		P2=0;
		P3=0;
	}
}FTGRParam;


// 4、FTGG:目标 Mark 画素的取得，取得执行“ＦＴＧＴ”结果的画素
// 参数 
// 		Ｐ１ １～４：Mark No.
// 回复 
// 		Ｒ１ －１：异常结束 ０：正常结束
// 		Ｒ２ Ｘ的画素（１００倍值）
// 		Ｒ３ Ｙ的画素（１００倍值）

// 发送：（[STX]FTGG P1[ETX]）
// 回复：（[STX]FTGG R1 R2 R3[ETX]）
typedef struct tagFTGGParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// 1~4: Mark no
	// 	int P2;			// ０～５１１００：Ｘ的画素（１／１００画素单位）
	// 	int P3;			// ０～４７９００：Ｙ的画素（１／１００画素单位）
	tagFTGGParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFTGG;
		m_nCmdVal = eCmdValGetTargetImagePosFTGG;
		m_nVCmdValue = eVCmdFTGG;
		m_nPNum = 1;
		P1=1;
		// 		P2=1;
		// 		P3=1;
	}
}FTGGParam;

// 发送：（[STX]FGAP P1[ETX]）
// 回复：（[STX]FGAP R1 R2 R3[ETX]）
typedef struct tagFGAPParam
{
	CString m_strCmd;
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			 // 1~4: Mark no
	int P2;          // 1~5: Pat no
	int P3;
	int P4;
	int P5;
	tagFGAPParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGAP;//_T("FGAP");
		m_nCmdVal= eCmdValGetImagePosFGAP;
		//		m_nVCmdValue = 16;
		m_nVCmdValue = eVCmdFGAP;
		m_nPNum = 5;
		m_nPType = -1;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}
}FGAPParam;


// 发送：（[STX]FGTC P1[ETX]）
// 回复：（[STX]FGTC R1 R2 R3[ETX]）
// 获取目标相对于图像中心的图像坐标偏差
typedef struct tagFGTCParam
{
	CString m_strCmd;
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			 // 1~4: Mark no
	int P2;          // 1~5: Pat no
	int P3;
	int P4;
	int P5;
	tagFGTCParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGTC;//_T("FGTC");
		m_nCmdVal= eCmdValGetImagePosFGTC;
		m_nVCmdValue = eVCmdFGTC;
		m_nPNum = 5;
		m_nPType = -1;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}
}FGTCParam;


// 5、FTGD:目标 Mark 指示线的全画面显示，在指定目标 Mark No.的目标 Mark 位置上显示全画面的指示线及刻度
// 参数
// 		Ｐ１  ０ ：在产品设定里所设定的显示方法
// 			  １ ～ ４：Mark No.
// 回复 
// 		Ｒ１ －１：异常结束(0)* ０：正常结束 (255)*

// 发送：（[STX]FTGD P1[ETX]）
// 回复：（[STX]FTGD R1[ETX]）
typedef struct tagFTGDParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０ ：在产品设定里所设定的显示方法，１ ～ ４：Mark No.
	tagFTGDParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFTGD;
		m_nCmdVal = eCmdValDisplayTargetFTGD;
		m_nVCmdValue = eVCmdFTGD;
		m_nPNum =1;
		P1=1;
	}
}FTGDParam;


//////////////////////////////////////////////////////////////////////////
// Mark:物体Mark
// 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
// 参　数	
// 		Ｐ1	１～４  ：靶标№
// 	　　		５  ：连续搜索靶标
// 			11～14：手动搜索靶标1－靶标4
// 			15  ：连续手动搜索靶标
// 回 复
// 		Ｒ１	－１：异常停止  ０：正常停止	  １：取消停止

// 发送：（[STX]FOBJ P1[ETX]）
// 回复：（[STX]FOBJ R1[ETX]）


typedef struct tagFOBJParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 1~4: Mark no 5：Mark连续搜索
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	int nP2;		// 是否检测
	int nP3;		// 扩展产品序号,有效范围1到n+1, n为扩展数目
	int nP4;        // 是否拍照后对位
	tagFOBJParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFOBJ;
		m_nCmdVal = eCmdValObjectSearchFOBJ;
//		m_nVCmdValue = 7;
		m_nVCmdValue = eVCmdFOBJ;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
		nP2 = 0;
		nP3 = 0;
		nP4 = 0;

	}

}FOBJParam;

//////////////////////////////////////////////////////////////////////////
// Mark:物体Mark
// 1、FCNT:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置(目标位置取得为图像中心)
// 参　数	
// 		Ｐ1	１～４  ：靶标№
// 	　　		５  ：连续搜索靶标
// 			11～14：手动搜索靶标1－靶标4
// 			15  ：连续手动搜索靶标
// 回 复
// 		Ｒ１	－１：异常停止  ０：正常停止	  １：取消停止

// 发送：（[STX]FCNT P1[ETX]）
// 回复：（[STX]FCNT R1[ETX]）
typedef struct tagFCNTParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 1~4: Mark no 5：Mark连续搜索
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	tagFCNTParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCNT;
		m_nCmdVal = eCmdValObjectSearchFCNT;
//		m_nVCmdValue = 15;
		m_nVCmdValue = eVCmdFCNT;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
	}

}FCNTParam;


// 发送：（[STX]FCNO P1[ETX]）
// 回复：（[STX]FCNO R1[ETX]）
typedef struct tagFCNOParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 1~4: Mark no 5：Mark连续搜索
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	tagFCNOParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCNO;
		m_nCmdVal = eCmdValTargetSearchFCNO;
		m_nVCmdValue = eVCmdFCNO;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
	}

}FCNOParam;

//////////////////////////////////////////////////////////////////////////
// Mark:物体Mark
// 1、FOBM:物体 Mark 位置的手动取得，取得物体 Mark（对准靶标）位置
// 参　数	
// 		Ｐ1	１～４  ：靶标№
// 	　　		５  ：连续搜索靶标
// 			11～14：手动搜索靶标1－靶标4
// 			15  ：连续手动搜索靶标
// 回 复
// 		Ｒ１	－１：异常停止  ０：正常停止	  １：取消停止

// 发送：（[STX]FOBJ P1[ETX]）
// 回复：（[STX]FOBJ R1[ETX]）


typedef struct tagFOBMParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 1~4: Mark no 5：Mark连续搜索
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	tagFOBMParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFOBM;
		m_nCmdVal = eCmdValObjectSearchFOBM;
		m_nVCmdValue = eVCmdFOBM;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
	}

}FOBMParam;

// 
typedef struct tagObjectSearchParam
{
	int P1;			// 1~4: Mark no 5：Mark连续搜索
	int P2;			// 1~5: 开始候补搜索号码

	tagObjectSearchParam()
	{
		P1=1;
		P2=1;
	}

}ObjectSearchParam;

// 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
// 参　数	
// 		Ｐ1	１～４：靶标№
// 　		５：连续进行靶标检出
// 		Ｐ２	１～５：候补搜索开始的编号
// 			１０：从上次成功的候补编号开始
//  回 复
// 		Ｒ１	＊成对进行目标靶标候补搜索时
// 				1～５：正常停止的候补搜索号码
//  　			－1：异常停止
// 		Ｒ１	＊单独进行目标　靶标候补搜索时
// 				０：正常停止
// 				－１：异常停止

// 发送：（[STX]FOBC P1 P2[ETX]）
// 回复：（[STX]FOBC R1[ETX]）
typedef struct tagFOBCParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 1~4: Mark no 5：Mark连续搜索
	int P2;				// 1~5: 开始候补搜索号码
	int nP3;            // 是否检测标志
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置

	int nP4;		// 扩展产品序号,有效范围1到n+1, n为扩展数目
	int nP5;		// 是否拍照后对位

	tagFOBCParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFOBC;
		m_nCmdVal = eCmdValObjectSearchFOBC;
//		m_nVCmdValue = 18;
		m_nVCmdValue = eVCmdFOBC;
		m_nPNum = 6;
		P1=1;
		P2=1;
		nP3=0;
		P3=0;
		P4=0;
		P5=0;
		nP4 = 0;
		nP5 = 0;
	}

}FOBCParam;


// 发送：（[STX]FPMI P1 [ETX]）
// 回复：（[STX]FOBC R1[ETX]）
typedef struct tagFPMIParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 权限有效时间


	tagFPMIParam()
	{
		
		m_strCmd = COMM_CMD_STRING_VISIONFPMI;
		m_nCmdVal = eCmdValFPMI;
		//		m_nVCmdValue = 18;
		m_nVCmdValue = eCmdFPMI;
		m_nPNum = 1;
		P1=1200;
	}

}FPMIParam;


// 发送：（[STX]FPMI P1 [ETX]）
// 回复：（[STX]FOBC R1[ETX]）
typedef struct tagFBISParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 对象，目标，检测位置
	int P2;				// 候补搜索开始的编号
	int P3;				// 用于记录显示来料平台的编号
	int P4;				// 用于不同标定信息进行检测


	tagFBISParam()
	{

		m_strCmd = COMM_CMD_STRING_VISIONFBIS;
		m_nCmdVal = eCmdValFBIS;
		//		m_nVCmdValue = 18;
		m_nVCmdValue = eCmdFBIS;
		m_nPNum = 4;
		P1=3;
		P2=1;
		P3=0;
		P4=0;

	}

}FBISParam;


// 2、FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
// 参　数	
// 		Ｐ1	１～４：靶标№
// 　		５：连续进行靶标检出
// 		Ｐ２	１～５：候补搜索开始的编号
// 			１０：从上次成功的候补编号开始
//  回 复
// 		Ｒ１	０：正常停止
// 				－１：异常停止
//		Ｒ2		X 平台坐标位置X
//		Ｒ3		Y 平台坐标位置Y

// 发送：（[STX]FGOW P1 P2[ETX]）
// 回复：（[STX]FGOW R1 R2 R3[ETX]）
typedef struct tagFGOWParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 1~4: Mark no 5：Mark连续搜索
	int P2;				// 1~5: 开始候补搜索号码
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置

	tagFGOWParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGOW;
		m_nCmdVal = eCmdValFGOW;
		//		m_nVCmdValue = 18;
		m_nVCmdValue = eVCmdFGOW;
		m_nPNum = 5;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}

}FGOWParam;

// 3、FOBG:物体 Mark 画素的取得，取得执行“ＦＯＢＪ”结果的画素
// 参数
// 		Ｐ１１～４：Mark No.
// 			５：Mark 连续搜索
// 回复		
// 		Ｒ１ －１：异常结束 ０：正常结束
// 		Ｒ２ Ｘ的画素（１００倍值）
// 		Ｒ３ Ｙ的画素（１００倍值）

// 发送：（[STX]FOBG P1[ETX]）
// 回复：（[STX]FOBG R1 R2 R3[ETX]）
typedef struct tagFOBGParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;				// 1~4: Mark no 5：Mark连续搜索
	//	int P2;			// 1~5: 开始候补搜索号码

	tagFOBGParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFOBG;
		m_nCmdVal = eCmdValGetObjectImagePosFOBG;
		m_nVCmdValue = eVCmdFOBG;
		m_nPNum = 1;
		P1=1;
		//		P2=1;
	}

}FOBGParam;


// 4、FMSC:手动搜索位置的取得，在自动对准时，当目标 Mark 或物体 Mark 搜索失败时，可利用手动方法（鼠标）指定Mark 位置
// 参数
// 		Ｐ１   0：目标 Mark 1：物体 Mark
// 回复 
// 		Ｒ１   －１：异常结束 (0)*   ０：正常结束 (255)*  １：取消结束

// 发送：（[STX]FMSC P1[ETX]）
// 回复：（[STX]FMSC R1[ETX]）
typedef struct tagFMSCParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;				// 0：目标 Mark 1：物体 Mark
	//	int P2;			// 

	tagFMSCParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFMSC;
		m_nCmdVal = eCmdValManualSearchFMSC;
		m_nVCmdValue = eVCmdFMSC;
		m_nPNum = 1;
		P1=1;
		//		P2=1;
	}

}FMSCParam;


// 5、FWLD：取得目标（目标靶标）或对象（对象靶标）的平台坐标上的位置
// 参　数	
// 		Ｐ１	０：目标 靶标 １：对象 靶标
// 		Ｐ２	１～４：靶标№
// 回 复
// 		Ｒ１	－１：异常停止	　０：正常停止
// 		Ｒ２	-2147483648～2147483647（1/10000mm单位）：Ｘ方向的平台座标位置
// 		 Ｒ３	-2147483648～2147483647（1/10000mm单位）：Ｙ方向的平台座标位置

// 发送：（[STX]FWLD P1 P2[ETX]）
// 回复：（[STX]FWLD R1 R2 R3[ETX]）
typedef struct tagFWLDParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;				// 0：目标 Mark 1：物体 Mark
	int P2;				// １～４：靶标№

	tagFWLDParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFWLD;
		m_nCmdVal = eCmdValGetSearchWorldPosFWLD;
		m_nVCmdValue = eVCmdFWLD;
		m_nPNum = 2;
		P1=0;
		P2=1;
	}

}FWLDParam;

// 5、FWDR：5.1、FWDR:获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
// 参　数	
// 		Ｐ１	０：目标 靶标 １：对象 靶标
// 		Ｐ２	１～４：靶标№，5：根据全部Mark计算出一个位置的R
// 回 复
// 		Ｒ１	－１：异常停止	　０：正常停止
// 		Ｒ２	-2147483648～2147483647（1/10000mm单位）：Ｘ方向的平台座标位置
// 		 Ｒ３	-2147483648～2147483647（1/10000mm单位）：Ｙ方向的平台座标位置

// 发送：（[STX]FWLR P1 P2[ETX]）
// 回复：（[STX]FWLR R1 R2 R3[ETX]）
typedef struct tagFWDRParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;				// 0：目标 Mark 1：物体 Mark
	int P2;				// １～４：靶标№，5：根据全部Mark计算出一个位置的R

	tagFWDRParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFWDR;
		m_nCmdVal = eCmdValFWDR;
		m_nVCmdValue = eVCmdFWDR;
		m_nPNum = 1;
		P1=0;
		P2=1;
	}

}FWDRParam;

//////////////////////////////////////////////////////////////////////////
// 对准
// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
// 参数 
// 		Ｐ１ ０：对准精度判定取得 １：最终精度判定取得
// 回复	
// 		Ｒ１ －１：异常结束 	０：正常结束
// 		Ｒ２ －2147483648～2147483647（ｍｍ单位，１００００倍值） ：Ｘ方向精度值
// 		Ｒ３ －2147483648～2147483647（ｍｍ单位，１００００倍值） ：Ｙ方向精度值 
// 		Ｒ４ －2147483648～2147483647（度单位，１０００００倍值） ：θ方向精度值

// 发送：（[STX]FGAL P1[ETX]）
// 回复：（[STX]FGAL R1 R2 R3 R4[ETX]）
typedef struct tagFGALParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;				// ０：对准精度判定取得 １：最终精度判定取得
	//	int P2;			// 

	tagFGALParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGAL;
		m_nCmdVal = eCmdValGetAlignPrecisionFGAL;
		m_nVCmdValue = eVCmdFGAL;
		m_nPNum = 1;
		P1=1;
		//		P2=1;
	}

}FGALParam;




// 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
// 参数
// 		Ｐ１  ０：设定对准精度的判定   １：设定最终对准精度判定
// 		Ｐ２ Ｘ（１／１００００ｍm 单位）
// 		Ｐ３ Ｙ（１／１００００ｍｍ单位）
// 		Ｐ４ θ（１／１０００００度单位）
// 回复
// 		Ｒ１  －１：异常结束 ０：正常结束

// 发送：（[STX]FALL P1 P2 P3 P4[ETX]）
// 回复：（[STX]FALL R1[ETX]）
typedef struct tagFALLParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// ０：设定对准精度的判定   １：设定最终对准精度判定
	double P2;				// Ｘ（１／１００００ｍm 单位）
	double P3;				// Y（１／１００００ｍm 单位）
	double P4;				// θ（１／１００００ｍm 单位）
	tagFALLParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFALL;
		m_nCmdVal = eCmdValSetAlignPrecisionFALL;
		m_nVCmdValue = eVCmdFALL;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
	}

}FALLParam;



// 3、FGAC:对准最多次数的取得，取得设定对准最多次数
// 参数
// 回复 
// 		Ｒ１ －１：异常结束 ０：正常结束
// 		Ｒ２ １～１０：对准最多对准次数

// 发送：（[STX]FGAC[ETX]）
// 回复：（[STX]FGAC R1 R2[ETX]）
typedef struct tagFGACParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFGACParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGAC;
		m_nCmdVal = eCmdValGetAlignTimeFGAC;
		m_nVCmdValue = eVCmdFGAC;
		m_nPNum = 0;
	}

}FGACParam;



// 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
// 参数 
// 		Ｐ１ １～１０：设定最多对准次数
// 回复 
// 		Ｒ１  －１：异常结束  ０：正常结束

// 发送：（[STX]FALC P1[ETX]）
// 回复：（[STX]FALC R1[ETX]）
typedef struct tagFALCParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;				// １～１０：设定最多对准次数
	tagFALCParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFALC;
		m_nCmdVal = eCmdValSetAlignTimeFALC;
		m_nVCmdValue = eVCmdFALC;
		m_nPNum = 1;
		P1=1;
	}

}FALCParam;

// 5、FAAL:自动对准的执行，执行自动对准（连续对准）
// 参数
// 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// 			１：参照内部补偿量值（参照参数设定值）
// 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			６：参照外部补偿量值（参照取得的自动补偿量值）
// 回复 
// 		Ｒ１ －１：异常结束 (0)* ０：正常结束 (255)*

// 注：另外一种解释：
// 参　数	
// 		Ｐ1	０：不参照偏移量值。（偏移量值为0）
// 			１、１０～１４：参照内部偏移量值（参照参数设定值）
//		P2  轴1位置X
//		P3  轴2位置Y
//		P4  轴3位置D
// 回 复
// 		Ｒ１	－１：异常停止 ０：正常停止
//（自动迭代对位指令）

// 发送：（[STX]FAAL P1[ETX]）
// 回复：（[STX]FAAL R1[ETX]）
typedef struct tagFAALParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;	// 指令参数的最大个数数量
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置
	int nP2;//保留或多目标对位序号
	int nP3;//下料补偿序号
	int nP4;//扩展产品编号或目标固定补偿序号

	tagFAALParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFAAL;
		m_nCmdVal = eCmdValAutoAlignFAAL;
//		m_nVCmdValue = 8;
		m_nVCmdValue = eVCmdFAAL;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
		nP2 = 10;
		nP3 = 0;
		nP4 = 1;
	}

}FAALParam;

// 发送：（[STX]FXYA P1[ETX]）
// 回复：（[STX]FXYA R1[ETX]）
typedef struct tagFXYAParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;	// 指令参数的最大个数数量
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	int P2;			// ０：没有使用候补搜索 １～５：候补搜索开始编号
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置

	tagFXYAParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFXYA;
		m_nCmdVal = eCmdValAutoAlignFXYA;
//		m_nVCmdValue = 23;
		m_nVCmdValue = eVCmdFXYA;
		m_nPNum = 5;
		m_nPType = -1;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}

}FXYAParam;

// 5、FAAB:自动对准的执行，执行自动对准（连续对准）,对位过程中增加了图像十字中心基准对位功能
// 参数
// 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// 			１：参照内部补偿量值（参照参数设定值）
// 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			６：参照外部补偿量值（参照取得的自动补偿量值）
// 回复 
// 		Ｒ１ －１：异常结束 (0)* ０：正常结束 (255)*
// 		Ｒ２ －2147483648～2147483647 Ｘ方向的偏移量（１／10000mm 单位）
// 		Ｒ３ －2147483648～2147483647 Ｙ方向的偏移量（１／10000mm 单位）
// 		Ｒ４ －2147483648～2147483647 θ方向的偏移量（１／100000 度单位）

// 注：另外一种解释：
// 参　数	
// 		Ｐ1	０：不参照偏移量值。（偏移量值为0）
// 			１、１０～１４：参照内部偏移量值（参照参数设定值）
//		P2;	（新增可选参数，原Fast指令中没有该参数）0:不进行对象基准对位；1：进行对象基准对位
// 回 复
// 		Ｒ１	－１：异常停止 ０：正常停止
// 		Ｒ２ －2147483648～2147483647 Ｘ方向的偏移量（１／10000mm 单位）
// 		Ｒ３ －2147483648～2147483647 Ｙ方向的偏移量（１／10000mm 单位）
// 		Ｒ４ －2147483648～2147483647 θ方向的偏移量（１／100000 度单位）
//（自动迭代对位指令）

// 发送：（[STX]FAAB P1[ETX]）
// 回复：（[STX]FAAB R1 R2 R3 R4[ETX]）
typedef struct tagFAABParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	tagFAABParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFAAB;
		m_nCmdVal = eCmdValAutoAlignFAAB;
		m_nVCmdValue = eVCmdFAAB;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
	}

}FAABParam;


// 5、FACB:自动对准的执行，执行自动对准（连续对准）,候补模板，对位过程中增加了图像十字中心基准对位功能
// 参数
// 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// 			１：参照内部补偿量值（参照参数设定值）
// 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			６：参照外部补偿量值（参照取得的自动补偿量值）
// 回复 
// 		Ｒ１ －１：异常结束 (0)* ０：正常结束 (255)*
// 		Ｒ２ －2147483648～2147483647 Ｘ方向的偏移量（１／10000mm 单位）
// 		Ｒ３ －2147483648～2147483647 Ｙ方向的偏移量（１／10000mm 单位）
// 		Ｒ４ －2147483648～2147483647 θ方向的偏移量（１／100000 度单位）

// 注：另外一种解释：
// 参　数	
// 		Ｐ1	０：不参照偏移量值。（偏移量值为0）
// 			１、１０～１４：参照内部偏移量值（参照参数设定值）
//		P2;	（新增可选参数，原Fast指令中没有该参数）0:不进行对象基准对位；1：进行对象基准对位
// 回 复
// 		Ｒ１	－１：异常停止 ０：正常停止
// 		Ｒ２ －2147483648～2147483647 Ｘ方向的偏移量（１／10000mm 单位）
// 		Ｒ３ －2147483648～2147483647 Ｙ方向的偏移量（１／10000mm 单位）
// 		Ｒ４ －2147483648～2147483647 θ方向的偏移量（１／100000 度单位）
//（自动迭代对位指令）

// 发送：（[STX]FACB P1[ETX]）
// 回复：（[STX]FACB R1 R2 R3 R4[ETX]）
typedef struct tagFACBParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	int P2;			// 1~5: 开始候补搜索号码
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置

	tagFACBParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFACB;
		m_nCmdVal = eCmdValAutoAlignFACB;
		m_nVCmdValue = eVCmdFACB;
		m_nPNum = 5;
		m_nPType = -1;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}

}FACBParam;
// 5.1、FAAC:自动对准的执行，执行自动对准（连续对准），对象候补搜索
// 参数
// 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// 			１：参照内部补偿量值（参照参数设定值）
// 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			６：参照外部补偿量值（参照取得的自动补偿量值）
// 		Ｐ2	０:候补对象序号
//		P3;	（新增可选参数，原Fast指令中没有该参数）0:不进行对象基准对位；1：进行对象基准对位
// 回复 
// 		Ｒ１ －１：异常结束 (0)* ０：正常结束 (255)*

// 注：另外一种解释：
// 参　数	
// 		Ｐ1	０：不参照偏移量值。（偏移量值为0）
// 			１、１０～１４：参照内部偏移量值（参照参数设定值）
// 回 复
// 		Ｒ１	－１：异常停止 ０：正常停止
//（自动迭代对位指令）

// 发送：（[STX]FAAL P1[ETX]）
// 回复：（[STX]FAAL R1[ETX]）
typedef struct tagFAACParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	int P2;			// 1~5: 开始候补搜索号码

	int nP3;//下料补偿序号和多目标序号
	int nP4;//扩展产品序号、目标固定补偿序号

	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置

	tagFAACParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFAAC;
		m_nCmdVal = eCmdValAutoAlignFAAC;
//		m_nVCmdValue = 19;
		m_nVCmdValue = eVCmdFAAC;
		m_nPNum = 7;
		m_nPType = -1;
		P1=1;
		P2=1;

		nP3=-1;
		nP4=0;

		P3=0;
		P4=0;
		P5=0;
	}

}FAACParam;

// 5.1、FAOT:自动对准的执行，执行自动对准（连续对准），对象候补搜索，目标候补搜索
// 参数
// 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// 			１：参照内部补偿量值（参照参数设定值）
// 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			６：参照外部补偿量值（参照取得的自动补偿量值）
// 		Ｐ2	０:候补对象序号
//		P3;	（新增可选参数，原Fast指令中没有该参数）0:不进行对象基准对位；1：进行对象基准对位
// 回复 
// 		Ｒ１ －１：异常结束 (0)* ０：正常结束 (255)*

// 注：另外一种解释：
// 参　数	
// 		Ｐ1	０：不参照偏移量值。（偏移量值为0）
// 			１、１０～１４：参照内部偏移量值（参照参数设定值）
// 回 复
// 		Ｒ１	－１：异常停止 ０：正常停止
//（自动迭代对位指令）

// 发送：（[STX]FAAL P1[ETX]）
// 回复：（[STX]FAAL R1[ETX]）
typedef struct tagFAOTParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	int P2;			// 1~5: 开始候补搜索号码
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置

	tagFAOTParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFAOT;
		m_nCmdVal = eCmdValAutoAlignFAOT;
		m_nVCmdValue = eVCmdFAOT;
		m_nPNum = 5;
		m_nPType = -1;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}

}FAOTParam;

// 5.1、FALX:自动对准的执行，执行自动对准（连续对准），对象候补搜索，目标候补搜索
// 参数
// 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// 			１：参照内部补偿量值（参照参数设定值）
// 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			６：参照外部补偿量值（参照取得的自动补偿量值）
// 		Ｐ2	０:候补对象序号
//		P3;	（新增可选参数，原Fast指令中没有该参数）0:不进行对象基准对位；1：进行对象基准对位
// 回复 
// 		Ｒ１ －１：异常结束 (0)* ０：正常结束 (255)*

// 注：另外一种解释：
// 参　数	
// 		Ｐ1	０：不参照偏移量值。（偏移量值为0）
// 			１、１０～１４：参照内部偏移量值（参照参数设定值）
// 回 复
// 		Ｒ１	－１：异常停止 ０：正常停止
//（自动迭代对位指令）

// 发送：（[STX]FAAL P1[ETX]）
// 回复：（[STX]FAAL R1[ETX]）
typedef struct tagFALXParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	int P2;			// 1~5: 开始候补搜索号码
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置

	tagFALXParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFALX;
		m_nCmdVal = eCmdValAutoAlignFALX;
		m_nVCmdValue = eVCmdFALX;
		m_nPNum = 5;
		m_nPType = -1;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}

}FALXParam;



// 5、FAHL:背光叠片机自动对准的执行，执行自动对准（连续对准）
// 参数
// 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// 			１：参照内部补偿量值（参照参数设定值）
// 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			６：参照外部补偿量值（参照取得的自动补偿量值）
// 回复 
// 		Ｒ１ －１：异常结束 (0)* ０：正常结束 (255)*

// 注：另外一种解释：
// 参　数	
// 		Ｐ1	０：不参照偏移量值。（偏移量值为0）
// 			１、１０～１４：参照内部偏移量值（参照参数设定值）
//		P2  轴1位置X
//		P3  轴2位置Y
//		P4  轴3位置D
// 回 复
// 		Ｒ１	－１：异常停止 ０：正常停止
//（自动迭代对位指令）

// 发送：（[STX]FAHL P1[ETX]）
// 回复：（[STX]FAHL R1[ETX]）
typedef struct tagFAHLParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;	// 指令参数的最大个数数量
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	tagFAHLParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFAHL;
		m_nCmdVal = eCmdValAutoAlignFAHL;
		m_nVCmdValue = eVCmdFAHL;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
	}

}FAHLParam;

// 6、FANS:对准后的偏移量的取得，取得对准后平台坐标系的偏移量
// 参数 －
// 回复
// 		Ｒ１		－１：异常结束  ０：正常结束
// 		Ｒ２ －2147483648～2147483647 Ｘ方向的偏移量（１／10000mm 单位）
// 		Ｒ３ －2147483648～2147483647 Ｙ方向的偏移量（１／10000mm 单位）
// 		Ｒ４ －2147483648～2147483647 θ方向的偏移量（１／100000 度单位）
// （获取对位完成后的偏差量）

// 发送：（[STX]FANS[ETX]）
// 回复：（[STX]FANS R1 R2 R3 R4[ETX]）
typedef struct tagFANSParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	//	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	//	int P2;		// 1~5: 开始候补搜索号码

	tagFANSParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFANS;
		m_nCmdVal = eCmdValGetAutoAlignOffsetFANS;
		m_nVCmdValue = eVCmdFANS;
		m_nPNum = 0;
		//	P1=1;
		//	P2=1;
	}

}FANSParam;

// 7、FALG:１次对准移动量的取得，取得１次对准移动量
// 参数
// 		Ｐ１	 ０：不参照补偿量值（补偿量值为０）
// 			 １：参照内部补偿量值（参照参数设定值）
// 			 ５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			 ６：参照外部补偿量值（参照取得的自动补偿量值）
// 回复
// 		Ｒ１ －１：异常结束 ０：正常结束
// 
// 				ＸＹθ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的絶对值移动量
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的絶对值移动量
// 		Ｒ４ －2147483648～2147483647（１／１０００００度单位）：θ轴的絶对值移动量
// 				ＵＶＷ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的絶对值移动量
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的絶对值移动量
// 		Ｒ４ －2147483648～2147483647（１／１００００ｍｍ单位）：W 轴的絶对值移动量
//（取得一次对位指令后平台移动量，根据已经获得的目标和对象的位置，进行对位计算，获得相应的平台移动量
//（注：目标和对象，已经通过其他指令获得，此过程不用再重新获得了））

// 发送：（[STX]FALG P1[ETX]）
// 回复：（[STX]FALG R1 R2 R3 R4[ETX]）
typedef struct tagFALGParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	//	int P2;		// 1~5: 开始候补搜索号码

	int nP2;//保留或多目标对位序号
	int nP3;//下料补偿序号
	int nP4;//扩展产品编号或固定目标补偿序号

	tagFALGParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFALG;
		m_nCmdVal = eCmdValOnceAlignMoveFALG;
//		m_nVCmdValue = 14;
		m_nVCmdValue = eVCmdFALG;
		m_nPNum = 1;
		P1=1;
		//		P2=1;
		nP2 = 10;
		nP3 = 0;
		nP4 = 1;
	}

}FALGParam;


// 7、FALP:１次对准拾取移动量的取得，取得１次对准拾取移动量
// 参数
// 		Ｐ１	 ０：不参照补偿量值（补偿量值为０）
// 			 １：参照内部补偿量值（参照参数设定值）
// 			 ５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			 ６：参照外部补偿量值（参照取得的自动补偿量值）
// 回复
// 		Ｒ１ －１：异常结束 ０：正常结束
// 
// 				ＸＹθ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的絶对值移动量
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的絶对值移动量
// 		Ｒ４ －2147483648～2147483647（１／１０００００度单位）：θ轴的絶对值移动量
// 				ＵＶＷ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的絶对值移动量
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的絶对值移动量
// 		Ｒ４ －2147483648～2147483647（１／１００００ｍｍ单位）：W 轴的絶对值移动量
//（取得一次对位指令后平台移动量，根据已经获得的目标和对象的位置，进行对位计算，获得相应的平台移动量
//（注：目标和对象，已经通过其他指令获得，此过程不用再重新获得了））

// 发送：（[STX]FALP P1[ETX]）
// 回复：（[STX]FALP R1 R2 R3 R4[ETX]）
typedef struct tagFALPParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;	// 指令参数的最大个数数量
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	int nP2;        //扩展产品序号
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置

	tagFALPParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFALP;
		m_nCmdVal = eCmdValOnceAlignMovePickFALP;
		m_nVCmdValue = eVCmdFALP;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		nP2 =0;
		P2=0;
		P3=0;
		P4=0;
	}

}FALPParam;



// 8、FALJ:1:次对准精度判定与对准移动量的取得，对准移动量是否在对准精度内的判定与对准移动量的取得
// 参数
// 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// 			１：参照内部补偿量值（参照参数设定值）
// 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			６：参照外部补偿量值（参照取得的自动补偿量值）
// 
// 			Ｐ２ ０：参照对准精度判定值
// 			    １：参照最终对准精度判定值（无效时请参照对准精度判定值）
// 回复
// 		Ｒ１ －１：异常结束 ０：正常结束
// 				ＸＹθ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的絶对值移动量
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的絶对值移动量
// 		Ｒ４ －2147483648～2147483647（１／１０００００度单位）：θ轴的絶对值移动量
// 			ＵＶＷ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的絶对值移动量
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的絶对值移动量
// 		Ｒ４ －2147483648～2147483647（１／１００００ｍｍ单位）：W 轴的絶对值移动量
//（取得一次对位指令后平台移动量，根据已经获得的目标，然后拍摄对象的位置进行对位计算，获得相应的平台移动量
//（注：目标已经通过其他指令获得，但对象还未获得；非迭代执行，也就是此过程中不会再二次检查对象位置））

// 发送：（[STX]FALJ P1[ETX]）
// 回复：（[STX]FALJ R1 R2 R3 R4[ETX]）
typedef struct tagFALJParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	int P2;			// ０：参照对准精度判定值 １：参照最终对准精度判定值（无效时请参照对准精度判定值）

	tagFALJParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFALJ;
		m_nCmdVal = eCmdValOnceAlignFALJ;
		m_nVCmdValue = eVCmdFALJ;
		m_nPNum = 2;
		P1=1;
		P2=1;
	}

}FALJParam;

// 9、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
// 参数 －
// 回复
// 		Ｒ１ －１：异常结束 ０：正常结束
// 		Ｒ２ －2147483648～2147483647 Ｘ方向的偏移量（１／10000mm 单位）
// 		Ｒ３ －2147483648～2147483647 Ｙ方向的偏移量（１／10000mm 单位）
// 		Ｒ４ －2147483648～2147483647 θ方向的偏移量（１／100000 度单位）
//（根据已经获得的目标和对象的位置，进行对位计算，获得目标和对象之间的偏差，但不考虑各种补偿量
//（注：目标和对象，已经通过其他指令获得，此过程不用再重新获得了））

// 发送：（[STX]FACO[ETX]）
// 回复：（[STX]FACO R1 R2 R3 R4[ETX]）
typedef struct tagFACOParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）

	int nP2;//下料补偿序号或多目标对位序号
	int nP3;//扩展产品编号或目标固定补偿序号
	tagFACOParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFACO;
		m_nCmdVal = eCmdValGetTargetObjectOffsetFACO;
//		m_nVCmdValue = 13;
		m_nVCmdValue = eVCmdFACO;
		m_nPNum = 1;
		P1 = 0;
		nP2 = 0;//下料补偿序号或多目标对位序号
		nP3 = 1;//扩展产品编号或目标固定补偿序号
	}
}FACOParam;

typedef struct tagFAAOParam
{
	CString m_strCmd;
	int     m_nCmdVal;
	int     m_nVCmdValue;
	int     m_nPNum;
	int     m_nPType;
	int     P1;
	
	double  P2;
	double  P3;
	double  P4;

	tagFAAOParam()
	{
		m_strCmd     = COMM_CMD_STRING_VISIONFAAO;
		m_nCmdVal    = eCmdValFAAO;
		m_nVCmdValue = eVCmdFAAO;

		m_nPNum      = 4;
		m_nPType     = 4;
		P1           = 0;
		P2           = 0.0;
		P3           = 0.0;
		P4           = 0.0;

	}
	
}FAAOParam;

// 9、FOLC:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
// 参数 －
// 回复
// 		Ｒ１ －１：异常结束 ０：正常结束
// 		Ｒ２ －2147483648～2147483647 Ｘ方向的偏移量（１／10000mm 单位）
// 		Ｒ３ －2147483648～2147483647 Ｙ方向的偏移量（１／10000mm 单位）
// 		Ｒ４ －2147483648～2147483647 θ方向的偏移量（１／100000 度单位）
//（根据已经获得的目标和对象的位置，进行对位计算，获得目标和对象之间的偏差，但不考虑各种补偿量
//（注：目标和对象，已经通过其他指令获得，此过程不用再重新获得了））

// 发送：（[STX]FOLC[ETX]）
// 回复：（[STX]FOLC R1 R2 R3 R4[ETX]）
typedef struct tagFOLCParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	double P2;		// 轴1位置
	double P3;		// 轴2位置
	double P4;		// 轴3位置
	tagFOLCParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFOLC;
		m_nCmdVal = eCmdValGetTargetObjectOffsetFOLC;
		//		m_nVCmdValue = 13;
		m_nVCmdValue = eVCmdFOLC;
		m_nPNum = 4;
		m_nPType = -1;
		P1 = 0;
		P2=0;
		P3=0;
		P4=0;
	}
}FOLCParam;

// 10、FABO:偏移量的取得，取得目标图像十字中心及物体 Mark 之间中心的平台坐标系偏移量
// 参数 －
// 回复
// 		Ｒ１ －１：异常结束 ０：正常结束
// 		Ｒ２ －2147483648～2147483647 Ｘ方向的偏移量（１／10000mm 单位）
// 		Ｒ３ －2147483648～2147483647 Ｙ方向的偏移量（１／10000mm 单位）
// 		Ｒ４ －2147483648～2147483647 θ方向的偏移量（１／100000 度单位）
//（根据已经获得的目标和对象的位置，进行对位计算，获得目标和对象之间的偏差，但不考虑各种补偿量
//（注：目标和对象，已经通过其他指令获得，此过程不用再重新获得了））

// 发送：（[STX]FABO[ETX]）
// 回复：（[STX]FABO R1 R2 R3 R4[ETX]）
typedef struct tagFABOParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	tagFABOParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFABO;
		m_nCmdVal = eCmdValGetTargetObjectOffsetFABO;
		m_nVCmdValue = eVCmdFABO;
		m_nPNum = 1;
		P1 = 0;
	}
}FABOParam;



//////////////////////////////////////////////////////////////////////////
// 补偿量・移动
// 1、FAOF:自动补偿量值的取得，对准偏移量以补偿量值取得
// 参数
// 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// 			１：参照内部补偿量值（参照参数设定值）
// 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// 			６：参照外部补偿量值（参照取得的自动补偿量值）
// 回复 
// 		Ｒ１	－１：异常结束 (255)*	０：正常结束 (0)*

// 发送：（[STX]FAOF P1[ETX]）
// 回复：（[STX]FAOF R1[ETX]）
typedef struct tagFAOFParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	tagFAOFParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFAOF;
		m_nCmdVal = eCmdValCalcuAlignOffsetFAOF;
		m_nVCmdValue = eVCmdFAOF;
		m_nPNum = 0;
		P1=1;
		//		P2=1;
	}
}FAOFParam;


// 2、FGOF:补偿量值的取得，取得设定的补偿量值
// 参数
// 		Ｐ１０：取得最后使用的补偿量值。（最后下达ＦＡＡＬ，ＦＡＬＧ，ＦＡＬＪ，ＦＡＯＦ，ＦＡＣＯ的其中一个指令时所使用的补偿量值。
// 		       但ＦＡＣＯ下达时会自动设定无补偿量，因此在下达ＦＡＣＯ后得到的回复为（Ｒ２，Ｒ３，Ｒ４）＝（０，０，０））。
// 			１：内部补偿量值（取得参数设定值）
// 			５：外部补偿量值（取得在“FOFS”的设定值）
// 			６：外部补偿量值（取得在“FAOF”指令所取得的自动补偿量值）
// 回复
// 		Ｒ１－１：异常结束 ０：正常结束
// 		Ｒ２ －2147483648～2147483647（mm 单位，１００００倍值）：Ｘ方向的补偿量值
// 		Ｒ３ －2147483648～2147483647（ｍｍ单位，１００００倍值）：Ｙ方向的补偿量值
// 		Ｒ４ －2147483648～2147483647（度单位，１０００００倍值）：θ方向的补偿量值

// 发送：（[STX]FGOF P1[ETX]）
// 回复：（[STX]FGOF R1 R2 R3 R4[ETX]）
typedef struct tagFGOFParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	//	int P2;			// ０：参照对准精度判定值 １：参照最终对准精度判定值（无效时请参照对准精度判定值）

	tagFGOFParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGOF;
		m_nCmdVal = eCmdValGetAlignOffsetFGOF;
		m_nVCmdValue = eVCmdFGOF;
		m_nPNum = 1;
		P1=1;
		//		P2=1;
	}

}FGOFParam;

// 3、FOFS:补偿量值的设定，设定补偿量值
// 参数
// 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的补偿量值
// 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ方向的补偿量值
// 		Ｐ３ －2147483648～2147483647（１／１０００００度单位）：θ方向的补偿量值
// 回复 
// 		Ｒ１－１：异常结束 ０：正常结束

// 发送：（[STX]FOFS P1 P2 P3 P4[ETX]）
// 回复：（[STX]FOFS R1[ETX]）
typedef struct tagFOFSParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 补偿量序号
	double P2;				// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的补偿量值
	double P3;				// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ方向的补偿量值
	double P4;				// －2147483648～2147483647（１／１０００００度单位）：θ方向的补偿量值


	tagFOFSParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFOFS;
		m_nCmdVal = eCmdValSetAlignOffsetFOFS;
		m_nVCmdValue = eVCmdFOFS;
		m_nPNum = 4;
		m_nPType = -1;
		P1=1;
		P2=0;
		P3=0;
		P4=0;
	}

}FOFSParam;

// 发送：（[STX]FCHB P1 P2[ETX]）
// 回复：（[STX]FCHB R1 R2[ETX]）
typedef struct tagFCHBParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 1~4: Mark no 5：Mark连续搜索
	int P2;				// 1~5: 开始候补搜索号码

	tagFCHBParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCHB;
		m_nCmdVal = eCmdValFCHB;
		m_nVCmdValue = eVCmdFCHB;
		m_nPNum = 2;
		P1=1;
		P2=1;

	}

}FCHBParam;

// 发送：（[STX]FCHC P1 P2 P3[ETX]）
// 回复：（[STX]FCHC R1 R2[ETX]）
typedef struct tagFCHCParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 1~4: Mark no 5：Mark连续搜索
	int P2;				// 1~5: 开始候补搜索号码
	int P3;				// 1~5: 终止候补搜索号码
	tagFCHCParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCHC;
		m_nCmdVal = eCmdValFCHC;
		m_nVCmdValue = eVCmdFCHC;
		m_nPNum = 3;
		P1=1;
		P2=1;
		P3=5;
	}

}FCHCParam;

// 发送：（[STX]FDRP P1[ETX]）
// 回复：（[STX]FDRP R1[ETX]）
typedef struct tagFDRPParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 1~4: 对应基准拾取位置0度、90度、180度、270度
	int P2;             // 扩展产品序号

	tagFDRPParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFDRP;
		m_nCmdVal = eCmdValFDRP;
		m_nVCmdValue = eVCmdFDRP;
		m_nPNum = 1;
		P1=1;
		P2=0;

	}

}FDRPParam;

// 4、FSFT:平台Shift 移动动作，选择内部转移量的设定值，并依照设定值将目前平台的位置做相对移动
// 参数
// 		Ｐ１ ０ ：不参照移动值（移动值为０）
// 			 １～３：参照内部移动值（参照参数设定值）
// 回复 
// 		Ｒ１ －１：异常结束(255)* ０：正常结束 (0)*

// 发送：（[STX]FSFT P1[ETX]）
// 回复：（[STX]FSFT R1[ETX]）
typedef struct tagFSFTParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	//	int P2;			// ０：参照对准精度判定值 １：参照最终对准精度判定值（无效时请参照对准精度判定值）

	tagFSFTParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFSFT;
		m_nCmdVal = eCmdValFSFT;
		m_nVCmdValue = eVCmdFSFT;
		m_nPNum = 1;
		P1=1;
		//		P2=1;
	}

}FSFTParam;

// 5、FGSF:对准Shift 移动量的取得，取得对准转移量
// 参数
// 		Ｐ１０ ：不参照移动值（移动值为０）
// 				１～３：参照内部移动值（参照参数设定值）
// 回复
// 		Ｒ１－１：异常结束 ０：正常结束
// 					ＸＹθ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的絶对值移动量
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的絶对值移动量
// 		Ｒ４ －2147483648～2147483647（１／１０００００度单位）：θ轴的絶对值移动量
// 					ＵＶＷ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的絶对值移动量
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的絶对值移动量
// 		Ｒ４ －2147483648～2147483647（１／１００００ｍｍ单位）：W 轴的絶对值移动量

// 发送：（[STX]FGSF P1[ETX]）
// 回复：（[STX]FGSF R1 R2 R3 R4[ETX]）
typedef struct tagFGSFParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	//	int P2;			// ０：参照对准精度判定值 １：参照最终对准精度判定值（无效时请参照对准精度判定值）

	tagFGSFParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGSF;
		m_nCmdVal = eCmdValFGSF;
		m_nVCmdValue = eVCmdFGSF;
		m_nPNum = 1;
		P1=1;
		//		P2=1;
	}

}FGSFParam;


// 6、MNPS:平台现在位置的取得，取得平台现在位置
// 参数 －
// 回复
// 		Ｒ１ －１：异常结束 ０：正常结束
// 					ＸＹθ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的现在位置
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的现在位置
// 		Ｒ４ －2147483648～2147483647（１／１０００００度单位）：θ轴的现在位置
// 					ＵＶＷ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的现在位置
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的现在位置
// 		Ｒ４ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｗ轴的现在位置

// 视觉主动发送，PLC被动响应
// 发送：（[STX]MNPS[ETX]）
// 回复：（[STX]MNPS R1 R2 R3 R4[ETX]）
typedef struct tagMNPSParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;				// 		Ｒ１ －１：异常结束 ０：正常结束
	double P2;				// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的现在位置
	double P3;				// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ方向的现在位置
	double P4;				// －2147483648～2147483647（１／１０００００度单位）：θ方向的现在位置

	int m_nVPNum;		// 视觉发送参数个数

	tagMNPSParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFMPS;
		m_nCmdVal = eCmdValGetPosMNPS;
//		m_nVCmdValue = 9;
		m_nVCmdValue = eVCmdFNPS;
		m_nPNum = 4;
		P1=0;
		P2=0;
		P3=0;
		P4=0;

		m_nVPNum= 0;
	}

}MNPSParam;



// 7、MMVA:平台絶对准置移动量的设定，设定平台絶对准置移动量
// 参数
// 							ＸＹθ轴平台时
// 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的絶对移动量
// 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的絶对移动量
// 		Ｐ３ －2147483648～2147483647（１／１０００００度单位）：θ轴的絶对移动量
// 							ＵＶＷ轴平台时
// 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的絶对移动量
// 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的絶对移动量
// 		Ｐ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｗ轴的絶对移动量
// 回复 
// 		Ｒ１ －１：异常结束 ０：正常结束

// 视觉主动发送，PLC被动响应
// 发送：（[STX]MMVA P1 P2 P3[ETX]）
// 回复：（[STX]MMVA R1[ETX]）
typedef struct tagMMVAParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;				// 		Ｒ１ －１：异常结束 ０：正常结束

	int m_nVPNum;		// 视觉发送参数个数
	double VP1;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
	double VP2;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
	double VP3;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量

	tagMMVAParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFMVA;
		m_nCmdVal = eCmdValMovePosMMVA;
//		m_nVCmdValue = 10;
		m_nVCmdValue = eVCmdFMVA;
		m_nPNum = 1;
		P1=0;
		
		m_nVPNum = 3;
		VP1 = 0;
		VP2 = 0;
		VP3 = 0;
	}

}MMVAParam;


// 8、MMVR:平台相对准置移动量的设定，设定平台相对准置移动量
// 参数
// 						ＸＹθ轴平台时
// 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的相对移动量
// 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的相对移动量
// 		Ｐ３ －2147483648～2147483647（１／１０００００度单位）：θ轴的相对移动量
// 						ＵＶＷ轴平台时
// 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的相对移动量
// 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的相对移动量
// 		Ｐ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｗ轴的相对移动量
// 回复		
// 		Ｒ１ －１：异常结束 ０：正常结束

// 视觉主动发送，PLC被动响应
// 发送：（[STX]MMVR P1 P2 P3[ETX]）
// 回复：（[STX]MMVR R1[ETX]）
typedef struct tagMMVRParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;				// 		Ｒ１ －１：异常结束 ０：正常结束

	int m_nVPNum;		// 视觉发送参数个数
	double VP1;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
	double VP2;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
	double VP3;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量

	tagMMVRParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFMVR;
		m_nCmdVal = eCmdValMoveRelPosMMVR;
//		m_nVCmdValue = 11;
		m_nVCmdValue = eVCmdFMVR;
		m_nPNum = 1;
		P1=0;

		m_nVPNum = 3;
		VP1 = 0;
		VP2 = 0;
		VP3 = 0;
	}

}MMVRParam;


// 9、FGPO:连续对位最终精度获取
// 参数
// 						ＸＹθ轴平台时
// 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的相对移动量
// 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的相对移动量
// 		Ｐ３ －2147483648～2147483647（１／１０００００度单位）：θ轴的相对移动量
// 						ＵＶＷ轴平台时
// 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的相对移动量
// 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的相对移动量
// 		Ｐ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｗ轴的相对移动量
// 回复		
// 		Ｒ１ －１：异常结束 ０：正常结束

// PLC发送请求，视觉被动响应
// 发送：（[STX]FGPO[ETX]）
// 回复：（[STX]FGPO VP1 VP2 VP3[ETX]）
typedef struct tagFGPOParam
{
	// 通用无协议
	CString m_strCmd;	// 指令字符串
	// 网口单字节
	int		m_nCmdVal;	// 指令整数值
	// 网口双字节
	int     m_nVCmdValue;// FAST读写寄存器

	int m_nPNum;
	int P1;				// 		Ｒ１ －１：异常结束 ０：正常结束

	int m_nVPNum;		// 视觉发送参数个数
	double VP1;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
	double VP2;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
	double VP3;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量

	tagFGPOParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGPO;
		m_nCmdVal = eCmdValGetOffsetFGPO;
		m_nVCmdValue = eVCmdFGPO;
		m_nPNum = 0;
		P1=0;

		m_nVPNum = 3;
		VP1 = 0;
		VP2 = 0;
		VP3 = 0;
	}
}FGPOParam;


//////////////////////////////////////////////////////////////////////////
// Alarm
// 1、FALM:Alarm资料的取得，取得错误发生的数据代码
// 参数 －
// 回复 
// 		Ｒ１ ０：没有错误  －５０～－２５０：错误代码
// 		Ｒ２ 仿真数据。（通常设定为“－１”）

// 发送：（[STX]FALM[ETX]）
// 回复：（[STX]FALM R1 R2[ETX]）
typedef struct tagFALMParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFALMParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFALM;
		m_nCmdVal = eCmdValGetAlarmFALM;
		m_nVCmdValue = eVCmdFALM;
		m_nPNum = 0;
	}

}FALMParam;


// 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
// 参数 －
// 回复 －

// 发送：（[STX]FRST[ETX]）
// 回复：（[STX]FRST[ETX]）
typedef struct tagFRSTParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值=1
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFRSTParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFRST;
		m_nCmdVal = eCmdValResetFRST;
//		m_nVCmdValue = 1;
		m_nVCmdValue = eVCmdFRST;
		m_nPNum = 0;
	}

}FRSTParam;

// 3、FSAC：保存校正、定位时的数据。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
// 参　数	－	
// 回 复
// 		Ｒ１	－１：异常停止 　０：正常停止
// 发送：（[STX]FSAC[ETX]）
// 回复：（[STX]FSAC R1[ETX]）
typedef struct tagFSACParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFSACParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFSAC;
		m_nCmdVal = eCmdValSaveProcessDataFSAC;
		m_nVCmdValue = eVCmdFSAC;
		m_nPNum = 0;
	}

}FSACParam;


// 4、FSCO：保存指令收发的历史纪录。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
// 参　数	－	
// 回 复
// 		Ｒ１	－１：异常停止 　０：正常停止
// 发送：（[STX]FSCO[ETX]）
// 回复：（[STX]FSCO R1[ETX]）
typedef struct tagFSCOParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFSCOParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFSCO;
		m_nCmdVal = eCmdValSaveCommDataFSCO;
		m_nVCmdValue = eVCmdFSCO;
		m_nPNum = 0;
	}

}FSCOParam;

// 发送：（[STX]FCHK 10[ETX]）
// 回复：（[STX]FCHK CY[ETX]）
typedef struct tagFCHKParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int P1;
	tagFCHKParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCHK;
		m_nCmdVal = eCmdValSpecialCommFCHK;
		m_nVCmdValue = eVCmdFCHK;
		m_nPNum = 1;
		P1 = 0;
	}

}FCHKParam;

// 相机曝光设置
typedef struct tagFSPEParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;	// 指令参数的最大个数数量
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 0：目标；1：对象相机曝光
	int P2;			// 相机位置序号
	int P3;			// 候补模板序号

	tagFSPEParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFSPE;
		m_nCmdVal = eCmdValSetProductExposureFSPE;
		m_nVCmdValue = eVCmdFSPE;
		m_nPNum = 3;
		m_nPType = 3;
		P1=0;
		P2=0;
		P3=0;

	}

}FSPEParam;


// 专用焊接指令,小FPC:XY,大FPC:XYD
typedef struct tagFWELParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int		m_nVCmdValue;
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	int P2;			// 1~5: 开始候补搜索号码
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置

	tagFWELParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFWEL;
		m_nCmdVal = eCmdValFWEL;
		m_nVCmdValue = eVCmdFWEL;
		m_nPNum = 5;
		m_nPType = -1;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}

}FWELParam;



// 专用焊接指令,小FPC:XYD,大FPC:XY
typedef struct tagFSOLParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int		m_nVCmdValue;
	int m_nPNum;
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
	int P2;			// 1~5: 开始候补搜索号码
	double P3;		// 轴1位置
	double P4;		// 轴2位置
	double P5;		// 轴3位置

	tagFSOLParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFSOL;
		m_nCmdVal = eCmdValFSOL;
		m_nVCmdValue = eVCmdFSOL;
		m_nPNum = 5;
		m_nPType = -1;
		P1=1;
		P2=1;
		P3=0;
		P4=0;
		P5=0;
	}

}FSOLParam;


// 发送：（[STX]FPCB P1[ETX]）
// 回复：（[STX]FPCB R1 R2....R17[ETX]）
typedef struct tagFPCBParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;	    // 指令参数类型（即实际参数的个数数量）
	int P1;				// 1:对象（4个相机）；2:目标（4个相机）
//	int P2;				// 1~4: 曝光时间序号


	tagFPCBParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFPCB;
		m_nCmdVal = eCmdValFPCB;
		m_nVCmdValue = eVCmdFPCB;
		m_nPType = -1;
		m_nPNum = 1;
		P1=0;
//		P2=0;
	}

}FPCBParam;


// PCB精度检测 PCB Inspect
typedef struct tagFPISParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;	// 指令参数的最大个数数量
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 检测补偿序号


	tagFPISParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFPIS;
		m_nCmdVal = eCmdValPCBInspectFPIS;
		m_nVCmdValue = eVCmdFPIS;
		m_nPNum = 1;
		m_nPType = -1;
		P1=0;
	}

}FPISParam;

typedef struct tagFCPIParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;	// 指令参数的最大个数数量
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 检测补偿序号


	tagFCPIParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCPI;
		m_nCmdVal = eCmdValPCBInspectFCPI;
		m_nVCmdValue = eVCmdFCPI;
		m_nPNum = 1;
		m_nPType = -1;
		P1=0;
	}

}FCPIParam;


// 检测产品有、无
typedef struct tagFINSParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	int	m_nPType;		// 指令参数类型（即实际参数的个数数量）
	int P1;				// 目标or对象
	int P2;				// 位置索引
	tagFINSParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFINS;
		m_nCmdVal = eCmdValInspectFINS;
		m_nVCmdValue = eVCmdFINS;
		m_nPNum = 2;
		m_nPType = -1;
		P1 = 0;
		P2 = 0;
	}

}FINSParam;

// FAST读写寄存器
// 	2	FHMS(暂无)
typedef struct tagFHMSParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFHMSParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFHMS;
		m_nCmdVal = eCmdValFHMS;
//		m_nVCmdValue = 2;
		m_nVCmdValue = eVCmdFHMS;
		m_nPNum = 0;
	}

}FHMSParam;

// 	3	FHOM(暂无)
typedef struct tagFHOMParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFHOMParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFHOM;
		m_nCmdVal = eCmdValFHOM;
//		m_nVCmdValue = 3;
		m_nVCmdValue = eVCmdFHOM;
		m_nPNum = 0;
	}

}FHOMParam;

// 	12	FPNT(暂无)
typedef struct tagFPNTParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFPNTParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFPNT;
		m_nCmdVal = eCmdValFPNT;
		m_nVCmdValue = eVCmdFPNT;
		m_nPNum = 0;
	}

}FPNTParam;

// 	22	FMAP(暂无)
typedef struct tagFMAPParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFMAPParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFMAP;
		m_nCmdVal = eCmdValFMAP;
//		m_nVCmdValue = 22;
		m_nVCmdValue = eVCmdFMAP;
		m_nPNum = 0;
	}

}FMAPParam;

// 	24	FRCT(暂无)
typedef struct tagFRCTParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFRCTParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFRCT;
		m_nCmdVal = eCmdValFRCT;
		m_nVCmdValue = eVCmdFRCT;
		m_nPNum = 0;
	}

}FRCTParam;
// 	25	FISP(暂无)
typedef struct tagFISPParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFISPParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFISP;
		m_nCmdVal = eCmdValFISP;
		m_nVCmdValue = eVCmdFISP;
		m_nPNum = 0;
	}

}FISPParam;
// 	26	FOLG(暂无)
typedef struct tagFOLGParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFOLGParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFOLG;
		m_nCmdVal = eCmdValFOLG;
		m_nVCmdValue = eVCmdFOLG;
		m_nPNum = 0;
	}

}FOLGParam;
// 	27	FTLG(暂无)
typedef struct tagFTLGParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;
	tagFTLGParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFTLG;
		m_nCmdVal = eCmdValFTLG;
		m_nVCmdValue = eVCmdFTLG;
		m_nPNum = 0;
	}

}FTLGParam;

// 备用获取轴位置指令
// MNPSB:平台现在位置的取得，取得平台现在位置
// 参数 －
// 回复
// 		Ｒ１ －１：异常结束 ０：正常结束
// 					ＸＹθ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的现在位置
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的现在位置
// 		Ｒ４ －2147483648～2147483647（１／１０００００度单位）：θ轴的现在位置
// 					ＵＶＷ轴平台时
// 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的现在位置
// 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的现在位置
// 		Ｒ４ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｗ轴的现在位置

// 视觉主动发送，PLC被动响应
// 发送：（[STX]MNPSB[ETX]）
// 回复：（[STX]MNPSB R1 R2 R3 R4[ETX]）
typedef struct tagMPSBParam
{
	CString m_strCmd;
	int m_nCmdVal;
	int m_nVCmdValue;
	int m_nPNum;
	int m_nVPNum;		// 视觉发送参数个数
	int m_nP1;
	double m_dP2;
	double m_dP3;
	double m_dP4;
	tagMPSBParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFMPB;
		m_nCmdVal = eCmdValMPSB;
		m_nVCmdValue = eVCmdMPSB;
		m_nPNum = 4;
		m_nVPNum = 0;
		m_nP1 = 0;
		m_dP2 = 0;
		m_dP3 = 0;
		m_dP4 = 0;

	}
}MPSBParam;

//存储2DCode
typedef struct tagFQRC
{
	CString m_strCmd;	//指令字符串
	int		m_nCmdVal;	//指令整数值
	int		m_nVCmdValue;//FAST读取寄存器
	tagFQRC()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFQRC;
		m_nCmdVal = eCmdValFQRC;
		m_nVCmdValue = eVCmdFQRC;
	}
}FQRCParam;

//检测DM、QR Code
typedef struct tagFQRI
{
	CString m_strCmd;	//指令字符串
	int		m_nCmdVal;	//指令整数值
	int		m_nVCmdValue;//FAST读取寄存器
	int m_nPNum;	// 指令参数的最大个数数量
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;				// 位置索引
	int P2;				// 候补索引
	tagFQRI()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFQRI;
		m_nCmdVal = eCmdValFQRI;
		m_nVCmdValue = eVCmdFQRI;
		m_nPNum = 2;
		m_nPType = -1;
		P1=1;
		P2=1;
	}
}FQRIParam;

//视觉进行目标拍照并发送目标平台坐标及轴位置给PLC
typedef struct tagFGTW
{
	CString m_strCmd;
	int     m_nCmdVal;
	int     m_nVCmdValue;
	int     m_nPNum;
	int     m_nPType;// 指令参数类型（即实际参数的个数数量）
	int     P1;// 1~4: Mark no 5：Mark连续搜索
	int     P2;// 1~5: 开始候补搜索号码
	double  P3;// 轴1位置
	double  P4;// 轴2位置
	double  P5;// 轴3位置
	int nP3;		// 是否检测或多目标对位序号
	int nP4;		// 扩展产品序号,有效范围1到n+1, n为扩展数目
	int nP5;        // 保留
	tagFGTW()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGTW;
		m_nCmdVal = eCmdValFGTW;
		m_nVCmdValue = eVCmdFGTW;
		m_nPNum = 2;
		m_nPType = -1;
		P1 = 0;
		P2 = 0;
		P3 = 0;
		P4 = 0;
		P5 = 0;
		nP3 = 0;
		nP4 = 0;
		nP5 = 0;
	}
}FGTWParam;

//目标采用靶标关联时计算目标坐标系到对象坐标系的固定补偿
typedef struct tagFAFO
{
	CString m_strCmd;
	int     m_nCmdVal;
	int     m_nVCmdValue;
	int     m_nPNum;
	int     m_nPType;
	int     P1; //操作类型：0 目标二维码拍照；1 对象二维码拍照；2 固定补偿计算
	int     P2; //拍照位置序号
	int     P3; //目标固定补偿序号
	int     nP4;//扩展产品序号
	double  P4;
	double  P5;
	double  P6;
	tagFAFO()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFAFO;
		m_nCmdVal = eCmdValFAFO;
		m_nVCmdValue = eVCmdFAFO;
		m_nPNum = 3;
		m_nPType = -1;
		P1 = 0;
		P2 = 0;
		P3 = 0;
		nP4 = 0;
		P4 = 0;
		P5 = 0;
		P6 = 0;
	}
}FAFOParam;

//视觉进行目标拍照并发送目标图像坐标及轴位置给PLC
typedef struct tagFGTP
{
	CString m_strCmd;
	int     m_nCmdVal;
	int     m_nVCmdValue;
	int     m_nPNum;
	int     m_nPType;
	int     P1;// 1~4: Mark no 5：Mark连续搜索
	int     P2;// 1~5: 开始候补搜索号码
	double  P3;// 轴1位置
	double  P4;// 轴2位置
	double  P5;// 轴3位置
	int nP3;		// 是否检测或多目标对位序号
	int nP4;		// 扩展产品序号,有效范围1到n+1, n为扩展数目
	int nP5;        // 保留
	
	tagFGTP()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFGTP;
		m_nCmdVal = eCmdValFGTP;
		m_nVCmdValue = eVCmdFGTP;
		m_nPNum = 2;
		m_nPType = -1;
		P1 = 0;
		P2 = 0;
		P3 = 0;
		P4 = 0;
		P5 = 0;
		nP3 = 0;
		nP4 = 0;
		nP5 = 0;
	}
}FGTPParam;

// MultiCalculateOffset 根据不同补偿多次计算目标对象平台坐标系偏差
typedef struct tagFMCOParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FAST读写寄存器
	int m_nPNum;	// 指令参数的最大个数数量
	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
	int P1;			// 目标补偿个数


	tagFMCOParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFMCO;
		m_nCmdVal = eCmdValGetTargetObjectOffsetFMCO;
		m_nVCmdValue = eVCmdFMCO;
		m_nPNum = 1;
		m_nPType = -1;
		P1=0;
	}

}FMCOParam;

//复制标定信息
typedef struct tagFCCIParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	int     m_nVCmdValue;// FCCI读写寄存器
	int m_nPNum;
	int P1;			// 1： 被复制的位置序号
	int P2;			// 1： 复制到的位置序号
	tagFCCIParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFCCI;
		m_nCmdVal = eCmdValFCCI;
		m_nVCmdValue = eVCmdFCCI;
		m_nPNum = 2;
		P1=1;
		P2=1;
	}
}FCCIParam;

// 产品清空
typedef struct tagFPEPParam
{
	CString m_strCmd;
	int		m_nCmdVal;
	int		m_nVCmdValue;
	int		m_nPNum;
	int     m_nPType;
	int		P1;// 大品种索引
	int		P2;// 小品种索引
	tagFPEPParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFPEP;
		m_nCmdVal = eCmdValFPEP;
		m_nVCmdValue = eVCmdFPEP;
		m_nPNum = 2;
		m_nPType = -1;
		P1 = 0;
		P2 = 0;

	}
}FPEPParam;

// 产品拷贝
typedef struct tagFPCPParam
{
	CString m_strCmd;
	int		m_nCmdVal;
	int		m_nVCmdValue;
	int		m_nPNum;
	int     m_nPType;
	int		P1;// 来源大品种索引
	int		P2;// 来源小品种索引
	int		P3;// 目的大品种索引
	int		P4;// 目的小品种索引
	tagFPCPParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFPCP;
		m_nCmdVal = eCmdValFPCP;
		m_nVCmdValue = eVCmdFPCP;
		m_nPNum = 4;
		m_nPType = -1;
		P1 = 0;
		P2 = 0;
		P3 = 0;
		P4 = 0;
	}
}FPCPParam;

// 产品重命名
typedef struct tagFPRNParam
{
	CString m_strCmd;
	int		m_nCmdVal;
	int		m_nVCmdValue;
	int		m_nPNum;
	int     m_nPType;
	int		P1;// 大品种索引
	int		P2;// 小品种索引
	int		P3;// 字符串长度

	tagFPRNParam()
	{
		m_strCmd = COMM_CMD_STRING_VISIONFPRN;
		m_nCmdVal = eCmdValFPRN;
		m_nVCmdValue = eVCmdFPRN;
		m_nPNum = 3;
		m_nPType = -1;
		P1 = 0;
		P2 = 0;
		P2 = 0;
	}
}FPRNParam;


//#define CommProtocol_YAMAHA

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Yamaha 机器人运动控制
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Yamaha 240
// 1）机器人工作模式查询、设定（手动模式manual、自动模式auto等）；
// 2）机器人自动模式下程序执行操作（自动模式下的运行Run、重置Reset、停止stop等）；
// 3）机器人手动移动速度查询、设定（mspeed）；
// 4）机器人移动命令（inch X、Y、Z等）；
// 5）机器人当前位置查询（WHRXY）；
// 6）设置机器人示教点位（TEACH 200）；
// 7）机器人点位查询（Read P200）；
// 8）执行机器人语言指令（@“机器人语言”，如@MOVE P, P100, S=20 [cr/lf]）.
// 9）执行中断处理（ETX(03)）
// 10）设置某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) = 20 [cr/lf]）
// 10）设置某个点的位置（@“机器人语言”，如@WRITE P100 = 100.00 ...... [cr/lf]）
// 11）查询某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) [cr/lf]）
// 12) 设置某个变量的值
// 13）设置某个变量的值
// 14）主机器人轴移动至可进行绝对式原点复位的位置(ABSADJ X-)
// 15）主机器人轴的绝对式原点复位(ABSRESET k)
// 16) 设置整型常量的值（@“机器人语言”，如@SGI0 = 20[cr/lf]）
// 17) 查询整型常量的值（@“机器人语言”，如@READ SGI0[cr/lf]）
// 16) 设置实数常量的值（@“机器人语言”，如@SGR0 = 20.0[cr/lf]）
// 17) 查询实数常量的值（@“机器人语言”，如@READ SGR0[cr/lf]）


#define COMM_ROBOT_CMD_STRING_OK			_T("OK")
#define COMM_ROBOT_CMD_STRING_MOD			_T("MOD")
#define COMM_ROBOT_CMD_STRING_AUTO			_T("AUTO")
#define COMM_ROBOT_CMD_STRING_PROGRAM		_T("PROGRAM")
#define COMM_ROBOT_CMD_STRING_MANUAL		_T("MANUAL")
#define COMM_ROBOT_CMD_STRING_SYSTEM		_T("SYSTEM")

#define COMM_ROBOT_CMD_STRING_RESET			_T("RESET")
#define COMM_ROBOT_CMD_STRING_RUN			_T("RUN")
#define COMM_ROBOT_CMD_STRING_STEP			_T("STEP")
#define COMM_ROBOT_CMD_STRING_SKIP			_T("SKIP")
#define COMM_ROBOT_CMD_STRING_NEXT			_T("NEXT")
#define COMM_ROBOT_CMD_STRING_STOP			_T("STOP")

#define COMM_ROBOT_CMD_STRING_MSPEED		_T("MSPEED")
#define COMM_ROBOT_CMD_STRING_INCH			_T("INCH")
#define COMM_ROBOT_CMD_STRING_INCH_X		_T("X")
#define COMM_ROBOT_CMD_STRING_INCH_Y		_T("Y")
#define COMM_ROBOT_CMD_STRING_INCH_R		_T("R")
#define COMM_ROBOT_CMD_STRING_INCH_Z		_T("Z")
#define COMM_ROBOT_CMD_STRING_INCH_A		_T("A")
#define COMM_ROBOT_CMD_STRING_INCH_B		_T("B")
#define COMM_ROBOT_CMD_STRING_INCH_ADD		_T("+")
#define COMM_ROBOT_CMD_STRING_INCH_SUB		_T("-")

#define COMM_ROBOT_CMD_STRING_WHRXY			_T("WHRXY")
#define COMM_ROBOT_CMD_STRING_TEACH			_T("TEACH")
#define COMM_ROBOT_CMD_STRING_READ			_T("READ")
#define COMM_ROBOT_CMD_STRING_WRITE			_T("WRITE")
#define COMM_ROBOT_CMD_STRING_MOVE			_T("MOVE")

#define COMM_ROBOT_CMD_STRING_SGI			_T("SGI") 
#define COMM_ROBOT_CMD_STRING_SGR			_T("SGR")

#define COMM_ROBOT_CMD_STRING_LOC			_T("LOC")
#define COMM_ROBOT_CMD_STRING_ABSADJ		_T("ABSADJ")
#define COMM_ROBOT_CMD_STRING_ABSRESET		_T("ABSRESET")
#define COMM_ROBOT_CMD_STRING_ABORTED		_T("* * * Aborted")


//////////////////////////////////////////////////////////////////////////
// Yamaha 340
// 1）机器人工作模式查询、设定（?MODE,手动模式manual、自动模式auto等）；
// 2）机器人自动模式下程序执行操作（自动模式下的运行Run、重置Reset、停止stop等）；
// 3）机器人手动移动速度查询、设定（mspeed）；
// 4）机器人移动命令（INCHXY X、Y、Z等）；
// 5）机器人当前位置查询（WHRXY）；
// 6）设置机器人示教点位（TCHXY[1] 200）；
// 7）机器人点位查询（Read P200）；
// 8）执行机器人语言指令（@“机器人语言”，如@MOVE P, P100, S=20 [cr/lf]）.
// 9）执行中断处理（ETX(03)）
// 10）设置某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) = 20 [cr/lf]）
// 10）设置某个点的位置（@“机器人语言”，如@WRITE P100 = 100.00 ...... [cr/lf]）
// 11）查询某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) [cr/lf]）
// 12) 设置某个变量的值
// 13）设置某个变量的值
// 14）主机器人轴移动至可进行绝对式原点复位的位置(ABSADJ X-)
// 15）主机器人轴的绝对式原点复位(ABSRESET k)
// 16) 设置整型常量的值（@“机器人语言”，如@SGI0 = 20[cr/lf]）
// 17) 查询整型常量的值（@“机器人语言”，如@READ SGI0[cr/lf]）
// 16) 设置实数常量的值（@“机器人语言”，如@SGR0 = 20.0[cr/lf]）
// 17) 查询实数常量的值（@“机器人语言”，如@READ SGR0[cr/lf]）

#define COMM_ROBOT_CMD_STRING_MOD_340			_T("MODE")	
#define COMM_ROBOT_CMD_STRING_AUTO_340			_T("1")
#define COMM_ROBOT_CMD_STRING_PROGRAM_340		_T("2")
#define COMM_ROBOT_CMD_STRING_MANUAL_340		_T("3")
#define COMM_ROBOT_CMD_STRING_SYSTEM_340		_T("4")
#define COMM_ROBOT_CMD_STRING_INCH_340			_T("INCHXY")
#define COMM_ROBOT_CMD_STRING_START_340			_T("RUN")				// 处理开始
#define COMM_ROBOT_CMD_STRING_END_340			_T("END")				// 处理完成
#define COMM_ROBOT_CMD_STRING_INCH_X_340		_T("1")
#define COMM_ROBOT_CMD_STRING_INCH_Y_340		_T("2")
#define COMM_ROBOT_CMD_STRING_INCH_R_340		_T("4")
#define COMM_ROBOT_CMD_STRING_INCH_Z_340		_T("3")
#define COMM_ROBOT_CMD_STRING_INCH_A_340		_T("5")
#define COMM_ROBOT_CMD_STRING_INCH_B_340		_T("6")
#define COMM_ROBOT_CMD_STRING_INCH_ADD_340		_T("+")
#define COMM_ROBOT_CMD_STRING_INCH_SUB_340		_T("-")
#define COMM_ROBOT_CMD_STRING_WHRXY_340			_T("WHRXY")
#define COMM_ROBOT_CMD_STRING_TEACH_340			_T("TCHXY")


//命令： @ MSPEED 50 [cr/lf]
//应答： OK [cr/lf]


//命令： @ INCH X+ [cr/lf]
//应答： OK [cr/lf]

// 机器人工作模式
typedef enum enumRobotMod{
	eRmUnknown = -1,			// 未知模式
	eRmAuto = 0,				// 自动模式
	eRmProgram = 1,
	eRmManual,
	eRmSystem
}RobotMod;

// 机器人自动模式下状态控制
// RESET ......................................复位程序
// RUN .........................................执行程序
// STEP ........................................执行1 行程序（进入子例程内部）
// SKIP ........................................ 跳过1 行程序（子例程跳过1 行）
// NEXT .......................................将 程序执行至下一行（子例程作为1 行执行）
// STOP .......................................停止程序
typedef enum enumRobotAutoControl{
	eRacUnknown = -1,	// 未知
	eRacReset = 0,		// 复位程序
	eRacRun = 1,		// 执行程序
	eRacStep,			// 执行1 行程序（进入子例程内部）
	eRacSkip,			// 跳过1 行程序（子例程跳过1 行）
	eRacNext,			// 将 程序执行至下一行（子例程作为1 行执行）
	eRacStop			// 停止程序
}RobotAutoControl;

// 点动时机器人轴类型
typedef enum enumRobotInchAxis{
	eRiaX = 0,		// X
	eRiaY = 1,		// Y
	eRiaR,			// R
	eRiaZ,			// Z
	eRiaA,			// A
	eRiaB			// B
}RobotInchAxis;

// 点动时机器人轴增减类型
typedef enum enumRobotInchType{
	eRitSub = -1,		// 减少
	eRitAdd = 1,		// 增加
}RobotInchType;

// 机器人坐标系类型
typedef enum enumRobotCoordType{
	eRctDefualt = 0,	// 未标注
	eRctRightHand,		// 右手坐标系
	eRctLeftHand		// 左手坐标系
}RobotCoordType;

// 机器人轴位置
typedef struct tagRobotPos
{
	double m_dPosX;			// X轴
	double m_dPosY;			// Y轴
	double m_dPosR;			// R轴
	double m_dPosZ;			// Z轴
	double m_dPosA;			// A轴
	double m_dPosB;			// B轴
	int	   m_nCoordType;	// 0:没有设置；1：右手坐标系；2：左手坐标系

	tagRobotPos()
	{
		m_dPosX = 0.0;
		m_dPosY = 0.0;
		m_dPosR = 0.0;
		m_dPosZ = 0.0;
		m_dPosA = 0.0;
		m_dPosB = 0.0;
		m_nCoordType = 1;
	}
}RobotPos;

#define RobotTeachSGIntMaxNum			5	// 机器人整型常量总数
#define RobotTeachSGRealMaxNum			5	// 机器人实数常量总数

#define RobotTeachPosTotalMaxNum		30	// 机器人示教点总数
#define RobotTeachPosMaxPageNum			6	// 机器人示教点分页数
#define RobotTeachPosMaxNumPerPage		5	// 机器人每页的示教点位数量
// 机器人示教信息
typedef struct tagRobotTeachInfo
{
	int m_nMSpeed;									// 手动移动速度 （0~99）
	std::vector<RobotPos> m_vrpRobotTeachPos;		// 示教点位置信息
	std::vector<BOOL>	  m_vbRobotTearchPosValid;	// 示教点位置信息是否有效

	std::vector<int>	  m_vnRobotTeachSGInt;		// 机器人整型常量信息 
	std::vector<BOOL>	  m_vbRobotTeachSGIntValid;	// 机器人整型常量信息是否有效

	std::vector<double>	  m_vdRobotTeachSGReal;		// 机器人实数常量信息 
	std::vector<BOOL>	  m_vbRobotTeachSGRealValid;// 机器人实数常量信息是否有效

	tagRobotTeachInfo()
	{
		m_nMSpeed = 20;
		m_vrpRobotTeachPos.resize(RobotTeachPosTotalMaxNum);
		m_vbRobotTearchPosValid.resize(RobotTeachPosTotalMaxNum, FALSE);

		m_vnRobotTeachSGInt.resize(RobotTeachSGIntMaxNum, 0);
		m_vbRobotTeachSGIntValid.resize(RobotTeachSGIntMaxNum, FALSE);

		m_vdRobotTeachSGReal.resize(RobotTeachSGRealMaxNum, 0);
		m_vbRobotTeachSGRealValid.resize(RobotTeachSGRealMaxNum, FALSE);

	}
}CRobotTeachInfo;

// 机器人示教范围信息
typedef struct tagRobotTeachRangeInfo
{
	CString m_strPassword;								// 示教范围操作密码
	int m_nMSpeedMax;									// 手动移动速度 （0~99）
	int m_nMSpeedMin;									// 手动移动速度 （0~99）
	BOOL m_bMSpeedRangeEnable;							// 手动移动速度限制是否启用			 
	RobotPos			  m_rpCurRobotPosMax;			// 当前位置最大
	RobotPos			  m_rpCurRobotPosMin;			// 当前位置最小
	BOOL				  m_bCurRobotPosRangeEnable;	// 当前位置限制是否启用	

	int	m_nRobotTeachPosStartPoint;						// 示教起始点
	std::vector<RobotPos> m_vrpRobotTeachPosMax;		// 示教点位置信息最大
	std::vector<RobotPos> m_vrpRobotTeachPosMin;		// 示教点位置信息最小
	std::vector<BOOL>	  m_vbRobotTeachPosRangeEnable;// 示教点位置信息是否启用
	std::vector<CString>  m_vstrRobotTeachPosName;		// 示教点位置名称
	std::vector<CString>  m_vstrRobotTeachPosPageName;	// 示教点位置示教页名称

	CString				  m_strRobotTeachSGPageName;		// 常量页

	int	m_nRobotTeachSGIntStartPoint;						// 示教整型常量起始点
	std::vector<int> m_vnRobotTeachSGIntMax;				// 示教整型常量信息最大
	std::vector<int> m_vnRobotTeachSGIntMin;				// 示教整型常量信息最小
	std::vector<BOOL>	  m_vbRobotTeachSGIntRangeEnable;	// 示教整型常量信息是否启用
	std::vector<CString>  m_vstrRobotTeachSGIntName;		// 示教整型常量名称

	int	m_nRobotTeachSGRealStartPoint;						// 示教实数常量起始点
	std::vector<double> m_vdRobotTeachSGRealMax;			// 示教实数常量信息最大
	std::vector<double> m_vdRobotTeachSGRealMin;			// 示教实数常量信息最小
	std::vector<BOOL>	  m_vbRobotTeachSGRealRangeEnable;	// 示教实数常量信息是否启用
	std::vector<CString>  m_vstrRobotTeachSGRealName;		// 示教实数常量名称

	tagRobotTeachRangeInfo()
	{
		m_strPassword = _T("123456");

		m_nMSpeedMax = 100;									// 手动移动速度 （0~99）
		m_nMSpeedMin = 0;									// 手动移动速度 （0~99）
		m_bMSpeedRangeEnable = FALSE;						// 手动移动速度限制是否启用		

		RobotPos rpRobotPosMax;
		RobotPos rpRobotPosMin;
		BOOL     bRobotPosRangeEnable = FALSE;
		rpRobotPosMax.m_dPosX = +1000;
		rpRobotPosMax.m_dPosY = +1000;
		rpRobotPosMax.m_dPosR = +360;
		rpRobotPosMax.m_dPosZ = +1000;
		rpRobotPosMax.m_dPosA = +1000;
		rpRobotPosMax.m_dPosB = +1000;

		rpRobotPosMin.m_dPosX = -1000;
		rpRobotPosMin.m_dPosY = -1000;
		rpRobotPosMin.m_dPosR = -360;
		rpRobotPosMin.m_dPosZ = -1000;
		rpRobotPosMin.m_dPosA = -1000;
		rpRobotPosMin.m_dPosB = -1000;

		m_rpCurRobotPosMax = rpRobotPosMax;
		m_rpCurRobotPosMin = rpRobotPosMin;
		m_bCurRobotPosRangeEnable = bRobotPosRangeEnable;

		m_nRobotTeachPosStartPoint = 101;

		m_vrpRobotTeachPosMax.resize(RobotTeachPosTotalMaxNum, rpRobotPosMax);
		m_vrpRobotTeachPosMin.resize(RobotTeachPosTotalMaxNum, rpRobotPosMin);
		m_vbRobotTeachPosRangeEnable.resize(RobotTeachPosTotalMaxNum, bRobotPosRangeEnable);

		CString strInfo;
		int i = 0;

		m_vstrRobotTeachPosName.resize(RobotTeachPosTotalMaxNum);
		for(i = 0; i < RobotTeachPosTotalMaxNum; i++)
		{
			strInfo.Format(_T("位置%d"), i+1);
			m_vstrRobotTeachPosName[i] = strInfo;
		}

		m_vstrRobotTeachPosPageName.resize(RobotTeachPosMaxPageNum);
		for(i = 0; i < RobotTeachPosMaxPageNum; i++)
		{
			strInfo.Format(_T("示教页%d"), i+1);
			m_vstrRobotTeachPosPageName[i] = strInfo;
		}



		strInfo.Format(_T("常量页"));
		m_strRobotTeachSGPageName = strInfo;

		int nSGIntMax = 10;
		int nSGIntMin = -10;
		BOOL bSGIntRangeEnable = FALSE;

		m_nRobotTeachSGIntStartPoint = 0;

		m_vnRobotTeachSGIntMax.resize(RobotTeachSGIntMaxNum, nSGIntMax);
		m_vnRobotTeachSGIntMin.resize(RobotTeachSGIntMaxNum, nSGIntMin);
		m_vbRobotTeachSGIntRangeEnable.resize(RobotTeachSGIntMaxNum, bSGIntRangeEnable);

		m_vstrRobotTeachSGIntName.resize(RobotTeachSGIntMaxNum);
		for(i = 0; i < RobotTeachSGIntMaxNum; i++)
		{
			strInfo.Format(_T("整型常量%d"), i+1);
			m_vstrRobotTeachSGIntName[i] = strInfo;
		}



		int nSGRealMax = 1;
		int nSGRealMin = -1;
		BOOL bSGRealRangeEnable = FALSE;

		m_nRobotTeachSGRealStartPoint = 0;

		m_vdRobotTeachSGRealMax.resize(RobotTeachSGRealMaxNum, nSGIntMax);
		m_vdRobotTeachSGRealMin.resize(RobotTeachSGRealMaxNum, nSGIntMin);
		m_vbRobotTeachSGRealRangeEnable.resize(RobotTeachSGRealMaxNum, bSGIntRangeEnable);

		m_vstrRobotTeachSGRealName.resize(RobotTeachSGRealMaxNum);
		for(i = 0; i < RobotTeachSGRealMaxNum; i++)
		{
			strInfo.Format(_T("实数常量%d"), i+1);
			m_vstrRobotTeachSGRealName[i] = strInfo;
		}

	}
}CRobotTeachRangeInfo;


// FAST读写寄存器
//////////////////////////////////////////////////////////////////////////
// 	1	FRST
// FRST:重新设定（清除错误等），清除各种错误及显示重新设定
// 参数 －
// 回复 －

// 发送：（[STX]FRST[ETX]）
// 回复：（[STX]FRST[ETX]）
// typedef struct tagVFRSTParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	tagVFRSTParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFRST;
// 		m_nCmdVal = 1;
// 		m_nPNum = 0;
// 	}
// 
// }VFRSTParam;
// // 	2	FHMS(暂无)
// typedef struct tagVFHMSParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	tagVFHMSParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFHMS;
// 		m_nCmdVal = 2;
// 		m_nPNum = 0;
// 	}
// 
// }VFHMSParam;
// // 	3	FHOM(暂无)
// typedef struct tagVFHOMParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	tagVFHOMParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFHOM;
// 		m_nCmdVal = 3;
// 		m_nPNum = 0;
// 	}
// 
// }VFHOMParam;
// // 	4	FSPC
// //      FSPC：产品切换的设定，将产品切换至指定的产品No.
// // 参　数	
// // 		Ｐ1	１～２５０：类型编号
// // 		Ｐ2	１：辅助编号：通常设定为“1”。
// // 回　复
// // 	   Ｒ１	－１：异常停止 ０：正常停止
// 
// // 发送：（[STX]FSPC P1 P2[ETX]）
// // 回复：（[STX]FSPC R1[ETX]）
// typedef struct tagVFSPCParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int P1;			// １～２５０：类型编号
// 	int P2;			// １：辅助编号：通常设定为“1”
// 	tagVFSPCParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFSPC;
// 		m_nCmdVal = 4;
// 		m_nPNum = 2;
// 		P1=1;
// 		P2=1;
// 	}
// }VFSPCParam;
// // 	5	FCLB
// //      FCLB:Calibration的执行，执行Calibration
// // 参　数	
// // 		Ｐ1	０：目标 靶标 	１：对象　靶标（通常设定为“1”。不支持“0”）
// // 		Ｐ2	１～４：靶标№
// // 回 复
// // 		Ｒ１	－１：异常停止 ０：正常停止
// 
// // 发送：（[STX]FCLB P1 P2[ETX]）
// // 回复：（[STX]FCLB R1[ETX]）
// 
// //标定命令参数
// typedef struct tagVFCLBParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;			// 0:目标mark 1: 物体mark(通常设置为"1",不支持"0")
// 	int P2;			// 1~4: Mark No  单相机:在第N个位置做标定 多相机：第N个相机做标定
// 	double P3;		// 轴1位置
// 	double P4;		// 轴2位置
// 	double P5;		// 轴3位置
// 	tagVFCLBParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFCLB;
// 		m_nCmdVal = 5;
// 		m_nPNum = 5;
// 		P1=1;
// 		P2=1;
// 		P3=0;
// 		P4=0;
// 		P5=0;
// 	}
// }VFCLBParam;
// // 	6	FTGT
// //      FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
// // 参　数	
// // 			Ｐ1	    １～４  ：靶标№
// // 	　　			    ５  ：靶标连续检出
// // 		   　		８  ：提取目标靶标位置
// // 					９  ：保存目标靶标位置
// // 回 复
// // 		    Ｒ１	－１：异常停止  ０：正常停止
// 
// // 发送：（[STX]FTGT P1[ETX]）
// // 回复：（[STX]FTGT R1[ETX]）
// 
// typedef struct tagVFTGTParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;			// 1~4: Mark no 5：Mark连续搜索
// 	double P2;		// 轴1位置
// 	double P3;		// 轴2位置
// 	double P4;		// 轴3位置
// 
// 	tagVFTGTParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFTGT;
// 		m_nCmdVal = 6;
// 		m_nPNum = 4;
// 		m_nPType = -1;
// 		P1=1;
// 		P2=0;
// 		P3=0;
// 		P4=0;
// 	}
// 
// }VFTGTParam;
// // 	7	FOBJ
// //      FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
// // 参　数	
// // 		Ｐ1	１～４  ：靶标№
// // 	　　		５  ：连续搜索靶标
// // 			11～14：手动搜索靶标1－靶标4
// // 			15  ：连续手动搜索靶标
// // 回 复
// // 		Ｒ１	－１：异常停止  ０：正常停止	  １：取消停止
// 
// // 发送：（[STX]FOBJ P1[ETX]）
// // 回复：（[STX]FOBJ R1[ETX]）
// 
// 
// typedef struct tagVFOBJParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;				// 1~4: Mark no 5：Mark连续搜索
// 	double P2;		// 轴1位置
// 	double P3;		// 轴2位置
// 	double P4;		// 轴3位置
// 
// 	tagVFOBJParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFOBJ;
// 		m_nCmdVal = 7;
// 		m_nPNum = 4;
// 		m_nPType = -1;
// 		P1=1;
// 		P2=0;
// 		P3=0;
// 		P4=0;
// 	}
// 
// }VFOBJParam;
// // 	8	FAAL
// //      FAAL:自动对准的执行，执行自动对准（连续对准）
// // 参数
// // 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// // 			１：参照内部补偿量值（参照参数设定值）
// // 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// // 			６：参照外部补偿量值（参照取得的自动补偿量值）
// // 回复 
// // 		Ｒ１ －１：异常结束 (0)* ０：正常结束 (255)*
// 
// // 注：另外一种解释：
// // 参　数	
// // 		Ｐ1	０：不参照偏移量值。（偏移量值为0）
// // 			１、１０～１４：参照内部偏移量值（参照参数设定值）
// //		P2  轴1位置X
// //		P3  轴2位置Y
// //		P4  轴3位置D
// // 回 复
// // 		Ｒ１	－１：异常停止 ０：正常停止
// //（自动迭代对位指令）
// 
// // 发送：（[STX]FAAL P1[ETX]）
// // 回复：（[STX]FAAL R1[ETX]）
// typedef struct tagVFAALParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;	// 指令参数的最大个数数量
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
// 	double P2;		// 轴1位置
// 	double P3;		// 轴2位置
// 	double P4;		// 轴3位置
// 
// 	tagVFAALParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFAAL;
// 		m_nCmdVal = 8;
// 		m_nPNum = 4;
// 		m_nPType = -1;
// 		P1=1;
// 		P2=0;
// 		P3=0;
// 		P4=0;
// 	}
// 
// }VFAALParam;
// // 	9	FNPS
// //      MNPS:平台现在位置的取得，取得平台现在位置
// // 参数 －
// // 回复
// // 		Ｒ１ －１：异常结束 ０：正常结束
// // 					ＸＹθ轴平台时
// // 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的现在位置
// // 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的现在位置
// // 		Ｒ４ －2147483648～2147483647（１／１０００００度单位）：θ轴的现在位置
// // 					ＵＶＷ轴平台时
// // 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的现在位置
// // 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的现在位置
// // 		Ｒ４ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｗ轴的现在位置
// 
// // 视觉主动发送，PLC被动响应
// // 发送：（[STX]MNPS[ETX]）
// // 回复：（[STX]MNPS R1 R2 R3 R4[ETX]）
// typedef struct tagVMNPSParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int P1;				// 		Ｒ１ －１：异常结束 ０：正常结束
// 	double P2;				// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的现在位置
// 	double P3;				// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ方向的现在位置
// 	double P4;				// －2147483648～2147483647（１／１０００００度单位）：θ方向的现在位置
// 
// 	int m_nVPNum;		// 视觉发送参数个数
// 
// 	tagVMNPSParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFMPS;
// 		m_nCmdVal = 9;
// 		m_nPNum = 4;
// 		P1=0;
// 		P2=0;
// 		P3=0;
// 		P4=0;
// 
// 		m_nVPNum= 0;
// 	}
// 
// }VMNPSParam;
// // 	10	FMVA
// //      MMVA:平台絶对准置移动量的设定，设定平台絶对准置移动量
// // 参数
// // 							ＸＹθ轴平台时
// // 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的絶对移动量
// // 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的絶对移动量
// // 		Ｐ３ －2147483648～2147483647（１／１０００００度单位）：θ轴的絶对移动量
// // 							ＵＶＷ轴平台时
// // 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的絶对移动量
// // 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的絶对移动量
// // 		Ｐ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｗ轴的絶对移动量
// // 回复 
// // 		Ｒ１ －１：异常结束 ０：正常结束
// 
// // 视觉主动发送，PLC被动响应
// // 发送：（[STX]MMVA P1 P2 P3[ETX]）
// // 回复：（[STX]MMVA R1[ETX]）
// typedef struct tagVMMVAParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int P1;				// 		Ｒ１ －１：异常结束 ０：正常结束
// 
// 	int m_nVPNum;		// 视觉发送参数个数
// 	double VP1;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
// 	double VP2;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
// 	double VP3;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
// 
// 	tagVMMVAParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFMVA;
// 		m_nCmdVal = 10;
// 		m_nPNum = 1;
// 		P1=0;
// 
// 		m_nVPNum = 3;
// 		VP1 = 0;
// 		VP2 = 0;
// 		VP3 = 0;
// 	}
// 
// }VMMVAParam;
// // 	11	FMVR
// //      MMVR:平台相对准置移动量的设定，设定平台相对准置移动量
// // 参数
// // 						ＸＹθ轴平台时
// // 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的相对移动量
// // 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的相对移动量
// // 		Ｐ３ －2147483648～2147483647（１／１０００００度单位）：θ轴的相对移动量
// // 						ＵＶＷ轴平台时
// // 		Ｐ１ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的相对移动量
// // 		Ｐ２ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的相对移动量
// // 		Ｐ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｗ轴的相对移动量
// // 回复		
// // 		Ｒ１ －１：异常结束 ０：正常结束
// 
// // 视觉主动发送，PLC被动响应
// // 发送：（[STX]MMVR P1 P2 P3[ETX]）
// // 回复：（[STX]MMVR R1[ETX]）
// typedef struct tagVMMVRParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int P1;				// 		Ｒ１ －１：异常结束 ０：正常结束
// 
// 	int m_nVPNum;		// 视觉发送参数个数
// 	double VP1;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
// 	double VP2;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
// 	double VP3;			// －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ方向的絶对移动量
// 
// 	tagVMMVRParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFMVR;
// 		m_nCmdVal = 11;
// 		m_nPNum = 1;
// 		P1=0;
// 
// 		m_nVPNum = 3;
// 		VP1 = 0;
// 		VP2 = 0;
// 		VP3 = 0;
// 	}
// 
// }VMMVRParam;
// // 	12	FPNT(暂无)
// typedef struct tagVFPNTParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	tagVFPNTParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFPNT;
// 		m_nCmdVal = 12;
// 		m_nPNum = 0;
// 	}
// 
// }VFPNTParam;
// // 	13	FACO
// //      FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
// // 参数 －
// // 回复
// // 		Ｒ１ －１：异常结束 ０：正常结束
// // 		Ｒ２ －2147483648～2147483647 Ｘ方向的偏移量（１／10000mm 单位）
// // 		Ｒ３ －2147483648～2147483647 Ｙ方向的偏移量（１／10000mm 单位）
// // 		Ｒ４ －2147483648～2147483647 θ方向的偏移量（１／100000 度单位）
// //（根据已经获得的目标和对象的位置，进行对位计算，获得目标和对象之间的偏差，但不考虑各种补偿量
// //（注：目标和对象，已经通过其他指令获得，此过程不用再重新获得了））
// 
// // 发送：（[STX]FACO[ETX]）
// // 回复：（[STX]FACO R1 R2 R3 R4[ETX]）
// typedef struct tagVFACOParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
// 	tagVFACOParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFACO;
// 		m_nCmdVal = 13;
// 		m_nPNum = 1;
// 		P1 = 0;
// 	}
// }VFACOParam;
// // 	14	FALG
// //      FALG:１次对准移动量的取得，取得１次对准移动量
// // 参数
// // 		Ｐ１	 ０：不参照补偿量值（补偿量值为０）
// // 			 １：参照内部补偿量值（参照参数设定值）
// // 			 ５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// // 			 ６：参照外部补偿量值（参照取得的自动补偿量值）
// // 回复
// // 		Ｒ１ －１：异常结束 ０：正常结束
// // 
// // 				ＸＹθ轴平台时
// // 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｘ轴的絶对值移动量
// // 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：Ｙ轴的絶对值移动量
// // 		Ｒ４ －2147483648～2147483647（１／１０００００度单位）：θ轴的絶对值移动量
// // 				ＵＶＷ轴平台时
// // 		Ｒ２ －2147483648～2147483647（１／１００００ｍｍ单位）：U 轴的絶对值移动量
// // 		Ｒ３ －2147483648～2147483647（１／１００００ｍｍ单位）：V 轴的絶对值移动量
// // 		Ｒ４ －2147483648～2147483647（１／１００００ｍｍ单位）：W 轴的絶对值移动量
// //（取得一次对位指令后平台移动量，根据已经获得的目标和对象的位置，进行对位计算，获得相应的平台移动量
// //（注：目标和对象，已经通过其他指令获得，此过程不用再重新获得了））
// 
// // 发送：（[STX]FALG P1[ETX]）
// // 回复：（[STX]FALG R1 R2 R3 R4[ETX]）
// typedef struct tagVFALGParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
// 	//	int P2;		// 1~5: 开始候补搜索号码
// 
// 	tagVFALGParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFALG;
// 		m_nCmdVal = 14;
// 		m_nPNum = 1;
// 		P1=1;
// 	}
// 
// }VFALGParam;
// // 	15	FCNT
// //      FCNT:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置(目标位置取得为图像中心)
// // 参　数	
// // 		Ｐ1	１～４  ：靶标№
// // 	　　		５  ：连续搜索靶标
// // 			11～14：手动搜索靶标1－靶标4
// // 			15  ：连续手动搜索靶标
// // 回 复
// // 		Ｒ１	－１：异常停止  ０：正常停止	  １：取消停止
// 
// // 发送：（[STX]FCNT P1[ETX]）
// // 回复：（[STX]FCNT R1[ETX]）
// typedef struct tagVFCNTParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;				// 1~4: Mark no 5：Mark连续搜索
// 	double P2;		// 轴1位置
// 	double P3;		// 轴2位置
// 	double P4;		// 轴3位置
// 
// 	tagVFCNTParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFCNT;
// 		m_nCmdVal = 15;
// 		m_nPNum = 4;
// 		m_nPType = -1;
// 		P1=1;
// 		P2=0;
// 		P3=0;
// 		P4=0;
// 	}
// 
// }VFCNTParam;
// // 	16	FGAP
// // 发送：（[STX]FGAP P1[ETX]）
// // 回复：（[STX]FGAP R1 R2 R3[ETX]）
// typedef struct tagVFGAPParam
// {
// 	CString m_strCmd;
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;			 // 1~4: Mark no
// 	int P2;          // 1~5: Pat no
// 	tagVFGAPParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFGAP;//_T("FGAP");
// 		m_nCmdVal= 16;
// 		m_nPNum = 2;
// 		m_nPType = -1;
// 		P1=1;
// 		P2=1;
// 	}
// }VFGAPParam;
// // 	17	FTGC
// //      FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
// // 参　数	
// // 			Ｐ1	１～４：靶标№
// // 　				５：连续搜索靶标
// // 			Ｐ２	１～５：候补检索开始的编号
// // 				１０：从上次成功的候补编号开始
// //  回 复
// // 			Ｒ１		＊目标成对进行候补检索
// // 						1～５：正常停止后的候补检索编号
// //  　					－1：异常停止
// // 			Ｒ１		＊独立进行目标的候补检索时
// //   　					０：正常停止
// // 						－１：异常停止
// 
// // 发送：（[STX]FTGC P1 P2[ETX]）
// // 回复：（[STX]FTGC R1[ETX]）
// 
// typedef struct tagVFTGCParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;			// 1~4: Mark no 5：Mark连续搜索
// 	int P2;			// 1~5: 开始候补搜索号码
// 	double P3;		// 轴1位置
// 	double P4;		// 轴2位置
// 	double P5;		// 轴3位置
// 	tagVFTGCParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFTGC;
// 		m_nCmdVal = 17;
// 		m_nPNum = 5;
// 		m_nPType = -1;
// 		P1=1;
// 		P2=1;
// 		P3=0;
// 		P4=0;
// 		P5=0;
// 	}
// }VFTGCParam;
// // 	18	FOBC
// //      FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
// // 参　数	
// // 		Ｐ1	１～４：靶标№
// // 　		５：连续进行靶标检出
// // 		Ｐ２	１～５：候补搜索开始的编号
// // 			１０：从上次成功的候补编号开始
// //  回 复
// // 		Ｒ１	＊成对进行目标靶标候补搜索时
// // 				1～５：正常停止的候补搜索号码
// //  　			－1：异常停止
// // 		Ｒ１	＊单独进行目标　靶标候补搜索时
// // 				０：正常停止
// // 				－１：异常停止
// 
// // 发送：（[STX]FOBC P1 P2[ETX]）
// // 回复：（[STX]FOBC R1[ETX]）
// typedef struct tagVFOBCParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;				// 1~4: Mark no 5：Mark连续搜索
// 	int P2;				// 1~5: 开始候补搜索号码
// 	double P3;		// 轴1位置
// 	double P4;		// 轴2位置
// 	double P5;		// 轴3位置
// 
// 	tagVFOBCParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFOBC;
// 		m_nCmdVal = 18;
// 		m_nPNum = 5;
// 		P1=1;
// 		P2=1;
// 		P3=0;
// 		P4=0;
// 		P5=0;
// 	}
// 
// }VFOBCParam;
// // 	19	FAAC
// //      FAAC:自动对准的执行，执行自动对准（连续对准），对象候补搜索
// // 参数
// // 		Ｐ１	０：不参照补偿量值（补偿量值为０）
// // 			１：参照内部补偿量值（参照参数设定值）
// // 			５：参照外部补偿量值（参照“ＦＯＦＳ”设定值）
// // 			６：参照外部补偿量值（参照取得的自动补偿量值）
// // 		Ｐ2	０:候补对象序号
// //		P3;	（新增可选参数，原Fast指令中没有该参数）0:不进行对象基准对位；1：进行对象基准对位
// // 回复 
// // 		Ｒ１ －１：异常结束 (0)* ０：正常结束 (255)*
// 
// // 注：另外一种解释：
// // 参　数	
// // 		Ｐ1	０：不参照偏移量值。（偏移量值为0）
// // 			１、１０～１４：参照内部偏移量值（参照参数设定值）
// // 回 复
// // 		Ｒ１	－１：异常停止 ０：正常停止
// //（自动迭代对位指令）
// 
// // 发送：（[STX]FAAL P1[ETX]）
// // 回复：（[STX]FAAL R1[ETX]）
// typedef struct tagVFAACParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
// 	int P2;			// 1~5: 开始候补搜索号码
// 	double P3;		// 轴1位置
// 	double P4;		// 轴2位置
// 	double P5;		// 轴3位置
// 
// 	tagVFAACParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFAAC;
// 		m_nCmdVal = 19;
// 		m_nPNum = 5;
// 		m_nPType = -1;
// 		P1=1;
// 		P2=1;
// 		P3=0;
// 		P4=0;
// 		P5=0;
// 	}
// 
// }VFAACParam;
// // 	20	FGSP
// //      FGSP：产品No.的取得，取得目前正在执行的产品No.
// // 参 数
// // 回 复	
// // 		 Ｒ１	－１：异常停止　０：正常停止
// // 		 Ｒ２	１～２５０：类型编号　
// // 		 Ｒ３	１：辅助编号(通常返回“1”）
// 
// // 发送：（[STX]FGSP[ETX]）
// // 回复：（[STX]FGSP R1 R2 R3[ETX]）
// typedef struct tagVFGSPParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	// 	int P1;			// ０：当前使用中的类型	１～２５０：类型编号
// 	// 	int P2;			// １：辅助编号：通常设定为“1”
// 	tagVFGSPParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFGSP;
// 		m_nCmdVal = 20;
// 		m_nPNum = 0;
// 	}
// }VFGSPParam;
// // 	21	FCBS
// //      标定命令参数
// typedef struct tagVFCBSParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;			// 0:目标mark 1: 物体mark(通常设置为"1",不支持"0")
// 	int P2;			// 1~4: Mark No  单相机:在第N个位置做标定 多相机：第N个相机做标定
// 	double P3;		// 轴1位置
// 	double P4;		// 轴2位置
// 	double P5;		// 轴3位置
// 	tagVFCBSParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFCBS;
// 		m_nCmdVal = 21;
// 		m_nPNum = 5;
// 		P1=1;
// 		P2=1;
// 		P3=0;
// 		P4=0;
// 		P5=0;
// 
// 	}
// }VFCBSParam;
// // 	22	FMAP(暂无)
// typedef struct tagVFMAPParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	tagVFMAPParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFMAP;
// 		m_nCmdVal = eCmdValResetFRST;
// 		m_nPNum = 0;
// 	}
// 
// }VFMAPParam;
// // 	23	FXYA
// // 发送：（[STX]FXYA P1[ETX]）
// // 回复：（[STX]FXYA R1[ETX]）
// typedef struct tagVFXYAParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;	// 指令参数的最大个数数量
// 	int	m_nPType;	// 指令参数类型（即实际参数的个数数量）
// 	int P1;			// ０：不参照补偿量值（补偿量值为０）１：参照内部补偿量值（参照参数设定值）
// 	int P2;			// ０：没有使用候补搜索 １～５：候补搜索开始编号
// 	double P3;		// 轴1位置
// 	double P4;		// 轴2位置
// 	double P5;		// 轴3位置
// 
// 	tagVFXYAParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFXYA;
// 		m_nCmdVal = 23;
// 		m_nPNum = 5;
// 		m_nPType = -1;
// 		P1=1;
// 		P2=1;
// 		P3=0;
// 		P4=0;
// 		P5=0;
// 	}
// 
// }VFXYAParam;
// // 	24	FRCT(暂无)
// typedef struct tagVFRCTParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	tagVFRCTParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFRCT;
// 		m_nCmdVal = 24;
// 		m_nPNum = 0;
// 	}
// 
// }VFRCTParam;
// // 	25	FISP(暂无)
// typedef struct tagVFISPParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	tagVFISPParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFISP;
// 		m_nCmdVal = 25;
// 		m_nPNum = 0;
// 	}
// 
// }VFISPParam;
// // 	26	FOLG(暂无)
// typedef struct tagVFOLGParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	tagVFOLGParam()
// 	{
// 		m_strCmd = COMM_CMD_STRING_VISIONFOLG;
// 		m_nCmdVal = 26;
// 		m_nPNum = 0;
// 	}
// 
// }VFOLGParam;
// // 	27	FTLG(暂无)
// typedef struct tagVFTLGParam
// {
// 	CString m_strCmd;	// 指令字符串
// 	int		m_nCmdVal;	// 指令整数值
// 	int m_nPNum;
// 	tagVFTLGParam()
// 	{
//  		m_strCmd = COMM_CMD_STRING_VISIONFTLG;
//  		m_nCmdVal = 27;
//  		m_nPNum = 0;
//  }
// 
// }VFTLGParam;

//线距检测结果（现用于角定位检查测量工具）
typedef struct tagPosL2LDistanceResult
{
	CResultL2LInspectDistance m_L2LDistanceResult;//线距检测结果
	BOOL m_bPosSearchFinished;//该位置是否已被检测
	tagPosL2LDistanceResult()
	{
		m_bPosSearchFinished=FALSE;
	}
}CPosL2LDistanceResult;

// 指令命名码和参数
typedef struct tagKeyenceCodeParam
{
	CString m_strCmd;	// 指令字符串
	int		m_nCmdVal;	// 指令整数值
	BOOL    m_bEnableNonFixedCode;	// 启用非固定指令序列：TRUE启用时，不执行定义的指令序列，而是从PLC指令参数数据中赋值获取； 此时P1-nP5的Size均为1
	std::vector<int>		P1;			// P1参数
	std::vector<int>		P2;			// P2参数
	std::vector<int>		nP3;		// P3参数
	std::vector<int>		nP4;		// P4参数
	std::vector<int>		nP5;		// P5参数

	tagKeyenceCodeParam()
	{
		m_strCmd = _T("FRST");
		m_nCmdVal = -1;
		m_bEnableNonFixedCode = FALSE;

		P1.resize(1);
		P2.resize(1);
		nP3.resize(1);
		nP4.resize(1);
		nP5.resize(1);

		P1.at(0) = 1;
		P2[0] = 1;
		nP3[0] = 0;
		nP4[0] = 0;
		nP5[0] = 0;
	}

	tagKeyenceCodeParam& operator = (const tagKeyenceCodeParam& lTPData)
	{
		if (this == &lTPData)
		{
			return *this;
		}
		m_strCmd = lTPData.m_strCmd;
		m_nCmdVal = lTPData.m_nCmdVal;

		m_bEnableNonFixedCode = lTPData.m_bEnableNonFixedCode;
		P1 = lTPData.P1;
		P2 = lTPData.P2;
		nP3 = lTPData.nP3;
		nP4 = lTPData.nP4;
		nP5 = lTPData.nP5;
		return *this;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		CString strInfo = _T("");
		// m_strCmd
		cXMLConfigurator.AddElem(_T("strCmd"), m_strCmd);
		// m_nCmdVal
		strInfo.Format(_T("%d"), m_nCmdVal);
		cXMLConfigurator.AddElem(_T("nCmdVal"), strInfo);
		// m_bEnableNonFixedCode
		strInfo.Format(_T("%d"), m_bEnableNonFixedCode);
		cXMLConfigurator.AddElem(_T("EnableNonFixedCode"), strInfo);

		// P1.size()
		strInfo.Format(_T("%d"), P1.size());
		cXMLConfigurator.AddElem(_T("nP1Size"), strInfo);

		for (int it=0;it<P1.size();it++)
		{
			strInfo.Format(_T("nParamList%d"), it);
			cXMLConfigurator.AddElem(strInfo);
			cXMLConfigurator.EnterElem();
			{
				// P1
				strInfo.Format(_T("%d"), P1[it]);
				cXMLConfigurator.AddElem(_T("P1"), strInfo);
				// P2
				strInfo.Format(_T("%d"), P2[it]);
				cXMLConfigurator.AddElem(_T("P2"), strInfo);
				// nP3
				strInfo.Format(_T("%d"), nP3[it]);
				cXMLConfigurator.AddElem(_T("nP3"), strInfo);
				// nP4
				strInfo.Format(_T("%d"), nP4[it]);
				cXMLConfigurator.AddElem(_T("nP4"), strInfo);
				// nP5
				strInfo.Format(_T("%d"), nP5[it]);
				cXMLConfigurator.AddElem(_T("nP5"), strInfo);
			}
			cXMLConfigurator.LeaveElem();
		}
	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		CString strInfo = _T("");
		bool bRet = false;
		// m_strCmd
		bRet = cXMLConfigurator.FindElem(_T("strCmd"));
		if (bRet)
		{
			m_strCmd = cXMLConfigurator.GetElemData();
		}
		// m_nCmdVal
		bRet = cXMLConfigurator.FindElem(_T("nCmdVal"));
		if (bRet)
		{
			m_nCmdVal = _ttoi(cXMLConfigurator.GetElemData());
		}

		// m_bEnableNonFixedCode
		bRet = cXMLConfigurator.FindElem(_T("EnableNonFixedCode"));
		if (bRet)
		{
			m_bEnableNonFixedCode = _ttoi(cXMLConfigurator.GetElemData());
		}

		// P1.size()
		bRet = cXMLConfigurator.FindElem(_T("nP1Size"));
		int nSize=1;
		if (bRet)
		{
			nSize = _ttoi(cXMLConfigurator.GetElemData());
		}

		P1.resize(nSize);
		P2.resize(nSize);
		nP3.resize(nSize);
		nP4.resize(nSize);
		nP5.resize(nSize);

		for (int it=0;it<nSize;it++)
		{
			strInfo.Format(_T("nParamList%d"), it);
			bRet = cXMLConfigurator.FindElem(strInfo);
			if(bRet)
			{
				cXMLConfigurator.EnterElem();
				// P1
				bRet = cXMLConfigurator.FindElem(_T("P1"));
				if (bRet)
				{
					P1[it] = _ttoi(cXMLConfigurator.GetElemData());
				}		
				// P2
				bRet = cXMLConfigurator.FindElem(_T("P2"));
				if (bRet)
				{
					P2[it] = _ttoi(cXMLConfigurator.GetElemData());
				}
				// nP3
				bRet = cXMLConfigurator.FindElem(_T("nP3"));
				if (bRet)
				{
					nP3[it] = _ttoi(cXMLConfigurator.GetElemData());
				}
				// nP4
				bRet = cXMLConfigurator.FindElem(_T("nP4"));
				if (bRet)
				{
					nP4[it] = _ttoi(cXMLConfigurator.GetElemData());
				}
				// nP5
				bRet = cXMLConfigurator.FindElem(_T("nP5"));
				if (bRet)
				{
					nP5[it] = _ttoi(cXMLConfigurator.GetElemData());
				}

				cXMLConfigurator.LeaveElem();
			}

		}


		
	}

}CKeyenceCodeParam;

typedef struct tagMulRegTriggerSet
{
	int m_nMulIndex; //段号索引
	int m_nMulRegOffset;// 该段相对基地址的相对偏移量
	BOOL m_bEnable; // 启用强制转换
	int m_nCodeTrigger; //转换前触发码;
	int m_nCodeTrans;	//转换后指令码;
	BOOL m_bParamNumber;     // 启用多命令功能
	int m_nParamListNumber; //多命令索引;

	tagMulRegTriggerSet()
	{
		m_nMulIndex = 1;
		m_nMulRegOffset = 0;
		m_bEnable = FALSE;
		m_nCodeTrigger = 1;
		m_nCodeTrans = 1;
		m_bParamNumber = FALSE;
		m_nParamListNumber = 0;
	}

	tagMulRegTriggerSet& operator = (const tagMulRegTriggerSet& lTPData)
	{
		if (this == &lTPData)
		{
			return *this;
		}
		m_nMulIndex = lTPData.m_nMulIndex;
		m_nMulRegOffset = lTPData.m_nMulRegOffset;

		m_bEnable = lTPData.m_bEnable;
		m_nCodeTrigger = lTPData.m_nCodeTrigger;
		m_nCodeTrans = lTPData.m_nCodeTrans;
		m_bParamNumber =  lTPData.m_bParamNumber;
		m_nParamListNumber = lTPData.m_nParamListNumber;
		return *this;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		CString strInfo = _T("");
		// m_nMulIndex
		strInfo.Format(_T("nMulIndex"));
		if(FALSE==cXMLConfigurator.FindElem(strInfo))
		{
			cXMLConfigurator.AddElem(strInfo);
		}
		cXMLConfigurator.SetElemData(m_nMulIndex);

		// m_nMulRegOffset
		strInfo.Format(_T("nMulRegOffset"));
		if(FALSE==cXMLConfigurator.FindElem(strInfo))
		{
			cXMLConfigurator.AddElem(strInfo);
		}
		cXMLConfigurator.SetElemData(m_nMulRegOffset);

		// m_bEnable
		strInfo.Format(_T("bEnable"));
		if(FALSE==cXMLConfigurator.FindElem(strInfo))
		{
			cXMLConfigurator.AddElem(strInfo);
		}
		cXMLConfigurator.SetElemData((int)m_bEnable);

		// m_nCodeTrigger
		strInfo.Format(_T("nCodeTrigger"));
		if(FALSE==cXMLConfigurator.FindElem(strInfo))
		{
			cXMLConfigurator.AddElem(strInfo);
		}
		cXMLConfigurator.SetElemData(m_nCodeTrigger);
		
		// m_nCodeTrans
		strInfo.Format(_T("nCodeTrans"));
		if(FALSE==cXMLConfigurator.FindElem(strInfo))
		{
			cXMLConfigurator.AddElem(strInfo);
		}
		cXMLConfigurator.SetElemData(m_nCodeTrans);

		// m_bParamNumber
		strInfo.Format(_T("nParamNumber"));
		if(FALSE==cXMLConfigurator.FindElem(strInfo))
		{
			cXMLConfigurator.AddElem(strInfo);
		}
		cXMLConfigurator.SetElemData((int)m_bParamNumber);

		// m_nParamListNumber
		strInfo.Format(_T("nParamListNumber"));
		if(FALSE==cXMLConfigurator.FindElem(strInfo))
		{
			cXMLConfigurator.AddElem(strInfo);
		}
		cXMLConfigurator.SetElemData(m_nParamListNumber);
	
	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		CString strInfo = _T("");
		bool bRet = false;

		bRet = cXMLConfigurator.FindElem(_T("nMulIndex"));
		if (bRet)
		{
			m_nMulIndex = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("nMulRegOffset"));
		if (bRet)
		{
			m_nMulRegOffset = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("bEnable"));
		if (bRet)
		{
			m_bEnable = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("nCodeTrigger"));
		if (bRet)
		{
			m_nCodeTrigger = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("nCodeTrans"));
		if (bRet)
		{
			m_nCodeTrans = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("nParamNumber"));
		if (bRet)
		{
			m_bParamNumber = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("nParamListNumber"));
		if (bRet)
		{
			m_nParamListNumber = _ttoi(cXMLConfigurator.GetElemData());
		}

	}

}CMulRegTriggerSet;

//////////////////////////////////////////////////////////////////////////
class CCommProtocolXYDFast: public CCommProtocol
{
public:
	CCommProtocolXYDFast();
	virtual ~CCommProtocolXYDFast();
public:
	// 设置、获取协议类型
	virtual void SetProtocolType(ProtocolType ptProtocolType);
	// 判断当前的协议类型是否为PLC链接协议，直接读取存储PLC寄存器的
	virtual BOOL IsPlcProtocolType();

public:
	// 
	virtual	SysWorkCmdType AnalyzeSysWorkCmdReceived(std::vector<int> vnReceivedData);
	BOOL PackWriteRegInfo( CString& strInfo, CString strRegAddr, int dWriteNum, std::vector<double> vWriteData );
	BOOL PackWriteRegInfo( CString& strInfo, CString strRegAddr, int dWriteNum, std::vector<int> vWriteData );
	BOOL PackReadRegInfo( CString& strInfo, CString strRegAddr, int nReadNum );
	BOOL AnalyzeAswWriteReg( CString strAsw );
	BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<double> &vReceiveData );
	BOOL AnalyzeAswReadReg( CString strAsw, int nReadNum, std::vector<int> &vReceiveData );

	//线距检测相关
	BOOL m_bEnablePackInspectDistance;//是否允许发送线距检测结果
	vector<CPosL2LDistanceResult> m_vstL2LDistanceResult;//各位置线距检测结果
	BOOL ClearL2LDistanceResult();//重置各位置线距检测结果
	//////////////////////////////////////////////////////////////////////////
	// 分析收到PLC或其他主控系统的指令
	virtual SysWorkCmdType AnalyzeSysWorkCmdReceived(CString strCmd); 
	BOOL GetCmdName(CString& strName);

	//////////////////////////////////////////////////////////////////////////
	//   打包发送给PLC或其他主控系统的指令
	//lzk 0408
	// FVPC
	BOOL PackReSetPositionVirtualFVPC(CString& strInfo,BOOL bResult);	

	// 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
	BOOL PackReSetProductIndexFSPC(CString& strInfo,BOOL bResult);	
	// 2、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
	BOOL PackReSaveProductIndexFSSV(CString& strInfo,BOOL bResult);
	// 3、 FGSP：产品No.的取得，取得目前正在执行的产品No.
	BOOL PackReGetProductIndexFGSP(CString& strInfo,BOOL bResult, int nCurProductIndex, int nAssitIndex = 1);

	// 1、 FVAS：产品虚拟Mark开关设置
	BOOL PackReSetProductVirtualAlnFVAS(CString& strInfo,BOOL bResult);	

	//回复PLC目标补偿计算结果
	BOOL PackReCalTargetFixOffsetFAFO(CString& strInfo,BOOL bResult);
	//目标对象不在同一个控制器且目标端二维码拍照时，回复给PLC目标平台坐标，轴位置、MARK点二维码靶标位置
	BOOL PackReCalTargetFixOffsetFAFO(CString& strInfo,BOOL bResult,std::vector<int> vnPosIndex,std::vector<CCoordPos>& vCoordPos,std::vector<CPlatformXYDAxisPos>& vPlatformAxis,std::vector<CCoordPos>& vBoardPos);
	// 1、FCLB:Calibration 的执行 执行Calibration
	BOOL PackReCalibrateFCLB(CString& strInfo,BOOL bResult);
	// 1、FCBS:Calibration 的执行 执行Calibration
	BOOL PackReCalibrateFCBS(CString& strInfo,BOOL bResult);
	// 1、FCBR:关联Calibration 的执行 执行Calibration
	BOOL PackReCalibrateFCBR(CString& strInfo,BOOL bResult);
	// 1、FCBP:关联Calibration 的执行 执行Calibration
	BOOL PackReCalibrateFCBP(CString& strInfo,BOOL bResult);

	// 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
	BOOL PackReTargetSearchFTGT(CString& strInfo,BOOL bResult);      
	// 1.1、FTGS:目标 Mark 位置的取得，取得目标 Mark（目标）位置，目标平台轴位置一致
	BOOL PackReTargetSearchFTGS(CString& strInfo,BOOL bResult); 
	// 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
	BOOL PackReTargetSearchFTGC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 1);      
	// 3、FTGR:目标 Mark 位置的登录，将目前使用的产品数据保存在指定的产品No.中并保存在CF 记忆卡
	BOOL PackReSaveTargetPosFTGR(CString& strInfo,BOOL bResult);	  
	// 4、FTGG:目标 Mark 画素的取得，取得执行“ＦＴＧＴ”结果的画素
	BOOL PackReGetTargetImagePosFTGG(CString& strInfo,BOOL bResult, double dImagePosX, double dImagePosY); 
	// 5、FTGD:目标 Mark 指示线的全画面显示，在指定目标 Mark No.的目标 Mark 位置上显示全画面的指示线及刻度
	BOOL PackReDisplayTargetFTGD(CString& strInfo,BOOL bResult);	 
	// 6、FTGM:目标 Mark 位置的手动取得，取得目标 Mark（目标）位置
	BOOL PackReTargetSearchFTGM(CString& strInfo,BOOL bResult);      
	// 4、FGAP: Mark 画素的取得，取得执行结果的画素
	BOOL PackReGetImagePosFGAP(CString& strInfo,BOOL bResult, double dImagePosX, double dImagePosY,double ImagePosD);
	BOOL PackReGetImagePosFGTC(CString& strInfo,BOOL bResult, double dImagePosX, double dImagePosY,double ImagePosD);

	// 1、FCNT:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
	BOOL PackReObjectSearchFCNT(CString& strInfo,BOOL bResult);      

	BOOL PackReTargetSearchFCNO(CString& strInfo,BOOL bResult);   

	// 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
	BOOL PackReObjectSearchFOBJ(CString& strInfo,BOOL bResult);      
	// 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
	BOOL PackReObjectSearchFOBC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 1);       
	// 3、FOBG:物体 Mark 画素的取得，取得执行“ＦＯＢＪ”结果的画素
	BOOL PackReGetObjectImagePosFOBG(CString& strInfo,BOOL bResult, double dImagePosX, double dImagePosY); 
	// 4、FMSC:手动搜索位置的取得，在自动对准时，当目标 Mark 或物体 Mark 搜索失败时，可利用手动方法（鼠标）指定Mark 位置
	BOOL PackReManualSearchFMSC(CString& strInfo,BOOL bResult);	  
	// 5、FWLD：取得目标（目标靶标）或对象（对象靶标）的平台坐标上的位置
	BOOL PackReGetSearchWorldPosFWLD(CString& strInfo,BOOL bResult, double dPlatPosX, double dPlatPosY);	
	// 5.1、FWDR:获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
	BOOL PackReGetSearchWorldRadiusFWDR(CString& strInfo,BOOL bResult, CCoordPos platOffset);
	// 5.1、FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
	BOOL PackReGetObjectWorldPosFGOW(CString& strInfo,BOOL bResult, CCoordPos platOffset);
	// 6、FOBM:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
	BOOL PackReObjectSearchFOBM(CString& strInfo,BOOL bResult);      

	// 5.2、FGTW:拍照后回复PLC目标平台坐标及轴位置
	BOOL PackReGetTargetPlatformPosFGTW(CString& strInfo,BOOL bResult, std::vector<int> vnPosIndex,std::vector<CCoordPos>& vCoordPos,std::vector<CPlatformXYDAxisPos>& vPlatformAxis);

	// 5.3、FGTP:拍照后回复PLC目标图像坐标及轴位置
	BOOL PackReGetTargetImagePosFGTP(CString& strInfo,BOOL bResult, std::vector<int> vnPosIndex,std::vector<CCoordPos>& vImagePos,std::vector<CPlatformXYDAxisPos>& vPlatformAxis);

	// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
	BOOL PackReGetAlignPrecisionFGAL(CString& strInfo,BOOL bResult, CCoordPos cpAlignPrecision);	
	// 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
	BOOL PackReSetAlignPrecisionFALL(CString& strInfo,BOOL bResult);	
	// 3、FGAC:对准最多次数的取得，取得设定对准最多次数
	BOOL PackReGetAlignTimeFGAC(CString& strInfo,BOOL bResult, int nAlignTime);		
	// 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
	BOOL PackReSetAlignTimeFALC(CString& strInfo,BOOL bResult);		
	// 5、FAAL:自动对准的执行，执行自动对准（连续对准）
//	BOOL PackReAutoAlignFAAL(CString& strInfo,BOOL bResult);			
	 // 6、FANS:对准后的偏移量的取得，取得对准后平台坐标系的偏移量
	BOOL PackReGetAutoAlignOffsetFANS(CString& strInfo,BOOL bResult, CCoordPos cpPlatOffset); 
	// 7、FALG:１次对准移动量的取得，取得１次对准移动量
//	BOOL PackReOnceAlignMoveFALG(CString& strInfo,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);		
	// 8、FALJ:1:次对准精度判定与对准移动量的取得，对准移动量是否在对准精度内的判定与对准移动量的取得
	BOOL PackReOnceAlignFALJ(CString& strInfo,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);			
	// 9、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
//	BOOL PackReGetTargetObjectOffsetFACO(CString& strInfo,BOOL bResult, CCoordPos cpPlatOffset);

	// 1、FAOF:自动补偿量值的取得，对准偏移量以补偿量值取得
	BOOL PackReCalcuAlignOffsetFAOF(CString& strInfo,BOOL bResult);	
	// 2、FGOF:补偿量值的取得，取得设定的补偿量值
	BOOL PackReGetAlignOffsetFGOF(CString& strInfo,BOOL bResult, CCoordPos cpAlignOffset);		
	// 3、FOFS:补偿量值的设定，设定补偿量值
	BOOL PackReSetAlignOffsetFOFS(CString& strInfo,BOOL bResult);		
	// 4、FSFT:平台Shift 移动动作，选择内部转移量的设定值，并依照设定值将目前平台的位置做相对移动
	BOOL PackReFSFT(CString& strInfo,BOOL bResult);					
	// 5、FGSF:对准Shift 移动量的取得，取得对准转移量
	BOOL PackReFGSF(CString& strInfo,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);	

	BOOL PackReGetAlignOffsetFGPO(CString& strInfo, BOOL bResult, CCoordPos& coAlignOffset);

	// 1、FALM:Alarm资料的取得，取得错误发生的数据代码
	BOOL PackReGetAlarmFALM(CString& strInfo,int nAlarmIndex, int nSimData = -1);			
	// 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
//	BOOL PackReResetFRST(CString& strInfo);													
	// 3、FSAC：保存校正、定位时的数据。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
	BOOL PackReSaveProcessDataFSAC(CString& strInfo,BOOL bResult);							
	// 4、FSCO：保存指令收发的历史纪录。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
	BOOL PackReSaveCommDataFSCO(CString& strInfo,BOOL bResult);								

	//检测
	// 2、FCHB:ACF检测
	BOOL PackReACFInspectFCHB(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 1); 
	BOOL PackReACFInspectFCHBD(CString& strInfo,BOOL bResult, int nSuccessPatIndex, std::vector<double> vdDistance);

	// 2、FCHC:候补ACF检测
	BOOL PackReACFInspectFCHC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 1); 

	BOOL PackReFDRP(CString& strInfo,BOOL bResult); 

	// FCCI：复制标定信息
	BOOL PackReCopyCalibrateInfoFCCI(CString& strInfo,BOOL bResult);	

	BOOL PackReClearProductFPEP(CString& strInfo,BOOL bResult);
	BOOL PackReProductCopyFPCP(CString& strInfo,BOOL bResult);
	BOOL PackReProductRenameFPRN(CString& strInfo,BOOL bResult);

	//////////////////////////////////////////////////////////////////////////
// 	// 1、FCLB:Calibration 的执行 执行Calibration
// 	BOOL PackReCalibrateFCLB(CString& strInfo,BOOL bResult);
// 	// 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
// 	BOOL PackReTargetSearchFTGC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 0);
// 	// 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
// 	BOOL PackReTargetSearchFTGT(CString& strInfo,BOOL bResult);
// 	// 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
// 	BOOL PackReObjectSearchFOBJ(CString& strInfo,BOOL bResult);
// 	// 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
// 	BOOL PackReObjectSearchFOBC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 0);

	BOOL PackReAlignResultInfoFACO(CString& strInfo,CCoordPos platOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);

	BOOL PackReAlignResultInfoFACO(CString& strInfo,std::vector<CCoordPos> vPlatOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	BOOL PackReAlignResultInfoFACO(CString& strInfo,CCoordPos platOffset, CCoordPos platOffsetResive, BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);

	BOOL PackReAlignResultInfoFAAO(CString& strInfo,CCoordPos platOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	BOOL PackReAlignResultInfoFOLC(CString& strInfo,CCoordPos platOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	BOOL PackReAlignResultInfoFABO(CString& strInfo,CCoordPos platOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	BOOL PackReAlignResultInfoFAAL(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, BOOL bSendTargetOffset = FALSE, CPlatformAxisPos* pTargetOffsetPlatformAxisPos = NULL);
	BOOL PackReAlignResultInfoFAAC(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, BOOL bSendTargetOffset = FALSE, CPlatformAxisPos* pTargetOffsetPlatformAxisPos = NULL);
	BOOL PackReAlignResultInfoFALG(CString& strInfo,CPlatformAxisPos* vpPlatAxisPos,BOOL bResult);
	BOOL PackReMulAlignPosInfoFMCO(CString& strInfo,std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset,BOOL bResult);

	BOOL PackReAlignResultInfoFALP(CString& strInfo,CPlatformAxisPos* vpPlatAxisPos,BOOL bResult); // 拾取对位响应字符串
	BOOL PackReAlignResultInfoFALP_int(CString& strInfo,CPlatformAxisPos* vpPlatAxisPos,int nResult); // 拾取对位响应字符串
	BOOL PackReMulAlignPosInfoFALP(CString& strInfo,std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos,BOOL bResult,int nPickMutiExMode = 0); // 拾取对位响应字符串
	BOOL PackReAlignResultNumberInfoFALP(CString& strInfo,CPlatformAxisPos* vpPlatAxisPos,int nResultNumber); // 拾取对位响应字符串

	BOOL PackReAlignResultInfoFAAB(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, CPlatformAxisPos* vpPlatAxisPos);
	BOOL PackReAlignResultInfoFACB(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, CPlatformAxisPos* vpPlatAxisPos);
	BOOL PackReAlignResultInfoFAHL(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset);
	BOOL PackReAlignResultInfoFXYA(CString& strInfo,BOOL bResult, BOOL bAlignBench = 0, CCoordPos platOffset = CCoordPos(0,0,0));
	BOOL PackReAlignResultInfoFAOT(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset);
	BOOL PackReAlignResultInfoFALX(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset);

	
	// 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
	BOOL  PackReResetFRST(CString& strInfo,BOOL bResult);
    BOOL  PackReAlignResultInfoFQRC( CString& strInfo, BOOL bResult);
	BOOL  PackReCommFCHK(CString& strInfo,BOOL bResult);

	BOOL PackReObjTarSearchFPCB(CString& strInfo,BOOL bResult,std::vector<CCoordPos> markPos);	// 对象、目标图像定位
	BOOL PackRePCBPrecisionFPIS(CString& strInfo,BOOL bResult);									// PCB 精度检测
	BOOL PackRePCBPrecisionFCPI( CString& strInfo,BOOL bResult );
	// 产品曝光时间设置
	BOOL PackReProductExposureFSPE(CString& strInfo,BOOL bResult);
	BOOL PackReAlignResultInfoFWEL(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset);	// 专用焊接指令
	BOOL PackReAlignResultInfoFSOL(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset);	// 专用焊接指令

	// 检测产品有、无
	BOOL PackReProductInspectFINS(CString& strInfo,BOOL bResult);	
	//////////////////////////////////////////////////////////////////////////
	// 视觉主动发送给PLC或其他主控系统的指令，以及PLC或其他主控系统返回的指令响应	
	virtual BOOL PackGetPlatformAbsPos(CString& strInfo);
	// 解析收到的“获取平台各轴绝对位置”响应
	virtual CmdAnsweredStatus AnalyzeAswGetGetPlatformAbsPos(CString strAsw, CPlatformAxisPos* vpPlatAxisPos);	
	
	virtual BOOL PackDrivePlatformMoveToAbsPosEx(CString& strData, CPlatformAxisPos* vpPlatAxisPos, CPlatformAxisPos* vpPlatAxisPosBase);	
	// 打包“指定平台各轴移动到的绝对位置”指令: TAR
	virtual BOOL PackDrivePlatformMoveToAbsPos(CString& strData, CPlatformAxisPos* vpPlatAxisPos);	
	// 解析收到的“指定平台各轴移动到的绝对位置”响应
	virtual CmdAnsweredStatus AnalyzeAswDrivePlatformMoveToAbsPos(CString strAsw);

	virtual BOOL PackGetPlatformAbsPosBackup(CString& strInfo);
	virtual CmdAnsweredStatus AnalyzeAswGetGetPlatformAbsPosBackup(CString strAsw, CPlatformAxisPos* vpPlatAxisPos);	

	//FOBC对象拍照并对位时对位结果回复
	BOOL PackReObjectSearchFOBCAndALGOrACO(CString& strInfo,BOOL bResult, int nSuccessPatIndex,CPlatformAxisPos* vpPlatAxisPos);
	//FOBJ对象拍照并对位时对位结果回复
	BOOL PackReObjectSearchFOBJAndALGOrACO(CString& strInfo,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	//FTGC目标拍照并对位时对位结果回复
	BOOL PackReTargetSearchFTGCAndALGOrACO(CString& strInfo,BOOL bResult, int nSuccessPatIndex,CPlatformAxisPos* vpPlatAxisPos);
	//FTGT目标拍照并对位时对位结果回复
	BOOL PackReTargetSearchFTGTAndALGOrACO(CString& strInfo,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	//对象或目标拍照并对位且寄存器通信时，将结果标志和对位结果轴位置一起打包发送（暂未使用）
	BOOL PackRePlcObjectSearchFOBCAndFALGOrACOAll(CString& strInfo,BOOL bResult, BOOL bTriggered,int nSuccessPatIndex,CPlatformAxisPos* vpPlatAxisPos);
	BOOL PackRePlcTargetSearchFTGCAndFALGOrACOAll(CString& strInfo,BOOL bResult, BOOL bTriggered,int nSuccessPatIndex,CPlatformAxisPos* vpPlatAxisPos);

	// FTGC 目标拍照后返回拾取上料计算返回偏差量
	BOOL PackReTargetSearchFTGCAndALGOrACO(CString& strInfo,BOOL bResult, int nSuccessPatIndex,CPlatformAxisPos* vpPlatAxisPos, CCoordPos* cpOffsetPick);
	// FTGT目标拍照并对位时对位结果回复上料拾取偏差量
	BOOL PackReTargetSearchFTGTAndALGOrACO(CString& strInfo,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos, CCoordPos* cpOffsetPick);


	//对象或目标拍照并对位且寄存器通信时，打包对位结果轴位置
	BOOL PackRePlcAlignAfterSearchAxisPos(CString& strInfo,BOOL bResult,CPlatformAxisPos* vpPlatAxisPos);
//////////////////////////////////////////////////////////////////////////
// 
public:

	deque<CString>				 m_dstrTranlateResultInfo;       //翻译的打包指令

	CString				  m_strCmdName;
	CalibParam            m_CalibParam;
	TargetSearchParam     m_TargetSearchParam;
	ObjectSearchParam     m_ObjectSearchParam;

	CPlatformXYDAxisPos	  m_CurPlatformXYDAxisPos;
	CPlatformXYDAxisPos	  m_CurPlatformXYDAxisPosBackup;
	CPlatformXYAxisPos	  m_CurCamXYAxisPos;
	CString				  m_strHead;			// 指令头
	CString				  m_strTail;			// 指令尾
	CString				  m_strSeparator;		// 分隔符
	//lzk 0408
	FVPCParam			  m_FVPCParam;//

	FSPCParam			  m_FSPCParam;// 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
	FSSVParam			  m_FSSVParam;// 2、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
	FGSPParam			  m_FGSPParam;// 3、 FGSP：产品No.的取得，取得目前正在执行的产品No.
	FVASParam			  m_FVASParam;// 1、 FVAS：虚拟Mark对位设定

	FCLBParam			  m_FCLBParam;// 1、FCLB:Calibration 的执行 执行Calibration
	FCBSParam			  m_FCBSParam;// 1、FCBS:Calibration 的执行 执行Calibration
	FCBRParam			  m_FCBRParam;// 1、FCBR:关联Calibration 的执行 执行Calibration
	FCPRParam			  m_FCPRParam;// 1、FCPR:相机轴标定
	FCBPParam			  m_FCBPParam;// 1、FCPR:相机轴标定

	FTGTParam			  m_FTGTParam;// 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
	FTGSParam			  m_FTGSParam;// 1.1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
	FTGCParam			  m_FTGCParam;// 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
	FTGRParam			  m_FTGRParam;// 3、FTGR:目标 Mark 位置的登录，将目前使用的产品数据保存在指定的产品No.中并保存在CF 记忆卡
	FTGGParam			  m_FTGGParam;// 4、FTGG:目标 Mark 画素的取得，取得执行“ＦＴＧＴ”结果的画素
	FTGDParam			  m_FTGDParam;// 5、FTGD:目标 Mark 指示线的全画面显示，在指定目标 Mark No.的目标 Mark 位置上显示全画面的指示线及刻度
	FOBJParam			  m_FOBJParam;// 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
	FOBCParam			  m_FOBCParam;// 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
	FPMIParam			  m_FPMIParam;// 3、FPMI:开启系统设置权限
	FBISParam			  m_FBISParam;// 3、FBIS:折弯贴合后检测

	FOBGParam			  m_FOBGParam;// 3、FOBG:物体 Mark 画素的取得，取得执行“ＦＯＢＪ”结果的画素
	FMSCParam			  m_FMSCParam;// 4、FMSC:手动搜索位置的取得，在自动对准时，当目标 Mark 或物体 Mark 搜索失败时，可利用手动方法（鼠标）指定Mark 位置
	FWLDParam			  m_FWLDParam;// 5、FWLD：取得目标（目标靶标）或对象（对象靶标）的平台坐标上的位置
	FWDRParam			  m_FWDRParam;// 5.1、FWDR:获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
	FGOWParam			  m_FGOWParam;// 5.1、FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
	FGAPParam			  m_FGAPParam;// 4、FGAP:目标 Mark 画素的取得，取得执行结果的画素
	FCNTParam			  m_FCNTParam;// 4、FGAP:目标 Mark 画素的取得，取得执行结果的画素
	FGTCParam			  m_FGTCParam;// 5、FGTC:获取目标相对于图像中心的图像坐标偏差

	FCNOParam			  m_FCNOParam;// 4、FCNO:目标定位同时将对象位置设为图像中心


	FGALParam			  m_FGALParam;// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
	FALLParam			  m_FALLParam;// 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
	FGACParam			  m_FGACParam;// 3、FGAC:对准最多次数的取得，取得设定对准最多次数
	FALCParam			  m_FALCParam;// 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
	FAALParam			  m_FAALParam;// 5、FAAL:自动对准的执行，执行自动对准（连续对准）
	FXYAParam			  m_FXYAParam;// 5、FAAL:自动对准的执行，执行自动对准（连续对准）
	FAABParam			  m_FAABParam;// 5、FAAB:自动对准的执行，执行自动对准（连续对准），增加内部图像十字中心基准对位过程
	FAACParam			  m_FAACParam;// 5、FAAC:自动对准的执行，执行自动对准（连续对准），对象候补搜索
	FACBParam			  m_FACBParam;// 5、FACB:自动对准的执行，执行自动对准（连续对准），对象候补搜索，增加内部图像十字中心基准对位过程
	FAHLParam			  m_FAHLParam;// 5、FAHL:背光叠片机自动对准的执行，执行自动对准（连续对准）
	FANSParam			  m_FANSParam;// 6、FANS:对准后的偏移量的取得，取得对准后平台坐标系的偏移量
	FALGParam			  m_FALGParam;// 7、FALG:１次对准移动量的取得，取得１次对准移动量
	FALPParam			  m_FALPParam;// 7、FALP:１次对准拾取移动量的取得，取得１次对准拾取移动量
	FALJParam			  m_FALJParam;// 8、FALJ:1:次对准精度判定与对准移动量的取得，对准移动量是否在对准精度内的判定与对准移动量的取得
	FACOParam			  m_FACOParam;// 9、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
	FAAOParam             m_FAAOParam;// 9、FAAO:偏移量的去的，目标对象自动拍照，目标为图像中心
	FOLCParam			  m_FOLCParam;// 9、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
	FABOParam			  m_FABOParam;// 9、FABO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
	FAOFParam			  m_FAOFParam;// 1、FAOF:自动补偿量值的取得，对准偏移量以补偿量值取得
	FGOFParam			  m_FGOFParam;// 2、FGOF:补偿量值的取得，取得设定的补偿量值
	FOFSParam			  m_FOFSParam;// 3、FOFS:补偿量值的设定，设定补偿量值
	FSFTParam			  m_FSFTParam;// 4、FSFT:平台Shift 移动动作，选择内部转移量的设定值，并依照设定值将目前平台的位置做相对移动
	FGSFParam			  m_FGSFParam;// 5、FGSF:对准Shift 移动量的取得，取得对准转移量
	FGPOParam			  m_FGPOParam;// 8、FGPO:获取连续对位最终精度
	FALMParam			  m_FALMParam;// 1、FALM:Alarm资料的取得，取得错误发生的数据代码
	FRSTParam			  m_FRSTParam;// 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
	FSACParam			  m_FSACParam;// 3、FSAC：保存校正、定位时的数据。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
	FSCOParam			  m_FSCOParam;// 4、FSCO：保存指令收发的历史纪录。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
	FCHKParam             m_FCHKParam;//特殊通讯参数
	FAOTParam			  m_FAOTParam;// 5、FAAC:自动对准的执行，执行自动对准（连续对准），对象候补搜索，目标候补搜索
	FALXParam			  m_FALXParam;// 5、FALX:自动对准的执行，执行自动对准（连续对准），对象候补搜索

	FCHBParam			  m_FCHBParam;
	FCHCParam			  m_FCHCParam;
	FDRPParam			  m_FDRPParam;
	FQRCParam			  m_FQRCParam;//存储二维码
	FQRIParam			  m_FQRIParam;//检测二维码

	MNPSParam			  m_MNPSParam;// 6、MNPS:平台现在位置的取得，取得平台现在位置
	MMVAParam			  m_MMVAParam;// 7、MMVA:平台絶对准置移动量的设定，设定平台絶对准置移动量
	MMVRParam			  m_MMVRParam;// 8、MMVR:平台相对准置移动量的设定，设定平台相对准置移动量

	FTGMParam			  m_FTGMParam;// 1、FTGM:目标 Mark 位置的手动取得，取得目标 Mark（目标）位置
	FOBMParam			  m_FOBMParam;// 1、FOBM:物体 Mark 位置的手动取得，取得物体 Mark（对准靶标）位置
	
	
	FSPEParam             m_FSPEParam;// 1、设置产品曝光
	FWELParam             m_FWELParam;// 1、专用焊接指令 ,小FPC:XY,大FPC:XYD
	FSOLParam			  m_FSOLParam;// 1、专用焊接指令,小FPC:XYD,大FPC:XY
	FPCBParam			  m_FPCBParam;// 1、对象、目标图像位置获得
	FPISParam			  m_FPISParam;// 1、PCB精度检测
	FCPIParam			  m_FCPIParam;// 1、更改平台序号

	FINSParam             m_FINSParam;// 1、检测产品有、无
	
	// FAST读写寄存器(暂无)
	FHMSParam				m_FHMSParam;// 		2	FHMS
	FHOMParam				m_FHOMParam;// 		3	FHOM
	FPNTParam				m_FPNTParam;// 		12	FPNT
	FMAPParam				m_FMAPParam;// 		22	FMAP
	FRCTParam				m_FRCTParam;// 		24	FRCT
	FISPParam				m_FISPParam;// 		25	FISP
	FOLGParam				m_FOLGParam;// 		26	FOLG
	FTLGParam				m_FTLGParam;// 		27	FTLG

	MPSBParam				m_MPSBParam;

	FGTWParam               m_FGTWParam;//      29  FGTW
	FAFOParam               m_FAFOParam;//      30  FAFO
	FGTPParam               m_FGTPParam;//      31  FGTP
	FMCOParam               m_FMCOParam;//      根据不同补偿多次计算目标对象平台坐标系偏差
	FCCIParam				m_FCCIParam;//lzk     200 复制标定信息

	FPEPParam				m_FPEPParam;// 清空产品
	FPCPParam				m_FPCPParam;// 复制产品
	FPRNParam				m_FPRNParam;// 重命名产品

// FAST读写寄存器
// 	VFRSTParam				m_VFRSTParam;// 	    1	FRST
// 	VFHMSParam				m_VFHMSParam;// 		2	FHMS
// 	VFHOMParam				m_VFHOMParam;// 		3	FHOM
// 	VFSPCParam				m_VFSPCParam;// 		4	FSPC
// 	VFCLBParam				m_VFCLBParam;// 		5	FCLB
// 	VFTGTParam				m_VFTGTParam;// 		6	FTGT
// 	VFOBJParam				m_VFOBJParam;// 		7	FOBJ
// 	VFAALParam				m_VFAALParam;// 		8	FAAL
// 	VMNPSParam				m_VMNPSParam;// 		9	FNPS
// 	VMMVAParam				m_VMMVAParam;// 		10	FMVA
// 	VMMVRParam				m_VMMVRParam;// 		11	FMVR
// 	VFPNTParam				m_VFPNTParam;// 		12	FPNT
// 	VFACOParam				m_VFACOParam;// 		13	FACO
// 	VFALGParam				m_VFALGParam;// 		14	FALG
// 	VFCNTParam				m_VFCNTParam;// 		15	FCNT
// 	VFGAPParam				m_VFGAPParam;// 		16	FGAP
// 	VFTGCParam				m_VFTGCParam;// 		17	FTGC
// 	VFOBCParam				m_VFOBCParam;// 		18	FOBC
// 	VFAACParam				m_VFAACParam;// 		19	FAAC
// 	VFGSPParam				m_VFGSPParam;// 		20	FGSP
// 	VFCBSParam				m_VFCBSParam;// 		21	FCBS
// 	VFMAPParam				m_VFMAPParam;// 		22	FMAP
// 	VFXYAParam				m_VFXYAParam;// 		23	FXYA
// 	VFRCTParam				m_VFRCTParam;// 		24	FRCT
// 	VFISPParam				m_VFISPParam;// 		25	FISP
// 	VFOLGParam				m_VFOLGParam;// 		26	FOLG
// 	VFTLGParam				m_VFTLGParam;// 		27	FTLG


private:
	// 解析是否系统标定指令
	virtual BOOL IsSysCalibrateCmd(CString strCmd);

    // 解析是否系统分步标定指令
	//virtual BOOL IsSysSepCalibrateCmd(CString strCmd);

	// 解析是否系统标定指令
	virtual BOOL IsSysResetCmd(CString strCmd);

	// 解析是否在线对位指令
	virtual BOOL IsOnLineAlignCmd(CString strCmd);

	// 解析是否在线对位指令
	virtual BOOL IsOnLineAlignCmdFACO(CString strCmd);
	// 解析是否在线对位指令 //迭代对位
	virtual BOOL IsOnLineAlignCmdFAAL(CString strCmd);
	// 解析是否在线对位指令 //一次对位
	virtual BOOL IsOnLineAlignCmdFALG(CString strCmd);

	// 解析是否标定搜索指令
	virtual BOOL IsCalibMarkSearchCmd(CString strCmd);

	// 解析是否实时目标搜索指令
	virtual BOOL IsTargetMarkSearchCmd(CString strCmd);
	// 解析是否实时目标搜索指令
	virtual BOOL IsTargetMarkSearchCmdFTGT(CString strCmd);

	// 解析是否实时目标搜索指令，目标候补搜索
	virtual BOOL IsTargetMarkSearchCmdFTGC(CString strCmd);

	// 解析是否实时对象搜索指令
	virtual BOOL IsObjectMarkSearchCmd(CString strCmd);

	// 解析是否虚拟对象搜索指令
	virtual BOOL IsVirtualMarkSearchCmd(CString strCmd);

	// 解析是否确定虚拟对象与实时对象关系指令
	virtual BOOL IsGetVirtualAndObjectMarkRelationShipCmd(CString strCmd);

	virtual BOOL IsGetAlignResult(CString strCmd);

	virtual BOOL IsSetTargetBench(CString strCmd);

public:
	virtual BOOL AnalyzeCommand(CString strCmd, CString& strName);
	virtual BOOL AnalyzeCommandParam(CString strCmd, CString& strName, std::vector<CString>& vstrParam);
	virtual BOOL PackParam(CString& strParam, int nParam, int nDataLenType, int nUnitExp = 0);
	virtual BOOL PackParam(CString& strParam, double dParam, int nDataLenType, int nUnitExp = 0);
	virtual BOOL ConvertToPlatformXYDAxisPos(std::vector<CString> vstrParam, CPlatformAxisPos* pPlatAxisPos);
	virtual BOOL ConvertToPlatformXYDAxisPos(std::vector<int> vnParam, CPlatformAxisPos* pPlatAxisPos);
	virtual BOOL ConvertToCamXYAxisPos(std::vector<CString> vstrParam, CPlatformAxisPos* vpCamAxisPos);

	virtual BOOL ReplaceNewCommCmd(const CString strOldCmd, CString& strNewCmd);



//////////////////////////////////////////////////////////////////////////
// Yamaha 机器人
public:
	CString				  m_strRobotHead;			// 指令头_T("@")
	CString				  m_strRobotTail;			// 指令尾
	CString				  m_strRobotSeparator;		// 分隔符空格_T(" ")
	CString				  m_strRobotEqual;			// 等号_T("=")
	CString				  m_strRobotQuery;			// 问号_T("？")

public:
	// 指令分析// 命令格式（[Pos] R1 R2 R3 R4 R5 R6）
	virtual BOOL AnalyzeRobotCommandParam(CString strCmd, CString& strName, std::vector<CString>& vstrParam);

public:
	// 1）机器人工作模式查询、设定（手动模式manual、自动模式auto等）；
	virtual BOOL PackRobotGetMod(CString& strInfo);
	virtual CmdAnsweredStatus AnalyzeAswRobotGetMod(CString strAsw, RobotMod& rmRobotMod);

	virtual BOOL PackRobotSetMod(CString& strInfo, RobotMod rmRobotMod);
	virtual CmdAnsweredStatus AnalyzeAswRobotSetMod(CString strAsw);

	// 2）机器人自动模式下程序执行操作（自动模式下的运行Run、重置Reset、停止stop等）；	
	virtual BOOL PackRobotAutoControl(CString& strInfo, RobotAutoControl racAutoControl);
	virtual CmdAnsweredStatus AnalyzeAswRobotAutoControl(CString strAsw);

	// 3）机器人手动移动速度查询、设定（mspeed；
	virtual BOOL PackRobotGetMSpeed(CString& strInfo);
	virtual CmdAnsweredStatus AnalyzeAswRobotGetMSpeed(CString strAsw, int& nSpeed);

	virtual BOOL PackRobotSetMSpeed(CString& strInfo, int nSpeed);
	virtual CmdAnsweredStatus AnalyzeAswRobotSetMSpeed(CString strAsw);

	// 4）机器人移动命令（inch X、Y、Z等)；
	virtual BOOL PackRobotInch(CString& strInfo, RobotInchAxis nAxis, RobotInchType nType);
	virtual CmdAnsweredStatus AnalyzeAswRobotInch(CString strAsw);

	// 5）机器人当前位置查询（WHRXY）；
	virtual BOOL PackRobotGetPosWHRXY(CString& strInfo);
	virtual CmdAnsweredStatus AnalyzeAswRobotGetPosWHRXY(CString strAsw, RobotPos& rpPos);

	// 6）设置机器人示教点位（TEACH 200）；
	virtual BOOL PackRobotTeachPos(CString& strInfo, int nPosIndex);
	virtual CmdAnsweredStatus AnalyzeAswRobotTeachPos(CString strAsw);

	// 7）机器人点位查询（Read P200）；
	virtual BOOL PackRobotReadPos(CString& strInfo, int nPosIndex);
	virtual CmdAnsweredStatus AnalyzeAswRobotReadPos(CString strAsw, RobotPos& rpPos);

	// 8）执行机器人语言指令（@“机器人语言”，如@MOVE P, P100, S=20 [cr/lf]）.
	virtual BOOL PackRobotMovePos(CString& strInfo, int nPosIndex, int nSpeed = 20);
	virtual CmdAnsweredStatus AnalyzeAswRobotMovePos(CString strAsw);

	// 9）执行中断处理（ETX(03)）
	virtual BOOL PackRobotETX(CString& strInfo);
	virtual CmdAnsweredStatus AnalyzeAswRobotETX(CString strAsw);

	// 10）设置某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) = 20 [cr/lf]）
	virtual BOOL PackRobotSetPosAixs(CString& strInfo, int nPosIndex, RobotInchAxis nAxis, double dVal);
	virtual CmdAnsweredStatus AnalyzeAswRobotSetPosAixs(CString strAsw);

	// 10）设置某个点的位置（@“机器人语言”，如@WRITE P100 = 100.00 ...... [cr/lf]）
	virtual BOOL PackRobotSetPos(CString& strInfo, int nPosIndex, RobotPos rpPos);
	virtual CmdAnsweredStatus AnalyzeAswRobotSetPos(CString strAsw);

	// 11）查询某个点某个轴的位置（@“机器人语言”，如@LOCX(P100) [cr/lf]）
	// 12) 设置某个变量的值
	// 13）设置某个变量的值
	// 14）主机器人轴移动至可进行绝对式原点复位的位置(ABSADJ X, -)
	virtual BOOL PackRobotABSADJ(CString& strInfo, RobotInchAxis nAxis, RobotInchType nType);
	virtual CmdAnsweredStatus AnalyzeAswRobotABSADJ(CString strAsw);

	// 15）主机器人轴的绝对式原点复位(ABSRESET k)
	virtual BOOL PackRobotABSRESET(CString& strInfo, RobotInchAxis nAxis);
	virtual CmdAnsweredStatus AnalyzeAswRobotABSRESET(CString strAsw);


	// 16) 设置整型常量的值（@“机器人语言”，如@SGI0 = 20[cr/lf]）
	virtual BOOL PackRobotWriteSGI(CString& strInfo, int nIndex, int nVal);
	virtual CmdAnsweredStatus AnalyzeAswRobotWriteSGI(CString strAsw);

	// 17) 查询整型常量的值（@“机器人语言”，如@READ SGI0[cr/lf]）
	virtual BOOL PackRobotReadSGI(CString& strInfo, int nIndex);
	virtual CmdAnsweredStatus AnalyzeAswRobotReadSGI(CString strAsw, int &nVal);

	// 18) 设置实数常量的值（@“机器人语言”，如@SGR0 = 20.0[cr/lf]）
	virtual BOOL PackRobotWriteSGR(CString& strInfo, int nIndex, double dVal);
	virtual CmdAnsweredStatus AnalyzeAswRobotWriteSGR(CString strAsw);

	// 18) 查询实数常量的值（@“机器人语言”，如@READ SGR0[cr/lf]）
	virtual BOOL PackRobotReadSGR(CString& strInfo, int nIndex);
	virtual CmdAnsweredStatus AnalyzeAswRobotReadSGR(CString strAsw, double &dVal);

	//////////////////////////////////////////////////////////////////////////
	// Yamaha 机器人340
public:
	// 1）机器人工作模式查询、设定（手动模式manual、自动模式auto等）；
	virtual BOOL PackRobotGetMod340(CString& strInfo);
	virtual CmdAnsweredStatus AnalyzeAswRobotGetMod340(CString strAsw, RobotMod& rmRobotMod);
	virtual CmdAnsweredStatus AnalyzeAswRobotGetModOK340(CString strAsw);

	virtual BOOL PackRobotSetMod340(CString& strInfo, RobotMod rmRobotMod);
	virtual CmdAnsweredStatus AnalyzeAswRobotSetMod340(CString strAsw);

	// 3）机器人手动移动速度查询、设定（mspeed；
	virtual BOOL PackRobotGetMSpeed340(CString& strInfo);
	virtual CmdAnsweredStatus AnalyzeAswRobotGetMSpeed340(CString strAsw, int& nSpeed);
	virtual CmdAnsweredStatus AnalyzeAswRobotGetMSpeedOK340(CString strAsw);

	virtual BOOL PackRobotSetMSpeed340(CString& strInfo, int nSpeed);
	virtual CmdAnsweredStatus AnalyzeAswRobotSetMSpeed340(CString strAsw);

	// 4）机器人移动命令（inch X、Y、Z等)；
	virtual BOOL PackRobotInch340(CString& strInfo, RobotInchAxis nAxis, RobotInchType nType);
	virtual CmdAnsweredStatus AnalyzeAswRobotInchStart340(CString strAsw);
	virtual CmdAnsweredStatus AnalyzeAswRobotInchEnd340(CString strAsw);

	// 5）机器人当前位置查询（WHRXY）；
	virtual BOOL PackRobotGetPosWHRXY340(CString& strInfo);
	virtual CmdAnsweredStatus AnalyzeAswRobotGetPosWHRXY340(CString strAsw, RobotPos& rpPos);
	virtual CmdAnsweredStatus AnalyzeAswRobotGetPosWHRXYOK340(CString strAsw);

	// 6）设置机器人示教点位（TCHXY 200）；
	virtual BOOL PackRobotTeachPos340(CString& strInfo, int nPosIndex);
	virtual CmdAnsweredStatus AnalyzeAswRobotTeachPos340(CString strAsw);

	// 7）机器人点位查询（?P200）；
	virtual BOOL PackRobotReadPos340(CString& strInfo, int nPosIndex);
	virtual CmdAnsweredStatus AnalyzeAswRobotReadPos340(CString strAsw, RobotPos& rpPos);
	virtual CmdAnsweredStatus AnalyzeAswRobotReadPosOK340(CString strAsw);

	// 10）设置某个点的位置（@“机器人语言”，如@P100 = 100.00 ...... [cr/lf]）
	virtual BOOL PackRobotSetPos340(CString& strInfo, int nPosIndex, RobotPos rpPos);
	virtual CmdAnsweredStatus AnalyzeAswRobotSetPos340(CString strAsw);

	// 16) 设置整型常量的值（@“机器人语言”，如@SGI0 = 20[cr/lf]）
	virtual BOOL PackRobotWriteSGI340(CString& strInfo, int nIndex, int nVal);
	virtual CmdAnsweredStatus AnalyzeAswRobotWriteSGI340(CString strAsw);

	// 17) 查询整型常量的值（@“机器人语言”，如@?SGI0[cr/lf]）
	virtual BOOL PackRobotReadSGI340(CString& strInfo, int nIndex);
	virtual CmdAnsweredStatus AnalyzeAswRobotReadSGI340(CString strAsw, int &nVal);
	virtual CmdAnsweredStatus AnalyzeAswRobotReadSGIOK340(CString strAsw);

	// 18) 设置实数常量的值（@“机器人语言”，如@SGR0 = 20.0[cr/lf]）
	virtual BOOL PackRobotWriteSGR340(CString& strInfo, int nIndex, double dVal);
	virtual CmdAnsweredStatus AnalyzeAswRobotWriteSGR340(CString strAsw);

	// 18) 查询实数常量的值（@“机器人语言”，如@?SGR0[cr/lf]）
	virtual BOOL PackRobotReadSGR340(CString& strInfo, int nIndex);
	virtual CmdAnsweredStatus AnalyzeAswRobotReadSGR340(CString strAsw, double &dVal);
	virtual CmdAnsweredStatus AnalyzeAswRobotReadSGROK340(CString strAsw);









//////////////////////////////////////////////////////////////////////////
// PLC寄存器操作处理
public:

	BOOL	SetTranslateResultInfo(CString strTranslateResultInfo);
	BOOL    GetTranslateResultInfo(CString& strTranslateResultInfo,BOOL bReceive);
	BOOL	SetWriteRegTranslateResultInfo(long dRegAddr,int nWriteNum,std::vector<int> &vWriteData,BOOL bDWORD=TRUE);
	BOOL	SetWriteRegTranslateResultInfo(long dRegAddr,int nWriteNum,std::vector<double> &vWriteData,BOOL bDWORD=TRUE);
	BOOL	SetAnalyzeAswWriteRegTransInfo();
	BOOL	SetReadRegTranslateResultInfo(long dRegAddr, int nReadNum,BOOL bDWORD = TRUE);
	BOOL	SetAnalyzeAswReadRegTransInfo(int nReceiveNum,std::vector<int> &vReceivedData,BOOL bDWORD = TRUE);
	BOOL	SetAnalyzeAswReadRegTransInfo(int nReceiveNum,std::vector<double> &vReceivedData,BOOL bDWORD = TRUE);

public:
//	BOOL PackWriteRegInfo( CString& strInfo, CString strRegAddr, int dWriteNum, std::vector<double> vWriteData );
//	BOOL PackWriteRegInfo( CString& strInfo, CString strRegAddr, int dWriteNum, std::vector<int> vWriteData );
	
	// 打包获取PLC动作指令或响应
	BOOL PackPlcReadSysWorkCmd( CString& strInfo);
	// 解析获取PLC动作指令或响应
	virtual	SysWorkCmdType AnalyzePlcSysWorkCmdReceived(CString strCmd, BOOL &bTriggered);


	//////////////////////////////////////////////////////////////////////////
	// 解析写入PLC后，PLC反馈的响应字符串（一般成功都是固定格式），几乎所有写入操作响应的解析都可以用这个函数
	virtual CmdAnsweredStatus AnalyzeAswPlcWriteReg(CString strAsw);

	//   打包发送给PLC或其他主控系统的指令
	// 1、 PLC Cmd Trigger：设置PLC命令触发器
	virtual BOOL PackPlcWriteSetPlcCmdTrigger(CString& strInfo, BOOL bTriggered);

	// 1、 PLC Cmd Trigger：设置PLC命令触发器
	virtual BOOL PackPlcWriteSetVisionCmdTrigger(CString& strInfo, BOOL bTriggered);

	// 设置视觉系统状态（写PLC寄存器）
	virtual BOOL PackPlcWriteSetVisionStatus(CString& strInfo, VisionStatus eVisionStatus);

	// 1、 设置视觉弹出手动搜索框：设置、清除PLC命令触发器
	virtual BOOL PackPlcWriteSetManualSearch(CString& strInfo, BOOL bShow);
	// 2、 设置视觉弹出手动搜索框：设置、清除PLC命令触发器-腾盛协议定制
	virtual BOOL PackPlcWriteSetManualSearch(CString& strInfo, BOOL bShow,int nOffSet);
	// 3、 设置视觉弹出手动搜索框：自定义设置、清除PLC命令触发器
	virtual BOOL PackPlcWriteSetManualSearch3(CString& strInfo, BOOL bShow,int nWriteAddr);

	//////////////////////////////////////////////////////////////////////////
	//   打包发送给PLC或其他主控系统的指令
	//lzk 0408 
	BOOL PackRePlcSetPositionVirtualFVPC(CString& strInfo,BOOL bResult);
	// 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
	BOOL PackRePlcSetProductIndexFSPC(CString& strInfo,BOOL bResult);	
	// 2、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
	BOOL PackRePlcSaveProductIndexFSSV(CString& strInfo,BOOL bResult);
	// 3、 FGSP：产品No.的取得，取得目前正在执行的产品No.
	BOOL PackRePlcGetProductIndexFGSP(CString& strInfo,BOOL bResult, int nCurProductIndex, int nAssitIndex = 1);

	// 1、 FVAS：
	BOOL PackRePlcSetProductVirtualAlnFVAS(CString& strInfo,BOOL bResult);

	//计算目标固定补偿时，回复PLC寄存器结果
	BOOL PackRePlcCalTargetFixOffsetFAFO(CString& strInfo,BOOL bResult);
	//对象目标不在同一个控制器且计算目标固定补偿时，发送给PLC目标平台坐标、拍照轴位置、MARK二维码靶标位置
	BOOL PackRePlcCalTargetFixOffsetFAFOPos(CString& strInfo,BOOL bResult,std::vector<int> vnPosIndex,std::vector<CCoordPos>& vCoordPos,std::vector<CPlatformXYDAxisPos> &vPlatformAxis,std::vector<CCoordPos>& vBoardPos);
	//对象目标不在同一个控制器且计算目标固定补偿时，发送给PLC目标MARK二维码靶标位置（暂未使用）
	BOOL PackRePlcCalTargetFixOffsetFAFOBoardPos(CString& strInfo,BOOL bResult,std::vector<int> vnPosIndex,std::vector<CCoordPos> vBoardPos);
	// 1、FCLB:Calibration 的执行 执行Calibration
	BOOL PackRePlcCalibrateFCLB(CString& strInfo,BOOL bResult);	
	// 1、FCBS:Calibration 的执行 执行Calibration
	BOOL PackRePlcCalibrateFCBS(CString& strInfo,BOOL bResult);	
	// 1、FCBR:关联Calibration 的执行 执行Calibration
	BOOL PackRePlcCalibrateFCBR(CString& strInfo,BOOL bResult);	
	// 1、FCLB:Calibration 的执行 执行Calibration
	BOOL PackRePlcCalibrateFCBP(CString& strInfo,BOOL bResult);	
	//相机轴标定
	BOOL PackReCamCalibrateFCPR(CString& strInfo,BOOL bResult);
	BOOL PackRePlcCamCalibrateFCPR(CString& strInfo,BOOL bResult);
	// 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
	BOOL PackRePlcTargetSearchFTGT(CString& strInfo,BOOL bResult);      
	// 1.1、FTGS:目标 Mark 位置的取得，取得目标 Mark（目标）位置，目标平台轴位置一致
	BOOL PackRePlcTargetSearchFTGS(CString& strInfo,BOOL bResult); 
	// 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
	BOOL PackRePlcTargetSearchFTGC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 1);      
	// 3、FTGR:目标 Mark 位置的登录，将目前使用的产品数据保存在指定的产品No.中并保存在CF 记忆卡
	BOOL PackRePlcSaveTargetPosFTGR(CString& strInfo,BOOL bResult);	  
	// 4、FTGG:目标 Mark 画素的取得，取得执行“ＦＴＧＴ”结果的画素
	BOOL PackRePlcGetTargetImagePosFTGG(CString& strInfo,BOOL bResult, double dImagePosX, double dImagePosY); 
	// 5、FTGD:目标 Mark 指示线的全画面显示，在指定目标 Mark No.的目标 Mark 位置上显示全画面的指示线及刻度
	BOOL PackRePlcDisplayTargetFTGD(CString& strInfo,BOOL bResult);	 
	// 6、FTGM:目标 Mark 位置的手动取得，取得目标 Mark（目标）位置
	BOOL PackRePlcTargetSearchFTGM(CString& strInfo,BOOL bResult);      

	// 1、FCNT:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
	BOOL PackRePlcObjectSearchFCNT(CString& strInfo,BOOL bResult);      
	BOOL PackRePlcTargetSearchFCNO(CString& strInfo,BOOL bResult);  

	// 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
	BOOL PackRePlcObjectSearchFOBJ(CString& strInfo,BOOL bResult);      
	// 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
	BOOL PackRePlcObjectSearchFOBC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 1);       
	// 3、FOBG:物体 Mark 画素的取得，取得执行“ＦＯＢＪ”结果的画素
	BOOL PackRePlcGetObjectImagePosFOBG(CString& strInfo,BOOL bResult, double dImagePosX, double dImagePosY); 
	// 4、FMSC:手动搜索位置的取得，在自动对准时，当目标 Mark 或物体 Mark 搜索失败时，可利用手动方法（鼠标）指定Mark 位置
	BOOL PackRePlcManualSearchFMSC(CString& strInfo,BOOL bResult);	  
	// 5、FWLD：取得目标（目标靶标）或对象（对象靶标）的平台坐标上的位置
	BOOL PackRePlcGetSearchWorldPosFWLD(CString& strInfo,BOOL bResult, double dPlatPosX, double dPlatPosY);	 

	// 打包FWLD轴位置（平台坐标）
	BOOL PackRePlcAlignResultInfoFWLDAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult);
	// 打包FWLD结果
	BOOL PackRePlcAlignResultInfoFWLD(CString& strInfo, BOOL bResult);

	// 5.1、FWDR:获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
	BOOL PackRePlcGetSearchWorldRadiusFWDRRadius(CString& strInfo,BOOL bResult, CCoordPos platOffset);	  
	// 5.1、FWDR:获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
	BOOL PackRePlcGetSearchWorldRadiusFWDR(CString& strInfo,BOOL bResult);

	// 5.1、FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
	BOOL PackRePlcGetObjectWorldPosFGOWAxisPos(CString& strInfo,BOOL bResult, CCoordPos platOffset);	  
	// 5.1、FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
	BOOL PackRePlcGetObjectWorldPosFGOW(CString& strInfo,BOOL bResult);

	// 6、FOBM:物体 Mark 位置的手动取得，取得物体 Mark（对准靶标）位置
	BOOL PackRePlcObjectSearchFOBM(CString& strInfo,BOOL bResult);      

	//打包回复给PLC寄存器的目标平台坐标及轴位置
	BOOL PackRePlcGetTargetPlartformPosFGTWPos(CString& strInfo,BOOL bResult, std::vector<int> vnPosIndex,std::vector<CCoordPos>& vCoordPos,std::vector<CPlatformXYDAxisPos> &vPlatformAxis);
	//打包回给复PLC寄存器获取目标平台坐标及轴位置结果标志
	BOOL PackRePlcGetTargetPlartformPosFGTW(CString& strInfo,BOOL bResult);
	//打包回复给PLC寄存器的目标图像坐标及轴位置
	BOOL PackRePlcGetTargetImagePosFGTPPos(CString& strInfo,BOOL bResult, std::vector<int> vnPosIndex,std::vector<CCoordPos>& vImagePos,std::vector<CPlatformXYDAxisPos> &vPlatformAxis);
	//打包回给复PLC寄存器获取目标图像坐标及轴位置结果标志
	BOOL PackRePlcGetTargetImagePosFGTP(CString& strInfo,BOOL bResult);

	// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
	BOOL PackRePlcGetAlignPrecisionFGALAxisPos(CString& strInfo,BOOL bResult, CCoordPos cpAlignPrecision);	
	// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
	BOOL PackRePlcGetAlignPrecisionFGAL(CString& strInfo,BOOL bResult);	
	// 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
	BOOL PackRePlcSetAlignPrecisionFALL(CString& strInfo,BOOL bResult);	
	// 3、FGAC:对准最多次数的取得，取得设定对准最多次数
	BOOL PackRePlcGetAlignTimeFGAC(CString& strInfo,BOOL bResult, int nAlignTime);		
	// 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
	BOOL PackRePlcSetAlignTimeFALC(CString& strInfo,BOOL bResult);		
	// 5、FAAL:自动对准的执行，执行自动对准（连续对准）
	//	BOOL PackRePlcAutoAlignFAAL(CString& strInfo,BOOL bResult);			
	// 6、FANS:对准后的偏移量的取得，取得对准后平台坐标系的偏移量
	BOOL PackRePlcGetAutoAlignOffsetFANS(CString& strInfo,BOOL bResult, CCoordPos cpPlatOffset); 
	// 7、FALG:１次对准移动量的取得，取得１次对准移动量
	//	BOOL PackRePlcOnceAlignMoveFALG(CString& strInfo,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);		
	// 8、FALJ:1:次对准精度判定与对准移动量的取得，对准移动量是否在对准精度内的判定与对准移动量的取得
	BOOL PackRePlcOnceAlignFALJ(CString& strInfo,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);			
	// 9、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
	//	BOOL PackRePlcGetTargetObjectOffsetFACO(CString& strInfo,BOOL bResult, CCoordPos cpPlatOffset);

	// 1、FAOF:自动补偿量值的取得，对准偏移量以补偿量值取得
	BOOL PackRePlcCalcuAlignOffsetFAOF(CString& strInfo,BOOL bResult);	
	// 2、FGOF:补偿量值的取得，取得设定的补偿量值
	BOOL PackRePlcGetAlignOffsetFGOF(CString& strInfo,BOOL bResult);
	BOOL PackRePlcGetAlignOffsetFGOFAxisPos(CString& strInfo,BOOL bResult, CCoordPos cpAlignOffset);
	// 3、FOFS:补偿量值的设定，设定补偿量值
	BOOL PackRePlcSetAlignOffsetFOFS(CString& strInfo,BOOL bResult);		
	// 4、FSFT:平台Shift 移动动作，选择内部转移量的设定值，并依照设定值将目前平台的位置做相对移动
	BOOL PackRePlcFSFT(CString& strInfo,BOOL bResult);					
	// 5、FGSF:对准Shift 移动量的取得，取得对准转移量
	BOOL PackRePlcFGSF(CString& strInfo,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);	

	// 1、FALM:Alarm资料的取得，取得错误发生的数据代码
	BOOL PackRePlcGetAlarmFALM(CString& strInfo,int nAlarmIndex, int nSimData = -1);			
	// 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
	//	BOOL PackRePlcResetFRST(CString& strInfo);													
	// 3、FSAC：保存校正、定位时的数据。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
	BOOL PackRePlcSaveProcessDataFSAC(CString& strInfo,BOOL bResult);							
	// 4、FSCO：保存指令收发的历史纪录。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
	BOOL PackRePlcSaveCommDataFSCO(CString& strInfo,BOOL bResult);								

	//检测
	BOOL PackRePlcACFInspectFCHB(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 1); 
	BOOL PackRePlcACFInspectFCHBD(CString& strInfo, std::vector<double> vdDistance);
	
	//计算弯折结果
	BOOL PackRePlcWZResult(CString& strInfo, std::vector<double> vdDate,int nOffset=0);
	BOOL PackRePlcYZRResult(CString& strInfo, std::vector<double> vdDate,long nSendAddr);
	
	BOOL PackRePlcACFInspectFCHBAreaInspectSubResult(CString& strInfo, std::vector<BOOL> vbAreaInspectSubResult);
	//候补检测
	BOOL PackRePlcACFInspectFCHC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 1); 

	BOOL PackRePlcFDRP(CString& strInfo,BOOL bResult); 


	BOOL PackRePlcClearProductFPEP(CString& strInfo,BOOL bResult);
	BOOL PackRePlcProductCopyFPCP(CString& strInfo, BOOL bResult);
	BOOL PackRePlcProductRenameFPRN(CString& strInfo,BOOL bResult);
	//////////////////////////////////////////////////////////////////////////
	// 	// 1、FCLB:Calibration 的执行 执行Calibration
	// 	BOOL PackRePlcCalibrateFCLB(CString& strInfo,BOOL bResult);
	// 	// 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
	// 	BOOL PackRePlcTargetSearchFTGC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 0);
	// 	// 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
	// 	BOOL PackRePlcTargetSearchFTGT(CString& strInfo,BOOL bResult);
	// 	// 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
	// 	BOOL PackRePlcObjectSearchFOBJ(CString& strInfo,BOOL bResult);
	// 	// 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
	// 	BOOL PackRePlcObjectSearchFOBC(CString& strInfo,BOOL bResult, int nSuccessPatIndex = 0);

	// 打包FACO基准中心对位轴位置
	BOOL PackRePlcAlignResultInfoFACOBenchAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, CPlatformAxisPos* vpPlatAxisPos);

	// 打包FACO 拾取按照上料对位轴位置
	BOOL PackRePlcAlignPickReviseResultInfoFACOBenchAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffsetRevise, CPlatformAxisPos* vpPlatAxisPos);


	// 打包FACO轴位置
	BOOL PackRePlcAlignResultInfoFACOAxisPos(CString& strInfo,std::vector<CCoordPos> platOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);

	// 打包FACO 拾取按照上料对位轴位置
	BOOL PackRePlcAlignPickReviseResultInfoFACOAxisPos(CString& strInfo,CCoordPos platOffsetRevise, BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);


	// 打包FACO轴位置
	BOOL PackRePlcAlignResultInfoFACOAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	
	// 打包FAAO轴位置
	BOOL PackRePlcAlignResultInfoFAAOAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);

	// 打包FACO结果
	BOOL PackRePlcAlignResultInfoFACO(CString& strInfo, BOOL bResult);
	// 打包FAAO结果
	BOOL PackRePlcAlignResultInfoFAAO(CString& strInfo, BOOL bResult);

	// 打包FMCO结果
	BOOL PackRePlcMulAlignPosResultInfoFMCOAxisPos(CString& strInfo,std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset,BOOL bResult);
	BOOL PackRePlcAlignResultInfoFMCO(CString& strInfo, BOOL bResult);

	// 打包FOLC轴位置
	BOOL PackRePlcAlignResultInfoFOLCAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	// 打包FOLC结果
	BOOL PackRePlcAlignResultInfoFOLC(CString& strInfo, BOOL bResult);

	// 打包FGAP轴位置
	BOOL PackRePlcAlignResultInfoFGAPAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult);
	// 打包FGAP结果
	BOOL PackRePlcAlignResultInfoFGAP(CString& strInfo, BOOL bResult);


	// 打包FABO轴位置
	BOOL PackRePlcAlignResultInfoFABOAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult, CPlatformAxisPos* vpPlatAxisPos);
	// 打包FABO结果
	BOOL PackRePlcAlignResultInfoFABO(CString& strInfo, BOOL bResult);

	// 打包FAAL基准中心对位轴位置
	BOOL PackRePlcAlignResultInfoFAALBenchAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, CPlatformAxisPos* vpPlatAxisPos);

	// 打包FAAC基准中心对位轴位置
	BOOL PackRePlcAlignResultInfoFAACBenchAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, CPlatformAxisPos* vpPlatAxisPos);

	// 打包FXYA基准中心对位轴位置
	BOOL PackRePlcAlignResultInfoFXYABenchAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, CPlatformAxisPos* vpPlatAxisPos);

	BOOL PackRePlcAlignResultInfoFAAL(CString& strInfo,BOOL bResult);

	BOOL PackRePlcAlignResultInfoFXYA(CString& strInfo,BOOL bResult);


	// 打包FAAC轴位置
	BOOL PackRePlcAlignResultInfoFAACAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult);
	BOOL PackRePlcAlignResultInfoFAAC(CString& strInfo,BOOL bResult);
	
	// 打包FAOT轴位置
	BOOL PackRePlcAlignResultInfoFAOTAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset);
	// 打包FAOT结果
	BOOL PackRePlcAlignResultInfoFAOT(CString& strInfo,BOOL bResult);

	// 打包FALX轴位置
	BOOL PackRePlcAlignResultInfoFALXAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset);
	// 打包FALX结果
	BOOL PackRePlcAlignResultInfoFALX(CString& strInfo,BOOL bResult);

	// 打包FALG基准中心对位轴位置
	BOOL PackRePlcAlignResultInfoFALGBenchAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, CPlatformAxisPos* vpPlatAxisPos);

	// 打包FALG轴位置
	BOOL PackRePlcAlignResultInfoFALGAxisPos(CString& strInfo,CPlatformAxisPos* vpPlatAxisPos,BOOL bResult);
	// 打包FALG结果
	BOOL PackRePlcAlignResultInfoFALG(CString& strInfo, BOOL bResult);

	//打包回复给PLC的线距检测结果
	BOOL PackRePlcAlignInspectDistanceInfo(CString& strInfo,BOOL bResult);

	// 打包FALP轴位置
	BOOL PackRePlcAlignResultInfoFALPAxisPos(CString& strInfo,CPlatformAxisPos* vpPlatAxisPos,BOOL bResult);
	BOOL PackRePlcMulAlignPosResultInfoFALPAxisPos(CString& strInfo,std::vector<int> &vnInfo,int &nRegAddr,std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos,BOOL bResult,int nPickMutiExMode = 0);
	BOOL PackRePlcAlignResultNumberInfoFALPAxisPos(CString& strInfo,CPlatformAxisPos* vpPlatAxisPos,int nResultNumber);

	// 打包FALP结果
	BOOL PackRePlcAlignResultInfoFALP(CString& strInfo, BOOL bResult);
	BOOL PackRePlcAlignResultInfoFALP_int(CString& strInfo, int nResult);
	BOOL PackRePlcAlignResultNumberInfoFALP(CString& strInfo, int nResultNumber);

	// 打包FGTC
	BOOL PackRePlcAlignResultInfoFGTCAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult);
	// 打包FGTC结果
	BOOL PackRePlcAlignResultInfoFGTC(CString& strInfo, BOOL bResult);


	//////////////////////////////////////////////////////////////////////////
	// 视觉主动发送给PLC或其他主控系统的指令，以及PLC或其他主控系统返回的指令响应	

	

	// 打包FAAB轴位置
	BOOL PackRePlcAlignResultInfoFAABAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, CPlatformAxisPos* vpPlatAxisPos);
	BOOL PackRePlcAlignResultInfoFACBAxisPos(CString& strInfo,BOOL bResult, BOOL bAlignBench, CCoordPos platOffset, CPlatformAxisPos* vpPlatAxisPos);
	// 打包FAAB结果
	BOOL PackRePlcAlignResultInfoFAAB(CString& strInfo,BOOL bResult);
// 打包FACB结果
	BOOL PackRePlcAlignResultInfoFACB(CString& strInfo,BOOL bResult);

	BOOL PackRePlcAlignResultInfoFAHL(CString& strInfo,BOOL bResult);

	// 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
	BOOL  PackRePlcResetFRST(CString& strInfo,BOOL bResult);

	// 2、FCHK:握手通讯
	BOOL  PackRePlcCommFCHK(CString& strInfo,BOOL bResult);

	// FCCI：复制标定信息
	BOOL PackRePlcCopyCalibrateInfoFCCI(CString& strInfo,BOOL bResult);

	//////////////////////////////////////////////////////////////////////////
	//对象目标不在同一个控制器且进行目标和对象关联时，打包目标端发送给PLC的MARK二维码靶标位置、图像尺寸
	virtual BOOL PackPlcSendDmCodeBoardAbsPos(CString& strInfo,std::vector<int> vnPosIndex,std::vector<CCoordPos> &vBoardPos,std::vector<CCoordPos> &vImageSize);
	//对象目标不在同一个控制器且进行目标和对象关联时，对象端从PLC获取目标端的MARK二维码靶标位置、图像尺寸
	virtual BOOL PackPlcGetDmCodeBoardAbsPos(CString& strInfo);
	//对象目标不在同一个控制器且进行目标和对象关联时，对象端解析从PLC获取的目标端MARK二维码靶标位置、图像尺寸
	virtual CmdAnsweredStatus AnalyzeAswPlcGetDmCodeBoardAbsPos(CString strAsw,std::vector<int> &vnPosIndex,std::vector<CCoordPos> &vImageSize,std::vector<CCoordPos> &vBoardPos);
	
	//对象目标不在同一个控制器、目标采用靶标关联且计算目标补偿对象二维码拍照时，对象端从PLC获取目标MARK二维码靶标位置
	virtual BOOL PackPlcGetBoardPosForTarFixOffset(CString& strInfo);
	//对象目标不在同一个控制器、目标采用靶标关联且计算目标补偿对象二维码拍照时，对象端解析从PLC获取的目标MARK二维码靶标位置
	virtual CmdAnsweredStatus AnalyzeAswPlcGetBoardPosForTarFixOffset(CString strAsw,std::vector<int> &vnPosIndex, std::vector<CCoordPos> &vBoardPos);

	//对象目标不在同一个控制器、目标采用靶标关联，计算目标固定补偿或对位时,从PLC获取目标平台坐标和轴位置
	virtual BOOL PackPlcGetTargetPlatformPos(CString& strInfo);
	//对象目标不在同一个控制器、目标采用靶标关联，计算目标固定补偿或对位时,解析从PLC获取的目标平台坐标和轴位置
	virtual CmdAnsweredStatus AnalyzeAswPlcGetTargetPlatformPos(CString strAsw,std::vector<int> &vnPosIndex, std::vector<CCoordPos> &vCoordPos,std::vector<CPlatformXYDAxisPos> &vPlatformAxis);
	//对象目标不在同一个控制器、采用目标和对象关联，对位时,从PLC获取目标图像坐标和轴位置
	virtual BOOL PackPlcGetTargetImagePos(CString& strInfo);
	//对象目标不在同一个控制器、采用目标和对象关联，对位时,解析从PLC获取的目标图像坐标和轴位置
	virtual CmdAnsweredStatus AnalyzeAswPlcGetTargetImagePos(CString strAsw,std::vector<int> &vnPosIndex, std::vector<CCoordPos> &vImagePos,std::vector<CPlatformXYDAxisPos> &vPlatformAxis);
	// 视觉主动发送给PLC或其他主控系统的指令，以及PLC或其他主控系统返回的指令响应	
	virtual BOOL PackPlcGetPlatformAbsPos(CString& strInfo);
	// 解析收到的“获取平台各轴绝对位置”响应
	virtual CmdAnsweredStatus AnalyzeAswPlcGetGetPlatformAbsPos(CString strAsw, CPlatformAxisPos* vpPlatAxisPos);	

	//视觉主动获取相机轴位置
	virtual BOOL PackPlcGetCamPlatformAbsInfo(CString& strInfo);
	//解析收到的“获取相机轴位置”响应
	virtual CmdAnsweredStatus AnalyzeAswPlcGetCamPlatformAbsPos
		(CString strAsw,std::vector<CPlatformXYAxisPos*>& vpCamPlatformXYAxisPos);
	virtual CmdAnsweredStatus AnalyzeAswGetCamPlatformAbsPos(
		CString strAsw,std::vector<CPlatformXYAxisPos*>& vpCamPlatformXYAxisPos);

	//////////////////////////////////////////////////////////////////////////
	// 移动：打包位置-》打包移动指令-》查询移动完成-》复位触发

	// 打包“指定平台各轴移动到的绝对位置”指令: 移动位置
	virtual BOOL PackPlcWriteDrivePlatformMoveToAbsPos(CString& strInfo, CPlatformAxisPos* vpPlatAxisPos);	

	// 打包“指定平台各轴移动到的绝对位置”指令: 移动指令
	virtual BOOL PackPlcWriteDrivePlatformMove(CString& strInfo);	

	//打包相机轴移动到绝对位置
	virtual BOOL PackPlcWriteDriveCamPlatformMoveToAbsPos(CString& strInfo, std::vector<CPlatformXYAxisPos*> vpPlatAxisPos);

	// 打包“指定平台各轴移动到的绝对位置”响应:打包读取是否移动完成
	virtual BOOL PackPlcReadDrivePlatformMoveCompleted(CString& strInfo);
	// 解析收到的“指定平台各轴移动到的绝对位置”响应：解析是否移动完成
	virtual CmdAnsweredStatus AnalyzeAswPlcDrivePlatformMoveCompleted(CString strAsw);


	// 打包写XYD绝对轴位置到备用轴位置寄存器
	virtual BOOL PackPlcWriteDrivePlatformMoveToAbsPosBackup(CString& strInfo, CPlatformAxisPos* pPlatformAxisPos);


	// 发送“获取平台坐标”命令	
	virtual BOOL PackPlcGetPlatformPosFCBR(CString& strInfo);
	// 解析“获取平台坐标”响应
	virtual CmdAnsweredStatus AnalyzeAswPlcGetPlatformPosFCBR(CString strAsw, double& dPlatformPos1X, double& dPlatformPos1Y, double& dPlatformPos2X, double& dPlatformPos2Y);	

	// 单字通信时，读取PLC设置的补偿值
	virtual BOOL PackPlcGetPlatformAbsPosBackup(CString& strInfo);

	virtual CmdAnsweredStatus AnalyzeAswPlcGetGetPlatformAbsPosBackup(CString& strInfo, CPlatformAxisPos* vpPlatAxisPos);

	virtual BOOL PackRePlcGetPlatformOffsetFGPOAxisPos(CString& strInfo, BOOL bResult, CCoordPos& cpAlignOffset);
	virtual BOOL PackRePlcGetPlatformOffsetFGPO(CString& strInfo, BOOL bResult);
//	virtual BOOL PackRePlcFGPOEnd(CString& strInfo, BOOL bResult);

	//打包读取二维码
	virtual BOOL PackPlcReadQRCode(CString& strInfo,int AddrOffset,int nDataNum=10);
	//打包读取工位数据
	virtual BOOL PackPlcReadSTATIONCode(CString& strInfo,int AddrOffset,int nDataNum=1);
	//打包读取本压工位数据
	virtual BOOL PackPlcReadInspectStationCode(CString& strInfo,int AddrOffset,int nDataNum=1);
	//打包读取指定位置指定数量的数据
	virtual BOOL PackPlcReadTimeCode(CString& strInfo,int AddrOffset,int startaddroffset,int nDataNum=1);

	//解析收到的二维码
	virtual CmdAnsweredStatus AnalyzeAswPlcReadQRCode(CString strAsw,vector<int>& vnReceivedData, int nQRLen, BOOL SnCodeInvert);
	//解析收到的时间数据
	virtual CmdAnsweredStatus AnalyzeAswPlcReadTimeCode(CString strAsw,vector<int>& vnReceivedData, int nQRLen, BOOL SnCodeInvert);

	//打包回复PLC
	virtual BOOL PackRePlcReadQRCode(CString& strInfo,BOOL bResult);

	BOOL PackRe_InspectQRCode(CString& strInfo,std::vector<int> vCode,BOOL bResult,CString strCode);
	BOOL PackRePLC_InspectQRCode(CString& strInfo,std::vector<int> vCode,BOOL bResult);
	BOOL PackRePlc_InspectQRCodeStatus(CString& strInfo, BOOL bResult);

	BOOL PackRePlcSetProductExposureFSPE(CString& strInfo,BOOL bResult);		// 设置产品曝光

	// 打包获取目标指定位置的图像坐标 FTGG
	BOOL PackRePlcAlignResultInfoFTGGAxisPos(CString& strInfo,CCoordPos imageOffset, BOOL bResult);
	BOOL PackRePlcAlignResultInfoFTGG(CString& strInfo, BOOL bResult);

	// 打包获取对象指定位置的图像坐标 FOBG
	BOOL PackRePlcAlignResultInfoFOBGAxisPos(CString& strInfo,CCoordPos imageOffset, BOOL bResult);
	BOOL PackRePlcAlignResultInfoFOBG(CString& strInfo, BOOL bResult);


	// 专用焊接指令
	BOOL PackRePlcAlignResultInfoFWEL(CString& strInfo,BOOL bResult);			// 专用焊接指令
	BOOL PackRePlcAlignResultInfoFWELAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult);
	BOOL PackRePlcAlignResultInfoFSOL(CString& strInfo,BOOL bResult);			// 专用焊接指令
	BOOL PackRePlcAlignResultInfoFSOLAxisPos(CString& strInfo,CCoordPos platOffset,BOOL bResult);


	BOOL PackRePlcObjTarSearchFPCB(CString& strInfo,BOOL bResult,std::vector<CCoordPos> markPos); // 对象、目标图像定位	
	BOOL PackRePlcPCBPrecisionFPIS(CString& strInfo,BOOL bResult); // PCB 精度检测
	BOOL PackRePlcPCBPrecisionFCPI(CString& strInfo,BOOL bResult);

	// 检测产品有、无
	BOOL PackRePlcProductInspectFINS(CString& strInfo,BOOL bResult);

	// 打包握手信息
	virtual BOOL PackPlcShakeHands(CString& strInfo);
	// 解析握手信息
	virtual CmdAnsweredStatus AnalyzeAswPlcShakeHands(CString strAsw);

	BOOL PackPlcReadNewName(CString& strInfo, int nDataNum);
	BOOL AnalyzePlcNewName(CString strCmd, int nDataNum, CString& strNewName);

	// TT 20200907 单字转双字
	int WordToDoubleWord(int nLowWord,int nHighWord);
	// TT 20200907 双字转单字
	BOOL DoubleWordToWord(int nDWord,int &nLowWord,int &nHighWord);
	// TT 20200907 通用打包写寄存器指令
	BOOL PackPlcWriteReg(CString& strInfo, std::vector<int> &vnWriteData,int nRegAddr,bool bRegType,bool bOffsetAddr);
	// TT 20200907 解析随指令参数的轴位置信息
	BOOL AnalyzeAxisInfoInCmdParam(std::vector<int> &vnReceivedData);
	BOOL AnalyzeAxisInfoInCmdParamEx(std::vector<int> &vnReceivedData);
	// TT 20200907 获取读取PLC指令时的读取个数
	int GetReadDataNum();
	int GetReadDataNumEx();
	// TT 20200907 保存需要发送给PLC的轴信息
	BOOL SaveAxisPosDataNeedSend(std::vector<int> &vnAxisPosData,CString &strInfo);
	// TT 20200907 在发送给PLC的指令参数后附加轴信息，指令参数较少时，以0填充
	BOOL AttachAxisDataToCmd(std::vector<int> &vnCmdWriteData);
	// TT 20200907 设置是否允许轴位置随指令一并读写
	BOOL EnableReadWriteAxisPosWithCmd(BOOL bEnbale);
	// TT 20200907 是否是三菱Q寄存器协议
	BOOL IsMitsubishiQProtocol();
	std::vector<int>	m_vnWriteDataAxisPos;		//TT 20200907 需要随指令一并发送的轴位置信息
	BOOL				m_bCmdAxisPosReady;			//TT 20200907 轴位置已随指令读取
protected:
	BOOL                m_bReadWriteAxisPosWithCmd;	//TT 20200907 轴位置随指令参数一并读写
	// 根据协议类型，创建PLC通信协议类对象，如果ptProtocolType不是PLC协议则返回False
	BOOL	CreateCommProPlc(ProtocolType ptProtocolType);
	virtual BOOL PackPlcParam(int& nPackParam, int nParam, int nUnitExp = 0);
	virtual BOOL PackPlcParam(int& nPackParam, double dParam, int nUnitExp = 0);
public:
	virtual	SysWorkCmdType AnalyzePlcSysWorkCmdReceivedEx(CString strCmd, BOOL &bTriggered);
	BOOL PackPlcReadSysWorkCmdEx( CString& strInfo);

	// 多段地址分别触发信号
	BOOL PackPlcReadSysWorkCmdEx2( CString& strInfo,int nOffset);

	virtual BOOL PackPlcWriteSetPlcCmdTriggerEx(CString& strInfo, BOOL bTriggered,int nOffSet=0);
	BOOL PackWritePLCFunc(CString& strInfo,int nResult,int nOffSet=0);	// 单数据写通用函数，nResult-打包回复的数值，nOffset-打包回复的基地址地址偏移量  strInfo为打包后字符串
	//BOOL PackWritePLCFuncInspect(CString& strInfo,vector<double> nResult,int nOffSet=0);	// 单数据写通用函数，nResult-打包回复的数值，nOffset-打包回复的基地址地址偏移量  strInfo为打包后字符串
	BOOL PackWritePLCWithDetectionResultFunc(CString& strInfo,int nResult,vector<double> DetectionResult,int nOffSet);//顺便发送线距检测
	BOOL PackWritePLCWithDetectionResultFunc2(CString& strInfo,int nResult,vector<int> DetectionResult,int nOffSet);//发送多个参数
	BOOL PackReadPLCFunc(CString& strInfo,int nDataNums,int nOffSet=0,BOOL bAxisGet = FALSE);	// 数据读通用函数，nDataNums-读取数据个数，nOffset-读取基地址地址+PLC操作区地址+偏移量nOffset开始的数据  strInfo为打包后字符串
	BOOL PackPlcWriteDrivePlatformMoveToAbsPosEx( CString& strInfo, CPlatformAxisPos* vpPlatAxisPos,int nOffSet=0);
	BOOL PackWritePLCFuncWithAxisEx( CString& strInfo, CPlatformAxisPos* vpPlatAxisPos,CPlatformAxisPos* vpPlatAxisPosBase,int nOffSet=0,int nResult = 0);
	BOOL PackWritePLCFuncWithAxisAndBenchCenterEx( CString& strInfo, CPlatformAxisPos* vpPlatAxisPos,CPlatformAxisPos* vpPlatAxisPosBase,CPlatformAxisPos* vpPlatAxisPosCenter,int nOffSet=0,int nResult = 0);
	
	BOOL PackPlcWriteDrivePlatformMoveToAbsPosEx( CString& strInfo, CPlatformAxisPos* vpPlatAxisPos,CPlatformAxisPos* vpPlatAxisPosBase,int nOffSet=0);
	CmdAnsweredStatus AnalyzeAswPlcDrivePlatformMoveCompletedEx( CString strAsw);
	
	BOOL PackReAlignResultInfo_NoProtocol( CString& strInfo, CCoordPos platOffset,int nResult, CPlatformAxisPos* vpPlatAxisPos);
	BOOL PackRePLCBaseFunc(CString& strInfo,int nResult);
	BOOL PackRePLCBaseFunc2(CString& strInfo,int nResult,vector<int> DetectionResult);

	// 指令解析模式2 通过m_vKeyenceCodeParam设置拍照对位触发指令码以及固定的参数：m_vKeyenceCodeParam具体设置目前通过 WorkStationConfig.xml文件中修改
	SysWorkCmdType AnalyzePlcSysWorkCmdReceivedEx_FixCode( CString strCmd, BOOL &bTriggered);
	SysWorkCmdType AnalyzePlcSysWorkCmdReceivedEx_FixCode( CString strCmd, BOOL &bTriggered,CKeyenceCodeParam &cCodeParam);
	
	SysWorkCmdType AnalyzePlcSysWorkCmdReceivedEx_FixCode2( CString strCmd, BOOL &bTriggered,CKeyenceCodeParam &cCodeParam,int nMulReg);
	std::vector<CKeyenceCodeParam> m_vKeyenceCodeParam; //指令码
	int m_nCurCodeIndex; // 当前指令码索引
	
	// 指令解析模式3：触发指令码所在区域对应不同指令参数
	SysWorkCmdType AnalyzePlcSysWorkCmdReceivedEx_CodeWithParam(CString strCmd, BOOL &bTriggered,CKeyenceCodeParam &cCodeParam);

	BOOL PackReMulAlignPosInfo_Ex(CString& strInfo,std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset,int nResult);
	BOOL PackRePlcMulAlignPosResultInfoAxisPos_Ex(CString& strInfo,std::vector<pair<CCoordPos, int>> vpPlatformXYDOffset,int nResult);

	BOOL PackReMulAlignPosInfoFALP_Ex(CString& strInfo,std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos,int nResult,int nPickMutiExMode/* = 0*/);
	BOOL PackRePlcMulAlignPosResultInfoFALPAxisPos_Ex(CString& strInfo,std::vector<int> &vnInfo,int &nRegAddr,std::vector<std::vector<CPlatformXYDAxisPos>> vvPlatformXYDAxisPos,int nResult,int nPickMutiExMode/* = 0*/);
	BOOL PackReMulRowColsSimpleInfo_Ex(CString& strInfo,int nResult,int nSize,std::vector<int> vnRows,std::vector<int> vnCols);
	BOOL PackReMulRowCols_Ex(CString& strInfo,int nSize,std::vector<int> vnRows,std::vector<int> vnCols,int nOffSet=0);

	BOOL PackRePlcMulResult_Ex(CString& strInfo, std::vector<double> vdDate,int nOffset=0);
	BOOL PackRePlcMulResult_SimpleInfo_Ex(CString& strInfo, std::vector<double> vdDate,int nResult,int nSearchNum);

	BOOL				m_bEnableMulRegTrigger;

	int m_nCurRegTriggerIndex;
	std::vector<CMulRegTriggerSet> m_vnMulRegTriggerSet;		// 多段地址触发时参数设置;

	BOOL ConvertToPlatformXYDAxisPos_Output(CCoordPos platOffset,std::vector<double>& vdParam,std::vector<int>& vnTrans ,std::vector<int>& vnUnitExp);

	BOOL PackWriteBendPressPara(CString& strInfo,long lAddre,std::vector<int> vnDtata);	
};

#endif // !defined(AFX_COMMPROTOCOLXYDFAST_H__55A666F4_9BA1_4092_8104_9169103F7805__INCLUDED_)
