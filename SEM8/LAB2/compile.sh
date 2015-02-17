#!/bin/bash
icc -std=c99 -O2 -opt-report=2 -qopt-report-phase=vec -D NOALIAS -D BLOCK main.c -o icc_block
icc -std=c99 -O2 -opt-report=2 -qopt-report-phase=vec -D NOALIAS main.c -o icc_unblock