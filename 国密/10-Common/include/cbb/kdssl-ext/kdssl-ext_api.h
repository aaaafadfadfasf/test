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
//"2.0.5.20181210"
//free memory when pub key parse fail
//"2.0.6.20181220"
//fix crash problem in 64bit linux
//add default log size to 4MB
//"2.0.7.20181220"
//fix cert fp is not closed problem
//"3.0.1.20190122"
//first version for kdssl-ext v2.0
//"3.0.2.20190123"
//support xingtang tf card
//"3.0.3.20190125"
//support xt card for a level
//"3.0.4.20190313"
//support sdf device
//"3.0.5.20190318"
//support xt sdf net device
//"3.0.6.20190413"
//set new create nal nal_ref_idc bit as 1
//fix time nal data bug
//return 0 as success for rand function
//"3.0.7.20190404"
//sync xt card mount
//"3.0.8.20190417"
//add log for android
//"3.0.9.20190419"
//use access fun replace xingtang SKF_EnumDev
//"3.0.10.20190423"
//mmcblkXp1 will be considered as mmcblkX
//"3.0.11.20190425"
//add read/write cert for sdf
//"3.1.0.20190426"
//support GB35114 C-level
//"3.1.1.20190428"
//add skf lock for szgx sign/verify
//"3.1.2.20190429"
//fix memory leak
//"3.1.2.20190506"
//fix read cert crash when dec
//"3.1.3.20190507"
//fix gen random files bug for gmtool
//"3.1.4.20190514"
//create new session for each sdf process
//"3.1.5.20190514"
//support sm2 dec by internal prikey
//"3.1.6.20190515"
//first use handle in memory when check dev
//"3.1.7.20190531"
//support apk
//"3.1.8.20190603"
//update skf lib for 3519a
//"3.2.0.20190619"
//update for swxa server
//"3.2.1.20190620"
//for 264 & 265, do sign after encrypt
//"3.2.2.20190626"
//modify dlopen path for x64 linux
//"3.2.3.20190627"
//support sha1 sha256
//"3.2.4.20190703"
//modify p10 subject for 35114
//"3.2.5.20190711"
//fix c-level last marco-block error
//support read cert cn id
//"3.2.6.20190722"
//return err when p-frame not find vkek

#define MODULE_VERSION	"3.2.6.20190722"

#include "kdvtype.h"
#include "kdssl-ext_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

//for _WINDOWS
#ifdef _MSC_VER
#define EXT_API __declspec(dllexport)
#else
#define EXT_API extern
#endif

/* 加1 是放尾部\0 用*/
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

/* 对称加密算法枚举 */
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

/* 非对称加密算法枚举 */
typedef enum EASymmetricCrypto
{
	KDC_EXT_RSA = 0x1000,
	KDC_EXT_SM2 = 0x2000,
} EASymmetricCrypto;

/* 散列算法枚举 */
typedef enum EHashAlgorithm
{
	KDC_EXT_SM3 	= 0x10000,
	KDC_EXT_SHA1	= 0x20000,
	KDC_EXT_SHA256	= 0x30000,
} EHashAlgorithm;

/* 非对称密钥类型枚举 */
typedef enum EKeyType
{
	KDC_EXT_KEY_PRIVKEY = 0x10,
	KDC_EXT_KEY_PUBKEY,
	KDC_EXT_KEY_PUBKEY_REQ,
	KDC_EXT_KEY_CERT,
} EKeyType;

/* 非对称密钥数据类型*/
typedef enum EDataType
{
	KDC_EXT_KEY_FILE_NAME,
	KDC_EXT_KEY_FILE_DATA,
	KDC_EXT_KEY_SRTING,
	KDC_EXT_KEY_ID_IN_HARDWARE,
} EDataType;

/* 证书持有者信息类型 */
typedef enum ECerHolderInfoType
{
	CER_HOLDER_FOR_NONE,
	CER_HOLDER_FOR_GB35114,
	CER_HOLDER_FOR_GM0015,
} ECerHolderInfoType;

/* VKEK结构体定义 */
/* 长度不包括尾部的\0 */
typedef struct tagExtVkekInfo
{
    u8	m_abyVkek[MAX_EXT_VKEK_LEN + 1];	  
    u32	m_dwVkekLen;
	u8	m_abyVesion[MAX_EXT_VKEK_VERSION_LEN + 1];
    u32	m_dwVesionLen;
} TExtVkekInfom;

/* 公钥/私钥/证书结构体定义 */
typedef struct tagPKey
{
    u8					*m_pbyPKeyBuff;
    u32					m_dwPKeyLen;
	EASymmetricCrypto	m_eCryptoType;
	EKeyType			m_eKeyType;
	EDataType			m_eDataType;
	s8					*m_pbyPassword;
} TPKey;

/* MediaCipher创建时参数 */
typedef struct tagMediaCipherCreateParam
{
	ECipherMode m_eMode;
	u32			dwEngineIdMd; //散列算法加密用engine id
	u32			dwEngineIdCipher;//对称加密算法加密用engine id
	u32			dwEngineIdPkey;//非对称加密算法加密用engine id
	u32			dwEngineIdRand;//随机数生成用engine id
} TMediaCipherCreateParam;

/* MediaCipher加密用参数信息结构体定义 */
typedef struct tagMediaCipherEncParam
{
	BOOL32				m_bEncryptFlag;	//是否加密
	ESymmetricCrypto	m_eEncryptType; //对称加密算法	
	BOOL32				m_bAuthFlag;	//是否签名
	EASymmetricCrypto	m_eAuthType;	//签名算法
	EHashAlgorithm		m_eHashType;	//hash算法	
	BOOL32				m_bHashOnlyIDR;	//是否只对IDR帧做hash签名	
	u32					m_dwHashPeriod; //m_bHashOnlyIDR为false时一次签名的帧数	
} TMediaCipherEncParam;

/* MediaCipher加密用vek 更新参数信息结构体定义 */
typedef struct tagMediaCipherEncVekParam
{
	u32					m_dwGopPeriod; //多少个GOP 更新一次VEK
} TMediaCipherEncVekParam;

/* MediaCipher加密用camera 信息 */
typedef struct tagMediaCipherEncCameraParam
{
	u8					m_abyCameraIdc[20];
	u8					m_abyCameraId[21];
} TMediaCipherEncCameraParam;

/* MediaCipher解密用参数信息结构体定义 */
typedef struct tagMediaCipherDecParam
{
	BOOL32				m_bAuthFlag;	//是否验签
} TMediaCipherDecParam;

/* 帧数据结构体 */
typedef struct tagFrameData
{
	EExtMediaType	m_eMediaType;
    u8				*m_pbyFrameBuff;
    u32				m_dwFrameLen;
	/* 如果填写则不做解析，提高效率，否则调用解析是否是I 帧 */
	EExtFrameType	m_eFrameType;
} TFrameData;

/* 解密返回信息 */
typedef struct tagExtMediaInfo
{
    BOOL32	m_bIFrame;	//是否是I帧   
	u16		m_wWidth;	//图像宽度	
	u16		m_wHeight;	//图像高度	
	/* TODO Profile level */
} TExtMediaInfo;

/* 验证签名返回信息 */
typedef struct tagExtAuthInfo
{
	BOOL32	m_bAuthReturn; //是否有签名数据返回
	BOOL32	m_bVerifyResult;//验签结果
	s32		m_byVerifyFrameNum;//认证图像的frame_num
	u8		m_byHour; //0~23
	u8		m_byMinute; //0~59
	u8		m_bySecond; //0~59
	u32		m_dwMicrosecond; //0~999999;
	u32		m_dwYear; //2000~2127 or 0
	u8		m_byMonth; //0~12
	u8		m_byDay; //0~31
} TExtAuthInfo;

/* 安全参数集信息 */
typedef struct tagSecurityParam
{
	/* vkek 版本信息*/
	u8						m_abyVesion[MAX_EXT_VKEK_VERSION_LEN + 1];
    u32						m_dwVesionLen;

	u8						m_abyCameraIdc[20];
	u8						m_abyCameraId[21];

	TMediaCipherEncParam	m_tParam;		//主要安全参数集信息
} TSecurityParam;

typedef struct tagCerHolderInfoGM0015
{
	/* 用户名 */
	s8	m_abyUsrName[256];

	/* 部门名 */
	s8	m_abyDepName[256];

	/* 组织名 */
	s8	m_abyOrgName[256];

	/* 省份 */
	s8	m_abyProvince[128];
} TCerHolderInfoGM0015;

/* 证书持有者信息 +1是方便strcpy后面多个尾0 */
typedef struct tagCerHolderInfoGB35114
{
	/* 设备标识 */
	s8	m_abyDeviceId[128];

	/* 密码模块id */
	s8	m_abyModuleId[128];

	/* 网络类型 01:公安信息网   02：视频专网 */
	s8	m_abyNetType[2 + 1];

	/* 00 */
	s8	m_abyReserved[2 + 1];

	/* 区县*/
	s8	m_abyDistrictCounty[2 + 1];

	/* 地市 */
	s8	m_abyCity[2 + 1];

	/* 省份 */
	s8	m_abyProvince[2 + 1];

	/* 国家 */
	s8	m_abyCounty[2 + 1];
} TCerHolderInfoGB35114;

/* req 结构体定义 */
typedef struct tagReqInfo
{
	s8							*m_pbyConfFile;
	ECerHolderInfoType			m_eCerHolderInfoType;
	union
	{
		TCerHolderInfoGB35114	m_tCerHolderInfoGB35114;
		TCerHolderInfoGM0015	m_tCerHolderInfoGM0015;
	};
} TReqInfo;

typedef s32 (*PFSECURITYPARAMCB)(TSecurityParam *ptParam, void* pvContext);

/*=====   set  OsApi_RegCommand fun  =====*/
EXT_API void KdSSL_SetOspRegCommond(void* pfFunc);

/*=====    set OsApi_Printf fun    =====*/
EXT_API void KdSSL_SetOspPrintf(void* pfFunc);

/*============================   通用密码功能部分 ============================*/
typedef void *KDSSL_MD_CTX;
typedef void *KDSSL_CIPHER_CTX;
typedef void *KDSSL_PKEY_CTX;

/*=====    初始化    =====*/
EXT_API s32 KdSSL_init_crypto(THwSetInfo *set);

/*=====   create req & priv key    =====*/
EXT_API s32 KdSSL_GenKeyPair(TPKey *ptPriKey, TPKey *ptPubKey, TReqInfo *ptReqInfo, u32 dwEngineId);

/*=====   import req & priv key    =====*/
EXT_API s32 KdSSL_ImportHwEncryptKeyPair(TPKey *ptPriKey, TPKey *ptPubKey, u32 dwEngineId);

/*=====   create req & priv key    =====*/
EXT_API s32 KdSSL_GenHwSignKeyPair(TPKey *ptPubKey, TReqInfo *ptReqInfo, u32 dwEngineId);

/*=====    随机数    =====*/
/* 种子生成 */
EXT_API void KdSSL_RAND_seed(const void *pbyBuf, s32 dwNum);
/* 随机数生成 */
EXT_API s32 KdSSL_RAND_bytes(u8 *pbyBuf, s32 dwNum, u32 dwEngineId);

/*=====    散列算法    =====*/
/* 创建句柄 */
EXT_API KDSSL_MD_CTX KdSSL_MD_CTX_create(void);

/* 句柄初始化 */
EXT_API s32 KdSSL_DigestInit(KDSSL_MD_CTX pCtx, EHashAlgorithm eType, u32 dwEngineId);

/* 进行散列运算 */
EXT_API s32 KdSSL_DigestUpdate(KDSSL_MD_CTX pCtx, const void *pvData, u32 dwCount);

/* 获得散列运算结果 */
EXT_API s32 KdSSL_DigestFinal(KDSSL_MD_CTX pCtx, u8 *pbyMd, u32 *pdwSize);

/* 销毁句柄 */
EXT_API void KdSSL_MD_CTX_destroy(KDSSL_MD_CTX pCtx);

/*=====    对称加密算法    =====*/
/* 创建句柄 */
EXT_API KDSSL_CIPHER_CTX KdSSL_CIPHER_CTX_new(void);

/* 句柄初始化 */
EXT_API s32 KdSSL_CipherInit_ex(KDSSL_CIPHER_CTX pCtx, ESymmetricCrypto eType, u32 dwEngineId, const u8 *pbyKey, const u8 *pbyIv, BOOL32 bEnc);

/* 进行对称加密运算 */
EXT_API s32 KdSSL_EncryptUpdate(KDSSL_CIPHER_CTX pCtx, u8 *pbyOut, s32 *pdwOutL, const u8 *pbyIn, s32 dwInL);

/* 获得对称加密运算结果 */
EXT_API s32 KdSSL_EncryptFinal_ex(KDSSL_CIPHER_CTX pCtx, u8 *pbyOut, s32 *pdwOutL);

/* 销毁句柄 */
EXT_API void KdSSL_CIPHER_CTX_free(KDSSL_CIPHER_CTX pCtx);

/*=====    非对称加密算法    =====*/
/* 创建句柄 */
EXT_API KDSSL_PKEY_CTX KdSSL_PKEY_CTX_new(TPKey *ptPKey, u32 dwEngineId);

/* 销毁句柄 */
EXT_API void KdSSL_PKEY_CTX_free(KDSSL_PKEY_CTX pCtx);

/* 非对称加密初始化 */
EXT_API s32 KdSSL_PKEY_encrypt_init(KDSSL_PKEY_CTX pCtx);

/* 进行非对称加密运算 */
EXT_API s32 KdSSL_PKEY_encrypt(KDSSL_PKEY_CTX pCtx, u8 *pbyOut, u32 *pdwOutlen, const u8 *pbyIn, u32 dwInLen);

/* 非对称解密初始化 */
EXT_API s32 KdSSL_PKEY_decrypt_init(KDSSL_PKEY_CTX pCtx);

/* 进行非对称解密运算 */
EXT_API s32 KdSSL_PKEY_decrypt(KDSSL_PKEY_CTX pCtx, u8 *pbyOut, u32 *pdwOutlen, const u8 *pbyIn, u32 dwInLen);

/* 签名初始化 */
EXT_API s32 KdSSL_PKEY_sign_init(KDSSL_PKEY_CTX pCtx);

/* 签名运算 */
EXT_API s32 KdSSL_PKEY_sign(KDSSL_PKEY_CTX pCtx, u8 *pbySig, u32 *pdwSigLen, const u8 *pbyTbs, u32 dwTbsLen);

/* 验签初始化 */
EXT_API s32 KdSSL_PKEY_verify_init(KDSSL_PKEY_CTX pCtx);

/* 验签运算 */
EXT_API s32 KdSSL_PKEY_verify(KDSSL_PKEY_CTX pCtx, const u8 *pbySig, u32 dwSigLen, const u8 *pbyTbs, u32 dwTbsLen);

/*=====   hash + 签名验签    =====*/
/* 句柄初始化 */
EXT_API s32 KdSSL_DigestSignInit(KDSSL_MD_CTX pCtx, TPKey *ptPKey, EHashAlgorithm eType, u32 dwEngineId);

/* 签名运算 */
EXT_API s32 KdSSL_DigestSignUpdate(KDSSL_MD_CTX pCtx, const void *pvData, u32 dwCount);

/* 句柄初始化 */
EXT_API s32 KdSSL_DigestSignFinal(KDSSL_MD_CTX pCtx, u8 *pbySig, u32 *pdwSigLen);

/* 进行散列运算 */
EXT_API s32 KdSSL_DigestVerifyInit(KDSSL_MD_CTX pCtx, TPKey *ptPKey, EHashAlgorithm eType, u32 dwEngineId);

/* 进行散列运算 */
EXT_API s32 KdSSL_DigestVerifyUpdate(KDSSL_MD_CTX pCtx, const void *pvData, u32 dwCount);

/* 验签运算 */
EXT_API s32 KdSSL_DigestVerifyFinal(KDSSL_MD_CTX pCtx, const u8 *pbySig, u32 dwSigLen);

/*============================ 视频码流加密解密签名验签部分 ============================*/
/* 创建Media Cipher的句柄 */
EXT_API MCIPHER_HANDLE MediaCipherCreate(TMediaCipherCreateParam *ptParam);

/* 销毁Media Cipher的句柄 */
EXT_API void MediaCipherDestory(MCIPHER_HANDLE pHandle);

/* 更新VKEK */
EXT_API s32 MediaCipherEncUpdateVkek(MCIPHER_HANDLE pHandle, TExtVkekInfom *ptVkek);

/* 设置Media Cipher加密参数 */
EXT_API s32 MediaCipherEncSetParam(MCIPHER_HANDLE pHandle, TMediaCipherEncParam *ptParam);

/* 获取Media Cipher加密参数 */
EXT_API s32 MediaCipherEncGetParam(MCIPHER_HANDLE pHandle, TMediaCipherEncParam *ptParam);

/* 设置Media Cipher vek 更新参数 */
EXT_API s32 MediaCipherEncSetVekParam(MCIPHER_HANDLE pHandle, TMediaCipherEncVekParam *ptParam);

/* 获取Media Cipher vek 更新参数 */
EXT_API s32 MediaCipherEncGetVekParam(MCIPHER_HANDLE pHandle, TMediaCipherEncVekParam *ptParam);

/* 设置Media Cipher Camera 更新参数 */
EXT_API s32 MediaCipherEncSetCameraParam(MCIPHER_HANDLE pHandle, TMediaCipherEncCameraParam *ptParam);

/* 获取Media Cipher Camera 更新参数 */
EXT_API s32 MediaCipherEncGetCameraParam(MCIPHER_HANDLE pHandle, TMediaCipherEncCameraParam *ptParam);

/* 更新签名私钥 */
EXT_API s32 MediaCipherEncUpdatePriKey(MCIPHER_HANDLE pHandle, TPKey *ptPriKey);

/* 加密处理 */
EXT_API s32 MediaCipherEncProcess(MCIPHER_HANDLE pHandle, TFrameData *ptIn, TFrameData *ptOut, TFrameData *ptInExt);

/* 更新VKEK */
EXT_API s32 MediaCipherDecUpdateVkek(MCIPHER_HANDLE pHandle, TExtVkekInfom *ptVkek);

/* 设置Media Cipher 解密用参数 */
EXT_API s32 MediaCipherDecSetParam(MCIPHER_HANDLE pHandle, TMediaCipherDecParam *ptParam);

/* 获取Media Cipher 解密用参数 */
EXT_API s32 MediaCipherDecGetParam(MCIPHER_HANDLE pHandle, TMediaCipherDecParam *ptParam);

/* 更新验签公钥 */
EXT_API s32 MediaCipherDecUpdatePubKey(MCIPHER_HANDLE pHandle, TPKey *ptPubKey);

/* 解密处理 */
EXT_API s32 MediaCipherDecProcess(MCIPHER_HANDLE pHandle, TFrameData *ptIn, TFrameData *ptOut, TExtMediaInfo *ptInfo, TExtAuthInfo *ptAuthInfo);

/* 解密安全参数集回调 */
EXT_API s32 MediaCipherDecSecurityParamCallback(MCIPHER_HANDLE pHandle, PFSECURITYPARAMCB pCallback, void *pvContext);

/* 获得最新的安全参数集 */
EXT_API s32 MediaCipherDecGetSecurityParam(MCIPHER_HANDLE pHandle, TSecurityParam *ptParam);

/*============================   skf 封装 ============================*/
/* 国密t卡or usb key信息*/
typedef struct tagSKFExtDevInfo
{
	char abySerialNumber[32];
	u32 m_dwTotalSpace;
	u32 m_dwFreeSpace;
	int m_dwDevType;
} TSKFExtDevInfo;

/* 目前不向上层提供SKF 裸读裸写文件*/
EXT_API s32 SKFWriteVerifyCert(u32 dwEngineId, u8 *pbyBuff, u32 dwLen);
EXT_API s32 SKFWriteEncCert(u32 dwEngineId, u8 *pbyBuff, u32 dwLen);
EXT_API s32 SKFWriteConVerifyCert(u32 dwEngineId, u8 *pbyBuff, u32 dwLen);
EXT_API s32 SKFWriteConEncCert(u32 dwEngineId, u8 *pbyBuff, u32 dwLen);

/* 获得加密卡的硬件信息 */
EXT_API s32 SKFExtGetDevInfo(u32 dwEngineId, TSKFExtDevInfo *ptSKFExtDevInfo);

typedef enum ESKFHwAction
{
	SKF_HW_ADD,
	SKF_HW_REMOVE,
} ESKFHwAction;

/* SKF安全设备结构体 */
typedef struct tagSKFDiskInfo
{
	u32 dwEngineId; //EngineID号
	s8 abyDiskName[32]; //盘符
	int dwDevType;
} TSKFDiskInfo;

/* SKF安全设备发现回调 */
typedef void (*PFSKFHWCALLBACK)(TSKFDiskInfo *ptDiskInfo, ESKFHwAction eAction, void* pvContext);

/* SKF 加密硬件生产测试安全设备初始化，或当前已插上设备，会触发回调 */
EXT_API s32 FactoryHwInit(PFSKFHWCALLBACK pCallback, void *pvContext);

/* SKF安全设备初始化，或当前已插上设备，会触发回调 */
EXT_API s32 SKFHwInit(PFSKFHWCALLBACK pCallback, void *pvContext);

/* 通过盘符来检查该设备是不是SKF安全设备 */
EXT_API BOOL32 SKFDiskCheck(s8 *pbyDiskName);

/* 通过盘符id 检查该设备是不是正常工作 */
EXT_API BOOL32 SKFIdCheck(u32 dwEngineId);

/* 通过盘符来打开SKF安全设备，并获得EngineID号 */
EXT_API s32 SKFDiskOpen(s8 *pbyDiskName, u32 *pdwEngineId);

/* 通过盘符来释放SKF安全设备 */
EXT_API s32 SKFDiskRelease(s8 *pbyDiskName);

/* 通过EngineId来释放SKF安全设备 */
EXT_API s32 SKFDiskReleaseById(u32 dwEngineId);

/* 读取平台验签证书 */
EXT_API s32 SKFReadVerifyCert(u32 dwEngineId, u8 *pbyBuff, u32 *pdwLen);

/* 读取平台加密证书 */
EXT_API s32 SKFReadEncCert(u32 dwEngineId, u8 *pbyBuff, u32 *pdwLen);

/* 读取本机验签证书 */
EXT_API s32 SKFReadConVerifyCert(u32 dwEngineId, u8 *pbyBuff, u32 *pdwLen);

/* 读取本机加密证书 */
EXT_API s32 SKFReadConEncCert(u32 dwEngineId, u8 *pbyBuff, u32 *pdwLen);

/* 兴唐加密卡设置 mount 路径 */
EXT_API s32 SKFXingTangSetMountPath(char *pbyPath);

/* 设置加密硬件为科达测试用模式 app name & container name */
EXT_API s32 SKFInitDevKedaMode(u32 dwEngineId);

/* 获得PEM格式key和req的公钥和私钥 */
EXT_API s32 KdSSL_Get_Key_Data(TPKey *ptPKey, char *pbyBuff, unsigned int *pdwDateLen);

EXT_API int KdSSL_PEM2DER(char *pbyInfile, char *pbyOutfile);
EXT_API int KdSSL_DER2PEM(char *pbyInfile, char *pbyOutfile);

EXT_API	char *KdSSL_GetDevName(int dwDevType);

/* 获得证书的id */
EXT_API s32 KdSSL_Get_Cert_SubjectCN(u8 *pbyCert, u32 dwLen, u8 *pbyMajorId, u8 *pbyMirrorId);

/* 获得证书的全部subject信息 */
EXT_API s32 KdSSL_Get_Cert_Subject(u8 *pbyCert, u32 dwLen, u8 *pbyBuff);

/* 国密测试 */
EXT_API int keda_gen_sm2_key(unsigned char *pubkey, unsigned char *prikey);
EXT_API int keda_sm2_sign(unsigned int engine, unsigned char *prikey, unsigned char *in, unsigned int in_len, unsigned char *sign, unsigned int *sign_len);
EXT_API int keda_sm2_verify(unsigned int engine, unsigned char *pubkey, unsigned char *in, unsigned int in_len, unsigned char *sign, unsigned int sign_len);
EXT_API int keda_sm2_enc(unsigned int engine, unsigned char *pubkey, unsigned char *in, unsigned int in_len, unsigned char *out, unsigned int *out_len);
EXT_API int keda_sm2_dec(unsigned int engine, unsigned char *prikey, unsigned char *in, unsigned int in_len, unsigned char *out, unsigned int *out_len);
EXT_API int keda_sm3_hash(unsigned int engine, unsigned char *pubkey, unsigned char *id, unsigned int id_len, unsigned char *dat, unsigned int dat_len, unsigned char *md, unsigned int *md_len);

/* 算法自测试*/
EXT_API s32 SKFSm3SelfTest(u32 dwEngineId);
EXT_API s32 SKFSm4EcbSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm1EcbSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm4OfbSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm1OfbSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm2SignSelfTest(u32 dwEngineId);
EXT_API s32 SKFSm2EncSelfTest(u32 dwEngineId);
EXT_API s32 SKFRandomSelfTest(u32 dwEngineId);

/* 计算文件的sm3 Hash, 不支持2GB 及以上的文件*/
EXT_API s32 CalcFileSm3Hash(u8 *pbyFileName, u8 *pbyBase64Hash, u32 *pdwLen);

#ifdef __cplusplus
}
#endif

#endif
