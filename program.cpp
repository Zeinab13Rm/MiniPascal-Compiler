#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem> // C++17 standard for filesystem operations

// Include all the necessary headers from your compiler project
#include "ast.h"
#include "parser.h" // Generated by Bison
#include "semantic_analyzer.h"
#include "codegenerator.h"

// External variables from Flex/Bison that connect the components
extern ProgramNode* root_ast_node; // The root of the AST, built by the parser
extern int yylex();                // The main lexer function
extern int yyparse();              // The main parser function
extern FILE* yyin;                 // The input file stream for the lexer
extern int lin;                    // The current line number, tracked by the lexer
extern int col;                    // The current column number, tracked by the lexer

// A global flag to halt compilation if any stage fails
bool compilation_has_error = false;

// Helper function to get the base name of a file path (e.g., "C:/path/to/Test01.pas" -> "Test01")
std::string get_base_filename(const std::string& path) {
    std::filesystem::path fs_path(path);
    return fs_path.stem().string();
}

// Main entry point for the compiler
int main(int argc, char* argv[]) {
    // 1. Check for the correct command-line argument
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file.pas>" << std::endl;
        return 1;
    }

    std::string input_filename(argv[1]);
    std::string base_name = get_base_filename(input_filename);
    std::string output_dir = "output";

    // --- Create the output directory if it doesn't exist ---
    try {
        if (!std::filesystem::exists(output_dir)) {
            std::filesystem::create_directory(output_dir);
            std::cout << "Created output directory: " << output_dir << std::endl;
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error creating output directory: " << e.what() << std::endl;
        return 1;
    }

    // --- Open the input file for the lexer ---
    yyin = fopen(input_filename.c_str(), "r");
    if (!yyin) {
        yyin = nullptr; // Ensure yyin is null on failure
    }
    if (!yyin) {
        std::cerr << "Error: Could not open input file " << input_filename << std::endl;
        return 1;
    }
    std::cout << "Compiling: " << input_filename << std::endl;

    // =============================================
    // PHASE 1 & 2: SYNTAX ANALYSIS (LEXER + PARSER)
    // =============================================
    std::cout << "\nPhase 1: Parsing (Lexical and Syntax Analysis)..." << std::endl;
    int parse_result = yyparse(); // This function calls yylex() internally

    if (parse_result != 0 || compilation_has_error || root_ast_node == nullptr) {
        std::cerr << "Compilation failed during parsing." << std::endl;
        fclose(yyin);
        return 1;
    }
    std::cout << "Parsing successful! Abstract Syntax Tree created." << std::endl;

    // Optional: Dump the AST to a file for debugging
    std::string ast_filepath = output_dir + "/" + base_name + ".ast.txt";
    std::ofstream ast_file(ast_filepath);
    if (ast_file.is_open()) {
        root_ast_node->print(ast_file);
        ast_file.close();
        std::cout << "AST dump written to " << ast_filepath << std::endl;
    }

    // =============================================
    // PHASE 3: SEMANTIC ANALYSIS
    // =============================================
    std::cout << "\nPhase 2: Semantic Analysis..." << std::endl;
    SemanticAnalyzer semanticAnalyzer;
    root_ast_node->accept(semanticAnalyzer);

    std::string semantics_filepath = output_dir + "/" + base_name + ".semantic_errors.log";
    std::ofstream semantics_file(semantics_filepath);

    if (semanticAnalyzer.hasErrors()) {
        std::cerr << "Compilation failed. Semantic errors found:" << std::endl;
        semanticAnalyzer.printErrors(std::cerr); // Print errors to console
        semanticAnalyzer.printErrors(semantics_file); // Also save to log file
        delete root_ast_node;
        fclose(yyin);
        semantics_file.close();
        return 1;
    }
    std::cout << "Semantic analysis successful! No errors found." << std::endl;
    semantics_file << "Semantic analysis successful. No errors found." << std::endl;
    semantics_file.close();

    // =============================================
    // PHASE 4: CODE GENERATION
    // =============================================
    std::cout << "\nPhase 3: Code Generation..." << std::endl;
    CodeGenerator codeGenerator;
    std::string assemblyCode;
    try {
        assemblyCode = codeGenerator.generateCode(*root_ast_node, semanticAnalyzer);
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Code generation failed with an error: " << e.what() << std::endl;
        delete root_ast_node;
        fclose(yyin);
        return 1;
    }

    std::string vm_filepath = output_dir + "/" + base_name + ".vm";
    std::ofstream vm_file(vm_filepath);
    vm_file << assemblyCode;
    vm_file.close();
    std::cout << "Code generation successful! Assembly written to " << vm_filepath << std::endl;
    std::cout << "\nCompilation finished successfully." << std::endl;


    // --- Cleanup ---
    delete root_ast_node;
    fclose(yyin);

    return 0;
}
