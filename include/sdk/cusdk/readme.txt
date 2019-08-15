//描述该目录下的详细情况

本目录下存放cusdk.h头文件和cusdk.wsdl，用于描述CU的WebService所有接口
客户端的使用者可通过以下gsoap命令生成自己的客户端代码

soapcpp2 -C  -T -i cusdk.h -I<你的gsoap安装目录soap-2.8\gsoap\import>

另外，在10-Common\include\cms目录下存放了kdtype.gsoap文件，该文件描述了WebService公共基础类型
也需要包含到以上命令的引用路径中

CU客户端:
soapcpp2 -C  -T -i cusdk.h -I../ -I../../system/soap/import/

CUI侧:
soapcpp2 -S  -T -i cusdk.h -I../ -I../../system/soap/import/