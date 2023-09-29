//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __constant_prop_h__
#define __constant_prop_h__

#include "compiler_structs.h"

unsigned int PropConstants(BasicBlock& bb);

#endif  // __constant_prop_h__
