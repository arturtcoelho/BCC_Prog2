CFLAGS = -Wall -lm
LIBS = -lm

all: wavcat wavinfo wavvol wavrev wavnorm wavecho wavwide wavmix wavext wavfreq wavvib

file_handle.o: file_handle.h file_handle.c header.h
data_handler.o: data_handler.h data_handler.c header.h

headers = data_handler.o data_handler.h file_handle.h file_handle.o header.h

wavcat.o: $(headers) wavcat.c 
wavinfo.o: $(headers) wavinfo.c  
wavecho.o: $(headers) wavecho.c 
wavnorm.o: $(headers) wavnorm.c 
wavrev.o: $(headers) wavrev.c
wavvol.o: $(headers) wavvol.c
wavwide.o: $(headers) wavwide.c
wavmix.o: $(headers) wavmix.c
wavext.o: $(headers) wavext.c
wavfreq.o: $(headers) wavfreq.c
wavvib.o: $(headers) wavvib.c

objects = file_handle.o data_handler.o

wavcat: wavcat.o $(objects) $(LIBS)
wavinfo: wavinfo.o $(objects) $(LIBS)
wavvol: wavvol.o $(objects) $(LIBS)
wavrev: wavrev.o $(objects) $(LIBS)
wavnorm: wavnorm.o $(objects) $(LIBS)
wavecho: wavecho.o $(objects) $(LIBS)
wavwide: wavwide.o $(objects) $(LIBS)
wavmix: wavmix.o $(objects) $(LIBS)
wavext: wavext.o $(objects) $(LIBS)
wavfreq: wavfreq.o $(objects) $(LIBS)
wavvib: wavvib.o $(objects) $(LIBS)

clean: all
	-rm -f ~. *.o

purge:
	-rm -f ~. *.o
	-rm -f ~. wavcat wavinfo wavecho wavnorm wavrev wavvol wavext wavfreq wavvib wavwide wavmix