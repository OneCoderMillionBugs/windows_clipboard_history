#include <windows.h>
#include <wchar.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")

#define DEBUG
#ifdef DEBUG
    #define LOGPATH L"c:\\users\\ilyas\\desktop\\log.txt"
    void writelog(WCHAR *message)
    {
        FILE *file = _wfopen(LOGPATH, L"a");
        fwprintf(file, L"%ls\n", message);
        fclose(file);
    }
#endif

#define BUFFER_SIZE 9
#define FILE_MEM_OFFSET 20

#define BLOCKS_PER_ROW 3
#define OFFSET_X 10
#define OFFSET_Y 10
#define BLOCK_W 200
#define BLOCK_H 100


struct record
{
    UINT format;
    uint8_t *data;
}
typedef RECORD;

struct vargs
{
    size_t *index;
    bool *updateScreen;
    RECORD *buffer;
}
typedef VARGS;


const UINT FORMATS[] = {
    CF_UNICODETEXT, CF_DIBV5, CF_HDROP, CF_DIB
};


UINT getClipboardFormat(void)
{
    for (int i = 0; i < sizeof(FORMATS) / sizeof(UINT); i++)
    {
        if (IsClipboardFormatAvailable(FORMATS[i]))
        {
            return FORMATS[i];
        }
    }
    return -1;
}


DWORD WINAPI monitorClip(LPVOID lpParam)
{
    VARGS *vargs = (VARGS *)lpParam;
    static DWORD currentNum = 0;
    static DWORD prevNum = 0;

    while (1)
    {
        Sleep(20);
        currentNum = GetClipboardSequenceNumber();

        if (currentNum != prevNum)
        {
            OpenClipboard(NULL);
            UINT format = getClipboardFormat();

            HANDLE h = GetClipboardData(format);
            size_t size = GlobalSize(h);
            uint8_t *ptr = (uint8_t *)GlobalLock(h);

            vargs->buffer[*(vargs->index)].data = (uint8_t *)malloc(size);
            memcpy(vargs->buffer[*(vargs->index)].data, ptr, size);

            GlobalUnlock(h);
            CloseClipboard();
            prevNum = currentNum;

            vargs->buffer[*(vargs->index)].format = format;
            *(vargs->index) = *(vargs->index) + 1;
            *(vargs->updateScreen) = true;
        }
    }

    return 0;
}


RECORD *initBuffer(void)
{
    RECORD *buffer = (RECORD *)malloc(BUFFER_SIZE * sizeof(RECORD));
    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i].format = 0;
        buffer[i].data = NULL;
    }
    return buffer;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_PAINT:
            RECORD *buffer = (RECORD *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HFONT hFont = CreateFontW(15, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
                                      0, 0, 0, 0, L"Georgia");
            HFONT holdFont = SelectObject(hdc, hFont);

                    FILE *file = _wfopen(LOGPATH, L"a");

            for (size_t i = 0; i < BUFFER_SIZE; i++)
            {
                int x = i % BLOCKS_PER_ROW;
                int y = i / BLOCKS_PER_ROW;
                Rectangle(hdc, 10 + x * BLOCK_W, 10 + y * BLOCK_H, 
                          x * BLOCK_W + BLOCK_W, y * BLOCK_H + BLOCK_H);
                WCHAR *text = (WCHAR *)(buffer[i].data);
                if (text != NULL)
                {
                    TextOutW(hdc, 10 + x * BLOCK_W, 10 + y * BLOCK_H, text, lstrlenW(text));
                    fwprintf(file, L"%ls\n", text);
                }
            }

            fwprintf(file, L"%ls\n", L"\n\n");
            fclose(file); 
            EndPaint(hwnd, &ps);
            break;

        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    size_t index = 0;
    bool updateScreen = false;
    RECORD *buffer = initBuffer();

    // Create window
    MSG msg = {0};
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"mainWindow";
    wc.lpfnWndProc = WindowProc;

    RegisterClassW(&wc);

    HWND window = CreateWindowW(L"mainWindow", L"Clipboard", 
                                WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                                200, 200, 800, 600, 
                                NULL, NULL, NULL, NULL);

    // ShowWindow(window, SW_MINIMIZE);
    // Shell_NotifyIconW(); Future feature with hiding the window

    // Pass the arguments to the monitorClip function
    VARGS vargs = {&index, &updateScreen, buffer};
    HANDLE hThread = CreateThread(NULL, 0, monitorClip, &vargs, 0, NULL);

    // Set GWLP_USERDATA so callback function can take additional parameters
    SetWindowLongPtrW(window, GWLP_USERDATA, (LONG_PTR)buffer);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (updateScreen)
        {
            InvalidateRect(window, NULL, TRUE);
            updateScreen = false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CloseHandle(hThread);
    return 0;
}

/**
 * To-do list:
 * 
 */