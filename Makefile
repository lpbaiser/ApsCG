############################# Makefile ##########################
all:
	gcc -c main.c car.c -lGL -lglut -lGLU -Wall -ggdb
	gcc -o main main.o car.o -lGL -lglut -lGLU -Wall -ggdb
	
clean:
	rm -rf *.o

clean-all: clean
	rm -rf main