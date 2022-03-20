#ifndef Py_INTERNAL_BYTESOBJECT_H
#define Py_INTERNAL_BYTESOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif


/* runtime lifecycle */

extern PyStatus _PyBytes_InitTypes(PyInterpreterState *);


/* Substring Search.

   Returns the index of the first occurence of
   a substring ("needle") in a larger text ("haystack").
   If the needle is not found, return -1.
   If the needle is found, add offset to the index.
*/

PyAPI_FUNC(Py_ssize_t)
_PyBytes_Find(const char *haystack, Py_ssize_t len_haystack,
              const char *needle, Py_ssize_t len_needle,
              Py_ssize_t offset);

/* Same as above, but search right-to-left */
PyAPI_FUNC(Py_ssize_t)
_PyBytes_ReverseFind(const char *haystack, Py_ssize_t len_haystack,
                     const char *needle, Py_ssize_t len_needle,
                     Py_ssize_t offset);

#include <string.h>

/** Helper function to implement the inplace repeat method on a buffer
 *
 * This method repeately doubles the number of bytes copied to reduce to the number of invocations of memcpy
 */
static inline void _PyBytes_RepeatInPlace_helper(char* buffer, Py_ssize_t start_len, Py_ssize_t end_len)
{
    printf("_PyBytes_RepeatInPlace_helper: start %lld %lld\n", start_len, end_len);
    Py_ssize_t copied = start_len;
    while (copied < end_len) {
        Py_ssize_t bytes_to_copy = Py_MIN(copied, end_len - copied);
        printf("_PyBytes_RepeatInPlace_helper %lld %lld %lld\n", copied, end_len, bytes_to_copy);
        memcpy(buffer + copied, buffer, bytes_to_copy);
        copied += bytes_to_copy;
    }
}

// Helper function to implement the inplace repeat method on a buffer
static inline void _PyBytes_RepeatInPlace(char* buffer, Py_ssize_t start_len, Py_ssize_t end_len)
{
    if (start_len == 1)
        memset(buffer, buffer[0], end_len);
    else
        _PyBytes_RepeatInPlace_helper(buffer, start_len, end_len);
}

// Helper function to implement the repeat method on a buffer
static inline void _PyBytes_Repeat(char* dest, Py_ssize_t len_dest, const char* src, Py_ssize_t len_src)
{
    if (len_src == 1)
        memset(dest, src[0], len_dest);
    else {
        if (len_dest >= len_src) {
            memcpy(dest, src, len_src);
            _PyBytes_RepeatInPlace_helper(dest, len_src, len_dest);
        }
    }
}

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTERNAL_BYTESOBJECT_H */
