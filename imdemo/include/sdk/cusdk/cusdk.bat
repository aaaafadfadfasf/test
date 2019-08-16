echo off
soapcpp2 -S  -T -i cusdk.h -I../ -I../../system/soap/import/

pause

move /Y *.xml ..\..\..\..\30-Cms\cui\cui_fcgi\soap\example\

move /Y cusdk.nsmap ..\..\..\..\30-Cms\cui\cui_fcgi\soap\

move /Y cusdk.xsd ..\..\..\..\30-Cms\cui\cui_fcgi\soap\

move /Y soapC.cpp ..\..\..\..\30-Cms\cui\cui_fcgi\soap\

move /Y soapcusdkService.cpp ..\..\..\..\30-Cms\cui\cui_fcgi\soap\

move /Y soapcusdkService.h ..\..\..\..\30-Cms\cui\cui_fcgi\soap\

move /Y soapH.h ..\..\..\..\30-Cms\cui\cui_fcgi\soap\

move /Y soapStub.h ..\..\..\..\30-Cms\cui\cui_fcgi\soap\

move /Y soapTester.cpp ..\..\..\..\30-Cms\cui\cui_fcgi\soap\
pause