#include "VDNotification.h"

VirtualDesktopNotification::VirtualDesktopNotification(IVDNotification* notify)
    : _notify(notify), _referenceCount(0)
{
}

STDMETHODIMP VirtualDesktopNotification::QueryInterface(REFIID riid, void** ppvObject)
{
    // Always set out parameter to NULL, validating it first.
    if (!ppvObject)
        return E_INVALIDARG;
    *ppvObject = NULL;

    if (riid == IID_IUnknown || riid == Win10::IID_IVirtualDesktopNotification)
    {
        // Increment the reference count and return the pointer.
        *ppvObject = (LPVOID) static_cast<Win10::IVirtualDesktopNotification*>(this);
        AddRef();
        return S_OK;
    }
    else if (riid == Win11::IID_IVirtualDesktopNotification)
    {
        // Increment the reference count and return the pointer.
        *ppvObject = (LPVOID) static_cast<Win11::IVirtualDesktopNotification*>(this);
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

STDMETHODIMP_(DWORD) VirtualDesktopNotification::AddRef()
{
    return InterlockedIncrement(&_referenceCount);
}

STDMETHODIMP_(DWORD) STDMETHODCALLTYPE VirtualDesktopNotification::Release()
{
    ULONG result = InterlockedDecrement(&_referenceCount);
    if (result == 0)
    {
        delete this;
    }
    return 0;
}

// Win10::IVirtualDesktopNotification

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopCreated(Win10::IVirtualDesktop* pDesktop)
{
    _notify->VirtualDesktopCreated(pDesktop);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopDestroyBegin(Win10::IVirtualDesktop* pDesktopDestroyed, Win10::IVirtualDesktop* pDesktopFallback)
{
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopDestroyFailed(Win10::IVirtualDesktop* pDesktopDestroyed, Win10::IVirtualDesktop* pDesktopFallback)
{
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopDestroyed(Win10::IVirtualDesktop* pDesktopDestroyed, Win10::IVirtualDesktop* pDesktopFallback)
{
    _notify->VirtualDesktopDestroyed(pDesktopDestroyed, pDesktopFallback);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::ViewVirtualDesktopChanged(IApplicationView* pView)
{
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::CurrentVirtualDesktopChanged(Win10::IVirtualDesktop* pDesktopOld, Win10::IVirtualDesktop* pDesktopNew)
{
    _notify->CurrentVirtualDesktopChanged(pDesktopOld, pDesktopNew);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopNameChanged(Win10::IVirtualDesktop* pDesktop, HSTRING name)
{
    _notify->VirtualDesktopNameChanged(pDesktop, name);
    return S_OK;
}

// Win11::IVirtualDesktopNotification

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopCreated(Win11::IVirtualDesktop* pDesktop)
{
    _notify->VirtualDesktopCreated(pDesktop);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopDestroyBegin(Win11::IVirtualDesktop* pDesktopDestroyed, Win11::IVirtualDesktop* pDesktopFallback)
{
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopDestroyFailed(Win11::IVirtualDesktop* pDesktopDestroyed, Win11::IVirtualDesktop* pDesktopFallback)
{
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopDestroyed(Win11::IVirtualDesktop* pDesktopDestroyed, Win11::IVirtualDesktop* pDesktopFallback)
{
    _notify->VirtualDesktopDestroyed(pDesktopDestroyed, pDesktopFallback);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopMoved(Win11::IVirtualDesktop* pDesktop, int64_t oldIndex, int64_t newIndex)
{
    _notify->VirtualDesktopMoved(pDesktop, oldIndex, newIndex);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopNameChanged11(Win11::IVirtualDesktop* pDesktop, HSTRING name)
{
    _notify->VirtualDesktopNameChanged(pDesktop, name);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::ViewVirtualDesktopChanged11(IApplicationView* pView)
{
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::CurrentVirtualDesktopChanged(Win11::IVirtualDesktop* pDesktopOld, Win11::IVirtualDesktop* pDesktopNew)
{
    _notify->CurrentVirtualDesktopChanged(pDesktopOld, pDesktopNew);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopWallpaperChanged(Win11::IVirtualDesktop* pDesktop, HSTRING name)
{
    _notify->VirtualDesktopWallpaperChanged(pDesktop, name);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::VirtualDesktopSwitched(Win11::IVirtualDesktop* pDesktop, enum VirtualDesktopSwitchType type)
{
    _notify->VirtualDesktopSwitched(pDesktop, type);
    return S_OK;
}

STDMETHODIMP VirtualDesktopNotification::RemoteVirtualDesktopConnected(Win11::IVirtualDesktop* pDesktop)
{
    _notify->RemoteVirtualDesktopConnected(pDesktop);
    return S_OK;
}
