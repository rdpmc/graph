:m1
c:\mingw\bin\gcc.exe -ggdb -c -o main.o main.c -I"C:\MinGW\freeglut\include"
c:\mingw\bin\gcc.exe -ggdb -c -o callbacks.o callbacks.c -I"C:\MinGW\freeglut\include"
c:\mingw\bin\gcc.exe -ggdb -c -o common.o common.c  -I"C:\MinGW\freeglut\include"
c:\mingw\bin\gcc.exe -o test.exe main.o  common.o callbacks.o  -L"C:\MinGW\freeglut\lib" -ggdb -gdwarf3 -lfreeglut -lopengl32 -lglu32 -Wl,--subsystem,windows

pause
goto m1