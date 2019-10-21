#include "alarmtypecommon.h"
#include "cms/cms_const.h"
#include "soapalarmtype.h"
#include <map>

#ifndef ENABLE_BOOST_BIMAP
#define ENABLE_BOOST_BIMAP false
#endif
//启用boost::bimap
#if ENABLE_BOOST_BIMAP
    #include "boost/typeof/typeof.hpp"
    #include "boost/bimap.hpp"
    #include "boost/bimap/list_of.hpp"
    #include "boost/bimap/set_of.hpp"
    #include "boost/bimap/multiset_of.hpp"
    #include "boost/bimap/unconstrained_set_of.hpp"
    #include "boost/bimap/unordered_set_of.hpp"
    #include "boost/bimap/vector_of.hpp"
    #include "boost/bimap/unordered_multiset_of.hpp"
    #include "boost/bimap/tags/tagged.hpp"
    #include "boost/assign.hpp"

    typedef boost::bimap< CMS_ALARM_TYPE, CU_ALARM_TYPE > CUI_CU_TYPE_MAP;
    typedef bm_type::value_type CUI_CU_PAIR;
    typedef bm_type::left_map::const_iterator  CMS_CU_ITR;
    typedef bm_type::right_map::const_iterator CU_CMS_ITR;
#else
    typedef std::map<CMS_ALARM_TYPE, CU_ALARM_TYPE  > CMS_CU_MAP;
    typedef std::map<CU_ALARM_TYPE,  CMS_ALARM_TYPE > CU_CMS_MAP;
    typedef CMS_CU_MAP::const_iterator  CMS_CU_ITR;
    typedef CU_CMS_MAP::const_iterator  CU_CMS_ITR;
    
    struct TAlarmBimap
    {
    public:
        TAlarmBimap()
        {
        }
        bool insert( const CMS_ALARM_TYPE &cmsType, CU_ALARM_TYPE cuType )
        {
            left [ cmsType ] = cuType;
            right[ cuType  ] = cmsType;
        }

    public:
        CMS_CU_MAP left;//该命名用以兼容 boost bimap，由于boost暂无法编译过，先用此代替
        CU_CMS_MAP right;
    };

    typedef TAlarmBimap CUI_CU_TYPE_MAP;

	struct TAlarmPair
	{
	public:
		TAlarmPair( const CMS_ALARM_TYPE &cmsAlmType, CU_ALARM_TYPE cuAlmType )
		{
			cmsType = cmsAlmType;
			cuType	= cuAlmType;
		}
		CMS_ALARM_TYPE cmsType;
		CU_ALARM_TYPE  cuType;
	};

    #ifndef CUI_CU_PAIR
    #define CUI_CU_PAIR(cmsType, cuType) \
        TAlarmPair( CMS_ALARM_TYPE(cmsType), CU_ALARM_TYPE(cuType) )
    #endif //CUI_CU_PAIR
#endif


#ifndef ADD_ALARM_TYPE
#define ADD_ALARM_TYPE( tPair )   \
        if( m_ptImpl != NULL )              \
        {                                   \
            m_ptImpl->m_cCuiCuMap.insert( tPair.cmsType, tPair.cuType  );       \
            /*m_ptImpl->m_cAlmDescKeyMap[ cmsType ] = MAKE_ALARM_KEY_NAME( #cmsType ); */ \
        } 
#endif //ADD_ALARM_TYPE

	
typedef std::map< CMS_ALARM_TYPE, std::string > ALM_DESC_KEY_MAP;
typedef std::map< CMS_ALARM_TYPE, std::string > ALM_DESC_MAP;
struct TAlarmTypeCommonImpl
{
    CUI_CU_TYPE_MAP     m_cCuiCuMap;
    ALM_DESC_KEY_MAP    m_cAlmDescKeyMap;
	ALM_DESC_MAP		m_cAlmDescMap;
};

//全局
//std::auto_ptr<CAlarmTypeCommon> g_apcAlarmTypeCommon( new CAlarmTypeCommon() );
CAlarmTypeCommon g_cAlarmTypeCommon;//由于内部采用 impl 模式，不再使用智能指针

const CMS_ALARM_TYPE INVALID_CMS_ALM_TYPE = "";
const CU_ALARM_TYPE  INVALID_CU_ALM_TYPE    = cusdk__AlarmTypeInvalid;
const CU_ALARM_TYPE  CU_ALM_TYPE_IN_V2_BEGIN= cusdk__AlarmExceptionEvent;
const CU_ALARM_TYPE  CU_ALM_TYPE_IN_V2_END  = cusdk__AlarmExceptionEvent + 100;



CAlarmTypeCommon::CAlarmTypeCommon(void):m_ptImpl(NULL)
{
    m_ptImpl = new TAlarmTypeCommonImpl();
    InitCuiCuAlmMap();
    InitCuiDescKeyMap();
	InitAlmDescMap();
}


CAlarmTypeCommon::~CAlarmTypeCommon(void)
{
    if( m_ptImpl != NULL )
    {
        delete m_ptImpl;
        m_ptImpl = NULL;
    }
}


//void CAlarmTypeCommon::AddAlarmType( const CMS_ALARM_TYPE &cmsType, CU_ALARM_TYPE cuType )
//{
//    m_ptImpl->m_cCuiCuMap.insert( CUI_CU_PAIR( cmsType, cuType )  )
//}

void CAlarmTypeCommon::InitCuiCuAlmMap()
{
    //cui
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_MOVE,           cusdk__AlarmMove        ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_PIN,            cusdk__AlarmInput       ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_DISKFULL,       cusdk__AlarmDiskfull    ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_VIDLOST,        cusdk__AlarmVideoLost   ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_INTELLIGENT,    cusdk__AlarmIntelligent ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_VIDEOPOLL,      cusdk__AlarmVideo       ) );

    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_EXCEPTION_EVENT,cusdk__AlarmExceptionEvent) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_DEFOCUS,        cusdk__AlarmDefocus      ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_SCENE_CHANGE,   cusdk__AlarmSceneChange  ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_REGION_INVASION,cusdk__AlarmRegionInvasion ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_REGION_LEAVING, cusdk__AlarmRegionLeaving) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_OBJECT_TAKEN,   cusdk__AlarmObjectTaken  ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_OBJECT_LEFT,    cusdk__AlarmObjectLeft   ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_PEOPLE_GATHER,  cusdk__AlarmPeopleGather ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_CORGON,         cusdk__AlarmCorgon       ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_AUDIO_ABNORMAL, cusdk__AlarmAudioAbnormal) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_LUMA_CHANGE,    cusdk__AlarmLumaChange   ) );

    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_GPS_OVERSPEED,  cusdk__AlarmGPSOverSpeed ) );
	ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_SIM_DATA_SERVICE_OVERUSED,  cusdk__AlarmSIMDataTrafficExceeded ) );

    //gb2016
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_ILLEGAL_DEV_DISASSEMBLY,cusdk__AlarmAntiDismantle   ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_DEVICE_OVER_HEAT,       cusdk__AlarmHighTemperature ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_DEVICE_OVER_COLD,       cusdk__AlarmLowTemperature  ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_ARTIFICIAL_SCENE,       cusdk__AlarmPersonVideo     ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_OBJECT_MOTION,          cusdk__AlarmSportGoalCheck  ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_TRIP_WIRE,              cusdk__AlarmTripwireCheck   ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_MOVING_AROUND,          cusdk__AlarmHoverCheck      ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_INDIVIDUAL_FLOW_STATISTICS,     cusdk__AlarmTrafficStatisticsCheck  ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_INDIVIDUAL_THICKNESS_STATISTICS,cusdk__AlarmDensityCheck            ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_VIDEO_EXCEPTION,        cusdk__AlarmVideoException  ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_MOVING_SWIFTLY,         cusdk__AlarmMoveFast        ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_STORAGE_DEV_DISK_FAULT, cusdk__AlarmDiskFault       ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_STORAGE_DEV_FAN_FAULT,  cusdk__AlarmFanFault        ) );
    ADD_ALARM_TYPE( CUI_CU_PAIR( ALARM_TYPE_GO_AGAINST_TRAFFIC,     cusdk__AlarmRetrogradeCheck ) );

}


void CAlarmTypeCommon::InitCuiDescKeyMap()
{
	    //cui;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_MOVE				] = LAN_ALARM_TYPE_MOVE;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_PIN				] = LAN_ALARM_TYPE_PIN;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_DISKFULL			] = LAN_ALARM_TYPE_DISKFULL;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_VIDLOST			] = LAN_ALARM_TYPE_VIDLOST;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_INTELLIGENT		] = LAN_ALARM_TYPE_INTELLIGENT;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_VIDEOPOLL		] = LAN_ALARM_TYPE_VIDEOPOLL;

    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_EXCEPTION_EVENT	] = LAN_ALARM_TYPE_EXCEPTION_EVENT;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_DEFOCUS			] = LAN_ALARM_TYPE_DEFOCUS;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_SCENE_CHANGE		] = LAN_ALARM_TYPE_SCENE_CHANGE;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_REGION_INVASION	] = LAN_ALARM_TYPE_REGION_INVASION;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_REGION_LEAVING	] = LAN_ALARM_TYPE_REGION_LEAVING;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_OBJECT_TAKEN		] = LAN_ALARM_TYPE_OBJECT_TAKEN;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_OBJECT_LEFT		] = LAN_ALARM_TYPE_OBJECT_LEFT;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_PEOPLE_GATHER	] = LAN_ALARM_TYPE_PEOPLE_GATHER;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_CORGON			] = LAN_ALARM_TYPE_CORGON;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_AUDIO_ABNORMAL	] = LAN_ALARM_TYPE_AUDIO_ABNORMAL;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_LUMA_CHANGE		] = LAN_ALARM_TYPE_LUMA_CHANGE;

    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_GPS_OVERSPEED    ] = LAN_ALARM_TYPE_GPS_OVERSPEED;
	m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_SIM_DATA_SERVICE_OVERUSED ] = LAN_ALARM_TYPE_SIM_DATA_SERVICE_OVERUSED;

    //gb2016;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_ILLEGAL_DEV_DISASSEMBLY ] = LAN_ALARM_TYPE_ILLEGAL_DEV_DISASSEMBLY;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_DEVICE_OVER_HEAT	] = LAN_ALARM_TYPE_DEVICE_OVER_HEAT;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_DEVICE_OVER_COLD	] = LAN_ALARM_TYPE_DEVICE_OVER_COLD;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_ARTIFICIAL_SCENE	] = LAN_ALARM_TYPE_ARTIFICIAL_SCENE;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_OBJECT_MOTION	] = LAN_ALARM_TYPE_OBJECT_MOTION;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_TRIP_WIRE		] = LAN_ALARM_TYPE_TRIP_WIRE;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_MOVING_AROUND	] = LAN_ALARM_TYPE_MOVING_AROUND;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_INDIVIDUAL_FLOW_STATISTICS		] = LAN_ALARM_TYPE_INDIVIDUAL_FLOW_STATISTICS;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_INDIVIDUAL_THICKNESS_STATISTICS	] = LAN_ALARM_TYPE_INDIVIDUAL_THICKNESS_STATISTICS;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_VIDEO_EXCEPTION					] = LAN_ALARM_TYPE_VIDEO_EXCEPTION;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_MOVING_SWIFTLY					] = LAN_ALARM_TYPE_MOVING_SWIFTLY;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_STORAGE_DEV_DISK_FAULT			] = LAN_ALARM_TYPE_STORAGE_DEV_DISK_FAULT;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_STORAGE_DEV_FAN_FAULT			] = LAN_ALARM_TYPE_STORAGE_DEV_FAN_FAULT;
    m_ptImpl->m_cAlmDescKeyMap[ ALARM_TYPE_GO_AGAINST_TRAFFIC			    ] = LAN_ALARM_TYPE_GO_AGAINST_TRAFFIC;

}

void CAlarmTypeCommon::InitAlmDescMap()
{
	if( m_ptImpl == NULL )
	{
		return;
	}
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_PIN				] = "并口告警\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_MOVE			] = "移动侦测告警\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_VIDLOST			] = "视频源丢失告警\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_DISKFULL		] = "磁盘满告警\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_INTELLIGENT		] = "智能告警\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_VIDEOPOLL		] = "视质轮询告警\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_EXCEPTION_EVENT ] = "异常事件消息\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_DEFOCUS			] = "虚焦检测\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_SCENE_CHANGE	] = "场景变换\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_REGION_INVASION ] = "区域入侵\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_REGION_LEAVING	] = "区域离开\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_OBJECT_TAKEN	] = "物品拿取\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_OBJECT_LEFT		] = "物品遗留\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_PEOPLE_GATHER	] = "人员聚集\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_CORGON			] = "警戒线\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_AUDIO_ABNORMAL	] = "声音异常\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_LUMA_CHANGE		] = "亮度变化\n";

	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_GPS_OVERSPEED			 ] = "gps 超速\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_SIM_DATA_SERVICE_OVERUSED] = "数据流量超标\n";

	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_ILLEGAL_DEV_DISASSEMBLY ] = "设备非法拆解\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_DEVICE_OVER_HEAT		] = "设备过热\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_DEVICE_OVER_COLD		] = "设备过冷\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_ARTIFICIAL_SCENE		] = "人工视频\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_OBJECT_MOTION			] = "移动检测\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_TRIP_WIRE				] = "绊线\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_MOVING_AROUND			] = "徘徊检测\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_INDIVIDUAL_FLOW_STATISTICS		] = "流量检测\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_INDIVIDUAL_THICKNESS_STATISTICS ] = "密度统计\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_VIDEO_EXCEPTION			] = "视频异常\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_MOVING_SWIFTLY			] = "快速移动\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_STORAGE_DEV_DISK_FAULT	] = "存储设备磁盘故障\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_STORAGE_DEV_FAN_FAULT	] = "存储设备风扇故障\n";
	m_ptImpl->m_cAlmDescMap[ ALARM_TYPE_GO_AGAINST_TRAFFIC	    ] = "逆向行驶\n";

}


CMS_ALARM_TYPE CAlarmTypeCommon::ConvertFromCuToCms( CU_ALARM_TYPE cuType  ) const
{
    CU_CMS_ITR cuCmsItr = m_ptImpl->m_cCuiCuMap.right.find(cuType);
    if( cuCmsItr == m_ptImpl->m_cCuiCuMap.right.end() )
    {
        return INVALID_CMS_ALM_TYPE;
    }
    else
    {
        return cuCmsItr->second;
    }
}

CU_ALARM_TYPE CAlarmTypeCommon::ConvertFromCmsToCu( const CMS_ALARM_TYPE &cmsType) const
{
    CMS_CU_ITR cmsCuItr = m_ptImpl->m_cCuiCuMap.left.find(cmsType);
    if( cmsCuItr == m_ptImpl->m_cCuiCuMap.left.end() )
    {
        return cusdk__AlarmTypeInvalid;
    }
    else
    {
        return cmsCuItr->second;
    }
}



bool CAlarmTypeCommon::HasCuAlarmType(CU_ALARM_TYPE &cuType) const
{
    return m_ptImpl->m_cCuiCuMap.right.find(cuType) != m_ptImpl->m_cCuiCuMap.right.end();
}
bool CAlarmTypeCommon::HasCmsAlarmType( const CMS_ALARM_TYPE &cmsType) const
{
    return m_ptImpl->m_cCuiCuMap.left.find(cmsType) != m_ptImpl->m_cCuiCuMap.left.end();
}

bool CAlarmTypeCommon::IsInCuV2Only( CU_ALARM_TYPE cuType ) const
{
    return cuType >= CU_ALM_TYPE_IN_V2_BEGIN 
        && cuType <  CU_ALM_TYPE_IN_V2_END;
}

bool CAlarmTypeCommon::WriteAllAlarmDesc(IAlarmDataWriter *pcWriter) const
{
    if( pcWriter == NULL || m_ptImpl == NULL)
    {
        return false;
    }
	ALM_DESC_KEY_MAP::const_iterator itrNext = m_ptImpl->m_cAlmDescKeyMap.begin();
	while( itrNext != m_ptImpl->m_cAlmDescKeyMap.end() )
	{
		TAlarmData tData;
		tData.strDescKey = itrNext->second;
		pcWriter->Write( itrNext->first, tData );
		++itrNext;
	}
    return true;
}

std::string CAlarmTypeCommon::GetAlmDesc( const CMS_ALARM_TYPE &cmsType) const
{
	ALM_DESC_MAP::const_iterator cmsItr = m_ptImpl->m_cAlmDescMap.find(cmsType);
    if( cmsItr == m_ptImpl->m_cAlmDescMap.end() )
    {
        return "未知告警类型\n";
    }
    else
    {
        return cmsItr->second;
    }
}


const CAlarmTypeCommon* GetCommonType()
{
    return &g_cAlarmTypeCommon;
}

