//
// Copyright (c) 2001-2012 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FX_CFG_EXP_FILTER_H__
#define __FX_CFG_EXP_FILTER_H__
#pragma once

#include "../fxb/fxb_filter.h"

#include "CfgDlg.h"

class CfgExpFilterDlg : public CfgDlg
{
    typedef CfgDlg super;

public:
    CfgExpFilterDlg(void);

protected:
    virtual void OnApply(void);

protected:
    xpr_sint_t insertFilterItem(xpr_sint_t aIndex, fxb::FilterItem *aFilterItem);
    xpr_sint_t addFilterItem(fxb::FilterItem *aFilterItem);
    void selectItem(xpr_sint_t aIndex);

protected:
    CImageList mImageList;
    CListCtrl  mListCtrl;

public:
    virtual xpr_bool_t PreTranslateMessage(MSG* pMsg);
    virtual xpr_bool_t DestroyWindow(void);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
    DECLARE_MESSAGE_MAP()
    virtual xpr_bool_t OnInitDialog(void);
    afx_msg void OnListAdd(void);
    afx_msg void OnListModify(void);
    afx_msg void OnListDelete(void);
    afx_msg void OnItemUp(void);
    afx_msg void OnItemDown(void);
    afx_msg void OnUseColor(void);
    afx_msg void OnTextColor(void);
    afx_msg void OnBackgroundColor(void);
    afx_msg void OnDefault(void);
    afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeleteitemList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemActivateList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);
};

#endif // __FX_CFG_EXP_FILTER_H__
