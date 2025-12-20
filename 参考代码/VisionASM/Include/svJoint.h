// svJoint.h: interface for the scJoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVJOINT_H__E613497B_1786_4026_B602_AEEDC438DFFA__INCLUDED_)
#define AFX_SVJOINT_H__E613497B_1786_4026_B602_AEEDC438DFFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "svJointDef.h"
#include "svImage.h"

#pragma pack(8)

enum JointMethod
{
	eBrighter = 0,		//取更亮的像素灰度
	eDarker,			//取更暗的像素灰度
	eImage1,			//取图像1像素灰度
	eImage2,			//取图像2像素灰度
};

class SVJOINT_API scJoint  
{
public:
	scJoint();
	virtual ~scJoint();

	//设置亚像素精度标记
	void EnableSubPixel(bool bSubPixel = true);

	//设置X方向和Y方向当量
	void SetRatio(double dRatioX,double dRatioY);
	void GetRatio(double& dRatioX,double& dRatioY);

	//开始
	bool Start(const scImageGray& image1);

	//拼接图像
	bool Jointing(const scImageGray& image2,double dMoveX,double dMoveY,JointMethod method = eBrighter);

	//获取拼接结果图像
	bool GetResultImage(scImageGray& image);

	//获取第一幅图像某点坐标(X,Y)对应拼接结果图像中的坐标(ResultX,ResultY);
	bool GetXYInResultImage(double X,double Y,double &ResultX,double &ResultY);

private:
	//计算新的图像大小，拼接当前图像
	void PutImageOnNewSize(scImageGray image,double dMoveX,double dMoveY,JointMethod method);
	void PutSubPixelImageOnNewSize(scImageGray image,double dMoveX,double dMoveY,JointMethod method);

private:
	//标记亚像素拼接
	bool m_bSubPixel;

	//标记拼接开始
	bool   m_bStart;

	//标定当量
	double m_dRatioX;
	double m_dRatioY;

	//拼接结果图像
	scImageGray	m_ResultImage;

	//拼接后新坐标原点到参考点的偏移量(像素单位)
	//注：参考点为第一幅图像左上角位置
	double m_dFromRefX;
	double m_dFromRefY;

	//当前输入图像左上角到拼接后新坐标原点偏移量(物理单位)
	double m_dFromOriginalX;
	double m_dFromOriginalY;

	//当前坐标原点到拼接后新坐标原点偏移量(物理单位)
	double m_dOrignialX;
	double m_dOrignialY;

	//拼图后结果图像大小(物理单位)
	double m_dObjWidth;
	double m_dObjHeight;

	//单幅图像大小(像素单位)
	long m_lWidth;
	long m_lHeight;
};

#pragma pack()

#endif // !defined(AFX_SVJOINT_H__E613497B_1786_4026_B602_AEEDC438DFFA__INCLUDED_)
