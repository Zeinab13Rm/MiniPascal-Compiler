#include "codegenerator.h"
#include <stdexcept>
#include <iostream>
#include <list>
#include <algorithm> // For std::reverse

// --- Entry Point ---
std::string CodeGenerator::generateCode(ProgramNode& ast_root, SemanticAnalyzer& semanticAnalyzer) {
    this->symbolTable = &semanticAnalyzer.getSymbolTable();
    ast_root.accept(*this);
    return code.str();
}

// --- Helper Methods ---
std::string CodeGenerator::newLabel(const std::string& prefix) {
    return "L_" + prefix + "_" + std::to_string(labelCounter++);
}

void CodeGenerator::emit(const std::string& instruction) {
    code << "    " << instruction << std::endl;
}

void CodeGenerator::emit(const std::string& instruction, const std::string& arg) {
    code << "    " << instruction << " " << arg << std::endl;
}

void CodeGenerator::emitLabel(const std::string& label) {
    code << label << ":" << std::endl;
}

// Helper to translate AST type nodes; useful for array declarations
EntryTypeCategory CodeGenerator::astToSymbolType(TypeNode* astTypeNode, ArrayDetails& outArrayDetails) {
    outArrayDetails.isInitialized = false;
    if (!astTypeNode) return EntryTypeCategory::ET_UNKNOWN;

    if (auto* stn = dynamic_cast<StandardTypeNode*>(astTypeNode)) {
        switch (stn->category) {
        case StandardTypeNode::TYPE_INTEGER: return EntryTypeCategory::ET_INTEGER;
        case StandardTypeNode::TYPE_REAL: return EntryTypeCategory::ET_REAL;
        case StandardTypeNode::TYPE_BOOLEAN: return EntryTypeCategory::ET_BOOLEAN;
        default: return EntryTypeCategory::ET_UNKNOWN;
        }
    }
    else if (auto* atn = dynamic_cast<ArrayTypeNode*>(astTypeNode)) {
        if (atn->elementType) {
            ArrayDetails tempDetails;
            outArrayDetails.elementType = astToSymbolType(atn->elementType, tempDetails);
        }
        if (atn->startIndex && atn->endIndex) {
            outArrayDetails.lowBound = atn->startIndex->value;
            outArrayDetails.highBound = atn->endIndex->value;
            outArrayDetails.isInitialized = true;
        }
        return EntryTypeCategory::ET_ARRAY;
    }
    return EntryTypeCategory::ET_UNKNOWN;
}


// --- Visitor Implementations ---

void CodeGenerator::visit(ProgramNode& node) {
    emit("start");
    // If there are functions/procedures, jump over them to the main entry point
    if (node.subprogs && !node.subprogs->subprograms.empty()) {
        emit("jump", "main_entry");
    }

    if (node.subprogs) node.subprogs->accept(*this);

    emitLabel("main_entry");
    if (node.decls) node.decls->accept(*this);
    if (node.mainCompoundStmt) node.mainCompoundStmt->accept(*this);
    emit("stop");
}

void CodeGenerator::visit(Declarations& node) {
    int simpleVarCount = 0;
    // Count simple variables to allocate space for them all at once
    for (const auto* decl : node.var_decl_items) {
        if (!dynamic_cast<ArrayTypeNode*>(decl->type)) {
            simpleVarCount += decl->identifiers->identifiers.size();
        }
    }
    if (simpleVarCount > 0) {
        emit("pushn", std::to_string(simpleVarCount));
    }
    // Now visit each declaration to handle array allocations
    for (auto* varDecl : node.var_decl_items) {
        varDecl->accept(*this);
    }
}

void CodeGenerator::visit(VarDecl& node) {
    // Re-populate the symbol table for the code generator's context
    if (!symbolTable->isGlobalScope()) {
        ArrayDetails ad;
        EntryTypeCategory var_type = astToSymbolType(node.type, ad);
        for (auto* ident : node.identifiers->identifiers) {
            SymbolEntry entry(ident->name, SymbolKind::SK_VARIABLE, var_type, ident->line, ident->column);
            entry.offset = local_offset++;
            if (var_type == EntryTypeCategory::ET_ARRAY) {
                entry.arrayDetails = ad;
            }
            symbolTable->addSymbol(entry);
        }
    }

    // Handle array allocations
    if (auto* arrayType = dynamic_cast<ArrayTypeNode*>(node.type)) {
        int low = arrayType->startIndex->value;
        int high = arrayType->endIndex->value;
        int size = high - low + 1;
        if (size <= 0) { throw std::runtime_error("CodeGen Error: Array size must be positive."); }
        for (auto* ident : node.identifiers->identifiers) {
            SymbolEntry* entry = symbolTable->lookupSymbol(ident->name);
            if (!entry) throw std::runtime_error("CodeGen Error: Symbol not found during array allocation: " + ident->name);
            emit("alloc", std::to_string(size));
            if (symbolTable->isGlobalScope()) { emit("storeg", std::to_string(entry->offset)); }
            else { emit("storel", std::to_string(entry->offset)); }
        }
    }
    
}

void CodeGenerator::visit(SubprogramDeclarations& node) {
    for (auto* subprog : node.subprograms) {
        if (subprog) subprog->accept(*this);
    }
}

void CodeGenerator::visit(SubprogramDeclaration& node) {
    currentFunctionContext = node.head;
    std::string subprogramName = node.head->name->name;
    std::string endLabel = subprogramName + "_end";

    emit("jump", endLabel);
    emitLabel(subprogramName);

    symbolTable->enterScope();
    local_offset = 0;
    param_offset = 0;

    if (node.head->arguments) node.head->arguments->accept(*this);
    if (node.local_declarations) node.local_declarations->accept(*this);

    // Now visit the body with the correct scope
    if (node.body) node.body->accept(*this);

    if (dynamic_cast<ProcedureHeadNode*>(node.head)) {
        emit("return");
    }

    emitLabel(endLabel);

    // *** FIX: Exit scope ***
    symbolTable->exitScope();
    currentFunctionContext = nullptr;
}
  
void CodeGenerator::visit(ArgumentsNode& node) {
    if (node.params) node.params->accept(*this);
}

void CodeGenerator::visit(ParameterList& node) {
    for (auto* param : node.paramDeclarations) {
        param->accept(*this);
    }
}

void CodeGenerator::visit(ParameterDeclaration& node) {
    // Re-populate the symbol table for the current scope, just like in SemanticAnalyzer
    ArrayDetails ad;
    EntryTypeCategory param_type = astToSymbolType(node.type, ad);
    for (auto* ident : node.ids->identifiers) {
        SymbolEntry entry(ident->name, SymbolKind::SK_PARAMETER, param_type, ident->line, ident->column);
        entry.offset = param_offset++; // Use CodeGenerator's own counter
        if (param_type == EntryTypeCategory::ET_ARRAY) {
            entry.arrayDetails = ad;
        }
        symbolTable->addSymbol(entry); // Add to the new local scope
    }
}

void CodeGenerator::visit(CompoundStatementNode& node) {
    if (node.stmts) node.stmts->accept(*this);
}

void CodeGenerator::visit(StatementList& node) {
    for (StatementNode* stmt : node.statements) {
        if (stmt) stmt->accept(*this);
    }
}

void CodeGenerator::visit(AssignStatementNode& node) {
    node.expression->accept(*this);

    if (node.variable->determinedType == EntryTypeCategory::ET_REAL &&
        node.expression->determinedType == EntryTypeCategory::ET_INTEGER) {
        emit("itof");
    }

    SymbolEntry* entry = symbolTable->lookupSymbol(node.variable->identifier->name);
    if (!entry) throw std::runtime_error("CodeGen Error: Symbol not found: " + node.variable->identifier->name);

    if (node.variable->index) { // Array element assignment
        // Stack: [value_to_store, base_address_of_array, index]
        node.variable->index->accept(*this); // Push index
        if (node.variable->scope == SymbolScope::LOCAL) {
            emit("pushl", std::to_string(entry->offset));
        }
        else {
            emit("pushg", std::to_string(entry->offset));
        }
        emit("swap"); // Stack: [..., base_addr, index, value_to_store]
        emit("pushi", std::to_string(entry->arrayDetails.lowBound));
        emit("sub"); // Calculate offset from base: index - low_bound
        emit("storen"); // stores value at base[offset]
    }
    else { // Simple variable assignment
        if (entry->kind == SymbolKind::SK_PARAMETER) {
            emit("storel", std::to_string(-(entry->offset + 1)));
        }
        else if (node.variable->scope == SymbolScope::LOCAL) {
            emit("storel", std::to_string(entry->offset));
        }
        else {
            emit("storeg", std::to_string(entry->offset));
        }
    }
}

void CodeGenerator::visit(IfStatementNode& node) {
    std::string elseLabel = newLabel("ELSE");
    std::string endIfLabel = newLabel("END_IF");

    node.condition->accept(*this);
    emit("jz", elseLabel); // Jump if condition is false (0)

    node.thenStatement->accept(*this);
    if (node.elseStatement) emit("jump", endIfLabel); // After THEN, skip the ELSE block

    emitLabel(elseLabel);
    if (node.elseStatement) node.elseStatement->accept(*this);

    emitLabel(endIfLabel);
}

void CodeGenerator::visit(WhileStatementNode& node) {
    std::string loopStartLabel = newLabel("WHILE_START");
    std::string loopEndLabel = newLabel("WHILE_END");

    emitLabel(loopStartLabel);
    node.condition->accept(*this);
    emit("jz", loopEndLabel); // Exit loop if condition is false

    node.body->accept(*this);
    emit("jump", loopStartLabel); // Go back to condition check

    emitLabel(loopEndLabel);
}

void CodeGenerator::visit(ProcedureCallStatementNode& node) {
    const std::string& procName = node.procName->name;

    if (procName == "write" || procName == "writeln") {
        if (node.arguments) {
            for (auto* arg : node.arguments->expressions) {
                arg->accept(*this);
                if (dynamic_cast<StringLiteralNode*>(arg)) emit("writes");
                else if (arg->determinedType == EntryTypeCategory::ET_INTEGER || arg->determinedType == EntryTypeCategory::ET_BOOLEAN) emit("writei");
                else if (arg->determinedType == EntryTypeCategory::ET_REAL) emit("writef");
            }
        }
        if (procName == "writeln") {
            emit("pushs", "\"\\n\"");
            emit("writes");
        }
        return;
    }

    if (procName == "read" || procName == "readln") {
        if (node.arguments) {
            for (ExprNode* argExpr : node.arguments->expressions) {
                // Determine variable details from the AST node, annotated by semantic analysis
                auto* varNode = dynamic_cast<VariableNode*>(argExpr);
                if (!varNode) throw std::runtime_error("CodeGen Error: Argument to read must be a variable.");
                SymbolEntry* entry = symbolTable->lookupSymbol(varNode->identifier->name);
                if (!entry) throw std::runtime_error("CodeGen Error: Symbol for read not found: " + varNode->identifier->name);

                // Generate code to store the read value
                emit("read");
                if (varNode->determinedType == EntryTypeCategory::ET_INTEGER) emit("atoi");
                else if (varNode->determinedType == EntryTypeCategory::ET_REAL) emit("atof");

                // Now store it in the right place
                if (entry->kind == SymbolKind::SK_PARAMETER) emit("storel", std::to_string(-(entry->offset + 1)));
                else if (varNode->scope == SymbolScope::LOCAL) emit("storel", std::to_string(entry->offset));
                else emit("storeg", std::to_string(entry->offset));
            }
        }
        return;
    }

    // Standard user-defined procedure call
    SymbolEntry* entry = symbolTable->lookupSymbol(procName);
    if (!entry) throw std::runtime_error("CodeGen Error: Procedure not found: " + procName);

    // Push arguments in reverse order
    if (node.arguments) {
        auto& exprs = node.arguments->expressions;
        for (auto it = exprs.rbegin(); it != exprs.rend(); ++it) {
            (*it)->accept(*this);
        }
    }

    emit("pusha", procName);
    emit("call");
    if (entry->numParameters > 0) emit("pop", std::to_string(entry->numParameters));
}

void CodeGenerator::visit(ReturnStatementNode& node) {
    if (node.returnValue) {
        if (!currentFunctionContext) throw std::runtime_error("CodeGen Error: Return statement found outside of a function context.");

        SymbolEntry* funcEntry = symbolTable->lookupSymbol(currentFunctionContext->name->name);
        if (!funcEntry) throw std::runtime_error("CodeGen Error: Could not retrieve function definition for return.");

        node.returnValue->accept(*this); // Push return value

        // Promote if necessary
        if (funcEntry->functionReturnType == EntryTypeCategory::ET_REAL && node.returnValue->determinedType == EntryTypeCategory::ET_INTEGER) {
            emit("itof");
        }

        // Store in the return value slot [fp - (num_params + 1)]
        int return_slot_offset = -(static_cast<int>(funcEntry->numParameters) + 1);
        emit("storel", std::to_string(return_slot_offset));
    }
    emit("return");
}

void CodeGenerator::visit(IntNumNode& node) { emit("pushi", std::to_string(node.value)); }
void CodeGenerator::visit(RealNumNode& node) { emit("pushf", std::to_string(node.value)); }
void CodeGenerator::visit(BooleanLiteralNode& node) { emit("pushi", node.value ? "1" : "0"); }
void CodeGenerator::visit(StringLiteralNode& node) { emit("pushs", "\"" + node.value + "\""); }

void CodeGenerator::visit(BinaryOpNode& node) {
    bool is_real_op = (node.left->determinedType == EntryTypeCategory::ET_REAL || node.right->determinedType == EntryTypeCategory::ET_REAL);
    // Real division always results in a real
    if (node.op == "/") is_real_op = true;

    node.left->accept(*this);
    if (is_real_op && node.left->determinedType == EntryTypeCategory::ET_INTEGER) emit("itof");

    node.right->accept(*this);
    if (is_real_op && node.right->determinedType == EntryTypeCategory::ET_INTEGER) emit("itof");

    if (node.op == "+") emit(is_real_op ? "fadd" : "add");
    else if (node.op == "-") emit(is_real_op ? "fsub" : "sub");
    else if (node.op == "*") emit(is_real_op ? "fmul" : "mul");
    else if (node.op == "/") emit("fdiv");
    else if (node.op == "DIV_OP") emit("div");
    else if (node.op == "EQ_OP") emit("equal");
    else if (node.op == "NEQ_OP") { emit("equal"); emit("not"); }
    else if (node.op == "LT_OP") emit(is_real_op ? "finf" : "inf");
    else if (node.op == "LTE_OP") emit(is_real_op ? "finfeq" : "infeq");
    else if (node.op == "GT_OP") emit(is_real_op ? "fsup" : "sup");
    else if (node.op == "GTE_OP") emit(is_real_op ? "fsupeq" : "supeq");
    else if (node.op == "AND_OP") emit("mul"); // Logical AND is like multiplication
    else if (node.op == "OR_OP") { emit("add"); emit("pushi", "0"); emit("sup"); } // Logical OR: if (a+b)>0
    else throw std::runtime_error("CodeGen Error: Unsupported binary operator '" + node.op + "'");
}

void CodeGenerator::visit(UnaryOpNode& node) {
    node.expression->accept(*this);
    if (node.op == "-") {
        if (node.expression->determinedType == EntryTypeCategory::ET_REAL) {
            emit("pushf", "0.0"); emit("swap"); emit("fsub");
        }
        else {
            emit("pushi", "0"); emit("swap"); emit("sub");
        }
    }
    else if (node.op == "NOT_OP") {
        emit("not");
    }
}

void CodeGenerator::visit(VariableNode& node) {
    SymbolEntry* entry = symbolTable->lookupSymbol(node.identifier->name);
    if (!entry) throw std::runtime_error("CodeGen Error: Symbol not found: " + node.identifier->name);

    if (node.index) { // Array element access
        if (!entry->arrayDetails.isInitialized) throw std::runtime_error("CodeGen Error: Array details missing for " + node.identifier->name);

        if (node.scope == SymbolScope::LOCAL) emit("pushl", std::to_string(entry->offset));
        else emit("pushg", std::to_string(entry->offset));

        node.index->accept(*this);
        emit("pushi", std::to_string(entry->arrayDetails.lowBound));
        emit("sub"); // Calculate offset
        emit("loadn"); // Load value from base[offset]
    }
    else { // Simple variable
        if (entry->kind == SymbolKind::SK_PARAMETER) {
            emit("pushl", std::to_string(-(entry->offset + 1)));
        }
        else if (node.scope == SymbolScope::LOCAL) {
            emit("pushl", std::to_string(entry->offset));
        }
        else {
            emit("pushg", std::to_string(entry->offset));
        }
    }
}

void CodeGenerator::visit(FunctionCallExprNode& node) {
    const std::string& funcName = node.funcName->name;
    SymbolEntry* entry = symbolTable->lookupSymbol(funcName);
    if (!entry) throw std::runtime_error("CodeGen Error: Function not found: " + funcName);

    emit("pushn", "1"); // Allocate space for return value

    if (node.arguments) {
        auto& exprs = node.arguments->expressions;
        for (auto it = exprs.rbegin(); it != exprs.rend(); ++it) {
            (*it)->accept(*this);
        }
    }

    emit("pusha", funcName);
    emit("call");

    if (entry->numParameters > 0) emit("pop", std::to_string(entry->numParameters));
}

void CodeGenerator::visit(IdExprNode& node) {
    SymbolEntry* entry = symbolTable->lookupSymbol(node.ident->name);
    if (!entry) throw std::runtime_error("CodeGen Error: Symbol not found for ID: " + node.ident->name);

    if (entry->kind == SymbolKind::SK_FUNCTION) {
        if (entry->numParameters != 0) {
            throw std::runtime_error("CodeGen Error: Function '" + node.ident->name + "' requires arguments.");
        }
        // Parameterless function call
        emit("pushn", "1");
        emit("pusha", node.ident->name);
        emit("call");
        return;
    }

    // Handle as a variable or parameter
    if (entry->kind == SymbolKind::SK_PARAMETER) {
        emit("pushl", std::to_string(-(entry->offset + 1)));
    }
    else if (node.scope == SymbolScope::LOCAL) {
        emit("pushl", std::to_string(entry->offset));
    }
    else {
        emit("pushg", std::to_string(entry->offset));
    }
}

