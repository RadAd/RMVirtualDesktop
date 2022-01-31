#pragma once

#include <atlbase.h>
#include <cassert>

template<class T>
struct ObjectArrayIndex
{
    UINT i;
    IObjectArray* pObjectArray;

    ObjectArrayIndex& operator++()
    {
        ++i;
        return *this;
    }

    CComPtr<T> operator*()
    {
        CComPtr<T> p;
        if (FAILED(pObjectArray->GetAt(i, IID_PPV_ARGS(&p))))
            return nullptr;
        return p;
    }
};

template<class T>
bool operator!=(const ObjectArrayIndex<T>& l, const ObjectArrayIndex<T>& r)
{
    assert(l.pObjectArray == r.pObjectArray);
    return l.i != r.i;
}

template<class T>
class ObjectArrayRange
{
private:
    IObjectArray* pObjectArray;

public:
    ObjectArrayRange(IObjectArray* pObjectArray)
        : pObjectArray(pObjectArray)
    {
    }

    ObjectArrayIndex<T> begin() const
    {
        return { 0, pObjectArray };
    }

    ObjectArrayIndex<T> end() const
    {
        UINT count;
        if (FAILED(pObjectArray->GetCount(&count)))
            count = 0;
        return { count, pObjectArray };
    }
};

template<class T>
struct ObjectArrayIndexRev
{
    UINT i;
    IObjectArray* pObjectArray;

    ObjectArrayIndexRev& operator++()
    {
        --i;
        return *this;
    }

    CComPtr<T> operator*()
    {
        CComPtr<T> p;
        if (FAILED(pObjectArray->GetAt(i - 1, IID_PPV_ARGS(&p))))
            return nullptr;
        return p;
    }
};

template<class T>
bool operator!=(const ObjectArrayIndexRev<T>& l, const ObjectArrayIndexRev<T>& r)
{
    assert(l.pObjectArray == r.pObjectArray);
    return l.i != r.i;
}

template<class T>
class ObjectArrayRangeRev
{
private:
    IObjectArray* pObjectArray;

public:
    ObjectArrayRangeRev(IObjectArray* pObjectArray)
        : pObjectArray(pObjectArray)
    {
    }

    ObjectArrayIndexRev<T> begin() const
    {
        UINT count;
        if (FAILED(pObjectArray->GetCount(&count)))
            count = 0;
        return { count, pObjectArray };
    }

    ObjectArrayIndexRev<T> end() const
    {
        return { 0, pObjectArray };
    }
};
