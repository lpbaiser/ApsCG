############################# Makefile ##########################
all:
	g++ -c teste.cpp -lGL -lglut -lGLU -Wall -ggdb
	gcc -c main.c casa.c -lGL -lglut -lGLU -Wall -ggdb
	
#	g++ -o teste teste.o -lGL -lglut -lGLU -Wall -ggdb
	g++ -o main teste.o main.o casa.o -lGL -lglut -lGLU -Wall -ggdb
	
clean:
	rm -rf *.o

clean-all: clean
	rm -rf main