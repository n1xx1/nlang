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

int main() {
	nlang::parser p(TESTPROGRAM, sizeof(TESTPROGRAM) - 1);
	try {
		auto f = p.parse();
	} catch(const std::exception* ex) {
		printf("Lexer Exception: %s\n", ex->what());
	}
	getchar();
	return 0;
}