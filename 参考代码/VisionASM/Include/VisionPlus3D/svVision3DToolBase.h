#pragma once

#include "svVision3DDef.h"
#include "vpImage.h"

#include "svZeroPlaneResult.h"
#include "vpPointCloudGenerateToolBase.h"

#pragma pack(push,8)

class scHeightTransformBase;

// 结果补正
typedef struct tagResultCorrection
{
	double m_dCoefficient; // 系数
	double m_dOffset;      // 偏移

	tagResultCorrection()
	{
		m_dCoefficient = 1.0;
		m_dOffset      = 0.0;
	}
}ResultCorrection,*PResultCorrection;

// 结果判断上下限组合
typedef struct tagJudgeCombine
{
	bool m_bHighLimitEnable; // 是否启用上限判定
	bool m_bLowLimitEnable;  // 是否启用下限判定

	double m_dHighLimit;     // 上限值
	double m_dLowLimit;      // 下限值

	tagJudgeCombine()
	{
		m_bHighLimitEnable = false;
		m_bLowLimitEnable  = false;

		m_dHighLimit = 0.0;
		m_dLowLimit  = 0.0;
	}
}JudgeCombine,*PJudgeCombine;

class SV_VISION_3D_API scVision3DToolBase
{
public:
	scVision3DToolBase();
	virtual ~scVision3DToolBase();

	virtual bool Excute(cpImage& image){return false;} // 执行检测
	virtual bool GetZeroPlane(scZeroPlaneResult& zeroPlane)const {return false;}      // 获取起算面
	virtual bool GetInspectPlane(scZeroPlaneResult& zeroPlane)const {return false;}   // 获取检测平面
	virtual bool GetExtractPlane(scZeroPlaneResult& zeroPlane)const {return false;}   // 获取抽取平面

	// 设置/获取点云生成类
	void SetPointCloudGenerateTool(cpPointCloudGenerateToolBase* pPointCloudGenerateTool);
	cpPointCloudGenerateToolBase* GetPointCloudGenerateTool()const;

	static ResultCorrection GetResultCorrection(double before,double after);  // 单点补正
	static ResultCorrection GetResultCorrection(double beforeOne,double afterOne,double beforeTwo,double afterTwo); // 两点补正
	static double GetResultCorrectionValue(double dBefor,const ResultCorrection& correction); // 获取补正后的值

	static bool JudgeInspectValue(double dInspectValue,const JudgeCombine& judge); // 判断检测值

protected:
	cpPointCloudGenerateToolBase* m_pPointCloudGenerateTool;
};

#pragma pack(pop)
