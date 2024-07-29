#pragma once
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace calc {
namespace lexer {

enum TokenKind {

  // operators
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_SLASH,
  TOKEN_STAR,

  // number
  TOKEN_NUM,
  TOKEN_ERROR,
  TOKEN_EOF,
};

struct Token {
  enum TokenKind tk_;
  std::string tokenContent_;
  size_t tokenStartPos_;
  size_t tokenEndPos_;
  std::optional<double> tokenValue_;

  Token(enum TokenKind tk, std::string tokenContent, size_t tokenStartPos,
        size_t tokenEndPos, std::optional<double> tokenValue)
      : tk_(tk), tokenContent_(tokenContent), tokenStartPos_(tokenStartPos),
        tokenEndPos_(tokenEndPos), tokenValue_(tokenValue) {}

  void dump() {
    fprintf(stderr, "Content: %s, startPos: %zu, currPos: %zu, value: %lf\n",
            tokenContent_.c_str(), tokenStartPos_, tokenEndPos_,
            tokenValue_ ? tokenValue_.value() : -69.69);
  }
};

struct Diagnostic {
  std::string message_;
  struct Token errorToken_;

  Diagnostic(std::string message, struct Token errorToken)
      : message_(message), errorToken_(errorToken) {}
};

class Lexer {
public:
  Lexer(std::string src)
      : hadError(false), src_(src), startPos_(0), curPos_(0),
        prevTokenError_(false), currTokenError_(false) {}
  void lex();

  std::vector<struct Diagnostic> GetDiagnosticList() { return diagnostics_; }
  std::vector<struct Token> GetTokenList() { return tokenList_; }
  bool hadError;

private:
  void lexOneToken();
  bool atEnd();
  char advance();
  char peek();
  void addToken(struct Token token);
  char peekNext();
  struct Token lexNumber();
  struct Token logError(std::string errorMsg);

  std::string src_;
  size_t startPos_;
  size_t curPos_;
  bool prevTokenError_;
  bool currTokenError_;
  std::vector<struct Diagnostic> diagnostics_;
  std::vector<struct Token> tokenList_;
  enum TokenKind currTokenKind_;

}; // namespace lexer
} // namespace lexer
} // namespace calc
