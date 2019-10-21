#include "dbixface.h"


CVkekInfoTable g_cVkekInfoTable;
CCertificateInfoTable g_cCertificateInfoTable;

struct TColInfoStruct
{
	LPCSTR colName;
	ENDBDataType colDataType;
};


TColInfoStruct  g_atVkekInfoTable[] = 
{
	{VKIT_CHANNELID, enDBDataTypeS32},
	{VKIT_PUID,      enDBDataTypeString},
	{VKIT_TMFROM,    enDBDataTypeString},
	{VKIT_VKEKS,     enDBDataTypeString},
};


#ifndef ARRAYSIZE
#define ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))
#endif


CVkekInfoTable::CVkekInfoTable() : CDBEntitySchema()
{
	vector<TColInfo> cColInfoList;

	for (int i=0; i<(int)ARRAYSIZE(g_atVkekInfoTable); i++)
	{
		TColInfo tColInfo;
		tColInfo.colName = g_atVkekInfoTable[i].colName;
		tColInfo.colDataType = g_atVkekInfoTable[i].colDataType;
		cColInfoList.push_back(tColInfo);
	}
	InitSchema(VKEK_INFO_TABLE, cColInfoList);
}

//证书信息
TColInfoStruct g_atCertificateInfoTable[] =
{
	{CRTIT_TYPE,    enDBDataTypeS32},
	{CRTIT_OWNERID, enDBDataTypeString},
	{CRTIT_SIGN,    enDBDataTypeString},
	{CRTIT_ENCRY,   enDBDataTypeString},
};

CCertificateInfoTable::CCertificateInfoTable() : CDBEntitySchema()
{
	vector<TColInfo> cColInfoList;

	for (int i = 0; i<(int)ARRAYSIZE(g_atCertificateInfoTable); i++)
	{
		TColInfo tColInfo;
		tColInfo.colName = g_atVkekInfoTable[i].colName;
		tColInfo.colDataType = g_atVkekInfoTable[i].colDataType;
		cColInfoList.push_back(tColInfo);
	}
	InitSchema(CRTIT_INFO_TABLE, cColInfoList);
}