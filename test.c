
#include <stdlib.h>
#include <stdio.h>

#include "meml.h"

int main() {

	int count = 0;
	for (int i = 0; i < 100; ++i) {
		void *p = NULL;

		int size = rand() % 1000;

		if (size & 1) p = malloc(size);
		else p = calloc(2, size);

		if (size % 4 == 0) {
			size += (size & 128 ? -1 : 1) * (rand() % 64);
			p = realloc(p, size);
		}

		if (size % 3 == 0) free(p);
		else {
			printf("%p <-- %d\n", p, size);
			++count;
		}
	}

	if (count != ml_report()) {
		printf("NOT ALL LISTED\n");
	} else printf("TOTAL: %d\n", count);
	return 0;
}
