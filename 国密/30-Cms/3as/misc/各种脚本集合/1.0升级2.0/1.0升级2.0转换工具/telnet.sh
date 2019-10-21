#!/bin/sh
#usage: example: telnet.sh pui | telnet.sh 2500

INPUT=$1
TELPORT=2500
# 0 = not find; 1 = find; 2 = the input is numeric
B_FINDPORT=0

if [ -z $INPUT ];then
	echo "Usage: telnet.sh PName or telnet TelPort"
	exit 0
fi

if [ ! -z `echo $INPUT | sed -n '/^[0-9][0-9]*$/p'` ];then
#if input is num, directly use
	TELPORT=$INPUT
	B_FINDPORT=2

else
#otherwise use netstat to find telnet port

	TMPRST=`netstat -anp | grep $INPUT | grep -P '\bLISTEN\b' |grep -P '0.0.0.0:25\d\d\b' |sort`
        [ -z "$TMPRST" ] && TMPRST=`netstat -anp | grep $INPUT | grep -P '\bLISTEN\b'`
	if [ ! -z "$TMPRST" ];then
		TELPORT=`echo $TMPRST |awk -F"0.0.0.0:" '{ print $2 }'`
		B_FINDPORT=1
	fi

fi

if [ $B_FINDPORT -ne 0 ];then
    telnet localhost $TELPORT
else
    echo "not find telnet port"
fi

