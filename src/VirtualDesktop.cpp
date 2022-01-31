#include <Windows.h>
#include <RainmeterAPI.h>
#include <string>
#include <cassert>

#include "VDUtils.h"
#include "VDNotification.h"

// For more information, see the SDK docs: https://docs.rainmeter.net/developers/plugin/cpp/

enum class MeasureType
{
    CURRENT,
    COUNT,
};

class Measure : private IVDNotification
{
public:
    Measure(void* rm)
        : rm(rm)
        , skin(RmGetSkin(rm))
    {
        RegisterForNotifications();
    }

    ~Measure()
    {
        UnregisterForNotifications();
    }

    void Reload(void* rm, double* maxValue)
    {
        assert(this->rm == rm);
        this->rm = rm;

        LPCWSTR value = RmReadString(rm, L"Type", L"Current");
        if (_wcsicmp(value, L"Current") == 0)
        {
            type = MeasureType::CURRENT;
        }
        else if (_wcsicmp(value, L"Count") == 0)
        {
            type = MeasureType::COUNT;
        }
        else
        {
            RmLogF(rm, LOG_ERROR, L"Invalid \"Type\" %s", value);
        }

        Update();

        //*maxValue = GetMax();
    }

    double Update() const
    {
        return value;
    }

    LPCWSTR GetString() const
    {
        if (string.empty())
            return nullptr;
        else
            return string.c_str();
    }

    void ExecuteBang(const int argc, const LPWSTR* argv) const
    {
        if (argc > 0)
        {
            if (_wcsicmp(argv[0], L"Next") == 0)
            {
                CComPtr<IVirtualDesktop> pDesktop = GetDesktopNext(RmLogF, rm);
                if (pDesktop)
                    SwitchDesktop(RmLogF, rm, pDesktop);
            }
            else if (_wcsicmp(argv[0], L"Prev") == 0)
            {
                CComPtr<IVirtualDesktop> pDesktop = GetDesktopPrev(RmLogF, rm);
                if (pDesktop)
                    SwitchDesktop(RmLogF, rm, pDesktop);
            }
            else if (_wcsicmp(argv[0], L"Create") == 0)
                CreateDesktop(RmLogF, rm);
            else if (_wcsicmp(argv[0], L"Remove") == 0)
            {
                if (argc > 1)
                {
                    int d = _wtoi(argv[1]);
                    CComPtr<IVirtualDesktop> pDesktop = GetDesktop(RmLogF, rm, d);
                    if (pDesktop)
                        RemoveDesktop(RmLogF, rm, pDesktop);
                }
                else
                    RemoveDesktop(RmLogF, rm, GetCurrentDesktop(RmLogF, rm));
            }
            else if (_wcsicmp(argv[0], L"Switch") == 0)
            {
                if (argc > 1)
                {
                    int d = _wtoi(argv[1]);
                    CComPtr<IVirtualDesktop> pDesktop = GetDesktop(RmLogF, rm, d);
                    if (pDesktop)
                        SwitchDesktop(RmLogF, rm, pDesktop);
                }
                else
                    RmLogF(rm, LOG_ERROR, L"Arg expected \"%s\"", argv[0]);
            }
            else
                RmLogF(rm, LOG_ERROR, L"Invalid Bang \"%s\"", argv[0]);
        }
    }

    LPCWSTR Name(const int argc, const LPWSTR* argv) const
    {
        if (argc == 1)
        {
            buffer.clear();
            int d = _wtoi(argv[0]);
            CComPtr<IVirtualDesktop> pDesktop = GetDesktop(RmLogF, rm, d);
            if (pDesktop)
                buffer = GetDesktopName(RmLogF, rm, pDesktop);
            //return buffer.empty() ? nullptr : buffer.c_str();
            return buffer.c_str();
        }
        else
            return nullptr;
    }

private:
    void* rm;
    void* skin;
    MeasureType type = MeasureType::CURRENT;
    int value = 0;
    std::wstring string;
    mutable std::wstring buffer;

    DWORD idVirtualDesktopNotification = 0;
    CComPtr<IVirtualDesktopNotification> pNotify;

private:
    virtual void VirtualDesktopCreated(IVirtualDesktop* pDesktop) override
    {
        if (type == MeasureType::COUNT)
        {
            Update();
            UpdateMeasure();
        }
    }

    virtual void VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) override
    {
        if (type == MeasureType::COUNT)
        {
            Update();
            UpdateMeasure();
        }
    }

    virtual void CurrentVirtualDesktopChanged(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew) override
    {
        if (type == MeasureType::CURRENT)
        {
            assert(GetCurrentDesktop(RmLogF, rm).IsEqualObject(pDesktopNew));
            UpdateCurrent(pDesktopNew);
            UpdateMeasure();
        }
    }

private:
    void RegisterForNotifications()
    {
        HRESULT hr;
        CComPtr<IVirtualDesktopNotificationService> pVirtualNotificationService = GetVirtualNotificationService(RmLogF, rm);

        pNotify = new VirtualDesktopNotification(this);
        if (idVirtualDesktopNotification == 0)
        {
            if (!pVirtualNotificationService || FAILED(hr = pVirtualNotificationService->Register(pNotify, &idVirtualDesktopNotification)))
                RmLogF(rm, LOG_ERROR, L"Error register DesktopNotificationService %d\n", hr);
        }
    }

    void UnregisterForNotifications()
    {
        HRESULT hr;
        CComPtr<IVirtualDesktopNotificationService> pVirtualNotificationService = GetVirtualNotificationService(RmLogF, rm);
        if (idVirtualDesktopNotification != 0)
        {
            if (!pVirtualNotificationService || FAILED(hr = pVirtualNotificationService->Unregister(idVirtualDesktopNotification)))
                OutputDebugString(L"RADVDDB: Error register DesktopNotificationService\n");
            idVirtualDesktopNotification = 0;
        }
    }

    void Update()
    {
        switch (type)
        {
        case MeasureType::COUNT:
            value = GetDesktopCount(RmLogF, rm);
            string.clear();
            break;
        case MeasureType::CURRENT:
            UpdateCurrent(GetCurrentDesktop(RmLogF, rm));
            break;
        }
    }

    void UpdateCurrent(const CComPtr<IVirtualDesktop>& pDesktop)
    {
        assert(type == MeasureType::CURRENT);
        value = GetDesktopNumber(RmLogF, rm, pDesktop);
        string = GetDesktopName(RmLogF, rm, pDesktop);
    }

    void UpdateMeasure() const
    {
        WCHAR buffer[128];
        _snwprintf_s(buffer, _TRUNCATE, L"!UpdateMeasure \"%s\"", RmGetMeasureName(rm));
        RmExecute(skin, buffer);
    }
};

//static HRESULT g_coinit = S_OK;

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
#if 0
    if (FAILED(g_coinit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
        RmLogF(rm, LOG_ERROR, L"FAILED CoInitialize %d\n", g_coinit);
#endif

    Measure* measure = new Measure(rm);
    *data = measure;
}

PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue)
{
    Measure* measure = (Measure*)data;
    measure->Reload(rm, maxValue);
}

PLUGIN_EXPORT double Update(void* data)
{
    const Measure* measure = (Measure*) data;
    return measure->Update();
}

PLUGIN_EXPORT LPCWSTR GetString(void* data)
{
    const Measure* measure = (Measure*)data;
    return measure->GetString();
}

PLUGIN_EXPORT void ExecuteBang(void* data, LPCWSTR args)
{
    const Measure* measure = (Measure*) data;
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(args, &argc);
    measure->ExecuteBang(argc, argv);
    LocalFree(argv);
}

PLUGIN_EXPORT LPCWSTR Name(void* data, const int argc, const LPWSTR argv[])
{
    const Measure* measure = (Measure*) data;
    return measure->Name(argc, argv);
}

PLUGIN_EXPORT void Finalize(void* data)
{
    const Measure* measure = (Measure*)data;
    delete measure;
    measure = nullptr;

#if 0
    if (g_coinit == S_OK)
        CoUninitialize();
#endif
}
