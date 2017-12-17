#pragma once

#include "common.h"

namespace nlang {


struct ast_node {
	int pos_line;
	int pos_col;
	virtual ~ast_node() {}
};

struct ast_expr : ast_node {
	virtual ~ast_expr() {}
};

struct ast_expr_name : ast_expr {
	std::string value;
	virtual ~ast_expr_name() {}
};

struct ast_expr_binop : ast_expr {
	token op;
	std::unique_ptr<ast_expr> x;
	std::unique_ptr<ast_expr> y;
	virtual ~ast_expr_binop() {}
};

struct ast_expr_unop : ast_expr {
	token op;
	std::unique_ptr<ast_expr> x;
	virtual ~ast_expr_unop() {}
};

struct ast_expr_paren : ast_expr {
	std::unique_ptr<ast_expr> x;
	virtual ~ast_expr_paren() {}
};

struct ast_expr_literal : ast_expr {
	token kind;
	std::string val;
	virtual ~ast_expr_literal() {}
};

struct ast_stmt : ast_node {
	virtual ~ast_stmt() {}
};

struct ast_file : ast_node {
	std::vector<std::unique_ptr<ast_stmt>> stmts;
	virtual ~ast_file() {}
};


struct ast_stmt_let : ast_stmt {
	std::unique_ptr<ast_expr_name> name;
	std::unique_ptr<ast_expr> type;
	std::unique_ptr<ast_expr> value;
	virtual ~ast_stmt_let() {}
};

struct ast_stmt_const : ast_stmt {
	std::unique_ptr<ast_expr_name> name;
	std::unique_ptr<ast_expr> type;
	std::unique_ptr<ast_expr> value;
	virtual ~ast_stmt_const() {}
};

}
