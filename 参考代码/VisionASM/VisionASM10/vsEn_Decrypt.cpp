#include "StdAfx.h"
#include "vsEn_Decrypt.h"


//加密
CString Encryptstr(CString strID,CString strDate)
{
	CString EncOutString;
	
	EncOutString.Empty();

	if (strID.GetLength()<5)
	{
		AfxMessageBox("ID错误！");
		return EncOutString;
	}
	if (strlen(strDate)!= 8)
	{
		AfxMessageBox("Input error");
		return EncOutString;
	}



	std::vector<int> nID;
	int n[8];
	for (int i=0;i<8;i++)
	{
		n[i] = strDate[i];

	}
	for (int j=0;j<strID.GetLength();j++)
	{
		int ntemp = strID[j];
		nID.push_back(ntemp);
	}


	//process

	int nCheckbit=0xFF;
	{
		//日期转换
		for (int i=0;i<8;i++)
		{
			int randNumber = rand()%3;
			switch (randNumber)
			{
			case 0:
				n[i]+=5;
				if (n[i]>57)
				{
					n[i]-=10;
				}
				break;
			case 1:
				n[i]+=49;
				break;
			case 2:
				n[i]+=33;
				break;
			default:
				break;

			}

		}
		//ID转换
		char Encodechars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
		for (int j=0;j<nID.size();j++)
		{
			for (int k=0;k<63;k++)
			{
				int nchar = Encodechars[k];
				if (nchar==nID.at(j))
				{
					k=k+10+ pow((float)(nID.size()-j),2);
					if (k>=63)
					{
						k=k%63;
					}
					nID.at(j) = Encodechars[k];
					break;
				}
			}


		}
	}



	if (nID.size()>=8)
	{
		for (int i=0;i<nID.size();i++)
		{
			char nchar = nID.at(i);
			EncOutString+=nchar;
			nCheckbit= nCheckbit^nID.at(i);
			if (i<8)
			{
				nchar = n[i];
				EncOutString+=nchar;
				nCheckbit= nCheckbit^ n[i];
			}
		}

	}
	else
	{
		for (int i=0;i<8;i++)
		{
			char nchar;
			if (i<nID.size())
			{
				nchar= nID.at(i);
				EncOutString+=nchar;
				nCheckbit= nCheckbit^ nID.at(i);
			}			
			nchar = n[i];
			EncOutString+=nchar;
			nCheckbit= nCheckbit^ n[i];

		}

	}

	CString strCheck;
	strCheck.Format("%02X",nCheckbit);
	CString finalstr;
	finalstr = strCheck+EncOutString;
	EncOutString = finalstr;



	return EncOutString;

}
//解密
BOOL Decryptstr(const CString strin,const CString strID,CString &strDate)
{
	CString DecOutString;

	DecOutString.Empty();

	if (strID.GetLength()<5)
	{
		//AfxMessageBox("ID错误！");
		return FALSE;
	}

	if ((strID.GetLength()+10)!= strin.GetLength() )
	{
		//AfxMessageBox("注册码错误！");
		return FALSE;

	}
	int nCheckbit =0XFF;
	CString strCheckbit;
	std::vector<int> nCode;
	strCheckbit = strin.Mid(0,2);

	for (int i=2;i<strin.GetLength();i++)
	{
		int nchar = strin[i];
		nCheckbit= nCheckbit^nchar;
		nCode.push_back(nchar);

	}
	CString strcheck;
	strcheck.Format("%02X",nCheckbit);
	if (strCheckbit!=strcheck)
	{
		//AfxMessageBox("注册码错误！");
		return FALSE;
	}

	CString strDeDate,strDeID;
	char Encodechars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";


	if (strID.GetLength()>=8)
	{
		for (int i=0;i<strID.GetLength();i++)
		{
			int nchar;char temp;
			if (i<8)
			{
				nchar = nCode.at(i*2);				
				temp= nchar;
				strDeID+=temp;

				nchar =nCode.at(i*2+1);
				temp= nchar;
				strDeDate+=temp;
			}
			else
			{
				nchar = nCode.at(i+8);				
				temp= nchar;
				strDeID+=temp;

			}	
		}
	}
	else
	{
		for (int i=0;i<8;i++)
		{
			int nchar;char temp;
			if (i<strID.GetLength())
			{
				nchar = nCode.at(i*2);				
				temp= nchar;
				strDeID+=temp;

				nchar =nCode.at(i*2+1);
				temp= nchar;
				strDeDate+=temp;
			}
			else
			{
				nchar = nCode.at(i+strID.GetLength());				
				temp= nchar;
				strDeDate+=temp;

			}			


		}

	}


	CString strdatetemp;
	for (int i=0;i<strDeDate.GetLength();i++)
	{
		int ntemp = strDeDate[i];
		if (ntemp>=48 &&ntemp<=57)
		{
			ntemp-=5;
			if (ntemp<48)
			{
				ntemp+=10;
			}
		}
		else if (ntemp>=97 && ntemp<=121)
		{
			ntemp-=49;
		}
		else if (ntemp>=65 && ntemp<=90)
		{
			ntemp-=33;
		}
		else
		{
			//AfxMessageBox("注册码错误！");
			return FALSE;
		}
		char nchar;
		nchar = ntemp;
		strdatetemp+= nchar;

	}
	strDate = strdatetemp;

	CString strIDtemp;
	for (int i=0;i<strDeID.GetLength();i++)
	{
		int nchartemp= strDeID[i];
		for (int k=0;k<63;k++)
		{
			int nchar = Encodechars[k];
			if (nchar==nchartemp)
			{
				k=k-10- pow((float)(strDeID.GetLength()-i),2);
				while(k<0)
				{
					k=k+63;
				}
				nchartemp = Encodechars[k];
				break;
			}
		}
		char chartemp = nchartemp;;
		strIDtemp+=chartemp;

	}
	strDeID = strIDtemp;



	if (strID!= strDeID)
	{
		//AfxMessageBox("注册码错误");
		return FALSE;
	}


	return TRUE;

}

//IsLeap函数判断一个年份是否为闰年，方法如下:
bool IsLeap(int year)
{
	return (year % 4 ==0 || year % 400 ==0) && (year % 100 !=0);
}

//上面的StringToDate函数用于取出日期中的年月日并判断日期是否合法
//从字符中最得年月日 规定日期的格式是yyyy-mm-dd
bool StringToDate(CString date, int& year, int& month, int& day)
{
	year = atoi((date.Mid(0,4)));
	month = atoi((date.Mid(4,2)));
	day = atoi((date.Mid(6,2)));
	int DAY[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	if(IsLeap(year)){
		DAY[1] = 29;
	}
	return year >= 0 && month<=12 && month>0 && day<=DAY[month-1] && day>0;
}

//DayInYear能根据给定的日期，求出它在该年的第几天，代码如下
int DayInYear(int year, int month, int day)
{
	//int _day = 0;
	int DAY[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	if(IsLeap(year))
		DAY[1] = 29;
	for(int i=0; i<month - 1; ++i)
	{
		day += DAY[i];
	}
	return day;
}

int DaysBetween2Date(CString date1, CString date2)
{
	//取出日期中的年月日
	int year1, month1, day1;
	int year2, month2, day2;
	if(!StringToDate(date1, year1, month1, day1) || !StringToDate(date2, year2,month2,day2))
	{
		return -1;
	}
	if(year1 == year2 && month1 == month2)
	{
		return  day1 - day2;

		//如果年相同
	}else if(year1 == year2)
	{
		int d1, d2;
		d1 = DayInYear(year1, month1, day1);
		d2 = DayInYear(year2, month2, day2);
		return d1 - d2 ;

		//年月都不相同
	}else{
		//确保year1年份比year2大
		if(year1 < year2)
		{
			return -1;
		}
		int d1,d2,d3;
		if(IsLeap(year2))
			d1 = 366 - DayInYear(year2,month2, day2); //取得这个日期在该年还于下多少天
		else
			d1 = 365 - DayInYear(year2,month2, day2);
		d2 = DayInYear(year1,month1,day1); //取得在当年中的第几天
		d3 = 0;

		for(int year = year2 + 1; year < year1; year++)
		{
			if(IsLeap(year))
				d3 += 366;
			else
				d3 += 365;
		}
		return d1 + d2 + d3;
	}
}

BOOL CheckIfDateInvalid(CString strDate)
{
	if (strDate.GetLength()!=8)
	{
		return FALSE;
	}

	for (int i=0;i<8;i++)
	{
		int nval = strDate[i];
		if (nval<48 || nval>57)
		{
			return FALSE;
		}
	}

	CString strCurtime;
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	strCurtime.Format("%d%02d%02d",curTime.wYear,curTime.wMonth,curTime.wDay);

	int ndeltadate = DaysBetween2Date(strDate,strCurtime);

	if (ndeltadate<0)
	{		
		return FALSE;
	}

	return TRUE;
}
