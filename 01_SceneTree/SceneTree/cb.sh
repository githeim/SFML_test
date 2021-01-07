#!/bin/bash
echo Configuration and Build
rm build -rf
FILE="compile_commands.json"
if [ -h "$FILE" ]; then
    echo "$FILE exists. now delete it"
    rm $FILE
fi
mkdir build
cd build ; cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=YES -G Ninja \
  && ln -s build/compile_commands.json .. \
  && ninja -v -j14 && cp ../resource/fonts/arial.ttf Debug
#cd build ; cmake .. && make -j


