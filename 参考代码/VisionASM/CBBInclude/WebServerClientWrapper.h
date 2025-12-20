#pragma once

#ifdef WebServerWrapper_Exports
#define WebServerWrapper_API __declspec(dllexport)
#else
#define WebServerWrapper_API __declspec(dllimport)
#endif

class WebServerWrapper_API CWebServerClientWrapper
{
public:
	CWebServerClientWrapper();
	~CWebServerClientWrapper();

	//创建接口
	bool CreateWebServiceMethod(CString strUrl, CString strMethodName, CString strSpaceName = "", CString strClassName = "");

	//调用接口
	bool CallWebServiceMethod(CString strUrl, CString strMethodName, CString strParam, CString& strReturn, CString strSpaceName = "", CString strClassName = "");

	//清空所有接口
	void ClearMethod();

	//获取错误信息
	CString GetErrorInfo();

private:
	void *m_cWebServerClient;
	CString m_strErrorInfo;
};