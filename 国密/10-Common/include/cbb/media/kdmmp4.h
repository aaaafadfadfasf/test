/*****************************************************************************
  模块名      : 
  文件名      : kdmmp4.h
  相关文件    : 
  文件实现功能: 
  作者        : 王晓辉
  版本        : V1.0  Copyright(C) 1997-2006 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2011/10/28   1.0         王晓辉      创建
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
	//  写ASF文件的初始化操作
	virtual u16	Open(IN LPCSTR pFileName, TKsjHeader * ptKsjHeader = NULL, u32 dwCreateTime = 0);
			
	//  将设置好的流添加到文件头中
	virtual u16  AddStream(IN TStreamProperty * pStreamProperty);
	virtual u16  SetMp4MdatSize(u32 dwMdatSize);
	virtual u16  SetMp4BufferSize(u32 dwBufferSize);
	//  开始写ASF文件的数据部分
	virtual u16  BeginWriting(void);

	//  按照不同的流序号写帧数据
	virtual u16  WriteSample(IN u8 byStreamNumber, IN u8 * pDataBuffer, IN u32 dwBufferSize,
			        		 IN BOOL32 bIsKeyFrame, IN u32 dwTimeStamp, IN u16 wDuration, 
                             OUT u32* pdwFileSize = NULL);
	
	//  结束写帧数据
	virtual u16  EndWriting(void);

	
	//  关闭文件，释放资源空间
	virtual u16  Close(void);    

	//增加下载时开始时间到录像第一帧时间
	virtual u16 AddDropFrameTime(u32 DropFrameTime = 0);

	//设置moov刷新频率，参数为视频帧间隔数
	virtual u16 SetMoovFreq(IN u32 dwFrameInterval = 0);

	//立即刷索引
	virtual u16 UpdateIndex();
	
	//索引合并接口
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
	
	//得到对应字段的长度
	virtual u16 GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength);

	//  根据索引获取ASF文件的每个流的信息,参数需设置好长度，分配好内存
	virtual u16	GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty);
    virtual u16	Start(IN u32 dwStartTime = 0, BOOL32 bIsAutoCallBack = true, BOOL32 bDragToKeyPoint = FALSE );
    virtual u16	Close(void);
	
	//获得最后一帧视频关键帧对应帧时间戳
	virtual u16 GetLastKeyFrameTime(OUT s64 *poLastKeyFrameTime);
	//获得最后一帧视频帧对应帧时间戳
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

