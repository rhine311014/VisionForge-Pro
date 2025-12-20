#pragma once

//注册码编码
//输入：加密锁ID，授权产生日期，授权到期日期
//输出：注册码
//返回值：编码成功返回TRUE；否则返回FALSE
BOOL EncodeRegisterString(CString strID, CString strGenerateDate, CString strExpireDate, CString& strReg);

//注册码解码
//输入：注册码，加密锁ID
//输出：授权产生日期，授权到期日期
//返回值：解码成功返回TRUE；否则返回FALSE
BOOL DecodeRegisterString(CString strReg, CString strID, CString& strGenerateDate, CString& strExpireDate);