#pragma once

#include "vsCommIOBase.h"

typedef enum tagIOCardBrand
{
	eIOCradBrand_Unknown = -1,		// 未知品牌
	eIOCradBrand_ADLink = 0,		// ADLink

}EnumIOCardBrand;

typedef struct tagIOCardBrandInfo
{
	tagIOCardBrandInfo()
	{

	}

	CString m_strBrandName;
	std::map<int , CString> m_mapCardTypeInfo;
}CIOCardBrandInfo;

// 创建卡MAP
VS_COMM_IO_API void _CreateIOCardMap();
// 创建IO卡指针
VS_COMM_IO_API vcCommIOBase*  _CreateCommIO(EnumIOCardBrand  eType);
// 释放IO卡指针
VS_COMM_IO_API void _DeleteCommIO(vcCommIOBase*  pComm);

// <品牌, <卡型号 ， 卡名称> >
extern VS_COMM_IO_API std::map<EnumIOCardBrand, CIOCardBrandInfo > g_mapIOCardBrand;
