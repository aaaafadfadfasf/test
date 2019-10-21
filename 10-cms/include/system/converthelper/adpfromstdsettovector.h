#ifndef ADP_FROM_STD_SET_TO_VECTOR_H
#define ADP_FROM_STD_SET_TO_VECTOR_H

#include <set>

namespace ConvertHelper{
namespace ToStruct{

template <class Type>
class CAdpFromStdSetToVector{
public:
    typedef std::set<Type> StdSet;
    typedef Type value_type;
public:
    CAdpFromStdSetToVector(StdSet& theStdSet) : m_stdSet(theStdSet){}
    void push_back(const Type& elem){
        m_stdSet.insert(elem);
    }
private:
    StdSet& m_stdSet;
};

template <class Type>
CAdpFromStdSetToVector<Type> CreateAdpFromStdSetToVector(std::set<Type>& stdSet){
    return CAdpFromStdSetToVector<Type>(stdSet);
}

}//end of ToStruct namespace
}//end of converthelper namespace

#endif
//endoffile

