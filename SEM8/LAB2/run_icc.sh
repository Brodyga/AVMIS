#!/bin/bash
./compile.sh
echo 'ICC - With Block Multiplication'
./icc_block
echo 'ICC - Without Block Multiplication'
./icc_unblock