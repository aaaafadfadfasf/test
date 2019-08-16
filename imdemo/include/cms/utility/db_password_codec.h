
#ifndef DB_PASSOWRD_CODEC_H
# define DB_PASSOWRD_CODEC_H

#include <string>
using std::string;

//加密数据库密码，使用的是des+base64实现。主要为了便于存储在xml里，直接调用该接口就可以
string EncryptMySqlPassword(const string& strBuf);

//解密数据库密码，使用的是des+base64实现，直接调用该接口就可以
string DecryptMySqlPassword(const string& strBuf);

#endif /* DB_PASSOWRD_CODEC_H */
