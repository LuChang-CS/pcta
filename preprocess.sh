#!/bin/bash

mkdir -p target
mkdir -p pcta/libs

echo "downloading libs"
curl -o pcta/libs/rapidjson-master.zip https://codeload.github.com/Tencent/rapidjson/zip/master
echo "download finished"
echo "unzipping libs"
unzip pcta/libs/rapidjson-master.zip -d pcta/libs/ > /dev/null 2>&1
echo "unzip finished"
mv pcta/libs/rapidjson-master/include/rapidjson pcta/libs/json
rm -r pcta/libs/rapidjson-master
rm pcta/libs/rapidjson-master.zip
