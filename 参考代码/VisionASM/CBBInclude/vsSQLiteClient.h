#pragma once
#include "vsDbClient.h"


#ifdef DbClientDll_Export 
#define DbClient_API _declspec(dllexport)
#else
#define DbClient_API _declspec(dllimport)
#endif



class DbClient_API CSQLiteClient : public CDbClient
{
public:
	CSQLiteClient();

	~CSQLiteClient(void);

	//执行，返回结果
	//virtual bool Execute(CString strSQLCmdC,vector<vector<CData>>& vectortSelectResult);

	//获取用户表名称
	//virtual vector<CString> GetUserTableName();
	//获取字段名称
	//virtual vector<CString> GetColName(CString Table_NameC);

	//获取数据库类型
	virtual DbType GetDbType();
};