#pragma once

#include "common.h"

namespace nlang {

enum token {
	T_UNKNOWN,
#define TOKEN(name, text) T_ ## name,
#include "tokens.inc.h"
	token_count
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
std::string token_to_text(token tk);

bool token_is_operator(token tk);
int token_operator_prec(token tk);
bool token_operator_lassoc(token tk);

}