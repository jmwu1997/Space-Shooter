all: serial_write

serial_write: serial_write.o
	gcc serial_write.o -o serial_write

serial_write.o: serial_write.c
	gcc -c serial_write.c

clean:
	rm -rf *o serial_write
