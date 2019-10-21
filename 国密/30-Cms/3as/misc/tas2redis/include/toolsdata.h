#ifndef _TOOLSDATA_H_
#define _TOOLSDATA_H_

#include <string>
#include <map>
#include <set>
#include "cms/tas/tas_proto.h"

using namespace std;


struct TExtDeviceInfo
{
	CDeviceInfo deviceInfo;
	TDeviceCapName deviceCapName;

	TExtDeviceInfo()
	{
		deviceInfo.Clear();
		deviceCapName.Clear();
	}
};

class CDeviceTable
{
public:
	map<string,TExtDeviceInfo> m_mapDeviceInfo;  //kdid-TDeviceInfo

public:
	void Clean();
	void AddDevice(const TExtDeviceInfo &tDev);
	void GetAllDevice(vector<TExtDeviceInfo> &tDev);
};

extern CDeviceTable g_cDeviceTable;


struct TUser2UserGrp
{
	string userid;
	string usergrpid;
};







#endif //_TOOLSDATA_H_


