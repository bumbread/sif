#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "checker.h"
#include "c_backend.h"

/*
TODO:
-put struct constants in a different block than struct instance members
-error when instantiating a polymorphic struct without parameters
-implicit polymorphism
-figure out extra newlines in #c_code directive
-control flow graph analysis
-pass command line params to `run`
-function pointers
-varargs
-runtime bounds checks
-constant bounds checks i.e. var arr: [4]int; arr[232];
-right now operator overloading requires the first parameter to be the struct that you are overloading for. this is not ideal because you want to be able to do float * vector
-reference-to-reference parsing bug: var x: >>int lexes as a shift-right
-assigning to reference-to-reference doesn't work. I'm not sure what the behaviour should be
-enum field expressions
-foreach loops
-slicing
-transmute
-switch statements
-#caller_location
-unions
-tagged unions
-defer
-#if
-type_info
-figure out if I should allow shadowing (maybe with a keyword?)
*/

struct Timer {
    double frequency = {};
};

void init_timer(Timer *timer) {
    LARGE_INTEGER large_integer_frequency = {};
    assert(QueryPerformanceFrequency(&large_integer_frequency) != 0);
    timer->frequency = large_integer_frequency.QuadPart/1000.0;
}

double query_timer(Timer *timer) {
    LARGE_INTEGER large_integer_counter = {};
    assert(QueryPerformanceCounter(&large_integer_counter) != 0);
    return large_integer_counter.QuadPart / timer->frequency;
}

void main(int argc, char **argv) {
    Timer timer = {};
    init_timer(&timer);
    double application_start_time = query_timer(&timer);

    if (argc < 3) {
        printf("Usage:\n  sif <build|run> <file> [-show-timings]\n");
        return;
    }

    bool show_timings = false;

    for (int i = 2; i < argc; i++) {
        char *arg = argv[i];
        if (strcmp(arg, "-show-timings") == 0) {
            show_timings = true;
        }
    }

    bool is_run   = strcmp(argv[1], "run")   == 0;
    bool is_build = strcmp(argv[1], "build") == 0;

    init_lexer_globals();
    init_parser();
    init_checker();

    double parsing_start_time = query_timer(&timer);

    Ast_Block *global_scope = begin_parsing(argv[2]);
    if (!global_scope) {
        printf("There were errors.\n");
        return;
    }

    add_global_declarations(global_scope);

    double checking_start_time = query_timer(&timer);

    bool check_success = typecheck_global_scope(global_scope);
    if (!check_success) {
        printf("There were errors.\n");
        return;
    }

    double codegen_start_time = query_timer(&timer);

    String_Builder c_code = generate_c_main_file(global_scope);
    write_entire_file("output.c", c_code.string());

    double c_compile_start_time = query_timer(&timer);

    if (system("cmd.exe /c \"cl output.c /nologo\"") != 0) {
        printf("\nInternal compiler error: sif encountered an error when compiling C output. Exiting.\n");
        return;
    }

    double compilation_end_time = query_timer(&timer);

    if (show_timings) {
        printf("-----------------------------\n");
        printf("|    sif compile timings    |\n");
        printf("-----------------------------\n");
        printf("Setup     time: %fs\n", (parsing_start_time   - application_start_time));
        printf("Parse     time: %fs\n", (checking_start_time  - parsing_start_time));
        printf("Check     time: %fs\n", (codegen_start_time   - checking_start_time));
        printf("Codegen   time: %fs\n", (c_compile_start_time - codegen_start_time));
        printf("C compile time: %fs\n", (compilation_end_time - c_compile_start_time));
        printf("Total     time: %fs\n", (compilation_end_time - application_start_time));
    }

    if (is_run) {
        system("cmd.exe /c \"output.exe\"");
    }
    else {
        assert(is_build);
    }
}