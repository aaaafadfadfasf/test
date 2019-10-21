#ifndef __CMU_UTILITY_H__
#define __CMU_UTILITY_H__

#include "cmu_struct.h"

struct TMediaDescType_EqualTo
{ 
public:
	TMediaDescType_EqualTo(const string& strMeidaType)
		:m_strMeidaType(strMeidaType) 
	{ 
	} 

	bool operator () (const TMediaDesc& tMediaDesc) const 
	{ 
		if (tMediaDesc.GetMediaType() == m_strMeidaType)
		{
			return true;
		}
		else 
		{
			return false;
		}
	} 

private:
	string m_strMeidaType;
};

struct TMediaTransChnType_EqualTo
{ 
public:
	TMediaTransChnType_EqualTo(const string& strTransChnType)
		:m_strTransChnType(strTransChnType) 
	{ 
	} 

	bool operator () (const TMediaTransChannel& tTransChn) const 
	{ 
		if (tTransChn.GetTransChnType() == m_strTransChnType)
		{
			return true;
		}
		else
		{
			return false;
		}
	} 

private:
	string m_strTransChnType;
};

class CMediaDescListAdapter
{
	CMediaDescListAdapter(TMediaDescList&  tMediaDescList)
		:m_tMediaDescList(tMediaDescList)
	{
	}

	void SetMediaFormat(const string& strMediaType, const TMediaFormat& tMediaFormat)
	{
		TMediaDescList::iterator itrMediaDesc = GetMediaDesc(strMediaType);
		itrMediaDesc->SetMediaFormat(tMediaFormat);
	}

	void SetMediaTransProto(const string& strMediaType, const TMediaTransProto& tMediaTransProto)
	{
		TMediaDescList::iterator itrMediaDesc = GetMediaDesc(strMediaType);
		itrMediaDesc->SetMediaTransProto(tMediaTransProto);
	}

	void SetMediaTransProto(const string& strMediaType, const string& strTransport, const string& strLowerTransport, const string& strCastMode)
	{
		TMediaDescList::iterator itrMediaDesc = GetMediaDesc(strMediaType);

		TMediaTransProto& tMediaTransProto = itrMediaDesc->GetMediaTransProto();
		tMediaTransProto.SetTransport(strTransport);
		tMediaTransProto.SetLowerTransport(strLowerTransport);
		tMediaTransProto.SetCastMode(strCastMode);
	}

	//(VIDEO,RTP,AddrList);
	void AddMediaTransChannel(const string& strMediaType, const string& strTransChnType, const TNetAddrList& addrList)
	{
		TMediaDescList::iterator itrMediaDesc = GetMediaDesc(strMediaType);
		if(strTransChnType == MEDIA_TRANS_CHN_RTP || strTransChnType == MEDIA_TRANS_CHN_RTCP)
		{
			TMediaTransProto& tMediaTransProto = itrMediaDesc->GetMediaTransProto();
			tMediaTransProto.SetTransport(MEDIA_TRANS_RTP);

			TMediaTransChannel tMeidaTransChn;
			tMeidaTransChn.SetTransChnType(strTransChnType);
			tMeidaTransChn.SetAddrList(addrList);
			itrMediaDesc->AddMediaTransChn(tMeidaTransChn);
		}
	}


protected:
	TMediaDescList::iterator GetMediaDesc(const string& strMediaType)
	{
		for(TMediaDescList::iterator itr = m_tMediaDescList.begin(); itr!=m_tMediaDescList.end(); itr++)
		{
			if(itr->GetMediaType() == strMediaType)
			{
				return itr;
			}
		}

		TMediaDesc tMediaDesc;
		tMediaDesc.SetMediaType(strMediaType);

		return m_tMediaDescList.insert(m_tMediaDescList.end(), tMediaDesc);
	}

protected:
	TMediaDescList&  m_tMediaDescList;
};

#endif  //#ifndef __CMU_UTILITY_H__

