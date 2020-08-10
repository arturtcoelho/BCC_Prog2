// GRR 20190471 Artur Temporal Coelho

// carrega o dicionario em memória
unsigned char **carregar_dicionario(int *tam);
// confere se uma palavra está contida no dicionário
int palavra_valida(unsigned char *palavra, unsigned char **dicionario, int tam);
// libera o espaço de memória alocado
void finaliza_programa(unsigned char **dicionario, int tam);