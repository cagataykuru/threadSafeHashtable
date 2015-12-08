
MYPATH = $(CURDIR)

all: libhash  demo

libhash: hash.c
	gcc -c -Wall -fpic hash.c
	gcc -shared -o libhash.so hash.o

demo: demo.c
	gcc -Wall -Wl,-rpath=${MYPATH} -L${MYPATH} -o demo demo.c -lpthread -lhash

clean: 
	rm -fr app1 app2 dp.o *.so *.o *~
