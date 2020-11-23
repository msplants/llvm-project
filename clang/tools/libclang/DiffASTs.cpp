#include "clang-c/DiffASTs.h"
#include "CXTranslationUnit.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Tooling/ASTDiff/ASTDiff.h"

CXASTDiff clang_CXASTDiff_diff(CXTranslationUnit TUSrc,
    CXTranslationUnit TUDest) {
    clang::ASTUnit* AUSrc = clang::cxtu::getASTUnit(TUSrc);
    clang::ASTUnit* AUDest = clang::cxtu::getASTUnit(TUDest);

    clang::diff::SyntaxTree SrcTree(AUSrc->getASTContext());
    clang::diff::SyntaxTree DstTree(AUDest->getASTContext());

    clang::diff::ComparisonOptions Options;
    return reinterpret_cast<CXASTDiff>(
        new clang::diff::ASTDiff(SrcTree, DstTree, Options));
}