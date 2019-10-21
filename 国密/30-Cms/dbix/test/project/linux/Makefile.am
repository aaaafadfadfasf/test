
AUTOMAKE_OPTIONS = subdir-objects


#CPPFLAGS    C/C++ preprocessor flags, e.g. -I and -D
#CXXFLAGS    C++ compiler flags
#LDADD       link libraries flags, e.g. -L and -l
#LDFLAGS     linker flags, except for -L and -l

bin_PROGRAMS = test

test_CPPFLAGS = -D_LINUX_
test_CPPFLAGS += -I../../../../../10-Common/include/ \
                -I../../code
                
test_CXXFLAGS = -Wall -Wformat
test_LDADD = -ldbix -lospext -lkdvsys -losp \
           -ldbi -lsybct -lsybcs -lsybtcl -lsybblk -lsybdb -lsybunic -lmysqlclient_r -lclntsh -lnnz10 \
           -ldl -lz -lpthread -lrt -lsysdbg

test_SOURCES = ../../code/dbcmd_demo.cpp \
            ../../code/dbentity_demo.cpp \
            ../../code/dbix_init.cpp \
            ../../code/dbobj_define.cpp \
            ../../code/dbresult_handler.cpp \
            ../../code/main.cpp
