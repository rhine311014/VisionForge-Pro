#pragma once

#ifdef SV_POINT_CLOUD_GENERATE_TOOL_BASE_EXPORTS
#define SV_POINT_CLOUD_GENERATE_TOOL_BASE_API __declspec(dllexport)
#else
#define SV_POINT_CLOUD_GENERATE_TOOL_BASE_API __declspec(dllimport)
#endif

#include <vector>

#pragma pack(push,8)

// 点云生成工具
class SV_POINT_CLOUD_GENERATE_TOOL_BASE_API cpPointCloudGenerateToolBase
{
public:
	cpPointCloudGenerateToolBase();
	virtual ~cpPointCloudGenerateToolBase();

	// 设置/获取单位像素X方向实际长度(mm)
	void SetUnitPixelPhysicalDisX(double dDisX);
	double GetUnitPixelPhysicalDisX()const;

	// 设置/获取单位像素Y方向实际长度(mm)
	void SetUnitPixelPhysicalDisY(double dDisY);
	double GetUnitPixelPhysicalDisY()const;

	// 单个像素值转换为真实高度
	virtual bool TransPixelToHeight(double pixel,double& dHeight)const;

	// 多个像素值转换为真实高度
	virtual bool TransPixelToHeight(const std::vector<double>& vdPixel,std::vector<double>& vdHeight)const;

	/*
	* 功能描述：整幅图像转换为实际的X坐标、Y坐标、高度值
	* 输入参数：
	            pImgData：  图像数据指针（必须为16位图像）
				lImgWidth： 图像宽度
				lImgHeight：图像高度
	* 输出参数：
	            vdX：       输出实际X坐标
				vdY：       输出实际Y坐标
				vdHeight：  输出实际高度值
	*/
	virtual bool TransPixelToHeight(void* pImgData,long lImgWidth,long lImgHeight,std::vector<double>& vdX,std::vector<double>& vdY,std::vector<double>& vdHeight)const;

	/*
	* 功能描述：将图像像素值在对应掩模图像像素值为255时，转换为实际的X坐标、Y坐标、高度值
	* 输入参数：
	            pImgData：   图像数据指针（必须为16位图像）
	            lImgWidth：  图像宽度
	            lImgHeight： 图像高度
				pMaskData：  掩模图像数据指针(必须为8位图像)
				lMaskWidth： 掩模图像宽度
				lMaskHeight：掩模图像高度
				lOffsetX：   掩模图像X偏移
				lOffsetY：   掩模图像Y偏移
	* 输出参数：
	            vdX：        输出实际X坐标
	            vdY：        输出实际Y坐标
	            vdHeight：   输出实际高度值
	*/
	virtual bool TransPixelToHeight(void* pImgData,long lImgWidth,long lImgHeight,void* pMaskData,long lMaskWidth,long lMaskHeight,long lOffsetX,long lOffsetY,std::vector<double>& vdX,std::vector<double>& vdY,std::vector<double>& vdHeight)const;

	// 单个像素实际高度(mm)转换为像素值
	virtual bool TransHeightToPixel(double dHeight,double& pixel)const;

	// 多个像素实际高度(mm)转换为像素值
	virtual bool TransHeightToPixel(const std::vector<double>& vdHeight,std::vector<double>& vlPixel)const;

	// 获取相机高度范围
	virtual bool GetHeightRange(double& dHeightMin,double& dHeightMax)const;

	// 像素差转换为高度差
	virtual bool TransDiffPixelToDiffHeight(double pixel,double& height)const;

	// 高度差转换为像素差
	virtual bool TransDiffHeightToDiffPixel(double height,double& pixel)const;

protected:
	double m_dUnitPixelPhysicalDisX;
	double m_dUnitPixelPhysicalDisY;
};

#pragma pack(pop)