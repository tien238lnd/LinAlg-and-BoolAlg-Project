#pragma once

#include <stack>
#include "String_Process.h"

int calculate_infix(Token &lvalue, Lexer &lex);

Matrix evaluate_string_2_matrix(const string &str);