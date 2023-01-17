#pragma once

#include <string>

#include "Win10Desktops.h"
#include "VDNotification.h"

typedef void __cdecl LogF(void* logdata, int level, LPCWSTR format, ...);

DWORD Register(LogF* pLog, void* logdata, VirtualDesktopNotification* pNotify);
void Unregister(LogF* pLog, void* logdata, DWORD idVirtualDesktopNotification);

int GetDesktopCount(LogF* pLog, void* logdata);
int GetCurrentDesktopNumber(LogF* pLog, void* logdata);
int GetDesktopNumber(LogF* pLog, void* logdata, Win10::IVirtualDesktop* pFindDesktop);
int GetDesktopNumber(LogF* pLog, void* logdata, Win11::IVirtualDesktop* pFindDesktop);
std::wstring GetDesktopName(LogF* pLog, void* logdata, int d);
std::wstring GetDesktopWallpaper(LogF* pLog, void* logdata, int d);
void SwitchDesktop(LogF* pLog, void* logdata, int d);
void SwitchDesktop(LogF* pLog, void* logdata, enum AdjacentDesktop direction);

void CreateDesktop(LogF* pLog, void* logdata);
void RemoveDesktop(LogF* pLog, void* logdata, int d);

bool IsCurrentDesktop(LogF* pLog, void* logdata, Win10::IVirtualDesktop* pDesktop);
bool IsCurrentDesktop(LogF* pLog, void* logdata, Win11::IVirtualDesktop* pDesktop);
