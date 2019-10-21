#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#ifndef _WIN32
#include <unistd.h>
#include <sys/time.h>
#endif

#include "kdvtype.h"
#include "kdssl-ext_api.h"

static int engine_id = 0;

static unsigned char HEXBYTES[]="0123456789ABCDEF";
static void printhex(unsigned char *data, size_t len)
{
	size_t i=0;

	printf("0x");
	while(i<len)
	{
		printf("%c", HEXBYTES[(data[i]&0xF0)>>4]);
		printf("%c", HEXBYTES[data[i]&0x0F]);
		i++;
	}
	printf("\n");
}

static void hexdump(FILE *f, const char *title, const unsigned char *s, int l)
{
	int n = 0;
	fprintf(f, "%s", title);
	for (; n < l; ++n)
	{
		if ((n % 16) == 0)
			fprintf(f, "\n%04x", n);
		fprintf(f, " %02x", s[n]);
	}
	fprintf(f, "\n");
}

static void show_help(void)
{
	printf("`h` - get help menu. ./evp_test 公钥文件名 私钥文件名(没有就不填)\n");

	/* 生成随机数*/
	printf("`1` - get 16 bytes rand number.\n");

	/* sm3计算hash */
	printf("`2` - SM3 test (auto).\n");

	/* sm4加密 */
	printf("`3` - SM4 test (auto).\n");

	/* sm2 加密解密 */
	printf("`4` - SM2 enc/dec test (soft & key is file).\n");
	printf("`5` - SM2 enc/dec test (soft & key is data).\n");

	/* sm2 签名验签*/
	printf("`6` - SM2 digest sign/verify test (soft & key is file).\n");

	/* sm2加密私钥 解密(私钥用内部) */
	printf("`7` - SM2 enc/dec test(hardware & dec private key).\n");

	/* sm2签名私钥 解密(私钥用内部) */
	printf("`8` - SM2 enc/dec test(hardware & sign private key).\n");

	/* sm2签名验签(私钥用内部) */
	printf("`9` - SM2 digest sign/verify test(hardware).\n");

	/* 生成文件hash */
	printf("`a` - create file sm3 hash (base64 encode).\n");

	printf("`q` - Quit.\n");
	printf("\n");
}

#define BUFF_LEN (1024 * 1024)

static void DiskNameCallback(TSKFDiskInfo *ptDiskInfo, ESKFHwAction eAction, void* pvContext)
{
	printf("%s: EngineId [%d] DiskName [%s] %s.\n", (char *)pvContext, \
		ptDiskInfo->dwEngineId, \
		ptDiskInfo->abyDiskName, \
		eAction == SKF_HW_ADD? "add" : "remove");
	/* 0为使用软件库处理，非0 使用对应的硬件加密卡*/
	if (eAction == SKF_HW_ADD)
	{
		engine_id = ptDiskInfo->dwEngineId;

		if (eAction == SKF_HW_ADD)
		{
			if (SKFSm3SelfTest(engine_id) || \
				SKFSm4EcbSelfTest(engine_id) || \
				SKFSm1EcbSelfTest(engine_id) || \
				SKFSm4OfbSelfTest(engine_id) || \
				SKFSm1OfbSelfTest(engine_id) || \
				SKFSm3SignSelfTest(engine_id) || \
				SKFSm2EncSelfTest(engine_id) || \
				SKFRandomSelfTest(engine_id))
				printf("Hw self test failed!\n");
			else
				printf("Hw self test pass.\n");
			printf("Dev %d is %s\n", engine_id, SKFIdCheck(engine_id) ? "Work" : "Not work");
		}
	}
	else
		engine_id = 0;
}

#define DATA_LEN 1024
#define KEDACOM "kedacom"

static int read_file(char *name, char *buff)
{
#ifndef _WIN32
	int fd;
	struct stat state;
#else
	FILE *fp;
#endif
	int file_len = -1;

#ifndef _WIN32
	fd = open(name, O_RDONLY);
	if (fd <= 0)
	{
		printf("open %s failed!\n", name);
		return -1;
	}
	fstat(fd, &state);
	file_len = state.st_size;
	read(fd, buff, file_len);
	close(fd);
#else
	fp = fopen(name, "rb");
	if (fp == NULL)
	{
		printf("fopen %s failed!\n", name);
		return -1;
	}
	fseek(fp, 0L, SEEK_END);
	file_len = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	fread(buff, file_len, 1, fp);
	fclose(fp);
#endif
	printf("file_len is %d\n", file_len);
	return file_len;
}

int main(int argc, char *argv[])
{
	int ret, i, inlen;
	char cmd[64];
	u8 buff[256];
	int not_first = 0;
	s32 count;
	u32 total_count;
	KDSSL_MD_CTX md_handle;
	KDSSL_CIPHER_CTX enc_cipher, dec_cipher;
	KDSSL_PKEY_CTX enc_pkey, dec_pkey;
	TPKey pub_key, pri_key;
	char prikey_data[2048], pubkey_data[2048];
	int prikey_len, pubkey_len;

#ifndef _WIN32
	struct timeval tv;
#else
	clock_t start, end;
#endif

    unsigned char key[16] = {
		0x17, 0x3d, 0x14, 0xba, 0x50, 0x03, 0x73, 0x1d,
		0x7a, 0x60, 0x04, 0x94, 0x70, 0xf0, 0x0a, 0x29,
	};

	unsigned char iv[16] = {
		0x66, 0x03, 0x54, 0x92, 0x78, 0x00, 0x00, 0x00,
		0x66, 0x03, 0x54, 0x92, 0x78, 0x00, 0x00, 0x00,
	};

	u8 *in = malloc(BUFF_LEN);
	u8 *out = malloc(BUFF_LEN);
	u8 *check = malloc(BUFF_LEN);
	u8 *tmp = malloc(BUFF_LEN);

	show_help();
	inlen = strlen("kedacom");

	for (i = 0; i < BUFF_LEN; i++)
		in[i] = (u8)i;

	KdSSL_SetOspRegCommond((void *)NULL);
	KdSSL_SetOspPrintf((void *)NULL);
	KdSSL_init_crypto(0);

	SKFHwInit(DiskNameCallback, "CB Test");

	gets(cmd);
	while (cmd[0] != 'q')
	{
		if (not_first)
			gets(cmd);

		not_first = 1;
		printf("=== input is %c ===\n", cmd[0]);

		switch (cmd[0])
		{
			case 'h':
				show_help();
				break;

			case '1':
				memset(buff, 0, sizeof(buff));

				if (engine_id)
					printf("Use hardware id %d!\n", engine_id);
				else
					printf("Use software lib kdssl!\n");

				if (!engine_id)
				{
					/*  伪随机需要一个种子*/
#ifdef _WIN32
					start = clock();
					KdSSL_RAND_seed(&start, sizeof(start));
#else
					gettimeofday(&tv, NULL);
					KdSSL_RAND_seed(&tv, sizeof(tv));
#endif
				}

				KdSSL_RAND_bytes(buff, 16, engine_id);
				printhex(buff, 16);
				break;

			case '2':
				if (engine_id)
					printf("Use hardware id %d!\n", engine_id);
				else
					printf("Use software lib kdssl!\n");
				md_handle = KdSSL_MD_CTX_create();
				ret = KdSSL_DigestInit(md_handle, KDC_EXT_SM3, engine_id);
				assert(!ret);

				memcpy(in, KEDACOM, inlen);
				ret = KdSSL_DigestUpdate(md_handle, in, inlen);
				assert(!ret);
				ret = KdSSL_DigestFinal(md_handle, out, (u32 *)&count);
				assert(!ret);
				hexdump(stderr, "hash result", out, (int)count);
				KdSSL_MD_CTX_destroy(md_handle);

				break;

			case '3':
				if (engine_id)
					printf("Use hardware id %d!\n", engine_id);
				else
					printf("Use software lib kdssl!\n");

				total_count = 0;
				for (i = 0; i < 1024; i++)
					in[i] = (u8)(i + 1);
				memset(check, 0, BUFF_LEN);
				memset(out, 0, BUFF_LEN);
				enc_cipher =  KdSSL_CIPHER_CTX_new();
				/* sm4 加密*/
				ret = KdSSL_CipherInit_ex(enc_cipher, KDC_EXT_SM4_OFB, engine_id, key, iv, 1);
				assert(!ret);
				KdSSL_EncryptUpdate(enc_cipher, out, &count, in, DATA_LEN);
				assert(!ret);
				total_count += count;
				printf("KdSSL_EncryptUpdate count ret %u\n", count);
				KdSSL_EncryptFinal_ex(enc_cipher, out + count, &count);
				assert(!ret);
				printf("KdSSL_EncryptFinal_ex count ret %u\n", count);
				total_count += count;
				KdSSL_CIPHER_CTX_free(enc_cipher);

				total_count = 0;
				dec_cipher =  KdSSL_CIPHER_CTX_new();
				ret = KdSSL_CipherInit_ex(dec_cipher, KDC_EXT_SM4_OFB, engine_id, key, iv, 0);
				assert(!ret);
				KdSSL_EncryptUpdate(dec_cipher, check, &count, out, DATA_LEN);
				assert(!ret);
				total_count += count;
				printf("KdSSL_EncryptUpdate count ret %u\n", count);
				/* sm4 解密 */
				KdSSL_EncryptFinal_ex(dec_cipher, check + count, &count);
				assert(!ret);
				printf("KdSSL_EncryptFinal_ex count ret %u\n", count);
				total_count += count;
				KdSSL_CIPHER_CTX_free(dec_cipher);

				/* 与原始数据比较*/
				for (i = 0; i <  DATA_LEN; i++)
					if (check[i] != in[i])
						{
							printf("check failed! i %d\n",i);
							break;
						}

				break;

			case '4':
				printf("open public key %s\n", argv[1]);

				/*  KDC_EXT_KEY_PUBKEY_REQ 是证书请求，也可以当公钥用! 一般生成的时候文件扩展名为req */
				pub_key.m_pbyPKeyBuff = (u8 *)argv[1];
				pub_key.m_dwPKeyLen = 0;
				pub_key.m_eKeyType = strstr(argv[1], "req") ? KDC_EXT_KEY_PUBKEY_REQ : KDC_EXT_KEY_CERT;
				pub_key.m_eDataType = KDC_EXT_KEY_FILE_NAME;

				enc_pkey = KdSSL_PKEY_CTX_new(&pub_key, 0);
				printf("enc_pkey %p %d\n", enc_pkey, pub_key.m_eKeyType);
				ret = KdSSL_PKEY_encrypt_init(enc_pkey);
				//ret = KdSSL_PKEY_sign_init(enc_pkey);
				assert(!ret);
				count = BUFF_LEN;//这个必须要, 输入out的内存长度至少inlen + 256, count 返回加密后数据的长度
				ret = KdSSL_PKEY_encrypt(enc_pkey, out, (u32 *)&count, KEDACOM, inlen);
				//break;
				assert(!ret);
				printf("after enc size %u\n", count);
				hexdump(stderr, "sm2 result", out, (int)count);

				printf("open private key %s\n", argv[2]);

				pri_key.m_pbyPKeyBuff = (u8 *)argv[2];
				pri_key.m_dwPKeyLen = 0;
				pri_key.m_eKeyType = KDC_EXT_KEY_PRIVKEY;
				pri_key.m_eDataType = KDC_EXT_KEY_FILE_NAME;
				pri_key.m_pbyPassword = NULL;

				dec_pkey = KdSSL_PKEY_CTX_new(&pri_key, 0);
				ret = KdSSL_PKEY_decrypt_init(dec_pkey);
				assert(!ret);
				total_count = BUFF_LEN;//这个必须要, 输入check的内存长度至少密文长度, total_count 返回解密后数据的长度
				ret = KdSSL_PKEY_decrypt(dec_pkey, check, &total_count, out, count);
				assert(!ret);
				printf("after dec size %u out is %s\n", total_count, check);
				KdSSL_PKEY_CTX_free(dec_pkey);
				break;

			/* 4 和5 一样，只是5 把key从文件中读出来，按数据形式写入*/
			case '5':
				printf("open public key %s\n", argv[1]);

				pubkey_len = read_file(argv[1], pubkey_data);

				assert(pubkey_len > 0);

				/*  KDC_EXT_KEY_PUBKEY_REQ 是证书请求，也可以当公钥用! 一般生成的时候文件扩展名为req */
				pub_key.m_pbyPKeyBuff = pubkey_data;
				pub_key.m_dwPKeyLen = pubkey_len;
				pub_key.m_eKeyType = strstr(argv[1], "req") ? KDC_EXT_KEY_PUBKEY_REQ : KDC_EXT_KEY_CERT;
				pub_key.m_eDataType = KDC_EXT_KEY_FILE_DATA;

				enc_pkey = KdSSL_PKEY_CTX_new(&pub_key, 0);
				printf("enc_pkey %p %d\n", enc_pkey, pub_key.m_eKeyType);
				ret = KdSSL_PKEY_encrypt_init(enc_pkey);
				//ret = KdSSL_PKEY_sign_init(enc_pkey);
				assert(!ret);
				count = BUFF_LEN;//这个必须要, 输入out的内存长度至少inlen + 256, count 返回加密后数据的长度
				ret = KdSSL_PKEY_encrypt(enc_pkey, out, (u32 *)&count, KEDACOM, inlen);
				//break;
				assert(!ret);
				printf("after enc size %u\n", count);
				hexdump(stderr, "sm2 result", out, (int)count);

				printf("open private key %s\n", argv[2]);

				prikey_len = read_file(argv[2], prikey_data);

				assert(pubkey_len > 0);

				pri_key.m_pbyPKeyBuff = prikey_data;
				pri_key.m_dwPKeyLen = prikey_len;
				pri_key.m_eKeyType = KDC_EXT_KEY_PRIVKEY;
				pri_key.m_eDataType = KDC_EXT_KEY_FILE_DATA;
				pri_key.m_pbyPassword = NULL;

				dec_pkey = KdSSL_PKEY_CTX_new(&pri_key, 0);
				ret = KdSSL_PKEY_decrypt_init(dec_pkey);
				assert(!ret);
				total_count = BUFF_LEN;//这个必须要, 输入check的内存长度至少密文长度, total_count 返回解密后数据的长度
				ret = KdSSL_PKEY_decrypt(dec_pkey, check, &total_count, out, count);
				assert(!ret);
				printf("after dec size %u out is %s\n", total_count, check);
				KdSSL_PKEY_CTX_free(dec_pkey);

				break;


			case '6':
				md_handle = KdSSL_MD_CTX_create();

				pri_key.m_pbyPKeyBuff = (u8 *)argv[2];
				pri_key.m_dwPKeyLen = 0;//if KDC_EXT_KEY_FILE_NAME, not care
				pri_key.m_eKeyType = KDC_EXT_KEY_PRIVKEY;
				pri_key.m_eDataType = KDC_EXT_KEY_FILE_NAME;
				pri_key.m_pbyPassword = NULL;

				ret = KdSSL_DigestSignInit(md_handle, &pri_key, KDC_EXT_SM3, 0);
				assert(!ret);
				ret = KdSSL_DigestSignUpdate(md_handle, KEDACOM, inlen);
				assert(!ret);
				count = BUFF_LEN;//这个必须要, 输入out的内存长度至少128 byte, count 返回签名数据的长度
				ret = KdSSL_DigestSignFinal(md_handle, out, (u32 *)&count);
				assert(!ret);
				KdSSL_MD_CTX_destroy(md_handle);

				md_handle = KdSSL_MD_CTX_create();

				pub_key.m_pbyPKeyBuff = (u8 *)argv[1];
				pub_key.m_dwPKeyLen = 0;
				pub_key.m_eKeyType = (strstr(argv[1], "pem") || strstr(argv[1], "cer")) ? KDC_EXT_KEY_CERT : KDC_EXT_KEY_PUBKEY_REQ;
				pub_key.m_eDataType = KDC_EXT_KEY_FILE_NAME;

				ret = KdSSL_DigestVerifyInit(md_handle, &pub_key, KDC_EXT_SM3, 0);
				assert(!ret);
				ret = KdSSL_DigestVerifyUpdate(md_handle, KEDACOM, inlen);
				assert(!ret);
				/* 验证签名结果，输入签名缓冲，和签名长度*/
				ret = KdSSL_DigestVerifyFinal(md_handle, out, (u32)count);
				printf("verify %s !!!\n", ret ? "failed" : "success");
				KdSSL_MD_CTX_destroy(md_handle);

				break;

			case '7':
			case '8':
				if (!engine_id)
				{
					printf("must use hw to test this function!!!\n");
					break;
				}

				printf("open public key %s\n", argv[1]);

				pub_key.m_pbyPKeyBuff = (u8 *)argv[1];
				pub_key.m_dwPKeyLen = 0;
				pub_key.m_eKeyType = (strstr(argv[1], "pem") || strstr(argv[1], "cer")) ? KDC_EXT_KEY_CERT : KDC_EXT_KEY_PUBKEY_REQ;
				pub_key.m_eDataType = KDC_EXT_KEY_FILE_NAME;

				enc_pkey = KdSSL_PKEY_CTX_new(&pub_key, engine_id);
				printf("enc_pkey %p\n", enc_pkey);
				ret = KdSSL_PKEY_encrypt_init(enc_pkey);
				//ret = KdSSL_PKEY_sign_init(enc_pkey);
				assert(!ret);
				count = BUFF_LEN;//这个必须要
				ret = KdSSL_PKEY_encrypt(enc_pkey, out, (u32 *)&count, KEDACOM, inlen);
				//break;
				assert(!ret);
				printf("after enc size %u\n", count);
				hexdump(stderr, "sm2 result", out, count);

				if (cmd[0] == '7')
				{
					/* 用解密私钥解密 ( 双证书)  */
					printf("using private %s key to dec\n", "dec");
					pri_key.m_pbyPKeyBuff = (u8 *)"dec";
				}
				else
				{
					/* 用签名 私钥解密 ( 单 证书)  */
					printf("using private %s key to dec\n", "sign");
					pri_key.m_pbyPKeyBuff = (u8 *)"sign";
				}
				pri_key.m_dwPKeyLen = 0;
				pri_key.m_eKeyType = KDC_EXT_KEY_PRIVKEY;
				pri_key.m_eDataType = KDC_EXT_KEY_ID_IN_HARDWARE;//KDC_EXT_KEY_ID_IN_HARDWARE;

				dec_pkey = KdSSL_PKEY_CTX_new(&pri_key, engine_id);
				ret = KdSSL_PKEY_decrypt_init(dec_pkey);
				assert(!ret);
				total_count = BUFF_LEN;//这个必须要
				ret = KdSSL_PKEY_decrypt(dec_pkey, check, &total_count, out, count);
				assert(!ret);
				printf("after dec size %u out is %s\n", total_count, check);
				KdSSL_PKEY_CTX_free(dec_pkey);
				break;

			case '9':
				if (!engine_id)
				{
					printf("must use hw to test this function!!!\n");
					break;
				}
				md_handle = KdSSL_MD_CTX_create();
				memset(&pri_key, 0, sizeof(pri_key));
				pri_key.m_pbyPKeyBuff = (u8 *)"sign";
				pri_key.m_dwPKeyLen = 0;
				pri_key.m_eKeyType = KDC_EXT_KEY_PRIVKEY;
				pri_key.m_eDataType = KDC_EXT_KEY_ID_IN_HARDWARE;

				ret = KdSSL_DigestSignInit(md_handle, &pri_key, KDC_EXT_SM3, engine_id);
				assert(!ret);
				ret = KdSSL_DigestSignUpdate(md_handle, KEDACOM, inlen);
				assert(!ret);
				count = BUFF_LEN;//这个必须要
				ret = KdSSL_DigestSignFinal(md_handle, out, (u32 *)&count);
				assert(!ret);
				KdSSL_MD_CTX_destroy(md_handle);
				printf("KdSSL_DigestSignFinal len %d\n", (int)count);

				md_handle = KdSSL_MD_CTX_create();
				memset(&pub_key, 0, sizeof(pub_key));
				pub_key.m_pbyPKeyBuff = (u8 *)argv[1];
				pub_key.m_dwPKeyLen = 0;//if KDC_EXT_KEY_FILE_NAME, not care
				pub_key.m_eKeyType = (strstr(argv[1], "pem") || strstr(argv[1], "cer")) ? KDC_EXT_KEY_CERT : KDC_EXT_KEY_PUBKEY_REQ;
				pub_key.m_eDataType = KDC_EXT_KEY_FILE_NAME;

				ret = KdSSL_DigestVerifyInit(md_handle, &pub_key, KDC_EXT_SM3, engine_id);
				assert(!ret);
				ret = KdSSL_DigestVerifyUpdate(md_handle, KEDACOM, inlen);
				assert(!ret);
				/* 验证签名结果，输入签名缓冲，和签名长度*/
				ret = KdSSL_DigestVerifyFinal(md_handle, out, (u32)count);
				printf("verify %s !!!\n", ret ? "failed" : "success");
				KdSSL_MD_CTX_destroy(md_handle);

				break;

			case 'a':
				count = 256;
				memset(buff, 0, sizeof(buff));
				ret = CalcFileSm3Hash(argv[1], buff, &count);
				printf("CalcFileSm3Hash ret = %d\n", ret);
				if (!ret)
					printf("hash: %s\n", buff);
				break;

			default:
				show_help();
				break;
		}
	}

	free(tmp);
	free(check);
	free(out);
	free(in);

	return 0;
}

