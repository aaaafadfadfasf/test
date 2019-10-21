#ifndef _TESTSECRRITY_H
#define _TESTSECRRITY_H
#include "kdsip.h"


#define _CIPHERTEXT_SIZE_MAX   (2*1024)            //默认密文buffer的最大长度
#define _CIPHERTEXT_SIZE_MIN    (97)               //密码模块默认密文buffer的最小长度


KDSIP_RETURN SecurityCalcCallBack(EKDSIP_CALLBACK_TYPE eCallBackType,
	LPCSTR szAlgorithm,
	LPCSTR szSrc,
	u32    dwSrcLen,
	TKdSipKeyArg* tKdSipKey,
	u32 dwEngineId,
	LPSTR  szResultBuffer,
	u32* ReSultBufferLen
	);
#endif