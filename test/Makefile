main: main.o controls.o
	g++ -o main main.o controls.o -lglfw -lGLEW -lGLU -lGL

main.o: main.cpp
	g++ -c -I./include main.cpp -o main.o -lglfw -lGLEW -lGLU -lGL

controls.o: controls.cpp
	g++ -c -I./include controls.cpp -o controls.o -lglfw -lGLEW -lGLU -lGL
