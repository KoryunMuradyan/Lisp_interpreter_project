#include <iostream>
#include <vector>
#include <string>
#include "expr.hpp"
#include "functions.hpp"

typedef std::vector<std::string> token_vec;

int main(int argc, char** argv)
{
	std::string script_str = read_script(argv[1]);
	token_vec vec = tokenize_script(script_str);
	std::vector<token_vec> arg_token_vec = split_script_into_modules(vec);
	std::vector<Expr> modules_vector = generate_modules_vector(arg_token_vec);
	interprate(std::move(modules_vector));
}
