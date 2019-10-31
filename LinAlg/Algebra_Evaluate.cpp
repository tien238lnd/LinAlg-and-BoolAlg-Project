#include "Algebra_Evaluate.h"

template <typename Type>
Type same_type_oper1(const Type &a, const Type &b, char op) {
	switch (op)
	{
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	}
}

template <typename Type>
Type same_type_oper2(const Type &a, const Type &b, char op) {
	switch (op)
	{
	case '+':
		return a + b;
	case '-':
		return a - b;
	}
}

int priority(char op);
int cal_operator(stack<Token> &operands, char op);

int calculate_infix(Token &lvalue, Lexer &lex)
{
	// return 0: ko lỗi
	// return 1: ký tự lạ hoặc sai chính tả
	// return 2: ko tìm thấy tên biến
	// return 3: tính ko được
	// return 4: lỗi số chiều
	// return 5: sai cú pháp
	// return 6: chia cho 0
	vector<Token> tokens;
	while (true)
	{
		Token tok;
		get_token(lex, tok);
		if (tok.type == TOK_EOL)
			break;
		if (tok.type == TOK_ERROR1 || tok.type == TOK_ERROR2) 
			return tok.type;	// ký tự lạ hoặc ko tìm thấy tên biến
		tokens.push_back(tok);
	}

	stack<char> operators;
	stack<Token> operands;
	for (Token &tok : tokens)
		if (tok.type == TOK_OPERATOR) {	// '*' '/' '+' '-' '~' '(' ')' 
			char c = *(char*)tok.valptr;
			switch (c)
			{
			case '(':
				operators.push('('); break;
			case ')':
				while (true) {
					char op = operators.top(); operators.pop();
					if (op != '(') {
						int ans_check = cal_operator(operands, op);
						if (ans_check != 0)
							return ans_check + 2; // 1+2 = 3: lỗi tính toán - 2+2 = 4: lỗi số chiều - 3+2 = 5: lỗi cú pháp
					}
					else break;
				}
				break;
			default: // '*' '/' '+' '-' 
				while (!operators.empty() && priority(c) <= priority(operators.top())) {
					int ans_check = cal_operator(operands, operators.top());
					if (ans_check != 0)
						return ans_check + 2; // 1+2 = 3: lỗi tính toán - 2+2 = 4: lỗi số chiều - 3+2 = 5: lỗi cú pháp
					operators.pop();
				}
				operators.push(c); break;
			}
		}
		else // TOK_NUM - TOK_VECTOR - TOK_MATRIX
			operands.push(tok);
			

	while (!operators.empty()) {
		int ans_check = cal_operator(operands, operators.top());
		if (ans_check != 0)
			return ans_check + 2; // 1+2 = 3: lỗi tính toán - 2+2 = 4: lỗi số chiều - 3+2 = 5: lỗi cú pháp
		operators.pop();
	}
	if (operands.size() == 0 || operands.size() > 1)
		return 3;	// lỗi cú pháp chi chi đó
	lvalue = operands.top();
	return 0;	// no error
}


int priority(char op)
{
	switch (op) {
	case '~':
		return 3;
	case '*': case '/':
		return 2;
	case '+': case '-':
		return 1;
	case '(':
		return 0;
	}
}

int cal_operator(stack<Token> &operands, char op)
{	// TOK_NUM double - TOK_VECTOR Vector - TOK_MATRIX Matrix
	// ở đây cũng có thể có lỗi, vd matrix + số -> lỗi
	// return 0 là ok
	// return 1 là lỗi ko tính được
	// return 2 là lỗi về số chiều
	// return 3 là lỗi syntax
	// return 4 là lỗi chia cho 0
	Token newtok;
	if (op == '~')
	{
		if (operands.size() == 0)
			return 3;
		switch (newtok.type = operands.top().type)
		{
		case TOK_NUM:
			newtok.valptr = new double((-1) * *(double*)operands.top().valptr); break;
		case TOK_VECTOR:
			newtok.valptr = new Vector((-1) * *(Vector*)operands.top().valptr); break;
		case TOK_MATRIX:
			newtok.valptr = new Matrix((-1) * *(Matrix*)operands.top().valptr); break;
		}
		operands.top() = newtok;
	}
	else {	// '+' '-' '*' '/'
		if (operands.size() < 2)
			return 3;
		Token tok2 = operands.top(); operands.pop();
		Token tok1 = operands.top(); operands.top();
		if (op == '/') {
			if (tok2.type != TOK_NUM)
				return 1;
			if (abs(*(double*)tok2.valptr - 0) < 10e-8)
				return 4;
			switch (newtok.type = tok1.type)
			{
			case TOK_NUM:
				newtok.valptr = new double(*(double*)tok1.valptr / *(double*)tok2.valptr); break;
			case TOK_VECTOR:
				newtok.valptr = new Vector((1/ *(double*)tok2.valptr) * *(Vector*)tok1.valptr); break;
			case TOK_MATRIX:
				newtok.valptr = new Matrix((1 / *(double*)tok2.valptr) * *(Matrix*)tok1.valptr); break;
			}
		}
		else if (tok2.type == tok1.type)
			if (tok2.type == TOK_VECTOR && op == '*') {
				newtok.type = TOK_NUM;	// nhân vô hướng 2 vector, trả về 1 số
				newtok.valptr = new double(*(Vector*)tok1.valptr * *(Vector*)tok2.valptr);
			}
			else
				switch (newtok.type = tok1.type)
				{	// cộng/trừ/nhân hai type cùng kiểu, trả về kiểu đó luôn
				case TOK_NUM:
					newtok.valptr = new double(same_type_oper1(*(double*)tok1.valptr, *(double*)tok2.valptr, op)); break;
				case TOK_VECTOR:
					newtok.valptr = new Vector(same_type_oper2(*(Vector*)tok1.valptr, *(Vector*)tok2.valptr, op)); break;
				case TOK_MATRIX:
					newtok.valptr = new Matrix(same_type_oper1(*(Matrix*)tok1.valptr, *(Matrix*)tok2.valptr, op)); break;
				}
		else if (op == '*') {
			if (tok1.type == TOK_NUM && tok2.type == TOK_VECTOR) {
				newtok.type = TOK_VECTOR;
				newtok.valptr = new Vector(*(double*)tok1.valptr * *(Vector*)tok2.valptr);
			}
			else if (tok1.type == TOK_VECTOR && tok2.type == TOK_NUM) {
				newtok.type = TOK_VECTOR;
				newtok.valptr = new Vector(*(double*)tok2.valptr * *(Vector*)tok1.valptr);
			}
			else if (tok1.type == TOK_NUM && tok2.type == TOK_MATRIX) {
				newtok.type = TOK_MATRIX;
				newtok.valptr = new Matrix(*(double*)tok1.valptr * *(Matrix*)tok2.valptr);
			}
			else if (tok1.type == TOK_MATRIX && tok2.type == TOK_NUM) {
				newtok.type = TOK_MATRIX;
				newtok.valptr = new Matrix(*(double*)tok2.valptr * *(Matrix*)tok1.valptr);
			}
			else if (tok1.type == TOK_VECTOR && tok2.type == TOK_MATRIX) {
				newtok.type = TOK_MATRIX;
				newtok.valptr = new Matrix(*(Vector*)tok1.valptr * *(Matrix*)tok2.valptr);
			}
			else return 1;	// matrix * vector ko tính được
		}
		else return 1;	// ko phải phép nhân mà kiểu cũng ko giống nhau, thì ko tính được
		switch (newtok.type)
		{	// check lỗi số chiều
		case TOK_NUM:
			if (*(double*)operands.top().valptr == DBL_MAX)
				return 2;
			break;
		case TOK_VECTOR:
			if (((Vector*)operands.top().valptr)->size() == 0)
				return 2;
			break;
		case TOK_MATRIX:
			if (((Matrix*)operands.top().valptr)->NRows() == 0)
				return 2;
			break;
		}
		operands.top() = newtok;
	}
	return 0;
}