#include "parser.h"

namespace calc {
namespace parser {
using namespace calc::lexer;
Parser::Parser(std::vector<struct Token> tokenList, std::string src) {
  tokenList_ = tokenList;
  curPtr_ = 0;
  src_ = src;
  precedenceMap_[TOKEN_NUM] = 0;
  precedenceMap_[TOKEN_PLUS] = 10;
  precedenceMap_[TOKEN_MINUS] = 10;
  precedenceMap_[TOKEN_STAR] = 20;
  precedenceMap_[TOKEN_SLASH] = 20;
}

static bool isOperator(enum TokenKind tt) {
  return tt == TOKEN_PLUS || tt == TOKEN_MINUS || tt == TOKEN_STAR ||
         tt == TOKEN_SLASH;
}

bool Parser::check(enum TokenKind tk) { return peek().tk_ == tk; }

bool Parser::consume(enum TokenKind tk, std::string errMsg) {
  if (check(tk)) {
    advance();
    return true;
  }
  logError(errMsg);
  return false;
}

struct Token Parser::advance() {
  curPtr_++;
  return previous();
}

struct Token Parser::peek() {
  return tokenList_[curPtr_];
}

struct Token Parser::previous() {
  return tokenList_[curPtr_ - 1];
}

int Parser::getPrecedence() {
  if (!isOperator(peek().tk_))
    return -1;
  int prec = precedenceMap_[peek().tk_];
  if (prec <= 0)
    return -1;
  return prec;
}

std::unique_ptr<Expr> Parser::expression() {
  auto lhs = parseUnary();
  if (!lhs)
    return nullptr;
  return parseBinary(0, std::move(lhs));
}

std::unique_ptr<Expr> Parser::parsePrimaryExpr() {
  switch (peek().tk_) {
  default:
    logError("This token is unexpected. Did you place it by mistake?");
    return nullptr;
  case TOKEN_LPAREN:
    return parseParen();
  case TOKEN_NUM: {
    advance();
    return std::make_unique<LiteralExpr>(previous());
  }
  }
}

std::unique_ptr<Expr> Parser::parseTopLevel() { return expression(); }

std::unique_ptr<Expr> Parser::parseParen() {
  advance();
  auto result = expression();

  if (!consume(TOKEN_RPAREN, "expect ')' to close a grouped expression"))
    return nullptr;

  if (!result) {
    logError("illegal expression inside parentheses");
    return nullptr;
  }
  return result;
}

std::unique_ptr<Expr> Parser::parseUnary() {

  enum TokenKind tt = peek().tk_;
  if (!isOperator(tt) || tt == TOKEN_LPAREN) {
    return parsePrimaryExpr();
  }
  advance();
  auto op = previous();
  if (auto rhs = parseUnary()) {
    return std::make_unique<UnaryExpr>(op, std::move(rhs));
  }
  logError("expected expression");
  return nullptr;
}

std::unique_ptr<Expr> Parser::parseBinary(int minPrec,
                                          std::unique_ptr<Expr> lhs) {

  while (true) {
    int curTokPrec = getPrecedence();
    if (curTokPrec < minPrec)
      return lhs;

    auto op = peek();
    if (!isOperator(op.tk_))
      return nullptr;
    advance();

    auto rhs = parseUnary();
    if (!rhs) {
      return nullptr;
    }
    int nextTokPrec = getPrecedence();
    if (curTokPrec < nextTokPrec) {
      rhs = parseBinary(curTokPrec + 1, std::move(rhs));
      if (!rhs) {
        logError("expected expression");
        return nullptr;
      }
    }
    lhs = std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
  }
}

void Parser::logError(std::string errorMsg) {
  std::string err = "\n\033[31;1merror: \033[0m";
  err.append(errorMsg);
  fprintf(stderr, "%s\n", err.c_str());
  fprintf(stderr, "| %s", src_.c_str());
  fflush(stderr);
  size_t x = 0;
  std::cerr << "| ";
  while (x++ < peek().tokenStartPos_) {
    std::cerr << " ";
  }
  std::cerr << "^";
  x++;
  while (x++ <= peek().tokenEndPos_) {
    std::cerr << "~";
  }
}
} // namespace parser
} // namespace calc
