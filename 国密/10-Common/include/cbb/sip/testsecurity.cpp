#include  <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kdvtype.h"
#include "kdsip.h"
#include "testsecurity.h"
#include "kdssl-ext_api.h"


enum KDALGORITHMTYPE{
	KDC_UNKNOW_ALGORITHM,
	//对称算法
	KDC_SM1_ECB,
	KDC_SM1_CBC,
	KDC_SM1_CFB,
	KDC_SM1_OFB,
	KDC_SM4_ECB,
	KDC_SM4_CBC,
	KDC_SM4_CFB,
	KDC_SM4_OFB,
	KDC_ZUC,

	//非对称算法
	KDC_RSA,
	KDC_SM2,

	//散列算法
	KDC_SM3, 	
	KDC_SHA1,	
	KDC_SHA256,	

	//签名算法
	KDC_SM3_SM2
};

//KDC_EXT_KEY_PUBKEY_REQ  ##没有经过认证的秘钥
//KDC_EXT_KEY_CERT       ##经过认证的秘钥

#define  SEC_LOG_TYPE_DEBUG 0x0010 

static size_t InnerStrlLen(const char* szString)
{
	if (NULL == szString)
	{
		return 0;
	}
	return strlen(szString);
}

static void hexdump(const unsigned char *s,u32 dwLen)
{
	u32 n = 0;
	for (; n < dwLen; ++n)
	{
		if ((n % 16) == 0)
		    printf("\n%04x", n);
		printf(" %02x", s[n]);
	}
	printf("\n");
}

static void ShowArg(TPKey* ptArg,u32 dwEngineId)
{
	if (NULL != ptArg)
	{
		if (NULL != ptArg->m_pbyPKeyBuff)
		{
			KDSipLog(16,"arg:,m_pbyPKeyBuff:%s(%d),keytype:(%d),datetype:(%d),EngineId:(%u)\n",ptArg->m_pbyPKeyBuff,ptArg->m_dwPKeyLen,ptArg->m_eKeyType,ptArg->m_eDataType,dwEngineId);
		}else
		{
			KDSipLog(16,"arg:,m_pbyPKeyBuff:NULL,keytype:(%d),datetype:(%d),EngineId:(%u)\n",ptArg->m_eKeyType,ptArg->m_eDataType,dwEngineId);
		}
		return;
	}
}

#define SAFE_STR(str) (str?str:"null")

static KDSIP_RETURN _ASymmetricEncrypt(LPCSTR szSrcString,u32 dwSrcLen,TKdSipKeyArg* tKdSipKey,u32 dwEngineId,LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	int nRet = -1;
	KDSSL_PKEY_CTX enc_pkey = NULL;
	TPKey pub_key ={0};
	
	KDSipLog(SEC_LOG_TYPE_DEBUG,
    	"[_ASymmetricEncrypt] input para:\n"
    	"--tKdSipKey: szKey=%s nKeyType=%d nKeyDataType=%d szPassword=%s dwEngineId=%u\n"
    	"--dwSrcLen=%d szSrcString=%s\n",
    	SAFE_STR(tKdSipKey->szKey), tKdSipKey->nKeyType, 
    	tKdSipKey->nKeyDataType, SAFE_STR(tKdSipKey->szPassword), dwEngineId, 
    	dwSrcLen, szSrcString);

	pub_key.m_pbyPKeyBuff = (u8* )tKdSipKey->szKey;
	pub_key.m_dwPKeyLen = InnerStrlLen(tKdSipKey->szKey);//state.st_size;
	pub_key.m_eKeyType = (EKeyType)tKdSipKey->nKeyType;
	pub_key.m_eDataType = (EDataType)tKdSipKey->nKeyDataType;

	enc_pkey = KdSSL_PKEY_CTX_new(&pub_key, dwEngineId);
	if (NULL == enc_pkey)
	{
		ShowArg(&pub_key,dwEngineId);
		KDSipLog(2,"[_ASymmetricEncrypt]ERROR,KdSSL_PKEY_CTX_new return NULL\n");
		return KDSIP_NO_SOURCE;
	}
	nRet = KdSSL_PKEY_encrypt_init(enc_pkey);
	if (nRet)
	{
		ShowArg(&pub_key,dwEngineId);
		KDSipLog(2,"[_ASymmetricEncrypt]ERROR,KdSSL_PKEY_encrypt_init return %d\n",nRet);
		KdSSL_PKEY_CTX_free(enc_pkey);
		return KDSIP_CIPHER_FAIL;
	}	
	nRet = KdSSL_PKEY_encrypt(enc_pkey, (u8*)szResultBuffer, ReSultBufferLen, (const u8*)szSrcString, (int)dwSrcLen);
	if (nRet)
	{
		ShowArg(&pub_key,dwEngineId);
		KDSipLog(2,"[_ASymmetricEncrypt]ERROR,KdSSL_PKEY_encrypt return %d\n",nRet);
		KdSSL_PKEY_CTX_free(enc_pkey);
		return KDSIP_CIPHER_FAIL;
	}
	KdSSL_PKEY_CTX_free(enc_pkey);
	return KDSIP_OK;
}


static KDSIP_RETURN _ASymmetricDecrypt(LPCSTR szSrcString,u32 dwSrcLen,TKdSipKeyArg* tKdSipKey,u32 dwEngineId,LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	int nRet = -1;
	KDSSL_PKEY_CTX dec_pkey = NULL;
	TPKey pri_key ={0};
	
	KDSipLog(SEC_LOG_TYPE_DEBUG,
    	"[_ASymmetricDecrypt] input para:\n"
    	"--tKdSipKey: szKey=%s nKeyType=%d nKeyDataType=%d szPassword=%s dwEngineId=%u\n",
    	SAFE_STR(tKdSipKey->szKey), tKdSipKey->nKeyType, 
    	tKdSipKey->nKeyDataType, SAFE_STR(tKdSipKey->szPassword)
    	);
	
	pri_key.m_eKeyType = (EKeyType)tKdSipKey->nKeyType;
	pri_key.m_eDataType = (EDataType)tKdSipKey->nKeyDataType;
	if (KDC_EXT_KEY_ID_IN_HARDWARE == pri_key.m_eDataType)
	{
		pri_key.m_pbyPKeyBuff = (u8*)"dec";
		pri_key.m_dwPKeyLen = 0;
	}
	else
	{
		pri_key.m_pbyPKeyBuff = (u8*)tKdSipKey->szKey;
		pri_key.m_dwPKeyLen = InnerStrlLen(tKdSipKey->szKey);
		pri_key.m_pbyPassword = (s8*)tKdSipKey->szPassword;
	}

	dec_pkey = KdSSL_PKEY_CTX_new(&pri_key, dwEngineId);
	if (NULL == dec_pkey)
	{
		ShowArg(&pri_key,dwEngineId);
		KDSipLog(2,"[_ASymmetricDecrypt]ERROR KdSSL_PKEY_CTX_new return NULL\n");
		return KDSIP_NO_SOURCE;
	}
	nRet = KdSSL_PKEY_decrypt_init(dec_pkey);
	if (nRet)
	{
		ShowArg(&pri_key,dwEngineId);
		KDSipLog(2,"[_ASymmetricDecrypt]ERROR KdSSL_PKEY_decrypt_init return %d\n",nRet);
		KdSSL_PKEY_CTX_free(dec_pkey);
		return KDSIP_CIPHER_FAIL;
	}
	nRet = KdSSL_PKEY_decrypt(dec_pkey, (u8*)szResultBuffer, ReSultBufferLen, (const u8*)szSrcString, (int)dwSrcLen);
	if (nRet)
	{
		ShowArg(&pri_key,dwEngineId);
		KDSipLog(2,"[_ASymmetricDecrypt]ERROR KdSSL_PKEY_decrypt failed return %d\n",nRet);
		KdSSL_PKEY_CTX_free(dec_pkey);
		return KDSIP_CIPHER_FAIL;
	}
	KdSSL_PKEY_CTX_free(dec_pkey);
	return KDSIP_OK;
}

static KDSIP_RETURN _HashCala(LPCSTR szSrcString,u32 dwSrcLen,EHashAlgorithm eHashAlgorithm,u32 dwEngineId,LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	KDSSL_MD_CTX md_handle = NULL;
	s32 nRet= -1;
	md_handle = KdSSL_MD_CTX_create();
	if (NULL == md_handle)
	{
		KDSipLog(2,"[_HashCala]KdSSL_MD_CTX_create failed return NULL\n");
		return KDSIP_NO_SOURCE;
	}
	nRet = KdSSL_DigestInit(md_handle, eHashAlgorithm, dwEngineId);
	if (nRet)
	{
		KDSipLog(2,"[_HashCala]ERROR KdSSL_DigestInit failed return %d",nRet);
		KdSSL_MD_CTX_destroy(md_handle);
		return KDSIP_CIPHER_FAIL;
	}
	nRet = KdSSL_DigestUpdate(md_handle, szSrcString, dwSrcLen);
	if (nRet)
	{
		KDSipLog(2,"[_HashCala]ERROR KdSSL_DigestUpdate failed return %d\n",nRet);
		KdSSL_MD_CTX_destroy(md_handle);
		return KDSIP_CIPHER_FAIL;
	}
	nRet = KdSSL_DigestFinal(md_handle, (u8*)szResultBuffer, ReSultBufferLen);
	if (nRet)
	{
		KDSipLog(2,"[_HashCala]ERROR KdSSL_DigestFinal failed return %d\n",nRet);
		KdSSL_MD_CTX_destroy(md_handle);
		return KDSIP_CIPHER_FAIL;
	}
	KdSSL_MD_CTX_destroy(md_handle);
	return KDSIP_OK;
}
static KDSIP_RETURN _SignCala(LPCSTR szSrcString,u32 dwSrcLen,TKdSipKeyArg* tKdSipKey,EHashAlgorithm eHashAlgorithm,u32 dwEngineId,LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	KDSSL_MD_CTX md_handle = NULL;
	s32 nRet = -1;
	TPKey pri_key ={0};

	md_handle = KdSSL_MD_CTX_create();
	if (NULL == md_handle)
	{
		KDSipLog(2,"[_SignCala]KdSSL_MD_CTX_create failed return NULL\n");
		return KDSIP_FAIL;
	}
	pri_key.m_eKeyType = (EKeyType)tKdSipKey->nKeyType;
	pri_key.m_eDataType = (EDataType)tKdSipKey->nKeyDataType;
	if (KDC_EXT_KEY_ID_IN_HARDWARE == pri_key.m_eDataType)
	{
		pri_key.m_dwPKeyLen = 0;
		pri_key.m_pbyPKeyBuff = (u8*)"sign";
	}
	else
	{
		pri_key.m_pbyPKeyBuff = (u8*)tKdSipKey->szKey;
		pri_key.m_dwPKeyLen = InnerStrlLen(tKdSipKey->szKey);
		pri_key.m_pbyPassword = (s8*)tKdSipKey->szPassword;
	}

	nRet = KdSSL_DigestSignInit(md_handle, &pri_key, eHashAlgorithm, dwEngineId);
	if (nRet)
	{
		ShowArg(&pri_key,dwEngineId);
		KDSipLog(2,"[_SignCala]ERROR KdSSL_DigestSignInit failed return %d\n",nRet);
		KdSSL_MD_CTX_destroy(md_handle);
		return KDSIP_CIPHER_FAIL;
	}
	nRet = KdSSL_DigestSignUpdate(md_handle, szSrcString, dwSrcLen);
	if (nRet)
	{
		ShowArg(&pri_key,dwEngineId);
		KDSipLog(2,"[_SignCala]ERROR KdSSL_DigestSignUpdate failed return %d\n",nRet);
		KdSSL_MD_CTX_destroy(md_handle);
		return KDSIP_CIPHER_FAIL;
	}
	nRet = KdSSL_DigestSignFinal(md_handle, (u8*)szResultBuffer, ReSultBufferLen);
	if (nRet)
	{
		ShowArg(&pri_key,dwEngineId);
		KDSipLog(2,"[_SignCala]ERROR KdSSL_DigestSignFinal failed return %d\n",nRet);
		KdSSL_MD_CTX_destroy(md_handle);
		return KDSIP_CIPHER_FAIL;
	}
	KdSSL_MD_CTX_destroy(md_handle);
	return KDSIP_OK;
}

static KDSIP_RETURN _VerifyCala(LPCSTR szSrcString,u32 dwSrcLen,TKdSipKeyArg* tKdSipKey,EHashAlgorithm eHashAlgorithm,u32 dwEngineId,LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	KDSSL_MD_CTX md_handle = NULL;
	s32 nRet = -1;
	TPKey pub_key ={0};

	md_handle = KdSSL_MD_CTX_create();

	pub_key.m_pbyPKeyBuff = (u8*)tKdSipKey->szKey;
	pub_key.m_dwPKeyLen = InnerStrlLen(tKdSipKey->szKey);//如果填的文件的话参数不起作用
	pub_key.m_eKeyType = (EKeyType)(tKdSipKey->nKeyType);
	pub_key.m_eDataType = (EDataType)(tKdSipKey->nKeyDataType);

	nRet = KdSSL_DigestVerifyInit(md_handle, &pub_key, eHashAlgorithm, dwEngineId);
	if (nRet)
	{
		ShowArg(&pub_key,dwEngineId);
		KDSipLog(2,"[_VerifyCala]ERROR KdSSL_DigestVerifyInit failed return %d\n",nRet);
		return KDSIP_CIPHER_FAIL;
	}
	nRet = KdSSL_DigestVerifyUpdate(md_handle, szSrcString, dwSrcLen);
	if (nRet)
	{
		ShowArg(&pub_key,dwEngineId);
		KDSipLog(2,"[_VerifyCala]ERROR KdSSL_DigestVerifyUpdate failed return %d\n",nRet);
		KdSSL_MD_CTX_destroy(md_handle);
		return KDSIP_CIPHER_FAIL;
	}
	nRet = KdSSL_DigestVerifyFinal(md_handle, (const u8*)szResultBuffer, *ReSultBufferLen);
	if (nRet)
	{
		ShowArg(&pub_key,dwEngineId);
		KDSipLog(2,"[_VerifyCala]ERROR KdSSL_DigestVerifyFinal failed return %d\n",nRet);
		KdSSL_MD_CTX_destroy(md_handle);
		return KDSIP_CIPHER_FAIL;
	}
	KdSSL_MD_CTX_destroy(md_handle);
	return KDSIP_OK;
}
static KDALGORITHMTYPE  Str2Algorithm(LPCSTR szAlgorithm)
{
	if (0 == strcmp(szAlgorithm,"SM2"))
	{
		return KDC_SM2;
	}
	if (0 == strcmp(szAlgorithm,"SM3"))
	{
		return KDC_SM3;
	}
	if (0 == strcmp(szAlgorithm,"SM3-SM2"))
	{
		return KDC_SM3_SM2;
	}
	return KDC_UNKNOW_ALGORITHM;
}

static KDSIP_RETURN _InnerDecrypt(	LPCSTR szAlgorithm,LPCSTR szSrcString,u32 dwSrcLen,TKdSipKeyArg* tKdSipKey,u32 dwEngineId,LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	if ((NULL == szAlgorithm) || 
		(NULL == szSrcString) || 
		(NULL == tKdSipKey)    ||
		(NULL == szResultBuffer)||
		(0 == *ReSultBufferLen)
		)
	{
		return KDSIP_INVALID_ARG;
	}
	KDALGORITHMTYPE eAlgType = KDC_UNKNOW_ALGORITHM;
	eAlgType = Str2Algorithm(szAlgorithm);
	switch(eAlgType)
	{
	case KDC_SM1_ECB:
	case KDC_SM1_CBC:
	case KDC_SM1_CFB:
	case KDC_SM1_OFB:
	case KDC_SM4_ECB:
	case KDC_SM4_CBC:
	case KDC_SM4_CFB:
	case KDC_SM4_OFB:
		printf("UNSURPPORT");
		break;
	case KDC_SM2:
	case KDC_RSA:
		return _ASymmetricDecrypt(szSrcString,dwSrcLen,tKdSipKey,dwEngineId,szResultBuffer,ReSultBufferLen);
	default:
		printf("Unknow type\n");
		break;
	}
	return KDSIP_FAIL;
}

static KDSIP_RETURN _InnerEncrypt(	LPCSTR szAlgorithm,LPCSTR szSrcString,u32 dwSrcLen,TKdSipKeyArg* tKdSipKey, u32 dwEngineId,LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	if ((NULL == szAlgorithm) || 
		(NULL == szSrcString) || 
		(NULL == tKdSipKey)       ||
		(NULL == szResultBuffer) ||
		(0 == *ReSultBufferLen)
		)
	{
		return KDSIP_INVALID_ARG;
	}
	KDALGORITHMTYPE eAlgType = KDC_UNKNOW_ALGORITHM;
	eAlgType = Str2Algorithm(szAlgorithm);
	switch(eAlgType)
	{
	case KDC_SM1_ECB:
	case KDC_SM1_CBC:
	case KDC_SM1_CFB:
	case KDC_SM1_OFB:
	case KDC_SM4_ECB:
	case KDC_SM4_CBC:
	case KDC_SM4_CFB:
	case KDC_SM4_OFB:
		printf("UNSURPPORT");
		break;
	case KDC_SM2:
	case KDC_RSA:
		return _ASymmetricEncrypt(szSrcString,dwSrcLen,tKdSipKey,dwEngineId,szResultBuffer,ReSultBufferLen);
	default:
		printf("Unknow type\n");
		break;
	}
	return KDSIP_FAIL;
}

static KDSIP_RETURN _InnerHash(	LPCSTR szAlgorithm,LPCSTR szSrcString,u32 dwSrcLen,u32 dwEngineId, LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	if ((NULL == szAlgorithm) || 
		(NULL == szSrcString) || 
		(NULL == szResultBuffer) ||
		(0 == *ReSultBufferLen)
		)
	{
		return KDSIP_INVALID_ARG;
	}
	KDALGORITHMTYPE eAlgType = KDC_UNKNOW_ALGORITHM;
	eAlgType = Str2Algorithm(szAlgorithm);
	switch(eAlgType)
	{
	case KDC_SM3:
		return _HashCala(szSrcString,dwSrcLen,KDC_EXT_SM3,dwEngineId,szResultBuffer,ReSultBufferLen);
	case KDC_SHA1:
		return _HashCala(szSrcString,dwSrcLen,KDC_EXT_SHA1,dwEngineId,szResultBuffer,ReSultBufferLen);
	case KDC_SHA256:
		return _HashCala(szSrcString,dwSrcLen,KDC_EXT_SHA256,dwEngineId,szResultBuffer,ReSultBufferLen);
	default:
		printf("Unknow type\n");
		break;
	}
	return KDSIP_FAIL;
}

static KDSIP_RETURN _InnerSign(LPCSTR szAlgorithm,LPCSTR szSrcString,u32 dwSrcLen,TKdSipKeyArg* tKdSipKey,u32 dwEngineId,LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	if ((NULL == szAlgorithm) || 
		(NULL == szSrcString) ||
		(NULL == tKdSipKey)       ||
		(NULL == szResultBuffer)||
		(0 == *ReSultBufferLen)
		)
	{
		return KDSIP_INVALID_ARG;
	}
	KDALGORITHMTYPE eAlgType = KDC_UNKNOW_ALGORITHM;
	eAlgType = Str2Algorithm(szAlgorithm);
	switch(eAlgType)
	{
	case KDC_SM3_SM2:
		return _SignCala(szSrcString,dwSrcLen,tKdSipKey,KDC_EXT_SM3,dwEngineId,szResultBuffer,ReSultBufferLen);
	default:
		printf("Unknow type\n");
		break;
	}
	return KDSIP_FAIL;
}

static KDSIP_RETURN _InnerVerify(LPCSTR szAlgorithm,LPCSTR szSrcString,u32 dwSrcLen,TKdSipKeyArg* tKdSipKey,u32 dwEngineId,LPSTR  szResultBuffer,u32* ReSultBufferLen)
{
	if ((NULL == szAlgorithm) || 
		(NULL == szSrcString) ||
		(NULL == tKdSipKey)       ||
		(NULL == szResultBuffer)||
		(0 == *ReSultBufferLen)
		)
	{
		return KDSIP_INVALID_ARG;
	}
	KDALGORITHMTYPE eAlgType = KDC_UNKNOW_ALGORITHM;
	eAlgType = Str2Algorithm(szAlgorithm);
	switch(eAlgType)
	{
	case KDC_SM3_SM2:
		return _VerifyCala(szSrcString,dwSrcLen,tKdSipKey,KDC_EXT_SM3,dwEngineId,szResultBuffer,ReSultBufferLen);
	default:
		printf("Unknow type\n");
		break;
	}
	return KDSIP_FAIL;
}

KDSIP_RETURN SecurityCalcCallBack(EKDSIP_CALLBACK_TYPE eCallBackType,
	LPCSTR szAlgorithm,
	LPCSTR szSrc,
	u32    dwSrcLen,
	TKdSipKeyArg* tKdSipKey,
	u32 dwEngineId,
	LPSTR  szResultBuffer,
	u32* ReSultBufferLen
	)
{
	switch(eCallBackType)
	{
	case KDSIP_CALLBACK_DECRYPT:
		return _InnerDecrypt(szAlgorithm,szSrc,dwSrcLen,tKdSipKey,dwEngineId,szResultBuffer,ReSultBufferLen);
	case KDSIP_CALLBACK_ENCRYPT:
		return _InnerEncrypt(szAlgorithm,szSrc,dwSrcLen,tKdSipKey,dwEngineId,szResultBuffer,ReSultBufferLen);
	case KDSIP_CALLBACK_HASH:
		return _InnerHash(szAlgorithm,szSrc,dwSrcLen,dwEngineId,szResultBuffer,ReSultBufferLen);
	case KDSIP_CALLBACK_SIGN:
		return _InnerSign(szAlgorithm,szSrc,dwSrcLen,tKdSipKey,dwEngineId,szResultBuffer,ReSultBufferLen);
	case KDSIP_CALLBACK_VERIFY:
		return _InnerVerify(szAlgorithm,szSrc,dwSrcLen,tKdSipKey,dwEngineId,szResultBuffer,ReSultBufferLen);
	default:
		break;
	}
	return KDSIP_FAIL;
}
/*#define  VKEK_ATTR_PRE "vkek:"
KDSIP_RETURN KdVkek2S(TKdVkek* ptVkek,const char* pAlthorithm,TKdSipKeyArg* ptPublicKey,char* pAttrBuffer,u32* pAttrBufferLen)
{
	if ((NULL == ptVkek)        ||
		(NULL == pAlthorithm)   ||
		(NULL == pAttrBuffer)   ||
		(0 == *pAttrBufferLen)
		)
	{
		return KDSIP_INVALID_ARG;
	}
	KDSIP_RETURN eKdRet = KDSIP_FAIL;
	KDALGORITHMTYPE eAlthType = Str2Algorithm(pAlthorithm);
	char* pBuffAvailableStart = pAttrBuffer;
	if (KDC_UNKNOW_ALGORITHM == eAlthType)
	{
		return KDSIP_FAIL;
	}
	if (KDC_SM2 == eAlthType)
	{
		s32 nMayLen = InnerStrlLen(ptVkek->pVkek)+98;
		//分配存储vkek加密后的内存
		char* pEncryptBuffer = (char*)malloc(nMayLen);
		if (NULL == pEncryptBuffer)
		{
			return KDSIP_BUFFSIZE_SMALL;
		}
		eKdRet = _ASymmetricEncrypt(ptVkek->pVkek,InnerStrlLen(ptVkek->pVkek),ptPublicKey,pEncryptBuffer,(u32*)&nMayLen);
		if (KDSIP_OK != eKdRet)
		{
			free(pEncryptBuffer);
			return KDSIP_FAIL;
		}
		//分配存储对加密后的内存做base64_encode需要的空间
		s32 nBaseEnVkekLen = nMayLen*4/3+6;
		char* pBaseEncodeVkek = (char*)malloc(nBaseEnVkekLen);
		if (NULL == pBaseEncodeVkek)
		{
			free(pEncryptBuffer);
			return KDSIP_NO_SOURCE; 
		}
		eKdRet = KDSipBase64_encode((unsigned char*)pBaseEncodeVkek,&nBaseEnVkekLen,(unsigned char*)pEncryptBuffer,nMayLen);
		if (KDSIP_OK != eKdRet)
		{
			free(pEncryptBuffer);
			free(pBaseEncodeVkek);
			return eKdRet;
		}
		free(pEncryptBuffer);

		//组合字符串
		//"vkek:1970-1-1T9:18:43 adfakelrjoejojaofafjalfd=="
		int nTotalFillLen = InnerStrlLen(VKEK_ATTR_PRE)+InnerStrlLen(ptVkek->pVkekVersion)+1+nBaseEnVkekLen +1;
		if (*pAttrBufferLen < nTotalFillLen)
		{
			*pAttrBufferLen = nTotalFillLen;
			free(pBaseEncodeVkek);
			return KDSIP_BUFFSIZE_SMALL;
		}
		int nCopyLen = InnerStrlLen(VKEK_ATTR_PRE);
		memcpy(pBuffAvailableStart,VKEK_ATTR_PRE,nCopyLen);
		pBuffAvailableStart += nCopyLen;

		nCopyLen = InnerStrlLen(ptVkek->pVkekVersion);
		memcpy(pBuffAvailableStart,ptVkek->pVkekVersion,nCopyLen);
		pBuffAvailableStart += nCopyLen;

		pBuffAvailableStart[0]=' ';
		pBuffAvailableStart++;

		nCopyLen = nBaseEnVkekLen;
		memcpy(pBuffAvailableStart,pBaseEncodeVkek,nCopyLen);
		pBuffAvailableStart += nCopyLen;
		pBuffAvailableStart[0]='\0';

		free(pBaseEncodeVkek);
		return KDSIP_OK;
	}
	return KDSIP_NONSUPPORT;
}

KDSIP_RETURN KdS2Vkek(const char* szVkekAttrString,const char* pAlthorithm,TKdSipKeyArg* ptPrivateKey,char* pVkekBuffer,u32* pVkekBufferLen,char* pVersionBuffer,u32* pVersionBufferLen)
{
	
	if ((NULL == szVkekAttrString)    || 
		(NULL == pAlthorithm)         ||
		(NULL == ptPrivateKey)        ||
		(NULL == ptPrivateKey->szKey) ||
		(NULL == pVkekBuffer)         ||
		(0 == *pVkekBufferLen)        ||
		(NULL == pVersionBuffer)      ||
		(0 == *pVersionBufferLen)
		)
	{
		return KDSIP_INVALID_ARG;
	}

	KDSIP_RETURN eKdVkekRet = KDSIP_OK;
	KDSIP_RETURN eKdVersionRet= KDSIP_OK;
	KDSIP_RETURN eKdRet = KDSIP_FAIL;

	const char* pFindStringStart = NULL;
	const char* pFindStringEnd   = NULL;
	pFindStringStart = strstr(szVkekAttrString,VKEK_ATTR_PRE);
	if (NULL == pFindStringStart)
	{
		return KDSIP_INVALID_ARG;
	}
	pFindStringStart += InnerStrlLen(VKEK_ATTR_PRE);
	
	//查找第一个空格
	pFindStringEnd = strchr(pFindStringStart,' ');
	if (NULL == pFindStringStart)
	{
		return KDSIP_INVALID_ARG;
	}
	int nCopyVkekLen = pFindStringEnd - pFindStringStart;
	if (*pVersionBufferLen > nCopyVkekLen)
	{
		memcpy(pVersionBuffer,pFindStringStart,nCopyVkekLen);
		pVersionBuffer[nCopyVkekLen] ='\0';
	}else
	{
		eKdVkekRet = KDSIP_BUFFSIZE_SMALL;
	}
	*pVersionBufferLen =  nCopyVkekLen +1;

	pFindStringEnd++;//跳过空格

	//剩下的应该是vkek加密经过base64编码后的值
	int nEncodeLen = InnerStrlLen(pFindStringEnd);
	char *pDecodeBuffer = (char*)malloc(nEncodeLen);
	if (NULL == pDecodeBuffer)
	{
		return KDSIP_NO_SOURCE;
	}	
	eKdRet = KDSipBase64_decode((unsigned char*)pDecodeBuffer,&nEncodeLen,(unsigned char*)pFindStringEnd,nEncodeLen);
	if (KDSIP_OK != eKdRet)
	{
		free(pDecodeBuffer);
		*pVkekBufferLen = nEncodeLen;
		return eKdRet;
	}
	eKdRet = _ASymmetricDecrypt(pDecodeBuffer,nEncodeLen,ptPrivateKey,pVkekBuffer,pVkekBufferLen);
	if (KDSIP_OK != eKdRet)
	{
		free(pDecodeBuffer);
		return eKdRet;
	}
	free(pDecodeBuffer);
	pVkekBuffer[*pVkekBufferLen] = '\0';
	return KDSIP_OK;
}*/