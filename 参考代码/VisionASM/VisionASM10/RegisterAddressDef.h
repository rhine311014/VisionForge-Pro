#pragma once


//////////////////////////////////////////////////////////////////////////
// PLC基地址
#define PLC_BASE_REG				("000000")		// PLC基地址

// DWORD
//////////////////////////////////////////////////////////////////////////
// 下面的PLC寄存器地址定义使用的是相对偏移量（相对于PLC基地址），每个平台最少要预留出100个地址

///////////////////////////////////////////////////////////////////////////////////////////
// PLC发送动作指令或响应寄存器（相对偏移量地址）
#define RREG_PLC_CMD_TRIGGER		("0000")		// PLC动作指令或响应触发寄存器(0000~0001)
#define RREG_PLC_CMD_DATA			("0002")		// PLC动作指令或响应指令寄存器(0002~0003)
#define RREG_PLC_CMD_PARAM_P1		("0004")		// PLC动作指令或响应参数P1(0004~0005)
#define RREG_PLC_CMD_PARAM_P2		("0006")		// PLC动作指令或响应参数P2(0006~0007)
#define RREG_PLC_CMD_PARAM_P3		("0008")		// PLC动作指令或响应参数P3(0008~0009)
#define RREG_PLC_CMD_PARAM_P4		("0010")		// PLC动作指令或响应参数P4(00010~0011)
#define RREG_PLC_CMD_PARAM_P5		("0012")		// PLC动作指令或响应参数P5(00012~0013)

// PLC平台当前轴位置数据寄存器（Vision读取PLC）
#define RREG_PLATFORM_CURRENT_POS_X ("0014")		// 平台X轴当前位置(00014~0015)
#define RREG_PLATFORM_CURRENT_POS_Y ("0016")		// 平台Y轴当前位置(00016~0017)
#define RREG_PLATFORM_CURRENT_POS_D ("0018")		// 平台D轴当前位置(00018~0019)

// PLC平台当前轴位置数据备用寄存器（Vision读取PLC）
#define RREG_PLATFORM_CURRENT_POS_X_BACKUP ("0020")		// 平台X轴当前位置(00020~0021)
#define RREG_PLATFORM_CURRENT_POS_Y_BACKUP ("0022")		// 平台Y轴当前位置(00022~0023)
#define RREG_PLATFORM_CURRENT_POS_D_BACKUP ("0024")		// 平台D轴当前位置(00024~0025)

#define RREG_PLATFORM_CURRENT_POS_X2 ("0026")		// 平台X轴当前位置(00026~0027)
#define RREG_PLATFORM_CURRENT_POS_Y2 ("0028")		// 平台Y轴当前位置(00028~0029)
#define RREG_PLATFORM_CURRENT_POS_D2 ("0030")		// 平台D轴当前位置(00030~0031)

#define RREG_QRCODE					("0020")		//二维码存储地址（0020~0037）
///////////////////////////////////////////////////////////////////////////////////////////
// Vision发送状态指令寄存器（Vision写入PLC）
#define RREG_VISION_STATUS			("0048")		// VISION状态寄存器(0048~0049)

///////////////////////////////////////////////////////////////////////////////////////////
// Vision发送视觉响应或指令寄存器（Vision写入PLC）
#define RREG_VISION_CMD_TRIGGER		("0050")		// VISION响应或动作指令触发寄存器(0050~0051)
#define RREG_VISION_CMD_DATA		("0052")		// PC响应或动作指令指令寄存器(0052~0053)
#define RREG_VISION_CMD_PARAM_P1	("0054")		// VISION响应或动作指令参数P1(0054~0055)
#define RREG_VISION_CMD_PARAM_P2	("0056")		// VISION响应或动作指令参数P2(0056~0057)
#define RREG_VISION_CMD_PARAM_P3	("0058")		// VISION响应或动作指令参数P3(0058~0059)
#define RREG_VISION_CMD_PARAM_P4	("0060")		// VISION响应或动作指令参数P4(00060~0061)
#define RREG_VISION_CMD_PARAM_P5	("0062")		// VISION响应或动作指令参数P5(00062~0063)

// Vision发送移动轴位置数据寄存器（Vision写入PLC）
#define RREG_PLATFORM_MOVE_POS_X	("0064")		// 平台X轴移动位置(00064~0065)
#define RREG_PLATFORM_MOVE_POS_Y	("0066")		// 平台Y轴移动位置(00066~0067)
#define RREG_PLATFORM_MOVE_POS_D	("0068")		// 平台D轴移动位置(00068~0069)

// Vision发送移动轴位置数据寄存器（Vision写入PLC）
#define RREG_PLATFORM_MOVE_POS_X_BACKUP	("0070")		// 平台X轴移动位置(00070~0071)
#define RREG_PLATFORM_MOVE_POS_Y_BACKUP	("0072")		// 平台Y轴移动位置(00072~0073)
#define RREG_PLATFORM_MOVE_POS_D_BACKUP	("0074")		// 平台D轴移动位置(00074~0075)

#define RREG_PLATFORM_MOVE_POS_X2	("0076")		// 平台X轴移动位置(00076~0077)
#define RREG_PLATFORM_MOVE_POS_Y2	("0078")		// 平台Y轴移动位置(00078~0079)
#define RREG_PLATFORM_MOVE_POS_D2	("0080")		// 平台D轴移动位置(00080~0081)




// 临时寄存器地址
#define RREG_VISION_PLATFORM_TEMP_POS	("0100")		// 视觉临时寄存器地址



//////////////////////////////////////////////////////////////////////////
// 下面的PLC寄存器地址定义使用的是相对偏移量（相对于PLC基地址），每个平台最少要预留出100个地址

// WORD
///////////////////////////////////////////////////////////////////////////////////////////
// PLC发送动作指令或响应寄存器（相对偏移量地址）
#define RREG_PLC_CMD_TRIGGER_WORD		("0000")		// PLC动作指令或响应触发寄存器
#define RREG_PLC_CMD_DATA_WORD			("0001")		// PLC动作指令或响应指令寄存器
#define RREG_PLC_CMD_PARAM_P1_WORD		("0002")		// PLC动作指令或响应参数P1
#define RREG_PLC_CMD_PARAM_P2_WORD		("0003")		// PLC动作指令或响应参数P2
#define RREG_PLC_CMD_PARAM_P3_WORD		("0004")		// PLC动作指令或响应参数P3
#define RREG_PLC_CMD_PARAM_P4_WORD		("0005")		// PLC动作指令或响应参数P4


// PLC平台当前轴位置数据寄存器（Vision读取PLC）
#define RREG_PLATFORM_CURRENT_POS_X_WORD ("0006")		// 平台X轴当前位置(00006~0007)
#define RREG_PLATFORM_CURRENT_POS_Y_WORD ("0008")		// 平台Y轴当前位置(00008~0009)
#define RREG_PLATFORM_CURRENT_POS_D_WORD ("0010")		// 平台D轴当前位置(00010~0011)

// 轴位置寄存器备用
#define RREG_PLATFORM_CURRENT_POS_X_BACKUP_WORD ("0012")		// 平台X轴当前位置(00012~0013)
#define RREG_PLATFORM_CURRENT_POS_Y_BACKUP_WORD ("0014")		// 平台Y轴当前位置(00014~0015)
#define RREG_PLATFORM_CURRENT_POS_D_BACKUP_WORD ("0016")		// 平台D轴当前位置(00016~0017)


#define RREG_STATIONCODE_WORD				("0068")		//工位存储地址（0068）
#define RREG_INSPECTSTATIONCODE_WORD				("0069")		//本压工位存储地址（0068）
#define RREG_QRCODE_WORD					("0070")		//二维码存储地址（0020~0037）

///////////////////////////////////////////////////////////////////////////////////////////
// Vision发送状态指令寄存器（Vision写入PLC）
#define RREG_VISION_STATUS_WORD			("0048")		// VISION状态寄存器(0048~0049)

///////////////////////////////////////////////////////////////////////////////////////////
// Vision发送视觉响应或指令寄存器（Vision写入PLC）
#define RREG_VISION_CMD_TRIGGER_WORD		("0020")		// VISION响应或动作指令触发寄存器
#define RREG_VISION_CMD_DATA_WORD		("0021")		// PC响应或动作指令指令寄存器
#define RREG_VISION_CMD_PARAM_P1_WORD	("0022")		// VISION响应或动作指令参数P1
#define RREG_VISION_CMD_PARAM_P2_WORD	("0023")		// VISION响应或动作指令参数P2
#define RREG_VISION_CMD_PARAM_P3_WORD	("0024")		// VISION响应或动作指令参数P3
#define RREG_VISION_CMD_PARAM_P4_WORD	("0025")		// VISION响应或动作指令参数P4
//#define RREG_VISION_CMD_PARAM_P5	("0026")		// VISION响应或动作指令参数P5

// Vision发送移动轴位置数据寄存器（Vision写入PLC）
#define RREG_PLATFORM_MOVE_POS_X_WORD	("0026")		// 平台X轴移动位置(00026~0027)
#define RREG_PLATFORM_MOVE_POS_Y_WORD	("0028")		// 平台Y轴移动位置(00028~0029)
#define RREG_PLATFORM_MOVE_POS_D_WORD	("0030")		// 平台D轴移动位置(00030~0031)

// 轴位置数据备用
#define RREG_PLATFORM_MOVE_POS_X_BACKUP_WORD	("0032")		// 平台X轴移动位置(00032~0033)
#define RREG_PLATFORM_MOVE_POS_Y_BACKUP_WORD	("0034")		// 平台Y轴移动位置(00034~0035)
#define RREG_PLATFORM_MOVE_POS_D_BACKUP_WORD	("0036")		// 平台D轴移动位置(00036~0037)

#define RREG_PLATFORM_RUN_WORD			("0039")		// 程序运行标志?

// 临时寄存器地址
#define RREG_VISION_PLATFORM_TEMP_POS_WORD	("0050")		// 视觉临时寄存器地址



//////////////////////////////////////////////////////////////////////////
// PLC寄存器值枚举定义

//////////////////////////////////////////////////////////////////////////
// PLC指令触发 
#define PLC_CMD_VAL_NOT_TRIGGERED				0		// 0-PLC指令未触发
#define PLC_CMD_VAL_TRIGGERED					1		// 1-PLC指令触发

//////////////////////////////////////////////////////////////////////////
// VISION指令触发 
#define VISION_CMD_VAL_NOT_TRIGGERED				0		// Vision指令未触发
#define VISION_CMD_VAL_TRIGGERED					1		// Vision指令触发



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//以下定义暂时不再使用

///////////////////////////////////////////////////////////////////////////////////////////
// PC发送运动数据寄存器
#define PLATFORM_RELATIVE_POS_X ("D*001810")				// 平台X轴相对位置
#define PLATFORM_RELATIVE_POS_Y ("D*001812")				// 平台Y轴相对位置
#define PLATFORM_RELATIVE_POS_D ("D*001814")				// 平台D轴相对位置

// PC读取位置数据寄存器
#define PLATFORM_CURRENT_POS_X ("D*001804")					// 平台X轴当前位置
#define PLATFORM_CURRENT_POS_Y ("D*001806")					// 平台Y轴当前位置
#define PLATFORM_CURRENT_POS_D ("D*001808")					// 平台D轴当前位置

///////////////////////////////////////////////////////////////////////////////////////////
// PLC发送动作寄存器
#define PLC_CMD_DATA_REG			("D*001800")		// PLC动作指令寄存器
#define PLC_CMD_TRIGER_REG			("D*001802")		// PLC指令触发寄存器



///////////////////////////////////////////////////////////////////////////////////////////
// PC发送状态指令寄存器
#define VISION_CMD_DATA_REG			("D*001816")		// PC动作指令寄存器
#define VISION_CMD_TRIGER_REG		("D*001818")		// PC指令触发寄存器


//////////////////////////////////////////////////////////////////////////
// PLC_CMD_DATA_REG
// 1-目标（芯片）拍照定位
// 2-对象（玻璃）拍照定位
// 3-标定平台与相机关系
// 4-对位对象和目标
// 5-标定平台电机轴移动完成OK
// 6-标定平台电机轴移动完成NG
// 7-对位平台电机轴移动完成OK
// 8-对位平台电机轴移动完成NG

// PLC_CMD_TRIGER_REG
// 0-PLC指令未触发
// 1-PLC指令触发

#define MC4_PLC_CMD_OBJECT_SEARCH				1		// 目标定位
#define MC4_PLC_CMD_TARGET_SEARCH				2		// 对象定位
#define MC4_PLC_CMD_CALIB						3		// 标定
#define MC4_PLC_CMD_ALIGN						4		// 对位
#define MC4_PLC_CMD_CALIB_MOVE_SUCCEED			5		// 标定平台电机轴移动完成OK
#define MC4_PLC_CMD_CALIB_MOVE_FAILED			6		// 标定平台电机轴移动完成NG
#define MC4_PLC_CMD_ALIGN_MOVE_SUCCEED			7		// 对位平台电机轴移动完成OK
#define MC4_PLC_CMD_ALIGN_MOVE_FAILED			8		// 对位平台电机轴移动完成NG

#define MC4_PLC_CMD_NOT_TRIGERED				0		// 0-PLC指令未触发
#define MC4_PLC_CMD_TRIGERED					1		// 1-PLC指令触发


//////////////////////////////////////////////////////////////////////////
// VISION_CMD_DATA_REG
// 1-目标拍照定位指令执行OK
// 2-目标拍照定位指令执行NG
// 3-对象拍照定位指令执行OK
// 4-对象拍照定位指令执行OK
// 5-标定OK（标定完成）
// 6-标定NG（标定完成）
// 7-对位OK（对位完成）
// 8-对位NG（对位完成）
// 9-标定时移动XYθ平台电机轴
// 10-对位时移动XYθ平台电机轴

// VISION_CMD_TRIGER_REG
// 0-Vision指令未触发
// 1-Vision指令触发

#define MC4_VISION_CMD_TARGET_SEARCH_SUCCEED	1		// 目标定位成功
#define MC4_VISION_CMD_TARGET_SEARCH_FAILED		2		// 目标定位成功
#define MC4_VISION_CMD_OBJ_SEARCH_SUCCEED		3		// 对象定位成功
#define MC4_VISION_CMD_OBJ_SEARCH_FAILED		4		// 对象定位成功
#define MC4_VISION_CMD_CALIB_SUCCEED			5		// 标定成功
#define MC4_VISION_CMD_CALIB_FAILED				6		// 标定失败
#define MC4_VISION_CMD_ALIGN_SUCCEED			7		// 对位成功，可以贴合
#define MC4_VISION_CMD_ALIGN_FAILED				8		// 对位成功，可以贴合
#define MC4_VISION_CMD_CALIB_MOVE_PLATFORM		9		// 移动平台电机组（标定）
#define MC4_VISION_CMD_ALIGN_MOVE_PLATFORM		10		// 移动平台电机组（对位）
#define MC4_VISION_CMD_OPERATION_FAILED			11		// 对象定位失败、目标定位失败或对位失败

#define MC4_VISION_CMD_NOT_TRIGERED				0		// Vision指令未触发
#define MC4_VISION_CMD_TRIGERED					1		// Vision指令触发