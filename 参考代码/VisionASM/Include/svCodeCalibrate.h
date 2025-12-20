#pragma once

#include "svDef.h"
#include "svImage.h"
#include "vpImage.h"
#include "svVector.h"
#include "svPolyline.h"
#include "svHomography.h"
#include <vector>

#include "svCodeCalibrateDef.h"
#include "svCodeDM.h"

#pragma pack(push,8)

#define MAX_CODE_LENGTH	512

//棋盘格角点提取参数
struct FindCornerPara
{
	double m_dGridSize;
	double m_dGridThre;
	bool m_bOutputOutPoints;

	FindCornerPara()
	{
		m_dGridSize = 1;
		m_dGridThre = 16;
		m_bOutputOutPoints = false;
	}
};

//二维码结果数据结构
struct Code2DResult
{
	char		m_CodeString[MAX_CODE_LENGTH];		//字符串
	sc2Vector	m_CodeCenterPixelCoordinate;		//中心像素坐标
	sc2Vector	m_CodeCenterPhysicalCoordinate;		//中心物理坐标
	scPolyline	m_CodePolyline;						//区域

	Code2DResult()
	{
		memset(m_CodeString, 0, MAX_CODE_LENGTH*sizeof(char));
		m_CodePolyline.IsClosed(true);
	}
};

class SVCODE_API scDMCodeCalibrate
{
public:
	scDMCodeCalibrate();
	~scDMCodeCalibrate();

	//老版接口，为保证兼容性而存在
	void SetFindCornerPara(double dGridSize, double dGridThre, bool bOutputOutPoints);
	void SetFindCodePara(int nDMCodeMaxNum, int nMaxDecodeTime, bool bDecodeImageMirror, bool bEnableReDecode);

	//新版接口
	void SetFindCornerPara(FindCornerPara para);
	FindCornerPara GetFindCornerPara();

	void SetFindDMCodePara(FindDMCodePara para);
	FindDMCodePara GetFindDMCodePara();

	void EnableReDecode(bool bEnable);
	bool IsEnableReDecode();

	bool Execute(const scImage8& image, scRect roiCalibrate, scRect roiDecode, svStd vector<sc2Vector>& arrayImgCorners, svStd vector<sc2Vector>& arrayObjCorners, svStd vector<Code2DResult>& arrayDMCodes, int& nErrorCode);
	bool Execute(cpImage& image, scRect roiCalibrate, scRect roiDecode, svStd vector<sc2Vector>& arrayImgCorners, svStd vector<sc2Vector>& arrayObjCorners, svStd vector<Code2DResult>& arrayDMCodes, int& nErrorCode);

protected:
	FindCornerPara m_paraFindCorner;// 棋盘格角点提取参数
	FindDMCodePara m_paraFindDMCode;// DM解码参数
	bool	m_bEnableReDecode;		// 重新检测二维码

	scDMCodeTool m_toolDMCode;		// DM解码工具
};

// 计算单应矩阵
SVCODE_API bool sfGetHomography(const svStd vector<sc2Vector>& arrayImgCorners_from, const svStd vector<sc2Vector>& arrayObjCorners_from, 
	const svStd vector<sc2Vector>& arrayImgCorners_to, const svStd vector<sc2Vector>& arrayObjCorners_to,
	double h[9], svStd vector<sc2Vector>& arrayImgCorners_from_result, svStd vector<sc2Vector>& arrayImgCorners_to_result, 
	HomographyDOF dof = eAuto);

// 计算单应矩阵(局外点)，不支持dof = eAuto
SVCODE_API bool sfGetHomographyOutLine(const svStd vector<sc2Vector>& arrayImgCorners_from, const svStd vector<sc2Vector>& arrayObjCorners_from, 
	const svStd vector<sc2Vector>& arrayImgCorners_to, const svStd vector<sc2Vector>& arrayObjCorners_to,
	double h[9], int outcount, svStd vector<bool>& used, svStd vector<sc2Vector>& arrayImgCorners_from_result, svStd vector<sc2Vector>& arrayImgCorners_to_result, 
	HomographyDOF dof);

#pragma pack(pop)
