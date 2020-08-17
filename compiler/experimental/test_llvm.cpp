//
// Created by Lezenn on 14/08/2020.
//
#include <iostream>

#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"


#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

//llvm::Value* log_error_v(const char* str)
//{
//    std::cout << str << std::endl;
//    return nullptr;
//}

/* Creates a simple LLVM program than outputs a hello world program
 */
int main()
{
    using namespace llvm;

    LLVMContext GlobalContext;
    std::unique_ptr<Module> MainModule = std::make_unique<Module>("MainModule", GlobalContext);
    std::unordered_map<std::string, Value*> NamedValues;


    // Main Function
    FunctionType* FT = FunctionType::get(Type::getInt32Ty(GlobalContext), false);
    Function* F = Function::Create(FT, Function::ExternalLinkage, "main", MainModule.get());
    BasicBlock* BB = BasicBlock::Create(GlobalContext, "EntryBlock", F);
    Value* three = ConstantInt::get(Type::getInt32Ty(GlobalContext), 3);
    Value* four = ConstantInt::get(Type::getInt32Ty(GlobalContext), 4);
    Instruction* add = BinaryOperator::CreateAdd(three, four, "tmpAdd", BB);

    // Code Generation
    IRBuilder<> Builder {GlobalContext};
    Builder.SetInsertPoint(BB);


    Function* putChar = MainModule->getFunction("putchar");
    Value* addRes = NamedValues["tmpAdd"];
    Builder.CreateCall(putChar, {addRes});

    LLVMInitializeX86Target();
    LLVMInitializeX86AsmParser();
    LLVMInitializeX86AsmPrinter();
    LLVMInitializeX86TargetInfo();
    LLVMInitializeX86TargetMC();

    auto target_triple = sys::getDefaultTargetTriple();
    MainModule->setTargetTriple(target_triple);

    std::string error;
    auto target = TargetRegistry::lookupTarget(target_triple, error);

    if (!target)
    {
        std::cerr << error << std::endl;
        return 1;
    }

    auto cpu = "generic";
    auto features = "";

    TargetOptions options;
    auto rm = Optional<Reloc::Model>();
    auto target_machine = target->createTargetMachine(target_triple, cpu, features, options, rm);

    MainModule->setDataLayout(target_machine->createDataLayout());

    // Output

    auto filename = "output.exe";
    std::error_code ec;
    raw_fd_ostream dest {filename, ec};

    if (ec)
    {
        std::cerr << "Could not open file : " << ec.message() << std::endl;
        return 1;
    }

    legacy::PassManager pass;
    auto file_type = CGFT_ObjectFile;

    if (target_machine->addPassesToEmitFile(pass, dest, nullptr, file_type))
    {
        errs() << "target_machine can't emit a file of this type";
        return 1;
    }

    pass.run(*MainModule);
    dest.flush();

    llvm::raw_fd_ostream llvmir("output_ir", ec);
    llvm::WriteBitcodeToFile(*MainModule, llvmir);
    llvmir.flush();

    return 0;
}