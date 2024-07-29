#include "lexer.h"
#include "parser.h"
#include <iostream>
using namespace calc::lexer;
using namespace calc::parser;
using namespace calc::ast;
using namespace calc::ast::cv;
int main() {
  for (;;) {
    char src[1024];
    fprintf(stderr, ">>> ");
    fflush(stderr);
    if (!fgets(src, 1023, stdin)) {

      break;
    }
    if (!strncmp(src, "exit", 4)) {
      return 0;
    }
    auto srcStr = std::string(src);
    Lexer lexer(srcStr);
    lexer.lex();
    if (!lexer.hadError) {
      std::cerr << "lexed successfully \n";
    }
    Parser parser = Parser(lexer.GetTokenList(), srcStr);
    auto expr = parser.parseTopLevel();
    if (!expr) {
      std::cerr << "\n";
      continue;
    }
    auto visitor = CodeVisitor();
    std::cerr << "  " << expr->eval(visitor);
    std::cerr << "\n";
  }
  return 0;
}
