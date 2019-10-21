// 功能：流量限制计数器. 可以应用于防止消息突发的场景
// 作者：胡志云
// 算法：一段时间内释放出一个指定的计数值，外部可以进行消费

#ifndef _FLOW_LIMIT_COUNTER_H_
#define _FLOW_LIMIT_COUNTER_H_

#include "cbb/osp/osp.h"

class CFlowLimitCounter
{
public:
    // 每dwTickInterval个tick时间内允许dwNumLimit个计数。提示：OspClkRateGet获取一秒钟多少个tick
    CFlowLimitCounter(u32 dwTickInterval, u32 dwNumLimit): 
      m_dwTickInterval(dwTickInterval), m_qwLastTickValue(0), m_dwNumLimit(dwNumLimit), m_dwUseableNum(0)
    {
    }

    // 有计数可以减小时返回true，否则返回false内部也不减小数值
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
