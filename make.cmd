@echo off
gcc -o demo.exe -std=c99 GenList.c demo.c || pause
exit/b