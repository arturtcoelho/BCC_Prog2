CFLAGS = -Wall

filters = wavcat wavinfo wavvol wavrev wavnorm wavecho wavwide wavmix wavext wavfreq

all: main $(filters)

file_handle.o: file_handle.h file_handle.c header.h
data_handler.o: data_handler.h data_handler.c header.h

main.o: data_handler.o data_handler.h file_handle.h file_handle.o header.h main.c
main: main.o file_handle.o data_handler.o

$(filters): $(if $(wildcard main),,main)
	ln -s -f main $@

clean: all
	-rm -f ~. *.o

purge:
	-rm -f ~. *.o
	-rm -f ~. $(filters)