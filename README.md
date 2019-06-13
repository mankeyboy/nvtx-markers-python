# nvtx-markers-python
Adding a quick python module for pretty NVTX markers to be used inside Python CUDA code.

----------------------------------------------

### Compile instructions:  
A compile_ppc64le.sh and compile_x86.sh file has been provided with flags to setup the markers for your usecase.  

Shown below is the `compile_ppc64le.sh` file. The x86 file works in a similar manner.  

```
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

$NVCC profilerModule.c -I$CUDA_PATH/include -L$CUDA_PATH/lib64 -lnvToolsExt  -I$PYTHON_INC_PATH -L/usr/lib/powerpc64le-linux-gnu -lpython3.6m --shared -o $OUT_FOLDER/profilerModule.so
```

### Working:  
The functions in profilerModule.c provide a simple python wrapper for the CUDA C/C++ API for the nvtx functions.

### Usage: 

```
#!/usr/bin/python
from profilerModule import *

print("Calling profilerStart: ") 
profilerStart("Start")

print("\nCalling a range push to mark this part: ")
profilerRangePush("MARKER")

print("\nThis portion shall be marked")

profilerRangePop("MARKER")

print("\nCalling profilerStop: ")
profilerStop("Stop")
```

