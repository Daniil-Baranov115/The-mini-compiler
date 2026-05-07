// src/llvm_codegen.cpp
#include "llvm_codegen.hpp"
#include "ast.hpp"
#include "driver.hpp"

// LLVM includes
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/DataLayout.h>

#include <unordered_map>
#include <vector>
#include <sstream>

// Целевая архитектура: ARM64
static constexpr const char *TARGET_TRIPLE = "aarch64-unknown-linux-gnu";

// Внутренние типы значений
enum class ValueKind
{
    Int,   // i64
    Bool,  // i1
    Double // double (если нужно)
};

// Значение с типом
struct TypedValue
{
    llvm::Value *value = nullptr;
    ValueKind kind = ValueKind::Int;
};

// Информация о переменной
struct VarInfo
{
    llvm::AllocaInst *storage = nullptr;
    ValueKind kind = ValueKind::Int;
};

// Цели для break/continue
struct LoopTargets
{
    llvm::BasicBlock *break_block;
    llvm::BasicBlock *continue_block;
};

// ================================================================
// РЕАЛИЗАЦИЯ КОДОГЕНЕРАТОРА
// ================================================================
struct LlvmCodeGenerator::Impl
{
    explicit Impl(Driver &driver) : driver_(driver), builder_(context_) {}

    Driver &driver_;
    llvm::LLVMContext context_;
    llvm::IRBuilder<> builder_;
    std::unique_ptr<llvm::Module> module_;

    // Текущая функция
    llvm::Function *current_function_ = nullptr;

    // Стеки для областей видимости и циклов
    std::vector<std::unordered_map<std::string, VarInfo>> scopes_;
    std::vector<LoopTargets> loop_stack_;

    // Слот для возвращаемого значения
    llvm::AllocaInst *return_slot_ = nullptr;
    llvm::BasicBlock *exit_block_ = nullptr;

    int error_count_ = 0;

    // ============================================================
    // ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ
    // ============================================================
    void report_error(const std::string &msg)
    {
        driver_.error("LLVM codegen: " + msg);
        error_count_++;
    }

    llvm::Type *int_type() { return llvm::Type::getInt64Ty(context_); }
    llvm::Type *bool_type() { return llvm::Type::getInt1Ty(context_); }

    llvm::Constant *int_constant(std::int64_t v)
    {
        return llvm::ConstantInt::get(int_type(), v, true);
    }

    llvm::Constant *bool_constant(bool v)
    {
        return llvm::ConstantInt::get(bool_type(), v);
    }

    llvm::Type *llvm_type(ValueKind kind)
    {
        switch (kind)
        {
        case ValueKind::Int:
            return int_type();
        case ValueKind::Bool:
            return bool_type();
        case ValueKind::Double:
            return llvm::Type::getDoubleTy(context_);
        }
        return int_type();
    }

    llvm::Constant *zero_constant(ValueKind kind)
    {
        switch (kind)
        {
        case ValueKind::Int:
            return int_constant(0);
        case ValueKind::Bool:
            return bool_constant(false);
        case ValueKind::Double:
            return llvm::ConstantFP::get(llvm::Type::getDoubleTy(context_), 0.0);
        }
        return int_constant(0);
    }

    // Создание alloca в начале функции
    llvm::AllocaInst *create_entry_alloca(const std::string &name, ValueKind kind)
    {
        llvm::IRBuilder<> tmp_builder(
            &current_function_->getEntryBlock(),
            current_function_->getEntryBlock().begin());
        return tmp_builder.CreateAlloca(llvm_type(kind), nullptr, name);
    }

    bool current_block_terminated() const
    {
        auto *block = builder_.GetInsertBlock();
        return block && block->getTerminator();
    }

    // ============================================================
    // УПРАВЛЕНИЕ ОБЛАСТЯМИ ВИДИМОСТИ
    // ============================================================
    void push_scope()
    {
        scopes_.emplace_back();
    }

    void pop_scope()
    {
        if (!scopes_.empty())
        {
            scopes_.pop_back();
        }
    }

    void declare_var(const std::string &name, llvm::AllocaInst *storage, ValueKind kind)
    {
        if (scopes_.empty())
            push_scope();
        scopes_.back()[name] = {storage, kind};
    }

    VarInfo *lookup_var(const std::string &name)
    {
        for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it)
        {
            auto found = it->find(name);
            if (found != it->end())
            {
                return &found->second;
            }
        }
        return nullptr;
    }

    // ============================================================
    // ПРЕОБРАЗОВАНИЕ ТИПОВ
    // ============================================================
    llvm::Value *to_bool(TypedValue val)
    {
        if (val.kind == ValueKind::Bool)
            return val.value;
        if (val.kind == ValueKind::Int)
        {
            return builder_.CreateICmpNE(val.value, int_constant(0), "to.bool");
        }
        return bool_constant(false);
    }

    TypedValue convert_implicit(TypedValue val, ValueKind target)
    {
        if (val.kind == target)
            return val;

        if (target == ValueKind::Bool)
        {
            return {to_bool(val), ValueKind::Bool};
        }

        if (target == ValueKind::Int && val.kind == ValueKind::Bool)
        {
            return {builder_.CreateZExt(val.value, int_type(), "bool.to.int"), ValueKind::Int};
        }

        report_error("implicit conversion not supported");
        return {zero_constant(target), target};
    }

    // ============================================================
    // ГЕНЕРАЦИЯ ВЫРАЖЕНИЙ
    // ============================================================
    TypedValue generate_expr(Expr &expr)
    {
        // Литералы
        if (auto *int_lit = dynamic_cast<IntegerLiteralExpr *>(&expr))
        {
            return {int_constant(int_lit->value), ValueKind::Int};
        }

        if (auto *bool_lit = dynamic_cast<BoolLiteralExpr *>(&expr))
        {
            return {bool_constant(bool_lit->value), ValueKind::Bool};
        }

        // Переменная
        if (auto *ident = dynamic_cast<IdentifierExpr *>(&expr))
        {
            VarInfo *var = lookup_var(ident->name);
            if (!var)
            {
                report_error("undefined variable: " + ident->name);
                return {int_constant(0), ValueKind::Int};
            }
            llvm::Value *load = builder_.CreateLoad(
                llvm_type(var->kind),
                var->storage,
                ident->name + ".load");
            return {load, var->kind};
        }

        // Бинарные операции
        if (auto *binary = dynamic_cast<BinaryExpr *>(&expr))
        {
            return generate_binary(*binary);
        }

        // Присваивание
        if (auto *assign = dynamic_cast<AssignExpr *>(&expr))
        {
            return generate_assign(*assign);
        }

        // Вызов функции
        if (auto *call = dynamic_cast<CallExpr *>(&expr))
        {
            return generate_call(*call);
        }

        report_error("unsupported expression");
        return {int_constant(0), ValueKind::Int};
    }

    TypedValue generate_binary(BinaryExpr &expr)
    {
        TypedValue lhs = generate_expr(*expr.lhs);
        TypedValue rhs = generate_expr(*expr.rhs);

        // Логические операции с short-circuit
        if (expr.op == BinaryOp::LogicalAnd)
        {
            return generate_logical_and(lhs, rhs);
        }
        if (expr.op == BinaryOp::LogicalOr)
        {
            return generate_logical_or(lhs, rhs);
        }

        // Арифметические и сравнительные операции
        switch (expr.op)
        {
        case BinaryOp::Add:
            return {builder_.CreateAdd(
                        convert_implicit(lhs, ValueKind::Int).value,
                        convert_implicit(rhs, ValueKind::Int).value,
                        "add"),
                    ValueKind::Int};
        case BinaryOp::Sub:
            return {builder_.CreateSub(
                        convert_implicit(lhs, ValueKind::Int).value,
                        convert_implicit(rhs, ValueKind::Int).value,
                        "sub"),
                    ValueKind::Int};
        case BinaryOp::Mul:
            return {builder_.CreateMul(
                        convert_implicit(lhs, ValueKind::Int).value,
                        convert_implicit(rhs, ValueKind::Int).value,
                        "mul"),
                    ValueKind::Int};
        case BinaryOp::Div:
            return {builder_.CreateSDiv(
                        convert_implicit(lhs, ValueKind::Int).value,
                        convert_implicit(rhs, ValueKind::Int).value,
                        "div"),
                    ValueKind::Int};
        case BinaryOp::Mod:
            return {builder_.CreateSRem(
                        convert_implicit(lhs, ValueKind::Int).value,
                        convert_implicit(rhs, ValueKind::Int).value,
                        "mod"),
                    ValueKind::Int};
        case BinaryOp::Equal:
            return {builder_.CreateICmpEQ(
                        convert_implicit(lhs, ValueKind::Int).value,
                        convert_implicit(rhs, ValueKind::Int).value,
                        "eq"),
                    ValueKind::Bool};
        case BinaryOp::Less:
            return {builder_.CreateICmpSLT(
                        convert_implicit(lhs, ValueKind::Int).value,
                        convert_implicit(rhs, ValueKind::Int).value,
                        "lt"),
                    ValueKind::Bool};
        // ... другие операции
        default:
            report_error("unsupported binary operation");
            return {int_constant(0), ValueKind::Int};
        }
    }

    TypedValue generate_logical_and(TypedValue lhs, TypedValue rhs)
    {
        llvm::Function *fn = builder_.GetInsertBlock()->getParent();
        llvm::BasicBlock *rhs_block = llvm::BasicBlock::Create(context_, "and.rhs", fn);
        llvm::BasicBlock *end_block = llvm::BasicBlock::Create(context_, "and.end", fn);

        llvm::Value *lhs_bool = to_bool(lhs);
        llvm::BasicBlock *lhs_block = builder_.GetInsertBlock();
        builder_.CreateCondBr(lhs_bool, rhs_block, end_block);

        builder_.SetInsertPoint(rhs_block);
        llvm::Value *rhs_val = to_bool(rhs);
        llvm::BasicBlock *rhs_end = builder_.GetInsertBlock();
        builder_.CreateBr(end_block);

        builder_.SetInsertPoint(end_block);
        llvm::PHINode *phi = builder_.CreatePHI(bool_type(), 2, "and.phi");
        phi->addIncoming(bool_constant(false), lhs_block);
        phi->addIncoming(rhs_val, rhs_end);

        return {phi, ValueKind::Bool};
    }

    TypedValue generate_logical_or(TypedValue lhs, TypedValue rhs)
    {
        llvm::Function *fn = builder_.GetInsertBlock()->getParent();
        llvm::BasicBlock *rhs_block = llvm::BasicBlock::Create(context_, "or.rhs", fn);
        llvm::BasicBlock *end_block = llvm::BasicBlock::Create(context_, "or.end", fn);

        llvm::Value *lhs_bool = to_bool(lhs);
        llvm::BasicBlock *lhs_block = builder_.GetInsertBlock();
        builder_.CreateCondBr(lhs_bool, end_block, rhs_block);

        builder_.SetInsertPoint(rhs_block);
        llvm::Value *rhs_val = to_bool(rhs);
        llvm::BasicBlock *rhs_end = builder_.GetInsertBlock();
        builder_.CreateBr(end_block);

        builder_.SetInsertPoint(end_block);
        llvm::PHINode *phi = builder_.CreatePHI(bool_type(), 2, "or.phi");
        phi->addIncoming(bool_constant(true), lhs_block);
        phi->addIncoming(rhs_val, rhs_end);

        return {phi, ValueKind::Bool};
    }

    TypedValue generate_assign(AssignExpr &expr)
    {
        auto *ident = dynamic_cast<IdentifierExpr *>(expr.lhs.get());
        if (!ident)
        {
            report_error("assignment target must be identifier");
            return {int_constant(0), ValueKind::Int};
        }

        VarInfo *var = lookup_var(ident->name);
        if (!var)
        {
            report_error("assignment to undeclared variable: " + ident->name);
            return {int_constant(0), ValueKind::Int};
        }

        TypedValue rhs = generate_expr(*expr.rhs);
        rhs = convert_implicit(rhs, var->kind);

        builder_.CreateStore(rhs.value, var->storage);
        return rhs;
    }

    TypedValue generate_call(CallExpr &expr)
    {
        auto *callee_id = dynamic_cast<IdentifierExpr *>(expr.callee.get());
        if (!callee_id)
        {
            report_error("indirect calls not supported");
            return {int_constant(0), ValueKind::Int};
        }

        llvm::Function *callee = module_->getFunction(callee_id->name);
        if (!callee)
        {
            report_error("unknown function: " + callee_id->name);
            return {int_constant(0), ValueKind::Int};
        }

        std::vector<llvm::Value *> args;
        for (auto &arg : expr.arguments)
        {
            TypedValue arg_val = generate_expr(*arg);
            args.push_back(convert_implicit(arg_val, ValueKind::Int).value);
        }

        llvm::Value *result = builder_.CreateCall(callee, args, "call");
        return {result, ValueKind::Int};
    }

    // ============================================================
    // ГЕНЕРАЦИЯ ОПЕРАТОРОВ
    // ============================================================
    void generate_statement(Stmt &stmt)
    {
        // Пустой оператор
        if (dynamic_cast<EmptyStmt *>(&stmt))
            return;

        // Выражение-оператор
        if (auto *expr_stmt = dynamic_cast<ExprStmt *>(&stmt))
        {
            if (expr_stmt->expr)
            {
                generate_expr(*expr_stmt->expr);
            }
            return;
        }

        // Return
        if (auto *return_stmt = dynamic_cast<ReturnStmt *>(&stmt))
        {
            TypedValue val = return_stmt->expr ? generate_expr(*return_stmt->expr) : TypedValue{int_constant(0), ValueKind::Int};
            builder_.CreateStore(convert_implicit(val, ValueKind::Int).value, return_slot_);
            builder_.CreateBr(exit_block_);
            return;
        }

        // Break
        if (dynamic_cast<BreakStmt *>(&stmt))
        {
            if (loop_stack_.empty())
            {
                report_error("break outside loop");
                return;
            }
            builder_.CreateBr(loop_stack_.back().break_block);
            return;
        }

        // Continue
        if (dynamic_cast<ContinueStmt *>(&stmt))
        {
            if (loop_stack_.empty())
            {
                report_error("continue outside loop");
                return;
            }
            builder_.CreateBr(loop_stack_.back().continue_block);
            return;
        }

        // Объявление переменной
        if (auto *var_decl = dynamic_cast<VarDeclStmt *>(&stmt))
        {
            ValueKind kind = (var_decl->type == TypeSpec::Bool) ? ValueKind::Bool : ValueKind::Int;
            llvm::AllocaInst *storage = create_entry_alloca(var_decl->name, kind);
            declare_var(var_decl->name, storage, kind);

            if (var_decl->initializer)
            {
                TypedValue init = generate_expr(*var_decl->initializer);
                builder_.CreateStore(convert_implicit(init, kind).value, storage);
            }
            else
            {
                builder_.CreateStore(zero_constant(kind), storage);
            }
            return;
        }

        // If
        if (auto *if_stmt = dynamic_cast<IfStmt *>(&stmt))
        {
            generate_if(*if_stmt);
            return;
        }

        // While
        if (auto *while_stmt = dynamic_cast<WhileStmt *>(&stmt))
        {
            generate_while(*while_stmt);
            return;
        }

        // Block
        if (auto *block = dynamic_cast<BlockStmt *>(&stmt))
        {
            generate_block(*block, true);
            return;
        }

        // ============================================================
        // SWITCH/CASE (ВАША ОСНОВНАЯ ФИЧА)
        // ============================================================
        if (auto *switch_stmt = dynamic_cast<SwitchStmt *>(&stmt))
        {
            generate_switch(*switch_stmt);
            return;
        }

        report_error("unsupported statement");
    }

    void generate_if(IfStmt &stmt)
    {
        llvm::Value *cond = to_bool(generate_expr(*stmt.condition));

        llvm::Function *fn = current_function_;
        llvm::BasicBlock *then_bb = llvm::BasicBlock::Create(context_, "if.then", fn);
        llvm::BasicBlock *else_bb = nullptr;
        llvm::BasicBlock *end_bb = llvm::BasicBlock::Create(context_, "if.end", fn);

        if (stmt.else_stmt)
        {
            else_bb = llvm::BasicBlock::Create(context_, "if.else", fn);
            builder_.CreateCondBr(cond, then_bb, else_bb);
        }
        else
        {
            builder_.CreateCondBr(cond, then_bb, end_bb);
        }

        // Then branch
        builder_.SetInsertPoint(then_bb);
        generate_statement(*stmt.then_stmt);
        if (!current_block_terminated())
        {
            builder_.CreateBr(end_bb);
        }

        // Else branch
        if (else_bb)
        {
            builder_.SetInsertPoint(else_bb);
            generate_statement(*stmt.else_stmt);
            if (!current_block_terminated())
            {
                builder_.CreateBr(end_bb);
            }
        }

        builder_.SetInsertPoint(end_bb);
    }

    void generate_while(WhileStmt &stmt)
    {
        llvm::Function *fn = current_function_;
        llvm::BasicBlock *cond_bb = llvm::BasicBlock::Create(context_, "while.cond", fn);
        llvm::BasicBlock *body_bb = llvm::BasicBlock::Create(context_, "while.body", fn);
        llvm::BasicBlock *end_bb = llvm::BasicBlock::Create(context_, "while.end", fn);

        builder_.CreateBr(cond_bb);

        // Condition block
        builder_.SetInsertPoint(cond_bb);
        llvm::Value *cond = to_bool(generate_expr(*stmt.condition));
        builder_.CreateCondBr(cond, body_bb, end_bb);

        // Loop targets for break/continue
        loop_stack_.push_back({end_bb, cond_bb});

        // Body block
        builder_.SetInsertPoint(body_bb);
        generate_statement(*stmt.body);
        if (!current_block_terminated())
        {
            builder_.CreateBr(cond_bb);
        }

        loop_stack_.pop_back();
        builder_.SetInsertPoint(end_bb);
    }

    void generate_switch(SwitchStmt &stmt)
    {
        llvm::Value *switch_val = generate_expr(*stmt.condition).value;

        llvm::Function *fn = current_function_;
        llvm::BasicBlock *end_bb = llvm::BasicBlock::Create(context_, "switch.end", fn);
        llvm::BasicBlock *default_bb = llvm::BasicBlock::Create(context_, "switch.default", fn);

        // Создаём switch-инструкцию
        auto *switch_inst = builder_.CreateSwitch(switch_val, default_bb, stmt.cases.size());

        // Словарь для case-блоков
        std::unordered_map<std::int64_t, llvm::BasicBlock *> case_blocks;

        for (size_t i = 0; i < stmt.cases.size(); ++i)
        {
            auto *case_stmt = stmt.cases[i].get();

            if (auto *case_clause = dynamic_cast<CaseClause *>(case_stmt))
            {
                auto *int_lit = dynamic_cast<IntegerLiteralExpr *>(case_clause->value.get());
                if (!int_lit)
                    continue;

                llvm::BasicBlock *case_bb = llvm::BasicBlock::Create(context_,
                                                                     "switch.case." + std::to_string(int_lit->value), fn);
                case_blocks[int_lit->value] = case_bb;
                switch_inst->addCase(llvm::cast<llvm::ConstantInt>(int_constant(int_lit->value)), case_bb);
            }
        }

        // Генерируем код для каждого case
        for (size_t i = 0; i < stmt.cases.size(); ++i)
        {
            auto *case_stmt = stmt.cases[i].get();

            if (auto *case_clause = dynamic_cast<CaseClause *>(case_stmt))
            {
                auto *int_lit = dynamic_cast<IntegerLiteralExpr *>(case_clause->value.get());
                if (!int_lit)
                    continue;

                builder_.SetInsertPoint(case_blocks[int_lit->value]);
                for (auto &s : case_clause->statements)
                {
                    generate_statement(*s);
                }
                if (!current_block_terminated())
                {
                    builder_.CreateBr(end_bb);
                }
            }
        }

        // Default блок
        builder_.SetInsertPoint(default_bb);
        bool has_default = false;
        for (auto &case_stmt : stmt.cases)
        {
            if (auto *default_clause = dynamic_cast<DefaultClause *>(case_stmt.get()))
            {
                has_default = true;
                for (auto &s : default_clause->statements)
                {
                    generate_statement(*s);
                }
                if (!current_block_terminated())
                {
                    builder_.CreateBr(end_bb);
                }
                break;
            }
        }
        if (!has_default && !current_block_terminated())
        {
            builder_.CreateBr(end_bb);
        }

        builder_.SetInsertPoint(end_bb);
    }

    void generate_block(BlockStmt &block, bool create_scope)
    {
        if (create_scope)
            push_scope();

        for (auto &stmt : block.statements)
        {
            generate_statement(*stmt);
            if (error_count_ > 0)
                break;
        }

        if (create_scope)
            pop_scope();
    }

    // ============================================================
    // ГЕНЕРАЦИЯ ФУНКЦИИ
    // ============================================================
    void generate_function(FunctionDecl &func)
    {
        llvm::Function *fn = module_->getFunction(func.name);
        if (!fn)
            return;

        current_function_ = fn;

        // Создаём блоки
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(context_, "entry", fn);
        exit_block_ = llvm::BasicBlock::Create(context_, func.name + ".exit", fn);
        builder_.SetInsertPoint(entry);

        // Слот для возвращаемого значения
        return_slot_ = create_entry_alloca("return.value", ValueKind::Int);
        builder_.CreateStore(int_constant(0), return_slot_);

        push_scope();

        // Параметры функции (в учебном компиляторе - один параметр типа int64_t)
        size_t param_idx = 0;
        for (auto &arg : fn->args())
        {
            if (param_idx < func.parameters.size())
            {
                arg.setName(func.parameters[param_idx].name);
                llvm::AllocaInst *storage = create_entry_alloca(arg.getName().str(), ValueKind::Int);
                builder_.CreateStore(&arg, storage);
                declare_var(arg.getName().str(), storage, ValueKind::Int);
            }
            param_idx++;
        }

        // Тело функции
        if (func.body)
        {
            generate_block(*func.body, false);
        }

        // Если нет return в конце, добавляем переход к exit
        if (!current_block_terminated())
        {
            builder_.CreateBr(exit_block_);
        }

        // Exit блок
        builder_.SetInsertPoint(exit_block_);
        llvm::Value *result = builder_.CreateLoad(int_type(), return_slot_, "result");
        builder_.CreateRet(result);

        pop_scope();
        current_function_ = nullptr;
        return_slot_ = nullptr;
        exit_block_ = nullptr;

        // Проверка функции
        if (llvm::verifyFunction(*fn, &llvm::errs()))
        {
            report_error("generated function is invalid: " + func.name);
        }
    }

    // ============================================================
    // ГЛАВНЫЙ МЕТОД ГЕНЕРАЦИИ
    // ============================================================
    bool generate(const Program &program)
    {
        error_count_ = 0;

        // Инициализация LLVM
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmParsers();
        llvm::InitializeAllAsmPrinters();

        // Создаём модуль
        module_ = std::make_unique<llvm::Module>("mini_compiler", context_);
        module_->setTargetTriple(TARGET_TRIPLE);

        // Объявляем функции
        for (const auto &func : program.functions)
        {
            llvm::FunctionType *fn_type = llvm::FunctionType::get(int_type(), {int_type()}, false);
            llvm::Function::Create(fn_type, llvm::Function::ExternalLinkage, func->name, module_.get());
        }

        // Генерируем код для каждой функции
        for (const auto &func : program.functions)
        {
            generate_function(*func);
            if (error_count_ > 0)
                return false;
        }

        // Проверка модуля
        std::string err;
        llvm::raw_string_ostream err_stream(err);
        if (llvm::verifyModule(*module_, &err_stream))
        {
            report_error("invalid module: " + err);
            return false;
        }

        return error_count_ == 0;
    }

    std::string ir_string() const
    {
        if (!module_)
            return "";
        std::string result;
        llvm::raw_string_ostream out(result);
        module_->print(out, nullptr);
        return result;
    }

    bool write_ir_file(const std::filesystem::path &path)
    {
        if (!module_)
            return false;

        std::error_code ec;
        llvm::raw_fd_ostream out(path.string(), ec, llvm::sys::fs::OF_None);
        if (ec)
        {
            report_error("cannot open IR file: " + path.string());
            return false;
        }
        module_->print(out, nullptr);
        return true;
    }

    bool emit_target_file(const std::filesystem::path &path, TargetFileKind kind)
    {
        if (!module_)
            return false;

        // Находим целевую машину для ARM64
        std::string error;
        const llvm::Target *target = llvm::TargetRegistry::lookupTarget(TARGET_TRIPLE, error);
        if (!target)
        {
            report_error(error);
            return false;
        }

        llvm::TargetOptions opt;
        std::unique_ptr<llvm::TargetMachine> target_machine(
            target->createTargetMachine(TARGET_TRIPLE, "generic", "", opt, std::nullopt));
        if (!target_machine)
        {
            report_error("cannot create target machine");
            return false;
        }

        module_->setDataLayout(target_machine->createDataLayout());

        std::error_code ec;
        llvm::raw_fd_ostream out(path.string(), ec, llvm::sys::fs::OF_None);
        if (ec)
        {
            report_error("cannot open output file: " + path.string());
            return false;
        }

        llvm::legacy::PassManager pass_mgr;
        llvm::CodeGenFileType file_type = (kind == TargetFileKind::Assembly)
                                              ? llvm::CodeGenFileType::AssemblyFile
                                              : llvm::CodeGenFileType::ObjectFile;

        if (target_machine->addPassesToEmitFile(pass_mgr, out, nullptr, file_type))
        {
            report_error("target machine cannot emit this file type");
            return false;
        }

        pass_mgr.run(*module_);
        return true;
    }
};

// ================================================================
// ПУБЛИЧНЫЙ ИНТЕРФЕЙС
// ================================================================
LlvmCodeGenerator::LlvmCodeGenerator(Driver &driver)
    : impl_(std::make_unique<Impl>(driver)) {}

LlvmCodeGenerator::~LlvmCodeGenerator() = default;

bool LlvmCodeGenerator::generate(const Program &program)
{
    return impl_->generate(program);
}

bool LlvmCodeGenerator::write_ir_file(const std::filesystem::path &path)
{
    return impl_->write_ir_file(path);
}

std::string LlvmCodeGenerator::ir_string() const
{
    return impl_->ir_string();
}

bool LlvmCodeGenerator::emit_target_file(const std::filesystem::path &path, TargetFileKind kind)
{
    return impl_->emit_target_file(path, kind);
}