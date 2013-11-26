//
// Copyright (c) 2001-2013 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXFILE_CFG_FUNC_LINK_DLG_H__
#define __FXFILE_CFG_FUNC_LINK_DLG_H__ 1
#pragma once

#include "cfg_dlg.h"

namespace fxfile
{
namespace cfg
{
class CfgFuncBookmarkDlg : public CfgDlg
{
    typedef CfgDlg super;

public:
    CfgFuncBookmarkDlg(void);

protected:
    virtual void onInit(Option::Config &aConfig);
    virtual void onApply(Option::Config &aConfig);

public:
    virtual xpr_bool_t PreTranslateMessage(MSG* pMsg);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
    DECLARE_MESSAGE_MAP()
    virtual xpr_bool_t OnInitDialog(void);
};
} // namespace cfg
} // namespace fxfile

#endif // __FXFILE_CFG_FUNC_LINK_DLG_H__