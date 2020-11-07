#!/bin/bash

valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind_log.txt ./mycc < test.c
