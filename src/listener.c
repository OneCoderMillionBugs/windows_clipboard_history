#include "listener.h"

UINT GetClipboardFormat(void)
{
    const UINT FORMATS[] = {CF_UNICODETEXT, CF_DIB, CF_HDROP};
    for (int i = 0; i < sizeof(FORMATS) / sizeof(UINT); i++)
    {
        if (IsClipboardFormatAvailable(FORMATS[i]))
        {
            return FORMATS[i];
        }
    }
    return -1;
}

DWORD WINAPI MonitorClip(LPVOID lpParam)
{
    static DWORD currentNum = 0;
    static DWORD prevNum = 0;
    GDATA *gdata = (GDATA *)lpParam;
    BUFFER *buffer = BufferCreate();
    GDataSet(BUF, buffer, gdata);

    while (1)
    {
        Sleep(20);
        prevNum = currentNum;
        currentNum = GetClipboardSequenceNumber();

        if (currentNum != prevNum)
        {
            UINT format = GetClipboardFormat();

            if (IsClipboardFormatAvailable(CF_IGNORE) || 
                format == -1 || 
                !OpenClipboard(NULL))
            {
                continue;
            }

            HANDLE h = GetClipboardData(format);
            UINT size = GlobalSize(h);
            BYTE *ptr = (BYTE *)GlobalLock(h);

            RECORD *newRecord = Record(format, size, ptr);
            BufferAdd(newRecord, buffer);

            GlobalUnlock(h);
            CloseClipboard();

            // Change update screen value
            SetGParamsUpdate(TRUE, GDataGet(GUI, gdata, NULL));
        }
    }

    return 0;
}