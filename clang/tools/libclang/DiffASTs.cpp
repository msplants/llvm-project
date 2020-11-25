#include "clang-c/DiffASTs.h"
#include "CXTranslationUnit.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Tooling/ASTDiff/ASTDiff.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace llvm;

int add(int a, int b) {
    return a + b;
}

CXASTDiff clang_CXASTDiff_diff(CXTranslationUnit TUSrc,
    CXTranslationUnit TUDest) {
    ASTUnit* AUSrc = cxtu::getASTUnit(TUSrc);
    ASTUnit* AUDest = cxtu::getASTUnit(TUDest);

    diff::SyntaxTree SrcTree(AUSrc->getASTContext());
    diff::SyntaxTree DstTree(AUDest->getASTContext());

    diff::ComparisonOptions Options;
    return reinterpret_cast<CXASTDiff>(
        new diff::ASTDiff(SrcTree, DstTree, Options));
}

static void printNode(raw_ostream& OS, diff::SyntaxTree& Tree,
	diff::NodeId Id) {
	if (Id.isInvalid()) {
		OS << "None";
		return;
	}
	OS << Tree.getNode(Id).getTypeLabel();
	std::string Value = Tree.getNodeValue(Id);
	if (!Value.empty())
		OS << ": " << Value;
	OS << "(" << Id << ")";
}

static void printDstChange(raw_ostream &OS, diff::ASTDiff &Diff,
	diff::SyntaxTree &SrcTree, diff::SyntaxTree &DstTree,
	diff::NodeId Dst) {
	const diff::Node &DstNode = DstTree.getNode(Dst);
	diff::NodeId Src = Diff.getMapped(DstTree, Dst);
	switch (DstNode.Change) {
	case diff::None:
		break;
	case diff::Delete:
		llvm_unreachable("The destination tree can't have deletions.");
	case diff::Update:
		OS << "Update ";
		printNode(OS, SrcTree, Src);
		OS << " to " << DstTree.getNodeValue(Dst) << "\n";
		break;
	case diff::Insert:
	case diff::Move:
	case diff::UpdateMove:
		if (DstNode.Change == diff::Insert)
			OS << "Insert";
		else if (DstNode.Change == diff::Move)
			OS << "Move";
		else if (DstNode.Change == diff::UpdateMove)
			OS << "Update and Move";
		OS << " ";
		printNode(OS, DstTree, Dst);
		OS << " into ";
		printNode(OS, DstTree, DstNode.Parent);
		OS << " at " << DstTree.findPositionInParent(Dst) << "\n";
		break;
	}
}

void clang_CXASTDiff_print(CXTranslationUnit TUSrc,
	CXTranslationUnit TUDest) {
    ASTUnit* AUSrc = cxtu::getASTUnit(TUSrc);
    ASTUnit* AUDest = cxtu::getASTUnit(TUDest);

    diff::SyntaxTree SrcTree(AUSrc->getASTContext());
    diff::SyntaxTree DstTree(AUDest->getASTContext());

    diff::ComparisonOptions Options;
    diff::ASTDiff Diff(SrcTree, DstTree, Options);

	for (diff::NodeId Dst : DstTree) {
		diff::NodeId Src = Diff.getMapped(DstTree, Dst);
		printDstChange(llvm::outs(), Diff, SrcTree, DstTree, Dst);
	}
}
