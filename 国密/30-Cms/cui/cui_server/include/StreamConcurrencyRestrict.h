#ifndef _INCLUDE_STREAM_CONCURRENCY_RESTRICT_H_
#define _INCLUDE_STREAM_CONCURRENCY_RESTRICT_H_

class CStreamConcurrencyRestrict
{
public:
    CStreamConcurrencyRestrict()
    {
        bEnabled    = false;
        dwStreamNum = 0;
    }
public:
    void SetEnabled( bool bEnableLimit )
    {
        bEnabled = bEnableLimit;
    }
    bool GetEnabled( bool bEnableLimit ) const
    {
        return bEnabled;
    }
    void SetStreamNum( u32 dwMaxStreamNum )
    {
        dwStreamNum = dwMaxStreamNum;
    }
    u32 GetStreamNum() const
    {
        return dwStreamNum;
    }
    void SetInAll( s32 nSwitchNum )
    {
        bEnabled = nSwitchNum != DISABLE_SWITCH_NUM;
        dwStreamNum = bEnabled ? nSwitchNum : 0;
    }
    bool IsIncreAllowed( u32 dwCur ) const
    {
        if( bEnabled )
        {
            return dwCur < dwStreamNum;
        }
        return true;
    }
    
public:
    static const int DISABLE_SWITCH_NUM = -1;
private:
    bool bEnabled;
    u32  dwStreamNum;
};

#endif //_INCLUDE_STREAM_CONCURRENCY_RESTRICT_H_