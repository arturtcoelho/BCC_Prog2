#include <stdio.h>
#include <string.h>

void come (char* str, int i){
    printf("%d\n", i);
    while (str[i+1]){
        str[i] = str[i+1];
        i++;
    }
}

int main (){
    char input[101];
    fgets(input, 100, stdin);
    for (int i = 0; i < strlen(input); i++){
        int isntvalid = 1;
        if (input[i] <= '0' && input[i] >= '9')
            isntvalid = 0;
        if (input[i] <= 'A' && input[i] >= 'Z')
            isntvalid = 0;
        if (input[i] <= 'a' && input[i] >= 'z')
            isntvalid = 0;
        printf("inst %d\n", isntvalid);
        if (isntvalid)
            come(input, i);
    }

    printf("%s", input);

    return 0;
}