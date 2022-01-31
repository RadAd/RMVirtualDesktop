#pragma once

#include <atlbase.h>
#include <string>

#include "Win10Desktops.h"

typedef void __cdecl LogF(void* logdata, int level, LPCWSTR format, ...);

const CComPtr<IServiceProvider>& GetImmersiveShell(LogF* pLog, void* logdata);
const CComPtr<IVirtualDesktopNotificationService>& GetVirtualNotificationService(LogF* pLog, void* logdata);
const CComPtr<IVirtualDesktopManagerInternal>& GetDesktopManagerInternal(LogF* pLog, void* logdata);
CComPtr<IVirtualDesktop> GetCurrentDesktop(LogF* pLog, void* logdata);

int GetDesktopCount(LogF* pLog, void* logdata);
int GetDesktopNumber(LogF* pLog, void* logdata, const CComPtr<IVirtualDesktop>& pFindDesktop);
std::wstring GetDesktopName(LogF* pLog, void* logdata, const CComPtr<IVirtualDesktop>& pDesktop);
CComPtr<IVirtualDesktop> GetDesktop(LogF* pLog, void* logdata, int d);
void SwitchDesktop(LogF* pLog, void* logdata, const CComPtr<IVirtualDesktop>& pDesktop);

CComPtr<IVirtualDesktop> GetDesktopNext(LogF* pLog, void* logdata);
CComPtr<IVirtualDesktop> GetDesktopPrev(LogF* pLog, void* logdata);
void CreateDesktop(LogF* pLog, void* logdata);
void RemoveDesktop(LogF* pLog, void* logdata, const CComPtr<IVirtualDesktop>& pDesktop);
