all: myshell.o argparser.o executor.o
	gcc -o myshell myshell.o argparser.o executor.o
	rm *.o

myshell.o: myshell.c argparser.h
	gcc -c myshell.c

argparser.o: argparser.c argparser.h
	gcc -c argparser.c

executor.o: executor.c executor.h
	gcc -c executor.c

# run
run:
	./myshell
