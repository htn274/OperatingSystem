obj-m+=generatorDev.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M="$(PWD)" modules
	$(CC) testGenerator.c -o test
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M="$(PWD)" clean
	rm test