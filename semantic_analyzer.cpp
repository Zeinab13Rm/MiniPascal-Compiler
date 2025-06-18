#include "semantic_analyzer.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>

// Constructor: Pre-populate the global scope with built-in procedures.
SemanticAnalyzer::SemanticAnalyzer() : currentFunctionContext(nullptr), global_offset(0), local_offset(0), param_offset(0) {
    // These built-ins don't have a standard signature, so we use an empty vector.
    // Their special argument handling is checked by name in the visit(ProcedureCallStatementNode) method.
    std::vector<std::pair<EntryTypeCategory, ArrayDetails>> empty_signature;

    symbolTable.addSymbol(SymbolEntry("read", empty_signature, 0, 0));
    symbolTable.addSymbol(SymbolEntry("readln", empty_signature, 0, 0));
    symbolTable.addSymbol(SymbolEntry("write", empty_signature, 0, 0));
    symbolTable.addSymbol(SymbolEntry("writeln", empty_signature, 0, 0));
}

void SemanticAnalyzer::recordError(const std::string& message, int line, int col) {
    std::string error = "Semantic Error (L:" + std::to_string(line) + ", C:" + std::to_string(col) + "): " + message;
    semanticErrors.push_back(error);
}

bool SemanticAnalyzer::hasErrors() const {
    return !semanticErrors.empty();
}

void SemanticAnalyzer::printErrors(std::ostream& out) const {
    for (const auto& err : semanticErrors) {
        out << err << std::endl;
    }
}

// --- Helper methods for type conversion and validation ---

EntryTypeCategory SemanticAnalyzer::astStandardTypeToSymbolType(StandardTypeNode* astStandardTypeNode) {
    if (!astStandardTypeNode) return EntryTypeCategory::ET_UNKNOWN;
    switch (astStandardTypeNode->category) {
    case StandardTypeNode::TYPE_INTEGER: return EntryTypeCategory::ET_INTEGER;
    case StandardTypeNode::TYPE_REAL:   return EntryTypeCategory::ET_REAL;
    case StandardTypeNode::TYPE_BOOLEAN:return EntryTypeCategory::ET_BOOLEAN;
    default:
        recordError("Internal: Unknown standard type category.", astStandardTypeNode->line, astStandardTypeNode->column);
        return EntryTypeCategory::ET_UNKNOWN;
    }
}

EntryTypeCategory SemanticAnalyzer::astToSymbolType(TypeNode* astTypeNode, ArrayDetails& outArrayDetails) {
    outArrayDetails.isInitialized = false;
    if (!astTypeNode) return EntryTypeCategory::ET_UNKNOWN;

    if (auto* stn = dynamic_cast<StandardTypeNode*>(astTypeNode)) {
        return astStandardTypeToSymbolType(stn);
    }
    else if (auto* atn = dynamic_cast<ArrayTypeNode*>(astTypeNode)) {
        if (atn->elementType) {
            outArrayDetails.elementType = astStandardTypeToSymbolType(atn->elementType);
        }
        else {
            recordError("Array type declaration is missing its element type.", atn->line, atn->column);
            outArrayDetails.elementType = EntryTypeCategory::ET_UNKNOWN;
        }
        if (atn->startIndex && atn->endIndex) {
            outArrayDetails.lowBound = atn->startIndex->value;
            outArrayDetails.highBound = atn->endIndex->value;
            outArrayDetails.isInitialized = true;
            if (outArrayDetails.lowBound > outArrayDetails.highBound) {
                recordError("For array type, lower bound cannot exceed upper bound.", atn->line, atn->column);
            }
        }
        else {
            recordError("Array type declaration is missing bounds.", atn->line, atn->column);
        }
        return EntryTypeCategory::ET_ARRAY;
    }
    recordError("Internal: Unrecognized AST type node.", astTypeNode->line, astTypeNode->column);
    return EntryTypeCategory::ET_UNKNOWN;
}

bool SemanticAnalyzer::isPrintableType(EntryTypeCategory type, ExprNode* argNode) {
    if (type == EntryTypeCategory::ET_INTEGER ||
        type == EntryTypeCategory::ET_REAL ||
        type == EntryTypeCategory::ET_BOOLEAN) {
        return true;
    }
    // MiniPascal only supports string literals for printing, not string variables.
    if (dynamic_cast<StringLiteralNode*>(argNode)) {
        return true;
    }
    return false;
}

bool SemanticAnalyzer::isReadableType(EntryTypeCategory type) {
    return type == EntryTypeCategory::ET_INTEGER ||
        type == EntryTypeCategory::ET_REAL;
}


// --- Visitor Implementations ---

void SemanticAnalyzer::visit(ProgramNode& node) {
    symbolTable.addSymbol(SymbolEntry(node.progName->name, SymbolKind::SK_PROGRAM_NAME, EntryTypeCategory::ET_NO_TYPE, node.progName->line, node.progName->column));
    if (node.decls) node.decls->accept(*this);
    if (node.subprogs) node.subprogs->accept(*this);
    if (node.mainCompoundStmt) node.mainCompoundStmt->accept(*this);
}

void SemanticAnalyzer::visit(Declarations& node) {
    for (VarDecl* varDecl : node.var_decl_items) {
        if (varDecl) varDecl->accept(*this);
    }
}

void SemanticAnalyzer::visit(VarDecl& node) {
    ArrayDetails ad;
    EntryTypeCategory var_type = astToSymbolType(node.type, ad);

    for (IdentNode* identNode : node.identifiers->identifiers) {
        if (!identNode) continue;

        int current_offset = symbolTable.isGlobalScope() ? global_offset++ : local_offset++;
        SymbolEntry entry(identNode->name, SymbolKind::SK_VARIABLE, var_type, identNode->line, identNode->column);
        entry.offset = current_offset;
        if (var_type == EntryTypeCategory::ET_ARRAY) {
            entry.arrayDetails = ad;
        }

        if (!symbolTable.addSymbol(entry)) {
            SymbolEntry* existing = symbolTable.lookupSymbolInCurrentScope(identNode->name);
            std::string conflictMsg = existing ? " Conflicts with " + symbolKindToString(existing->kind) + " at L:" + std::to_string(existing->declLine) : "";
            recordError("Variable '" + identNode->name + "' re-declared in this scope." + conflictMsg, identNode->line, identNode->column);
        }
    }
}

void SemanticAnalyzer::visit(SubprogramDeclarations& node) {
    // First pass: Add all subprogram headers to the symbol table to allow for forward declarations and mutual recursion.
    for (SubprogramDeclaration* subDecl : node.subprograms) {
        if (subDecl && subDecl->head) {
            subDecl->head->accept(*this);
        }
    }
    // Second pass: Visit the full declaration body.
    for (SubprogramDeclaration* subDecl : node.subprograms) {
        if (subDecl) {
            subDecl->accept(*this);
        }
    }
}

void SemanticAnalyzer::visit(SubprogramDeclaration& node) {
    if (!node.head || !node.head->name) return;

    // The head was already processed in the first pass of visit(SubprogramDeclarations),
    // so the name is already in the parent scope's symbol table.

    symbolTable.enterScope();
    local_offset = 0; // Reset for local variables
    param_offset = 0; // Reset for parameters

    FunctionHeadNode* previousFunctionContext = currentFunctionContext;
    if (auto funcHead = dynamic_cast<FunctionHeadNode*>(node.head)) {
        currentFunctionContext = funcHead;
    }
    else {
        currentFunctionContext = nullptr; // It's a procedure
    }

    if (node.head->arguments) node.head->arguments->accept(*this);
    if (node.local_declarations) node.local_declarations->accept(*this);
    if (node.body) node.body->accept(*this);

    symbolTable.exitScope();
    currentFunctionContext = previousFunctionContext; // Restore context
}

void SemanticAnalyzer::visit(FunctionHeadNode& node) {
    // This is part of the first pass. It adds the function signature to the current scope.
    EntryTypeCategory return_type = EntryTypeCategory::ET_UNKNOWN;
    if (node.returnType) {
        return_type = astStandardTypeToSymbolType(node.returnType);
    }
    else {
        recordError("Function '" + node.name->name + "' is missing its return type.", node.line, node.column);
    }

    std::vector<std::pair<EntryTypeCategory, ArrayDetails>> signature;
    if (node.arguments && node.arguments->params) {
        for (const auto& param_decl_group : node.arguments->params->paramDeclarations) {
            if (param_decl_group && param_decl_group->ids && param_decl_group->type) {
                ArrayDetails ad_param;
                EntryTypeCategory param_cat_type = astToSymbolType(param_decl_group->type, ad_param);
                for (size_t i = 0; i < param_decl_group->ids->identifiers.size(); ++i) {
                    signature.push_back({ param_cat_type, ad_param });
                }
            }
        }
    }

    SymbolEntry entry(node.name->name, return_type, signature, node.name->line, node.name->column);
    if (!symbolTable.addSymbol(entry)) {
        SymbolEntry* existing = symbolTable.lookupSymbolInCurrentScope(node.name->name);
        std::string conflictMsg = existing ? " Conflicts with existing " + symbolKindToString(existing->kind) + " at L:" + std::to_string(existing->declLine) : "";
        recordError("Function '" + node.name->name + "' re-declared in this scope." + conflictMsg, node.name->line, node.name->column);
    }
}

void SemanticAnalyzer::visit(ProcedureHeadNode& node) {
    // This is part of the first pass. It adds the procedure signature to the current scope.
    std::vector<std::pair<EntryTypeCategory, ArrayDetails>> signature;
    if (node.arguments && node.arguments->params) {
        for (const auto& param_decl_group : node.arguments->params->paramDeclarations) {
            if (param_decl_group && param_decl_group->ids && param_decl_group->type) {
                ArrayDetails ad_param;
                EntryTypeCategory param_cat_type = astToSymbolType(param_decl_group->type, ad_param);
                for (size_t i = 0; i < param_decl_group->ids->identifiers.size(); ++i) {
                    signature.push_back({ param_cat_type, ad_param });
                }
            }
        }
    }
    SymbolEntry entry(node.name->name, signature, node.name->line, node.name->column);
    if (!symbolTable.addSymbol(entry)) {
        SymbolEntry* existing = symbolTable.lookupSymbolInCurrentScope(node.name->name);
        std::string conflictMsg = existing ? " Conflicts with existing " + symbolKindToString(existing->kind) + " at L:" + std::to_string(existing->declLine) : "";
        recordError("Procedure '" + node.name->name + "' re-declared in this scope." + conflictMsg, node.name->line, node.name->column);
    }
}

void SemanticAnalyzer::visit(ArgumentsNode& node) {
    // This processes the parameters during the second pass, inside the new scope.
    if (node.params) {
        node.params->accept(*this);
    }
}

void SemanticAnalyzer::visit(ParameterList& node) {
    for (ParameterDeclaration* paramDecl : node.paramDeclarations) {
        if (paramDecl) paramDecl->accept(*this);
    }
}

void SemanticAnalyzer::visit(ParameterDeclaration& node) {
    ArrayDetails ad;
    EntryTypeCategory param_type = astToSymbolType(node.type, ad);

    for (IdentNode* identNode : node.ids->identifiers) {
        SymbolEntry entry(identNode->name, SymbolKind::SK_PARAMETER, param_type, 
                         identNode->line, identNode->column);
        entry.offset = param_offset++;  // Critical for code generation
        
        std::cout << "Adding parameter: " << identNode->name 
                  << " type: " << entryTypeToString(param_type)
                  << " offset: " << entry.offset << "\n";
        if (param_type == EntryTypeCategory::ET_ARRAY) {
            entry.arrayDetails = ad;
        }

        if (!symbolTable.addSymbol(entry)) {
            SymbolEntry* existing = symbolTable.lookupSymbolInCurrentScope(identNode->name);
            std::string conflictMsg = existing ? " Conflicts with existing " + symbolKindToString(existing->kind) + " at L:" + std::to_string(existing->declLine) : "";
            recordError("Parameter '" + identNode->name + "' re-declared in this scope." + conflictMsg, identNode->line, identNode->column);
        }
    }
}

void SemanticAnalyzer::visit(CompoundStatementNode& node) {
    if (node.stmts) node.stmts->accept(*this);
}

void SemanticAnalyzer::visit(StatementList& node) {
    for (StatementNode* stmt : node.statements) {
        if (stmt) stmt->accept(*this);
    }
}

void SemanticAnalyzer::visit(AssignStatementNode& node) {
    if (!node.variable || !node.expression) return;

    node.variable->accept(*this);
    node.expression->accept(*this);

    EntryTypeCategory lhsType = node.variable->determinedType;
    EntryTypeCategory rhsType = node.expression->determinedType;

    if (lhsType == EntryTypeCategory::ET_UNKNOWN || rhsType == EntryTypeCategory::ET_UNKNOWN) {
        return; // Avoid cascading errors from undeclared vars
    }

    bool compatible = false;
    if (lhsType == rhsType) {
        if (lhsType == EntryTypeCategory::ET_ARRAY && node.variable->index == nullptr) {
            recordError("Whole array assignment is not supported.", node.line, node.column);
        }
        else {
            compatible = true;
        }
    }
    else if (lhsType == EntryTypeCategory::ET_REAL && rhsType == EntryTypeCategory::ET_INTEGER) {
        compatible = true; // Allow assigning an integer to a real
    }

    if (!compatible) {
        recordError("Type mismatch in assignment. Cannot assign type '" + entryTypeToString(rhsType) + "' to a variable of type '" + entryTypeToString(lhsType) + "'.", node.line, node.column);
    }
}

void SemanticAnalyzer::visit(IfStatementNode& node) {
    node.condition->accept(*this);
    if (node.condition->determinedType != EntryTypeCategory::ET_BOOLEAN && node.condition->determinedType != EntryTypeCategory::ET_UNKNOWN) {
        recordError("IF condition must be a BOOLEAN expression, but found '" + entryTypeToString(node.condition->determinedType) + "'.", node.condition->line, node.condition->column);
    }
    node.thenStatement->accept(*this);
    if (node.elseStatement) node.elseStatement->accept(*this);
}

void SemanticAnalyzer::visit(WhileStatementNode& node) {
    node.condition->accept(*this);
    if (node.condition->determinedType != EntryTypeCategory::ET_BOOLEAN && node.condition->determinedType != EntryTypeCategory::ET_UNKNOWN) {
        recordError("WHILE condition must be a BOOLEAN expression, but found '" + entryTypeToString(node.condition->determinedType) + "'.", node.condition->line, node.condition->column);
    }
    node.body->accept(*this);
}

void SemanticAnalyzer::visit(VariableNode& node) {
    SymbolEntry* entry = symbolTable.lookupSymbol(node.identifier->name);
    if (!entry) {
        recordError("Identifier '" + node.identifier->name + "' is not declared.", node.identifier->line, node.identifier->column);
        node.determinedType = EntryTypeCategory::ET_UNKNOWN;
        return;
    }

    // Annotate the node with info from the symbol table for the code generator
    node.offset = entry->offset;
    node.kind = entry->kind;

    // Determine scope
    SymbolEntry* current_scope_check = symbolTable.lookupSymbolInCurrentScope(node.identifier->name);
    if (current_scope_check && current_scope_check == entry) {
        node.scope = SymbolScope::LOCAL;
    }
    else {
        node.scope = SymbolScope::GLOBAL;
    }

    if (entry->kind != SymbolKind::SK_VARIABLE && entry->kind != SymbolKind::SK_PARAMETER) {
        recordError("Identifier '" + node.identifier->name + "' is a " + symbolKindToString(entry->kind) + " and cannot be used as a variable here.", node.identifier->line, node.identifier->column);
        node.determinedType = EntryTypeCategory::ET_UNKNOWN;
        return;
    }

    if (node.index) { // This is an array element access like my_array[i]
        if (entry->type != EntryTypeCategory::ET_ARRAY) {
            recordError("Identifier '" + node.identifier->name + "' is not an array and cannot be indexed.", node.identifier->line, node.identifier->column);
            node.determinedType = EntryTypeCategory::ET_UNKNOWN;
            return;
        }
        node.index->accept(*this);
        if (node.index->determinedType != EntryTypeCategory::ET_INTEGER && node.index->determinedType != EntryTypeCategory::ET_UNKNOWN) {
            recordError("Array index must be an INTEGER expression.", node.index->line, node.index->column);
        }
        node.determinedType = entry->arrayDetails.elementType;
    }
    else { // Simple variable access
        node.determinedType = entry->type;
        if (entry->type == EntryTypeCategory::ET_ARRAY) {
            node.determinedArrayDetails = entry->arrayDetails;
        }
    }
}

void SemanticAnalyzer::visit(ProcedureCallStatementNode& node) {
    const std::string& procNameStr = node.procName->name;
    SymbolEntry* entry = symbolTable.lookupSymbol(procNameStr);

    if (!entry) {
        recordError("Procedure or function '" + procNameStr + "' is not declared.", node.procName->line, node.procName->column);
        return;
    }

    // Special handling for built-in I/O
    if (procNameStr == "write" || procNameStr == "writeln") {
        if (node.arguments) {
            for (ExprNode* argExpr : node.arguments->expressions) {
                argExpr->accept(*this);
                if (!isPrintableType(argExpr->determinedType, argExpr)) {
                    recordError("Argument of type '" + entryTypeToString(argExpr->determinedType) + "' is not printable.", argExpr->line, argExpr->column);
                }
            }
        }
        return;
    }
    else if (procNameStr == "read" || procNameStr == "readln") {
        if (!node.arguments || node.arguments->expressions.empty()) {
            recordError("Procedure '" + procNameStr + "' requires at least one variable argument.", node.procName->line, node.procName->column);
        }
        else {
            for (ExprNode* argExpr : node.arguments->expressions) {
                // Arguments to read must be assignable l-values
                if (!dynamic_cast<VariableNode*>(argExpr) && !dynamic_cast<IdExprNode*>(argExpr)) {
                    recordError("Argument to '" + procNameStr + "' must be a variable.", argExpr->line, argExpr->column);
                    continue;
                }
                argExpr->accept(*this);
                if (!isReadableType(argExpr->determinedType)) {
                    recordError("Variable argument of type '" + entryTypeToString(argExpr->determinedType) + "' cannot be read into.", argExpr->line, argExpr->column);
                }
            }
        }
        return;
    }

    // Standard user-defined procedure call
    if (entry->kind != SymbolKind::SK_PROCEDURE) {
        recordError("'" + procNameStr + "' is a " + symbolKindToString(entry->kind) + ", not a procedure.", node.procName->line, node.procName->column);
        return;
    }

    size_t actualArgCount = node.arguments ? node.arguments->expressions.size() : 0;
    if (actualArgCount != entry->numParameters) {
        recordError("Procedure '" + procNameStr + "' called with " + std::to_string(actualArgCount) + " arguments, but expects " + std::to_string(entry->numParameters) + ".", node.procName->line, node.procName->column);
    }

    // Still check types for the arguments that are provided
    if (node.arguments) node.arguments->accept(*this);
}


void SemanticAnalyzer::visit(FunctionCallExprNode& node) {
    const std::string& funcNameStr = node.funcName->name;
    SymbolEntry* entry = symbolTable.lookupSymbol(funcNameStr);

    if (!entry) {
        recordError("Function '" + funcNameStr + "' is not declared.", node.funcName->line, node.funcName->column);
        node.determinedType = EntryTypeCategory::ET_UNKNOWN;
        return;
    }
    if (entry->kind != SymbolKind::SK_FUNCTION) {
        recordError("'" + funcNameStr + "' is a " + symbolKindToString(entry->kind) + ", not a function.", node.funcName->line, node.funcName->column);
        node.determinedType = EntryTypeCategory::ET_UNKNOWN;
        return;
    }

    node.determinedType = entry->functionReturnType;

    size_t actualArgCount = node.arguments ? node.arguments->expressions.size() : 0;
    if (actualArgCount != entry->numParameters) {
        recordError("Function '" + funcNameStr + "' called with " + std::to_string(actualArgCount) + " arguments, but expects " + std::to_string(entry->numParameters) + ".", node.funcName->line, node.funcName->column);
    }

    if (node.arguments) node.arguments->accept(*this);
}

void SemanticAnalyzer::visit(ReturnStatementNode& node) {
    if (!currentFunctionContext) {
        recordError("RETURN statement can only be used inside a function.", node.line, node.column);
        return;
    }
    if (!node.returnValue) {
        recordError("RETURN statement in a function must provide a value.", node.line, node.column);
        return;
    }

    node.returnValue->accept(*this);
    EntryTypeCategory actualReturnType = node.returnValue->determinedType;

    SymbolEntry* funcEntry = symbolTable.lookupSymbol(currentFunctionContext->name->name);
    if (!funcEntry || funcEntry->kind != SymbolKind::SK_FUNCTION) return; // Should not happen

    EntryTypeCategory expectedReturnType = funcEntry->functionReturnType;

    bool compatible = (actualReturnType == expectedReturnType) ||
        (expectedReturnType == EntryTypeCategory::ET_REAL && actualReturnType == EntryTypeCategory::ET_INTEGER);

    if (!compatible && actualReturnType != EntryTypeCategory::ET_UNKNOWN) {
        recordError("Return type mismatch. Function '" + funcEntry->name + "' expects '" + entryTypeToString(expectedReturnType) + "' but got '" + entryTypeToString(actualReturnType) + "'.", node.returnValue->line, node.returnValue->column);
    }
}


void SemanticAnalyzer::visit(IdExprNode& node) {
    SymbolEntry* entry = symbolTable.lookupSymbol(node.ident->name);
    if (!entry) {
        recordError("Identifier '" + node.ident->name + "' is not declared.", node.ident->line, node.ident->column);
        node.determinedType = EntryTypeCategory::ET_UNKNOWN;
        return;
    }

    // Annotate the node with info for the code generator
    node.offset = entry->offset;
    node.kind = entry->kind;
    SymbolEntry* current_scope_check = symbolTable.lookupSymbolInCurrentScope(node.ident->name);
    node.scope = (current_scope_check && current_scope_check == entry) ? SymbolScope::LOCAL : SymbolScope::GLOBAL;

    if (entry->kind == SymbolKind::SK_VARIABLE || entry->kind == SymbolKind::SK_PARAMETER) {
        node.determinedType = entry->type;
        if (entry->type == EntryTypeCategory::ET_ARRAY) {
            node.determinedArrayDetails = entry->arrayDetails;
        }
    }
    else if (entry->kind == SymbolKind::SK_FUNCTION) {
        // This handles parameterless function calls, e.g., `x := my_func;`
        if (entry->numParameters == 0) {
            node.determinedType = entry->functionReturnType;
        }
        else {
            recordError("Function '" + node.ident->name + "' requires arguments and parentheses for a call.", node.ident->line, node.ident->column);
            node.determinedType = EntryTypeCategory::ET_UNKNOWN;
        }
    }
    else {
        recordError("Identifier '" + node.ident->name + "' (" + symbolKindToString(entry->kind) + ") cannot be used as an expression.", node.ident->line, node.ident->column);
        node.determinedType = EntryTypeCategory::ET_UNKNOWN;
    }
}


void SemanticAnalyzer::visit(BinaryOpNode& node) {
    node.determinedType = EntryTypeCategory::ET_UNKNOWN;
    node.left->accept(*this);
    node.right->accept(*this);

    EntryTypeCategory leftType = node.left->determinedType;
    EntryTypeCategory rightType = node.right->determinedType;

    if (leftType == EntryTypeCategory::ET_UNKNOWN || rightType == EntryTypeCategory::ET_UNKNOWN) return;

    const std::string& op = node.op;
    if (op == "+" || op == "-" || op == "*") {
        if ((leftType == EntryTypeCategory::ET_INTEGER || leftType == EntryTypeCategory::ET_REAL) &&
            (rightType == EntryTypeCategory::ET_INTEGER || rightType == EntryTypeCategory::ET_REAL)) {
            node.determinedType = (leftType == EntryTypeCategory::ET_REAL || rightType == EntryTypeCategory::ET_REAL) ? EntryTypeCategory::ET_REAL : EntryTypeCategory::ET_INTEGER;
        }
        else {
            recordError("Operands for '" + op + "' must both be numeric.", node.line, node.column);
        }
    }
    else if (op == "/") { // Real division
        if ((leftType == EntryTypeCategory::ET_INTEGER || leftType == EntryTypeCategory::ET_REAL) &&
            (rightType == EntryTypeCategory::ET_INTEGER || rightType == EntryTypeCategory::ET_REAL)) {
            node.determinedType = EntryTypeCategory::ET_REAL;
        }
        else {
            recordError("Operands for '/' must both be numeric.", node.line, node.column);
        }
    }
    else if (op == "DIV_OP") { // Integer division
        if (leftType == EntryTypeCategory::ET_INTEGER && rightType == EntryTypeCategory::ET_INTEGER) {
            node.determinedType = EntryTypeCategory::ET_INTEGER;
        }
        else {
            recordError("Operands for 'DIV' must both be INTEGER.", node.line, node.column);
        }
    }
    else if (op == "AND_OP" || op == "OR_OP") {
        if (leftType == EntryTypeCategory::ET_BOOLEAN && rightType == EntryTypeCategory::ET_BOOLEAN) {
            node.determinedType = EntryTypeCategory::ET_BOOLEAN;
        }
        else {
            recordError("Operands for '" + op + "' must both be BOOLEAN.", node.line, node.column);
        }
    }
    else if (op == "EQ_OP" || op == "NEQ_OP" || op == "LT_OP" || op == "LTE_OP" || op == "GT_OP" || op == "GTE_OP") {
        bool compatible = (leftType == rightType && (leftType == EntryTypeCategory::ET_BOOLEAN || leftType == EntryTypeCategory::ET_INTEGER || leftType == EntryTypeCategory::ET_REAL)) ||
            ((leftType == EntryTypeCategory::ET_REAL || leftType == EntryTypeCategory::ET_INTEGER) && (rightType == EntryTypeCategory::ET_REAL || rightType == EntryTypeCategory::ET_INTEGER));
        if (compatible) {
            node.determinedType = EntryTypeCategory::ET_BOOLEAN;
        }
        else {
            recordError("Incompatible types for relational operator '" + op + "'.", node.line, node.column);
        }
    }
}

void SemanticAnalyzer::visit(UnaryOpNode& node) {
    node.determinedType = EntryTypeCategory::ET_UNKNOWN;
    node.expression->accept(*this);
    EntryTypeCategory operandType = node.expression->determinedType;
    if (operandType == EntryTypeCategory::ET_UNKNOWN) return;

    if (node.op == "-") {
        if (operandType == EntryTypeCategory::ET_INTEGER || operandType == EntryTypeCategory::ET_REAL) {
            node.determinedType = operandType;
        }
        else {
            recordError("Operand for unary '-' must be numeric.", node.line, node.column);
        }
    }
    else if (node.op == "NOT_OP") {
        if (operandType == EntryTypeCategory::ET_BOOLEAN) {
            node.determinedType = EntryTypeCategory::ET_BOOLEAN;
        }
        else {
            recordError("Operand for 'NOT' must be BOOLEAN.", node.line, node.column);
        }
    }
}

// Visit methods for literals just set the determined type
void SemanticAnalyzer::visit(IntNumNode& node) { node.determinedType = EntryTypeCategory::ET_INTEGER; }
void SemanticAnalyzer::visit(RealNumNode& node) { node.determinedType = EntryTypeCategory::ET_REAL; }
void SemanticAnalyzer::visit(BooleanLiteralNode& node) { node.determinedType = EntryTypeCategory::ET_BOOLEAN; }
void SemanticAnalyzer::visit(StringLiteralNode& node) { node.determinedType = EntryTypeCategory::ET_UNKNOWN; /* Strings are typeless for assignment */ }

// Visit methods for lists just recurse
void SemanticAnalyzer::visit(ExpressionList& node) { for (auto expr : node.expressions) expr->accept(*this); }
void SemanticAnalyzer::visit(IdentifierList& node) { for (auto id : node.identifiers) id->accept(*this); }

// These nodes are processed by their parents, so their visit methods can be empty
void SemanticAnalyzer::visit(IdentNode& node) {}
void SemanticAnalyzer::visit(StandardTypeNode& node) {}
void SemanticAnalyzer::visit(ArrayTypeNode& node) {}
