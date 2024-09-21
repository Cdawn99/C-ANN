#ifndef UTILS_H_
#define UTILS_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFER_RETURN(ret_val) \
    do {                      \
        result = (ret_val);   \
        goto defer;           \
    } while (0)

#define UTILS_DA_FREE(da)  \
    do {                   \
        free((da)->items); \
    } while (0)

#define UTILS_DA_DEFAULT_CAPACITY 16

#define UTILS_DA_APPEND(da, elem)                                                    \
    do {                                                                             \
        if ((da)->length == (da)->capacity) {                                        \
            (da)->capacity *= 2;                                                     \
            if ((da)->capacity == 0) {                                               \
                (da)->capacity = UTILS_DA_DEFAULT_CAPACITY;                          \
            }                                                                        \
            void *temp = realloc((da)->items, (da)->capacity * sizeof *(da)->items); \
            assert(temp && "Not enough RAM for realloc");                            \
            (da)->items = temp;                                                      \
        }                                                                            \
        (da)->items[(da)->length++] = (elem);                                        \
    } while (0)

#define UTILS_DA_PREPEND(da, elem)                                                   \
    do {                                                                             \
        if ((da)->length == (da)->capacity) {                                        \
            (da)->capacity *= 2;                                                     \
            if ((da)->capacity == 0) {                                               \
                (da)->capacity = UTILS_DA_DEFAULT_CAPACITY;                          \
            }                                                                        \
            void *temp = realloc((da)->items, (da)->capacity * sizeof *(da)->items); \
            assert(temp && "Not enough RAM for realloc");                            \
            (da)->items = temp;                                                      \
        }                                                                            \
        for (size_t i = (da)->length; i > 0; i--) {                                  \
            (da)->items[i] = (da)->items[i-1];                                       \
        }                                                                            \
        (da)->items[0] = (elem);                                                     \
        (da)->length++;                                                              \
    } while (0)

static inline int mod(int x, int n) {
    return ((x%n) + n) % n;
}

/**
 * Read the contents of the given file.
 *
 * @param filepath The path to the file to be read.
 * @param content The file's contents are allocated and read into the pointer
 *      pointed to by this pointer. Must be freed by caller.
 * @return The number of bytes read or zero, if a failure occured while reading.
 *      When a failure occurs, an error message is printed to stderr.
 */
size_t read_entire_file(const char *filepath, char **content);

#endif // UTILS_H_

#ifdef UTILS_IMPLEMENTATION

size_t read_entire_file(const char *filepath, char **content) {
    size_t result;

    FILE *f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, "Failed to open file: %s\n", filepath);
        DEFER_RETURN(0);
    }

    if (fseek(f, 0, SEEK_END)) {
        perror("Failed to get to the end of file");
        DEFER_RETURN(0);
    }

    long f_size = ftell(f);
    if (f_size < 0) {
        perror("Failed to get the size of the file");
        DEFER_RETURN(0);
    }

    if (fseek(f, 0, SEEK_SET)) {
        perror("Failed to get to the start of file");
        DEFER_RETURN(0);
    }

    *content = calloc(f_size + 1, 1);
    if (!*content) {
        fprintf(stderr, "Failed to allocate memory for content from %s\n", filepath);
        DEFER_RETURN(0);
    }

    fread(*content, 1, f_size, f);
    if (ferror(f)) {
        fprintf(stderr, "There was an error while reading %s\n", filepath);
        free(*content);
        DEFER_RETURN(0);
    }

    result = f_size;

defer:
    if (f) {
        fclose(f);
    }
    return result;
}

#endif // UTILS_IMPLEMENTATION
