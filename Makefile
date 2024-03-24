cflags=-Wall 
CC = g++ 
LIBS = -lX11 -lglut -lGL -lGLU -lm
FLAGS = -L/usr/X11R6/lib/ 
PROGS = robot

all: ${PROGS}
robot: robot.o geometry.o objik.o scene.o 
	${CC} ${FLAGS} robot.o objik.o geometry.o scene.o -o robot ${LIBS}


robot.o: robot.cc geometry.o
	${CC} -c robot.cc -o robot.o 
objik.o: objik.cc objik.h
	${CC} -c objik.cc -o objik.o 

geometry.o: geometry.cc  geometry.h
	${CC} -c geometry.cc -o geometry.o
scene.o: scene.cpp scene.h
	${CC} -c scene.cpp -o scene.o

