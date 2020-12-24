#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>

#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "checker.h"
#include "c_backend.h"

/*
TODO:
-+=, -=, etc
-{} blocks at statement level
-#include
-enum field expressions
-foreach loops
-switch statements
-struct/array literals
-function pointers
-operator overloading
*/

void main(int argc, char **argv) {
    if (argc <= 1) {
        printf("Usage:\n  sif <file>\n");
        return;
    }

    char *root_file = argv[1];
    int len = 0;
    char *root_file_text = read_entire_file(root_file, &len);
    defer(free(root_file_text));

    init_lexer_globals();
    init_parser();
    init_checker();

    Lexer lexer(root_file, root_file_text);

    Ast_Block *global_scope = parse_block(&lexer);
    if (!global_scope) {
        printf("There were errors.\n");
        return;
    }

    add_global_declarations(global_scope);
    bool resolve_identifiers_success = resolve_identifiers();
    if (!resolve_identifiers_success) {
        printf("There were errors.\n");
        return;
    }

    bool check_success = typecheck_global_scope(global_scope);
    if (!check_success) {
        printf("There were errors.\n");
        return;
    }

    String_Builder c_code = generate_c_main_file(global_scope);
    write_entire_file("output.cpp", c_code.string());
}