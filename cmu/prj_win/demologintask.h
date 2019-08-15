/*========================================================================================
模块名    ：server
文件名    ：demologintask.h
相关文件  ：
实现功能  ：demo用户登录注册事务
作者      ：zhaiyb
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2019/08/14         V1.0              zhaiyb                                 新建文件
=========================================================================================*/

#ifndef _DEMON_LOGIN_TASK_H
#define _DEMON_LOGIN_TASK_H

#include "loginsession.h"



class CDemoLoginTask : public CLoginSession
{
public:
	CDemoLoginTask(CInstExt *pcInst);
	virtual ~CDemoLoginTask();


private:
	

};









#endif