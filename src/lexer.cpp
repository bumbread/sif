#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <stdarg.h>

#include "basic.h"
#include "lexer.h"

static char *token_string_map[TK_COUNT];
static char *token_name_map[TK_COUNT];

void init_lexer_globals() {
    token_string_map[TK_INVALID]               = "INVALID";
    token_string_map[TK_IDENTIFIER]            = "IDENTIFIER";
    token_string_map[TK_NUMBER]                = "NUMBER";
    token_string_map[TK_STRING]                = "STRING";

    token_string_map[TK_VAR]                   = "var";
    token_string_map[TK_CONST]                 = "const";
    token_string_map[TK_PROC]                  = "proc";
    token_string_map[TK_OPERATOR]              = "operator";
    token_string_map[TK_STRUCT]                = "struct";
    token_string_map[TK_ENUM]                  = "enum";
    token_string_map[TK_RETURN]                = "return";
    token_string_map[TK_NULL]                  = "null";
    token_string_map[TK_TRUE]                  = "true";
    token_string_map[TK_FALSE]                 = "false";
    token_string_map[TK_SIZEOF]                = "sizeof";
    token_string_map[TK_TYPEOF]                = "typeof";
    token_string_map[TK_IF]                    = "if";
    token_string_map[TK_ELSE]                  = "else";
    token_string_map[TK_FOR]                   = "for";
    token_string_map[TK_WHILE]                 = "while";
    token_string_map[TK_BREAK]                 = "break";
    token_string_map[TK_CONTINUE]              = "continue";
    token_string_map[TK_CAST]                  = "cast";

    token_string_map[TK_DIRECTIVE_PRINT]       = "#print";
    token_string_map[TK_DIRECTIVE_ASSERT]      = "#assert";
    token_string_map[TK_DIRECTIVE_FOREIGN]     = "#foreign";
    token_string_map[TK_DIRECTIVE_C_CODE]      = "#c_code";
    token_string_map[TK_DIRECTIVE_INCLUDE]     = "#include";

    token_string_map[TK_ASSIGN]                = "=";
    token_string_map[TK_PLUS]                  = "+";
    token_string_map[TK_PLUS_ASSIGN]           = "+=";
    token_string_map[TK_MINUS]                 = "-";
    token_string_map[TK_MINUS_ASSIGN]          = "-=";
    token_string_map[TK_MULTIPLY]              = "*";
    token_string_map[TK_MULTIPLY_ASSIGN]       = "*=";
    token_string_map[TK_DIVIDE]                = "/";
    token_string_map[TK_DIVIDE_ASSIGN]         = "/=";
    token_string_map[TK_LEFT_SHIFT]            = "<<";
    token_string_map[TK_LEFT_SHIFT_ASSIGN]     = "<<=";
    token_string_map[TK_RIGHT_SHIFT]           = ">>";
    token_string_map[TK_RIGHT_SHIFT_ASSIGN]    = ">>=";
    token_string_map[TK_AMPERSAND]             = "&";
    token_string_map[TK_BIT_AND_ASSIGN]        = "&=";
    token_string_map[TK_BIT_OR]                = "|";
    token_string_map[TK_BIT_OR_ASSIGN]         = "|=";

    token_string_map[TK_DOLLAR]                = "$";

    token_string_map[TK_NOT]                   = "!";
    token_string_map[TK_NOT_EQUAL_TO]          = "!=";
    token_string_map[TK_LESS_THAN]             = "<";
    token_string_map[TK_LESS_THAN_OR_EQUAL]    = "<=";
    token_string_map[TK_GREATER_THAN]          = ">";
    token_string_map[TK_GREATER_THAN_OR_EQUAL] = ">=";
    token_string_map[TK_EQUAL_TO]              = "==";
    token_string_map[TK_BOOLEAN_AND]           = "&&";
    token_string_map[TK_BOOLEAN_AND_ASSIGN]    = "&&=";
    token_string_map[TK_BOOLEAN_OR]            = "||";
    token_string_map[TK_BOOLEAN_OR_ASSIGN]     = "||=";

    token_string_map[TK_LEFT_CURLY]            = "{";
    token_string_map[TK_RIGHT_CURLY]           = "}";
    token_string_map[TK_LEFT_SQUARE]           = "[";
    token_string_map[TK_RIGHT_SQUARE]          = "]";
    token_string_map[TK_LEFT_PAREN]            = "(";
    token_string_map[TK_RIGHT_PAREN]           = ")";
    token_string_map[TK_SEMICOLON]             = ";";
    token_string_map[TK_COLON]                 = ":";
    token_string_map[TK_DOT]                   = ".";
    token_string_map[TK_DOT_DOT]               = "..";
    token_string_map[TK_COMMA]                 = ",";
    token_string_map[TK_CARET]                 = "^";

    token_string_map[TK_COMMENT]               = "COMMENT";

    token_name_map[TK_INVALID]               = "TK_INVALID";
    token_name_map[TK_IDENTIFIER]            = "TK_IDENTIFIER";
    token_name_map[TK_NUMBER]                = "TK_NUMBER";
    token_name_map[TK_STRING]                = "TK_STRING";

    token_name_map[TK_VAR]                   = "TK_VAR";
    token_name_map[TK_CONST]                 = "TK_CONST";
    token_name_map[TK_PROC]                  = "TK_PROC";
    token_name_map[TK_OPERATOR]              = "TK_OPERATOR";
    token_name_map[TK_STRUCT]                = "TK_STRUCT";
    token_name_map[TK_ENUM]                  = "TK_ENUM";
    token_name_map[TK_RETURN]                = "TK_RETURN";
    token_name_map[TK_NULL]                  = "TK_NULL";
    token_name_map[TK_TRUE]                  = "TK_TRUE";
    token_name_map[TK_FALSE]                 = "TK_FALSE";
    token_name_map[TK_SIZEOF]                = "TK_SIZEOF";
    token_name_map[TK_TYPEOF]                = "TK_TYPEOF";
    token_name_map[TK_IF]                    = "TK_IF";
    token_name_map[TK_ELSE]                  = "TK_ELSE";
    token_name_map[TK_FOR]                   = "TK_FOR";
    token_name_map[TK_WHILE]                 = "TK_WHILE";
    token_name_map[TK_BREAK]                 = "TK_BREAK";
    token_name_map[TK_CONTINUE]              = "TK_CONTINUE";
    token_name_map[TK_CAST]                  = "TK_CAST";

    token_name_map[TK_DIRECTIVE_PRINT]       = "TK_DIRECTIVE_PRINT";
    token_name_map[TK_DIRECTIVE_ASSERT]      = "TK_DIRECTIVE_ASSERT";
    token_name_map[TK_DIRECTIVE_FOREIGN]     = "TK_DIRECTIVE_FOREIGN";
    token_name_map[TK_DIRECTIVE_C_CODE]      = "TK_DIRECTIVE_C_CODE";
    token_name_map[TK_DIRECTIVE_INCLUDE]     = "TK_DIRECTIVE_INCLUDE";

    token_name_map[TK_ASSIGN]                = "TK_ASSIGN";
    token_name_map[TK_PLUS]                  = "TK_PLUS";
    token_name_map[TK_PLUS_ASSIGN]           = "TK_PLUS_ASSIGN";
    token_name_map[TK_MINUS]                 = "TK_MINUS";
    token_name_map[TK_MINUS_ASSIGN]          = "TK_MINUS_ASSIGN";
    token_name_map[TK_MULTIPLY]              = "TK_MULTIPLY";
    token_name_map[TK_MULTIPLY_ASSIGN]       = "TK_MULTIPLY_ASSIGN";
    token_name_map[TK_DIVIDE]                = "TK_DIVIDE";
    token_name_map[TK_DIVIDE_ASSIGN]         = "TK_DIVIDE_ASSIGN";
    token_name_map[TK_LEFT_SHIFT]            = "TK_LEFT_SHIFT";
    token_name_map[TK_LEFT_SHIFT_ASSIGN]     = "TK_LEFT_SHIFT_ASSIGN";
    token_name_map[TK_RIGHT_SHIFT]           = "TK_RIGHT_SHIFT";
    token_name_map[TK_RIGHT_SHIFT_ASSIGN]    = "TK_RIGHT_SHIFT_ASSIGN";
    token_name_map[TK_AMPERSAND]             = "TK_AMPERSAND";
    token_name_map[TK_BIT_AND_ASSIGN]        = "TK_BIT_AND_ASSIGN";
    token_name_map[TK_BIT_OR]                = "TK_BIT_OR";
    token_name_map[TK_BIT_OR_ASSIGN]         = "TK_BIT_OR_ASSIGN";

    token_name_map[TK_DOLLAR]                = "TK_DOLLAR";

    token_name_map[TK_NOT]                   = "TK_NOT";
    token_name_map[TK_NOT_EQUAL_TO]          = "TK_NOT_EQUAL_TO";
    token_name_map[TK_LESS_THAN]             = "TK_LESS_THAN";
    token_name_map[TK_LESS_THAN_OR_EQUAL]    = "TK_LESS_THAN_OR_EQUAL";
    token_name_map[TK_GREATER_THAN]          = "TK_GREATER_THAN";
    token_name_map[TK_GREATER_THAN_OR_EQUAL] = "TK_GREATER_THAN_OR_EQUAL";
    token_name_map[TK_EQUAL_TO]              = "TK_EQUAL_TO";
    token_name_map[TK_BOOLEAN_AND]           = "TK_BOOLEAN_AND";
    token_name_map[TK_BOOLEAN_AND_ASSIGN]    = "TK_BOOLEAN_AND_ASSIGN";
    token_name_map[TK_BOOLEAN_OR]            = "TK_BOOLEAN_OR";
    token_name_map[TK_BOOLEAN_OR_ASSIGN]     = "TK_BOOLEAN_OR_ASSIGN";

    token_name_map[TK_LEFT_CURLY]            = "TK_LEFT_CURLY";
    token_name_map[TK_RIGHT_CURLY]           = "TK_RIGHT_CURLY";
    token_name_map[TK_LEFT_SQUARE]           = "TK_LEFT_SQUARE";
    token_name_map[TK_RIGHT_SQUARE]          = "TK_RIGHT_SQUARE";
    token_name_map[TK_LEFT_PAREN]            = "TK_LEFT_PAREN";
    token_name_map[TK_RIGHT_PAREN]           = "TK_RIGHT_PAREN";
    token_name_map[TK_SEMICOLON]             = "TK_SEMICOLON";
    token_name_map[TK_COLON]                 = "TK_COLON";
    token_name_map[TK_DOT]                   = "TK_DOT";
    token_name_map[TK_DOT_DOT]               = "TK_DOT_DOT";
    token_name_map[TK_COMMA]                 = "TK_COMMA";
    token_name_map[TK_CARET]                 = "TK_CARET";

    token_name_map[TK_COMMENT]               = "TK_COMMENT";

    for (int i = 0; i < TK_COUNT; i++) {
        if (token_string_map[i] == nullptr) {
            printf("No token string for token: %d\n", i);
            assert(false);
        }
        if (token_name_map[i] == nullptr) {
            printf("No token name for token: %d\n", i);
            assert(false);
        }
    }
}

void print_token(Token token) {
    printf("%s %s\n", token_string_map[token.kind], token.text);
}



void advance(Lexer *lexer, int count) {
    lexer->location.index += count;
    lexer->location.character += count;
}

bool is_letter_or_underscore(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool is_whitespace(char c) {
    return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r') || (c == '\v'); // todo(josh): what other whitespace chars are there?
}

bool is_digit(char c) {
    return (c >= '0') && (c <= '9');
}

bool is_one_to_nine(char c) {
    return (c >= '1') && (c <= '9');
}

char *clone_string(char *string, int length) {
    char *new_string = (char *)malloc(length+1);
    memcpy((void *)new_string, (void *)string, length);
    new_string[length] = 0;
    return new_string;
}

char *scan_identifier(char *text, int *out_length) {
    char *start = text;
    assert(is_letter_or_underscore(*start));
    while (is_letter_or_underscore(*text) || is_digit(*text)) {
        text += 1;
    }
    int length = text - start;
    *out_length = length;
    char *result = clone_string(start, length);
    return result;
}

char *unescape_string(char *str, int *out_escaped_length) {
    // trim out quotes
    assert(str[0] != '"');
    int string_length = strlen(str);
    assert(str[string_length-1] != '"');
    int escaped_length = string_length;

    bool escape = false;
    String_Builder sb = make_string_builder(default_allocator(), 1024);
    for (char *c = str; (*c != '\0' || escape); c++) {
        if (!escape) {
            if (*c == '\\') {
                escape = true;
                escaped_length -= 1;
            }
            else if (*c == '\r') {
                // skip it
            }
            else {
                sb.printf("%c", *c);
            }
        }
        else {
            escape = false;
            switch (*c) {
                case '"':  sb.printf("%c", '\"'); break;
                case '\\': sb.printf("%c", '\\'); break;
                case 'b':  sb.printf("%c", '\\b');  break;
                case 'f':  sb.printf("%c", '\\f');  break;
                case 'n':  sb.printf("%c", '\\n');  break;
                case 'r':  sb.printf("%c", '\\r');  break;
                case 't':  sb.printf("%c", '\\t');  break;
                // case 'u':  fmt.sbprint(&sb, '\u'); // todo(josh): unicode
                default: {
                    printf("Unexpected escape character: %c\n", *c);
                    assert(false);
                }
            }
        }
    }
    assert(escape == false && "end of string from within escape sequence");
    *out_escaped_length = escaped_length;
    return sb.string();
}

// note(josh): returns the string without quotes, out_length includes the quotes though since the lexer needs to know how much to advance by
char *scan_string(char delim, char *text, int *out_scanner_length, int *out_escaped_length, char **escaped_string, int *out_newlines_in_string) {
    assert(*text == delim);
    text += 1;
    char *start = text;
    bool escaped = false;
    int newlines = 0;
    while ((*text != 0) && (*text != delim || escaped)) {
        escaped = false;
        if (*text == '\n') {
            newlines += 1;
        }
        if (*text == '\\') {
            escaped = true;
        }
        text += 1;
    }
    assert(*text == delim); // todo(josh): check for EOF
    int length = (text - start);
    text += 1;
    *out_scanner_length = length + 2; // note(josh): +2 for the quotation marks
    *out_newlines_in_string = newlines;
    char *duplicate = clone_string(start, length);
    *escaped_string = unescape_string(duplicate, out_escaped_length);
    return duplicate;
}

char *scan_number(char *text, int *out_length, bool *out_has_a_dot) {
    char *start = text;

    if (*text == '0') {
        text += 1;
    }
    else if (is_one_to_nine(*text)) {
        while (is_digit(*text)) {
            text += 1;
        }
    }
    else {
        assert(false && "scan_number called without being at a number");
    }

    if (*text == '.') {
        text += 1;
        *out_has_a_dot = true;
        while (is_digit(*text)) {
            text += 1;
        }
    }

    int length = text - start;
    *out_length = length;
    char *result = clone_string(start, length);
    return result;
}

bool get_next_token(Lexer *lexer, Token *out_token) {
    if (lexer->errored) {
        return false;
    }

    *out_token = {};

    while (is_whitespace(lexer->text[lexer->location.index])) {
        if (lexer->text[lexer->location.index] == '\n') {
            lexer->location.line += 1;
            lexer->location.character = 0; // advance() directly below will make this 1
        }
        advance(lexer, 1);
    }

    if (lexer->text[lexer->location.index] == 0) {
        return false;
    }

    #define SIMPLE_TOKEN(c, t) if (lexer->text[lexer->location.index] == c) { \
        advance(lexer, 1); \
        out_token->kind = t; \
        out_token->text = token_string_map[t]; \
    }

    Location token_location = lexer->location;
    token_location.text = &lexer->text[lexer->location.index];

    if (is_letter_or_underscore(lexer->text[lexer->location.index])) {
        int length = 0;
        char *identifier = scan_identifier(&lexer->text[lexer->location.index], &length);
        advance(lexer, length);
        out_token->kind = TK_IDENTIFIER;
        out_token->text = identifier;

        #define CHECK_KEYWORD(keyword, token) if (strcmp(identifier, keyword) == 0) { \
            out_token->kind = token; \
        }

        CHECK_KEYWORD("return", TK_RETURN)
        else CHECK_KEYWORD("var", TK_VAR)
        else CHECK_KEYWORD("const", TK_CONST)
        else CHECK_KEYWORD("proc", TK_PROC)
        else CHECK_KEYWORD("operator", TK_OPERATOR)
        else CHECK_KEYWORD("struct", TK_STRUCT)
        else CHECK_KEYWORD("enum", TK_ENUM)
        else CHECK_KEYWORD("null", TK_NULL)
        else CHECK_KEYWORD("true", TK_TRUE)
        else CHECK_KEYWORD("false", TK_FALSE)
        else CHECK_KEYWORD("sizeof", TK_SIZEOF)
        else CHECK_KEYWORD("typeof", TK_TYPEOF)
        else CHECK_KEYWORD("if", TK_IF)
        else CHECK_KEYWORD("else", TK_ELSE)
        else CHECK_KEYWORD("for", TK_FOR)
        else CHECK_KEYWORD("while", TK_WHILE)
        else CHECK_KEYWORD("break", TK_BREAK)
        else CHECK_KEYWORD("continue", TK_CONTINUE)
        else CHECK_KEYWORD("cast", TK_CAST)
    }
    else if (is_digit(lexer->text[lexer->location.index])) {
        int length = 0;
        bool has_a_dot = false;
        char *number = scan_number(&lexer->text[lexer->location.index], &length, &has_a_dot);
        advance(lexer, length);
        out_token->kind = TK_NUMBER;
        out_token->text = number;
        out_token->has_a_dot = has_a_dot;
    }
    else if (lexer->text[lexer->location.index] == '`') {
        int scanner_length = 0;
        int escaped_length = 0;
        char *escaped_string = nullptr;
        int newlines = 0;
        char *string = scan_string('`', &lexer->text[lexer->location.index], &scanner_length, &escaped_length, &escaped_string, &newlines);
        lexer->location.line += newlines;
        advance(lexer, scanner_length);
        out_token->kind = TK_STRING;
        out_token->text = string;
        out_token->escaped_text = escaped_string;
        out_token->escaped_length = escaped_length;
        out_token->scanner_length = scanner_length-2; // note(josh): -2 for the quotes
    }
    else if (lexer->text[lexer->location.index] == '"') {
        int scanner_length = 0;
        int escaped_length = 0;
        char *escaped_string = nullptr;
        int newlines = 0;
        char *string = scan_string('"', &lexer->text[lexer->location.index], &scanner_length, &escaped_length, &escaped_string, &newlines);
        lexer->location.line += newlines;
        advance(lexer, scanner_length);
        out_token->kind = TK_STRING;
        out_token->text = string;
        out_token->escaped_text = escaped_string;
        out_token->escaped_length = escaped_length;
        out_token->scanner_length = scanner_length-2; // note(josh): -2 for the quotes
    }
    // todo(josh): make a macro for these operators
    else if (lexer->text[lexer->location.index] == '+') {
        advance(lexer, 1);
        out_token->kind = TK_PLUS;
        out_token->text = "+";
        if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_PLUS_ASSIGN;
            out_token->text = "+=";
        }
    }
    else if (lexer->text[lexer->location.index] == '-') {
        advance(lexer, 1);
        out_token->kind = TK_MINUS;
        out_token->text = "-";
        if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_MINUS_ASSIGN;
            out_token->text = "-=";
        }
    }
    else if (lexer->text[lexer->location.index] == '*') {
        advance(lexer, 1);
        out_token->kind = TK_MULTIPLY;
        out_token->text = "*";
        if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_MULTIPLY_ASSIGN;
            out_token->text = "*=";
        }
    }
    else if (lexer->text[lexer->location.index] == '/') {
        advance(lexer, 1);
        out_token->kind = TK_DIVIDE;
        out_token->text = "/";
        if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_DIVIDE_ASSIGN;
            out_token->text = "/=";
        }
        else if (lexer->text[lexer->location.index] == '/') {
            while ((lexer->text[lexer->location.index] != '\n') && (lexer->text[lexer->location.index] != 0)) {
                advance(lexer, 1);
            }
            return get_next_token(lexer, out_token);
        }
    }
    else if (lexer->text[lexer->location.index] == '=') {
        advance(lexer, 1);
        out_token->kind = TK_ASSIGN;
        out_token->text = "=";
        if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_EQUAL_TO;
            out_token->text = "==";
        }
    }
    else if (lexer->text[lexer->location.index] == '!') {
        advance(lexer, 1);
        out_token->kind = TK_NOT;
        out_token->text = "!";
        if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_NOT_EQUAL_TO;
            out_token->text = "!=";
        }
    }
    else if (lexer->text[lexer->location.index] == '<') {
        advance(lexer, 1);
        out_token->kind = TK_LESS_THAN;
        out_token->text = "<";
        if (lexer->text[lexer->location.index] == '<') {
            advance(lexer, 1);
            out_token->kind = TK_LEFT_SHIFT;
            out_token->text = "<<";
            if (lexer->text[lexer->location.index] == '=') {
                advance(lexer, 1);
                out_token->kind = TK_LEFT_SHIFT_ASSIGN;
                out_token->text = "<<=";
            }
        }
        else if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_LESS_THAN_OR_EQUAL;
            out_token->text = "<=";
        }
    }
    else if (lexer->text[lexer->location.index] == '>') {
        advance(lexer, 1);
        out_token->kind = TK_GREATER_THAN;
        out_token->text = ">";
        if (lexer->text[lexer->location.index] == '>') {
            advance(lexer, 1);
            out_token->kind = TK_RIGHT_SHIFT;
            out_token->text = ">>";
            if (lexer->text[lexer->location.index] == '=') {
                advance(lexer, 1);
                out_token->kind = TK_RIGHT_SHIFT_ASSIGN;
                out_token->text = ">>=";
            }
        }
        else if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_GREATER_THAN_OR_EQUAL;
            out_token->text = ">=";
        }
    }
    else if (lexer->text[lexer->location.index] == '&') {
        advance(lexer, 1);
        out_token->kind = TK_AMPERSAND;
        out_token->text = "&";
        if (lexer->text[lexer->location.index] == '&') {
            advance(lexer, 1);
            out_token->kind = TK_BOOLEAN_AND;
            out_token->text = "&&";
            if (lexer->text[lexer->location.index] == '=') {
                advance(lexer, 1);
                out_token->kind = TK_BOOLEAN_AND_ASSIGN;
                out_token->text = "&&=";
            }
        }
        else if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_BIT_AND_ASSIGN;
            out_token->text = "&=";
        }
    }
    else if (lexer->text[lexer->location.index] == '|') {
        advance(lexer, 1);
        out_token->kind = TK_BIT_OR;
        out_token->text = "|";
        if (lexer->text[lexer->location.index] == '|') {
            advance(lexer, 1);
            out_token->kind = TK_BOOLEAN_OR;
            out_token->text = "||";
            if (lexer->text[lexer->location.index] == '=') {
                advance(lexer, 1);
                out_token->kind = TK_BOOLEAN_OR_ASSIGN;
                out_token->text = "||=";
            }
        }
        else if (lexer->text[lexer->location.index] == '=') {
            advance(lexer, 1);
            out_token->kind = TK_BIT_OR_ASSIGN;
            out_token->text = "|=";
        }
    }
    else if (lexer->text[lexer->location.index] == '#') {
        advance(lexer, 1);
        int length = 0;
        char *identifier = scan_identifier(&lexer->text[lexer->location.index], &length);
        advance(lexer, length);
        if (strcmp(identifier, "assert") == 0) {
            out_token->kind = TK_DIRECTIVE_ASSERT;
            out_token->text = "#assert";
        }
        else if (strcmp(identifier, "print") == 0) {
            out_token->kind = TK_DIRECTIVE_PRINT;
            out_token->text = "#print";
        }
        else if (strcmp(identifier, "foreign") == 0) {
            out_token->kind = TK_DIRECTIVE_FOREIGN;
            out_token->text = "#foreign";
        }
        else if (strcmp(identifier, "c_code") == 0) {
            out_token->kind = TK_DIRECTIVE_C_CODE;
            out_token->text = "#c_code";
        }
        else if (strcmp(identifier, "include") == 0) {
            out_token->kind = TK_DIRECTIVE_INCLUDE;
            out_token->text = "#include";
        }
        else {
            lexer->errored = true;
            report_error(token_location, "Unknown directive: %s", identifier);
            return false;
        }
    }
    else if (lexer->text[lexer->location.index] == '.') {
        advance(lexer, 1);
        out_token->kind = TK_DOT;
        out_token->text = ".";
        if (lexer->text[lexer->location.index] == '.') {
            advance(lexer, 1);
            out_token->kind = TK_DOT_DOT;
            out_token->text = "..";
        }
    }
    else SIMPLE_TOKEN('(', TK_LEFT_PAREN)
    else SIMPLE_TOKEN(')', TK_RIGHT_PAREN)
    else SIMPLE_TOKEN('{', TK_LEFT_CURLY)
    else SIMPLE_TOKEN('}', TK_RIGHT_CURLY)
    else SIMPLE_TOKEN('[', TK_LEFT_SQUARE)
    else SIMPLE_TOKEN(']', TK_RIGHT_SQUARE)
    else SIMPLE_TOKEN(';', TK_SEMICOLON)
    else SIMPLE_TOKEN(':', TK_COLON)
    else SIMPLE_TOKEN(',', TK_COMMA)
    else SIMPLE_TOKEN('^', TK_CARET)
    else SIMPLE_TOKEN('$', TK_DOLLAR)
    else {
        lexer->errored = true;
        report_error(token_location, "Unknown character: %c.", lexer->text[lexer->location.index]);
        return false;
    }

    out_token->location = token_location;
    return true;
}

bool peek_next_token(Lexer *lexer, Token *out_token) {
    Lexer copy = *lexer;
    bool ok = get_next_token(&copy, out_token);
    return ok;
}

void eat_next_token(Lexer *lexer, Token *out_token) {
    Token token;
    if (out_token == nullptr) {
        out_token = &token;
    }
    bool ok = get_next_token(lexer, out_token);
    assert(ok);
}

void report_error(Location location, const char *fmt, ...) {
    printf("%s(%d:%d) Error: ", location.filepath, location.line, location.character);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void report_info(Location location, const char *fmt, ...) {
    printf("%s(%d:%d)        ", location.filepath, location.line, location.character);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void unexpected_token(Lexer *lexer, Token token, Token_Kind expected) {
    if (expected != TK_INVALID) {
        report_error(token.location, "Unexpected token %s, expected %s.", token_string(token.kind), token_string(expected));
    }
    else {
        report_error(token.location, "Unexpected token %s.", token_string(token.kind));
    }
}

bool expect_token(Lexer *lexer, Token_Kind kind, Token *out_token) {
    Token backup;
    if (out_token == nullptr) {
        out_token = &backup;
    }
    bool ok = get_next_token(lexer, out_token);
    if (!ok || (out_token->kind != kind)) {
        unexpected_token(lexer, *out_token, kind);
        return false;
    }
    return true;
}

char *token_string(Token_Kind kind) {
    return token_string_map[kind];
}

char *token_name(Token_Kind kind) {
    return token_name_map[kind];
}