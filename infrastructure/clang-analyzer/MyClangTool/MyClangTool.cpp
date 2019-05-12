#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Analysis/CFG.h"
#include <iostream>
#include "string.h"

using namespace clang::tooling;
using namespace llvm;
using namespace clang;

class FindBasicBlocksVisitor : public clang::RecursiveASTVisitor<FindBasicBlocksVisitor> {
private:
    ASTContext *astContext; // used for getting additional AST info

public:
    explicit FindBasicBlocksVisitor(CompilerInstance *CI) : astContext(&(CI->getASTContext())) // obtain the ASTContext
    { }

    virtual bool VisitFunctionDecl(FunctionDecl *func) {
        SourceManager &mySourceManager = astContext->getSourceManager();
        std::string funcName = func->getNameInfo().getName().getAsString();
        std::cout << "function name: " << funcName << std::endl;
        std::string funcStartLoc = func->getSourceRange().getBegin().printToString(mySourceManager);
        std::string funcEndLoc = func->getSourceRange().getEnd().printToString(mySourceManager); 
        std::cout << "function start location: " << funcStartLoc << std::endl;
        std::cout << "function end location: " << funcEndLoc << std::endl;

        clang::CFG::BuildOptions Options;
        std::unique_ptr<CFG> cfg = clang::CFG::buildCFG(nullptr, func->getBody(), astContext, Options);
        
        if (!cfg)
            // std::cout << "CFG build failed!" << std::endl; 
            int i = 0;
        else {
            int bb_counter = 1;

            for (clang::CFG::iterator CfgBlocks = cfg->begin(), CfgBlocksEnd = cfg->end(); CfgBlocks != CfgBlocksEnd; ++CfgBlocks) {
                // std::cout << "CFG basic block number: " << bb_counter << std::endl;
                bb_counter++;
                int stmt_counter = 1;
                for (clang::CFGBlock::iterator BlockElements = (*CfgBlocks)->begin(), BlockElementsEnd = (*CfgBlocks)->end(); BlockElements != BlockElementsEnd; ++BlockElements) {
                    if (BlockElements->getKind() == clang::CFGElement::Statement) {
                        // Only print out the first statement in each basic block.
                        if (stmt_counter == 1) {
                            clang::CFGStmt BlockElementStmt = BlockElements->castAs<clang::CFGStmt>();
                            std::string str(BlockElementStmt.getStmt()->getStmtClassName());
                            // std::cout << "Statement class: " << str << std::endl;
                            if (str != "DeclStmt")
                            {
                              BlockElementStmt.getStmt()->getLocStart().dump(mySourceManager);
                              std::cout << " " << funcName << " " << funcStartLoc << " " << funcEndLoc << std::endl;
                            }
                            // BlockElementStmt.getStmt()->dump();
                        }
                        stmt_counter++;
                    }
                }
            }
        }       

        return true;     
    }     
};

class FindBasicBlocksConsumer : public clang::ASTConsumer {
private:
    FindBasicBlocksVisitor *Visitor; 

public:
    explicit FindBasicBlocksConsumer(CompilerInstance *CI) : Visitor(new FindBasicBlocksVisitor(CI)) // initialize the Visitor
    { }

    virtual void HandleTranslationUnit(clang::ASTContext &Context) {
        std::cout << "Start traversing the AST of the source file" << std::endl;
        Visitor->TraverseDecl(Context.getTranslationUnitDecl());
    }
};

class FindBasicBlocksAction : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
        return std::unique_ptr<clang::ASTConsumer>(new FindBasicBlocksConsumer(&Compiler)); // pass CI pointer to ASTConsumer
    }
};

static llvm::cl::OptionCategory MyToolCategory("My clang tool options");

int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
    std::cout << "My clang analysis tool starts running ..." << std::endl;
    return Tool.run(newFrontendActionFactory<FindBasicBlocksAction>().get());
}
