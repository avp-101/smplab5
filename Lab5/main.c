#include <stdio.h>
extern void reverse(char *); /* extern "C" pentru un proiect C++ */
int main(int argc, char * argv[])
{
	char mesaj[] = "HELLO";
	printf("%s\n", mesaj);
	reverse(mesaj);
	printf("%s\n", mesaj);
	getchar();
	return 0;
}