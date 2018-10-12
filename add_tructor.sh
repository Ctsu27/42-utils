#!/bin/sh

if [ $# -eq 0 ]
then
	echo "Usage: ./add_tructor.sh [target_file]"
	exit -1;
fi

echo "" >> $1
echo "__attribute__((constructor)) void ke_begin (void)" >> $1;
echo "{" >> $1
echo "	write(1, \"enter: \", 7);" >> $1
echo "	write(1, __FUNCTION__, strlen(__FUNCTION__));" >> $1
echo "	write(1, \"\\\n\", 1);" >> $1
echo "}" >> $1

echo "" >> $1
echo "__attribute__((destructor)) void ke_end (void)" >> $1;
echo "{" >> $1
echo "	write(1, \"enter: \", 7);" >> $1
echo "	write(1, __FUNCTION__, strlen(__FUNCTION__));" >> $1
echo "	write(1, \"\\\n\", 1);" >> $1
echo "	while (1) ;" >> $1
echo "}" >> $1
