#include "gui.h"

void CalculateClientWin(UINT *cw, UINT *ch, UINT *ww, UINT *wh)
{
    UINT w = GetSystemMetrics(SM_CXSCREEN) / 2;
    *cw = w + (60 - w % 60) % 60;

    UINT h = GetSystemMetrics(SM_CYSCREEN) / 2;
    *ch = h + (60 - h % 60) % 60;

    RECT winsize = {0, 0, *cw, *ch};
    AdjustWindowRect(&winsize, 
                     WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME |
                     WS_VISIBLE | WS_VSCROLL, 
                     FALSE);

    *ww = winsize.right - winsize.left;
    *wh = winsize.bottom - winsize.top;
}

void DisplayText(HDC hdc, RECT *area, BYTE *data)
{
    WCHAR *text = (WCHAR *)data;
    RECT textArea = {area->left + 10, area->top + 10, 
                     area->right - 10, area->bottom - 10};
    DrawTextW(hdc, text, lstrlenW(text), &textArea, 
              DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK);
}

void DisplayBMP(HDC hdc, RECT *area, BYTE *data)
{
    BITMAPINFO *bmi = (BITMAPINFO *)data;
    SetStretchBltMode(hdc, HALFTONE);
    StretchDIBits(hdc, area->left + 5, area->top + 5,
                  area->right - area->left - 10, area->bottom - area->top - 10,
                  0, 0, bmi->bmiHeader.biWidth, bmi->bmiHeader.biHeight,
                  data + sizeof(BITMAPINFO), 
                  bmi,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

void DisplayFiles(HDC hdc, RECT *area, BYTE *data)
{
    UINT count = DragQueryFileW((HDROP)data, 0xFFFFFFFF, NULL, 0);
    LPWSTR *files = (LPWSTR *)malloc(count * sizeof(LPWSTR));
    UINT bufferSizeSum = 1;

    for (UINT j = 0; j < count; j++)
    {
        UINT bufferSize = 1 + DragQueryFileW((HDROP)data, j, NULL, 0);
        files[j] = (LPWSTR)malloc(bufferSize * sizeof(WCHAR));
        DragQueryFileW((HDROP)data, j, files[j], bufferSize);
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
    result[bufferSizeSum - 1] = L'\0';

    RECT headArea = {area->left + 10, area->top + 5, 
                       area->right - 10, area->bottom - 5};

    RECT textArea = {area->left + 10, area->top + 30, 
                     area->right - 10, area->bottom - 30};

    COLORREF yellow = RGB(67, 130, 192);
    COLORREF old = SetTextColor(hdc, yellow);
    DrawTextW(hdc, L"FILE(S):", lstrlenW(L"FILE(S):"), &headArea,
              DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK);

    SetTextColor(hdc, old);
    DrawTextW(hdc, result, lstrlenW(result), &textArea, 
              DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK);

    free(files);
    free(result);
}

void mousewheel(HWND hwnd, GUIPARAMS *gp, WPARAM wp)
{
    SCROLLINFO *si = GetGParamsScroll(gp, NULL);

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
}

void vscroll(HWND hwnd, GUIPARAMS *gp, WPARAM wp)
{
    SCROLLINFO *si = GetGParamsScroll(gp, NULL);
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
}

void rbuttondown(HWND hwnd, GUIPARAMS *gp, BUFFER *buffer,
                 UINT *copyIndex, WPARAM wp, LPARAM lp)
{
    SCROLLINFO *si = GetGParamsScroll(gp, NULL);
    WORD x = LOWORD(lp);
    WORD y = HIWORD(lp);

    UINT bufOffset = si->nPos / si->nPage * GetGParamsBPP(gp, NULL);
    *copyIndex = bufOffset + GetGParamsBPR(gp, NULL) * 
                 (y / GetGParamsBHeight(gp, NULL)) + (x / GetGParamsBWidth(gp, NULL));

    if (OpenClipboard(hwnd))
    {
        EmptyClipboard();
        // Copy data from buffer to the new (Global) address since 
        // after calling SetClipBoardData the system takes control 
        // over the given chunk of memory
        UINT format = RecordGetFormat(BufferGet(*copyIndex, buffer, NULL));
        HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, RecordGetSize(BufferGet(*copyIndex, buffer, NULL)));
        BYTE *gptr = GlobalLock(hg);
        memcpy(gptr, 
               RecordGetData(BufferGet(*copyIndex, buffer, NULL)), 
               RecordGetSize(BufferGet(*copyIndex, buffer, NULL)));

        SetClipboardData(format, hg);
        SetClipboardData(CF_IGNORE, NULL);
        GlobalUnlock(hg);
        CloseClipboard();
    }

    InvalidateRect(hwnd, NULL, TRUE);
}

void paint(HWND hwnd, GUIPARAMS *gp, BUFFER *buffer, UINT copyIndex)
{
    UINT BLOCK_H = GetGParamsBHeight(gp, NULL);
    UINT BLOCK_W = GetGParamsBWidth(gp, NULL);
    UINT BPP = GetGParamsBPP(gp, NULL);
    UINT RPP = GetGParamsRPP(gp, NULL);
    UINT BPR = GetGParamsBPR(gp, NULL);
    UINT VAL = GetGParamsAlignV(gp, NULL);
    UINT HAL = GetGParamsAlignH(gp, NULL);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    HPEN hPen= CreatePen(PS_SOLID, 5, RGB(67, 130, 192));
    HFONT hFont = CreateFontW(15, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
                              0, 0, 0, 0, L"Georgia");
    SelectObject(hdc, hFont);

    SCROLLINFO *si = GetGParamsScroll(gp, NULL);
    si->nPage = GetGParamsCHeight(gp, NULL);
    si->nMax = si->nPage * (BufferSize(buffer, NULL) / GetGParamsBPP(gp, NULL) + 1);

    SetScrollInfo(hwnd, SB_VERT, si, TRUE);
    UINT bufOffset = si->nPos / si->nPage * GetGParamsBPP(gp, NULL);

    for (UINT i = 0; i < BPP; i++)
    {
        UINT x = i % BPR;
        UINT y = i / BPR;

        RECT drawArea = {x * (BLOCK_W + HAL) + HAL, 
                         y * (BLOCK_H + VAL) + VAL,
                         x * (BLOCK_W + HAL) + HAL + BLOCK_W, 
                         y * (BLOCK_H + VAL) + VAL + BLOCK_H};

        if (i + bufOffset == copyIndex)
        {
            HPEN defaultPen = SelectObject(hdc, hPen);
            Rectangle(hdc, drawArea.left, drawArea.top, 
                      drawArea.right, drawArea.bottom);
            SelectObject(hdc, defaultPen);
        }
        else
        {
            Rectangle(hdc, drawArea.left, drawArea.top, 
                      drawArea.right, drawArea.bottom);
        }

        UINT format = RecordGetFormat(BufferGet(i + bufOffset, buffer, NULL));
        BYTE *data = RecordGetData(BufferGet(i + bufOffset, buffer, NULL));

        switch (format)
        {
            case CF_UNICODETEXT:
                DisplayText(hdc, &drawArea, data);
                break;

            case CF_DIB:
                DisplayBMP(hdc, &drawArea, data);
                break;

            case CF_HDROP:
                DisplayFiles(hdc, &drawArea, data);
                break;
        }
    }

    EndPaint(hwnd, &ps);

    // Clean up the memory
    DeleteObject(hFont);
    DeleteObject(hPen);
    DeleteDC(hdc);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    GDATA *gdata = (GDATA *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    GUIPARAMS *gp = GDataGet(GUI, gdata, NULL);
    BUFFER *buffer = GDataGet(BUF, gdata, NULL);
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
            mousewheel(hwnd, gp, wp);
            break;
        }
        case WM_VSCROLL:
        {
            vscroll(hwnd, gp, wp);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            rbuttondown(hwnd, gp, buffer, &copyIndex, wp, lp);
            break;
        }
        case WM_PAINT:
        {
            paint(hwnd, gp, buffer, copyIndex);
            break;
        }
        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}

HWND SetupMainWindow(GUIPARAMS *gp, HINSTANCE hInst, LPCWSTR winname)
{
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"mainWindow";
    wc.lpfnWndProc = WindowProc;

    RegisterClassW(&wc);

    UINT CLIENT_W = 0;
    UINT CLIENT_H = 0;
    UINT WINDOW_W = 0;
    UINT WINDOW_H = 0;
    CalculateClientWin(&CLIENT_W, &CLIENT_H, &WINDOW_W, &WINDOW_H);

    HWND window = CreateWindowW(L"mainWindow", winname, 
                                WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_VISIBLE | WS_VSCROLL, 
                                200, 200, WINDOW_W, WINDOW_H, 
                                NULL, NULL, NULL, NULL);

    SetScrollInfo(window, SB_VERT, GetGParamsScroll(gp, NULL), TRUE);
    SetGParamsCWidth(CLIENT_W, gp);
    SetGParamsCHeight(CLIENT_H, gp);
    SetGParamsBPR(3, gp);
    SetGParamsRPP(3, gp);
    SetGParamsBPP(9, gp);
    SetGParamsBHeight(CLIENT_H * 0.3, gp);
    SetGParamsBWidth(CLIENT_W * 0.3, gp);
    SetGParamsAlignH(CLIENT_W * 0.025, gp);
    SetGParamsAlignV(CLIENT_H * 0.025, gp);

    return window;
}