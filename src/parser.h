#pragma once
#include <map>
#include <memory>
#include <utility>

#include "ast.h"
#include "lexer.h"

namespace calc {
namespace parser {
using namespace lexer;
using namespace calc::ast;
class Parser {
public:
  Parser(std::vector<struct Token> tokenList, std::string src);
  double eval();
  std::unique_ptr<Expr> parseTopLevel();

private:
  void logError(std::string errMsg);
  struct Token advance();
  struct Token peek();
  struct Token previous();
  int getPrecedence();
  bool consume(enum TokenKind tk, std::string errMsg);
  bool check(enum TokenKind expected);
  std::unique_ptr<Expr> parseUnary();
  std::unique_ptr<Expr> parsePrimaryExpr();
  std::unique_ptr<Expr> parseParen();
  std::unique_ptr<Expr> parseBinary(int prec, std::unique_ptr<Expr> lhs);
  std::unique_ptr<Expr> expression();

  std::map<enum TokenKind, int> precedenceMap_;
  std::vector<struct Token> tokenList_;
  size_t curPtr_;
  std::string src_;
}; // namespace parser
} // namespace parser
} // namespace calc
