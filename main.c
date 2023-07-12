#include <windows.h>
#include <wchar.h>
#include <time.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Shell32.lib")

#define CLIENT_W 790
#define CLIENT_H 500
#define BUFFER_SIZE 27
#define BLOCKS_PER_ROW 3
#define ROWS_PER_PAGE 3
#define BLOCKS_PER_PAGE 9
#define BLOCK_W 250
#define BLOCK_H 150

#define DEBUG
#ifdef DEBUG
    #define LOGPATH L"log.txt"
    void writelog(LPCWSTR message)
    {
        FILE *file = _wfopen(LOGPATH, L"a");
        fwprintf(file, L"%ls\n", message);
        fclose(file);
    }
#endif

struct record
{
    UINT format;
    UINT size;
    BYTE *data;
}
typedef RECORD;

struct vargs
{
    UINT *index;
    BOOL *updateScreen;
    RECORD *buffer;
}
typedef VARGS;

struct userdata
{
    RECORD *buffer;
    SCROLLINFO *si;
}
typedef USERDATA;


const UINT FORMATS[] = {
    CF_UNICODETEXT, CF_DIB, CF_HDROP
};
// Custom format for avoiding copy of data
// from clipboard history (buffer)
UINT CF_IGNORE;


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
        prevNum = currentNum;
        currentNum = GetClipboardSequenceNumber();

        if (currentNum != prevNum)
        {
            UINT format = getClipboardFormat();

            if (IsClipboardFormatAvailable(CF_IGNORE) || 
                !OpenClipboard(NULL) || 
                format == -1)
            {
                continue;
            }

            HANDLE h = GetClipboardData(format);
            UINT size = GlobalSize(h);
            BYTE *ptr = (BYTE *)GlobalLock(h);

            vargs->buffer[*(vargs->index)].data = (BYTE *)malloc(size);
            memcpy(vargs->buffer[*(vargs->index)].data, ptr, size);

            GlobalUnlock(h);
            CloseClipboard();

            vargs->buffer[*(vargs->index)].size = size;
            vargs->buffer[*(vargs->index)].format = format;
            *(vargs->index) = *(vargs->index) + 1;
            *(vargs->updateScreen) = TRUE;
        }
    }

    return 0;
}


RECORD *initBuffer(void)
{
    RECORD *buffer = (RECORD *)malloc(BUFFER_SIZE * sizeof(RECORD));
    for (UINT i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i].format = 0;
        buffer[i].data = NULL;
    }
    return buffer;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    USERDATA *ud = (USERDATA *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    static UINT copyIndex = -1;

    switch (msg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            SCROLLINFO *si = ud->si;

            if ((short)HIWORD(wp) / WHEEL_DELTA < 0 && si->nPos < si->nMax - si->nPage)
            {
                si->nPos = si->nPos + si->nPage;
                SetScrollInfo(hwnd, SB_VERT, si, TRUE);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if ((short)HIWORD(wp) / WHEEL_DELTA > 0 && si->nPos > si->nMin)
            {
                si->nPos = si->nPos - si->nPage;
                SetScrollInfo(hwnd, SB_VERT, si, TRUE);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        case WM_VSCROLL:
        {
            SCROLLINFO *si = ud->si;
            switch (LOWORD(wp))
            {
                case SB_LINEDOWN:
                {
                    if (si->nPos < si->nMax - si->nPage)
                    {
                        si->nPos = si->nPos + si->nPage;
                        SetScrollInfo(hwnd, SB_VERT, si, TRUE);
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }
                case SB_LINEUP:
                {
                    if (si->nPos > si->nMin)
                    {
                        si->nPos = si->nPos - si->nPage;
                        SetScrollInfo(hwnd, SB_VERT, si, TRUE);
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }
            }
            break;
        }
        case WM_RBUTTONDOWN:
        {
            SCROLLINFO *si = ud->si;
            WORD x = LOWORD(lp);
            WORD y = HIWORD(lp);

            const SCROLLINFO *csi = ud->si;
            UINT bufOffset = csi->nPos / csi->nPage * BLOCKS_PER_PAGE;
            copyIndex = bufOffset + BLOCKS_PER_ROW * (y / BLOCK_H) + (x / BLOCK_W);

            if (OpenClipboard(hwnd) && ud->buffer[copyIndex].format != 0)
            {
                EmptyClipboard();
                // Copy data from buffer to the new (Global) address since 
                // after calling SetClipBoardData the system takes control 
                // over the given chunk of memory
                UINT format = ud->buffer[copyIndex].format;
                HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, ud->buffer[copyIndex].size);
                BYTE *gptr = GlobalLock(hg);
                memcpy(gptr, ud->buffer[copyIndex].data, ud->buffer[copyIndex].size);

                if (SetClipboardData(format, hg) == NULL)
                {
                    writelog(L"ERROR");
                }
                SetClipboardData(CF_IGNORE, NULL);
                GlobalUnlock(hg);
                CloseClipboard();
            }

            InvalidateRect(hwnd, NULL, TRUE);

            break;
        }
        case WM_PAINT:
        {
            const SCROLLINFO *csi = ud->si;
            RECORD *buffer = ud->buffer;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HFONT hFont = CreateFontW(15, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
                                      0, 0, 0, 0, L"Georgia");
            HFONT holdFont = SelectObject(hdc, hFont);

            // Buffer index offset depending on the current page
            UINT bufOffset = csi->nPos / csi->nPage * BLOCKS_PER_PAGE;

            for (UINT i = 0; i < BLOCKS_PER_PAGE; i++)
            {
                int x = i % BLOCKS_PER_ROW;
                int y = i / BLOCKS_PER_ROW;

                if (i + bufOffset == copyIndex)
                {
                    HPEN hlPen = CreatePen(PS_SOLID, 5, RGB(67, 130, 192));
                    HPEN defaultPen = SelectObject(hdc, hlPen);
                    Rectangle(hdc, 10 + x * BLOCK_W, 10 + y * BLOCK_H, 
                              x * BLOCK_W + BLOCK_W, y * BLOCK_H + BLOCK_H);
                    SelectObject(hdc, defaultPen);
                }
                else
                {
                    Rectangle(hdc, 10 + x * BLOCK_W, 10 + y * BLOCK_H, 
                              x * BLOCK_W + BLOCK_W, y * BLOCK_H + BLOCK_H);
                }

                RECT drawArea = {20 + x * BLOCK_W, 20 + y * BLOCK_H, 
                                 x * BLOCK_W + BLOCK_W - 20, y * BLOCK_H + BLOCK_H - 20};

                switch(buffer[i + bufOffset].format)
                {
                    case CF_UNICODETEXT:
                    {
                        WCHAR *text = (WCHAR *)buffer[i + bufOffset].data;
                        DrawTextW(hdc, text, lstrlenW(text), &drawArea, 
                                  DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK);
                        break;
                    }
                    case CF_HDROP:
                    {
                        UINT count = DragQueryFileW((HDROP)buffer[i + bufOffset].data, 0xFFFFFFFF, NULL, 0);
                        LPWSTR *files = (LPWSTR *)malloc(count * sizeof(LPWSTR));
                        UINT bufferSizeSum = 1;

                        for (UINT j = 0; j < count; j++)
                        {
                            UINT bufferSize = 1 + DragQueryFileW((HDROP)buffer[i + bufOffset].data, j, NULL, 0);
                            files[j] = (LPWSTR)malloc(bufferSize * sizeof(WCHAR));
                            DragQueryFileW((HDROP)buffer[i + bufOffset].data, j, files[j], bufferSize);
                            bufferSizeSum += bufferSize;
                        }

                        LPWSTR result = (LPWSTR)malloc(bufferSizeSum * sizeof(WCHAR));
                        UINT resIndx = 0;

                        for (UINT j = 0; j < count; j++)
                        {
                            for (UINT k = 0; k < lstrlenW(files[j]); k++)
                            {
                                result[resIndx++] = files[j][k];
                            }
                            result[resIndx++] = L'\n';
                            free(files[j]);
                        }
                        result[resIndx] = L'\0';

                        DrawTextW(hdc, result, lstrlenW(result), &drawArea, 
                                  DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK);
                        free(files);
                        free(result);
                        break;
                    }
                    case CF_DIB:
                    {
                        BITMAPINFO *bmi = (BITMAPINFO *)buffer[i + bufOffset].data;
                        SetStretchBltMode(hdc, HALFTONE);
                        StretchDIBits(hdc, 10 + x * BLOCK_W, 10 + y * BLOCK_H, BLOCK_W - 10, BLOCK_H - 10,
                                      0, 0, bmi->bmiHeader.biWidth, bmi->bmiHeader.biHeight,
                                      buffer[i + bufOffset].data + sizeof(BITMAPINFO), 
                                      bmi,
                                      DIB_RGB_COLORS,
                                      SRCCOPY);
                        break;
                    }
                }
            }

            EndPaint(hwnd, &ps);
            break;
        }
        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    UINT index = 0;
    BOOL updateScreen = FALSE;
    RECORD *buffer = initBuffer();

    CF_IGNORE = RegisterClipboardFormatW(L"CF_IGNORE");

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
                                WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_VISIBLE | WS_VSCROLL, 
                                200, 200, CLIENT_W, CLIENT_H, 
                                NULL, NULL, NULL, NULL);

    // Setup scrollbar
    SCROLLINFO si = {sizeof(SCROLLINFO)};
    si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
    si.nPos = 0;
    si.nPage = CLIENT_H;
    si.nMin = 0;
    si.nMax = BUFFER_SIZE / BLOCKS_PER_PAGE * CLIENT_H;

    SetScrollInfo(window, SB_VERT, &si, TRUE);

    // Pass the arguments to the monitorClip function
    VARGS vargs = {&index, &updateScreen, buffer};
    HANDLE hThread = CreateThread(NULL, 0, monitorClip, &vargs, 0, NULL);

    // Set GWLP_USERDATA so callback function can take additional parameters
    USERDATA ud = {buffer, &si};
    SetWindowLongPtrW(window, GWLP_USERDATA, (LONG_PTR)&ud);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (updateScreen)
        {
            InvalidateRect(window, NULL, TRUE);
            updateScreen = FALSE;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CloseHandle(hThread);
    return 0;
}

/**
 * TODO:
 * Some of the objects may have several formats!
 * History overflow
 * Shell_NotifyIconW(); Future feature with hiding the window
 * Saving history in file
 * 
 * TODO: later, mb never :P
 * Refactor code (add more functions)
 */