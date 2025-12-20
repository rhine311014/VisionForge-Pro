#pragma once

#include "svVision3DToolBase.h"

#pragma pack(push,8)

// 检测结果
typedef struct tagHeightInspectResult
{
	double m_dMaxHeightX;        // 最大高度X坐标
	double m_dMaxHeightY;        // 最大高度Y坐标
	double m_dMaxHeightZ;        // 最大高度Z坐标
	double m_dMaxHeightH;        // 最大高度H
	double m_dMinHeightX;        // 最小高度X坐标
	double m_dMinHeightY;        // 最小高度Y坐标
	double m_dMinHeightZ;        // 最小高度Z坐标
	double m_dMinHeightH;        // 最小高度H
	double m_dAveHeightZ;        // 平均高度Z坐标
	double m_dAveHeightH;        // 平均高度H
	double m_dPeakToPeakHeight;  // 峰间值高度
	long   m_nValidPixelCount;   // 有效像素个数
	double m_dConcaveArea;       // 凹面积
	double m_dConvexArea;        // 凸面积
	double m_dConcaveVolume;     // 凹体积
	double m_dConvexVolume;      // 凸体积
	double m_dFlatness;			 // 平面度
	double m_dFlatnessX;		 // 平面度X坐标
	double m_dFlatnessY;		 // 平面度Y坐标
	double m_dFlatnessZ;		 // 平面度Z坐标
	double m_dMaxHeightRelativeX; // 相对最大高度X坐标
	double m_dMaxHeightRelativeY; // 相对最大高度Y坐标
	double m_dMaxHeightRelativeZ; // 相对最大高度Z坐标
	double m_dMaxHeightRelativeH; // 相对最大高度H
	double m_dMinHeightRelativeX; // 相对最小高度X坐标
	double m_dMinHeightRelativeY; // 相对最小高度Y坐标
	double m_dMinHeightRelativeZ; // 相对最小高度Z坐标
	double m_dMinHeightRelativeH; // 相对最小高度H
	double m_dPercentRelativeMaxHeight;	// 相对最大高度百分比
	double m_dPercentRelativeMinHeight;	// 相对最小高度百分比
	double m_dMaxAveHeightPercentZ;  // 百分比最大平均高度Z
	double m_dMaxAveHeightPercentH;  // 百分比最大平均高度H
	double m_dMinAveHeightPercentZ;  // 百分比最小平均高度Z
	double m_dMinAveHeightPercentH;  // 百分比最小平均高度H

	scZeroPlaneResult m_ZeroInspectPlane; // 检测平面

	tagHeightInspectResult()
	{
		m_dMaxHeightX = 0.0;
		m_dMaxHeightY = 0.0;
		m_dMaxHeightZ = 0.0;
		m_dMaxHeightH = 0.0;
		m_dMinHeightX = 0.0;
		m_dMinHeightY = 0.0;
		m_dMinHeightZ = 0.0;
		m_dMinHeightH = 0.0;
		m_dAveHeightZ = 0.0;
		m_dAveHeightH = 0.0;
		m_dPeakToPeakHeight = 0.0;
		m_nValidPixelCount = 0;
		m_dConcaveArea = 0.0;
		m_dConvexArea = 0.0;
		m_dConcaveVolume = 0.0;
		m_dConvexVolume = 0.0;
		m_dFlatness = 0.0;
		m_dFlatnessX = 0.0;
		m_dFlatnessY = 0.0;
		m_dFlatnessZ = 0.0;
		m_dMaxHeightRelativeX = 0.0;
		m_dMaxHeightRelativeY = 0.0;
		m_dMaxHeightRelativeZ = 0.0;
		m_dMaxHeightRelativeH = 0.0;
		m_dMinHeightRelativeX = 0.0;
		m_dMinHeightRelativeY = 0.0;
		m_dMinHeightRelativeZ = 0.0;
		m_dMinHeightRelativeH = 0.0;
		m_dPercentRelativeMaxHeight = 0.0;
		m_dPercentRelativeMinHeight = 0.0;
		m_dMaxAveHeightPercentZ = 0.0;
		m_dMaxAveHeightPercentH = 0.0;
		m_dMinAveHeightPercentZ = 0.0;
		m_dMinAveHeightPercentH = 0.0;
	}

}HeightInspectResult,*PHeightInspectResult;

class SV_VISION_3D_API scVision3DHeightInspectTool : public scVision3DToolBase
{
public:
	scVision3DHeightInspectTool();
	virtual ~scVision3DHeightInspectTool();

	void AddInspectRegion(scShape* shape);            // 添加检测范围
	void RemoveInspectRegion(int nIndex);             // 移除检测范围
	void RemoveInspectRegionAll();                    // 移除所有检测范围
	void SetInspectRegion(scShape* shape,int nIndex); // 设置检测范围
	scShape* GetInspectRegion(int nIndex) const;      // 获取检测范围

	void AddMaskRegion(scShape* shape);               // 添加屏蔽范围
	void RemoveMaskRegion(int nIndex);                // 移除屏蔽范围
	void RemoveMaskRegionAll();                       // 移除所有屏蔽范围
	void SetMaskRegion(scShape* shape,int nIndex);    // 设置屏蔽范围
	scShape* GetMaskRegion(int nIndex) const;         // 获取屏蔽范围   

	// 设置/获取启用图像掩模
	void EnableImageRegionGenertor(bool bEnable); 
	bool IsEnableImageRegionGenertor() const;

	// 设置/获取图像范围工具检测目标
	void SetImageRegionGenertorInspectTarget(int nMode);
	int GetImageRegionGenertorInspectTarget() const;

	// 设置/获取图像范围图像
	void SetRegionGenertorImage(cpImage& image);
	const cpImage& GetRegionGenertorImage() const;

	// 设置/获取起算面
	void SetZeroPlane(scZeroPlaneResult& zeroPlane);
	scZeroPlaneResult GetZeroPlane() const;

	// 设置/获取去除干扰(mm)
	void SetNoiseCut(double dNoiseCut);
	double GetNoiseCut()const;

	// 设置/获取Z偏移(mm)
	void SetOffsetZ(double dOffsetZ);
	double GetOffsetZ()const;

	// 设置/获取相对最大检测高度百分比
	void SetPercentRelativeMaxHeight(double dPercent);
	double GetPercentRelativeMaxHeight()const;

	// 设置/获取相对最小检测高度百分比
	void SetPercentRelativeMinHeight(double dPercent);
	double GetPercentRelativeMinHeight()const;

	// 设置/获取百分比最大平均高度百分比
	void SetMaxAveHeightPercent(double dPercent);
	double GetMaxAveHeightPercent()const;

	// 设置/获取百分比最小平均高度百分比
	void SetMinAveHeightPercent(double dPercent);
	double GetMinAveHeightPercent()const;

	// 执行检测
	virtual bool Excute(cpImage& image);

	virtual bool GetZeroPlane(scZeroPlaneResult& zeroPlane)const;     // 获取起算面
	virtual bool GetInspectPlane(scZeroPlaneResult& zeroPlane)const;  // 获取检测平面

	// 设置、获取是否检测平面
	void EnableInpectPlane(bool bEnable);
	bool IsEnableInspectPlane()const;

	// 设置、获取是否检测最大高度
	void EnableInspectMaxHeight(bool bEnable) {m_bInspectMaxHeight = bEnable;}
	bool IsEnableInspectMaxHeight()const {return m_bInspectMaxHeight;}

	// 设置、获取是否检测最小高度
	void EnableInspectMinHeight(bool bEnable) {m_bInspectMinHeight = bEnable;}
	bool IsEnableInspectMinHeight()const {return m_bInspectMinHeight;}

	// 设置、获取是否检测平均高度
	void EnableInspectAveHeight(bool bEnable) {m_bInspectAveHeight = bEnable;}
	bool IsEnableInspectAveHeight()const {return m_bInspectAveHeight;}

	// 设置、获取是否检测峰间值高度
	void EnableInspectPeakToPeakHeight(bool bEnable) {m_bInspectPeakToPeak = bEnable;}
	bool IsEnableInspectPeakToPeakHeight()const {return m_bInspectPeakToPeak;}

	// 设置、获取是否检测有效像素
	void EnableInspectValidPixel(bool bEnable) {m_bInspectValidPixel = bEnable;}
	bool IsEnableInsectValidPixel()const {return m_bInspectValidPixel;}

	// 设置、获取是否检测面积
	void EnableInspectArea(bool bEnable) {m_bInspectArea = bEnable;}
	bool IsEnableInspectArea()const {return m_bInspectArea;}

	// 设置、获取是否检测体积
	void EnableInspectVolume(bool bEnable) {m_bInspectVolume = bEnable;}
	bool IsEnableInspectVolume()const {return m_bInspectVolume;}

	// 设置、获取是否检测平面度
	void EnableInspectFlatness(bool bEnable) {m_bInspectFlatness = bEnable;}
	bool IsEnableInspectFlatness()const {return m_bInspectFlatness;}

	// 设置、获取是否检测相对最小高度
	void EnableInspectRelativeMinHeight(bool bEnable) {m_bInspectRelativeMinHeight = bEnable;}
	bool IsEnableInspectRelativeMinHeight()const {return m_bInspectRelativeMinHeight;}

	// 设置、获取是否检测相对最大高度
	void EnableInspectRelativeMaxHeight(bool bEnable) {m_bInspectRelativeMaxHeight = bEnable;}
	bool IsEnableInspectRelativeMaxHeight()const {return m_bInspectRelativeMaxHeight;}

	// 设置、获取是否检测百分比最大平均高度
	void EnableInspectPercentMaxAveHeight(bool bEnable) {m_bInspectPercentMaxAveHeight = bEnable;}
	bool IsEnableInspectPercentMaxAveHeight()const {return m_bInspectPercentMaxAveHeight;}

	// 设置、获取是否检测百分比最小平均高度
	void EnableInspectPercentMinAveHeight(bool bEnable) {m_bInspectPercentMinAveHeight = bEnable;}
	bool IsEnableInspectPercentMinAveHeight()const {return m_bInspectPercentMinAveHeight;}

	// 设置、获取平面度取点方式
	void SetAvePointsCalFlatness(bool bAve) {m_bAvePointsCalFlatness = bAve;}
	bool GetAvePointsCalFlatness()const {return m_bAvePointsCalFlatness;}

	// 设置、获取最大高度X坐标补正
	void SetCorrectionMaxHeightX(ResultCorrection& correction);
	ResultCorrection GetCorrectionMaxHeightX()const;

	// 设置、获取最大高度Y坐标补正
	void SetCorrectionMaxHeightY(ResultCorrection& correction);
	ResultCorrection GetCorrectionMaxHeightY()const;

	// 设置、获取最大高度Z坐标补正
	void SetCorrectionMaxHeightZ(ResultCorrection& correction);
	ResultCorrection GetCorrectionMaxHeightZ()const;

	// 设置、获取最大高度H坐标补正
	void SetCorrectionMaxHeightH(ResultCorrection& correction);
	ResultCorrection GetCorrectionMaxHeightH()const;

	// 设置、获取最小高度X坐标补正
	void SetCorrectionMinHeightX(ResultCorrection& correction);
	ResultCorrection GetCorrectionMinHeightX()const;

	// 设置、获取最小高度Y坐标补正
	void SetCorrectionMinHeightY(ResultCorrection& correction);
	ResultCorrection GetCorrectionMinHeightY()const;

	// 设置、获取最小高度Z坐标补正
	void SetCorrectionMinHeightZ(ResultCorrection& correction);
	ResultCorrection GetCorrectionMinHeightZ()const;

	// 设置、获取最小高度H坐标补正
	void SetCorrectionMinHeightH(ResultCorrection& correction);
	ResultCorrection GetCorrectionMinHeightH()const;

	// 设置、获取平均高度Z坐标补正
	void SetCorrectionAveHeightZ(ResultCorrection& correction);
	ResultCorrection GetCorrectionAveHeightZ()const;

	// 设置、获取平均高度H坐标补正
	void SetCorrectionAveHeightH(ResultCorrection& correction);
	ResultCorrection GetCorrectionAveHeightH()const;

	// 设置、获取峰间值高度补正
	void SetCorrectionPeakToPeakHeight(ResultCorrection& correction);
	ResultCorrection GetCorrectionPeakToPeakHeight()const;

	// 设置、获取凹面积补正
	void SetCorrectionConcaveArea(ResultCorrection& correction);
	ResultCorrection GetCorrectionConcaveArea()const;

	// 设置、获取凸面积补正
	void SetCorrectionConvexArea(ResultCorrection& correction);
	ResultCorrection GetCorrectionConvexArea()const;

	// 设置、获取凹体积补正
	void SetCorrectionConcaveVolume(ResultCorrection& correction);
	ResultCorrection GetCorrectionConcaveVolume()const;

	// 设置、获取凸体积补正
	void SetCorrectionConvexVolume(ResultCorrection& correction);
	ResultCorrection GetCorrectionConvexVolume()const;

	// 设置、获取平面度补正
	void SetCorrectionFlatness(ResultCorrection& correction);
	ResultCorrection GetCorrectionFlatness()const;

	// 设置、获取平面度X坐标补正
	void SetCorrectionFlatnessX(ResultCorrection& correction);
	ResultCorrection GetCorrectionFlatnessX()const;

	// 设置、获取平面度Y坐标补正
	void SetCorrectionFlatnessY(ResultCorrection& correction);
	ResultCorrection GetCorrectionFlatnessY()const;

	// 设置、获取平面度Z坐标补正
	void SetCorrectionFlatnessZ(ResultCorrection& correction);
	ResultCorrection GetCorrectionFlatnessZ()const;

	// 设置、获取相对最大高度X坐标补正
	void SetCorrectionMaxHeightRelativeX(ResultCorrection& correction);
	ResultCorrection GetCorrectionMaxHeightRelativeX()const;

	// 设置、获取相对最大高度Y坐标补正
	void SetCorrectionMaxHeightRelativeY(ResultCorrection& correction);
	ResultCorrection GetCorrectionMaxHeightRelativeY()const;

	// 设置、获取相对最大高度Z坐标补正
	void SetCorrectionMaxHeightRelativeZ(ResultCorrection& correction);
	ResultCorrection GetCorrectionMaxHeightRelativeZ()const;

	// 设置、获取相对最大高度H坐标补正
	void SetCorrectionMaxHeightRelativeH(ResultCorrection& correction);
	ResultCorrection GetCorrectionMaxHeightRelativeH()const;

	// 设置、获取相对最小高度X坐标补正
	void SetCorrectionMinHeightRelativeX(ResultCorrection& correction);
	ResultCorrection GetCorrectionMinHeightRelativeX()const;

	// 设置、获取相对最小高度Y坐标补正
	void SetCorrectionMinHeightRelativeY(ResultCorrection& correction);
	ResultCorrection GetCorrectionMinHeightRelativeY()const;

	// 设置、获取相对最小高度Z坐标补正
	void SetCorrectionMinHeightRelativeZ(ResultCorrection& correction);
	ResultCorrection GetCorrectionMinHeightRelativeZ()const;

	// 设置、获取相对最小高度H坐标补正
	void SetCorrectionMinHeightRelativeH(ResultCorrection& correction);
	ResultCorrection GetCorrectionMinHeightRelativeH()const;

	// 设置、获取百分比最大平均高度Z补正
	void SetCorrectionPercentMaxHeightZ(ResultCorrection& correction);
	ResultCorrection GetCorrectionPercentMaxHeightZ()const;

	// 设置、获取百分比最大平均高度H补正
	void SetCorrectionPercentMaxHeightH(ResultCorrection& correction);
	ResultCorrection GetCorrectionPercentMaxHeightH()const;

	// 设置、获取百分比最小平均高度Z补正
	void SetCorrectionPercentMinHeightZ(ResultCorrection& correction);
	ResultCorrection GetCorrectionPercentMinHeightZ()const;

	// 设置、获取百分比最小平均高度H补正
	void SetCorrectionPercentMinHeightH(ResultCorrection& correction);
	ResultCorrection GetCorrectionPercentMinHeightH()const;

	// 设置、获取最大高度X坐标判定条件
	void SetJudgeMaxHeightX(JudgeCombine& judge);
	JudgeCombine GetJudgeMaxHeightX()const;

	// 设置、获取最大高度Y坐标判定条件
	void SetJudgeMaxHeightY(JudgeCombine& judge);
	JudgeCombine GetJudgeMaxHeightY()const;

	// 设置、获取最大高度Z坐标判定条件
	void SetJudgeMaxHeightZ(JudgeCombine& judge);
	JudgeCombine GetJudgeMaxHeightZ()const;

	// 设置、获取最大高度H坐标判定条件
	void SetJudgeMaxHeightH(JudgeCombine& judge);
	JudgeCombine GetJudgeMaxHeightH()const;

	// 设置、获取最小高度X坐标判定条件
	void SetJudgeMinHeightX(JudgeCombine& judge);
	JudgeCombine GetJudgeMinHeightX()const;

	// 设置、获取最小高度Y坐标判定条件
	void SetJudgeMinHeightY(JudgeCombine& judge);
	JudgeCombine GetJudgeMinHeightY()const;

	// 设置、获取最小高度Z坐标判定条件
	void SetJudgeMinHeightZ(JudgeCombine& judge);
	JudgeCombine GetJudgeMinHeightZ()const;

	// 设置、获取最小高度H坐标判定条件
	void SetJudgeMinHeightH(JudgeCombine& judge);
	JudgeCombine GetJudgeMinHeightH()const;

	// 设置、获取平均高度Z坐标判定条件
	void SetJudgeAveHeightZ(JudgeCombine& judge);
	JudgeCombine GetJudgeAveHeightZ()const;

	// 设置、获取平均高度H坐标判定条件
	void SetJudgeAveHeightH(JudgeCombine& judge);
	JudgeCombine GetJudgeAveHeightH()const;

	// 设置、获取峰间值高度判定条件
	void SetJudgePeakToPeakHeight(JudgeCombine& judge);
	JudgeCombine GetJudgePeakToPeakHeight()const;

	// 设置、获取有效像素判定条件
	void SetJudgeValidPixel(JudgeCombine& judge);
	JudgeCombine GetJudgeValidPixel()const;

	// 设置、获取凹面积判定条件
	void SetJudgeConcaveArea(JudgeCombine& judge);
	JudgeCombine GetJudgeConcaveArea()const;

	// 设置、获取凸面积判定条件
	void SetJudgeConvexArea(JudgeCombine& judge);
	JudgeCombine GetJudgeConvexArea()const;

	// 设置、获取凹体积判定条件
	void SetJudgeConcaveVolume(JudgeCombine& judge);
	JudgeCombine GetJudgeConcaveVolume()const;

	// 设置、获取凸体积判定条件
	void SetJudgeConvexVolume(JudgeCombine& judge);
	JudgeCombine GetJudgeConvexVolume()const;

	// 设置、获取平面度判定条件
	void SetJudgeFlatness(JudgeCombine& judge);
	JudgeCombine GetJudgeFlatness()const;

	// 设置/获取平面度X坐标判定条件
	void SetJudgeFlatnessX(JudgeCombine& judge);
	JudgeCombine GetJudgeFlatnessX()const;

	// 设置/获取平面度Y坐标判定条件
	void SetJudgeFlatnessY(JudgeCombine& judge);
	JudgeCombine GetJudgeFlatnessY()const;

	// 设置/获取平面度Z坐标判定条件
	void SetJudgeFlatnessZ(JudgeCombine& judge);
	JudgeCombine GetJudgeFlatnessZ()const;

	// 设置、获取相对最大高度X坐标判定条件
	void SetJudgeMaxHeightRelativeX(JudgeCombine& judge);
	JudgeCombine GetJudgeMaxHeightRelativeX()const;

	// 设置、获取相对最大高度Y坐标判定条件
	void SetJudgeMaxHeightRelativeY(JudgeCombine& judge);
	JudgeCombine GetJudgeMaxHeightRelativeY()const;

	// 设置、获取相对最大高度Z坐标判定条件
	void SetJudgeMaxHeightRelativeZ(JudgeCombine& judge);
	JudgeCombine GetJudgeMaxHeightRelativeZ()const;

	// 设置、获取相对最大高度H坐标判定条件
	void SetJudgeMaxHeightRelativeH(JudgeCombine& judge);
	JudgeCombine GetJudgeMaxHeightRelativeH()const;

	// 设置、获取相对最小高度X坐标判定条件
	void SetJudgeMinHeightRelativeX(JudgeCombine& judge);
	JudgeCombine GetJudgeMinHeightRelativeX()const;

	// 设置、获取相对最小高度Y坐标判定条件
	void SetJudgeMinHeightRelativeY(JudgeCombine& judge);
	JudgeCombine GetJudgeMinHeightRelativeY()const;

	// 设置、获取相对最小高度Z坐标判定条件
	void SetJudgeMinHeightRelativeZ(JudgeCombine& judge);
	JudgeCombine GetJudgeMinHeightRelativeZ()const;

	// 设置、获取相对最小高度H坐标判定条件
	void SetJudgeMinHeightRelativeH(JudgeCombine& judge);
	JudgeCombine GetJudgeMinHeightRelativeH()const;

	// 设置、获取百分比最大平均高度Z判定条件
	void SetJudgePercentMaxAveHeightZ(JudgeCombine& judge);
	JudgeCombine GetJudgePercentMaxAveHeightZ()const;

	// 设置、获取百分比最大平均高度H判定条件
	void SetJudgePercentMaxAveHeightH(JudgeCombine& judge);
	JudgeCombine GetJudgePercentMaxAveHeightH()const;

	// 设置、获取百分比最小平均高度Z判定条件
	void SetJudgePercentMinAveHeightZ(JudgeCombine& judge);
	JudgeCombine GetJudgePercentMinAveHeightZ()const;

	// 设置、获取百分比最小平均高度H判定条件
	void SetJudgePercentMinAveHeightH(JudgeCombine& judge);
	JudgeCombine GetJudgePercentMinAveHeightH()const;

	// 获取检测结果
	HeightInspectResult GetHeightInspectResult() const;

	// 获取执行状态
	/*
	*  0 - 未执行检测
	*  1 - 检测成功
	* -1 - 不支持的图像格式
	* -2 - 高度转换指针为空
	* -3 - 图像生成范围图像宽高于检测图像宽高不一致
	* -4 - 检测平面有效点数小于3
	* -5 - 平面度平面拟合失败
	* -6 - 图像范围生成图像不是8位灰度图
	* -7 - 高度转换指针转换高度失败
	* -8 - 有效像素个数为0
	* -9 - 检测平面拟合失败
	* -10 - 最大高度X判定失败
	* -11 - 最大高度Y判定失败
	* -12 - 最大高度Z判定失败
	* -13 - 最大高度H判定失败
	* -14 - 最小高度X判定失败
	* -15 - 最小高度Y判定失败
	* -16 - 最小高度Z判定失败
	* -17 - 最小高度H判定失败
	* -18 - 平均高度Z判定失败
	* -19 - 平均高度H判定失败
	* -20 - 峰间值判定失败
	* -21 - 有效像素个数判定失败
	* -22 - 凹面积判定失败
	* -23 - 凸面积判定失败
	* -24 - 凹体积判定失败
	* -25 - 凸体积判定失败
	* -26 - 平面度判定失败
	* -27 - 平面度点X判定失败
	* -28 - 平面度点Y判定失败
	* -29 - 平面度点Z判定失败
	* -30 - 相对最小高度X判定失败
	* -31 - 相对最小高度Y判定失败
	* -32 - 相对最小高度Z判定失败
	* -33 - 相对最小高度H判定失败
	* -34 - 相对最大高度X判定失败
	* -35 - 相对最大高度Y判定失败
	* -36 - 相对最大高度Z判定失败
	* -37 - 相对最大高度H判定失败
	* -38 - 百分比最大平均高度检测失败，无有效高度点
	* -39 - 百分比最小平均高度检测失败，无有效高度点
	* -40 - 百分比最大平均高度Z判定失败
	* -41 - 百分比最大平均高度H判定失败
	* -42 - 百分比最小平均高度Z判定失败
	* -43 - 百分比最小平均高度H判定失败
	*/
	int GetRunStatus()const {return m_nRunStatus;}

private:          
	std::vector<scShape*> m_vpInspectRegion; // 检测范围
	std::vector<scShape*> m_vpMaskRegion;    // 屏蔽范围

	bool m_bImgRegionGenerator;    // 是否启用图像范围生成工具
	int  m_nInspectTarget;         // 检测目标（0：白色，1：黑色）
	cpImage m_RegionGenertorImage; // 图像范围生成图像

	scZeroPlaneResult m_ZeroPlane; // 起算面

	int m_nRunStatus;

	bool m_bInspectPlane;             // 是否检测平面
	bool m_bInspectMaxHeight;         // 是否检测最大高度
	bool m_bInspectMinHeight;         // 是否检测最小高度
	bool m_bInspectAveHeight;         // 是否检测平均高度
	bool m_bInspectPeakToPeak;        // 是否检测峰间值高度
	bool m_bInspectValidPixel;        // 是否检测有效像素
	bool m_bInspectArea;              // 是否检测面积
	bool m_bInspectVolume;            // 是否检测体积
	bool m_bInspectFlatness;          // 是否检测平面度
	bool m_bInspectRelativeMaxHeight; // 是否检测相对最大高度
	bool m_bInspectRelativeMinHeight; // 是否检测相对最小高度
	bool m_bAvePointsCalFlatness;     // 计算平面度方式（true: 一个区域求平均点计算，false: 区域每个点参与计算）
	bool m_bInspectPercentMaxAveHeight; // 是否检测百分比最大平均高度
	bool m_bInspectPercentMinAveHeight; // 是否检测百分比最小平均高度

	double m_dNoiseCut;   // 去除突起
	double m_dOffsetZ;    // Z偏移

	double m_dPercentMaxHeightRelative;	// 相对最大高度百分比
	double m_dPercentMinHeightRelative;	// 相对最小高度百分比

	double m_dPercentMaxAveHeight;   // 百分比最大平均高度百分比
	double m_dPercentMinAveHeight;   // 百分比最小平均高度百分比

	ResultCorrection m_CorrectionMaxHeightX;        // 最大高度X坐标补正
	ResultCorrection m_CorrectionMaxHeightY;        // 最大高度Y坐标补正
	ResultCorrection m_CorrectionMaxHeightZ;        // 最大高度Z坐标补正
	ResultCorrection m_CorrectionMaxHeightH;        // 最大高度H补正
	ResultCorrection m_CorrectionMinHeightX;        // 最小高度X坐标补正
	ResultCorrection m_CorrectionMinHeightY;        // 最小高度Y坐标补正
	ResultCorrection m_CorrectionMinHeightZ;        // 最小高度Z坐标补正
	ResultCorrection m_CorrectionMinHeightH;        // 最小高度H补正
	ResultCorrection m_CorrectionAveHeightZ;        // 平均高度Z坐标补正
	ResultCorrection m_CorrectionAveHeightH;        // 平均高度H补正
	ResultCorrection m_CorrectionPeakToPeakHeight;  // 峰间值高度补正
	ResultCorrection m_CorrectionConcaveArea;       // 凹面积补正
	ResultCorrection m_CorrectionConvexArea;        // 凸面积补正
	ResultCorrection m_CorrectionConcaveVolume;     // 凹体积补正
	ResultCorrection m_CorrectionConvexVolume;      // 凸体积补正
	ResultCorrection m_CorrectionFlatness;			// 平面度补正
	ResultCorrection m_CorrectionFlatnessX;			// 平面度X坐标补正
	ResultCorrection m_CorrectionFlatnessY;			// 平面度Y坐标补正
	ResultCorrection m_CorrectionFlatnessZ;			// 平面度Z坐标补正
	ResultCorrection m_CorrectionMaxHeightRelativeX;        // 相对最大高度X坐标补正
	ResultCorrection m_CorrectionMaxHeightRelativeY;        // 相对最大高度Y坐标补正
	ResultCorrection m_CorrectionMaxHeightRelativeZ;        // 相对最大高度Z坐标补正
	ResultCorrection m_CorrectionMaxHeightRelativeH;        // 相对最大高度H补正
	ResultCorrection m_CorrectionMinHeightRelativeX;        // 相对最小高度X坐标补正
	ResultCorrection m_CorrectionMinHeightRelativeY;        // 相对最小高度Y坐标补正
	ResultCorrection m_CorrectionMinHeightRelativeZ;        // 相对最小高度Z坐标补正
	ResultCorrection m_CorrectionMinHeightRelativeH;        // 相对最小高度H补正
	ResultCorrection m_CorrectionPercentMaxAveHeightZ;         // 百分比最大平均高度Z补正
	ResultCorrection m_CorrectionPercentMaxAveHeightH;         // 百分比最大平均高度H补正
	ResultCorrection m_CorrectionPercentMinAveHeightZ;         // 百分比最小平均高度Z补正
	ResultCorrection m_CorrectionPercentMinAveHeightH;         // 百分比最小平均高度H补正

	JudgeCombine     m_JudgeMaxHeightX;             // 最大高度X坐标判定
	JudgeCombine     m_JudgeMaxHeightY;             // 最大高度Y坐标判定
	JudgeCombine     m_JudgeMaxHeightZ;             // 最大高度Z坐标判定
	JudgeCombine     m_JudgeMaxHeightH;             // 最大高度H坐标判定
	JudgeCombine     m_JudgeMinHeightX;             // 最小高度X坐标判定
	JudgeCombine     m_JudgeMinHeightY;             // 最小高度Y坐标判定
	JudgeCombine     m_JudgeMinHeightZ;             // 最小高度Z坐标判定
	JudgeCombine     m_JudgeMinHeightH;             // 最小高度H坐标判定
	JudgeCombine     m_JudgeAveHeightZ;             // 平均高度Z坐标判定
	JudgeCombine     m_JudgeAveHeightH;             // 平均高度H坐标判定
	JudgeCombine     m_JudgePeakToPeakHeight;       // 峰值间高度判定
	JudgeCombine     m_JudgeValidPixelNum;          // 有效像素个数判定
	JudgeCombine     m_JudgeConcaveArea;            // 凹面积判定
	JudgeCombine     m_JudgeConvexArea;             // 凸面积判定
	JudgeCombine     m_JudgeConcaveVolume;          // 凹体积判定
	JudgeCombine     m_JudgeConvexVolume;           // 凸体积判定
	JudgeCombine	 m_JudgeFlatness;				// 平面度判定
	JudgeCombine	 m_JudgeFlatnessX;				// 平面度X坐标判定
	JudgeCombine	 m_JudgeFlatnessY;				// 平面度Y坐标判定
	JudgeCombine	 m_JudgeFlatnessZ;				// 平面度Z坐标判定
	JudgeCombine     m_JudgeMaxHeightRelativeX;     // 相对最大高度X坐标判定
	JudgeCombine     m_JudgeMaxHeightRelativeY;     // 相对最大高度Y坐标判定
	JudgeCombine     m_JudgeMaxHeightRelativeZ;     // 相对最大高度Z坐标判定
	JudgeCombine     m_JudgeMaxHeightRelativeH;     // 相对最大高度H坐标判定
	JudgeCombine     m_JudgeMinHeightRelativeX;     // 相对最小高度X坐标判定
	JudgeCombine     m_JudgeMinHeightRelativeY;     // 相对最小高度Y坐标判定
	JudgeCombine     m_JudgeMinHeightRelativeZ;     // 相对最小高度Z坐标判定
	JudgeCombine     m_JudgeMinHeightRelativeH;     // 相对最小高度H坐标判定
	JudgeCombine     m_JudgePercentMaxAveHeightZ;   // 百分比最大平均高度Z判定
	JudgeCombine     m_JudgePercentMaxAveHeightH;   // 百分比最大平均高度H判定
	JudgeCombine     m_JudgePercentMinAveHeightZ;   // 百分比最小平均高度Z判定
	JudgeCombine     m_JudgePercentMinAveHeightH;   // 百分比最小平均高度H判定

	HeightInspectResult m_HeightInspectResult;      // 检测结果
};

#pragma pack(pop)