Trabalho de Programação 2 UFPR 2020 
Período especial com prof. Maziero

O arquivo main.c contém a chamada para os diversos filtros, 
esses filtros estão implementados no arquivo data_handler.c.

O arquivo file_handle.c realiza as funções 
relacionadas aos arquivos wav

Os filtros são aplicados com links simbolicos para o executavel 
main e são utilizados como demosntrados a seguir:

A opção -s (safe) limita o volume 
máximo de entrada das amostras

wavinfo -i input
    Mostra as informações de cabeçalho
wavvol -i input -o output -l level
    Altera a amplitude pelo valor level
wavnorm -i input -o output - level
    Normaliza as amostras
wavecho -i input -o output -t tempo -l level
    Realiza o efeito de eco, com delay e nivel
wavrev -i input -o output 
    Inverte o padrão de amostras
wavwide -i input -o output -l level
    Amplia o efeito estéreo
wavfreq -i input -o output -l level
    Modifica a taxa de amostragem em level
wavext -i input -o output
    Extrai o primeiro canal da amostra
wavcat <arquivos> -o output
    Concatena as amostras em sequencia
wavmix <arquivos> -o output 
    Mistura as amostras