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

/**************************大数运算*******************************************/

/*=============================================================================
函数名		：StrToNumber
功能		：将字串转换为大整数
算法实现	：（可选项）
输入参数说明：
            pszText   以十六进制数表示的字符串("user"->"75736572")
            pvlNum    要输出的大整数
返回值说明：bool
=============================================================================*/
BOOL32 StrToNumber(char *pszText, verylong *pvlNum);

/*=============================================================================
函数名		：NumberToStr
功能		：将大整数转换为字串
算法实现	：（可选项）
输入参数说明：
            pszText   以十六进制数表示的字符串("user"->"75736572")
            pvlNum    输入的大整数
返回值说明：bool
=============================================================================*/
BOOL32 NumberToStr(char *pszText, verylong vlNum);

BOOL32 KdvVerylongNumFree();

BOOL32 KdvFreeKarMem();

/*=============================================================================
函数名		：KdvExpMod
功能		：bb = (a^e) mod n
返回值说明：bool
=============================================================================*/
BOOL32 KdvExpMod(verylong a,  verylong e,   verylong n, verylong *bb);

BOOL32 KdvRandomPrime(long lBits, verylong *bb);

void vlFree(verylong *a);

/*=============================================================================
函数名		：kdvstrrev
功能		：函字符串倒序
返回值说明：
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
