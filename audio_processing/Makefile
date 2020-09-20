# Artur Temporal Coelho GRR20190471 
CFLAGS = -Wall
filters = wavcat wavinfo wavvol wavrev wavnorm wavecho wavwide wavmix wavext wavfreq

all: main $(filters)

# compilação dos arquivos de funções de arquivos binario e processamento de dados
file_handle.o: file_handle.h file_handle.c
data_handler.o: data_handler.h data_handler.c

# compilação e montagem do arquivo binário principal
main.o: data_handler.o data_handler.h file_handle.h file_handle.o main.c
main: main.o file_handle.o data_handler.o

# geração dos links simbólicos para o arquivo principal
$(filters): $(if $(wildcard main),,main)
	ln -s -f main $@

# limpeza dos arquivos
clean: all
	-rm -f ~. *.o

purge:
	-rm -f ~. *.o
	-rm -f ~. $(filters) main