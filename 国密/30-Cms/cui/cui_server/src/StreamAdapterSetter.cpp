#include "sdk/redissdk/redissdk.h"
#include "StreamDecentrationAdapter.h"

//����Ŀ����
extern CRedisSdk g_cCuiRedisSdk;

class CAdapterSetter
{
public:
    CAdapterSetter()
    {
        CStreamDecentrationAdapter::SetRedisSdk( &g_cCuiRedisSdk );
    }
};

//��ȫ�ֱ��������� Ϊ�������������� redissdkʵ��
CAdapterSetter g_cAdapterSetter;