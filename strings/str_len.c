#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define SIZE 100

int main()
{
    // locale e inicialização
    setlocale(LC_CTYPE, "pt_BR.ISO-8859-1");
    unsigned char *s = (char*)malloc((SIZE+1) * sizeof(unsigned char));

    // leitura
    fgets(s, SIZE, stdin);
    s[strlen(s)-1] = 0;

    // strlen(s)
    int i;
    for (i = 0; s[i]; i++);
    printf("%d\n", i);
    

    return 0;
}
