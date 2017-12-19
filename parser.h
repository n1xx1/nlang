#pragma once

#include "common.h"
#include "lexer.h"
#include "ast.h"

namespace nlang {

class parser {
public:
	parser(const char* src, int w);

	std::unique_ptr<ast_file> parse();
	std::vector<std::unique_ptr<ast_stmt>> parse_stmt_list();
	std::unique_ptr<ast_stmt> parse_stmt_or_null();

	std::unique_ptr<ast_stmt_const> parse_stmt_const();
	std::unique_ptr<ast_stmt_let> parse_stmt_let();
	std::unique_ptr<ast_stmt_if> parse_stmt_if();
	std::unique_ptr<ast_stmt_for> parse_stmt_for();
	std::unique_ptr<ast_stmt_expr> parse_stmt_expr();

	std::unique_ptr<ast_expr> parse_expr();
	std::unique_ptr<ast_expr> parse_expr_binary(std::unique_ptr<ast_expr> x, int prec);
	std::unique_ptr<ast_expr> parse_expr_unary();
	std::unique_ptr<ast_expr> parse_expr_primary(bool keep_parens);
	std::unique_ptr<ast_expr> parse_expr_operand(bool keep_parens);
	std::unique_ptr<ast_expr_name> parse_expr_name();

private:
	position pos(int line = -1, int col = -1);
	bool got(token tk);
	void want(token tk);
	void advance(std::initializer_list<token> tks = {});
	void error(std::string msg, int line = -1, int col = -1);
	void syntax_error(std::string msg, int line = -1, int col = -1);

	lexer _lex;
};


}

