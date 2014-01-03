#!/bin/bash
echo -e "***************RAINYDAY COMPILATION SCRIPT***************************************
**************This compile release version of rainyday***************************
*********************************************************************************
"
cd ui
uic mainwindow.ui>ui_mainwindow.h 
cd ..
mkdir -p build
cd build
cmake ..
make


