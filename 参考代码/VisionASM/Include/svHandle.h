// svHandle.h

#pragma once

#include "svdef.h"
#include "svInterlock.h"
#include <cassert>

#pragma pack(push,8)


// ---------------------------------------------------------------------
//                            指针句柄
// ---------------------------------------------------------------------
// 指针句柄是可以对在堆内存上分配的对象进行引用计数的指针类.指针句柄不仅
// 可以像指针那样工作,还可以对被关联对象进行引用计数.指针句柄可以确保在堆
// 内存上分配的关联对象得到正确的释放.



// 释放内存，仅用于内部
template <typename T> inline void sfRelease(T* p) { delete p; }


// 引用减1计数,仅用于内部
template <class T> inline long sfRefDec(T* rep)
{ assert(rep); return sfInterlockedDecrement(&rep->m_lRefc);}


// 引用加1计数,仅用于内部
template <class T> inline long sfRefInc(T* rep)
{ assert(rep); return sfInterlockedIncrement(&rep->m_lRefc);}


template <class T> class scPtrHandleBase;
template <class T> class scPtrHandleBase_const;


// 类scRepBase用于作为被引用类的基类,以便于指针句柄类访问被引用类
class SVCORE_API scRepBase
{
public:	
	// 返回当前对象被引用的数量
	long Refc() const {return m_lRefc;}
	// 析构函数
//	virtual ~scRepBase() {assert(!m_lRefc);}
	~scRepBase() {assert(!m_lRefc);}

protected:
	scRepBase() : m_lRefc(0) {}
	scRepBase(const scRepBase&) : m_lRefc(0) {}
	scRepBase& operator=(const scRepBase& rhs){ return *this;}

private:
	// 友元类scPtrHandleBaseRepAccessor
	friend class scPtrHandleBaseRepAccessor;

public: 
	long m_lRefc;// 当前对象被引用的数量
};


// 类scPtrHandleBaseRepAccessor为指针句柄基类,定义指针句柄类的最基本操作
class SVCORE_API scPtrHandleBaseRepAccessor
{
protected:
	// 返回对象r被引用的数量
	long& Refc(scRepBase& r) {return r.m_lRefc;}
	virtual ~scPtrHandleBaseRepAccessor() {}
};

// 类scPtrHandleBase_const是const类型被引用类的指针句柄基类，派生自类scPtrHandleBaseRepAccessor
template <class T>
class scPtrHandleBase_const : public scPtrHandleBaseRepAccessor
{
public:
	scPtrHandleBase_const(const scPtrHandleBase_const&);				// 拷贝构造函数		
	scPtrHandleBase_const& operator=(const scPtrHandleBase_const&);		// 重载=操作符
	scPtrHandleBase_const& operator=(T* rep);							// 重载=操作符
	
	T* DisconnectRep();													// 断开指针句柄与被引用类对象之间的关联

	bool operator==(const scPtrHandleBase_const& rhs) const				// 重载==操作符
	{ return Rep() == rhs.Rep();}

	bool operator!=(const scPtrHandleBase_const& rhs) const				// 重载!=操作符
	{ return Rep() != rhs.Rep();}

	long Refc() const {return m_pRep ? m_pRep->Refc() : 0;}				// 返回被引用类对象的引用计数

	~scPtrHandleBase_const();											// 析构函数

protected:
	scPtrHandleBase_const():m_pRep(0){};
	T* Rep() const {return m_pRep;}										// 返回被引用类对象的实际指针


	long Decrement() {return sfRefDec(m_pRep);}							// 增加计数
	long Increment() {return sfRefInc(m_pRep);}							// 减少计数

private:
	T *m_pRep;
};

// 拷贝构造函数	
template <class T>
scPtrHandleBase_const<T>::scPtrHandleBase_const
(const scPtrHandleBase_const<T>& rhs)
  : m_pRep(rhs.m_pRep)
{
	if (m_pRep)
		Increment();
}

// 析构函数
template <class T>
scPtrHandleBase_const<T>::~scPtrHandleBase_const()
{
	if(m_pRep && !Decrement())
		sfRelease(m_pRep);
}

// 重载=操作符
template<class T>
scPtrHandleBase_const<T>& scPtrHandleBase_const<T>::operator=
(const scPtrHandleBase_const<T>& rhs)
{
	if(m_pRep != rhs.m_pRep)
	{
		if(m_pRep && !Decrement())
			sfRelease(m_pRep);

		m_pRep = rhs.m_pRep;

		if(m_pRep)
			Increment();
	}
	return *this;
}

// 重载=操作符
template<class T>
scPtrHandleBase_const<T>& scPtrHandleBase_const<T>::operator=(T* rep)
{
	if(m_pRep != rep)
	{
		if(m_pRep && !Decrement())
			sfRelease(m_pRep);

		m_pRep = rep;

		if(m_pRep)
			Increment();
	}
	return *this;
}

// 断开指针句柄与被引用类对象之间的关联
template <class T>
T* scPtrHandleBase_const<T>::DisconnectRep()
{
	if(m_pRep)
		Decrement();

	T* p = m_pRep;
	m_pRep = 0;
	return p;
}


// 类scPtrHandleBase是非const类型被引用类的指针句柄基类，派生自类scPtrHandleBase_const
template <class T>
class scPtrHandleBase : public virtual scPtrHandleBase_const<T>
{
};


// 类scPtrHandle_const是const类型被引用类的指针句柄类
template <class T,class B = scPtrHandleBase_const<T> >
class scPtrHandle_const : public virtual B
{
public:

	// 构造函数
	scPtrHandle_const(const T* rep = 0)
	{if(rep) B::operator=(const_cast<T *>(rep)); }
	
	// 重载=操作符
	scPtrHandle_const& operator=(T*rep)
	{B::operator=(rep); return *this;}

	// 重载=操作符
	scPtrHandle_const& operator=(const scPtrHandle_const& rhs)
	{B::operator=(rhs); return *this;}

	// 返回const类型的被引用对象的实际指针
	const T* Rep() const {return static_cast<const T*>(B::Rep()); }

	// 断开指针句柄与被引用类对象之间的关联
	const T* DisconnectRep()
	{const T *p = Rep(); B::DisconnectRep(); return p;}

	// 重载*操作符
	const T& operator*() const { assert(Rep()); return *Rep(); }

	// 重载->操作符
	const T* operator->() const { assert(Rep()); return Rep(); }
	
	// 重载!操作符
	bool operator!() const { return Rep() == 0; }
	operator const void* () const { return Rep() ? this : 0; }


};


// 类scPtrHandle是非const类型被引用类的指针句柄类
template <class T,class B = scPtrHandleBase<T>, class C = scPtrHandleBase_const<T> >
class scPtrHandle : public B,public virtual scPtrHandle_const<T,C>
{
public:
	// 构造函数
	scPtrHandle(T* rep = 0)
	{if(rep) C::operator=(rep);}

	// 重载=操作符
	scPtrHandle& operator=(T* rep)
	{*((scPtrHandle_const<T,C>*)this) = rep; return *this;}

	// 重载=操作符
	scPtrHandle& operator=(const scPtrHandle& rhs)
	{*((scPtrHandle_const<T,C>*)this) = (scPtrHandle_const<T,C>&)rhs;
	return *this;}

	// 返回非const被引用对象的实际指针
	T* Rep() const
	{return const_cast<T*>(((scPtrHandle_const<T,C>*)this)->Rep()); }

	// 断开指针句柄与被引用类对象之间的关联
	T* DisconnectRep()
	{T *p = Rep(); C::DisconnectRep(); return p;}

	// 重载*操作符
	T& operator*() const { assert(Rep()); return *Rep(); }

	// 重载->操作符
	T* operator->() const { assert(Rep()); return Rep(); }

	// 重载!操作符
	bool operator!() const { return Rep() == 0; }

	operator const void* () const { return Rep() ? this : 0; }

};


// 定义被引用类的派生类的指针句柄类型
#define smDerivedPtrHdlDcl(derivedHdl,derivedRep,baseHdl,baseHdl_const) \
typedef scPtrHandle<derivedRep,baseHdl,baseHdl_const > derivedHdl;\
typedef scPtrHandle_const<derivedRep,baseHdl_const > derivedHdl##_const


// 宏定义显示实例化指针句柄类模板
#define smInstantiatePointerHandle(x)\
  template class scPtrHandle<x >;\
  template class scPtrHandle_const<x >;\
  template class scPtrHandleBase<x >;\
  template class scPtrHandleBase_const<x >

// 宏定义显示实例化派生类的指针句柄类模板
#define smInstantiateDerivedPtrh(D, B)\
  template class scPtrHandle<D,scPtrHandle<B,scPtrHandleBase<B>,\
    scPtrHandleBase_const<B> >,\
    scPtrHandle_const<B,scPtrHandleBase_const<B> > >;\
  template class scPtrHandle_const<D,scPtrHandle_const<B,\
    scPtrHandleBase_const<B> > >

#pragma pack(pop)
