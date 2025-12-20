
#pragma once

#include<vector>
using namespace std;

#ifdef DbClientDll_Export 
#define DbClient_API _declspec(dllexport)
#else
#define DbClient_API _declspec(dllimport)
#endif

enum DbType
{
	eSqlServer = 0,
	eOracle = 1,
	eSQLite = 2,
};
enum SearchAttrib
{
	eUnknown = 0,
	/// <summary>
	/// 目标值=m_strValue
	/// </summary>
	eEqual = 1,
	/// <summary>
	/// 目标值>m_strValue
	/// </summary>
	eGreater = 2,
	/// <summary>
	/// 目标值<m_strValue
	/// </summary>
	eLess = 3,
	/// <summary>
	/// 目标值>=m_strValue
	/// </summary>
	eGreaterOrEqual = 4,
	/// <summary>
	/// 目标值<=m_strValue
	/// </summary>
	eLessOrEqual = 5,
	/// <summary>
	/// Sql语句between,m_strMinValue与m_strMaxValue二者之间
	/// </summary>
	eBetween = 6,
	/// <summary>
	/// 不等于  目标值<>m_strValue
	/// </summary>
	eNotEqual = 7
};

typedef  struct tagData
{
public:
	tagData()
	{
		m_strKey = _T("");

		m_strValue = _T("");
	}
	//字段
	CString m_strKey;
	//值
	CString m_strValue;
}CData;


typedef struct tagQueryConditions
{
public:
	tagQueryConditions()
	{
		m_strKey = _T("");

		m_strValue = _T("");

		m_strMaxValue = _T("");

		m_strMinValue = _T("");

		m_enumSearchAttrib = eUnknown;

	}
	//字段
	CString m_strKey;
	//字段对应的值
	CString m_strValue;
	//上限
	CString m_strMaxValue;
	//下限
	CString m_strMinValue;
	//查询属性
	SearchAttrib m_enumSearchAttrib;
}CQueryConditions;


typedef struct tagErrorInformation
{
public:
	tagErrorInformation()
	{
		m_nErrorNum =0;

		m_strErrorInfo = _T("");
	}
	//错误号
	int m_nErrorNum ;
	//错误信息
	CString m_strErrorInfo;
}CErrorInformation;

//连接信息
typedef struct tagConnInfor
{
public:
	tagConnInfor()
	{
		m_strServerAddress = _T("");

		m_strUser = _T("");

		m_strPwd = _T("");

		m_strDataBase = _T("");

		m_IntegratedSecurity = false;

		m_strConnectionTimeout = _T("15");

		m_strCommandTimeout = _T("30");

	}
	//服务器地址
	CString  m_strServerAddress;
	//用户名称
	CString  m_strUser;
	// 用户密码
	CString  m_strPwd;
	// 数据库名称
	CString  m_strDataBase;
	//是否Windows身份验证的方式，默认Sql Server身份验证
	bool  m_IntegratedSecurity;
	//连接超时设置
	CString  m_strConnectionTimeout;
	//执行超时设置
	CString  m_strCommandTimeout;
}CConnInfor;




class DbClient_API CDbClient
{
public:
	CDbClient();

	~CDbClient(void);

public:

	void * m_pvsDbClient;
	//打开连接
	virtual bool Open(CConnInfor  connInfor);
	//判断连接状态
	virtual bool IsOpen();
	//关闭连接
	virtual bool Close();
	//获取上一次执行的Sql语句
	virtual CString GetSqlCmd();
	//获取错误信息
	virtual CErrorInformation  GetErrorInformation();
	//插入
	virtual bool Insert(CString strTableName,vector<CData> vectorData);
	//删除
	virtual bool Delete(CString strTableNameC,vector<CQueryConditions> vectorQueryConditions ,int& nDeleteNum);
	//更新
	virtual bool Update(CString strTableNameC,vector<CQueryConditions> vectorQueryConditions ,vector<CData> vectorData,int& nUpdateNum);
	//查询
	virtual bool Query(CString strTableNameC,vector<CQueryConditions> vectorQueryConditions ,vector<CString> vectorstrkeys,vector<vector<CData>>& vectortSelectResult);
	//执行
	virtual bool Execute(CString strSQLCmdC,int& nInspactNum);
	//执行，返回结果
	virtual bool Execute(CString strSQLCmdC,vector<vector<CData>>& vectortSelectResult);

	//获取用户表名称
	virtual vector<CString> GetUserTableName();
	//获取字段名称
	virtual vector<CString> GetColName(CString Table_NameC);
	//获取数据库类型
	virtual DbType GetDbType() = 0;

};