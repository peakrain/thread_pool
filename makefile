OBJ=main.o LinkQueue.o threadpool.o
LIB=-lpthread
main:${OBJ}
	gcc -o main ${OBJ} ${LIB}
clean:
	rm -f ${OBJ}
