#include <stddef.h>
#include <stdlib.h>

#ifndef __meml_h__
#define __meml_h__

#define malloc(size) ml_malloc(size, __FILE__, __LINE__)
#define calloc(n, size) ml_calloc(n, size, __FILE__, __LINE__)
#define free(ptr) ml_free(ptr, __FILE__, __LINE__)
#define realloc(ptr, size) ml_realloc(ptr, size, __FILE__, __LINE__)

/// Replacement for malloc()
void *ml_malloc(size_t size, const char *file, int line);
/// Replacement for calloc()
void *ml_calloc(size_t n, size_t size, const char *file, int line);
/// Replacement for realloc()
void *ml_realloc(void *ptr, size_t size, const char *file, int line);
/// Replacement for free()
void ml_free(void *ptr, const char *file, int line);

/// Prints unfreed pointers
int ml_report(void);

#endif // __meml_h__
