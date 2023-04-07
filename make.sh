#!/usr/bin/env bash

CFLAGS="-std=c++20 -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror -O2"

set -xe

g++ $CFLAGS src/*.cpp -o A2.out

