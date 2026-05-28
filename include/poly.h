#ifndef POLY_H
#define POLY_H

#include "expr_tree.h"


ExprTree poly_expand(const ExprTree tree);
ExprTree poly_simplify(const ExprTree tree);

#endif
