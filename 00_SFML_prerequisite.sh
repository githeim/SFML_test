#!/bin/bash
sudo apt-get install -y ninja-build
sudo apt-get install libsfml-dev -y

# google test installation
sudo apt-get install libgtest-dev -y
pushd /usr/src/gtest
ls
sudo cmake CMakeLists.txt
sudo make -j12
sudo cp lib/*.a /usr/lib
sudo mkdir -p /usr/local/lib/gtest
sudo ln -s /usr/lib/libgtest.a /usr/local/lib/gtest/libgtest.a
sudo ln -s /usr/lib/libgtest_main.a /usr/local/lib/gtest/libgtest_main.a
popd
