#ifndef _CUI_PROTO_CONVERTOR_H_
#define _CUI_PROTO_CONVERTOR_H_
#include "../soap/soapcusdkService.h"
#include "../../common/cuiproto.h"
#include "../../common/cuistruct.h"
#include "cms/utility/crontime.h"
#include "cbb/zlib/zlib.h"
#include "cms/utility/base64codec.h"
#include "cms/dps/dps_proto.h"
#include "cms/pu/mtctrl.h"
#include "cms/ospsip/ospsip.h"
#include "alarmtypecommon.h"
enum ECuDevicePri
{
	en_CU_PriPUInvalid		= 0, //无效的设备权限
	en_CU_PriPURecStart		= 1, // 开始前端录像.
	en_CU_PriPURecPlay		= 2, // 播放前端录像
	en_CU_PriPURecDel		= 3, // 删除前端录像.
	en_CU_PriPURecDownload	= 4, // 前端录像下载.

	en_CU_PriPUConfig		= 5, // 前端参数配置. 针对视频源的部分怎么处理??
	en_CU_PriPUAlarmOutput	= 6, // 前端开关量输出控制.
	en_CU_PriPUAlarmConfig	= 7, // 布防配置.	
	en_CU_PriPuAudioCtrl	= 8, // 音频控制.
	en_CU_PriPuSavePreset	= 9, // 预置位保存权限.
	en_CU_PriPuPtzLock      = 10,// 预置位保存权限.
};


const int DISABLE_MAX_SWITCH_NUM = -1;
template<typename CuStruct, typename CuiClass>
void MaxStreamCuToCui( const CuStruct &cu, CuiClass &cui )
{
    cui.SetSwitchNum( cu.maxStreamNumEnable ? cu.maxStreamNum : DISABLE_MAX_SWITCH_NUM );
}

template<typename CuStruct, typename CuiClass>
void MaxStreamCuiToCu( const CuiClass &cui, CuStruct &cu )
{
    cu.maxStreamNumEnable = cui.GetSwitchNum() != DISABLE_MAX_SWITCH_NUM ;
    cu.maxStreamNum = cu.maxStreamNumEnable ? cui.GetSwitchNum() : 0;
}

template<typename CuStruct, typename CuiClass>
void WaterMarkCuToCui( const CuStruct &cu, CuiClass &cui )
{
    if( cu.waterMarkSupported )
    {
        cui.SetWaterMarkEnabled( cu.waterMarkEnabled );	
        cui.SetWaterMarkFont(  cu.waterMarkFont );	
        cui.SetWaterMarkFontSize(cu.waterMarkFontSize );	
        cui.SetWaterMarkRotationAngle(cu.waterMarkRotationAngle );	
        cui.SetWaterMarkColor( cu.waterMarkColor );
        cui.SetWaterMarkTransparent( cu.waterMarkTransparent );
        cui.SetWaterMarkTiled( cu.waterMarkTiled );
    }
}

template<typename CuStruct, typename CuiClass>
void WaterMarkCuiToCu( const CuiClass &cui, CuStruct &cu )
{
    cu.waterMarkEnabled         = cui.GetWaterMarkEnabled();	
    cu.waterMarkFont            = cui.GetWaterMarkFont();	
    cu.waterMarkFontSize        = cui.GetWaterMarkFontSize();	
    cu.waterMarkRotationAngle   = cui.GetWaterMarkRotationAngle();	
    cu.waterMarkColor	        = cui.GetWaterMarkColor();
    cu.waterMarkTransparent     = cui.GetWaterMarkTransparent();
    cu.waterMarkTiled           = cui.GetWaterMarkTiled();
    cu.waterMarkSupported       = true;
}

void ConvertCuTimeRangeToCmsTimeRange( const cusdk__TimeRange tCuTimeRange, TTimeRange &tCmsTimeRange )
{
    CCMSTime begin( tCuTimeRange.startTime );
    tCmsTimeRange.SetStartTime(begin.ToString());
    CCMSTime end( tCuTimeRange.endTime );
    tCmsTimeRange.SetEndTime(end.ToString());
}

void ConvertCPuLongOsdParamFromCuiToCu(const TLongOsdParam &tFrom, cusdk__LongOSDParam &tTo)
{
	tTo.color = tFrom.GetColor();
	tTo.leftMargin = tFrom.GetLeftMargin();
	switch (tFrom.GetState())
	{
	case OSD_DISABLE:
		tTo.state = cusdk__OSDStateDisable;
		break;
	case OSD_TITLE:
		tTo.state = cusdk__OSDStateText;
		break;
	case OSD_LOGOBMP:
		tTo.state = cusdk__OSDStateDisable;
		break;
	default:
		tTo.state = cusdk__OSDStateInvalid;
		break;
	}
	tTo.text = tFrom.GetText();
	tTo.topMargin = tFrom.GetTopMargin();
}

void ConvertNetAddrFromCuToCui(const cusdk__NetworkAddr &tFrom, TNetAddr &tTo)
{
    tTo.SetNetIp(tFrom.ip);
    tTo.SetNetPort(tFrom.port);
}

void ConvertNetAddrFromCuiToCu(const TNetAddr &tFrom, cusdk__NetworkAddr &tTo)
{
    tTo.ip = tFrom.GetNetIp();
    tTo.port = tFrom.GetNetPort();
}

void UnzipTvWallSchema(const string& strZipSchema,TTvWallScheme& tScheme)
{
	string strZipString = Base64Decode(strZipSchema);
	u8 abyBuff[100*1024] = {0};
	uLong dwBuffLen = sizeof(abyBuff);
	if (Z_OK != uncompress(abyBuff, &dwBuffLen, (u8*)strZipString.c_str(),strZipString.length()))
	{
		return;
	}
	string strUnzip((char*)abyBuff,dwBuffLen);
	tScheme.FromXml(strUnzip);
}

void ConvertSubscriptionListFromCUToCUI(const vector<cusdk__NotifySubscription >&tFrom, vector<CNotifySubscription> &tTo)
{
    if (tFrom.empty())
    {
        return;
    }

    vector< cusdk__NotifySubscription >::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
        CNotifySubscription cSubscription;
        cSubscription.SetDeviceURI(pItem->deviceId);
        cSubscription.SetSubscribeNotifyType(pItem->subscribeNotifyType);
        set<int> &tmpSet = cSubscription.GetSubscribeType();
        std::set<enum cusdk__EPuSubscribeType >::const_iterator pSubTypeItem = pItem->subscribeTypes.begin();
        while ( pSubTypeItem != pItem->subscribeTypes.end() )
        {
            tmpSet.insert(static_cast<int>(*pSubTypeItem));
            pSubTypeItem++;
        }
        tTo.push_back(cSubscription);
        pItem++;
    }
}

void ConvertSubscriptionListFromCUIToCU(const vector<CNotifySubscription> &tFrom, vector<cusdk__NotifySubscription >&tTo)
{
    if (tFrom.empty())
    {
        return;
    }

    vector<CNotifySubscription>::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
        cusdk__NotifySubscription cSubscription;
        cSubscription.subscribeNotifyType = static_cast<cusdk__ESubscribeNotifyType>(pItem->GetSubscribeNotifyType());
        cSubscription.deviceId = pItem->GetDeviceURI();
        const set<int> &tmpSet = pItem->GetSubscribeType();
        set<int>::const_iterator pSubTypeItem = tmpSet.begin();
        while ( pSubTypeItem != tmpSet.end() )
        {
            cSubscription.subscribeTypes.insert(static_cast<enum cusdk__EPuSubscribeType>(*pSubTypeItem));
            pSubTypeItem++;
        }
        tTo.push_back(cSubscription);
        pItem++;
    }
}

void ConvertUserPriFromCuiToCu(const CUserPrivilege& tFrom, set<enum cusdk__EUserPri > &tTo)
{
    for ( int nPri=1;nPri<32;nPri++ )
    {
        CUserPrivilege::TPrivilege emPri = static_cast<CUserPrivilege::TPrivilege>(nPri);
        if ( tFrom.HasPrivilege(emPri) )
        {
            tTo.insert(static_cast<cusdk__EUserPri>(emPri));
        }
    }
}

void ConvertUserInfoFromCuiToCu( const CCuUserInfo &tFrom, cusdk__UserInfo &tTo)
{
    tTo.uuid = tFrom.GetUserID();
    tTo.userName = tFrom.GetUserName();
    tTo.userPassword = tFrom.GetUserPwd();
    tTo.domainID = tFrom.GetDomainID();
    tTo.userType = static_cast<cusdk__EUserType>(tFrom.GetUserType());

    //将set<int> 强转为 set<enum cusdk__EUserPri >
    ConvertUserPriFromCuiToCu(tFrom.GetUserPrivilege(), tTo.userPri);
    tTo.creator = tFrom.GetCreatorID();
    tTo.userGroup = tFrom.GetUsergrpID();
    tTo.description = tFrom.GetDescription();
    CCMSTime cNameExpireData(tFrom.GetNameExpireDate());
    tTo.accountOverdueTime = cNameExpireData.GetTime();
    CCMSTime cPwdExpireData(tFrom.GetPwdExpireDate());
    tTo.passwordOverdueTime = cPwdExpireData.GetTime();
    tTo.enable = tFrom.GetEnabledFlag();
    tTo.allowMultilogin = tFrom.GetMLoginFlag();
	tTo.passwordExpireEnable = tFrom.GetPwdExpireEnabledFlag();
	tTo.passwordExpire = tFrom.GetPwdExpireTime();
	tTo.bindingAddress = tFrom.GetLoginIpList();
	tTo.bindIPEnable = tFrom.GetIpListEnabledFlag();
	tTo.IsIPAppliedToMobileEnd = tFrom.GetIpBindFlagForMobileClient();
    MaxStreamCuiToCu( tFrom, tTo );

    WaterMarkCuiToCu( tFrom, tTo );
    
}


void ConvertPlatRecMapToVector(const std::map<int,TRecStatus> &tFrom, std::vector<cusdk__RecordStatus > &tTo)
{
    map<int,TRecStatus>::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
        std::vector<cusdk__RecordStatus >::value_type tItem;
        tItem.encoderChannelId = pItem->first;
        tItem.errorCode = pItem->second.GetLastError();

        if(PLAT_REC_NO == pItem->second.GetRecStatus())
        {
            tItem.recordStatus =cusdk__RecordStatusIdel;
        }
        else if(PLAT_REC_OK == pItem->second.GetRecStatus())
        {
            tItem.recordStatus =cusdk__RecordStatusOk;
        }
        else if(PLAT_REC_TRY == pItem->second.GetRecStatus())
        {
            tItem.recordStatus =cusdk__RecordStatusTry;
        }
        else if(PLAT_REC_STOPPING == pItem->second.GetRecStatus())
        {
            tItem.recordStatus = cusdk__RecordStatusStopping;
        }


        tTo.push_back(tItem);
        pItem++;
    }
}

template<typename TMap, typename TVect>
void ConvertMapToVector(TMap &tFrom, TVect &tTo)
{
    typename TMap::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
        typename TVect::value_type tItem;
        tItem.id = pItem->first;
        tItem.value = pItem->second;
        tTo.push_back(tItem);
        pItem++;
    }
}

template<>
void ConvertMapToVector<const std::map<int,TChannel >, std::vector<cusdk__VidDecoderToPuChn> >
    ( const std::map<int,TChannel > &tFrom, std::vector<cusdk__VidDecoderToPuChn> &tTo )
{
    std::map<int,TChannel >::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
        std::vector<cusdk__VidDecoderToPuChn>::value_type tItem;
        tItem.id = pItem->first;
        cusdk__DevChannel tChan;
        tChan.deviceId = pItem->second.GetDevUri();
        tChan.channelId = pItem->second.GetChnNO();
        tItem.puChn = tChan;
        tTo.push_back(tItem);

        pItem++;
    }
}

template<>
void ConvertMapToVector<const std::map<int,TChannel >, std::vector<cusdk__AudDecoderToCuChn> >
    ( const std::map<int,TChannel > &tFrom, std::vector<cusdk__AudDecoderToCuChn> &tTo )
{
    std::map<int,TChannel >::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
        std::vector<cusdk__AudDecoderToCuChn>::value_type tItem;
        tItem.id = pItem->first;
        cusdk__DevChannel tChan;
        tChan.deviceId = pItem->second.GetDevUri();
        tChan.channelId = pItem->second.GetChnNO();
        tItem.cuChn = tChan;
        tTo.push_back(tItem);

        pItem++;
    }
}

// cms/pu/mtctrl.h
void ConvertVideoResByTitle(const CVidRes &videoRes, int &nWidth, int &nHeigh)
{
	nWidth = nHeigh = MTI_VIDEO_RESOLUTION_NONE;

	if (videoRes == VID_RES_AUTO)
	{
		return;
	}
	else if (videoRes == VID_RES_QCIF)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_176;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_144;
	}
	else if (videoRes == VID_RES_CIF)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_352;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_288;
	}
	else if (videoRes == VID_RES_2CIF)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_704;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_288;
	}
	else if (videoRes == VID_RES_4CIF)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_704;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_576;
	}
	else if (videoRes == VID_RES_QQCIF)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_88;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_72;
	}
	else if (videoRes == VID_RES_VGA)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_640;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_480;
	}
	else if (videoRes == VID_RES_QVGA)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_320;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_240;
	}
	else if (videoRes == VID_RES_720P)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_1280;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_720;
	}
	else if (videoRes == VID_RES_1080P)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_1920;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_1080;
	}
	else if (videoRes == VID_RES_QXGA)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_2048;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_1536;
	}
	else if (videoRes == VID_RES_960P)
	{
		nWidth = MTI_VIDEO_RESLUTION_VAL_1280;
		nHeigh = MTI_VIDEO_RESLUTION_VAL_960;
	}
}

// 取字符串最前面两个数字(小数点会作为分隔符处理)， 正(+)/负(-)符号 不可作为分隔符
void ConvertVideoResByPixel(const CVidRes &strVideoRes, int &nWidth, int &nHeigh)
{
	nWidth = nHeigh = 0;

	string strRes = strVideoRes;
	std::stringstream sstream;
	string strPixel;
	int nPixel = 0;

	while ( !strRes.empty() )
	{
		sstream.clear();
		sstream.str("");
		sstream << strRes;

		nPixel = 0;
		sstream >> nPixel;

		if ( nPixel == 0 )
		{
			strRes.assign(strRes, 1, string::npos);
			continue;
		}

		if ( nWidth == 0 )
		{
			nWidth = abs(nPixel);

			sstream.clear();
			sstream.str("");
			sstream << nPixel;
			sstream >> strPixel;

			strRes.assign(strRes, strRes.find_first_of(strPixel)+strPixel.size(), string::npos);
			continue;
		}

		nHeigh = abs(nPixel);
		break;
	}

	if ( nWidth == 0 || nHeigh == 0 )
	{
		nWidth = nHeigh = 0;
	}
}
void ConvertPuCfgToEncVidRes( bool isOldPu, const CPuChnCfgList &chnCfgList, std::vector<cusdk__EncoderVideoRes> &tTo )
{
	for ( CPuChnCfgList::const_iterator cIter = chnCfgList.begin();
		cIter != chnCfgList.end(); cIter++ )
	{
		cusdk__EncoderVideoRes encVidRes;
		encVidRes.id = cIter->first;
		const CVidRes &videoRes = cIter->second.GetChnVidFormat().GetVideoRes();

		int nWidth = 0;
		int nHeight = 0;

		if ( isOldPu )
		{
			ConvertVideoResByTitle(videoRes, nWidth, nHeight);
		}
		else
		{
			ConvertVideoResByPixel(videoRes, nWidth, nHeight);
		}

		encVidRes.width = nWidth;
		encVidRes.height = nHeight;
		
		tTo.push_back(encVidRes);
	}
}

//兼容平台多个notify对应cusdk的一个通知接口
#define ETYPE_TRANSPARENT_QUR_NOTIFY  44
#define ETYPE_DEVICE_TRANSDATA_NOTIFY 30
void ConvertStdListToVector(const std::list<int> &tFrom,std::vector<xsd__int> &tTo)
{
	std::list<int>::const_iterator pItem = tFrom.begin();
	while ( pItem != tFrom.end() )
	{
		if (ETYPE_TRANSPARENT_QUR_NOTIFY == *pItem)
		{
			tTo.push_back(ETYPE_DEVICE_TRANSDATA_NOTIFY);
		}
		else
		{
			tTo.push_back(*pItem);
		}
		pItem++;
	}
}

template<typename TList, typename TVect>
void ConvertListToVector(const TList &tFrom, TVect &tTo)
{
    typename TList::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
		tTo.push_back(*pItem);
        pItem++;
    }
}

void ConverTempTvPollFromCuiToCu(const CTvPollList &tFrom, std::vector<cusdk__TVWallWindowPollStep > &tTo)
{
    CTvPollList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TVWallWindowPollStep tTVWallWindowPollStep;
        tTVWallWindowPollStep.duration = item->GetPollTime();
        tTVWallWindowPollStep.encoderChn.channelId = item->GetEncChn().GetChnNO();
        tTVWallWindowPollStep.encoderChn.deviceId = item->GetEncChn().GetDevUri();

        tTo.push_back(tTVWallWindowPollStep);

        item++;
    }
}

void ConverTempTvWallDivFromCuiToCu(const TTvWinDivList &tFrom, std::vector<cusdk__TVWALLTVDivScheme > &tTo)
{
    TTvWinDivList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TVWALLTVDivScheme tTVWALLTVDivScheme;
        tTVWALLTVDivScheme.divId = item->GetTvWinDivId();

        ConverTempTvPollFromCuiToCu(item->GetPollList(), tTVWALLTVDivScheme.pollSteps);

        tTo.push_back(tTVWALLTVDivScheme);

        item++;
    }
}

void ConverTempTvWinFromCuiToCu(const CTvWinList &tFrom, std::vector<cusdk__TVWallTVScheme > &tTo)
{
    CTvWinList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TVWallTVScheme tTVWallTVScheme;
        if(item->GetStyle() == ETV_WIN_STYLE_1)
        {
            tTVWallTVScheme.divStyle = cusdk__TVDIV_1;
        }
        else if (item->GetStyle() == ETV_WIN_STYLE_2)
        {
            tTVWallTVScheme.divStyle = cusdk__TVDIV_2;
        }
        else if(item->GetStyle() ==  ETV_WIN_STYLE_3)
        {
            tTVWallTVScheme.divStyle = cusdk__TVDIV_3;
        }
        else if(item->GetStyle() ==  ETV_WIN_STYLE_4)
        {
            tTVWallTVScheme.divStyle = cusdk__TVDIV_4;
        }
		else if (item->GetStyle() ==  ETV_WIN_STYLE_6)
		{
			tTVWallTVScheme.divStyle = cusdk__TVDIV_6;
		}
		else if (item->GetStyle() ==  ETV_WIN_STYLE_8)
		{
			tTVWallTVScheme.divStyle = cusdk__TVDIV_8;
		}
        else if(item->GetStyle() ==  ETV_WIN_STYLE_9)
        {
            tTVWallTVScheme.divStyle = cusdk__TVDIV_9;
        }
        else if(item->GetStyle() == ETV_WIN_STYLE_16)
        {
            tTVWallTVScheme.divStyle = cusdk__TVDIV_16;
        }
		else if(item->GetStyle() == ETV_WIN_STYLE_6_EQUAL)//20170725 add by Royan Support6EqualStyle
		{
			tTVWallTVScheme.divStyle = cusdk__TVDIV_6_EQUAL;
		}

        tTVWallTVScheme.tvId = item->GetTvWinId();

        ConverTempTvWallDivFromCuiToCu(item->GetDivList(), tTVWallTVScheme.tvWallDivList);

        tTo.push_back(tTVWallTVScheme);

        item++;
    }
}

void ConverTempTvDecoderBindersCuiToCu(const vector<TvDecoderBinder>& tFrom, std::vector<cusdk__TVDecoderBind >& tTo)
{
    std::vector<TvDecoderBinder >::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TVDecoderBind tTvDecoderBinder;
        tTvDecoderBinder.decoderId = item->decoderId;
        tTvDecoderBinder.tvId = item->tvId;
        tTvDecoderBinder.decoderOutputId = item->decoderOutputId;

        tTo.push_back(tTvDecoderBinder);

        item++;
    }
}

void ConverTempTvWallTvStateFromCuiToCu(const TTvWinDivStateList& tFrom, std::vector<cusdk__TVWallTVDivState >& tTo)
{
    TTvWinDivStateList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TVWallTVDivState tTvWallTvDivState;
        tTvWallTvDivState.divId = item->GetTvDivId();
        tTvWallTvDivState.errorCode = item->GetPlayState();

        if( WIN_MEDIA_SERVICE_REAL_MEDIAL == item->GetMediaService())
        {
            tTvWallTvDivState.divPlayMode = cusdk__TVPlayModeLiveStream;
        }
        else if(WIN_MEDIA_SERVICE_PLAT_REC == item->GetMediaService())
        {
            tTvWallTvDivState.divPlayMode = cusdk__TVPlayModePlatformRecord;
        }
        else if(WIN_MEDIA_SERVICE_PU_REC == item->GetMediaService())
        {
            tTvWallTvDivState.divPlayMode = cusdk__TVPlayModePuRecord;
        }

        tTvWallTvDivState.recordPlaySession = item->GetRecToken();

        if( WIN_POLL_START == item->GetWinPollState() )
        {
            tTvWallTvDivState.pollState = cusdk__PollStarted;
        }
        else if( WIN_POLL_PAUSE == item->GetWinPollState())
        {
            tTvWallTvDivState.pollState = cusdk__PollPaused;
        }
        else if(WIN_POLL_STOP == item->GetWinPollState())
        {
            tTvWallTvDivState.pollState = cusdk__PollStoped;
        }

        tTvWallTvDivState.encoderChn.channelId = item->GetTvChnVidSrc().GetChnNO();
        tTvWallTvDivState.encoderChn.deviceId = item->GetTvChnVidSrc().GetDevUri();

        CCMSTime startTime(item->GetTimeRange().GetStartTime());
        CCMSTime endTime(item->GetTimeRange().GetEndTime());
        tTvWallTvDivState.recordPlayDuration.durationTimeRange.startTime = startTime.GetTime();
        tTvWallTvDivState.recordPlayDuration.durationTimeRange.endTime = endTime.GetTime();

        //todo 当前不实现，存在问题，录像上墙去掉
        tTvWallTvDivState.recordPlayDuration.encoderChn.channelId = item->GetEncChn().GetChnNO();
        tTvWallTvDivState.recordPlayDuration.encoderChn.deviceId = item->GetEncChn().GetDevUri();
        //         //tTvWallTvDivState.recordPlayDuration.recordDomainName = item->get
        //         tTvWallTvDivState.recordPlayDuration.recordDurationId = item->GetRecToken();

        tTo.push_back(tTvWallTvDivState);
        item++;
    }
}

void ConverTempTvWinStateFromCuiToCu(const CTvWinStateList& tFrom, std::vector<cusdk__TVWallTVState >& tTo)
{
    CTvWinStateList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TVWallTVState tTvWallTvState;
        tTvWallTvState.tvId = item->GetTvId();
        ConverTempTvWallTvStateFromCuiToCu(item->GetDivStateList(), tTvWallTvState.divStateList);

        tTo.push_back(tTvWallTvState);

        item++;
    }
}

void ConverTempTvWallSchemePollCuiToCu(const CTTvWallSchemePollList &tFrom, std::vector<cusdk__TVWallSchemePollStep > &tTo)
{
    CTTvWallSchemePollList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TVWallSchemePollStep tTVWallSchemePollStep;
        tTVWallSchemePollStep.duration = item->GetPollTime();
        tTVWallSchemePollStep.tvWallSchemeName = item->GetSchemeName();

        tTo.push_back(tTVWallSchemePollStep);

        item++;
    }
}

void ConvertDeviceCapIndexsFromCuiToCu(const vector<TDeviceCapIndexs> &tFrom, vector<int > &tToVideoSrcIdList)
{
    vector<TDeviceCapIndexs>::const_iterator pDevCapIndexItem = tFrom.begin();
    while ( pDevCapIndexItem != tFrom.end() )
    {
        CMS_ASSERT( 0 != pDevCapIndexItem->id);
        switch (pDevCapIndexItem->id)
        {
        case DEVICE_CAP_VIDEOSOURCE:
            {
                tToVideoSrcIdList = pDevCapIndexItem->indexs;
            }
            break;
        case DEVICE_CAP_LOCAL_STORAGE:
            break;
        case DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT:
            //{
                //tToAlarmInputChnList = pDevCapIndexItem->indexs;
            //}
            break;
        case DEVICE_CAP_ENCODE_CHANNEL:
            break;
        case DEVICE_CAP_DECODE_CHANNEL:
            break;
        default:
            {
                CMS_ASSERT(0 && "非法的能力类型标识");
            }
            break;
        }

        pDevCapIndexItem++;
    }
}

//todo 由于黄志春没有给出转换方案，暂时不做
void ConverTempCronTimeCuiToCu(const TCronTimeList &tFrom, std::vector<string > &tTo)
{
    TCronTimeList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        tTo.push_back(*item);

        item++;
    }
}

void ConverTempLinkActionCuiToCu(const CLinkActionList &tFrom, std::vector<enum cusdk__EAlarmLinkActionType > &tTo)
{
    CLinkActionList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        if(*item == ALARMLINK_PLAT_REC)
        {
            tTo.push_back(cusdk__AlarmLinkActionPlatformRecord);
        }
        else if(*item == ALARMLINK_TV_WALL)
        {
            tTo.push_back(cusdk__AlarmLinkActionPlayOnTV);
        }
        else if(*item == ALARMLINK_READ_SEC)
        {
            tTo.push_back(cusdk__AlarmLinkActionReadSecond);
        }
        else if(*item == ALARMLINK_IO_OUT)
        {
            tTo.push_back(cusdk__AlarmLinkActionIOOutput);
        }
        else if(*item == ALARMLINK_CAM)
        {
            tTo.push_back(cusdk__AlarmLinkCameraPrePos);
        }

        item++;
    }
};

void ConverTempAlarmLinkDstCuiToCu(const CAlarmLinkDstList &tFrom, std::vector<cusdk__AlarmLinkAction >  &tTo)
{
    CAlarmLinkDstList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__AlarmLinkAction tAlarmLinkAction;
        tAlarmLinkAction.encoderChn.deviceId = item->GetEncChn().GetDevUri();
        tAlarmLinkAction.encoderChn.channelId = item->GetEncChn().GetChnNO();
        tAlarmLinkAction.camPrePos = item->GetCamPrePos();
        tAlarmLinkAction.tvWallId = item->GetTvWallId();
        tAlarmLinkAction.tvId = item->GetTvId();
        tAlarmLinkAction.tvDivId = item->GetTvDivId();
        //         tAlarmLinkAction.SMSPhoneNumList = item->GetMsgNoList();
        //         tAlarmLinkAction.voicePhoneNumList = item->GetVocNoList();
        //         tAlarmLinkAction.mailList = item->GetMailNoList();
        tAlarmLinkAction.alarmIOOutputChnId = item->GetAlarmOutChn();
        ConverTempLinkActionCuiToCu(item->GetLinkActionList(), tAlarmLinkAction.actionType);

        tTo.push_back(tAlarmLinkAction);

        item++;
    }
}

void ConvertUserGroupInfoFromCuiToCu(const CUserGroupInfo &tFrom, cusdk__UserGroup &tTo)
{
    tTo.id = tFrom.GetGroupNO();
    //tTo.parentId = tFrom.GetParentGroupNO();
    tTo.name = tFrom.GetGroupName();
    tTo.description = tFrom.GetDescription();
    tTo.creator = tFrom.GetCreateUser();
    tTo.userType = static_cast<enum cusdk__EUserType>(tFrom.GetUserType());
    MaxStreamCuiToCu( tFrom, tTo );

    //将set<int> 强转为 set<enum cusdk__EUserPri >
    ConvertUserPriFromCuiToCu(tFrom.GetUserPrivilege(), tTo.userPri);

    WaterMarkCuiToCu( tFrom, tTo );
}

void ConvertCameraTypeFromCuiToCu(const map<int,int>& tFrom, std::vector<cusdk__VideoSrcCamType>& tTo)
{
    map<int,int>::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
        std::vector<cusdk__VideoSrcCamType>::value_type tItem;
        tItem.id = pItem->first;
        tItem.camType = (cusdk__ECameraProtocolType)pItem->second;
        tTo.push_back(tItem);
        pItem++;
    }
}

void ConvertDeviceBaseInfoFromCuiToCu(const CDeviceInfo &tFrom, cusdk__DeviceInfo &tTo)
{
	tTo.deviceId = tFrom.GetDeviceUUID();
	tTo.domainId = tFrom.GetDomainUUID();
	tTo.name = tFrom.GetDeviceName();
	tTo.deviceAlias = tFrom.GetDeviceAlias();
	tTo.deviceType = static_cast<enum cusdk__EDeviceType>(tFrom.GetDeviceType());
	tTo.manufacturer = tFrom.GetManufacturer();
	tTo.creatorType = (int)tFrom.GetCreater();
	tTo.description = tFrom.GetDescription();
	tTo.deviceModel = tFrom.GetModelName();
	tTo.deviceIP = tFrom.GetNetAddress();
	if ( !tFrom.GetExpireDate().empty() )
	{
		tTo.deviceExtInfo = tFrom.GetDevData().ToXml();
	}

	vector<TDeviceCapInfo>::const_iterator pDevCapInfoItem = tFrom.GetDeviceModelCapDetail().begin();
	while (pDevCapInfoItem != tFrom.GetDeviceModelCapDetail().end())
	{
		if (0 != pDevCapInfoItem->num)
		{
			switch (pDevCapInfoItem->type)
			{
			case DEVICE_CAP_VIDEOSOURCE:
				{
					tTo.videoSrcNum = pDevCapInfoItem->num;
				}
				break;
			case DEVICE_CAP_LOCAL_STORAGE:
				{
					tTo.deviceCapLabelList.push_back(cusdk__DEVICE_CAP_LOCAL_STORAGE);
				}
				break;
			case DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT:
				{
					tTo.alarmInputNum = pDevCapInfoItem->num;
				}
				break;
			case DEVICE_CAP_ENCODE_CHANNEL:
				{
					tTo.encodeChnNum = pDevCapInfoItem->num;
				}
				break;
			case DEVICE_CAP_DECODE_CHANNEL:
				{
					tTo.decodeChnNum = pDevCapInfoItem->num;
				}
				break;
			case DEVICE_CAP_DECODE_OUTPUTCHANNEL:
				{
					tTo.videoOutputChnNum = pDevCapInfoItem->num;
				}
				break;
			case DEVICE_CAP_LABEL_ENCODE_NVR:
				{
					tTo.deviceCapLabelList.push_back(cusdk__DEVICE_CAP_LABEL_ENCODE_NVR);
				}
				break;
			case DEVICE_CAP_LABEL_ENCODE_SVR:
				{
					tTo.deviceCapLabelList.push_back(cusdk__DEVICE_CAP_LABEL_ENCODE_SVR);
				}
				break;
			case DEVICE_CAP_LABEL_ENCODE_IPC_BULLET:
				{
					tTo.deviceCapLabelList.push_back(cusdk__DEVICE_CAP_LABEL_ENCODE_IPC_BULLET);
				}
				break;
			case DEVICE_CAP_LABEL_ENCODE_IPC_DOME:
				{
					tTo.deviceCapLabelList.push_back(cusdk__DEVICE_CAP_LABEL_ENCODE_IPC_DOME);
				}
				break;
			case DEVICE_CAP_LABEL_ENCODE_GENERAL:
				{
					tTo.deviceCapLabelList.push_back(cusdk__DEVICE_CAP_LABEL_ENCODE_GENERAL);
				}
				break;
			case DEVICE_CAP_LABEL_ENCODE_WIRELESS:
				{
					tTo.deviceCapLabelList.push_back(cusdk__DEVICE_CAP_LABEL_ENCODE_WIRELESS);
				}
				break;
			case DEVICE_CAP_LABEL_GBDEVICE:
				{
					tTo.deviceCapLabelList.push_back(cusdk__DEVICE_CAP_LABEL_GBDEVICE);
				}
				break;
			case DEVICE_CAP_LABEL_GBRAWSTREAM:
				{
					tTo.deviceCapLabelList.push_back(cusdk__DEVICE_CAP_LABEL_GBRAWSTREAM);
				}
				break;
			default:
				break;
			}
		}
		pDevCapInfoItem++;
	}
}

void ConvertDevConfigFromCuiToCu(const TPuConfig &tPuConfig, cusdk__DevConfigNotify &cCusdkDevConfig)
{
	//devURI
	cCusdkDevConfig.deviceId = tPuConfig.GetDevUri();
	cCusdkDevConfig.devIp = tPuConfig.GetDevIp();
	cCusdkDevConfig.devType = tPuConfig.GetDevType();
	cCusdkDevConfig.devMode = tPuConfig.GetDevMode();

	//baseCapSet
	cCusdkDevConfig.baseCapSet.videoEncNum = tPuConfig.GetBaseCapSet().GetVidEncNum();
	cCusdkDevConfig.baseCapSet.videoDecNum = tPuConfig.GetBaseCapSet().GetVidDecNum();
	cCusdkDevConfig.baseCapSet.audioEncNum = tPuConfig.GetBaseCapSet().GetAudEncNum();
	cCusdkDevConfig.baseCapSet.audioDecNum = tPuConfig.GetBaseCapSet().GetAudDecNum();
	cCusdkDevConfig.baseCapSet.serialPortNum = tPuConfig.GetBaseCapSet().GetSerialPortNum();
	cCusdkDevConfig.baseCapSet.alarmInputChnNum = tPuConfig.GetBaseCapSet().GetAlarmInNum();
	cCusdkDevConfig.baseCapSet.alarmOutputChnNum = tPuConfig.GetBaseCapSet().GetAlarmOutNum();
	cCusdkDevConfig.baseCapSet.videoInPortNum = tPuConfig.GetBaseCapSet().GetVideoInPortNum();

	//extCapSet
	ConvertMapToVector(tPuConfig.GetExtCapSet(), cCusdkDevConfig.extCapSet);

	//inputPinEnable
	if (!tPuConfig.GetInputPinEnable().empty())
	{
		for (u32 nIndex = 0; nIndex < tPuConfig.GetInputPinEnable().size(); nIndex++)
		{
			cusdk__AlarmInputChnEnable tItem;
			tItem.id = nIndex;
			tItem.value = tPuConfig.GetInputPinEnable().at(nIndex) == '0' ? false : true;
			cCusdkDevConfig.alarmInputChnEnable.push_back(tItem);
		}
	}
	//ConvertMapToVector(tPuConfig.GetInputPinEnable(), cCusdkDevConfig.inputPinEnable);

	//chnVidFormat
	ConvertMapToVector(tPuConfig.GetCurrVidFormat(), cCusdkDevConfig.chnVidFormat);

	CPuChnCfgList chnCfgList;
	for (int nVidEndIdx = 0; nVidEndIdx < tPuConfig.GetBaseCapSet().GetVidEncNum(); nVidEndIdx++)
	{
		if (tPuConfig.GetVideoEncodeParam().find(nVidEndIdx) != tPuConfig.GetVideoEncodeParam().end())
		{
			chnCfgList[nVidEndIdx] = tPuConfig.GetChnConfig(nVidEndIdx);
		}
	}

	//chnVidRes
	ConvertPuCfgToEncVidRes((tPuConfig.GetExtCapSet().find(VSIP_ENTITY_SUPPORT_RESOLUTION_WH) == tPuConfig.GetExtCapSet().end())
		, chnCfgList, cCusdkDevConfig.chnVideoRes);
	
	//chnVidDynValue
	ConvertMapToVector(tPuConfig.GetVidDynValue(), cCusdkDevConfig.chnVidDynValue);

	//vidSrcChnSet
	ConvertMapToVector(tPuConfig.GetVidSrcChnSet(), cCusdkDevConfig.vidSrcChnSet);

	//chnVidSrcSet
	ConvertMapToVector(tPuConfig.GetChnVidSrcSet(), cCusdkDevConfig.chnVidSrcSet);

	//camType
	ConvertCameraTypeFromCuiToCu(tPuConfig.GetCamType(), cCusdkDevConfig.camType);


	//camId
	ConvertMapToVector(tPuConfig.GetCamId(), cCusdkDevConfig.camId);

	//bitRate
	ConvertMapToVector(tPuConfig.GetBitRate(), cCusdkDevConfig.bitRate);

	cusdk__EncoderChnStatus tChnStatus;
	for (map<TChannelID, TPuChnStateConfig>::const_iterator it = tPuConfig.GetChnStateSet().begin();
	it != tPuConfig.GetChnStateSet().end(); ++it)
	{
		tChnStatus.id = it->first;
		tChnStatus.status = it->second.GetChnState();
		cCusdkDevConfig.chnStatus.push_back(tChnStatus);
	}

	//freeLicenceNum
	cCusdkDevConfig.freeLicenceNum = tPuConfig.GetFreeLicenceNum();
	cCusdkDevConfig.directTransNum = tPuConfig.GetDirectTransNum();
	cCusdkDevConfig.isDecCombined = tPuConfig.GetIsDecCombined();
}

void ConvertServiceStatusFromCuiToCu(const string &strDevId, const TPuServiceStatus &tPuServiceStatus, cusdk__DevServiceStatusNotify &cCusdkServiceStatus)
{
    cCusdkServiceStatus.deviceId = strDevId;

    // 音频呼叫状态转换
    ConvertMapToVector(tPuServiceStatus.GetAudioCallSrc(), cCusdkServiceStatus.audioCallSrc);

    // 平台预录状态转换
    ConvertMapToVector(tPuServiceStatus.GetIsPlatPreRec(), cCusdkServiceStatus.isPlatPreRec);

    // 平台录像状态转换
    ConvertPlatRecMapToVector(tPuServiceStatus.GetPlatRec(), cCusdkServiceStatus.platRecStatus);

    // 前端预录状态转换
    ConvertMapToVector(tPuServiceStatus.GetIsPuPreRec(), cCusdkServiceStatus.isPuPreRec);

    // 前端录像状态转换
    ConvertMapToVector(tPuServiceStatus.GetIsPuRec(), cCusdkServiceStatus.isPuRec);
}

void ConvertAlarmStatusFromCuiToCu(const TPuAlarmStatus &tPuAlarmStatus, vector<cusdk__DevAlarmNotify> &cCusdkAlarmStatus)
{
    std::vector< TAlarmInput >::const_iterator pVtem = tPuAlarmStatus.GetAlarmArray().begin();

    while (pVtem != tPuAlarmStatus.GetAlarmArray().end())
    {
        cusdk__DevAlarmNotify cCusdkAlarmNotify;
        //devURI
        cCusdkAlarmNotify.deviceId = pVtem->GetDevUri();

        //by zzc
        //通过双向映射表 查询 cu cms 对应的alarmtype
        CMS_ALARM_TYPE cmsType = (*pVtem).GetAlarmType();
        CU_ALARM_TYPE  cuType  = cusdk__AlarmTypeInvalid;
        const CAlarmTypeCommon *pcAlmTypeUtil = GetCommonType();
        if( pcAlmTypeUtil != NULL )
        {
            cuType = pcAlmTypeUtil->ConvertFromCmsToCu( cmsType );
            if( !pcAlmTypeUtil->IsInCuV2Only( cuType ) )
            {
                cCusdkAlarmNotify.alarmType = cusdk__EAlarmType(cuType);
            }
            cCusdkAlarmNotify.alarmTypeV2 = cuType;
        }

        cCusdkAlarmNotify.alarmChnId = (*pVtem).GetInputId();

        if ((*pVtem).GetAlarmFlag() == ALARM_FLAG_OCCUR)
        {
            cCusdkAlarmNotify.alarmStatus = cusdk__AlarmOccur;
        }
        else if ((*pVtem).GetAlarmFlag() == ALARM_FLAG_RESTORE)
        {
            cCusdkAlarmNotify.alarmStatus = cusdk__AlarmRestore;
        }

        cCusdkAlarmNotify.detAreaId = (*pVtem).GetDetAreaId();

        string strTime = (*pVtem).GetAlarmTime();
        if (!strTime.empty())
        {
            CCMSTime time(strTime);
            cCusdkAlarmNotify.time = time.GetTime();
        }

        cCusdkAlarmNotify.alarmData = (*pVtem).GetAlarmData();
        cCusdkAlarmNotify.alarmName = (*pVtem).GetAlarmAlias();

        cCusdkAlarmStatus.push_back(cCusdkAlarmNotify);

        pVtem++;
    }
}

void ConvertUserPriFromCuToCui( const set<enum cusdk__EUserPri > &tFrom, CUserPrivilege& tTo)
{
    set<enum cusdk__EUserPri >::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
        int nPri = static_cast<int>(*pItem);
        if ( nPri > 0 )
        {
            tTo.SetPrivilege(static_cast<CUserPrivilege::TPrivilege>(nPri));
        }
        pItem++;
    }
}



void ConvertUserInfoFromCuToCui( const cusdk__UserInfo &tFrom, CCuUserInfo &tTo)
{
    tTo.SetUserID(tFrom.uuid);
    tTo.SetUserName(tFrom.userName);
    tTo.SetUserPwd(tFrom.userPassword);
    tTo.SetDomainID(tFrom.domainID);
    tTo.SetUserType(static_cast<int>(tFrom.userType));

    //将 set<enum cusdk__EUserPri >强转为 set<int>
    CUserPrivilege cPri;
    ConvertUserPriFromCuToCui(tFrom.userPri, cPri);

    tTo.SetUserPrivilege(cPri);
    tTo.SetCreatorID(tFrom.creator);
    tTo.SetUsergrpID(tFrom.userGroup);
    tTo.SetDescription(tFrom.description);
    CCMSTime cNameExpireData(tFrom.accountOverdueTime);
    tTo.SetNameExpireDate(cNameExpireData.GetXSDDateTime());
    CCMSTime cPwdExpireData(tFrom.passwordOverdueTime);
    tTo.SetPwdExpireDate(cPwdExpireData.GetXSDDateTime());
    tTo.SetEnabledFlag(tFrom.enable);
    tTo.SetMLoginFlag(tFrom.allowMultilogin);
	tTo.SetPwdExpireEnabledFlag(tFrom.passwordExpireEnable);
	tTo.SetPwdExpireTime(tFrom.passwordExpire);
	tTo.SetLoginIpList(tFrom.bindingAddress);
	tTo.SetIpListEnabledFlag(tFrom.bindIPEnable);
	tTo.SetIpBindFlagForMobileClient(tFrom.IsIPAppliedToMobileEnd);
    MaxStreamCuToCui( tFrom, tTo );

    WaterMarkCuToCui( tFrom, tTo );

}


cusdk__EGBDeviceGroupType ConvertDevGrpTypeFromCUIToCu( TDevGroupType cuiType )
{
    switch( cuiType )
    {
    case TDEV_GROUP_TYPE_CIVILCODE:     //行政区划子分组
        return cusdk__GBDeviceGroupTypeCivilcode;
        break;
    case TDEV_GROUP_TYPE_VIRTUALGRP:     //虚拟组织子分组:
        return cusdk__GBDeviceGroupTypeVitual;
        break;
    case TDEV_GROUP_TYPE_UNKNOWN://未知类型，兼容老版本cu，后台统一按照行政区划子分组处理
    default:
        return cusdk__GBDeviceGroupTypeInvalid;          
        break;
    }
}


TDevGroupType ConvertDevGrpTypeFromCuToCUI( cusdk__EGBDeviceGroupType cuType )
{
    switch( cuType )
    {
    case cusdk__GBDeviceGroupTypeCivilcode:     //行政区划子分组
        return TDEV_GROUP_TYPE_CIVILCODE;
        break;
    case cusdk__GBDeviceGroupTypeVitual:        //虚拟组织子分组:
        return TDEV_GROUP_TYPE_VIRTUALGRP;
        break;
    case cusdk__GBDeviceGroupTypeInvalid:       //未知类型，兼容老版本cu，后台统一按照行政区划子分组处理
    default:
        return TDEV_GROUP_TYPE_UNKNOWN;          
        break;
    }
}

void ConvertDevGrpInfoBaseFromCuiToCu( const CDeviceGroupInfo *ptFrom, cusdk__DeviceGroupInfo *ptTo )
{
    if(     ptFrom == NULL 
        ||  ptTo == NULL )
    {
        return;
    }
    ptTo->id          = ptFrom->GetDeviceGroupID();
    ptTo->parentId    = ptFrom->GetDeviceParentGroupID();
    ptTo->domainId    = ptFrom->GetDomainUUID();
    ptTo->name        = ptFrom->GetDeviceGroupName();
    ptTo->description = ptFrom->GetDeviceGroupDesc();
    ptTo->fOrderNumber= ptFrom->GetGrpseq();
    ptTo->devTreeId   = ptFrom->GetDevtreeId();
    ptTo->originalGBId= ptFrom->GetOldgbid();
    ptTo->newGBID     = ptFrom->GetNewgbid();
    ptTo->GBDeviceGroupType = ConvertDevGrpTypeFromCUIToCu( ptFrom->GetGrpType() );
	ptTo->CreatorType = (cusdk__EDeviceCreatorType)ptFrom->GetCreater();
}

void ConvertDevGrpInfoBaseFromCuToCui( const cusdk__DeviceGroupInfo *ptFrom, CDeviceGroupInfo *ptTo )
{
    if(     ptFrom == NULL 
        ||  ptTo == NULL )
    {
        return;
    }
    ptTo->SetDeviceGroupID  ( ptFrom->id        );
    ptTo->SetDeviceParentGroupID(ptFrom->parentId );
    ptTo->SetDomainUUID     ( ptFrom->domainId  );
    ptTo->SetDeviceGroupName( ptFrom->name      );
    ptTo->SetDeviceGroupDesc( ptFrom->description );
    ptTo->SetGrpseq (   ptFrom->fOrderNumber    );
    ptTo->SetDevtreeId( ptFrom->devTreeId       );
    ptTo->SetOldgbid(   ptFrom->originalGBId    );
    ptTo->SetNewgbid(   ptFrom->newGBID         );

    TDevGroupType type = ConvertDevGrpTypeFromCuToCUI( (cusdk__EGBDeviceGroupType)ptFrom->GBDeviceGroupType );
    ptTo->SetGrpType( type );
	ptTo->SetCreater((TCreater)ptFrom->CreatorType);
}


void ConvertUserDevGrpInfoExFromCuiToCu( const CCuUserDeviceGroupInfo *ptFrom, cusdk__DeviceGroup *ptTo )
{
    if(     ptFrom == NULL 
        ||  ptTo == NULL )
    {
        return;
    }
    ConvertDevGrpInfoBaseFromCuiToCu( ptFrom, ptTo );

    ptTo->hasDevice     = ptFrom->IsGroupHasDev();
    ptTo->recursiveEmpty= ptFrom->IsEmptyDevGroup();
}

void ConvertUserDeviceGroupInfoFromCUIToCu(const vector<CCuUserDeviceGroupInfo> &tFrom, vector<cusdk__DeviceGroup > &tTo)
{
    vector<CCuUserDeviceGroupInfo>::const_iterator pItem = tFrom.begin();
    while ( pItem != tFrom.end() )
    {
        cusdk__DeviceGroup cCuDevGroup;
        ConvertUserDevGrpInfoExFromCuiToCu( &(*pItem), &cCuDevGroup );
        
        cusdk__EGBDeviceGroupType cuDevGrpType = ConvertDevGrpTypeFromCUIToCu( pItem->GetGrpType() );
        cCuDevGroup.GBDeviceGroupType = cuDevGrpType;

        tTo.push_back(cCuDevGroup);
        pItem++;
    }
}


void ConvertDevicePrivilegeFromCuiToCu(const CDevicePrivilege &tFrom, set<  int > &tTo)
{
    for ( int nPri=1;nPri<32;nPri++ )
    {
        if ( tFrom.IsBitOn(nPri - 1) )
        {
            if(nPri <= cusdk__PriPUAlarmConfig)
            {
                tTo.insert(nPri);
            }        
        }
    }
};

void ConvertDevicePrivilegeFromCuiToCu(const CDevicePrivilege &tFrom, set<  enum cusdk__EDevicePri > &tTo)
{
	for ( int nPri=1;nPri<32;nPri++ )
	{
		if ( tFrom.IsBitOn(nPri - 1) )
		{
			if(nPri <= cusdk__PriPUAlarmConfig)
			{
				tTo.insert(static_cast<cusdk__EDevicePri>(nPri));
			}        
		}
	}
};

void ConvertDevicePrivilegeFromCuToCui(const set<  int > &tFrom, CDevicePrivilege &tTo)
{
	set< int >::const_iterator pPriItem = tFrom.begin();
	while ( pPriItem != tFrom.end() )
	{
		if ( static_cast<u32>(*pPriItem) > 0 )
		{
			tTo.SetBit(static_cast<u32>(*pPriItem) - 1, true);
		}

		pPriItem++;
	}
};

void ConvertVideosourcePrivilegeFromCuToCui(const set<int> &tFrom, CVideosourcePrivilege &tTo)
{
	set<int>::const_iterator pItem = tFrom.begin();
	if (tFrom.find(cusdk__PriPuAudioCtrl) != tFrom.end())
	{
		tTo.SetPrivilege(CVideosourcePrivilege::en_AUDIO_PLAY);
	}
	if (tFrom.find(cusdk__PriPuSavePreset) != tFrom.end())
	{
		tTo.SetPrivilege(CVideosourcePrivilege::en_PrePos_Set);
	}
	if (tFrom.find(en_CU_PriPuPtzLock) != tFrom.end())
	{
		tTo.SetPrivilege(CVideosourcePrivilege::en_PTZ_Lock);
	}

	return;
}

void ConvertDevicefullinfoFromCuiToCu(const vector<TDevicefullinfoAndPrivilege> &tFrom, vector<cusdk__DeviceInfo > &tTo)
{
    vector<TDevicefullinfoAndPrivilege>::const_iterator pDevFullInfoItem = tFrom.begin();
    while (pDevFullInfoItem != tFrom.end())
    {
        cusdk__DeviceInfo cDevFullInfo;
        ConvertDeviceBaseInfoFromCuiToCu(pDevFullInfoItem->deviceInfo, cDevFullInfo);

		ConvertDevicePrivilegeFromCuiToCu(pDevFullInfoItem->devicePrivilege, cDevFullInfo.priList);
		ConvertDevicePrivilegeFromCuiToCu(pDevFullInfoItem->devicePrivilege, cDevFullInfo.priListV2);


        for( size_t i=0; i!=pDevFullInfoItem->vidSrcIndex.size(); ++i )
        {
            int nVidSrcIdx = pDevFullInfoItem->vidSrcIndex[i];

            map<int, TUserVideoSourceCapInfo>::const_iterator pVidSrcItem = pDevFullInfoItem->videosourceinfos.find( nVidSrcIdx );
            if( pVidSrcItem == pDevFullInfoItem->videosourceinfos.end() )
            {
                continue;
            }

			TDeviceCapExtData tDevExtData;
			if (!pVidSrcItem->second.extData.empty()) tDevExtData.FromXml(pVidSrcItem->second.extData);

            cusdk__VideoSrc cCuVideSrc;
            cCuVideSrc.id = pVidSrcItem->first;
            cCuVideSrc.name = pVidSrcItem->second.name;
		    tDevExtData.GetLat(cCuVideSrc.latitude);
			tDevExtData.GetLon(cCuVideSrc.longitude);
            cCuVideSrc.ptzLevel = GetPtzPriLevelFromSet(pVidSrcItem->second.privilege);
            if (pVidSrcItem->second.privilege.HasPrivilege(CVideosourcePrivilege::en_AUDIO_PLAY))
            {
                cCuVideSrc.priList.insert(cusdk__PriPuAudioCtrl);
				cCuVideSrc.priListV2.insert(cusdk__PriPuAudioCtrl);
            }
			if (pVidSrcItem->second.privilege.HasPrivilege(CVideosourcePrivilege::en_PrePos_Set))
			{
				//cCuVideSrc.priList.insert(cusdk__PriPuSavePreset);
				cCuVideSrc.priListV2.insert(cusdk__PriPuSavePreset);
			}
			if (pVidSrcItem->second.privilege.HasPrivilege(CVideosourcePrivilege::en_PTZ_Lock))
			{
				cCuVideSrc.priListV2.insert(en_CU_PriPuPtzLock);
			}
            cDevFullInfo.videoSrcList.push_back(cCuVideSrc);
            pVidSrcItem++;
        }

        tTo.push_back(cDevFullInfo);
        pDevFullInfoItem ++;
    }
}

void ConvertDeviceCapIndexsFromCuToCui(const vector<int > &tFromVideoSrcIdList, vector<TDeviceCapIndexs> &tTo )
{
    if ( ! tFromVideoSrcIdList.empty() )
    {
        TDeviceCapIndexs cDevCap;
        cDevCap.id = DEVICE_CAP_VIDEOSOURCE;
        cDevCap.indexs = tFromVideoSrcIdList;
        tTo.push_back(cDevCap);
    }
//     if ( ! tFromAlarmInputChnList.empty() )
//     {
//         TDeviceCapIndexs cDevCap;
//         cDevCap.id = DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT;
//         cDevCap.indexs = tFromAlarmInputChnList;
//         tTo.push_back(cDevCap);
//     }
}

void ConvertDevGroupDevInfoFromCuiToCu(const vector<CDeviceGroupDeviceInfo> &tFrom, vector< cusdk__DeviceGroupRelation > &tTo)
{
    vector<CDeviceGroupDeviceInfo>::const_iterator pDevGrpDevInfoItem = tFrom.begin();
    while ( pDevGrpDevInfoItem != tFrom.end() )
    {
        cusdk__DeviceGroupRelation cDeviceGroupRelation;
        cDeviceGroupRelation.devGroupId = pDevGrpDevInfoItem->GetDeviceGroupID();
        cDeviceGroupRelation.devId = pDevGrpDevInfoItem->GetDeviceID();

       // ConvertDeviceCapIndexsFromCuiToCu(pDevGrpDevInfoItem->GetDeviceCapIndexs(), cDeviceGroupRelation.videoSrcIdList, cDeviceGroupRelation.alarmInputChnList);
        ConvertDeviceCapIndexsFromCuiToCu(pDevGrpDevInfoItem->GetDeviceCapIndexs(), cDeviceGroupRelation.videoSrcIdList);
        tTo.push_back(cDeviceGroupRelation);
        pDevGrpDevInfoItem++;
    }
}

void ConvertAlarmTypesInfoFromCuiToCu(const vector<TAlarmType> &tFrom, vector< cusdk__AlarmType > &tTo)
{
	vector<TAlarmType>::const_iterator pAlarmType = tFrom.begin();
	while( pAlarmType != tFrom.end() )
	{
		cusdk__AlarmType cAlarmType;
		//cAlarmType.alarmType = pAlarmInput->GetAlarmType();
        CMS_ALARM_TYPE cmsType = pAlarmType->GetAlarmType();

        const CAlarmTypeCommon *pcAlmUtil = GetCommonType();
        if( pcAlmUtil != NULL )
        {
            cAlarmType.alarmType = pcAlmUtil->ConvertFromCmsToCu( cmsType );
            cAlarmType.alarmTypeName = pAlarmType->GetAlarmTypeName();
        }

		tTo.push_back(cAlarmType);
		pAlarmType++;
	}
}

void ConvertDevGroupGBDevVsInfoFromCuiToCu(const TDeviceInfo4Gb &tFrom, cusdk__CVideoSrcInfoGB &tTo)
{
	tTo.videoSrcId = tFrom.GetVidIndex();
	tTo.videoSrcName = tFrom.GetVidSrcName();
	tTo.primaryOriginalGBId = tFrom.GetGbid().GetOldprimarygbid();
	tTo.secondOriginalGBId = tFrom.GetGbid().GetOldsecondarygbid();
	tTo.primaryNewGBId = tFrom.GetGbid().GetNewprimarygbid();
	tTo.secondNewGBId = tFrom.GetGbid().GetNewsecondarygbid();
}

void ConvertDevGroupGBDevInfoFromCuiToCu(const CDeviceGroupDeviceInfo4Gb &tFrom, cusdk__CDeviceInfoGB &tTo)
{
	tTo.deviceTreeViewId = tFrom.GetDevDevTreeId();
	tTo.deviceGroupId = tFrom.GetDevGrpId();
	tTo.deviceId = tFrom.GetDeviceId();
	tTo.deviceName = tFrom.GetDevName();

	for (size_t nIndex = 0; nIndex < tFrom.GetDevinfo().size(); nIndex++)
	{
		cusdk__CVideoSrcInfoGB vsGbInfo;
		ConvertDevGroupGBDevVsInfoFromCuiToCu(tFrom.GetDevinfo()[nIndex], vsGbInfo);
		vsGbInfo.deviceId = tTo.deviceId;
		vsGbInfo.deviceName = tTo.deviceName;
		tTo.videoSrcList.push_back(vsGbInfo);
	}
}

void ConvertDevGroupDevInfoFromCUToCUI(const vector< cusdk__DeviceGroupRelation > &tFrom, vector<CDeviceGroupDeviceInfo> &tTo)
{
    vector< cusdk__DeviceGroupRelation >::const_iterator pDevGrpRelItem = tFrom.begin();
    while ( pDevGrpRelItem != tFrom.end() )
    {
        CDeviceGroupDeviceInfo cDevGrpDevInfo;
        cDevGrpDevInfo.SetDeviceGroupID(pDevGrpRelItem->devGroupId);
        cDevGrpDevInfo.SetDeviceID(GetNameWithoutDomainFromURI(pDevGrpRelItem->devId));

        ConvertDeviceCapIndexsFromCuToCui(pDevGrpRelItem->videoSrcIdList, cDevGrpDevInfo.GetDeviceCapIndexs());
        tTo.push_back(cDevGrpDevInfo);

        pDevGrpRelItem++;
    }
}


void ConvertDeviceCapPrivilegeFromCuiToCu(const vector<TDeviceCapPrivilege>& tFrom, cusdk__AssignDeviceInfo &tTo)
{
    vector<TDeviceCapPrivilege>::const_iterator pDevCapItem = tFrom.begin();
    while ( pDevCapItem != tFrom.end() )
    {
        switch (pDevCapItem->id)
        {
        case DEVICE_CAP_VIDEOSOURCE:
            {
                map<int, CBitsContainer>::const_iterator pVideoSrcCapItem = pDevCapItem->privileges.begin();
                while ( pVideoSrcCapItem != pDevCapItem->privileges.end() )
                {
                    cusdk__AssignVideoSrc tAssignVideoSrc;
                    tAssignVideoSrc.vieoSrcId = pVideoSrcCapItem->first;
                    tAssignVideoSrc.ptzLevel = GetPtzPriLevelFromSet(pVideoSrcCapItem->second); 
                    if (pVideoSrcCapItem->second.IsBitOn(CVideosourcePrivilege::en_AUDIO_PLAY-1))
                    {
                        tAssignVideoSrc.devPriSet.insert(cusdk__PriPuAudioCtrl);
                    }
					if (pVideoSrcCapItem->second.IsBitOn(CVideosourcePrivilege::en_PrePos_Set-1))
					{
						tAssignVideoSrc.devPriSet.insert(cusdk__PriPuSavePreset);
					}
					if (pVideoSrcCapItem->second.IsBitOn(CVideosourcePrivilege::en_PTZ_Lock - 1))
					{
						tAssignVideoSrc.devPriSet.insert(en_CU_PriPuPtzLock);
					}
                    tTo.videoSrcIdList.push_back(tAssignVideoSrc);
                    pVideoSrcCapItem++;
                }
            }
            break;
        case DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT:
//             {
//                 map<int, CBitsContainer>::const_iterator pAlarmInputCapItem = pDevCapItem->privileges.begin();
//                 while ( pAlarmInputCapItem != pDevCapItem->privileges.end() )
//                 {
//                     cusdk__AssignAlarminputChn tAssignAlarmInputChn;
//                     tAssignAlarmInputChn = pAlarmInputCapItem->first;
//                     tTo.alarmInputChnList.push_back(tAssignAlarmInputChn);
//                     pAlarmInputCapItem++;
//                 }
//             }
            break;
        default:
            break;
        }
        pDevCapItem++;
    }
}

void ConvertDeviceCapPrivilegeFromCuToCui(const cusdk__AssignDeviceInfo &tFrom, vector<TDeviceCapPrivilege>& tTo)
{
    if ( ! tFrom.videoSrcIdList.empty() )
    {
        TDeviceCapPrivilege cVidSrcCapPri;
        cVidSrcCapPri.id = DEVICE_CAP_VIDEOSOURCE;
        vector< cusdk__AssignVideoSrc >::const_iterator pVidSrcItem = tFrom.videoSrcIdList.begin();
        while ( pVidSrcItem != tFrom.videoSrcIdList.end() )
        {
            CVideosourcePrivilege cVidSrcPri;
            cVidSrcPri.SetPTZCtrl(pVidSrcItem->ptzLevel);
            if (pVidSrcItem->devPriSet.find(cusdk__PriPuAudioCtrl) != pVidSrcItem->devPriSet.end())
            {
                cVidSrcPri.SetPrivilege(CVideosourcePrivilege::en_AUDIO_PLAY);
			}
			if (pVidSrcItem->devPriSet.find(cusdk__PriPuSavePreset) != pVidSrcItem->devPriSet.end())
			{
				cVidSrcPri.SetPrivilege(CVideosourcePrivilege::en_PrePos_Set);
			}
			if (pVidSrcItem->devPriSet.find(en_CU_PriPuPtzLock) != pVidSrcItem->devPriSet.end())
			{
				cVidSrcPri.SetPrivilege(CVideosourcePrivilege::en_PTZ_Lock);
			}

            cVidSrcCapPri.privileges.insert(make_pair<int, CBitsContainer>(pVidSrcItem->vieoSrcId, cVidSrcPri));
            pVidSrcItem++;
        }
        tTo.push_back(cVidSrcCapPri);
    }

//     if( !tFrom.devPriSet.empty())
//     {
//          std::set<  enum cusdk__EDevicePri >::const_iterator pDevPriSet = tFrom.devPriSet.begin();
//          while (pDevPriSet != tFrom.devPriSet.end())
//          {
// 
//          }
//     }
}

//这里不转换UserID，所以需要调用时转换
void ConvertUserDeviceInfoFromCuToCui( const cusdk__AssignDeviceInfo &tFrom, CUserDeviceInfo &tTo)
{
    tTo.SetDeviceID(GetNameWithoutDomainFromURI(tFrom.devId));
    ConvertDevicePrivilegeFromCuToCui(tFrom.devPriSet, tTo.GetDevicePrivilege());
    ConvertDeviceCapPrivilegeFromCuToCui(tFrom, tTo.GetDeviceCapPrivilege());
}

void ConvertUserDeviceInfoFromCuiToCu( const CUserDeviceInfo &tFrom, cusdk__AssignDeviceInfo &tTo)
{
    tTo.devId = tFrom.GetDeviceID();
    ConvertDevicePrivilegeFromCuiToCu(tFrom.GetDevicePrivilege(), tTo.devPriSet);
    ConvertDeviceCapPrivilegeFromCuiToCu(tFrom.GetDeviceCapPrivilege(), tTo);
}

//这里不转换UserGroupID，所以需要调用时转换
void ConvertUserGroupDeviceInfoFromCuToCui( const cusdk__AssignDeviceInfo &tFrom, CUserGroupDeviceInfo &tTo)
{
    tTo.SetDeviceID(GetNameWithoutDomainFromURI(tFrom.devId));
    ConvertDevicePrivilegeFromCuToCui(tFrom.devPriSet, tTo.GetDevicePrivilege());
    ConvertDeviceCapPrivilegeFromCuToCui(tFrom, tTo.GetDeviceCapPrivilege());
}

void ConvertUserGroupDeviceInfoFromCuiToCu( const CUserGroupDeviceInfo &tFrom, cusdk__AssignDeviceInfo &tTo)
{
    tTo.devId = tFrom.GetDeviceID();
    ConvertDevicePrivilegeFromCuiToCu(tFrom.GetDevicePrivilege(), tTo.devPriSet);
    ConvertDeviceCapPrivilegeFromCuiToCu(tFrom.GetDeviceCapPrivilege(), tTo);
}


void ConvertTempAuthorizationInfoFromCuiToCu(const vector<CTempAuthorizationInfo>& tFrom, vector<cusdk__AuthorizationDeviceInfo > &tTo)
{
    vector<CTempAuthorizationInfo>::const_iterator pTmpAuthDevItem = tFrom.begin();
    while ( pTmpAuthDevItem != tFrom.end() )
    {
        cusdk__AuthorizationDeviceInfo cCuAuthDevInfo;
        cCuAuthDevInfo.userName = pTmpAuthDevItem->GetUsername() + "@" + pTmpAuthDevItem->GetDestinationDomain();
        cCuAuthDevInfo.devInfo.devId = pTmpAuthDevItem->GetDeviceID();
        ConvertDevicePrivilegeFromCuiToCu(pTmpAuthDevItem->GetDevicePrivilege(), cCuAuthDevInfo.devInfo.devPriSet);
        ConvertDeviceCapPrivilegeFromCuiToCu(pTmpAuthDevItem->GetDeviceCapPrivilege(), cCuAuthDevInfo.devInfo);
        cCuAuthDevInfo.description = pTmpAuthDevItem->GetDesc();
        CCMSTime cCreateTime(pTmpAuthDevItem->GetCreateTime());
        cCuAuthDevInfo.createTime = cCreateTime.GetTime();

        tTo.push_back(cCuAuthDevInfo);
        pTmpAuthDevItem++;
    }
}

void ConvertTempAuthorizationInfoFromCuToCui(const cusdk__AuthorizationDeviceInfo &tFrom, CTempAuthorizationInfo &tTo)
{
    tTo.SetUsername(GetNameWithoutDomainFromURI(tFrom.userName));
    tTo.SetDeviceID(tFrom.devInfo.devId);
    ConvertDevicePrivilegeFromCuToCui(tFrom.devInfo.devPriSet, tTo.GetDevicePrivilege());
    ConvertDeviceCapPrivilegeFromCuToCui(tFrom.devInfo, tTo.GetDeviceCapPrivilege());
    tTo.SetDesc(tFrom.description);
    tTo.SetDestinationDomain(GetDomainNameWithoutPreNameFromURI(tFrom.userName));
}

void ConvertTempCoverPolicyFromCuToCui(const TCoverPolicy &tFrom, cusdk__ERS_CoverPolicy &tTo)
{
    if(RCS_COVERPOLICY_STOPRECORD == tFrom)
    {
        tTo = cusdk__RS_COVERPOLICY_STOPRECORD;
    }
    else if(RCS_COVERPOLICY_COVERALL == tFrom)
    {
        tTo = cusdk__RS_COVERPOLICY_COVERALL;
    }
    else if(RCS_COVERPOLICY_COVERNORMAL == tFrom)
    {
        tTo = cusdk__RS_COVERPOLICY_COVERNORMAL;
    }
    else if(RCS_COVERPOLICY_UNKNOWN == tFrom)
    {
        tTo = cusdk__RS_COVERPOLICY_UNKNOWN;
    }
}

void ConvertTempCoverPolicyFromCuiToCu(const cusdk__ERS_CoverPolicy &tFrom,  TCoverPolicy &tTo)
{
    if(cusdk__RS_COVERPOLICY_STOPRECORD == tFrom)
    {
        tTo = RCS_COVERPOLICY_STOPRECORD;
    }
    else if(cusdk__RS_COVERPOLICY_COVERALL == tFrom)
    {
        tTo = RCS_COVERPOLICY_COVERALL;
    }
    else if(cusdk__RS_COVERPOLICY_COVERNORMAL == tFrom)
    {
        tTo = RCS_COVERPOLICY_COVERNORMAL;
    }
    else if(cusdk__RS_COVERPOLICY_UNKNOWN == tFrom)
    {
        tTo = RCS_COVERPOLICY_UNKNOWN;
    }
}

void ConvertTempNruInfoFromCuToCui(const vector<TNruInfo>& tFrom, vector<cusdk__NruInfo> &tTo)
{
    vector<TNruInfo>::const_iterator pNruInfo = tFrom.begin();
    while ( pNruInfo != tFrom.end() )
    {
        cusdk__NruInfo cNruInfo;
        cNruInfo.nruId   = (*pNruInfo).GetNruBaseInfo().GetNruID();
        cNruInfo.devName = (*pNruInfo).GetNruBaseInfo().GetDevName();
        cNruInfo.devType = (*pNruInfo).GetNruBaseInfo().GetDevType();
        cNruInfo.manuFactory = (*pNruInfo).GetNruBaseInfo().GetManuFactory();

        cNruInfo.ifName = (*pNruInfo).GetNetworkInfo().GetIfName();
        cNruInfo.ipAddr = (*pNruInfo).GetNetworkInfo().GetIpAddr();
        cNruInfo.netMask = (*pNruInfo).GetNetworkInfo().GetNetMask();
        cNruInfo.gateway = (*pNruInfo).GetNetworkInfo().GetGateWay();
        cNruInfo.RMSAddr = (*pNruInfo).GetSvrAddr();
        cNruInfo.maxBandWidthKb = (*pNruInfo).GetMaxBandWidth();
        cNruInfo.online = (*pNruInfo).GetIsOnline();
        cNruInfo.used = (*pNruInfo).GetIsUsed();

        tTo.push_back(cNruInfo);

        pNruInfo++;
    }
}

void ConverTempChnFromCuiToCu(const TChannelList &tFrom, vector<cusdk__DevChannel> &tTo)
{
    TChannelList::const_iterator pChnInfo = tFrom.begin();
    while(pChnInfo != tFrom.end())
    {
        cusdk__DevChannel cDevChannel;
        cDevChannel.channelId  = pChnInfo->GetChnNO();
        cDevChannel.deviceId = pChnInfo->GetDevUri();

        tTo.push_back(cDevChannel);

        pChnInfo++;
    }
}

void ConverTempChnFromCuToCui(const vector<cusdk__DevChannel> &tFrom, TChannelList &tTo)
{
    vector<cusdk__DevChannel>::const_iterator cDevChn = tFrom.begin();
    while(cDevChn != tFrom.end())
    {
        TChannel tChn;
        tChn.SetChnNO(cDevChn->channelId);
        tChn.SetDevUri(cDevChn->deviceId);

        tTo.push_back(tChn);

        cDevChn++;
    }
}

void ConverTempRecordDayFromCuiToCu(const TPuChnRecDaysList &tFrom, std::vector<cusdk__OnePuChnRecDays > &tTo, cusdk__TimeRange tTimeRange)
{
    TPuChnRecDaysList::const_iterator pPuChnRecDaysList = tFrom.begin();
    while(pPuChnRecDaysList != tFrom.end())
    {
        cusdk__OnePuChnRecDays cOnePuChnRecDays;
        cOnePuChnRecDays.puChn.channelId = pPuChnRecDaysList->GetPuChn().GetChnNO();
        cOnePuChnRecDays.puChn.deviceId = pPuChnRecDaysList->GetPuChn().GetDevUri();

        int i = 0;
        for(; i < 32; i++)
        {
            bool bTmp = (bool)((pPuChnRecDaysList->GetDayBitMask() & (1 << i)) != 0 );
            cOnePuChnRecDays.recordStateInDays.push_back(bTmp);
        }
        cOnePuChnRecDays.timeRange = tTimeRange;

        tTo.push_back(cOnePuChnRecDays);

        pPuChnRecDaysList++;
    }
}

void ConverTempPuRecordDayFromCuiToCu(int tFrom, std::vector<cusdk__OnePuChnRecDays > &tTo, cusdk__TimeRange& tTimeRange, cusdk__DevChannel& cDevChn)
{
    cusdk__OnePuChnRecDays cOnePuChnRecDays;
    cOnePuChnRecDays.puChn = cDevChn;

    int i = 0;
    for(; i < 32; i++)
    {
        bool bTmp = (bool)((tFrom & (1 << i)) != 0 );
        cOnePuChnRecDays.recordStateInDays.push_back(bTmp);
    }
    cOnePuChnRecDays.timeRange = tTimeRange;

    tTo.push_back(cOnePuChnRecDays);
}

void ConverTempRecordDurationFromCuiToCu(const TRecDurationList &tFrom, std::vector<cusdk__RecDuration > &tTo, const string& strRecordDomainName)
{
    TRecDurationList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {        
        cusdk__RecDuration cRecDuration;
        cRecDuration.recordDurationId = item->GetRecDurationToken();
       
        cRecDuration.recordDomainName = strRecordDomainName;
        
        CCMSTime cDuStartTime(item->GetTimeRange().GetStartTime());
        CCMSTime cDuEndTime(item->GetTimeRange().GetEndTime());
        cRecDuration.durationTimeRange.endTime   = cDuEndTime.GetTime();
        cRecDuration.durationTimeRange.startTime = cDuStartTime.GetTime();

		cRecDuration.encoderChn.deviceId = item->GetPuChn().GetDevUri();
		cRecDuration.encoderChn.channelId = item->GetPuChn().GetChnNO();
		cRecDuration.locked = item->GetRecLocked();
		cRecDuration.lockExpireTime = -1;

        TRecTimeRangeList::const_iterator tRecTimeRangListItem = item->GetRecTimeRangeList().begin();
        while(tRecTimeRangListItem != item->GetRecTimeRangeList().end())
        {
            cusdk__RecTimeRange cRecTimeRange;
            CCMSTime cStartTime(tRecTimeRangListItem->GetTimeRange().GetStartTime());
            cRecTimeRange.timeRange.startTime = cStartTime.GetTime();
            CCMSTime cEndTime(tRecTimeRangListItem->GetTimeRange().GetEndTime());
            cRecTimeRange.timeRange.endTime   = cEndTime.GetTime();

            if(tRecTimeRangListItem->GetRecType() == RCS_RECTYPE_ALARM)
            {
                cRecTimeRange.recEventType = cusdk__RS_RECTYPE_ALARM;
            }
            else if(tRecTimeRangListItem->GetRecType() == RCS_RECTYPE_HANDLE)
            {
                cRecTimeRange.recEventType = cusdk__RS_RECTYPE_HANDLE;
            }
            else if(tRecTimeRangListItem->GetRecType() == RCS_RECTYPE_PLAN)
            {
                cRecTimeRange.recEventType = cusdk__RS_RECTYPE_PLAN;
            }
            else if(tRecTimeRangListItem->GetRecType() == RCS_RECTYPE_UNKNOWN)
            {
                cRecTimeRange.recEventType = cusdk__RS_RECTYPE_UNKNOWN;
            }
            else if(tRecTimeRangListItem->GetRecType().empty())
            {
                cRecTimeRange.recEventType = cusdk__RS_RECTYPE_ALL;
            }

            cRecDuration.subTimeRangeList.push_back(cRecTimeRange);

            tRecTimeRangListItem++;
        }

        tTo.push_back(cRecDuration);

        item++;
    }
}

// void ConverTempRecordPlanFromCuToCui(const std::vector<cusdk__CronScedule > &tFrom, TCronTimeList &tTo)
// {
//     std::vector<cusdk__CronScedule >::const_iterator item = tFrom.begin();
//     while(item != tFrom.end())
//     {
//         TCronTime tCronTime;
//         tCronTime.SetDuration(item->duration);
//         item->cronStart;
// 
//         tCronTime.SetMonth();
//         tCronTime.SetDay();
//         tCronTime.SetHour();
//         tCronTime.SetMinute();
//         tCronTime.SetWday();
//     }
// }

void ConverTempDomainInfoFromCuiToCu(const vector<CDomainRelationInfo>& tFrom, std::vector<cusdk__DomainInfo >& tTo)
{
    vector<CDomainRelationInfo>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__DomainInfo cDomainInfo;
        cDomainInfo.domainId = item->GetDomainID();
        cDomainInfo.domainName = item->GetDomainName();
        tTo.push_back(cDomainInfo);

        item++;
    }
}

void ConverTempTvwallInfoFromCuiToCu(const vector<CTvwallInfo>& tFrom, std::vector<cusdk__TVWall >& tTo)
{
    vector<CTvwallInfo>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TVWall tTvWall;
        tTvWall.tvwallId = item->GetDeviceUUID();
        tTvWall.domainId = item->GetDomainUUID();
        tTvWall.name = item->GetDeviceName();
        tTvWall.tvNum = item->GetTvNum();
        tTvWall.tvwallStyle = item->GetTvwallStyle();
        tTvWall.tvwallCustomStyleData = item->GetTvwallCustomStyleData();
        tTvWall.desc = item->GetDescription();

        CCMSTime time(item->GetCreateDate());
        tTvWall.createTime = time.GetTime();

        vector<TvDecoderBinder>::const_iterator iDecoderBinder = item->GetTvDecoderBinders().begin();
        while(iDecoderBinder != item->GetTvDecoderBinders().end())
        {
            cusdk__TVDecoderBind tTVDecoderBind;
            tTVDecoderBind.decoderId = iDecoderBinder->decoderId;
            tTVDecoderBind.tvId = iDecoderBinder->tvId;
            tTVDecoderBind.decoderOutputId = iDecoderBinder->decoderOutputId;

            tTvWall.tvDecoderBind.push_back(tTVDecoderBind);

            iDecoderBinder++;
        }
        tTo.push_back(tTvWall);

        item++;
    }
}

void ConverTempTvwallIdFromCuiToCu(const vector<CTvwallInfo>& tFrom, std::vector<xsd__string>& tTo)
{
    vector<CTvwallInfo>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        tTo.push_back(item->GetDeviceUUID());

        item++;
    }
}

void ConverTempTvDecoderBindersCuToCui(const std::vector<cusdk__TVDecoderBind >& tFrom, vector<TvDecoderBinder>& tTo)
{
    std::vector<cusdk__TVDecoderBind >::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        TvDecoderBinder tTvDecoderBinder;
        tTvDecoderBinder.decoderId = item->decoderId;
        tTvDecoderBinder.tvId = item->tvId;
        tTvDecoderBinder.decoderOutputId = item->decoderOutputId;

        tTo.push_back(tTvDecoderBinder);

        item++;
    }
}

void ConverDecoderSdpToCuAddr(const TSdp& tSdp,cusdk__PlayRecordOnTVWallRsp &rsp)
{
	const TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	for(TMediaDescList::const_iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
		{
			const TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					const TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::const_iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
							rsp.decMediaPortInfo.deviceIP = itrNatAddr->GetNetIp();
							rsp.decMediaPortInfo.rtpVideoPort = itrNatAddr->GetNetPort();
					}
				}
				else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					const TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::const_iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						rsp.decMediaPortInfo.rtcpVideoPort = itrNatAddr->GetNetPort();
					}
				}
			}
		}
		else if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_AUDIO)
		{
			const TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					const TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::const_iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						rsp.decMediaPortInfo.rtpAudioPort = itrNatAddr->GetNetPort();
					}
				}
				else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					const TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::const_iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						rsp.decMediaPortInfo.rtcpAudioPort = itrNatAddr->GetNetPort();
					}
				}
			}
		}
	}
}

void ConverTempTvPollFromCuToCui(const std::vector<cusdk__TVWallWindowPollStep >&tFrom,  CTvPollList &tTo)
{
    std::vector<cusdk__TVWallWindowPollStep>::const_iterator item = tFrom.begin();
    int dwIndex = 0;
    while(item != tFrom.end())
    {
        TTvPollUnit tTvPollUnit;
        TChannel tchn;
        tchn.SetChnNO(item->encoderChn.channelId);
        tchn.SetDevUri(item->encoderChn.deviceId);
        tTvPollUnit.SetEncChn(tchn);
        tTvPollUnit.SetPollTime(item->duration);
        tTvPollUnit.SetPollInex(dwIndex);


        tTo.push_back(tTvPollUnit);

        dwIndex++;
        item++;
    }
}

void ConverTempTvWinDivFromCuToCui(const std::vector<cusdk__TVWALLTVDivScheme >&tFrom,  TTvWinDivList &tTo)
{
    std::vector<cusdk__TVWALLTVDivScheme>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        TTvWinDiv tTvWinDiv;
        tTvWinDiv.SetTvWinDivId(item->divId);

        ConverTempTvPollFromCuToCui(item->pollSteps, tTvWinDiv.GetPollList());

        tTo.push_back(tTvWinDiv);

        item++;
    }
}

void ConverTempTvWinFromCuToCui(const std::vector<cusdk__TVWallTVScheme > &tFrom, CTvWinList &tTo)
{
    std::vector<cusdk__TVWallTVScheme>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        TTvWin tTvWin;
        tTvWin.SetTvWinId(item->tvId);
        switch(item->divStyle)
        {
        case cusdk__TVDIV_1:
            {
                tTvWin.SetStyle(ETV_WIN_STYLE_1);
            }
            break;
        case cusdk__TVDIV_2:
            {
                tTvWin.SetStyle(ETV_WIN_STYLE_2);
            }
            break;
        case cusdk__TVDIV_3:
            {
                tTvWin.SetStyle(ETV_WIN_STYLE_3);
            }
            break;
        case cusdk__TVDIV_4:
            {
                tTvWin.SetStyle(ETV_WIN_STYLE_4);
            }
            break;
		case cusdk__TVDIV_6:
			{
				tTvWin.SetStyle(ETV_WIN_STYLE_6);
			}
			break;
		case cusdk__TVDIV_8:
			{
				tTvWin.SetStyle(ETV_WIN_STYLE_8);
			}
			break;
        case cusdk__TVDIV_9:
            {
                tTvWin.SetStyle(ETV_WIN_STYLE_9);
            }
            break;
        case cusdk__TVDIV_16:
            {
                tTvWin.SetStyle(ETV_WIN_STYLE_16);
            }
            break;
		case cusdk__TVDIV_6_EQUAL://20170725 add by Royan Support6EqualStyle
			{
				tTvWin.SetStyle(ETV_WIN_STYLE_6_EQUAL);
			}
			break;
        default:
            {

            }
            break;
        }

        ConverTempTvWinDivFromCuToCui(item->tvWallDivList, tTvWin.GetDivList());

        tTo.push_back(tTvWin);

        item++;
    }
}

void ConverTempTvWallSchemeFromCuiToCu(const CTTvWallSchemeList &tFrom, std::vector<cusdk__TVWallScheme > &tTo)
{
    CTTvWallSchemeList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TVWallScheme tTVWallScheme;
        //tTVWallScheme.schemeId = item->GetSchemeId();
		TTvWallScheme tScheme;
		UnzipTvWallSchema(*item,tScheme);

        tTVWallScheme.schemeName = tScheme.GetSchemeName();
//         if(item->GetOwnerType() == TVWALL_SCHEME_OWNER_TYPE_PUBLIC)
//         {
//             tTVWallScheme.schemeType = cusdk__SchemePublic;
//         }
//         else if(item->GetOwnerType() == TVWALL_SCHEME_OWNER_TYPE_PRIVATE)
//         {
//             tTVWallScheme.schemeType = cusdk__SchemePrivate;
//         }     
        tTVWallScheme.tvWallId = tScheme.GetTvWallId();

        ConverTempTvWinFromCuiToCu(tScheme.GetTvWinList(), tTVWallScheme.tvSchemeList);
        tTo.push_back(tTVWallScheme);

        item++;
    }
}

void ConverTempvWallSchemePollCuToCui(const std::vector<cusdk__TVWallSchemePollStep > &tFrom, CTTvWallSchemePollList &tTo)
{
    std::vector<cusdk__TVWallSchemePollStep>::const_iterator item = tFrom.begin();
    int  pollIndex = 0;
    while(item != tFrom.end())
    {    
        TTvWallSchemePollUnit tTvWallSchemePollUnit;
        tTvWallSchemePollUnit.SetPollTime(item->duration);
        tTvWallSchemePollUnit.SetSchemeName(item->tvWallSchemeName);
        tTvWallSchemePollUnit.SetPollInex(pollIndex);

        tTo.push_back(tTvWallSchemePollUnit);

        pollIndex++;
        item++;
    }
}

void ConverTempMapCuiToCu(vector<MapInfo> &tFrom, std::vector<cusdk__Map > &tTo)
{
    vector<MapInfo>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {        
        cusdk__Map tMap;
        tMap.mapId = item->GetMapId();
        tMap.mapName = item->GetMapName();

        if(item->GetMapType() == MAP_GOOGLE)
        {
            tMap.mapType = cusdk__MapTypeGoogle;
        }
        else if(item->GetMapType() == MAP_JPEG)
        {
            tMap.mapType = cusdk__MapTypeJpeg;
        }
  
        tMap.mapUrl = item->GetMapUrl();
        tMap.parentMapId = item->GetParentMapId();
        tMap.description = item->GetDescription();
        tMap.minLongitude =  item->GetBounds().GetLeftTop().GetLongitude();
        tMap.maxLongitude = item->GetBounds().GetRightBottom().GetLongitude();
        tMap.minLatitude = item->GetBounds().GetLeftTop().GetLatitude();
        tMap.maxLatitude = item->GetBounds().GetRightBottom().GetLatitude();
        tMap.minScaleLevel = item->GetMinScaleLevel();
        tMap.maxScaleLevel = item->GetMaxScaleLevel();
        tMap.screenCenterLatitude = item->GetCenterPos().GetLatitude();
        tMap.screenCenterLongitude = item->GetCenterPos().GetLongitude();
        tMap.defaultScaleLevel = item->GetDefaultScaleLevel();
        tMap.version = item->GetVersion();

        tTo.push_back(tMap);

        item++;
    }
}

void ConverTempMapPositionCuToCui(const std::vector<cusdk__MapPosition > &tFrom, std::vector< MapPosition > &tTo)
{
    std::vector<cusdk__MapPosition >::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        MapPosition tMapPosition;
        tMapPosition.SetLatitude(item->latitude);
        tMapPosition.SetLongitude(item->longitude);

        tTo.push_back(tMapPosition);

        item++;
    }
}

void ConverTempPartInfoCuiToCu(const TPartInfoList &tFrom, std::vector<cusdk__PartitionInfo > &tTo)
{
    TPartInfoList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__PartitionInfo tPartitionInfo;
        tPartitionInfo.diskId = item->GetDiskID();
        tPartitionInfo.partitionId = item->GetPartID();
        tPartitionInfo.partName = item->GetPartName();
        tPartitionInfo.totalSize = item->GetTotalSize();
        tPartitionInfo.freeSize = item->GetFreeSize();

        tPartitionInfo.partState = cusdk__RS_PARTSTATE_INVALID;
        if(RCS_PARTSTATE_OFFLINE == item->GetPartState())
        {
            tPartitionInfo.partState = cusdk__RS_PARTSTATE_OFFLINE;
        }
        else if( RCS_PARTSTATE_UNFORMATTED == item->GetPartState())
        {
            tPartitionInfo.partState = cusdk__RS_PARTSTATE_UNFORMATTED;
        }
        else if(RCS_PARTSTATE_FORMATTED == item->GetPartState())
        {
            tPartitionInfo.partState = cusdk__RS_PARTSTATE_FORMATTED;
        }
        else if(RCS_PARTSTATE_MOUNTED == item->GetPartState())
        {
            tPartitionInfo.partState = cusdk__RS_PARTSTATE_MOUNTED;
        }
        else if(RCS_PARTSTATE_UMOUNTED == item->GetPartState())
        {
            tPartitionInfo.partState = cusdk__RS_PARTSTATE_UMOUNTEDD;
        }
        else if(RCS_PARTSTATE_UNKNOWN == item->GetPartState())
        {
            tPartitionInfo.partState = cusdk__RS_PARTSTATE_UNKNOWN;
        }

        tPartitionInfo.partType = item->GetPartType();
        tPartitionInfo.abnNum = item->GetAbnNum();
        tPartitionInfo.mountPath = item->GetMountPath();
        tPartitionInfo.start = item->GetStart();
        tPartitionInfo.end = item->GetEnd();
        tPartitionInfo.recordNum = item->GetRecordNum();

        tTo.push_back(tPartitionInfo);

        item++;
    }
}

void ConverTempVidResCuiToCu(cusdk__EVideoFormat eVideoFormat, const CVidResList &tFrom, std::vector<cusdk__VideoMode > &tTo)
{
    CVidResList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__VideoMode tVideoMode;

		if ( eVideoFormat == cusdk__VF_SN4
				|| eVideoFormat == cusdk__VF_MPEG4
				|| eVideoFormat == cusdk__VF_H261
				|| eVideoFormat == cusdk__VF_H263
				|| eVideoFormat == cusdk__VF_H264
				|| eVideoFormat == cusdk__VF_H265)
		{
			tVideoMode.videoFormat = eVideoFormat;
		}
		tVideoMode.videoFormatV2 = eVideoFormat;

		tVideoMode.videoResStr = (*item);

        if((*item) == VID_RES_AUTO)
        {
            tVideoMode.videoRes = cusdk__VS_AUTO;
        }
        else if((*item) == VID_RES_QCIF)
        {
            tVideoMode.videoRes = cusdk__VS_QCIF;
        }
        else if((*item) == VID_RES_CIF)
        {
            tVideoMode.videoRes = cusdk__VS_CIF;
        }
        else if((*item) == VID_RES_2CIF)
        {
            tVideoMode.videoRes = cusdk__VS_2CIF;
        }
        else if((*item) == VID_RES_4CIF)
        {
            tVideoMode.videoRes = cusdk__VS_4CIF;
        }
        else if((*item) == VID_RES_QQCIF)
        {
            tVideoMode.videoRes = cusdk__VS_QQCIF;
        }
        else if((*item) == VID_RES_VGA)
        {
            tVideoMode.videoRes = cusdk__VS_VGA;
        }
        else if((*item) == VID_RES_QVGA)
        {
            tVideoMode.videoRes = cusdk__VS_QVGA;
        }
        else if((*item) == VID_RES_720P)
        {
            tVideoMode.videoRes = cusdk__VS_720P;
        }
        else if((*item) == VID_RES_1080P)
        {
            tVideoMode.videoRes = cusdk__VS_1080P;
        }
		else if ((*item) == VID_RES_960P)
		{
			tVideoMode.videoRes = cusdk__VS_960P;
		}
        else if((*item) == VID_RES_QXGA)
        {
            tVideoMode.videoRes = cusdk__VS_QXGA;
        }
        else
        {
            // tVideoMode.videoRes新加的整形字段不再支持了
        }

        tVideoMode.videoResStr = (*item);

        tTo.push_back(tVideoMode);

        item++;
    }
}

// void ConverTempMapPositionCuiToCu(const std::vector< MapPosition > &tFrom, std::vector<cusdk__MapPosition > &tTo)
// {
//     vector<MapPosition>::const_iterator item = tFrom.begin();
//     while(item != tFrom.end())
//     {
//         cusdk__MapPosition tMapPosition;
//         tMapPosition.latitude = item->GetLatitude();
//         tMapPosition.longitude = item->GetLongitude();
// 
//         tTo.push_back(tMapPosition);
// 
//         item++;
//     }
// }

void ConverTempMapElementCuiToCu(const vector<MapElement> &tFrom, std::vector<cusdk__MapElement > &tTo)
{
    vector<MapElement>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__MapElement tMapElement;

        tMapElement.id = item->GetId();
        tMapElement.name = item->GetName();
        tMapElement.mapId = item->GetMapId();
        tMapElement.layerId = item->GetLayerId();
		tMapElement.storeGPSTrace = item->GetStoreTrace();
        //tMapElement.elementGroupId = item->GetGroupId();

        //todo 接口信息对应不上，找周昌鸿协商后修改
        if(item->GetType() == MAP_ElEMENT_COMMAN)
        {
            tMapElement.type = cusdk__MapEleCommon;
        }
        else if(item->GetType() == MAP_ELEMENT_LIVE)
        {
            tMapElement.type = cusdk__MapEleMove;
        }
//         else if(item->GetType() == MAP_ELEMENT_SUBMAP)
//         {
//             tMapElement.type = cusdk__MapEleSubMap; 
//         }

        tMapElement.deviceId = item->GetDeviceId();
//        tMapElement.deviceDomainId = item->GetDeviceDomainId();
        tMapElement.deviceChannel = item->GetDeviceChannel();

        //todo 接口信息对应不上，找周昌鸿协商后修改
        if(item->GetChannelType() == MAP_DEVCHANNEL_VIDEO)
        {
            tMapElement.channelType = cusdk__DevChnTypeVideoSource;
        }
        else if(item->GetChannelType() == MAP_DEVCHANNEL_ALARMIN)
        {
            tMapElement.channelType = cusdk__DevChnTypeAlarmInput;
        }
        
        tMapElement.position.latitude   =  item->GetPosition().GetLatitude();
        tMapElement.position.longitude  = item->GetPosition().GetLongitude();
        if( true == item->GetEarthCoord())
        {
            tMapElement.coordType = cusdk__MapCoordEarth;
        }
        else
        {
            tMapElement.coordType = cusdk__MapCoordMar;
        }
        
		tMapElement.marPosition.latitude = item->GetMarsPosition().GetLatitude();
		tMapElement.marPosition.longitude = item->GetMarsPosition().GetLongitude();
        
        tMapElement.picUrl = item->GetPicUrl();
        tMapElement.description = item->GetDescription();

        tTo.push_back(tMapElement);

        item++;
    }
}

void ConverTempAlarmLinkageCuiToCu(const CAlarmLinkageList &tFrom, std::vector<cusdk__AlarmLinkConfig > &tTo)
{
    CAlarmLinkageList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__AlarmLinkConfig tAlarmLinkConfig;
//        tAlarmLinkConfig.alarmName = item->GetAlarmAlias();
        tAlarmLinkConfig.alarmSource.alarmDeviceId = item->GetAlarmLinkSrc().GetDevUri();
        tAlarmLinkConfig.alarmSource.alarmInputChnId = item->GetAlarmLinkSrc().GetInputId();

        CMS_ALARM_TYPE cmsType = item->GetAlarmLinkSrc().GetAlarmType();//ALARM_TYPE_PIN
        const CAlarmTypeCommon *pcAlmUtils = GetCommonType();
        if( pcAlmUtils != NULL )
        {
            //cusdk__AlarmInput
            tAlarmLinkConfig.alarmSource.alarmType = cusdk__EAlarmType( pcAlmUtils->ConvertFromCmsToCu( cmsType ) ); //cusdk__AlarmInput
        }

        tAlarmLinkConfig.enabled = item->GetIsUsed();

        ConverTempAlarmLinkDstCuiToCu(item->GetAlarmLinkDstList(), tAlarmLinkConfig.alarmLinkActionList);
        ConverTempCronTimeCuiToCu(item->GetDefenceSched(), tAlarmLinkConfig.enabledCronTimeList);

        tTo.push_back(tAlarmLinkConfig);

        item++;
    }
}

void ConverTempCronTimeCuToCui(const std::vector<string > &tFrom, TCronTimeList &tTo)
{
    std::vector<string >::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        tTo.push_back(*item);

        item++;
    }
}

void ConverTempLinkActionCuToCui(const std::vector<enum cusdk__EAlarmLinkActionType > &tFrom, CLinkActionList &tTo)
{
    std::vector<cusdk__EAlarmLinkActionType >::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        switch(*item)
        {
        case cusdk__AlarmLinkActionPlatformRecord:
            {
                tTo.insert(ALARMLINK_PLAT_REC);
            }
            break;
        case cusdk__AlarmLinkActionPlayOnTV:
            {
                tTo.insert(ALARMLINK_TV_WALL);
            }
            break;
        case cusdk__AlarmLinkActionReadSecond:
            {
                tTo.insert(ALARMLINK_READ_SEC);
            }
            break;          
        case cusdk__AlarmLinkActionIOOutput:
            {
                tTo.insert(ALARMLINK_IO_OUT);
            }
            break;          
        case cusdk__AlarmLinkCameraPrePos:
            {
                tTo.insert(ALARMLINK_CAM);
            }
            break;
        case cusdk__AlarmLinkActionTypeInvalid:
        default:
            break;
        }

        item++;
    }
};

void ConverTempAlarmLinkCuToCui(const std::vector<cusdk__AlarmLinkAction > &tFrom, CAlarmLinkDstList &tTo)
{
    std::vector<cusdk__AlarmLinkAction >::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        TAlarmLinkDst tAlarmLinkDst;
        ConverTempLinkActionCuToCui(item->actionType, tAlarmLinkDst.GetLinkActionList());
        TChannel encChn;            //告警联动目的编码通道
        encChn.SetChnNO(item->encoderChn.channelId);
        encChn.SetDevUri(item->encoderChn.deviceId);
        tAlarmLinkDst.SetEncChn(encChn);
        tAlarmLinkDst.SetCamPrePos(item->camPrePos);
        tAlarmLinkDst.SetTvWallId(item->tvWallId);
        tAlarmLinkDst.SetTvId(item->tvId);
//         tAlarmLinkDst.GetMsgNoList() = item->SMSPhoneNumList ;
//         tAlarmLinkDst.GetVocNoList() = item->voicePhoneNumList;
//         tAlarmLinkDst.GetMailNoList() = item->mailList;
        tAlarmLinkDst.SetTvDivId(item->tvDivId);
        tAlarmLinkDst.SetAlarmOutChn(item->alarmIOOutputChnId);

        tTo.push_back(tAlarmLinkDst);

        item++;
    }
}

void ConverTempOperateLogCuiToCu(const vector<TOperateLogInfo> &tFrom, std::vector<cusdk__UserLog > &tTo)
{
    vector<TOperateLogInfo>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__UserLog tUserLog;

        tUserLog.logId = item->logId;
        tUserLog.loginSession = item->logTag;
        tUserLog.userId = item->userUri;
        tUserLog.domainName = GetDomainNameWithoutPreNameFromURI(item->userUri);
        tUserLog.userName = item->userName;
        tUserLog.ipAddr = item->netAddr;
        CCMSTime thappenedTime(item->happenedTime);
        tUserLog.happenedTime = thappenedTime.GetTime();
        tUserLog.operateType = (cusdk__EUserOperateType)item->operateType;
        tUserLog.operateResult = item->operateResult;
        tUserLog.operateDesc = item->operateDesc;

        tTo.push_back(tUserLog);

        item++;
    }
}

void ConverTempDeviceLogCuiToCu(const vector<TDeviceLogInfo> &tFrom, std::vector<cusdk__DeviceLog > &tTo)
{
    vector<TDeviceLogInfo>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__DeviceLog tDeviceLog;

        tDeviceLog.logId = item->logId;
        tDeviceLog.deviceId = item->devUri;
        //tDeviceLog.deviceDomainName = item->devDomainName;
        tDeviceLog.deviceName = item->devName;
        tDeviceLog.deviceIp = item->netAddr;
        tDeviceLog.deviceAlias = item->devAlias;

        CCMSTime happendedTime(item->happenedTime);
        tDeviceLog.happenedTime = happendedTime.GetTime();
        tDeviceLog.logType = (cusdk__EDeviceLogType)item->logType;
        tDeviceLog.logDesc = item->logDesc;

        tTo.push_back(tDeviceLog);

        item++;
    }
}

void ConverTempGpsTraceCuiToCu(const vector<TGpsStatus> &tFrom, std::vector<cusdk__GPSInfo> &tTo)
{
	vector<TGpsStatus>::const_iterator item = tFrom.begin();
	while(item != tFrom.end())
	{
		cusdk__GPSInfo tGpsInfo;
		tGpsInfo.marLatitude = item->GetLatitude();
		tGpsInfo.marLongitude= item->GetLongitude();
		tGpsInfo.speed  = item->GetSpeed();
		CCMSTime revTime(item->GetTimeStamp());
		tGpsInfo.revTime= revTime.GetTime();
        tGpsInfo.height = item->GetAltitude();
        tGpsInfo.angle  = item->GetDirection();

		tTo.push_back(tGpsInfo);
		item++;
	}
}

void ConverTempAlarmLogCuiToCu(const vector<TAlarmLogInfo> &tFrom, std::vector<cusdk__AlarmLog > &tTo)
{
    vector<TAlarmLogInfo>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__AlarmLog tAlarmLog;
        
        tAlarmLog.logId = item->logId;
        tAlarmLog.deviceId = item->devUri;
        //tAlarmLog.deviceDomainName = item->devDomainName;
        tAlarmLog.deviceName = item->devName;
        tAlarmLog.deviceAlias = item->devAlias;
        tAlarmLog.alarmChannelId = item->channId;
        
        CMS_ALARM_TYPE cmsType = item->type;//ALARM_TYPE_PIN
        const CAlarmTypeCommon *pcAlmUtils = GetCommonType();
        if( pcAlmUtils != NULL )
        {
            tAlarmLog.alarmType = cusdk__EAlarmType( pcAlmUtils->ConvertFromCmsToCu( cmsType ) ); //cusdk__AlarmInput
        }
		
        if(ALARM_FLAG_OCCUR == item->flag)
        {
            tAlarmLog.alarmStatus = cusdk__AlarmOccur;
        }
        else if(ALARM_FLAG_RESTORE == item->flag)
        {
            tAlarmLog.alarmStatus = cusdk__AlarmRestore;
        }

        CCMSTime occur(item->occurTime);
        tAlarmLog.occurTime = occur.GetTime();
        CCMSTime resume(item->resumeTime);
        tAlarmLog.resumeTime = resume.GetTime();
        tAlarmLog.alarmName = item->alias;
        tAlarmLog.description = item->desc;

        tTo.push_back(tAlarmLog);

        item++;
    }
}

void ConvertVideoEncParamCuiToCu(const TVideoEncodeParam &tFrom, cusdk__VideoEncodeParam  &tTo)
{
	if (tFrom.GetVideoType() == VID_FMT_SN4)
	{
		tTo.videoMode.videoFormat = cusdk__VF_SN4;
		tTo.videoMode.videoFormatV2 = cusdk__VF_SN4;
	}
	else if (tFrom.GetVideoType() == VID_FMT_MPEG4)
	{
		tTo.videoMode.videoFormat = cusdk__VF_MPEG4;
		tTo.videoMode.videoFormatV2 = cusdk__VF_MPEG4;
	}
	else if (tFrom.GetVideoType() == VID_FMT_H261)
	{
		tTo.videoMode.videoFormat = cusdk__VF_H261;
		tTo.videoMode.videoFormatV2 = cusdk__VF_H261;
	}
	else if (tFrom.GetVideoType() == VID_FMT_H263)
	{
		tTo.videoMode.videoFormat = cusdk__VF_H263;
		tTo.videoMode.videoFormatV2 = cusdk__VF_H263;
	}
	else if (tFrom.GetVideoType() == VID_FMT_H264)
	{
		tTo.videoMode.videoFormat = cusdk__VF_H264;
		tTo.videoMode.videoFormatV2 = cusdk__VF_H264;
	}
	else if (tFrom.GetVideoType() == VID_FMT_H265)
	{
		tTo.videoMode.videoFormat = cusdk__VF_H265;
		tTo.videoMode.videoFormatV2 = cusdk__VF_H265;
	}
	else if (tFrom.GetVideoType() == VID_FMT_SVAC)
	{
		tTo.videoMode.videoFormatV2 = cusdk__VF_SVAC;
	}
	else if (tFrom.GetVideoType() == VID_FMT_MJPEG)
	{
		tTo.videoMode.videoFormatV2 = cusdk__VF_MJPEG;
	}

	//todo 可能不支持1080a，后续需要讨论
	tTo.videoMode.videoResStr = tFrom.GetVideoRes();
	if (tFrom.GetVideoRes() == VID_RES_AUTO)
	{
		tTo.videoMode.videoRes = cusdk__VS_AUTO;
	}
	else if (tFrom.GetVideoRes() == VID_RES_QCIF)
	{
		tTo.videoMode.videoRes = cusdk__VS_QCIF;
	}
	else if (tFrom.GetVideoRes() == VID_RES_CIF)
	{
		tTo.videoMode.videoRes = cusdk__VS_CIF;
	}
	else if (tFrom.GetVideoRes() == VID_RES_2CIF)
	{
		tTo.videoMode.videoRes = cusdk__VS_2CIF;
	}
	else if (tFrom.GetVideoRes() == VID_RES_4CIF)
	{
		tTo.videoMode.videoRes = cusdk__VS_4CIF;
	}
	else if (tFrom.GetVideoRes() == VID_RES_QQCIF)
	{
		tTo.videoMode.videoRes = cusdk__VS_QQCIF;
	}
	else if (tFrom.GetVideoRes() == VID_RES_VGA)
	{
		tTo.videoMode.videoRes = cusdk__VS_QVGA;
	}
	else if (tFrom.GetVideoRes() == VID_RES_QVGA)
	{
		tTo.videoMode.videoRes = cusdk__VS_VGA;
	}
	else if (tFrom.GetVideoRes() == VID_RES_720P)
	{
		tTo.videoMode.videoRes = cusdk__VS_720P;
	}
	else if (tFrom.GetVideoRes() == VID_RES_1080P)
	{
		tTo.videoMode.videoRes = cusdk__VS_1080P;
	}
	else if (tFrom.GetVideoRes() == VID_RES_960P)
	{
		tTo.videoMode.videoRes = cusdk__VS_960P;
	}
	else if (tFrom.GetVideoRes() == VID_RES_QXGA)
	{
		tTo.videoMode.videoRes = cusdk__VS_QXGA;
	}

	tTo.frameRate = tFrom.GetFrameRate();

	if (tFrom.GetQuality() == VID_QUALITY_PRIOR)
	{
		tTo.quality = cusdk__VIDEO_QUALITY_PRIOR;
	}
	else if (tFrom.GetQuality() == VID_SPEED_PRIOR)
	{
		tTo.quality = cusdk__VIDEO_SPEED_PRIOR;
	}
	tTo.bitRate = tFrom.GetBitRate();
	tTo.interval = tFrom.GetInterval();
	tTo.brightness = tFrom.GetBright();
	tTo.contrast = tFrom.GetContrast();
	tTo.saturation = tFrom.GetSaturation();
	tTo.sharpness = tFrom.GetSharpness();
}

void ConverTempVidTypeCuiToCu(const CVidTypeList &tFrom, std::vector<cusdk__VideoMode > &tTo)
{
    CVidTypeList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        std::vector<cusdk__VideoMode > var;
        if(item->first == VID_FMT_SN4)
        {
            var.clear();
            ConverTempVidResCuiToCu(cusdk__VF_SN4, item->second.GetVidResSet(), var);

            tTo.insert(tTo.end(), var.begin(), var.end());
        }
        else if(item->first == VID_FMT_MPEG4)
        {
            var.clear();
            ConverTempVidResCuiToCu(cusdk__VF_MPEG4, item->second.GetVidResSet(), var);

            tTo.insert(tTo.end(), var.begin(), var.end());
        }
        else if(item->first == VID_FMT_H261)
        {
            var.clear();
            ConverTempVidResCuiToCu(cusdk__VF_H261, item->second.GetVidResSet(), var);

            tTo.insert(tTo.end(), var.begin(), var.end());
        }
        else if(item->first == VID_FMT_H263)
        {
            var.clear();
            ConverTempVidResCuiToCu(cusdk__VF_H263, item->second.GetVidResSet(), var);

            tTo.insert(tTo.end(), var.begin(), var.end());
        }
        else if(item->first == VID_FMT_H264)
        {
            var.clear();
            ConverTempVidResCuiToCu(cusdk__VF_H264, item->second.GetVidResSet(), var);

            tTo.insert(tTo.end(), var.begin(), var.end());
		}
		else if(item->first == VID_FMT_H265)
		{
			var.clear();
			ConverTempVidResCuiToCu(cusdk__VF_H265, item->second.GetVidResSet(), var);

			tTo.insert(tTo.end(), var.begin(), var.end());
		}
		else if(item->first == VID_FMT_SVAC)
		{
			var.clear();
			ConverTempVidResCuiToCu(cusdk__VF_SVAC, item->second.GetVidResSet(), var);

			tTo.insert(tTo.end(), var.begin(), var.end());
		}
		else if(item->first == VID_FMT_MJPEG)
		{
			var.clear();
			ConverTempVidResCuiToCu(cusdk__VF_MJPEG, item->second.GetVidResSet(), var);

			tTo.insert(tTo.end(), var.begin(), var.end());
		}

        item++;
    }
}

void CoverTempAudioModeCuiToCu(const string &tFrom, int &tTo)
{
	if (tFrom == AUD_FMT_PCMA)
	{
		tTo = cusdk__AF_PCMA;
	}
	else if (tFrom == AUD_FMT_PCMU)
	{
		tTo = cusdk__AF_PCMU;
	}
	else if (tFrom == AUD_FMT_GSM)
	{
		tTo = cusdk__AF_GSM;
	}
	else if (tFrom == AUD_FMT_G729)
	{
		tTo = cusdk__AF_G729;
	}
	else if (tFrom == AUD_FMT_ADPCM)
	{
		tTo = cusdk__AF_ADPCM;
	}
	else if (tFrom == AUD_FMT_G7221C)
	{
		tTo = cusdk__AF_G7221C;
	}
	else if (tFrom == AUDIO_FMT_G722)
	{
		tTo = cusdk__AF_G722;
	}
	else if (tFrom == AUDIO_FMT_AACLC)
	{
		tTo = cusdk__AF_AACLC;
	}
	else if (tFrom == AUDIO_FMT_G711)
	{
		tTo = cusdk__AF_G711;
	}
	else if (tFrom == AUDIO_FMT_G726)
	{
		tTo = cusdk__AF_G726;
	}
	else if (tFrom == AUDIO_FMT_AAC)
	{
		tTo = cusdk__AF_AAC;
	}
	else
	{
		tTo = cusdk__AF_Invalid;
	}

	return;
}

void ConvertAudioEncParamCuiToCu(const TAudioEncodeParam& tFrom, cusdk__AudioEncoderParam &tTo)
{
	CoverTempAudioModeCuiToCu(tFrom.GetAudioType(), tTo.audioForamt);
	tTo.volume = tFrom.GetVolume();
	tTo.AECState = tFrom.GetAecState();
	tTo.sampleRate = tFrom.GetSmpRate();
	tTo.sampleBits = tFrom.GetBitRate();
	tTo.channleNum = tFrom.GetChannel();
	tTo.samplePerFrame = tFrom.GetSamplePerF();
}

void ConverTempAudTypeCuiToCu(const CAudTypeList &tFrom, std::vector< cusdk__AudioMode > &tTo)
{
	cusdk__AudioMode cuAudioMode;
    CAudTypeList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
		CoverTempAudioModeCuiToCu(*item, cuAudioMode.audioFormat);

		tTo.push_back(cuAudioMode);

        item++;
    }
};

void ConverTempDiskInfoCuiToCu(const TDiskInfoList &tFrom, std::vector<cusdk__DiskInfo > &tTo)
{
    TDiskInfoList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__DiskInfo tDiskInfo;
        tDiskInfo.diskId = item->GetDiskID();
        tDiskInfo.diskName = item->GetDiskName();
        tDiskInfo.diskSize = item->GetDiskSize();
        tDiskInfo.diskAbnNum = item->GetDiskAbnNum();

        tDiskInfo.diskState = cusdk__RS_DISKSTATE_INVALID;
        if(RCS_DISKSTATE_ONLINE == item->GetDiskState())
        {
            tDiskInfo.diskState = cusdk__RS_DISKSTATE_ONLINE;
        }
        else if(RCS_DISKSTATE_OFFLINE == item->GetDiskState())
        {
            tDiskInfo.diskState = cusdk__RS_DISKSTATE_OFFLINE;
        }
        else if(RCS_DISKSTATE_UNKNOWN == item->GetDiskState())
        {
            tDiskInfo.diskState = cusdk__RS_DISKSTATE_UNKNOWN;
        }

        tDiskInfo.diskSource = item->GetDiskSource();
        tDiskInfo.cylinders = item->GetCylinders();
        tDiskInfo.heads = item->GetHeads();
        tDiskInfo.sectors = item->GetSectors();
        
        ConverTempPartInfoCuiToCu(item->GetPartList(), tDiskInfo.partitionList);

        tTo.push_back(tDiskInfo);

        item++;
    }
}

void ConvertMediaTransChannelFromCuToCui(const std::vector<cusdk__TransChannel > &tFrom, TMediaTransChannelList &tTo)
{
    std::vector<cusdk__TransChannel >::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        TMediaTransChannel tMediaTransChannel;
        switch(item->transChannelType)
        {
        case cusdk__TransChannelTypeRTP:
            {
                tMediaTransChannel.SetTransChnType(MEDIA_TRANS_CHN_RTP);
            }
            break;
        case cusdk__TransChannelTypeRTCP:
            {
                tMediaTransChannel.SetTransChnType(MEDIA_TRANS_CHN_RTCP);
            }
            break;
        case cusdk__TransChannelTypeData:
            {
                tMediaTransChannel.SetTransChnType(MEDIA_TRANS_CHN_DATA);
            }
            break;
        case cusdk__MediaTypeInvalid:
        default:
           {
               tMediaTransChannel.SetTransChnType(MEDIA_TRANS_CHN_UNKNOWN);
           }
           break;
        }      
        tMediaTransChannel.SetMediaChannelKey(item->natData);

        std::vector<cusdk__NetworkAddr >::const_iterator addrIterm = item->netAddrList.begin();
        while(addrIterm != item->netAddrList.end())
        {
            TNetAddr addr;
            ConvertNetAddrFromCuToCui(*addrIterm, addr);
            if ( !addr.GetNetIp().empty() )
            {
				// 去掉没有IP的无用网络地址
				tMediaTransChannel.GetAddrList().push_back(addr);
            }

            addrIterm++;
        }
        
        tTo.push_back(tMediaTransChannel);

        item++;
    }
}


void ConvertMediaTransChannelFromCuiToCu(const TMediaTransChannelList &tFrom, std::vector<cusdk__TransChannel > &tTo)
{
    TMediaTransChannelList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__TransChannel tMediaTransChannel;
        if( MEDIA_TRANS_CHN_RTP == item->GetTransChnType())
        {
            tMediaTransChannel.transChannelType = cusdk__TransChannelTypeRTP;
        }
        else if(MEDIA_TRANS_CHN_RTCP == item->GetTransChnType())
        {
            tMediaTransChannel.transChannelType = cusdk__TransChannelTypeRTCP;
        }
        else if(MEDIA_TRANS_CHN_DATA == item->GetTransChnType())
        {
            tMediaTransChannel.transChannelType = cusdk__TransChannelTypeData;
        }
        else if(MEDIA_TRANS_CHN_UNKNOWN == item->GetTransChnType())
        {
            tMediaTransChannel.transChannelType = cusdk__TransChannelTypeInvalid;
        }

        tMediaTransChannel.natData = item->GetMediaChannelKey(); 

        TNetAddrList::const_iterator tNetAddrItem = item->GetAddrList().begin();
        while(tNetAddrItem != item->GetAddrList().end())
        {
            cusdk__NetworkAddr addr;
            ConvertNetAddrFromCuiToCu(*tNetAddrItem, addr);

            tMediaTransChannel.netAddrList.push_back(addr);

            tNetAddrItem++;
        }
              
       tTo.push_back(tMediaTransChannel);

       item++;
    }
}

void ConverTempMotionDetectInfoCuToCui(const std::vector<cusdk__MotionDetectAreaConfig>& tFrom, TMotionDetectList& tTo)
{
    std::vector<cusdk__MotionDetectAreaConfig>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        TMotionDetectParam tMotionDetectParam;
        tMotionDetectParam.SetAlarmOccurRate(item->alarmRate);
        tMotionDetectParam.SetLeftTopX(item->detectArea.x);
        tMotionDetectParam.SetLeftTopY(item->detectArea.y);
        tMotionDetectParam.SetHeight(item->detectArea.height);
        tMotionDetectParam.SetWidth(item->detectArea.width);
        tMotionDetectParam.SetEnable(item->enable);
        tMotionDetectParam.SetAlarmRestoreRate(item->restoreRate);

        tTo.push_back(tMotionDetectParam);

        item++;
    }
}

void ConverTempAreaShadeParamCuiToCu(const TAreaShadeList& tFrom, std::vector<cusdk__ShadeAreaConfig >& tTo)
{
    TAreaShadeList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__ShadeAreaConfig cShadeAreaConfig;
        cShadeAreaConfig.enable = item->GetEnable();
        cShadeAreaConfig.shadeArea.x = item->GetLeftTopX();
        cShadeAreaConfig.shadeArea.y = item->GetLeftTopY();
        cShadeAreaConfig.shadeArea.height = item->GetHeight();
        cShadeAreaConfig.shadeArea.width = item->GetWidth();

        tTo.push_back(cShadeAreaConfig);

        item++;
    }
}

void ConverTempAreaShadeParamCuToCui(const std::vector<cusdk__ShadeAreaConfig >& tFrom, TAreaShadeList& tTo)
{
    std::vector<cusdk__ShadeAreaConfig >::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        TAreaShadeParam tAreaShadeParam;
        tAreaShadeParam.SetEnable(item->enable);
        tAreaShadeParam.SetHeight(item->shadeArea.height);
        tAreaShadeParam.SetWidth(item->shadeArea.width);
        tAreaShadeParam.SetLeftTopX(item->shadeArea.x);
        tAreaShadeParam.SetLeftTopY(item->shadeArea.y);

        tTo.push_back(tAreaShadeParam);
        
        item++;
    }
}

void ConverTempMotionDetectInfoCuiToCu(const TMotionDetectList& tFrom, std::vector<cusdk__MotionDetectAreaConfig>& tTo)
{
    TMotionDetectList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {
        cusdk__MotionDetectAreaConfig cMotionDetectAreaConfig;
        cMotionDetectAreaConfig.alarmRate = item->GetAlarmOccurRate();
        cMotionDetectAreaConfig.detectArea.x = item->GetLeftTopX();
        cMotionDetectAreaConfig.detectArea.y = item->GetLeftTopY();
        cMotionDetectAreaConfig.detectArea.height = item->GetHeight();
        cMotionDetectAreaConfig.detectArea.width = item->GetWidth();
        cMotionDetectAreaConfig.enable = item->GetEnable();
        cMotionDetectAreaConfig.restoreRate = item->GetAlarmRestoreRate();

        tTo.push_back(cMotionDetectAreaConfig);

        item++;
    }
}

void ConverTempRtpStreamProperty(const std::vector<cusdk__MediaFormat >& tMediaFormat,const cusdk__EMediaType& eMediaType,TMediaFormat& tStreamProperty )
{
	if (!tMediaFormat.empty())
	{
		if (cusdk__MediaTypeVideo == eMediaType)
		{
			tStreamProperty.SetFormatParam(VIDEO_ENC_BIT_RATE,tMediaFormat[0].bitRate);
			tStreamProperty.SetFormatParam(VIDEO_ENC_RATE_TYPE,tMediaFormat[0].bitRateType);
			tStreamProperty.SetFormatParam(VIDEO_ENC_CLOCK_RATE,tMediaFormat[0].clockRate);
			tStreamProperty.SetFormatParam(VIDEO_ENC_FRAME_RATE,tMediaFormat[0].frameRate);
			char buf[256]={0};
			sprintf( buf, "W:%d,H:%d", tMediaFormat[0].videoWidth, tMediaFormat[0].videoHeight);
			tStreamProperty.SetFormatParam(VIDEO_ENC_PARAM_RESOLUTION, buf);
			tStreamProperty.SetFormatParam(VIDEO_ENC_ENCODE_FORMAT,tMediaFormat[0].encodingType);
		}
		else if (cusdk__MediaTypeAudio == eMediaType)
		{
			tStreamProperty.SetFormatParam(AUDIO_ENC_CHANNEL_NUM,tMediaFormat[0].audioChannels);
			tStreamProperty.SetFormatParam(AUDIO_ENC_BIT_RATE,tMediaFormat[0].bitRate);
			tStreamProperty.SetFormatParam(AUDIO_ENC_CLOCK_RATE,tMediaFormat[0].clockRate);
			tStreamProperty.SetFormatParam(AUDIO_ENC_SAMPLE_RATE,tMediaFormat[0].samplingRate);
			tStreamProperty.SetFormatParam(AUDIO_ENC_ENCODE_FORMAT,tMediaFormat[0].encodingType);
		}
	}
}

void ConverMediaAddrCuToCui( const cusdk__PlayRecordOnTVWallReq &cReq, TMediaDescList& tMediaDescList ) 
{

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		TMediaTransProto&        tMediaTransProto = itrMediaDesc->GetMediaTransProto();           //如：RTP/AVP/UDP;unicast

		if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
		{
			//设置媒体传输协议
			tMediaTransProto.SetTransport(MEDIA_TRANS_RTP);
			itrMediaDesc->SetMediaTransProto(tMediaTransProto);

			TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					TNetAddr tAddr;
					tAddr.SetNetIp(cReq.localMediaPortInfo.deviceIP);
					tAddr.SetNetPort(cReq.localMediaPortInfo.rtpVideoPort);
					itrTransChn->AddAddrAtBack(tAddr);
				}
				else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					TNetAddr tAddr;
					tAddr.SetNetIp(cReq.localMediaPortInfo.deviceIP);
					tAddr.SetNetPort(cReq.localMediaPortInfo.rtcpVideoPort);
					itrTransChn->AddAddrAtBack(tAddr);             
				}
			}
		}
		else if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_AUDIO)
		{
			//设置媒体传输协议
			tMediaTransProto.SetTransport(MEDIA_TRANS_RTP);
			itrMediaDesc->SetMediaTransProto(tMediaTransProto);

			TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					TNetAddr tAddr;
					tAddr.SetNetIp(cReq.localMediaPortInfo.deviceIP);
					tAddr.SetNetPort(cReq.localMediaPortInfo.rtpAudioPort);

					itrTransChn->AddAddrAtBack(tAddr);
				}
				else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					TNetAddr tAddr;
					tAddr.SetNetIp(cReq.localMediaPortInfo.deviceIP);
					tAddr.SetNetPort(cReq.localMediaPortInfo.rtcpAudioPort);

					itrTransChn->AddAddrAtBack(tAddr);                   
				}
			}
		}
	}
}


void ConverTempMediaDescListCuToCui( const std::vector<cusdk__MediaDescription > &mediaDescriptionList, TMediaDescList& tMediaDescList ) 
{
    vector<cusdk__MediaDescription >::const_iterator pCuMediaDescItem = mediaDescriptionList.begin();
    while(pCuMediaDescItem != mediaDescriptionList.end())
    {   
        //todo 黄志春 格式参数如何填写？
        TMediaDesc tMediaDesc;
        if(cusdk__MediaTypeVideo == pCuMediaDescItem->mediaType)
        {
            tMediaDesc.SetMediaType(MEDIA_TYPE_VIDEO);
        }
        else if(cusdk__MediaTypeAudio == pCuMediaDescItem->mediaType)
        {
            tMediaDesc.SetMediaType(MEDIA_TYPE_AUDIO);
        }
        else if(cusdk__MediaTypeData == pCuMediaDescItem->mediaType)
        {
            tMediaDesc.SetMediaType(MEDIA_TYPE_DATA);
        }

        std::vector<cusdk__MediaFormat >::const_iterator pCuMediaFormatItem = pCuMediaDescItem->supportedFormats.begin();
        while(pCuMediaFormatItem != pCuMediaDescItem->supportedFormats.end())
        {
            TMediaFormat mediaFormat;
            mediaFormat.SetEncodingName(pCuMediaFormatItem->encodingName);
            mediaFormat.SetManufacture(pCuMediaFormatItem->manufacture);
            mediaFormat.SetPayloadType(pCuMediaFormatItem->payloadType);
            //todo 缺少详细的格式参数。根据实际需要设置，比如分辨率、关键帧间隔等，找李之兴协商
            //mediaFormat.SetFormatParam();
            tMediaDesc.SetMediaFormat(mediaFormat);

            pCuMediaFormatItem++;
        }       

        TMediaTransProto mediaTransProto; 
        //if( cusdk__TransModeUDP == pCuMediaDescItem->transMode)
        //{
            mediaTransProto.SetLowerTransport(LOWER_TRANS_UDP);
        //}
        //else if(cusdk__TransModeTCP == pCuMediaDescItem->transMode)
        //{
        //    mediaTransProto.SetLowerTransport(LOWER_TRANS_TCP);
        //}
        tMediaDesc.SetMediaTransProto(mediaTransProto);

        //tMediaDesc.SetMediaType(pCuMediaDescItem->supportedResolution.front());
		ConverTempRtpStreamProperty(pCuMediaDescItem->supportedFormats,pCuMediaDescItem->mediaType,tMediaDesc.GetMediaFormat());
        ConvertMediaTransChannelFromCuToCui( pCuMediaDescItem->transChannelList, tMediaDesc.GetMediaTransChnList());

        tMediaDescList.push_back(tMediaDesc);

        pCuMediaDescItem++;
    }
}

void ConverTempMediaDescListCuiToCu( const TMediaDescList &tMediaDescList, std::vector<cusdk__MediaDescription > &cMediaDescList ) 
{
    TMediaDescList::const_iterator pCuiMediaDescItem = tMediaDescList.begin();
    while(pCuiMediaDescItem != tMediaDescList.end())
    {
		cusdk__MediaFormat cuMediaFormat;
        cusdk__MediaDescription cusdkMediaDescription;
        if( MEDIA_TYPE_VIDEO == pCuiMediaDescItem->GetMediaType())
        {
            cusdkMediaDescription.mediaType = cusdk__MediaTypeVideo;
			pCuiMediaDescItem->GetMediaFormat().GetFormatParam(VIDEO_ENC_BIT_RATE, cuMediaFormat.bitRate);
			pCuiMediaDescItem->GetMediaFormat().GetFormatParam(VIDEO_ENC_CLOCK_RATE,cuMediaFormat.clockRate);
			pCuiMediaDescItem->GetMediaFormat().GetFormatParam(VIDEO_ENC_RATE_TYPE,cuMediaFormat.bitRateType);
			pCuiMediaDescItem->GetMediaFormat().GetFormatParam(VIDEO_ENC_ENCODE_FORMAT,cuMediaFormat.encodingType);
        }
        else if( MEDIA_TYPE_AUDIO == pCuiMediaDescItem->GetMediaType())
        {
            cusdkMediaDescription.mediaType = cusdk__MediaTypeAudio;
			pCuiMediaDescItem->GetMediaFormat().GetFormatParam(AUDIO_ENC_CLOCK_RATE,cuMediaFormat.clockRate);
			pCuiMediaDescItem->GetMediaFormat().GetFormatParam(AUDIO_ENC_CHANNEL_NUM,cuMediaFormat.audioChannels);
			pCuiMediaDescItem->GetMediaFormat().GetFormatParam(AUDIO_ENC_SAMPLE_RATE,cuMediaFormat.samplingRate);
			pCuiMediaDescItem->GetMediaFormat().GetFormatParam(AUDIO_ENC_BIT_RATE,cuMediaFormat.bitRate);
			pCuiMediaDescItem->GetMediaFormat().GetFormatParam(AUDIO_ENC_ENCODE_FORMAT,cuMediaFormat.encodingType);
        }
        else if( MEDIA_TYPE_DATA == pCuiMediaDescItem->GetMediaType())
        {
            cusdkMediaDescription.mediaType = cusdk__MediaTypeData;
        }
        else if(MEDIA_TYPE_UNKNOWN == pCuiMediaDescItem->GetMediaType())
        {
            cusdkMediaDescription.mediaType = cusdk__MediaTypeInvalid;
        }

        cuMediaFormat.encodingName = pCuiMediaDescItem->GetMediaFormat().GetEncodingName();
        cuMediaFormat.manufacture = pCuiMediaDescItem->GetMediaFormat().GetManufacture();
        cuMediaFormat.payloadType = pCuiMediaDescItem->GetMediaFormat().GetPayloadType();
		string strVideoResolution;
		pCuiMediaDescItem->GetMediaFormat().GetFormatParam(VIDEO_ENC_PARAM_RESOLUTION, strVideoResolution);
		GetValueWithoutDescFromFormatParam(strVideoResolution, cuMediaFormat.videoWidth, cuMediaFormat.videoHeight);
		pCuiMediaDescItem->GetMediaFormat().GetFormatParam(VIDEO_ENC_KEY_FRAME_INTVL, cuMediaFormat.keyframeInterval);
		pCuiMediaDescItem->GetMediaFormat().GetFormatParam(VIDEO_ENC_FRAME_RATE, cuMediaFormat.frameRate);

        cusdkMediaDescription.supportedFormats.push_back(cuMediaFormat);

        //if( LOWER_TRANS_UDP == pCuiMediaDescItem->GetMediaTransProto().GetLowerTransport())
        //{
            cusdkMediaDescription.transMode = cusdk__TransModeUDP;
        //}
        //else if(LOWER_TRANS_TCP == pCuiMediaDescItem->GetMediaTransProto().GetLowerTransport() )
        //{
        //    cusdkMediaDescription.transMode = cusdk__TransModeTCP;
        //}

        ConvertMediaTransChannelFromCuiToCu(pCuiMediaDescItem->GetMediaTransChnList(), cusdkMediaDescription.transChannelList);



        cMediaDescList.push_back(cusdkMediaDescription);

        pCuiMediaDescItem++;
    }
}

void ConverTempMediaExtInfoCuiToCu(const TExtInfo &tFrom, std::vector<cusdk__MediaExtInfo > &tTo)
 {
     TExtInfo::const_iterator item = tFrom.begin();
     while(item != tFrom.end())
     {
         cusdk__MediaExtInfo cMediaExtInfo;
         cMediaExtInfo.extInfoName = item->first;
         cMediaExtInfo.extInfoValue = item->second;
 
         tTo.push_back(cMediaExtInfo);

         item++;
     }
 }

void ConvertTempMediaDescAndExtInfoCuiToCu(const TMediaDescList &tMediaDescFrom, std::vector<cusdk__MediaDescription > &cMediaDescTo,
    const TExtInfo &tMediaExtFrom, std::vector<cusdk__MediaExtInfo > &cMediaExtTo)
{
    ConverTempMediaDescListCuiToCu(tMediaDescFrom, cMediaDescTo);
    ConverTempMediaExtInfoCuiToCu(tMediaExtFrom, cMediaExtTo);

    TStreamTag tStreamTag;
    TExtInfo::const_iterator it = tMediaExtFrom.find(SDP_EXT_INFO_STREAM_TAG);
    if (it != tMediaExtFrom.end())
    {
        if (!tStreamTag.Parse(it->second))
            return;

        for (std::vector<cusdk__MediaDescription >::iterator itMediaDesc = cMediaDescTo.begin();
            itMediaDesc != cMediaDescTo.end(); ++itMediaDesc)
        {
            for (std::vector< cusdk__MediaFormat >::iterator itMediaFormat = itMediaDesc->supportedFormats.begin();
                 itMediaFormat != itMediaDesc->supportedFormats.end(); ++itMediaFormat)
            {
                itMediaFormat->manufacture = tStreamTag.GetManufacture();
                itMediaFormat->mediaStreamType = tStreamTag.GetMediaStreamType();
            }
        }
    }
}

void ConverTempChannelAliasCuiToCu(const vector<TDeviceCapIndexName>& tDeviceCapIndexNames, std::vector< cusdk__ChannelName >& chnAliasList)
{
    vector<TDeviceCapIndexName>::const_iterator item = tDeviceCapIndexNames.begin();
    while(item != tDeviceCapIndexNames.end())
    {
        cusdk__ChannelName cChannelAlias;
        cChannelAlias.chnId = item->index;
        cChannelAlias.chnName = item->name;
		item->extData.GetLat(cChannelAlias.latitude);
		item->extData.GetLon(cChannelAlias.longitude);
        chnAliasList.push_back(cChannelAlias);

        item++;
    }
}

void ConverTempMapTypeInfoFromCuiToCu(const vector<MapTypeInfo>&  tMapTypeInfos, std::vector< cusdk__MapTypeInfo >& cMapTypeInfos)
{
    vector<MapTypeInfo>::const_iterator pMapTypeInfoItem = tMapTypeInfos.begin();
    while(pMapTypeInfoItem != tMapTypeInfos.end())
    {
        cusdk__MapTypeInfo cMapTypeInfo;
        cMapTypeInfo.accessUrl = pMapTypeInfoItem->GetAccessUrl();
        cMapTypeInfo.mapVersion = pMapTypeInfoItem->GetVersion();
        if( MAP_GOOGLE == pMapTypeInfoItem->GetName())
        {
            cMapTypeInfo.mapType = cusdk__MapTypeGoogle; 
        }
        else if( MAP_JPEG == pMapTypeInfoItem->GetName())
        {
            cMapTypeInfo.mapType = cusdk__MapTypeJpeg; 
        }

		map<string,string>::const_iterator pIter = pMapTypeInfoItem->GetModuleAttrs().begin();
		for (;pIter!=pMapTypeInfoItem->GetModuleAttrs().end();pIter++)
		{
			cusdk__MapModuleAttrs cMapModuleAttrs;
			cMapModuleAttrs.key = pIter->first;
			cMapModuleAttrs.value = pIter->second;
			cMapTypeInfo.moduleAttrs.push_back(cMapModuleAttrs);
		}
	    cMapTypeInfos.push_back(cMapTypeInfo);
        pMapTypeInfoItem++;
    }
}

void ConverTempMapPositionFromCuiToCu(const vector<MapPosition>&  tMapPostions, std::vector<cusdk__MapPosition>& cMapPostions)
{
    vector<MapPosition>::const_iterator pMapPostionItem = tMapPostions.begin();
    while(pMapPostionItem != tMapPostions.end())
    {
        cusdk__MapPosition cMapPosition;
        cMapPosition.longitude = pMapPostionItem->GetLongitude();
        cMapPosition.latitude = pMapPostionItem->GetLatitude();

        cMapPostions.push_back(cMapPosition);

        pMapPostionItem++;
    }
}

void ConverTempMapAreaFromCuToCui(const std::vector< cusdk__MapArea >& cMapAreas, vector<MapArea>& tMapAreas)
{
    vector<cusdk__MapArea>::const_iterator pMapAreaItem = cMapAreas.begin();
    while(pMapAreaItem != cMapAreas.end())
    {
        MapArea tMapArea;
        tMapArea.SetOwnId(pMapAreaItem->parentMapId);
        tMapArea.SetCustomStyle(pMapAreaItem->customStyle);
        tMapArea.SetLayerId(pMapAreaItem->layerId);
        tMapArea.SetOpacity(pMapAreaItem->opacity);
        tMapArea.SetUuid(pMapAreaItem->areaId);
        tMapArea.SetRefMapId(pMapAreaItem->refMapId);
        tMapArea.SetColor(pMapAreaItem->layerColor);

        ConverTempMapPositionCuToCui(pMapAreaItem->trace, tMapArea.GetTrace());

        tMapAreas.push_back(tMapArea);

        pMapAreaItem++;
    }
}

void ConverTempMapAreaFromCuiToCu(const vector<MapArea>& tMapAreas, std::vector< cusdk__MapArea >& cMapAreas)
{
    vector<MapArea>::const_iterator pMapAreaItem = tMapAreas.begin();
    while(pMapAreaItem != tMapAreas.end())
    {
        cusdk__MapArea tMapArea;
        tMapArea.areaId = pMapAreaItem->GetUuid();
        tMapArea.customStyle = pMapAreaItem->GetCustomStyle();
        tMapArea.layerId = pMapAreaItem->GetLayerId();
        tMapArea.opacity = pMapAreaItem->GetOpacity();
        tMapArea.parentMapId = pMapAreaItem->GetOwnId();
        tMapArea.refMapId = pMapAreaItem->GetRefMapId();
        tMapArea.layerColor = pMapAreaItem->GetColor();
        
        ConverTempMapPositionFromCuiToCu(pMapAreaItem->GetTrace(), tMapArea.trace);

        cMapAreas.push_back(tMapArea);

        pMapAreaItem++;
    }
}

void ConverTempPuRecInfoFromCuiToCu(const CPuRecInfoList& tFrom, std::vector< cusdk__RecDuration >& tTo)
{
    CPuRecInfoList::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {        
        cusdk__RecDuration cRecDuration;
        cRecDuration.recordDurationId = item->GetRecToken().ToString();

        CCMSTime cStartTime(item->GetTimeRange().GetStartTime());
        CCMSTime cEndTime(item->GetTimeRange().GetEndTime());
        cRecDuration.durationTimeRange.endTime   = cEndTime.GetTime();
        cRecDuration.durationTimeRange.startTime = cStartTime.GetTime(); 

        cusdk__RecTimeRange cRecTimeRange;
        cRecTimeRange.timeRange.startTime = cStartTime.GetTime();
        cRecTimeRange.timeRange.endTime   = cEndTime.GetTime();

        if(item->GetRecToken().GetRecType() == PU_EVENT_TYPE_ALL)
        {
            cRecTimeRange.recEventType = cusdk__RS_RECTYPE_ALL;
        }
        else if(item->GetRecToken().GetRecType() == PU_EVENT_TYPE_MANUAL)
        {
            cRecTimeRange.recEventType = cusdk__RS_RECTYPE_HANDLE;
        }
        else if(item->GetRecToken().GetRecType() == PU_EVENT_TYPE_PIN_ALARM ||
            item->GetRecToken().GetRecType() == PU_EVENT_TYPE_MOTION_DETECT_ALARM)
        {
            cRecTimeRange.recEventType = cusdk__RS_RECTYPE_ALARM;
        }
        else if(item->GetRecToken().GetRecType() ==  PU_EVENT_TYPE_TIMELY ||
            item->GetRecToken().GetRecType() ==  PU_EVENT_TYPE_PERIODLY)
        {
            cRecTimeRange.recEventType = cusdk__RS_RECTYPE_PLAN;
        }
        else if(item->GetRecToken().GetRecType().empty())
        {
            cRecTimeRange.recEventType = cusdk__RS_RECTYPE_ALL;
        }

        cRecDuration.subTimeRangeList.push_back(cRecTimeRange);

        tTo.push_back(cRecDuration);

        item++;
    }
}

void ConverTempVideosourcePresetcfgListCuiToCu(const vector<TVideosourcePresetCfgInfo>& tFrom, std::vector<cusdk__PresetName >& tTo)
{
    vector<TVideosourcePresetCfgInfo>::const_iterator item = tFrom.begin();
    while(item != tFrom.end())
    {        

        vector<TPresetPositionName>::const_iterator posNameItem = item->posNames.begin();
        while(posNameItem != item->posNames.end())
        {
            cusdk__PresetName cPresetName;
            cPresetName.presetId = posNameItem->posId;
            cPresetName.presetName = posNameItem->posName; 

            tTo.push_back(cPresetName);

            posNameItem++;
        }      

        item++;
    }
}

void ConverCapsDevListCuiToCu(const CWechatDeviceList& tFrom, vector<cusdk__CAPSDevice>& tTo)
{
	CWechatDeviceList::const_iterator item = tFrom.begin();
	while(item != tFrom.end())
	{
		cusdk__CAPSDevice cCAPSDevice;

		cCAPSDevice.devId=item->GetDevUri();
		cCAPSDevice.chnId=item->GetChnNO();
		cCAPSDevice.name=item->GetAlias();
		cCAPSDevice.devGBId=item->GetGBId();
		cCAPSDevice.domainGBId=item->GetGBDomainId();

		tTo.push_back(cCAPSDevice);
		item++;
	}
}

void ConverCapsDevListCuToCui(const vector<cusdk__CAPSDevice>& tFrom, CWechatDeviceList& tTo)
{
	vector<cusdk__CAPSDevice>::const_iterator item = tFrom.begin();
	while(item != tFrom.end())
	{        
		CWechatDevice cDevice;
		cDevice.SetDevUri(item->devId);
		cDevice.SetChnNO(item->chnId);
		cDevice.SetAlias(item->name);
		cDevice.SetGBId(item->devGBId);
		cDevice.SetGBDomainId(item->domainGBId);

		tTo.push_back(cDevice);
		item++;
	}
}

void ConvertDevListCuiToCu(const TDeviceVideoSrcIdList& tFrom,cusdk__GetOfflineDeviceStatRsp& tTo )// vector<cusdk__DeviceVideosrcId >& tOffline,vector<cusdk__DeviceVideosrcId >& tIdle)
{
	TDeviceVideoSrcIdList::const_iterator item = tFrom.begin();
	while(item != tFrom.end())
	{
		cusdk__DeviceVideosrcId cDevVidSrcDevice;

		cDevVidSrcDevice.deviceId=item->GetDeviceId();
		cDevVidSrcDevice.videoSrcId=item->GetVideoSrcId();
		cDevVidSrcDevice.deviceOnline=item->GetDeviceOnline();

		if (CHNSTATUS_OFFLINE == item->GetChnStat())
		{
			tTo.offlineDeviceVideosrcList.push_back(cDevVidSrcDevice);
		}
		else if (CHNSTATUS_IDEL == item->GetChnStat())
		{
			tTo.idleStatVideoSrcList.push_back(cDevVidSrcDevice);
		}
		item++;
	}
}

void ConvertDevTreeInfoFromCUIToCu(const vector<CDevTreeInfo>& tFrom, std::vector<cusdk__CustomTreeViewInfo>& tTo )// vector<cusdk__DeviceVideosrcId >& tOffline,vector<cusdk__DeviceVideosrcId >& tIdle)
{
	vector<CDevTreeInfo>::const_iterator item = tFrom.begin();
	while(item != tFrom.end())
	{
		cusdk__CustomTreeViewInfo customTreeViewInfo;

		customTreeViewInfo.id = item->GetDevtreeId();
		customTreeViewInfo.name = item->GetDevtreeName();
		customTreeViewInfo.dataVersion = item->GetDevtreeVer();

		customTreeViewInfo.canUsedForGBPush = item->GetCanBeUsed4Gb();
		customTreeViewInfo.treeViewType = (int)item->GetType();
		tTo.push_back(customTreeViewInfo);
		item++;
	}
}

void ConvertDevGroupInfoFromCUIToCu(const vector<CDeviceGroupInfo>& tFrom, std::vector<cusdk__DeviceGroupInfo>& tTo )
{
	vector<CDeviceGroupInfo>::const_iterator item = tFrom.begin();
	while(item != tFrom.end())
	{
		cusdk__DeviceGroupInfo deviceGroupInfo;

        ConvertDevGrpInfoBaseFromCuiToCu( &(*item), &deviceGroupInfo );

		tTo.push_back(deviceGroupInfo);
		item++;
	}
}

void ConverSearchDPSResultCuiToCu(const CDPSSearchResultList &tFrom, vector<cusdk__SearchResultItem> &tTo )
{
	CDPSSearchResultList::const_iterator item = tFrom.begin();
	while (item != tFrom.end())
	{
		cusdk__SearchResultItem resultIter;
		resultIter.type = item->GetType();
		resultIter.id = item->Id();
		resultIter.vsId = item->GetVSId();
		resultIter.name = item->Name();
		resultIter.online = item->IsOnline();

		resultIter.domainId = item->DomainId();
		resultIter.manufacturer = item->Manufactor();

		CDeviceGroupPathItemList::const_iterator pathItem = item->DGPath().begin();
		while (pathItem != item->DGPath().end() )
		{
			cusdk__DeviceGroupPathItem dgPathItem;
			dgPathItem.groupId = pathItem->GroupId();
			dgPathItem.groupName = pathItem->GroupName();
			resultIter.fullDeviceGroupPath.push_back(dgPathItem);
			pathItem++;
		}

		CDeviceCapLabelList::const_iterator capLableItem = item->CapList().begin();
		while (capLableItem != item->CapList().end())
		{
			resultIter.deviceCapLabelList.push_back((enum cusdk__EDeviceCapLabel)*capLableItem);
			capLableItem++;
		}
		
		tTo.push_back(resultIter);
		item++;
	}
}

void ConverDevGroupVsNums(const CCuDevGrpVidsrcTotalnumGetReq &cReq, const std::vector<CDevGrpVidSrcNum> &tFrom, std::vector<cusdk__GroupVSNum> &tTo)
{
	std::map<string, string> mapUUIDtoName;

	TSipURI devGroupUri;
	for (size_t nIndex = 0; nIndex < cReq.GetDevGrpIds().size(); nIndex++)
	{
		devGroupUri.SetURIString(cReq.GetDevGrpIds()[nIndex]);
		mapUUIDtoName[devGroupUri.GetUser()] = devGroupUri.GetDomain();
	}

	string strDomainName;
	std::vector<CDevGrpVidSrcNum>::const_iterator iter = tFrom.begin();
	cusdk__GroupVSNum groupVsNum;
	while (iter != tFrom.end())
	{
		strDomainName = mapUUIDtoName[iter->GetDevGrpId()];
		if (iter->GetDevGrpId().empty()||strDomainName.empty())
		{
			continue;
		}

		devGroupUri.SetUser(iter->GetDevGrpId());
		devGroupUri.SetDomain(strDomainName);

		groupVsNum.groupId = devGroupUri.GetURIString();
		groupVsNum.num = iter->GetVidSrcNum();

		tTo.push_back(groupVsNum);

		iter++;
	}
}

void ConverDevVsList(const vector< cusdk__VideoSrcId > &tFrom, vector<CDevVidsrc> &tTo, std::map<string, string> &mapUUIDtoName)
{
	CDevVidsrc devVs;
	TSipURI deviceUri;

	for (size_t nIndex = 0; nIndex < tFrom.size(); nIndex++)
	{
		deviceUri.SetURIString(tFrom[nIndex].deviceId);
		mapUUIDtoName[deviceUri.GetUser()] = deviceUri.GetDomain();

		devVs.SetDevId(tFrom[nIndex].deviceId);
		devVs.SetVidSrcIdx(tFrom[nIndex].vsId);
		tTo.push_back(devVs);
	}
}

void ConverDevVsOfflineInfo(const std::map<string, string> &mapUUIDtoName, const vector<CDevVidSrcInfo> &tFrom, vector< cusdk__VSOfflineInfo > &tTo)
{
	cusdk__VSOfflineInfo vsInfo;

	string devUUid;

	for (size_t nIndex = 0; nIndex < tFrom.size(); nIndex++)
	{
		devUUid = tFrom[nIndex].GetDevId();
		if (devUUid.empty())
		{
			continue;
		}

		std::map<string, string>::const_iterator iter = mapUUIDtoName.find(devUUid);
		if (iter == mapUUIDtoName.end())
		{
			continue;
		}

		vsInfo.videosrcId.deviceId = devUUid + "@" + iter->second;

		vsInfo.videosrcId.vsId = tFrom[nIndex].GetVidsrcIdx();

		if (!tFrom[nIndex].GetExist())
		{
			tTo.push_back(vsInfo);
			continue;
		}

		vsInfo.groupId = tFrom[nIndex].GetDevGrpId();
		vsInfo.devName = tFrom[nIndex].GetDevName();
		vsInfo.name = tFrom[nIndex].GetVidAlias();
		vsInfo.deviceIP = tFrom[nIndex].GetIpAddress();

		tFrom[nIndex].GetExtData().GetLat(vsInfo.latitude);
		tFrom[nIndex].GetExtData().GetLon(vsInfo.longitude);

		tTo.push_back(vsInfo);
	}
}

void ConvertDevicePushErrorInfoFromCUIToCu(const TDevGbidErrItem &tFrom, cusdk__DevicePushErrorInfo &tTo)
{
	tTo.errorType = tFrom.gbidErrType;
	tTo.deviceGroupId = tFrom.devGrpId;
	tTo.indexInDeviceGroup = tFrom.vidSrcGrpIdx;

	tTo.videoSrc.videoSrcId = tFrom.vidSrcId;
	tTo.videoSrc.deviceId = tFrom.devId;
	tTo.videoSrc.deviceName = tFrom.devName;
	tTo.videoSrc.videoSrcName = tFrom.vidSrcName;

	tTo.videoSrc.primaryOriginalGBId = tFrom.primaryOldGbid;
	tTo.videoSrc.secondOriginalGBId = tFrom.secondaryOldGbid;
	tTo.videoSrc.primaryNewGBId = tFrom.primaryNewGbid;
	tTo.videoSrc.secondNewGBId = tFrom.secondaryNewGbid;
	tTo.errIndex = tFrom.errIndex;

	ConvertDevGroupInfoFromCUIToCu(tFrom.devGrpFullGbinfos, tTo.videoSrc.deviceGroupPath);
}

void ConvertDeviceFullGbInfoListFromCuiToCu(const std::vector<CDeviceFullGbInfo> &tFrom, std::vector<cusdk__CVideoSrcInfoGBInGroup> &tTo)
{
	for (size_t nIndex = 0; nIndex < tFrom.size(); nIndex++)
	{
		cusdk__CVideoSrcInfoGBInGroup devGroupInfo;
		devGroupInfo.deviceId = tFrom[nIndex].GetDevId();
		devGroupInfo.videoSrcId = tFrom[nIndex].GetVidIdx();
		devGroupInfo.deviceName = tFrom[nIndex].GetDevName();
		devGroupInfo.videoSrcName = tFrom[nIndex].GetVidSrcName();

		devGroupInfo.primaryOriginalGBId = tFrom[nIndex].GetOldPriGbid();
		devGroupInfo.secondOriginalGBId = tFrom[nIndex].GetOldSecGbid();
		devGroupInfo.primaryNewGBId = tFrom[nIndex].GetNewPriGbid();
		devGroupInfo.secondNewGBId = tFrom[nIndex].GetNewSecGbid();

		ConvertDevGroupInfoFromCUIToCu(tFrom[nIndex].GetDevGrpInfos(), devGroupInfo.deviceGroupPath);
		
		tTo.push_back(devGroupInfo);
	}
}

void ConvertDevGrpFullGbInfoListFromCuiToCu(const std::vector<TDevGrpFullGbInfos> &tFrom, std::vector<cusdk__SearchGBDeviceGroupInfo> &tTo)
{
	for (size_t nIndex = 0; nIndex < tFrom.size(); nIndex++)
	{
		cusdk__SearchGBDeviceGroupInfo gbDevGrpInfo;
		
		ConvertDevGroupInfoFromCUIToCu(tFrom[nIndex], gbDevGrpInfo.groupInfo);

		tTo.push_back(gbDevGrpInfo);
	}
}

void ConvertTPuVidSrcOsdCfgFromCuToCui(const std::vector<cusdk__PuVidSrcOSDItem> &tFrom, std::vector<TPuVidSrcOsdCfgItem> &to)
{
	for (size_t nIndex = 0; nIndex < tFrom.size(); nIndex++)
	{
		TPuVidSrcOsdCfgItem tPuVidSrcOsdCfgItem;

		tPuVidSrcOsdCfgItem.SetPuVidSrcOsdId(tFrom[nIndex].id);
		tPuVidSrcOsdCfgItem.SetPuVidSrcOsdType(tFrom[nIndex].type);
		tPuVidSrcOsdCfgItem.SetPuVidSrcOsdEnable(tFrom[nIndex].enabled);
		tPuVidSrcOsdCfgItem.SetPuVidSrcOsdContent(tFrom[nIndex].content);

		to.push_back(tPuVidSrcOsdCfgItem);
	}
}

void ConvertTPuVidSrcOsdCfgFromCuiToCu(const std::vector<TPuVidSrcOsdCfgItem> &tFrom, std::vector<cusdk__PuVidSrcOSDItem> &to)
{
	for (size_t nIndex = 0; nIndex < tFrom.size(); nIndex++)
	{
		cusdk__PuVidSrcOSDItem tPuVidSrcOSDItem;

		tPuVidSrcOSDItem.id = tFrom[nIndex].GetPuVidSrcOsdId();
		tPuVidSrcOSDItem.type = tFrom[nIndex].GetPuVidSrcOsdType();
		tPuVidSrcOSDItem.enabled = tFrom[nIndex].GetPuVidSrcOsdEnable();
		tPuVidSrcOSDItem.content = tFrom[nIndex].GetPuVidSrcOsdContent();
		
		to.push_back(tPuVidSrcOSDItem);
	}
}


void ConvertCGetNotifyRspTocusdk__GetNotifyRsp(const CGetNotifyRsp &cIn, cusdk__GetNotifyRsp &cOut)
{
	cOut.errorCode = cIn.GetErrorCode();

	const CCuNotifyPool &cNotifyPool = cIn.notifyPool;

	if (!cNotifyPool.deviceAddNotify.empty())
	{
		std::list< CDeviceInfo >::const_iterator pItem = cNotifyPool.deviceAddNotify.begin();
		while (pItem != cNotifyPool.deviceAddNotify.end())
		{
			cusdk__DeviceInfo tDevInfo;
			cusdk__DeviceFullInfo tDevFullInfo;
			ConvertDeviceBaseInfoFromCuiToCu(*pItem, tDevFullInfo);

			cOut.deviceAddNotify.push_back(tDevFullInfo);
			pItem++;
		}
	}

	//作为遗留问题后续解决 hzytodo2 设备的add, mod, del通知 
	if (!cNotifyPool.deviceDelNotify.empty())
	{
		cusdk__AssignDeviceInfo assignDeviceInfo;
		for (std::list<string>::const_iterator iter = cNotifyPool.deviceDelNotify.begin();
		iter != cNotifyPool.deviceDelNotify.end(); iter++)
		{
			if (!iter->empty())
			{
				assignDeviceInfo.devId = *iter;
				cOut.deviceDelNotify.push_back(assignDeviceInfo);
			}
		}
	}

	//视频源名称更新
	if (cNotifyPool.CapChangeNotify.size() > 0)
	{
		std::list< CDeviceCapIndexNameModNtfReq>::const_iterator pItem = cNotifyPool.CapChangeNotify.begin();
		while (pItem != cNotifyPool.CapChangeNotify.end())
		{
			std::vector< cusdk__DeviceChannelNameChangeNotify>  cCusdkNotify;
			const vector<TDeviceCapIndexName> &TDevCapNameList = pItem->GetDeviceCapName().deviceCapIndexNames;

			switch (pItem->GetDeviceCapName().capId)
			{
			case DEVICE_CAP_VIDEOSOURCE:
				{
					vector<TDeviceCapIndexName>::const_iterator pDevCapIndexName = TDevCapNameList.begin();
					while (pDevCapIndexName != TDevCapNameList.end())
					{
						cusdk__DeviceChannelNameChangeNotify CCusdkDeviceChannelNameChangeNotify;
						CCusdkDeviceChannelNameChangeNotify.deviceId = pItem->GetDeviceCapName().deviceId;
						CCusdkDeviceChannelNameChangeNotify.chnId = pDevCapIndexName->index;
						CCusdkDeviceChannelNameChangeNotify.chnName = pDevCapIndexName->name;
						pDevCapIndexName->extData.GetLat(CCusdkDeviceChannelNameChangeNotify.latitude);
						pDevCapIndexName->extData.GetLon(CCusdkDeviceChannelNameChangeNotify.longitude);
						CCusdkDeviceChannelNameChangeNotify.chnType = cusdk__DevChnTypeVideoSource;
						cOut.deviceChannelNameChangeNotify.push_back(CCusdkDeviceChannelNameChangeNotify);

						pDevCapIndexName++;
					}
				}
				break;
			case DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT:
				{
					vector<TDeviceCapIndexName>::const_iterator pDevCapIndexName = TDevCapNameList.begin();
					while (pDevCapIndexName != TDevCapNameList.end())
					{
						cusdk__DeviceChannelNameChangeNotify CCusdkDeviceChannelNameChangeNotify;
						CCusdkDeviceChannelNameChangeNotify.deviceId = pItem->GetDeviceCapName().deviceId;
						CCusdkDeviceChannelNameChangeNotify.chnId = pDevCapIndexName->index;
						CCusdkDeviceChannelNameChangeNotify.chnName = pDevCapIndexName->name;
						CCusdkDeviceChannelNameChangeNotify.chnType = cusdk__DevChnTypeAlarmInput;
						cOut.deviceChannelNameChangeNotify.push_back(CCusdkDeviceChannelNameChangeNotify);

						pDevCapIndexName++;
					}
				}
				break;
			default:
				{

				}
				break;
			}

			pItem++;
		}
	}

	//add cusdk__DeviceGroup
	if (cNotifyPool.deviceGroupAddNotify.size() > 0)
	{
		std::list< CDeviceGroupAddNtfReq >::const_iterator pItem = cNotifyPool.deviceGroupAddNotify.begin();
		while (pItem != cNotifyPool.deviceGroupAddNotify.end())
		{
			cusdk__DeviceGroupInfo cCusdkDevGroupInfo;

            ConvertDevGrpInfoBaseFromCuiToCu( &(pItem->GetDeviceGroupInfo()), &cCusdkDevGroupInfo );
            
			cOut.deviceGroupAddNotify.push_back(cCusdkDevGroupInfo);

			pItem++;
		}
	}

	//deviceGroupDelNotify
	ConvertListToVector(cNotifyPool.deviceGroupDelNotify, cOut.deviceGroupDelNotify);

	//deviceGroupModNotify
	if (cNotifyPool.deviceGroupModNotify.size() > 0)
	{
		std::list< CDeviceGroupModNtfReq >::const_iterator pItem = cNotifyPool.deviceGroupModNotify.begin();
		while (pItem != cNotifyPool.deviceGroupModNotify.end())
		{
			cusdk__DeviceGroupInfo cCusdkDevGroupInfo;

            ConvertDevGrpInfoBaseFromCuiToCu( &(pItem->GetDeviceGroupInfo()), &cCusdkDevGroupInfo );

			cOut.deviceGroupModNotify.push_back(cCusdkDevGroupInfo);

			pItem++;
		}
	}

	//devOnlineNotify
	if (cNotifyPool.devOnlineNotify.size() > 0)
	{
		std::list< CPuStatusNtyReq >::const_iterator pItem = cNotifyPool.devOnlineNotify.begin();
		while (pItem != cNotifyPool.devOnlineNotify.end())
		{
			cusdk__DevOnlineNotify cCusdkDevOnline;
			cCusdkDevOnline.deviceId = pItem->GetDevURI();
			cCusdkDevOnline.online = pItem->GetOnOffStatus().GetIsOnline();

			cOut.devOnlineNotify.push_back(cCusdkDevOnline);
			pItem++;
		}
	}

	//devConfigNotify
	if (cNotifyPool.devConfigNotify.size() > 0)
	{
		std::list< CPuConfigNtyReq >::const_iterator pItem = cNotifyPool.devConfigNotify.begin();
		while (pItem != cNotifyPool.devConfigNotify.end())
		{
			cusdk__DevConfigNotify cCusdkDevConfig;

			TPuConfig tPuConfig;
			tPuConfig.MergeDevConfig(pItem->GetConfig());

			//devURI
			cCusdkDevConfig.deviceId = tPuConfig.GetDevUri();
			cCusdkDevConfig.devIp = tPuConfig.GetDevIp();
			cCusdkDevConfig.devType = tPuConfig.GetDevType();
			cCusdkDevConfig.devMode = tPuConfig.GetDevMode();

			//baseCapSet
			cCusdkDevConfig.baseCapSet.videoEncNum = tPuConfig.GetBaseCapSet().GetVidEncNum();
			cCusdkDevConfig.baseCapSet.videoDecNum = tPuConfig.GetBaseCapSet().GetVidDecNum();
			cCusdkDevConfig.baseCapSet.audioEncNum = tPuConfig.GetBaseCapSet().GetAudEncNum();
			cCusdkDevConfig.baseCapSet.audioDecNum = tPuConfig.GetBaseCapSet().GetAudDecNum();
			cCusdkDevConfig.baseCapSet.serialPortNum = tPuConfig.GetBaseCapSet().GetSerialPortNum();
			cCusdkDevConfig.baseCapSet.alarmInputChnNum = tPuConfig.GetBaseCapSet().GetAlarmInNum();
			cCusdkDevConfig.baseCapSet.alarmOutputChnNum = tPuConfig.GetBaseCapSet().GetAlarmOutNum();
			cCusdkDevConfig.baseCapSet.videoInPortNum = tPuConfig.GetBaseCapSet().GetVideoInPortNum();

			//extCapSet
			ConvertMapToVector(tPuConfig.GetExtCapSet(), cCusdkDevConfig.extCapSet);

			//inputPinEnable
			if (!tPuConfig.GetInputPinEnable().empty())
			{
				for (u32 nIndex = 0; nIndex < tPuConfig.GetInputPinEnable().size(); nIndex++)
				{
					cusdk__AlarmInputChnEnable tItem;
					tItem.id = nIndex;
					tItem.value = tPuConfig.GetInputPinEnable().at(nIndex) == '0' ? false : true;
					cCusdkDevConfig.alarmInputChnEnable.push_back(tItem);
				}
			}
			//ConvertMapToVector(tPuConfig.GetInputPinEnable(), cCusdkDevConfig.inputPinEnable);

			//chnVidFormat
			ConvertMapToVector(tPuConfig.GetCurrVidFormat(), cCusdkDevConfig.chnVidFormat);

			//chnVidRes
			ConvertPuCfgToEncVidRes((tPuConfig.GetExtCapSet().find(VSIP_ENTITY_SUPPORT_RESOLUTION_WH) == tPuConfig.GetExtCapSet().end())
				, pItem->GetConfig().GetChnCfgList(), cCusdkDevConfig.chnVideoRes);

			//chnVidDynValue
			ConvertMapToVector(tPuConfig.GetVidDynValue(), cCusdkDevConfig.chnVidDynValue);

			//vidSrcChnSet
			ConvertMapToVector(tPuConfig.GetVidSrcChnSet(), cCusdkDevConfig.vidSrcChnSet);

			//chnVidSrcSet
			ConvertMapToVector(tPuConfig.GetChnVidSrcSet(), cCusdkDevConfig.chnVidSrcSet);

			//camType
			ConvertCameraTypeFromCuiToCu(tPuConfig.GetCamType(), cCusdkDevConfig.camType);


			//camId
			ConvertMapToVector(tPuConfig.GetCamId(), cCusdkDevConfig.camId);

			//bitRate
			ConvertMapToVector(tPuConfig.GetBitRate(), cCusdkDevConfig.bitRate);

			cusdk__EncoderChnStatus tChnStatus;
			for (map<TChannelID, TPuChnStateConfig>::const_iterator it = tPuConfig.GetChnStateSet().begin();
			it != tPuConfig.GetChnStateSet().end(); ++it)
			{
				tChnStatus.id = it->first;
				tChnStatus.status = it->second.GetChnState();
				cCusdkDevConfig.chnStatus.push_back(tChnStatus);
			}

			//freeLicenceNum
			cCusdkDevConfig.freeLicenceNum = tPuConfig.GetFreeLicenceNum();
			cCusdkDevConfig.directTransNum = tPuConfig.GetDirectTransNum();
			cCusdkDevConfig.isDecCombined = tPuConfig.GetIsDecCombined();

			cOut.devConfigNotify.push_back(cCusdkDevConfig);
			pItem++;
		}
	}

	//devServiceStatusNotify
	if (cNotifyPool.devServiceStatusNotify.size() > 0)
	{
		std::list< CPuServiceStatusNtyReq >::const_iterator pItem = cNotifyPool.devServiceStatusNotify.begin();
		while (pItem != cNotifyPool.devServiceStatusNotify.end())
		{
			cusdk__DevServiceStatusNotify cCusdkDevServiceStatus;
			const TPuServiceStatus &tPuServiceStatus = pItem->GetServiceStatus();

			//devURI
			cCusdkDevServiceStatus.deviceId = pItem->GetDevURI();

			//decChnVidSrc
			//ConvertMapToVector(tPuServiceStatus.GetDecChnVidSrc(), cCusdkDevServiceStatus.decChnVidSrc);

			//audioCallSrc
			ConvertMapToVector(tPuServiceStatus.GetAudioCallSrc(), cCusdkDevServiceStatus.audioCallSrc);

			//isPlatPreRec
			ConvertMapToVector(tPuServiceStatus.GetIsPlatPreRec(), cCusdkDevServiceStatus.isPlatPreRec);

			//platRec
			ConvertPlatRecMapToVector(tPuServiceStatus.GetPlatRec(), cCusdkDevServiceStatus.platRecStatus);

			//isPuPreRec
			ConvertMapToVector(tPuServiceStatus.GetIsPuPreRec(), cCusdkDevServiceStatus.isPuPreRec);

			//isPuRec
			ConvertMapToVector(tPuServiceStatus.GetIsPuRec(), cCusdkDevServiceStatus.isPuRec);

			//decPoll
			//ConvertMapToVector(tPuServiceStatus.GetDecPoll(), cCusdkDevServiceStatus.decPoll);

			cOut.devServiceStatusNotify.push_back(cCusdkDevServiceStatus);
			pItem++;
		}
	}

	//devAlarmNotify
	if (cNotifyPool.devAlarmNotify.size() > 0)
	{
		std::list< CPuAlarmStatusNtyReq >::const_iterator pItem = cNotifyPool.devAlarmNotify.begin();
		while (pItem != cNotifyPool.devAlarmNotify.end())
		{
			const TPuAlarmStatus &tPuAlarmStatus = pItem->GetAlarmStatus();

			if (tPuAlarmStatus.GetAlarmArray().size() != 0)
			{
                ConvertAlarmStatusFromCuiToCu(tPuAlarmStatus, cOut.devAlarmNotify);
			}

			pItem++;
		}
	}
	if (!cNotifyPool.devGpsNotify.empty())
	{
		cusdk__GPSInfo cNtf;
		for (std::list< CPuGpsNtyReq >::const_iterator it = cNotifyPool.devGpsNotify.begin();
		it != cNotifyPool.devGpsNotify.end(); ++it)
		{
			CCMSTime cTime(it->GetGpsStatusExt().GetTimeStamp());
			cNtf.revTime        = cTime.GetTime();
			cNtf.deviceId       = it->GetDevURI();
			cNtf.longitude      = it->GetGpsStatusExt().GetRawLongitude();
			cNtf.latitude       = it->GetGpsStatusExt().GetRawLatitude();
			cNtf.marLongitude   = it->GetGpsStatusExt().GetLongitude();
			cNtf.marLatitude    = it->GetGpsStatusExt().GetLatitude();
			cNtf.speed          = it->GetGpsStatusExt().GetSpeed();
            cNtf.angle          = it->GetGpsStatusExt().GetDirection();
            cNtf.height         = it->GetGpsStatusExt().GetAltitude();
			cOut.gpsNotify.push_back(cNtf);
		}
	}
	if (cNotifyPool.devTransdataNotify.size() > 0)
	{
		cusdk__TransDataNotify cNtf;
		for (std::list< CPuTransDataStatusNtyReq >::const_iterator it = cNotifyPool.devTransdataNotify.begin();
		it != cNotifyPool.devTransdataNotify.end(); ++it)
		{
			cNtf.deviceId = it->GetDevURI();
			if (it->GetData().GetType() == PU_TRANS_DATA_TYPE_COMDATA)
			{
				cNtf.transChnType = cusdk__TransChnCOMPort;
			}
			else if (it->GetData().GetType().empty())
			{
				CMS_ASSERT(false);
				cNtf.transChnType = cusdk__TransChnCOMPort;
			}
			else
			{
				cNtf.transChnType = cusdk__TransChnUserDef;
			}
			cNtf.transDataBase64 = it->GetData().GetData();

			cOut.transDataNotify.push_back(cNtf);
		}
	}

	//inviteStopNotify
	if (cNotifyPool.inviteStopNotify.size() > 0)
	{
		std::list< CInviteStopNotify >::const_iterator pStopNotify = cNotifyPool.inviteStopNotify.begin();
		while (pStopNotify != cNotifyPool.inviteStopNotify.end())
		{
			cusdk__StopPlayNotify cStopPlayNotify;
			cStopPlayNotify.playId = pStopNotify->playId;
			cStopPlayNotify.errorCode = pStopNotify->errorCode;

			cOut.stopPlayNotify.push_back(cStopPlayNotify);
			pStopNotify++;
		}
	}

	//tvWallStatusNotify
	if (cNotifyPool.tvWallStatusNotify.size() > 0)
	{
		std::list< CTvWallStatusNtyReq >::const_iterator pTvWallStatusNtf = cNotifyPool.tvWallStatusNotify.begin();
		while (pTvWallStatusNtf != cNotifyPool.tvWallStatusNotify.end())
		{
			cusdk__TVWallStateNotify cTvWallState;
			cTvWallState.tvWallId = pTvWallStatusNtf->GetDevURI();

			TTvWallState tTvWallState;
			string strTvWallState = pTvWallStatusNtf->GetState();
			strTvWallState = Base64Decode(strTvWallState);
			u8 abyBuff[512 * 1024] = { 0 };
			uLong dwBuffLen = sizeof(abyBuff);
			if (Z_OK != uncompress(abyBuff, &dwBuffLen, (u8*)strTvWallState.c_str(), strTvWallState.length()))
			{
				pTvWallStatusNtf++;
				continue;
			}
			string strState((char*)abyBuff, dwBuffLen);
			tTvWallState.FromXml(strState);

			if (SCHEME_POLL_START == tTvWallState.GetSchemePollState())
			{
				cTvWallState.schemePollState = cusdk__PollStarted;
			}
			else if (SCHEME_POLL_PAUSE == tTvWallState.GetSchemePollState())
			{
				cTvWallState.schemePollState = cusdk__PollPaused;
			}
			else if (SCHEME_POLL_STOP == tTvWallState.GetSchemePollState())
			{
				cTvWallState.schemePollState = cusdk__PollStoped;
			}

			cTvWallState.curSchemeInfo.schemeName = tTvWallState.GetCurScheme().GetSchemeName();
			cTvWallState.curSchemeInfo.tvWallId = tTvWallState.GetCurScheme().GetTvWallId();

			ConverTempTvWinFromCuiToCu(tTvWallState.GetCurScheme().GetTvWinList(), cTvWallState.curSchemeInfo.tvSchemeList);
			ConverTempTvWinStateFromCuiToCu(tTvWallState.GetWinStateList(), cTvWallState.tvStateList);
			ConverTempTvWallSchemePollCuiToCu(tTvWallState.GetSchemeGroup().GetPollList(), cTvWallState.pollSteps);

			cOut.tvWallStateNotify.push_back(cTvWallState);

			pTvWallStatusNtf++;
		}
	}

	//userAddNotify
	if (cNotifyPool.userAddNotify.size() > 0)
	{
		std::list< CCuUserInfo >::const_iterator pUserAddNtf = cNotifyPool.userAddNotify.begin();
		while (pUserAddNtf != cNotifyPool.userAddNotify.end())
		{
			cusdk__UserInfo cUserInfo;
			ConvertUserInfoFromCuiToCu(*pUserAddNtf, cUserInfo);

			cOut.userAddNotify.push_back(cUserInfo);
			pUserAddNtf++;
		}
	}


	//userModNotify;
	if (cNotifyPool.userModNotify.size() > 0)
	{
		std::list < CCuUserInfo>::const_iterator pUserModNtf = cNotifyPool.userModNotify.begin();
		while (pUserModNtf != cNotifyPool.userModNotify.end())
		{
			cusdk__UserInfo cUserInfo;
			ConvertUserInfoFromCuiToCu(*pUserModNtf, cUserInfo);

			cOut.userModNotify.push_back(cUserInfo);
			pUserModNtf++;
		}
	}

	//userDelNotify;
	if (cNotifyPool.userDelNotify.size() > 0)
	{
		std::list <string>::const_iterator pUserDelNtf = cNotifyPool.userDelNotify.begin();
		while (pUserDelNtf != cNotifyPool.userDelNotify.end())
		{
			cOut.userDelNotify.push_back(*pUserDelNtf);

			pUserDelNtf++;
		}
	}

	//tvwalladd
	if (cNotifyPool.tvWallAddNotify.size() > 0)
	{
		std::list <CTvwallAddNtfReq>::const_iterator pTvWallAddNtf = cNotifyPool.tvWallAddNotify.begin();
		while (pTvWallAddNtf != cNotifyPool.tvWallAddNotify.end())
		{
			cusdk__TVWall tvWall;
			tvWall.tvwallId = pTvWallAddNtf->GetTvwallInfo().GetDeviceUUID();
			tvWall.domainId = pTvWallAddNtf->GetTvwallInfo().GetDomainUUID();
			tvWall.name = pTvWallAddNtf->GetTvwallInfo().GetDeviceName();
			tvWall.tvNum = pTvWallAddNtf->GetTvwallInfo().GetTvNum();
			tvWall.tvwallStyle = pTvWallAddNtf->GetTvwallInfo().GetTvwallStyle();
			tvWall.tvwallCustomStyleData = pTvWallAddNtf->GetTvwallInfo().GetTvwallCustomStyleData();
			tvWall.desc = pTvWallAddNtf->GetTvwallInfo().GetDescription();
			CCMSTime createTime(pTvWallAddNtf->GetTvwallInfo().GetCreateDate());
			tvWall.createTime = createTime.GetTime();
			ConverTempTvDecoderBindersCuiToCu(pTvWallAddNtf->GetTvwallInfo().GetTvDecoderBinders(), tvWall.tvDecoderBind);


			cOut.tvWallAddNotify.push_back(tvWall);

			pTvWallAddNtf++;
		}
	}

	//tvwalldel
	if (cNotifyPool.tvWallDelNotify.size() > 0)
	{
		std::list <string>::const_iterator pTvWallDelNtf = cNotifyPool.tvWallDelNotify.begin();
		while (pTvWallDelNtf != cNotifyPool.tvWallDelNotify.end())
		{
			cOut.tvWallDelNotify.push_back(*pTvWallDelNtf);

			pTvWallDelNtf++;
		}
	}

	//tvwallmod
	if (cNotifyPool.tvWallModNotify.size() > 0)
	{
		std::list <CTvwallModNtfReq>::const_iterator pTvWallModNtf = cNotifyPool.tvWallModNotify.begin();
		while (pTvWallModNtf != cNotifyPool.tvWallModNotify.end())
		{
			cusdk__TVWall tvWall;
			tvWall.tvwallId = pTvWallModNtf->GetTvwallInfo().GetDeviceUUID();
			tvWall.domainId = pTvWallModNtf->GetTvwallInfo().GetDomainUUID();
			tvWall.name = pTvWallModNtf->GetTvwallInfo().GetDeviceName();
			tvWall.tvNum = pTvWallModNtf->GetTvwallInfo().GetTvNum();
			tvWall.tvwallStyle = pTvWallModNtf->GetTvwallInfo().GetTvwallStyle();
			tvWall.tvwallCustomStyleData = pTvWallModNtf->GetTvwallInfo().GetTvwallCustomStyleData();
			tvWall.desc = pTvWallModNtf->GetTvwallInfo().GetDescription();
			CCMSTime createTime(pTvWallModNtf->GetTvwallInfo().GetCreateDate());
			tvWall.createTime = createTime.GetTime();
			ConverTempTvDecoderBindersCuiToCu(pTvWallModNtf->GetTvwallInfo().GetTvDecoderBinders(), tvWall.tvDecoderBind);


			cOut.tvWallModNotify.push_back(tvWall);

			pTvWallModNtf++;
		}
	}
	//userGroupAddNotify
	if (cNotifyPool.userGroupAddNotify.size() > 0)
	{
		std::list <CUserGroupAddNtfReq>::const_iterator pUserGroupAddNtf = cNotifyPool.userGroupAddNotify.begin();
		while (pUserGroupAddNtf != cNotifyPool.userGroupAddNotify.end())
		{
			cusdk__UserGroup tUserGroup;
			ConvertUserGroupInfoFromCuiToCu(pUserGroupAddNtf->GetUserGroupInfo(), tUserGroup);
			cOut.userGroupAddNotify.push_back(tUserGroup);

			pUserGroupAddNtf++;
		}
	}

	//userGroupDelNotify
	if (cNotifyPool.userGroupDelNotify.size() > 0)
	{
		std::list< string > ::const_iterator pUserGroupDelNtf = cNotifyPool.userGroupDelNotify.begin();
		while (pUserGroupDelNtf != cNotifyPool.userGroupDelNotify.end())
		{
			cOut.userGroupDelNotify.push_back(*pUserGroupDelNtf);

			pUserGroupDelNtf++;
		}
	}

	//userGroupModNotify
	if (cNotifyPool.userGroupModNotify.size() > 0)
	{
		std::list< CUserGroupModNtfReq > ::const_iterator pUserGroupModNtf = cNotifyPool.userGroupModNotify.begin();
		while (pUserGroupModNtf != cNotifyPool.userGroupModNotify.end())
		{
			cusdk__UserGroup tUserGroup;
			ConvertUserGroupInfoFromCuiToCu(pUserGroupModNtf->GetUserGroupInfo(), tUserGroup);
			cOut.userGroupModNotify.push_back(tUserGroup);

			pUserGroupModNtf++;
		}
	}

	//deviceGroupAddDeviceNotify
	if (cNotifyPool.devGroupDevAddNotify.size() > 0)
	{
		std::list <CDeviceGroupDeviceAddNtfReq>::const_iterator pDevGroupDevAddNtf = cNotifyPool.devGroupDevAddNotify.begin();
		while (pDevGroupDevAddNtf != cNotifyPool.devGroupDevAddNotify.end())
		{
			cusdk__DeviceGroupRelation tDevGroupRelation;
			tDevGroupRelation.devId = pDevGroupDevAddNtf->GetDeviceGroupDeviceInfo().GetDeviceID();
			tDevGroupRelation.devGroupId = pDevGroupDevAddNtf->GetDeviceGroupDeviceInfo().GetDeviceGroupID();
			ConvertDeviceCapIndexsFromCuiToCu(pDevGroupDevAddNtf->GetDeviceGroupDeviceInfo().GetDeviceCapIndexs()
				, tDevGroupRelation.videoSrcIdList);

			cOut.deviceGroupAddDeviceNotify.push_back(tDevGroupRelation);

			pDevGroupDevAddNtf++;
		}
	}

	//deviceGroupDelDeviceNotify
	if (cNotifyPool.devGroupDevDelNotify.size() > 0)
	{
		std::list <CDeviceGroupDeviceDelNtfReq>::const_iterator pDevGroupDevDelNtf = cNotifyPool.devGroupDevDelNotify.begin();
		while (pDevGroupDevDelNtf != cNotifyPool.devGroupDevDelNotify.end())
		{
			cusdk__DeviceGroupRelation tDevGroupRelation;
			tDevGroupRelation.devId = pDevGroupDevDelNtf->GetDeviceGroupDeviceInfoKey().GetDeviceID();
			tDevGroupRelation.devGroupId = pDevGroupDevDelNtf->GetDeviceGroupDeviceInfoKey().GetDeviceGroupID();
			ConvertDeviceCapIndexsFromCuiToCu(pDevGroupDevDelNtf->GetDeviceGroupDeviceInfoKey().GetDeviceCapIndexs()
				, tDevGroupRelation.videoSrcIdList);

			cOut.deviceGroupDelDeviceNotify.push_back(tDevGroupRelation);

			pDevGroupDevDelNtf++;
		}
	}

	//CAlarmLinkModNotify
	if (cNotifyPool.alarmLinkModNotify.size() > 0)
	{
		std::list< CAlarmLinkModNtfReq>::const_iterator pAlarmLinkModNtf = cNotifyPool.alarmLinkModNotify.begin();
		while (pAlarmLinkModNtf != cNotifyPool.alarmLinkModNotify.end())
		{
			cusdk__AlarmLinkConfig cAlarmLinkConfig;
			cAlarmLinkConfig.alarmSource.alarmDeviceId = pAlarmLinkModNtf->GetCfg().GetAlarmLinkSrc().GetDevUri();
			cAlarmLinkConfig.alarmSource.alarmInputChnId = pAlarmLinkModNtf->GetCfg().GetAlarmLinkSrc().GetInputId();

            CMS_ALARM_TYPE cmsType = pAlarmLinkModNtf->GetCfg().GetAlarmLinkSrc().GetAlarmType();//ALARM_TYPE_PIN
            const CAlarmTypeCommon *pcAlmUtils = GetCommonType();
            if( pcAlmUtils != NULL )
            {
                //cusdk__AlarmMove,cusdk__AlarmInput
                cAlarmLinkConfig.alarmSource.alarmType = cusdk__EAlarmType( pcAlmUtils->ConvertFromCmsToCu( cmsType ) ); //
            }
            
			cAlarmLinkConfig.enabled = pAlarmLinkModNtf->GetCfg().GetIsUsed();
			ConverTempCronTimeCuiToCu(pAlarmLinkModNtf->GetCfg().GetDefenceSched(), cAlarmLinkConfig.enabledCronTimeList);
			ConverTempAlarmLinkDstCuiToCu(pAlarmLinkModNtf->GetCfg().GetAlarmLinkDstList(), cAlarmLinkConfig.alarmLinkActionList);

			cOut.alarmLinkConfigChangeNotify.push_back(cAlarmLinkConfig);

			pAlarmLinkModNtf++;
		}
	}

	//CExportFailNotify
	if (cNotifyPool.exportFailNotify.size() > 0)
	{
		std::list< CCuExportFailNotify >::const_iterator pExportFailNtf = cNotifyPool.exportFailNotify.begin();
		while (pExportFailNtf != cNotifyPool.exportFailNotify.end())
		{
			cusdk__ExportFail cExportFail;
			cExportFail.errorCode = pExportFailNtf->errorCode;
			cExportFail.exportFileUrl = pExportFailNtf->exportFileUrl;

			cOut.exportFailNotify.push_back(cExportFail);

			pExportFailNtf++;
		}
	}

	//CStunUpdateReq
	if (cNotifyPool.stunUpdateNotify.size() > 0)
	{
		std::list< CStunUpdateReq>::const_iterator pStunUpdateNtf = cNotifyPool.stunUpdateNotify.begin();
		while (pStunUpdateNtf != cNotifyPool.stunUpdateNotify.end())
		{
			cusdk__STUNServerInfo cStunServerInfo;
			cStunServerInfo.ip = pStunUpdateNtf->GetStunAddr().GetNetIp();
			cStunServerInfo.port = pStunUpdateNtf->GetStunAddr().GetNetPort();
			cOut.stunChangedNotify.push_back(cStunServerInfo);

			pStunUpdateNtf++;
		}
	}
	if (!cNotifyPool.serviceStatusNotify.empty())
	{
		std::list< TServiceStatus>::const_iterator it = cNotifyPool.serviceStatusNotify.begin();
		cusdk__ServiceStatus cItem;
		while (it != cNotifyPool.serviceStatusNotify.end())
		{
			cItem.serviceType = static_cast<cusdk__EServiceType>(it->nServiceType);
			cItem.serviceOk = it->bInService;

			cOut.serviceStatusNotify.push_back(cItem);
			++it;
		}
	}

	if (!cNotifyPool.mapTransparentNotify.empty())
	{
		std::list<CCuPuTransparentQryNtfReq>::const_iterator it = cNotifyPool.mapTransparentNotify.begin();
		cusdk__TransDataNotify cItem;
		while (it != cNotifyPool.mapTransparentNotify.end())
		{
			cItem.deviceId = it->GetDevChn().GetDevUri();
			cItem.chnId = it->GetDevChn().GetChnNO();
			cItem.dataMsgType = it->GetMsgType();
			cItem.transDataBase64 = it->GetData();
			cItem.isEnd = it->IsLastNtf();
			cItem.transChnType = cusdk__TransChnExtData;
			cOut.transDataNotify.push_back(cItem);
			++it;
		}
	}

	// 31 devPushProgressNotify
	if (!cNotifyPool.gbGbsGbPDCLProgressNotify.empty())
	{
		std::list<CGbsGbPDCLProgressNtfReq>::const_iterator it = cNotifyPool.gbGbsGbPDCLProgressNotify.begin();
		cusdk__DevicePushProgressNotify cItem;
		while (it != cNotifyPool.gbGbsGbPDCLProgressNotify.end())
		{
			cItem.pos = it->GetProgressPercent();
			cItem.errorDsc = it->GetProgressTip();
			cOut.devPushProgressNotify.push_back(cItem);
			++it;
		}
	}

	// 32 alarmEventNotify
	if (!cNotifyPool.alarmEventNotify.empty())
	{
		std::list<CAlarmEventNtfReq>::const_iterator it = cNotifyPool.alarmEventNotify.begin();
		cusdk__CMSMessageNotify cItem;
		while (it != cNotifyPool.alarmEventNotify.end())
		{
			cItem.messageTime = it->GetEventOccurTime();
			cItem.messageType = it->GetEventType();
			cItem.messageContent = it->GetEventContent();
			cOut.cmsMessageNotify.push_back(cItem);
			++it;
		}
	}

	// 33
	if (!cNotifyPool.startEncoderTransNotify.empty())
	{
		std::list<CCuStartEncoderTransNtfReq>::const_iterator it = cNotifyPool.startEncoderTransNotify.begin();
		cusdk__StartEncoderTransNotify cItem;
		while (it != cNotifyPool.startEncoderTransNotify.end())
		{
			cItem.encodeChnId = it->GetClientEncChnId();
			ConvertTempMediaDescAndExtInfoCuiToCu(it->GetTMediaDescList(), cItem.mediaDescriptionList, it->GetTExtInfo(), cItem.mediaExtInfo);
			cOut.startEncoderTransNotify.push_back(cItem);
			++it;
		}
	}
	//seqMarks & hasDataLeft
	ConvertStdListToVector(cNotifyPool.seqMarks, cOut.seqMarks);
	cOut.hasDataLeft = cNotifyPool.hasDataLeft;
}

#endif

