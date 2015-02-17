#!/bin/bash
./compile.sh
echo 'GCC'
./gcc
echo 'ICC - With Vectorization'
./icc_vec
echo 'ICC - Without Vectorization'
./icc_unvec
echo 'ICC - SSE2'
./icc_sse2