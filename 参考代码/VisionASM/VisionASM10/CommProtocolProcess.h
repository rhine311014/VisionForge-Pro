// CommProtocolProcess.h: interface for the CCommProtocolProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMPROTOCOLPROCESS_H__30AE4D85_B685_4811_B0DD_163B0D8DFBB6__INCLUDED_)
#define AFX_COMMPROTOCOLPROCESS_H__30AE4D85_B685_4811_B0DD_163B0D8DFBB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vsComm.h"
#include "vsAlignToolDef.h"
#include "SystemOptionConfig.h"
#include "CommProRegBase.h"

/**********************驱动平台各轴移动到指定位置************************/
// 指令
#define CMD_DRIVE_PLATFORM_TO_ABS               _T("&TAR")
// 响应
#define CMD_DRIVE_PLATFORM_SUCCEED			    _T("&TAR00")	// 平台移动响应指令： 正常结束
#define CMD_DRIVE_PLATFORM_COMM_ERR			    _T("&TAR01")	// 平台移动响应指令： 通信错误
#define CMD_DRIVE_PLATFORM_CMD_PARAM_ERR	    _T("&TAR02")	// 平台移动响应指令： 指令参数错误
#define CMD_DRIVE_PLATFORM_CMD_ERR		     	_T("&TAR03")	// 平台移动响应指令： 指令错误
#define CMD_DRIVE_PLATFORM_UNKNOWN_ERR		    _T("&TAR05")	// 平台移动响应指令： 其他错误

/****************************获取平台各轴位置****************************/
// 指令
#define CMD_GET_PLATFORM_AXIS_ABS_POS            _T("2MNPS13")//for Fast  //Old// _T("&TAG")		// 获取平台各轴位置
// 响应
#define CMD_GET_PLATFORM_POS_SUCCEED			_T("&TAR00")	// 获取平台各轴位置响应指令： 正常结束
#define CMD_GET_PLATFORM_POS_COMM_ERR			_T("&TAR01")	// 获取平台各轴位置响应指令： 通信错误
#define CMD_GET_PLATFORM_POS_CMD_PARAM_ERR	    _T("&TAR02")	// 获取平台各轴位置响应指令： 指令参数错误
#define CMD_GET_PLATFORM_POS_CMD_ERR		    _T("&TAR03")	// 获取平台各轴位置响应指令： 指令错误
#define CMD_GET_PLATFORM_POS_UNKNOWN_ERR		_T("&TAR05")	// 获取平台各轴位置响应指令： 其他错误

/********************驱动相机平台各轴移动到指定位置**********************/
// 指令
#define CMD_DRIVE_CAM_PLATFORM_TO_ABS            _T("&TCR")
// 响应
#define CMD_DRIVE_CAM_PLATFORM_SUCCEED			 _T("&TCR00")	// 相机平台移动响应指令： 正常结束
#define CMD_DRIVE_CAM_PLATFORM_COMM_ERR			 _T("&TCR01")	// 相机平台移动响应指令： 通信错误
#define CMD_DRIVE_CAM_PLATFORM_CMD_PARAM_ERR	 _T("&TCR02")	// 相机平台移动响应指令： 指令参数错误
#define CMD_DRIVE_CAM_PLATFORM_CMD_ERR		     _T("&TCR03")	// 相机平台移动响应指令： 指令错误
#define CMD_DRIVE_CAM_PLATFORM_UNKNOWN_ERR		 _T("&TCR05")	// 相机平台移动响应指令： 其他错误

/************************获取相机平台各轴位置****************************/
// 指令
#define CMD_GET_CAM_PLATFORM_AXIS_ABS_POS        _T("&TCG")		// 获取相机平台各轴位置
// 响应
#define CMD_GET_CAM_PLATFORM_POS_SUCCEED	     _T("&TCG00")	// 获取相机平台各轴位置响应指令： 正常结束
#define CMD_GET_CAM_PLATFORM_POS_COMM_ERR		 _T("&TCG01")	// 获取相机平台各轴位置响应指令： 通信错误
#define CMD_GET_CAM_PLATFORM_POS_CMD_PARAM_ERR	 _T("&TCG02")	// 获取相机平台各轴位置响应指令： 指令参数错误
#define CMD_GET_CAM_PLATFORM_POS_CMD_ERR		 _T("&TCG03")	// 获取相机平台各轴位置响应指令： 指令错误
#define CMD_GET_CAM_PLATFORM_POS_UNKNOWN_ERR	 _T("&TCG05")	// 获取相机平台各轴位置响应指令： 其他错误

/******************************执行系统标定******************************/
// 指令
#define CMD_EXCUTE_CALIB                         _T("&CAE")     // 请求执行系统标定
// 响应
#define CMD_CALIB_SUCCEED		            	 _T("&CAE00")	// 标定响应指令： 正常结束
#define CMD_CALIB_COMM_ERR			             _T("&CAE01")	// 标定响应指令： 通信错误
#define CMD_CALIB_CMD_PARAM_ERR		             _T("&CAE02")	// 标定响应指令： 指令参数错误
#define CMD_CALIB_CMD_ERR			             _T("&CAE03")	// 标定响应指令： 指令错误	
#define CMD_CALIB_SEARCH_ERR		             _T("&CAE04")	// 标定响应指令： 搜索错误
#define CMD_CALIB_UNKNOWN_ERR	                 _T("&CAE05")	// 标定响应指令： 其他错误

/******************************执行系统对位******************************/
// 指令
#define CMD_ALIGN_REQUEST			            _T("&AAE")		// 请求执行对位
// 响应
#define CMD_ALIGN_SUCCEED			            _T("&AAE00")	// 对位响应指令： 正常结束
#define CMD_ALIGN_COMM_ERR			            _T("&AAE01")	// 对位响应指令： 通信错误
#define CMD_ALIGN_CMD_PARAM_ERR	             	_T("&AAE02")	// 对位响应指令： 指令参数错误
#define CMD_ALIGN_CMD_ERR			            _T("&AAE03")	// 对位响应指令： 指令错误	
#define CMD_ALIGN_SEARCH_ERR		            _T("&AAE04")	// 对位响应指令： 搜索错误
#define CMD_ALIGN_UNKNOWN_ERR		            _T("&AAE05")	// 对位响应指令： 其他错误

/****************************执行标定定位搜索*****************************/
// 指令
#define CMD_CAG_SEARCH_REQUEST			        _T("&CAG")		// 请求执行标定定位
// 响应
#define CMD_CAG_SEARCH_SUCCEED			        _T("&CAG00")	// 标定定位搜索响应指令： 正常结束
#define CMD_CAG_SEARCH_COMM_ERR			        _T("&CAG01")	// 标定定位搜索响应指令： 通信错误
#define CMD_CAG_SEARCH_CMD_PARAM_ERR	        _T("&CAG02")	// 标定定位搜索响应指令： 指令参数错误
#define CMD_CAG_SEARCH_CMD_ERR			        _T("&CAG03")	// 标定定位目标搜索响应指令： 指令错误
#define CMD_CAG_SEARCH_SEARCH_ERR		        _T("&CAG04")	// 标定定位搜索响应指令： 搜索错误
#define CMD_CAG_SEARCH_UNKNOWN_ERR		        _T("&CAG05")	// 标定定位目标搜索响应指令： 其他错误

/****************************执行实时目标搜索*****************************/
// 指令
#define CMD_TGG_SEARCH_REQUEST			        _T("&TGG")		// 请求执行实时目标搜索
// 响应
#define CMD_TGG_SEARCH_SUCCEED			        _T("&TGG00")	// 实时目标搜索响应指令： 正常结束
#define CMD_TGG_SEARCH_COMM_ERR			        _T("&TGG01")	// 实时目标搜索响应指令： 通信错误
#define CMD_TGG_SEARCH_CMD_PARAM_ERR	        _T("&TGG02")	// 实时目标搜索响应指令： 指令参数错误
#define CMD_TGG_SEARCH_CMD_ERR			        _T("&TGG03")	// 实时目标搜索响应指令： 指令错误
#define CMD_TGG_SEARCH_SEARCH_ERR		        _T("&TGG04")	// 实时目标搜索响应指令： 搜索错误
#define CMD_TGG_SEARCH_UNKNOWN_ERR		        _T("&TGG05")	// 实时目标搜索响应指令： 其他错误

/****************************执行实时对象搜索*****************************/
// 指令
#define CMD_OBG_SEARCH_REQUEST			        _T("&OBG")		// 请求执行实时对象搜索
// 响应
#define CMD_OBG_SEARCH_SUCCEED			        _T("&OBG00")	// 实时对象搜索响应指令： 正常结束
#define CMD_OBG_SEARCH_COMM_ERR			        _T("&OBG01")	// 实时对象搜索响应指令： 通信错误
#define CMD_OBG_SEARCH_CMD_PARAM_ERR	        _T("&OBG02")	// 实时对象搜索响应指令： 指令参数错误
#define CMD_OBG_SEARCH_CMD_ERR			        _T("&OBG03")	// 实时对象搜索响应指令： 指令错误
#define CMD_OBG_SEARCH_SEARCH_ERR		        _T("&OBG04")	// 实时对象搜索响应指令： 搜索错误
#define CMD_OBG_SEARCH_UNKNOWN_ERR		        _T("&OBG05")	// 实时对象搜索响应指令： 其他错误

/****************************执行虚拟对象搜索*****************************/
// 指令
#define CMD_VBG_SEARCH_REQUEST			        _T("&VBG")		// 请求执行虚拟对象搜索
// 响应
#define CMD_VBG_SEARCH_SUCCEED			        _T("&VBG00")	// 虚拟对象搜索响应指令： 正常结束
#define CMD_VBG_SEARCH_COMM_ERR			        _T("&VBG01")	// 虚拟对象搜索响应指令： 通信错误
#define CMD_VBG_SEARCH_CMD_PARAM_ERR	        _T("&VBG02")	// 虚拟对象搜索响应指令： 指令参数错误
#define CMD_VBG_SEARCH_CMD_ERR			        _T("&VBG03")	// 虚拟对象搜索响应指令： 指令错误
#define CMD_VBG_SEARCH_SEARCH_ERR		        _T("&VBG04")	// 虚拟对象搜索响应指令： 搜索错误
#define CMD_VBG_SEARCH_UNKNOWN_ERR		        _T("&VBG05")	// 虚拟对象搜索响应指令： 其他错误

/******************建立虚拟对象与实时对象间的位置关系**********************/
// 指令
#define CMD_COV_SEARCH_REQUEST			        _T("&COV")		// 请求建立虚拟对象与实时对象间的位置关系
// 响应
#define CMD_COV_SUCCEED			                _T("&COV00")	// 建立关系响应指令： 正常结束
#define CMD_COV_COMM_ERR			            _T("&COV01")	// 建立关系响应指令： 通信错误
#define CMD_COV_CMD_PARAM_ERR	                _T("&COV02")	// 建立关系响应指令： 指令参数错误
#define CMD_COV_CMD_ERR			                _T("&COV03")	// 建立关系响应指令： 指令错误
#define CMD_COV_SEARCH_ERR		                _T("&COV04")	// 建立关系响应指令： 搜索错误
#define CMD_COV_UNKNOWN_ERR		                _T("&COV05")	// 建立关系响应指令： 其他错误


/****************************切换产品*****************************/
// 指令
#define CMD_TP_REQUEST							_T("TP,1234567890")		// 请求切换产品
// 响应
#define CMD_TP_SUCCEED							_T("TP,00")				// 请求切换产品响应指令： 正常结束
#define CMD_TP_UNKNOWN_ERR						_T("TP,01")				// 请求切换产品响应指令： 其他错误

// 指令
#define CMD_TN_REQUEST							_T("TN,000")				// 请求切换产品
// 响应
#define CMD_TN_SUCCEED							_T("TN,00")				// 请求切换产品响应指令： 正常结束
#define CMD_TN_UNKNOWN_ERR						_T("TN,01")				// 请求切换产品响应指令： 其他错误


// 
typedef struct tagCommProtocolParam
{
	tagCommProtocolParam()
	{
		m_nCommUnitExp = 0;
		m_nCommXUnitExp = 4;				// 串口X单位幂4
		m_nCommYUnitExp = 4;				// 串口Y单位幂4
		m_nCommDUnitExp = 5;				// 串口D单位幂5
		m_nCommDataLenType = 0;				// 串口数据长度：0：固定长度；1：实际长度；
		m_nCommTimeOut = 30;				// 串口超时
		m_nCommReSendTimes = 3;				// 串口重复次数
	}
	int		m_nCommUnitExp;
	int		m_nCommXUnitExp;			// 串口X单位幂4
	int		m_nCommYUnitExp;			// 串口X单位幂4
	int		m_nCommDUnitExp;			// 串口D单位幂5
	int		m_nCommDataLenType;			// 串口数据长度：0：固定长度；1：实际长度；
	int		m_nCommTimeOut;				// 串口超时
	int     m_nCommReSendTimes;			// 串口重复次数
}CommProtocolParam;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//lzk 0408
#define COMM_CMD_STRING_FVPC	("FVPC") // 7、FVPC:切换目标对象虚拟Mark位置模式
#define COMM_CMD_STRING_FPMI	("FPMI") // 8、FPMI:通过FPMI指令来开启系统设置权限
#define COMM_CMD_STRING_FBIS	("FBIS") // 8、FPMI:通过FPMI指令来开启系统设置权限

// 产品
#define COMM_CMD_STRING_FSPC	("FSPC") // 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
#define COMM_CMD_STRING_FSSV	("FSSV") // 2、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
#define COMM_CMD_STRING_FGSP	("FGSP") // 3、 FGSP：产品No.的取得，取得目前正在执行的产品No.
#define COMM_CMD_STRING_FVAS	("FVAS") // 1、 FVAS：设置虚拟Mark开启状态
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Mark：标定
#define COMM_CMD_STRING_FCLB	("FCLB") // 1、FCLB: Calibration    的执行 执行Calibration
#define COMM_CMD_STRING_FCBS	("FCBS") // 1、FCBS:分步Calibration 的执行 执行Calibration
#define COMM_CMD_STRING_FCBR	("FCBR") // 1、FCBR:关联Calibration 的执行 执行Calibration
#define COMM_CMD_STRING_FCPR	("FCPR") // 1、FCPR:相机轴标定
#define COMM_CMD_STRING_FCBP    ("FCBP") // 1、FCBP:自动关联
//////////////////////////////////////////////////////////////////////////
// Mark:目标Mark
#define COMM_CMD_STRING_FTGT	("FTGT") // 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
#define COMM_CMD_STRING_FTGS	("FTGS") // 1、FTGS:目标 Mark 位置的取得，取得目标 Mark（目标）位置
#define COMM_CMD_STRING_FTGC	("FTGC") // 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
#define COMM_CMD_STRING_FTGR	("FTGR") // 3、FTGR:目标 Mark 位置的登录，将目前使用的产品数据保存在指定的产品No.中并保存在CF 记忆卡
#define COMM_CMD_STRING_FTGG	("FTGG") // 4、FTGG:目标 Mark 画素的取得，取得执行“ＦＴＧＴ”结果的画素
#define COMM_CMD_STRING_FTGD	("FTGD") // 5、FTGD:目标 Mark 指示线的全画面显示，在指定目标 Mark No.的目标 Mark 位置上显示全画面的指示线及刻度
#define COMM_CMD_STRING_FGTW    ("FGTW") // 6、FGTW:视觉进行目标拍照并发送目标平台坐标和轴位置给PLC
#define COMM_CMD_STRING_FAFO    ("FAFO") // 7、FAFO:目标采用靶标关联时，计算目标坐标系到对象坐标系的固定补偿
#define COMM_CMD_STRING_FGTP    ("FGTP") // 8、FGTW:视觉进行目标拍照并发送目标图像坐标和轴位置给PLC
//////////////////////////////////////////////////////////////////////////
// Mark:物体Mark
#define COMM_CMD_STRING_FOBJ	("FOBJ") // 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
#define COMM_CMD_STRING_FOBC	("FOBC") // 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
#define COMM_CMD_STRING_FOBG	("FOBG") // 3、FOBG:物体 Mark 画素的取得，取得执行“ＦＯＢＪ”结果的画素
#define COMM_CMD_STRING_FMSC	("FMSC") // 4、FMSC:手动搜索位置的取得，在自动对准时，当目标 Mark 或物体 Mark 搜索失败时，可利用手动方法（鼠标）指定Mark 位置
#define COMM_CMD_STRING_FWLD	("FWLD") // 5、FWLD：取得目标（目标靶标）或对象（对象靶标）的平台坐标上的位置
#define COMM_CMD_STRING_FGAP	("FGAP") // 6、FGAP:对象 Mark 画素的取得，取得执行结果的画素
#define COMM_CMD_STRING_FCNT	("FCNT") // 7、FCNT:对象定位同时将目标位置设为图像中心，参数类似于FOBJ P1
#define COMM_CMD_STRING_FGTC	("FGTC") // 8、FGTC:获取目标相对于图像中心的图像坐标偏差

#define COMM_CMD_STRING_FCNO	("FCNO") // 7、FCNT:目标定位同时将对象位置设为图像中心

//////////////////////////////////////////////////////////////////////////
// 对准
#define COMM_CMD_STRING_FGAL	("FGAL") // 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
#define COMM_CMD_STRING_FALL	("FALL") // 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
#define COMM_CMD_STRING_FGAC	("FGAC") // 3、FGAC:对准最多次数的取得，取得设定对准最多次数
#define COMM_CMD_STRING_FALC	("FALC") // 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
#define COMM_CMD_STRING_FAAL	("FAAL") // 5、FAAL:自动对准的执行，执行自动对准（连续对准）
#define COMM_CMD_STRING_FXYA	("FXYA") // 5、XY+D平台FAAL:自动对准的执行，执行自动对准（连续对准）
#define COMM_CMD_STRING_FAAB	("FAAB") // 5、FAAB:自动对准的执行，执行自动对准（连续对准），增加图像十字中心基准对位过程
#define COMM_CMD_STRING_FAAC	("FAAC") // 5.1、FAAC:自动对准的执行，执行自动对准（连续对准）,对象候补搜索
#define COMM_CMD_STRING_FACB	("FACB") // 5.1、FACB:自动对准的执行，执行自动对准（连续对准）,对象候补搜索，增加图像十字中心基准对位过程
#define COMM_CMD_STRING_FAHL	("FAHL") // 5.2、FAHL:Align high vs low layer背光贴片机自动对准的执行，执行自动对准（连续对准）
#define COMM_CMD_STRING_FANS	("FANS") // 6、FANS:对准后的偏移量的取得，取得对准后平台坐标系的偏移量
#define COMM_CMD_STRING_FALG	("FALG") // 7、FALG:１次对准移动量的取得，取得１次对准移动量
#define COMM_CMD_STRING_FALP	("FALP") // 7、FALP:１次对准拾取移动量的取得，取得１次对准拾取移动量
#define COMM_CMD_STRING_FALJ	("FALJ") // 8、FALJ:1:次对准精度判定与对准移动量的取得，对准移动量是否在对准精度内的判定与对准移动量的取得
#define COMM_CMD_STRING_FACO	("FACO") // 9、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
#define COMM_CMD_STRING_FAAO    ("FAAO") // 9、FAAO:偏移量的去的，对象目标自动拍照，目标为图像中心
#define COMM_CMD_STRING_FOLC	("FOLC") // 9、FOLC:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
#define COMM_CMD_STRING_FABO	("FABO") // 10、FABO:偏移量的取得，取得目标图像十字中心及物体 Mark 之间中心的平台坐标系偏移量
#define COMM_CMD_STRING_FAOT	("FAOT") // 5.1、FAOT:自动对准的执行，执行自动对准（连续对准）,对象候补搜索,目标候补连续搜索
#define COMM_CMD_STRING_FALX	("FALX") // 5.1、FALX:自动对准的执行，执行自动对准（连续对准）,对象候补连续搜索
#define COMM_CMD_STRING_FWDR	("FWDR") // 5.1、FWDR:获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
#define COMM_CMD_STRING_FGOW	("FGOW") // 6、FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
#define COMM_CMD_STRING_FMCO	("FMCO") // 9、FMCO(MultiCalculateOffset):偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
// 检测
#define COMM_CMD_STRING_FCHB	("FCHB") // 2、ACF检测
#define COMM_CMD_STRING_FCHC	("FCHC") // 2、候补ACF检测
#define COMM_CMD_STRING_FDRP	("FDRP") // 7、FDRP:计算基准拾取位置自动流程
#define COMM_CMD_STRING_FQRC	("FQRC")
#define COMM_CMD_STRING_FQRI	("FQRI")
// 
//////////////////////////////////////////////////////////////////////////
// 补偿量・移动
#define COMM_CMD_STRING_FAOF	("FAOF") // 1、FAOF:自动补偿量值的取得，对准偏移量以补偿量值取得
#define COMM_CMD_STRING_FGOF	("FGOF") // 2、FGOF:补偿量值的取得，取得设定的补偿量值
#define COMM_CMD_STRING_FOFS	("FOFS") // 3、FOFS:补偿量值的设定，设定补偿量值
#define COMM_CMD_STRING_FSFT	("FSFT") // 4、FSFT:平台Shift 移动动作，选择内部转移量的设定值，并依照设定值将目前平台的位置做相对移动
#define COMM_CMD_STRING_FGSF	("FGSF") // 5、FGSF:对准Shift 移动量的取得，取得对准转移量
#define COMM_CMD_STRING_FMPS	("MNPS") // 6、MNPS:平台现在位置的取得，取得平台现在位置
#define COMM_CMD_STRING_FMVA	("MMVA") // 7、MMVA:平台絶对准置移动量的设定，设定平台絶对准置移动量
#define COMM_CMD_STRING_FMVR	("MMVR") // 8、MMVR:平台相对准置移动量的设定，设定平台相对准置移动量
#define COMM_CMD_STRING_FGPO	("FGPO") // 9、FGPO:获取连续对位的最终精度

//////////////////////////////////////////////////////////////////////////
// Alarm
#define COMM_CMD_STRING_FALM	("FALM") // 1、FALM:Alarm资料的取得，取得错误发生的数据代码
#define COMM_CMD_STRING_FRST	("FRST") // 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
#define COMM_CMD_STRING_FSAC	("FSAC") // 3、FSAC：保存校正、定位时的数据。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
#define COMM_CMD_STRING_FSCO	("FSCO") // 4、FSCO：保存指令收发的历史纪录。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）

//////////////////////////////////////////////////////////////////////////
#define COMM_CMD_STRING_FCHK	("FCHK") // 4、FCHK：特殊通讯指令，互相握手后，软件有效

#define COMM_CMD_STRING_FTGM	("FTGM") // 1、FTGT:目标 Mark 位置的手动取得，取得目标 Mark（目标）位置
#define COMM_CMD_STRING_FOBM	("FOBM") // 1、FOBJ:物体 Mark 位置的手动取得，取得物体 Mark（对准靶标）位置

#define COMM_CMD_STRING_FSPE	("FSPE") // 相机曝光时间设置
#define COMM_CMD_STRING_FWEL	("FWEL") // 专用焊接指令,小FPC:XY,大FPC:XYD
#define COMM_CMD_STRING_FSOL	("FSOL") // 专用焊接指令,小FPC:XYD,大FPC:XY
#define COMM_CMD_STRING_FMPB	("MPSB") // 获取备用轴位置
#define COMM_CMD_STRING_FPCB	("FPCB") // 对象、目标Mark图像位置
#define COMM_CMD_STRING_FPIS	("FPIS") // PCB精度检测 PCB Inspect
#define COMM_CMD_STRING_FCPI	("FCPI") // 更改平台序号
#define COMM_CMD_STRING_FINS	("FINS") // 检测产品有、无
#define COMM_CMD_STRING_FCCI	("FCCI") // 1、FCCI:复制标定信息
//////////////////////////////////////////////////////////////////////////
// 产品
#define COMM_CMD_STRING_VSPC	("VSPC") // 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
#define COMM_CMD_STRING_VSSV	("VSSV") // 2、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
#define COMM_CMD_STRING_VGSP	("VGSP") // 3、 FGSP：产品No.的取得，取得目前正在执行的产品No.
#define COMM_CMD_STRING_VVAS	("VVAS") // 1、 FVAS：虚拟Mark开关设定

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Mark：标定
#define COMM_CMD_STRING_VCLB	("VCLB") // 1、FCLB:Calibration 的执行 执行Calibration
#define COMM_CMD_STRING_VCBS	("VCBS") // 1、FCBS:分步Calibration 的执行 执行Calibration
#define COMM_CMD_STRING_VCBR	("VCBR") // 1、FCBR:关联Calibration 的执行 执行Calibration
#define COMM_CMD_STRING_VCPR	("VCPR") // 1、相机轴标定
#define COMM_CMD_STRING_VCBP    ("VCBP") // 1、FCBP:自动关联
//////////////////////////////////////////////////////////////////////////
// Mark:目标Mark
#define COMM_CMD_STRING_VTGT	("VTGT") // 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
#define COMM_CMD_STRING_VTGS	("VTGS") // 1、FTGS:目标 Mark 位置的取得，取得目标 Mark（目标）位置
#define COMM_CMD_STRING_VTGC	("VTGC") // 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
#define COMM_CMD_STRING_VTGR	("VTGR") // 3、FTGR:目标 Mark 位置的登录，将目前使用的产品数据保存在指定的产品No.中并保存在CF 记忆卡
#define COMM_CMD_STRING_VTGG	("VTGG") // 4、FTGG:目标 Mark 画素的取得，取得执行“ＦＴＧＴ”结果的画素
#define COMM_CMD_STRING_VTGD	("VTGD") // 5、FTGD:目标 Mark 指示线的全画面显示，在指定目标 Mark No.的目标 Mark 位置上显示全画面的指示线及刻度
#define COMM_CMD_STRING_VGTW    ("VGTW")
#define COMM_CMD_STRING_VAFO    ("VAFO")
#define COMM_CMD_STRING_VGTP    ("VGTP")
//////////////////////////////////////////////////////////////////////////
// Mark:物体Mark
#define COMM_CMD_STRING_VOBJ	("VOBJ") // 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
#define COMM_CMD_STRING_VOBC	("VOBC") // 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
#define COMM_CMD_STRING_VOBG	("VOBG") // 3、FOBG:物体 Mark 画素的取得，取得执行“ＦＯＢＪ”结果的画素
#define COMM_CMD_STRING_VMSC	("VMSC") // 4、FMSC:手动搜索位置的取得，在自动对准时，当目标 Mark 或物体 Mark 搜索失败时，可利用手动方法（鼠标）指定Mark 位置
#define COMM_CMD_STRING_VWLD	("VWLD") // 5、FWLD：取得目标（目标靶标）或对象（对象靶标）的平台坐标上的位置
#define COMM_CMD_STRING_VGAP	("VGAP") // 4、FGAP:对象Mark 画素的取得，取得执行结果的画素
#define COMM_CMD_STRING_VCNT	("VCNT") // 4、FCNT:对象定位同时将目标位置设为图像中心，参数类似于FOBJ P1
#define COMM_CMD_STRING_VGTC	("VGTC") // 5、FGTC:获取目标相对于图像中心的图像坐标偏差

#define COMM_CMD_STRING_VCNO	("VCNO") // 4、FCNT:对象定位同时将目标位置设为图像中心，参数类似于FOBJ P1

#define COMM_CMD_STRING_VVPC	("VVPC")  // 6、FVPC:切换目标对象虚拟Mark位置模式

//////////////////////////////////////////////////////////////////////////
// 对准
#define COMM_CMD_STRING_VGAL	("VGAL") // 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
#define COMM_CMD_STRING_VALL	("VALL") // 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
#define COMM_CMD_STRING_VGAC	("VGAC") // 3、FGAC:对准最多次数的取得，取得设定对准最多次数
#define COMM_CMD_STRING_VALC	("VALC") // 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
#define COMM_CMD_STRING_VAAL	("VAAL") // 5、FAAL:自动对准的执行，执行自动对准（连续对准）
#define COMM_CMD_STRING_VXYA	("VXYA") // 5、FAAL:自动对准的执行，执行自动对准（连续对准）
#define COMM_CMD_STRING_VAAB	("VAAB") // 5、FAAB:自动对准的执行，执行自动对准（连续对准），增加图像十字中心基准对位过程
#define COMM_CMD_STRING_VAAC	("VAAC") // 5.1、FAAC:自动对准的执行，执行自动对准（连续对准）,对象候补搜索
#define COMM_CMD_STRING_VACB	("VACB") // 5.1、FACB:自动对准的执行，执行自动对准（连续对准）,对象候补搜索，增加图像十字中心基准对位过程
#define COMM_CMD_STRING_VAHL	("VAHL") // 5.2、FAHL:Align high vs low layer背光贴片机自动对准的执行，执行自动对准（连续对准）
#define COMM_CMD_STRING_VANS	("VANS") // 6、FANS:对准后的偏移量的取得，取得对准后平台坐标系的偏移量
#define COMM_CMD_STRING_VALG	("VALG") // 7、FALG:１次对准移动量的取得，取得１次对准移动量
#define COMM_CMD_STRING_VALP	("VALP") // 7、FALP:１次对准拾取移动量的取得，取得１次对准拾取移动量
#define COMM_CMD_STRING_VALJ	("VALJ") // 8、FALJ:1:次对准精度判定与对准移动量的取得，对准移动量是否在对准精度内的判定与对准移动量的取得
#define COMM_CMD_STRING_VACO	("VACO") // 9、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
#define COMM_CMD_STRING_VAAO    ("VAAO") // 9、FAAO:偏移量的去的，对象目标自动拍照，目标为图像中心
#define COMM_CMD_STRING_VOLC	("VOLC") // 9、FOLC:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
#define COMM_CMD_STRING_VABO	("VABO") // 10、FABO:偏移量的取得，取得目标图像十字中心及物体 Mark 之间中心的平台坐标系偏移量
#define COMM_CMD_STRING_VAOT	("VAOT") // 5.1、FAOT:自动对准的执行，执行自动对准（连续对准）,对象候补搜索,目标候补连续搜索
#define COMM_CMD_STRING_VALX	("VALX") // 5.1、FALX:自动对准的执行，执行自动对准（连续对准）,对象候补连续搜索
#define COMM_CMD_STRING_VWDR	("VWDR") // 5.1、FWDR:获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
#define COMM_CMD_STRING_VGOW	("VGOW") // 6、VGOW:拍照取得对象（对象靶标）的平台坐标上的位置
#define COMM_CMD_STRING_VMCO	("VMCO") // 9、FMCO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
//#define COMM_CMD_STRING_VPSB	("VPSB")
// 
// 检测
#define COMM_CMD_STRING_VCHB	("VCHB") // 2、ACF检测
#define COMM_CMD_STRING_VCHC	("VCHC") // 2、候补ACF检测
#define COMM_CMD_STRING_VDRP	("VDRP") // 2、计算基准拾取位置
#define COMM_CMD_STRING_VQRC	("VQRC")
#define COMM_CMD_STRING_VQRI	("VQRI") //二维码检测指令，指令功能:触发相机拍二维码、利用检测模板检测二维码，获取二维码信息，将二维码信息发送给PLC
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 补偿量・移动
#define COMM_CMD_STRING_VAOF	("VAOF") // 1、FAOF:自动补偿量值的取得，对准偏移量以补偿量值取得
#define COMM_CMD_STRING_VGOF	("VGOF") // 2、FGOF:补偿量值的取得，取得设定的补偿量值
#define COMM_CMD_STRING_VOFS	("VOFS") // 3、FOFS:补偿量值的设定，设定补偿量值
#define COMM_CMD_STRING_VSFT	("VSFT") // 4、FSFT:平台Shift 移动动作，选择内部转移量的设定值，并依照设定值将目前平台的位置做相对移动
#define COMM_CMD_STRING_VGSF	("VGSF") // 5、FGSF:对准Shift 移动量的取得，取得对准转移量
#define COMM_CMD_STRING_VMPS	("VMPS") // 6、MNPS:平台现在位置的取得，取得平台现在位置
#define COMM_CMD_STRING_VMVA	("VMVA") // 7、MMVA:平台絶对准置移动量的设定，设定平台絶对准置移动量
#define COMM_CMD_STRING_VMVR	("VMVR") // 8、MMVR:平台相对准置移动量的设定，设定平台相对准置移动量
#define COMM_CMD_STRING_VGPO	("VGPO") // 9、FGPO:获取连续对位的最终精度

//////////////////////////////////////////////////////////////////////////
// Alarm
#define COMM_CMD_STRING_VALM	("VALM") // 1、FALM:Alarm资料的取得，取得错误发生的数据代码
#define COMM_CMD_STRING_VRST	("VRST") // 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
#define COMM_CMD_STRING_VSAC	("VSAC") // 3、FSAC：保存校正、定位时的数据。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
#define COMM_CMD_STRING_VSCO	("VSCO") // 4、FSCO：保存指令收发的历史纪录。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）

#define COMM_CMD_STRING_VCHK	("VCHK") // ：特殊通讯指令，互相握手后，软件有效

#define COMM_CMD_STRING_VTGM	("VTGM") // 1、FTGM:目标 Mark 位置的手动取得，取得目标 Mark（目标）位置
#define COMM_CMD_STRING_VOBM	("VOBM") // 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
#define COMM_CMD_STRING_VSPE	("VSPE") // 曝光时间设置
#define COMM_CMD_STRING_VWEL	("VWEL") // 专用焊接指令
#define COMM_CMD_STRING_VMPB	("VMPB") // 获取备用轴位置
#define COMM_CMD_STRING_VPCB	("VPCB") // FPCB:对象、目标Mark图像位置
#define COMM_CMD_STRING_VPIS	("VPIS") // PCB 精度检测 PCB Inspect
#define COMM_CMD_STRING_VCPI	("VCPI")
#define COMM_CMD_STRING_VINS	("VINS") // 检测产品有、无
// 暂无
#define	COMM_CMD_STRING_FHMS	("FHMS")
#define COMM_CMD_STRING_FHOM	("FHOM")
#define COMM_CMD_STRING_FPNT	("FPNT")
#define COMM_CMD_STRING_FMAP	("FMAP")
#define COMM_CMD_STRING_FRCT	("FRCT")
#define COMM_CMD_STRING_FISP	("FISP")
#define COMM_CMD_STRING_FOLG	("FOLG")
#define COMM_CMD_STRING_FTLG	("FTLG")
//#define COMM_CMD_STRING_MPSB	("MPSB")
#define COMM_CMD_STRING_FPEP	("FPEP")
#define COMM_CMD_STRING_FPCP	("FPCP")
#define COMM_CMD_STRING_FPRN	("FPRN")

#define	COMM_CMD_STRING_VHMS	("VHMS")
#define COMM_CMD_STRING_VHOM	("VHOM")
#define COMM_CMD_STRING_VPNT	("VPNT")
#define COMM_CMD_STRING_VMAP	("VMAP")
#define COMM_CMD_STRING_VRCT	("VRCT")
#define COMM_CMD_STRING_VISP	("VISP")
#define COMM_CMD_STRING_VOLG	("VOLG")
#define COMM_CMD_STRING_VTLG	("VTLG")
#define COMM_CMD_STRING_VCCI	("VCCI") //复制标定信息
#define COMM_CMD_STRING_VPEP	("VPEP")
#define COMM_CMD_STRING_VPCP	("VPCP")
#define COMM_CMD_STRING_VPRN	("VPRN")

//#define COMM_CMD_STRING_VISION
#ifdef COMM_CMD_STRING_VISION
{
	//////////////////////////////////////////////////////////////////////////
	//lzk 0408
	#define	COMM_CMD_STRING_VISIONFVPC	COMM_CMD_STRING_FVPC
	#define	COMM_CMD_STRING_VISIONFPMI	COMM_CMD_STRING_FPMI
	#define	COMM_CMD_STRING_VISIONFBIS	COMM_CMD_STRING_FBIS
	// 产品
	#define COMM_CMD_STRING_VISIONFSPC	COMM_CMD_STRING_VSPC
	#define COMM_CMD_STRING_VISIONFSSV	COMM_CMD_STRING_VSSV
	#define COMM_CMD_STRING_VISIONFGSP	COMM_CMD_STRING_VGSP

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// Mark：标定					 标定
	#define COMM_CMD_STRING_VISIONFCLB	COMM_CMD_STRING_VCLB
	#define COMM_CMD_STRING_VISIONFCBS	COMM_CMD_STRING_VCBS
	#define COMM_CMD_STRING_VISIONFCBR	COMM_CMD_STRING_VCBR
	#define COMM_CMD_STRING_VISIONFCPR	COMM_CMD_STRING_VCPR
	#define COMM_CMD_STRING_VISIONFCBP	COMM_CMD_STRING_VCBP
	////////////////////////////////////////////////////
	// Mark:目标Mark				  目标Mark
	#define COMM_CMD_STRING_VISIONFTGT	COMM_CMD_STRING_VTGT
	#define COMM_CMD_STRING_VISIONFTGS	COMM_CMD_STRING_VTGS
	#define COMM_CMD_STRING_VISIONFTGC	COMM_CMD_STRING_VTGC
	#define COMM_CMD_STRING_VISIONFTGR	COMM_CMD_STRING_VTGR
	#define COMM_CMD_STRING_VISIONFTGG	COMM_CMD_STRING_VTGG
	#define COMM_CMD_STRING_VISIONFTGD	COMM_CMD_STRING_VTGD
	#define COMM_CMD_STRING_VISIONFGAP	COMM_CMD_STRING_VGAP
	#define COMM_CMD_STRING_VISIONFCNT	COMM_CMD_STRING_VCNT
	#define COMM_CMD_STRING_VISIONFGTC	COMM_CMD_STRING_VGTC

	#define COMM_CMD_STRING_VISIONFCNO	COMM_CMD_STRING_VCNO
	#define COMM_CMD_STRING_VISIONFGTW  COMM_CMD_STRING_VGTW
	#define COMM_CMD_STRING_VISIONFAFO  COMM_CMD_STRING_VAFO
	#define COMM_CMD_STRING_VISIONFGTP  COMM_CMD_STRING_VGTP
	////////////////////////////////////////////////////
	// Mark:物体Mark				  物体Mark
	#define COMM_CMD_STRING_VISIONFOBJ	COMM_CMD_STRING_VOBJ
	#define COMM_CMD_STRING_VISIONFOBC	COMM_CMD_STRING_VOBC
	#define COMM_CMD_STRING_VISIONFOBG	COMM_CMD_STRING_VOBG
	#define COMM_CMD_STRING_VISIONFMSC	COMM_CMD_STRING_VMSC
	#define COMM_CMD_STRING_VISIONFWLD	COMM_CMD_STRING_VWLD

	////////////////////////////////////////////////////
	// 对准
	#define COMM_CMD_STRING_VISIONFGAL	COMM_CMD_STRING_VGAL
	#define COMM_CMD_STRING_VISIONFALL	COMM_CMD_STRING_VALL
	#define COMM_CMD_STRING_VISIONFGAC	COMM_CMD_STRING_VGAC
	#define COMM_CMD_STRING_VISIONFALC	COMM_CMD_STRING_VALC
	#define COMM_CMD_STRING_VISIONFAAL	COMM_CMD_STRING_VAAL
	#define COMM_CMD_STRING_VISIONFXYA	COMM_CMD_STRING_VXYA
	#define COMM_CMD_STRING_VISIONFAAB	COMM_CMD_STRING_VAAB
	#define COMM_CMD_STRING_VISIONFACB	COMM_CMD_STRING_VACB
	#define COMM_CMD_STRING_VISIONFAAC	COMM_CMD_STRING_VAAC
	#define COMM_CMD_STRING_VISIONFAHL	COMM_CMD_STRING_VAHL
	#define COMM_CMD_STRING_VISIONFANS	COMM_CMD_STRING_VANS
	#define COMM_CMD_STRING_VISIONFALG	COMM_CMD_STRING_VALG
	#define COMM_CMD_STRING_VISIONFALP	COMM_CMD_STRING_VALP
	#define COMM_CMD_STRING_VISIONFALJ	COMM_CMD_STRING_VALJ
	#define COMM_CMD_STRING_VISIONFACO	COMM_CMD_STRING_VACO
    #define COMM_CMD_STRING_VISIONFAAO  COMM_CMD_STRING_VAAO
	#define COMM_CMD_STRING_VISIONFOLC	COMM_CMD_STRING_VOLC
	#define COMM_CMD_STRING_VISIONFABO	COMM_CMD_STRING_VABO
	#define COMM_CMD_STRING_VISIONFAOT	COMM_CMD_STRING_VAOT
	#define COMM_CMD_STRING_VISIONFALX	COMM_CMD_STRING_VALX
	#define COMM_CMD_STRING_VISIONFWDR COMM_CMD_STRING_VWDR
	#define COMM_CMD_STRING_VISIONFGOW COMM_CMD_STRING_VGOW
	#define COMM_CMD_STRING_VISIONFMCO COMM_CMD_STRING_VMCO
	// 检测
	#define COMM_CMD_STRING_VISIONFCHB	COMM_CMD_STRING_VCHB
	#define COMM_CMD_STRING_VISIONFCHC	COMM_CMD_STRING_VCHC
	#define COMM_CMD_STRING_VISIONFDRP	COMM_CMD_STRING_VDRP
	#define COMM_CMD_STRING_VISIONFQRC  COMM_CMD_STRING_VQRC
	#define COMM_CMD_STRING_VISIONFQRI  COMM_CMD_STRING_VQRI
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// 补偿量・移动				  ・移动
	#define COMM_CMD_STRING_VISIONFAOF	COMM_CMD_STRING_VAOF
	#define COMM_CMD_STRING_VISIONFGOF	COMM_CMD_STRING_VGOF
	#define COMM_CMD_STRING_VISIONFOFS	COMM_CMD_STRING_VOFS
	#define COMM_CMD_STRING_VISIONFSFT	COMM_CMD_STRING_VSFT
	#define COMM_CMD_STRING_VISIONFGSF	COMM_CMD_STRING_VGSF
	#define COMM_CMD_STRING_VISIONFMPS	COMM_CMD_STRING_VMPS
	#define COMM_CMD_STRING_VISIONFMVA	COMM_CMD_STRING_VMVA
	#define COMM_CMD_STRING_VISIONFMVR	COMM_CMD_STRING_VMVR
	#define COMM_CMD_STRING_VISIONFGPO	COMM_CMD_STRING_VGPO

	////////////////////////////////////////////////////
	// Alarm
	#define COMM_CMD_STRING_VISIONFALM	COMM_CMD_STRING_VALM
	#define COMM_CMD_STRING_VISIONFRST	COMM_CMD_STRING_VRST
	#define COMM_CMD_STRING_VISIONFSAC	COMM_CMD_STRING_VSAC
	#define COMM_CMD_STRING_VISIONFSCO	COMM_CMD_STRING_VSCO
    ////////////////////////////////////////////////////
	// Special comm
    #define COMM_CMD_STRING_VISIONFCHK	COMM_CMD_STRING_VCHK

	#define COMM_CMD_STRING_VISIONFTGM	COMM_CMD_STRING_VTGM
	#define COMM_CMD_STRING_VISIONFOBM	COMM_CMD_STRING_VOBM
	#define COMM_CMD_STRING_VISIONFSPE	COMM_CMD_STRING_VSPE
	#define COMM_CMD_STRING_VISIONFWEL	COMM_CMD_STRING_VWEL
	#define COMM_CMD_STRING_VISIONFMPB	COMM_CMD_STRING_VMPB
	#define COMM_CMD_STRING_VISIONFPCB	COMM_CMD_STRING_VPCB
	#define COMM_CMD_STRING_VISIONFPIS	COMM_CMD_STRING_VPIS
	#define COMM_CMD_STRING_VISIONFCPI	COMM_CMD_STRING_VCPI
	#define COMM_CMD_STRING_VISIONFINS	COMM_CMD_STRING_VINS
	#define COMM_CMD_STRING_VISIONFCCI	COMM_CMD_STRING_VCCI

	#define COMM_CMD_STRING_VISIONFPEP COMM_CMD_STRING_VPEP
	#define COMM_CMD_STRING_VISIONFPCP COMM_CMD_STRING_VPCP
	#define COMM_CMD_STRING_VISIONFPRN COMM_CMD_STRING_VPRN

}
#else
//{
	//////////////////////////////////////////////////////////////////////////
	//lzk 0408
	#define	COMM_CMD_STRING_VISIONFVPC	COMM_CMD_STRING_FVPC
	#define	COMM_CMD_STRING_VISIONFPMI	COMM_CMD_STRING_FPMI
	#define	COMM_CMD_STRING_VISIONFBIS	COMM_CMD_STRING_FBIS
	// 产品
	#define COMM_CMD_STRING_VISIONFSPC	COMM_CMD_STRING_FSPC
	#define COMM_CMD_STRING_VISIONFSSV	COMM_CMD_STRING_FSSV
	#define COMM_CMD_STRING_VISIONFGSP	COMM_CMD_STRING_FGSP
    #define COMM_CMD_STRING_VISIONFVAS	COMM_CMD_STRING_FVAS

		////////////////////////////////////////////////////
		////////////////////////////////////////////////////
		// Mark：标定					 标定
	#define COMM_CMD_STRING_VISIONFCLB	COMM_CMD_STRING_FCLB
    #define COMM_CMD_STRING_VISIONFCBS	COMM_CMD_STRING_FCBS
	#define COMM_CMD_STRING_VISIONFCBR	COMM_CMD_STRING_FCBR

	#define COMM_CMD_STRING_VISIONFCPR	COMM_CMD_STRING_FCPR
	#define COMM_CMD_STRING_VISIONFCBP	COMM_CMD_STRING_FCBP
		////////////////////////////////////////////////////
		// Mark:目标Mark				  目标Mark
	#define COMM_CMD_STRING_VISIONFTGT	COMM_CMD_STRING_FTGT
	#define COMM_CMD_STRING_VISIONFTGS	COMM_CMD_STRING_FTGS
	#define COMM_CMD_STRING_VISIONFTGC	COMM_CMD_STRING_FTGC
	#define COMM_CMD_STRING_VISIONFTGR	COMM_CMD_STRING_FTGR
	#define COMM_CMD_STRING_VISIONFTGG	COMM_CMD_STRING_FTGG
	#define COMM_CMD_STRING_VISIONFTGD	COMM_CMD_STRING_FTGD
	#define COMM_CMD_STRING_VISIONFGAP	COMM_CMD_STRING_FGAP
	#define COMM_CMD_STRING_VISIONFCNT	COMM_CMD_STRING_FCNT
	#define COMM_CMD_STRING_VISIONFGTC	COMM_CMD_STRING_FGTC

	#define COMM_CMD_STRING_VISIONFCNO	COMM_CMD_STRING_FCNO
	#define COMM_CMD_STRING_VISIONFGTW  COMM_CMD_STRING_FGTW
	#define COMM_CMD_STRING_VISIONFAFO  COMM_CMD_STRING_FAFO
    #define COMM_CMD_STRING_VISIONFGTP  COMM_CMD_STRING_FGTP
		////////////////////////////////////////////////////
		// Mark:物体Mark				  物体Mark
	#define COMM_CMD_STRING_VISIONFOBJ	COMM_CMD_STRING_FOBJ
	#define COMM_CMD_STRING_VISIONFOBC	COMM_CMD_STRING_FOBC
	#define COMM_CMD_STRING_VISIONFOBG	COMM_CMD_STRING_FOBG
	#define COMM_CMD_STRING_VISIONFMSC	COMM_CMD_STRING_FMSC
	#define COMM_CMD_STRING_VISIONFWLD	COMM_CMD_STRING_FWLD


		////////////////////////////////////////////////////
		// 对准
	#define COMM_CMD_STRING_VISIONFGAL	COMM_CMD_STRING_FGAL
	#define COMM_CMD_STRING_VISIONFALL	COMM_CMD_STRING_FALL
	#define COMM_CMD_STRING_VISIONFGAC	COMM_CMD_STRING_FGAC
	#define COMM_CMD_STRING_VISIONFALC	COMM_CMD_STRING_FALC
	#define COMM_CMD_STRING_VISIONFAAL	COMM_CMD_STRING_FAAL
    #define COMM_CMD_STRING_VISIONFXYA	COMM_CMD_STRING_FXYA
	#define COMM_CMD_STRING_VISIONFAAB	COMM_CMD_STRING_FAAB
    #define COMM_CMD_STRING_VISIONFACB	COMM_CMD_STRING_FACB
	#define COMM_CMD_STRING_VISIONFAAC	COMM_CMD_STRING_FAAC
	#define COMM_CMD_STRING_VISIONFAHL	COMM_CMD_STRING_FAHL
	#define COMM_CMD_STRING_VISIONFANS	COMM_CMD_STRING_FANS
	#define COMM_CMD_STRING_VISIONFALG	COMM_CMD_STRING_FALG
	#define COMM_CMD_STRING_VISIONFALP	COMM_CMD_STRING_FALP
	#define COMM_CMD_STRING_VISIONFALJ	COMM_CMD_STRING_FALJ
	#define COMM_CMD_STRING_VISIONFACO	COMM_CMD_STRING_FACO
    #define COMM_CMD_STRING_VISIONFAAO  COMM_CMD_STRING_FAAO
	#define COMM_CMD_STRING_VISIONFOLC	COMM_CMD_STRING_FOLC
	#define COMM_CMD_STRING_VISIONFABO	COMM_CMD_STRING_FABO
    #define COMM_CMD_STRING_VISIONFAOT	COMM_CMD_STRING_FAOT
    #define COMM_CMD_STRING_VISIONFALX	COMM_CMD_STRING_FALX
	#define COMM_CMD_STRING_VISIONFWDR	COMM_CMD_STRING_FWDR
	#define COMM_CMD_STRING_VISIONFGOW	COMM_CMD_STRING_FGOW
	#define COMM_CMD_STRING_VISIONFMCO	COMM_CMD_STRING_FMCO
		// 检测
    #define COMM_CMD_STRING_VISIONFCHB	COMM_CMD_STRING_FCHB
	#define COMM_CMD_STRING_VISIONFCHC	COMM_CMD_STRING_FCHC
	#define COMM_CMD_STRING_VISIONFDRP	COMM_CMD_STRING_FDRP
	#define COMM_CMD_STRING_VISIONFQRC  COMM_CMD_STRING_FQRC
	#define COMM_CMD_STRING_VISIONFQRI  COMM_CMD_STRING_FQRI
		// 
		////////////////////////////////////////////////////
		// 补偿量・移动				  ・移动
	#define COMM_CMD_STRING_VISIONFAOF	COMM_CMD_STRING_FAOF
	#define COMM_CMD_STRING_VISIONFGOF	COMM_CMD_STRING_FGOF
	#define COMM_CMD_STRING_VISIONFOFS	COMM_CMD_STRING_FOFS
	#define COMM_CMD_STRING_VISIONFSFT	COMM_CMD_STRING_FSFT
	#define COMM_CMD_STRING_VISIONFGSF	COMM_CMD_STRING_FGSF
	#define COMM_CMD_STRING_VISIONFMPS	COMM_CMD_STRING_FMPS
	#define COMM_CMD_STRING_VISIONFMVA	COMM_CMD_STRING_FMVA
	#define COMM_CMD_STRING_VISIONFMVR	COMM_CMD_STRING_FMVR
	#define COMM_CMD_STRING_VISIONFGPO	COMM_CMD_STRING_FGPO

		////////////////////////////////////////////////////
		// Alarm
	#define COMM_CMD_STRING_VISIONFALM	COMM_CMD_STRING_FALM
	#define COMM_CMD_STRING_VISIONFRST	COMM_CMD_STRING_FRST
	#define COMM_CMD_STRING_VISIONFSAC	COMM_CMD_STRING_FSAC
    #define COMM_CMD_STRING_VISIONFSCO	COMM_CMD_STRING_FSCO
    ////////////////////////////////////////////////////
    // Special comm
    #define  COMM_CMD_STRING_VISIONFCHK   COMM_CMD_STRING_FCHK

	#define COMM_CMD_STRING_VISIONFTGM	COMM_CMD_STRING_FTGM
	#define COMM_CMD_STRING_VISIONFOBM	COMM_CMD_STRING_FOBM

	#define COMM_CMD_STRING_VISIONFSPE	COMM_CMD_STRING_FSPE
	#define COMM_CMD_STRING_VISIONFWEL	COMM_CMD_STRING_FWEL
	#define COMM_CMD_STRING_VISIONFSOL	COMM_CMD_STRING_FSOL
	#define COMM_CMD_STRING_VISIONFMPB	COMM_CMD_STRING_FMPB
	#define COMM_CMD_STRING_VISIONFPCB	COMM_CMD_STRING_FPCB
	#define COMM_CMD_STRING_VISIONFPIS	COMM_CMD_STRING_FPIS
	#define COMM_CMD_STRING_VISIONFCPI	COMM_CMD_STRING_FCPI
	#define COMM_CMD_STRING_VISIONFINS  COMM_CMD_STRING_FINS

	// FAST读写寄存器
	//////////////////////////////////////////////////////////////////////////
	#define COMM_CMD_STRING_VISIONFHMS COMM_CMD_STRING_FHMS// 	2	FHMS
	#define COMM_CMD_STRING_VISIONFHOM COMM_CMD_STRING_FHOM// 	3	FHOM
	#define COMM_CMD_STRING_VISIONFPNT COMM_CMD_STRING_FPNT// 	12	FPNT
	#define COMM_CMD_STRING_VISIONFMAP COMM_CMD_STRING_FMAP// 	22	FMAP
	#define COMM_CMD_STRING_VISIONFRCT COMM_CMD_STRING_FRCT// 	24	FRCT
	#define COMM_CMD_STRING_VISIONFISP COMM_CMD_STRING_FISP// 	25	FISP
	#define COMM_CMD_STRING_VISIONFOLG COMM_CMD_STRING_FOLG// 	26	FOLG
	#define COMM_CMD_STRING_VISIONFTLG COMM_CMD_STRING_FTLG// 	27	FTLG
	#define	COMM_CMD_STRING_VISIONFCCI COMM_CMD_STRING_FCCI//	200 FCCI
	#define COMM_CMD_STRING_VISIONFPEP COMM_CMD_STRING_FPEP
	#define COMM_CMD_STRING_VISIONFPCP COMM_CMD_STRING_FPCP
	#define COMM_CMD_STRING_VISIONFPRN COMM_CMD_STRING_FPRN
//	#define COMM_CMD_STRING_VISIONMPSB COMM_CMD_STRING_MPSB //

// 	#define COMM_CMD_STRING_VISIONVFRST	COMM_CMD_STRING_FRST// 1	FRST
// 	#define COMM_CMD_STRING_VISIONVFHMS COMM_CMD_STRING_FHMS// 	2	FHMS
// 	#define COMM_CMD_STRING_VISIONVFHOM COMM_CMD_STRING_FHOM// 	3	FHOM
// 	#define COMM_CMD_STRING_VISIONVFSPC	COMM_CMD_STRING_FSPC// 	4	FSPC
// 	#define COMM_CMD_STRING_VISIONVFCLB COMM_CMD_STRING_FCLB// 	5	FCLB
// 	#define COMM_CMD_STRING_VISIONVFTGT COMM_CMD_STRING_FTGT// 	6	FTGT
// 	#define COMM_CMD_STRING_VISIONVFOBJ COMM_CMD_STRING_FOBJ// 	7	FOBJ
// 	#define COMM_CMD_STRING_VISIONVFAAL COMM_CMD_STRING_FAAL// 	8	FAAL
// 	#define COMM_CMD_STRING_VISIONVFNPS COMM_CMD_STRING_FMPS// 	9	FNPS
// 	#define COMM_CMD_STRING_VISIONVFMVA COMM_CMD_STRING_FMVA// 	10	FMVA
// 	#define COMM_CMD_STRING_VISIONVFMVR COMM_CMD_STRING_FMVR// 	11	FMVR
// 	#define COMM_CMD_STRING_VISIONVFPNT COMM_CMD_STRING_FPNT// 	12	FPNT
// 	#define COMM_CMD_STRING_VISIONVFACO COMM_CMD_STRING_FACO// 	13	FACO
// 	#define COMM_CMD_STRING_VISIONVFALG COMM_CMD_STRING_FALG// 	14	FALG
// 	#define COMM_CMD_STRING_VISIONVFCNT COMM_CMD_STRING_FCNT// 	15	FCNT
// 	#define COMM_CMD_STRING_VISIONVFGAP COMM_CMD_STRING_FGAP// 	16	FGAP
// 	#define COMM_CMD_STRING_VISIONVFTGC COMM_CMD_STRING_FTGC// 	17	FTGC
// 	#define COMM_CMD_STRING_VISIONVFOBC COMM_CMD_STRING_FOBC// 	18	FOBC
// 	#define COMM_CMD_STRING_VISIONVFAAC COMM_CMD_STRING_FAAC// 	19	FAAC
// 	#define COMM_CMD_STRING_VISIONVFGSP COMM_CMD_STRING_FGSP// 	20	FGSP
// 	#define COMM_CMD_STRING_VISIONVFCBS COMM_CMD_STRING_FCBS// 	21	FCBS
// 	#define COMM_CMD_STRING_VISIONVFMAP COMM_CMD_STRING_FMAP// 	22	FMAP
// 	#define COMM_CMD_STRING_VISIONVFXYA COMM_CMD_STRING_FXYA// 	23	FXYA
//  #define COMM_CMD_STRING_VISIONVFRCT COMM_CMD_STRING_FRCT// 	24	FRCT
//  #define COMM_CMD_STRING_VISIONVFISP COMM_CMD_STRING_FISP// 	25	FISP
//  #define COMM_CMD_STRING_VISIONVFOLG COMM_CMD_STRING_FOLG// 	26	FOLG
//  #define COMM_CMD_STRING_VISIONVFTLG COMM_CMD_STRING_FTLG// 	27	FTLG

	
//}

#endif

// 接收到的系统工作指令
typedef enum enumSysWorkCmdType
{
   eCmdUnknow  = -1,         // 未知类型
   eCmdCalibrate   ,        // 执行系统标定:CAE
   eCmdOnlineAlign ,       // 执行在线对位:AAE
   eCmdOnlineAlignFACO ,   // 执行在线对位:FACO 对位一次,返回相对位置偏移值 
   eCmdCalibSearch,        // 执行标定搜索:CAG  
   eCmdTargetSearch,       // 执行实时目标搜索: TGG
   eCmdObjectSearch,       // 执行实时对象搜索: OBG
   eCmdVirtualSearch,      // 执行虚拟对象搜索: VBG
   eCmdCntObAndVb,         // 执行执行获取实时对象与虚拟对象之间的关系: COV
   eCmdSetProductName,     // 执行设置当前产品名称
   eCmdSetProductIndex,     // 执行设置当前产品索引
   eCmdGetAlignResult,      // 获取对位结果
   eCmdSetTargetBench,      // 设置基准目标位置

   eCmdSetProductIndexFSPC,	// 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
   eCmdSaveProductIndexFSSV,// 2、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
   eCmdGetProductIndexFGSP,// 3、 FGSP：产品No.的取得，取得目前正在执行的产品No.
   eCmdSetProductVirtualAlnFVAS,	// 1、 FVAS：设置产品虚拟Mark开关.
 
   eCmdCalibrateFCLB,		  // 1、FCLB:Calibration 的执行 执行Calibration
   eCmdCalibrateFCBS,         // 1、FCBS:分步Calibration 的执行 执行Calibration
   eCmdCalibrateFCBR,         // 1、FCBR:关联Calibration 的执行 执行Calibration
   eCmdCalibrateFCPR,		  // FCPR:相机轴标定
   
   eCmdTargetSearchFTGT,      // 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
   eCmdTargetSearchFTGS,      // 1、FTGS:目标 Mark 位置的取得，取得目标 Mark（目标）位置,目标位置一致
   eCmdTargetSearchFTGC,      // 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
   eCmdSaveTargetPosFTGR,	  // 3、FTGR:目标 Mark 位置的登录，将目前使用的产品数据保存在指定的产品No.中并保存在CF 记忆卡
   eCmdGetTargetImagePosFTGG, // 4、FTGG:目标 Mark 画素的取得，取得执行“ＦＴＧＴ”结果的画素
   eCmdDisplayTargetFTGD,	  // 5、FTGD:目标 Mark 指示线的全画面显示，在指定目标 Mark No.的目标 Mark 位置上显示全画面的指示线及刻度
   eCmdObjectSearchFOBJ,      // 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
   eCmdObjectSearchFOBC,      // 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
   eCmdGetObjectImagePosFOBG, // 3、FOBG:物体 Mark 画素的取得，取得执行“ＦＯＢＪ”结果的画素
   eCmdManualSearchFMSC,	  // 4、FMSC:手动搜索位置的取得，在自动对准时，当目标 Mark 或物体 Mark 搜索失败时，可利用手动方法（鼠标）指定Mark 位置
   eCmdGetSearchWorldPosFWLD, // 5、FWLD：取得目标（目标靶标）或对象（对象靶标）的平台坐标上的位置
   eCmdGetImagePosFGAP,       // 4、FGAP: Mark 画素的取得，取得执行结果的画素   
   eCmdObjectSearchFCNT,       // 4、FCNT: 对象定位同时将目标位置设为图像中心，参数类似于FOBJ P1      
   eCmdGetImagePosFGTC,			//5、FGTC:获取目标相对于图像中心的图像坐标偏差

   eCmdTargetSearchFCNO, 

   eCmdGetAlignPrecisionFGAL,	// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
   eCmdSetAlignPrecisionFALL,	// 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
   eCmdGetAlignTimeFGAC,		// 3、FGAC:对准最多次数的取得，取得设定对准最多次数
   eCmdSetAlignTimeFALC,		// 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
   eCmdAutoAlignFAAL,			// 5、FAAL:自动对准的执行，执行自动对准（连续对准）
   eCmdAutoAlignFXYA,			// 5、XY+D 平台FXYA:自动对准的执行，执行自动对准（连续对准）
   eCmdAutoAlignFAAB,			// 5、FAAB:自动对准的执行，执行自动对准（连续对准），增加内部图像十字中心基准对位过程
   eCmdAutoAlignFAAC,			// 5.1、FAAL:自动对准的执行，执行自动对准（连续对准），对象候补搜索
   eCmdAutoAlignFACB,			// 5.1、FACB:自动对准的执行，执行自动对准（连续对准），对象候补搜索，增加内部图像十字中心基准对位过程
   eCmdAutoAlignFAHL,			// 5.2、FAHL:Align high vs low layer背光贴片机自动对准的执行，执行自动对准（连续对准）
   eCmdGetAutoAlignOffsetFANS,  // 6、FANS:对准后的偏移量的取得，取得对准后平台坐标系的偏移量
   eCmdOnceAlignMoveFALG,		// 7、FALG:１次对准移动量的取得，取得１次对准移动量
   eCmdOnceAlignMovePickFALP,		// 7、FALP:１次对准拾取移动量的取得，取得１次对准拾取移动量
   eCmdOnceAlignFALJ,			 // 8、FALJ:1:次对准精度判定与对准移动量的取得，对准移动量是否在对准精度内的判定与对准移动量的取得
   eCmdGetTargetObjectOffsetFACO,// 9、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
   eCmdGetTargetObjectOffsetFOLC,// 9、FOLC:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
   eCmdGetTargetObjectOffsetFABO,// 9、FABO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
   eCmdCalcuAlignOffsetFAOF,	// 1、FAOF:自动补偿量值的取得，对准偏移量以补偿量值取得
   eCmdGetAlignOffsetFGOF,		// 2、FGOF:补偿量值的取得，取得设定的补偿量值
   eCmdSetAlignOffsetFOFS,		// 3、FOFS:补偿量值的设定，设定补偿量值
   eCmdFSFT,					// 4、FSFT:平台Shift 移动动作，选择内部转移量的设定值，并依照设定值将目前平台的位置做相对移动
   eCmdFGSF,					// 5、FGSF:对准Shift 移动量的取得，取得对准转移量
   eCmdFGPO,					// 9、FGPO:获取多次对位最终精度
   eCmdGetAlarmFALM,			// 1、FALM:Alarm资料的取得，取得错误发生的数据代码
   eCmdResetFRST,				// 2、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
   eCmdSaveProcessDataFSAC,		// 3、FSAC：保存校正、定位时的数据。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
   eCmdSaveCommDataFSCO,		// 4、FSCO：保存指令收发的历史纪录。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
   eCmdGetPosMNPS,				// 1、获取当前轴位置
   eCmdMovePosMMVA,				// 2、移动绝对位置.
   eCmdMoveRelPosMMVR,			// 2、移动相对位置.
   eCmdSpecialCommFCHK,         // 特殊通讯指令，收取FCHK 10，发送FCHK CY 程序有效
   eCmdTargetSearchFTGM,			// 1、FTGM:目标 Mark 位置的取得，手动取得目标 Mark（目标）位置
   eCmdObjectSearchFOBM,			// 1、FOBM:物体 Mark 位置的取得，手动取得物体 Mark（对准靶标）位置
   eCmdAutoAlignFAOT,			// 5.1、FAAL:自动对准的执行，执行自动对准（连续对准），对象候补搜索,目标候补搜索
   eCmdAutoAlignFALX,			// 5.1、FALX:自动对准的执行，执行自动对准（连续对准），对象候补搜索
   eCmdFWDR,					 // 5.1、FWDR:获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
   eCmdGetObjectWorldPosFGOW, // 6、FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
   eCmdACFInspectFCHB,          // FCHB:ACF检测
   eCmdACFInspectFCHC,          // FCHC:ACF检测
   eCmdFDRP,					// FDRP:计算基准拾取位置
   eCmdGetObjectTargetOffsetFAAO, //FAAO，自动目标对象拍照，目标Mark为图像中心，返回偏移量
   //eCmdMPSB,
   eCmdGetPosBackupMPSB,			// MPSB:获取当前轴位置（备用）
   eCmdSetProductExposureFSPE,		// FSPE:设置产品曝光
   eCmdAutoAlignFWEL,				// FWEL：专用焊接指令,小FPC:XY,大FPC:XYD
   eCmdAutoAlignFSOL,               // FSOL: 专用焊接指令,小FPC:XYD,大FPC:XY
   eCmdObjTarSearchFPCB,			// 对象、目标图像定位
   eCmdPCBPrecisionFPIS,			// PCB 精度检测
   eCmdChangePlatformIndex,			// 更改平台序号
   eCmdSaveQRCode,			        //存储二维码
   eCmdInspectQRCode,			        //检测二维码
   eCmdInspectFINS,					// 检测产品有、无
   eCmdProductEmptyPEP,            //产品清空
   eCmdProductCopyFPCP,             //产品拷贝
   eCmdProductRenameFPRN,           //产品重命名
   eCmdCalibrateFCBP,               //自动关联
   eCmdGetTargetPlatformPosFGTW,    //视觉进行目标拍照并发送目标平台坐标和轴位置给PLC
   eCmdFAFO,                        //目标采用靶标关联时，计算目标坐标系到对象坐标系的固定补偿
   eCmdGetTargetImagePosFGTP,       //视觉进行目标拍照并发送目标图像坐标和轴位置给PLC
   eCmdGetTargetObjectOffsetFMCO ,  //9、FMCO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
   eCmdCopyCalibrateInfoFCCI,       //lzk 复制标定信息
   eCmdFVPC,						//切换虚拟位置模式
   eCmdObjectSearchFPMI,						//开启系统设置权限
   eCmdACFInspectFBIS,          // FBIS:分步检测
}SysWorkCmdType;


// Vision接收到的PLC寄存器系统工作指令值(是唯一的)
// 注：Vision反馈给PLC响应时同样使用该工作指令值。即PLC寄存器指令值与Vision响应指令值一般是一样的。
typedef enum enumSysWorkCmdValue
{
	eCmdValUnknow				 = 0,		// 未知类型
	eCmdValResetFRST			 = 1,		// 1、FRST:重新设定（清除错误等），清除各种错误及显示重新设定
	eCmdValGetPosMNPS			 = 2,		// 2、获取当前轴位置
	eCmdValMovePosMMVA			 = 3,		// 3、移动绝对位置.
	eCmdValMoveRelPosMMVR		 = 4,		// 4、移动相对位置

	eCmdValSetProductIndexFSPC	 = 5,		// 1、 FSPC：产品切换的设定，将产品切换至指定的产品No.
	eCmdValGetProductIndexFGSP	 = 6,		// 2、 FGSP：产品No.的取得，取得目前正在执行的产品No.
	eCmdValSaveProductIndexFSSV	 = 7,		// 3、 FSSV：指定产品No.的保存，将目前使用的产品数据保存在指定的产品No.并保存在CF 记忆卡中
   
    eCmdValFHMS                  = 8,
    eCmdValFHOM                  = 9,

	eCmdValCalibrateFCLB		 = 10,		// 1、FCLB:Calibration 的执行 执行Calibration
	eCmdValCalibrateFCBS		 = 11,      // 2、FCBS:分步Calibration 的执行 执行Calibration
	eCmdValCalibrateFCBR		 = 12,      // 3、FCBR:关联Calibration 的执行 执行Calibration
    eCmdValCalibrateFCPR         = 13,
	
    eCmdValGetAlignPrecisionFGAL = 14,		// 1、FGAL:对准精度判定设定的取得，取得对准精度的判定设定
    eCmdValSetAlignPrecisionFALL = 15,		// 2、FALL:对准精度判定的更新，更新对准时的对准精度的判定
    eCmdValGetAlignTimeFGAC		 = 16,		// 3、FGAC:对准最多次数的取得，取得设定对准最多次数
    eCmdValSetAlignTimeFALC		 = 17,		// 4、FALC:对准最多次数的更新，执行自动对准时，容许最多对准次数的更新
    eCmdValGetAlignOffsetFGOF    = 18,		// 5、FGOF:补偿量值的取得，取得设定的补偿量值
    eCmdValSetAlignOffsetFOFS    = 19,		// 6、FOFS:补偿量值的设定，设定补偿量值	

    eCmdValTargetSearchFTGT		 = 20,		 // 1、FTGT:目标 Mark 位置的取得，取得目标 Mark（目标）位置
	eCmdValTargetSearchFTGC      = 21,		 // 2、FTGC:目标 Mark 位置的取得（候补搜索），利用候补搜索取得目标 Mark（目标）位置
	eCmdValTargetSearchFTGS		 = 22,		 // 3、FTGS:目标 Mark 位置的取得，取得目标 Mark（目标）位置,目标位置一致
	eCmdValGetTargetImagePosFTGG = 23,		 // 4、FTGG:目标 Mark 画素的取得，取得执行“ＦＴＧＴ”结果的画素
	eCmdValGetImagePosFGAP       = 24,		 // 5、FGAP: Mark 画素的取得，取得执行“FGAP”结果的画素
	
	eCmdValObjectSearchFOBJ		 = 25,		// 1、FOBJ:物体 Mark 位置的取得，取得物体 Mark（对准靶标）位置
	eCmdValObjectSearchFOBC		 = 26,		// 2、FOBC:物体 Mark 位置的取得（候补搜索），利用候补搜索取得物体 Mark（对准靶标）位置
	eCmdValTargetSearchFOBS		 = 27,		// 3、FOBS:物体 Mark 位置的取得，取得物体 Mark（物体）位置,物体位置一致 (无该指令)
	eCmdValGetObjectImagePosFOBG = 28,		// 4、FOBG:物体 Mark 画素的取得，取得执行“ＦＯＢＪ”结果的画素


	eCmdValGetTargetObjectOffsetFOLC = 29,  // 1、FOLC:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量	
	eCmdValGetTargetObjectOffsetFACO = 30,  // 1、FACO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
	eCmdValOnceAlignMoveFALG	 = 31,		// 2、FALG:１次对准移动量的取得，取得１次对准移动量
	eCmdValAutoAlignFAAL		 = 32,		// 3、FAAL:自动对准的执行，执行自动对准（连续对准）
	eCmdValAutoAlignFAAC		 = 33,		// 4、FAAL:自动对准的执行，执行自动对准（连续对准），对象候补搜索
	eCmdValAutoAlignFAHL		 = 34,		// 5、FAHL:Align high vs low layer背光贴片机自动对准的执行，执行自动对准（连续对准）
	eCmdValAutoAlignFAAB		 = 35,		// 6、FAAB:自动对准的执行，执行自动对准（连续对准），增加内部图像十字中心基准对位过程
	eCmdValGetTargetObjectOffsetFABO = 36,  // 7、FABO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
	eCmdValAutoAlignFXYA		 = 37,		// 8、XY+D 自动对准的执行，执行自动对准（连续对准）
	eCmdValOnceAlignMovePickFALP = 38,		// 9、FALP:１次对准拾取移动量的取得，取得１次对准拾取移动量
	eCmdValAutoAlignFACB		 = 39,		// 10、FACB:自动对准的执行，执行自动对准（连续对准），对象候补搜索,增加内部图像十字中心基准对位过程
	eCmdValAutoAlignFAOT		 = 40,		// 5.1、FAOT:自动对准的执行，执行自动对准（连续对准），对象候补搜索,目标候补搜索
	eCmdValAutoAlignFALX		 = 41,		// 5.1、FALX:自动对准的执行，执行自动对准（连续对准），对象候补搜索

    eCmdValGetAutoAlignOffsetFANS = 42,		// 6、FANS:对准后的偏移量的取得，取得对准后平台坐标系的偏移量
    eCmdValOnceAlignFALJ		  = 43,		// 8、FALJ:1:次对准精度判定与对准移动量的取得，对准移动量是否在对准精度内的判定与对准移动量的取得
    eCmdValCalcuAlignOffsetFAOF   = 44,		// 1、FAOF:自动补偿量值的取得，对准偏移量以补偿量值取得

    eCmdValSaveTargetPosFTGR	 = 45,		// 1、FTGR:目标 Mark 位置的登录，将目前使用的产品数据保存在指定的产品No.中并保存在CF 记忆卡
    eCmdValDisplayTargetFTGD	 = 46,		// 2、FTGD:目标 Mark 指示线的全画面显示，在指定目标 Mark No.的目标 Mark 位置上显示全画面的指示线及刻度
    eCmdValManualSearchFMSC		 = 47,		// 3、FMSC:手动搜索位置的取得，在自动对准时，当目标 Mark 或物体 Mark 搜索失败时，可利用手动方法（鼠标）指定Mark 位置
    eCmdValTargetSearchFTGM		  = 48,		// 7、FTGM:目标 Mark 位置的手动取得，取得目标 Mark（目标）位置
    eCmdValObjectSearchFOBM		  = 49,		// 8、FOBM:物体 Mark 位置的手动取得，取得物体 Mark（对准靶标）位置

	eCmdValSpecialCommFCHK       = 50,     // 6、特殊通讯指令，收取FCHK 10，发送FCHK CY 程序有效
	eCmdValObjectSearchFCNT      = 51,		// 5、FCNT：对象定位同时将目标位置设为图像中心，参数类似于FOBJ P1
	eCmdValFAAO					 = 52,
	eCmdValGetSearchWorldPosFWLD = 53,		// 4、FWLD：取得目标（目标靶标）或对象（对象靶标）的平台坐标上的位置
	eCmdValFCHA					 = 54,		// FCHA
	eCmdValFALS					 = 55,		// FALS
	eCmdValFPCB					 = 56,		// FPCB
	eCmdValFCHB					 = 57,		// FCHB	
	eCmdValFVAS					 = 58,		// FVAS
	eCmdValFGTC					 = 59,		// FGTC
	eCmdValFWEL					 = 60,		// FWEL
	eCmdValFWDR					 = 61,		// 5.1、FWDR:获取目标/对象Mark的平台坐标系半径（相对于旋转中心的距离）
	eCmdValFGOW					 = 62,		// 6、FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
	eCmdValGetImagePosFGTC       = 63,		// 5、FGTC:获取目标相对于图像中心的图像坐标偏差
	eCmdValFDRP					 = 64,		// FDRP
	eCmdValFCHC					 = 65,		// FCHC	
	eCmdValMPSB					 = 66,		// MPSB 获取备用寄存器轴位置
	eCmdValGetOffsetFGPO		 = 67,		// FGPO 获取最终精度（连续对位中使用）
	eCmdValSetProductExposureFSPE = 68,		// 设置产品曝光

	// 以下枚举值保留不用，如果需要用，则从上面的值重新排
	
    eCmdValFAOJ                   =69,
	eCmdValFSCM                   =70,
	
    eCmdValSaveProcessDataFSAC    = 71,		// 4、FSAC：保存校正、定位时的数据。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
    eCmdValSaveCommDataFSCO		  = 72,		// 5、FSCO：保存指令收发的历史纪录。所保存的数据为最近500次操作时的数据（不到500次时保存实际次数的数据）
	
	eCmdValFSFT					  = 73,		// 1、FSFT:平台Shift 移动动作，选择内部转移量的设定值，并依照设定值将目前平台的位置做相对移动
	eCmdValFGSF					  = 74,		// 2、FGSF:对准Shift 移动量的取得，取得对准转移量
	
	//FAST 读写寄存器（暂无）
    eCmdValFISP                   = 75,
    eCmdValFOLG                   = 76,
    eCmdValFTLG                   = 77,
	eCmdValFPNT                   = 78,
	eCmdValFMAP                   = 79,
	eCmdValFRCT                   = 80,

    eCmdValPCBInspectFCPI         = 81,
    eCmdValPCBInspectFPIS         = 82,
    eCmdValTargetSearchFCNO       = 83,
	eCmdValGetAlarmFALM			  = 84,		// 3、FALM:Alarm资料的取得，取得错误发生的数据代码
	//读取二维码
	eCmdValFQRC				      = 85,
	eCmdValInspectFINS			  = 86,		// 检测产品有、无
    eCmdValFPEP                   = 87,     // 产品清空
    eCmdValFPCP                   = 88,     // 产品拷贝
    eCmdValFPRN                   = 89,     // 产品重命名
	eCmdValFCBP                   = 90,     // 自动关联
	eCmdValFGTW                   = 91,     // 视觉进行目标拍照并发送目标平台坐标和轴位置给PLC
	eCmdValFAFO                   = 92,     // 目标采用靶标关联时，计算目标坐标系到对象坐标系的固定补偿 
	eCmdValFGTP                   = 93,     // 视觉进行目标拍照并发送目标图像坐标和轴位置给PLC
	eCmdValGetTargetObjectOffsetFMCO = 94,  // 1、FMCO:偏移量的取得，取得目标 Mark 及物体 Mark 之间中心的平台坐标系偏移量
	eCmdValFVPC					  = 95,		// 切换虚拟位置模式
	
	eCmdValFSOL					  = 96,		// FSOL

	eCmdValFQRI				      = 97,
	eCmdValFPMI					  = 98,		// 开启系统设置权限
	eCmdValFBIS					  = 99,	

	eCmdValFCCI					  = 200,    // lzk 复制标定信息
}SysWorkCmdValue;

// Vision接收到的PLC寄存器系统工作指令值(是唯一的)
// 注：Vision反馈给PLC响应时同样使用该工作指令值。即PLC寄存器指令值与Vision响应指令值一般是一样的。
typedef enum enumVSysWorkCmdValue
{
	eVCmdFRST = 1,// 	1	FRST
	eVCmdFHMS = 2,// 	2	FHMS
	eVCmdFHOM = 3,// 	3	FHOM
	eVCmdFSPC = 4,// 	4	FSPC
	eVCmdFCLB = 5,// 	5	FCLB
	eVCmdFTGT = 6,// 	6	FTGT
	eVCmdFOBJ = 7,// 	7	FOBJ
	eVCmdFAAL = 8,// 	8	FAAL
	eVCmdFNPS = 9,// 	9	FNPS
	eVCmdFMVA = 10,// 	10	FMVA
	eVCmdFMVR = 11,// 	11	FMVR
	eVCmdFPNT = 12,// 	12	FPNT
	eVCmdFACO = 13,// 	13	FACO
	eVCmdFALG = 14,// 	14	FALG
	eVCmdFCNT = 15,// 	15	FCNT
	eVCmdFGAP = 16,// 	16	FGAP
	eVCmdFTGC = 17,// 	17	FTGC
	eVCmdFOBC = 18,// 	18	FOBC
	eVCmdFAAC = 19,// 	19	FAAC
	eVCmdFGSP = 20,// 	20	FGSP
	eVCmdFCBS = 21,// 	21	FCBS
	eVCmdFMAP = 22,// 	22	FMAP
	eVCmdFXYA = 23,// 	23	FXYA
	eVCmdFRCT = 24,// 	24	FRCT
	eVCmdFISP = 25,// 	25	FISP
	eVCmdFOLG = 26,// 	26	FOLG
	eVCmdFTLG = 27,// 	27	FTLG
	eVCmdFOLC = 28,// 	28	FOLC

	eVCmdFALP = 29,// 	29	FALP	// 已用
	eVCmdFTGM = 30,// 	30	FTGM
	eVCmdFCHK = 31,// 	31	FCHK
	eVCmdFCHB = 32,// 	32	FCHB
	eVCmdFTGG = 33,// 	33	FTGG	// 已用
	eVCmdFTGD = 34,// 	34	FTGD
	eVCmdFOBM = 35,// 	35	FOBM
	eVCmdFOBG = 36,// 	36	FOBG	// 已用
	eVCmdFMSC = 37,// 	37	FMSC
	eVCmdFWLD = 38,// 	38	FWLD	// 已用
	eVCmdFGAL = 39,// 	39	FGAL
	eVCmdFALL = 40,// 	40	FALL
	eVCmdFGAC = 41,// 	41	FGAC
	eVCmdFALC = 42,// 	42	FALC
	eVCmdFAAB = 43,// 	43	FAAB
	eVCmdFACB = 44,// 	44	FACB
	eVCmdFAHL = 45,// 	45	FAHL
	eVCmdFANS = 46,// 	46	FANS
	eVCmdFALJ = 47,// 	47	FALJ
	eVCmdFABO = 48,// 	48	FABO
	eVCmdFAOF = 49,// 	49	FAOF
	eVCmdFGOF = 50,// 	50	FGOF
	eVCmdFOFS = 51,	// 	51	FOFS
	eVCmdFSFT = 52,// 	52	FSFT
	eVCmdFGSF = 53,// 	53	FGSF
	eVCmdFGTC1 = 54,// 	54	FGTC1
	eVCmdFSAC = 55,// 	55	FSAC
	eVCmdFSCO = 56,// 	56	FSCO
	eVCmdFTGS = 57,// 	57	FTGS
	eVCmdFSSV = 58,// 	58	FSSV
    eVCmdFOBS = 59,//   59  FOBS
	eVCmdFAOT = 60,// 	60	FAOT
	eVCmdFALX = 61,// 	61	FALX
	eVCmdFAAO = 62,// 	62	FAAO
	eVCmdFCBR = 63,// 	63	FCBR
	eVCmdFCHA = 64,// 	64	FCHA
	eVCmdFALS = 65,// 	65	FALS
	eVCmdFPCB = 66,// 	66	FPCB
	eVCmdFTGR = 67,// 	67	FTGR
	eVCmdFVAS = 68,// 	68	FVAS
	eVCmdFGTC = 69,// 	69	FGTC
	eVCmdFWEL = 70,// 	70	FWEL
	eVCmdFWDR = 71,// 	71	FWDR
	eVCmdFGOW = 72,//   72、FGOW:拍照取得对象（对象靶标）的平台坐标上的位置
	eVCmdFCHC = 73,// 	73	FCHC
	eVCmdFDRP = 74,// 	74	FDRP
	eVCmdMPSB = 75,//   75  MPSB 获取备用寄存器轴位置
	eVCmdFGPO = 76,//   76  FGPO 发送最终对位精度偏差
	eVCmdFSPE = 77,	// 	77、设置产品曝光
    eVCmdFAOJ = 78,// 	78	FAOJ
    eVCmdFCPR = 79,// 	79	FCPR
    eVCmdFSCM = 80,// 	80	FSCM
    eVCmdFCPI = 81,// 	81	FCPI
    eVCmdFPIS = 82,// 	82	FPIS
    eVCmdFCNO = 83,// 	83	FCNO
    eVCmdFALM = 84,// 	84	FALM
	eVCmdFQRC = 85,//   85 存储二维码
	eVCmdFINS = 86,	// 	86、检测产品有、无
    eVCmdFPEP = 87, //  87、产品清空
    eVCmdFPCP = 88,//   88、产品拷贝
    eVCmdFPRN = 89,//   89、产品重命名
	eVCmdFCBP = 90,//   90、自动关联
	eVCmdFGTW = 91,//   91、视觉进行目标拍照并发送目标平台坐标和轴位置给PLC
	eVCmdFAFO = 92,//   92、目标采用靶标关联时，计算目标坐标系到对象坐标系的固定补偿
	eVCmdFGTP = 93,//   93、视觉进行目标拍照并发送目标图像坐标和轴位置给PLC
	eVCmdFMCO = 94,// 	94	FMCO
	eVCmdFVPC = 95,//   95、切换虚拟位置模式
	eVCmdFSOL = 96,// 	96	FSOL
	eVCmdFQRI = 97,//   97 检测二维码
	eCmdFPMI	= 98,// 开启系统设置权限
	eCmdFBIS	= 99,
	eVCmdFCCI = 200,//  lzk 复制标定信息
	
}SysVWorkCmdValue;



// 指令响应的反馈状态
typedef enum enumCmdAnsweredStatus
{
   eAswSucceed = 0,    // 成功
   eAswComErr,         // 通信错误（超时）
   eAswCmdParamErr,    // 指令参数错误（即数据有问题）
   eAswCmdErr,         // 指令错误（即指令名、起始符、结束符、指令长度错误）
   eAsWCmdSearchErr,   // 定位错误
   eAswUnkown,         // 其他错误

}CmdAnsweredStatus;

// 视觉系统状态
typedef enum enumVisionStatus
{
	eVsConneted = 1,			// 链接到了PLC，可以进行通信
	eVsDisconneted = 2,			// 断开与PLC的链接，不可以进行通信
	eVsCloseSoft = 3,			// 退出软件，断开与PLC的链接，不可以进行通信
	eVsCloseWindow = 4,			// 关闭电脑，断开与PLC的链接，不可以进行通信
}VisionStatus;

// 指令参数顺序类型
typedef enum enumCmdParamOrderType
{
	eCPOrderXYD = 0,    // XYD
	eCPOrderYXD,        // YXD
	eCPOrderXDY,
	eCPOrderDXY,
	eCPOrderYDX,
	eCPOrderDYX,
}CmdParamOrderType;


// 运动控制类型
typedef enum enumMotionControlType
{
	eMCNo = 0,			// 无运动控制
	eMCYamahaRobot,		// 雅马哈机器人
	eMCYamahaRobot340	// 雅马哈机器人340
}MotionControlType;

typedef enum enumKeyenceCodeType
{
	eCodeNone = 0,
	eCodeOK,			// 成功
	eCodeNG,			// 失败
	eCodeContinued,		// 继续
	eCodeSnapOK,		// 采集完成
	eCodeSizeNG         //尺寸检查NG
}KeyenceCodeType;
#define RREG_KEYENCE_HEARTBEAT_TRIGGER "0000"		// 心跳触发地址偏差量
#define RREG_KEYENCE_VISION_TRIGGER "0002"			// 视觉发送命令地址偏差量
#define RREG_KEYENCE_VISION_MMVA_TRIGGER "0003"		// 视觉开始驱动的地址偏差量
#define RREG_KEYENCE_VISION_CALIB_TRIGGER "0004"	// 视觉发送标定成功地址偏差量

#define RREG_KEYENCE_VISION_PARAM_RESERVED "0005"	// 视觉发送命令地址偏差量
#define RREG_KEYENCE_VISION_PARAM_DIALOG "0006"	    // 视觉通知PLC弹窗地址偏差量


#define RREG_KEYENCE_VISION_AXIS_READ "0010"		// 视觉给出偏差量地址偏差量

#define RREG_KEYENCE_VISION_INSPECT_READ "0030"		// 检测数据地址偏差量

#define RREG_KEYENCE_PLC_TRIGGER "0050"				// 基恩士指令格式： PLC发送命令触发地址偏差量
#define RREG_KEYENCE_PLC_MMVA "0051"				// 基恩士指令格式： 驱动完成信号地址偏差量
#define RREG_KEYENCE_PLC_AXIS_READ "0060"			// PLC实时轴地址偏差量

#define RREG_KEYENCE_PLC_REG_NUMS 200				// 基恩士格式码 地址段长度; 

typedef enum enumKeyenceWorkCmdType
{
	eKeyenceCmdNone = 0,
	eKeyenceCmdFAAL,		// 1 -FAAL
	eKeyenceCmdCalibrate,	// 2- 标定指令

	eKeyenceCmdFOBC,		// 3- 对象拍照
	eKeyenceCmdFTGC,		// 4- 目标拍照
	eKeyenceCmdFSPC,		// 5- 切换品种
	eKeyenceCmdFCHB,		// 6- 检测指令
	eKeyenceCmdFALP,		// 7- 拾取指令
	eKeyenceCmdFAAC,		// 8- 多次对位
	eKeyenceCmdFMCO,		// 9- 一次发送多目标结果
	eKeyenceCmdFCBP,		// 10-自动关联
	eKeyenceCmdFCBS,		// 11-FCBS 分步标定
	eKeyenceCmdFACO,		// 12- 对位指令：自动执行目标、对象拍照并对位（之前逻辑，调用OnlineAlignExFACO_WithSnap_Ex）  目前调整为（OnlineAlignExFACO_Ex 需要发送对象、目标拍照指令，才能执行此指令）
	eKeyenceCmdFPMI,		// 13-开启系统设置权限
	eKeyenceCmdFBIS,		// 13-开启系统设置权限
}KeyenceWorkCmdType;

#define Code_Group_Max_Num 1000
typedef enum enumKeyenceWorkCmdGroupType
{
	// eKeyenceCmdGroupFACO --eKeyenceCmdGroupCalibrate之间的数据：例如1001代表触发FACO 1指令
	// 规则指令码的百十位数设置为P1；指令码的个位数设置P2 例如 FOBC 12 1--对应3121、FOBC 5 2-对应3052;
	eKeyenceCmdGroupNone		= -1,
	eKeyenceCmdGroupFACO		= 1 * Code_Group_Max_Num,		// 1- 对位指令：自动执行目标、对象拍照并对位（之前逻辑，调用OnlineAlignExFACO_WithSnap_Ex）  目前调整为（OnlineAlignExFACO_Ex 需要发送对象、目标拍照指令，才能执行此指令）
	eKeyenceCmdGroupCalibrate	= 2 * Code_Group_Max_Num,		// 2- 标定指令
	eKeyenceCmdGroupFOBC		= 3 * Code_Group_Max_Num,		// 3- 对象拍照
	eKeyenceCmdGroupFTGC		= 4 * Code_Group_Max_Num,		// 4- 目标拍照
	eKeyenceCmdGroupFSPC		= 5 * Code_Group_Max_Num,		// 5- 切换品种
	eKeyenceCmdGroupFCHB		= 6 * Code_Group_Max_Num,		// 6- 检测指令
	eKeyenceCmdGroupFALP		= 7 * Code_Group_Max_Num,		// 7- 拾取指令
	eKeyenceCmdGroupFAAC		= 8 * Code_Group_Max_Num,		// 8- 多次对位
	eKeyenceCmdGroupFMCO		= 9 * Code_Group_Max_Num,		// 9- 一次发送多目标结果
	eKeyenceCmdGroupFCBP		= 10 * Code_Group_Max_Num,		// 10- 自动关联
	eKeyenceCmdGroupFCBS		= 11 * Code_Group_Max_Num,		// 11- 分步标定
	eKeyenceCmdGroupFAAL		= 12 * Code_Group_Max_Num,		// 11- 分步标定
	eKeyenceCmdGroupFPMI		= 13 * Code_Group_Max_Num,		// 13- 开启系统设置权限
	eKeyenceCmdGroupFBIS		= 14 * Code_Group_Max_Num,		// 14- 分步检测
}KeyenceWorkCmdGroupType;
class CCommProtocol    
{
public:
	CCommProtocol();
	virtual ~CCommProtocol();

public:

	// 设置、获取通信协议参数
	void SetCommProtocolParam(CommProtocolParam param);
	CommProtocolParam  GetCommProtocolParam();

	// 设置、获取系统选项配置
	void SetSystemOptionConfig(SystemOptionConfig systemOptionConfig);
	SystemOptionConfig GetSystemOptionConfig();

	// 设置、获取中间平台
	void SetMidPlatformInfo(PlatformType ePlatformType, CPlatformInfo* pPlatformInfo);
	CPlatformInfo* GetMidPlatformInfo();

	// 设置、获取平台序号
	virtual void SetPlatformIndex(int nPlatformIndex);
	virtual int GetPlatformIndex();

	// 设置、获取PLC寄存器基地址
	virtual void SetPLCBaseRegAddr(long nBaseRegAddr);
	virtual long GetPLCBaseRegAddr();

	// 设置、获取PLC寄存器弯折路径地址
	virtual void SetPLCWZRegAddr(long nWARegAddr);
	virtual long GetPLCWZRegAddr();

	// 设置、获取PLC寄存器ID地址
	virtual void SetPLCIDRegAddr(long nBaseRegAddr);
	virtual long GetPLCIDRegAddr();
	virtual void SetPLCIDLength(int nLength);
	virtual int GetPLCIDLength();
	virtual void SetEnableIDRead(BOOL bEnable);
	virtual BOOL GetIsEnableIDRead();
	virtual void SetEnableIDInvert(BOOL bEnable);
	virtual BOOL GetIsEnableIDInvert();

	// 设置、获取PLC临时寄存器基地址
	virtual void SetPLCTempBaseRegAddr(long nTempBaseRegAddr);
	virtual long GetPLCTempBaseRegAddr();

	// 设置、获取读取/写入PLC寄存器字节数
	virtual void SetPLCRegType(bool bRegType);
	virtual bool GetPLCRegType();

	// 设置、获取协议类型
	virtual void SetPLCRegDataTypeIndex(int nRegDataTypeIndex);
	virtual int GetPLCRegDataTypeIndex();

	// 设置、获取协议类型
	virtual void SetProtocolType(ProtocolType ptProtocolType);
	virtual ProtocolType GetProtocolType();

	// 设置、获取扩展协议类型
	virtual void SetExProtocolType(ExProtocolType ptExProtocolType);
	virtual ExProtocolType GetExProtocolType();

	// 判断当前的协议类型是否为PLC链接协议，直接读取存储PLC寄存器的
	virtual BOOL IsPlcProtocolType();

	// 设置服务器（PLC）通信信息
	virtual BOOL SetServerCommInfo(vcCommInfo *commInfo);	
	// 设置本地（PC）通信信息
	virtual BOOL SetLocalCommInfo(vcCommInfo *commInfo);

	// 设置、获取命令参数顺序类型
	void SetCmdParamOrderType(CmdParamOrderType cpOrderType);
	CmdParamOrderType GetCmdParamOrderType();

	// 设置、获取通信单产品多标定扩展
	void SetEnableCommMultiCalibExtension(BOOL bEnable);
	BOOL GetEnableCommMultiCalibExtension();

	//设置、获取多目标定位通信
	void SetEnableCommMutiTarget(BOOL bEnable);
	BOOL GetEnableCommMutiTarget();
	/////////////////////////////// 解析接收到的数据////////////////////////////////
	
	// 解析收到的系统工作指令类型
    virtual SysWorkCmdType AnalyzeSysWorkCmdReceived(CString strCmd);    
	virtual SysWorkCmdType AnalyzeSysWorkCmdReceived(std::vector<int> vnReceivedData);

	
	// 解析收到的“获取平台各轴绝对位置”响应
    virtual CmdAnsweredStatus AnalyzeAswGetGetPlatformAbsPos(CString strAsw, CPlatformAxisPos* vppPlatAxisPos) = 0;
   
	// 解析收到的“指定平台各轴移动到的绝对位置”响应
	virtual CmdAnsweredStatus AnalyzeAswDrivePlatformMoveToAbsPos(CString strAsw);

    // 解析收到的“获取相机平台各轴绝对位置”响应
	virtual CmdAnsweredStatus AnalyzeAswGetCamPlatformAbsPos(CString strAsw, std::vector<CPlatformXYAxisPos*>& vpCamPltAxisPos);
	
    // 解析收到的“指定相机平台各轴移动到的绝对位置”响应
	virtual CmdAnsweredStatus AnalyzeAswDriveCamPlatformMoveToAbsPos(CString strAsw);    
	
   
	// 解析收到的“等待相机平台将相机由A位置移动至B位置”响应
	virtual CmdAnsweredStatus AnalyzeAswWaitCamPltMove(CString strAsw);
	
	// 解析收到的产品名称
	virtual CmdAnsweredStatus AnalyzeAswSetProductName(CString strAsw, CString& strProductName);
	
	// 解析收到的产品索引
	virtual CmdAnsweredStatus AnalyzeAswSetProductIndex(CString strAsw, int& nProductIndex);

private:
	
	// 解析是否系统标定指令
    virtual BOOL IsSysCalibrateCmd(CString strCmd);

	// 解析是否系统标定指令
    virtual BOOL IsSysResetCmd(CString strCmd);
	
	// 解析是否在线对位指令
	virtual BOOL IsOnLineAlignCmd(CString strCmd);

		// 解析是否在线对位指令
	virtual BOOL IsOnLineAlignCmdFACO(CString strCmd);
	// 解析是否在线对位指令
	virtual BOOL IsOnLineAlignCmdFAAL(CString strCmd);
	// 解析是否在线对位指令
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
	
	// 解析是否设置当前产品名称指令
    virtual BOOL IsSetProductNameCmd(CString strCmd);
	
	// 解析是否设置当前产品索引指令
    virtual BOOL IsSetProductIndexCmd(CString strCmd);

	virtual BOOL IsGetAlignResult(CString strCmd);

	virtual BOOL IsSetTargetBench(CString strCmd);



public:

	/////////////////////////////// 打包需要发送的数据//////////////////////////////
	
	// <平台移动>

	// 打包“获取平台各轴绝对位置”指令： TAG
	virtual BOOL PackGetPlatformAbsPos(CString& strData);	

	// 打包“指定平台各轴移动到的绝对位置”指令: TAR
	virtual BOOL PackDrivePlatformMoveToAbsPos(CString& strData, CPlatformAxisPos* vpPlatAxisPos) = 0;
    virtual BOOL PackDrivePlatformMoveToAbsPosEx(CString& strData, CPlatformAxisPos* vpPlatAxisPos, CPlatformAxisPos* vpPlatAxisPosBase) = 0;

	// <相机移动>
   
	// 打包“获取相机平台各轴绝对位置”指令： TCG
	virtual BOOL PackGetCamPlatformAbsPos(CString& strData);	
	
	// 打包“指定相机平台各轴移动到的绝对位置”指令: TCR
	virtual BOOL PackDriveCamPlatformMoveToAbsPos(CString& strData, const std::vector<CPlatformXYAxisPos*> vpCamPltAxisPos);
    
    
	//<其他>
	// 标定/对位中，“等待相机平台将相机由A位置移动至B位置”指令: WAT（仅在单相机对位中用）
    virtual BOOL PackWaitCamPltMove(CString& strData);

	//////////////////////////////////////////////////////////////////////////////////

public:
	CommProtocolParam m_CommProtocolParam;
	SystemOptionConfig m_SystemOptionCofing;
	CPlatformInfo	  *m_pMidPlatformInfo;
	PlatformType	   m_eMidPlatformType;
	CmdParamOrderType  m_eCmdParamOrderType;
	ProtocolType	   m_eProtocolType;
	ExProtocolType	   m_eExProtocolType;
	int				   m_nPlatformIndex;
	long			   m_nPLCBaseRegAddr;
	long			   m_nPLCWZRegAddr;
	long			   m_nPLCTempBaseRegAddr;
	long			   m_nPLCIDRegAddr;
	int				   m_nIDLength;
	BOOL			   m_bEnableIDRaed;
	BOOL			   m_bIDInvert;
	CCommProRegBase		   *m_pCommProRegBase;
	BOOL				m_bRegType;				// 读取寄存器是单字节/双字节 = TURE
	int				   m_nRegDataTypeIndex; // 寄存器地址数据类型序号
	BOOL				m_bEnableCommMultiCalibExtension;
	BOOL                m_bEnableCommMutiTarget;
public:
	BOOL IsStringNumerical( LPCTSTR lpszVal );

};

#endif // !defined(AFX_COMMPROTOCOLPROCESS_H__30AE4D85_B685_4811_B0DD_163B0D8DFBB6__INCLUDED_)
