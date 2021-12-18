#!/bin/bash
name=${1%.*}
nasm -f elf -gdwarf $1
ld -m elf_i386 "$name.o" -o "$name"
rm "$name.o"