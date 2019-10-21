#include "cms/cppunitlite/cppunitlite.h"
#include "cms/cppunitlite/testresultxml.h"

int main(int argc,char** argv)
{
    char achModName[256] = {0};
    strncpy(achModName, argv[0], sizeof(achModName)-1);

    std::string szFilePath(achModName);
    szFilePath += ".xml";
    TestResultXml tr(szFilePath);

    TestRegistry::runAllTests(tr);
    return tr.wasSucessful() ? 0 : 1;
}

