#ifndef _KDM_BASE_FILE_READER_H_
#define _KDM_BASE_FILE_READER_H_

//�������⣺
//1 ��ʱ���Ļ�������  wxh 2011-12-15
//  ��ʱ�������ź���1ͬ�����ļ��ӿں�ʱ��ص�����ʹ���ź���2ͬ��������ɵ��ö�ʱ�����ƺͻص�������������

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
    //  ��ASF�ļ��ĳ�ʼ������������ж��Ƿ�ΪASF�ļ�������ȡASF�ļ���ͷ��Ϣ
	virtual u16	Open(IN const char * pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN u32 dwContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL) = 0;

	//  ���ô���֡���ݵĻص�����
	virtual u16	SetCallback(IN u32 dwUser, IN ASFFrameProcCallBack inCallback);

	
	//  ��ȡASF�ļ���������Ϣ
	virtual u16	GetASFFileProPerty(OUT TASFFileProperty * pFileProperty) = 0;	

	//  ��ȡASF�ļ��������ĸ���
	virtual u16	GetStreamTotal(OUT u16 *wStreamCount) = 0;
	
	//�õ���Ӧ�ֶεĳ���
	virtual u16 GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength) = 0;

	//  ����������ȡASF�ļ���ÿ��������Ϣ,���������úó��ȣ�������ڴ�
	virtual u16	GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty) = 0;
		

	//���ÿ�����źͱ���
    virtual u16 SetPlayRate(IN u16 wPlayMode, IN u32 wPlayRate);	

	//  �����������ʼʱ�䣬�첽��ȡASF�ļ���֡����
	//bIsAutoCallBack���Ƿ��Զ��ص����Զ��ص��͵�֡�ص����л�
	//�������Զ��ص����ǵ�֡�ص������������ûص�������������ô˺�����    
	virtual u16	Start(IN u32 dwStartTime = 0, BOOL32 bIsAutoCallBack = true) = 0;
	
	//��֡��ȡ������û����һ��
	virtual u16	GetNextFrame( u32& dwStatus, TStreamDataPayload **ppoFrameData);

	//  ��ͣ�첽��ȡASF�ļ���֡����
	virtual u16	Pause(void);
	//  ����ͣ��λ�������첽��ȡASF�ļ���֡����,���ɵ�֡�ص�ģʽ���ص��Զ��ص�ģʽ
	virtual u16	Resume(void);
	//  ֹͣ�첽��ȡASF�ļ���֡����
	virtual u16	Stop(void);
	//  �ر�ASF�ļ���ȡ�����ͷ���Դ
	virtual u16	Close(void) = 0;
        
private:
    //��ʱ������������
    static void AsfTimerProc(void* param, u32 dwUserData);
    void TimerCallBAck(u32 dwUserData);

    //��ȡ��һ֡��ʱ�䣬�ں����ڲ�ʵ�ֲ���Ҫ���л���,�ɵ����߽��л���
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
