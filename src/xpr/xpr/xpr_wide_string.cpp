//
// Copyright (c) 2012 Leon Lee author. All rights reserved.
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#include "xpr_wide_string.h"
#include "xpr_bit.h"
#include "xpr_memory.h"
#include "xpr_debug.h"
#include "xpr_char.h"
#include "xpr_cstr.h"

namespace xpr
{
WideString::WideString(void)
    : mString(XPR_NULL)
    , mLength(0)
    , mCapacity(0)
{
    assign(XPR_WIDE_STRING_LITERAL(""), 0);
}

WideString::WideString(const WideString &aString)
    : mString(XPR_NULL)
    , mLength(0)
    , mCapacity(0)
{
    assign(aString);
}

WideString::WideString(const WideString &aString, xpr_size_t aPos, xpr_size_t aLength)
    : mString(XPR_NULL)
    , mLength(0)
    , mCapacity(0)
{
    assign(aString, aPos, aLength);
}

WideString::WideString(const xpr_wchar_t *aString, xpr_size_t aLength)
    : mString(XPR_NULL)
    , mLength(0)
    , mCapacity(0)
{
    if (XPR_IS_NULL(aString))
    {
        assign(XPR_WIDE_STRING_LITERAL(""), 0);
    }
    else
    {
        assign(aString, aLength);
    }
}

WideString::WideString(const xpr_wchar_t *aString)
    : mString(XPR_NULL)
    , mLength(0)
    , mCapacity(0)
{
    if (XPR_IS_NULL(aString))
    {
        assign(XPR_WIDE_STRING_LITERAL(""), 0);
    }
    else
    {
        assign(aString);
    }
}

WideString::WideString(const xpr_size_t aNumber, xpr_wchar_t aChar)
    : mString(XPR_NULL)
    , mLength(0)
    , mCapacity(0)
{
    assign(aNumber, aChar);
}

WideString::WideString(Iterator aFirst, Iterator aLast)
    : mString(XPR_NULL)
    , mLength(0)
    , mCapacity(0)
{
    assign(aFirst, aLast);
}

WideString::~WideString(void)
{
    clear();
}

WideString::Iterator WideString::begin(void)
{
    return Iterator(mString);
}

WideString::Iterator WideString::end(void)
{
    return Iterator(mString + mLength);
}

WideString::ConstIterator WideString::begin(void) const
{
    return ConstIterator(mString);
}

WideString::ConstIterator WideString::end(void) const
{
    return ConstIterator(mString + mLength);
}

WideString::ReverseIterator WideString::rbegin(void)
{
    return ReverseIterator(mString + mLength - 1);
}

WideString::ReverseIterator WideString::rend(void)
{
    return ReverseIterator(mString - 1);
}

WideString::ConstReverseIterator WideString::rbegin(void) const
{
    return ConstReverseIterator(mString + mLength - 1);
}

WideString::ConstReverseIterator WideString::rend(void) const
{
    return ConstReverseIterator(mString - 1);
}

xpr_size_t WideString::size(void) const
{
    return mLength;
}

xpr_size_t WideString::length(void) const
{
    return mLength;
}

xpr_size_t WideString::bytes(void) const
{
    return mLength * sizeof(xpr_wchar_t);
}

xpr_size_t WideString::max_size(void) const
{
    return (xpr_size_t)-1;
}

void WideString::resize(xpr_size_t aNumber, xpr_wchar_t aChar)
{
    if (aNumber == 0)
        return;

    if (mCapacity > aNumber)
    {
        if (aNumber > mLength)
        {
            wmemset(mString + mLength, aChar, aNumber - mLength);

            if (aChar != 0)
            {
                mString[aNumber] = 0;
            }
        }
        else
        {
            mString[aNumber] = 0;

            mLength = aNumber;
        }
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(aNumber + 1, sNewCapacity);

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return;
        }

        wcsncpy(sNewString, mString, mLength);
        wmemset(sNewString + mLength, aChar, aNumber - mLength);

        if (aChar != 0)
        {
            sNewString[aNumber] = 0;
        }

        xpr_size_t sNewLength = aNumber;
        if (aChar == 0)
        {
            sNewLength = mLength;
        }

        reset();

        mString = sNewString;
        mLength = sNewLength;
        mCapacity = sNewCapacity;
    }
}

xpr_wchar_t *WideString::alloc(const xpr_size_t &sMinCapacity, xpr_size_t &sCapacity) const
{
    xpr_size_t sNewCapacity = sMinCapacity;

    xpr_wchar_t *sNewString = new xpr_wchar_t[sNewCapacity];
    if (XPR_IS_NULL(sNewString))
        return XPR_NULL;

    sCapacity = sNewCapacity;

    return sNewString;
}

void WideString::resize(xpr_size_t aNumber)
{
    resize(aNumber, 0);
}

xpr_size_t WideString::capacity(void) const
{
    return mCapacity;
}

void WideString::reserve(xpr_size_t aCapacity)
{
    if (aCapacity > mCapacity)
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(aCapacity, sNewCapacity);

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
            return;

        wcsncpy(sNewString, mString, mLength);
        sNewString[mLength] = 0;

        xpr_size_t sLength = mLength;

        reset();

        mString = sNewString;
        mLength = sLength;
        mCapacity = sNewCapacity;
    }
}

void WideString::reset(void)
{
    XPR_SAFE_DELETE_ARRAY(mString);
    mLength = 0;
    mCapacity = 0;
}

void WideString::clear(void)
{
    mLength = 0;
}

xpr_bool_t WideString::empty(void)
{
    return (XPR_IS_NULL(mString) || mLength == 0) ? XPR_TRUE : XPR_FALSE;
}

WideString& WideString::assign(const WideString &aString)
{
    return assign(aString.mString, aString.mLength);
}

// aLength may be npos.
// aLegnth cannot be greater than length of aString.
WideString& WideString::assign(const WideString &aString, xpr_size_t aPos, xpr_size_t aLength)
{
    if (aLength == 0)
    {
        return *this;
    }

    XPR_ASSERT(aPos <= aString.mLength);

    if (aLength == npos)
    {
        aLength = aString.mLength - aPos;
    }
    else if ((aString.mLength - aPos) < aLength)
    {
        aLength = aString.mLength - aPos;
    }

    return assign(aString.mString + aPos, aLength);
}

// If aLength is greater than length of aString, then memory allocate greater but mLength is length of aString.
WideString& WideString::assign(const xpr_wchar_t *aString, xpr_size_t aLength)
{
    if (XPR_IS_NULL(aString))
    {
        return *this;
    }

    xpr_size_t sLength = wcslen(aString);
    xpr_size_t sActualLength = (aLength > sLength) ? sLength : aLength;

    if (mCapacity > aLength)
    {
        wcsncpy(mString, aString, sActualLength);

        if (aLength > sLength)
        {
            memset(mString + sLength, 0, (aLength - sLength) * sizeof(xpr_wchar_t));
        }
        else
        {
            mString[aLength] = 0;
        }

        mLength = sActualLength;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(aLength + 1, sNewCapacity);

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return *this;
        }

        wcsncpy(sNewString, aString, sActualLength);

        if (aLength > sLength)
        {
            memset(sNewString + sLength, 0, (aLength - sLength) * sizeof(xpr_wchar_t));
        }
        else
        {
            sNewString[aLength] = 0;
        }

        reset();

        mString = sNewString;
        mLength = sActualLength;
        mCapacity = sNewCapacity;
    }

    return *this;
}

WideString& WideString::assign(const xpr_wchar_t *aString)
{
    if (XPR_IS_NULL(aString))
    {
        return *this;
    }

    xpr_size_t sLength = wcslen(aString);

    return assign(aString, sLength);
}

WideString& WideString::assign(xpr_size_t aNumber, xpr_wchar_t aChar)
{
    if (aNumber == 0)
    {
        return *this;
    }

    xpr_size_t sActualLength = (aChar == 0) ? 0 : aNumber;

    if (mCapacity > aNumber)
    {
        wmemset(mString, aChar, aNumber);

        if (aChar != 0)
        {
            mString[aNumber] = 0;
        }

        mLength = sActualLength;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(aNumber + 1, sNewCapacity);

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return *this;
        }

        wmemset(sNewString, aChar, aNumber);

        if (aChar != 0)
        {
            sNewString[aNumber] = 0;
        }

        reset();

        mString = sNewString;
        mLength = sActualLength;
        mCapacity = sNewCapacity;
    }

    return *this;
}

WideString& WideString::assign(Iterator aFirst, Iterator aLast)
{
    xpr_wchar_t *sString = &(*aFirst);
    xpr_size_t  sLength = aLast - aFirst;

    return assign(sString, sLength);
}

const xpr_wchar_t& WideString::operator[] (xpr_size_t aPos) const
{
    XPR_ASSERT(aPos < mLength);

    return mString[aPos];
}

xpr_wchar_t& WideString::operator[] (xpr_size_t aPos)
{
    XPR_ASSERT(aPos < mLength);

    return mString[aPos];
}

const xpr_wchar_t& WideString::at(xpr_size_t aPos) const
{
    XPR_ASSERT(aPos < mLength);

    return mString[aPos];
}

xpr_wchar_t& WideString::at(xpr_size_t aPos)
{
    XPR_ASSERT(aPos < mLength);

    return mString[aPos];
}

WideString& WideString::operator= (const WideString &aString)
{
    return assign(aString);
}

WideString& WideString::operator= (const xpr_wchar_t *aString)
{
    return assign(aString);
}

WideString& WideString::operator= (xpr_wchar_t aChar)
{
    return assign(1, aChar);
}

WideString& WideString::operator+= (const WideString &aString)
{
    return append(aString);
}

WideString& WideString::operator+= (const xpr_wchar_t *aString)
{
    return append(aString);
}

WideString& WideString::operator+= (xpr_wchar_t aChar)
{
    return append(1, aChar);
}

WideString& WideString::append(const WideString &aString)
{
    return append(aString.mString, aString.mLength);
}

// aLength may be npos.
// aLegnth cannot be greater than length of aString.
WideString& WideString::append(const WideString &aString, xpr_size_t aPos, xpr_size_t aLength)
{
    if (aLength == 0)
    {
        return *this;
    }

    XPR_ASSERT(aPos <= aString.mLength);

    if (aLength == npos)
    {
        aLength = aString.mLength - aPos;
    }
    else if ((aString.mLength - aPos) < aLength)
    {
        aLength = aString.mLength - aPos;
    }

    return append(aString.mString + aPos, aLength);
}

// If aLength is greater than length of aString, then memory allocate greater but mLength is length of aString.
WideString& WideString::append(const xpr_wchar_t *aString, xpr_size_t aLength)
{
    if (XPR_IS_NULL(aString) || aLength == 0)
    {
        return *this;
    }

    xpr_size_t sLength = wcslen(aString);
    xpr_size_t sActualLength = (aLength > sLength) ? sLength : aLength;

    if (mCapacity > (mLength + aLength))
    {
        wcsncpy(mString + mLength, aString, sActualLength);

        if (aLength > sLength)
        {
            memset(mString + mLength + sLength, 0, (aLength - sLength) * sizeof(xpr_wchar_t));
        }
        else
        {
            mString[mLength + aLength] = 0;
        }

        mLength += sActualLength;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(mLength + aLength + 1, sNewCapacity);
        xpr_size_t sNewLength = mLength + sActualLength;

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return *this;
        }

        wcsncpy(sNewString, mString, mLength);
        wcsncpy(sNewString + mLength, aString, sActualLength);

        if (aLength > sLength)
        {
            memset(sNewString + mLength + sLength, 0, (aLength - sLength) * sizeof(xpr_wchar_t));
        }
        else
        {
            sNewString[mLength + aLength] = 0;
        }

        reset();

        mString = sNewString;
        mLength = sNewLength;
        mCapacity = sNewCapacity;
    }

    return *this;
}

WideString& WideString::append(const xpr_wchar_t *aString)
{
    if (XPR_IS_NULL(aString))
    {
        return *this;
    }

    xpr_size_t sLength = wcslen(aString);

    return append(aString, sLength);
}

WideString& WideString::append(xpr_size_t aNumber, xpr_wchar_t aChar)
{
    if (aNumber == 0)
    {
        return *this;
    }

    xpr_size_t sActualLength = (aChar == 0) ? 0 : aNumber;

    if (mCapacity > (mLength + aNumber))
    {
        wmemset(mString + mLength, aChar, aNumber);

        if (aChar != 0)
        {
            mString[mLength + aNumber] = 0;
        }

        mLength += sActualLength;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(mLength + aNumber + 1, sNewCapacity);
        xpr_size_t sNewLength = mLength + sActualLength;

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return *this;
        }

        wcsncpy(sNewString, mString, mLength);
        wmemset(sNewString + mLength, aChar, aNumber);

        if (aChar != 0)
        {
            sNewString[mLength + aNumber] = 0;
        }

        reset();

        mString = sNewString;
        mLength = sNewLength;
        mCapacity = sNewCapacity;
    }

    return *this;
}

WideString& WideString::append(Iterator aFirst, Iterator aLast)
{
    xpr_wchar_t *sString = &(*aFirst);
    xpr_size_t  sLength = aLast - aFirst;

    return append(sString, sLength);
}

void WideString::push_back(xpr_wchar_t aChar)
{
    append(1, aChar);
}

WideString& WideString::insert(xpr_size_t aPos, const WideString &aString)
{
    return insert(aPos, aString.mString, aString.mLength);
}

WideString& WideString::insert(xpr_size_t aPos1, const WideString &aString, xpr_size_t aPos2, xpr_size_t aLength)
{
    XPR_ASSERT(aPos2 <= aString.mLength);

    if (aLength == npos)
    {
        aLength = aString.mLength - aPos2;
    }
    else if ((aString.mLength - aPos2) < aLength)
    {
        aLength = aString.mLength - aPos2;
    }

    return insert(aPos1, aString.mString + aPos2, aLength);
}

WideString& WideString::insert(xpr_size_t aPos, const xpr_wchar_t *aString, xpr_size_t aLength)
{
    if (XPR_IS_NULL(aString) || aLength == 0)
    {
        return *this;
    }

    if (aPos == npos || aPos >= mLength)
    {
        return append(aString, aLength);
    }

    xpr_size_t sLength = wcslen(aString);
    if (sLength > aLength)
        sLength = aLength;

    if (mCapacity > (mLength + sLength))
    {
        memmove(mString + aPos + sLength, mString + aPos, sLength * sizeof(xpr_wchar_t));
        wcsncpy(mString + aPos, aString, sLength);

        mLength += sLength;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(mLength + sLength + 1, sNewCapacity);
        xpr_size_t sNewLength = mLength + sLength;

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return *this;
        }

        wcsncpy(sNewString, mString, aPos);
        wcsncpy(sNewString + aPos, aString, sLength);
        wcsncpy(sNewString + aPos + sLength, mString + aPos, mLength - aPos);
        sNewString[mLength + sLength] = 0;

        reset();

        mString = sNewString;
        mLength = sNewLength;
        mCapacity = sNewCapacity;
    }

    return *this;
}

WideString& WideString::insert(xpr_size_t aPos, const xpr_wchar_t *aString)
{
    if (XPR_IS_NULL(aString))
    {
        return *this;
    }

    xpr_size_t sLength = wcslen(aString);

    return insert(aPos, aString, sLength);
}

WideString& WideString::insert(xpr_size_t aPos, xpr_size_t aNumber, xpr_wchar_t aChar)
{
    if (aNumber == 0 || aChar == 0)
    {
        return *this;
    }

    if (aPos == npos || aPos >= mLength)
    {
        return append(aNumber, aChar);
    }

    if (mCapacity > (mLength + aNumber))
    {
        memmove(mString + aPos + aNumber, mString + aPos, aNumber * sizeof(xpr_wchar_t));
        wmemset(mString + aPos, aChar, aNumber);

        mLength += aNumber;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(mLength + aNumber + 1, sNewCapacity);
        xpr_size_t sNewLength = mLength + aNumber;

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return *this;
        }

        wcsncpy(sNewString, mString, aPos);
        wmemset(sNewString + aPos, aChar, aNumber);
        wcsncpy(sNewString + aPos + aNumber, mString + aPos, mLength - aPos);
        sNewString[mLength + aNumber] = 0;

        reset();

        mString = sNewString;
        mLength = sNewLength;
        mCapacity = sNewCapacity;
    }

    return *this;
}

WideString::Iterator WideString::insert(Iterator aPos, xpr_wchar_t aChar)
{
    return insert(aPos, 1, aChar);
}

WideString::Iterator WideString::insert(Iterator aPos, xpr_size_t aNumber, xpr_wchar_t aChar)
{
    xpr_size_t sPos = aPos.mString - mString;

    insert(sPos, aNumber, aChar);

    return Iterator(mString + sPos);
}

WideString::Iterator WideString::insert(Iterator aPos, Iterator aFirst, Iterator aLast)
{
    xpr_size_t  sPos    = aPos.mString - mString;
    xpr_wchar_t *sString = &(*aFirst);
    xpr_size_t  sLength = aLast - aFirst;

    insert(sPos, sString, sLength);

    return Iterator(mString + sPos);
}

WideString& WideString::erase(xpr_size_t aPos, xpr_size_t aLength)
{
    if (aPos == 0)
    {
        if (aLength == npos || mLength >= aLength)
        {
            clear();
            return *this;
        }
    }

    if (aLength == npos)
    {
        aLength = mLength - aPos;
    }

    memmove(mString + aPos, mString + aPos + aLength, (mLength - (aPos + aLength)) * sizeof(xpr_wchar_t));
    mString[mLength - aLength] = 0;

    mLength -= aLength;

    return *this;
}

WideString::Iterator WideString::erase(Iterator aPos)
{
    xpr_size_t sPos = aPos.mString - mString;

    erase(sPos, 1);

    return Iterator(mString + sPos);
}

WideString::Iterator WideString::erase(Iterator aFirst, Iterator aLast)
{
    xpr_size_t sPos    = aFirst.mString - mString;
    xpr_size_t sLength = aLast - aFirst;

    erase(sPos, sLength);

    return Iterator(mString + sPos);
}

WideString& WideString::replace(xpr_size_t aPos, xpr_size_t aLength, const WideString &aString)
{
    return replace(aPos, aLength, aString.mString, aString.mLength);
}

WideString& WideString::replace(Iterator aIterator1, Iterator aIterator2, const WideString &aString)
{
    xpr_size_t sPos    = aIterator1.mString - mString;
    xpr_size_t sLength = aIterator2 - aIterator1;

    return replace(sPos, sLength, aString.mString, aString.mLength);
}

WideString& WideString::replace(xpr_size_t aPos1, xpr_size_t aLength1, const WideString &aString, xpr_size_t aPos2, xpr_size_t aLength2)
{
    XPR_ASSERT(aPos2 <= aString.mLength);

    if (aLength2 == npos)
    {
        aLength2 = aString.mLength - aPos2;
    }
    else if ((aString.mLength - aPos2) < aLength2)
    {
        aLength2 = aString.mLength - aPos2;
    }

    return replace(aPos1, aLength1, aString.mString + aPos2, aLength2);
}

WideString& WideString::replace(xpr_size_t aPos, xpr_size_t aLength1, const xpr_wchar_t *aString, xpr_size_t aLength2)
{
    if (aPos == npos || aPos >= mLength || XPR_IS_NULL(aString) || aLength2 == 0)
    {
        return *this;
    }

    xpr_size_t sLength = wcslen(aString);
    if (sLength > aLength2)
        sLength = aLength2;

    xpr_size_t sNewLength = mLength - aLength1 + sLength;

    if (mCapacity > sNewLength)
    {
        memmove(mString + aPos + sLength, mString + aPos + aLength1, (mLength - aPos - aLength1) * sizeof(xpr_wchar_t));
        wcsncpy(mString + aPos, aString, sLength);
        mString[sNewLength] = 0;

        mLength = sNewLength;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(sNewLength + 1, sNewCapacity);

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return *this;
        }

        wcsncpy(sNewString, mString, aPos);
        wcsncpy(sNewString + aPos, aString, sLength);
        wcsncpy(sNewString + aPos + sLength, mString + aPos + aLength1, mLength - aPos - aLength1);
        sNewString[sNewLength] = 0;

        reset();

        mString = sNewString;
        mLength = sNewLength;
        mCapacity = sNewCapacity;
    }

    return *this;
}

WideString& WideString::replace(Iterator aIterator1, Iterator aIterator2, const xpr_wchar_t *aString, xpr_size_t aLength)
{
    xpr_size_t sPos    = aIterator1.mString - mString;
    xpr_size_t sLength = aIterator2 - aIterator1;

    return replace(sPos, sLength, aString, aLength);
}

WideString& WideString::replace(xpr_size_t aPos, xpr_size_t aLength, const xpr_wchar_t *aString)
{
    if (XPR_IS_NULL(aString))
    {
        return *this;
    }

    xpr_size_t sLength = wcslen(aString);

    return replace(aPos, aLength, aString, sLength);
}

WideString& WideString::replace(Iterator aIterator1, Iterator aIterator2, const xpr_wchar_t *aString)
{
    if (XPR_IS_NULL(aString))
    {
        return *this;
    }

    xpr_size_t sPos     = aIterator1.mString - mString;
    xpr_size_t sLength1 = aIterator2 - aIterator1;

    xpr_size_t sLength2 = wcslen(aString);

    return replace(sPos, sLength1, aString, sLength2);
}

WideString& WideString::replace(xpr_size_t aPos, xpr_size_t aLength, xpr_size_t aNumber, xpr_wchar_t aChar)
{
    if (aPos == npos || aPos >= mLength || aNumber == 0 || aChar == 0)
    {
        return *this;
    }

    xpr_size_t sNewLength = mLength - aLength + aNumber;

    if (mCapacity > sNewLength)
    {
        memmove(mString + aPos + aNumber, mString + aPos + aLength, (mLength - aPos - aLength) * sizeof(xpr_wchar_t));
        wmemset(mString + aPos, 0, aNumber);
        wmemset(mString + aPos, aChar, aNumber);
        mString[sNewLength] = 0;

        mLength = sNewLength;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(sNewLength + 1, sNewCapacity);

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return *this;
        }

        wcsncpy(sNewString, mString, aPos);
        wmemset(sNewString + aPos, aChar, aNumber);
        wcsncpy(sNewString + aPos + aNumber, mString + aPos + aLength, mLength - aPos - aLength);
        sNewString[sNewLength] = 0;

        reset();

        mString = sNewString;
        mLength = sNewLength;
        mCapacity = sNewCapacity;
    }

    return *this;
}

WideString& WideString::replace(Iterator aIterator1, Iterator aIterator2, xpr_size_t aNumber, xpr_wchar_t aChar)
{
    xpr_size_t sPos    = aIterator1.mString - mString;
    xpr_size_t sLength = aIterator2 - aIterator1;

    return replace(sPos, sLength, aNumber, aChar);
}

WideString& WideString::replace(Iterator aIterator1, Iterator aIterator2, Iterator aInputIterator1, Iterator aInputIterator2)
{
    xpr_size_t  sPos    = aIterator1.mString - mString;
    xpr_size_t  sLength1 = aIterator2 - aIterator1;

    xpr_wchar_t *sString  = aInputIterator1.mString;
    xpr_size_t  sLength2 = aInputIterator2 - aInputIterator1;

    return replace(sPos, sLength1, sString, sLength2);
}

void WideString::swap(WideString &aString)
{
    xpr_wchar_t *sTempString   = mString;
    xpr_size_t  sTempLength   = mLength;
    xpr_size_t  sTempCapacity = mCapacity;

    mString   = aString.mString;
    mLength   = aString.mLength;
    mCapacity = aString.mCapacity;

    aString.mString   = sTempString;
    aString.mLength   = sTempLength;
    aString.mCapacity = sTempCapacity;
}

void WideString::upper_case(void)
{
    _wcsupr(mString);
}

void WideString::lower_case(void)
{
    _wcslwr(mString);
}

void WideString::trim(void)
{
    trim_right();
    trim_left();
}

void WideString::trim_left(void)
{
    xpr_size_t i;
    for (i = 0; i < mLength; ++i)
    {
        if (xpr::isSpace((xpr_char_t)mString[i]) == XPR_FALSE)
        {
            erase(0, i);
            break;
        }
    }
}

void WideString::trim_right(void)
{
    xpr_size_t i;
    for (i = mLength - 1; i >= 0; --i)
    {
        if (xpr::isSpace((xpr_char_t)mString[i]) == XPR_FALSE)
        {
            erase(i, mLength - i - 1);
            break;
        }
    }
}

void WideString::format(const xpr_wchar_t *aFormat, ...)
{
    va_list sArgs;
    va_start(sArgs, aFormat);

    xpr_sint_t sLength = _vscwprintf(aFormat, sArgs);
    if (sLength <= 0)
        return;

    if (mCapacity > (xpr_size_t)sLength)
    {
        vswprintf(mString, aFormat, sArgs);
        mString[sLength] = 0;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(sLength + 1, sNewCapacity);

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return;
        }

        vswprintf(sNewString, aFormat, sArgs);
        sNewString[sLength] = 0;

        reset();

        mString = sNewString;
        mLength = sLength;
        mCapacity = sNewCapacity;
    }

    va_end(sArgs);
}

void WideString::append_format(const xpr_wchar_t *aFormat, ...)
{
    va_list sArgs;
    va_start(sArgs, aFormat);

    xpr_sint_t sLength = _vscwprintf(aFormat, sArgs);
    if (sLength <= 0)
        return;

    xpr_size_t sNewLength = mLength + (xpr_size_t)sLength;

    if (mCapacity > sNewLength)
    {
        vswprintf(mString + mLength, aFormat, sArgs);
        mString[sNewLength] = 0;
    }
    else
    {
        xpr_size_t sNewCapacity;
        xpr_wchar_t *sNewString = alloc(sNewLength + 1, sNewCapacity);

        XPR_ASSERT(sNewString != XPR_NULL);
        if (XPR_IS_NULL(sNewString))
        {
            return;
        }

        wcsncpy(sNewString, mString, mLength);
        vswprintf(sNewString + mLength, aFormat, sArgs);
        sNewString[sNewLength] = 0;

        reset();

        mString = sNewString;
        mLength = sNewLength;
        mCapacity = sNewCapacity;
    }

    va_end(sArgs);
}

const xpr_wchar_t *WideString::c_str(void) const
{
    return mString;
}

const xpr_wchar_t *WideString::data(void) const
{
    return mString;
}

xpr_size_t WideString::copy(xpr_wchar_t *aString, xpr_size_t aLength, xpr_size_t aPos) const
{
    if (XPR_IS_NULL(aString) || aLength == 0 || aPos == npos || aPos >= mLength)
    {
        return 0;
    }

    xpr_size_t sLength = aLength;

    if ((aPos + sLength) > mLength)
    {
        sLength = mLength - aPos;
    }

    wcsncpy(aString, mString + aPos, sLength);

    return sLength;
}

xpr_size_t WideString::find(const WideString &aString, xpr_size_t aPos) const
{
    return find(aString.mString, aPos, mLength);
}

xpr_size_t WideString::find(const xpr_wchar_t *aString, xpr_size_t aPos, xpr_size_t aLength) const
{
    if (XPR_IS_NULL(aString) || aPos == npos || aPos >= mLength)
    {
        return npos;
    }

    const xpr_wchar_t *sString = mString + aPos;
    const xpr_wchar_t *sSearch = aString;
    xpr_wchar_t sSubStrChar, sChar;
    xpr_size_t sLength;

    if ((sSubStrChar = *sSearch++) != '\0')
    {
        sLength = wcslen(sSearch);
        if (sLength > aLength)
            sLength = aLength;

        do
        {
            do
            {
                if ((sChar = *sString++) == '\0')
                {
                    return XPR_NULL;
                }

            } while (sChar != sSubStrChar);

            if (sLength > aLength)
            {
                return XPR_NULL;
            }

        } while (wcsncmp(sString, sSearch, sLength) != 0);

        sString--;
    }

    xpr_size_t sFoundIndex = (xpr_size_t)(sString - mString);

    return sFoundIndex;
}

xpr_size_t WideString::find(const xpr_wchar_t *aString, xpr_size_t aPos) const
{
    if (XPR_IS_NULL(aString))
    {
        return npos;
    }

    xpr_size_t sLength = wcslen(aString);

    return find(aString, aPos, sLength);
}

xpr_size_t WideString::find(xpr_wchar_t aChar, xpr_size_t aPos) const
{
    return find(&aChar, aPos, 1);
}

xpr_size_t WideString::rfind(const WideString &aString, xpr_size_t aPos) const
{
    return rfind(aString.mString, aPos, aString.mLength);
}

xpr_size_t WideString::rfind(const xpr_wchar_t *aString, xpr_size_t aPos, xpr_size_t aLength) const
{
    if (XPR_IS_NULL(aString) || (aPos != npos && aPos >= mLength))
    {
        return npos;
    }

    if (aLength == 0)
    {
        return 0;
    }

    if (aPos == npos)
    {
        aPos = mLength - 1;
    }

    const xpr_wchar_t *sSearch = aString;
    xpr_size_t sLength = wcslen(aString);
    if (sLength > aLength)
        sLength = aLength;

    xpr_sint_t sPos = (xpr_sint_t)aPos;
    const xpr_wchar_t *sResult = XPR_NULL;

    if (mLength == 0 || mLength < sLength)
    {
        return XPR_NULL;
    }

    while (sPos >= 0)
    {
        if (wcsncmp(mString + sPos, sSearch, sLength) == 0)
        {
            sResult = (xpr_wchar_t *)mString + sPos;
            break;
        }

        sPos--;
    }

    if (XPR_IS_NULL(sResult))
    {
        return npos;
    }

    xpr_size_t sFoundIndex = (xpr_size_t)(sResult - mString);

    return sFoundIndex;
}

xpr_size_t WideString::rfind(const xpr_wchar_t *aString, xpr_size_t aPos) const
{
    if (XPR_IS_NULL(aString))
    {
        return npos;
    }

    xpr_size_t sLength = wcslen(aString);

    return rfind(aString, aPos, sLength);
}

xpr_size_t WideString::rfind(xpr_wchar_t aChar, xpr_size_t aPos) const
{
    return rfind(&aChar, aPos, 1);
}

xpr_size_t WideString::find_first_of(const WideString &aString, xpr_size_t aPos) const
{
    return find_first_of(aString.mString, aPos, aString.mLength);
}

xpr_size_t WideString::find_first_of(const xpr_wchar_t *aString, xpr_size_t aPos, xpr_size_t aLength) const
{
    if (XPR_IS_NULL(aString) || (aPos != npos && aPos >= mLength))
    {
        return npos;
    }

    xpr_size_t sFoundIndex = npos;
    const xpr_wchar_t *sString = mString + aPos;
    const xpr_wchar_t *sEndString = mString + mLength - 1;
    const xpr_wchar_t *sResult;

    do
    {
        sResult = (const xpr_wchar_t *)wmemchr(aString, *sString, aLength);
        if (XPR_IS_NOT_NULL(sResult))
        {
            sFoundIndex = (xpr_size_t)(sString - mString);
            break;
        }
    } while (++sString <= sEndString);

    return sFoundIndex;
}

xpr_size_t WideString::find_first_of(const xpr_wchar_t *aString, xpr_size_t aPos) const
{
    if (XPR_IS_NULL(aString))
    {
        return npos;
    }

    xpr_size_t sLength = wcslen(aString);

    return find_first_of(aString, aPos, sLength);
}

xpr_size_t WideString::find_first_of(xpr_wchar_t aChar, xpr_size_t aPos) const
{
    return find_first_of(&aChar, aPos, 1);
}

xpr_size_t WideString::find_last_of(const WideString &aString, xpr_size_t aPos) const
{
    return find_last_of(aString.mString, aPos, aString.mLength);
}

xpr_size_t WideString::find_last_of(const xpr_wchar_t *aString, xpr_size_t aPos, xpr_size_t aLength) const
{
    if (XPR_IS_NULL(aString) || (aPos != npos && aPos >= mLength))
    {
        return npos;
    }

    if (aPos == npos)
    {
        aPos = mLength - 1;
    }

    xpr_size_t sFoundIndex = npos;
    const xpr_wchar_t *sString = mString + aPos;
    const xpr_wchar_t *sResult;

    do
    {
        sResult = (const xpr_wchar_t *)wmemchr(aString, *sString, aLength);
        if (XPR_IS_NOT_NULL(sResult))
        {
            sFoundIndex = (xpr_size_t)(sString - mString);
            break;
        }
    } while (--sString >= mString);

    return sFoundIndex;
}

xpr_size_t WideString::find_last_of(const xpr_wchar_t *aString, xpr_size_t aPos) const
{
    if (XPR_IS_NULL(aString))
    {
        return npos;
    }

    xpr_size_t sLength = wcslen(aString);

    return find_last_of(aString, aPos, sLength);
}

xpr_size_t WideString::find_last_of(xpr_wchar_t aChar, xpr_size_t aPos) const
{
    return find_last_of(&aChar, aPos, 1);
}

xpr_size_t WideString::find_first_not_of(const WideString &aString, xpr_size_t aPos) const
{
    return find_first_not_of(aString.mString, aPos, aString.mLength);
}

xpr_size_t WideString::find_first_not_of(const xpr_wchar_t *aString, xpr_size_t aPos, xpr_size_t aLength) const
{
    if (XPR_IS_NULL(aString) || (aPos != npos && aPos >= mLength))
    {
        return npos;
    }

    xpr_size_t sFoundIndex = npos;
    const xpr_wchar_t *sString = mString + aPos;
    const xpr_wchar_t *sEndString = mString + mLength - 1;
    const xpr_wchar_t *sResult;

    do
    {
        sResult = (const xpr_wchar_t *)wmemchr(aString, *sString, aLength);
        if (XPR_IS_NULL(sResult))
        {
            sFoundIndex = (xpr_size_t)(sString - mString);
            break;
        }
    } while (++sString <= sEndString);

    return sFoundIndex;
}

xpr_size_t WideString::find_first_not_of(const xpr_wchar_t *aString, xpr_size_t aPos) const
{
    if (XPR_IS_NULL(aString))
    {
        return npos;
    }

    xpr_size_t sLength = wcslen(aString);

    return find_first_not_of(aString, aPos, sLength);
}

xpr_size_t WideString::find_first_not_of(xpr_wchar_t aChar, xpr_size_t aPos) const
{
    return find_first_not_of(&aChar, aPos, 1);
}

xpr_size_t WideString::find_last_not_of(const WideString &aString, xpr_size_t aPos) const
{
    return find_last_not_of(aString.mString, aPos, aString.mLength);
}

xpr_size_t WideString::find_last_not_of(const xpr_wchar_t *aString, xpr_size_t aPos, xpr_size_t aLength) const
{
    if (XPR_IS_NULL(aString) || (aPos != npos && aPos >= mLength))
    {
        return npos;
    }

    if (aPos == npos)
    {
        aPos = mLength - 1;
    }

    xpr_size_t sFoundIndex = npos;
    const xpr_wchar_t *sString = mString + aPos;
    const xpr_wchar_t *sResult;

    do
    {
        sResult = (const xpr_wchar_t *)wmemchr(aString, *sString, aLength);
        if (XPR_IS_NULL(sResult))
        {
            sFoundIndex = (xpr_size_t)(sString - mString);
            break;
        }
    } while (--sString >= mString);

    return sFoundIndex;
}

xpr_size_t WideString::find_last_not_of(const xpr_wchar_t *aString, xpr_size_t aPos) const
{
    if (XPR_IS_NULL(aString))
    {
        return npos;
    }

    xpr_size_t sLength = wcslen(aString);

    return find_last_not_of(aString, aPos, sLength);
}

xpr_size_t WideString::find_last_not_of(xpr_wchar_t aChar, xpr_size_t aPos) const
{
    return find_last_not_of(&aChar, aPos, 1);
}

WideString WideString::substr(xpr_size_t aPos, xpr_size_t aLength) const
{
    if (aPos != npos && aPos >= mLength)
    {
        return WideString();
    }

    xpr_size_t sLength = aLength;
    if ((aPos + sLength) > mLength)
        sLength = mLength - aPos;

    return WideString(*this, aPos, sLength);
}

xpr_sint_t WideString::compare(const WideString &aString) const
{
    return compare(0, mLength, aString.mString, aString.mLength);
}

xpr_sint_t WideString::compare(const xpr_wchar_t *aString) const
{
    if (XPR_IS_NULL(aString))
        return 1;

    xpr_size_t sLength = wcslen(aString);

    return compare(0, mLength, aString, sLength);
}

xpr_sint_t WideString::compare(xpr_size_t aPos, xpr_size_t aLength, const WideString &aString) const
{
    return compare(aPos, aLength, aString.mString, aString.mLength);
}

xpr_sint_t WideString::compare(xpr_size_t aPos, xpr_size_t aLength, const xpr_wchar_t *aString) const
{
    if (XPR_IS_NULL(aString))
        return 1;

    xpr_size_t sLength = wcslen(aString);

    return compare(aPos, aLength, aString, sLength);
}

xpr_sint_t WideString::compare(xpr_size_t aPos1, xpr_size_t aLength1, const WideString &aString, xpr_size_t aPos2, xpr_size_t aLength2) const
{
    return compare(aPos1, aLength1, aString.mString + aPos2, aLength2);
}

xpr_sint_t WideString::compare(xpr_size_t aPos1, xpr_size_t aLength1, const xpr_wchar_t *aString, xpr_size_t aLength2) const
{
    XPR_ASSERT(aPos1 <= mLength);

    if (mLength <= (aPos1 + aLength1))
        aLength1 = mLength - aPos1;

    xpr_sint_t sCompare = wmemcmp(mString + aPos1, aString, (aLength1 < aLength2) ? aLength1 : aLength2);
    if (sCompare != 0)
        return sCompare;

    if (aLength1 == aLength2)
        return 0;

    return (aLength1 < aLength2) ? -1 : 1;
}

xpr_sint_t WideString::compare_incase(const WideString &aString) const
{
    return compare_incase(0, mLength, aString.mString, aString.mLength);
}

xpr_sint_t WideString::compare_incase(const xpr_wchar_t *aString) const
{
    if (XPR_IS_NULL(aString))
        return 1;

    xpr_size_t sLength = wcslen(aString);

    return compare_incase(0, mLength, aString, sLength);
}

xpr_sint_t WideString::compare_incase(xpr_size_t aPos, xpr_size_t aLength, const WideString &aString) const
{
    return compare_incase(aPos, aLength, aString.mString, aString.mLength);
}

xpr_sint_t WideString::compare_incase(xpr_size_t aPos, xpr_size_t aLength, const xpr_wchar_t *aString) const
{
    if (XPR_IS_NULL(aString))
        return 1;

    xpr_size_t sLength = wcslen(aString);

    return compare_incase(aPos, aLength, aString, sLength);
}

xpr_sint_t WideString::compare_incase(xpr_size_t aPos1, xpr_size_t aLength1, const WideString &aString, xpr_size_t aPos2, xpr_size_t aLength2) const
{
    return compare_incase(aPos1, aLength1, aString.mString + aPos2, aLength2);
}

xpr_sint_t WideString::compare_incase(xpr_size_t aPos1, xpr_size_t aLength1, const xpr_wchar_t *aString, xpr_size_t aLength2) const
{
    XPR_ASSERT(aPos1 <= mLength);

    if (mLength <= (aPos1 + aLength1))
        aLength1 = mLength - aPos1;

    xpr_sint_t sCompare = _wcsnicmp(mString + aPos1, aString, (aLength1 < aLength2) ? aLength1 : aLength2);
    if (sCompare != 0)
        return sCompare;

    if (aLength1 == aLength2)
        return 0;

    return (aLength1 < aLength2) ? -1 : 1;
}
} // namespace xpr
