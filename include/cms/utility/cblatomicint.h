//ԭ�ӱ���������ʵ�����ü����������������Ϊ2147483647��
//���ǣ�������˵linux�ļ���ֵ��Чλ��Ϊ24��û����֤��...
//ע�⣬ԭ�ӱ����洢ʱһ��Ҫ32-bit�߽���롣
//���ߣ���־��

#ifndef _CBLATOMICINT_H_
#define _CBLATOMICINT_H_

#include "kdvsys.h"

// ԭ������1������������1ǰ����ֵ��ע�����������Ϊ32-bit�߽����
s32 AtomicIncrement(s32 *pn);
// ԭ���Լ�1���������ؼ�1ǰ����ֵ��ע�����������Ϊ32-bit�߽����
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
    // �������֮ǰ��ֵ
    s32 Add(s32 nValue);
    // �������֮ǰ��ֵ
    s32 Sub(s32 nValue);
private:
    volatile LONG m_lCount; // MSDN������ȷָ��LONG��32λ�з�������
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
    // �������֮ǰ��ֵ
    s32 Add(s32 nValue);
    // �������֮ǰ��ֵ
    s32 Sub(s32 nValue);
private:
    s32 m_nCount;
};

#endif // __linux__

#endif // _CBLATOMICINT_H_
