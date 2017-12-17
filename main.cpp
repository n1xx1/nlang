#include <iostream>
#include <string>

#include "lexer.h"

const char* TESTPROGRAM = R"TESTPROGRAM(
let test1 /* example */ = 0
let test2 = 1 /* example */
let test3 = 2 // ok
let test4 = 3
)TESTPROGRAM";

int main() {
	nlang::lexer lex{ TESTPROGRAM, sizeof(TESTPROGRAM) - 1 };
	try {
		lex.next();
	} catch(const std::exception& ex) {
		std::cout << "Lexer Exception: " << ex.what() << std::endl;
	}
	getchar();
}