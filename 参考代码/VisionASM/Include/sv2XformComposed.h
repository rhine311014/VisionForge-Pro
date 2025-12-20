#pragma once

#include "svDef.h"
#include "svError.h"
#include "svMath.h"
#include "svVector.h"
#include "svMatrix.h"
#include "sv2XformBase.h"
#include <vector>
#include <cassert>

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            二维组合变换sc2XformComposed
// ---------------------------------------------------------------------
// 二维组合变换由若干个二维变换组成。

class SVCORE_API sc2XformComposed : public sc2XformBase
{
public:
	//默认构造函数，构造一个由单一单位变换组成的组合变换
	sc2XformComposed();

	//构造一个由xfm变换组成的组合变换
	//如果copy为true，构造函数对xfm进行深拷贝；否则，只是保存xfm的指针。
	//如果copy为false，必须确保xfm为在堆上动态分配而来，并确保它不被改变或提前销毁。（推荐用户一般情况设置copy为true）
	sc2XformComposed(const sc2XformBase& xfm, bool copy = true);

	//构造一个由xfm1变换和xfm2变换组成的组合变换，<this> = <xfm1> * <xfm2>
	//关于copy1和copy2的相关情况参见前面描述
	sc2XformComposed(const sc2XformBase& xfm1, const sc2XformBase& xfm2, bool copy1 = true, bool copy2 = true);

	//组合当前变换与变换other，<this> = <other> * <this>
	//关于copy的相关情况参见前面描述
	void PreCompose(const sc2XformBase& other,  bool copy = true);

	//组合当前变换与变换other，<this> = <this> * <other>
	//关于copy的相关情况参见前面描述
	void PostCompose(const sc2XformBase& other,  bool copy = true);

	//颠倒该组合变换的组合顺序
	void Invert();

	// ------重载基类的虚函数------ //

	//返回当前变换是否线性变换
	virtual bool IsLinear() const;

	//返回当前变换在给定点的最优线性变换
	virtual sc2XformLinear LinearXform(const sc2Vector& atThisPt) const;

	//映射一个二维坐标点
	virtual sc2Vector MapPoint(const sc2Vector& pt) const;

	//从左到右组合两个变换
	virtual sc2XformBasePtrh ComposeBase(const sc2XformBase& rhs) const;

	//逆变换
	virtual sc2XformBasePtrh InverseBase() const;

	//拷贝到一个新分配的对象
	virtual sc2XformBasePtrh Clone() const;

private:
	svStd vector<sc2XformBasePtrh_const> m_xformVector;
};


#pragma pack(pop)
