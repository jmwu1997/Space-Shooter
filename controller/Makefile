all: controller
	rm -rf *.o

controller: controller.o
	gcc controller.c -o controller

controller.o:controller.c
	gcc -c controller.c

clean:
	rm -rf *.o controller

