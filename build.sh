#!/bin/bash

clang -std=c23 -Wall -Wextra -Werror -Wno-unused-function main.c -o a -lm -lc
