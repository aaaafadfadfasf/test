

#ifndef TESTRESULT_H
#define TESTRESULT_H

// TestResult is a simple little class which accepts notifications from
// tests.  In its current form, it takes these notifications and prints
// them on the standard output.
//
// If you need different behavior, you can alter it in place or subclass 
// TestResult to provide it.  The TestResult hierarchy can be as simple
// or complex as you require to support your application.

#include <vector>
#include "failure.h"

class TestResult
{
public:
    TestResult () : testCount(0) {}
    virtual ~TestResult(){}
    void testWasRun ();
    void startTests ();
    void addFailure (Failure failure);
    virtual void endTests ();
    bool wasSucessful()const;
    void addSuccess(const Success& success );

protected:
    int testCount;
    std::vector<Failure> failures;
    std::vector<Success> successes;
};

#endif

//END OF FILE
