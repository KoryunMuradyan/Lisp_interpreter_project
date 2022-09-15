#ifndef __BUILT__IN__FUNCTIONS__HPP__
#define __BUILT__IN__FUNCTIONS__HPP__

#include <string>
#include <vector>
#include <set>
#include <map>

class Expr;
struct Functions;
std::string type_of(Expr&);
void defun(std::vector<Expr>&);
bool lisp_if(Expr&);
Expr setq(Expr&);
Expr print(Expr&);

#endif   //  __BUILT__IN__FUNCTIONS__HPP__
