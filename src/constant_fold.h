#ifndef __constant_fold_h__
#define __constant_fold_h__

#include "compiler_structs.h"

unsigned int FoldConstants(BasicBlock& bb);

#endif  // __constant_fold_h__
