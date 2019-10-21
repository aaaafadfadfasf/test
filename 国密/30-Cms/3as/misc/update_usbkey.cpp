#include <cstdio>
#include "cbb/license/usbkeyconst.h"
#include "cbb/license/readlicense.h"
#include <string.h>

// 放置于30-Cms/kdmutil/prj_linux目录下面使用下面的命名编译
// g++ -O2 update_usbkey.cpp -o update_usbkey -D_LINUX_ -I../../../10-Common/include -I../../../10-Common/include/cbb/osp -L../../../10-Common/lib/release/linux -llicensesdk -lhkapi -lepassapi -lusb -lEleCrypt -lsenseELE -losp -lrt 
int main(int argc, char* argv[])
{
	int nret=10043;
	if(argc!=2) return nret;
	
	if(!IsUsbKeyOnline()) 
	{
		printf("no usbkey");
		return nret;
	}
	
	int nTmp;
	s8 usbkeyid[1024];
	s32 nLicenseLen;
	
	nTmp = ReadLicenseFile(USB_PATH);
	if (0 != nTmp)
    {
        printf("ReadLicenseFile4usb failed , errorcode=%d", nTmp);
        return nret;
    }
	nLicenseLen=sizeof(usbkeyid)-1;
	nTmp=GetValue("KEY_LICENSE_KEY",usbkeyid, nLicenseLen);
	if(0!=nTmp)
	{
		printf("get usb licenseid failed , errorcode=%d", nTmp);
		return nret;
	}
	

	s8 filekeyid[1024];
    nTmp = ReadLicenseFile(argv[1]);
    if (0 != nTmp)
    {
        printf("ReadLicenseFile4file failed , errorcode=%d", nTmp);
        return nret;
    }
	nLicenseLen=sizeof(filekeyid)-1;
	nTmp=GetValue("KEY_LICENSE_KEY",filekeyid, nLicenseLen);
	if(0!=nTmp)
	{
		printf("get file licenseid failed , errorcode=%d", nTmp);
		return nret;
	}
	if(strncmp(usbkeyid,filekeyid,1024)!=0)
	{
		printf("filekey's keyid is not same to usekey's keyid");
        return nret;
	}
	
    
	nTmp = WriteLicenseFile(TRUE);
    if (SUCCESS_LICENSE != nTmp)
    {
        printf("WriteLicenseFile failed, errnocode=%d", nTmp);
        return nret;
    }
	
	return 0;
}
