#include "inner_common.h"

void ProtocolTest()
{
    CVideosourcePrivilege cPrivi;
    cPrivi.SetPTZCtrl(9);
    printf("ptz 9, int:%u, string:%s\n", cPrivi.Convert2Int(), cPrivi.ToXml().c_str());
}

void GenerateXmlContent()
{
    CConfigTasSetReq cReq;
    cReq.SetParentDomainId("6e503463ef9e4dd7b3f933f3888aaede");
    cReq.SetParentDomainName("china");
    printf("\nXmlContent:%s\n", cReq.ToXml().c_str());

    CChildtasAddReq cReq2;
    cReq2.SetChildDomainId("111");
    cReq2.SetChildDomainName("jiangsu");
    printf("\nXmlContent:%s\n", cReq2.ToXml().c_str());

    CChildtasDelReq cReq3;
    cReq3.SetChildDomainId("111");
    printf("\nXmlContent:%s\n", cReq3.ToXml().c_str());

    CDeviceCapIndexNameSetReq cReq4;
    TDeviceCapName &tCapName = cReq4.GetDeviceCapName();
    tCapName.deviceId = "32969c54c0a0460ea1fdbf6d79d731f6";
    tCapName.capId = DEVICE_CAP_VIDEOSOURCE;
    TDeviceCapIndexName tDevCapIndexname;
    tDevCapIndexname.index = 0;
    tDevCapIndexname.name = "alias test";
    tCapName.deviceCapIndexNames.push_back(tDevCapIndexname);
    printf("\nXmlContent:%s\n", cReq4.ToXml().c_str());
}
