//
// Copyright (c) 2001-2012 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FX_EXPLORER_CTRL_OBSERVER_H__
#define __FX_EXPLORER_CTRL_OBSERVER_H__
#pragma once

#include "ViewSet.h"

class ExplorerCtrl;

class ExplorerCtrlObserver
{
public:
    virtual void onExplore(ExplorerCtrl &aExplorerCtrl, LPITEMIDLIST aFullPidl, xpr_bool_t aUpdateBuddy, const xpr_tchar_t *aCurPath) = 0;
    virtual void onUpdateStatus(ExplorerCtrl &aExplorerCtrl, xpr_size_t aRealSelCount) = 0;
    virtual void onUpdateFreeSize(ExplorerCtrl &aExplorerCtrl) = 0;
    virtual void onSetFocus(ExplorerCtrl &aExplorerCtrl) = 0;
    virtual void onKillFocus(ExplorerCtrl &aExplorerCtrl) = 0;
    virtual void onRename(ExplorerCtrl &aExplorerCtrl) = 0;
    virtual void onContextMenuDelete(ExplorerCtrl &aExplorerCtrl) = 0;
    virtual void onContextMenuRename(ExplorerCtrl &aExplorerCtrl) = 0;
    virtual void onContextMenuPaste(ExplorerCtrl &aExplorerCtrl) = 0;
    virtual void onGoDrive(ExplorerCtrl &aExplorerCtrl) = 0;
    virtual xpr_sint_t onDrop(ExplorerCtrl &aExplorerCtrl, COleDataObject *aOleDataObject, xpr_tchar_t *aTargetDir) = 0;
    virtual xpr_bool_t onGetExecParam(ExplorerCtrl &aExplorerCtrl, const xpr_tchar_t *aPath, xpr_tchar_t *aParam, xpr_size_t aMaxLen) = 0;
    virtual void onExecError(ExplorerCtrl &aExplorerCtrl, const xpr_tchar_t *aPath) = 0;
    virtual void onSetViewStyle(ExplorerCtrl &aExplorerCtrl, xpr_sint_t aStyle, xpr_bool_t aRefresh) = 0;
    virtual void onUseColumn(ExplorerCtrl &aExplorerCtrl, ColumnId *aColumnId) = 0;
    virtual void onSortItems(ExplorerCtrl &aExplorerCtrl, ColumnId *aColumnId, xpr_bool_t aAscending) = 0;
    virtual void onSetColumnWidth(ExplorerCtrl &aExplorerCtrl, xpr_sint_t sColumnIndex, xpr_sint_t sWidth) = 0;
    virtual void onMoveFocus(ExplorerCtrl &aExplorerCtrl) = 0;
    virtual void onRunFile(ExplorerCtrl &aExplorerCtrl, LPITEMIDLIST aFullPidl) = 0;
};

#endif // __FX_EXPLORER_CTRL_OBSERVER_H__
