#ifndef BUFFER_H
#define BUFFER_H

#include <windows.h>
#include "global.h"

typedef struct record RECORD;
typedef struct buffer BUFFER;

// Create new record (copies data from BYTE *)
RECORD *Record(UINT format, UINT size, BYTE *ptr);

// Get data from record
BYTE *RecordGetData(RECORD *record);

// Get format of a record
UINT RecordGetFormat(RECORD *record);

// Get size of a record
UINT RecordGetSize(RECORD *record);

// Create new buffer
BUFFER *BufferCreate(void);

// Add record to the buffer
UINT BufferAdd(RECORD *record, BUFFER *buffer);

// Get element at some index
RECORD *BufferGet(UINT index, BUFFER *buffer, UINT *errcode);

// Get size of a buffer
INT BufferSize(BUFFER *buffer, UINT *errcode);

#endif