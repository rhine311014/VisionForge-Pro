// vsXMLConfigurator.h: interface for the vcXMLConfigurator class.
//

#pragma once

#include "svDef.h"

#pragma pack(push,8)

#define XML_ELEM_NAME_LEN	128
#define XML_DATA_CACHE_NUM	16

// XML文件读写类
class SVCORE_API vcXMLConfigurator
{
public:
	vcXMLConfigurator();
	virtual ~vcXMLConfigurator();

public:
	bool Load(LPCTSTR strXMLFilePath);										// 加载文件
	bool Save(LPCTSTR strXMLFilePath, int iEncodeMode=0);					// 保存文件,iEncodeMode:0,UTF-8;1,UTF-16
	bool SetDoc(LPCTSTR strDoc);											// 设置文档内容
	bool FindElem(LPCTSTR strElem = NULL);									// 查找Elem
	bool ResetDocElemPos();													// 重置整个文档的标签位置	
	bool ResetCurElemPos();													// 重置父标签中的子标签位置
	bool EnterElem();														// 进入Elem
	bool LeaveElem();														// 退出Elem
	bool HasChild();														// 当前元素是否有子节点
	bool HasAttribute();													// 当前元素是否有属性

	////////////////////元素相关/////////////////////////////
	// 增加元素
	bool AddElem(LPCTSTR strElem, LPCTSTR strData = NULL);					// 添加Elem属性
	bool AddElem(LPCTSTR strElem, int nValue);								// 添加Elem(int型)
	bool AddElem(LPCTSTR strElem, double dValue);							// 添加Elem(double型)
	// 设置元素内容
	bool SetElemData(LPCTSTR strData);										// 设置当前Elem的Data(LPCTSTR型)
	bool SetElemData(int nValue);											// 设置当前Elem的Data(int型)
	bool SetElemData(double dValue);										// 设置当前Elem的Data(double型)
	// 获取元素内容
	LPCTSTR GetElemData();													// 获取当前Elem的Data(LPCTSTR型)
	bool GetElemData(int &nValue);											// 获取当前Elem的Data(int型)
	bool GetElemData(double &dValue);										// 获取当前Elem的Data(double型)	
	// 删除元素
	bool DeleteElem(LPCTSTR strElem = NULL);								// 删除Elem
	// 获取元素标签名称
	LPCTSTR GetElemTagName();												// 获取当前Elem的标签名称

	////////////////////元素属性相关/////////////////////////////
	int  GetAttributeCnt();													// 获取属性个数
	LPCTSTR GetAttributeName(int iIndex);									// 获取属性名称
	// 增加属性
	bool AddAttribute(LPCTSTR strAttrib, LPCTSTR strData);					// 添加属性(LPCTSTR型)
	bool AddAttribute(LPCTSTR strAttrib, int nValue);						// 添加属性(int型)
	bool AddAttribute(LPCTSTR strAttrib, double dValue);					// 添加属性(double型)
	// 设置属性
	bool SetAttribute(LPCTSTR strAttrib, LPCTSTR strData);					// 设置属性值(LPCTSTR型)
	bool SetAttribute(LPCTSTR strAttrib, int nValue);						// 设置属性(int型)
	bool SetAttribute(LPCTSTR strAttrib, double dValue);					// 设置属性(double型)
	// 获取属性值
	LPCTSTR GetAttribute(LPCTSTR strAttrib);								// 获取属性值(LPCTSTR型)
	bool GetAttribute(LPCTSTR strAttrib, int &nValue);						// 获取属性值(int型)
	bool GetAttribute(LPCTSTR strAttrib, double &dValue);					// 获取属性值(double型)

private:
	void *m_pXmlHandler; // XML读取类	

	LPTSTR		m_apszDataCache[XML_DATA_CACHE_NUM];
	int			m_anCacheLen[XML_DATA_CACHE_NUM];
	int	m_nCacheIdx;
};

#pragma pack(pop)
