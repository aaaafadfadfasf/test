#include "sql_layer.h"

DBIX_NS_BEGIN

std::string DBData( bool val )
{
    return val? "1":"0"; 
}

std::string DBData( s32 val )
{
    s8 szBuf[40];
    sprintf(szBuf, "%d", val);
    return szBuf; 
}

std::string DBData( double val )
{
    s8 szBuf[40];
    snprintf(szBuf, sizeof(szBuf), "%.15f", val);
    szBuf[sizeof(szBuf)-1]='\0';
    return szBuf; 
}

std::string DBData( const std::string &val )
{
    string strtmp(val);
    string strSrc="'";
    string strDst="\\'";
    _replace_str(strtmp, strSrc, strDst);

    strtmp.insert(0, 1, '\'');
    strtmp.append(1, '\'');

    return strtmp;
}

std::string DBData( const s8* val )
{
    string strtmp;
    strtmp = val;
    return DBData(strtmp);
}

std::string DBData( const CCMSTime &val )
{
    std::stringstream ssNum;
    ssNum << static_cast<u64>(val.GetTime());
    return ssNum.str().c_str();
}


// like�ﾳ�¶��������ַ���֧�֣���ͬ���ݿ���ܲ�ͬ���˺����Գ�%,_,\��������ַ��Զ�ת��
std::string DBLikeString(const std::string &val, ENDBType enDBType)
{
    string strtmp = val;
    
    if (enDBTypeMysql == enDBType)
    {
        // none
    }
    else if (enDBTypeSybase == enDBType)
    {
        // ?
    }
    else if (enDBTypeOracle == enDBType)
    {
        // ?
    }
    else
    {
        log(enLOGLVL_ERROR, "DBLikeString detected not supported database type");
    }

    strtmp = DBData(strtmp);

    return strtmp; // hzytodo ��ͬ���ݿ���������Զ�ת��
}

// �������ĺ�������ʹ�ó�ʼ����ʱ���ӵ����ݿ�������ΪenDBType
std::string DBLikeString(const std::string &val)
{
    return DBLikeString(val, g_tDBIXConf.vecTaskQueueConf.front().tDBConnectCfg.enDbType);
}

void GenerateExecProcSql(string &strSql, const string &strProcName, const vector<TVariantWrapper> &tParms, ENDBType enDbType)
{
    TSqlString tSql;

    if (enDBTypeMysql == enDbType)
    {
        tSql.Format("call %s(", strProcName.c_str());
        for (u32 i = 0; i < tParms.size(); ++i)
        {
            tSql.AppendString("%s", DBData(tParms[i].DataVal(), tParms[i].DataType()).c_str());
            tSql.AppendString(",");
        }

        tSql.EraseLastRedundantComma();
        tSql.AppendString(")");
    }
    else if (enDBTypeOracle == enDbType)
    {
        tSql.Format("exec %s(", strProcName.c_str());
        for (u32 i = 0; i < tParms.size(); ++i)
        {
            tSql.AppendString("%s", DBData(tParms[i].DataVal(), tParms[i].DataType()).c_str());
            tSql.AppendString(",");
        }

        tSql.EraseLastRedundantComma();
        tSql.AppendString(")");
    }
    else if (enDBTypeSybase == enDbType)
    {
        tSql.Format("exec %s ", strProcName.c_str());
        for (u32 i = 0; i < tParms.size(); ++i)
        {
            tSql.AppendString("%s", DBData(tParms[i].DataVal(), tParms[i].DataType()).c_str());
            tSql.AppendString(",");
        }

        tSql.EraseLastRedundantComma();
    }
    else
    {
        log(enLOGLVL_ERROR, "GenerateExecProcSql detected not supportted database type");
    }

    strSql = tSql.GetBuf();
}

DBIX_NS_END
