#include "lexer.h"

namespace calc {
namespace lexer {

bool Lexer::atEnd() { return curPos_ >= src_.size(); }

char Lexer::advance() {
  curPos_++;
  return src_[curPos_ - 1];
}

char Lexer::peek() {
  if (atEnd())
    return '\0';

  return src_[curPos_];
}

char Lexer::peekNext() {
  if (curPos_ + 1 >= src_.size())
    return '\0';

  return src_[curPos_ + 1];
}

void Lexer::lex() {
  while (!atEnd()) {
    lexOneToken();
  }
  addToken(Token(TOKEN_EOF, "eof", 0, 0, std::nullopt));
}

void Lexer::lexOneToken() {
  if (!currTokenError_ && !prevTokenError_)
    startPos_ = curPos_;

  char c = advance();
  switch (c) {
  case '+':
    currTokenError_ = false;
    if (prevTokenError_ && !currTokenError_) {

      curPos_--;
      addToken(logError("unknown token encountered in stream"));
      prevTokenError_ = currTokenError_ = false;

      startPos_ = curPos_;
    }
    addToken(Token(TOKEN_PLUS, "+", startPos_, startPos_, std::nullopt));
    currTokenKind_ = TOKEN_PLUS;
    break;
  case '-':
    currTokenError_ = false;
    if (prevTokenError_ && !currTokenError_) {
      curPos_--;
      addToken(logError("unknown token encountered in stream"));
      prevTokenError_ = currTokenError_ = false;

      startPos_ = curPos_;
    }
    addToken(Token(TOKEN_MINUS, "-", startPos_, startPos_, std::nullopt));
    currTokenKind_ = TOKEN_MINUS;
    break;
  case '*':
    currTokenError_ = false;
    if (prevTokenError_ && !currTokenError_) {
      curPos_--;
      addToken(logError("unknown token encountered in stream"));
      prevTokenError_ = currTokenError_ = false;
      startPos_ = curPos_;
    }
    addToken(Token(TOKEN_STAR, "*", startPos_, startPos_, std::nullopt));
    currTokenKind_ = TOKEN_STAR;
    break;
  case '/':
    currTokenError_ = false;
    if (prevTokenError_ && !currTokenError_) {
      curPos_--;
      addToken(logError("unknown token encountered in stream"));
      prevTokenError_ = currTokenError_ = false;
      startPos_ = curPos_;
    }
    addToken(Token(TOKEN_SLASH, "/", startPos_, startPos_, std::nullopt));
    currTokenKind_ = TOKEN_SLASH;
    break;
  case ' ':
  case '\t':
    break;
  default:
    if (std::isdigit(c)) {
      currTokenError_ = false;
      if (prevTokenError_ && !currTokenError_) {
        curPos_--;
        addToken(logError("unknown token encountered in stream"));
        prevTokenError_ = currTokenError_ = false;
        startPos_ = curPos_;
      }
      addToken(lexNumber());
      break;
    }

    if ((prevTokenError_ && !currTokenError_) || (prevTokenError_ && atEnd())) {
      addToken(logError("unknown token encountered in stream"));
    }
    prevTokenError_ = currTokenError_ = true;
    hadError = true;

    break;
  }
}

struct Token Lexer::lexNumber() {
  while (std::isdigit(peek()))
    advance();
  if (peek() == '.' && std::isdigit(peekNext())) {
    // consume the decimal
    advance();
    while (std::isdigit(peek()))
      advance();
  }

  std::string contents = src_.substr(startPos_, curPos_);
  double value = std::stod(contents);

  return Token(TOKEN_NUM, contents, startPos_, curPos_,
               std::optional<double>{value});
}

struct Token Lexer::logError(std::string errorMsg) {
  std::string err = "\033[31;1merror: \033[0m";
  err.append(errorMsg);
  fprintf(stderr, "%s\n", err.c_str());
  fprintf(stderr, "| %s", src_.c_str());
  fflush(stderr);
  size_t x = 0;
  std::cerr << "  ";
  while (x++ < startPos_) {
    std::cerr << " ";
  }
  std::cerr << "^";
  x++;
  while (x++ < curPos_) {
    std::cerr << "~";
  }
  std::cerr << "\n";
  return Token(TOKEN_ERROR, "", startPos_, curPos_, std::nullopt);
}

void Lexer::addToken(struct Token token) { tokenList_.push_back(token); }

} // namespace lexer
} // namespace calc
