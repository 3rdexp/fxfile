//
// Copyright (c) 2001-2012 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FX_FILE_OP_UNDO_H__
#define __FX_FILE_OP_UNDO_H__
#pragma once

namespace fxb
{
enum
{
    MODE_COPY = 0,
    MODE_MOVE,
    MODE_RENAME,
    MODE_TRASH,
};

class FileOpUndo
{
public:
    FileOpUndo(void);
    virtual ~FileOpUndo(void);

public:
    void addOperation(SHFILEOPSTRUCT *aShFileOpStruct);
    void addRename(const xpr_tchar_t *aSource, const xpr_tchar_t *aTarget);
    void addRename(const std::tstring &aSource, const std::tstring &aTarget);

    static void deleteUndoDir(void);

public:
    void start(void);

    static xpr_sint_t getMode(void);
    static xpr_bool_t isUndo(void);
    static xpr_sint_t getCount(void);

protected:
    SHFILEOPSTRUCT *startCopy(FILE *aFile);
    SHFILEOPSTRUCT *startMove(FILE *aFile);
    SHFILEOPSTRUCT *startRename(FILE *aFile);
    SHFILEOPSTRUCT *startRecycleBin(FILE *aFile);

    void validateUndoDir(void);

    FILE *beginAddFile(void);
    void endAddFile(FILE *aFile);

    FILE *beginUndoFile(xpr_bool_t aUpdate = XPR_TRUE);
    void endUndoFile(FILE *aFile, xpr_bool_t aUpdate = XPR_TRUE);

protected:
    static xpr_tchar_t *mPath;
    static xpr_sint_t   mIndex;
    static xpr_sint_t   mMode;
};
} // namespace fxb

#endif // __FX_FILE_OP_UNDO_H__
