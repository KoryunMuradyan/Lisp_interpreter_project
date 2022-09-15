#ifndef __FUNCTION__HPP__
#define __FUNCTION__HPP__



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

	void set_function_args(Expr& f_args) {
		args_number = f_args.get_args_vec().size();
		for (Expr& each_arg: f_args.get_args_vec()) {
			function_vars.insert(std::make_pair(each_arg.get_obj_value(), 
						Expr("")));
		}
	}

	void set_function_vars(Expr& arg_obj) {
		auto it = function_vars.find(arg_obj.get_obj_value());
		if (it == function_vars.end()) {
			function_vars.insert(std::make_pair(arg_obj.get_obj_value(),
						arg_obj));
		} else {
			it->second = arg_obj;
		}
	}

	void set_function_body(std::vector<Expr>::iterator& begin, 
			std::vector<Expr>::iterator& end) {
		while (begin != end + 1) {
			m_instructions.push_back(Expr(*begin));
		}
	}

	void set_function_instructions(std::vector<Expr>& instructions_vec) {
		for (auto each_instruction: instructions_vec) {
			m_instructions.push_back(Expr(each_instruction));
		}
	}
};

#endif // __FUNCTION__HPP__
