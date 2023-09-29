//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __constant_fold_h__
#define __constant_fold_h__

#include "compiler_structs.h"

unsigned int FoldConstants(BasicBlock& bb);

#endif  // __constant_fold_h__
