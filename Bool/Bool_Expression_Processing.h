#pragma once

#include <string>
#include <vector>
#include <stack>
using namespace std;

bool preprocess(string &expression, vector<char> &variables_names);
// xóa các khoảng trắng và đồng nhất cách viết các toán tử
// lấy danh sách biến 
// kiểm tra ký tự cho phép

bool invalid_check(const string &expression);
// kiểm tra lỗi cú pháp

void postfix_convert(const string &expression, string &postfix_expr);
// chuyển sang biểu thức hậu tố