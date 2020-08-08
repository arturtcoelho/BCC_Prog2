#include <stdio.h>

int main(int argc, char const *argv[])
{
	FILE *fp;
	char str[100];

	fp = fopen("txt", "r");

	while(!feof(fp)){
		fgets(str, 99, fp);
		printf("%s", str);
	}
	printf("\n");

	return 0;
}