#ifndef _RS_TAKE_FREE_APP_H_
#define _RS_TAKE_FREE_APP_H_

#include "osp.h"

template <class CAppClass>
class CTakeFreeApp
{
public:
	CTakeFreeApp()
	{
		m_ptAppArray = NULL;
		m_wAppArraySize = 0;
		m_wLastTakeAppId = 0;
	}

	void SetAppArray(CAppClass *pcAppClassArray, u16 wArraySize)
	{
		m_ptAppArray = pcAppClassArray;
		m_wAppArraySize = wArraySize;
	}

	BOOL32 TakeAFreeApp(u16& wAppID)
	{
		if (0 == m_wAppArraySize || NULL == m_ptAppArray)
		{
			return FALSE;
		}

		u16 wStartPos = GetCurrentStartPos();
		u16 wPos = wStartPos;
		
        do
        {
            if(!m_ptAppArray[wPos].IsBusy())
            {
                wAppID = m_ptAppArray[wPos].appId;
                m_wLastTakeAppId = wPos;
                m_ptAppArray[wPos].SetBusy(TRUE);

                return TRUE;
            }
            wPos = GetNextPos(wPos);
        }while((wPos % m_wAppArraySize) != wStartPos);

		return FALSE;
	}

	BOOL32 TakeAFreeAppExceptFirstOne(u16& wAppID)
	{
		if (0 == m_wAppArraySize || NULL == m_ptAppArray)
		{
			return FALSE;
		}

		u16 wStartPos = GetCurrentStartPos();
		u16 wPos = wStartPos;
		
        do
        {
            if(!m_ptAppArray[wPos].IsBusy() && wPos!=0)
            {
                wAppID = m_ptAppArray[wPos].appId;
                m_wLastTakeAppId = wPos;
                m_ptAppArray[wPos].SetBusy(TRUE);
                return TRUE;
            }
            wPos = GetNextPos(wPos);
        }while((wPos % m_wAppArraySize) != wStartPos);

		return FALSE;
	}

	void SetAppBusy(u16 wAppID, BOOL32 bAppBusy)
	{
		for(u16 wIndex = 0; wIndex < m_wAppArraySize; wIndex++)
		{
			if (m_ptAppArray[wIndex].appId == wAppID)
			{
				m_ptAppArray[wIndex].SetBusy(bAppBusy);
				return;
			}
		}
	}

private:

	u16 GetCurrentStartPos()
    {
        return GetNextPos(m_wLastTakeAppId);
    }

    u16 GetNextPos(u16 wPos)
    {
        return ++wPos % m_wAppArraySize;
    }

private:
	CAppClass *m_ptAppArray;
	u16        m_wAppArraySize;
	u16        m_wLastTakeAppId;

};
#endif
