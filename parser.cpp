#include "parser.h"

using namespace nlang;

parser::parser(const char* src, int w) :
	_lex(src, w) {}


std::unique_ptr<ast_file> parser::parse() {
	_lex.next();

	auto f = std::make_unique<ast_file>();
	f->pos_col = _lex._tcol;
	f->pos_line = _lex._tline;


	while(_lex._ttok != T_EOF) {
		switch(_lex._ttok) {
		case T_KW_CONST:
			_lex.next();
			f->stmts.push_back(parse_stmt_const());
			break;
		case T_KW_LET:
			_lex.next();
			f->stmts.push_back(parse_stmt_let());
			break;
		}

		if(_lex._ttok != T_EOF && !got(T_SEMI)) {
			syntax_error("after top level declaration");
			advance({ T_KW_CONST, T_KW_LET });
		}
	}
	return f;
}


// stmt_list = { stmt ";" } .
std::vector<std::unique_ptr<ast_stmt>> parser::parse_stmt_list() {
	std::vector<std::unique_ptr<ast_stmt>> list;
	while(_lex._ttok != T_EOF && _lex._ttok != T_RBRACE) {
		auto s = parse_stmt_or_null();
		if(!s) break;
		
		list.push_back(std::move(s));
		if(!got(T_SEMI) && _lex._ttok != T_RBRACE) {
			syntax_error("at end of statement");
			advance({ T_SEMI, T_RBRACE });
			got(T_SEMI);
		}
	}
	return list;
}

// stmt = 
std::unique_ptr<ast_stmt> parser::parse_stmt_or_null() {
	return nullptr;
}


// stmt_const = "const" identifier = expression
std::unique_ptr<ast_stmt_const> parser::parse_stmt_const() {
	auto n = std::make_unique<ast_stmt_const>();
	n->pos_col = _lex._tcol;
	n->pos_line = _lex._tline;

	n->name = std::move(parse_expr_name());
	want(T_ASSIGN);
	n->value = std::move(parse_expr());
	return n;
}

// stmt_let = "let" identifier = expression
std::unique_ptr<ast_stmt_let> parser::parse_stmt_let() {
	return nullptr;
}

// expr = binary_expr
std::unique_ptr<ast_expr> parser::parse_expr() {
	return parse_expr_binary(0);
}

static bool is_operator(token tk) {
	return T_LOR <= tk && tk <= T_SHR;
}
static int operator_prec(token tk) {
	if(tk == T_LOR) return 1;
	else if(tk == T_LAND) return 2;
	else if(T_EQ <= tk && tk <= T_NEQ) return 3;
	else if(T_ADD <= tk && tk <= T_XOR) return 4;
	else if(T_MUL <= tk && tk <= T_SHR) return 5;
	return 0;
}

// binary_expr = unary_expr | expr binop expr
std::unique_ptr<ast_expr> parser::parse_expr_binary(int prec) {
	auto x = parse_expr_unary();
	while(is_operator(_lex._ttok) && operator_prec(_lex._ttok) > prec) {
		auto t = std::make_unique<ast_expr_binop>();
		t->pos_col = _lex._tcol;
		t->pos_line = _lex._tline;
		t->op = _lex._ttok;
		t->x = std::move(x);

		int tprec = operator_prec(_lex._ttok);
		_lex.next();
		t->y = parse_expr_binary(tprec);
		x = std::move(t);
	}
	return x;
}

static std::unique_ptr<ast_expr> unparen(std::unique_ptr<ast_expr> x) {
	while(true) {
		auto p = dynamic_unique_cast<ast_expr_paren, ast_expr>(std::move(x));
		if(!p) break;
		x = std::move(p->x);
	}
	return x;
}

// unary_expr = primary_expr | unop unary_expr
std::unique_ptr<ast_expr> parser::parse_expr_unary() {
	switch(_lex._ttok) {
	case T_MUL: case T_ADD: case T_SUB: case T_NOT: case T_XOR: {
		auto x = std::make_unique<ast_expr_unop>();
		x->pos_col = _lex._tcol;
		x->pos_line = _lex._tline;
		x->op = _lex._ttok;
		_lex.next();
		x->x = parse_expr_unary();
		return std::move(x); }
	case T_AND: {
		auto x = std::make_unique<ast_expr_unop>();
		x->pos_col = _lex._tcol;
		x->pos_line = _lex._tline;
		x->op = _lex._ttok;
		_lex.next();
		x->x = unparen(parse_expr_unary());
		return std::move(x); }
	}
	return parse_expr_primary(true);
}

// expr_primary = expr_operand
//              | expr_primary "." identifier
//              | expr_primary "[" expr "]"
//              | expr_primary "[" expr ":" expr "]"
//              | expr_primary "(" expr_list ")"
std::unique_ptr<ast_expr> parser::parse_expr_primary(bool keep_parens) {
	auto x = parse_expr_operand(keep_parens);
	return x;
}

// expr_operand = int_lit | float_lit | string_lit
//              | identifier
//              | qualified_ident
//              | "(" expr ")"
std::unique_ptr<ast_expr> parser::parse_expr_operand(bool keep_parens) {
	switch(_lex._ttok) {
	case T_IDENT:
		return parse_expr_name();
	case T_INT: case T_FLOAT: case T_STRING: case T_CHAR: {
		auto l = std::make_unique<ast_expr_literal>();
		l->pos_line = _lex._tline;
		l->pos_col = _lex._tcol;
		l->kind = _lex._ttok;
		l->val = _lex._tlit;
		_lex.next();
		return move(l); }
	case T_LPAREN: {
		int bkline = _lex._tline;
		int bkcol = _lex._tcol;
		_lex.next();
		auto x = parse_expr();
		want(T_RPAREN);

		if(keep_parens) {
			auto p = std::make_unique<ast_expr_paren>();
			p->pos_line = bkline;
			p->pos_col = bkcol;
			p->x = move(x);
			return move(p);
		}
		return x; }
	default:
		syntax_error("expecting expression");
		return nullptr;
	}
}

// identifier 
std::unique_ptr<ast_expr_name> parser::parse_expr_name() {
	if(_lex._ttok == T_IDENT) {
		auto n = std::make_unique<ast_expr_name>();
		n->pos_line = _lex._tline;
		n->pos_col = _lex._tcol;
		n->value = _lex._tlit;
		_lex.next();
		return n;
	}
	syntax_error("expecting name");
	return nullptr;
}

bool parser::got(token tk) {
	if(_lex._ttok == tk) {
		_lex.next();
		return true;
	}
	return false;
}

void parser::want(token tk) {
	if(!got(tk)) {
		syntax_error("expecting " + token_to_string(tk));
		advance();
	}
}

void parser::advance(std::initializer_list<token> tks) {
	if(tks.size() == 0) {
		_lex.next();
		return;
	}

	while(true) {
		bool found = false;
		for(auto tk : tks)
			if(tk == _lex._ttok) {
				found = true;
				break;
			}
		if(found) break;

		_lex.next();
	}
}

void parser::error(std::string msg, int line, int col) {
	throw new std::runtime_error(msg + " at " + std::to_string(line) + ":" + std::to_string(col));
}

static bool has_prefix(const std::string& in, const std::string& prefix) {
	return 0 == in.compare(0, prefix.size(), prefix);
}

void parser::syntax_error(std::string msg, int line, int col) {
	if(has_prefix(msg, "in") || has_prefix(msg, "at") || has_prefix(msg, "after")) {
		msg = " " + msg;
	} else if(has_prefix(msg, "expecting")) {
		msg = ", " + msg;
	} else {
		error("syntax error: " + msg);
		return;
	}
	error("syntax error: unexpected " + token_to_string(_lex._ttok) + msg);
}
