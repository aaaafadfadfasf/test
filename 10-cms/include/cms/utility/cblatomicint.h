//原子变量常用于实现引用计数，符号整数最大为2147483647，
//但是，有资料说linux的计数值有效位数为24，没有验证过...
//注意，原子变量存储时一定要32-bit边界对齐。
//作者：胡志云

#ifndef _CBLATOMICINT_H_
#define _CBLATOMICINT_H_

#include "kdvsys.h"

// 原子性增1操作，返回增1前的数值；注意操作数必须为32-bit边界对齐
s32 AtomicIncrement(s32 *pn);
// 原子性减1操作，返回减1前的数值；注意操作数必须为32-bit边界对齐
s32 AtomicDecrement(s32 *pn);

#ifdef _MSC_VER
#include <Windows.h>

class CAtomicInt
{
public:
    CAtomicInt():m_lCount(0){}
    ~CAtomicInt(){}
private:
    CAtomicInt& operator=(const CAtomicInt&);
public:
    s32 Get() const;
    void Set(s32 nValue);
    void Increase();
    void Decrease();
    // 返回相加之前的值
    s32 Add(s32 nValue);
    // 返回相减之前的值
    s32 Sub(s32 nValue);
private:
    volatile LONG m_lCount; // MSDN里面明确指出LONG是32位有符号整数
};

#endif // _MSC_VER

#ifdef __linux__

class CAtomicInt
{
public:
    CAtomicInt():m_nCount(0){}
    ~CAtomicInt(){}
private:
    CAtomicInt& operator=(const CAtomicInt&);
public:
    s32 Get() const;
    void Set(s32 nValue);
    void Increase();
    void Decrease();
    // 返回相加之前的值
    s32 Add(s32 nValue);
    // 返回相减之前的值
    s32 Sub(s32 nValue);
private:
    s32 m_nCount;
};

#endif // __linux__

#endif // _CBLATOMICINT_H_
