#ifndef _I_LOG_PRINTER_H_
#define _I_LOG_PRINTER_H_

#include <string>

namespace Log4KDM
{
    namespace Interface{
        //
        /**
         *  »’÷æ¥Ú”°
         */
        class ILogPrinter
        {
        public:
            virtual ~ILogPrinter(){}
            enum EmLogLevel
            {
                emError,
                    emWarning,
                    emInfo
            };
            virtual void Log(EmLogLevel emLogLev,const std::string& szLog) = 0;
        };
    }
}


#endif

//end of file

