#!/usr/bin/env bash
# IMPERVIOUS Core Engine First-Principles Build Script

set -e

printf "[BUILD] Compiling IMPERVIOUS source architecture...\n"

# Execute native GCC compilation pass
g++ -O3 -std=c++17 src/main.cpp -o impervious_engine

printf "[SUCCESS] Native engine compiled successfully.\n"
printf "[RUN] Execute via: ./impervious_engine <input> <output.imp>\n"
