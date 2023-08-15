#ifndef LISTENER_H
#define LISTENER_H

#include <windows.h>
#include <time.h>
#include <wchar.h>
#include "gui.h"

// Function for monitoring clipboard changes
DWORD WINAPI MonitorClip(LPVOID lpParam);

#endif