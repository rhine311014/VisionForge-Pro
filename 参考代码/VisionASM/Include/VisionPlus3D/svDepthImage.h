#pragma once

#ifdef SV_POINT_CLOUD_PROC_EXPORTS
#define SV_DEPTH_IMAGE_API __declspec(dllexport)
#else
#define SV_DEPTH_IMAGE_API __declspec(dllimport)
#endif


#pragma pack(push,1)

typedef struct tagDepthPints
{
	float x;
	float y;
	float z;
	float range;

	tagDepthPints()
	{
		x = -1.f;
		y = -1.f;
		z = 0.f;
		range = 0.f;
	}
}DepthPoints;

#pragma pack(pop)

#pragma pack(push,8)

#include "vpImage.h"
#include "svVector3D.h"
#include "svMatrix4.h"

class scMesh;

class SV_DEPTH_IMAGE_API scDepthImage
{
public:
	scDepthImage();
	scDepthImage(const scDepthImage& image);
	~scDepthImage();
	
	scDepthImage& operator=(const scDepthImage& image);

	// 根据图像文件生成深度图
	bool SetDepthImage(const TCHAR* filePath,float centerX,float centerY,float focalX,float focalY,float scale = 0.001f,float angular_res = -1.f);

	// 根据内存数据生成深度图
	bool SetDepthImage(const unsigned char* pdata,int width,int height,int depth,float centerX,float centerY,float focalX,float focalY,float scale = 0.001f,float angular_res = -1.f); // 读取图像内存转换为深度图
	
	// 根据视点、角度生成深度图
	// mesh,点云数据指针
	// angleResX,x方向角分辨率
	// angleResY,y方向角分辨率
	// maxAngleX，x方向最大角度
	// maxAnlgeY,y方向最大角度
	// mat,相机坐标系转换矩阵
	bool MeshTransToDepthImageWithAngle(const scMesh* mesh,float angleResX,float angleResY,float maxAngleX,float maxAnlgeY ,const scMatrix4& mat);

	// 根据视点、宽高生成深度图
	// mesh,点云数据指针
	// width,图像宽度
	// height,图像高度
	// mat,相机坐标系转换矩阵
	bool MeshTransToDepthImageWithSize(const scMesh* mesh,int width,int height,const scMatrix4& mat);

	// 获取可视化的深度图
	bool GetVisualizeImage(cpImage& image)const;

	// 获取图例
	void GetRampImage(cpImage& image);

	// 深度图转换为点云数据
	bool GetMesh(scMesh* mesh);

	// 获取深度图数据
	std::vector<DepthPoints>& GetDepthPoints() {return m_vDepthPoints;}

	int Width()const  {return (int)m_nWidth;}
	int Height()const {return (int)m_nHeight;}

	float GetMaxRange() const {return m_dMaxRange;}
	float GetMinRange() const {return m_dMinRange;}

	// low= [0,1] high = [0,1]
	void SetColorRange(float low,float high);
	void GetColorRange(float& low,float& hight);

private:
	void PointTransToAngle(float pointX,float pointY,float pointZ,float viewX,float viewY,float viewZ,float& angleX,float& angleY,float& range);
	void AngleTransToImageCoord(float angleX,float angleY,float angleResX,float angleResY,float offsetX,float offsetY,float& imageX,float& imageY);

private:
	std::vector<DepthPoints> m_vDepthPoints;
	float          m_dMaxRange;
	float          m_dMinRange;

	float          m_dColorRangeLow;
	float          m_dColorRangeHigh;

	unsigned int   m_nWidth;
	unsigned int   m_nHeight;
	float          m_dCenterX;
	float          m_dCenterY;
	float          m_dFocalX;
	float          m_dFocalY;
	float          m_dScale;
};

#pragma pack(pop)