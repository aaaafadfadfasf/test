#include "cppunitlite/cppunitlite.h"
#include "cms/cms_proto.h"



TEST_GROUP( CCmsVer )
{

};


TEST( CCmsVer, FromString )
{
#define CMS_VERSION1     "ViewShot 2.1.1.0.0 2012.1.16"
    CCmsVer cVer(CMS_VERSION1);

	CK_STR_EQ( "ViewShot", cVer.GetName().c_str() );
    CK_STR_EQ( "2012.1.16", cVer.GetDate().c_str());
    CK_EQ( 2, cVer.GetVersion());
    CK_EQ( 1, cVer.GetRelease());
    CK_EQ( 1, cVer.GetBuild());
    CK_EQ( 0, cVer.GetSp());
    CK_EQ( 0, cVer.GetFix());

#define CMS_VERSION2     "ViewShot 2.1.1.0.0   2012.1.16"
    cVer.FromString(CMS_VERSION2);

    CK_STR_EQ( "ViewShot", cVer.GetName().c_str() );
    CK_STR_EQ( "2012.1.16", cVer.GetDate().c_str());
    CK_EQ( 2, cVer.GetVersion());
    CK_EQ( 1, cVer.GetRelease());
    CK_EQ( 1, cVer.GetBuild());
    CK_EQ( 0, cVer.GetSp());
    CK_EQ( 0, cVer.GetFix());
}

TEST( CCmsVer, ToString )
{
#define CMS_VERSION3     "ViewShot 1.2.3.4.5 2012.08.09"
    CCmsVer cVer;
    cVer.SetName(VER_NAME);
    cVer.SetVersion(1);
    cVer.SetRelease(2);
    cVer.SetBuild(3);
    cVer.SetSp(4);
    cVer.SetFix(5);
    cVer.SetDate("2012.08.09");

    CK_STR_EQ( "ViewShot", cVer.GetName().c_str() );
    CK_STR_EQ( "2012.08.09", cVer.GetDate().c_str());
    CK_EQ( 1, cVer.GetVersion());
    CK_EQ( 2, cVer.GetRelease());
    CK_EQ( 3, cVer.GetBuild());
    CK_EQ( 4, cVer.GetSp());
    CK_EQ( 5, cVer.GetFix());

    CK_STR_EQ( CMS_VERSION3, cVer.ToString().c_str() );
}

TEST( CCmsVer, Compare )
{
#define CMS_VERSION4     "ViewShot 1.2.3.4.5 2012.08.09"
    CCmsVer cVer;
    cVer.SetName(VER_NAME);
    cVer.SetVersion(1);
    cVer.SetRelease(2);
    cVer.SetBuild(3);
    cVer.SetSp(4);
    cVer.SetFix(5);
    cVer.SetDate("2012.08.09");

    CCmsVer cVer1(CMS_VERSION4);
    CK_EQ( cVer, cVer1);

#define CMS_VERSION5     "ViewShot 2.2.3.4.5 2012.08.09"
    CCmsVer cVer2(CMS_VERSION5);
    CK_EQ( true, (cVer1<cVer2));

    cVer2.SetVersion(0);
    CK_EQ( true, (cVer1>cVer2));
}



