#include "ast.h"

namespace calc {
namespace ast {
using namespace cv;

double LiteralExpr::eval(CodeVisitor &visitor) {
  return visitor.visitLiteral(const_cast<calc::ast::LiteralExpr *>(this));
}

double BinaryExpr::eval(CodeVisitor &visitor) {
  return visitor.visitBinaryExpr(const_cast<calc::ast::BinaryExpr *>(this));
}

double UnaryExpr::eval(CodeVisitor &visitor) {
  return visitor.visitUnaryExpr(const_cast<calc::ast::UnaryExpr *>(this));
}
} // namespace ast
} // namespace calc
