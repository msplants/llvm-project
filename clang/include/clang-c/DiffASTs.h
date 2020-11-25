#ifndef LLVM_CLANG_DIFFASTS_H
#define LLVM_CLANG_DIFFASTS_H

#include "clang-c/CXString.h"
#include "clang-c/ExternC.h"
#include "clang-c/Index.h"
#include "clang-c/Platform.h"

LLVM_CLANG_C_EXTERN_C_BEGIN

typedef void* CXASTDiff;

CINDEX_LINKAGE int add(int a, int b);

CINDEX_LINKAGE CXASTDiff clang_CXASTDiff_diff(CXTranslationUnit TUSrc,
	CXTranslationUnit TUDest);

CINDEX_LINKAGE void clang_CXASTDiff_print(CXTranslationUnit TUSrc,
	CXTranslationUnit TUDest);

LLVM_CLANG_C_EXTERN_C_END

#endif
