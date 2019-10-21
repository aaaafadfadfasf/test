'''
#headfileparse.py
Created on 2013-10-20
#提供类的每一行的基本解析
@author: liangli
'''

import re


#判断行是否为类定义，包括class A | class A : public B | struct A 等
#如果是一个类，则返回一个元祖(ClassName, ParentClassName)
classDefinePattern = re.compile(r'^\s*\bstruct\b\s+.*|^\s*\bclass\b\s+.*')
def IsClassDefine(line):
    m = classDefinePattern.match(line)
    return m

commentLinePattern = re.compile(r'^\s*//|^\s*/\*')
def IsCommentLine(line):
    m = commentLinePattern.match(line)
    return m

multiCommentLineBeginPattern = re.compile(r'^\s*/\*')
def IsMultiCommentLineBegin(line):
    m = multiCommentLineBeginPattern.match(line)
    return m

multiCommentLineEndPattern = re.compile(r'.*\*/\s*$')
def IsMultiCommentLineEnd(line):
    m = multiCommentLineEndPattern.match(line)
    return m

dataMemberPattern = re.compile(r'^\s*(NeedAutoGen\s+)?(static\s+)?((\w+\s*<\s*\w+\s*>)|(\w+))\s+\w+\s*(?!\();.*')
def IsDataMember(line):
    m = dataMemberPattern.match(line)
    return m

funcMemberPattern = re.compile(r'^.+[\(\)].*')
def IsFuncMemberDefineBegin(line):
    m = funcMemberPattern.match(line)
    return m

funcMemberDefineEndPattern = re.compile(r'.*\)\s*[;]?.*$')
def IsFuncMemberDefineEnd(line):
    m = funcMemberDefineEndPattern.match(line)
    return m

funcBodyBeginPattern = re.compile(r'.*{.*')
def IsFuncBodyBegin(line):
    m = funcBodyBeginPattern.match(line)
    return m

funcBodyEndPattern = re.compile(r'.*}.*')
def IsFuncBodyEnd(line):
    m = funcBodyEndPattern.match(line)
    return m

blankLinePattern = re.compile(r'^\s*$')
def IsBlankLine(line):
    m = blankLinePattern.match(line)
    return m

typedefPattern = re.compile(r'^\s*typedef\s+((\w+\s*<\s*\w+\s*>)|(\w+))\s+\w+\s*;.*')
def IsTypedef(line):
    m = typedefPattern.match(line)
    return m


















