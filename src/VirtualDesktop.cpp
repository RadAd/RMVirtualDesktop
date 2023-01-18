#include <Windows.h>
#include <RainmeterAPI.h>
#include <string>
#include <cassert>
#include <atlbase.h>

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
                RmLogF(rm, LOG_DEBUG, L"Next");
                SwitchDesktop(RmLogF, rm, RightDirection);
            }
            else if (_wcsicmp(argv[0], L"Prev") == 0)
            {
                RmLogF(rm, LOG_DEBUG, L"Prev");
                SwitchDesktop(RmLogF, rm, LeftDirection);
            }
            else if (_wcsicmp(argv[0], L"Create") == 0)
            {
                RmLogF(rm, LOG_DEBUG, L"Create");
                CreateDesktop(RmLogF, rm);
            }
            else if (_wcsicmp(argv[0], L"Remove") == 0)
            {
                if (argc > 1)
                {
                    int d = _wtoi(argv[1]);
                    RmLogF(rm, LOG_DEBUG, L"Remove %d", d);
                    RemoveDesktop(RmLogF, rm, d);
                }
                else
                    RemoveDesktop(RmLogF, rm, -1);
            }
            else if (_wcsicmp(argv[0], L"Switch") == 0)
            {
                if (argc > 1)
                {
                    int d = _wtoi(argv[1]);
                    RmLogF(rm, LOG_DEBUG, L"Switch %d", d);
                    SwitchDesktop(RmLogF, rm, d);
                }
                else
                    RmLogF(rm, LOG_ERROR, L"Arg expected \"%s\"", argv[0]);
            }
            else if (_wcsicmp(argv[0], L"SetName") == 0)
            {
                if (argc > 2)
                {
                    int d = _wtoi(argv[1]);
                    LPCWSTR name = argv[2];
                    RmLogF(rm, LOG_DEBUG, L"SetName %d \"%s\"", d, name);
                    SetDesktopName(RmLogF, rm, d, name);
                }
                else
                    RmLogF(rm, LOG_ERROR, L"Args expected \"%s\"", argv[0]);
            }
            else if (_wcsicmp(argv[0], L"SetWallpaper") == 0)
            {
                if (argc > 2)
                {
                    int d = _wtoi(argv[1]);
                    LPCWSTR wallpaper = argv[2];
                    RmLogF(rm, LOG_DEBUG, L"SetWallpaper %d \"%s\"", d, wallpaper);
                    SetDesktopWallpaper(RmLogF, rm, d, wallpaper);
                }
                else
                    RmLogF(rm, LOG_ERROR, L"Args expected \"%s\"", argv[0]);
            }
            else
                RmLogF(rm, LOG_ERROR, L"Invalid Bang \"%s\"", argv[0]);
        }
    }

    LPCWSTR Name(const int argc, const LPWSTR* argv) const
    {
        if (argc == 0)
        {
            buffer.clear();
            buffer = GetDesktopName(RmLogF, rm, -1);
            RmLogF(rm, LOG_DEBUG, L"Name \"%s\"", buffer.c_str());
            //return buffer.empty() ? nullptr : buffer.c_str();
            return buffer.c_str();
        }
        else if (argc == 1)
        {
            buffer.clear();
            int d = _wtoi(argv[0]);
            buffer = GetDesktopName(RmLogF, rm, d);
            RmLogF(rm, LOG_DEBUG, L"Name %d ret \"%s\"", d, buffer.c_str());
            //return buffer.empty() ? nullptr : buffer.c_str();
            return buffer.c_str();
        }
        else
            return nullptr;
    }

    LPCWSTR Wallpaper(const int argc, const LPWSTR* argv) const
    {
        if (argc == 0)
        {
            buffer.clear();
            buffer = GetDesktopWallpaper(RmLogF, rm, -1);
            RmLogF(rm, LOG_DEBUG, L"Wallpaper ret \"%s\"", buffer.c_str());
            //return buffer.empty() ? nullptr : buffer.c_str();
            return buffer.c_str();
        }
        else if (argc == 1)
        {
            buffer.clear();
            int d = _wtoi(argv[0]);
            buffer = GetDesktopWallpaper(RmLogF, rm, d);
            RmLogF(rm, LOG_DEBUG, L"Wallpaper %d ret \"%s\"", d, buffer.c_str());
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
    CComPtr<VirtualDesktopNotification> pNotify;

private:
    virtual void VirtualDesktopCreated(Win10::IVirtualDesktop* pDesktop) override
    {
        if (type == MeasureType::COUNT)
        {
            Update();
            UpdateMeasure();
        }
    }

    virtual void VirtualDesktopDestroyed(Win10::IVirtualDesktop* pDesktopDestroyed, Win10::IVirtualDesktop* pDesktopFallback) override
    {
        if (type == MeasureType::COUNT)
        {
            Update();
            UpdateMeasure();
        }
    }

    virtual void CurrentVirtualDesktopChanged(Win10::IVirtualDesktop* pDesktopOld, Win10::IVirtualDesktop* pDesktopNew) override
    {
        if (type == MeasureType::CURRENT)
        {
            assert(IsCurrentDesktop(RmLogF, rm, pDesktopNew));
            UpdateCurrent(pDesktopNew);
            UpdateMeasure();
        }
    }

    virtual void VirtualDesktopNameChanged(Win10::IVirtualDesktop* pDesktop, HSTRING name) override
    {
        if (type == MeasureType::CURRENT && IsCurrentDesktop(RmLogF, rm, pDesktop))
        {
            UpdateCurrent(pDesktop);
            UpdateMeasure();
        }
    }

    virtual void VirtualDesktopCreated(Win11::IVirtualDesktop* pDesktop) override
    {
        if (type == MeasureType::COUNT)
        {
            Update();
            UpdateMeasure();
        }
    }

    virtual void VirtualDesktopDestroyed(Win11::IVirtualDesktop* pDesktopDestroyed, Win11::IVirtualDesktop* pDesktopFallback) override
    {
        if (type == MeasureType::COUNT)
        {
            Update();
            UpdateMeasure();
        }
    }

    virtual void VirtualDesktopMoved(Win11::IVirtualDesktop* pDesktop, int64_t oldIndex, int64_t newIndex) override
    {
        // TODO
    }

    virtual void VirtualDesktopNameChanged(Win11::IVirtualDesktop* pDesktop, HSTRING name) override
    {
        if (type == MeasureType::CURRENT && IsCurrentDesktop(RmLogF, rm, pDesktop))
        {
            UpdateCurrent(pDesktop);
            UpdateMeasure();
        }
    }

    virtual void CurrentVirtualDesktopChanged(Win11::IVirtualDesktop* pDesktopOld, Win11::IVirtualDesktop* pDesktopNew) override
    {
        if (type == MeasureType::CURRENT)
        {
            assert(IsCurrentDesktop(RmLogF, rm, pDesktopNew));
            UpdateCurrent(pDesktopNew);
            UpdateMeasure();
        }
    }

private:
    void RegisterForNotifications()
    {
        if (idVirtualDesktopNotification == 0)
        {
            pNotify = new VirtualDesktopNotification(this);
            idVirtualDesktopNotification = Register(RmLogF, rm, pNotify);
        }
    }

    void UnregisterForNotifications()
    {
        if (idVirtualDesktopNotification != 0)
        {
            Unregister(RmLogF, rm, idVirtualDesktopNotification);
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
            //UpdateCurrent(GetCurrentDesktop10(RmLogF, rm));
            value = GetCurrentDesktopNumber(RmLogF, rm);
            string = GetDesktopName(RmLogF, rm, value);
            break;
        }
    }

    void UpdateCurrent(const CComPtr<Win10::IVirtualDesktop>& pDesktop)
    {
        assert(type == MeasureType::CURRENT);
        value = GetDesktopNumber(RmLogF, rm, pDesktop);
        string = GetDesktopName(RmLogF, rm, value);
    }

    void UpdateCurrent(const CComPtr<Win11::IVirtualDesktop>& pDesktop)
    {
        assert(type == MeasureType::CURRENT);
        value = GetDesktopNumber(RmLogF, rm, pDesktop);
        string = GetDesktopName(RmLogF, rm, value);
    }

    void UpdateMeasure() const
    {
        WCHAR buffer[128];
        _snwprintf_s(buffer, _TRUNCATE, L"!UpdateMeasure \"%s\"", RmGetMeasureName(rm));
        RmExecute(skin, buffer);
        RmLog(rm, LOG_DEBUG, buffer);
    }
};

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{

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

PLUGIN_EXPORT LPCWSTR Wallpaper(void* data, const int argc, const LPWSTR argv[])
{
    const Measure* measure = (Measure*) data;
    return measure->Wallpaper(argc, argv);
}

PLUGIN_EXPORT void Finalize(void* data)
{
    const Measure* measure = (Measure*)data;
    delete measure;
    measure = nullptr;
}
