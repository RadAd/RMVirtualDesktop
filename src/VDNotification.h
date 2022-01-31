#pragma once

#include "Win10Desktops.h"

class IVDNotification
{
public:
    virtual void VirtualDesktopCreated(IVirtualDesktop* pDesktop) = 0;
    virtual void VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) = 0;
    virtual void CurrentVirtualDesktopChanged(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew) = 0;
};

class VirtualDesktopNotification : public IVirtualDesktopNotification
{
private:
    IVDNotification* _notify;
    ULONG _referenceCount;
public:
    VirtualDesktopNotification(IVDNotification* notify);

    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(DWORD) AddRef() override;
    STDMETHODIMP_(DWORD) STDMETHODCALLTYPE Release() override;

    STDMETHODIMP VirtualDesktopCreated(IVirtualDesktop* pDesktop) override;
    STDMETHODIMP VirtualDesktopDestroyBegin(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) override;
    STDMETHODIMP VirtualDesktopDestroyFailed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) override;
    STDMETHODIMP VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback) override;
    STDMETHODIMP ViewVirtualDesktopChanged(IApplicationView* pView) override;
    STDMETHODIMP CurrentVirtualDesktopChanged(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew) override;
};
