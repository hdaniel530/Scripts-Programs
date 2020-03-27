#include <stdio.h>

char buf[1];


int main() {
	printf("Hello World!\n");
	char hello[] = "hello";
	printf("%s",hello);
	memrealloc(hello);
	return 0;
}

void memrealloc(char* p) {
	char* newarray = malloc(2*sizeof(p));
	memove(newarray, p, sizeof(p));
	printf(1, "%s", newarray);
	free(newarray);
}

/*
#include "types.h"
#include "stat.h"
#include "user.h"

int main(void){
	printf(1,"Hello world\n");
	exit();
}
*/
