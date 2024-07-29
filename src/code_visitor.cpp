#include "code_visitor.h"
#include "ast.h"
#include "lexer.h"

namespace calc {

namespace ast {
namespace cv {
double CodeVisitor::visitLiteral(LiteralExpr *astNode) {
  return astNode->literalContent_.tokenValue_.value();
}
double CodeVisitor::visitUnaryExpr(UnaryExpr *astNode) {
  double value = astNode->rhs_->eval(*this);
  if (astNode->unaryOp_.tk_ == calc::lexer::TOKEN_MINUS)
    return -value;
  return value;
}

double CodeVisitor::visitBinaryExpr(BinaryExpr *astNode) {
  double lhs = astNode->lhs_->eval(*this);
  double rhs = astNode->rhs_->eval(*this);
  switch (astNode->binaryOp_.tk_) {
  case calc::lexer::TOKEN_MINUS:
    return lhs - rhs;

  case calc::lexer::TOKEN_PLUS:
    return lhs + rhs;

  case calc::lexer::TOKEN_STAR:
    return lhs * rhs;

  case calc::lexer::TOKEN_SLASH:
    return lhs / rhs;
  default:
    break;
  }
  return -69.6969;
}
} // namespace cv
} // namespace ast
} // namespace calc
