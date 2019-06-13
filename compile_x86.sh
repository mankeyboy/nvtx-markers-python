#!/bin/bash

# Please export CUDA_PATH to give the path to your CUDA installation, typically /usr/local/cuda
NVCC=$CUDA_PATH/bin/nvcc

# Things to note:
# This assumes a conda setup but if you don't have conda, just replace this value with your python include folder
# Default installation path would generally be /usr/include/python3.x without conda or virtualenv
# Make sure that the python library folder is in your $LD_LIBRARY_PATH
PYTHON_INC_PATH=$CONDA_PREFIX/include/python3.6m 

# Set this up to the folder you'll be running your python code from.
OUT_FOLDER=''  

gcc -c -fPIC  profilerModule.c -I$CUDA_PATH/include -I$PYTHON_INC_PATH -o profilerModule.o 
$NVCC profilerModule.o -I$CUDA_PATH/include -L$CUDA_PATH/lib64 -lnvToolsExt -I$PYTHON_INC_PATH -L/usr/lib/x86_64-linux-gnu -lpython3.6m --shared -o $OUT_FOLDER/profilerModule.so 