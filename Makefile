CC = gcc
linux:
	${CC} ./*.c -o classic-games -lGLU -lGL -lX11 -lm

windows: 
	${CC} ./*.c -lopengl32 -lgdi32 -o classic-games