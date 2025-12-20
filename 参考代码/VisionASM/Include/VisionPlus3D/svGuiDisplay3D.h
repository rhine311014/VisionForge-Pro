#pragma once

#include <vector>
#include <map>

#include "svGuiDef3D.h"
#include "svGuiContainer3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"
#include "svMatrix4.h"
#include "svDepthImage.h"

class scMesh;
class scShader;
class cpImage;
class sc2Vector;
class CCustomToolBar;
class scGuiDepthImage;

#pragma pack(push,8)

struct Character {
	unsigned int TextureID;
	int Size[2];
	int Bearing[2];
	unsigned int Advance;
};

class SVGUI_API_3D scGuiDisplay3D
{
public:
	scGuiDisplay3D();
	~scGuiDisplay3D();

	enum DisplayMode
	{
		eDisplayMesh = 0,  // 点云模式
		eDisplayDepth,     // 深度图模式
		eDisplayPopMenu
	};

	enum MeshMode
	{
		eMeshPointCloud    = (1 << 0),  // 显示点云
		eMeshWireFrame     = (1 << 1),  // 显示线框
		eMeshFace          = (1 << 2),  // 显示表面
		eMeshExcuboid      = (1 << 3),  // 显示外接立方体
		eMeshAxis          = (1 << 4),  // 显示坐标轴
		eMeshNormal        = (1 << 5),  // 显示法向量
		eMeshDepth         = (1 << 6),  // 显示深度图
		eMeshDepthRamp     = (1 << 7),  // 显示深度图图例
		eMeshMirror       = (1 << 8),  // 显示镜像

		eMeshDefault     = eMeshPointCloud | eMeshDepth | eMeshDepthRamp
	};

	enum ToolBarMode
	{
		eToolBarNone       = 0,        // 不启用工具条
		eToolBarOpenFile   = (1 << 0), // 打开文件按钮可用
		eToolBarSaveFile   = (1 << 1), // 保存文件按钮可用
		eToolBarFitView    = (1 << 2), // 适应窗口按钮可用
		eToolBarPointCloud = (1 << 3), // 显示点云按钮可用
		eToolBarWireFrame  = (1 << 4), // 显示线框按钮可用
		eToolBarFace       = (1 << 5), // 显示表面按钮可用
		eToolBarExcuboid   = (1 << 6), // 显示外接立方体按钮可用
		eToolBarAxis       = (1 << 7), // 显示坐标轴按钮可用
		eToolBarNormal     = (1 << 8), // 显示法向量按钮可用
		eToolBarDepth      = (1 << 9), // 显示深度图按钮可用
		eToolBarMirror     = (1 << 10), // 显示镜像按钮可用
		eToolBarAutoFit	   = (1 << 11), // 显示镜像按钮可用

		eToolBarDefault    = 2*eToolBarAutoFit - 1  // 默认所有按钮都可用
	};

	enum DisplayProjection
	{
		eDisplayOrthographic,  // 正射投影
		eDisplayPerspective    // 透视投影
	};

	enum ZoomMode
	{
		eMeshCenter,       // 以图像中心为中心进行缩放
		eCursorPos         // 以鼠标位置为中心进行缩放
	};

	enum CullFace
	{
		eCullNone,          // 不剔除表面
		eCullFront,         // 剔除正面
		eCullBack,          // 剔除背面
		eCullFrontAndBack   // 剔除正面和背面
	};

	void SetMesh(scMesh* mesh,bool bCopy = false);   // 设置点云
	scMesh* GetMesh() const;

	bool Create(UINT nPosID, CWnd* pParentWnd, DWORD dwStyle = NULL, DWORD dwExStyle = NULL);  // 创建窗口
	bool Create(LPRECT lpRect, CWnd* pParentWnd, DWORD dwStyle = NULL, DWORD dwExStyle = NULL);

	HWND GetHWND()const; // 获取窗口句柄

	CWnd* GetParentWnd()const {return m_pParentWnd;}

	void SetDisplayMode(DisplayMode displayMode);  // 设置显示模式
	void SetMeshMode(UINT meshMode);               // 设置点云显示内容
	void SetToolBarMode(UINT toolbarMode);         // 设置工具条按钮显示

	void SetBackgroundColor(COLORREF color);  // 设置背景颜色

	void SetPointCloudVisible(bool bShow = true);
	void SetTriangleVisible(bool bShow = true);
	void SetFaceVisible(bool bShow = true);
	void SetNormalVisible(bool bShow = true);
	void SetAxisVisible(bool bShow = true);
	void SetExternalCuboidVisible(bool bShow = true);
	void SetMirrorVisible(bool bShow = true);

	void SetNormalColor(COLORREF color) {m_clrNormalColor = color;}
	void SetZoomMode(ZoomMode mode) {m_ZoomMode = mode;}
	void SetCullFaceMode(CullFace mode) {m_CullFaceMode = mode;}

	int GetDisplayMode() const;
	UINT GetMeshMode() const;
	UINT GetToolBarMode() const;

	COLORREF GetBackgroundColor() const; // 获取背景颜色

	bool GetPointCloudVisible() const;
	bool GetTriangleVisible() const;
	bool GetFaceVisible() const;
	bool GetNormalVisible() const;
	bool GetAxisVisible() const;
	bool GetExternalCuboidVisible() const;
	bool GetMirrorVisible() const;

	COLORREF GetNormalColor()const {return m_clrNormalColor;}
	ZoomMode GetZoomMode()const {return m_ZoomMode;}
	CullFace GetCullFaceMode()const {return m_CullFaceMode;}

	void MyInvalidate(); // 刷新

	void Move(float lastX,float lastY,float curX,float curY);
	void Rotate(float lastX,float lastY,float curX,float curY);
	void Zoom(int mode,float curX,float curY);

	void FitView();

	void VisualizeCurrentDepthImage(); // 当前视点下，可视化深度图
	void SetDepthRampVisible(bool bVisible);
	bool GetDepthRampVisible()const;

	void SetWindowPos(LPRECT lpRect);
	void SetWindowPos(UINT nPosID, CWnd* pParentWnd);

	// message
	UINT GetWindowMessage()const;
	void GetCursorPos(float& x,float& y);

	void SetWindowMessage(UINT msg);
	void SetCursorPos(float x,float y);

	// GuiContainer
	void SetGuiContainer3D(scGuiContainer3D* pGuiContainer);
	scGuiContainer3D* GetGuiContainer3D() const;

	// matrix
	scMatrix4 GetModelMatrix4x4()const {return m_matModel;}
	scMatrix4 GetViewMatrix4x4()const {return m_matView;}
	scMatrix4 GetProjectedMatrix4x4()const {return m_matProject;}
	scMatrix4 GetZoomMatrix4x4()const {return m_matZoom;}

	void UpdateMVPZMatrix();

	// 根据窗口坐标获取全局坐标
	// x、y为窗口坐标
	// disPlane为转换的三维坐标平面到视点的距离
	scPoint3D GetGlobalCoordinate(float x,float y,float disPlane = 1.f)const;

	// 根据全局坐标，转换为窗口坐标
	sc2Vector GetWindowCoordinate(const scPoint3D& pos)const;

	// 获取三维坐标转到窗口坐标的4x4矩阵
	scMatrix4 GetGlobalToWindowTransform()const;

	// 获取窗口坐标转到三维坐标的4x4矩阵
	// disPlane表示坐标转换平面到视点的距离
	// 透视投影下，disPlane必须大于0
	scMatrix4 GetWindowToGlobalTransform(float disPlane = 1.f)const;

	// 获取屏幕指定坐标的颜色
	COLORREF GetScreenColor(int x,int y)const;

	// 获取屏幕图像
	cpImage GetScreenImage()const;

	/*-------------- ViewPoint --------------------*/
	// 设置投影方式
	void SetProjectMode(DisplayProjection mode) {m_ProjectMode = mode;}
	DisplayProjection GetProjectMode()const {return m_ProjectMode;}

	// 设置视点位置
	void SetViewPoint(const scPoint3D& viewPoint);           
	scPoint3D GetViewPoint()const;

	// 设置视点中心
	void SetViewCenter(const scPoint3D& viewCenter);
	scPoint3D GetViewCenter()const;

	// 设置世界坐标系到视点坐标系绕z轴的角度(角度)
	void SetRoll(float roll);
	float GetRoll()const;

	// 设置近裁平面到视点的距离
	void SetNearPlaneDistance(float dis);
	float GetNearPlaneDistance()const;

	// 设置远裁平面到视点的距离
	void SetFarPlaneDistance(float dis);
	float GetFarPlaneDistance()const;

	// 设置透视投影水平视角（角度）
	void SetFOV(float fov);
	float GetFOV()const;

	// 设置透视投影宽高比
	void SetWidthHeightRatio(float ratio) {m_dWidthHeightRatio = ratio;}
	float GetWidthHeightRatio()const {return m_dWidthHeightRatio;}

	// 设置正视投影边距
	void SetOrthBound(float left,float right,float bottom,float top);
	void GetOrthBound(float& left,float& right,float& bottom,float& top)const;

	// 设置缩放参数
	void SetZoomParam(float left,float right,float bottom,float top);
	void GetZoomParam(float& left,float& right,float& bottom,float& top)const;

	// 设置X,Y偏移量
	void SetOffsetParam(float moveX,float moveY);
	void GetOffsetParam(float& moveX,float& moveY)const;

	// 视图自适应
	void SetAutoFit(bool bAuto) {m_bAutoFit = bAuto;}
	bool IsAutoFit()const {return m_bAutoFit;}

	// fit时，光源位置是否与视点位置一致
	void SetLightPosSameAsViewPointWhenFit(bool bSame) {m_bLightPosSameAsViewPointWhenFit = bSame;}
	bool IsLightPosSameAsViewPointWhenFit()const {return m_bLightPosSameAsViewPointWhenFit;}

	// 获取视点坐标系的三个欧拉角
	void GetViewCoordinateEulerAngles(float& yaw,float& pitch,float& roll)const;

	/*-------------- ViewPoint --------------------*/

	/*-------------- Light ------------------------*/

	// 是否开启光源进行渲染,默认开启
	void EnableLightRender(bool bEnable) {m_bLightRender = bEnable;}
	bool IsEnableLightRender()const {return m_bLightRender;}

	// 光源位置
	void SetLightPos(const scPoint3D& lightPos);
	scPoint3D GetLightPos()const;

	// 光源颜色
	void SetLightColor(COLORREF color) {m_LightColor = color;}
	COLORREF GetLightColor()const {return m_LightColor;}

	// 环境光强度
	void SetAmbientStrength(const scVector3D& ambient) {m_AmbientStrength = ambient;}
	scVector3D GetAmbientStrength()const {return m_AmbientStrength;}

	// 漫反射光强度
	void SetDiffuseStrength(const scVector3D& diffuse) {m_DiffuseStrength = diffuse;}
	scVector3D GetDiffuseStrength()const {return m_DiffuseStrength;}

	// 镜面反射光强度
	void SetSpecularStrength(const scVector3D& specular) {m_SpecularStrength = specular;}
	scVector3D GetSpecularStrength()const {return m_SpecularStrength;}

	// 光源位置是否不随视角变化而变化
	void SetLightPosFixed(bool bFixed) {m_bLightPosFixed = bFixed;}
	bool IsLightPosFixed()const {return m_bLightPosFixed;}

	/*-------------- Light ------------------------*/

	/*-------------- Interactive ------------------*/

	void DragStart(float x,float y);
	void DragStop();

	/*-------------- Interactive ------------------*/
	void SetGrayRatio(float ratio) {m_dGrayRatio = ratio;}
	float GetGrayRatio()const {return m_dGrayRatio;}

	// 需要在窗口销毁之前释放显存
	void ReleaseWindowResource();

private:
	void DrawBackground();
	void DrawPointCloud();
	void DrawTriangle();
	void DrawFace();
	void DrawNormal();
	void DrawAxis();
	void DrawExternalCuboid();

	bool CompileShaderProgram();

	void InitRender();

	void UpdateViewPoint();
	void UpdateExcuboid();
	void UpdateAxis();
	void UpdateNormal();

	void LoadFont(const char* ttfName);
private:
	void*    m_glWindow;
	CWnd*    m_pParentWnd;
	scMesh*  m_Mesh;

	DisplayMode   m_DisplayMode;
	UINT          m_MeshMode;
	UINT          m_ToolBarMode;

	CCustomToolBar* m_pToolBar;

	// 视点参数
	float    m_dViewFOV,m_dDistance;
	float m_dRoll;

	scPoint3D m_ViewPoint;
	scPoint3D m_ViewCenter;
	float     m_dNearPlaneDis;
	float     m_dFarPlaneDis;
	float     m_dWidthHeightRatio;

	float    m_dOrthLeft;
	float    m_dOrthRight;
	float    m_dOrthBottom;
	float    m_dOrthTop;

	bool     m_bAutoFit;

	float    m_dScale;

	float    m_clrBackGround[4];

	bool     m_bCopyMesh;

	scShader*     m_ShaderRender;
	scShader*     m_TextureRender;

	int      m_nViewX;
	int      m_nViewY;
	int      m_nViewWidth;
	int      m_nViewHeight;

	bool     m_bLight;

	COLORREF m_clrNormalColor;
	float    m_dNormalLength;

	std::map<char,Character> m_mapCharacter;
	unsigned int m_FontArrayRender,m_FontVertexRender;

	UINT     m_WindowMessage;
	float    m_CursorPosX;
	float    m_CursorPosY;

	scGuiContainer3D* m_pGuiContainer;

	DisplayProjection m_ProjectMode;
	ZoomMode  m_ZoomMode;

	float m_dZoomLeft,m_dZoomTop,m_dZoomRight,m_dZoomBottom;

	float m_dMoveX,m_dMoveY;

	float m_dViewFrustumMaxLength;

	CullFace m_CullFaceMode;

	// light
	bool m_bLightRender;

	scPoint3D m_LightPos;
	COLORREF  m_LightColor;
	scVector3D m_AmbientStrength;
	scVector3D m_DiffuseStrength;
	scVector3D m_SpecularStrength;

	bool  m_bLightPosFixed;
	bool  m_bLightPosSameAsViewPointWhenFit;

	// Interactive
	scGuiGraphic3D* m_pGuiSelectedGraphic;
	bool m_bDragMesh;
	float m_dIntersectDis;

	// matrix
	scMatrix4 m_matModel;
	scMatrix4 m_matView;
	scMatrix4 m_matProject;
	scMatrix4 m_matZoom;

	// depth
	scGuiDepthImage* m_pGuiDepthImage;

	// Order Mesh
	float m_dGrayRatio;
};

#pragma pack(pop)