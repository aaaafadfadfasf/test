#include "sdk/redissdk/redissdk.h"
#include "StreamDecentrationAdapter.h"

//由项目定义
extern CRedisSdk g_cCuiRedisSdk;

class CAdapterSetter
{
public:
    CAdapterSetter()
    {
        CStreamDecentrationAdapter::SetRedisSdk( &g_cCuiRedisSdk );
    }
};

//该全局变量仅用于 为适配器设置设置 redissdk实例
CAdapterSetter g_cAdapterSetter;