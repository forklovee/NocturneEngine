#!/bin/bash

git submodule init
git submodule update

cmake -B build
cmake --build build


