#include "Expression_processing.h"

bool preprocess(string &expression, vector<char> &variables_names)
{
	for (int i = 0; i < expression.length(); i++)	// để rảnh thử sửa lại for (char c : expression) xem sao
		switch (expression[i]) {
		case ' ':
			expression.erase(i, 1); i--; break;
		case '-': case '~':
			expression[i] = '!'; break;
		case 'V': case 'v': case '|':
			expression[i] = '+'; break;
		case '^': case '*': case '&':
			expression[i] = '.'; break;
		case '!': case '+': case '.': case '(': case ')': case '0': case '1':
			break;
		default:
			if ('a' <= expression[i] && expression[i] <= 'z' || 'A' <= expression[i] && expression[i] <= 'Z')
			{    // kiểm tra xem biến đã xuất hiện trước đó chưa
				if (find(variables_names.begin(), variables_names.end(), expression[i]) == variables_names.end())
					variables_names.push_back(expression[i]);
			}
			else return 0;	// invalid character(s) in the function
		}
	// find duplicated NOT '!!'
	for (int i = 0; i < expression.length() - 1; i++)
		if (expression[i] == '!' && expression[i + 1] == '!')
			expression.erase(i--, 2);
	return 1;
}

bool invalid_check(const string &expression)
{
	if (expression[0] == '.' || expression[0] == '+' || expression[0] == ')')
		return 0;
	if (expression.back() == '.' || expression.back() == '+' || expression.back() == '(' || expression.back() == '!')
		return 0;
	int open_parenthesis_count = 0;
	for (int i = 0; i < expression.length() - 1; i++)
		switch (expression[i]) {
		case '.': case '+': case '!':
			switch (expression[i + 1]) {
			case ')': case '+': case '.':
				return 0;
			}
			break;
		case '(':
			switch (expression[i + 1]) {
			case '+': case '.':
				return 0;
			}
			open_parenthesis_count++; break;
		case ')':
			if (open_parenthesis_count == 0)
				return 0;
			switch (expression[i + 1]) {
			case '.': case '+': case ')':
				open_parenthesis_count--; break;
			default:
				return 0;
			}
			break;
		default:
			switch (expression[i + 1]) {
			case '.': case '+': case ')':
				break;
			default:
				return 0;
			}
		}
	if (open_parenthesis_count > 1 || (open_parenthesis_count == 0) == (expression.back() == ')'))
		return 0;
	return 1;
}

int priority(char my_operator)
{
	switch (my_operator) {
	case '!':
		return 3;
	case '.':
		return 2;
	case '+':
		return 1;
	case '(':
		return 0;
	}
}

void postfix_convert(const string &expression, string &postfix_expr)
{	// Su dung thuat toan Shunting-yard algorithm
	stack<char> operator_stack;
	for (char c : expression)
		switch (c)
		{
		case '(':
			operator_stack.push(c); break;
		case ')':
			while (true) {
				char x = operator_stack.top();
				operator_stack.pop();
				if (x != '(')
					postfix_expr += x;
				else break;
			}
			break;
		case '!': case '.': case '+':
			while (!operator_stack.empty() && priority(c) < priority(operator_stack.top())) {
				postfix_expr += operator_stack.top();
				operator_stack.pop();
			}
			operator_stack.push(c);
			break;
		default: // variables(also operands)
			postfix_expr += c;
		}
	while (!operator_stack.empty()) {
		postfix_expr += operator_stack.top();
		operator_stack.pop();
	}
}

