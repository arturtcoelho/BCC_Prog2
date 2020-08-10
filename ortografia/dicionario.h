// GRR 20190471 Artur Temporal Coelho
// carrega o dicionario em memória
char **carregar_dicionario(int *tam);
// confere se um char é uma letra válida
int eh_letra(char caractere);
// confere se uma palavra está contida no dicionário
int palavra_valida(char *palavra, char **dicionario, int tam);
// libera o espaço de memória alocado
void finaliza_programa(char **dicionario, int tam);