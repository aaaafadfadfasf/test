# -*- coding: cp936 -*-
#!python
'''
Author: liangli
Create: 2013.10.21
Usage: like cmsauto.exe
'''

import os
import sys
from headfileparse import *
#import headfileparse
import re

g_RawTypes = ["int", "bool", "double", "enum"]
#只支持序列容器，暂不支持map以及容器嵌套
g_ContainerTypes = ["vector"]
g_PrefixDict = {"int":"n", "bool":"b", "double":"f", "enum":"em", "string":"str", "other":"t"}
#搜集类型别名 exp: typedef vector<TNetAddr> CIpAddrList; = {“CIpAddrList”，“vector<TNetAddr>”}
g_TypedefDict = {}
g_NeedAutoGen = "NeedAutoGen"
g_Test = False
g_DebugLog = False


#结果集的类定义
class Collector:
    def Init(self):
        self.DataMembers = []
        self.FuncMembers = []
        self.IsClass = True
    def SetClassName(self, name):
        self.ClassName = name.strip()
    def SetParentName(self, parent):
        self.ParentClassName = parent.strip()
    def GetClassName(self):
        return self.ClassName
    def GetParentName(self):
        return self.ParentClassName
    def AddDataMember(self, dm):
        self.DataMembers.append(dm)
    def AddFuncMember(self, fm):
        self.FuncMembers.append(fm)
    def SetRawEndLine(self, line):
        self.RawEndLine = line
    def SetIsClass(self, bClass):
        self.IsClass = bClass
    def HasFunction(self, FuncName):
        for fm in self.FuncMembers:
            if FuncName == fm.Name:
                return True
        return False

class DataMember:
    def SetType(self, t):
        global g_TypedefDict
        self.Type = t.strip()
        self.Typedef = g_TypedefDict.get(self.Type, self.Type)
    def SetName(self, name):
        self.Name = name.strip()
    def SetNeedAutoGen(self, bNeed):
        self.NeedAutoGen = bNeed

class FuncMember:
    def SetReturnType(self, t):
        self.ReturnType = t.strip()
        self.Typedef = g_TypedefDict.get(self.ReturnType, self.ReturnType)
    def SetName(self, name):
        self.Name = name.strip()

#设置调试开关
def SetDebugLog(bLog):
    global g_DebugLog
    g_DebugLog = bLog

#调试打印函数
def DebugLog(*argv):
    global g_DebugLog
    if not g_DebugLog:
        return None
    sep = ' '
    end = '\n'
    of = sys.stdout
    output = ''
    first = True
    for a in argv:
        output += ('' if first else sep) + str(a)
        first = False
    of.write(output + end)

def GetFirstCapitalStr(s):
    l = s[:1]
    l = l.upper() + s[1:]
    return l


#解析类定义，分析类名和父类名
def ParseClassDefine(line):
    Divs = line.split(':')
    Words = Divs[0].strip('\t ').split()
    ClassName = Words[1]
    ParentClassName = ""
    bClass = False
    if line.find("class") != -1:
        #有class关键字
        bClass = True
    if len(Divs) > 1:
        Words = Divs[1].strip('\t ').split()
        ParentClassName = Words[1]
    return (ClassName,ParentClassName,bClass)

def ParseDataMember(line):
    global g_NeedAutoGen
    #可能还需要去掉注释
    Divs = line.strip().split(';')
    #类型有可能是 vector< ... >等复合类型
    dm = Divs[0].replace("static", "").strip()
    Words = dm.split()
    Type = Words[-2]
    Name = Words[-1]
    #存在模板参数
    if dm.find('<') != -1:
        Type = ""
        Name = ""
        #exp: a b < c > d
        Divs = dm.split('<') #Divs = ['a b ', ' c > d']
        tmpDivs = Divs[0].strip().split() #tmpDivs = ['a', 'b']
        Divs = Divs[1].strip().split('>') #Divs = [' c ', ' d']
        Type = tmpDivs[-1].strip() + "<" + Divs[0].strip() + ">"
        Name = Divs[1].strip()

    bNeedAutoGen = False
    if -1 != line.find(g_NeedAutoGen):
        bNeedAutoGen = True
    return (Type, Name, bNeedAutoGen)

#解析typedef，支持类型重命名
def ParseTypedef(line):
    global g_TypedefDict
    #由于typdef的格式与数据成员定义格式一致，可以直接调用ParseDataMember
    (Type, Name, bNeedAutoGen) = ParseDataMember(line)
    g_TypedefDict.setdefault(Name, Type)

def ParseFuncMember(line):
    Div = line.split('(')
    fm = Div[0].strip()
    Words = fm.split()
    RetType=""
    Name=""
    if len(Words) > 1:
        #构造函数没有返回类型
        if fm.find("<") != -1:
            #返回值存在模板类型 exp: a b < c > d
            Divs = fm.split('<') #Divs = ['a b ', ' c > d']
            tmpDivs = Divs[0].strip().split() #tmpDivs = ['a', 'b']
            Divs = Divs[1].strip().split('>') #Divs = [' c ', ' d']
            RetType = tmpDivs[-1].strip() + "<" + Divs[0].strip() + ">"
        else:
            RetType = Words[-2]
    Name = Words[-1]
    return (RetType, Name)

#生成类的成员函数XML访问接口
def GenFuncMemberXmlAccess(c):
    OffSet = "    "
    RetLines = []
    #自动生成Clear()等函数
    if c.IsClass:
        if not c.HasFunction("Clear"):
            s = "%svirtual void Clear();\r\n" % (OffSet); RetLines.append(s)
        if not c.HasFunction("ToXml"):
            s = "%svirtual const std::string ToXml()const;\r\n" % (OffSet); RetLines.append(s)
        if not c.HasFunction("ToXmlWithoutClassNameNode"):
            s = "%svirtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;\r\n" % (OffSet); RetLines.append(s)
        if not c.HasFunction("FromXml"):
            s = "%svirtual bool FromXml(const std::string& xml_string);\r\n" % (OffSet); RetLines.append(s)
            s = "%svirtual bool FromXml(const TiXmlNode& xmlNode);\r\n" % (OffSet); RetLines.append(s)
    else:
        if not c.HasFunction("Clear"):
            s = "%svirtual void Clear();\r\n" % (OffSet); RetLines.append(s)
        if not c.HasFunction("ToXmlWithoutClassNameNode"):
            s = "%svirtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;\r\n" % (OffSet); RetLines.append(s)
        if not c.HasFunction("FromXml"):
            s = "%svirtual bool FromXml(const TiXmlNode& xmlNode);\r\n" % (OffSet); RetLines.append(s)
    return RetLines

#生成类的成员函数set/get
def GenFuncMemberGetAndSet(c):
    global g_PrefixDict
    global g_RawTypes
    OffSet = "    "
    RetLines = []
    #自动生成get/set函数
    for dm in c.DataMembers:
        if dm.NeedAutoGen:
            #开始生成set/get
            bRawType=False
            TypeExt=dm.Type+"&"
            if dm.Type in g_RawTypes:
                bRawType=True
                TypeExt=dm.Type
            Prefix=g_PrefixDict.get(dm.Type, "t")
            ParamName=Prefix + GetFirstCapitalStr(dm.Name)
            #void set(const int nVal)
            s = "%svoid Set%s(const %s %s)\r\n" % (OffSet, dm.Name, TypeExt, ParamName); RetLines.append(s)
            s = OffSet + "{\r\n"; RetLines.append(s)
            s = "%s    %s = %s;\r\n" % (OffSet, dm.Name, ParamName); RetLines.append(s)
            s = OffSet + "}\r\n"; RetLines.append(s)
            #const int get() const
            s = "%sconst %s Get%s() const\r\n" % (OffSet, TypeExt, dm.Name); RetLines.append(s)
            s = OffSet + "{\r\n"; RetLines.append(s)
            s = "%s    return %s;\r\n" % (OffSet, dm.Name); RetLines.append(s)
            s = OffSet + "}\r\n"; RetLines.append(s)
            if not bRawType:
                s = "%s%s Get%s()\r\n" % (OffSet, TypeExt, dm.Name); RetLines.append(s)
                s = OffSet + "{\r\n"; RetLines.append(s)
                s = "%s    return %s;\r\n" % (OffSet, dm.Name); RetLines.append(s)
                s = OffSet + "}\r\n"; RetLines.append(s)
    return RetLines

def IsTypeInSet(Type, TypeList):
    bRet = False
    for t in TypeList:
        if Type.find(t) != -1:
            bRet = True
            break
    return bRet

#在自动生成cpp代码时，抽象出void xxx::Clear()函数的数据成员处理
def GenCppDataMemberClear(DataMembers, OffSet, RetLines):
    global g_RawTypes
    global g_ContainerTypes
    for dm in DataMembers:
        #简单数据成员直接清空
        if dm.Typedef == "double":
            s = "%s%s = INVALID_LONGITUDE;\r\n" % (OffSet, dm.Name);RetLines.append(s)
        elif dm.Typedef in g_RawTypes:
            s = "%s%s = 0;\r\n" % (OffSet, dm.Name);RetLines.append(s)
        #string类型
        elif dm.Typedef.find("string") != -1:
            s = "%s%s.clear();\r\n" % (OffSet, dm.Name);RetLines.append(s)
        #vector等容器类型
        elif IsTypeInSet(dm.Typedef, g_ContainerTypes):
            s = "%s%s.clear();\r\n" % (OffSet, dm.Name);RetLines.append(s)
        #结构体类型
        else:
            s = "%s%s.Clear();\r\n" % (OffSet, dm.Name);RetLines.append(s)

#在自动生成cpp代码时，抽象出void xxx::ToXml()函数的数据成员处理
def GenCppDataMemberToXml(DataMembers, OffSet, RetLines):
    global g_RawTypes
    global g_ContainerTypes
    for dm in DataMembers:
        #原生类型
        if dm.Typedef == "double":
            s = "%sif ( ! (INVALID_LONGITUDE-1 < %s) )\r\n" % (OffSet, dm.Name);RetLines.append(s)
            s = "%s{\r\n" % (OffSet);RetLines.append(s)
            s = "%sConvertHelper::ToXml::AddChildNode(xmlNode,\"%s\",\"%s\",%s);\r\n" % (OffSet*2, dm.Name, dm.Type, dm.Name);RetLines.append(s)
            s = "%s}\r\n\r\n" % (OffSet);RetLines.append(s)
        elif dm.Typedef in g_RawTypes:
            s = "%sConvertHelper::ToXml::AddChildNode(xmlNode,\"%s\",\"%s\",%s);\r\n\r\n" % (OffSet, dm.Name, dm.Type, dm.Name);RetLines.append(s)
        #string类型
        elif dm.Typedef.find("string") != -1:
            s = "%sif(! %s.empty() )\r\n" % (OffSet, dm.Name);RetLines.append(s)
            s = "%s{\r\n" % (OffSet);RetLines.append(s)
            s = "%sConvertHelper::ToXml::AddChildNode(xmlNode,\"%s\",\"%s\",%s);\r\n" % (OffSet*2, dm.Name, dm.Type, dm.Name);RetLines.append(s)
            s = "%s}\r\n\r\n" % (OffSet);RetLines.append(s)
        #vector等容器类型
        elif IsTypeInSet(dm.Typedef, g_ContainerTypes):
            s = "%sConvertHelper::ToXml::AddContainerGBStyle(xmlNode,%s,\"%s\",\"Item\");\r\n\r\n" % (OffSet, dm.Name, dm.Name);RetLines.append(s)
        #kdmext类型以及其它类型
        else:
            s = "%sif(! %s.empty() )\r\n" % (OffSet, dm.Name);RetLines.append(s)
            s = "%s{\r\n" % (OffSet);RetLines.append(s)
            s = "%sConvertHelper::ToXml::AddChildNode(xmlNode,\"%s\",\"%s\",%s);\r\n" % (OffSet*2, dm.Name, dm.Type, dm.Name);RetLines.append(s)
            s = "%s}\r\n\r\n" % (OffSet);RetLines.append(s)

#在自动生成cpp代码时，抽象出void xxx::FromXml()函数的数据成员处理
def GenCppDataMemberFromXml(DataMembers, OffSet, RetLines):
    global g_RawTypes
    RawTypes = g_RawTypes[:]
    RawTypes.append("string")
    for dm in DataMembers:
        #原生类型(包括string)
        if IsTypeInSet(dm.Typedef, RawTypes):
            s = "%sConvertHelper::ToStruct::ParseChildNode(*pXmlNode,\"%s\",%s);\r\n\r\n" % (OffSet, dm.Name, dm.Name);RetLines.append(s)
        elif IsTypeInSet(dm.Typedef, g_ContainerTypes):
            s = "%sConvertHelper::ToStruct::ParseContainerGBStyle(*pXmlNode,%s,\"%s\",\"Item\");\r\n\r\n" % (OffSet, dm.Name, dm.Name);RetLines.append(s)
        else:
            s = "%sConvertHelper::ToStruct::ParseChildNode(xmlNode,\"%s\",%s);\r\n\r\n" % (OffSet, dm.Name, dm.Name);RetLines.append(s)
            
def GenCppDataMemberFromXmlRevise(DataMembers, OffSet, RetLines):
    global g_RawTypes
    RawTypes = g_RawTypes[:]
    RawTypes.append("string")
    for dm in DataMembers:
        #原生类型(包括string)
        if IsTypeInSet(dm.Typedef, RawTypes):
            s = "%sConvertHelper::ToStruct::ParseChildNode(*pXmlNode,\"%s\",%s);\r\n\r\n" % (OffSet, dm.Name[:-3], dm.Name);RetLines.append(s)
        elif IsTypeInSet(dm.Typedef, g_ContainerTypes):
            s = "%sConvertHelper::ToStruct::ParseContainerGBStyle(*pXmlNode,%s,\"%s\",\"Item\");\r\n\r\n" % (OffSet, dm.Name[:-3], dm.Name);RetLines.append(s)
        else:
            s = "%sConvertHelper::ToStruct::ParseChildNode(xmlNode,\"%s\",%s);\r\n\r\n" % (OffSet, dm.Name[:-3], dm.Name);RetLines.append(s)
                        
def GenCppForStruct(c, RetLines):
    OffSet = "    "
    global g_PrefixDict
    global g_RawTypes
    s = "void %s::Clear()\r\n" % (c.ClassName);RetLines.append(s)
    s = "{\r\n";RetLines.append(s)
    #Clear() body
    GenCppDataMemberClear(c.DataMembers, OffSet, RetLines)
    if c.ParentClassName:
        s = "%s%s::Clear();\r\n\r\n" % (OffSet, c.ParentClassName);RetLines.append(s)
    s = "}\r\n\r\n";RetLines.append(s)
    #ToXmlWithoutClassNameNode() Body
    s = "void %s::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode) const\r\n" % (c.ClassName);RetLines.append(s)
    s = "{\r\n";RetLines.append(s)
    GenCppDataMemberToXml(c.DataMembers, OffSet, RetLines)
    s = "}\r\n\r\n";RetLines.append(s)
    #bool FromXml(xmlNode ) Body
    s = "bool %s::FromXml(const TiXmlNode& xmlNode)\r\n" % (c.ClassName);RetLines.append(s)
    s = "{\r\n";RetLines.append(s)
    s = "%sbool bRet = true;\r\n" % (OffSet);RetLines.append(s)
    s = "%sClear();\r\n\r\n" % (OffSet);RetLines.append(s)
    if len(c.DataMembers) > 0:
        s = "%sconst TiXmlNode* pXmlNode = xmlNode.ToElement();\r\n\r\n" % (OffSet);RetLines.append(s)
    GenCppDataMemberFromXml(c.DataMembers, OffSet, RetLines)
    if c.ClassName == "TConfigVideoParamOpt":
        s = "%sif (empty())\r\n" %(OffSet);RetLines.append(s)
        s = "%s{\r\n" %(OffSet);RetLines.append(s)
        GenCppDataMemberFromXmlRevise(c.DataMembers, OffSet*2, RetLines)
        s = "%s}\r\n" %(OffSet);RetLines.append(s)
    s = "%sreturn bRet;\r\n" % (OffSet);RetLines.append(s)
    s = "}\r\n\r\n";RetLines.append(s)

def GenCppForClass(c, RetLines):
    OffSet = "    "
    global g_PrefixDict
    global g_RawTypes
    s = "void %s::Clear()\r\n" % (c.ClassName);RetLines.append(s)
    s = "{\r\n";RetLines.append(s)
    #Clear() body
    GenCppDataMemberClear(c.DataMembers, OffSet, RetLines)
    if c.ParentClassName:
        s = "%s%s::Clear();\r\n\r\n" % (OffSet, c.ParentClassName);RetLines.append(s)
    s = "}\r\n\r\n";RetLines.append(s)
    #string ToXml() Body
    s = "const std::string %s::ToXml() const\r\n" % (c.ClassName);RetLines.append(s)
    s = "{\r\n";RetLines.append(s)
    s = "%sClearXmlDocument();\r\n" % (OffSet);RetLines.append(s)
    if c.ParentClassName:
        s = "%s%s::ToXml(xmlDoc);\r\n\r\n" % (OffSet, c.ParentClassName);RetLines.append(s)
    s = "%sToXmlWithoutClassNameNode(GetXmlNode());\r\n\r\n" % (OffSet);RetLines.append(s)
    s = "%sTiXmlPrinter printer;\r\n" % (OffSet);RetLines.append(s)
    s = "%sprinter.SetStreamPrinting(\"\\r\\n\");\r\n" % (OffSet);RetLines.append(s)
    s = "%sxmlDoc.Accept(&printer);\r\n" % (OffSet);RetLines.append(s)
    s = "%sreturn printer.Str();\r\n" % (OffSet);RetLines.append(s)
    s = "}\r\n\r\n";RetLines.append(s)
    #ToXmlWithoutClassNameNode() Body
    s = "void %s::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode) const\r\n" % (c.ClassName);RetLines.append(s)
    s = "{\r\n";RetLines.append(s)
    GenCppDataMemberToXml(c.DataMembers, OffSet, RetLines)
    s = "}\r\n\r\n";RetLines.append(s)
    #bool FromXml(string ) Body
    s = "bool %s::FromXml(const std::string& xml_string)\r\n" % (c.ClassName);RetLines.append(s)
    s = "{\r\n";RetLines.append(s)
    s = "%sTiXmlDocument xmlNode;\r\n" % (OffSet);RetLines.append(s)
    s = "%sxmlNode.Parse(xml_string.c_str());\r\n" % (OffSet);RetLines.append(s)
    s = "%sif (xmlNode.Error())\r\n" % (OffSet);RetLines.append(s)
    s = "%s{\r\n" % (OffSet);RetLines.append(s)
    s = "%sCMS_ASSERT(0&&\"error Parse xml string, maybe not integrated\");\r\n" % (OffSet*2);RetLines.append(s)
    s = "%sreturn false;\r\n" % (OffSet*2);RetLines.append(s)
    s = "%s}\r\n\r\n" % (OffSet);RetLines.append(s)
    s = "%sreturn FromXml(*xmlNode.FirstChild()->NextSibling());\r\n" % (OffSet);RetLines.append(s)
    s = "}\r\n\r\n";RetLines.append(s)
    #bool FromXml(xmlNode ) Body
    s = "bool %s::FromXml(const TiXmlNode& xmlNode)\r\n" % (c.ClassName);RetLines.append(s)
    s = "{\r\n";RetLines.append(s)
    s = "%sbool bRet = true;\r\n" % (OffSet);RetLines.append(s)
    s = "%sClear();\r\n\r\n" % (OffSet);RetLines.append(s)
    if c.ParentClassName:
        s = "%sbRet &= %s::FromXml(xmlNode);\r\n\r\n" % (OffSet, c.ParentClassName);RetLines.append(s)

    if len(c.DataMembers) > 0:
        s = "%sconst TiXmlNode* pXmlNode = xmlNode.ToElement();\r\n\r\n" % (OffSet);RetLines.append(s)
    GenCppDataMemberFromXml(c.DataMembers, OffSet, RetLines)
    s = "%sreturn bRet;\r\n" % (OffSet);RetLines.append(s)
    s = "}\r\n\r\n";RetLines.append(s)


#生成cpp源代码
def GenProtoFunc(AllClass):
    RetLines = []
    OffSet = "    "
    s = "#include \"cms/gb/gb_struct.h\"\r\n";RetLines.append(s)
    s = "#include \"cms/gb/gb_proto.h\"\r\n\r\n";RetLines.append(s)
    for c in AllClass:
        if c.IsClass:
            GenCppForClass(c, RetLines)
        else:
            GenCppForStruct(c, RetLines)
    s = "//End of file\r\n\r\n";RetLines.append(s)
    return RetLines

#解析头文件，生成类列表
def ParseHeadFile(inputLines, allClass):
    curIndex = 0
    totalLine = len(inputLines)

    while curIndex < totalLine:
        #跳过注释行
        if IsCommentLine(inputLines[curIndex]):
            bComment = True
            while bComment:
                if IsCommentLine(inputLines[curIndex]):
                    DebugLog( "line_%d is comment: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                    bMultiComment = False
                    if IsMultiCommentLineBegin(inputLines[curIndex]):
                        DebugLog( "line_%d is multi-line comment begin: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                        bMultiComment = True
                        while bMultiComment:
                            if IsMultiCommentLineEnd(inputLines[curIndex]):
                                DebugLog( "line_%d is multi-line comment End: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                                break
                            curIndex+=1
                    curIndex+=1
                    #判断注释是否为文件最后一行
                    if curIndex >= totalLine:
                        return None
                    DebugLog( "Jump out of comment at line_%d: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                else:
                    break

        if IsTypedef(inputLines[curIndex]):
            DebugLog( "line_%d is typedef: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
            ParseTypedef(inputLines[curIndex])
            curIndex+=1
            continue

        if not IsClassDefine(inputLines[curIndex]):
            DebugLog( "line_%d is not class/struct define, pass: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
            curIndex+=1
            continue
        #开始解析类
        c = Collector()
        c.Init()
        #获取类名
        (name, parent, bClass) = ParseClassDefine(inputLines[curIndex])
        c.SetClassName(name)
        c.SetParentName(parent)
        c.SetIsClass(bClass)

        DebugLog( "Line_%d is class/struct define Begin: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
        DebugLog( "\tclassName: %s, ParentName: %s" % (name, parent))

        curIndex+=1
        bIsInClassBody=True
        while bIsInClassBody:
            DebugLog( "Parse class body at line_%d: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))

            #跳过注释行
            if IsCommentLine(inputLines[curIndex]):
                bComment = True
                while bComment:
                    if IsCommentLine(inputLines[curIndex]):
                        DebugLog( "line_%d is comment: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                        bMultiComment = False
                        if IsMultiCommentLineBegin(inputLines[curIndex]):
                            DebugLog( "line_%d is multi-line comment begin: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                            bMultiComment = True
                            while bMultiComment:
                                if IsMultiCommentLineEnd(inputLines[curIndex]):
                                    DebugLog( "line_%d is multi-line comment End: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                                    break
                                curIndex+=1
                        curIndex+=1
                        DebugLog( "Jump out of comment at line_%d: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                    else:
                        break

            #解析typedef
            if IsTypedef(inputLines[curIndex]):
                DebugLog( "line_%d is typedef: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                ParseTypedef(inputLines[curIndex])
                curIndex+=1

            #解析类体
            elif IsDataMember(inputLines[curIndex]):
                #数据成员必须在一行内
                dm = DataMember()
                (Type, Name, bNeedAutoGen) = ParseDataMember(inputLines[curIndex])
                DebugLog( "line_%d is DataMember: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                DebugLog( "\tType: %s, Name: %s, IsNeedAutoGen: %s" % (Type, Name, bNeedAutoGen) )

                dm.SetType(Type)
                dm.SetName(Name)
                dm.SetNeedAutoGen(bNeedAutoGen)
                c.AddDataMember(dm)
                #去除g_NeedAutoGen标记
                if bNeedAutoGen:
                    inputLines[curIndex] = "    "+inputLines[curIndex].replace(g_NeedAutoGen, '').lstrip()
                    DebugLog( "\tRemove Flag: %s to line: %s" % (g_NeedAutoGen, inputLines[curIndex]))
                curIndex+=1
            elif IsFuncMemberDefineBegin(inputLines[curIndex]):
                #记录函数定义所在行
                FuncDefineLine = curIndex
                DebugLog( "line_%d Is FuncMember define Begin: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))

                bIsInFuncDefine=True
                bIsInFuncBody=False

                #解析函数头定义，可能有多行
                while bIsInFuncDefine:
                    #以 ) 为标识找到函数定义的参数列表结束
                    if IsFuncMemberDefineEnd(inputLines[curIndex]):
                        DebugLog("line_%d find FuncMember define End: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))

                        if inputLines[curIndex].find(';') != -1:
                            #成员函数定义以 ; 结尾，没有内联实现，直接解析下一行
                            DebugLog("\tFunc End WITHOUT body")
                            curIndex+=1
                        else:
                            #成员函数还有内联实现，还要继续解析Body
                            bIsInFuncBody=True
                            DebugLog("\tFunc End, a body after!")

                        fm = FuncMember()
                        (Type, Name) = ParseFuncMember(inputLines[FuncDefineLine])
                        fm.SetReturnType(Type)
                        fm.SetName(Name)
                        c.AddFuncMember(fm)
                        bIsInFuncDefine=False
                        DebugLog( "\tFunc ReturnType: %s, Name: %s" % (Type, Name))
                        break
                    else:
                        curIndex+=1

                #解析函数体定义，采用大括号成对出现引用计数法，因此不支持一行内出现{{两次
                beginRef=0
                endRef=0
                while bIsInFuncBody:
                    DebugLog("line_%d is in FuncBody: %s"  % (curIndex+1, inputLines[curIndex].rstrip() ))

                    if IsFuncBodyBegin(inputLines[curIndex]):
                        beginRef+=1
                        DebugLog("line_%d via FuncBody Begin, beginRef = %d: %s" % (curIndex+1, beginRef, inputLines[curIndex].rstrip() ))

                    if IsFuncBodyEnd(inputLines[curIndex]):
                        endRef+=1
                        DebugLog("line_%d via FuncBody End, endRef = %d: %s" % (curIndex+1, endRef, inputLines[curIndex].rstrip() ))

                    if beginRef == endRef and beginRef > 0:
                        #函数部分结束
                        DebugLog("line_%d is FuncBody Ended!: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                        curIndex+=1
                        bIsInFuncBody=False
                        break

                    curIndex+=1
                    #从算法上看，函数体结束不能与类定义结束在同一行，否则解析不出类定义
            elif -1 != inputLines[curIndex].find("};"):
                #类定义结束
                DebugLog( "line_%d Encounter class End: %s"  % (curIndex+1, inputLines[curIndex].rstrip() ))

                c.SetRawEndLine(curIndex)
                allClass.append(c)
                curIndex+=1
                bIsInClassBody=False
                break
            else:
            #其他行忽略，包括注释行
                DebugLog( "line_%d is neither DataMember nor FuncMember, Ignore it: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                curIndex+=1
    return None




def PrintAllClass(allclass):
    OffSet = "    "
    for c in allclass:
        print "class ", c.ClassName, " Parent :", c.ParentClassName, "IsClass: ", c.IsClass
        for dm in c.DataMembers:
            print OffSet, "DataMember ", dm.Name, ",Type = ", dm.Type, ",NeedAutoGen = ", dm.NeedAutoGen
        for fm in c.FuncMembers:
            print OffSet, "FuncMember ", fm.Name, ",ReturnType = ", fm.ReturnType
        print "class End at Line", c.RawEndLine
        print "Gen XML Access FuncMember"
        Lines = GenFuncMemberXmlAccess(c)
        for l in Lines:
            print l
        print "Gen Get Set FuncMember"
        #todo 需要逆向生成
        Lines = GenFuncMemberGetAndSet(c)
        for l in Lines:
            print l

def TestRegx(Lines, func):
    LineIndex = 0
    for l in Lines:
        LineIndex+=1
        m = func(l)
        if m:
            (Type, Name, bNeedAutoGen) = ParseDataMember(l)
            print "line_%d: %s --> DataType=%s, Name=%s, NeedAutoGen=%s" % (LineIndex, l.rstrip(), Type, Name, bNeedAutoGen)

#主函数，输入头文件，输入写入新文件
def GBProtoAuto(inputFileName, outputFileName):
    global g_DebugLog
    global g_Test

    try:
        inf = open(inputFileName, "rb+")
    except IOError:
        print "can NOT open %s" % (inputFileName)
        return None
    try:
        outf = open(outputFileName, "wb+");
    except IOError:
        print "can NOT open %s" % (outputFileName)
        return None

    inputLines = inf.readlines();

    if g_Test:
        inf.seek(0, 0)
        inputContent = inf.read();
        RetStr = re.findall("^\s*(static\s+)?((\w+)|(\w+\s*<\s*\w+\s*>))\s+\w+\s*(?!\();.*", inputContent, re.M)
        print RetStr
        print "============================\n"
        TestRegx(inputLines, IsDataMember)
        inf.close()
        outf.close()
        return None

    print("Begin to parse headfile %s" % (inputFileName) )
    allClass = []
    ParseHeadFile(inputLines, allClass)

    if g_DebugLog:
        print "================ print all class ================\n"
        PrintAllClass(allClass)

    #生成CPP文件
    print ("Begin to create proto function cpp file\n\n")
    Lines = GenProtoFunc(allClass)
    if len(Lines) > 0:
        outf.seek(0,0)
        outf.writelines(Lines)

    #重新生成头文件
    allClass.reverse()
    for c in allClass:
        InsertIndex = c.RawEndLine
        #生成XML的相关接口
        Lines = GenFuncMemberXmlAccess(c)
        if len(Lines) > 0:
            s = "public:\r\n"
            inputLines.insert(InsertIndex, s); InsertIndex+=1
            for l in Lines:
                inputLines.insert(InsertIndex, l); InsertIndex+=1
        #生成get/set接口
        Lines = GenFuncMemberGetAndSet(c)
        if len(Lines) > 0:
            s = "public:\r\n"
            inputLines.insert(InsertIndex, s); InsertIndex+=1
            for l in Lines:
                inputLines.insert(InsertIndex, l); InsertIndex+=1

    inf.seek(0,0)
    inf.writelines(inputLines)

    #关闭文件并退出
    inf.close()
    outf.close()







