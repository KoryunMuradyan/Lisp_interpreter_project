#include <iostream>
#include <vector>
#include <string>
#include "expr.hpp"
#include "functions.hpp"

typedef std::vector<std::string> token_vec;

int main()
{
	//std::string str1 = "(defun gcd(a b)(print a)) (print (+ 777 223 (- 300 400)))";
	//std::string str1 = "(defun gcd(a b)(print a)) (print (+ 777 223))";
	//std::string str1 = "(print (< 777 1223))";
	//std::string str1 = "(print (= 777 223))";
	//std::string str1 = "(print (= 777 777))";
	//std::string str1 = "(if (< 0 3)(print (= 777 777)) (print (> 222 400)))";
	//std::string str1 = "(print \"start\")(if (< 0 3)(print (= 777 777)) (print (< 222 400)))";
	std::string str1 = "(defun gcd(a b)(print a)) (print (+ 777 223 (- 300 400)))(gcd 2 3)";
	token_vec vec = tokenize_script(str1);
	std::vector<token_vec> arg_token_vec = split_script_into_modules(vec);
	std::vector<Expr> modules_vector = generate_modules_vector(arg_token_vec);
	interprate(std::move(modules_vector));
}
