#ifndef _NAT_AGENT_RUNABLE_H_
#define _NAT_AGENT_RUNABLE_H_


namespace LibEvent{namespace Concurrency{
    class CQueueMsg;
    class IRunnable
    {
    public:
        virtual ~IRunnable(){}
        virtual void DealEvent(const CQueueMsg& msg) = 0;
    };
    
}}

#endif
//end of file

