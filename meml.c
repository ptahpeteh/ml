#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

//#include "meml.h"

/// Capacity of the array holding allocated pointers
#define ML_MAX_COUNT			200


/** List of allocated pointers
*/

static struct {
    const void *ptr;			///< allocated pointer
    size_t size;				///< size of allocated memory
    const char *file;			///< src file where allocation happened
    int line;					///< line number where allocation happened
} pointers[ML_MAX_COUNT] = { 0, };


/** Simple logging, like printf()

	@param format printf format specifier
*/

static void logger(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}


/** Registers allocated pointer

	@param ptr pointer
	@param size of allocated memory
	@param file src file where allocation happened
	@param line src file line number where allocation happened

*/

static void reg(const void *ptr, size_t size, const char *file, int line) {

	if (ptr) {

        for (int i = 0; i < sizeof(pointers) / sizeof(pointers[0]); ++i) {

            if (!pointers[i].ptr) {

                pointers[i].ptr = ptr;
                pointers[i].size = size;
                pointers[i].file = file;
                pointers[i].line = line;

                return;
            }
        }

		logger("warning: no room for ptr %p, malloc from [%s]:%d\n", ptr, file, line);
	}
}


/** Unregisters freed pointer

	@param ptr pointer
	@param file src file where free happened
	@param line src file line number where free happened
*/

static void unreg(const void *ptr, const char *file, int line) {

	if (ptr) {

        for (int i = 0; i < sizeof(pointers) / sizeof(pointers[0]); ++i) {

            if (pointers[i].ptr == ptr) {
                pointers[i].ptr = NULL;
                return;
            }
        }

        logger("warning: no ptr %p found, free in [%s]:%d\n", ptr, file, line);
    }
}


/** Prints unfreed pointers

	@return Number of dangling pointers
*/

int ml_report(void) {

	logger("\n*** DANGLING POINTERS: ***\n");

	int count = 0;
    for (int i = 0; i < sizeof(pointers) / sizeof(pointers[0]); ++i) {
        if (pointers[i].ptr) {
            logger("%p size %d --> %s:%d\n", pointers[i].ptr, pointers[i].size, pointers[i].file, pointers[i].line);
			++count;
		}
    }

	return count;
}


/** Replacement for malloc()

	@param size of memory to allocate
	@param file src file where allocation happened
	@param line src file line number where allocation happened

	@return allocated pointer
*/

void *ml_malloc(size_t size, const char *file, int line) {

    void *ptr = malloc(size);
	reg(ptr, size, file, line);

    return ptr;
}


/** Replacement for calloc()

	@param n number of elements to allocate
	@param size of an element
	@param file src file where allocation happened
	@param line src file line number where allocation happened

	@return allocated pointer
*/

void *ml_calloc(size_t n, size_t size, const char *file, int line) {

	void *ptr = calloc(n, size);
	reg(ptr, size * n, file, line);

	return ptr;
}


/** Replacement for free()

	@param ptr memory to be freed
	@param file src file where free happened
	@param line src file line number where free happened
*/

void ml_free(void *ptr, const char *file, int line) {
	unreg(ptr, file, line);
    free(ptr);
}


/** Replacement for realloc()

	@param ptr memory to be resized
	@param size new size
	@param file src file where realloc happened
	@param line src file line number where realloc happened

	@return reallocated pointer
*/

void *ml_realloc(void *ptr, size_t size, const char *file, int line) {

	unreg(ptr, file, line);
	ptr = realloc(ptr, size);
	reg(ptr, size, file, line);

	return ptr;
}
