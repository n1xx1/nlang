#pragma once

#include "common.h"

namespace nlang {

enum token {
	T_UNKNOWN,
	T_EOF,

	T_IDENT,
	T_STRING,
	T_CHAR,
	T_INT,
	T_FLOAT,

	// DELIMITERS
	T_SEMI, // ;
	T_LBRACE, // {
	T_RBRACE, // }
	T_LBRACK, // [
	T_RBRACK, // ]
	T_LPAREN, // (
	T_RPAREN, // )
	T_DOT, // .
	T_DOTDOTDOT, // ...
	T_COMMA, // ,
	T_COLON, // :

	// INC OPERATORS
	T_INC, // ++
	T_DEC, // --

	// OPERATORS
	T_ARROW, // ->
	T_DARROW, // =>
	T_NOT, // !
	// precLOR
	T_LOR, // ||
	// precLAND
	T_LAND, // &&
	// precCOMP
	T_EQ, // ==
	T_LT, // <
	T_GT, // >
	T_LTE, // <=
	T_GTE, // >=
	T_NEQ, // !=
	// precADD
	T_ADD, // +
	T_SUB, // -
	T_OR, // |
	T_XOR, // ^
	// precMUL
	T_MUL, // *
	T_DIV, // /
	T_REM, // %
	T_AND, // &
	T_SHL, // <<
	T_SHR, // >>
	
	// ASSIGNMENT OPERATORS
	T_ASSIGN, // =
	T_ASSIGN_ADD, // +=
	T_ASSIGN_SUB, // -=
	T_ASSIGN_MUL, // *=
	T_ASSIGN_REM, // %=
	T_ASSIGN_DIV, // /=
	T_ASSIGN_OR, // |=
	T_ASSIGN_AND, // &=
	T_ASSIGN_XOR, // ^=
	T_ASSIGN_SHL, // <<=
	T_ASSIGN_SHR, // >>=

	T_KW_LET, // let
	T_KW_CONST, // const
	T_KW_FN, // func
	T_KW_TYPE, // type
	T_KW_BREAK, // break
	T_KW_CONTINUE, // continue
	T_KW_RETURN, // return
};


class lexer {
public:
	lexer(const char* src, int w);
	void next();

	int _tline, _tcol;
	token _ttok;
	std::string _tlit;

private:
	void ident();
	void number(u32 c);
	void line_comment();
	void full_comment();
	void error(std::string err);

	u32 getr();
	void ungetr();
	void ungetr2();

	void start_lit();
	std::string stop_lit();

	const char* _src;
	int _r0, _r, _w;
	int _line0, _line;
	int _col0, _col;
	int _suf;

	bool _nlsemi;
};

std::string token_to_string(token tk);

}