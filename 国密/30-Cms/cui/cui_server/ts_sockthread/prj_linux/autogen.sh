#!/bin/sh
# Convenience script for regenerating Makefile and invoking it.

# Usage: ./autogen.sh [clean]

if [ "$1" = "clean" ]; then
    [ -f Makefile ] && make distclean
    [ -e aclocal.m4 ] && rm -rf aclocal.m4
    [ -e autom4te.cache ] && rm -rf autom4te.cache
    [ -e config.h.in ] && rm -rf config.h.in
    [ -e configure ] && rm -rf configure
    [ -e depcomp ] && rm -rf depcomp
    [ -e install-sh ] && rm -rf install-sh
    [ -e Makefile.in ] && rm -rf Makefile.in
    [ -e missing ] && rm -rf missing
    
    [ -e config.h.in~ ] && rm -rf config.h.in~
    [ -e config.guess ] && rm -rf config.guess
    [ -e config.sub ] && rm -rf config.sub
    [ -e ltmain.sh ] && rm -rf ltmain.sh
    exit
fi

[ ! -f Makefile ] && autoreconf --install && ./configure
make
