#pragma once

#ifdef SVCODE_EXPORTS
#define SVCODE_API __declspec(dllexport)
#else
#define SVCODE_API __declspec(dllimport)
#endif

enum CodeType1D
{
	e1DCODE_NONE = 0,		// unsupport
	e1DCODE_EAN8 = 1,		// EAN8 Code
	e1DCODE_UPCE = 2,		// UPCE Code
	e1DCODE_UPCA = 3,		// UPCA Code
	e1DCODE_EAN13 = 4,		// EAN13 Code
	e1DCODE_ISBN13 = 5,		// ISBN13 Code
	e1DCODE_CODABAR = 6,	// codabar Code
	e1DCODE_ITF25 = 7,		// itf 25 Code
	e1DCODE_39 = 8,			// Code 39
	e1DCODE_93 = 9,			// Code 93
	e1DCODE_128 = 10,		// Code 128
};

enum CodeType2D
{
	e2DCODE_NONE = 0,		// 无可识别条码
	e2DCODE_DM = 1,			// DM码
	e2DCODE_QR = 2,			// QR码
};

enum DecodeMethod
{
	eDecodeMethod_Default=0,
	eDecodeMethod_Vsb,
	eDecodeMethod_Vsc,
};

enum AppMode
{
	eAppMode_Normal=0,
	eAppMode_Pro,
	eAppMode_Fast,
};

enum DiscreteFlag
{
	eDiscreteFlag_Continuous=0,
	eDiscreteFlag_Disperse,
	eDiscreteFlag_Both,
};

enum DistortionFlag
{
	eDistortionFlag_Close=0,
	eDistortionFlag_Open,
};

enum MirrorMode
{
	eMirrorMode_Close=0,
	eMirrorMode_Open,
	eMirrorMode_Both,
};

enum PolarityMode
{
	ePolarityMode_BlackOnWhite=0,
	ePolarityMode_WhiteOnBlack,
	ePolarityMode_Both,
};

enum RectangleFlag
{
	eRectangleFlag_Square=0,
	eRectangleFlag_Rectangle,
	eRectangleFlag_Both,
};

enum SegmentationMethod
{
	eSegmentationMethod_Local =0,
	eSegmentationMethod_Global,
};

enum BoxDisplayMode
{
	eBoxDisplayMode_Raw=0,     //非精确
	eBoxDisplayMode_Accurate,  //精确
};

enum SymbolType
{
	eSymbolType_WhiteOnBlack = 0,
	eSymbolType_BlackOnWhite,
	eSymbolType_Outline,
	eSymbolType_Normal,
};

enum BitsPerCell
{
	eBitsPerCell_PerCell2 = 0,
	eBitsPerCell_PerCell4,
	eBitsPerCell_PerCell8,
	eBitsPerCell_PerCell16,
};

enum EdacLevel
{
	eEdacLevel_Auto = 0,
	eEdacLevel_LEVEL_12_5,
	eEdacLevel_LEVEL_25,
};

enum CompressionMode
{
	eCompressionMode_None = 0,
	eCompressionMode_Numric,
	eCompressionMode_AlphaNumric,
};

enum  ProcessingPlatformType
{
	ePlatform_Undefine = 0,
	ePlatform_CPU = 1,
	ePlatform_GPU = 2,
};

enum CodeType
{
	eCodeType_None = 0,         // 无可识别条码
	eCodeType_EAN8 = 1,         // EAN8码
	eCodeType_UPCE = 2,         // UPCE码
	eCodeType_UPCA = 3,			// UPCA码
	eCodeType_EAN13 = 4,        // EAN13码
	eCodeType_ISBN13 = 5,       // ISBN13码--暂不支持
	eCodeType_CODABAR = 6,      // 库德巴码
	eCodeType_ITF25 = 7,		// 交叉25码
	eCodeType_CODE39 = 8,       // Code 39
	eCodeType_CODE93 = 9,       // Code 93
	eCodeType_CODE128 = 10,     // Code 128
	eCodeType_ITF14 = 11,       // 交叉14码
	eCodeType_MATRIX25 = 12,    // MATRIX25码识别模块
	eCodeType_MSI = 13,			// MSI码识别模块
	eCodeType_CODE11 = 14,      // code11
	eCodeType_INDUSTRIAL25 = 15,     // industrial25
	eCodeType_CHINAPOST = 16,        // 中国邮政码

	eCodeType_DM = 101,       // DM码
	eCodeType_QR = 102,       // QR码
	eCodeType_ECC140 = 103,   // ECC140码
};

// 评级标准
enum VerifyStandard
{
	eVerifyISOStandard_15415 = 0,         // iso 15415标准
	eVerifyISOStandard_29158 = 1,         // iso 29158标准
	eVerifyISOStandard_15416 = 2,         // iso 15416标准
};

// 验证方式
enum VerifyLabel
{
	eVerifyLabel_Standard = 0,         // 码评级完全按照iso标准
	eVerifyLabel_Custom = 1,           // 部分指标采用定制
};

// 处理类型
enum VerifyProcessType
{
	eVerifyProcessType_I = 0,	 // 只支持DM码,效果较优
	eVerifyProcessType_II = 1,   // 支持DM码和QR码
};

void SVCODE_API ProLoadCodeDll();