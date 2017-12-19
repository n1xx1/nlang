#pragma once

#include "common.h"

namespace nlang {


struct ast_node {
	position pos;
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

struct ast_stmt_simple : ast_stmt {
	virtual ~ast_stmt_simple() {}
};

struct ast_file : ast_node {
	std::vector<std::unique_ptr<ast_stmt>> stmts;
	virtual ~ast_file() {}
};

struct ast_stmt_list : ast_stmt {
	std::vector<std::unique_ptr<ast_stmt>> stmts;
	virtual ~ast_stmt_list() {}
};

struct ast_stmt_let : ast_stmt_simple {
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

struct ast_stmt_if : ast_stmt {
	std::unique_ptr<ast_stmt_simple> init;
	std::unique_ptr<ast_expr> cond;
	std::unique_ptr<ast_stmt_list> body;
	std::unique_ptr<ast_stmt> els;
	virtual ~ast_stmt_if() {}
};

struct ast_stmt_for : ast_stmt {
	std::unique_ptr<ast_stmt_simple> init;
	std::unique_ptr<ast_expr> cond;
	std::unique_ptr<ast_stmt_simple> incr; // can't be ast_stmt_let
	virtual ~ast_stmt_for() {}
};

struct ast_stmt_expr : ast_stmt_simple {
	std::unique_ptr<ast_expr> expr;
	virtual ~ast_stmt_expr() {}
};

}
