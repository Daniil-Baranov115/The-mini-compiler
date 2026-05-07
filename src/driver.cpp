// src/driver.cpp
#include "driver.hpp"
#include "lexer.hpp"
#include "sema.hpp"
#include "llvm_codegen.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

struct Driver::Impl {
    std::string filename;
    std::string source;
};

Driver::Driver() : impl_(std::make_unique<Impl>()) {}
Driver::~Driver() = default;

void Driver::error(const std::string& msg) {
    errors_.push_back(msg);
    std::cerr << "[ERROR] " << msg << std::endl;
}

void Driver::set_program(std::unique_ptr<Program> program) {
    program_ = std::move(program);
}

yy::Parser::symbol_type Driver::next_token() {
    return lexer_->next_token();
}

bool Driver::compile(const std::string& filename, 
                     bool emit_ir, 
                     bool emit_asm, 
                     bool emit_obj,
                     bool trace) {
    trace_mode_ = trace;
    errors_.clear();
    
    // 1. Чтение файла
    std::ifstream file(filename);
    if (!file) {
        error("cannot open file: " + filename);
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    impl_->source = buffer.str();
    impl_->filename = filename;
    
    if (trace) {
        std::cout << "=== SOURCE CODE ===" << std::endl;
        std::cout << impl_->source << std::endl;
    }
    
    // 2. Лексический и синтаксический анализ
    lexer_ = std::make_unique<Lexer>(*this, impl_->source);
    yy::Parser parser(*this);
    if (parser.parse() != 0) {
        error("syntax analysis failed");
        return false;
    }
    
    if (trace && program_) {
        std::cout << "=== AST ===" << std::endl;
        program_->dump(std::cout);
    }
    
    // 3. Семантический анализ
    SemanticAnalyzer sema(*this);
    if (!sema.analyze(*program_)) {
        error("semantic analysis failed");
        return false;
    }
    
    if (trace) {
        std::cout << "=== SEMANTIC ANALYSIS PASSED ===" << std::endl;
    }
    
    // 4. Генерация LLVM IR
    LlvmCodeGenerator codegen(*this);
    if (!codegen.generate(*program_)) {
        error("LLVM IR generation failed");
        return false;
    }
    
    ir_string_ = codegen.ir_string();
    
    if (trace || emit_ir) {
        std::cout << "=== LLVM IR ===" << std::endl;
        std::cout << ir_string_ << std::endl;
    }
    
    // 5. Сохранение IR файла
    if (emit_ir) {
        std::filesystem::path ir_path = filename;
        ir_path.replace_extension(".ll");
        if (!codegen.write_ir_file(ir_path)) {
            error("cannot write IR file");
            return false;
        }
        std::cout << "IR written to: " << ir_path << std::endl;
    }
    
    // 6. Выпуск объектного файла или ассемблера
    if (emit_obj || emit_asm) {
        TargetFileKind kind = emit_asm ? TargetFileKind::Assembly : TargetFileKind::Object;
        std::filesystem::path out_path = filename;
        out_path.replace_extension(emit_asm ? ".s" : ".o");
        
        if (!codegen.emit_target_file(out_path, kind)) {
            error("cannot emit target file");
            return false;
        }
        std::cout << "Output written to: " << out_path << std::endl;
    }
    
    return true;
}

void Driver::dump_ast() const {
    if (program_) {
        program_->dump(std::cout);
    }
}

void Driver::dump_ir() const {
    std::cout << ir_string_ << std::endl;
}