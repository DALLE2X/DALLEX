#!/bin/sh

rm -rf a.out

projectDir=$(find / -type d -name "DreamStudio")
echo $projectDir
cd $projectDir/build && rm -rf ./*&&cmake ..&&make clean &&make
cd $projectDir/DreamAi/src
export LD_LIBRARY_PATH=:$projectDir/lib/
gcc Ai.c -I ../include/ -I ../../Rainbow/include/ -I ../../cJSON/include/ -I ../../base64/include/  -I ../../sqlite3/include/ -I ../../logger/include/ -lcurl -L ../../lib -ldream -lpthread -ldl
./a.out
