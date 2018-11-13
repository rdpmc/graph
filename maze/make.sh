#apt get install libglut3-dev
gcc -Wall -ggdb -o 1 main.c callbacks.c common.c   -lm -lGL -lglut  -lGLU 
