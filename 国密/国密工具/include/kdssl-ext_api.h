#ifndef __KDCSSL_EXT_API__
#define __KDCSSL_EXT_API__

//"1.1.2.20171023"
//support mult-pthread for szgx
//"1.1.3.20171026"
//support parsing dev name for android & hisi
//"1.1.4.20171027"
//support extshow
//"1.1.5.20171027"
//support save file
//"1.1.6.20171031"
//support show vkek, add close for szgx sm1sm4
//"1.2.1.20171107"
//support save cer & prikey to SKF dev
//"1.2.2.20171108"
//replace udev by netlink
//"1.2.2.20171115"
//add thread for video enc
//"1.2.3.20171121"
//add frame_num for auth info for dec process api
//"1.2.4.20171125"
//support save gop video of ipc
//support set enc & auth of ipc by OSP telnet
//"1.2.5.20171126"
//fix crash problem when browsing H.265 for three min
//add osp cmd to fix vek & iv
//"1.2.6.20171130"
//add pdb file to out dir for winodws
//"1.2.6.20171204"
//rename lock file as .file
//make the default block dev name not include id
//"1.2.7.20171221"
//release x509 & evek handle memory when unused
//use prev vkek when video change
//"1.2.7.20180109"
//check input strlen for sm2 & sm3
//"1.2.7.20180115"
//parse width & height for 265 not include sps nal
//"1.2.8.20180126"
//fix forbidden_zero_bit wrong for svac new nal
//"1.2.9.20180208"
//bug: SDM-00102272 	SDM-00101908
//"1.3.1.20180228"
//add racing code for encrypto stream
//use sdX as the check name for usbkey
//"1.4.1.20180321"
//use standard form save key & cert
//"1.5.1.20180402"
//sign data as der
//"1.5.2.20180404"
//sm2 sign/verify use Z value
//"1.5.3.20180411"
//support goal ca cert
//"1.5.4.20180413"
//fix bug read pub key from pem cer wrong
//"1.5.5.20180418"
//support dec by sign key in hardware
//"1.5.6.20180425"
//support get plat cert from hw
//add lock between destory and process
//"1.5.6.20180503"
//use the 111111 as sign for Coal cont
//"1.5.7.20180524"
//remove the added align data for OFB encrypto
//"1.6.1.20180612"
//add sei nal for hash check frame
//"1.7.1.20180711"
//support for new kdssl lib
//"1.8.1.20181015"
//remove the 0x4 header
//"1.8.2.20181016"
//do asn1 enc for sm2 sign data
//"2.0.1.20181025"
//fix asn len is wrong problem
//"2.0.2.20181114"
//add mem realloc for ofb encrypt
//"2.0.3.20181115"
//support gm tool, add api
//"2.0.4.20181126"
//add hw self test
//add file hash check

#define MODULE_VERSION	"2.0.4.20181126"

#include "kdvtype.h"

#ifdef __cplusplus
extern "C" {
#endif

//for _WIN32
#ifdef _MSC_VER
#define EXT_API __declspec(dllexport)
#else
#define EXT_API extern
#endif

/* ��1 �Ƿ�β��\0 ��*/
#define MAX_EXT_VKEK_LEN				(16)
#define MAX_EXT_VKEK_VERSION_LEN		(32)

typedef void *MCIPHER_HANDLE;

typedef enum ECipherMode
{
	KDC_EXT_ENCRYPT,
	KDC_EXT_DECRYPT,
} ECipherMode;

typedef enum EExtMediaType
{
	KDC_EXT_UNKNOW_VIDEO,
	KDC_EXT_H264,
	KDC_EXT_H265,
	KDC_EXT_SVAC2,
} EExtMediaType;

typedef enum EExtFrameType
{
	KDC_EXT_UNKNOW_FRAME,
	KDC_EXT_I_FRAME,
	KDC_EXT_NOT_I_FRAME,
} EExtFrameType;

/* �ԳƼ����㷨ö�� */
typedef enum ESymmetricCrypto
{
	KDC_EXT_SM1_ECB,
	KDC_EXT_SM1_CBC,
	KDC_EXT_SM1_CFB,
	KDC_EXT_SM1_OFB,
	KDC_EXT_SM4_ECB,
	KDC_EXT_SM4_CBC,
	KDC_EXT_SM4_CFB,
	KDC_EXT_SM4_OFB,
	KDC_EXT_ZUC,
} ESymmetricCrypto;

/* �ǶԳƼ����㷨ö�� */
typedef enum EASymmetricCrypto
{
	KDC_EXT_RSA = 0x1000,
	KDC_EXT_SM2 = 0x2000,
} EASymmetricCrypto;

/* ɢ���㷨ö�� */
typedef enum EHashAlgorithm
{
	KDC_EXT_SM3 	= 0x10000,
	KDC_EXT_SHA1	= 0x20000,
	KDC_EXT_SHA256	= 0x30000,
} EHashAlgorithm;

/* �ǶԳ���Կ����ö�� */
typedef enum EKeyType
{
	KDC_EXT_KEY_PRIVKEY = 0x10,
	KDC_EXT_KEY_PUBKEY,
	KDC_EXT_KEY_PUBKEY_REQ,
	KDC_EXT_KEY_CERT,
} EKeyType;

/* �ǶԳ���Կ��������*/
typedef enum EDataType
{
	KDC_EXT_KEY_FILE_NAME,
	KDC_EXT_KEY_FILE_DATA,
	KDC_EXT_KEY_SRTING,
	KDC_EXT_KEY_ID_IN_HARDWARE,
} EDataType;

/* ����ģ��ID ���� */
typedef enum EHardwareIdType
{
	HARDWARE_ID_AUTO_GET,
	HARDWARE_ID_NULL,
	HARDWARE_ID_IN_DEVICE_ID,
} EHardwareIdType;

/* ֤���������Ϣ���� */
typedef enum ECerHolderInfoType
{
	CER_HOLDER_FOR_NONE,
	CER_HOLDER_FOR_GBXXXX,
} ECerHolderInfoType;

/* VKEK�ṹ�嶨�� */
/* ���Ȳ�����β����\0 */
typedef struct tagExtVkekInfo
{
    u8	m_abyVkek[MAX_EXT_VKEK_LEN + 1];	  
    u32	m_dwVkekLen;
	u8	m_abyVesion[MAX_EXT_VKEK_VERSION_LEN + 1];
    u32	m_dwVesionLen;
} TExtVkekInfom;

/* ��Կ/˽Կ/֤��ṹ�嶨�� */
typedef struct tagPKey
{
    u8					*m_pbyPKeyBuff;
    u32					m_dwPKeyLen;
	EASymmetricCrypto	m_eCryptoType;
	EKeyType			m_eKeyType;
	EDataType			m_eDataType;
	s8					*m_pbyPassword;
} TPKey;

/* MediaCipher����ʱ���� */
typedef struct tagMediaCipherCreateParam
{
	ECipherMode m_eMode;
	u32			dwEngineIdMd; //ɢ���㷨������engine id
	u32			dwEngineIdCipher;//�ԳƼ����㷨������engine id
	u32			dwEngineIdPkey;//�ǶԳƼ����㷨������engine id
	u32			dwEngineIdRand;//�����������engine id
} TMediaCipherCreateParam;

/* MediaCipher�����ò�����Ϣ�ṹ�嶨�� */
typedef struct tagMediaCipherEncParam
{
	BOOL32				m_bEncryptFlag;	//�Ƿ����
	ESymmetricCrypto	m_eEncryptType; //�ԳƼ����㷨	
	BOOL32				m_bAuthFlag;	//�Ƿ�ǩ��
	EASymmetricCrypto	m_eAuthType;	//ǩ���㷨
	EHashAlgorithm		m_eHashType;	//hash�㷨	
	BOOL32				m_bHashOnlyIDR;	//�Ƿ�ֻ��IDR֡��hashǩ��	
	u32					m_dwHashPeriod; //m_bHashOnlyIDRΪfalseʱһ��ǩ����֡��	
} TMediaCipherEncParam;

/* MediaCipher������vek ���²�����Ϣ�ṹ�嶨�� */
typedef struct tagMediaCipherEncVekParam
{
	u32					m_dwGopPeriod; //���ٸ�GOP ����һ��VEK
} TMediaCipherEncVekParam;

/* MediaCipher������camera ��Ϣ */
typedef struct tagMediaCipherEncCameraParam
{
	u8					m_abyCameraIdc[20];
	u8					m_abyCameraId[21];
} TMediaCipherEncCameraParam;

/* MediaCipher�����ò�����Ϣ�ṹ�嶨�� */
typedef struct tagMediaCipherDecParam
{
	BOOL32				m_bAuthFlag;	//�Ƿ���ǩ
} TMediaCipherDecParam;

/* ֡���ݽṹ�� */
typedef struct tagFrameData
{
	EExtMediaType	m_eMediaType;
    u8				*m_pbyFrameBuff;
    u32				m_dwFrameLen;
	/* �����д�������������Ч�ʣ�������ý����Ƿ���I ֡ */
	EExtFrameType	m_eFrameType;
} TFrameData;

/* ���ܷ�����Ϣ */
typedef struct tagExtMediaInfo
{
    BOOL32	m_bIFrame;	//�Ƿ���I֡   
	u16		m_wWidth;	//ͼ����	
	u16		m_wHeight;	//ͼ��߶�	
	/* TODO Profile level */
} TExtMediaInfo;

/* ��֤ǩ��������Ϣ */
typedef struct tagExtAuthInfo
{
	BOOL32	m_bAuthReturn; //�Ƿ���ǩ�����ݷ���
	BOOL32	m_bVerifyResult;//��ǩ���
	s32		m_byVerifyFrameNum;//��֤ͼ���frame_num
	u8		m_byHour; //0~23
	u8		m_byMinute; //0~59
	u8		m_bySecond; //0~59
	u32		m_dwMicrosecond; //0~999999;
	u32		m_dwYear; //2000~2127 or 0
	u8		m_byMonth; //0~12
	u8		m_byDay; //0~31
} TExtAuthInfo;

/* ��ȫ��������Ϣ */
typedef struct tagSecurityParam
{
	/* vkek �汾��Ϣ*/
	u8						m_abyVesion[MAX_EXT_VKEK_VERSION_LEN + 1];
    u32						m_dwVesionLen;

	u8						m_abyCameraIdc[20];
	u8						m_abyCameraId[21];

	TMediaCipherEncParam	m_tParam;		//��Ҫ��ȫ��������Ϣ
} TSecurityParam;

/* ֤���������Ϣ */
typedef struct tagCerHolderInfoGB
{
	EHardwareIdType m_eHardwareIdType;
	/* �豸��ʶ */
	s8	m_abyDeviceId[128 + 1];
	/* �������� */
	s8	m_abyNetType[2 + 1];
	/* 00 */
	s8	m_abyReserved[2 + 1];
	/* ����*/
	s8	m_abyDistrictCounty[2 + 1];
	/* ���� */
	s8	m_abyCity[2 + 1];
	/* ʡ�� */
	s8	m_abyProvince[2 + 1];
	/* ���� */
	s8	m_abyCounty[2 + 1];
} TCerHolderInfoGB;

/* req �ṹ�嶨�� */
typedef struct tagReqInfo
{
	s8						*m_pbyConfFile;
	ECerHolderInfoType		m_eCerHolderInfoType;
	union
	{
		TCerHolderInfoGB	m_tCerHolderInfoGB;
	};
} TReqInfo;

typedef s32 (*PFSECURITYPARAMCB)(TSecurityParam *ptParam, void* pvContext);

/*=====   set  OsApi_RegCommand fun  =====*/
EXT_API void KdSSL_SetOspRegCommond(void* pfFunc);

/*=====    set OsApi_Printf fun    =====*/
EXT_API void KdSSL_SetOspPrintf(void* pfFunc);

/*============================   ͨ�����빦�ܲ��� ============================*/
typedef void *KDSSL_MD_CTX;
typedef void *KDSSL_CIPHER_CTX;
typedef void *KDSSL_PKEY_CTX;

/*=====    ��ʼ��    =====*/
EXT_API s32 KdSSL_init_crypto(u32 reserved);

/*=====   create req & priv key    =====*/
EXT_API s32 KdSSL_GenKeyPair(TPKey *ptPriKey, TPKey *ptPubKey, TReqInfo *ptReqInfo);

/*=====   import req & priv key    =====*/
EXT_API s32 KdSSL_ImportHwEncryptKeyPair(TPKey *ptPriKey, TPKey *ptPubKey, u32 dwEngineId);

/*=====   create req & priv key    =====*/
EXT_API s32 KdSSL_GenHwSignKeyPair(TPKey *ptPubKey, TReqInfo *ptReqInfo, u32 dwEngineId);

/*=====    �����    =====*/
/* �������� */
EXT_API void KdSSL_RAND_seed(const void *pbyBuf, s32 dwNum);
/* ��������� */
EXT_API s32 KdSSL_RAND_bytes(u8 *pbyBuf, s32 dwNum, u32 dwEngineId);

/*=====    ɢ���㷨    =====*/
/* ������� */
EXT_API KDSSL_MD_CTX KdSSL_MD_CTX_create(void);

/* �����ʼ�� */
EXT_API s32 KdSSL_DigestInit(KDSSL_MD_CTX pCtx, EHashAlgorithm eType, u32 dwEngineId);

/* ����ɢ������ */
EXT_API s32 KdSSL_DigestUpdate(KDSSL_MD_CTX pCtx, const void *pvData, u32 dwCount);

/* ���ɢ�������� */
EXT_API s32 KdSSL_DigestFinal(KDSSL_MD_CTX pCtx, u8 *pbyMd, u32 *pdwSize);

/* ���پ�� */
EXT_API void KdSSL_MD_CTX_destroy(KDSSL_MD_CTX pCtx);

/*=====    �ԳƼ����㷨    =====*/
/* ������� */
EXT_API KDSSL_CIPHER_CTX KdSSL_CIPHER_CTX_new(void);

/* �����ʼ�� */
EXT_API s32 KdSSL_CipherInit_ex(KDSSL_CIPHER_CTX pCtx, ESymmetricCrypto eType, u32 dwEngineId, const u8 *pbyKey, const u8 *pbyIv, BOOL32 bEnc);

/* ���жԳƼ������� */
EXT_API s32 KdSSL_EncryptUpdate(KDSSL_CIPHER_CTX pCtx, u8 *pbyOut, s32 *pdwOutL, const u8 *pbyIn, s32 dwInL);

/* ��öԳƼ��������� */
EXT_API s32 KdSSL_EncryptFinal_ex(KDSSL_CIPHER_CTX pCtx, u8 *pbyOut, s32 *pdwOutL);

/* ���پ�� */
EXT_API void KdSSL_CIPHER_CTX_free(KDSSL_CIPHER_CTX pCtx);

/*=====    �ǶԳƼ����㷨    =====*/
/* ������� */
EXT_API KDSSL_PKEY_CTX KdSSL_PKEY_CTX_new(TPKey *ptPKey, u32 dwEngineId);

/* ���پ�� */
EXT_API void KdSSL_PKEY_CTX_free(KDSSL_PKEY_CTX pCtx);

/* �ǶԳƼ��ܳ�ʼ�� */
EXT_API s32 KdSSL_PKEY_encrypt_init(KDSSL_PKEY_CTX pCtx);

/* ���зǶԳƼ������� */
EXT_API s32 KdSSL_PKEY_encrypt(KDSSL_PKEY_CTX pCtx, u8 *pbyOut, u32 *pdwOutlen, const u8 *pbyIn, u32 dwInLen);

/* �ǶԳƽ��ܳ�ʼ�� */
EXT_API s32 KdSSL_PKEY_decrypt_init(KDSSL_PKEY_CTX pCtx);

/* ���зǶԳƽ������� */
EXT_API s32 KdSSL_PKEY_decrypt(KDSSL_PKEY_CTX pCtx, u8 *pbyOut, u32 *pdwOutlen, const u8 *pbyIn, u32 dwInLen);

/* ǩ����ʼ�� */
EXT_API s32 KdSSL_PKEY_sign_init(KDSSL_PKEY_CTX pCtx);

/* ǩ������ */
EXT_API s32 KdSSL_PKEY_sign(KDSSL_PKEY_CTX pCtx, u8 *pbySig, u32 *pdwSigLen, const u8 *pbyTbs, u32 dwTbsLen);

/* ��ǩ��ʼ�� */
EXT_API s32 KdSSL_PKEY_verify_init(KDSSL_PKEY_CTX pCtx);

/* ��ǩ���� */
EXT_API s32 KdSSL_PKEY_verify(KDSSL_PKEY_CTX pCtx, const u8 *pbySig, u32 dwSigLen, const u8 *pbyTbs, u32 dwTbsLen);

/*=====   hash + ǩ����ǩ    =====*/
/* �����ʼ�� */
EXT_API s32 KdSSL_DigestSignInit(KDSSL_MD_CTX pCtx, TPKey *ptPKey, EHashAlgorithm eType, u32 dwEngineId);

/* ǩ������ */
EXT_API s32 KdSSL_DigestSignUpdate(KDSSL_MD_CTX pCtx, const void *pvData, u32 dwCount);

/* �����ʼ�� */
EXT_API s32 KdSSL_DigestSignFinal(KDSSL_MD_CTX pCtx, u8 *pbySig, u32 *pdwSigLen);

/* ����ɢ������ */
EXT_API s32 KdSSL_DigestVerifyInit(KDSSL_MD_CTX pCtx, TPKey *ptPKey, EHashAlgorithm eType, u32 dwEngineId);

/* ����ɢ������ */
EXT_API s32 KdSSL_DigestVerifyUpdate(KDSSL_MD_CTX pCtx, const void *pvData, u32 dwCount);

/* ��ǩ���� */
EXT_API s32 KdSSL_DigestVerifyFinal(KDSSL_MD_CTX pCtx, const u8 *pbySig, u32 dwSigLen);

/*============================ ��Ƶ�������ܽ���ǩ����ǩ���� ============================*/
/* ����Media Cipher�ľ�� */
EXT_API MCIPHER_HANDLE MediaCipherCreate(TMediaCipherCreateParam *ptParam);

/* ����Media Cipher�ľ�� */
EXT_API void MediaCipherDestory(MCIPHER_HANDLE pHandle);

/* ����VKEK */
EXT_API s32 MediaCipherEncUpdateVkek(MCIPHER_HANDLE pHandle, TExtVkekInfom *ptVkek);

/* ����Media Cipher���ܲ��� */
EXT_API s32 MediaCipherEncSetParam(MCIPHER_HANDLE pHandle, TMediaCipherEncParam *ptParam);

/* ��ȡMedia Cipher���ܲ��� */
EXT_API s32 MediaCipherEncGetParam(MCIPHER_HANDLE pHandle, TMediaCipherEncParam *ptParam);

/* ����Media Cipher vek ���²��� */
EXT_API s32 MediaCipherEncSetVekParam(MCIPHER_HANDLE pHandle, TMediaCipherEncVekParam *ptParam);

/* ��ȡMedia Cipher vek ���²��� */
EXT_API s32 MediaCipherEncGetVekParam(MCIPHER_HANDLE pHandle, TMediaCipherEncVekParam *ptParam);

/* ����Media Cipher Camera ���²��� */
EXT_API s32 MediaCipherEncSetCameraParam(MCIPHER_HANDLE pHandle, TMediaCipherEncCameraParam *ptParam);

/* ��ȡMedia Cipher Camera ���²��� */
EXT_API s32 MediaCipherEncGetCameraParam(MCIPHER_HANDLE pHandle, TMediaCipherEncCameraParam *ptParam);

/* ����ǩ��˽Կ */
EXT_API s32 MediaCipherEncUpdatePriKey(MCIPHER_HANDLE pHandle, TPKey *ptPriKey);

/* ���ܴ��� */
EXT_API s32 MediaCipherEncProcess(MCIPHER_HANDLE pHandle, TFrameData *ptIn, TFrameData *ptOut, TFrameData *ptInExt);

/* ����VKEK */
EXT_API s32 MediaCipherDecUpdateVkek(MCIPHER_HANDLE pHandle, TExtVkekInfom *ptVkek);

/* ����Media Cipher �����ò��� */
EXT_API s32 MediaCipherDecSetParam(MCIPHER_HANDLE pHandle, TMediaCipherDecParam *ptParam);

/* ��ȡMedia Cipher �����ò��� */
EXT_API s32 MediaCipherDecGetParam(MCIPHER_HANDLE pHandle, TMediaCipherDecParam *ptParam);

/* ������ǩ��Կ */
EXT_API s32 MediaCipherDecUpdatePubKey(MCIPHER_HANDLE pHandle, TPKey *ptPubKey);

/* ���ܴ��� */
EXT_API s32 MediaCipherDecProcess(MCIPHER_HANDLE pHandle, TFrameData *ptIn, TFrameData *ptOut, TExtMediaInfo *ptInfo, TExtAuthInfo *ptAuthInfo);

/* ���ܰ�ȫ�������ص� */
EXT_API s32 MediaCipherDecSecurityParamCallback(MCIPHER_HANDLE pHandle, PFSECURITYPARAMCB pCallback, void *pvContext);

/* ������µİ�ȫ������ */
EXT_API s32 MediaCipherDecGetSecurityParam(MCIPHER_HANDLE pHandle, TSecurityParam *ptParam);

/*============================   skf ��װ ============================*/
/* ����t��or usb key��Ϣ*/
typedef struct tagSKFExtDevInfo
{
	char abySerialNumber[32];
	u32 m_dwTotalSpace;
	u32 m_dwFreeSpace;
} TSKFExtDevInfo;

/* Ŀǰ�����ϲ��ṩSKF �����д�ļ�*/
EXT_API s32 SKFWriteVerifyCert(u32 dwEngineId, u8 *pbyBuff, u32 dwLen);
EXT_API s32 SKFWriteEncCert(u32 dwEngineId, u8 *pbyBuff, u32 dwLen);
EXT_API s32 SKFWriteConVerifyCert(u32 dwEngineId, u8 *pbyBuff, u32 dwLen);
EXT_API s32 SKFWriteConEncCert(u32 dwEngineId, u8 *pbyBuff, u32 dwLen);

/* ��ü��ܿ���Ӳ����Ϣ */
EXT_API s32 SKFExtGetDevInfo(u32 dwEngineId, TSKFExtDevInfo *ptSKFExtDevInfo);

typedef enum ESKFHwAction
{
	SKF_HW_ADD,
	SKF_HW_REMOVE,
} ESKFHwAction;

/* SKF��ȫ�豸�ṹ�� */
typedef struct tagSKFDiskInfo
{
	u32 dwEngineId; //EngineID��
	s8 abyDiskName[32]; //�̷�
} TSKFDiskInfo;

/* SKF��ȫ�豸���ֻص� */
typedef void (*PFSKFHWCALLBACK)(TSKFDiskInfo *ptDiskInfo, ESKFHwAction eAction, void* pvContext);

/* SKF ����Ӳ���������԰�ȫ�豸��ʼ������ǰ�Ѳ����豸���ᴥ���ص� */
EXT_API s32 FactoryHwInit(PFSKFHWCALLBACK pCallback, void *pvContext);

/* SKF��ȫ�豸��ʼ������ǰ�Ѳ����豸���ᴥ���ص� */
EXT_API s32 SKFHwInit(PFSKFHWCALLBACK pCallback, void *pvContext);

/* ͨ���̷��������豸�ǲ���SKF��ȫ�豸 */
EXT_API BOOL32 SKFDiskCheck(s8 *pbyDiskName);

/* ͨ���̷�id �����豸�ǲ�����������*/
EXT_API BOOL32 SKFIdCheck(u32 dwEngineId);

/* ͨ���̷�����SKF��ȫ�豸�������EngineID�� */
EXT_API s32 SKFDiskOpen(s8 *pbyDiskName, u32 *pdwEngineId);

/* ͨ���̷����ͷ�SKF��ȫ�豸 */
EXT_API s32 SKFDiskRelease(s8 *pbyDiskName);

/* ͨ��EngineId���ͷ�SKF��ȫ�豸 */
EXT_API s32 SKFDiskReleaseById(u32 dwEngineId);

/* ��ȡ��Կ֤�� */
EXT_API s32 SKFDiskOpen(s8 *pbyDiskName, u32 *pdwEngineId);

/* ͨ���̷����ͷ�SKF��ȫ�豸 */
EXT_API s32 SKFDiskRelease(s8 *pbyDiskName);

/* ͨ��EngineId���ͷ�SKF��ȫ�豸 */
EXT_API s32 SKFDiskReleaseById(u32 dwEngineId);

/* ��ȡ��ǩ֤�� */
EXT_API s32 SKFReadVerifyCert(u32 dwEngineId, u8 *pbyBuff, u32 *pdwLen);

/* ��ȡ����֤�� */
EXT_API s32 SKFReadEncCert(u32 dwEngineId, u8 *pbyBuff, u32 *pdwLen);

/* ��ȡ��ǩ֤�� */
EXT_API s32 SKFReadConVerifyCert(u32 dwEngineId, u8 *pbyBuff, u32 *pdwLen);

/* ��ȡ����֤�� */
EXT_API s32 SKFReadConEncCert(u32 dwEngineId, u8 *pbyBuff, u32 *pdwLen);

/* ��ȡ����֤�� */
EXT_API s32 SKFInitDevKedaMode(u32 dwEngineId);

EXT_API int KdSSL_PEM2DER(char *pbyInfile, char *pbyOutfile);
EXT_API int KdSSL_DER2PEM(char *pbyInfile, char *pbyOutfile);

/* ���ܲ���*/
EXT_API int keda_gen_sm2_key(unsigned char *pubkey, unsigned char *prikey);
EXT_API int keda_sm2_sign(unsigned int engine, unsigned char *prikey, unsigned char *in, unsigned int in_len, unsigned char *sign, unsigned int *sign_len);
EXT_API int keda_sm2_verify(unsigned int engine, unsigned char *pubkey, unsigned char *in, unsigned int in_len, unsigned char *sign, unsigned int sign_len);
EXT_API int keda_sm2_enc(unsigned int engine, unsigned char *pubkey, unsigned char *in, unsigned int in_len, unsigned char *out, unsigned int *out_len);
EXT_API int keda_sm2_dec(unsigned int engine, unsigned char *prikey, unsigned char *in, unsigned int in_len, unsigned char *out, unsigned int *out_len);
EXT_API int keda_sm3_hash(unsigned int engine, unsigned char *pubkey, unsigned char *id, unsigned int id_len, unsigned char *dat, unsigned int dat_len, unsigned char *md, unsigned int *md_len);

/* �㷨�Բ���*/
EXT_API s32 SKFSm3SelfTest(u32 dwEngineId);
EXT_API s32 SKFSm4EcbSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm1EcbSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm4OfbSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm1OfbSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm3SignSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm2EncSelfTest(u32 dwEngineId);
EXT_API s32 SKFRandomSelfTest(u32 dwEngineId);

/* �����ļ���sm3 Hash, ��֧��2GB �����ϵ��ļ�*/
EXT_API s32 CalcFileSm3Hash(u8 *pbyFileName, u8 *pbyBase64Hash, u32 *pdwLen);

#ifdef __cplusplus
}
#endif

#endif
