#ifndef Py_INTERNAL_BYTESOBJECT_H
#define Py_INTERNAL_BYTESOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif

#include <string.h>

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


static inline void
_PyBytes_Repeat(char* dest, Py_ssize_t len_dest,
    const char* src, Py_ssize_t len_src)
{
    if (len_dest == 0)
        return;
    if (len_src == 1) {
        if (src)
            memset(dest, src[0], len_dest);
        else
            memset(dest, dest[0], len_dest);
    }
    else {
        if (src!=0)
            memcpy(dest, src, len_src);

        Py_ssize_t copied = len_src;
        while (copied < len_dest) {
            Py_ssize_t bytes_to_copy = Py_MIN(copied, len_dest - copied);
            memcpy(dest + copied, dest, bytes_to_copy);
            copied += bytes_to_copy;
        }
    }
}

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTERNAL_BYTESOBJECT_H */
