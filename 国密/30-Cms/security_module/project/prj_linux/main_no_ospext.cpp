#include "security_module_proto.h"
#include <cstdio>

int main(){
	CSecurityModuleLoginReq req;
	req.SetLoginId("testid");
	string str=req.ToXml();
	printf("%s %d\n",str.c_str(),req.GetSeqNum());
	CSecurityModuleLoginRsp rsp;
    return 0;
}
