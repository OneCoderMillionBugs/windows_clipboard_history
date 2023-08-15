#ifndef ERROR_H
#define ERROR_H

#include <windows.h>

// Function error codes
#define ERR_NO_ERROR 0
#define ERR_NULL_INPUT 1
#define ERR_MUTEX_ABANDONED 3
#define ERR_MUTEX_TIMEOUT 4
#define ERR_MUTEX_FAILED 5

inline void WriteCode(UINT *ptr, UINT code)
{
    if (ptr != NULL)
    {
        *ptr = code;
    }
}

#endif