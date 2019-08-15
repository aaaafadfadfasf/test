#ifndef TEST_RESULT_WRITE_FILE_XML_H
#define TEST_RESULT_WRITE_FILE_XML_H

#include <string>
#include "cms/cppunitlite/testresult.h"
namespace CppUnitLite
{
    class TiXmlElement;
}

class TestResultXml : public TestResult
{
public:
    TestResultXml(std::string szFilePath);
    virtual void endTests();
protected:
    void GenerateFailures(CppUnitLite::TiXmlElement* pParent);


    void GenerateSuccess(CppUnitLite::TiXmlElement* pParent);

    void GenerateStatistics(CppUnitLite::TiXmlElement* pParent);
private:
    std::string m_szFilePath;
};


#endif

//end of file


