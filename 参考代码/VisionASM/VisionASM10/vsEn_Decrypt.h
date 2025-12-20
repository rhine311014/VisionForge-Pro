//vision Encrypt and Decrypt file

#include "Afx.h"
#include "math.h"
#include <vector>

using namespace std;

CString Encryptstr(CString strID,CString strDate);
BOOL Decryptstr(const CString strin,CString strID,CString &strDate);

//是否是闰年
bool IsLeap(int year);
//上面的StringToDate函数用于取出日期中的年月日并判断日期是否合法
//从字符中最得年月日 规定日期的格式是yyyy-mm-dd
bool StringToDate(CString date, int& year, int& month, int& day);
//DayInYear能根据给定的日期，求出它在该年的第几天，代码如下
int DayInYear(int year, int month, int day);
int DaysBetween2Date(CString date1, CString date2);

BOOL  CheckIfDateInvalid(CString strDate);
