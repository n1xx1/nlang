#ifndef TOKEN
#define TOKEN(name, text) 
#endif
#ifndef TOKEN_OP
#define TOKEN_OP(name, text, prec, lassoc) TOKEN(name, text)
#endif
#ifndef TOKEN_KW
#define TOKEN_KW(name, text) TOKEN(name, text)
#endif

TOKEN(EOF, "<eof>")

TOKEN(IDENT, "ident")
TOKEN(STRING, "string")
TOKEN(CHAR, "char")
TOKEN(INT, "int")
TOKEN(FLOAT, "float")

TOKEN(LBRACE, "{")
TOKEN(RBRACE, "}")
TOKEN(LBRACK, "[")
TOKEN(RBRACK, "]")
TOKEN(LPAREN, "(")
TOKEN(RPAREN, ")")
TOKEN(SEMI, ";")
TOKEN(DOT, ".")
TOKEN(DOTDOTDOT, "...")
TOKEN(COMMA, ",")
TOKEN(COLON, ":")

//TOKEN(ARROW, "->")
//TOKEN(DARROW, "=>")

TOKEN(NOT, "!")
TOKEN(INC, "++")
TOKEN(DEC, "--")
TOKEN_OP( LOR, "||",  4, 1)
TOKEN_OP(LAND, "&&",  5, 1)
TOKEN_OP(  EQ, "==",  6, 1)
TOKEN_OP(  LT,  "<",  6, 1)
TOKEN_OP(  GT,  ">",  6, 1)
TOKEN_OP( LTE, "<=",  6, 1)
TOKEN_OP( GTE, ">=",  6, 1)
TOKEN_OP( NEQ, "!=",  6, 1)
TOKEN_OP(  OR,  "|",  7, 1)
TOKEN_OP( XOR,  "^",  8, 1)
TOKEN_OP( AND,  "&",  9, 1)
TOKEN_OP( SHL, "<<", 10, 1)
TOKEN_OP( SHR, ">>", 10, 1)
TOKEN_OP( ADD,  "+", 11, 1)
TOKEN_OP( SUB,  "-", 11, 1)
TOKEN_OP( MUL,  "*", 12, 1)
TOKEN_OP( DIV,  "/", 12, 1)
TOKEN_OP( REM,  "%", 12, 1)
TOKEN_OP(  AS, "as", 13, 0)

TOKEN(ASSIGN, "=")
TOKEN(ASSIGN_ADD, "+=")
TOKEN(ASSIGN_SUB, "-=")
TOKEN(ASSIGN_MUL, "*=")
TOKEN(ASSIGN_REM, "%=")
TOKEN(ASSIGN_DIV, "/=")
TOKEN(ASSIGN_OR, "|=")
TOKEN(ASSIGN_AND, "&=")
TOKEN(ASSIGN_XOR, "^=")
TOKEN(ASSIGN_SHL, "<<=")
TOKEN(ASSIGN_SHR, ">>=")

TOKEN_KW(LET, "let")
TOKEN_KW(CONST, "const")
TOKEN_KW(FN, "func")
TOKEN_KW(TYPE, "type")
TOKEN_KW(IF, "if")
TOKEN_KW(ELSE, "else")
TOKEN_KW(FOR, "for")
TOKEN_KW(BREAK, "break")
TOKEN_KW(CONTINUE, "continue")
TOKEN_KW(RETURN, "return")

#undef TOKEN
#undef TOKEN_OP
#undef TOKEN_KW