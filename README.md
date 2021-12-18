## Description
The objective of this project was to write a program that could interpret or
transpile to NASM programs written in the Brainfuck language.

This program works by parsing each character into intermediate node and creates a list from those. That makes it easy to interpret or transpile the generated node list instead of parsing the inital program multiple times. Also this allows the program to do some very
simple optimizations like joining together consecutive value increment or decrement operations and also consecutive move operations.

## Usage
```
brainfuck [OPTIONS] INPUT
INPUT is a Brainfuck program string or a filename if -f is specified

Arguments that can be used
    -i             Runs program in interpreter mode instead of transpiling 
                   input Brainfuck program to NASM.
    -f             Treats INPUT argument as filename otherwise it is used as 
                   Brainfuck program string.
    -o FILENAME    Ouput filename where the transpiled NASM code is written. 
                   By default STDOUT is used.

Examples:
    // Transpile Brainfuck program given as argument to STDOUT
    brainfuck ">++++++++[<+++++++>-]<."

    // Transpile Brainfuck program given as argument to file out.asm
    brainfuck -o out.asm ">++++++++[<+++++++>-]<."
    
    // Transpile Brainfuck program from file given as argument to file out.asm
    brainfuck -f -o out.asm ./tests/hello_world.b

    // Interpret Brainfuck program given as argument
    brainfuck -i ">++++++++[<+++++++>-]<."
    
    // Interpret Brainfuck program from file given as argument
    brainfuck -i -f ./tests/hello_world.b
```
There is also included a shell script that can be used to compile the generated NASM code. The script takes the NASM source file as it's argument and creates a binary with it's name
```
./build_nasm.sh hello_world.asm
./hello_world
```
Building the asm program requires functions.asm file that is included in ./asm directory. If you want to compile the program without using the provided shell script then this file has to be included in the nasm search directory. Additional directories can be specified with the -i option.

## Building
```
make build
```

## Cleaning
```
make clean
```
