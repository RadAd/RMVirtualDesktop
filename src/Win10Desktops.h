// https://github.com/Ciantic/VirtualDesktopAccessor

// See for more up-to-date
// https://github.com/skottmckay/VirtualDesktopAccessor

#pragma once

//#include "framework.h"
#include <cstdint>
#include <inspectable.h>
#include <ObjectArray.h>

#define FC_USER_MARSHAL void

class DECLSPEC_UUID("C2F03A33-21F5-47FA-B4BB-156362A2F239") ImmersiveShell;
const CLSID CLSID_ImmersiveShell = __uuidof(ImmersiveShell);

class DECLSPEC_UUID("C5E0CDCA-7B6E-41B2-9FC4-D93975CC467B") VirtualDesktopManagerInternal;
const CLSID CLSID_VirtualDesktopManagerInternal = __uuidof(VirtualDesktopManagerInternal);

class DECLSPEC_UUID("A501FDEC-4A09-464C-AE4E-1B9C21B84918") VirtualNotificationService;
const CLSID CLSID_VirtualNotificationService = __uuidof(VirtualNotificationService);

class DECLSPEC_UUID("B5A399E7-1C87-46B8-88E9-FC5747B171BD") VirtualDesktopPinnedApps;
const CLSID CLSID_VirtualDesktopPinnedApps = __uuidof(VirtualDesktopPinnedApps);

// Ignore following API's:
interface IAsyncCallback;
interface IImmersiveMonitor;
interface APPLICATION_VIEW_COMPATIBILITY_POLICY;
interface IShellPositionerPriority;
interface IApplicationViewOperation;
interface APPLICATION_VIEW_CLOAK_TYPE;
interface IApplicationViewPosition;
interface IImmersiveApplication;
interface IApplicationViewChangeListener;


DECLARE_INTERFACE_IID_(IApplicationView, IInspectable, "372E1D3B-38D3-42E4-A15B-8AB2B178F513")
{
    STDMETHOD(SetFocus)(THIS) PURE; // Proc6
    STDMETHOD(SwitchTo)(THIS) PURE; // Proc7
    STDMETHOD(TryInvokeBack)(THIS_ IAsyncCallback*) PURE; // Proc8
    STDMETHOD(GetThumbnailWindow)(THIS_ HWND*) PURE; // Proc9
    STDMETHOD(GetMonitor)(THIS_ IImmersiveMonitor**) PURE; // Proc10
    STDMETHOD(GetVisibility)(THIS_ int*) PURE; // Proc11
    STDMETHOD(SetCloak)(THIS_ APPLICATION_VIEW_CLOAK_TYPE, int) PURE; // Proc12
    STDMETHOD(GetPosition)(THIS_ REFIID, void**) PURE; // Proc13
    STDMETHOD(SetPosition)(THIS_ IApplicationViewPosition*) PURE; // Proc14
    STDMETHOD(InsertAfterWindow)(THIS_ HWND) PURE; // Proc15
    STDMETHOD(GetExtendedFramePosition)(THIS_ RECT*) PURE; // Proc16
    STDMETHOD(GetAppUserModelId)(THIS_ PWSTR*) PURE; // Proc17
    STDMETHOD(SetAppUserModelId)(THIS_ PCWSTR) PURE; // Proc18
    STDMETHOD(IsEqualByAppUserModelId)(THIS_ PCWSTR, int*) PURE; // Proc19
    STDMETHOD(GetViewState)(THIS_ UINT*) PURE; // Proc20
    STDMETHOD(SetViewState)(THIS_ UINT) PURE; // Proc21
    STDMETHOD(GetNeediness)(THIS_ int*) PURE; // Proc22
    STDMETHOD(GetLastActivationTimestamp)(THIS_ ULONGLONG*) PURE; // Proc23
    STDMETHOD(SetLastActivationTimestamp)(THIS_ ULONGLONG) PURE; // Proc24
    STDMETHOD(GetVirtualDesktopId)(THIS_ GUID*) PURE; // Proc25
    STDMETHOD(SetVirtualDesktopId)(THIS_ REFGUID) PURE; // Proc26
    STDMETHOD(GetShowInSwitchers)(THIS_ int*) PURE; // Proc27
    STDMETHOD(SetShowInSwitchers)(THIS_ int) PURE; // Proc28
    STDMETHOD(GetScaleFactor)(THIS_ int*) PURE; // Proc29
    STDMETHOD(CanReceiveInput)(THIS_ BOOL*) PURE; // Proc30
    STDMETHOD(GetCompatibilityPolicyType)(THIS_ APPLICATION_VIEW_COMPATIBILITY_POLICY*) PURE; // Proc31
    STDMETHOD(SetCompatibilityPolicyType)(THIS_ APPLICATION_VIEW_COMPATIBILITY_POLICY) PURE; // Proc32
    //STDMETHOD(GetPositionPriority)(THIS_ IShellPositionerPriority**) PURE; // removed in 1803 // Proc33
    //STDMETHOD(SetPositionPriority)(THIS_ IShellPositionerPriority*) PURE; // removed in 1803 // Proc34
    STDMETHOD(GetSizeConstraints)(THIS_ IImmersiveMonitor*, SIZE*, SIZE*) PURE; // Proc35
    STDMETHOD(GetSizeConstraintsForDpi)(THIS_ UINT, SIZE*, SIZE*) PURE; // Proc36
    STDMETHOD(SetSizeConstraintsForDpi)(THIS_ const UINT*, const SIZE*, const SIZE*) PURE; // Proc37
    //STDMETHOD(QuerySizeConstraintsFromApp)(THIS) PURE; // removed in 1803 // Proc38
    STDMETHOD(OnMinSizePreferencesUpdated)(THIS_ HWND) PURE; // Proc39
    STDMETHOD(ApplyOperation)(THIS_ IApplicationViewOperation*) PURE; // Proc40
    STDMETHOD(IsTray)(THIS_ BOOL*) PURE; // Proc41
    STDMETHOD(IsInHighZOrderBand)(THIS_ BOOL*) PURE; // Proc42
    STDMETHOD(IsSplashScreenPresented)(THIS_ BOOL*) PURE; // Proc43
    STDMETHOD(Flash)(THIS) PURE; // Proc44
    STDMETHOD(GetRootSwitchableOwner)(THIS_ IApplicationView**) PURE; // proc45
    STDMETHOD(EnumerateOwnershipTree)(THIS_ IObjectArray**) PURE; // proc46
    STDMETHOD(GetEnterpriseId)(THIS_ PWSTR*) PURE; // proc47
    STDMETHOD(IsMirrored)(THIS_ BOOL*) PURE; // Proc48
    STDMETHOD(Proc49)(THIS_ int*) PURE;
    STDMETHOD(Proc50)(THIS_ /* ENUM32 */ int32_t*) PURE;
    STDMETHOD(Proc51)(THIS_ int*) PURE;
    STDMETHOD(Proc52)(THIS_ int) PURE;
    STDMETHOD(Proc53)(THIS_ int*) PURE;
    STDMETHOD(Proc54)(THIS_ int) PURE;
    STDMETHOD(Proc55)(THIS) PURE;
    STDMETHOD(Proc56)(THIS_ int*) PURE;
    STDMETHOD(Proc57)(THIS_ int) PURE;
    STDMETHOD(Proc58)(THIS_ /* ENUM32 */ uint32_t, /* ENUM32 */ uint32_t) PURE;
    STDMETHOD(Proc59)(THIS_ int) PURE;
    STDMETHOD(Proc60)(THIS_ SIZE*) PURE;
    STDMETHOD(Proc61)(THIS_ PWSTR*) PURE;
};

const __declspec(selectany) IID& IID_IApplicationView = __uuidof(IApplicationView);

DECLARE_INTERFACE_IID_(IApplicationViewCollection, IUnknown, "1841C6D7-4F9D-42C0-AF41-8747538F10E5")
{
    STDMETHOD(GetViews)(THIS_ IObjectArray**) PURE;
    STDMETHOD(GetViewsByZOrder)(THIS_ IObjectArray**) PURE;
    STDMETHOD(GetViewsByAppUserModelId)(THIS_ PCWSTR, IObjectArray**) PURE;
    STDMETHOD(GetViewForHwnd)(THIS_ HWND, IApplicationView**) PURE;
    STDMETHOD(GetViewForApplication)(THIS_ IImmersiveApplication*, IApplicationView**) PURE;
    STDMETHOD(GetViewForAppUserModelId)(THIS_ PCWSTR, IApplicationView**) PURE;
    STDMETHOD(GetViewInFocus)(THIS_ IApplicationView**) PURE;
    STDMETHOD(TryGetLastActiveVisibleView)(THIS_ IApplicationView**) PURE;
    STDMETHOD(RefreshCollection)(THIS) PURE;
    STDMETHOD(RegisterForApplicationViewChanges)(THIS_ IApplicationViewChangeListener*, DWORD*) PURE;
    STDMETHOD(UnregisterForApplicationViewChanges)(THIS_ DWORD) PURE;
};

const __declspec(selectany) IID& IID_IApplicationViewCollection = __uuidof(IApplicationViewCollection);

DECLARE_INTERFACE_IID_(IVirtualDesktopPinnedApps, IUnknown, "4CE81583-1E4C-4632-A621-07A53543148F")
{
    STDMETHOD(IsAppIdPinned)(THIS_ PCWSTR appId, BOOL*) PURE;
    STDMETHOD(PinAppID)(THIS_ PCWSTR appId) PURE;
    STDMETHOD(UnpinAppID)(THIS_ PCWSTR appId) PURE;
    STDMETHOD(IsViewPinned)(THIS_ IApplicationView*, BOOL*) PURE;
    STDMETHOD(PinView)(THIS_ IApplicationView*) PURE;
    STDMETHOD(UnpinView)(THIS_ IApplicationView*) PURE;
};

namespace Win10 {

    MIDL_INTERFACE("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4")
        IVirtualDesktop : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsViewVisible(
        _In_ IApplicationView* pView,
        _Out_ BOOL* pfVisible) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetID(
            _Out_ GUID* pGuid) = 0;
    };

    MIDL_INTERFACE("31EBDE3F-6EC3-4CBD-B9FB-0EF6D09B41F4")
        IVirtualDesktop2: public IVirtualDesktop
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetName(
        _Out_ HSTRING* pStr) = 0;
    };

}

namespace Win11 {

    MIDL_INTERFACE("536D3495-B208-4CC9-AE26-DE8111275BF8")
        IVirtualDesktop : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsViewVisible(
        _In_ IApplicationView* pView,
        _Out_ BOOL* pfVisible) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetID(
            _Out_ GUID* pGuid) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetMonitor(
            _Out_ HMONITOR* pMonitor) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetName(
            _Out_ HSTRING* p0) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetWallpaperPath(
            _Out_ FC_USER_MARSHAL** p0) = 0;
    };

}

enum AdjacentDesktop
{
    LeftDirection = 3,
    RightDirection = 4
};

namespace Win10 {

    MIDL_INTERFACE("F31574D6-B682-4CDC-BD56-1827860ABEC6")
        IVirtualDesktopManagerInternal : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCount(
        _Out_ UINT* pCount) = 0;

        virtual HRESULT STDMETHODCALLTYPE MoveViewToDesktop(
            _In_ IApplicationView* pView,
            _In_ IVirtualDesktop* pDesktop) = 0;

        // Since build 10240
        virtual HRESULT STDMETHODCALLTYPE CanViewMoveDesktops(
            _In_ IApplicationView* pView,
            _Out_ BOOL* pfCanViewMoveDesktops) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetCurrentDesktop(
            _Out_ IVirtualDesktop** desktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetDesktops(
            _Out_ IObjectArray** ppDesktops) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetAdjacentDesktop(
            _In_ IVirtualDesktop* pDesktopReference,
            _In_ AdjacentDesktop uDirection,
            _Out_ IVirtualDesktop** ppAdjacentDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE SwitchDesktop(
            _In_ IVirtualDesktop* pDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE CreateDesktopW(
            _Out_ IVirtualDesktop** ppNewDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE RemoveDesktop(
            _In_ IVirtualDesktop* pRemove,
            _In_ IVirtualDesktop* pFallbackDesktop) = 0;

        // Since build 10240
        virtual HRESULT STDMETHODCALLTYPE FindDesktop(
            _In_ GUID* desktopId,
            _Out_ IVirtualDesktop** ppDesktop) = 0;

        // Since build ??
        virtual HRESULT STDMETHODCALLTYPE Proc13(
            _In_ IVirtualDesktop* pDesktop,
            _Out_ IObjectArray** ppDesktops1,
            _Out_ IObjectArray** ppDesktops2) = 0;
    };

    MIDL_INTERFACE("0F3A72B0-4566-487E-9A33-4ED302F6D6CE")
        IVirtualDesktopManagerInternal2 : public IVirtualDesktopManagerInternal
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetName(
        _In_ IVirtualDesktop* p0,
            _In_ HSTRING name) = 0;
    };

    MIDL_INTERFACE("FE538FF5-D53B-4F5A-9DAD-8E72873CB360")
        IVirtualDesktopManagerInternal3 : public IVirtualDesktopManagerInternal2
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Proc15(
        _In_ IApplicationView* p0,
        _In_ IApplicationView* p1) = 0;
    };

}

namespace Win11 {

    MIDL_INTERFACE("B2F925B9-5A0F-4D2E-9F4D-2B1507593C10")
        IVirtualDesktopManagerInternal : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCount(
            _In_opt_ HMONITOR monitor,
        _Out_ UINT* pCount) = 0;

        virtual HRESULT STDMETHODCALLTYPE MoveViewToDesktop(
            _In_ IApplicationView* pView,
            _In_ IVirtualDesktop* pDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE CanViewMoveDesktops(
            _In_ IApplicationView* pView,
            _Out_ BOOL* pfCanViewMoveDesktops) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetCurrentDesktop(
            _In_opt_ HMONITOR monitor,
            _Out_ IVirtualDesktop** desktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetAllCurrentDesktops(
            _Out_ IObjectArray** ppDesktops) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetDesktops(
            _In_opt_ HMONITOR monitor,
            _Out_ IObjectArray** ppDesktops) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetAdjacentDesktop(
            _In_ IVirtualDesktop* pDesktopReference,
            _In_ AdjacentDesktop uDirection,
            _Out_ IVirtualDesktop** ppAdjacentDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE SwitchDesktop(
            _In_opt_ HMONITOR monitor,
            _In_ IVirtualDesktop* pDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE CreateDesktopW(
            _In_opt_ HMONITOR monitor,
            _Out_ IVirtualDesktop** ppNewDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE MoveDesktop(    // New for Win11
            _In_ IVirtualDesktop* desktop,
            _In_opt_ HMONITOR monitor,
            _In_ INT32 index) = 0;

        virtual HRESULT STDMETHODCALLTYPE RemoveDesktop(
            _In_ IVirtualDesktop* pRemove,
            _In_ IVirtualDesktop* pFallbackDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE FindDesktop(
            _In_ GUID* desktopId,
            _Out_ IVirtualDesktop** ppDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetDesktopSwitchIncludeExcludeViews(
            _In_ IVirtualDesktop* pDesktop,
            _Out_ IObjectArray** ppDesktops1,
            _Out_ IObjectArray** ppDesktops2) = 0;

        virtual HRESULT STDMETHODCALLTYPE SetName(
            _In_ IVirtualDesktop* p0,
            _In_ HSTRING name) = 0;

        virtual HRESULT STDMETHODCALLTYPE SetWallpaper(
            _In_ IVirtualDesktop* p0,
            _In_ HSTRING name) = 0;

        virtual HRESULT STDMETHODCALLTYPE UpdateWallpaperPathForAllDesktops(
            _In_ HSTRING name) = 0;

        virtual HRESULT STDMETHODCALLTYPE CopyDesktopState(
            _In_ IApplicationView* p0,
            _In_ IApplicationView* p1) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetDesktopIsPerMonitor(
            _Out_ BOOL* p0) = 0;

        virtual HRESULT STDMETHODCALLTYPE SetDesktopIsPerMonitor(
            _In_ BOOL p0) = 0;
    };

}

namespace Win10 {

    MIDL_INTERFACE("C179334C-4295-40D3-BEA1-C654D965605A")
        IVirtualDesktopNotification : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopCreated(
        _In_ IVirtualDesktop* pDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyBegin(
            _In_ IVirtualDesktop* pDesktopDestroyed,
            _In_ IVirtualDesktop* pDesktopFallback) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyFailed(
            _In_ IVirtualDesktop* pDesktopDestroyed,
            _In_ IVirtualDesktop* pDesktopFallback) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyed(
            _In_ IVirtualDesktop* pDesktopDestroyed,
            _In_ IVirtualDesktop* pDesktopFallback) = 0;

        virtual HRESULT STDMETHODCALLTYPE ViewVirtualDesktopChanged(
            _In_ IApplicationView* pView) = 0;

        virtual HRESULT STDMETHODCALLTYPE CurrentVirtualDesktopChanged(
            _In_ IVirtualDesktop* pDesktopOld,
            _In_ IVirtualDesktop* pDesktopNew) = 0;
    };

    const __declspec(selectany) IID& IID_IVirtualDesktopNotification = __uuidof(IVirtualDesktopNotification);

    MIDL_INTERFACE("1BA7CF30-3591-43FA-ABFA-4AAF7ABEEDB7")
        IVirtualDesktopNotification2 : public IVirtualDesktopNotification
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopNameChanged(
        _In_ IVirtualDesktop* pDesktop,
            _In_ HSTRING p1) = 0;
    };

    MIDL_INTERFACE("0CD45E71-D927-4F15-8B0A-8FEF525337BF")
        IVirtualDesktopNotificationService : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Register(
            _In_ IVirtualDesktopNotification * pNotification,
            _Out_ DWORD * pdwCookie) = 0;

        virtual HRESULT STDMETHODCALLTYPE Unregister(
            _In_ DWORD dwCookie) = 0;
    };

}

namespace Win11 {

    MIDL_INTERFACE("CD403E52-DEED-4C13-B437-B98380F2B1E8")
        IVirtualDesktopNotification : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopCreated(
        _In_ IObjectArray* monitors,
        _In_ IVirtualDesktop* pDesktop) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyBegin(
            _In_ IObjectArray* monitors,
            _In_ IVirtualDesktop* pDesktopDestroyed,
            _In_ IVirtualDesktop* pDesktopFallback) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyFailed(
            _In_ IObjectArray* monitors,
            _In_ IVirtualDesktop* pDesktopDestroyed,
            _In_ IVirtualDesktop* pDesktopFallback) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyed(
            _In_ IObjectArray* monitors,
            _In_ IVirtualDesktop* pDesktopDestroyed,
            _In_ IVirtualDesktop* pDesktopFallback) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopIsPerMonitorChanged(
            _In_ BOOL isPerMonitor) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopMoved(
            _In_ IObjectArray* monitors,
            _In_ IVirtualDesktop* pDesktop,
            _In_ int64_t oldIndex,
            _In_ int64_t newIndex) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopNameChanged11(
            _In_ IVirtualDesktop* pDesktop,
            _In_ HSTRING name) = 0;

        virtual HRESULT STDMETHODCALLTYPE ViewVirtualDesktopChanged11(
            _In_ IApplicationView* pView) = 0;

        virtual HRESULT STDMETHODCALLTYPE CurrentVirtualDesktopChanged(
            _In_ IObjectArray* monitors,
            _In_ IVirtualDesktop* pDesktopOld,
            _In_ IVirtualDesktop* pDesktopNew) = 0;

        virtual HRESULT STDMETHODCALLTYPE VirtualDesktopWallpaperChanged(
            _In_ IVirtualDesktop* pDesktop,
            _In_ HSTRING name) = 0;
    };

    const __declspec(selectany) IID& IID_IVirtualDesktopNotification = __uuidof(IVirtualDesktopNotification);

    MIDL_INTERFACE("0CD45E71-D927-4F15-8B0A-8FEF525337BF")
        IVirtualDesktopNotificationService : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Register(
            _In_ IVirtualDesktopNotification * pNotification,
            _Out_ DWORD * pdwCookie) = 0;

        virtual HRESULT STDMETHODCALLTYPE Unregister(
            _In_ DWORD dwCookie) = 0;
    };

}

template <class VDMI, class VD>
inline HRESULT GetCurrentDesktop(VDMI* pDesktopManagerInternal, VD** desktop)
{
    return pDesktopManagerInternal->GetCurrentDesktop(desktop);
}

template <>
inline HRESULT GetCurrentDesktop(Win11::IVirtualDesktopManagerInternal* pDesktopManagerInternal, Win11::IVirtualDesktop** desktop)
{
    return pDesktopManagerInternal->GetCurrentDesktop(NULL, desktop);
}

template <class VDMI>
inline HRESULT GetDesktops(VDMI* pDesktopManagerInternal, IObjectArray** ppDesktops)
{
    return pDesktopManagerInternal->GetDesktops(ppDesktops);
}

template <>
inline HRESULT GetDesktops(Win11::IVirtualDesktopManagerInternal* pDesktopManagerInternal, IObjectArray** ppDesktops)
{
    return pDesktopManagerInternal->GetDesktops(NULL, ppDesktops);
}

template <class VDMI, class VD>
inline HRESULT SwitchDesktop(VDMI* pDesktopManagerInternal, VD* desktop)
{
    return pDesktopManagerInternal->SwitchDesktop(desktop);
}

template <>
inline HRESULT SwitchDesktop(Win11::IVirtualDesktopManagerInternal* pDesktopManagerInternal, Win11::IVirtualDesktop* desktop)
{
    return pDesktopManagerInternal->SwitchDesktop(NULL, desktop);
}

template <class VDMI, class VD>
inline HRESULT CreateDesktop(VDMI* pDesktopManagerInternal, VD** desktop)
{
    return pDesktopManagerInternal->CreateDesktop(desktop);
}

template <>
inline HRESULT CreateDesktop(Win11::IVirtualDesktopManagerInternal* pDesktopManagerInternal, Win11::IVirtualDesktop** desktop)
{
    return pDesktopManagerInternal->CreateDesktop(NULL, desktop);
}
