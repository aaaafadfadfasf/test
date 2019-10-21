#ifndef __KDSSL_EXT_ERRNO__
#define __KDSSL_EXT_ERRNO__

#define K_E_ERR			1	/* unknow reason */
#define K_E_HANDLE		2	/* handle is null or bad */
#define K_E_INVAL		3	/* Invalid argument */
#define K_E_SUPPORT		4	/* type not supported */
#define K_E_NOMEM		5	/* memory is not enough */
#define	K_E_MALLOC		6	/* malloc return null */
#define	K_E_CREATE_KEY	7	/* create key fail */
#define K_E_UNSUPPORT	8	/* alog is not support  */
#define K_E_PKEY_INIT	9	/* pkey handle init failed */

#define	K_E_EVP_INIT	10	/* evp init error */
#define	K_E_EVP_UPDATE	11	/* evp update error */
#define	K_E_EVP_FINISH	12	/* evp finish error */
#define	K_E_EVP_SIGN	13	/* evp sign error */
#define	K_E_EVP_VERIFY	14	/* evp verify error */
#define	K_E_EVP_ENC		15	/* evp enc error */
#define	K_E_EVP_DEC		16	/* evp dec error */

#define	K_E_NO_VKEK		21	/* vkek not set */
#define	K_E_FIND_VKEK	22	/* vkek not find */

#define	K_E_UNKNOW_DISK	30	/* disk name not find in list */
#define K_E_INVAL_ID	31  /* engine id is invalid */
#define K_E_UNKNOW_ID	32  /* engine id not find in list */
#define K_E_INVAL_H_ID	33  /* stream cipher id not find in list */

#define K_E_FILE_EXIST	40 /* file not exist */
#define K_E_FILE_READ	41 /* file read failed */

#endif
