/*****************************************************************************
  ģ����      : 
  �ļ���      : kdmmp4.h
  ����ļ�    : 
  �ļ�ʵ�ֹ���: 
  ����        : ������
  �汾        : V1.0  Copyright(C) 1997-2006 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2011/10/28   1.0         ������      ����
******************************************************************************/

#ifndef _KDM_MP4_FILE_H_
#define _KDM_MP4_FILE_H_

#ifdef _SUPPORT_KCS_CLOUD_
#include "kdmfilecloud.h"
#else
#include "kdmfileinterface.h"
#endif

#include "kdmbasefilereader.h"

const u64 MILLISECONDS = (1000);            // 10 ^ 3
const u64 NANOSECONDS = (1000000000);       // 10 ^ 9
const u64 UNITS = (NANOSECONDS / 100);      // 10 ^ 7


#include <vector>
#include <list>
using namespace std;


class CWriterMovie;

class  CKdmMp4FileWriter : public CKdmBaseFileWriter
{
public:
    CKdmMp4FileWriter();
    virtual ~CKdmMp4FileWriter();
public:    
	//  дASF�ļ��ĳ�ʼ������
	virtual u16	Open(IN LPCSTR pFileName, TKsjHeader * ptKsjHeader = NULL, u32 dwCreateTime = 0);
			
	//  �����úõ�����ӵ��ļ�ͷ��
	virtual u16  AddStream(IN TStreamProperty * pStreamProperty);
	virtual u16  SetMp4MdatSize(u32 dwMdatSize);
	virtual u16  SetMp4BufferSize(u32 dwBufferSize);
	//  ��ʼдASF�ļ������ݲ���
	virtual u16  BeginWriting(void);

	//  ���ղ�ͬ�������д֡����
	virtual u16  WriteSample(IN u8 byStreamNumber, IN u8 * pDataBuffer, IN u32 dwBufferSize,
			        		 IN BOOL32 bIsKeyFrame, IN u32 dwTimeStamp, IN u16 wDuration, 
                             OUT u32* pdwFileSize = NULL);
	
	//  ����д֡����
	virtual u16  EndWriting(void);

	
	//  �ر��ļ����ͷ���Դ�ռ�
	virtual u16  Close(void);    

	//��������ʱ��ʼʱ�䵽¼���һ֡ʱ��
	virtual u16 AddDropFrameTime(u32 DropFrameTime = 0);

	//����moovˢ��Ƶ�ʣ�����Ϊ��Ƶ֡�����
	virtual u16 SetMoovFreq(IN u32 dwFrameInterval = 0);

	//����ˢ����
	virtual u16 UpdateIndex();
	
	//�����ϲ��ӿ�
	virtual u16 FixFile(IN LPCSTR pFileName, IN LPCSTR pMoovName);
private:
    CWriterMovie* m_pMovie;    
	u32 m_dwMoovFreq;
	u32	m_dwSetMoovFreq;
	u32	m_dwFirstFrame;
};



class CReaderMovie;
class CKdmMp4FileReader : public CKdmBaseFileReader
{
public:
    CKdmMp4FileReader();
    virtual ~CKdmMp4FileReader();
    
	virtual u16	CloudOpen(IN TKdmFile_CloudConnectParam* ptCloudConnectParam, IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN void* pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL);
    virtual u16	Open(IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN KD_PTR pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL);
    virtual u16	GetASFFileProPerty(OUT TASFFileProperty * pFileProperty);   

    virtual u16	GetStreamTotal(OUT u16 *wStreamCount);
	virtual u16	CopyRight(OUT  BOOL32 *IsKedacomFile);
	
	//�õ���Ӧ�ֶεĳ���
	virtual u16 GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength);

	//  ����������ȡASF�ļ���ÿ��������Ϣ,���������úó��ȣ�������ڴ�
	virtual u16	GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty);
    virtual u16	Start(IN u32 dwStartTime = 0, BOOL32 bIsAutoCallBack = true, BOOL32 bDragToKeyPoint = FALSE );
    virtual u16	Close(void);
	
	//������һ֡��Ƶ�ؼ�֡��Ӧ֡ʱ���
	virtual u16 GetLastKeyFrameTime(OUT s64 *poLastKeyFrameTime);
	//������һ֡��Ƶ֡��Ӧ֡ʱ���
	virtual u16 GetLastVideoFrameTime(OUT s64 *poLastKeyFrameTime);
private:
    virtual u16 GetNextFrameTime(u32* dwNextFrameTime);
    virtual u16 GetNextFrameData(TStreamDataPayload *ppoFrameData);

private:
    CReaderMovie*  m_pMovie;

#ifdef _SUPPORT_KCS_CLOUD_
public:
	cdb::BlobStor * m_pcBlobStor;
#endif
};

#endif

