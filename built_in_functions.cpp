#include <set>
#include <map>
#include "expr.hpp"
#include "expr.hpp"
#include "functions.hpp"
#include "built_in_functions.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <sstream> 

struct Function;
extern std::map<std::string, Function> functions_map;
extern std::map<std::string, Expr> declared_vars;
extern std::set<std::string> math_op_set;
extern std::set<std::string> keywords_set;

struct Function
{
	std::string function_name = "";
	int args_number = 0;
	std::map<std::string, Expr> function_vars = {};
	std::vector<Expr> m_instructions = {};

	Function()
	{}

	Function(const std::string& arg_name)
		:function_name(arg_name)
	{}

	void set_function_args(Expr& f_args)
	{
		args_number = f_args.get_args_vec().size();
		for(Expr& each_arg: f_args.get_args_vec()) {
			function_vars.insert(std::make_pair(each_arg.get_obj_value(), Expr("")));
		}
	}

	void set_function_vars(Expr& arg_obj)
	{
		auto it = function_vars.find(arg_obj.get_obj_value());
		if (it == function_vars.end()) {
			function_vars.insert(std::make_pair(arg_obj.get_obj_value(), arg_obj));
		} else {
			it->second = arg_obj;
		}
	}

	void set_function_body(std::vector<Expr>::iterator& begin, std::vector<Expr>::iterator& end)
	{
		while(begin != end + 1) {
			m_instructions.push_back(Expr(*begin));
		}
	}

	void set_function_instructions(std::vector<Expr>& instructions_vec)
	{
		for(auto each_instruction: instructions_vec)
		{
			m_instructions.push_back(Expr(each_instruction));
		}
	}
};

std::map<std::string, Function> functions_map  =  {};
std::set<std::string> math_op_set = {"+", "-", "/", "*", "<", ">", ">=", "<="};
std::set<std::string> keywords_set = {"defun", "setq", "let", "print", "if", "loop", "type-of"};
std::map<std::string, Expr> declared_vars = {};

Expr redefine_identifier(Expr& arg_obj, std::map<std::string, Expr>& vars)
{
	auto it = vars.find(arg_obj.get_obj_value());
	if (it != vars.end())
	{
		arg_obj = it->second;
	}
}

Expr redefine(Expr& arg_obj, std::map<std::string, Expr>& vars)
{
	auto vec = arg_obj.get_args_vec();
	for (auto& each_obj: vec)
	{
		redefine_identifier(each_obj, vars);
	}
	return arg_obj;
}

void run_function(std::vector<Expr> function_arguments, Function& foo)
{
	for(auto each_arg: function_arguments)
	{
		foo.set_function_vars(each_arg);
	}
	auto it_begin = foo.m_instructions.begin();
	while(it_begin != foo.m_instructions.end())
	{
		auto tmp_vec = it_begin->get_args_vec();
		auto tmp_value = tmp_vec[0].get_obj_value();
		if ("setq" == tmp_value) {
			auto it = foo.function_vars.find(tmp_vec[1].get_obj_value());
			if (it == foo.function_vars.find(tmp_vec[1].get_obj_value())) {
				foo.function_vars.insert(std::make_pair(tmp_vec[1].get_obj_value(), setq(tmp_vec[2])));
			} else {
				it->second = setq(tmp_vec[2]);
				continue;
			}
		} else if ("print" == tmp_value) {
			if (list == tmp_vec[1].get_obj_type()) {
				tmp_vec[1] = redefine(tmp_vec[1], foo.function_vars);
			} else {

				tmp_vec[1] = redefine_identifier(tmp_vec[1], foo.function_vars);
			}
			print(tmp_vec[1]);
			++it_begin;
			continue;
		}
		if (math_op_set.find(tmp_value) != math_op_set.end()) {
			Expr tmp_obj(it_begin->resolve());
			tmp_obj.set_var_value(tmp_obj.get_var_value());
			tmp_obj.set_var_type(tmp_obj.get_var_value());
		} else if (functions_map.end() != functions_map.find(tmp_value)) {
			auto it = functions_map.find(tmp_value);
			auto it_2 = tmp_vec.begin() + 1;
			run_function(std::vector<Expr>(it_2, tmp_vec.end()), it->second);
		}
	}
}

void call_built_in_functions(std::string& tmp_value, std::vector<Expr>& tmp_vec)
{
	if ("if" == tmp_value) {
		std::vector<Expr> new_vec = {};
		if (lisp_if(tmp_vec[1])) {
			new_vec.push_back(std::move(tmp_vec[2]));
		} else {
			new_vec.push_back(std::move(tmp_vec[3]));
		}
		interprate(std::move(new_vec));
	} else if ("setq" == tmp_value) {
		declared_vars.insert_or_assign(tmp_vec[1].get_obj_value(), 
				setq(tmp_vec[2]));
	} else if ("print" == tmp_value) {
		print(tmp_vec[1]);
	} else if ("defun" == tmp_value) {
		auto it =  tmp_vec.begin() + 1;
		auto tmp_vector = std::vector<Expr>(it, tmp_vec.end());
		defun(tmp_vector);
	}
}

void interprate(std::vector<Expr>&& modules_vector)
{
	auto it_begin = modules_vector.begin();
	while(it_begin != modules_vector.end())
	{
		std::vector<Expr> tmp_vec = it_begin->get_args_vec();
		std::string tmp_value = tmp_vec[0].get_obj_value();
		if (keywords_set.find(tmp_value) != keywords_set.end()) {
			call_built_in_functions(tmp_value, tmp_vec);
			++it_begin;
			continue;
		}
		if (math_op_set.find(tmp_value) != math_op_set.end()) {
			Expr tmp_obj(it_begin->resolve());
		} else if (functions_map.end() != functions_map.find(tmp_value)) {
			auto it = functions_map.find(tmp_value);
			auto it_2 = tmp_vec.begin() + 1;
			run_function(std::vector<Expr>(it_2, tmp_vec.end()), it->second);
		}
		++it_begin;
	}
}

void defun(std::vector<Expr>& arg_obj_vac)
{
	Function foo(arg_obj_vac[0].get_obj_value());
	foo.set_function_args(arg_obj_vac[1]);
	std::vector<Expr> instructions_vec(arg_obj_vac.begin() + 2, arg_obj_vac.end());
	foo.set_function_instructions(instructions_vec);
	auto it = functions_map.find(arg_obj_vac[0].get_obj_value());
	std::string function_name = "";
	function_name = arg_obj_vac[0].get_obj_value();
	if (it != functions_map.end()) {
		std::cerr << "redefinition of  '"  << arg_obj_vac[0].get_obj_value() << "'  function" << std::endl;
		return;
	}
	functions_map.insert_or_assign(std::move(function_name), std::move(foo));
}

std::string type_of(Expr& arg_obj)
{	
	arg_obj = arg_obj.resolve();
	arg_obj.set_var_type(arg_obj.get_var_value());
	return(arg_obj.get_var_type());
}

Expr setq(Expr& rightside)
{
	Expr obj_to_return(rightside.resolve());
	obj_to_return.set_var_type(obj_to_return.get_var_value());
	obj_to_return.set_var_value(obj_to_return.get_var_value());
	return Expr(obj_to_return);
}

Expr print(Expr& arg_obj)
{
	Expr tmp_obj = arg_obj.resolve();
	tmp_obj.set_var_type(tmp_obj.get_var_value());
	if("array" == tmp_obj.get_var_type()) {
		std::cout << tmp_obj.get_obj_value() << std::endl;
	} else if("string" == tmp_obj.get_var_type()) {
		std::cout << tmp_obj.get_var_value() << std::endl;
	} else {
		std::cout << tmp_obj.get_obj_value() << std::endl;
	}
	return Expr();
}

bool lisp_if(Expr& lisp_bool)
{
	lisp_bool.resolve();
	lisp_bool.set_var_value(lisp_bool.get_obj_value());
	return("#t" == lisp_bool.get_var_value());
}
