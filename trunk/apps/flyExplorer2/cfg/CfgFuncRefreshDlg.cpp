//
// Copyright (c) 2001-2013 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "CfgFuncRefreshDlg.h"

#include "../Option.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CfgFuncRefreshDlg::CfgFuncRefreshDlg(void)
    : super(IDD_CFG_FUNC_REFRESH, XPR_NULL)
{
}

void CfgFuncRefreshDlg::DoDataExchange(CDataExchange* pDX)
{
    super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfgFuncRefreshDlg, super)
END_MESSAGE_MAP()

xpr_bool_t CfgFuncRefreshDlg::OnInitDialog(void) 
{
    super::OnInitDialog();

    SetDlgItemText(IDC_CFG_REFRESH_NO_REFRESH, theApp.loadString(XPR_STRING_LITERAL("popup.cfg.body.function.bookmark.check.no_refresh")));
    SetDlgItemText(IDC_CFG_REFRESH_AUTO_SORT,  theApp.loadString(XPR_STRING_LITERAL("popup.cfg.body.function.bookmark.check.automatic_sort")));

    return XPR_TRUE;
}

void CfgFuncRefreshDlg::onInit(Option::Config &aConfig)
{
    ((CButton *)GetDlgItem(IDC_CFG_REFRESH_NO_REFRESH))->SetCheck(aConfig.mNoRefresh);
    ((CButton *)GetDlgItem(IDC_CFG_REFRESH_AUTO_SORT ))->SetCheck(aConfig.mRefreshSort);
}

void CfgFuncRefreshDlg::onApply(Option::Config &aConfig)
{
    aConfig.mNoRefresh   = ((CButton *)GetDlgItem(IDC_CFG_REFRESH_NO_REFRESH))->GetCheck();
    aConfig.mRefreshSort = ((CButton *)GetDlgItem(IDC_CFG_REFRESH_AUTO_SORT ))->GetCheck();
}