#pragma once

#include <string>
#include <vector>
#include <stack>
using namespace std;

bool preprocess(string &expression, vector<char> &variables_names);
// xoa cac khoang trang va chuyen cac phep toan co nhieu cach viet ve 1 cach viet
// lay danh sach bien
// kiem tra ky tu ko cho phep

bool invalid_check(const string &expression);

void postfix_convert(const string &expression, string &postfix_expr);