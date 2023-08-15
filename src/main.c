#include "gui.h"
#include "global.h"
#include "listener.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    // GUI parameters
    GUIPARAMS *gp = GParamsCreate();

    // Create window
    MSG msg = {0};
    HWND window = SetupMainWindow(gp, hInst, L"Clipboard History");

    // Set global data
    GDATA *gdata = GlobalDataInit(gp, NULL);
    SetWindowLongPtrW(window, GWLP_USERDATA, (LONG_PTR)gdata);

    // Create clipboard listener
    HANDLE hThread = CreateThread(NULL, 0, MonitorClip, gdata, 0, NULL);

    // Application loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (GetGParamsUpdate(gp, NULL))
        {
            InvalidateRect(window, NULL, TRUE);
            SetGParamsUpdate(FALSE, gp);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CloseHandle(hThread);
    return 0;
}