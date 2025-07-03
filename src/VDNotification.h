#pragma once

#include "Win10Desktops.h"

class IVDNotification
{
public:
    // Win10::IVirtualDesktopNotification
    virtual void VirtualDesktopCreated(Win10::IVirtualDesktop* pDesktop) = 0;
    virtual void VirtualDesktopDestroyed(Win10::IVirtualDesktop* pDesktopDestroyed, Win10::IVirtualDesktop* pDesktopFallback) = 0;
    virtual void CurrentVirtualDesktopChanged(Win10::IVirtualDesktop* pDesktopOld, Win10::IVirtualDesktop* pDesktopNew) = 0;
    virtual void VirtualDesktopNameChanged(Win10::IVirtualDesktop* pDesktop, HSTRING name) = 0;

    // Win11::IVirtualDesktopNotification
    virtual void VirtualDesktopCreated(Win11::IVirtualDesktop* pDesktop) = 0;
    virtual void VirtualDesktopDestroyed(Win11::IVirtualDesktop* pDesktopDestroyed, Win11::IVirtualDesktop* pDesktopFallback) = 0;
    virtual void VirtualDesktopMoved(Win11::IVirtualDesktop* pDesktop, int64_t oldIndex, int64_t newIndex) = 0;
    virtual void VirtualDesktopNameChanged(Win11::IVirtualDesktop* pDesktop, HSTRING name) = 0;
    virtual void CurrentVirtualDesktopChanged(Win11::IVirtualDesktop* pDesktopOld, Win11::IVirtualDesktop* pDesktopNew) = 0;
    virtual void VirtualDesktopWallpaperChanged(Win11::IVirtualDesktop* pDesktop, HSTRING name) = 0;
    virtual void VirtualDesktopSwitched(Win11::IVirtualDesktop* pDesktop, enum VirtualDesktopSwitchType type) = 0;
    virtual void RemoteVirtualDesktopConnected(Win11::IVirtualDesktop* pDesktop) = 0;
};

class VirtualDesktopNotification : public Win10::IVirtualDesktopNotification2, public Win11::IVirtualDesktopNotification
{
private:
    IVDNotification* _notify;
    ULONG _referenceCount;
public:
    VirtualDesktopNotification(IVDNotification* notify);

    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(DWORD) AddRef() override;
    STDMETHODIMP_(DWORD) STDMETHODCALLTYPE Release() override;

    // Win10::IVirtualDesktopNotification
    STDMETHODIMP VirtualDesktopCreated(Win10::IVirtualDesktop* pDesktop) override;
    STDMETHODIMP VirtualDesktopDestroyBegin(Win10::IVirtualDesktop* pDesktopDestroyed, Win10::IVirtualDesktop* pDesktopFallback) override;
    STDMETHODIMP VirtualDesktopDestroyFailed(Win10::IVirtualDesktop* pDesktopDestroyed, Win10::IVirtualDesktop* pDesktopFallback) override;
    STDMETHODIMP VirtualDesktopDestroyed(Win10::IVirtualDesktop* pDesktopDestroyed, Win10::IVirtualDesktop* pDesktopFallback) override;
    STDMETHODIMP ViewVirtualDesktopChanged(IApplicationView* pView) override;
    STDMETHODIMP CurrentVirtualDesktopChanged(Win10::IVirtualDesktop* pDesktopOld, Win10::IVirtualDesktop* pDesktopNew) override;
    // Win10::IVirtualDesktopNotification2
    STDMETHODIMP VirtualDesktopNameChanged(Win10::IVirtualDesktop* pDesktop, HSTRING name) override;

    // Win11::IVirtualDesktopNotification
    STDMETHODIMP VirtualDesktopCreated(Win11::IVirtualDesktop* pDesktop) override;
    STDMETHODIMP VirtualDesktopDestroyBegin(Win11::IVirtualDesktop* pDesktopDestroyed, Win11::IVirtualDesktop* pDesktopFallback) override;
    STDMETHODIMP VirtualDesktopDestroyFailed(Win11::IVirtualDesktop* pDesktopDestroyed, Win11::IVirtualDesktop* pDesktopFallback) override;
    STDMETHODIMP VirtualDesktopDestroyed(Win11::IVirtualDesktop* pDesktopDestroyed, Win11::IVirtualDesktop* pDesktopFallback) override;
    STDMETHODIMP VirtualDesktopMoved(Win11::IVirtualDesktop* pDesktop, int64_t oldIndex, int64_t newIndex) override;
    STDMETHODIMP VirtualDesktopNameChanged11(Win11::IVirtualDesktop* pDesktop, HSTRING name) override;
    STDMETHODIMP ViewVirtualDesktopChanged11(IApplicationView* pView) override;
    STDMETHODIMP CurrentVirtualDesktopChanged(Win11::IVirtualDesktop* pDesktopOld, Win11::IVirtualDesktop* pDesktopNew) override;
    STDMETHODIMP VirtualDesktopWallpaperChanged(Win11::IVirtualDesktop* pDesktop, HSTRING name) override;
    STDMETHODIMP VirtualDesktopSwitched(Win11::IVirtualDesktop* pDesktop, enum VirtualDesktopSwitchType) override;
    STDMETHODIMP RemoteVirtualDesktopConnected(Win11::IVirtualDesktop* pDesktop) override;
};
