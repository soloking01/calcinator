#pragma once
#include "code_visitor.h"
#include "lexer.h"
#include <memory>
using namespace calc::lexer;

namespace calc {
namespace ast {
using namespace cv;
class Expr {
public:
  virtual ~Expr() = default;
  virtual double eval(CodeVisitor &) = 0;
};

class LiteralExpr : public Expr {
public:
  struct Token literalContent_;

  LiteralExpr(struct Token tok) : literalContent_(tok){};
  double eval(CodeVisitor &) override;
};
class UnaryExpr : public Expr {
public:
  struct Token unaryOp_;
  std::unique_ptr<Expr> rhs_;

  UnaryExpr(struct Token unaryOp, std::unique_ptr<Expr> rhs)
      : unaryOp_(unaryOp), rhs_(std::move(rhs)){};
  double eval(CodeVisitor &) override;
};

class BinaryExpr : public Expr {
public:
  struct Token binaryOp_;
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;

  BinaryExpr(struct Token binaryOp, std::unique_ptr<Expr> lhs,
             std::unique_ptr<Expr> rhs)
      : binaryOp_(binaryOp), lhs_(std::move(lhs)), rhs_(std::move(rhs)){};
  double eval(CodeVisitor &) override;
};
} // namespace ast
} // namespace calc
