#include "cms/cppunitlite/cppunitlite.h"
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cuidata.h"

TEST_GROUP(CUserMgr)
{
    CUserMgr m_cUserMgr;
};

TEST(CUserMgr, testInsertUser)
{
    string strUserName = "admin@china";
    CUserAccount cUserAccount(&m_cUserMgr);
    cUserAccount.m_strUserName = strUserName;
    m_cUserMgr.m_mapUserName2UserAccount.Insert(cUserAccount.m_strUserName, cUserAccount);
    CHECK( 1 == m_cUserMgr.m_mapUserName2UserAccount.GetSize() );
    CUserAccount *pcUserAccount = m_cUserMgr.FindUserAccountByUserName(strUserName);
    CHECK(pcUserAccount != NULL);
}

