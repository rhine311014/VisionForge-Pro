#pragma once
/*
* 函数名  ：MotionData
* 函数功能：计算数据
* 输入参数：Xt 角度  ;  R2 界面输入参数1（吸附位置W）  ;   R界面输入参数2(R角)    
* 输出参数：YN输出参数,ZN输出参数
* 返回值  ：执行成功返回1  ,失败返回-1
* 
*/
__declspec(dllexport) int MotionData(const double Xt, const double R2, const double R, double &YN, double &ZN);


