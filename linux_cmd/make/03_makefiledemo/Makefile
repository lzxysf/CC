main.out : main.o add.o multi.o
	gcc main.o add.o multi.o -o main.out

main.o : main.c add.h multi.h
	gcc -c main.c -o main.o

add.o : add.c
	gcc -c add.c -o add.o

multi.o : multi.c
	gcc -c multi.c -o multi.o

.PHONY:clean
clean:
	rm -rf *.o *.out
