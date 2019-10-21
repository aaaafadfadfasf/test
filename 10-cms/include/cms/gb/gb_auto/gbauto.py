# -*- coding: cp936 -*-
#!python
'''
Author: liangli
Create: 2013.10.21
Usage: like cmsauto.exe
'''
import sys
from gb_proto_auto import GBProtoAuto
from gb_proto_auto import SetDebugLog

def GetOutputFileName(inputFileName):
    outFileName = inputFileName[:-2] + ".cpp"
    return outFileName

g_inputFiles = ["gb_proto.h", "gb_struct.h"]
g_inputPath = "..\\"
g_outputPath = "..\..\\..\\..\\..\\30-Cms\\gbprotolib\\src\\"

SetDebugLog(True)

for inputFile in g_inputFiles:
    outputFile = g_outputPath + GetOutputFileName(inputFile)
    inputFile = g_inputPath + inputFile
    GBProtoAuto(inputFile, outputFile)

