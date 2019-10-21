// ���ܣ��������Ƽ�����. ����Ӧ���ڷ�ֹ��Ϣͻ���ĳ���
// ���ߣ���־��
// �㷨��һ��ʱ�����ͷų�һ��ָ���ļ���ֵ���ⲿ���Խ�������

#ifndef _FLOW_LIMIT_COUNTER_H_
#define _FLOW_LIMIT_COUNTER_H_

#include "cbb/osp/osp.h"

class CFlowLimitCounter
{
public:
    // ÿdwTickInterval��tickʱ��������dwNumLimit����������ʾ��OspClkRateGet��ȡһ���Ӷ��ٸ�tick
    CFlowLimitCounter(u32 dwTickInterval, u32 dwNumLimit): 
      m_dwTickInterval(dwTickInterval), m_qwLastTickValue(0), m_dwNumLimit(dwNumLimit), m_dwUseableNum(0)
    {
    }

    // �м������Լ�Сʱ����true�����򷵻�false�ڲ�Ҳ����С��ֵ
    bool Decrease(u32 dwNum = 1)
    {
        u64 qwTick = OspTickGet64();
        if (m_qwLastTickValue + m_dwTickInterval < qwTick)
        {
            m_qwLastTickValue = qwTick;
            m_dwUseableNum = m_dwNumLimit;
        }

        if (m_dwUseableNum < dwNum)
        {
            return false;
        }
        else
        {
            m_dwUseableNum -= dwNum;
            return true;
        }
    }

private:
    u32 m_dwTickInterval;
    u64 m_qwLastTickValue;
    u32 m_dwNumLimit;
    u32 m_dwUseableNum;
};

#endif // _FLOW_LIMIT_COUNTER_H_
