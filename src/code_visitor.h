#pragma once

namespace calc {
namespace ast {
class UnaryExpr;
class BinaryExpr;
class LiteralExpr;
namespace cv {

class CodeVisitor {
public:
  CodeVisitor() noexcept = default;
  double visitUnaryExpr(calc::ast::UnaryExpr *);
  double visitBinaryExpr(calc::ast::BinaryExpr *);
  double visitLiteral(calc::ast::LiteralExpr *);
};
} // namespace cv
} // namespace ast
} // namespace calc
