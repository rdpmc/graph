:m0
nasm -O9 -fobj -t 1fr.asm
alink 1fr.obj  -oPE -o 1fr.exe
pause
goto m0
