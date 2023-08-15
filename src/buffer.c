#include "buffer.h"
#include "linked_list.h"

struct record
{
    BYTE *data;
    UINT format;
    UINT size;
};

struct buffer
{
    LinkedList *list;
};

RECORD *Record(UINT format, UINT size, BYTE *ptr)
{
    RECORD *record = (RECORD *)malloc(sizeof(RECORD));
    record->format = format;
    record->size = size;
    record->data = (BYTE *)malloc(size);
    memcpy(record->data, ptr, size);

    return record;
}

BYTE *RecordGetData(RECORD *record)
{
    if (record == NULL)
    {
        return NULL;
    }
    return record->data;
}

UINT RecordGetFormat(RECORD *record)
{
    if (record == NULL)
    {
        return 0;
    }
    return record->format;
}

UINT RecordGetSize(RECORD *record)
{
    if (record == NULL)
    {
        return 0;
    }
    return record->size;
}

BUFFER *BufferCreate(void)
{
    CreateMutexW(NULL, FALSE, L"buffer");
    BUFFER *buffer = (BUFFER *)malloc(sizeof(BUFFER));
    buffer->list = ListCreate();

    return buffer;
}

UINT BufferAdd(RECORD *record, BUFFER *buffer)
{
    if (buffer == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"buffer");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            ListAdd(record, buffer->list);
            ReleaseMutex(mutex);
            break;

        case WAIT_ABANDONED:
            errcode = ERR_MUTEX_ABANDONED;
            break;

        case WAIT_FAILED:
            errcode = ERR_MUTEX_FAILED;
            break; 
    }

    CloseHandle(mutex);
    return errcode;
}

RECORD *BufferGet(UINT index, BUFFER *buffer, UINT *errcode)
{
    if (buffer == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return NULL;
    }

    RECORD *result = NULL;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"buffer"); 
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = ListGet(index, buffer->list);
            ReleaseMutex(mutex);
            WriteCode(errcode, ERR_NO_ERROR);
            break;

        case WAIT_ABANDONED:
            WriteCode(errcode, ERR_MUTEX_ABANDONED);
            break;

        case WAIT_FAILED:
            WriteCode(errcode, ERR_MUTEX_FAILED);
            break; 
    }

    CloseHandle(mutex);
    return result;
}

INT BufferSize(BUFFER *buffer, UINT *errcode)
{
    if (buffer == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return -1;
    }

    INT result = -1;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"buffer"); 
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = ListSize(buffer->list);
            ReleaseMutex(mutex);
            WriteCode(errcode, ERR_NO_ERROR);
            break;

        case WAIT_ABANDONED:
            WriteCode(errcode, ERR_MUTEX_ABANDONED);
            break;

        case WAIT_FAILED:
            WriteCode(errcode, ERR_MUTEX_FAILED);
            break; 
    }

    CloseHandle(mutex);
    return result;
}