#!/usr/bin/env bash

LIB_DIR=$PWD

build_tiff() {
  if [ ! -d ./tiff-4.7.0 ]; then
    cd $LIB_DIR
    # download
    wget https://download.osgeo.org/libtiff/tiff-4.7.0.tar.gz
    tar xzvf tiff-4.7.0.tar.gz
    # build
    cd ./tiff-4.7.0/
    mkdir lib
    ./configure --prefix=$PWD/lib
    make -j
  fi
}

build_tiff
