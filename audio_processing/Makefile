CFLAGS = -Wall

all: wavinfo wavvol wavrev wavnorm wavecho

file_handle.o: file_handle.h file_handle.c header.h

wavinfo.o: file_handle.h header.h file_handle.o wavinfo.c  
wavecho.o: file_handle.h header.h file_handle.o wavecho.c 
wavnorm.o: file_handle.h header.h file_handle.o wavnorm.c 
wavrev.o: file_handle.h header.h file_handle.o wavrev.c
wavvol.o: file_handle.h header.h file_handle.o wavvol.c

wavinfo: wavinfo.o file_handle.o
wavvol: wavvol.o file_handle.o
wavrev: wavrev.o file_handle.o
wavnorm: wavnorm.o file_handle.o
wavecho: wavecho.o file_handle.o

clean: all
	-rm -f ~. *.o

purge:
	-rm -f ~. wavinfo wavecho wavnorm wavrev wavvol