CFLAGS = -Wall

all: wavcat wavinfo wavvol wavrev wavnorm wavecho

file_handle.o: file_handle.h file_handle.c header.h
data_handler.o: data_handler.h data_handler.c header.h

wavcat.o: data_handler.o data_handler.h file_handle.h header.h file_handle.o wavcat.c 
wavinfo.o: data_handler.o data_handler.h file_handle.h header.h file_handle.o wavinfo.c  
wavecho.o: data_handler.o data_handler.h file_handle.h header.h file_handle.o wavecho.c 
wavnorm.o: data_handler.o data_handler.h file_handle.h header.h file_handle.o wavnorm.c 
wavrev.o: data_handler.o data_handler.h file_handle.h header.h file_handle.o wavrev.c
wavvol.o: data_handler.o data_handler.h file_handle.h header.h file_handle.o wavvol.c

wavcat: wavcat.o file_handle.o data_handler.o
wavinfo: wavinfo.o file_handle.o data_handler.o
wavvol: wavvol.o file_handle.o data_handler.o
wavrev: wavrev.o file_handle.o data_handler.o
wavnorm: wavnorm.o file_handle.o data_handler.o
wavecho: wavecho.o file_handle.o data_handler.o

clean: all
	-rm -f ~. *.o

purge:
	-rm -f ~. *.o
	-rm -f ~. wavcat wavinfo wavecho wavnorm wavrev wavvol