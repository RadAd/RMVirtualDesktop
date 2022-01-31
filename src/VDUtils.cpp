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

const CComPtr<IServiceProvider>& GetImmersiveShell(LogF* pLog, void* logdata)
{
    static CComPtr<IServiceProvider> pServiceProvider;
    if (!pServiceProvider)
        LogHR(pLog, logdata, pServiceProvider.CoCreateInstance(CLSID_ImmersiveShell, NULL, CLSCTX_LOCAL_SERVER), L"creating CLSID_ImmersiveShell");
    return pServiceProvider;
}

const CComPtr<IVirtualDesktopNotificationService>& GetVirtualNotificationService(LogF* pLog, void* logdata)
{
    static CComPtr<IVirtualDesktopNotificationService> pDesktopNotificationService;
    if (!pDesktopNotificationService)
    {
        const CComPtr<IServiceProvider>& pServiceProvider = GetImmersiveShell(pLog, logdata);
        LogHR(pLog, logdata, pServiceProvider->QueryService(CLSID_IVirtualNotificationService, &pDesktopNotificationService), L"creating CLSID_IVirtualNotificationService");
    }
    return pDesktopNotificationService;
}

const CComPtr<IVirtualDesktopManagerInternal>& GetDesktopManagerInternal(LogF* pLog, void* logdata)
{
    static CComPtr<IVirtualDesktopManagerInternal> pDesktopManagerInternal;;
    if (!pDesktopManagerInternal)
    {
        const CComPtr<IServiceProvider>& pServiceProvider = GetImmersiveShell(pLog, logdata);
        LogHR(pLog, logdata, pServiceProvider->QueryService(CLSID_VirtualDesktopManagerInternal, &pDesktopManagerInternal), L"obtaining CLSID_VirtualDesktopManagerInternal");
    }
    return pDesktopManagerInternal;
}

CComPtr<IVirtualDesktop> GetCurrentDesktop(LogF* pLog, void* logdata)
{
    CComPtr<IVirtualDesktop> pCurrentDesktop;;
    const CComPtr<IVirtualDesktopManagerInternal>& pDesktopManagerInternal = GetDesktopManagerInternal(pLog, logdata);
    LogHR(pLog, logdata, pDesktopManagerInternal->GetCurrentDesktop(&pCurrentDesktop), L"GetCurrentDesktop");
    return pCurrentDesktop;
}

static CComPtr<IVirtualDesktop> GetAdjacentDesktop(LogF* pLog, void* logdata, AdjacentDesktop uDirection)
{
    const CComPtr<IVirtualDesktopManagerInternal>& pDesktopManagerInternal = GetDesktopManagerInternal(pLog, logdata);

    CComPtr<IVirtualDesktop> pDesktop = GetCurrentDesktop(pLog, logdata);

    CComPtr<IVirtualDesktop> pAdjacentDesktop;
    LogHR(pLog, logdata, pDesktopManagerInternal->GetAdjacentDesktop(pDesktop, uDirection, &pAdjacentDesktop), TYPE_E_OUTOFBOUNDS, L"GetAdjacentDesktop");

    return pAdjacentDesktop;
}

void SwitchDesktop(LogF* pLog, void* logdata, const CComPtr<IVirtualDesktop>& pDesktop)
{
    const CComPtr<IVirtualDesktopManagerInternal>& pDesktopManagerInternal = GetDesktopManagerInternal(pLog, logdata);
    LogHR(pLog, logdata, pDesktopManagerInternal->SwitchDesktop(pDesktop), L"SwitchDesktop");
}

int GetDesktopCount(LogF* pLog, void* logdata)
{
    const CComPtr<IVirtualDesktopManagerInternal>& pDesktopManagerInternal = GetDesktopManagerInternal(pLog, logdata);
    CComPtr<IObjectArray> pDesktopArray;
    if (pDesktopManagerInternal && SUCCEEDED(pDesktopManagerInternal->GetDesktops(&pDesktopArray)))
    {
        UINT count;
        if (!LogHR(pLog, logdata, pDesktopArray->GetCount(&count), L"GetDesktopCount"))
            count = 0;
        return count;
    }
    return 0;
}

int GetDesktopNumber(LogF* pLog, void* logdata, const CComPtr<IVirtualDesktop>& pFindDesktop)
{
    int dn = -1;
    const CComPtr<IVirtualDesktopManagerInternal>& pDesktopManagerInternal = GetDesktopManagerInternal(pLog, logdata);
    CComPtr<IObjectArray> pDesktopArray;
    if (pDesktopManagerInternal && SUCCEEDED(pDesktopManagerInternal->GetDesktops(&pDesktopArray)))
    {
        for (CComPtr<IVirtualDesktop> pDesktop : ObjectArrayRange<IVirtualDesktop>(pDesktopArray))
        {
            ++dn;
            if (pDesktop.IsEqualObject(pFindDesktop))
            {
                return dn;
            }
        }
    }
    return dn;
}

std::wstring GetDesktopName(LogF* pLog, void* logdata, const CComPtr<IVirtualDesktop>& pDesktop)
{
    std::wstring ret;

    CComPtr<IVirtualDesktop2> pDesktop2;
    if (SUCCEEDED(pDesktop.QueryInterface(&pDesktop2)))
    {
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
            _snwprintf_s(buffer, _TRUNCATE, L"Desktop %d", GetDesktopNumber(pLog, logdata, pDesktop) + 1);
            ret = buffer;
        }
    }

    return ret;
}

CComPtr<IVirtualDesktop> GetDesktop(LogF* pLog, void* logdata, int d)
{
    const CComPtr<IVirtualDesktopManagerInternal>& pDesktopManagerInternal = GetDesktopManagerInternal(pLog, logdata);
    CComPtr<IObjectArray> pDesktopArray;
    if (pDesktopManagerInternal && SUCCEEDED(pDesktopManagerInternal->GetDesktops(&pDesktopArray)))
    {
        CComPtr<IVirtualDesktop> pDesktop;
        LogHR(pLog, logdata, pDesktopArray->GetAt(d, IID_PPV_ARGS(&pDesktop)), E_INVALIDARG, L"IObjectArray GetAt");
        return pDesktop;
    }
    else
        return {};
}

CComPtr<IVirtualDesktop> GetDesktopNext(LogF* pLog, void* logdata)
{
    return GetAdjacentDesktop(pLog, logdata, AdjacentDesktop::RightDirection);
}

CComPtr<IVirtualDesktop> GetDesktopPrev(LogF* pLog, void* logdata)
{
    return GetAdjacentDesktop(pLog, logdata, AdjacentDesktop::LeftDirection);
}

void CreateDesktop(LogF* pLog, void* logdata)
{
    const CComPtr<IVirtualDesktopManagerInternal>& pDesktopManagerInternal = GetDesktopManagerInternal(pLog, logdata);
    CComPtr<IVirtualDesktop> pDesktop;
    LogHR(pLog, logdata, pDesktopManagerInternal->CreateDesktop(&pDesktop), L"CreateDesktop");
    if (pDesktop)
        SwitchDesktop(pLog, logdata, pDesktop);
}

void RemoveDesktop(LogF* pLog, void* logdata, const CComPtr<IVirtualDesktop>& pDesktop)
{
    const CComPtr<IVirtualDesktopManagerInternal>& pDesktopManagerInternal = GetDesktopManagerInternal(pLog, logdata);
    CComPtr<IVirtualDesktop> pFallbackDesktop = GetCurrentDesktop(pLog, logdata);
    if (!pFallbackDesktop || pFallbackDesktop.IsEqualObject(pDesktop))
    {
        pFallbackDesktop = GetAdjacentDesktop(pLog, logdata, AdjacentDesktop::RightDirection);
        if (!pFallbackDesktop)
            pFallbackDesktop = GetAdjacentDesktop(pLog, logdata, AdjacentDesktop::LeftDirection);
    }
    if (pFallbackDesktop)
        LogHR(pLog, logdata, pDesktopManagerInternal->RemoveDesktop(pDesktop, pFallbackDesktop), L"RemoveDesktop");
}
