#ifndef _PMS_SPECIAL_STRUCT_H_
#define _PMS_SPECIAL_STRUCT_H_

#include <string>
#include <vector>
using std::string;
using std::vector;
#include "cms/utility/strtok.h"
#include "cms/utility/digital_string.h"


struct TPmcLoginRspData
{
	#define TPMC_LOGIN_RSP_DATA_EMPTY "empty"
	#define TPMC_LOGIN_RSP_DATA_DELIM ":"
public:
	TPmcLoginRspData()
	{
		Clear();
	}
	void Clear()
	{
		ipaddr.clear();
		securemail.clear();
		needsetsecuremail = 0;
		pwdLockedLeftTime = 0;
		logoutInterval = 0;
		pwdExpireLeftTime = 0;
	}
	
public:
	string ipaddr;
	string securemail;
	int needsetsecuremail;   //0为不需要，1为需要
	int  pwdLockedLeftTime; //密码被锁定剩余时间,单位：秒
	int  logoutInterval;    //自动注销间隔
	int  pwdExpireLeftTime; //密码到期剩余时间
public:
	string ToString()
	{
		string tmp;
		if(ipaddr.empty())
		{
			tmp += string(TPMC_LOGIN_RSP_DATA_EMPTY);
		}
		else
		{
			tmp += ipaddr;
		}
		tmp += string(TPMC_LOGIN_RSP_DATA_DELIM);
		if(securemail.empty())
		{
			tmp += string(TPMC_LOGIN_RSP_DATA_EMPTY);
		}
		else
		{
			tmp += securemail;
		}
		tmp += string(TPMC_LOGIN_RSP_DATA_DELIM);
		char tmpneedsetsecuremail[8]="";
		sprintf(tmpneedsetsecuremail, "%d", needsetsecuremail);

		tmp += string(tmpneedsetsecuremail);
		tmp += string(TPMC_LOGIN_RSP_DATA_DELIM);

		tmp += digital_string(pwdLockedLeftTime);
		tmp += string(TPMC_LOGIN_RSP_DATA_DELIM);
		tmp += digital_string(logoutInterval);
		tmp += string(TPMC_LOGIN_RSP_DATA_DELIM);
		tmp += digital_string(pwdExpireLeftTime);
			
		return tmp;
	}
	void FromString(const string& strRspData)
	{
		vector<string> vecTmp;
	    strtok(strRspData.c_str(), TPMC_LOGIN_RSP_DATA_DELIM, vecTmp);
	    if (vecTmp.size() >= 6)
	    {
	       ipaddr = vecTmp[0];
	       securemail = vecTmp[1];
		   needsetsecuremail = atoi(vecTmp[2].c_str());
		   pwdLockedLeftTime = atoi(vecTmp[3].c_str());
		   logoutInterval = atoi(vecTmp[4].c_str());
		   pwdExpireLeftTime = atoi(vecTmp[5].c_str());
	     }
	     else
	     {
	     	 ipaddr.clear();
	     	 securemail.clear();
			 needsetsecuremail = 0;
			 pwdLockedLeftTime = 0;
			 logoutInterval = 0;
			 pwdExpireLeftTime = 0;
	 	 }
	 	 if(ipaddr == TPMC_LOGIN_RSP_DATA_EMPTY)
	 	 {
	 	 	 ipaddr.clear();
	 	 }
	 	 if(securemail == TPMC_LOGIN_RSP_DATA_EMPTY)
	 	 {
	 	 	 securemail.clear();
	 	 }
	}
};
#endif // _PMS_SPECIAL_STRUCT_H_







