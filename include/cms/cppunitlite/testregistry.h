
#ifndef TESTREGISTRY_H
#define TESTREGISTRY_H

// TestRegistry is a primitive singleton which collects all of the
// tests in a system and allows them to be executed.  To see how
// tests are added to the TestRegistry look at the Test.h file


#include <vector>

class TestCreator;
class TestResult;

class TestRegistry
{
public:
	static void addFastTest (TestCreator *pTest);
    static void addSlowTest(TestCreator* pTest);
    static void addOnlyTest(TestCreator* pTest);

	static void runAllTests (TestResult& result);
    static void runFastTests (TestResult& result);
    static void runSlowTests (TestResult& result);    
private:
    TestRegistry();
	static TestRegistry&	instance ();
	void					DoAddFastTest (TestCreator *pTest);
    void                    DoAddSlowTest(TestCreator *pTest);
    void                    DoAddOnlyTest(TestCreator* pTest);
	void					DoRunFastTest (TestResult& result);
    void					DoRunSlowTest (TestResult& result);
    void                    DoRunTestOnly(TestResult& result);
    bool                    IsUsingTestOnly()const;

	std::vector<TestCreator *>		m_fastTests;
    std::vector<TestCreator *>      m_slowTests;
    TestCreator* m_pOnlyTest;

};




#endif

//END OF FILE
