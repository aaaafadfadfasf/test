#ifndef _KDM_BASE_FILE_READER_H_
#define _KDM_BASE_FILE_READER_H_

//遗留问题：
//1 定时器的互斥问题  wxh 2011-12-15
//  定时器采用信号量1同步，文件接口和时间回调函数使用信号量2同步，会造成调用定时器控制和回调函数调用死锁

#include "kdmfiletimer.h"
#include "kdmfileinterface.h"
#include "kdmfileautolock.h"

extern u32 g_nKdmFileDebugLevel;
extern u32 g_nKdmFileDebugShow;
void KdmFilePrintStreamProperty(const TStreamProperty* pPerperty);

API void kdmfilehelp();
API void kdmfiledebugon();
API void kdmfiledebugoff();
API void kdmfiletimeron();
API void kdmfiletimeroff();

API void kdmfiledebug(s32 nKdmFileDebugShow);


class CKdmBaseFileWriter : public IKdmFileWriter
{
protected:
    CKdmBaseFileWriter(void){};
    virtual ~CKdmBaseFileWriter(void){};

    CKdmFileSem m_cSemStatus;
};
 
class CKdmBaseFileReader : public IKdmFileReader
{
protected:
	// Constructors and destructor.
	CKdmBaseFileReader(void);
	virtual ~CKdmBaseFileReader(void);
public:    
    //  读ASF文件的初始化操作，完成判断是否为ASF文件，并读取ASF文件的头信息
	virtual u16	Open(IN const char * pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN u32 dwContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL) = 0;

	//  设置处理帧数据的回调函数
	virtual u16	SetCallback(IN u32 dwUser, IN ASFFrameProcCallBack inCallback);

	
	//  获取ASF文件的属性信息
	virtual u16	GetASFFileProPerty(OUT TASFFileProperty * pFileProperty) = 0;	

	//  获取ASF文件所有流的个数
	virtual u16	GetStreamTotal(OUT u16 *wStreamCount) = 0;
	
	//得到对应字段的长度
	virtual u16 GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength) = 0;

	//  根据索引获取ASF文件的每个流的信息,参数需设置好长度，分配好内存
	virtual u16	GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty) = 0;
		

	//设置快放慢放和倍数
    virtual u16 SetPlayRate(IN u16 wPlayMode, IN u32 wPlayRate);	

	//  根据输入的起始时间，异步读取ASF文件的帧数据
	//bIsAutoCallBack，是否自动回调，自动回调和单帧回调可切换
	//不管是自动回调还是单帧回调，都必须设置回调函数，必须调用此函数。    
	virtual u16	Start(IN u32 dwStartTime = 0, BOOL32 bIsAutoCallBack = true) = 0;
	
	//单帧获取函数，没调用一次
	virtual u16	GetNextFrame( u32& dwStatus, TStreamDataPayload **ppoFrameData);

	//  暂停异步读取ASF文件的帧数据
	virtual u16	Pause(void);
	//  由暂停的位置重新异步读取ASF文件的帧数据,或由单帧回调模式返回到自动回调模式
	virtual u16	Resume(void);
	//  停止异步读取ASF文件的帧数据
	virtual u16	Stop(void);
	//  关闭ASF文件读取，并释放资源
	virtual u16	Close(void) = 0;
        
private:
    //定时器的两个函数
    static void AsfTimerProc(void* param, u32 dwUserData);
    void TimerCallBAck(u32 dwUserData);

    //获取下一帧的时间，在函数内部实现不需要进行互斥,由调用者进行互斥
    virtual u16 GetNextFrameTime(u32* dwNextFrameTime) = 0;
    virtual u16 GetNextFrameData(TStreamDataPayload *ppoFrameData) = 0;
protected:    
    ASFFrameProcCallBack m_Callback;
    u32 m_dwUserData;    

    u32 m_bIsCallBack;
    u16 m_wStatus;
    HTimer m_pTimer;
    
    u16 m_wPlayMode;
    u32 m_dwPlayRate;

    u32 m_dwStartSystemTime;
    u32 m_dwNextFrameTime;
    u32 m_dwStartFrameTime;
    u32 m_dwCurrentFrameTime;
    float m_lfFrameElapsed;    

    TStreamDataPayload m_tStreamDataPayload;
protected:
    CKdmFileSem m_cSemStatus;
};

#endif
