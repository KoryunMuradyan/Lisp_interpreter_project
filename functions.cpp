
#include "keys.hpp"
#include "expr.hpp"
#include "functions.hpp"
#include "built_in_functions.hpp"

#include <algorithm>
#include <sstream>
#include <fstream>
#include <iterator>     // back_inserter
#include <string>
#include "vector"


////d/////////////////(print (let ((c 1)) (let* ((c 2) (a (+ c 1))) a)))

typedef std::vector<std::string> token_vec;

void interprate(std::vector<Expr>& modules_vector)
{
	auto it_begin = modules_vector.begin();
	while(it_begin != modules_vector.end())
	{
		auto tmp_vec = it_begin->get_args_vec();
		auto tmp_value = tmp_vec[0].get_obj_value();
		if(my_keys.keywords_set.find(tmp_value) == my_keys.keywords_set.end) {
			run_built_in_foo()
		}
		if ("setq" == tmp_value) {
			auto it = function_vars.find(tmp_vec[1].get_obj_value());
			function_vars.insert_or_assign(tmp_vec[1].get_obj_value(), setq(tmp_vec[2]));
			continue;
		} else if ("print" == tmp_value) {
			print(tmp_vec[1]);
			continue;
		} else if ("defun" == tmp_value) {
			defun(std::vector<Expr>(tmp_vec.begin() + 1, tmp_vec.end()));
		}
		/*
		   std::vector<Expr> operands_vec(tmp_vec.begin() + 1, tmp_vec.end());
		   Expr tmp_var("");
		   */
		if (my_keys.math_op_set.find(tmp_value) != my_keys.math_op_set.end()) {
			Expr tmp_obj(it_begin->resolve);
		} else if (functions.functions_map.end() != functions.functions_map.find(tmp_value)) {
			auto it = functions.functions_map.find(tmp_value);
			it->second.run_function(std::vector<Expr>(it->begin() + 1, it->end()));
		}
	}
}

std::string read_script(const std::string& filename) 
{
	std::ifstream file_to_read(filename);
	std::string str_file = "";
	std::string tmp_str = "";
	while(std::getline(file_to_read, tmp_str)){
		str_file += tmp_str;
	}
	return str_file;
}

std::vector<Expr> generate_modules_vector(std::vector<token_vec >& obj_vec)
{	
	std::vector<Expr> modules_vector = {};
	for(auto module: obj_vec)
	{
		auto it_begin = module.begin();
		auto it_end = module.end();
		modules_vector.push_back(Expr(it_begin, --it_end));
	}
	return modules_vector;
}

std::vector<token_vec> split_script_into_modules(token_vec& arg_token_vec)
{
	token_vec::iterator op_brc_it = arg_token_vec.begin(), co_brc_it;
	co_brc_it = find_coobrace(op_brc_it);
	std::vector<token_vec> modules_vector = {};
	while(op_brc_it != arg_token_vec.end()) {
		co_brc_it = find_coobrace(op_brc_it);
		token_vec tmp_vec(op_brc_it, co_brc_it + 1);
		modules_vector.push_back(tmp_vec);
		++co_brc_it;
		op_brc_it = co_brc_it;
	}
	return modules_vector;
}

token_vec::iterator find_coobrace(token_vec::iterator& begin)
{
	int counter = 0;
	token_vec::iterator current = begin;
	while(true)
	{
		if("(" == *current) {
			++counter;
		} else if(")" == *current) {
			--counter;
		}
		if(0 == counter){
			return current;
		}
		++current;
	}
}

float find_GCD(float arg_num_1, float arg_num_2)
{
	if (arg_num_1 == arg_num_2) {
		return arg_num_1;
	if (arg_num_1 > arg_num_2)
		return find_GCD(arg_num_1-arg_num_2,arg_num_2);
	return find_GCD(arg_num_1, arg_num_2-arg_num_1);
	}
	if (arg_num_1 > arg_num_2) {
		return find_GCD(arg_num_1-arg_num_2,arg_num_2);
	}
	return find_GCD(arg_num_1, arg_num_2-arg_num_1);
}

float str_to_number(const std::string& numeric_str) 
{
	std::stringstream ss; 
	float num;
	ss << numeric_str;
	ss >> num;
}

bool is_number(const std::string& arg_str)
{
	for(char ch: arg_str)
	{
		if(ch < '0' || ch > '9') {
			if('.' != ch) {
				return false;
			}
		}
	}
	return true;
}

bool is_bool(const std::string& arg_str)
{
	return("#f" == arg_str || "#t" == arg_str);
}

bool is_array(const std::string& arg_str)
{
	return('\'' == arg_str[0]);
}

bool is_string(const std::string& arg_str)
{
	return('"' == arg_str[0]);
}

bool is_literal(const std::string& arg_str)
{
	return((is_number(arg_str)) || (is_bool(arg_str))
			|| (is_array(arg_str)) || (is_string(arg_str)));
}
