all: main.o shell.o
	gcc -o program main.o shell.o

main.o: main.c shell.h
	gcc -c main.c

shell.o: shell.c shell.h
	gcc -c shell.c

run:
	./program.exe

clean:
	rm *.o
