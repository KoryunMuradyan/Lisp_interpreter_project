#ifndef __KEYS__HPP__
#define __KEYS__HPP__
#include "built_in_functions.hpp"
#include <map>
#include <set>

struct Function;

struct Functions
{
	std::map<std::string, Function> functions_map  =  {};
}functions;


struct keys
{
	std::set<std::string> math_op_set = {"+", "-", "/", "*", "<", ">", ">=", "<="};

	std::set<std::string> keywords_set = {"define", "defun", "set", "setf", "setq", "let", "eq", 
		"eql", "eval", "if", "do", "loop", "type-of", "find-method"};
}my_keys;


#endif  // __KEYS__HPP__
