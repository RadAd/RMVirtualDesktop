#include "VDUtils.h"

#include "ComUtils.h"

#include <winstring.h>

static const int LOG_ERROR = 1;

static inline bool LogHR(LogF* pLog, void* logdata, HRESULT hr, LPCWSTR name)
{
    if (FAILED(hr))
    {
        if (pLog)
            pLog(logdata, LOG_ERROR, L"%s %#10.8x\n", name, hr);
        return false;
    }
    else
        return true;
}

static inline bool LogHR(LogF* pLog, void* logdata, HRESULT hr, HRESULT ignore, LPCWSTR name)
{
    if (FAILED(hr) && hr != ignore)
    {
        if (pLog)
            pLog(logdata, LOG_ERROR, L"%s %#10.8x\n", name, hr);
        return false;
    }
    else
        return true;
}

static const CComPtr<IServiceProvider>& GetImmersiveShell(LogF* pLog, void* logdata)
{
    static CComPtr<IServiceProvider> pServiceProvider;
    if (!pServiceProvider)
        LogHR(pLog, logdata, pServiceProvider.CoCreateInstance(CLSID_ImmersiveShell, NULL, CLSCTX_LOCAL_SERVER), L"creating CLSID_ImmersiveShell");
    return pServiceProvider;
}

template <class VDMI>
static const CComPtr<VDMI>& GetVirtualNotificationService(LogF* pLog, void* logdata)
{
    static CComPtr<VDMI> pDesktopNotificationService;
    if (!pDesktopNotificationService)
    {
        const CComPtr<IServiceProvider>& pServiceProvider = GetImmersiveShell(pLog, logdata);
        LogHR(pLog, logdata, pServiceProvider->QueryService(CLSID_VirtualNotificationService, &pDesktopNotificationService), L"creating CLSID_IVirtualNotificationService");
    }
    return pDesktopNotificationService;
}

template <class VDMI>
static const CComPtr<VDMI>& GetDesktopManagerInternal(LogF* pLog, void* logdata)
{
    static CComPtr<VDMI> pDesktopManagerInternal;
    if (!pDesktopManagerInternal)
    {
        const CComPtr<IServiceProvider>& pServiceProvider = GetImmersiveShell(pLog, logdata);
        //LogHR(pLog, logdata, pServiceProvider->QueryService(CLSID_VirtualDesktopManagerInternal, &pDesktopManagerInternal), L"creating CLSID_VirtualDesktopManagerInternal");
        pServiceProvider->QueryService(CLSID_VirtualDesktopManagerInternal, &pDesktopManagerInternal);
    }
    return pDesktopManagerInternal;
}

DWORD Register(LogF* pLog, void* logdata, VirtualDesktopNotification* pNotify)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    DWORD idVirtualDesktopNotification = 0;
    if (pDesktopManagerInternal10)
    {
        const CComPtr<Win10::IVirtualDesktopNotificationService> pVirtualNotificationService = GetVirtualNotificationService<Win10::IVirtualDesktopNotificationService>(pLog, logdata);
        if (pVirtualNotificationService)
            LogHR(pLog, logdata, pVirtualNotificationService->Register(pNotify, &idVirtualDesktopNotification), L"Register DesktopNotificationService");
    }
    else if (pDesktopManagerInternal11)
    {
        const CComPtr<Win11::IVirtualDesktopNotificationService> pVirtualNotificationService = GetVirtualNotificationService<Win11::IVirtualDesktopNotificationService>(pLog, logdata);
        if (pVirtualNotificationService)
            LogHR(pLog, logdata, pVirtualNotificationService->Register(pNotify, &idVirtualDesktopNotification), L"Register DesktopNotificationService");
    }
    else if (pLog)
        pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");
    return idVirtualDesktopNotification;
}

void Unregister(LogF* pLog, void* logdata, DWORD idVirtualDesktopNotification)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
    {
        const CComPtr<Win10::IVirtualDesktopNotificationService> pVirtualNotificationService = GetVirtualNotificationService<Win10::IVirtualDesktopNotificationService>(pLog, logdata);
        if (pVirtualNotificationService)
            LogHR(pLog, logdata, pVirtualNotificationService->Unregister(idVirtualDesktopNotification), L"Unregister DesktopNotificationService");
    }
    else if (pDesktopManagerInternal11)
    {
        const CComPtr<Win11::IVirtualDesktopNotificationService> pVirtualNotificationService = GetVirtualNotificationService<Win11::IVirtualDesktopNotificationService>(pLog, logdata);
        if (pVirtualNotificationService)
            LogHR(pLog, logdata, pVirtualNotificationService->Unregister(idVirtualDesktopNotification), L"Unregister DesktopNotificationService");
    }
    else if (pLog)
        pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");
}

template <class VD, class VDMI>
CComPtr<VD> GetDesktop(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal, int d)
{
    CComPtr<IObjectArray> pDesktopArray;
    if (d == -1)
    {
        CComPtr<VD> pCurrentDesktop;
        LogHR(pLog, logdata, pDesktopManagerInternal->GetCurrentDesktop(&pCurrentDesktop), L"GetCurrentDesktop");
        return pCurrentDesktop;
    }
    else if (pDesktopManagerInternal && SUCCEEDED(pDesktopManagerInternal->GetDesktops(&pDesktopArray)))
    {
        CComPtr<VD> pDesktop;
        LogHR(pLog, logdata, pDesktopArray->GetAt(d - 1, IID_PPV_ARGS(&pDesktop)), E_INVALIDARG, L"IObjectArray GetAt");
        return pDesktop;
    }
    else
        return {};
}

template<class VD, class VDMI>
void SwitchDesktop(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal, int d)
{
    CComPtr<VD> pDesktop = GetDesktop<VD>(pLog, logdata, pDesktopManagerInternal, d);
    if (pDesktop)
        LogHR(pLog, logdata, SwitchDesktop(pDesktopManagerInternal, static_cast<VD*>(pDesktop), false), L"SwitchDesktop");
}

void SwitchDesktop(LogF* pLog, void* logdata, int d)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        SwitchDesktop<Win10::IVirtualDesktop>(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal10), d);
    else if (pDesktopManagerInternal11)
        SwitchDesktop<Win11::IVirtualDesktop>(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11), d);
    else if (pLog)
        pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");
}

template<class VD, class VDMI>
void SwitchDesktop(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal, enum AdjacentDesktop direction)
{
    CComPtr<VD> pCurrentDesktop;
    LogHR(pLog, logdata, pDesktopManagerInternal->GetCurrentDesktop(&pCurrentDesktop), L"GetCurrentDesktop");
    CComPtr<VD> pDesktop;
    LogHR(pLog, logdata, pDesktopManagerInternal->GetAdjacentDesktop(pCurrentDesktop, direction, &pDesktop), TYPE_E_OUTOFBOUNDS, L"GetAdjacentDesktop");
    if (pDesktop)
        LogHR(pLog, logdata, SwitchDesktop(pDesktopManagerInternal, static_cast<VD*>(pDesktop), false), L"SwitchDesktop");
}

void SwitchDesktop(LogF* pLog, void* logdata, enum AdjacentDesktop direction)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        SwitchDesktop<Win10::IVirtualDesktop>(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal10), direction);
    else if (pDesktopManagerInternal11)
        SwitchDesktop<Win11::IVirtualDesktop>(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11), direction);
    else if (pLog)
        pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");
}

template <class VDMI>
int GetDesktopCount(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal)
{
    CComPtr<IObjectArray> pDesktopArray;
    if (SUCCEEDED(pDesktopManagerInternal->GetDesktops(&pDesktopArray)))
    {
        UINT count;
        if (!LogHR(pLog, logdata, pDesktopArray->GetCount(&count), L"GetDesktopCount"))
            count = 0;
        return count;
    }
    else
        return 0;
}

int GetDesktopCount(LogF* pLog, void* logdata)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        return GetDesktopCount(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal10));
    else if (pDesktopManagerInternal11)
        return GetDesktopCount(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11));
    else
    {
        if (pLog)
            pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");
        return 0;
    }
}

template <class VDMI, class VD>
int GetDesktopNumber(VDMI* pDesktopManagerInternal, VD* pFindDesktop)
{
    int dn = 0;
    CComPtr<IObjectArray> pDesktopArray;
    if (pDesktopManagerInternal && SUCCEEDED(pDesktopManagerInternal->GetDesktops(&pDesktopArray)))
    {
        for (CComPtr<VD> pDesktop : ObjectArrayRange<VD>(pDesktopArray))
        {
            ++dn;
            if (pDesktop.IsEqualObject(pFindDesktop))
            {
                return dn;
            }
        }
    }
    return 0;
}

template <class VD, class VDMI>
int GetCurrentDesktopNumber(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal)
{
    CComPtr<VD> pCurrentDesktop;
    LogHR(pLog, logdata, pDesktopManagerInternal->GetCurrentDesktop(&pCurrentDesktop), L"GetCurrentDesktop");
    return GetDesktopNumber(pDesktopManagerInternal, static_cast<VD*>(pCurrentDesktop));
}

int GetCurrentDesktopNumber(LogF* pLog, void* logdata)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        return GetCurrentDesktopNumber<Win10::IVirtualDesktop>(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal10));
    else if (pDesktopManagerInternal11)
        return GetCurrentDesktopNumber<Win11::IVirtualDesktop>(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11));
    else
    {
        if (pLog)
            pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");
        return 0;
    }
}

int GetDesktopNumber(LogF* pLog, void* logdata, Win10::IVirtualDesktop* pFindDesktop)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    return GetDesktopNumber(static_cast<Win10::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal10), pFindDesktop);
}

int GetDesktopNumber(LogF* pLog, void* logdata, Win11::IVirtualDesktop* pFindDesktop)
{
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    return GetDesktopNumber(static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11), pFindDesktop);
}

template<class VD, class VD2 = VD, class VDMI>
std::wstring GetDesktopName(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal, int d)
{
    std::wstring ret;

    CComPtr<VD> pDesktop = GetDesktop<VD>(pLog, logdata, pDesktopManagerInternal, d);
    if (pDesktop)
    {
        CComQIPtr<VD2> pDesktop2 = pDesktop;

        HSTRING s = NULL;
        LogHR(pLog, logdata, pDesktop2->GetName(&s), L"GetDesktopName");

        if (s != NULL)
        {
            ret = WindowsGetStringRawBuffer(s, nullptr);

            WindowsDeleteString(s);
        }
        else
        {
            WCHAR buffer[128];
            _snwprintf_s(buffer, _TRUNCATE, L"Desktop %d", GetDesktopNumber(pDesktopManagerInternal, static_cast<VD*>(pDesktop)));
            ret = buffer;
        }
    }

    return ret;
}

std::wstring GetDesktopName(LogF* pLog, void* logdata, int d)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        return GetDesktopName<Win10::IVirtualDesktop, Win10::IVirtualDesktop2>(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal10), d);
    else if (pDesktopManagerInternal11)
        return GetDesktopName<Win11::IVirtualDesktop>(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11), d);
    else
    {
        if (pLog)
            pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");

        return {};
    }
}

template<class VD, class VDMI>
void SetDesktopName(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal, int d, LPCWSTR name)
{
    CComPtr<VD> pDesktop = GetDesktop<VD>(pLog, logdata, pDesktopManagerInternal, d);
    if (pDesktop)
    {
        HSTRING s = NULL;
        LogHR(pLog, logdata, WindowsCreateString(name, static_cast<UINT32>(wcslen(name)), &s), L"WindowsCreateString");
        LogHR(pLog, logdata, pDesktopManagerInternal->SetDesktopName(pDesktop , s), L"SetDesktopName");
        WindowsDeleteString(s);
    }
}

void SetDesktopName(LogF* pLog, void* logdata, int d, LPCWSTR name)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal2>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal2>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        return SetDesktopName<Win10::IVirtualDesktop>(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal2*>(pDesktopManagerInternal10), d, name);
    else if (pDesktopManagerInternal11)
        return SetDesktopName<Win11::IVirtualDesktop>(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11), d, name);
    else
    {
        if (pLog)
            pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");

        return;
    }
}

template<class VD, class VDMI>
std::wstring GetDesktopWallpaper(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal, int d)
{
    std::wstring ret;

    CComPtr<VD> pDesktop = GetDesktop<VD>(pLog, logdata, pDesktopManagerInternal, d);
    if (pDesktop)
    {
        HSTRING s = NULL;
        LogHR(pLog, logdata, pDesktop->GetWallpaperPath(&s), L"GetDesktopName");

        if (s != NULL)
        {
            ret = WindowsGetStringRawBuffer(s, nullptr);

            WindowsDeleteString(s);
        }
        else
        {
            WCHAR buffer[128];
            _snwprintf_s(buffer, _TRUNCATE, L"Desktop %d", GetDesktopNumber(pDesktopManagerInternal, static_cast<VD*>(pDesktop)));
            ret = buffer;
        }
    }

    return ret;
}

std::wstring GetDesktopWallpaper(LogF* pLog, void* logdata, int d)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        return {}; // GetDesktopWallpaper<Win10::IVirtualDesktop2>(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal10), d);
    else if (pDesktopManagerInternal11)
        return GetDesktopWallpaper<Win11::IVirtualDesktop>(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11), d);
    else
    {
        if (pLog)
            pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");

        return {};
    }
}

template<class VD, class VDMI>
void SetDesktopWallpaper(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal, int d, LPCWSTR wallpaper)
{
    CComPtr<VD> pDesktop = GetDesktop<VD>(pLog, logdata, pDesktopManagerInternal, d);
    if (pDesktop)
    {
        HSTRING s = NULL;
        LogHR(pLog, logdata, WindowsCreateString(wallpaper, static_cast<UINT32>(wcslen(wallpaper)), &s), L"WindowsCreateString");
        LogHR(pLog, logdata, pDesktopManagerInternal->SetDesktopWallpaper(pDesktop, s), L"SetDesktopWallpaper");
        WindowsDeleteString(s);
    }
}

void SetDesktopWallpaper(LogF* pLog, void* logdata, int d, LPCWSTR wallpaper)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal2>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal2>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        return; // SetDesktopWallpaper<Win10::IVirtualDesktop>(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal2*>(pDesktopManagerInternal10), d, wallpaper);
    else if (pDesktopManagerInternal11)
        return SetDesktopWallpaper<Win11::IVirtualDesktop>(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11), d, wallpaper);
    else
    {
        if (pLog)
            pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");

        return;
    }
}

template <class VD, class VDMI>
void CreateDesktop(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal)
{
    CComPtr<VD> pDesktop;
    LogHR(pLog, logdata, pDesktopManagerInternal->CreateDesktop(&pDesktop), L"CreateDesktop");
    if (pDesktop)
        LogHR(pLog, logdata, SwitchDesktop(pDesktopManagerInternal, static_cast<VD*>(pDesktop), false), L"SwitchDesktop");
}

void CreateDesktop(LogF* pLog, void* logdata)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        CreateDesktop<Win10::IVirtualDesktop>(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal10));
    else if (pDesktopManagerInternal11)
        CreateDesktop<Win11::IVirtualDesktop>(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11));
    else if (pLog)
        pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");
}

template <class VD, class VDMI>
void RemoveDesktop(LogF* pLog, void* logdata, VDMI* pDesktopManagerInternal, int d)
{
    CComPtr<VD> pCurrentDesktop;
    LogHR(pLog, logdata, pDesktopManagerInternal->GetCurrentDesktop(&pCurrentDesktop), L"GetCurrentDesktop");
    CComPtr<VD> pDesktop = d == -1 ? pCurrentDesktop : GetDesktop<VD>(pLog, logdata, pDesktopManagerInternal, d);
    CComPtr<VD> pFallbackDesktop = pCurrentDesktop;
    if (!pFallbackDesktop || pFallbackDesktop.IsEqualObject(pDesktop))
    {
        pFallbackDesktop.Release();
        LogHR(pLog, logdata, pDesktopManagerInternal->GetAdjacentDesktop(pCurrentDesktop, AdjacentDesktop::RightDirection, &pFallbackDesktop), TYPE_E_OUTOFBOUNDS, L"GetAdjacentDesktop");
        if (!pFallbackDesktop)
            LogHR(pLog, logdata, pDesktopManagerInternal->GetAdjacentDesktop(pCurrentDesktop, AdjacentDesktop::LeftDirection, &pFallbackDesktop), TYPE_E_OUTOFBOUNDS, L"GetAdjacentDesktop");
    }
    if (pFallbackDesktop)
        LogHR(pLog, logdata, pDesktopManagerInternal->RemoveDesktop(pDesktop, pFallbackDesktop), L"RemoveDesktop");
}

void RemoveDesktop(LogF* pLog, void* logdata, int d)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    if (pDesktopManagerInternal10)
        RemoveDesktop<Win10::IVirtualDesktop>(pLog, logdata, static_cast<Win10::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal10), d);
    else if (pDesktopManagerInternal11)
        RemoveDesktop<Win11::IVirtualDesktop>(pLog, logdata, static_cast<Win11::IVirtualDesktopManagerInternal*>(pDesktopManagerInternal11), d);
    else if (pLog)
        pLog(logdata, LOG_ERROR, L"GetDesktopManagerInternal\n");
}

bool IsCurrentDesktop(LogF* pLog, void* logdata, Win10::IVirtualDesktop* pDesktop)
{
    const CComPtr<Win10::IVirtualDesktopManagerInternal>& pDesktopManagerInternal10 = GetDesktopManagerInternal<Win10::IVirtualDesktopManagerInternal>(pLog, logdata);
    CComPtr<Win10::IVirtualDesktop> pCurrentDesktop;
    LogHR(pLog, logdata, pDesktopManagerInternal10->GetCurrentDesktop(&pCurrentDesktop), L"GetCurrentDesktop");
    return pCurrentDesktop.IsEqualObject(pDesktop);
}

bool IsCurrentDesktop(LogF* pLog, void* logdata, Win11::IVirtualDesktop* pDesktop)
{
    const CComPtr<Win11::IVirtualDesktopManagerInternal>& pDesktopManagerInternal11 = GetDesktopManagerInternal<Win11::IVirtualDesktopManagerInternal>(pLog, logdata);
    CComPtr<Win11::IVirtualDesktop> pCurrentDesktop;
    LogHR(pLog, logdata, pDesktopManagerInternal11->GetCurrentDesktop(&pCurrentDesktop), L"GetCurrentDesktop");
    return pCurrentDesktop.IsEqualObject(pDesktop);
}
