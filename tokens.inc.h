#ifndef TOKEN
#define TOKEN(name, text) 
#endif
#ifndef TOKEN_OP
#define TOKEN_OP(name, text, prec) TOKEN(name, text)
#endif
#ifndef TOKEN_KW
#define TOKEN_KW(name, text) TOKEN(KW_ ## name, text)
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

TOKEN(INC, "++")
TOKEN(DEC, "--")

TOKEN(ARROW, "->")
TOKEN(DARROW, "=>")
TOKEN(NOT, "!")

TOKEN_OP( LOR, "||", 1)
TOKEN_OP(LAND, "&&", 2)
TOKEN_OP(  EQ, "==", 3)
TOKEN_OP(  LT,  "<", 3)
TOKEN_OP(  GT,  ">", 3)
TOKEN_OP( LTE, "<=", 3)
TOKEN_OP( GTE, ">=", 3)
TOKEN_OP( NEQ, "!=", 3)
TOKEN_OP( ADD,  "+", 4)
TOKEN_OP( SUB,  "-", 4)
TOKEN_OP(  OR,  "|", 4)
TOKEN_OP( XOR,  "^", 4)
TOKEN_OP( MUL,  "*", 5)
TOKEN_OP( DIV,  "/", 5)
TOKEN_OP( REM,  "%", 5)
TOKEN_OP( AND,  "&", 5)
TOKEN_OP( SHL, "<<", 5)
TOKEN_OP( SHR, ">>", 5)

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
TOKEN_KW(BREAK, "break")
TOKEN_KW(CONTINUE, "continue")
TOKEN_KW(RETURN, "return")

#undef TOKEN
#undef TOKEN_OP
#undef TOKEN_KW