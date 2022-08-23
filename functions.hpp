#ifndef __DECLARATIONS__HPP__
#define __DECLARATIONS__HPP__

#include <vector>
#include <string>

class Expr;

typedef std::vector<std::string> token_vec;

void interprate(std::vector<Expr>&&);

token_vec::iterator find_coobrace(token_vec::iterator&);
token_vec tokenize_script(const std::string&);
std::string read_script(const std::string&);
std::vector<token_vec> split_script_into_modules(token_vec&);
std::vector<Expr> generate_modules_vector(std::vector<token_vec >&);

float find_GCD(float, float);
float str_to_number(const std::string&);

bool is_number(const std::string&);
bool is_bool(const std::string&);
bool is_array(const std::string&);
bool is_string(const std::string&);
bool is_literal(const std::string&);
//std::vector<Expr> split_script_into_modules(token_vec&);

#endif //  __DECLARATIONS__HPP__
