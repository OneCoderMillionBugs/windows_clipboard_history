#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include "buffer.h"
#include "guiparams.h"

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Shell32.lib")

// Create main window
HWND SetupMainWindow(GUIPARAMS *gp, HINSTANCE hInst, LPCWSTR winname);

#endif