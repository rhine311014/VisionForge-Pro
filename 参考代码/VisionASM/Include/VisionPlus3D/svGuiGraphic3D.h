#pragma once

#include "svGuiDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"
#include <map>

#pragma pack(push,8)

typedef void (*glProcPtr)(void);

class SVGEOMETRICSGUI_API_3D scGuiGraphic3D
{
public:
	scGuiGraphic3D();
	virtual ~scGuiGraphic3D();

	virtual GuiGraphicType3D GetType() const = 0; // 获取gui类型

public:
	virtual void Draw(unsigned int nProgramID) const = 0; // 绘制
	virtual void GenGpuData() = 0;     // 生成GPU显存数据区
	virtual void TransDataToGPU() = 0; // 将图形数据更新到GPU
	virtual void ReleaseGpuData() = 0; // 释放GPU显存数据区

	virtual scShape3D* GetShape() {return NULL;} // 获取图形数据结构

	virtual void SetViewFrustumMaxLength(float length) {return ;} // 设置Display视锥最大长度（绘制无限图形时使用）

	void SetDOF(UINT dof) {m_DOF = dof;}
	UINT GetDOF()const {return m_DOF;}

	void SetVisible(bool bShow = true);  // 设置图形是否可见
	bool IsVisible() const;

	void SetLineWidth(int width);   // 设置线宽（点大小）
	int  GetLineWidth() const;

	// 是否开启透明度，默认不开启
	// 透明度与渲染冲突，两者只能选其中一种开启
	void EnableTransParency(bool enable);
	bool IsEnableTransParency()const;

	// 设置线条颜色，alpha为透明度，0为完全透明，255位完全不透明,开启透明度有效
	void SetLineColor(COLORREF color,BYTE alpha = 255);
	COLORREF GetLineColor() const;

	// 设置handle颜色
	void SetHandleColor(COLORREF color) {m_clrHandleColor = color;}
	COLORREF GetHandleColor()const {return m_clrHandleColor;}

	// 是否开启几何元素渲染，立体几何元素默认开启，平面几何元素默认不开启
	void EnableGeometricsRender(bool bEnable); 
	bool IsEnableGeometricsRender()const;

	// 图形是否可交互
	void EnableInteractive(bool bEnbale);
	bool IsEnableInteractive()const;

    // 设置交互距离（像素距离）
	void SetInteractiveDistance(float dis) {m_fInteractiveDistance = dis;}
	float GetInteractiveDistance()const {return m_fInteractiveDistance;}

	// 设置图形是否选中
	void SetGuiSelected(bool bSelected) {m_bSelected = bSelected;}
	bool IsGuiSelected()const {return m_bSelected;}

	// 设置图形是否正在拖拽
	void SetGuiDragging(bool bDragging) {m_bDragging = bDragging;}
	bool IsGuiDragging()const{return m_bDragging;}

	// 设置选中颜色
	void SetSelectedColor(COLORREF color,BYTE alpha = 255);
	COLORREF GetSelectedColor()const {return m_clrSelectedColor;}

	// 设置拖拽颜色
	void SetDraggingColor(COLORREF color,BYTE alpha = 255);
	COLORREF GetDraggingColor()const {return m_clrDraggingColor;}

	// 设置窗口坐标到世界坐标的转换矩阵
	void SetWindowToGlobalTransform(float* matrix) {memcpy(m_matWindowToGlobal,matrix,sizeof(float)*16);}

	// 设置世界坐标到窗口坐标的转换矩阵
	void SetGlobalToWindowTransform(float* matrix) {memcpy(m_matGlobalToWindow,matrix,sizeof(float)*16);}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ) {return false;}

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop) {return ;}

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle) {return ;}

	void LoadOpenGLPtr(glProcPtr ptr); // 加载OpenGL指针
	bool CheckOpenGLPtr()const; // 检查OpenGL指针是否加载

	void SetGuiDisplay3D(void* display) {m_pGuiDisplay3D = display;} // 设置用于显示的窗口,供Display3D使用
	void* GetGuiDisplay3D()const {return m_pGuiDisplay3D;} // 获取当前的显示窗口

	void SetGLFWwindow(void* pWindow) {m_pGLFWwindow = pWindow;}
	void* GetGLFWwindow()const {return m_pGLFWwindow;}

	void SetGuiContainer(void* pGuiContainer) {m_pGuiContainer = pGuiContainer;}
	void* GetGuiContainer()const {return m_pGuiContainer;}

	static std::map<int,scGuiGraphic3D*>& GetObjectPtr() {return g_ObjectPtr;} // 获取所有Gui对象

protected:
	void UpdateLineParam(unsigned int nProgramID)const; // 更新线段信息到GPU
	void UpdateHandleParam(unsigned int nProgramID)const; // 更新handle信息到GPU

protected:
	bool m_bVisible;
	int  m_nLineWidth;
	COLORREF m_clrLineColor;
	COLORREF m_clrHandleColor;

	unsigned int m_nArrayObj;
	unsigned int m_nVertexObj;
	unsigned int m_nIndexObj;

	bool m_bGenGpuData;
	bool m_bDataChanged;

	bool m_bEnableTransParency;
	bool m_bEnableGeometricsRender;
	bool m_bEnableInteractive;

	int m_ObjectID;

	float m_fInteractiveDistance;

	UINT m_DOF;
	UINT m_HitResult;

	bool m_bSelected;
	bool m_bDragging;

	COLORREF m_clrSelectedColor;
	COLORREF m_clrDraggingColor;

	void* m_pGuiDisplay3D;
	void* m_pGLFWwindow;
	void* m_pGuiContainer;

	float m_matWindowToGlobal[16];
	float m_matGlobalToWindow[16];

	static int g_nObjectCount;
	static bool g_bLoadGLPtrSucess;
	static std::map<int,scGuiGraphic3D*> g_ObjectPtr;
};

#pragma pack(pop)
