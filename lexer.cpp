#include "lexer.h"

using namespace nlang;

lexer::lexer(const char* src, int w) :
	_src(src),
	_r0(0), _r(0), _w(w), 
	_line0(0), _line(0),
	_col0(0), _col(0), 
	_nlsemi(false), 
	_tline(0), _tcol(0),
	_ttok(T_UNKNOWN),
	_tbegin(0), _tend(0) {}

void lexer::next() {

}