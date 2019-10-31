#include "String_Process.h"

extern unordered_map<char, Token> Vars_names;
extern char var_assign;

bool _digit(char);
bool _operator(char);
bool varName(char c);

void init(Lexer &lex, string &str)
{
	// find duplicated minus '--'
	for (int i = 0; i < str.length() - 1; i++)
		if (str[i] == '-' && str[i + 1] == '-')
		{
			str.erase(i, 1);
			str[i--] = '+';
		}

	lex.str = str;
	lex.pos = 0;
}

void get_token(Lexer &lex, Token &tok)
{
	char c;
	if (lex.pos == lex.str.length())
		tok.type = TOK_EOL;
	else if ((c = lex.str[lex.pos]) == '-')
	{
		char b;
		if (lex.pos == 0 || _operator(b = lex.str[lex.pos - 1]) && b != ')')
			tok.valptr = new char('~');	// unary minus
		else tok.valptr = new char('-');
		tok.type = TOK_OPERATOR; lex.pos++;
	}
	else if (_operator(c))	// '+' || '*' || '(' || ')'
	{
		tok.type = TOK_OPERATOR; tok.valptr = new char(c);
		lex.pos++;
	}
	else if (varName(c))
	{
		auto find = Vars_names.find(c);
		if (find != Vars_names.end())
			tok = find->second;	// deep-copy
		else
			tok.type = TOK_ERROR2;
		lex.pos++;
	}
	else if (_digit(c))
	{
		tok.type = TOK_NUM; tok.valptr = new double;
		istringstream numstr(lex.str.substr(lex.pos));
		string remain_str;
		numstr >> *(double*)tok.valptr >> remain_str;
		lex.pos = lex.str.length() - remain_str.length();
	}
	else if (c == '[')
	{
		int begin_pos = lex.pos;
		int where_close_bracket;
		do {
			where_close_bracket = lex.str.find(']', lex.pos);
			if (where_close_bracket == lex.str.npos) {
				tok.type = TOK_ERROR1; lex.pos++;
				return;
			}
			lex.pos = where_close_bracket + 1;
		} while (lex.pos < lex.str.length() && lex.str[lex.pos] == '[');

		Matrix mat(lex.str.substr(begin_pos, lex.pos - begin_pos));	// lấy hết chuỗi [...] đưa vào mat(chuỗi)
		if (mat.NRows() == 0)
			tok.type = TOK_ERROR1;
		else if (mat.NRows() == 1) {
			tok.type = TOK_VECTOR;
			tok.valptr = new Vector(mat[0]);
		}
		else {
			tok.type = TOK_MATRIX;
			tok.valptr = new Matrix(mat);
		}
	}
	else
	{
		tok.type = TOK_ERROR1;	// ký tự lạ hoặc sai chính tả
		lex.pos++;
	}
	return;
}


bool _digit(char c)
{
	if ('0' <= c && c <= '9' || c == '.')
		return true;
	return false;
}

bool varName(char c)
{
	if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_')
		return true;
	return false;
}

bool _operator(char c)
{
	if (c == '*' || c == '/' || c == '+' || c == '-' || c == '(' || c == ')')
		return true;
	return false;
}

void delete_token(Token &tok)
{
	if (tok.valptr == NULL || tok.type == -1)
		return;
	switch (tok.type)
	{
	case TOK_OPERATOR:
		delete (char*)tok.valptr; break;
	case TOK_NUM:
		delete (double*)tok.valptr; break;
	case TOK_VECTOR:
		delete (Vector*)tok.valptr; break;
	case TOK_MATRIX:
		delete (Matrix*)tok.valptr; break;
	}
	tok.valptr = NULL; tok.type = -1;
}

void erase_space(string &str)
{
	int where_space;
	while ((where_space = str.find(" ")) != str.npos)
		str.erase(where_space, 1);
}

bool seperate_lvalue_rvalue(string &expression, string &r_Express)
{
	auto where_equal = expression.find("=");
	string left_expression;
	if (where_equal != expression.npos) {
		left_expression = expression.substr(0, where_equal);
		if (left_expression.length() == 0 || left_expression.length() > 1)
			return 0;
		var_assign = left_expression[0];
		if (!varName(var_assign))
			return 0;
		r_Express = expression.substr(where_equal + 1);
	}
	else
		r_Express = expression;	// nếu có nhiều hơn 1 dấu bằng thì các dấu bằng thứ 2 trở đi sẽ nằm bên r_Express

	if (r_Express.find("=") != r_Express.npos)
		return 0;
	return 1;
}

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