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

    std::vector<Type*> PutCharArgs = { Type::getInt32Ty(Context) };
    FunctionType* PutCharType = FunctionType::get(Type::getInt32Ty(Context), PutCharArgs, false);
    Function::Create(PutCharType, Function::ExternalLinkage, "putchar", M);





    // Create the main function: first create the type 'int ()'
    FunctionType* FT = FunctionType::get(Type::getInt32Ty(Context), /*not vararg*/ false);

    // By passing a module as the last parameter to the Function constructor,
    // it automatically gets appended to the Module.
    Function* F = Function::Create(FT, Function::ExternalLinkage, "main", M);

    // Add a basic block to the function... again, it automatically inserts
    // because of the last argument.
    BasicBlock* BB = BasicBlock::Create(Context, "EntryBlock", F);
    Value* Two = ConstantInt::get(Type::getInt32Ty(Context), 2);
    Value* Three = ConstantInt::get(Type::getInt32Ty(Context), 3);
    Instruction* Add = BinaryOperator::Create(Instruction::Add, Two, Three, "addresult");
    BB->getInstList().push_back(Add);




    IRBuilder<> Builder(Context);
    Builder.SetInsertPoint(BB);

    auto putchar = M->getFunction("putchar");

    Value* display = ConstantInt::get(Type::getInt32Ty(Context), 97);
    Builder.CreateCall(putchar, {display});


    // Create the return instruction and add it to the basic block
    BB->getInstList().push_back(ReturnInst::Create(Context, Three));

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