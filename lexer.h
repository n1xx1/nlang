#include <string>
#include <type_traits>

namespace nlang {

using u32 = uint32_t;

enum token {
	T_UNKNOWN,
	T_EOF,

	T_IDENT,
	T_STRING,
	T_CHAR,
	T_INT,
	T_FLOAT,

	// DELIMITERS
	T_LBRACE, // {
	T_RBRACE, // }
	T_LBRACKET, // [
	T_RBRACKET, // ]
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
	T_ASSIGN_MUL, // /=
	T_ASSIGN_DIV, // /=
	T_ASSIGN_OR, // |=
	T_ASSIGN_AND, // &=
	T_ASSIGN_XOR, // ^=
	T_ASSIGN_SHL, // <<=
	T_ASSIGN_SHR, // >>=
	T_ASSIGN_LOR, // ||=
	T_ASSIGN_LAND, // &&=

	T_LET, // let
	T_CONST, // const
	T_FN, // func
	T_TYPE, // type
};


class lexer {
public:
	lexer(const char* src, int w);
	void next();

private:
	const char* _src;
	int _r0, _r, _w;
	int _line0, _line;
	int _col0, _col;

	bool _nlsemi;

	int _tline, _tcol;
	token _ttok;
	int _tbegin, _tend;
};


}