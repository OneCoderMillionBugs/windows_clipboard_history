#include "global.h"

struct gdata
{
    void **data;
};

GDATA *GlobalDataInit(void *lp, void *rp)
{
    CF_IGNORE = RegisterClipboardFormatW(L"CF_IGNORE");
    CreateMutexW(NULL, FALSE, L"global");

    GDATA *gdata = (GDATA *)malloc(sizeof(GDATA));
    gdata->data = (void **)malloc(2 * sizeof(void *));
    gdata->data[GUI] = lp;
    gdata->data[BUF] = rp;

    return gdata;
}

void *GDataGet(UINT index, GDATA *gdata, UINT *errcode)
{
    if (gdata == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return NULL;
    }

    void *result = NULL;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"global");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gdata->data[index];
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

UINT GDataSet(UINT index, void *ptr, GDATA *gdata)
{
    if (gdata == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"global");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gdata->data[index] = ptr;
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