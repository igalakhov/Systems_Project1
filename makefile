all: myshell.o ./parser/argparser.o ./executor/executor.o
	gcc -o myshell myshell.o argparser.o executor.o
	rm *.o

myshell.o: myshell.c ./parser/argparser.h
	gcc -c myshell.c

parser/argparser.o: ./parser/argparser.c ./parser/argparser.h
	gcc -c ./parser/argparser.c

executor/executor.o: ./executor/executor.c ./executor/executor.h
	gcc -c ./executor/executor.c

run:
	./myshell

run-tests:
	./myshell < tests
