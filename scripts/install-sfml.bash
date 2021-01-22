#!/bin/bash

# Install sfml version 2.5.1

set -e

function clean_before_exit {
	rm -rf $sfml
	rm -rf $current_directory/.tmp
}

trap clean_before_exit EXIT

current_directory=$(pwd)
sfml=$(pwd)/libs/sfml
cache=$(pwd)/.cache

mkdir -p $cache

sfml_archive=$cache/sfml.zip
sfml_dl_url=https://www.sfml-dev.org/files/SFML-2.5.1-sources.zip

if [ ! -f "$sfml_archive" ]
then
	curl -o $sfml_archive -L $sfml_dl_url
fi

mkdir .tmp
cd .tmp

unzip -qq $sfml_archive

cd SFML-2.5.1
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=$sfml -DSFML_BUILD_AUDIO=n -DSFML_BUILD_NETWORK=n  ..
make install

cd ../../..
rm -rf .tmp

trap - EXIT
