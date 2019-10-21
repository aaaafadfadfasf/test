
AUTOMAKE_OPTIONS = subdir-objects


#CPPFLAGS    C/C++ preprocessor flags, e.g. -I and -D
#CXXFLAGS    C++ compiler flags
#LDADD       link libraries flags, e.g. -L and -l
#LDFLAGS     linker flags, except for -L and -l

bin_PROGRAMS = cui

cui_CPPFLAGS = -DDEBUG -D_LINUX_ -DUSE_EPOLL
cui_CPPFLAGS += -I../../include \
                -I../../../cui_fcgi/include \
                -I../../../common \
                -I../../../../../10-Common/include \
                -I../../../../../10-Common/include/system/soap 

CXXFLAGS = -O0 
cui_CXXFLAGS = -Wall -Wformat -g
cui_LDADD = -L../../../../../10-Common/lib/debug/linux \
            -lpthread -lrt -luuid -lospsip -lospext -lkdsip \
            -ltinyxml -lcmcfg -lcmsproto -lkdvencrypt -losp -ldl -lz -lnipdebug -ltcmalloc_debug -lfcgi
cui_LDFLAGS = -O0

cui_SOURCES = ../../src/cuistruct.cpp \
            ../../src/transutility.cpp \
            ../../src/transthread.cpp \
            ../../../cui_fcgi/src/serviceappimp.cpp \
            ../src/main.cpp