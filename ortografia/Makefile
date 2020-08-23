CFLAGS = -Wall

objs = dicionario.o ortografia.o

all: ortografia

ortografia: $(objs)

ortografia.o: ortografia.c dicionario.h
dicionario.o: dicionario.c dicionario.h

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f ortografia