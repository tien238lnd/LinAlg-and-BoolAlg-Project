#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include "Lexer.h"
using namespace std;


int priority(char op)
{
	switch (op) {
	case '~':
		return 3;
	case '*':
		return 2;
	case '+': case '-':
		return 1;
	case '(':
		return 0;
	}
}

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

int cal_operator(stack<Token> &operands, char op)
{	// TOK_NUM double - TOK_VECTOR Vector - TOK_MATRIX Matrix
	// ở đây cũng có thể có lỗi, vd matrix + số -> lỗi
	// return 0 là ok
	// return 1 là lỗi ko tính được
	// return 2 là lỗi về số chiều
	Token newtok;
	if (op == '~')
	{
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
	else {	// '+' '-' '*'
		Token tok2 = operands.top(); operands.pop();
		Token tok1 = operands.top(); operands.top();
		if (tok2.type == tok1.type)
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
				newtok.valptr = new Matrix(*(double*)tok2.valptr * *(Vector*)tok1.valptr);
			}
			else if (tok1.type == TOK_VECTOR && tok2.type == TOK_MATRIX) {
				newtok.type = TOK_MATRIX;
				newtok.valptr = new Matrix(*(Vector*)tok1.valptr * *(Matrix*)tok2.valptr);
			}
			else return 1;	// matrix * vector ko tính được
		}
		else return 1;	// ko phải phép nhân mà kiểu cũng ko giống nhau, thì ko tính được
		operands.top() = newtok;
		switch (operands.top().type)
		{	// check lỗi số chiều
		case TOK_NUM:
			if (*(double*)operands.top().valptr == DBL_MAX)
				return 2;
		case TOK_VECTOR:
			if (((Vector*)operands.top().valptr)->size() == 0)
				return 2;	
		case TOK_MATRIX:
			if (((Matrix*)operands.top().valptr)->NRows() == 0)
				return 2;
		}
	}
	return 0;
}

int calculate_infix(Token &lvalue, Lexer &lex, unordered_map<char, Token> &Vars_names)
{
	// return 0: ko lỗi
	// return 1: ký tự lạ hoặc sai chính tả
	// return 2: ko tìm thấy tên biến
	// return 3: sai cú pháp
	// return 4: tính ko được
	// return 5: lỗi số chiều
	vector<Token> tokens;
	while (true)
	{
		Token tok;
		get_token(lex, tok, Vars_names);
		if (tok.type == TOK_EOL)
			break;
		if (tok.type == TOK_ERROR1 || tok.type == TOK_ERROR2) 
			return tok.type;	// ký tự lạ hoặc ko tìm thấy tên biến
		tokens.push_back(tok);	// tạm thời để cho hắn deep-copy ri đã, cải tiến sau (đưa Token tok ra ngoài vòng for để hắn khỏi bị hủy, rồi viết lại thành shallow copy)
		// th cứ để cho hắn deep-copy đi, lo test cho xong đã nè

		//tokens.push_back(Token());	
		//get_token(lex, tokens.back(), Vars_names);
		//if (tokens.back().type == TOK_EOL) {
		//	tokens.pop_back(); break;
		//}
		//if (tokens.back().type == TOK_ERROR1 || tokens.back().type == TOK_ERROR2)
		//	return tokens.back().type;	// ký tự lạ hoặc ko tìm thấy tên biến
	}

	stack<char> operators;
	stack<Token> operands;
	for (Token &tok : tokens)
		if (tok.type == TOK_OPERATOR) {	// '*' || '+' || '-' || '~' || '(' || ')' 
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
							return ans_check + 3; // 1+3 = 4: lỗi tính toán - 2+3= 5: lỗi số chiều
					}
					else break;
				}
				break;
			default: // '*' || '+' || '-' 
				while (!operators.empty() && priority(c) <= priority(operators.top())) {
					int ans_check = cal_operator(operands, operators.top());
					if (ans_check != 0)
						return ans_check + 3; // 1+3 = 4: lỗi tính toán; 2+3= 5: lỗi số chiều
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
			return ans_check + 3; // 1+3 = 4: lỗi tính toán - 2+3= 5: lỗi số chiều
		operators.pop();
	}
	if (operands.size() == 0 || operands.size() > 1)
		return 3;	// lỗi cú pháp chi chi đó
	lvalue = operands.top();
	return 0;	// no error
}

// Hàm in giá trị của biến ra
void print_ans(const Token &tok)
{
	switch (tok.type)
	{
	case TOK_NUM:
		cout << *(double*)tok.valptr << endl; break;
	case TOK_VECTOR:
		cout << *(Vector*)tok.valptr; break;
	case TOK_MATRIX:
		cout << *(Matrix*)tok.valptr; break;
	}
	return;
}

bool seperate_lvalue_rvalue(string &expression, char &var_assign, string &right_expression)
{
	auto where_equal = expression.find("=");
	string left_expression;
	if (where_equal != expression.npos) {
		left_expression = expression.substr(0, where_equal);
		if (left_expression.length() == 0 || left_expression.length() > 1)
			return 0;
		var_assign = left_expression[0];
		right_expression = expression.substr(where_equal + 1);
	}
	else
		right_expression = expression;	// nếu có nhiều hơn 1 dấu bằng thì các dấu bằng thứ 2 trở đi sẽ nằm bên right_expression

	if (right_expression.find("=") != right_expression.npos)
		return 0;
	return 1;
}

int main()
{
	cout << "Ten chuong trinh, huong dan nay no blah blah:\n";
	unordered_map<char, Token> Vars_names;
	while (true)
	{
		cout << "\n>>> ";
		string expression;
		getline(cin, expression);

		//cin.ignore(INT_MAX, '\n');

		// xóa tất cả khoảng trắng
		int where_space;
		while ((where_space = expression.find(" ")) != expression.npos)
			expression.erase(where_space, 1);

		if (expression.length() == 0)
			continue;
		if (expression == "exit()")
			break;

		char var_assign = '\0';
		string right_expression;
		if (seperate_lvalue_rvalue(expression, var_assign, right_expression) == 0) {
			cout << "Wrong expression!\n";
			continue;
		}
		/*if (right_expression.substr(0, 7) == "invert(" && right_expression.back() == ')')
		{
			Matrix mat(right_expression.substr(8, right_expression.length() - 8));
			if (mat.NRows() >= 2)



		}*/
		Lexer lexer_expression;
		init(lexer_expression, right_expression);
		int check_ans = calculate_infix(Vars_names['_'], lexer_expression, Vars_names);
		switch (check_ans)
		{
		case 1:
			cout << "Invalid character or misspelling!\n"; break;
		case 2:
			cout << "Variable not found!\n"; break;
		case 3:
			cout << "Syntax error!\n"; break;
		case 4:
			cout << "Operator error!\n"; break;
		case 5:
			cout << "Dimension error!\n"; break;
		default:
		{
			print_ans(Vars_names['_']);
			if (var_assign != '\0')
				Vars_names[var_assign] = Vars_names['_'];
		}
		}
	}
	// trong thuật toán chuyển trung tố về hậu tố, mỗi phần tử của mảng expression có 2 loại: 1 là operand, 2 là operator
	// vậy việc tiếp theo là nhận diện operator và operand
	// nhận diện operand bao gồm: số, ma trận, vector, tên biến
	// nhận diện operator bao gồm: + - * det(.) invert(.) solve(., .) rank(.) vector(., .) matrix(. )
	// nếu giới hạn các tr/h sau là lệnh và chỉ được gọi một mình, kèm với một phép gán, thì chỉ còn lại các phép + - *
	// nếu tiếp tục giới hạn nữa thì sẽ quá đơn giản.
	// ý tưởng là sau khi nhận diện được thì lưu lại hết thành một mảng, chứ k để như chuỗi thô nữa
	// làm sao nhận diện được số? matrix? vector?
	// chắc phải viết ra cụ thể tất cả các trường hợp như hôm bữa với hàm bool, rồi check invalid, 
	return 0;
}