#pragma once
#include <vector>
//****窗口布局基类

class CLayout
{
public:
	enum LayoutType
	{
		eLayout_Horizontal = 0, // 水平布局
		eLayout_Vertical = 1,   // 垂直布局
		eLayout_Grid = 2,       // 网格布局
	};


	enum SizePolicy
	{
		eSizePolicy_Fixed = 0, // 固定
		eSizePolicy_Expand = 1, // 扩展
		eSizePolicy_Preferred = 2, // 默认
	};



	struct _Child_Ptr 
	{
		bool bIsLayout;	     // 当前子类类型:true 布局 : false 窗口
		void* pPtr;			 // 当前子类指针
		
		SizePolicy eHoriSizePolicy;   // 水平方向扩展属性
		SizePolicy eVertSizePolicy;   // 垂直方向扩展属性

		int nWidth;					// 子类宽
		int nheight;				// 子类高

		int nTopAlignOffset;			// 上对齐偏差 
		int nRightAlignOffset;          // 右对齐偏差
		int nBottomAlignOffset;			// 下对其偏差
		int nLeftAlignOffset;           // 左对齐偏差

		bool bAreaInvisible;            // 隐藏状态是否分配面积

		_Child_Ptr()
		{
			nWidth = 50;
			nheight = 50;
			bIsLayout = false;
			pPtr = NULL;
			eHoriSizePolicy = eSizePolicy_Preferred;
			eVertSizePolicy = eSizePolicy_Preferred;

			nTopAlignOffset = 1;
			nRightAlignOffset = 1;
			nBottomAlignOffset = 1;
			nLeftAlignOffset = 1;

			bAreaInvisible = false;
		}
	};

	
public:
	CLayout(CLayout* pParentLayout=NULL);
	virtual ~CLayout(void);
public:
	// 接口1 更新所有子窗口的大小
	virtual void OnSize() = 0;



public:	// 属性接口

	void SetVisible(bool bVisible) { m_bIsVisible = bVisible; }
	int IsVisible() { return m_bIsVisible; }

	void SetParent(CWnd* pParent);
	CWnd* GetParent() { return m_pParentWnd; }

	void SetParentLayout(CLayout* pParentLayout);
	CLayout* GetParentLayout(){ return m_pParentLayout; }

	void SetAutoDeleteChild(bool bAutoDelete) { m_bAutoDeleteChild = bAutoDelete; }

	void SetLayoutType(LayoutType eLayoutType) { m_eLayoutType = eLayoutType; }
	LayoutType GetLayoutType() { return m_eLayoutType; }


	void SetLayoutRect(RECT* rt);
	RECT GetLayoutRect() { return m_rtLayout; }

	void AddWnd(CWnd* pWnd, int w = 0, int h = 0, SizePolicy eSizePolicy = eSizePolicy_Preferred, int nLeftOffset=1, int nTopOffset=1, int nRightOffset=1, int nBottomOffset=1, bool bAreaInvisible=false);
	void AddLayout(CLayout* pLayout, int w = 0 , int h = 0, SizePolicy eSizePolicy = eSizePolicy_Preferred, int nLeftOffset=0, int nTopOffset=0, int nRightOffset=0, int nBottomOffset=0);
	
	void ResetWnd(CWnd* pWnd, int w = 0, int h = 0, SizePolicy eSizePolicy = eSizePolicy_Preferred, int nLeftOffset=1, int nTopOffset=1, int nRightOffset=1, int nBottomOffset=1);
	void ResetLayout(CLayout* pLayout, int w = 0 , int h = 0, SizePolicy eSizePolicy = eSizePolicy_Preferred, int nLeftOffset=1, int nTopOffset=1, int nRightOffset=1, int nBottomOffset=1);

	
	int GetChildCount();
	void SetAllChildVisible(bool bVisile);
	void SetChildVisible(int nIndex, bool bVisile);


	void OnDraw();
	void SetPaintBkColor(bool bPaintBkColor) { m_bPaintBkColor = bPaintBkColor; }
	void SetBkgroundColor(COLORREF clr) { m_backGroundColor = clr; }

	 

public:   // 属性
	LayoutType					 m_eLayoutType;		// 类型
	CWnd*						 m_pParentWnd;      // 布局所在窗口 
	CLayout*					 m_pParentLayout;   // 父布局管理器
	std::vector<_Child_Ptr>		 m_vpChildPtr;      // 布局子类
	RECT						 m_rtLayout;		// 当前布局位置及大小
	bool						 m_bUseParentLayoutSize; // 是否使用父布局的大小， 当使用SetParentLayout时触发有效, AddLayout无效
	bool						 m_bIsVisible;      // 是否显示

	COLORREF					 m_backGroundColor; // 背景颜色
	bool						 m_bPaintBkColor;
	bool						 m_bAutoDeleteChild; // 自动删除子类
	
};

