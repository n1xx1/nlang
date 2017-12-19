#include <iostream>
#include <cstdio>

#include "parser.h"

const char TESTPROGRAM[] = R"TESTPROGRAM(
    // constant (type infer)
    const const1 = 1
    // typed constant
    const const2 = 1 as i32
    // variable (type infer)
    let var1 = 1
    // typed variable
    let var2 = 1 as i32
)TESTPROGRAM";


template<typename Class, typename Arg>
Arg arg_type(void(Class::*)(Arg&) const) { return Arg(); }

template<typename Class, typename Arg>
Arg arg_type(void(Class::*)(Arg&)) { return Arg(); }

template <class Func>
bool visit_ast_node_type(nlang::ast_node* n, Func f) {
	using To = decltype(arg_type(&Func::operator()));
	if(To* cast = dynamic_cast<To*>(n)) {
		f(*cast);
		return true;
	}
	return false;
}

template <class Func, class... Funcs>
bool visit_ast_node(nlang::ast_node* n, Func f, Funcs... fs) {
	return visit_ast_node_type(n, f) || visit_ast_node(n, fs...);
}

template <class Func>
bool visit_ast_node(nlang::ast_node* n, Func f) {
	return visit_ast_node_type(n, f);
}

void print_walk(nlang::ast_node* n, int i) {
	bool ret = visit_ast_node(n, 
		[=](nlang::ast_file& f) {
			printf("%*sast_file\n", 2 * i, "");
			for(auto& stmt : f.stmts) print_walk(stmt.get(), i + 1);
		},
		[=](nlang::ast_stmt_let& s) {
			printf("%*sast_stmt_let\n", 2 * i, "");
			print_walk(s.name.get(), i + 1);
			print_walk(s.type.get(), i + 1);
			print_walk(s.value.get(), i + 1);
		},
		[=](nlang::ast_stmt_const& s) {
			printf("%*sast_stmt_const\n", 2 * i, "");
			print_walk(s.name.get(), i + 1);
			print_walk(s.type.get(), i + 1);
			print_walk(s.value.get(), i + 1);
		},
		[=](nlang::ast_expr_name& s) {
			printf("%*sast_expr_name value:%s\n", 2 * i, "", s.value.c_str());
		},
		[=](nlang::ast_expr_binop& s) {
			printf("%*sast_expr_binop op:%s\n", 2 * i, "", nlang::token_to_string(s.op).c_str());
			print_walk(s.x.get(), i + 1);
			print_walk(s.y.get(), i + 1);
		},
		[=](nlang::ast_expr_unop& s) {
			printf("%*sast_expr_unop op:%s\n", 2 * i, "", nlang::token_to_string(s.op).c_str());
			print_walk(s.x.get(), i + 1);
		},
		[=](nlang::ast_expr_paren& s) {
			printf("%*sast_expr_paren\n", 2 * i, "");
			print_walk(s.x.get(), i + 1);
		},
		[=](nlang::ast_expr_literal& s) {
			printf("%*sast_expr_literal kind:%s, value:%s\n", 2 * i, "", nlang::token_to_string(s.kind).c_str(), s.val.c_str());
		}
	);
	if(!ret) printf("%*s%s\n", 2 * i, "", n == nullptr ? "nullptr" : "unknown node");
}

int main() {
	nlang::parser p(TESTPROGRAM, sizeof(TESTPROGRAM) - 1);
	try {
		auto f = p.parse();
		print_walk(f.get(), 0);
	} catch(const std::exception* ex) {
		printf("Lexer Exception: %s\n", ex->what());
	}
	getchar();
	return 0;
}