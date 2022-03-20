/* ByteArray object interface */

#ifndef Py_BYTEARRAYOBJECT_H
#define Py_BYTEARRAYOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

/** Helper function to implement the inplace repeat method on a buffer
 *
 * This method repeately doubles the number of bytes copied to reduce to the number of invocations of memcpy
 */
inline void _PyBytes_RepeatInPlace_helper(char* buffer, Py_ssize_t start_len, Py_ssize_t end_len)
{
    Py_ssize_t copied = start_len;
    while (copied < end_len) {
        Py_ssize_t bytes_to_copy = Py_MIN(copied, end_len - copied);
        memcpy(buffer + copied, buffer, bytes_to_copy);
        copied += bytes_to_copy;
    }
}

// Helper function to implement the inplace repeat method on a buffer
inline void _PyBytes_RepeatInPlace(char* buffer, Py_ssize_t start_len, Py_ssize_t end_len)
{
    if (start_len==1)
        memset(buffer, buffer[0], end_len);
    else
        _PyBytes_RepeatInPlace_helper(buffer, start_len, end_len);
}

// Helper function to implement the repeat method on a buffer
inline void _PyBytes_Repeat(char* dest, Py_ssize_t len_dest, const char* src, Py_ssize_t len_src)
{
    if (len_src == 1)
        memset(dest, src[0], len_dest);
    else {
        memcpy(dest, src, len_src);
        _PyBytes_RepeatInPlace_helper(dest, len_src, len_dest);
    }
}



/* Type PyByteArrayObject represents a mutable array of bytes.
 * The Python API is that of a sequence;
 * the bytes are mapped to ints in [0, 256).
 * Bytes are not characters; they may be used to encode characters.
 * The only way to go between bytes and str/unicode is via encoding
 * and decoding.
 * For the convenience of C programmers, the bytes type is considered
 * to contain a char pointer, not an unsigned char pointer.
 */

/* Type object */
PyAPI_DATA(PyTypeObject) PyByteArray_Type;
PyAPI_DATA(PyTypeObject) PyByteArrayIter_Type;

/* Type check macros */
#define PyByteArray_Check(self) PyObject_TypeCheck(self, &PyByteArray_Type)
#define PyByteArray_CheckExact(self) Py_IS_TYPE(self, &PyByteArray_Type)

/* Direct API functions */
PyAPI_FUNC(PyObject *) PyByteArray_FromObject(PyObject *);
PyAPI_FUNC(PyObject *) PyByteArray_Concat(PyObject *, PyObject *);
PyAPI_FUNC(PyObject *) PyByteArray_FromStringAndSize(const char *, Py_ssize_t);
PyAPI_FUNC(Py_ssize_t) PyByteArray_Size(PyObject *);
PyAPI_FUNC(char *) PyByteArray_AsString(PyObject *);
PyAPI_FUNC(int) PyByteArray_Resize(PyObject *, Py_ssize_t);

#ifndef Py_LIMITED_API
#  define Py_CPYTHON_BYTEARRAYOBJECT_H
#  include "cpython/bytearrayobject.h"
#  undef Py_CPYTHON_BYTEARRAYOBJECT_H
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_BYTEARRAYOBJECT_H */
