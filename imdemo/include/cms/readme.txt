//描述该目录下的详细情况

cms目录用于存放平台服务器自己的头文件

//公共类
commonconfig 存放所有服务器模块的公共配置，其中包括防拷贝功能
container    存放xmap等自定义容器类
cppunitlite  存放unittest定义
ospext       存放Osp扩展库头文件定义
ospsip       存放Sip协议栈封装库，让其使用
utility      存放一些工具类，例如UUID生成器，netmanger

//公共文件定义
cms_const.h  所有常量定义
cms_eventid.h 消息ID分段
cms_proto.h  消息体公共基类
cmsassert.h  断言
cms_errorcode.h 所有错误码

//模块定义
pms          存放PMC_PMS之间的消息接口定义
tas          存放3AC_3AS之间的消息接口定义
cmu          存放所有与CMU打交道的消息接口定义，包括CUI,PUI,VTDU

//模块消息定义
以TAS举例
tas/tas_event.h 存放3AC_3AS的所有消息
tas/tas_proto.h 存放所有消息体定义，这些消息体带有toxml和fromxml接口
tas/tas_struct.h 存放消息体中的一些公共结构体，这些结构体只是数据复用，没有toxml等接口