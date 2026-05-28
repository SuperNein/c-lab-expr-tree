#ifndef POLY_H
#define POLY_H

#include "expr_tree.h"


ExprTree poly_expand(const ExprTree tree);

ExprTree poly_simplify_constants(const ExprTree tree);
ExprTree poly_simplify_algebraic(const ExprTree tree);

ExprTree poly_normalize(const ExprTree tree);

#endif
