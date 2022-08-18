#ifndef __BUILT__IN__FUNCTIONS__HPP__
#define __BUILT__IN__FUNCTIONS__HPP__

#include "expr.hpp"
#include <string>
#include <vector>

class Expr;

struct keys;

struct Functions;

struct Function;



std::string type_of(Expr&);

void defun(std::vector<Expr>&);

/*
Expr let(std::vector<Expr>&);
*/

Expr setq(Expr&);

Expr print(Expr&);

#endif   //  __BUILT__IN__FUNCTIONS__HPP__
