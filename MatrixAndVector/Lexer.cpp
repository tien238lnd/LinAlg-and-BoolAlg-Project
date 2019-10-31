#include "Lexer.h"

bool _digit(char);
bool _operator(char);
bool _varName(char c);

void init(Lexer &lex, string &str)
{
	lex.str = str;
	lex.pos = 0;
}

void get_token(Lexer &lex, Token &tok, const unordered_map<char, Token> &varNames)
{
	//delete_token(tok);
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
	else if (_varName(c))
	{
		auto find = varNames.find(c);
		if (find != varNames.end())
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

bool _varName(char c)
{
	if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_')
		return true;
	return false;
}

bool _operator(char c)
{
	if (c == '*' || c == '+' || c == '-' || c == '(' || c == ')')
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
}