#include "lexer.h"

using namespace nlang;

lexer::lexer(const char* src, int w) :
	_src(src),
	_r0(0), _r(0), _w(w), 
	_line0(0), _line(1),
	_col0(0), _col(1),
	_suf(-1),
	_nlsemi(false), 
	_tline(0), _tcol(0),
	_ttok(T_UNKNOWN) {}

static bool is_letter(u32 c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}
static bool is_digit(u32 c) {
	return ('0' <= c && c <= '9');
}

void lexer::next() {
	bool nlsemi = _nlsemi;
	_nlsemi = false;

redo:
	u32 c = getr();
	while(c == ' ' || c == '\t' || (c == '\n' && !nlsemi) || c == '\r') c = getr();

	_tline = _line0;
	_tcol = _col0;
	_tlit = "";

	if(is_letter(c)) {
		ident();
		return;
	}
		
	switch(c) {
	case -1:
		if(nlsemi) {
			_tlit = "EOF";
			_ttok = T_SEMI;
			return;
		}
		_ttok = T_EOF;
		return;
	case '\n':
		_tlit = "newline";
		_ttok = T_SEMI;
		return;
		// TODO: '"' -> T_STRING
		// TODO: '\'' -> T_CHAR
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		number(c);
		return;
	case ';':
		_tlit = "semicolon";
		_ttok = T_SEMI;
		return;
	case '{':
		_ttok = T_LBRACE;
		return;
	case '}':
		_ttok = T_RBRACE;
		return;
	case '[':
		_ttok = T_LBRACK;
		return;
	case ']':
		_ttok = T_RBRACK;
		return;
	case '(':
		_ttok = T_LPAREN;
		return;
	case ')':
		_ttok = T_RPAREN;
		return;
	case ',':
		_ttok = T_COMMA;
		return;
	case '.':
		c = getr();
		if(is_digit(c)) {
			ungetr2();
			number('.');
			return;
		}
		if(c == '.') {
			c = getr();
			if(c == '.') {
				_ttok = T_DOTDOTDOT;
				return;
			}
			ungetr2();
		}
		ungetr();
		_ttok = T_DOT;
		return;
	case '+':
		c = getr();
		if(c == '+') {
			_ttok = T_INC;
			_nlsemi = true;
		} else if(c == '=') {
			_ttok = T_ASSIGN_ADD;
		} else {
			_ttok = T_ADD;
			ungetr();
		}
		return;
	case '-':
		c = getr();
		if(c == '-') {
			_ttok = T_DEC;
			_nlsemi = true;
		} else if(c == '=') {
			_ttok = T_ASSIGN_SUB;
		} else {
			_ttok = T_SUB;
			ungetr();
		}
		return;
	case '*':
		c = getr();
		if(c == '=') {
			_ttok = T_ASSIGN_MUL;
		} else {
			_ttok = T_MUL;
			ungetr();
		}
		return;
	case '/':
		c = getr();
		if(c == '/') {
			line_comment();
			goto redo;
		} else if(c == '*') {
			full_comment();
			if(_line > _tline && nlsemi) {
				_tlit = "newline";
				_ttok = T_SEMI;
				return;
			}
			goto redo;
		} else if(c == '=') {
			_ttok = T_ASSIGN_DIV;
		} else {
			_ttok = T_DIV;
			ungetr();
		}
		return;
	case '%':
		c = getr();
		if(c == '=') {
			_ttok = T_ASSIGN_REM;
		} else {
			_ttok = T_REM;
			ungetr();
		}
		return;
	case '&':
		c = getr();
		if(c == '&') {
			_ttok = T_LAND;
		} else if(c == '=') {
			_ttok = T_ASSIGN_AND;
		} else {
			_ttok = T_AND;
			ungetr();
		}
		return;
	case '|':
		c = getr();
		if(c == '|') {
			_ttok = T_LOR;
		} else if(c == '=') {
			_ttok = T_ASSIGN_OR;
		} else {
			_ttok = T_OR;
			ungetr();
		}
		return;
	case '^':
		c = getr();
		if(c == '=') {
			_ttok = T_ASSIGN_XOR;
		} else {
			_ttok = T_XOR;
			ungetr();
		}
		return;
	case '<':
		c = getr();
		if(c == '=') {
			_ttok = T_LTE;
		} else if(c == '<') {
			c = getr();
			if(c == '=') {
				_ttok = T_ASSIGN_SHL;
			} else {
				ungetr();
				_ttok = T_SHL;
			}
		} else {
			_ttok = T_LT;
			ungetr();
		}
		return;
	case '>':
		c = getr();
		if(c == '=') {
			_ttok = T_GTE;
		} else if(c == '>') {
			c = getr();
			if(c == '=') {
				_ttok = T_ASSIGN_SHR;
			} else {
				ungetr();
				_ttok = T_SHR;
			}
		} else {
			_ttok = T_GT;
			ungetr();
		}
		return;
	case '=':
		c = getr();
		if(c == '=') {
			_ttok = T_EQ;
		} else {
			_ttok = T_ASSIGN;
			ungetr();
		}
		return;
	case '!':
		c = getr();
		if(c == '=') {
			_ttok = T_NEQ;
		} else {
			_ttok = T_NOT;
			ungetr();
		}
		return;
	default:
		error(std::string("invalid character: ") + (char)c);
	}
}

void lexer::ident() {
	start_lit();

	u32 c = getr();
	while(is_letter(c) || is_digit(c) || c == '_') c = getr();
	ungetr();

	_tlit = stop_lit();
	_ttok = T_IDENT;
	_nlsemi = true;

	if(_tlit.size() >= 2) {
		_nlsemi = false;
		if(_tlit == "let") _ttok = T_KW_LET;
		else if(_tlit == "const") _ttok = T_KW_CONST;
		else if(_tlit == "func") _ttok = T_KW_FN;
		else if(_tlit == "type") _ttok = T_KW_TYPE;
		else {
			_nlsemi = true;
			if(_tlit == "break") _ttok = T_KW_BREAK;
			else if(_tlit == "continue") _ttok = T_KW_CONTINUE;
			else if(_tlit == "return") _ttok = T_KW_RETURN;
		}
	}
}

void lexer::number(u32 c) {
	start_lit();

	if(c != '.') {
		_ttok = T_INT;
		if(c == '0') {
			c = getr();
			if(c == 'x' || c == 'X') {
				// hex
				c = getr();
				bool has_digit = false;
				while(is_digit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F')) {
					c = getr();
					has_digit = true;
				}
				if(!has_digit) error("malformed hex constant");
				goto done;
			}
			bool has8or9 = false;
			while(is_digit(c)) {
				if(c >= '7') has8or9 = true;
				c = getr();
			}
			if(c != '.' && c != 'e' && c != 'E') {
				// octal
				if(has8or9) error("malformed octal constant");
				goto done;
			}
		} else {
			// decimal or float
			while(is_digit(c)) c = getr();
		}
	}

	// float
	if(c == '.') {
		_ttok = T_FLOAT;
		c = getr();
		while(is_digit(c)) c = getr();
	}

	// exponent
	if(c == 'e' || c == 'E') {
		_ttok = T_FLOAT;
		c = getr();
		if(c == '-' || c == '+') c = getr();
		if(!is_digit(c)) error("malformed floating-point constant exponent");
		while(is_digit(c)) c = getr();
	}

done:
	ungetr();
	_nlsemi = true;
	_tlit = stop_lit();
}

void lexer::line_comment() {
	u32 r = getr();
	while(r != -1) {
		if(r == '\n') {
			ungetr(); // dont consume \n
			break;
		}
		r = getr();
	}
}
void lexer::full_comment() {
	while(true) {
		u32 r = getr();
		while(r == '*') {
			r = getr();
			if(r == '/') return;
			
			if(r == -1) error("comment not terminated");
			return;
		}
	}
}

void lexer::error(std::string err) {
	std::string e = "[" + std::to_string(_line0) + ":" + std::to_string(_col0) + "] " + err;
	throw new std::runtime_error(e);
}

#define DEBUG_GETR 0

u32 lexer::getr() {
	_r0 = _r;
	_line0 = _line;
	_col0 = _col;

	if(_r == _w) return -1; // eof
	
	char b = _src[_r];
	if(b < 0x80) {
		// ascii character

#if DEBUG_GETR
		char dstr[3] = { b, '\0', '\0' };
		if(b == '\n') { dstr[0] = '\\'; dstr[1] = '\n'; }
		printf("_src[%d] = %s [%d:%d]\n", _r, dstr, _line, _col);
#endif

		_r++;
		_col++;

		if(b == 0) throw new std::runtime_error("invalid NUL character");
		if(b == '\n') {
			_line++;
			_col = 1;
		}
		return (u32)b;
	}
	error("UTF8 not implemented yet");
}
void lexer::ungetr() {
	_r = _r0;
	_line = _line0;
	_col = _col0;
}
// like ungetr, but enables second ungetr (line must be the same)
void lexer::ungetr2() {
	ungetr();
	_r0--;
	_col0--;
}

void lexer::start_lit() {
	_suf = _r0;
}
std::string lexer::stop_lit() {
	std::string lit = std::string(_src + _suf, _r - _suf);
	_suf = -1;
	return lit;
}

std::string nlang::token_to_string(token tk) {
	switch(tk) {
	case T_EOF: return "TK_EOF";

	case T_IDENT: return "TK_IDENT";
	case T_STRING: return "TK_STRING";
	case T_INT: return "TK_INT";
	case T_FLOAT: return "TK_FLOAT";

	case T_SEMI: return "T_SEMI";
	case T_LBRACE: return "TK_LBRACE";
	case T_RBRACE: return "TK_RBRACE";
	case T_LBRACK: return "TK_LBRACK";
	case T_RBRACK: return "TK_RBRACK";
	case T_LPAREN: return "TK_LPAREN";
	case T_RPAREN: return "TK_RPAREN";
	case T_DOT: return "T_DOT";
	case T_DOTDOTDOT: return "T_DOTDOTDOT";
	case T_COLON: return "T_COLON";
	case T_COMMA: return "T_COMMA";

	case T_INC: return "T_INC";
	case T_DEC: return "T_DEC";

	case T_ADD: return "T_ADD";
	case T_SUB: return "T_SUB";
	case T_MUL: return "T_MUL";
	case T_REM: return "T_REM";
	case T_DIV: return "T_DIV";

	case T_ASSIGN: return "T_ASSIGN";
	case T_ASSIGN_ADD: return "T_ASSIGN_ADD";
	case T_ASSIGN_SUB: return "T_ASSIGN_SUB";
	case T_ASSIGN_MUL: return "T_ASSIGN_MUL";
	case T_ASSIGN_REM: return "T_ASSIGN_REM";
	case T_ASSIGN_DIV: return "T_ASSIGN_DIV";
	case T_ASSIGN_OR: return "T_ASSIGN_OR";
	case T_ASSIGN_AND: return "T_ASSIGN_AND";
	case T_ASSIGN_XOR: return "T_ASSIGN_XOR";
	case T_ASSIGN_SHL: return "T_ASSIGN_SHL";
	case T_ASSIGN_SHR: return "T_ASSIGN_SHR";

	case T_ARROW: return "TK_ARROW";
	case T_DARROW: return "TK_DARROW";

	case T_EQ: return "TK_EQ";
	case T_LT: return "TK_LT";
	case T_GT: return "TK_GT";
	case T_LTE: return "TK_LTE";
	case T_GTE: return "TK_GTE";
	case T_NEQ: return "TK_NEQ";


	case T_KW_LET: return "T_KW_LET";
	case T_KW_CONST: return "T_KW_CONST";
	case T_KW_FN: return "T_KW_FN";
	case T_KW_TYPE: return "T_KW_TYPE";
	case T_KW_BREAK: return "T_KW_BREAK";
	case T_KW_CONTINUE: return "T_KW_CONTINUE";
	case T_KW_RETURN: return "T_KW_RETURN";

	default: return "?unknown?";
	}
}