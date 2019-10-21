gb协议自动生成工具目录。

1. 数据成员自动生成方法
在数据成员的前面添加 NeedAutoGen，然后运行gbauto.py。
比如
private:
NeedAutoGen string StartTime;
NeedAutoGen string EndTime;

注：以下类型本工具暂不能很好支持，可使用其他类型替代，具体如下（箭头前为不支持类型，箭头后为支持类型）：
float-->double
size_t-->int
xsd__dateTimer---->string


附录：
windows上面python工具安装，安装之后gbauto.py脚本就可以直接双击运行了。
\\172.16.0.2\监控-平台产品部\00公共资料\python\python-2.7.5.msi