#include "stdafx.h"
#include <vector>
#include "vsRegisterString.h"

BOOL EncodeRegisterString(CString strID, CString strGenerateDate, CString strExpireDate, CString& strReg)
{
	if (strID.GetLength() < 5)
	{
		return FALSE;
	}
	if (strGenerateDate.GetLength() != 8)
	{
		return FALSE;
	}
	if (strExpireDate.GetLength() != 8)
	{
		return FALSE;
	}

	CString strOut;

	//ID
	std::vector<int> nID;
	for (int i=0;i<strID.GetLength();i++)
	{
		int ntemp = strID[i];
		nID.push_back(ntemp);
	}

	//日期
	int nDate[16];
	for (int i=0;i<8;i++)
	{
		nDate[i] = strGenerateDate[i];
		nDate[i+8] = strExpireDate[i];
	}

	//日期转换
	for (int i=0;i<16;i++)
	{
		int randNumber = rand()%3;
		switch (randNumber)
		{
		case 0:
			nDate[i]+=5;
			if (nDate[i]>57)
			{
				nDate[i]-=10;
			}
			break;
		case 1:
			nDate[i]+=49;
			break;
		case 2:
			nDate[i]+=33;
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

	int nCheckbit=0xFF;
	if (nID.size()>=16)
	{
		for (int i=0;i<nID.size();i++)
		{
			char nchar = nID.at(i);
			strOut+=nchar;
			nCheckbit= nCheckbit^nID.at(i);
			if (i<16)
			{
				nchar = nDate[i];
				strOut+=nchar;
				nCheckbit= nCheckbit^nDate[i];
			}
		}
	}
	else
	{
		for (int i=0;i<16;i++)
		{
			char nchar;
			if (i<nID.size())
			{
				nchar= nID.at(i);
				strOut+=nchar;
				nCheckbit= nCheckbit^nID.at(i);
			}			
			nchar = nDate[i];
			strOut+=nchar;
			nCheckbit= nCheckbit^ nDate[i];
		}
	}

	CString strCheck;
	strCheck.Format("%02X",nCheckbit);
	CString finalstr;
	finalstr = strCheck+strOut;

	strReg = finalstr;

	return TRUE;
}

BOOL DecodeRegisterString(CString strReg, CString strID, CString& strGenerateDate, CString& strExpireDate)
{
	if (strID.GetLength() < 5)
	{
		return FALSE;
	}

	if ((strID.GetLength()+18) != strReg.GetLength() )
	{
		return FALSE;
	}

	//校验
	CString strCheckbit;
	strCheckbit = strReg.Mid(0,2);

	int nCheckbit =0XFF;
	std::vector<int> nCode;
	for (int i=2;i<strReg.GetLength();i++)
	{
		int nchar = strReg[i];
		nCheckbit= nCheckbit^nchar;
		nCode.push_back(nchar);
	}
	CString strcheck;
	strcheck.Format("%02X",nCheckbit);

	if (strCheckbit!=strcheck)
	{
		return FALSE;
	}

	//ID转换
	CString strDeDate,strDeID;
	char Encodechars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

	if (strID.GetLength()>=16)
	{
		for (int i=0;i<strID.GetLength();i++)
		{
			int nchar;
			char temp;
			if (i<16)
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
				nchar = nCode.at(i+16);				
				temp= nchar;
				strDeID+=temp;
			}	
		}
	}
	else
	{
		for (int i=0;i<16;i++)
		{
			int nchar;
			char temp;
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
			return FALSE;
		}
		char nchar;
		nchar = ntemp;
		strdatetemp+= nchar;
	}
	strGenerateDate = strdatetemp.Left(8);
	strExpireDate = strdatetemp.Right(8);

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
		return FALSE;
	}

	return TRUE;
}