#include "keys.hpp"
#include "built_in_functions.hpp"
#include "functions.hpp"
#include <set>
#include <map>
#include <iostream>
#include <string>
#include <sstream> 

/*
*/

struct Function
{
	std::string function_name = "";
	int args_number = 0;
	std::map<std::string, Expr> function_vars = {};
	std::vector<Expr> m_instructions = {};

	Function()
	{}

	Function(std::string arg_name)
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

	void run_function(std::vector<Expr> function_arguments)
	{
		auto it_begin = m_instructions.begin();
		while(it_begin != m_instructions.end())
		{
			auto tmp_vec = it_begin->get_args_vec();
			auto tmp_value = tmp_vec[0].get_obj_value();
			if (my_keys.keywords_set.find(tmp_value) != my_keys.keywords_set.end()) {
				m_instructions.push_back(it_begin->resolve());
			}
			if ("setq" == tmp_value) {
				auto it = function_vars.find(tmp_vec[1].get_obj_value());
				if (it == function_vars.find(tmp_vec[1].get_obj_value())) {
					function_vars.insert(std::make_pair(tmp_vec[1].get_obj_value(), setq(tmp_vec[2])));
				} else {
					it->second = setq(tmp_vec[2]);
					continue;
				}
			} else if ("print" == tmp_value) {
				print(tmp_vec[1]);
				continue;
			}
			/*
			std::vector<Expr> operands_vec(tmp_vec.begin() + 1, tmp_vec.end());
			Expr tmp_var("");
			*/
			if (my_keys.math_op_set.find(tmp_value) != my_keys.math_op_set.end()) {
				Expr tmp_obj(it_begin->resolve());
				tmp_obj.set_var_value(tmp_obj.get_var_value());
				tmp_obj.set_var_type(tmp_obj.get_var_value());
			} else if (functions.functions_map.end() != functions.functions_map.find(tmp_value)) {
				auto it = functions.functions_map.find(tmp_value);
				auto it_2 = tmp_vec.begin() + 1;
				it->second.run_function(std::vector<Expr>(it_2, tmp_vec.end()));
			}
		}
	}
};

void defun(std::vector<Expr>& arg_obj_vac)
{
	Function foo;
	foo = Function(arg_obj_vac[0].get_obj_value());
	foo.set_function_args(arg_obj_vac[1]);
	std::vector<Expr> instructions_vec = arg_obj_vac[2].get_args_vec();
	foo.set_function_instructions(instructions_vec);
	/*
	for(Expr& obj: arg_obj_vac) {
		if("setf" == obj.get_obj_value()) {
			foo.set_function_vars()   //   (write ((lambda (a b c x)(print 77777))4 2 9 3))
		}
	}
	*/
	//Functions.functions_map.insert_or_assign(arg_obj_vac[0].get_obj_value(), foo);
	auto it = functions.functions_map.find(arg_obj_vac[0].get_obj_value());
	if (it == functions.functions_map.end()) {

		//functions.functions_map.insert(std::make_pair(function_name, foo));

	} else {
		std::cerr << "redefinition of  '"  << arg_obj_vac[0].get_obj_value() << "'  function" << std::endl;
	}
	/*
	   Function foo;
	   if("" != arg_obj_vac[0].get_obj_value() && 1 != arg_obj_vac[0].get_obj_type()) {
	   foo = Function(arg_obj_vac[0].get_obj_value());
	   foo.set_function_args(arg_obj_vac[1]);
	   } else {
	   for(Expr& obj: arg_obj_vac) {
	   set_function_vars(obj);
	   }
	   foo.set_function_vars(arg_obj_vac[0])
	   }
	   for(Expr& obj: arg_obj_vac) {
	   if("setf" == obj.get_obj_value()) {
	   foo.set_function_vars()   //   (write ((lambda (a b c x)(print 77777))4 2 9 3))
	   }
	   }
	   Functions.functions_map.insert_or_assign(arg_obj_vac[0].get_obj_value(), foo);
	   */
}



std::string type_of(Expr& arg_obj)
{	
	arg_obj = arg_obj.resolve();
	arg_obj.set_var_type(arg_obj.get_var_value());
	return(arg_obj.get_var_type());
}

/*
Expr let(std::vector<Expr>& arg_obj_vac)
{
	
}
*/

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

