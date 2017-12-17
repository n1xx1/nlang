#include <iostream>
#include <cstdio>
#include <string>

#include "lexer.h"

const char TESTPROGRAM[] = R"TESTPROGRAM(
let test1 = example
)TESTPROGRAM";

int main() {
	nlang::lexer lex{ TESTPROGRAM, sizeof(TESTPROGRAM) - 1 };
	try {
		while(true) {
			lex.next();
			printf("%d:%d %s = '%s'\n", lex._tline, lex._tcol, nlang::token_to_string(lex._ttok).c_str(), lex._tlit.c_str());

			if(lex._ttok == nlang::T_EOF) break;
		}
	} catch(const std::exception& ex) {
		printf("Lexer Exception: %s\n", ex.what());
	}
	getchar();
	return 0;
}