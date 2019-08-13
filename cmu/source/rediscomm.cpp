
#include "rediscomm.h"
#include "cmuconst.h"
#include "cms/tas/tas_redis.h"
#include "cms/cmu/cmu_redis.h"
#include "cmudata.h"
#include "puregtask.h"
#include "puiregtask.h"
#include "cmuconnecttask.h"

// Fastdb���Ļص�����
void cbRedisSub(CFastDBInterface *, const string &, const string &, void *);

//===============================================================================================================================

CRedisPipe::CRedisPipe()
{
}

CRedisPipe::~CRedisPipe()
{
}

void CRedisPipe::SetRedisParam(const string &strRedisIp, u16 wRedisPort, const string &strRedisPassword)
{
    m_strRedisIp = strRedisIp;
    m_wRedisPort = wRedisPort;
    m_strRedisPassword = strRedisPassword;
}

bool CRedisPipe::Connect()
{
    return (CMS_SUCCESS == m_cRedisSdk.Initialize(string(RedisModuleType_Cmu) + string(":") + string("cmu"), 
        m_strRedisIp, m_wRedisPort, m_strRedisPassword, cbRedisSdkCallback, this));
}

bool CRedisPipe::IsConnected()
{
    return m_cRedisSdk.GetFastDBInstance().IsConnected();
}

void CRedisPipe::Disconnect()
{
    m_cRedisSdk.Quit();
}

bool CRedisPipe::InitDevStatus(const vector<string> &cDevId, bool bPublishState)
{
    /*
     * Lua script��
     */
    s8 szPublish[1024] = {0};
    if (bPublishState)
    {
        snprintf(szPublish, sizeof(szPublish),  
            "    local tmp = redis.call('HMGET', 'dev:'..list[i], 'dom', 'use'); \n"
            "    local domname = redis.call('HGET', 'domain:'..tmp[1], 'name'); \n"
            "    redis.call('PUBLISH', list[i]..':PU_ON_OFF', "
                "'<TPuStatus><isInTop>1</isInTop><isUsed>'..tmp[2]..'</isUsed><isOnline>0</isOnline></TPuStatus>'); \n"
            "    redis.call('PUBLISH', tmp[1]..'#PU_ON_OFF', "
                "'<CPuStatusNtyReq><event>PU_STATUS_NTF_REQ</event><devURI>'..list[i]..'@'..domname..'</devURI>"
                "<onOff><isInTop>1</isInTop><isUsed>'..tmp[2]..'</isUsed><isOnline>0</isOnline></onOff></CPuStatusNtyReq>'); \n");
    }

    s8 szLua[4096] = {0};
    snprintf(szLua, sizeof(szLua), 
        "local list = {"
        " KEYS[1],  KEYS[2],  KEYS[3],  KEYS[4],  KEYS[5],  KEYS[6],  KEYS[7],  KEYS[8],  KEYS[9],  KEYS[10],"
        " KEYS[11], KEYS[12], KEYS[13], KEYS[14], KEYS[15], KEYS[16], KEYS[17], KEYS[18], KEYS[19], KEYS[20],"
        " KEYS[21], KEYS[22], KEYS[23], KEYS[24], KEYS[25], KEYS[26], KEYS[27], KEYS[28], KEYS[29], KEYS[30],"
        " KEYS[31], KEYS[32], KEYS[33], KEYS[34], KEYS[35], KEYS[36], KEYS[37], KEYS[38], KEYS[39], KEYS[40],"
        " KEYS[41], KEYS[42], KEYS[43], KEYS[44], KEYS[45], KEYS[46], KEYS[47], KEYS[48], KEYS[49], KEYS[50],"
        " KEYS[51], KEYS[52], KEYS[53], KEYS[54], KEYS[55], KEYS[56], KEYS[57], KEYS[58], KEYS[59], KEYS[60],"
        " KEYS[61], KEYS[62], KEYS[63], KEYS[64], KEYS[65], KEYS[66], KEYS[67], KEYS[68], KEYS[69], KEYS[70],"
        " KEYS[71], KEYS[72], KEYS[73], KEYS[74], KEYS[75], KEYS[76], KEYS[77], KEYS[78], KEYS[79], KEYS[80],"
        " KEYS[81], KEYS[82], KEYS[83], KEYS[84], KEYS[85], KEYS[86], KEYS[87], KEYS[88], KEYS[89], KEYS[90],"
        " KEYS[91], KEYS[92], KEYS[93], KEYS[94], KEYS[95], KEYS[96], KEYS[97], KEYS[98], KEYS[99], KEYS[100]"
        "}; \n"
        "for i=1,#list do \n"
        "  if #list[i] ~= 0 then \n"
        "    local chnnum = tonumber(redis.call('HGET', 'dev:'..list[i], '%s')) or 0; \n"
        "    for j=1,chnnum do \n"
        "      local status = redis.call('HGET', 'dev:'..list[i], 'chnstatus:'..(j-1)); \n"
        "      if nil ~= string.find(tostring(status), '<chnState>1</chnState>') then \n"
        "          redis.call('HSET', 'dev:'..list[i], 'chnstatus:'..(j-1), '<TPuChnStateConfig><chnState>2</chnState></TPuChnStateConfig>'); \n"
        "      end \n"
        "    end \n"
        "    redis.call('HSET', 'dev:'..list[i], 'online', 0); \n"
        "%s"
        "  end \n"
        "end \n"
        "return 0; \n", 
        TPuBaseCapSet_videoEncNum, szPublish);

    s32 nRet = EC_OK;
    u32 i = 0, j = 0;
    while (j < cDevId.size())
    {
        if (++j % m_swRedisBatchProcNumPerTime == 0)             // �ﵽ������
        {
            m_cRedisSdk.GetFastDBInstance().Eval(szLua, vector<string>(cDevId.begin() + i, cDevId.begin() + j), vector<string>(), nRet);
            if (EC_OK != nRet)
            {
                return false;
            }

            i = j;
        }
    }

    if (i < j)
    {
        /*
         * ����ط��漰�����redis luaʵ�ֵ�һ���Ż���redis��Ե��õ�lua�ű���ȡ�˻�����ƣ�
         * Ϊ��ʹ��ÿ�����ɵ�lua�ű����Ա�lua�������У�ÿ�δ��ݹ�ȥ��KEYS�������ǹ̶��ģ�
         * fastdb��eval�ӿ��Ǹ���keys��size������KEYS�������ģ����Ե�cLast����������ʱ��
         * ʹ����resize��������Ӧ������
         */
        vector<string> cLast(cDevId.begin() + i, cDevId.end());
        cLast.resize(m_swRedisBatchProcNumPerTime);

        m_cRedisSdk.GetFastDBInstance().Eval(szLua, cLast, vector<string>(), nRet);
        if (EC_OK != nRet)
        {
            return false;
        }
    }

    return true;
}

bool CRedisPipe::BatchGetPuRegInfo(const vector<string> &cDevId, vector<TPuRegInfo> &cInfo)
{
    /*
     * Lua script��������ȡ�豸��ע����Ϣ
     *
     * ��ȡ��ÿ���豸��ע����Ϣ�������¼��
     *     1. use�����ñ�־��
     *     2. expireDate���������ڣ�
     *     3. encode������ͨ��������
     *     4. freelicense����ѵ�����
     *     5. license������ʱָ����license������
     *     6. creat�������ߣ�
     *     7. name���豸���ƣ�
     *     8. alias���豸������
     *     9. connectip���豸����ip��
     *    10. puiip���豸��Ӧ��pui��ip��
     *    11. chnstatusflag���豸ͨ��״̬��־��false-����ͨ���������ߣ�true-��������ͨ����
     */
    s8 szLua[4096] = {0};
    snprintf(szLua, sizeof(szLua), 
        "local list = {"
        " KEYS[1],  KEYS[2],  KEYS[3],  KEYS[4],  KEYS[5],  KEYS[6],  KEYS[7],  KEYS[8],  KEYS[9],  KEYS[10],"
        " KEYS[11], KEYS[12], KEYS[13], KEYS[14], KEYS[15], KEYS[16], KEYS[17], KEYS[18], KEYS[19], KEYS[20],"
        " KEYS[21], KEYS[22], KEYS[23], KEYS[24], KEYS[25], KEYS[26], KEYS[27], KEYS[28], KEYS[29], KEYS[30],"
        " KEYS[31], KEYS[32], KEYS[33], KEYS[34], KEYS[35], KEYS[36], KEYS[37], KEYS[38], KEYS[39], KEYS[40],"
        " KEYS[41], KEYS[42], KEYS[43], KEYS[44], KEYS[45], KEYS[46], KEYS[47], KEYS[48], KEYS[49], KEYS[50],"
        " KEYS[51], KEYS[52], KEYS[53], KEYS[54], KEYS[55], KEYS[56], KEYS[57], KEYS[58], KEYS[59], KEYS[60],"
        " KEYS[61], KEYS[62], KEYS[63], KEYS[64], KEYS[65], KEYS[66], KEYS[67], KEYS[68], KEYS[69], KEYS[70],"
        " KEYS[71], KEYS[72], KEYS[73], KEYS[74], KEYS[75], KEYS[76], KEYS[77], KEYS[78], KEYS[79], KEYS[80],"
        " KEYS[81], KEYS[82], KEYS[83], KEYS[84], KEYS[85], KEYS[86], KEYS[87], KEYS[88], KEYS[89], KEYS[90],"
        " KEYS[91], KEYS[92], KEYS[93], KEYS[94], KEYS[95], KEYS[96], KEYS[97], KEYS[98], KEYS[99], KEYS[100]"
        "}; \n"
        "local result={}; \n"
        "for i=1,#list do \n"
        "  if #list[i] ~= 0 then \n"
        "    result[i] = redis.call('HMGET', 'dev:'..list[i], '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s'); \n"
        "    result[i][11] = 0; \n"
        "    for j=1,tonumber(result[i][3]) or 0 do \n"
        "      local status = redis.call('HGET', 'dev:'..list[i], 'chnstatus:'..(j-1)); \n"
        "      if nil ~= string.find(tostring(status), '<chnState>1</chnState>') then \n"
        "          result[i][11] = 1; \n"
        "          break; \n"
        "      end \n"
        "    end \n"
        "  else \n"         // KEYΪ�� todo by xiongxh ����Lua��table��ת��Ϊredis��arrayʱ����truncate���������������������Ĭ��ֵ
        "    result[i] = {}; \n"
        "  end \n"
        "end \n"
        "return result; \n",
        CDeviceInfo_usedFlag, CDeviceInfo_expireDate, TPuBaseCapSet_videoEncNum, TPuConfig_freeLicenceNum, Redis_LicenceNum, 
        CDeviceInfo_creater, CDeviceInfo_name, CDeviceInfo_alias, TPuConfig_devIp, Redis_puiip);

    u32 i = 0, j = 0;
    cInfo.resize(cDevId.size());
    while (j < cDevId.size())
    {
        if (++j % m_swRedisBatchProcNumPerTime == 0)             // �ﵽ������
        {
            redisReply *pcRely = NULL;
            if (EC_OK == m_cRedisSdk.GetFastDBInstance().Eval(szLua, vector<string>(cDevId.begin() + i, cDevId.begin() + j), vector<string>(), &pcRely)
                && NULL != pcRely)
            {
                CMS_ASSERT(m_swRedisBatchProcNumPerTime == pcRely->elements && "pcReply->elements != m_swRedisBatchProcNumPerTime!!!");

                if (NULL != pcRely->element && REDIS_REPLY_ARRAY == pcRely->type)
                {
                    for (size_t m = 0; m < pcRely->elements; ++m)
                    {
                        redisReply *pcItemReply = (pcRely->element)[m];

                        RedisReply2PuRegInfo(pcItemReply, cInfo[i + m]);
                    }
                }

                // ����һ��Ҫע�⣬��������£�����Ҫ�ֶ��ͷ�redisReply
                if (NULL != pcRely)
                {
                    freeReplyObject(pcRely);
                    pcRely = NULL;
                }
            }
            else
            {
                return false;
            }

            i = j;
        }
    }

    if (i < j)
    {
        /*
         * ����ط��漰�����redis luaʵ�ֵ�һ���Ż���redis��Ե��õ�lua�ű���ȡ�˻�����ƣ�
         * Ϊ��ʹ��ÿ�����ɵ�lua�ű����Ա�lua�������У�ÿ�δ��ݹ�ȥ��KEYS�������ǹ̶��ģ�
         * fastdb��eval�ӿ��Ǹ���keys��size������KEYS�������ģ����Ե�cLast����������ʱ��
         * ʹ����resize��������Ӧ������
         */
        vector<string> cLast(cDevId.begin() + i, cDevId.end());
        cLast.resize(m_swRedisBatchProcNumPerTime);

        redisReply *pcRely = NULL;
        if (EC_OK == m_cRedisSdk.GetFastDBInstance().Eval(szLua, cLast, vector<string>(), &pcRely)
            && NULL != pcRely)
        {
            CMS_ASSERT(m_swRedisBatchProcNumPerTime == pcRely->elements && "pcReply->elements != m_swRedisBatchProcNumPerTime!!!");

            if (NULL != pcRely->element && REDIS_REPLY_ARRAY == pcRely->type)
            {
                for (size_t m = 0; m < pcRely->elements && i + m < j; ++m)
                {
                    redisReply *pcItemReply = (pcRely->element)[m];

                    RedisReply2PuRegInfo(pcItemReply, cInfo[i + m]);
                }
            }

            // ����һ��Ҫע�⣬��������£�����Ҫ�ֶ��ͷ�redisReply
            if (NULL != pcRely)
            {
                freeReplyObject(pcRely);
                pcRely = NULL;
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool CRedisPipe::BatchPuOnline(const vector<string> &cDevId, bool bPublishState)
{
    /*
     * Lua script�������Ľ��豸״̬����Ϊ���ߣ�ͬʱ������ȥ
     *
     */
    s8 szPublish[1024] = {0};
    if (bPublishState)
    {
        snprintf(szPublish, sizeof(szPublish),  
            "    local domid = redis.call('HGET', 'dev:'..list[i], 'dom'); \n"
            "    local domname = redis.call('HGET', 'domain:'..domid, 'name'); \n"
            "    redis.call('PUBLISH', list[i]..':PU_ON_OFF', "
                "'<TPuStatus><isInTop>1</isInTop><isUsed>1</isUsed><isOnline>1</isOnline></TPuStatus>'); \n"
            "    redis.call('PUBLISH', domid..'#PU_ON_OFF', "
                "'<CPuStatusNtyReq><event>PU_STATUS_NTF_REQ</event><devURI>'..list[i]..'@'..domname..'</devURI>"
                "<onOff><isInTop>1</isInTop><isUsed>1</isUsed><isOnline>1</isOnline></onOff></CPuStatusNtyReq>'); \n");
    }

    s8 szLua[4096] = {0};
    snprintf(szLua, sizeof(szLua), 
        "local list = {"
        " KEYS[1],  KEYS[2],  KEYS[3],  KEYS[4],  KEYS[5],  KEYS[6],  KEYS[7],  KEYS[8],  KEYS[9],  KEYS[10],"
        " KEYS[11], KEYS[12], KEYS[13], KEYS[14], KEYS[15], KEYS[16], KEYS[17], KEYS[18], KEYS[19], KEYS[20],"
        " KEYS[21], KEYS[22], KEYS[23], KEYS[24], KEYS[25], KEYS[26], KEYS[27], KEYS[28], KEYS[29], KEYS[30],"
        " KEYS[31], KEYS[32], KEYS[33], KEYS[34], KEYS[35], KEYS[36], KEYS[37], KEYS[38], KEYS[39], KEYS[40],"
        " KEYS[41], KEYS[42], KEYS[43], KEYS[44], KEYS[45], KEYS[46], KEYS[47], KEYS[48], KEYS[49], KEYS[50],"
        " KEYS[51], KEYS[52], KEYS[53], KEYS[54], KEYS[55], KEYS[56], KEYS[57], KEYS[58], KEYS[59], KEYS[60],"
        " KEYS[61], KEYS[62], KEYS[63], KEYS[64], KEYS[65], KEYS[66], KEYS[67], KEYS[68], KEYS[69], KEYS[70],"
        " KEYS[71], KEYS[72], KEYS[73], KEYS[74], KEYS[75], KEYS[76], KEYS[77], KEYS[78], KEYS[79], KEYS[80],"
        " KEYS[81], KEYS[82], KEYS[83], KEYS[84], KEYS[85], KEYS[86], KEYS[87], KEYS[88], KEYS[89], KEYS[90],"
        " KEYS[91], KEYS[92], KEYS[93], KEYS[94], KEYS[95], KEYS[96], KEYS[97], KEYS[98], KEYS[99], KEYS[100]"
        "}; \n"
        "for i=1,#list do \n"
        "  if #list[i] ~= 0 then \n"
        "    redis.call('HSET', 'dev:'..list[i], '%s', 1); \n"
        "%s"
        "  end \n"
        "end \n"
        "return 0; \n", 
        TPuStatus_isOnline, szPublish);

    s32 nRet = EC_OK;
    u32 i = 0, j = 0;
    while (j < cDevId.size())
    {
        if (++j % m_swRedisBatchProcNumPerTime == 0)             // �ﵽ������
        {
            m_cRedisSdk.GetFastDBInstance().Eval(szLua, vector<string>(cDevId.begin() + i, cDevId.begin() + j), vector<string>(), nRet);
            if (EC_OK != nRet)
            {
                return false;
            }

            i = j;
        }
    }

    if (i < j)
    {
        /*
         * ����ط��漰�����redis luaʵ�ֵ�һ���Ż���redis��Ե��õ�lua�ű���ȡ�˻�����ƣ�
         * Ϊ��ʹ��ÿ�����ɵ�lua�ű����Ա�lua�������У�ÿ�δ��ݹ�ȥ��KEYS�������ǹ̶��ģ�
         * fastdb��eval�ӿ��Ǹ���keys��size������KEYS�������ģ����Ե�cLast����������ʱ��
         * ʹ����resize��������Ӧ������
         */
        vector<string> cLast(cDevId.begin() + i, cDevId.end());
        cLast.resize(m_swRedisBatchProcNumPerTime);

        m_cRedisSdk.GetFastDBInstance().Eval(szLua, cLast, vector<string>(), nRet);
        if (EC_OK != nRet)
        {
            return false;
        }
    }

    return true;
}

bool CRedisPipe::GetDevIdList(vector<string> &cDevId)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDeviceIDList(cDevId));
}

bool CRedisPipe::GetDomainIdList(vector<string> &cDomainId)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDomainIDList(cDomainId));
}

bool CRedisPipe::GetDomainName(const string &strDomainId, string &strDomainName)
{
    CDomainRelationInfo cDomainInfo;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDomainRelationInfo(strDomainId, cDomainInfo, CDomainRelationInfo_domainName))
    {
        strDomainName = cDomainInfo.GetDomainName();

        return true;
    }

    return false;
}

bool CRedisPipe::GetDomainKdmNO(const string &strDomainId, string &strKdmNO)
{
    CDomainRelationInfo cDomainInfo;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDomainRelationInfo(strDomainId, cDomainInfo, CDomainRelationInfo_domainAlias))
    {
        const map<TDomainExtDataType, string> &tExtData = cDomainInfo.GetDomainAlias().GetExtData();
        map<TDomainExtDataType, string>::const_iterator it = tExtData.find(DOMAIN_EXT_KDMNO);
        if (tExtData.end() != it)
        {
            strKdmNO = it->second;
        }

        return true;
    }

    return false;
}

bool CRedisPipe::GetDomainRelationInfo(const string &strDomainId, CDomainRelationInfo &cInfo)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetFullDomainRelationInfo(strDomainId, cInfo));
}

bool CRedisPipe::GetPuInUse(const string &strDevId, bool &bInUse)
{
	TPuStatus tPuStatus;
	if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuStatus(strDevId, tPuStatus, TPuStatus_isUsed))
	{
        bInUse = tPuStatus.GetIsUsed();

		return true;
	}

	return false;
}

bool CRedisPipe::GetDevInfo(const string &strDevId, CDeviceInfo &cInfo)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetFullDeviceInfo(strDevId, cInfo));
}

bool CRedisPipe::GetDevDomainId(const string &strDevId, string &strDomainId)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_domainId, strDomainId));
}

bool CRedisPipe::GetDevType(const string &strDevId, s32 &nDevType)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_type, nDevType));
}

bool CRedisPipe::GetDevGBFlag(const string &strDevId, s32 &nGBFlag)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, TPuConfig_isGBDevice, nGBFlag));
}

bool CRedisPipe::GetDevName(const string &strDevId, string &strDevName)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_name, strDevName));
}

bool CRedisPipe::GetDevAlias(const string &strDevId, string &strAlias)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_alias, strAlias));
}

bool CRedisPipe::GetDevNetAddr(const string &strDevId, string &strAddr)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_netAddress, strAddr));
}

bool CRedisPipe::GetDevModelId(const string &strDevId, string &strModelId)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_modelId, strModelId));
}

bool CRedisPipe::GetDevManufacturer(const string &strDevId, string &strManufacturer)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_manufacturer, strManufacturer));
}

bool CRedisPipe::GetNetType(const string &strDevId, s32 &nNetType)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_netType, nNetType));
}

bool CRedisPipe::GetCreater(const string &strDevId, s32 &nCreater)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_creater, nCreater));
}

bool CRedisPipe::GetOwner(const string &strDevId, string &strOwner)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_owner, strOwner));
}

bool CRedisPipe::GetExpireDate(const string &strDevId, string &strExpireDate)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_expireDate, strExpireDate));
}

bool CRedisPipe::GetKdmNO(const string &strDevId, string &strKdmNO)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, CDeviceInfo_kdmno, strKdmNO));
}

bool CRedisPipe::GetPuVidSrcNum(const string &strDevId, s32 &nVidSrcNum)
{
    TPuBaseCapSet tPuBaseCapSet;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuBaseCapSet(strDevId, tPuBaseCapSet, TPuBaseCapSet_videoInPortNum))
    {
        nVidSrcNum = tPuBaseCapSet.GetVideoInPortNum();

        return true;
    }

    return false;
}

bool CRedisPipe::GetPuHdmiOutputNum(const string &strDevId, s32 &nHdmiOutputNum)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, Redis_hdmioutnum, nHdmiOutputNum));
}

bool CRedisPipe::GetDirectTransNum(const string &strDevId, s32 &nTransNum)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, TPuConfig_directTransNum))
    {
        nTransNum = tPuConfig.GetDirectTransNum();

        return true;
    }

    return false;
}

bool CRedisPipe::GetVidSrcAliasMap(const string &strDevId, map<int, string> &cAliasMap)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, ALL_TPuConfig_vidSrcAlias))
    {
        cAliasMap = tPuConfig.GetVidSrcName();

        return true;
    }

    return false;
}

bool CRedisPipe::GetVidSrcAlias(const string &strDevId, s32 nVidSrcIdx, string &strAlias)
{
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, TPuConfig_vidSrcAlias(nVidSrcIdx), strAlias);
    if ( IsFdbResultPositive(emRet) )
    {
        return true;
    }

    return false;
}

bool CRedisPipe::GetPuVidSrcAliasMap(const string &strDevId, map<int, string> &cAliasMap)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, ALL_TPuConfig_vidSrcAlias))
    {
        cAliasMap = tPuConfig.GetVidSrcName();

        return true;
    }

    return false;
}

bool CRedisPipe::GetPuVidSrcAlias(const string &strDevId, s32 nVidSrcIdx, string &strAlias)
{
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, TPuConfig_vidSrcAlias(nVidSrcIdx), strAlias);
    if ( IsFdbResultPositive(emRet) )
    {
        return true;
    }

    return false;
}

bool CRedisPipe::GetVideoEncodeParam(const string &strDevId, s32 nChnNO, TVideoEncodeParam &tParam)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, TPuConfig_chnVidEncParam(nChnNO)))
    {
        const map<int,TVideoEncodeParam> &tEncParams = tPuConfig.GetVideoEncodeParam();
        map<int,TVideoEncodeParam>::const_iterator it = tEncParams.find(nChnNO);
        if (tEncParams.end() != it)
        {
            tParam = it->second;

            return true;
        }
    }

    return false;
}

bool CRedisPipe::GetAudioEncodeParam(const string &strDevId, s32 nChnNO, TAudioEncodeParam &tParam)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, TPuConfig_chnAudEncParam(nChnNO)))
    {
        const map<int,TAudioEncodeParam> &tEncParams = tPuConfig.GetAudioEncodeParam();
        map<int,TAudioEncodeParam>::const_iterator it = tEncParams.find(nChnNO);
        if (tEncParams.end() != it)
        {
            tParam = it->second;

            return true;
        }
    }

    return false;
}

bool CRedisPipe::GetVidSrcChn(const string &strDevId, s32 nVidSrcNO, CPuChnSet &cChn)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, TPuConfig_vidSrcChnSet(nVidSrcNO)))
    {
        const map<int,CPuChnSet> &tChnSet = tPuConfig.GetVidSrcChnSet();
        map<int,CPuChnSet>::const_iterator it = tChnSet.find(nVidSrcNO);
        if (tChnSet.end() != it)
        {
            cChn = it->second;

            return true;
        }

        return true;
    }

    return false;
}

bool CRedisPipe::GetVidSrcChnSet(const string &strDevId, map<int, CPuChnSet> &cChnSet)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, ALL_TPuConfig_vidSrcChnSet))
    {
        cChnSet = tPuConfig.GetVidSrcChnSet();

        return true;
    }

    return false;
}

bool CRedisPipe::GetChnVidSrc(const string &strDevId, s32 nChnNO, CVidSrcSet &cVidSrc)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, TPuConfig_chnVidSrcSet(nChnNO)))
    {
        const map<int,CVidSrcSet> &tVidSrcSet = tPuConfig.GetChnVidSrcSet();
        map<int,CVidSrcSet>::const_iterator it = tVidSrcSet.find(nChnNO);
        if (tVidSrcSet.end() != it)
        {
            cVidSrc = it->second;

            return true;
        }

        return true;
    }

    return false;
}

bool CRedisPipe::GetChnVidSrcSet(const string &strDevId, map<int, CVidSrcSet> &cVidSrcSet)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, ALL_TPuConfig_chnVidSrcSet))
    {
        cVidSrcSet = tPuConfig.GetChnVidSrcSet();

        return true;
    }

    return false;
}

bool CRedisPipe::GetPuChnState(const string &strDevId, s32 nChnNO, TPuChnStateConfig &tChnState)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, TPuConfig_chnStatusSet(nChnNO)))
    {
        tChnState = tPuConfig.GetChnStateConfig(nChnNO);
        
        return true;
    }

    return false;
}

bool CRedisPipe::GetPuChnStateSet(const string &strDevId, map<int, TPuChnStateConfig> &cChnStateSet)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, ALL_TPuConfig_chnStatusSet))
    {
        cChnStateSet = tPuConfig.GetChnStateSet();

        return true;
    }

    return false;
}

bool CRedisPipe::GetPuConnectAddr(const string &strDevId, string &strDevIp)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, TPuConfig_devIp))
    {
        strDevIp = tPuConfig.GetDevIp();

        return true;
    }

    return false;
}

bool CRedisPipe::GetPuVidEncNum(const string &strDevId, s32 &nVidEncNum)
{
    TPuBaseCapSet tPuBaseCapSet;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuBaseCapSet(strDevId, tPuBaseCapSet, TPuBaseCapSet_videoEncNum))
    {
        nVidEncNum = tPuBaseCapSet.GetVidEncNum();

        return true;
    }

    return false;
}

bool CRedisPipe::GetPuFreeLicenseNum(const string &strDevId, s32 &nFreeNum)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, TPuConfig_freeLicenceNum))
    {
        nFreeNum = tPuConfig.GetFreeLicenceNum();

        return true;
    }

    return false;
}

bool CRedisPipe::GetBaseCapSet(const string &strDevId, TPuBaseCapSet &tCapSet)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetFullPuBaseCapSet(strDevId, tCapSet));
}

bool CRedisPipe::GetPuiIp(const string &strDevId, string &strPuiIp)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetDevInfo(strDevId, Redis_puiip, strPuiIp));
}

bool CRedisPipe::GetPuStatus(const string &strDevId, TPuStatus &tPuStatus)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuStatus(strDevId, tPuStatus, ALL_TPuStatus));
}

bool CRedisPipe::SetPuStatus(const string &strDevId, const TPuStatus &tPuStatus)
{
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().IsKeyExists(MAKE_DEV_KEY(strDevId)))
    {
        return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuStatus(strDevId, tPuStatus, ALL_TPuStatus));
    }

    return false;
}

bool CRedisPipe::GetPuIsOnline(const string &strDevId, bool &bOnline)
{
    TPuStatus tPuStatus;
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetPuStatus(strDevId, tPuStatus, TPuStatus_isOnline);
    if ( IsFdbResultPositive(emRet) )
    {
        bOnline = tPuStatus.GetIsOnline();

        return true;
    }

    return false;
}

bool CRedisPipe::SetPuIsOnline(const string &strDevId, bool bOnline)
{
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().IsKeyExists(MAKE_DEV_KEY(strDevId)))
    {
        TPuStatus tPuStatus;
        tPuStatus.SetIsOnline(bOnline);

        return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuStatus(strDevId, tPuStatus, TPuStatus_isOnline));
    }
    
    return false;
}

bool CRedisPipe::CleanupDevChnState(const string &strDevId)
{
    map<int, TPuChnStateConfig> cChnStateSet;
    if (GetPuChnStateSet(strDevId, cChnStateSet))
    {
        if (!cChnStateSet.empty())
        {
            for (map<int, TPuChnStateConfig>::iterator it = cChnStateSet.begin(); cChnStateSet.end() != it; ++it)
            {
                if (CHNSTATUS_ONLINE == it->second.GetChnState())
                {
                    it->second.SetChnState(CHNSTATUS_OFFLINE);
                }
            }

            return SetPuChnStateSet(strDevId, cChnStateSet);
        }

        return true;
    }

    return false;
}

bool CRedisPipe::SetPuChnState(const string &strDevId, s32 nChnNO, TPuChnStateConfig &tChnState)
{
    TPuConfig tPuConfig;
    tPuConfig.SetChnState(nChnNO, tChnState);

    return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuConfig(strDevId, tPuConfig, TPuConfig_chnStatusSet(nChnNO)));
}

bool CRedisPipe::SetPuChnStateSet(const string &strDevId, map<int, TPuChnStateConfig> &tChnStateSet)
{
    TPuConfig tPuConfig;
    tPuConfig.SetChnStateSet(tChnStateSet);

    return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuConfig(strDevId, tPuConfig, ALL_TPuConfig_chnStatusSet));
}

bool CRedisPipe::GetPuConfig(const string &strDevId, TPuConfig &tPuConfig)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().GetFullPuConfig(strDevId, tPuConfig));
}

bool CRedisPipe::SetPuConfig(const string &strDevId, TPuConfig &tPuConfig)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuConfig(strDevId, tPuConfig, ALL_TPuConfig));
}

bool CRedisPipe::GetPuDevConfig(const string &strDevId, TPuDevConfig &tPuDevConfig)
{
    TPuConfig tPuConfig;
    if (GetPuConfig(strDevId, tPuConfig))
    {
        tPuDevConfig = tPuConfig.GetDevConfig();

        return true;
    }

    return false;
}

bool CRedisPipe::MergePuConfig(const string &strDevId, TPuDevConfig& tDevConfig)
{
    TPuConfig tPuConfig;
    if (GetPuConfig(strDevId, tPuConfig))
    {
        tPuConfig.MergeDevConfig(tDevConfig);

        return (SetPuConfig(strDevId, tPuConfig));
    }

    return false;
}

bool CRedisPipe::GetExtCapSet(const string &strDevId, map<int,int> &cExtCapSet)
{
    TPuConfig tPuConfig;
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().GetPuConfig(strDevId, tPuConfig, TPuConfig_extCapSet))
    {
        cExtCapSet = tPuConfig.GetExtCapSet();

        return true;
    }

    return false;
}

bool CRedisPipe::SetExtCapSet(const string &strDevId, const map<int,int> &cExtCapSet)
{
    TPuConfig tPuConfig;
    tPuConfig.SetExtCapSet(cExtCapSet);

    return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuConfig(strDevId, tPuConfig, TPuConfig_extCapSet));
}

bool CRedisPipe::GetPuServiceStatus(const string &strDevId, TPuServiceStatus &tPuServiceStatus)
{
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetFullPuServiceStatus(strDevId, tPuServiceStatus);
    if ( IsFdbResultPositive(emRet) )
    {
        return true;
    }

    return false;
}

bool CRedisPipe::SetPuServiceStatus(const string &strDevId, const TPuServiceStatus &tPuServiceStatus)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuServiceStatus(strDevId, tPuServiceStatus, ALL_TPuServiceStatus));
}

bool CRedisPipe::GetIsPuRec(const string &strDevId, s32 nChnNO, bool &bIsRec)
{
    TPuServiceStatus tPuServiceStatus;
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetPuServiceStatus(strDevId, tPuServiceStatus, TPuServiceStatus_isPuRec(nChnNO));
    if ( IsFdbResultPositive(emRet) )
    {
        const map<int,bool> &cIsPuRec    = tPuServiceStatus.GetIsPuRec();
        map<int,bool>::const_iterator it = cIsPuRec.find(nChnNO);
        if (cIsPuRec.end() != it)
        {
            bIsRec = it->second;
        }
        else
        {
            bIsRec = false;
        }

        return true;
    }

    return false;
}

bool CRedisPipe::SetIsPuRec(const string &strDevId, s32 nChnNO, bool bIsPuRec)
{
    TPuServiceStatus tPuServiceStatus;
    tPuServiceStatus.GetIsPuRec().insert(map<int,bool>::value_type(nChnNO, bIsPuRec));

    return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuServiceStatus(strDevId, tPuServiceStatus, TPuServiceStatus_isPuRec(nChnNO)));
}

bool CRedisPipe::GetPuAudioCallSrc(const string &strDevId, s32 nChnNO, TChannel &tChn)
{
    TPuServiceStatus tPuServiceStatus;
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetPuServiceStatus(strDevId, tPuServiceStatus, TPuServiceStatus_audioCallSrc(nChnNO));
    if ( IsFdbResultPositive(emRet) )
    {
        const map<int,TChannel> &cAudioCallSrc    = tPuServiceStatus.GetAudioCallSrc();
        map<int,TChannel>::const_iterator it = cAudioCallSrc.find(nChnNO);
        if (cAudioCallSrc.end() != it)
        {
            tChn = it->second;
        }
        else
        {
            tChn.SetDevUri(string());
            tChn.SetChnNO(0);
        }

        return true;
    }

    return false;
}

bool CRedisPipe::SetPuAudioCallSrc(const string &strDevId, s32 nChnNO, const TChannel &tSrcChn)
{
    TPuServiceStatus tPuServiceStatus;
    tPuServiceStatus.GetAudioCallSrc().insert(map<int,TChannel>::value_type(nChnNO, tSrcChn));

    return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuServiceStatus(strDevId, tPuServiceStatus, TPuServiceStatus_audioCallSrc(nChnNO)));
}

bool CRedisPipe::ClearPuServiceStatus(const string &strDevId)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().ClearServiceStatus(strDevId));
}

bool CRedisPipe::GetPuAlarmStatus(const string &strDevId, TPuAlarmStatus &tPuAlarmStatus)
{
    CRedisPuData cPuData;
    map<std::string, CAlarmInputList> &cAlarmInfo = cPuData.GetPuAlarmCollector();
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetAlarmChnInfo(strDevId, cAlarmInfo);
    if ( IsFdbResultPositive(emRet) )
    {
        tPuAlarmStatus = cPuData.GetPuAlarmStatus();

        return true;
    }

    return false;
}

bool CRedisPipe::SetAlarmStatus(const string &strDevId, const TPuAlarmStatus &tPuAlarmStatus)
{
    if (EC_OK == m_cRedisSdk.GetFastDBInstance().ClearAlarmChnInfo(strDevId))
    {
        CRedisPuData cPuData;
        cPuData.UpdateAlarmStatus(tPuAlarmStatus);

        return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetAlarmChnInfo(strDevId, cPuData.GetPuAlarmCollector()));
    }

    return false;
}

bool CRedisPipe::UpdateAlarmStatus(const string &strDevId, const TPuAlarmStatus &tPuAlarmStatus)
{
    CRedisPuData cPuData;
    map<std::string, CAlarmInputList> &cAlarmInfo = cPuData.GetPuAlarmCollector();
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetAlarmChnInfo(strDevId, cAlarmInfo);
    if ( IsFdbResultPositive(emRet) )
    {
        cPuData.UpdateAlarmStatus(tPuAlarmStatus);

        return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetAlarmChnInfo(strDevId, cPuData.GetPuAlarmCollector()));
    }

    return false;
}

bool CRedisPipe::UpdateAlarmInput(const string &strDevId, const TAlarmInput &tAlarmInput)
{
    CRedisPuData cPuData;
    map<std::string, CAlarmInputList> &cAlarmInfo = cPuData.GetPuAlarmCollector();
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetAlarmChnInfo(strDevId, cAlarmInfo);
    if ( IsFdbResultPositive(emRet) )
    {
        cPuData.UpdateAlarmInput(tAlarmInput);

        return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetAlarmChnInfo(strDevId, cPuData.GetPuAlarmCollector()));
    }

    return false;
}

bool CRedisPipe::ClearAlarmStatus(const string &strDevId)
{
    return (EC_OK == m_cRedisSdk.GetFastDBInstance().ClearAlarmChnInfo(strDevId));
}

bool CRedisPipe::GetPuGpsData(const string &strDevId, TGpsData &tGpsData)
{
    CRedisPuData cPuData;
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetPuData(strDevId, cPuData, CRedisDevData_m_tGpsData);
    if ( IsFdbResultPositive(emRet) )
    {
        tGpsData = cPuData.GetPuGpsData();

        return true;
    }

    return false;
}

bool CRedisPipe::SetPuGpsData(const string &strDevId, const TGpsData &tGpsData)
{
    if (!tGpsData.GetData().empty())
    {
        CRedisPuData cPuData;
        cPuData.SetPuGpsData(tGpsData);

        return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuData(strDevId, cPuData, CRedisDevData_m_tGpsData));
    }
    
    return true;
}

bool CRedisPipe::GetPuTransparentData(const string &strDevId, TTransparentData &tTransData)
{
    CRedisPuData cPuData;
    cPuData.GetPuTransparentData().SetType(PU_TRANS_DATA_TYPE_COMDATA);
    EFDB_ErrorCode emRet = m_cRedisSdk.GetFastDBInstance().GetPuData(strDevId, cPuData, CRedisDevData_m_tTransparentData(PU_TRANS_DATA_TYPE_COMDATA));
    if ( IsFdbResultPositive(emRet) )
    {
        tTransData = cPuData.GetPuTransparentData();

        return true;
    }
    
    return false;
}

bool CRedisPipe::SetPuTransparentData(const string &strDevId, const TTransparentData &tTransData)
{
    if (tTransData.GetData().empty())
    {
        CRedisPuData cPuData;
        cPuData.SetPuTransparentData(tTransData);

        return (EC_OK == m_cRedisSdk.GetFastDBInstance().SetPuData(strDevId, cPuData, CRedisDevData_m_tTransparentData(tTransData.GetType())));
    }
    
    return true;
}

void CRedisPipe::PublishPuStatus(const string &strDomainId, const string &strDomainName, const string &strDevId, 
    const TPuStatus &tPuStatus, TSsMode tSsMode)
{
    if (ALL_SS == tSsMode || LIST_SS == tSsMode)
    {
         m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetOnOffSubChn(strDevId), tPuStatus.ToXml());
    }

    if (ALL_SS == tSsMode || CATALOG_SS == tSsMode)
    {
        TSipURI tDevUri;
        tDevUri.SetUser(strDevId);
        tDevUri.SetDomain(strDomainName);

        CPuStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(tDevUri.GetURIString());
        cNtfReq.SetOnOffStatus(tPuStatus);
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetOnOffCatalogSubChn(strDomainId), cNtfReq.ToXml());
    }
}

void CRedisPipe::PublishPuConfig(const string &strDomainId, const string &strDomainName, const string &strDevId, 
    const TPuDevConfig &tPuDevConfig, TSsMode tSsMode)
{
    if (ALL_SS == tSsMode || LIST_SS == tSsMode)
    {
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetConfigSubChn(strDevId), tPuDevConfig.ToXml());
    }

    if (ALL_SS == tSsMode || CATALOG_SS == tSsMode)
    {
        TSipURI tDevUri;
        tDevUri.SetUser(strDevId);
        tDevUri.SetDomain(strDomainName);

        CPuDevConfigNtyReq cNtfReq;
        cNtfReq.SetDevURI(tDevUri.GetURIString());
        cNtfReq.SetConfig(tPuDevConfig);
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetConfigCatalogSubChn(strDomainId), cNtfReq.ToXml());
    }
}

void CRedisPipe::PublishPuServiceStatus(const string &strDomainId, const string &strDomainName, const string &strDevId, 
    const TPuServiceStatus &tPuServiceStatus, TSsMode tSsMode)
{
    if (ALL_SS == tSsMode || LIST_SS == tSsMode)
    {
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetServiceSubChn(strDevId), tPuServiceStatus.ToXml());
    }

    if (ALL_SS == tSsMode || CATALOG_SS == tSsMode)
    {
        TSipURI tDevUri;
        tDevUri.SetUser(strDevId);
        tDevUri.SetDomain(strDomainName);

        CPuServiceStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(tDevUri.GetURIString());
        cNtfReq.SetServiceStatus(tPuServiceStatus);
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetServiceCatalogSubChn(strDomainId), cNtfReq.ToXml());
    }
}

void CRedisPipe::PublishPuAlarmStatus(const string &strDomainId, const string &strDomainName, const string &strDevId, 
    const TPuAlarmStatus &tPuAlarmStatus, TSsMode tSsMode)
{
    if (ALL_SS == tSsMode || LIST_SS == tSsMode)
    {
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetAlarmSubChn(strDevId), tPuAlarmStatus.ToXml());
    }

    if (ALL_SS == tSsMode || CATALOG_SS == tSsMode)
    {
        TSipURI tDevUri;
        tDevUri.SetUser(strDevId);
        tDevUri.SetDomain(strDomainName);

        CPuAlarmStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(tDevUri.GetURIString());
        cNtfReq.SetAlarmStatus(tPuAlarmStatus);
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetAlarmCatalogSubChn(strDomainId), cNtfReq.ToXml());
    }
}

void CRedisPipe::PublishPuGpsData(const string &strDomainId, const string &strDomainName, const string &strDevId, 
    const TGpsData &tGpsData, TSsMode tSsMode)
{
    if (ALL_SS == tSsMode || LIST_SS == tSsMode)
    {
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetGpsSubChn(strDevId), tGpsData.ToXml());
    }

    if (ALL_SS == tSsMode || CATALOG_SS == tSsMode)
    {
        TSipURI tDevUri;
        tDevUri.SetUser(strDevId);
        tDevUri.SetDomain(strDomainName);

        CPuGpsStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(tDevUri.GetURIString());
        cNtfReq.SetGpsData(tGpsData);
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetGpsCatalogSubChn(strDomainId), cNtfReq.ToXml());
    }
}

void CRedisPipe::PublishPuTransparentData(const string &strDomainId, const string &strDomainName, const string &strDevId, 
    const TTransparentData &tTransData, TSsMode tSsMode)
{
    if (ALL_SS == tSsMode || LIST_SS == tSsMode)
    {
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetTransDataSubChn(strDevId), tTransData.ToXml());
    }

    if (ALL_SS == tSsMode || CATALOG_SS == tSsMode)
    {
        TSipURI tDevUri;
        tDevUri.SetUser(strDevId);
        tDevUri.SetDomain(strDomainName);

        CPuTransDataStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(tDevUri.GetURIString());
        cNtfReq.SetData(tTransData);
        m_cRedisSdk.GetFastDBInstance().FDBPublish(RedisGetTransDataCatalogSubChn(strDomainId), cNtfReq.ToXml());
    }
}

void CRedisPipe::AddSs(const string &strChn)
{
    vector<string> cChn;
    cChn.push_back(strChn);
    AddSs(cChn);
}

void CRedisPipe::AddSs(const vector<string> &cChn)
{
    vector<string> cSsList;
    for (vector<string>::const_iterator it = cChn.begin(); cChn.end() != it; ++it)
    {
        set<string>::const_iterator it4Sub = m_cSsList.find(*it);
        if (m_cSsList.end() == it4Sub)
        {
            cSsList.push_back(*it);
        }
    }

    if (!cSsList.empty() && EC_OK == m_cRedisSdk.GetFastDBInstance().FDBSubscribe(cSsList, cbRedisSub, this))
    {
        m_cSsList.insert(cSsList.begin(), cSsList.end());
    }
}

void CRedisPipe::DelSs(const string &strChn)
{
    vector<string> cChn;
    cChn.push_back(strChn);

    DelSs(cChn);
}

void CRedisPipe::DelSs(const vector<string> &cChn)
{
    vector<string> cSsList;
    for (vector<string>::const_iterator it = cChn.begin(); cChn.end() != it; ++it)
    {
        set<string>::iterator it4Sub = m_cSsList.find(*it);
        if (m_cSsList.end() != it4Sub)
        {
            m_cSsList.erase(it4Sub);

            cSsList.push_back(*it);
        }
    }

    if (!cSsList.empty())
    {
        m_cRedisSdk.GetFastDBInstance().FDBUnsubscribe(cSsList);
    }
}

void CRedisPipe::RedisReply2PuRegInfo(redisReply *pcItemReply, TPuRegInfo &tInfo)
{
    if (NULL != pcItemReply && NULL != pcItemReply->element && REDIS_REPLY_ARRAY == pcItemReply->type)
    {
        // Lua�ű���ÿ���豸����ȡ��11���ֶ���Ϣ
        CMS_ASSERT(pcItemReply->elements == 11 && "impossible!!!");

        redisReply *pcFieldReply = (pcItemReply->element)[0];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            tInfo.use = atoi(pcFieldReply->str);
        }

        pcFieldReply = (pcItemReply->element)[1];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            CCMSTime cTime;
            cTime.FromString(pcFieldReply->str);

            tInfo.expire = time(NULL) > cTime.GetTime() ? true : false;
        }

        pcFieldReply = (pcItemReply->element)[2];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            tInfo.chnnum = atoi(pcFieldReply->str);
        }

        pcFieldReply = (pcItemReply->element)[3];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            tInfo.freeLicense = atoi(pcFieldReply->str);
        }

        pcFieldReply = (pcItemReply->element)[4];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            tInfo.license = atoi(pcFieldReply->str);
        }

        pcFieldReply = (pcItemReply->element)[5];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            tInfo.creator = atoi(pcFieldReply->str);
        }

        pcFieldReply = (pcItemReply->element)[6];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            tInfo.name = pcFieldReply->str;
        }

        pcFieldReply = (pcItemReply->element)[7];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            tInfo.alias = pcFieldReply->str;
        }

        pcFieldReply = (pcItemReply->element)[8];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            tInfo.connectIp = pcFieldReply->str;
        }

        pcFieldReply = (pcItemReply->element)[9];
        if (NULL != pcFieldReply && REDIS_REPLY_STRING == pcFieldReply->type)
        {
            tInfo.puiIp = pcFieldReply->str;
        }

        pcFieldReply = (pcItemReply->element)[10];
        if (NULL != pcFieldReply && REDIS_REPLY_INTEGER == pcFieldReply->type)
        {
            tInfo.chnStatusFlag = (bool)pcFieldReply->integer;
        }
    }
}

TRedisSwitchType CRedisPipe::SdpSessionName2RedisSwitchType(const TSdpSessionName &tSessionName)
{
    if (SDP_SESSION_PLAY == tSessionName)
    {
        return RedisSwitchType_Play;
    }
    else if (SDP_SESSION_PLAYBACK == tSessionName)
    {
        return RedisSwitchType_Playback;
    }
    else if (SDP_SESSION_DOWNLOAD == tSessionName)
    {
        return RedisSwitchType_Download;
    }
    else if (SDP_SESSION_AUDIOCALL == tSessionName)
    {
        return RedisSwitchType_Audiocall;
    }
    else
    {
        return TRedisSwitchType();
    }
}

TSdpSessionName  CRedisPipe::RedisSwitchType2SdpSessionName(const TRedisSwitchType &tSwitchType)
{
    if (RedisSwitchType_Play == tSwitchType)
    {
        return SDP_SESSION_PLAY;
    }
    else if (RedisSwitchType_Playback == tSwitchType)
    {
        return SDP_SESSION_PLAYBACK;
    }
    else if (RedisSwitchType_Download == tSwitchType)
    {
        return SDP_SESSION_DOWNLOAD;
    }
    else if (RedisSwitchType_Audiocall == tSwitchType)
    {
        return SDP_SESSION_AUDIOCALL;
    }
    else
    {
        return TSdpSessionName();
    }
}
