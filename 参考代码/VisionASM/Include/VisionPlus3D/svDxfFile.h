#pragma once

#include "svModelData.h"
#include "svDxfFileDef.h"
#include "svGuiGraphic.h"
#include "svGuiGraphicData.h"
#include "svShapeData.h"
#include <map>
#include <fstream>

#pragma pack(push,8)

typedef struct tagDXFEntitiesAttrib
{
	CString m_strGroupID;
	CString m_strLayerName;
	CString m_strLineType;
	int m_nLineWidth;
	int m_nColor;
	int m_nEdgeVisibleFlag;    // Entities3DFace的不可见边标志， 默认值为0，1：第一条边不可见，2：第二条边不可见，4：第三条边不可见，8：第四条边不可见
	CString m_strBlockName;
	int m_nRGBColor;         // 透明度
	tagDXFEntitiesAttrib()
	{
		m_strGroupID = "";
		m_strLayerName = "0";
		m_strLineType = "Continuous";
		m_nLineWidth = -3;
		m_nColor = 7;
		m_nEdgeVisibleFlag = 0;
		m_strBlockName = "";
		m_nRGBColor = 0;
	}

}CDXFEntitiesAttrib;

typedef struct tagDXFLayerAttrib
{
	CString m_strLayerName;
	CString m_strLineType;
	int m_nLineWidth;
	int m_nColor;
	int m_nFlags;   //图层标准标记

	tagDXFLayerAttrib()
	{
		m_strLayerName = "0";
		m_strLineType = "Continuous";
		m_nLineWidth = -3;
		m_nColor = 7;
		m_nFlags = 0;
	}

}CDXFLayerAttrib;

typedef struct tagDXFGroupAttrib
{
	CString m_strGroupName;
	CString m_strGroupID;

	tagDXFGroupAttrib()
	{
		m_strGroupName = "";
		m_strGroupID = "";
	}

}CDXFGroupAttrib;

typedef struct tagDXFBlockAttrib
{
	CString m_strGroupID;
	CString m_strLayerName;
	CString m_strBlockName;
	int m_nFlagType; 
	int m_nColor;

	tagDXFBlockAttrib()
	{
		m_strGroupID = "";
		m_strLayerName = "0";
		m_strBlockName = "";
		m_nFlagType = 0;
		m_nColor = 7;
	}

}CDXFBlockAttrib;

typedef struct tagDXFLineTypeAttrib
{
	CString m_strName;
	CString m_strDescription;
	int m_nFlag;
	int m_nNum;
	double m_dTotalLength;
	std::vector<double> m_vLength;   //虚线、点或空间长度

	tagDXFLineTypeAttrib()
	{
		m_strName = "";
		m_strDescription = "";
		m_nFlag = 0;
		m_nNum = 0;
		m_dTotalLength = 0.0;
	}
}CDXFLineTypeAttrib;

enum DxfVersion {
	AC1032,				// 2018
	AC1027,				// 2013
	AC1024,				// 2010
	AC1021,				// 2007
	AC1018,				// 2004
	AC1015,				// 2000
};


class SVDXFFILE_API scDxfFile
{
public:
	scDxfFile();
	~scDxfFile();

	// 读取dxf文件
	bool Init(const char* filePath);
	bool Init(const scModelData& model);
	bool Init(const scGuiGraphicData& guiGraphicData);
	bool Init(const scShapeData& shapeData);

	// 相近图形是否连接
	void ConnectEnable(bool bEnable);
	bool IsConnectEnable()const{return m_bConnectEnable;};

	// 相近图形连接距离阈值
	void SetConnectTolerance(double dTorlerance) {m_dTorlerance = dTorlerance;};
	double GetConnectTolerance()const {return m_dTorlerance;};

	// 获取所有模型数据
	scModelData GetModel()const;
	scModelData GetModel(bool bFlagBlockEntities)const;    // false — 块图元 ， true — 实体图元

	// 获取GUI图形、图形数据
	scGuiGraphicData GetGuiGraphicData();
	scGuiGraphicData GetGuiGraphicData(bool bFlagBlockEntities);
	scShapeData GetShapeData();
	scShapeData GetShapeData(bool bFlagBlockEntities);

	// 模型数据与Gui图形数据的转换
	void ModelToGuiGraphic(const scModelData& model,std::vector<CDXFEntitiesAttrib>& vEntitiesAttrib,scGuiGraphicData& guiGraphic);
	void GuiGraphicToModel(const scGuiGraphicData& guiGraphic,scModelData& model,std::vector<CDXFEntitiesAttrib>& vEntitiesAttrib);

	// 删除模型数据
	void Clear();

	// 获取所有组名称
	std::vector<CString> GetGroups()const;

	// 获取所有图层名称
	std::vector<CString> GetLayers()const;

	// 获取所有块名称
	std::vector<CString> GetBlocks()const;

	// 从组中获取图元、图元属性
	scModelData GetModelFromGroup(CString groupName)const;
	scModelData GetModelFromGroup(CString groupName, bool bFlagBlockEntities)const;
	std::vector<CDXFEntitiesAttrib> GetAttribFromGroup(CString groupName)const;
	std::vector<CDXFEntitiesAttrib> GetAttribFromGroup(CString groupName, bool bFlagBlockEntities)const;

	// 从图层中获取图元、图元属性
	scModelData GetModelFromLayer(CString LayerName)const;
	scModelData GetModelFromLayer(CString LayerName, bool bFlagBlockEntities)const;
	std::vector<CDXFEntitiesAttrib> GetAttribFromLayer(CString LayerName)const;
	std::vector<CDXFEntitiesAttrib> GetAttribFromLayer(CString LayerName, bool bFlagBlockEntities)const;

	// 从块中获取图元、图元属性
	scModelData GetModelFromBlock(CString BlockName)const;
	scModelData GetModelFromBlock(CString BlockName, bool bFlagBlockEntities)const;
	std::vector<CDXFEntitiesAttrib> GetAttribFromBlock(CString BlockName)const;
	std::vector<CDXFEntitiesAttrib> GetAttribFromBlock(CString BlockName, bool bFlagBlockEntities)const;


	// 获取图元数量
	int GetEntitiesNum()const;
	int GetEntitiesNum(bool bFlagBlockEntities)const;

	// 获取组数量
	int GetGroupNum()const;

	// 获取图层数量
	int GetLayerNum()const;

	// 获取块数量
	int GetBlockNum()const;

	//获取版本号
	DxfVersion GetVersion();

	// 获取组属性
	CDXFGroupAttrib GetGroupAttrib(int nIndex)const;
	std::vector<CDXFGroupAttrib> GetGroupAttrib()const;

	//获取图层属性
	CDXFLayerAttrib GetLayerAttrib(int nIndex)const;
	std::vector<CDXFLayerAttrib> GetLayerAttrib()const;

	// 获取图元属性
	CDXFEntitiesAttrib GetEntitiesAttrib(int nIndex)const;
	std::vector<CDXFEntitiesAttrib> GetEntitiesAttrib()const;

	CDXFEntitiesAttrib GetEntitiesAttrib(int nIndex,bool bFlagBlockEntities)const;
	std::vector<CDXFEntitiesAttrib> GetEntitiesAttrib(bool bFlagBlockEntities)const;
	
	// 获取块属性
	CDXFBlockAttrib GetBlockAttrib(int nIndex)const;
	std::vector<CDXFBlockAttrib> GetBlockAttrib()const;

	// 获取图元范围的最大、最小值
	sc2Vector GetGraphicMinValue();
	sc2Vector GetGraphicMaxValue();

	// 设置/ 获取读取图元的种类
	void SetEnableReadEntities(UINT32 entities){m_nEntities = entities;};
	UINT32 GetEnableReadEntities(){return m_nEntities;};

	// 将cad中图元的线型、颜色转换成gui
	void TransformEntitiesAttrib(CDXFEntitiesAttrib& entitiesAttrib, scGuiGraphic::StyleFlags& lineStyle,COLORREF& lineColor);

	// 将Gui中图形的线型、颜色转换成cad中图元属性
	void TransformGuiGraphic(const scGuiGraphic::StyleFlags& lineStyle,const COLORREF& lineColor,CDXFEntitiesAttrib& entitiesAttrib);


	// 写入dxf文件
	bool Write(const char* filePath,DxfVersion version = AC1032, bool bAscii = true);

private:
	void Connect(std::vector<CEntitiesBase*>& vDxfEntitiesData,std::vector<CDXFEntitiesAttrib>& vDxfEntitiesAttrib,int& nFlag);

	//读取组码和组值
	bool ReadDxfGroupsCode(char* buffer, std::vector<sc3Vector>& vPoints,std::vector<int>& vPointsIndex,int& nPointsNum,CString& strVertexGroupID,bool bFlagOpenClose,std::vector<double>& vLength,CEntitiesSpline& spline,std::vector<CString>& vStrName);

	//处理组码和组值
	bool ProcessDXFGroupsCode(int line,int groupCode, const CString& groupValue, std::vector<sc3Vector>& vPoints,std::vector<int>& vPointsIndex,int& nPointsNum,CString& strVertexGroupID,bool bFlagOpenClose,std::vector<double>& vLength,CEntitiesSpline& spline,std::vector<CString>& vStrName);
	//// 是否解析
	//bool EnableAnalysis(int groupCode, const CString& groupValue);
	// 从文件中获取一行
	bool GetStrippedLine(char* c,std::string& s, unsigned int size,/*char* buffer,*/ bool stripSpace = true);
	bool StripWhiteSpace(char** s, bool stripSpaces = true);

	// 从块中删除图元和图元属性
	void DeleteModelFromBlock(CString BlockName, bool bFlagBlockEntities);

	//添加版本号
	void AddVersion();

	//添加图层属性
	void AddLayerAtrrib();

	// 添加图元属性
	void AddEntitiesAttrib();

	// 添加块属性、块图元属性
	void AddBlock();
	void AddBlockEntitiesAttrib();

	//添加组
	bool AddGroup();

	// 添加线性定义信息
	void AddLineTypeAttrib(const std::vector<double>& vLength);
	bool HandleLineTypeAttrib(std::vector<double>& vLength,int& nPointsNum);

	// 添加2D图元数据
	void AddEntitiesArc();

	void AddEntitiesCircle();

	void AddEntitiesEllipse();

	void AddEntitiesEllipseArc();

	void AddEntitiesLine();

	void AddEntitiesPoint();

	void AddEntitiesContour();

	void AddEntitiesRay();

	void AddEntitiesXline();

	void AddEntitiesInsert(std::vector<CString>& vStrName);

	void AddEntitiesLWPolyline(std::vector<sc3Vector>& vPoints);

	bool HandleLWPolylineData(std::vector<sc3Vector>& vPoints,int& nPointsNum);

	void AddEntitiesSpline(CEntitiesSpline& spline);

	bool HandleSplineData(CEntitiesSpline& spline);

	//添加3D图元数据
	void AddEntities3DFace();

	// 添加文本
	void AddText();

	// 添加多行文本
	void AddMText();
	bool HandleMTextData(bool& bReadMTextData);

	// 添加3D图元属性、数据
	void AddEntitiesVertexAttrib(const CString& strVertexGroupID);      
	void AddEntitiesVertex(std::vector<sc3Vector>& vPoints,std::vector<int>& vPointsIndex);
	void EndEntitiesVertex(std::vector<sc3Vector>& vPoints,std::vector<int>& vPointsIndex,CString& strVertexGroupID,bool bFlagOpenClose);        // 结束当前vertex数据的添加

	int GetIntValue(int code, int def);
	int GetInt16Value(int code, int def);
	double GetRealValue(int code, double def);
	CString GetStringValue(int code, const CString def);

	// 加载xml文件
	bool LoadConfigFromFile(CString strXmlPath, int& nEnabledLog);    // nEnabledLog: 0 - 不生成日志， 1 - 日志只存储异常信息， 2 - 日志存储全部信息
	bool CreateDirectoryPro(CString strPath);

	//写dxf文件
	void DxfString(int gc, const char* value) const;
	void DxfString(int gc, const CString& value) const;
	void DxfReal(int gc, double value) ;
	void DxfInt(int gc, int value)const;
	void DxfHex(int gc, int value) const;
	void StrReplace(char* str, char src, char dest);
	// 处理句柄
	void ResetHandle() const; 
    unsigned long GetHandle(int gc=5) const;

	void WriteHeader(DxfVersion version);
	void WriteSection(const char* name) const;
	void WriteCoord(int gc, double x, double y, double z=0) ;

	// 写图元数据和属性
	void WritePoint(const CEntitiesPoint* data,const CDXFEntitiesAttrib& attrib);
	void WriteLine(const CEntitiesLine* data,const CDXFEntitiesAttrib& attrib);
	void WriteXline(const CEntitiesXline* data, const CDXFEntitiesAttrib& attrib);
	void WriteRay(const CEntitiesRay* data, const CDXFEntitiesAttrib& attrib);
	void WriteCircle(const CEntitiesCircle* data, const CDXFEntitiesAttrib& attrib);
	void WriteArc(const CEntitiesArc* data, const CDXFEntitiesAttrib& attrib);
	void WriteEllipse(const CEntitiesEllipse* data, const CDXFEntitiesAttrib& attrib);
	void WriteEllipseArc(const CEntitiesEllipseArc* data, const CDXFEntitiesAttrib& attrib);
	void WriteLWPolyline(const CEntitiesLWPolyline* data, const CDXFEntitiesAttrib& attrib);
	void WritePolyline(const CEntitiesPolyline* data, const CDXFEntitiesAttrib& attrib);
	void WriteSpline(const CEntitiesSpline* data, const CDXFEntitiesAttrib& attrib);
	void WriteVertex(const CEntitiesVertex* data, const CDXFEntitiesAttrib& attrib);
	void Write3DFace(const CEntities3DFace* data, const CDXFEntitiesAttrib& attrib);
	void WriteMText(const CEntitiesMText* data, const CDXFEntitiesAttrib& attrib);
	void WriteText(const CEntitiesText* data, const CDXFEntitiesAttrib& attrib);
	void WriteEntitiesAttrib(const CDXFEntitiesAttrib& attrib);
	void WriteEntities(const CEntitiesBase* data, const CDXFEntitiesAttrib& attrib);

	void WriteTable(const char* name, int num, int nHandle = 0);
	void WriteTableEntry(const CString& name, unsigned long int handle=0);
	void WriteTableVPort();
	void WriteTableLinetype(const CString& name);
	void WriteTableLinetype(const CDXFLineTypeAttrib& lineType);
	void WriteTableLayer(const CDXFLayerAttrib& attrib);
	void WriteTableStyle();
	void WriteTableAppid(const CString& name);
	void WriteTableDimStyle();
	void WriteTableBlockRecord();
	void WriteTableBlockRecord(const CString& name);
	void WriteBlock(const CDXFBlockAttrib& attrib);
	void WriteEndBlock(const CString& strLayerName,int nCorlor);
	void WriteBlockSpace(const CString& name);
	void WriteGroup(const std::vector<CDXFGroupAttrib>& vAttrib);
	void WriteObject();

public:
	void AddLWPolyline(const CEntitiesLWPolyline& lwPolyline);

private:
	std::vector<CEntitiesBase*> m_vDxfEntitiesData;             // 实体图元数据
	std::vector<CDXFEntitiesAttrib> m_vDxfEntitiesAttrib;       // 实体图元属性
	std::vector<CDXFLayerAttrib> m_vDxfLayerAttrib;				// 图层属性
	std::vector<CDXFGroupAttrib> m_vDxfGroupAttrib;				// 组属性
	int m_nGroupCode;											// 组码
	CString m_strGroupValue;									// 组值
	int m_nCurrentObjectType;									// 当前图元类型
	CString m_strVersion;										// dxf文件版本
	std::map<int, CString> m_vNormalValues;
	double m_dTorlerance;										// 相近图形连接阈值
	bool m_bConnectEnable;										// 相近图形是否连接

	bool m_bFlagBlockEntities;									 // false —— 块图元 ， true —— 实体图元
	std::vector<CEntitiesBase*> m_vDxfBlockEntitiesData;         // 块图元数据
	std::vector<CDXFBlockAttrib> m_vDxfBlockAttrib;              // 块属性
	std::vector<CDXFEntitiesAttrib> m_vDxfBlockEntitiesAttrib;   // 块图元属性
	sc2Vector m_vMinValue;                                       // 图形范围最小值
	sc2Vector m_vMaxValue;										 // 图形范围最大值
	std::vector<CDXFLineTypeAttrib> m_vLineType;				 // 线型属性

	UINT32 m_nEntities;                                          
	void* m_readDxfLogFile;
	int m_nEnabledLog;

	// 写dxf文件参数
	mutable std::ofstream m_ofile;
	mutable unsigned long m_lHandle;                                 // 句柄
};

#pragma pack(pop)