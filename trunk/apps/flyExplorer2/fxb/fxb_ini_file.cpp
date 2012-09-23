//
// Copyright (c) 2001-2012 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "fxb_ini_file.h"

namespace fxb
{
#define MAX_FGETS_LINE      5120
#define MAX_KEY             128
#define MAX_ENTRY           128
#define MAX_VALUE           300
#define MAX_COMMENT         2048

IniFile::IniFile(const xpr_tchar_t *aIniPath)
    : mCaseInsensitive(XPR_TRUE)
{
    mPath[0] = XPR_STRING_LITERAL('\0');

    if (XPR_IS_NOT_NULL(aIniPath))
        _tcscpy(mPath, aIniPath);
}

IniFile::~IniFile(void)
{
    erase(XPR_TRUE);
}

const xpr_tchar_t *IniFile::getPath(void)
{
    return mPath;
}

void IniFile::setPath(const xpr_tchar_t *aIniPath)
{
    if (XPR_IS_NOT_NULL(aIniPath))
        _tcscpy(mPath, aIniPath);
}

xpr_bool_t IniFile::readFile(void)
{
#ifdef XPR_CFG_UNICODE
    return readFileW();
#else
    return readFileA();
#endif
}

xpr_bool_t IniFile::writeFile(void)
{
#ifdef XPR_CFG_UNICODE
    return writeFileW();
#else
    return writeFileA();
#endif
}

xpr_bool_t IniFile::readFileA(void)
{
    FILE *sFile = _tfopen(mPath, XPR_STRING_LITERAL("rt"));
    if (XPR_IS_NULL(sFile))
        return XPR_FALSE;

    xpr_char_t sLineA[MAX_FGETS_LINE + 1] = {0};
    xpr_tchar_t sLine[MAX_FGETS_LINE + 1] = {0};
    xpr_tchar_t sKey[MAX_KEY + 1] = {0};
    xpr_size_t sOutputBytes;
    xpr_char_t sKeyA[MAX_KEY + 1] = {0};
    xpr_char_t *sSubA, *sSub2A;
    xpr_tchar_t sSub[MAX_FGETS_LINE + 1] = {0};

    sKeyA[0] = '\0';

    while (fgets(sLineA, MAX_FGETS_LINE, sFile))
    {
        if (sLineA[strlen(sLineA) - 1] == '\r')
            sLineA[strlen(sLineA) - 1] = '\0';

        if (sLineA[strlen(sLineA) - 1] == '\n')
            sLineA[strlen(sLineA) - 1] = '\0';

        if (sLineA[0] != 0)
        {
            if (isprint(sLineA[0]) == XPR_FALSE)
            {
                fclose(sFile);
                return XPR_FALSE;
            }

            switch (sLineA[0])
            {
            case '[':
                sSubA = strchr(sLineA, ']');
                if (XPR_IS_NOT_NULL(sSubA))
                {
                    sSubA[0] = '\0';

                    strcpy(sKeyA, sLineA+1);

                    sOutputBytes = MAX_KEY * sizeof(xpr_tchar_t);
                    XPR_MBS_TO_TCS(sLineA + 1, strlen(sLineA + 1) * sizeof(xpr_char_t), sKey, &sOutputBytes);
                    sKey[sOutputBytes / sizeof(xpr_tchar_t)] = 0;

                    addKeyName(sKey);
                }
                break;

            case ';':
            case '#':
                {
                    sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_tchar_t);
                    XPR_MBS_TO_TCS(sLineA + 1, strlen(sLineA + 1) * sizeof(xpr_char_t), sLine, &sOutputBytes);
                    sLine[sOutputBytes / sizeof(xpr_tchar_t)] = 0;

                    setComment(sLine);
                    break;
                }

            default:
                sSubA = strchr(sLineA, '=');
                if (XPR_IS_NOT_NULL(sSubA))
                {
                    sSub2A = sSubA;

                    if (sSub2A[0] == ' ')
                    {
                        while (sSub2A[0] == ' ')
                            sSub2A--;
                        sSub2A++;
                    }
                    sSub2A[0] = '\0';

                    while (sSubA[0] == ' ')
                        sSubA++;
                    sSubA++;

                    sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_tchar_t);
                    XPR_MBS_TO_TCS(sLineA, strlen(sLineA) * sizeof(xpr_char_t), sLine, &sOutputBytes);
                    sLine[sOutputBytes / sizeof(xpr_tchar_t)] = 0;

                    sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_tchar_t);
                    XPR_MBS_TO_TCS(sSubA, strlen(sSubA) * sizeof(xpr_char_t), sSub, &sOutputBytes);
                    sSub[sOutputBytes / sizeof(xpr_tchar_t)] = 0;

                    setValueS(sKey, sLine, sSub);
                }
                break;
            }
        }
    }

    fclose(sFile);

    return mKeys.empty() ? XPR_FALSE : XPR_TRUE;
}

xpr_bool_t IniFile::readFileW(void)
{
    FILE *sFile = _tfopen(mPath, XPR_STRING_LITERAL("rb"));
    if (XPR_IS_NULL(sFile))
        return XPR_FALSE;

    xpr_wchar_t sLineW[MAX_FGETS_LINE + 1] = {0};
    xpr_tchar_t sLine[MAX_FGETS_LINE + 1] = {0};

    fread(sLineW, 2, 1, sFile);
    if (sLineW[0] != 0xFEFF)
    {
        fclose(sFile);
        return XPR_FALSE;
    }

    xpr_size_t sLen;
    xpr_size_t sRead;
    xpr_tchar_t sKey[MAX_KEY + 1] = {0};
    xpr_size_t sOutputBytes;
    xpr_wchar_t *sSubW, *sSub2W;
    xpr_tchar_t sSub[MAX_FGETS_LINE + 1] = {0};

    sKey[0] = 0;

    while (true)
    {
        sLen = 0;

        while (true)
        {
            sRead = fread(sLineW + sLen, sizeof(xpr_wchar_t), 1, sFile);
            if (sRead > 0)
                sLen++;

            if (sLen >= 1 && sLineW[sLen-1] == L'\n')
            {
                sLineW[sLen-1] = L'\0';
                sLen--;

                if (sLen >= 1 && sLineW[sLen-1] == L'\r')
                {
                    sLineW[sLen-1] = L'\0';
                    sLen--;
                }

                break;
            }

            if (sRead <= 0)
                break;
        }

        if (sLineW[0] != 0)
        {
            switch (sLineW[0])
            {
            case L'[':
                sSubW = wcsrchr(sLineW, L']'); // ex) [c:\[123]]
                if (XPR_IS_NOT_NULL(sSubW))
                {
                    sSubW[0] = XPR_STRING_LITERAL('\0');

                    sOutputBytes = MAX_KEY * sizeof(xpr_tchar_t);
                    XPR_UTF16_TO_TCS(sLineW + 1, wcslen(sLineW + 1) * sizeof(xpr_wchar_t), sKey, &sOutputBytes);
                    sKey[sOutputBytes / sizeof(xpr_tchar_t)] = 0;

                    addKeyName(sKey);
                }
                break;

            case L';':
            case L'#':
                {
                    sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_tchar_t);
                    XPR_UTF16_TO_TCS(sLineW + 1, wcslen(sLineW + 1) * sizeof(xpr_wchar_t), sLine, &sOutputBytes);
                    sLine[sOutputBytes / sizeof(xpr_tchar_t)] = 0;

                    setComment(sLine);
                    break;
                }

            default:
                sSubW = wcschr(sLineW, L'=');
                if (XPR_IS_NOT_NULL(sSubW))
                {
                    sSub2W = sSubW;

                    if (sSub2W[0] == L' ')
                    {
                        while (sSub2W[0] == L' ')
                            sSub2W--;
                        sSub2W++;
                    }
                    sSub2W[0] = L'\0';

                    while (sSubW[0] == L' ')
                        sSubW++;
                    sSubW++;

                    sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_tchar_t);
                    XPR_UTF16_TO_TCS(sLineW, wcslen(sLineW) * sizeof(xpr_wchar_t), sLine, &sOutputBytes);
                    sLine[sOutputBytes / sizeof(xpr_tchar_t)] = 0;

                    sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_tchar_t);
                    XPR_UTF16_TO_TCS(sSubW, wcslen(sSubW) * sizeof(xpr_wchar_t), sSub, &sOutputBytes);
                    sSub[sOutputBytes / sizeof(xpr_tchar_t)] = 0;

                    setValueS(sKey, sLine, sSub);
                }
                break;
            }
        }

        if (sRead <= 0)
            break;
    }

    fclose(sFile);

    return mKeys.empty() ? XPR_FALSE : XPR_TRUE;
}

xpr_bool_t IniFile::writeFileA(void)
{
    FILE *sFile = _tfopen(mPath, XPR_STRING_LITERAL("wt"));
    if (XPR_IS_NULL(sFile))
        return XPR_FALSE;

    {
        xpr_tchar_t *sComment;
        xpr_char_t sCommentA[MAX_FGETS_LINE + 1];
        xpr_size_t sOutputBytes;

        std::vector<xpr_tchar_t *>::iterator sIterator = mComments.begin();
        for (; sIterator != mComments.end(); ++sIterator)
        {
            sComment = *sIterator;

            sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_char_t);
            XPR_TCS_TO_MBS(sComment, _tcslen(sComment) * sizeof(xpr_tchar_t), sCommentA, &sOutputBytes);
            sCommentA[sOutputBytes / sizeof(xpr_char_t)] = 0;

            fprintf(sFile, ";%s%s", sCommentA, XPR_EOLA);
        }

        if (mComments.empty() == false)
            fprintf(sFile, "%s", XPR_EOLA);
    }

    {
        std::vector<Key>::iterator sIterator;
        std::vector<Entry>::iterator sEntryIterator;

        xpr_char_t sNameA[MAX_FGETS_LINE + 1];
        xpr_char_t sEntryA[MAX_FGETS_LINE + 1];
        xpr_char_t sValueA[MAX_FGETS_LINE + 1];
        xpr_size_t sOutputBytes;

        sIterator = mKeys.begin();
        for (; sIterator != mKeys.end(); ++sIterator)
        {
            sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_char_t);
            XPR_TCS_TO_MBS(sIterator->mName, _tcslen(sIterator->mName) * sizeof(xpr_tchar_t), sNameA, &sOutputBytes);
            sNameA[sOutputBytes / sizeof(xpr_char_t)] = 0;

            fprintf(sFile, "[%s]%s", sNameA, XPR_EOLA);

            sEntryIterator = sIterator->mEntryVector.begin();
            for (; sEntryIterator != sIterator->mEntryVector.end(); ++sEntryIterator)
            {
                sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_char_t);
                XPR_TCS_TO_MBS(sEntryIterator->mEntry, _tcslen(sEntryIterator->mEntry) * sizeof(xpr_tchar_t), sEntryA, &sOutputBytes);
                sEntryA[sOutputBytes / sizeof(xpr_char_t)] = 0;

                sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_char_t);
                XPR_TCS_TO_MBS(sEntryIterator->mValue, _tcslen(sEntryIterator->mValue) * sizeof(xpr_tchar_t), sValueA, &sOutputBytes);
                sValueA[sOutputBytes / sizeof(xpr_char_t)] = 0;

                fprintf(sFile, "%s=%s%s", sEntryA, sValueA, XPR_EOLA);
            }

            fprintf(sFile, "%s", XPR_EOLA);
        }
    }

    fclose(sFile);

    return XPR_TRUE;
}

xpr_bool_t IniFile::writeFileW(void)
{
    FILE *sFile = _tfopen(mPath, XPR_STRING_LITERAL("wb"));
    if (XPR_IS_NULL(sFile))
        return XPR_FALSE;

    xpr_uint_t sCode = 0xFEFF;
    fwrite(&sCode, 2, 1, sFile);

    xpr_tchar_t *sComment;
    xpr_wchar_t sKeyW[MAX_KEY + 1];
    xpr_wchar_t sEntryW[MAX_ENTRY + 1];
    xpr_wchar_t sValueW[MAX_VALUE + 1];
    xpr_wchar_t sCommentW[MAX_FGETS_LINE + 1];
    xpr_wchar_t sLineW[MAX_FGETS_LINE + 1];
    xpr_size_t sOutputBytes;

    {
        std::vector<xpr_tchar_t *>::iterator sIterator = mComments.begin();
        for (; sIterator != mComments.end(); ++sIterator)
        {
            sComment = *sIterator;

            sOutputBytes = MAX_FGETS_LINE * sizeof(xpr_wchar_t);
            XPR_TCS_TO_UTF16(sComment, _tcslen(sComment) * sizeof(xpr_tchar_t), sCommentW, &sOutputBytes);
            sCommentW[sOutputBytes / sizeof(xpr_wchar_t)] = 0;

            swprintf(sLineW, XPR_WIDE_STRING_LITERAL(";%s%s"), sCommentW, XPR_EOLW);
            fwrite(sLineW, wcslen(sLineW)*sizeof(xpr_wchar_t), 1, sFile);
        }

        if (mComments.empty() == false)
        {
            swprintf(sLineW, XPR_WIDE_STRING_LITERAL("%s"), XPR_EOLW);
            fwrite(sLineW, wcslen(sLineW)*sizeof(xpr_wchar_t), 1, sFile);
        }
    }

    {
        std::vector<Key>::iterator sIterator;
        std::vector<Entry>::iterator sEntryIterator;

        sIterator = mKeys.begin();
        for (; sIterator != mKeys.end(); ++sIterator)
        {
            sOutputBytes = MAX_KEY * sizeof(xpr_wchar_t);
            XPR_TCS_TO_UTF16(sIterator->mName, _tcslen(sIterator->mName) * sizeof(xpr_tchar_t), sKeyW, &sOutputBytes);
            sKeyW[sOutputBytes / sizeof(xpr_wchar_t)] = 0;

            swprintf(sLineW, XPR_WIDE_STRING_LITERAL("[%s]%s"), sKeyW, XPR_EOLW);
            fwrite(sLineW, wcslen(sLineW)*sizeof(xpr_wchar_t), 1, sFile);

            sEntryIterator = sIterator->mEntryVector.begin();
            for (; sEntryIterator != sIterator->mEntryVector.end(); ++sEntryIterator)
            {
                sOutputBytes = MAX_ENTRY * sizeof(xpr_wchar_t);
                XPR_TCS_TO_UTF16(sEntryIterator->mEntry, _tcslen(sEntryIterator->mEntry) * sizeof(xpr_tchar_t), sEntryW, &sOutputBytes);
                sEntryW[sOutputBytes / sizeof(xpr_wchar_t)] = 0;

                sOutputBytes = MAX_VALUE * sizeof(xpr_wchar_t);
                XPR_TCS_TO_UTF16(sEntryIterator->mValue, _tcslen(sEntryIterator->mValue) * sizeof(xpr_tchar_t), sValueW, &sOutputBytes);
                sValueW[sOutputBytes / sizeof(xpr_wchar_t)] = 0;

                swprintf(sLineW, XPR_WIDE_STRING_LITERAL("%s=%s%s"), sEntryW, sValueW, XPR_EOLW);
                fwrite(sLineW, wcslen(sLineW)*sizeof(xpr_wchar_t), 1, sFile);
            }

            swprintf(sLineW, XPR_WIDE_STRING_LITERAL("%s"), XPR_EOLW);
            fwrite(sLineW, wcslen(sLineW)*sizeof(xpr_wchar_t), 1, sFile);
        }
    }

    fclose(sFile);

    return XPR_TRUE;
}

xpr_size_t IniFile::findKey(const xpr_tchar_t *aKeyName)
{
    xpr_size_t i;
    std::vector<Key>::iterator sIterator;

    sIterator = mKeys.begin();
    for (i = 0; sIterator != mKeys.end(); ++sIterator, ++i)
    {
        if (compare(sIterator->mName, aKeyName) == 0)
            return i;
    }

    return InvalidIndex;
}

xpr_size_t IniFile::findEntry(xpr_size_t aKey, const xpr_tchar_t *aEntry)
{
    if (!XPR_STL_IS_INDEXABLE(aKey, mKeys))
        return InvalidIndex;

    xpr_size_t i;
    std::vector<Entry>::iterator sIterator;

    sIterator = mKeys[aKey].mEntryVector.begin();
    for (i = 0; sIterator != mKeys[aKey].mEntryVector.end(); ++sIterator, ++i)
    {
        if (compare(sIterator->mEntry, aEntry) == 0)
            return i;
    }

    return InvalidIndex;
}

xpr_size_t IniFile::findEntry(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry)
{
    if (XPR_IS_NULL(aKey) || XPR_IS_NULL(aEntry))
        return InvalidIndex;

    xpr_size_t sKey = findKey(aKey);
    if (sKey == InvalidIndex)
        return InvalidIndex;

    xpr_size_t i;
    std::vector<Entry>::iterator sIterator;

    sIterator = mKeys[sKey].mEntryVector.begin();
    for (i = 0; sIterator != mKeys[sKey].mEntryVector.end(); ++sIterator, ++i)
    {
        if (compare(sIterator->mEntry, aEntry) == 0)
            return i;
    }

    return InvalidIndex;
}

xpr_size_t IniFile::addKeyName(const xpr_tchar_t *aKey)
{
    xpr_size_t sKey = findKey(aKey);
    if (sKey == InvalidIndex)
    {
        xpr_tchar_t *sKey2 = new xpr_tchar_t[_tcslen(aKey)+1];
        _tcscpy(sKey2, aKey);

        Key key;
        key.mName = sKey2;
        key.mSort = InvalidIndex;

        std::map<std::tstring, xpr_size_t>::iterator sIterator = mSortKey.find(aKey);
        if (sIterator != mSortKey.end())
            key.mSort = sIterator->second;

        mKeys.resize(mKeys.size() + 1, key);

        sKey = mKeys.size() - 1;
    }

    return sKey;
}

const xpr_tchar_t *IniFile::getKeyName(xpr_size_t aKeyIndex)
{
    if (!XPR_STL_IS_INDEXABLE(aKeyIndex, mKeys))
        return XPR_NULL;

    return mKeys[aKeyIndex].mName;
}

bool IniFile::sortKeyName(IniFile::Key &aKey1, IniFile::Key &aKey2)
{
    if (aKey1.mSort == InvalidIndex) return false;
    if (aKey2.mSort == InvalidIndex) return true;

    return aKey1.mSort < aKey2.mSort;
}

void IniFile::sortKey(void)
{
    std::sort(mKeys.begin(), mKeys.end(), sortKeyName);
}

xpr_bool_t IniFile::setSortKey(const xpr_tchar_t *aKey, xpr_size_t aSort)
{
    if (XPR_IS_NULL(aKey))
        return XPR_FALSE;

    mSortKey[aKey] = aSort;

    xpr_size_t sKey = findKey(aKey);
    if (sKey == InvalidIndex)
        return XPR_FALSE;

    mKeys[sKey].mSort = aSort;

    return XPR_TRUE;
}

xpr_size_t IniFile::getKeyCount(void)
{
    return mKeys.size();
}

xpr_size_t IniFile::getEntryCount(xpr_size_t aKeyIndex)
{
    if (!XPR_STL_IS_INDEXABLE(aKeyIndex, mKeys))
        return 0;

    return mKeys[aKeyIndex].mEntryVector.size();
}

xpr_size_t IniFile::getEntryCount(const xpr_tchar_t *aKeyName)
{
    xpr_size_t sKeyIndex = findKey(aKeyName);
    if (sKeyIndex == InvalidIndex)
        return 0;

    return mKeys[sKeyIndex].mEntryVector.size();
}

const xpr_tchar_t *IniFile::getEntryName(xpr_size_t aKey, xpr_size_t aEntry)
{
    if (!XPR_STL_IS_INDEXABLE(aKey, mKeys))
        return XPR_NULL;

    if (!XPR_STL_IS_INDEXABLE(aEntry, mKeys[aKey].mEntryVector))
        return XPR_NULL;

    return mKeys[aKey].mEntryVector[aEntry].mEntry;
}

const xpr_tchar_t *IniFile::getValueName(xpr_size_t aKeyIndex, xpr_size_t aValueIndex)
{
    if (!XPR_STL_IS_INDEXABLE(aKeyIndex, mKeys))
        return XPR_NULL;

    if (!XPR_STL_IS_INDEXABLE(aValueIndex, mKeys[aKeyIndex].mEntryVector))
        return XPR_NULL;

    return mKeys[aKeyIndex].mEntryVector[aValueIndex].mEntry;
}

const xpr_tchar_t *IniFile::getValueName(const xpr_tchar_t *aKeyName, xpr_size_t aValueIndex)
{
    xpr_size_t sKeyIndex = findKey(aKeyName);
    if (sKeyIndex == InvalidIndex)
        return XPR_NULL;

    return getValueName(sKeyIndex, aValueIndex);
}

xpr_bool_t IniFile::setValue(xpr_size_t aKey, xpr_size_t aEntry, const xpr_tchar_t *aValue)
{
    if (!XPR_STL_IS_INDEXABLE(aKey, mKeys))
        return XPR_FALSE;

    if (!XPR_STL_IS_INDEXABLE(aEntry, mKeys[aKey].mEntryVector))
        return XPR_FALSE;

    xpr_tchar_t *sValue2 = new xpr_tchar_t[_tcslen(aValue) + 1];
    _tcscpy(sValue2, aValue);

    delete[] mKeys[aKey].mEntryVector[aEntry].mValue;
    mKeys[aKey].mEntryVector[aEntry].mValue = sValue2;

    return XPR_FALSE;
}

xpr_bool_t IniFile::setValueS(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, const xpr_tchar_t *aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_size_t sKey = findKey(aKey);
    if (sKey == InvalidIndex)
    {
        if (XPR_IS_TRUE(aCreateIfNotExist))
            sKey = addKeyName(aKey);
        else
            return XPR_FALSE;
    }

    xpr_size_t sEntry = findEntry(sKey, aEntry);
    if (sEntry == InvalidIndex)
    {
        if (XPR_IS_FALSE(aCreateIfNotExist))
            return XPR_FALSE;

        Entry sEntry;
        sEntry.mEntry = new xpr_tchar_t[_tcslen(aEntry)+1];
        sEntry.mValue = new xpr_tchar_t[_tcslen(aValue)+1];
        _tcscpy(sEntry.mEntry, aEntry);
        _tcscpy(sEntry.mValue, aValue);

        mKeys[sKey].mEntryVector.resize(mKeys[sKey].mEntryVector.size() + 1, sEntry);
    }
    else
    {
        xpr_tchar_t *sValue2 = new xpr_tchar_t[_tcslen(aValue)+1];
        _tcscpy(sValue2, aValue);

        delete[] mKeys[sKey].mEntryVector[sEntry].mValue;
        mKeys[sKey].mEntryVector[sEntry].mValue = sValue2;
    }

    return XPR_TRUE;
}

xpr_bool_t IniFile::setValueI(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_sint_t aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%d"), aValue);
    return setValueS(aKey, aEntry, sValue);
}

xpr_bool_t IniFile::setValueU(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_uint_t aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%u"), aValue);
    return setValueS(aKey, aEntry, sValue);
}

xpr_bool_t IniFile::setValueI64(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_sint64_t aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%I64d"), aValue);
    return setValueS(aKey, aEntry, sValue);
}

xpr_bool_t IniFile::setValueU64(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_uint64_t aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%I64u"), aValue);
    return setValueS(aKey, aEntry, sValue);
}

xpr_bool_t IniFile::setValueB(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_bool_t bValue, xpr_bool_t aCreateIfNotExist)
{
    return setValueI(aKey, aEntry, (xpr_sint_t)bValue, aCreateIfNotExist);
}

xpr_bool_t IniFile::setValueC(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, COLORREF aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%d,%d,%d"), GetRValue(aValue), GetGValue(aValue), GetBValue(aValue));
    return setValueS(aKey, aEntry, sValue);
}

xpr_bool_t IniFile::setValueF(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_double_t aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%f"), aValue);
    return setValueS(aKey, aEntry, sValue);
}

xpr_bool_t IniFile::setValueR(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, CRect aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%d,%d,%d,%d"), aValue.left, aValue.top, aValue.right, aValue.bottom);
    return setValueS(aKey, aEntry, sValue);
}

xpr_bool_t IniFile::setValueZ(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, CSize aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%d,%d"), aValue.cx, aValue.cy);
    return setValueS(aKey, aEntry, sValue);
}

xpr_bool_t IniFile::setValueP(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, CPoint aValue, xpr_bool_t aCreateIfNotExist)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%d,%d"), aValue.x, aValue.y);
    return setValueS(aKey, aEntry, sValue);
}

xpr_bool_t IniFile::setValueV(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, const xpr_tchar_t *aFormat, ...)
{
    va_list args;
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};

    va_start(args, aFormat);
    _vstprintf(sValue, aFormat, args);
    va_end(args);

    return setValueS(aKey, aEntry, sValue);
}

const xpr_tchar_t *IniFile::getValue(xpr_size_t aKey, xpr_size_t aEntry, const xpr_tchar_t *aDefault)
{
    if (!XPR_STL_IS_INDEXABLE(aKey, mKeys))
        return aDefault;

    if (!XPR_STL_IS_INDEXABLE(aEntry, mKeys[aKey].mEntryVector))
        return aDefault;

    return mKeys[aKey].mEntryVector[aEntry].mValue;
}

const xpr_tchar_t *IniFile::getValueS(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, const xpr_tchar_t *aDefault)
{
    xpr_size_t sKey = findKey(aKey);
    if (sKey == InvalidIndex)
        return aDefault;

    xpr_size_t sEntry = findEntry(sKey, aEntry);
    if (sEntry == InvalidIndex)
        return aDefault;

    return mKeys[sKey].mEntryVector[sEntry].mValue;
}

xpr_sint_t IniFile::getValueI(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_sint_t aDefault)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%d"), aDefault);
    return _ttoi(getValueS(aKey, aEntry, sValue)); 
}

xpr_uint_t IniFile::getValueU(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_uint_t aDefault)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};
    _stprintf(sValue, XPR_STRING_LITERAL("%u"), aDefault);
    return _ttoi(getValueS(aKey, aEntry, sValue)); 
}

xpr_sint64_t IniFile::getValueI64(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_sint64_t aDefault)
{
    const xpr_tchar_t *sValue = getValueS(aKey, aEntry, XPR_NULL);
    if (XPR_IS_NULL(sValue))
        return aDefault;

    xpr_sint64_t nValue = aDefault;
    _stscanf(sValue, XPR_STRING_LITERAL("%I64d"), &nValue);

    return nValue;
}

xpr_uint64_t IniFile::getValueU64(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_uint64_t aDefault)
{
    const xpr_tchar_t *sValueText = getValueS(aKey, aEntry, XPR_NULL);
    if (XPR_IS_NULL(sValueText))
        return aDefault;

    xpr_uint64_t sValue = aDefault;
    _stscanf(sValueText, XPR_STRING_LITERAL("%I64u"), &sValue);

    return sValue;
}

xpr_bool_t IniFile::getValueB(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_bool_t aDefault)
{
    return (xpr_bool_t)getValueI(aKey, aEntry, (xpr_sint_t)aDefault);
}

xpr_sint_t IniFile::getKeyCompatibleValueI(const xpr_tchar_t *aKey1, const xpr_tchar_t *aKey2, const xpr_tchar_t *aEntry, xpr_sint_t aDefault)
{
    xpr_size_t sEntry = findEntry(aKey1, aEntry);
    if (sEntry != InvalidIndex)
        return getValueI(aKey1, aEntry, aDefault);

    return getValueI(aKey2, aEntry, aDefault);
}

xpr_bool_t IniFile::getKeyCompatibleValueB(const xpr_tchar_t *aKey1, const xpr_tchar_t *aKey2, const xpr_tchar_t *aEntry, xpr_bool_t aDefault)
{
    xpr_size_t sEntry = findEntry(aKey1, aEntry);
    if (sEntry != InvalidIndex)
        return getValueB(aKey1, aEntry, aDefault);

    return getValueB(aKey2, aEntry, aDefault);
}

xpr_sint_t IniFile::getEntryCompatibleValueI(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry1, const xpr_tchar_t *aEntry2, xpr_sint_t aDefault)
{
    xpr_size_t sEntry = findEntry(aKey, aEntry1);
    if (sEntry != InvalidIndex)
        return getValueI(aKey, aEntry1, aDefault);

    return getValueI(aKey, aEntry2, aDefault);
}

xpr_bool_t IniFile::getEntryCompatibleValueB(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry1, const xpr_tchar_t *aEntry2, xpr_bool_t aDefault)
{
    xpr_size_t sEntry = findEntry(aKey, aEntry1);
    if (sEntry != InvalidIndex)
        return getValueB(aKey, aEntry1, aDefault);

    return getValueB(aKey, aEntry2, aDefault);
}

COLORREF IniFile::getValueC(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, COLORREF aDefault)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};

    xpr_sint_t r = 0, g = 0, b = 0;
    _stprintf(sValue, XPR_STRING_LITERAL("%d,%d,%d"), GetRValue(aDefault), GetGValue(aDefault), GetBValue(aDefault));
    _stscanf(getValueS(aKey, aEntry, sValue), XPR_STRING_LITERAL("%d,%d,%d"), &r, &g, &b);

    return RGB((xpr_byte_t)r, (xpr_byte_t)g, (xpr_byte_t)b);
}

xpr_double_t IniFile::getValueF(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, xpr_double_t aDefault)
{
    xpr_tchar_t sDefValue[MAX_VALUE + 1] = {0};
    _stprintf(sDefValue, XPR_STRING_LITERAL("%f"), aDefault);

    const xpr_tchar_t *sValue = getValueS(aKey, aEntry, sDefValue);
    if (XPR_IS_NULL(sValue))
        return 0.0;

    xpr_char_t sValueA[MAX_VALUE + 1];
    xpr_size_t sOutputBytes;

    sOutputBytes = MAX_VALUE * sizeof(xpr_char_t);
    XPR_TCS_TO_MBS(sValue, _tcslen(sValue) * sizeof(xpr_tchar_t), sValueA, &sOutputBytes);
    sValueA[sOutputBytes / sizeof(xpr_char_t)] = 0;

    return atof(sValueA);
}

CRect IniFile::getValueR(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, CRect aDefault)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};

    CRect sRect;
    _stprintf(sValue, XPR_STRING_LITERAL("%d,%d,%d,%d"), aDefault.left, aDefault.top, aDefault.right, aDefault.bottom);
    _stscanf(getValueS(aKey, aEntry, sValue), XPR_STRING_LITERAL("%d,%d,%d,%d"), &sRect.left, &sRect.top, &sRect.right, &sRect.bottom);

    return sRect;
}

CSize IniFile::getValueZ(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, CSize aDefault)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};

    CSize sSize;
    _stprintf(sValue, XPR_STRING_LITERAL("%d,%d"), aDefault.cx, aDefault.cy);
    _stscanf(getValueS(aKey, aEntry, sValue), XPR_STRING_LITERAL("%d,%d"), &sSize.cx, &sSize.cy);

    return sSize;
}

CPoint IniFile::getValueP(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, CPoint aDefault)
{
    xpr_tchar_t sValue[MAX_VALUE + 1] = {0};

    CPoint sPoint;
    _stprintf(sValue, XPR_STRING_LITERAL("%d,%d"), aDefault.x, aDefault.y);
    _stscanf(getValueS(aKey, aEntry, sValue), XPR_STRING_LITERAL("%d,%d"), &sPoint.x, &sPoint.y);

    return sPoint;
}

xpr_sint_t IniFile::getValueV(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry, const xpr_tchar_t *aFormat,
                              void *v1, void *v2, void *v3, void *v4,
                              void *v5, void *v6, void *v7, void *v8,
                              void *v9, void *v10, void *v11, void *v12,
                              void *v13, void *v14, void *v15, void *v16)
{
    const xpr_tchar_t *sValue = getValueS(aKey, aEntry);
    if (sValue[0] != 0)
        return XPR_FALSE;

    xpr_sint_t sVals = _stscanf(sValue, aFormat,
        v1, v2, v3, v4, v5, v6, v7, v8,
        v9, v10, v11, v12, v13, v14, v15, v16);

    return sVals;
}

xpr_bool_t IniFile::deleteEntry(const xpr_tchar_t *aKey, const xpr_tchar_t *aEntry)
{
    xpr_size_t sKey = findKey(aKey);
    if (sKey == InvalidIndex)
        return XPR_FALSE;

    xpr_size_t sEntry = findEntry(sKey, aEntry);
    if (sEntry == InvalidIndex)
        return XPR_FALSE;

    std::vector<Entry>::iterator sIterator = mKeys[sKey].mEntryVector.begin() + sEntry;

    delete[] sIterator->mEntry;
    delete[] sIterator->mValue;

    mKeys[sKey].mEntryVector.erase(sIterator);

    return XPR_TRUE;
}

xpr_bool_t IniFile::deleteKey(const xpr_tchar_t *aKey)
{
    xpr_size_t sKey = findKey(aKey);
    if (sKey == InvalidIndex)
        return XPR_FALSE;

    std::vector<Entry>::iterator sIterator;

    sIterator = mKeys[sKey].mEntryVector.begin();
    for (; sIterator != mKeys[sKey].mEntryVector.end(); ++sIterator)
    {
        delete[] sIterator->mEntry;
        delete[] sIterator->mValue;
    }
    mKeys[sKey].mEntryVector.clear();

    delete[] mKeys[sKey].mName;

    mKeys.erase(mKeys.begin() + sKey);

    return XPR_TRUE;
}

void IniFile::deleteAllKey(void)
{
    std::vector<Key>::iterator sIterator;
    std::vector<Entry>::iterator sEntryIterator;

    sIterator = mKeys.begin();
    for (; sIterator != mKeys.end(); ++sIterator)
    {
        sEntryIterator = sIterator->mEntryVector.begin();
        for (; sEntryIterator != sIterator->mEntryVector.end(); ++sEntryIterator)
        {
            delete[] sEntryIterator->mEntry;
            delete[] sEntryIterator->mValue;
        }
        sIterator->mEntryVector.clear();

        delete[] sIterator->mName;
    }

    mKeys.clear();
}

void IniFile::erase(xpr_bool_t aWithSortKey)
{
    {
        std::vector<Key>::iterator sIterator;
        std::vector<Entry>::iterator sEntryIterator;

        sIterator = mKeys.begin();
        for (; sIterator != mKeys.end(); ++sIterator)
        {
            delete[] sIterator->mName;

            sEntryIterator = sIterator->mEntryVector.begin();
            for (; sEntryIterator != sIterator->mEntryVector.end(); ++sEntryIterator)
            {
                delete[] sEntryIterator->mEntry;
                delete[] sEntryIterator->mValue;
            }
        }
    }

    {
        std::vector<xpr_tchar_t *>::iterator sIterator = mComments.begin();
        for (; sIterator != mComments.end(); ++sIterator)
        {
            delete[] *sIterator;
        }
    }

    mKeys.clear();
    mComments.clear();

    if (XPR_IS_TRUE(aWithSortKey))
        mSortKey.clear();
}

void IniFile::clear(xpr_bool_t aWithSortKey)
{
    erase(aWithSortKey);
}

void IniFile::reset(xpr_bool_t aWithSortKey)
{
    erase(aWithSortKey);
}

xpr_size_t IniFile::getCommentCount(void)
{
    return mComments.size();
}

void IniFile::setComment(const xpr_tchar_t *aComment)
{
    xpr_tchar_t *sComment2 = new xpr_tchar_t[_tcslen(aComment)+1];
    _tcscpy(sComment2, aComment);

    mComments.resize(mComments.size() + 1, sComment2);
}

const xpr_tchar_t *IniFile::getComment(xpr_size_t aComment)
{
    if (!XPR_STL_IS_INDEXABLE(aComment, mComments))
        return XPR_NULL;

    return mComments[aComment];
}

xpr_bool_t IniFile::deleteComment(xpr_size_t aComment)
{
    if (!XPR_STL_IS_INDEXABLE(aComment, mComments))
        return XPR_FALSE;

    std::vector<xpr_tchar_t *>::iterator sIterator = mComments.begin() + aComment;

    delete[] *sIterator;

    mComments.erase(sIterator);

    return XPR_TRUE;
}

void IniFile::deleteAllComments(void)
{
    std::vector<xpr_tchar_t *>::iterator sIterator;

    sIterator = mComments.begin();
    for (; sIterator != mComments.end(); ++sIterator)
    {
        delete[] *sIterator;
    }

    mComments.clear();
}

xpr_sint_t IniFile::compare(const xpr_tchar_t *aString1, const xpr_tchar_t *aString2)
{
    if (XPR_IS_TRUE(mCaseInsensitive))
        return _tcsicmp(aString1, aString2);

    return _tcscmp(aString1, aString2);
}
} // namespace fxb
