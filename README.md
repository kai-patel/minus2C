# minus2C
A simple compiler for the --C language, written in C

# Usage

A Makefile is provided in order to build the code. This should work on almost all Unix machines.

```bash
make clean
make
./mycc < source.c
```

# Modes

## Default
By default the compiler will interpret input files and print the AST of the file.

## Interpreter Mode
To suppress printing of the AST, use the __-i__ flag to interpret source files. 

## TAC Generation Mode
To print out the TAC representation of the interpreted code, use the __-t__ flag. This will not print the AST of the source code.

## Compilation Mode
In order to compile source code to MIPS, use the __-c__ flag. The compiled output will be available in the __a.s__ file.
