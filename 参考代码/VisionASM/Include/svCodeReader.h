#pragma once

#include "svDef.h"
#include "svCodeCalibrateDef.h"
#include "vpImage.h"
#include "svVector.h"
#include <vector>
#include "svCodeVerifyDef.h"

#pragma pack(push,8)

#define MAX_CODE_LENGTH	512

struct FindCodePara
{
	//全局采样参数
	int m_nSampleLevelGlobal;

	ProcessingPlatformType m_TypePlatform;
	int m_nWaittimeOut;
	bool m_bPerfMode;

	// 一维码
	bool m_bCode39Flag;
	bool m_bCode128Flag;
	bool m_bCodaBarFlag;
	bool m_bCodeEAN8Flag;
	bool m_bCodeITF25Flag;
	bool m_bCode93Flag;
	bool m_bCodeUPCEFlag;
	bool m_bCodeUPCAFlag;
	bool m_bCodeEAN13Flag;
	bool m_bCodeITF14Flag;
	bool m_bCodeMATRIX25Flag;
	bool m_bCodeMSIFlag;
	bool m_bCodeCODE11Flag;
	bool m_bCodeINDUSTRIAL25Flag;
	bool m_bCodeCHINAPOSTFlag;

	int m_n1DCodeNum;
	
	// 二维码
	bool m_bDMFlag;
	bool m_bQRFlag;
	bool m_bECC140Flag;

	int m_n2DCodeNum;

	FindCodePara()
	{
		m_nSampleLevelGlobal = 0;
		m_TypePlatform = ePlatform_CPU;
		m_nWaittimeOut = 5000;
		m_bPerfMode = false;

		// 一维码
		m_bCode39Flag = true;
		m_bCode128Flag = true;
		m_bCodaBarFlag = true;
		m_bCodeEAN8Flag = true;
		m_bCodeITF25Flag = true;
		m_bCode93Flag = true;
		m_bCodeUPCEFlag = true;
		m_bCodeUPCAFlag = true;
		m_bCodeEAN13Flag = true;
		m_bCodeITF14Flag = true;
		m_bCodeMATRIX25Flag = true;
		m_bCodeMSIFlag = true;
		m_bCodeCODE11Flag = true;
		m_bCodeINDUSTRIAL25Flag = true;
		m_bCodeCHINAPOSTFlag = true;

		m_n1DCodeNum = 5;

		// 二维码
		m_bDMFlag = true;
		m_bQRFlag = true;
		m_bECC140Flag = true;

		m_n2DCodeNum = 5;
	}
	
};

struct CodeResult
{
	sc2Vector m_pt[4];	//位置

	char m_OutputData[MAX_CODE_LENGTH];	//内容

	CodeType m_typeCode;	//类型

	double m_dAngle;		//角度

	Code1DVerifyResult m_Code1DVerifyResult;

	Code2DVerifyResult m_Code2DVerifyResult;

	CodeResult()
	{
		m_pt[0] = sc2Vector(0, 0);
		m_pt[1] = sc2Vector(1, 0);
		m_pt[2] = sc2Vector(1, 1);
		m_pt[3] = sc2Vector(0, 1);

		memset(m_OutputData, 0, MAX_CODE_LENGTH * sizeof(char));
	}

	CodeResult& operator=(const CodeResult& ret)
	{
		if (this != &ret)
		{
			m_pt[0] = ret.m_pt[0];
			m_pt[1] = ret.m_pt[1];
			m_pt[2] = ret.m_pt[2];
			m_pt[3] = ret.m_pt[3];

			memcpy(m_OutputData, ret.m_OutputData, MAX_CODE_LENGTH * sizeof(char));

			m_typeCode = ret.m_typeCode;

			m_dAngle = ret.m_dAngle;

			m_Code1DVerifyResult = ret.m_Code1DVerifyResult;
			m_Code2DVerifyResult = ret.m_Code2DVerifyResult;
		}

		return *this;
	}
};


// 只在x64编译下使用
class SVCODE_API scCodeReaderTool
{
public:
	scCodeReaderTool();
	~scCodeReaderTool();

	bool SetFindCodePara(FindCodePara para);
	FindCodePara GetFindCodePara();

	void Set1DCodeVerifyEnable(bool bEnable);
	bool Get1DCodeVerifyEnable();

	bool Set1DCodeVerifyPara(Code1DVerifyPara para);
	Code1DVerifyPara Get1DCodeVerifyPara();

	void Set2DCodeVerifyEnable(bool bEnable);
	bool Get2DCodeVerifyEnable();

	bool Set2DCodeVerifyPara(Code2DVerifyPara para);
	Code2DVerifyPara Get2DCodeVerifyPara();

	bool Execute(const scImage8& image, const scRect& roi, svStd vector<CodeResult>& resultarray);
	bool Execute(cpImage& image, const scRect& roi, svStd vector<CodeResult>& resultarray);

	// 按行按列排序
	bool SortByCoordXY(svStd vector<CodeResult>& resultarray, double dGap = 64, CoordSortMethod method = eCoordYFirst, SortOrder orderX = eAscending, SortOrder orderY = eAscending, BOOL bImageCoord = TRUE);

protected:
	FindCodePara m_para;
	Code1DVerifyPara m_1DCodeVerifyPara;
	Code2DVerifyPara m_2DCodeVerifyPara;
	bool m_b1DCodeVerifyEnable;
	bool m_b2DCodeVerifyEnable;
	void* m_pCodeReaderTool;
	void* m_pCodeVerifyTool;

};

