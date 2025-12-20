#pragma once

#include "svCodeCalibrateDef.h"

//一维码质量评级参数(按照ISO-15416评级)
struct Code1DVerifyPara
{
	bool m_bDecFlag;
	bool m_bEdgeFlag;
	bool m_bSCFlag;
	bool m_bMinrFlag;
	bool m_bMineFlag;
	bool m_bModFlag;
	bool m_bDcdFlag;
	bool m_bDefFlag;
	bool m_bQzFlag;
	int m_nDcdThrA;
	int	m_nDcdThrB;
	int m_nDcdThrC;
	int m_nDcdThrD;
	int m_nDefThrA;
	int m_nDefThrB;
	int m_nDefThrC;
	int m_nDefThrD;
	int m_nMinrThrA;
	int m_nMinrThrB;
	int m_nMinrThrC;
	int m_nMinrThrD;
	int m_nMineThrA;
	int m_nMineThrB;
	int m_nMineThrC;
	int m_nMineThrD;
	int m_nModThrA;
	int m_nModThrB;
	int m_nModThrC;
	int m_nModThrD;
	int m_nScThrA;
	int m_nScThrB;
	int m_nScThrC;
	int m_nScThrD;
	bool m_bApertureEnable;
	int m_nAperture;

	Code1DVerifyPara()
	{
		m_bDecFlag = true;
		m_bEdgeFlag = true;
		m_bSCFlag = true;
		m_bMinrFlag = true;
		m_bMineFlag = true;
		m_bModFlag = true;
		m_bDcdFlag = true;
		m_bDefFlag = true;
		m_bQzFlag = true;
		m_nDcdThrA = 62;
		m_nDcdThrB = 50;
		m_nDcdThrC = 37;
		m_nDcdThrD = 25;
		m_nDefThrA = 15;
		m_nDefThrB = 20;
		m_nDefThrC = 25;
		m_nDefThrD = 30;
		m_nMinrThrA = 50;
		m_nMinrThrB = 50;
		m_nMinrThrC = 50;
		m_nMinrThrD = 50;
		m_nMineThrA = 15;
		m_nMineThrB = 15;
		m_nMineThrC = 15;
		m_nMineThrD = 15;
		m_nModThrA = 70;
		m_nModThrB = 60;
		m_nModThrC = 50;
		m_nModThrD = 40;
		m_nScThrA = 70;
		m_nScThrB = 55;
		m_nScThrC = 40;
		m_nScThrD = 20;
		m_bApertureEnable = false;
		m_nAperture = 3;
	}
};

//一维码质量评级结果(按照ISO-15416评级)
struct Code1DVerifyResult
{
	bool m_bVerifyResultValid;						// 评级结果是否有效
	unsigned char m_nOverQuality;					// 总体质量等级
	unsigned char m_nDecodeGrade;					// 译码等级
	unsigned char m_nEdgDeterminGrade;				// 边缘确定度质量等级
	unsigned char m_nSymbolContrastGrade;			// 对比度质量等级
	unsigned char m_nMinmumReflectGrade;			// 最小反射率质量等级
	unsigned char m_nMinEdgeContrastGrade;			// 最小边缘对比度质量等级
	unsigned char m_nModulationGrade;				// 模块均匀性等级
	unsigned char m_nDecodabilityGrade;				// 可译码性质量等级
	unsigned char m_nDefectsGrade;					// 缺陷质量等级
	unsigned char m_nQuietZoneGrade;				// 静区质量等级
	float m_fEdgDeterminScore;						// 边缘确定度分数
	float m_fSymbolContrastScore;					// 对比度质量评分
	float m_fMinmumReflectScore;					// 最小反射率分数
	float m_fMinEdgeContrastScore;					// 最小边缘对比度分数
	float m_fModulationScore;						// 模块均匀性分数
	float m_fDecodabilityScore;						// 可译码性分数
	float m_fDefectsScore;							// 缺陷分数
	float m_fQuietZoneScore;						// 静区分数

	Code1DVerifyResult()
	{
		m_bVerifyResultValid = false;
		m_nOverQuality = 0;
		m_nDecodeGrade = 0;
		m_nEdgDeterminGrade = 0;
		m_nSymbolContrastGrade = 0;
		m_nMinmumReflectGrade = 0;
		m_nMinEdgeContrastGrade = 0;
		m_nModulationGrade = 0;
		m_nDecodabilityGrade = 0;
		m_nDefectsGrade = 0;
		m_nQuietZoneGrade = 0;
		m_fEdgDeterminScore = 0.0;
		m_fSymbolContrastScore = 0.0;
		m_fMinmumReflectScore = 0.0;
		m_fMinEdgeContrastScore = 0.0;
		m_fModulationScore = 0.0;
		m_fDecodabilityScore = 0.0;
		m_fDefectsScore = 0.0;
		m_fQuietZoneScore = 0.0;
	}
};

//二维码质量评级参数
struct Code2DVerifyPara
{
	VerifyStandard m_verifyStandard;
	VerifyLabel m_verifyLabel;
	VerifyProcessType m_verifyProcessType;
	PolarityMode m_modePolarity;
	int m_nSymbolCols;
	int m_nSymbolRows;
	int m_nMinCellSize;
	int m_nAperture;
	MirrorMode m_modeMirror;
	int m_nStdAperture;
	int m_nSysMag;
	DiscreteFlag m_edgeFeature;

	Code2DVerifyPara()
	{
		m_verifyStandard = eVerifyISOStandard_15415;
		m_verifyLabel = eVerifyLabel_Standard;
		m_verifyProcessType = eVerifyProcessType_I;
		m_modePolarity = ePolarityMode_BlackOnWhite;
		m_nSymbolCols = 16;
		m_nSymbolRows = 16;
		m_nMinCellSize = 1;
		m_nAperture = 10;
		m_modeMirror = eMirrorMode_Close;
		m_nStdAperture = 400;
		m_nSysMag = 400;
		m_edgeFeature = eDiscreteFlag_Continuous;
	}
};

//二维码质量评级结果
struct Code2DVerifyResult
{
	bool m_bVerifyResultValid;								// 评级结果是否有效
	unsigned char m_nOverQuality;							// 总体质量等级
	unsigned char m_nDecodeGrade;							// 译码等级
	unsigned char m_nSymbolContrastGrade;					// 对比度质量等级
	unsigned char m_nModulationGrade;						// 模块均匀性等级
	unsigned char m_nFixedPatternDamageGrade;				// 固定模式损坏等级
	unsigned char m_nAxialNonuniformityGrade;				// 码轴规整性等级
	unsigned char m_nGridNonuniformityGrade;				// 基础网格均匀性质量等级
	unsigned char m_nUnusedErrorCorrectionGrade;			// 未使用纠错功能等级
	unsigned char m_nPrintGrowthHorizontalGrade;			// 打印伸缩（水平）等级
	unsigned char m_nPrintGrowthVerticalGrade;				// 打印伸缩（垂直）等级
	unsigned char m_nReflectanceMarginGrade;				// 反射率余量等级
	float m_fSymbolContrastScore;							// 对比度质量评分
	float m_fModulationScore;								// 模块均匀性评分
	float m_fFixedPatternDamageScore;						// 固定模式损坏评分
	float m_fAxialNonuniformityScore;						// 码轴规整性评分
	float m_fGridNonuniformityScore;						// 基础网格均匀性质量评分
	float m_fUnusedErrorCorrectionScore;					// 未使用纠错功能评分
	float m_fPrintGrowthHorizontalScore;					// 打印伸缩（水平）评分
	float m_fPrintGrowthVerticalScore;						// 打印伸缩（垂直）评分
	float m_fReflectanceMarginScore;						// 反射率余量评分

	Code2DVerifyResult()
	{
		m_bVerifyResultValid = false;
		m_nOverQuality = 0;
		m_nDecodeGrade = 0;
		m_nSymbolContrastGrade = 0;
		m_nModulationGrade = 0;
		m_nFixedPatternDamageGrade = 0;
		m_nAxialNonuniformityGrade = 0;
		m_nGridNonuniformityGrade = 0;
		m_nUnusedErrorCorrectionGrade = 0;
		m_nPrintGrowthHorizontalGrade = 0;
		m_nPrintGrowthVerticalGrade = 0;
		m_nReflectanceMarginGrade = 0;
		m_fSymbolContrastScore = 0.0;
		m_fModulationScore = 0.0;
		m_fFixedPatternDamageScore = 0.0;
		m_fAxialNonuniformityScore = 0.0;
		m_fGridNonuniformityScore = 0.0;
		m_fUnusedErrorCorrectionScore = 0.0;
		m_fPrintGrowthHorizontalScore = 0.0;
		m_fPrintGrowthVerticalScore = 0.0;
		m_fReflectanceMarginScore = 0.0;
	}
};