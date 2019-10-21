#include <math.h>
#include "dbopr.h"
#include "dboprdef.h"
#include "utility/uuid.h"
#include "license_mng.h"
#include "dbcache.h"
#include "config_util/tas_config.h"
#include "cctrl.h"
#include "sync_analyzer_mng.h"
#include "tasks/parent_tas_conn_task.h"
#include "cms/utility/mischelper.h"
#include "cms/utility/cmszip.h"
#include "db_task_pool.h"
#include "tas_redis.h"
#include "tas_struct.h"
#include "redis/fastdbmng.h"
#include "inner_msg.h"
#include "kdvsys.h"
#include "cms/utility/digital_string.h"


#define AppendUpdateValuePair(expr, var, tSql, field) if (expr(var)) {AppendFieldValuePairImp(tSql, field, var); tSql.AppendString(",");}
#define AppendUpdateDateValuePair(expr, var, tSql, field) if (expr(var)) {AppendFieldDatetimeValuePairImp(tSql, field, var); tSql.AppendString(",");}
#define DbStr(str) DbStrImp(str).c_str()
#define DEVICE_DEVDATA_LENGTH    4096				// tblDevice表中devData（扩展信息字段）的最大长度

void CDbOpr::DeviceBatchChange( const CTaskMsgTransfer<CTasDeviceBatchChangeReq> *pcMsg )
{
	//按顺序逐个处理，请求类型包括：设备入网、设备修改、设备退网、设备分组划归、视频源别名设置（含经纬度同步）
	CTasDeviceBatchChangeReq cReq = pcMsg->GetMsgObj();
	CTasDeviceBatchChangeRsp cRsp ;
	cRsp.taskno = cReq.taskno;
	
	CDeviceAddReq cDevAddReq;
	CDeviceModReq cDevModReq;
	CDeviceDelReq cDevDelReq;
	CDeviceCapIndexNameSetReq cDevCapIdxNameSetReq;
	CDeviceGroupDeviceAddReq cDevGrpDevAddReq;

	ECMS_ERRORCODE errCode;
	for (vector<TTasDeviceBatchChangeItem> ::const_iterator it = cReq.Items.begin(); it != cReq.Items.end(); ++it)
	{
		errCode = CMS_SUCCESS;
		if (it->type == OspExtEventDesc(TAS_DEVICE_ADD_REQ))
		{
			cDevAddReq.FromXml(it->content);
			cDevAddReq.SetSession(cReq.loginsession);
			cDevAddReq.SetSeqNum(pcMsg->GetOspTaskNo());
			DevBatchChg4DevAdd(cDevAddReq, errCode);
		}
		else if (it->type == OspExtEventDesc(TAS_DEVICE_DEL_REQ))
		{
			cDevDelReq.FromXml(it->content);
			cDevDelReq.SetSession(cReq.loginsession);
			cDevDelReq.SetSeqNum(pcMsg->GetOspTaskNo());
			DevBatchChg4DevDel(cDevDelReq, errCode);
		}
		else if (it->type == OspExtEventDesc(TAS_DEVICE_MOD_REQ))
		{
			cDevModReq.FromXml(it->content);
			cDevModReq.SetSession(cReq.loginsession);
			cDevModReq.SetSeqNum(pcMsg->GetOspTaskNo());
			DevBatchChg4DevMod(cDevModReq, errCode);
		}
		else if (it->type == OspExtEventDesc(TAS_DEVICE_CAP_INDEX_NAME_SET_REQ))
		{
			cDevCapIdxNameSetReq.FromXml(it->content);
			cDevCapIdxNameSetReq.SetSession(cReq.loginsession);
			cDevCapIdxNameSetReq.SetSeqNum(pcMsg->GetOspTaskNo());
			DevBatchChg4VidSrcMod(cDevCapIdxNameSetReq, errCode);
		}
		else if (it->type == OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_ADD_REQ))
		{
			CTaskMsgTransfer<CDeviceGroupDeviceAddReq> transferReq(pcMsg->GetOspTaskNo(), TKDSIP_TRANS_ID(), TAS_DEVICE_GROUP_DEVICE_ADD_REQ);
			transferReq.GetMsgObj().FromXml(it->content);
			transferReq.GetMsgObj().SetSession(cReq.loginsession);
			DevBatchChg4DevGrpDevChg(&transferReq, errCode);

		}
		else if (it->type == OspExtEventDesc(TAS_DEV_CAPINFO_MOD_REQ))
		{
			CTaskMsgTransfer<CDevCapInfoModReq> transferReq(pcMsg->GetOspTaskNo(), TKDSIP_TRANS_ID(), TAS_DEV_CAPINFO_MOD_REQ);
			transferReq.GetMsgObj().FromXml(it->content);
			transferReq.GetMsgObj().SetSession(cReq.loginsession);
			DevBatchChg4DevModelChg(&transferReq, errCode);
		}
		else
		{
			continue;
		}
		cRsp.Items.push_back(errCode);
	}
	
	if(!RedisPublishDeviceBatchRsp(cReq.gbuId, cRsp))
	{
		GLOBELOG(ERROR_LEV, "DeviceBatchChange--RedisPublishDeviceBatchRsp fail \n");
	}
}

void CDbOpr::DevBatchChg4DevAdd( const CDeviceAddReq& cDevAddReq, ECMS_ERRORCODE& errCode )
{
	CTaskMsgTransfer<CDeviceAddReq> cTransReq(cDevAddReq.GetSeqNum(), TKDSIP_TRANS_ID(), TAS_DEVICE_ADD_REQ);
	CTaskMsgTransfer<CDeviceAddNtfReq> cTransNtf;
	cTransReq.SetMsgObj(cDevAddReq);
	CDeviceAddNtfReq& cNtf = cTransNtf.GetMsgObj();
	bool bHasNtf = false;

	CDeviceInfo cDev = cDevAddReq.GetDeviceInfo();

	CUUID cDevId;
	if (!cDev.GetDeviceUUID().empty())		// 支持指定设备ID入网
	{
		cDevId = cDev.GetDeviceUUID();
	}

	bool bNeedRollbackDevice  = false;
	bool bNeedRollbackLicense = false;
	CDeviceModelInfo cModel;
	do 
	{
		if (UUID_LENGTH != cDevId.size())					// 检查DeviceId长度是否合法
		{
			errCode = ERR_TAS_INVALID_DEVICEID;
			break;
		}

		cDev.SetDomainUUID(CurrentDomainId());


		cModel.SetDeviceModelName(cDev.GetModelName());
		cModel.SetDeviceTypeId(cDev.GetDeviceType());
		cModel.SetDomainId(CurrentDomainId());
		cModel.SetManufacturer(cDev.GetManufacturer());
		cModel.SetOwner(cDev.GetOwner());
		cModel.SetCreater(cDev.GetCreater());
		cModel.SetDeviceCapInfo(cDev.GetDeviceModelCapDetail());

		CDeviceModelAddReq cModelAddReq;
		cModelAddReq.SetDeviceModelInfo(cModel);
		CTaskMsgTransfer<CDeviceModelAddReq> cModelAddTrans;
		cModelAddTrans.SetMsgObj(cModelAddReq);
		//cModelAddTrans.SetTransfer()
		CTaskMsgTransfer<CDeviceModelAddReq> *pcMsg = &cModelAddTrans;
		if (cModel.GetDeviceModelId().empty())
		{
			GetExistDeviceModelId(CurrentDomainId(), cModel.GetDeviceTypeId(), 
				cModel.GetDeviceModelName(),false, cModel.GetManufacturer(), cModel.GetCreater(), 
				cModel.GetDeviceCapInfo(), cModel.GetDeviceModelId());
			if (cModel.GetDeviceModelId().empty())
			{
				cModel.SetDeviceModelId(DeviceModelAdd(pcMsg,true));
			}
		}

		if (cModel.GetDeviceModelId().empty() || cModel.GetDomainId() != CurrentDomainId())
		{
			errCode = ERR_TAS_INVALID_DEVICEMODELID;
			break;
		}
		cDev.SetDeviceModel(cModel.GetDeviceModelId());
		cDev.SetDeviceType(cModel.GetDeviceTypeId());

		if (cDev.GetExpireDate().empty())
		{
			errCode = ERR_TAS_INVALID_EXPIRE_DATE;
			break;
		}

		ENTasErrno enErrno = AllocLicense(cModel,cDev.GetCreater());
		if (CMS_SUCCESS != enErrno)
		{
			errCode = enErrno;
			break;
		} 
		bNeedRollbackLicense = true;

		CCMSTime cDateNow;
		cDev.SetDeviceUUID(cDevId);
		cDev.SetCreateDate(cDateNow.ToString());
		errCode = DeviceItemAdd(cDev);
		if (CMS_SUCCESS != errCode)
		{
			break;
		}
		bNeedRollbackDevice  = true;
		bNeedRollbackLicense = false;

		errCode = CuserDevAddDefault(cDevId, cDev.GetDeviceModel());
		if (errCode != CMS_SUCCESS) break;

		CDeviceGroupDeviceAddReq cGrpAddDevReq;
		//指定设备组入网  [11/23/2015 pangubing]
		if (!cDev.GetGroupId().empty())
		{
			vector<TDeviceCapIndexs> deviceCapIndexs;
			TDeviceCapIndexs tCapIndex;
			tCapIndex.id=DEVICE_CAP_VIDEOSOURCE;
			vector<int> vecCapIndex;
			vector<TDeviceCapInfo> &atCapInfos = cModel.GetDeviceCapInfo();
			for (vector<TDeviceCapInfo>::const_iterator it = atCapInfos.begin(); it != atCapInfos.end(); ++it)
			{
				if (it->type != DEVICE_CAP_VIDEOSOURCE)
				{
					continue;
				}

				for (u32 i = 0; i < static_cast<u32>(it->num); ++i)
				{
					vecCapIndex.push_back(i);
					break;
				}
			}
			tCapIndex.indexs=vecCapIndex;
			deviceCapIndexs.push_back(tCapIndex);
			cGrpAddDevReq.GetDeviceGroupDeviceInfo().SetDeviceID(cDevId);
			cGrpAddDevReq.GetDeviceGroupDeviceInfo().SetDeviceGroupID(cDev.GetGroupId());
			cGrpAddDevReq.GetDeviceGroupDeviceInfo().SetDomainId(cDev.GetDomainUUID());
			cGrpAddDevReq.GetDeviceGroupDeviceInfo().SetDeviceCapIndexs(deviceCapIndexs);

			// 直接调用原有设备组增加设备接口 [11/24/2015 pangubing]
			CTaskMsgTransfer<CDeviceGroupDeviceAddReq> cTransGrpAddDevReq;
			cTransGrpAddDevReq.SetMsgObj(cGrpAddDevReq);
			//cTransGrpAddDevReq.SetTransfer(*pcMsg,cGrpAddDevReq.GetEventId());

			errCode = DevGrpDevAdd(&cTransGrpAddDevReq,true);
			if (errCode != CMS_SUCCESS) break;
		}
		else
		{
			errCode = DevGrpDevAddDefault(cDevId, cDev.GetDeviceModel());
			if (errCode != CMS_SUCCESS) break;

			// 将新入网的设备划归给自动更新组对应的用户和用户组(设备入网默认是未分组，故需要划归给以“所有设备”为自动更新组对应下的用户和用户组)
			AutoAssignDevice(&cTransReq, cDevAddReq, cDevId, CurrentDomainId(), cModel, CurrentDomainId());

		}

		bHasNtf = (CMS_SUCCESS == DeviceItemQry(cDevId, cNtf.GetDeviceInfo()));

		SyncAnalyse(enSyncDataDeviceInfo, enSyncOperateAdd, cDev.GetDomainUUID(), &cDev.GetDeviceUUID(), cDev.GetDeviceType(), 
			cDev.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev), 0);

		if (cDev.GetDeviceType() == DEVICE_TYPE_ENCODER)
		{
			string strDevGrpId = cDev.GetGroupId();
			if (strDevGrpId.empty())
			{
				strDevGrpId = CurrentDomainId();
			}
			string strDevTreeId="";
			double dDevSeq;
			int vidSrcNum;
			if (cDev.GetGroupId().empty())
			{
				CDeviceModelInfo cModel = g_cDbCache.GetDeviceModelInfo(cDev.GetDeviceModel(), this);
				for (int i = 0; i<cModel.GetDeviceCapInfo().size(); i++)
				{
					if (cModel.GetDeviceCapInfo()[i].type != DEVICE_CAP_VIDEOSOURCE)
					{
						continue;
					}
					for (int j = 0; j<cModel.GetDeviceCapInfo()[i].num; j++)
					{
						if (QryVidSrcDevSeq(strDevTreeId, strDevGrpId, cDev.GetDeviceUUID(), j,dDevSeq))
						{
							SyncAnalyse(enSyncDataDevGrpDevSeq, enSyncOperateMod, CurrentDomainId(),
								&strDevGrpId,
								&cDev.GetDeviceUUID(),
								j,
								dDevSeq,
								cDev.GetDeviceType());
						}
					}
				}
			}
			else
			{
				vector<TDeviceCapIndexs>& vecDevCaps = cGrpAddDevReq.GetDeviceGroupDeviceInfo().GetDeviceCapIndexs();
				for (int i = 0; i<vecDevCaps.size(); i++)
				{
					if (vecDevCaps[i].id != DEVICE_CAP_VIDEOSOURCE)
					{
						continue;
					}
					for (int j = 0; j<vecDevCaps[i].indexs.size(); j++)
					{
						if (QryVidSrcDevSeq(strDevTreeId, strDevGrpId, cDev.GetDeviceUUID(), vecDevCaps[i].indexs[j],dDevSeq))
						{
							SyncAnalyse(enSyncDataDevGrpDevSeq, enSyncOperateMod, CurrentDomainId(),
								&strDevGrpId,
								&cDev.GetDeviceUUID(),
								vecDevCaps[i].indexs[j],
								dDevSeq,
								cDev.GetDeviceType());
						}
					}
				}
			}
		}
	} while (false);

	if (errCode == CMS_SUCCESS)
	{
		// 之前新入网设备只可能属于admin，现在有了自动同步组的概念后，新入网设备也可以是其他用户，故需要更新所有拥有该设备的用户设备树版本信息
		//UpdateDevTreeVer(cDevId);
		// 修改入网解码器没有更新设备树版本号 [11/23/2015 pangubing]
		if (cDev.GetDeviceType() != DEVICE_TYPE_DECODER)
		{
			UpdateDevTreeVer(cDevId);
		}
		else
		{
			UpdateDevTreeVer("");
		}
	}
	else
	{
		if (bNeedRollbackDevice)
		{
			DeviceDelImp(cDevId);
		}

		bHasNtf = false; // 失败时取消动态通知消息

		if (bNeedRollbackLicense)
		{
			DeallocLicense(cModel);
		}
	}

	if (bHasNtf)
	{
		RedisDeviceAdd(cDev);
		PostTaskMsgTransfer2CCtrl(cTransReq, cDevAddReq, cNtf, cTransNtf);
	}
}

void CDbOpr::DevBatchChg4DevMod( const CDeviceModReq& cDevModReq, ECMS_ERRORCODE& errCode )
{
	CTaskMsgTransfer<CDeviceModReq> cTransReq(cDevModReq.GetSeqNum(), TKDSIP_TRANS_ID(), TAS_DEVICE_MOD_REQ);
	CTaskMsgTransfer<CDeviceModNtfReq> cTransNtf;
	cTransReq.SetMsgObj(cDevModReq);
	CDeviceModNtfReq &cNtf = cTransNtf.GetMsgObj();
	bool bHasNtf = false;

	bool bNeedRollbackLicense = false;
	CDeviceModelInfo cOldModel, cNewModel;
	set<TChangedField> setChangedFields;
	do 
	{
		CDeviceInfo cInfo;

		ENTasErrno enErrno = DeviceItemQry(cDevModReq.GetDeviceUUID(), cInfo);
		if (CMS_SUCCESS != enErrno)
		{
			errCode = enErrno;
			break;
		}

		TDevicePropertyFlags cPropertyFlags;
		if (cDevModReq.GetDevicePropertyFlags(cPropertyFlags))
		{
			if (cPropertyFlags.HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_DISABLE_REASSIGN) 
				!= cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_DISABLE_REASSIGN))
			{
				errCode = ERR_TAS_INVALID_OPERATION;
				break;
			}
			if (cPropertyFlags.HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_ABLE_REASSIGN) 
				!= cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_ABLE_REASSIGN))
			{
				errCode = ERR_TAS_INVALID_OPERATION;
				break;
			}
		}
		else
		{
			cPropertyFlags = cInfo.GetDevicePropertyFlags();
		}
		if (cInfo.GetDomainUUID() != CurrentDomainId())
		{
			errCode = ERR_TAS_INVALID_OPERATION;
			break;
		}

		DeviceModelItemQry(cInfo.GetDeviceModel(), cOldModel);

		UUIDString tDevModelId;
		if (cDevModReq.GetDeviceModelId(tDevModelId) && tDevModelId != cInfo.GetDeviceModel())
		{
			setChangedFields.insert(TCHANGED_FIELD_DEVMODEL);
			DeviceModelItemQry(tDevModelId, cNewModel);
			if (cNewModel.GetDeviceTypeId() != cOldModel.GetDeviceTypeId())
			{
				errCode = ERR_TAS_INVALID_OPERATION;
				break;
			}

			bNeedRollbackLicense = true;
			errCode = AllocLicense(cNewModel,cInfo.GetCreater());
			if (errCode != CMS_SUCCESS)
			{
				bNeedRollbackLicense = false;
				break;
			}

			errCode = DeviceModelPreMod(cInfo.GetDeviceUUID(), cOldModel, cNewModel);
			if (errCode != CMS_SUCCESS) break;
		}

		TSqlString tSql;
		tSql.Format("update %s set ", T_DEVICE);
		AppendUpdateValuePair(cDevModReq.GetDeviceName, m_strFieldValue, tSql, F_DEVICE_DEVNAME)
		AppendUpdateValuePair(cDevModReq.GetDeviceAlias, m_strFieldValue, tSql, F_DEVICE_DEVALIAS)
		AppendUpdateValuePair(cDevModReq.GetDeviceModelId, m_strFieldValue, tSql, F_DEVICE_DEVMODELID)
		AppendUpdateValuePair(cDevModReq.GetUsedFlag, m_bFieldValue, tSql, F_DEVICE_USEFLAG)
		AppendUpdateValuePair(cDevModReq.GetNetType, m_nFieldValue, tSql, F_DEVICE_NETTYPE)
		AppendUpdateValuePair(cDevModReq.GetNetAddress, m_strFieldValue, tSql, F_DEVICE_NETADDR)
		AppendUpdateValuePair(cDevModReq.GetDescription, m_strFieldValue, tSql, F_DEVICE_DEVDESC)
		AppendUpdateDateValuePair(cDevModReq.GetExpireDate, m_strFieldValue, tSql, F_DEVICE_EXPIREDATE)

		string strTemp;
		bool bTemp;
		int  nTemp;
		if (cDevModReq.GetDeviceAlias(strTemp) && strTemp != cInfo.GetDeviceAlias())
		{
			setChangedFields.insert(TCHANGED_FIELD_DEVALIAS);
		}
		if (cDevModReq.GetUsedFlag(bTemp) && bTemp!= cInfo.GetUsedFlag())
		{
			setChangedFields.insert(TCHANGED_FIELD_DEVUSEFLAG);
		}
		if (cDevModReq.GetNetType(nTemp) && nTemp != cInfo.GetNetType())
		{
			setChangedFields.insert(TCHANGED_FIELD_DEVNETTYPE);
		}
		if (cDevModReq.GetNetAddress(strTemp) && strTemp != cInfo.GetNetAddress())
		{
			setChangedFields.insert(TCHANGED_FIELD_DEVNETADDRESS);
		}

		TDevExtData tDevExtData;
		if (cDevModReq.GetDevData(tDevExtData) && !tDevExtData.GetExtData().empty())
		{
			// 需要先查询出原有的扩展信息，然后进行增量的修改
			TSqlString tSelSql;
			tSelSql.Format("select %s from %s where %s='%s' ", 
				F_DEVICE_DEVDATA, T_DEVICE, 
				F_DEVICE_DEVID, DbStr(cDevModReq.GetDeviceUUID()));

			if (!ExecSql(tSelSql, enExecMayRet))
			{
				errCode = ERR_TAS_DBERROR;
				break;
			}

			string strDevExtData;
			if (IsNotEof())					// 已经存在着扩展信息
			{
				strDevExtData = GetString(F_DEVICE_DEVDATA);
				TDevExtData tExistExtData;
				if (!strDevExtData.empty())
				{
					tExistExtData.FromXml(strDevExtData);

					for (map<TDevExtDataType, string>::const_iterator it = tDevExtData.GetExtData().begin(); 
						tDevExtData.GetExtData().end() != it; ++it)
					{
						tExistExtData.GetExtData()[it->first] = it->second;
					}

					strDevExtData = tExistExtData.ToXml();
				}
				else
				{
					strDevExtData = tDevExtData.ToXml();
				}
			}
			else
			{
				errCode = ERR_TAS_RECORD_FETCH_FAILED;
				break;
			}

			if (DEVICE_DEVDATA_LENGTH <= strDevExtData.length())
			{
				errCode = ERR_TAS_EXTDATA_TOO_LONG;
				break;
			}
			else
			{
				AppendFieldValuePairImp(tSql, F_DEVICE_DEVDATA, strDevExtData); 
				tSql.AppendString(",");
			}
		}

		tSql.AppendString(" %s=%d", F_DEVICE_DEVPROPERTYFLAGS, (s32)cPropertyFlags.Convert2Int());
		tSql.EraseLastRedundantComma();
		tSql.AppendString(" where %s = '%s'", F_DEVICE_DEVID, DbStr(cDevModReq.GetDeviceUUID()));
		if (!ExecSql(tSql, enExecNoRet))
		{
			errCode = ERR_TAS_DBERROR;
			break;
		}
		if (m_cDbEngine.GetAffectedRowCount() == 0)
		{
			break;
		}

		//对于国标子域上报的设备，devicename变更时要同步更新其视频源别名
		string strNewDevName;
		if (CREATE_BY_GBS_FOREIGN_DOMAIN == cInfo.GetCreater()
			&&cDevModReq.GetDeviceName(strNewDevName)
			&&strNewDevName != cInfo.GetDeviceName())
		{
			setChangedFields.insert(TCHANGED_FIELD_DEVNAME);
			u32 vidSrcNum = 1;
			if(cDevModReq.GetDeviceModelId(tDevModelId) && tDevModelId != cInfo.GetDeviceModel())
			{
				for(vector<TDeviceCapInfo> ::iterator it = cNewModel.GetDeviceCapInfo().begin();
					it != cNewModel.GetDeviceCapInfo().end(); ++it)
				{
					if (it->type == DEVICE_CAP_VIDEOSOURCE)
					{
						vidSrcNum = it->num;
						break;
					}
				}
			}
			else
			{
				for(vector<TDeviceCapInfo> ::iterator it = cInfo.GetDeviceModelCapDetail().begin();
					it != cInfo.GetDeviceModelCapDetail().end(); ++it)
				{
					if (it->type == DEVICE_CAP_VIDEOSOURCE)
					{
						vidSrcNum = it->num;
						break;
					}
				}
			}

			TDeviceCapName tCapNameNtf;
			tCapNameNtf.devDomainId = cInfo.GetDomainUUID();
			tCapNameNtf.deviceId = cInfo.GetDeviceUUID();
			tCapNameNtf.capId = DEVICE_CAP_VIDEOSOURCE;
			TDeviceCapIndexName tIndexName;
			vector<int> vidsrcIndex;
			vector<string> vidsrcName;	
			if (vidSrcNum==1)
			{
				string strOldVidSrcName;
				//先查询该设备所有视频源别名，之后注意判断是否需要同步修改
				tSql.Format("select %s,%s from %s where %s='%s' and %s=%d",
					F_DEVICECAPINDEXDATA_DEVICECAPINDEX,F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME,T_DEVICECAPINDEXDATA,
					F_DEVICECAPINDEXDATA_DEVICEID,DbStr(cInfo.GetDeviceUUID()),
					F_DEVICECAPINDEXDATA_DEVICECAPID, DEVICE_CAP_VIDEOSOURCE);
				if (ExecSql(tSql, enExecMayRet))
				{
					strOldVidSrcName = string(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME));
					if (strOldVidSrcName == cInfo.GetDeviceName())
					{
						vidsrcIndex.push_back(0);
						vidsrcName.push_back(strNewDevName);
						ModDeviceVidSrcName(cInfo.GetDeviceUUID(), 0, strNewDevName);

						tIndexName.index = 0;
						tIndexName.name = strNewDevName;
						tCapNameNtf.deviceCapIndexNames.push_back(tIndexName);
					}
				}
			}
			else if(vidSrcNum>1)
			{
				string strOldVidSrcName,strNewVidSrcName;
				string strOldName = cInfo.GetDeviceName()+"_";
				int nVidsrcIndex = -1;
				//先查询该设备所有视频源别名，之后注意判断是否需要同步修改
				tSql.Format("select %s,%s from %s where %s='%s' and %s=%d",
					F_DEVICECAPINDEXDATA_DEVICECAPINDEX,F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME,T_DEVICECAPINDEXDATA,
					F_DEVICECAPINDEXDATA_DEVICEID,DbStr(cInfo.GetDeviceUUID()),
					F_DEVICECAPINDEXDATA_DEVICECAPID, DEVICE_CAP_VIDEOSOURCE);
				if (ExecSql(tSql, enExecMayRet))
				{
					while(IsNotEof())
					{
						strOldVidSrcName = string(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME));
						nVidsrcIndex = GetInt(F_DEVICECAPINDEXDATA_DEVICECAPINDEX);

						vidsrcIndex.push_back(nVidsrcIndex);
						vidsrcName.push_back(strOldVidSrcName);

						MoveNext();
					}
					for (int i = 0; i< vidsrcIndex.size(); ++i)
					{
						strOldVidSrcName = strOldName+digital_string(vidsrcIndex[i]+1);
						if (strOldVidSrcName == vidsrcName[i])
						{
							strNewVidSrcName = strNewDevName+"_"+digital_string(vidsrcIndex[i]+1);
							vidsrcName[i]=strNewVidSrcName;

							ModDeviceVidSrcName(cInfo.GetDeviceUUID(), vidsrcIndex[i], strNewVidSrcName);

							tIndexName.index = vidsrcIndex[i];
							tIndexName.name = strNewVidSrcName;
							tCapNameNtf.deviceCapIndexNames.push_back(tIndexName);
						}
					}

					DevicePinyinSearchVideoNameSet(cInfo.GetDeviceUUID(), vidsrcIndex, vidsrcName);
				}
			}
			if (!tCapNameNtf.deviceCapIndexNames.empty())
			{
				CTaskMsgTransfer<CDeviceCapIndexNameModNtfReq> cTransNameNtf;
				CDeviceCapIndexNameModNtfReq& cNtfReq=cTransNameNtf.GetMsgObj();
				cNtfReq.SetDomainId(cInfo.GetDomainUUID());
				cNtfReq.SetDeviceCapName(tCapNameNtf);

				PostTaskMsgTransfer2CCtrl(cTransReq, cDevModReq, cNtfReq, cTransNameNtf);
			}
		}

		if (cOldModel.GetDeviceTypeId() != DEVICE_TYPE_DECODER)
		{
			UpdateDevTreeVer(cDevModReq.GetDeviceUUID());

		}
		else
		{
			UpdateDevTreeVer("");
		}

		if (bNeedRollbackLicense)
		{
			DeallocLicense(cOldModel);
			bNeedRollbackLicense = false;
		}
		bHasNtf = (CMS_SUCCESS == DeviceItemQry(cDevModReq.GetDeviceUUID(), cNtf.GetDeviceInfo()));

		SyncAnalyse(enSyncDataDeviceInfo, enSyncOperateMod, cInfo.GetDomainUUID(), &cInfo.GetDeviceUUID(), cInfo.GetDeviceType(), 
			cPropertyFlags.HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev),
			cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev));
	} while (false);

	if (bNeedRollbackLicense)
	{
		DeallocLicense(cNewModel);
	}

	if (bHasNtf)
	{
		RedisDeviceMod(cNtf.GetDeviceInfo(), setChangedFields);
		PostTaskMsgTransfer2CCtrl(cTransReq, cDevModReq, cNtf, cTransNtf);
	}
}

void CDbOpr::DevBatchChg4DevDel( const CDeviceDelReq& cDevDelReq, ECMS_ERRORCODE& errCode )
{
	CTaskMsgTransfer<CDeviceDelReq> cTransReq(cDevDelReq.GetSeqNum(), TKDSIP_TRANS_ID(), TAS_DEVICE_DEL_REQ);
	CTaskMsgTransfer<CDeviceDelNtfReq> cTransNtf;
	cTransReq.SetMsgObj(cDevDelReq);
	CDeviceDelNtfReq &cNtf = cTransNtf.GetMsgObj();
	bool bHasNtf = false;

	CDeviceModelInfo cModel;
	CDeviceInfo cInfo;
	do 
	{
		ENTasErrno enErrno = DeviceItemQry(cDevDelReq.GetDeviceUUID(), cInfo);
		if (CMS_SUCCESS != enErrno)
		{
			errCode = enErrno;
			break;
		}
		if (cInfo.GetDomainUUID() != CurrentDomainId())
		{
			errCode = ERR_TAS_INVALID_OPERATION;
			break;
		}
		enErrno = DeviceModelItemQry(cInfo.GetDeviceModel(), cModel);
		if (CMS_SUCCESS != enErrno)
		{
			errCode = enErrno;
			break;
		}

		UpdateDevTreeVer(cDevDelReq.GetDeviceUUID());

		enErrno = DeviceDelImp(cDevDelReq.GetDeviceUUID());
		if (CMS_SUCCESS != enErrno)
		{
			break;
		}
		if (m_cDbEngine.GetAffectedRowCount() == 0)
		{
			enErrno = ERR_TAS_RECORD_NOT_EXISTS;
			break;
		}

		DeallocLicense(cModel);

		bHasNtf = true;
		cNtf.SetDeviceUUID(cDevDelReq.GetDeviceUUID());
		cNtf.SetDomainId(cInfo.GetDomainUUID());
		cNtf.SetCreater(cInfo.GetCreater());

		SyncAnalyse(enSyncDataDeviceInfo, enSyncOperateDel, cInfo.GetDomainUUID(), &cInfo.GetDeviceUUID(), cInfo.GetDeviceType(), 
			cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev),
			cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev));
	} while (false);

	if (errCode == CMS_SUCCESS)
	{
		if (cModel.GetDeviceTypeId() != DEVICE_TYPE_DECODER)
		{
			UpdateDevTreeVer(cDevDelReq.GetDeviceUUID());
		}
		else
		{
			UpdateDevTreeVer("");
		}
	}

	if (bHasNtf)
	{
		RedisDeviceDel(cInfo);
		PostTaskMsgTransfer2CCtrl(cTransReq, cDevDelReq, cNtf, cTransNtf);
	}
}

void CDbOpr::DevBatchChg4VidSrcMod( const CDeviceCapIndexNameSetReq& cDevCapIdxNameSetReq, ECMS_ERRORCODE& errCode)
{
	CTaskMsgTransfer<CDeviceCapIndexNameSetReq> cTransReq(cDevCapIdxNameSetReq.GetSeqNum(), TKDSIP_TRANS_ID(), TAS_DEVICE_CAP_INDEX_NAME_SET_REQ);
	cTransReq.SetMsgObj(cDevCapIdxNameSetReq);
    CTaskMsgTransfer<CDeviceCapIndexNameModNtfReq> cTransNtf;
    CDeviceCapIndexNameModNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf=false;
	const TDeviceCapName& tCapName = cDevCapIdxNameSetReq.GetDeviceCapName();
    do 
    {
        CDeviceInfo cDevInfo;
        ENTasErrno enDevQryErr = DeviceItemQry(tCapName.deviceId, cDevInfo);
        if (CMS_SUCCESS != enDevQryErr)
        {
           errCode = enDevQryErr;
            break;
        }

		/* 需求要求上级能修改下级的经纬度
        if (cDevInfo.GetDomainUUID() != CurrentDomainId())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }
		*/

        errCode = DeviceCapIndexNameItemSet(tCapName);
        if (errCode != CMS_SUCCESS)
        {
            break;
        }

        bHasNtf=true;
		cNtf.SetDomainId(cDevInfo.GetDomainUUID());

		TDeviceCapName tNtfCapName=tCapName;
		tNtfCapName.devDomainId = cDevInfo.GetDomainUUID();
        cNtf.SetDeviceCapName(tNtfCapName);

        for (u32 i = 0; i < tCapName.deviceCapIndexNames.size(); ++i)
        {
            SyncAnalyse(enSyncDataDeviceCapIndexName, enSyncOperateMod, CurrentDomainId(), 
                &tCapName.deviceId, tCapName.capId, tCapName.deviceCapIndexNames[i].index,
                cDevInfo.GetDeviceType());
        }

        UpdateDevTreeVer(tCapName.deviceId);
    } while (false);

    if (bHasNtf)
	{
		RedisVidSrcAliasOrGpsSet(tCapName);
		PostTaskMsgTransfer2CCtrl(cTransReq, cDevCapIdxNameSetReq, cNtf, cTransNtf);
	}
}

void CDbOpr::DevBatchChg4DevGrpDevChg( const CTaskMsgTransfer<CDeviceGroupDeviceAddReq> *pcMsg, ECMS_ERRORCODE& errCode )
{
	CDeviceGroupDeviceAddReq cReq = pcMsg->GetMsgObj();
	CTaskMsgTransfer<CDeviceGroupDeviceAddNtfReq> cTransNtf;
	CDeviceGroupDeviceAddNtfReq &cNtf = cTransNtf.GetMsgObj();
	bool bHasNtf = false;

	const string& strDevTreeId=cReq.GetDeviceGroupDeviceInfo().GetDevDevTreeId();
	const string& strGrpTreeId=cReq.GetDeviceGroupDeviceInfo().GetGrpDevTreeId();
	/*
	 1、同一设备树内操作（剪切）：从源组删除，目的组添加
	 2、不同设备树之间操作（复制）：直接在目的组添加
	 3、目前自定义设备树只支持2（不同设备树直接的操作）
	*/
	bool bSystemDevTree=(strDevTreeId.empty()&&strGrpTreeId.empty());
	do 
	{
		const CDeviceGroupDeviceInfo &cInfo = cReq.GetDeviceGroupDeviceInfo();

		if (!strDevTreeId.empty()&&strGrpTreeId.empty())
		{
			errCode = ERR_TAS_INVALID_OPERATION;
			break;
		}

		CDeviceGroupInfo cDevGrpInfo;
		errCode = DevGrpItemQry(cInfo.GetDeviceGroupID(), cDevGrpInfo, strGrpTreeId);
		if (CMS_SUCCESS != errCode)
		{
			break;
		}

		CDeviceInfo cDevInfo;
		errCode = DeviceItemQry(cInfo.GetDeviceID(), cDevInfo);
		if (CMS_SUCCESS != errCode)
		{
			break;
		}

		if (cDevGrpInfo.GetDomainUUID() != cDevInfo.GetDomainUUID() || cDevGrpInfo.GetDomainUUID() != CurrentDomainId())
		{
			errCode = ERR_TAS_INVALID_OPERATION;
			break;
		}

		if (cDevGrpInfo.IsAuthorizationDevGrp())
		{
			// 跨域授权组下不允许手工添加设备
			errCode = ERR_TAS_INVALID_OPERATION;
			break;
		}

		// 设备带设备组入网，不需要取消划归，提高效率 [11/23/2015 pangubing]
		if (bSystemDevTree)
		{
			// 将设备从源组中移除之前先更新原拥有者的设备树版本信息（否则过后设备属于新的拥有者了，就没有办法更新原拥有者的版本信息了）
			UpdateDevTreeVer(cInfo.GetDeviceID());

			// 先从源组对应的用户划归信息中清除出该设备信息
			AutoRevokeDevice(pcMsg, cReq, cInfo.GetDeviceID(), cInfo.GetDeviceCapIndexs(), string());
		}

		errCode = DevGrpDevItemAdd(cInfo,false,strGrpTreeId,strDevTreeId);
		if (errCode != CMS_SUCCESS)
		{
			break;
		}

		if (bSystemDevTree)
		{
			// 再根据目标组添加新的用户划归信息
			AutoAssignDevice(pcMsg, cReq, cInfo.GetDeviceID(), cInfo.GetDeviceCapIndexs(), cInfo.GetDeviceGroupID());
		
			for (u32 i = 0; i < cInfo.GetDeviceCapIndexs().size(); ++i)
			{
				for (u32 j = 0; j < cInfo.GetDeviceCapIndexs()[i].indexs.size(); ++j)
				{
					SyncAnalyse(enSyncDataDeviceGroup2Dev, enSyncOperateAdd, CurrentDomainId(), 
						&cInfo.GetDeviceGroupID(),
						&cInfo.GetDeviceID(),
						cInfo.GetDeviceCapIndexs()[i].id, 
						cInfo.GetDeviceCapIndexs()[i].indexs[j], cDevInfo.GetDeviceType());
				}
			}

			UpdateDevTreeVer(cInfo.GetDeviceID());

			bHasNtf = true;
			cNtf.SetDeviceGroupDeviceInfo(cInfo);
			cNtf.GetDeviceGroupDeviceInfo().SetDomainId(cDevInfo.GetDomainUUID());
		}
		else
		{
			UpdateDevTreeVer("",strGrpTreeId);
		}

	} while (false);
	
	if (bHasNtf)
	{
		PostTaskMsgTransfer2CCtrl4LargeNtf(CDeviceGroupDeviceAddNtfReq, GetDeviceGroupDeviceInfo, GetDeviceCapIndexs)
	}
}

void CDbOpr::DevBatchChg4DevModelChg( const CTaskMsgTransfer<CDevCapInfoModReq> *pcMsg,ECMS_ERRORCODE& errCode )
{
	const CDevCapInfoModReq &cReq = pcMsg->GetMsgObj();

	CDeviceModelInfo cOldModel, cNewModel;
	s32 nOldVidNum = 0;
	s32 nNewVidNum = 0;
	bool bNeedRollback = false;
	set<TChangedField> setChangedFields;
	do 
	{
		CDeviceInfo cInfo;

		errCode = DeviceItemQry(cReq.GetDeviceUUID(), cInfo);
		if (CMS_SUCCESS != errCode)
		{
			break;
		}

		TDevicePropertyFlags cPropertyFlags = cInfo.GetDevicePropertyFlags();;

		if (cInfo.GetDomainUUID() != CurrentDomainId())
		{
			errCode = ERR_TAS_INVALID_OPERATION;
			break;
		}

		DeviceModelItemQry(cInfo.GetDeviceModel(), cOldModel);
		for (vector<TDeviceCapInfo>::const_iterator it = cOldModel.GetDeviceCapInfo().begin();
			it != cOldModel.GetDeviceCapInfo().end(); ++it)
		{
			if (it->type == DEVICE_CAP_VIDEOSOURCE)
			{
				nOldVidNum = it->num;
				break;
			}
		}
		for (vector<TDeviceCapInfo>::const_iterator it = cReq.GetDeviceModelCapDetail().begin();
			it != cReq.GetDeviceModelCapDetail().end(); ++it)
		{
			if (it->type == DEVICE_CAP_VIDEOSOURCE)
			{
				nNewVidNum = it->num;
				break;
			}
		}

        // 根据请求内容更新型号
		cNewModel = cOldModel;

        bool bFound = false;
        vector<TDeviceCapInfo>::iterator loopCapIt;
		for (int i = 0; i< cReq.GetDeviceModelCapDetail().size(); ++i)
		{
            bFound = false;
            for (loopCapIt = cNewModel.GetDeviceCapInfo().begin(); 
                loopCapIt != cNewModel.GetDeviceCapInfo().end(); ++loopCapIt)
			{
				if (cReq.GetDeviceModelCapDetail()[i].type == loopCapIt->type)
				{
                    // 请求指定num和license为0，直接移除该能力标签
                    if (cReq.GetDeviceModelCapDetail()[i].num == 0 && cReq.GetDeviceModelCapDetail()[i].license == 0)
                    {
                        cNewModel.GetDeviceCapInfo().erase(loopCapIt);
                    }
                    else
                    {
					    loopCapIt->num = cReq.GetDeviceModelCapDetail()[i].num;
					    loopCapIt->license = cReq.GetDeviceModelCapDetail()[i].license;
                    }

                    bFound = true;
                    break;
				}
			}

            // license和num同时为0的能力标签无需补充
            if (!bFound && (cReq.GetDeviceModelCapDetail()[i].num != 0 || cReq.GetDeviceModelCapDetail()[i].license != 0))
            {
                cNewModel.GetDeviceCapInfo().push_back(cReq.GetDeviceModelCapDetail()[i]);
            }
		}
                
		// 检测已存在的设备型号
		string strModelId;
		errCode = GetExistDeviceModelId(CurrentDomainId(), cNewModel.GetDeviceTypeId(), cNewModel.GetDeviceModelName(), true,
			cNewModel.GetManufacturer(), cNewModel.GetCreater(),cNewModel.GetDeviceCapInfo(), strModelId);
        if (errCode != CMS_SUCCESS || strModelId.empty())
        {
		    errCode = GenerateModelName(cNewModel);
		    if (CMS_SUCCESS != errCode)
		    {
			    break;
		    }

		    CDeviceModelAddReq cDevModelAddReq;
		    cDevModelAddReq.SetDeviceModelInfo(cNewModel);
		    CTaskMsgTransfer<CDeviceModelAddReq> cTransDevModelAddReq;
		    cTransDevModelAddReq.SetMsgObj(cDevModelAddReq);
		    cTransDevModelAddReq.SetTransfer(*pcMsg,cDevModelAddReq.GetEventId());
		    strModelId = DeviceModelAdd(&cTransDevModelAddReq,true);
		    if (strModelId.empty())
		    {
			    errCode = ERR_TAS_DBERROR;
			    break;
            }
        }

		cNewModel.Clear();
		DeviceModelItemQry(strModelId, cNewModel);

        // license
		DeallocLicense(cOldModel);
		setChangedFields.insert(TCHANGED_FIELD_DEVMODEL);

		ENTasErrno enErrno = AllocLicense(cNewModel, cInfo.GetCreater());
		if (CMS_SUCCESS != enErrno)
		{
			errCode = enErrno;
            AllocLicense(cOldModel, cInfo.GetCreater());
			break;
		}
		bNeedRollback = true;

		errCode = DeviceModelPreMod(cInfo.GetDeviceUUID(), cOldModel, cNewModel);
		if (CMS_SUCCESS != errCode)
		{
			break;
		}

		if (DEVICE_TYPE_DECODER == cNewModel.GetDeviceTypeId())
		{
			Tv2DecoderUpdate(pcMsg,cInfo.GetDeviceUUID(), cOldModel, cNewModel);
		}


		TSqlString tSql;
		tSql.Format("update %s set %s='%s' where %s='%s'", T_DEVICE,
			F_DEVICE_DEVMODELID,DbStr(strModelId), F_DEVICE_DEVID,DbStr(cReq.GetDeviceUUID()));

		if (!ExecSql(tSql, enExecNoRet))
		{
			errCode = ERR_TAS_DBERROR;
			break;
		}
		bNeedRollback = false;

		if (cOldModel.GetDeviceTypeId() != DEVICE_TYPE_DECODER)
		{
			UpdateDevTreeVer(cReq.GetDeviceUUID());
		}
		else
		{
			UpdateDevTreeVer("");
		}

		CDeviceInfo devInfo;
		if (CMS_SUCCESS == DeviceItemQry(cReq.GetDeviceUUID(), devInfo))
		{
			RedisDeviceMod(devInfo, setChangedFields);
		}

		SyncAnalyse(enSyncDataDeviceInfo, enSyncOperateMod, cInfo.GetDomainUUID(), &cInfo.GetDeviceUUID(), cInfo.GetDeviceType(), 
			cPropertyFlags.HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev),
			cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev));
		
	} while (false);

	if (bNeedRollback)
	{
		DeallocManagePULicense(nNewVidNum - nOldVidNum);
	}
}