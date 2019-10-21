#ifndef __KDSSL_EXT_HW_H__
#define __KDSSL_EXT_HW_H__

/* supplier vendor */
#define KD_SZGX		(1)
#define KD_SDT		(2)
#define KD_SWXA		(3)
#define KD_WXT		(4)

/* hw type */
#define KD_USBKEY	(1)
#define KD_TF		(2)
#define KD_SERVER	(3)
#define KD_IC		(4)

#define EXT_HW_VENDOR(x)	((x) << 24)
#define EXT_HW_TYPE(x)		((x) << 16)
#define EXT_HW_DEV_ID(x)	((x) & 0xffff)

#define EXT_GET_VENDOR(x)	((x) >> 24)
#define EXT_GET_TYPE(x)		((x) >> 16)

#define SZGX_TF_3302	(EXT_HW_VENDOR(KD_SZGX) | EXT_HW_TYPE(KD_TF) | EXT_HW_DEV_ID(1))
#define SDT_TF_XXX		(EXT_HW_VENDOR(KD_SDT)  | EXT_HW_TYPE(KD_TF) | EXT_HW_DEV_ID(1))
#define SWXA_TF_XXX		(EXT_HW_VENDOR(KD_SWXA) | EXT_HW_TYPE(KD_TF) | EXT_HW_DEV_ID(1))
#define WXT_TF_XXX		(EXT_HW_VENDOR(KD_WXT)  | EXT_HW_TYPE(KD_TF) | EXT_HW_DEV_ID(1))

#define SZGX_UK_3301	(EXT_HW_VENDOR(KD_SZGX) | EXT_HW_TYPE(KD_USBKEY) | EXT_HW_DEV_ID(1))

#define SZGX_SER_903T	(EXT_HW_VENDOR(KD_SZGX) | EXT_HW_TYPE(KD_SERVER) | EXT_HW_DEV_ID(1))
#define SWXA_SER_310	(EXT_HW_VENDOR(KD_SWXA) | EXT_HW_TYPE(KD_SERVER) | EXT_HW_DEV_ID(1))
#define SDT_SER_XXX		(EXT_HW_VENDOR(KD_SDT)  | EXT_HW_TYPE(KD_SERVER) | EXT_HW_DEV_ID(1))

typedef struct tagServerrInfo
{
	char ip_addr[16];
	char port[16];
} TServerInfo;

typedef struct tagHwSetInfo
{
	int dev_type;

	union
	{
		TServerInfo	m_tServerInfo;
	};
} THwSetInfo;

#endif
