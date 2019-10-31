#pragma once

#include <string>
#include "Vector.h"
#include "Matrix.h"
#include <sstream>
#include <unordered_map>
using namespace std;

#define TOK_EOL 0
#define TOK_ERROR1 1		// ký tự lạ hoặc sai chính tả
#define TOK_ERROR2 2		// ko tìm thấy tên biến
#define TOK_OPERATOR 3		// char
#define TOK_NUM 4			// double
#define TOK_VECTOR 5		// Vector
#define TOK_MATRIX 6		// Matrix

class Token
{
public:
	char type;
	void* valptr;
	Token() { type = -1; valptr = NULL; }
	Token(const Token &other) { *this = other; }
	~Token() { delete_token(*this); }
	Token& operator=(const Token &other) {
		if (&other == this)
			return *this;
		delete_token(*this);
		type = other.type;
		if (other.valptr == NULL)
			return *this;
		switch (type)
		{
		case TOK_OPERATOR:
			valptr = new char(*(char*)other.valptr); break;
		case TOK_NUM:
			valptr = new double(*(double*)other.valptr); break;
		case TOK_VECTOR:
			valptr = new Vector(*(Vector*)other.valptr); break;
		case TOK_MATRIX:
			valptr = new Matrix(*(Matrix*)other.valptr); break;
		}
		return *this;
	}
	friend void delete_token(Token &tok);
};

struct Lexer		// một chuỗi và một con chạy trên chuỗi đó
{
	string str;
	int pos;
};


void init(Lexer &lex, string &str);
void get_token(Lexer &lex, Token &tok, const unordered_map<char, Token> &varNames);

