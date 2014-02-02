//
// Copyright (c) 2014 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXFILE_CFG_APPEARANCE_TAB_DLG_H__
#define __FXFILE_CFG_APPEARANCE_TAB_DLG_H__ 1
#pragma once

#include "cfg_dlg.h"
#include "gui/ColourPickerXP.h"

namespace fxfile
{
namespace cfg
{
class CfgAppearanceTabDlg : public CfgDlg
{
    typedef CfgDlg super;

public:
    CfgAppearanceTabDlg(void);

protected:
    virtual void onInit(const Option::Config &aConfig);
    virtual void onApply(Option::Config &aConfig);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
    DECLARE_MESSAGE_MAP()
    virtual xpr_bool_t OnInitDialog(void);
};
} // namespace cfg
} // namespace fxfile

#endif // __FXFILE_CFG_APPEARANCE_TAB_DLG_H__
