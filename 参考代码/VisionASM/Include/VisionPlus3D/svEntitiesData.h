#pragma once

#pragma pack(push,8)

#include "svVector.h"
#include "svFeature.h"

#define eEntitiesAll 262143         // 所有支持读取的图元

// 图元类型
enum EntitiesType
{
	eEntitiesNone = 0,

	// 2D图形
	eEntitiesArc = 1,				 // 圆弧   - 对应DXF图元: ARC
	eEntitiesCircle = 2,			 // 圆     - 对应DXF图元: CIRCLE
	eEntitiesEllipse = 4,			 // 椭圆   - 对应DXF图元: ELLIPSE
	eEntitiesEllipseArc = 8,		 // 椭圆弧 - 对应DXF图元: ELLIPSE
	eEntitiesLine = 16,				 // 线段   - 对应DXF图元: LINE
	eEntitiesPoint = 32,			 // 点     - 对应DXF图元: POINT
	eEntitiesContour = 64,			 // 轮廓   - 对应DXF图元: INSERT,LWPOLYLINE,MLINE,POLYLINE,SPLINE
	eEntitiesRay = 128,				 // 射线   - 对应DXF图元: RAY
	eEntitiesXline = 256,			 // 直线   - 对应DXF图元: XLINE
	eEntitiesInsert = 512,           // 阵列或块参照		- 对应DXF图元: INSERT
	eEntitiesLWPolyline = 1024,      // 多段线				- 对应DXF图元: LWPOLYLINE
	eEntitiesPolyline = 2048,		 // 多段线曲线拟合		- 对应DXF图元: POLYLINE
	eEntitiesSpline = 4096,			 // 样条曲线			- 对应DXF图元: SPLINE
	// 3D图形
	eEntities3DFace = 8192,        // 3D面片 - 对应DXF图元: 3DFACE,SOLID,TRACE
	eEntitiesVertex = 16384,        // 索引3D顶点模型  - 对应DXF图元：VERTEX

	eEntitiesMText = 32768,          // 文本   - 对应DXF图元: MTEXT
	eEntitiesBlock = 65536,          // 块   
	eEntitiesText = 131072,			 // 文本     - 对应DXF图元: TEXT
};

typedef struct tagEntitiesBase
{
	virtual EntitiesType GetEntitiesType()const = 0;   // 获取图元类型
}CEntitiesBase;

typedef struct tagEntitiesArc : public tagEntitiesBase
{
	sc2Vector m_Center;       // 中心点
	double    m_dRadius;      // 半径
	double    m_dStartAngle;  // 起始角度
	double    m_dEndAngle;    // 终止角度

	tagEntitiesArc()
	{
		m_Center = sc2Vector(0,0);
		m_dRadius = 0.0;
		m_dStartAngle = 0.0;
		m_dEndAngle = 0.0;
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesArc;}
}CEntitiesArc;

typedef struct tagEntitiesCircle : public tagEntitiesBase
{
	sc2Vector m_Center;       // 中心点
	double    m_dRadius;      // 半径

	tagEntitiesCircle()
	{
		m_Center = sc2Vector(0,0);
		m_dRadius = 0.0;
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesCircle;}
}CEntitiesCircle;

typedef struct tagEntitiesEllipse : public tagEntitiesBase
{
	sc2Vector m_Center;             // 中心点
	sc2Vector m_LongAxisEndPoint;  // 长轴端点相对于中心点的坐标
	double    m_dRadiusRatio;       // 短轴与长轴的比例

	tagEntitiesEllipse()
	{
		m_Center = sc2Vector(0.0,0.0);
		m_LongAxisEndPoint = sc2Vector(0.0,0.0);
		m_dRadiusRatio = 1.0;
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesEllipse;}
}CEntitiesEllipse;

typedef struct tagEntitiesEllipseArc : public tagEntitiesBase
{
	sc2Vector m_Center;             // 中心点
	sc2Vector m_LongAxisEndPoint;   // 相对于中心的长轴端点的
	double    m_dRadiusRatio;       // 短轴与长轴的比例
	double    m_dStartAngle;        // 起始角度
	double    m_dEndAngle;          // 终止角度

	tagEntitiesEllipseArc()
	{
		m_Center = sc2Vector(0.0,0.0);
		m_LongAxisEndPoint = sc2Vector(0.0,0.0);
		m_dRadiusRatio = 1.0;
		m_dStartAngle = 0.0;
		m_dEndAngle = 0.0;
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesEllipseArc;}

}CEntitiesEllipseArc;

typedef struct tagEntitiesLine : public tagEntitiesBase
{
	sc2Vector m_StartPoint;        // 起止点
	sc2Vector m_EndPoint;          // 终止点

	tagEntitiesLine()
	{
		m_StartPoint = sc2Vector(0,0);
		m_EndPoint   = sc2Vector(0,0);
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesLine;}
}CEntitiesLine;

typedef struct tagEntitiesPoint : public tagEntitiesBase
{
	sc2Vector m_Point;  // 点坐标

	tagEntitiesPoint()
	{
		m_Point = sc2Vector(0,0);
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesPoint;}
}CEntitiesPoint;

typedef struct tagEntitiesContour : public tagEntitiesBase
{
	std::vector<std::vector<sc2Vector>> m_vPoints;

	virtual EntitiesType GetEntitiesType()const {return eEntitiesContour;}
}CEntitiesContour;

typedef struct tagEntitiesRay : public tagEntitiesBase
{
	sc2Vector m_EndPoint;   // 端点
	sc2Vector m_Direction;  // 方向

	tagEntitiesRay()
	{
		m_EndPoint = sc2Vector(0,0);
		m_Direction = sc2Vector(0,0);
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesRay;}
}CEntitiesRay;

typedef struct tagEntitiesXline : public tagEntitiesBase
{
	sc2Vector m_OrigionPoint;   // 源点
	sc2Vector m_Direction;      // 方向

	tagEntitiesXline()
	{
		m_OrigionPoint = sc2Vector(0,0);
		m_Direction    = sc2Vector(0,0);
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesXline;}
}CEntitiesXline;

typedef struct tagEntitiesInsert : public tagEntitiesBase
{
	sc2Vector m_RelativePoint;     // 相对坐标
	double m_dAngle;			  // 旋转角度
	sc2Vector m_zoom;			  // 缩放系数
	CString m_strBlockName;		  // 参照/阵列块的名称
	tagEntitiesInsert()
	{
		m_RelativePoint = sc2Vector(0,0);
		m_dAngle = 0;
		m_zoom = sc2Vector(1,1);
		m_strBlockName = ""; 
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesInsert;}
}CEntitiesInsert;

typedef struct tagEntitiesLWPolyline : public tagEntitiesBase
{
	std::vector<sc2Vector> m_vPoints;;   // 坐标
	std::vector<double> m_vCrown;		 // 圆弧凸度，直线的凸度为0
	bool m_bFlagOpenClose;               // fasle — 开 , ture —关
	
	tagEntitiesLWPolyline()
	{
		m_bFlagOpenClose = false;
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesLWPolyline;}
}CEntitiesLWPolyline;

typedef struct tagEntitiesPolyline : public tagEntitiesBase
{
	std::vector<sc2Vector> m_vFitPoints;;			// 拟合顶点坐标
	std::vector<sc2Vector> m_vControlPoints;		// 控制点坐标
	bool m_bFlagOpenClose;							// fasle — 开 , ture —关

	tagEntitiesPolyline()
	{
		m_bFlagOpenClose = false;
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesPolyline;}
}CEntitiesPolyline;

typedef struct tagEntitiesSpline : public tagEntitiesBase
{
	std::vector<double> m_vNode;					// 节点
	std::vector<sc2Vector> m_vControlPoints;		// 控制点坐标
	std::vector<sc2Vector> m_vFitPoints;;			// 拟合点坐标
	bool m_bFlagOpenClose;							// fasle — 开 , ture —关
	int m_nOrder;									// 样条曲线阶数
	double m_dNodeTolerance;						// 节点公差
	double m_dControlTolerance;						// 控制点公差
	double m_dFitTolerance;							// 拟合公差
	std::vector<double> m_vdWeight;					// 权重
	tagEntitiesSpline()
	{
		m_bFlagOpenClose = false;
		m_nOrder = 3;
		m_dNodeTolerance = 0.0;
		m_dControlTolerance = 0.0;
		m_dFitTolerance = 0.0;
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesSpline;}
}CEntitiesSpline;

typedef struct tagEntities3DFace : public tagEntitiesBase
{
	sc3Vector m_vPoints[4]; // 面片顶点
	int m_nEdgeVisibleFlag; // Entities3DFace的不可见边标志， 默认值为0，1：第一条边不可见，2：第二条边不可见，4：第三条边不可见，8：第四条边不可见 
	tagEntities3DFace()
	{
		for(int i = 0;i < 4;i++)
		{
			m_vPoints[i] = sc3Vector(0,0,0);
			m_nEdgeVisibleFlag = 0;
		}
	}
	virtual EntitiesType GetEntitiesType()const {return eEntities3DFace;}
}CEntities3DFace;

typedef struct tagEntitiesVertex : public tagEntitiesBase
{
	std::vector<sc3Vector> m_vPoints; // 索引顶点
	std::vector<int> m_vPointsIndex;  // 索引值,4点表示一个面

	virtual EntitiesType GetEntitiesType()const {return eEntitiesVertex;}
}CEntitiesVertex;

typedef struct tagEntitiesMText : public tagEntitiesBase
{
	sc2Vector m_OrigionPoint;       // 文本输出位置
	double	  m_dRotation;         // 文本输出的旋转角度,单位弧度
	CString   m_strText;           // 文本
	double    m_dHeight;           // 文本高度
	double    m_dWidth;			   // 文本参考矩形宽度
	int		  m_nPos;			   // 附着点
	CString   m_strTextStyle;      // 文本样式名 
	tagEntitiesMText()
	{
		m_OrigionPoint = sc2Vector(0,0);
		m_dRotation     = 0;
		m_strText       = "";
		m_dHeight       = 0.0;
		m_dWidth        = 0.0;
		m_nPos          = 1;
		m_strTextStyle  = "标准";
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesMText;}
}CEntitiesMText;

typedef struct tagEntitiesText : public tagEntitiesBase
{
	sc2Vector m_OrigionPoint;       // 文本输出位置
	double	  m_dRotation;         // 文本输出的旋转角度
	CString   m_strText;           // 文本
	double    m_dHeight;           // 文本高度
	int		  m_nPos;			   // 附着点
	CString   m_strTextStyle;      // 文本样式名 
	int       m_nHorType;          // 水平对正类型
	int       m_nVerType;		   // 垂直对正类型
	tagEntitiesText()
	{
		m_OrigionPoint = sc2Vector(0,0);
		m_dRotation     = 0;
		m_strText       = "";
		m_dHeight       = 0.0;
		m_nPos          = 7;
		m_strTextStyle  = "标准";
		m_nHorType      = 0;
		m_nVerType      = 0;
	}

	virtual EntitiesType GetEntitiesType()const {return eEntitiesText;}
}CEntitiesText;

CEntitiesBase* CreateEntities(EntitiesType entitiesType);
void DeleteEntities(CEntitiesBase* entities);
bool CloneEntities(const CEntitiesBase* src,CEntitiesBase* dst);
bool CreateAndCloneEntities(const CEntitiesBase* src,CEntitiesBase* &dst);

#pragma pack(pop)