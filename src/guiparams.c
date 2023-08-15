#include "guiparams.h"

struct guiparams
{
    SCROLLINFO *si;
    UINT CLIENT_W;
    UINT CLIENT_H;
    UINT BLOCKS_PER_ROW;
    UINT ROWS_PER_PAGE;
    UINT BLOCKS_PER_PAGE;
    UINT BLOCK_W;
    UINT BLOCK_H;
    UINT ALIGN_H;
    UINT ALIGN_V;
    BOOL updateScreen;
};

GUIPARAMS *GParamsCreate(void)
{
    CreateMutexW(NULL, FALSE, L"gui");
    GUIPARAMS *gp = (GUIPARAMS *)calloc(1, sizeof(GUIPARAMS));
    gp->si = (SCROLLINFO *)malloc(sizeof(SCROLLINFO));
    gp->si->cbSize = sizeof(SCROLLINFO);
    gp->si->fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
    gp->si->nPos = 0;
    gp->si->nPage = 0;
    gp->si->nMin = 0;
    gp->si->nMax = 0;

    return gp;
}

UINT SetGParamsUpdate(BOOL updateScreen, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->updateScreen = updateScreen;
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

UINT SetGParamsBHeight(UINT height, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->BLOCK_H = height;
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

UINT SetGParamsBWidth(UINT width, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->BLOCK_W = width;
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

UINT SetGParamsBPP(UINT bpp, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->BLOCKS_PER_PAGE = bpp;
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

UINT SetGParamsRPP(UINT rpp, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->ROWS_PER_PAGE = rpp;
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

UINT SetGParamsBPR(UINT bpr, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->BLOCKS_PER_ROW = bpr;
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

UINT SetGParamsCHeight(UINT height, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->CLIENT_H = height;
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

UINT SetGParamsCWidth(UINT width, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->CLIENT_W = width;
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

UINT SetGParamsAlignH(UINT align, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->ALIGN_H = align;
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

UINT SetGParamsAlignV(UINT align, GUIPARAMS *gp)
{
    if (gp == NULL)
    {
        return ERR_NULL_INPUT;
    }

    UINT errcode = ERR_NO_ERROR;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            gp->ALIGN_V = align;
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

BOOL GetGParamsUpdate(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return FALSE;
    }

    BOOL result = FALSE;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->updateScreen;
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

UINT GetGParamsBHeight(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return 0;
    }

    UINT result = 0;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->BLOCK_H;
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

UINT GetGParamsBWidth(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return 0;
    }

    UINT result = 0;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->BLOCK_W;
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

UINT GetGParamsBPP(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return 0;
    }

    UINT result = 0;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->BLOCKS_PER_PAGE;
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

UINT GetGParamsRPP(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return 0;
    }

    UINT result = 0;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->ROWS_PER_PAGE;
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

UINT GetGParamsBPR(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return 0;
    }

    UINT result = 0;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->BLOCKS_PER_ROW;
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

UINT GetGParamsCHeight(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return 0;
    }

    UINT result = 0;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->CLIENT_H;
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

UINT GetGParamsCWidth(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return 0;
    }

    UINT result = 0;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->CLIENT_W;
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

SCROLLINFO *GetGParamsScroll(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return NULL;
    }

    SCROLLINFO *result = NULL;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->si;
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

UINT GetGParamsAlignH(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return 0;
    }

    UINT result = 0;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->ALIGN_H;
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

UINT GetGParamsAlignV(GUIPARAMS *gp, UINT *errcode)
{
    if (gp == NULL)
    {
        WriteCode(errcode, ERR_NULL_INPUT);
        return 0;
    }

    UINT result = 0;
    HANDLE mutex = OpenMutexW(SYNCHRONIZE, FALSE, L"gui");
    DWORD waitRes = WaitForSingleObject(mutex, INFINITE);

    switch (waitRes)
    {
        case WAIT_OBJECT_0:
            result = gp->ALIGN_V;
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