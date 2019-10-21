#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_
#include "kdvtype.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <winsock2.h>
#endif

typedef long*  verylong;

/**************************��������*******************************************/

/*=============================================================================
������		��StrToNumber
����		�����ִ�ת��Ϊ������
�㷨ʵ��	������ѡ�
�������˵����
            pszText   ��ʮ����������ʾ���ַ���("user"->"75736572")
            pvlNum    Ҫ����Ĵ�����
����ֵ˵����bool
=============================================================================*/
BOOL32 StrToNumber(char *pszText, verylong *pvlNum);

/*=============================================================================
������		��NumberToStr
����		����������ת��Ϊ�ִ�
�㷨ʵ��	������ѡ�
�������˵����
            pszText   ��ʮ����������ʾ���ַ���("user"->"75736572")
            pvlNum    ����Ĵ�����
����ֵ˵����bool
=============================================================================*/
BOOL32 NumberToStr(char *pszText, verylong vlNum);

BOOL32 KdvVerylongNumFree();

BOOL32 KdvFreeKarMem();

/*=============================================================================
������		��KdvExpMod
����		��bb = (a^e) mod n
����ֵ˵����bool
=============================================================================*/
BOOL32 KdvExpMod(verylong a,  verylong e,   verylong n, verylong *bb);

BOOL32 KdvRandomPrime(long lBits, verylong *bb);

void vlFree(verylong *a);

/*=============================================================================
������		��kdvstrrev
����		�����ַ�������
����ֵ˵����
=============================================================================*/
void kdvstrrev(char *s);

/*****************************************************************************/

/**********************des****************************************************/
/* what */
typedef enum QFDES_what {qfDES_encrypt, qfDES_decrypt} QFDES_what;

/* mode */
typedef enum QFDES_mode {qfDES_ecb, qfDES_cbc, qfDES_cfb, qfDES_ofb} QFDES_mode;

void KdvDES(u8	*key, u8 *data,  u32 size,
		  const QFDES_what what,
		  const QFDES_mode mode,
		  u8 *initVec);

BOOL32 IsWeakDesKey(u8 *key);
/****************************************************************************/

/**********************AES****************************************************/

/*  Generic Defines  */
#define     DIR_ENCRYPT           0 /*  Are we encrpyting?  */
#define     DIR_DECRYPT           1 /*  Are we decrpyting?  */
#define     MODE_ECB              1 /*  Are we ciphering in ECB mode?   */
#define     MODE_CBC              2 /*  Are we ciphering in CBC mode?   */
#define     MODE_CFB1             3 /*  Are we ciphering in 1-bit CFB mode? */

/*  Algorithm-specific Defines  */
#define     MAX_KEY_SIZE         32 /* # of ASCII char's needed to represent a key */
#define     MAX_IV_SIZE          16 /* # bytes needed to represent an IV  */

int KdvAES(s8 *keyMaterial, s32 keyLen, u8 mode,u8 direction,s8 *IV,
           u8 *input, s32 inputLen, u8 *outBuffer);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif
