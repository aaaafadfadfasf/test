#include "toolsdata.h"


CDeviceTable g_cDeviceTable;


void CDeviceTable::Clean()
{
	m_mapDeviceInfo.clear();
}

void CDeviceTable::AddDevice(const TExtDeviceInfo &tDev)
{
	m_mapDeviceInfo.insert(pair< string,TExtDeviceInfo >(tDev.deviceInfo.GetDeviceUUID(),tDev));
}

void CDeviceTable::GetAllDevice(vector<TExtDeviceInfo> &tDev)
{
	tDev.clear();
	map<string,TExtDeviceInfo>::const_iterator it = m_mapDeviceInfo.begin();
	for (;it != m_mapDeviceInfo.end();it++)
	{
		tDev.push_back(it->second);
	}
}