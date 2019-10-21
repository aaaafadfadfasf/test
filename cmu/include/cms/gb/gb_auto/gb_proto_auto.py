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
#ֻ֧�������������ݲ�֧��map�Լ�����Ƕ��
g_ContainerTypes = ["vector"]
g_PrefixDict = {"int":"n", "bool":"b", "double":"f", "enum":"em", "string":"str", "other":"t"}
#�Ѽ����ͱ��� exp: typedef vector<TNetAddr> CIpAddrList; = {��CIpAddrList������vector<TNetAddr>��}
g_TypedefDict = {}
g_NeedAutoGen = "NeedAutoGen"
g_Test = False
g_DebugLog = False


#��������ඨ��
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

#���õ��Կ���
def SetDebugLog(bLog):
    global g_DebugLog
    g_DebugLog = bLog

#���Դ�ӡ����
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


#�����ඨ�壬���������͸�����
def ParseClassDefine(line):
    Divs = line.split(':')
    Words = Divs[0].strip('\t ').split()
    ClassName = Words[1]
    ParentClassName = ""
    bClass = False
    if line.find("class") != -1:
        #��class�ؼ���
        bClass = True
    if len(Divs) > 1:
        Words = Divs[1].strip('\t ').split()
        ParentClassName = Words[1]
    return (ClassName,ParentClassName,bClass)

def ParseDataMember(line):
    global g_NeedAutoGen
    #���ܻ���Ҫȥ��ע��
    Divs = line.strip().split(';')
    #�����п����� vector< ... >�ȸ�������
    dm = Divs[0].replace("static", "").strip()
    Words = dm.split()
    Type = Words[-2]
    Name = Words[-1]
    #����ģ�����
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

#����typedef��֧������������
def ParseTypedef(line):
    global g_TypedefDict
    #����typdef�ĸ�ʽ�����ݳ�Ա�����ʽһ�£�����ֱ�ӵ���ParseDataMember
    (Type, Name, bNeedAutoGen) = ParseDataMember(line)
    g_TypedefDict.setdefault(Name, Type)

def ParseFuncMember(line):
    Div = line.split('(')
    fm = Div[0].strip()
    Words = fm.split()
    RetType=""
    Name=""
    if len(Words) > 1:
        #���캯��û�з�������
        if fm.find("<") != -1:
            #����ֵ����ģ������ exp: a b < c > d
            Divs = fm.split('<') #Divs = ['a b ', ' c > d']
            tmpDivs = Divs[0].strip().split() #tmpDivs = ['a', 'b']
            Divs = Divs[1].strip().split('>') #Divs = [' c ', ' d']
            RetType = tmpDivs[-1].strip() + "<" + Divs[0].strip() + ">"
        else:
            RetType = Words[-2]
    Name = Words[-1]
    return (RetType, Name)

#������ĳ�Ա����XML���ʽӿ�
def GenFuncMemberXmlAccess(c):
    OffSet = "    "
    RetLines = []
    #�Զ�����Clear()�Ⱥ���
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

#������ĳ�Ա����set/get
def GenFuncMemberGetAndSet(c):
    global g_PrefixDict
    global g_RawTypes
    OffSet = "    "
    RetLines = []
    #�Զ�����get/set����
    for dm in c.DataMembers:
        if dm.NeedAutoGen:
            #��ʼ����set/get
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

#���Զ�����cpp����ʱ�������void xxx::Clear()���������ݳ�Ա����
def GenCppDataMemberClear(DataMembers, OffSet, RetLines):
    global g_RawTypes
    global g_ContainerTypes
    for dm in DataMembers:
        #�����ݳ�Աֱ�����
        if dm.Typedef == "double":
            s = "%s%s = INVALID_LONGITUDE;\r\n" % (OffSet, dm.Name);RetLines.append(s)
        elif dm.Typedef in g_RawTypes:
            s = "%s%s = 0;\r\n" % (OffSet, dm.Name);RetLines.append(s)
        #string����
        elif dm.Typedef.find("string") != -1:
            s = "%s%s.clear();\r\n" % (OffSet, dm.Name);RetLines.append(s)
        #vector����������
        elif IsTypeInSet(dm.Typedef, g_ContainerTypes):
            s = "%s%s.clear();\r\n" % (OffSet, dm.Name);RetLines.append(s)
        #�ṹ������
        else:
            s = "%s%s.Clear();\r\n" % (OffSet, dm.Name);RetLines.append(s)

#���Զ�����cpp����ʱ�������void xxx::ToXml()���������ݳ�Ա����
def GenCppDataMemberToXml(DataMembers, OffSet, RetLines):
    global g_RawTypes
    global g_ContainerTypes
    for dm in DataMembers:
        #ԭ������
        if dm.Typedef == "double":
            s = "%sif ( ! (INVALID_LONGITUDE-1 < %s) )\r\n" % (OffSet, dm.Name);RetLines.append(s)
            s = "%s{\r\n" % (OffSet);RetLines.append(s)
            s = "%sConvertHelper::ToXml::AddChildNode(xmlNode,\"%s\",\"%s\",%s);\r\n" % (OffSet*2, dm.Name, dm.Type, dm.Name);RetLines.append(s)
            s = "%s}\r\n\r\n" % (OffSet);RetLines.append(s)
        elif dm.Typedef in g_RawTypes:
            s = "%sConvertHelper::ToXml::AddChildNode(xmlNode,\"%s\",\"%s\",%s);\r\n\r\n" % (OffSet, dm.Name, dm.Type, dm.Name);RetLines.append(s)
        #string����
        elif dm.Typedef.find("string") != -1:
            s = "%sif(! %s.empty() )\r\n" % (OffSet, dm.Name);RetLines.append(s)
            s = "%s{\r\n" % (OffSet);RetLines.append(s)
            s = "%sConvertHelper::ToXml::AddChildNode(xmlNode,\"%s\",\"%s\",%s);\r\n" % (OffSet*2, dm.Name, dm.Type, dm.Name);RetLines.append(s)
            s = "%s}\r\n\r\n" % (OffSet);RetLines.append(s)
        #vector����������
        elif IsTypeInSet(dm.Typedef, g_ContainerTypes):
            s = "%sConvertHelper::ToXml::AddContainerGBStyle(xmlNode,%s,\"%s\",\"Item\");\r\n\r\n" % (OffSet, dm.Name, dm.Name);RetLines.append(s)
        #kdmext�����Լ���������
        else:
            s = "%sif(! %s.empty() )\r\n" % (OffSet, dm.Name);RetLines.append(s)
            s = "%s{\r\n" % (OffSet);RetLines.append(s)
            s = "%sConvertHelper::ToXml::AddChildNode(xmlNode,\"%s\",\"%s\",%s);\r\n" % (OffSet*2, dm.Name, dm.Type, dm.Name);RetLines.append(s)
            s = "%s}\r\n\r\n" % (OffSet);RetLines.append(s)

#���Զ�����cpp����ʱ�������void xxx::FromXml()���������ݳ�Ա����
def GenCppDataMemberFromXml(DataMembers, OffSet, RetLines):
    global g_RawTypes
    RawTypes = g_RawTypes[:]
    RawTypes.append("string")
    for dm in DataMembers:
        #ԭ������(����string)
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
        #ԭ������(����string)
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


#����cppԴ����
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

#����ͷ�ļ����������б�
def ParseHeadFile(inputLines, allClass):
    curIndex = 0
    totalLine = len(inputLines)

    while curIndex < totalLine:
        #����ע����
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
                    #�ж�ע���Ƿ�Ϊ�ļ����һ��
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
        #��ʼ������
        c = Collector()
        c.Init()
        #��ȡ����
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

            #����ע����
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

            #����typedef
            if IsTypedef(inputLines[curIndex]):
                DebugLog( "line_%d is typedef: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                ParseTypedef(inputLines[curIndex])
                curIndex+=1

            #��������
            elif IsDataMember(inputLines[curIndex]):
                #���ݳ�Ա������һ����
                dm = DataMember()
                (Type, Name, bNeedAutoGen) = ParseDataMember(inputLines[curIndex])
                DebugLog( "line_%d is DataMember: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                DebugLog( "\tType: %s, Name: %s, IsNeedAutoGen: %s" % (Type, Name, bNeedAutoGen) )

                dm.SetType(Type)
                dm.SetName(Name)
                dm.SetNeedAutoGen(bNeedAutoGen)
                c.AddDataMember(dm)
                #ȥ��g_NeedAutoGen���
                if bNeedAutoGen:
                    inputLines[curIndex] = "    "+inputLines[curIndex].replace(g_NeedAutoGen, '').lstrip()
                    DebugLog( "\tRemove Flag: %s to line: %s" % (g_NeedAutoGen, inputLines[curIndex]))
                curIndex+=1
            elif IsFuncMemberDefineBegin(inputLines[curIndex]):
                #��¼��������������
                FuncDefineLine = curIndex
                DebugLog( "line_%d Is FuncMember define Begin: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))

                bIsInFuncDefine=True
                bIsInFuncBody=False

                #��������ͷ���壬�����ж���
                while bIsInFuncDefine:
                    #�� ) Ϊ��ʶ�ҵ���������Ĳ����б����
                    if IsFuncMemberDefineEnd(inputLines[curIndex]):
                        DebugLog("line_%d find FuncMember define End: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))

                        if inputLines[curIndex].find(';') != -1:
                            #��Ա���������� ; ��β��û������ʵ�֣�ֱ�ӽ�����һ��
                            DebugLog("\tFunc End WITHOUT body")
                            curIndex+=1
                        else:
                            #��Ա������������ʵ�֣���Ҫ��������Body
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

                #���������嶨�壬���ô����ųɶԳ������ü���������˲�֧��һ���ڳ���{{����
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
                        #�������ֽ���
                        DebugLog("line_%d is FuncBody Ended!: %s" % (curIndex+1, inputLines[curIndex].rstrip() ))
                        curIndex+=1
                        bIsInFuncBody=False
                        break

                    curIndex+=1
                    #���㷨�Ͽ�������������������ඨ�������ͬһ�У�������������ඨ��
            elif -1 != inputLines[curIndex].find("};"):
                #�ඨ�����
                DebugLog( "line_%d Encounter class End: %s"  % (curIndex+1, inputLines[curIndex].rstrip() ))

                c.SetRawEndLine(curIndex)
                allClass.append(c)
                curIndex+=1
                bIsInClassBody=False
                break
            else:
            #�����к��ԣ�����ע����
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
        #todo ��Ҫ��������
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

#������������ͷ�ļ�������д�����ļ�
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

    #����CPP�ļ�
    print ("Begin to create proto function cpp file\n\n")
    Lines = GenProtoFunc(allClass)
    if len(Lines) > 0:
        outf.seek(0,0)
        outf.writelines(Lines)

    #��������ͷ�ļ�
    allClass.reverse()
    for c in allClass:
        InsertIndex = c.RawEndLine
        #����XML����ؽӿ�
        Lines = GenFuncMemberXmlAccess(c)
        if len(Lines) > 0:
            s = "public:\r\n"
            inputLines.insert(InsertIndex, s); InsertIndex+=1
            for l in Lines:
                inputLines.insert(InsertIndex, l); InsertIndex+=1
        #����get/set�ӿ�
        Lines = GenFuncMemberGetAndSet(c)
        if len(Lines) > 0:
            s = "public:\r\n"
            inputLines.insert(InsertIndex, s); InsertIndex+=1
            for l in Lines:
                inputLines.insert(InsertIndex, l); InsertIndex+=1

    inf.seek(0,0)
    inf.writelines(inputLines)

    #�ر��ļ����˳�
    inf.close()
    outf.close()







