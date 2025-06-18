#include "ast.h"
#include "semantic_visitor.h"
#include <iostream>
#include <sstream>
#include <iterator>

/*
* SEMANTIC TYPE HELPER IMPLEMENTATIONS
*/
inline std::string symbolKindToString(SymbolKind kind) {
    switch (kind) {
    case SymbolKind::SK_VARIABLE: return "SK_VARIABLE";
    case SymbolKind::SK_PARAMETER: return "Parameter";
    case SymbolKind::SK_FUNCTION: return "Function";
    case SymbolKind::SK_PROCEDURE: return "Procedure";
    case SymbolKind::SK_PROGRAM_NAME: return "ProgramName";
    default: return "UnknownKind";
    }
}

inline std::string entryTypeToString(EntryTypeCategory type) {
    switch (type) {
    case EntryTypeCategory::ET_NO_TYPE: return "NoType";
    case EntryTypeCategory::ET_INTEGER: return "Integer";
    case EntryTypeCategory::ET_REAL: return "Real";
    case EntryTypeCategory::ET_BOOLEAN: return "Boolean";
    case EntryTypeCategory::ET_ARRAY: return "Array";
    case EntryTypeCategory::ET_UNKNOWN: return "UnknownType";
    default: return "OtherType";
    }
}


/*
* SYMBOL TABLE IMPLEMENTATIONS
*/
SymbolEntry::SymbolEntry()
    : kind(SymbolKind::SK_UNKNOWN), type(EntryTypeCategory::ET_UNKNOWN),
    offset(0), functionReturnType(EntryTypeCategory::ET_NO_TYPE), numParameters(0),
    declLine(0), declColumn(0) {
    arrayDetails.isInitialized = false;
}

SymbolEntry::SymbolEntry(std::string id_name, SymbolKind k, EntryTypeCategory et, int line, int col)
    : name(std::move(id_name)), kind(k), type(et),
    offset(0), functionReturnType(EntryTypeCategory::ET_NO_TYPE),
    numParameters(0), declLine(line), declColumn(col) {
    arrayDetails.isInitialized = false;
}

SymbolEntry::SymbolEntry(std::string id_name, SymbolKind k, EntryTypeCategory el_type, int low, int high, int line, int col)
    : name(std::move(id_name)), kind(k), type(EntryTypeCategory::ET_ARRAY),
    offset(0), functionReturnType(EntryTypeCategory::ET_NO_TYPE),
    numParameters(0), declLine(line), declColumn(col) {
    arrayDetails.elementType = el_type;
    arrayDetails.lowBound = low;
    arrayDetails.highBound = high;
    arrayDetails.isInitialized = true;
}

SymbolEntry::SymbolEntry(std::string func_name, EntryTypeCategory ret_type,
    const std::vector<std::pair<EntryTypeCategory, ArrayDetails>>& signature,
    int line, int col)
    : name(std::move(func_name)), kind(SymbolKind::SK_FUNCTION), type(EntryTypeCategory::ET_NO_TYPE),
    offset(0), functionReturnType(ret_type),
    formalParameterSignature(signature), numParameters(signature.size()),
    declLine(line), declColumn(col) {
    arrayDetails.isInitialized = false;
}

SymbolEntry::SymbolEntry(std::string proc_name,
    const std::vector<std::pair<EntryTypeCategory, ArrayDetails>>& signature,
    int line, int col)
    : name(std::move(proc_name)), kind(SymbolKind::SK_PROCEDURE), type(EntryTypeCategory::ET_NO_TYPE),
    offset(0), functionReturnType(EntryTypeCategory::ET_NO_TYPE),
    formalParameterSignature(signature), numParameters(signature.size()),
    declLine(line), declColumn(col) {
    arrayDetails.isInitialized = false;
}

std::string SymbolEntry::toString() const {
    std::stringstream ss;
    ss << "'" << name << "' (" << symbolKindToString(kind) << ")";
    if (kind == SymbolKind::SK_VARIABLE || kind == SymbolKind::SK_PARAMETER) {
        ss << " Type: " << entryTypeToString(type);
        if (type == EntryTypeCategory::ET_ARRAY && arrayDetails.isInitialized) {
            ss << " [Array Of " << entryTypeToString(arrayDetails.elementType)
                << ", " << arrayDetails.lowBound << ".." << arrayDetails.highBound << "]";
        }
    }
    else if (kind == SymbolKind::SK_FUNCTION || kind == SymbolKind::SK_PROCEDURE) {
        if (kind == SymbolKind::SK_FUNCTION) ss << " Returns: " << entryTypeToString(functionReturnType);
        ss << ", Params: (" << numParameters << " total)";
    }
    ss << " Declared at: (L:" << declLine << ", C:" << declColumn << ")";
    return ss.str();
}

SymbolTable::SymbolTable() : currentLevel(-1) {
    enterScope(); // Create the global scope
}

void SymbolTable::enterScope() {
    scopeStack.emplace_back();
    currentLevel++;
}

void SymbolTable::exitScope() {
    if (!scopeStack.empty()) {
        scopeStack.pop_back();
        currentLevel--;
    }
}

bool SymbolTable::isGlobalScope() const { return currentLevel == 0; }
int SymbolTable::getCurrentLevel() const { return currentLevel; }

bool SymbolTable::addSymbol(const SymbolEntry& entry) {
    if (scopeStack.empty()) return false;
    Scope& currentScope = scopeStack.back();
    if (currentScope.count(entry.name)) return false; // Already exists in this scope
    currentScope[entry.name] = entry;
    return true;
}

SymbolEntry* SymbolTable::lookupSymbol(const std::string& name) {
    if (scopeStack.empty()) return nullptr;
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
        auto& scope = *it;
        auto found = scope.find(name);
        if (found != scope.end()) {
            return &(found->second);
        }
    }
    return nullptr;
}

SymbolEntry* SymbolTable::lookupSymbolInCurrentScope(const std::string& name) {
    if (scopeStack.empty()) return nullptr;
    Scope& currentScope = scopeStack.back();
    auto found = currentScope.find(name);
    return (found != currentScope.end()) ? &(found->second) : nullptr;
}

void SymbolTable::printCurrentScope() const {
    if (scopeStack.empty()) {
        std::cout << "No active scope." << std::endl;
        return;
    }
    std::cout << "--- Scope Level " << currentLevel << " ---" << std::endl;
    const Scope& currentScope = scopeStack.back();
    for (const auto& pair : currentScope) {
        std::cout << "  " << pair.second.toString() << std::endl;
    }
}


/*
* AST NODE IMPLEMENTATIONS
*/

// Helper for indentation
static void print_indent(std::ostream& out, int indentLevel) {
    for (int i = 0; i < indentLevel; ++i) out << "  ";
}

// Base Node
Node::Node(int l, int c) : line(l), column(c), father(nullptr) {}

// Raw Lexer Types
Expr::Expr(int l, int c) : Node(l, c) {}
void Expr::print(std::ostream& out, int indentLevel) const { print_indent(out, indentLevel); out << "Raw Expr (Should not be in final AST)" << std::endl; }
void Expr::accept(SemanticVisitor& visitor) { /* Stub */ }
Ident::Ident(const std::string& n, int l, int c) : Node(l, c), name(n) {}
void Ident::print(std::ostream& out, int indentLevel) const { print_indent(out, indentLevel); out << "Raw Ident (Should not be in final AST)" << std::endl; }
void Ident::accept(SemanticVisitor& visitor) { /* Stub */ }
Num::Num(int val, int l, int c) : Expr(l, c), value(val) {}
void Num::print(std::ostream& out, int indentLevel) const { print_indent(out, indentLevel); out << "Raw Num (Should not be in final AST)" << std::endl; }
void Num::accept(SemanticVisitor& visitor) { /* Stub */ }
RealLit::RealLit(double val, int l, int c) : Expr(l, c), value(val) {}
void RealLit::print(std::ostream& out, int indentLevel) const { print_indent(out, indentLevel); out << "Raw RealLit (Should not be in final AST)" << std::endl; }
void RealLit::accept(SemanticVisitor& visitor) { /* Stub */ }

// Base AST Nodes
ExprNode::ExprNode(int l, int c) : Node(l, c), determinedType(EntryTypeCategory::ET_UNKNOWN) { determinedArrayDetails.isInitialized = false; }
void ExprNode::print(std::ostream& out, int indentLevel) const { print_indent(out, indentLevel); out << "ExprNode (Base)" << std::endl; }
StatementNode::StatementNode(int l, int c) : Node(l, c) {}
void StatementNode::print(std::ostream& out, int indentLevel) const { print_indent(out, indentLevel); out << "StatementNode (Base)" << std::endl; }
TypeNode::TypeNode(int l, int c) : Node(l, c) {}
void TypeNode::print(std::ostream& out, int indentLevel) const { print_indent(out, indentLevel); out << "TypeNode (Base)" << std::endl; }

// --- Accept Method Implementations ---
void ProgramNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void IdentifierList::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void IdentNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void Declarations::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void VarDecl::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void StandardTypeNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void ArrayTypeNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void SubprogramDeclarations::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void SubprogramDeclaration::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void FunctionHeadNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void ProcedureHeadNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void ArgumentsNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void ParameterList::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void ParameterDeclaration::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void CompoundStatementNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void StatementList::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void AssignStatementNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void IfStatementNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void WhileStatementNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void VariableNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void ProcedureCallStatementNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void ExpressionList::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void IntNumNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void RealNumNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void BooleanLiteralNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void StringLiteralNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void BinaryOpNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void UnaryOpNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void IdExprNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void FunctionCallExprNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }
void ReturnStatementNode::accept(SemanticVisitor& visitor) { visitor.visit(*this); }


// --- Constructor & Print Method Implementations ---

IdentNode::IdentNode(const std::string& n, int l, int c) : ExprNode(l, c), name(n) {}
void IdentNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IdentNode (Name: " << name << ", L:" << line << ", C:" << column << ")" << std::endl;
}

IntNumNode::IntNumNode(int val, int l, int c) : ExprNode(l, c), value(val) {}
void IntNumNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IntNumNode (Value: " << value << ")" << std::endl;
}

RealNumNode::RealNumNode(double val, int l, int c) : ExprNode(l, c), value(val) {}
void RealNumNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "RealNumNode (Value: " << value << ")" << std::endl;
}

BooleanLiteralNode::BooleanLiteralNode(bool val, int l, int c) : ExprNode(l, c), value(val) {}
void BooleanLiteralNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "BooleanLiteralNode (Value: " << (value ? "true" : "false") << ")" << std::endl;
}

StringLiteralNode::StringLiteralNode(const char* val, int l, int c) : ExprNode(l, c), value(val ? val : "") {}
void StringLiteralNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "StringLiteralNode (Value: \"" << value << "\")" << std::endl;
}

IdentifierList::IdentifierList(IdentNode* firstIdent, int l, int c) : Node(l, c) {
    if (firstIdent) {
        identifiers.push_back(firstIdent);
        firstIdent->father = this;
    }
}
void IdentifierList::addIdentifier(IdentNode* ident) {
    if (ident) {
        identifiers.push_back(ident);
        ident->father = this;
    }
}
void IdentifierList::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IdentifierList" << std::endl;
    for (IdentNode* id : identifiers) {
        if (id) id->print(out, indentLevel + 1);
    }
}

StandardTypeNode::StandardTypeNode(StandardTypeNode::TypeCategory cat, int l, int c) : TypeNode(l, c), category(cat) {}
void StandardTypeNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "StandardTypeNode (Type: ";
    switch (category) {
    case TYPE_INTEGER: out << "INTEGER"; break;
    case TYPE_REAL:    out << "REAL";    break;
    case TYPE_BOOLEAN: out << "BOOLEAN"; break;
    }
    out << ")" << std::endl;
}

ArrayTypeNode::ArrayTypeNode(IntNumNode* start, IntNumNode* end, StandardTypeNode* elemType, int l, int c)
    : TypeNode(l, c), startIndex(start), endIndex(end), elementType(elemType) {
    if (startIndex) startIndex->father = this;
    if (endIndex) endIndex->father = this;
    if (elementType) elementType->father = this;
}
void ArrayTypeNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ArrayTypeNode" << std::endl;
    startIndex->print(out, indentLevel + 1);
    endIndex->print(out, indentLevel + 1);
    elementType->print(out, indentLevel + 1);
}

VarDecl::VarDecl(IdentifierList* ids, TypeNode* t, int l, int c) : StatementNode(l, c), identifiers(ids), type(t) {
    if (identifiers) identifiers->father = this;
    if (type) type->father = this;
}
void VarDecl::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "VarDecl" << std::endl;
    identifiers->print(out, indentLevel + 1);
    type->print(out, indentLevel + 1);
}

Declarations::Declarations(int l, int c) : Node(l, c) {}
void Declarations::addVarDecl(VarDecl* vd) {
    if (vd) {
        var_decl_items.push_back(vd);
        vd->father = this;
    }
}
bool Declarations::isEmpty() const { return var_decl_items.empty(); }
void Declarations::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "Declarations" << std::endl;
    for (VarDecl* vd : var_decl_items) {
        vd->print(out, indentLevel + 1);
    }
}

ExpressionList::ExpressionList(int l, int c) : Node(l, c) {}
ExpressionList::ExpressionList(ExprNode* firstExpr, int l, int c) : Node(l, c) {
    addExpression(firstExpr);
}
void ExpressionList::addExpression(ExprNode* expr) {
    if (expr) {
        expressions.push_back(expr);
        expr->father = this;
    }
}
void ExpressionList::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ExpressionList" << std::endl;
    for (ExprNode* expr : expressions) {
        expr->print(out, indentLevel + 1);
    }
}

ParameterDeclaration::ParameterDeclaration(IdentifierList* idList, TypeNode* t, int l, int c) : Node(l, c), ids(idList), type(t) {
    if (ids) ids->father = this;
    if (type) type->father = this;
}
void ParameterDeclaration::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ParameterDeclaration" << std::endl;
    ids->print(out, indentLevel + 1);
    type->print(out, indentLevel + 1);
}

ParameterList::ParameterList(ParameterDeclaration* firstParamDecl, int l, int c) : Node(l, c) {
    addParameterDeclarationGroup(firstParamDecl);
}
void ParameterList::addParameterDeclarationGroup(ParameterDeclaration* paramDecl) {
    if (paramDecl) {
        paramDeclarations.push_back(paramDecl);
        paramDecl->father = this;
    }
}
void ParameterList::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ParameterList" << std::endl;
    for (ParameterDeclaration* pd : paramDeclarations) {
        pd->print(out, indentLevel + 1);
    }
}

ArgumentsNode::ArgumentsNode(int l, int c) : Node(l, c), params(nullptr) {}
ArgumentsNode::ArgumentsNode(ParameterList* pList, int l, int c) : Node(l, c), params(pList) {
    if (params) params->father = this;
}
void ArgumentsNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ArgumentsNode" << std::endl;
    if (params) params->print(out, indentLevel + 1);
}

SubprogramHead::SubprogramHead(IdentNode* n, ArgumentsNode* args, int l, int c) : Node(l, c), name(n), arguments(args) {
    if (name) name->father = this;
    if (arguments) arguments->father = this;
}
void SubprogramHead::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "SubprogramHead (Base)" << std::endl;
}

FunctionHeadNode::FunctionHeadNode(IdentNode* n, ArgumentsNode* args, StandardTypeNode* retType, int l, int c)
    : SubprogramHead(n, args, l, c), returnType(retType) {
    if (returnType) returnType->father = this;
}
void FunctionHeadNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "FunctionHeadNode" << std::endl;
    name->print(out, indentLevel + 1);
    arguments->print(out, indentLevel + 1);
    returnType->print(out, indentLevel + 1);
}

ProcedureHeadNode::ProcedureHeadNode(IdentNode* n, ArgumentsNode* args, int l, int c) : SubprogramHead(n, args, l, c) {}
void ProcedureHeadNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ProcedureHeadNode" << std::endl;
    name->print(out, indentLevel + 1);
    arguments->print(out, indentLevel + 1);
}

StatementList::StatementList(int l, int c) : Node(l, c) {}
StatementList::StatementList(StatementNode* firstStmt, int l, int c) : Node(l, c) {
    addStatement(firstStmt);
}
void StatementList::addStatement(StatementNode* stmt) {
    if (stmt) {
        statements.push_back(stmt);
        stmt->father = this;
    }
}
void StatementList::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "StatementList" << std::endl;
    for (StatementNode* stmt : statements) {
        stmt->print(out, indentLevel + 1);
    }
}

CompoundStatementNode::CompoundStatementNode(StatementList* sList, int l, int c) : StatementNode(l, c), stmts(sList) {
    if (stmts) stmts->father = this;
}
void CompoundStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "CompoundStatementNode" << std::endl;
    if (stmts) stmts->print(out, indentLevel + 1);
}

SubprogramDeclaration::SubprogramDeclaration(SubprogramHead* h, Declarations* local_decls, CompoundStatementNode* b, int l, int c)
    : Node(l, c), head(h), local_declarations(local_decls), body(b) {
    if (head) head->father = this;
    if (local_declarations) local_declarations->father = this;
    if (body) body->father = this;
}
void SubprogramDeclaration::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "SubprogramDeclaration" << std::endl;
    head->print(out, indentLevel + 1);
    local_declarations->print(out, indentLevel + 1);
    body->print(out, indentLevel + 1);
}

SubprogramDeclarations::SubprogramDeclarations(int l, int c) : Node(l, c) {}
void SubprogramDeclarations::addSubprogramDeclaration(SubprogramDeclaration* subprog) {
    if (subprog) {
        subprograms.push_back(subprog);
        subprog->father = this;
    }
}
void SubprogramDeclarations::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "SubprogramDeclarations" << std::endl;
    for (SubprogramDeclaration* sd : subprograms) {
        sd->print(out, indentLevel + 1);
    }
}

VariableNode::VariableNode(IdentNode* id, ExprNode* idx, int l, int c)
    : ExprNode(l, c), identifier(id), index(idx), offset(0), kind(SymbolKind::SK_UNKNOWN), scope(SymbolScope::GLOBAL) {
    if (identifier) identifier->father = this;
    if (index) index->father = this;
}
void VariableNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "VariableNode" << std::endl;
    identifier->print(out, indentLevel + 1);
    if (index) index->print(out, indentLevel + 1);
}

AssignStatementNode::AssignStatementNode(VariableNode* var, ExprNode* expr, int l, int c)
    : StatementNode(l, c), variable(var), expression(expr) {
    if (variable) variable->father = this;
    if (expression) expression->father = this;
}
void AssignStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "AssignStatementNode" << std::endl;
    variable->print(out, indentLevel + 1);
    expression->print(out, indentLevel + 1);
}

IfStatementNode::IfStatementNode(ExprNode* cond, StatementNode* thenStmt, StatementNode* elseStmt, int l, int c)
    : StatementNode(l, c), condition(cond), thenStatement(thenStmt), elseStatement(elseStmt) {
    if (condition) condition->father = this;
    if (thenStatement) thenStatement->father = this;
    if (elseStatement) elseStatement->father = this;
}
void IfStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IfStatementNode" << std::endl;
    condition->print(out, indentLevel + 1);
    thenStatement->print(out, indentLevel + 1);
    if (elseStatement) elseStatement->print(out, indentLevel + 1);
}

WhileStatementNode::WhileStatementNode(ExprNode* cond, StatementNode* b, int l, int c)
    : StatementNode(l, c), condition(cond), body(b) {
    if (condition) condition->father = this;
    if (body) body->father = this;
}
void WhileStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "WhileStatementNode" << std::endl;
    condition->print(out, indentLevel + 1);
    body->print(out, indentLevel + 1);
}

ProcedureCallStatementNode::ProcedureCallStatementNode(IdentNode* name, ExpressionList* args, int l, int c)
    : StatementNode(l, c), procName(name), arguments(args) {
    if (procName) procName->father = this;
    if (arguments) arguments->father = this;
}
void ProcedureCallStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ProcedureCallStatementNode" << std::endl;
    procName->print(out, indentLevel + 1);
    if (arguments) arguments->print(out, indentLevel + 1);
}

IdExprNode::IdExprNode(IdentNode* id, int l, int c)
    : ExprNode(l, c), ident(id), offset(0), kind(SymbolKind::SK_UNKNOWN), scope(SymbolScope::GLOBAL) {
    if (ident) ident->father = this;
}
void IdExprNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IdExprNode" << std::endl;
    ident->print(out, indentLevel + 1);
}

FunctionCallExprNode::FunctionCallExprNode(IdentNode* name, ExpressionList* args, int l, int c)
    : ExprNode(l, c), funcName(name), arguments(args) {
    if (funcName) funcName->father = this;
    if (arguments) arguments->father = this;
}
void FunctionCallExprNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "FunctionCallExprNode" << std::endl;
    funcName->print(out, indentLevel + 1);
    if (arguments) arguments->print(out, indentLevel + 1);
}

BinaryOpNode::BinaryOpNode(ExprNode* l, const std::string& op_str, ExprNode* r, int ln, int col)
    : ExprNode(ln, col), left(l), op(op_str), right(r) {
    if (left) left->father = this;
    if (right) right->father = this;
}
void BinaryOpNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "BinaryOpNode (Operator: " << op << ")" << std::endl;
    left->print(out, indentLevel + 1);
    right->print(out, indentLevel + 1);
}

UnaryOpNode::UnaryOpNode(const std::string& oper, ExprNode* expr, int l, int c)
    : ExprNode(l, c), op(oper), expression(expr) {
    if (expression) expression->father = this;
}
void UnaryOpNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "UnaryOpNode (Operator: " << op << ")" << std::endl;
    expression->print(out, indentLevel + 1);
}

ReturnStatementNode::ReturnStatementNode(ExprNode* retVal, int l, int c) : StatementNode(l, c), returnValue(retVal) {
    if (returnValue) returnValue->father = this;
}
void ReturnStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ReturnStatementNode" << std::endl;
    if (returnValue) returnValue->print(out, indentLevel + 1);
}

ProgramNode::ProgramNode(IdentNode* name, Declarations* d, SubprogramDeclarations* s, CompoundStatementNode* cStmt, int l, int c)
    : Node(l, c), progName(name), decls(d), subprogs(s), mainCompoundStmt(cStmt) {
    if (progName) progName->father = this;
    if (decls) decls->father = this;
    if (subprogs) subprogs->father = this;
    if (mainCompoundStmt) mainCompoundStmt->father = this;
}
void ProgramNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ProgramNode" << std::endl;
    progName->print(out, indentLevel + 1);
    decls->print(out, indentLevel + 1);
    subprogs->print(out, indentLevel + 1);
    mainCompoundStmt->print(out, indentLevel + 1);
}
