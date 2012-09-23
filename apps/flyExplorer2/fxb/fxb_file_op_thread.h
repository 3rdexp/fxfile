//
// Copyright (c) 2001-2012 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXB_FILE_OP_THREAD_H__
#define __FXB_FILE_OP_THREAD_H__
#pragma once

namespace fxb
{
typedef struct HandleToMappings
{
    xpr_uint_t      mNumberOfMappings;  // number of mappings in array
    LPSHNAMEMAPPING mSHNameMapping;    // pointer to array of mappings
} HandleToMappings;

typedef struct PasteSelItems
{
    const xpr_tchar_t *mTarget;
    const xpr_tchar_t *mSource;
    HandleToMappings  *mHandleToMappings;
} PasteSelItems;

class FileOpThread : public CWnd
{
public:
    FileOpThread(void);

public:
    void start(
        SHFILEOPSTRUCT *aShFileOpStruct,
        xpr_bool_t      aSelectItem = XPR_FALSE,
        HWND            aNotifyHwnd = XPR_NULL,
        xpr_uint_t      aMsg = 0);
    void start(
        xpr_bool_t      aCopy,
        xpr_tchar_t    *aSource,
        xpr_sint_t      aSourceCount,
        xpr_tchar_t    *aTarget,
        xpr_bool_t      aDuplicate = XPR_FALSE,
        xpr_bool_t      aSelectedItem = XPR_FALSE,
        HWND            aNotifyHwnd = XPR_NULL,
        xpr_uint_t      aMsg = 0);

    void setUndo(xpr_bool_t aUndo);

protected:
    static unsigned __stdcall FileOpProc(LPVOID lpParam);
    void OnFileOp(void);

protected:
    SHFILEOPSTRUCT *mShFileOpStruct;
    xpr_bool_t      mSelectItem;
    HWND            mNotifyHwnd;
    xpr_uint_t      mMsg;

    HANDLE          mThread;
    unsigned        mThreadId;
    HANDLE          mStopEvent;
    xpr_bool_t      mUndo;

public:
    static xpr_sint_t getRefCount(void) { return mRefCount; }
    xpr_sint_t getRefIndex(void) { return mRefIndex; }

protected:
    static xpr_sint_t mRefCount;
    xpr_sint_t mRefIndex;

public:
    virtual xpr_bool_t DestroyWindow(void);

protected:
    virtual void PostNcDestroy(void);

protected:
    virtual ~FileOpThread(void);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnTimer(xpr_uint_t nIDEvent);
    afx_msg void OnClose(void);
    afx_msg LRESULT OnPostEnd(WPARAM, LPARAM);
};
} // namespace fxb

#endif // __FXB_FILE_OP_THREAD_H__
