
#ifndef DB_PASSOWRD_CODEC_H
# define DB_PASSOWRD_CODEC_H

#include <string>
using std::string;

//�������ݿ����룬ʹ�õ���des+base64ʵ�֡���ҪΪ�˱��ڴ洢��xml�ֱ�ӵ��øýӿھͿ���
string EncryptMySqlPassword(const string& strBuf);

//�������ݿ����룬ʹ�õ���des+base64ʵ�֣�ֱ�ӵ��øýӿھͿ���
string DecryptMySqlPassword(const string& strBuf);

#endif /* DB_PASSOWRD_CODEC_H */
