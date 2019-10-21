/*****************************************************************************
   ����        : gbu redisͷ�ļ�����
   ����        : liuyongtao
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2016/03/9  1.0         liuyongtao       ����
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

//���ݷ���ͨ��
const string FDB_CHN_GBU_DEVICE_CHANGE="gbu_device_change";             // GBU�����豸���ͨ��(to tas),��Ϣ�壺TTasDeviceBatchChangeItem
const string FDB_CHN_GBU_GBCATALOG_CHANGE="gbu_gbcatalog_change";       // GBU����Ŀ¼���ͨ����to gbs��,��Ϣ�壺CGbuDeviceCatalogNtf

//keyǰ׺
const string FDB_KEY_PREFIX_GBUA_TO_GBU = "gbua2gbu:";                 //keyǰ׺��gbua-->gbuId
const string FDB_KEY_PREFIX_GBUA_TO_GBUASTATU = "gbua2gbuastat:";      //keyǰ׺��gbua-->gbua״̬��ON,OFF,REGING��
const string FDB_KEY_PREFIX_GBUA_TO_DEVOFFLINE = "gbua2offlinedev:";       //keyǰ׺��gbua-->���ߵ�gbdev�豸
const string FDB_KEY_PREFIX_GBUA_TO_GBID = "gbua2gbid:";                   //keyǰ׺��gbua-->�ϱ���gbid
const string FDB_KEY_PREFIX_GBUA_TO_GBID_HANDLING = "gbua2gbid_handling:"; //��ʾ���ڴ���FDB_KEY_PREFIX_GBUA_TO_GBID

#endif // _GBU_REDIS_H_











