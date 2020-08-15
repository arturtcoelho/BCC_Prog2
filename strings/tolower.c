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

    // tolower(s)
    for (int i = 0; i < strlen(s); i++){
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] +32;
    }
    printf("%s\n", s);

    return 0;
}
