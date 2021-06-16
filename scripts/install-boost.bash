#!/bin/bash

# Install boost version 1.75.0

set -e

function clean_before_exit {
	rm -rf $boost
	rm -rf $current_directory/.tmp
}

trap clean_before_exit EXIT

current_directory=$(pwd)
boost=$(pwd)/libs/boost
cache=$(pwd)/.cache

mkdir -p $cache

boost_archive=$cache/boost.tar.gz
boost_dl_url=https://boostorg.jfrog.io/artifactory/main/release/1.75.0/source/boost_1_75_0.tar.gz

if [ ! -f "$boost_archive" ]
then
	curl -o $boost_archive -L $boost_dl_url
fi

mkdir .tmp
cd .tmp

tar -xf $boost_archive

cd boost_1_75_0
./bootstrap.sh --prefix=$boost --with-toolset=clang 
./b2 --toolset=clang --with-program_options --link=static install

cd ../..
rm -rf .tmp

trap - EXIT
