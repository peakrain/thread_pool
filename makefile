OBJ=main.o LinkQueue.o threadpool.o packet.o
LIB=-lpthread -lpcap
main:${OBJ}
	gcc -o main ${OBJ} ${LIB}
clean:
	rm -f ${OBJ}
