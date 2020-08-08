#include <stdio.h>
#include <string.h>

int main (){
	char str[100];
	fgets(str, 100, stdin);
	printf("%s", str);
	for (int i = 0; i < strlen(str); i++){
		printf("%d", str[i]);
	}
	printf("\n");
	printf("%ld\n", strlen(str));
	return 0;
}
