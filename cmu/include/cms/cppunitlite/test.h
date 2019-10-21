
#ifndef TEST_H
#define TEST_H


// Test is a base class for all tests.  It provides a command interface for
// running tests (run) as well as a data member for recording the name of 
// the test.
//
// Tests are constructed using the TEST macro.  TEST creates a subclass of
// Test and static instance of that subclass.  If you look at the constructor
// for the Test class, you'll notice that it registers the created object 
// with a global TestRegistry.  These features combine to make test creation
// particularly easy.

#ifndef __GNUC__
#pragma warning( disable : 4786 )
#endif

#include <vector>
#include <string>

class TestResult;


class Test
{
protected:
    virtual ~Test();
public:
    Test();
	virtual void run (TestResult& result);
    virtual void setUp(){}
    virtual void tearDown(){}
protected:
	virtual void runTest (TestResult& result);
    virtual void doTest (TestResult& result) = 0;
    virtual const std::string getName() = 0;
    bool bIsFailed;
};

class TestCreator
{
protected:
    virtual ~TestCreator();
public:
    TestCreator (bool bSlow);    
public:
    virtual void run (TestResult& result);
    virtual Test& GetTestInstance() = 0;
};


#define TEST_GROUP_BASE(testGroup, baseclass) \
    int externTestGroup##testGroup = 0; \
struct CppUTestGroup##testGroup : public baseclass

#define TEST_BASE(testBaseClass) \
struct testBaseClass : public Test

#define TEST_GROUP(testGroup) \
TEST_GROUP_BASE(testGroup, Test)

#define TEST_GROUP_CTOR(testGroup)\
    CppUTestGroup##testGroup()

#define TEST_SETUP() \
virtual void setUp()

#define TEST_TEARDOWN() \
  virtual void tearDown()

#define TEST_CASE_IMPL(testGroup,testName)\
    class testGroup##_##testName##_Test : public CppUTestGroup##testGroup \
{ \
public: \
    testGroup##_##testName##_Test () : CppUTestGroup##testGroup (),name(#testName) {} \
protected:\
    void doTest (TestResult& result_); \
    const std::string getName(){return name;}\
protected:\
    std::string name;\
}; \

#define TEST_CREATOR_IMPL(testGroup, testName,bSlowTest)\
TEST_CASE_IMPL(testGroup,testName)\
class testGroup##_##testName##_TestCreator : public TestCreator\
{\
public:\
    testGroup##_##testName##_TestCreator():TestCreator(bSlowTest){}\
    virtual Test& GetTestInstance()\
    {\
        static testGroup##_##testName##_Test s_instance;\
        return s_instance;\
    }\
}testGroup##_##testName##_TestCreator##Instance;\
void testGroup##_##testName##_Test::doTest (TestResult& result_) \


#define TEST(testGroup, testName) TEST_CREATOR_IMPL(testGroup, testName, false)
#define TEST_SLOW(testGroup, testName) TEST_CREATOR_IMPL(testGroup, testName, true)

class TestCreatorForTestOnlyOneTest : public TestCreator
{
public: 
    TestCreatorForTestOnlyOneTest();
};
#define TEST_ONLY(testGroup,testName)\
TEST_CASE_IMPL(testGroup,testName)\
    class testGroup##_##testName##_TestCreator : public TestCreatorForTestOnlyOneTest\
{\
public:\
    testGroup##_##testName##_TestCreator(){}\
    virtual Test& GetTestInstance()\
{\
    static testGroup##_##testName##_Test s_instance;\
    return s_instance;\
}\
}testGroup##_##testName##_TestCreator##Instance;\
void testGroup##_##testName##_Test::doTest (TestResult& result_) \

// Here is a collection of testing macros that can be used in the 
// bodies of tests.  CHECK tests a boolean expression and records
// a failure if the expression evaluates to false.  CHECK_LONGS_EQUAL
// and CHECK_DOUBLES_EQUAL compare longs and doubles respectively.
//
// To make this an industrial strength test harness, you should
// add equals macros for various low level types as you develop them.
// If, for instance, you have a daterange class, the ability to compare
// them directly and print out their values in the test output is 
// invaluable.


#define CHECK(condition) \
do{\
    if (!(condition)) \
    {\
        result_.addFailure (Failure (#condition, name, __FILE__, __LINE__));\
        bIsFailed = true;\
    }\
}while(0)


#define CHECK_DOUBLES_EQUAL(expected,actual)\
do{\
    double _expected = (expected);\
    double _actual = (actual);\
    if (fabs ((expected)-(actual)) > 0.001) {\
        char message [80];\
        sprintf (message, "expected %lf but was: %lf", (expected), (actual));\
        result_.addFailure (Failure (message, name, __FILE__, __LINE__));\
        bIsFailed = true;\
    }\
}while(0)

#define STRCMP_EQUAL(expected,actual)\
do{\
    int nDiff = strcmp((expected),(actual));\
    if(nDiff != 0)\
    {\
        std::string message("expected ");\
        message += (expected);\
        message += " but was: ";\
        message += (actual);\
        result_.addFailure (Failure (message, name, __FILE__, __LINE__));\
        bIsFailed = true;\
    }\
}while(0)

#define CK_EQ(expected,actual) CHECK((expected) == (actual))

#define CK_NOT_EQ(expected,actual) CHECK((expected) != (actual))

#define EXPECT_EQ(expected,actual) CHECK((expected) == (actual))

#define EXPECT_NOT_EQ(expected,actual) CHECK((expected) != (actual))

#define CHECK_DOUBLE_EQ CHECK_DOUBLES_EQUAL

#define CK_STR_EQ STRCMP_EQUAL

#define CK_STR_EQ_UTF8(expected,actual) CK_STR_EQ( _UTF8( expected ).c_str(), _UTF8( actual ).c_str() )

#define CK_STR_NOT_EQ(expected,actual)\
do{\
    int nDiff = strcmp((expected),(actual));\
    if(nDiff == 0)\
    {\
        std::string message("expected ");\
        message += (expected);\
        message += " but was: ";\
        message += (actual);\
        result_.addFailure (Failure (message, name, __FILE__, __LINE__));\
        bIsFailed = true;\
    }\
}while(0)


#endif

//END OF FILE
