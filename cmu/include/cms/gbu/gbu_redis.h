/*****************************************************************************
   功能        : gbu redis头文件定义
   作者        : liuyongtao
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2016/03/9  1.0         liuyongtao       创建
******************************************************************************/
#ifndef _GBU_REDIS_H_
#define _GBU_REDIS_H_

#include "kdvtype.h"
#include "cms/cmsassert.h"
#include "cms/utility/uuid.h"
#include "cms/cms_proto.h"
#include "cms/cms_const.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using std::string;
using std::vector;
using std::set;
using std::map;

//数据发布通道
const string FDB_CHN_GBU_DEVICE_CHANGE="gbu_device_change";             // GBU发布设备变更通道(to tas),消息体：TTasDeviceBatchChangeItem
const string FDB_CHN_GBU_GBCATALOG_CHANGE="gbu_gbcatalog_change";       // GBU发布目录变更通道（to gbs）,消息体：CGbuDeviceCatalogNtf

//key前缀
const string FDB_KEY_PREFIX_GBUA_TO_GBU = "gbua2gbu:";                 //key前缀，gbua-->gbuId
const string FDB_KEY_PREFIX_GBUA_TO_GBUASTATU = "gbua2gbuastat:";      //key前缀，gbua-->gbua状态（ON,OFF,REGING）
const string FDB_KEY_PREFIX_GBUA_TO_DEVOFFLINE = "gbua2offlinedev:";       //key前缀，gbua-->下线的gbdev设备
const string FDB_KEY_PREFIX_GBUA_TO_GBID = "gbua2gbid:";                   //key前缀，gbua-->上报的gbid
const string FDB_KEY_PREFIX_GBUA_TO_GBID_HANDLING = "gbua2gbid_handling:"; //表示正在处理FDB_KEY_PREFIX_GBUA_TO_GBID

#endif // _GBU_REDIS_H_











