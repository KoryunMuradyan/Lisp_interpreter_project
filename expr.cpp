#include <algorithm>
#include "expr.hpp"
#include "functions.hpp"
#include <map>
#include <set>

extern std::map<std::string, Expr> declared_vars;
extern std::set<std::string> math_op_set;
extern std::set<std::string> keywords_set;
#include "Function.hpp"

Expr::Expr(std::vector<Expr>& arg_vec)
	:m_expr_vector(arg_vec.begin(), arg_vec.end())
{}

Expr::Expr(Expr& other_obj)
	:m_expr_vector(other_obj.get_args_vec())
	,m_value(other_obj.get_obj_value())
	,m_this_obj_type(other_obj.get_obj_type())
	,m_var_type(other_obj.get_var_type())
	,m_var_value(other_obj.get_var_value())
{}

Expr::Expr(Expr&& other_obj)
	:m_expr_vector(other_obj.get_args_vec())
	,m_value(other_obj.get_obj_value())
	,m_this_obj_type(other_obj.get_obj_type())
	,m_var_type(other_obj.get_var_type())
	,m_var_value(other_obj.get_var_value())
{}

Expr::Expr(token_vec::iterator& begin, token_vec::iterator& end):
	m_this_obj_type(list)
{
	define_obj_vector(++begin, --end);
}

Expr::Expr(const std::string& arg_str):
	m_value(arg_str)
{
	define_type(arg_str);
	set_var_type(arg_str);
	set_var_value(arg_str);
}

Expr Expr::operator= (Expr right_obj) {
	this->m_value = right_obj.get_obj_value();
	this->m_var_value = right_obj.get_var_value();
	this->m_this_obj_type = right_obj.get_obj_type();
	this->m_expr_vector = right_obj.get_args_vec();
	this->m_var_value = right_obj.get_var_value();
	this->m_var_type = right_obj.get_var_type();
	return *this;
}

int Expr::get_obj_type() {
	return m_this_obj_type;
}

std::string Expr::get_var_type() {
	return m_var_type;
}

std::string Expr::get_obj_value() {
	return m_value;
}

std::string Expr::get_var_value() {
	return m_var_value;
}

std::vector<Expr> Expr::get_args_vec() {
	return std::vector<Expr>(m_expr_vector.begin(), m_expr_vector.end());
}

void Expr::arithmetic_solve(Expr& tmp_var, std::string& tmp_value, std::vector<Expr>& operands_vec) {
	if ("+" == tmp_value) {
		tmp_var = tmp_var + operands_vec;
	} else if ("-" == tmp_value) {
		tmp_var = tmp_var - operands_vec;
	} else if ("*" == tmp_value) {
		tmp_var = tmp_var * operands_vec;
	} else if ("/" == tmp_value) {
		tmp_var = tmp_var / operands_vec;
	} else {
		Expr tmp("#t");
		tmp.set_var_type("#t");
		tmp.set_var_value("#t");
		tmp_var = tmp;
		if ("<" == tmp_value) {
			tmp_var = tmp_var < operands_vec;
		} else if (">" == tmp_value) {
			tmp_var = tmp_var > operands_vec;
		} else if ("<=" == tmp_value) {
			tmp_var = tmp_var <= operands_vec;
		} else if (">=" == tmp_value) {
			tmp_var = tmp_var >= operands_vec;
		} else if ("=" == tmp_value) {
			tmp_var = (tmp_var == operands_vec);
		}
	}
}

Expr Expr::resolve() {
	if (is_literal(m_value)) {
		set_var_type(m_value);
		set_var_value(m_value);
		return *this;
	} else if (identifier == m_this_obj_type) {
		auto it = declared_vars.find(m_value); 
		if (it != declared_vars.end()) {
			return it->second;
		} else {
			std::cerr << "not declared variable!" << std::endl;
			return Expr();
		}
	}
	std::string tmp_value = m_expr_vector[0].get_obj_value();
	std::vector<Expr> operands_vec(m_expr_vector.begin() + 1, 
						m_expr_vector.end());
	Expr tmp_var("");
	arithmetic_solve(tmp_var, tmp_value, operands_vec);
	return tmp_var;
}

void Expr::set_var_type(const std::string& arg_str) {
	if (is_number(arg_str)) {
		auto it = find(arg_str.begin(), arg_str.end(), '.');
		if (it == arg_str.end()) {
			m_var_type = "int";
		} else {
			m_var_type = "float";
		}
	} else if (is_string(arg_str)) {
		m_var_type = "string";
	} else if (is_array(arg_str)) {
		m_var_type = "array";
	} else if (is_bool(arg_str)) {
		m_var_type = "bool";
	} 
}

void Expr::set_string(const std::string& arg_str) {
	auto begin = arg_str.begin() + 1;
	auto end = arg_str.end() - 1;
	if (is_string(arg_str)) {
		m_var_value = std::string(begin, end);
	}
}

void Expr::set_var_value(const std::string& arg_str) {
	if (is_array(arg_str)) {
		set_array(arg_str);
	} else if (is_string(arg_str)) {
		set_string(arg_str);
	} else if (is_number(arg_str) || is_bool(arg_str)) {
		m_var_value = arg_str;
	}
}

void Expr::set_array(const std::string& arg_str) {
	auto begin = arg_str.begin() + 2;
	auto end = arg_str.end() - 1;
	if (is_array(arg_str)) {
		std::vector<std::string> str_vec = tokenize_script(std::string(begin, end));
		for (auto str: str_vec) {
			m_expr_vector.push_back(Expr(str));
		}
	}
}

void Expr::set_obj_value(const std::string& arg_str) {
	m_value = arg_str;
}

void Expr::define_type(const std::string& arg_value) {
	if (-1 == m_this_obj_type) {
		if (is_literal(arg_value)) {
			m_this_obj_type = literal;
		} else {
			m_this_obj_type = identifier;
		}
	}
}

void Expr::define_obj_vector(token_vec::iterator& begin, token_vec::iterator& end) {
	token_vec::iterator current = begin;
	while(true) {
		if (current == end) {
			if (")" != *current) {
				m_expr_vector.push_back(Expr(*current));
			}
			return;
		}
		if (")" == *current) {
			++current;
			continue;
		}
		if ("(" == *current) {
			token_vec::iterator op_brc_it = current;
			token_vec::iterator co_brc_it = find_coobrace(current);
			m_expr_vector.push_back(Expr(op_brc_it, co_brc_it));
			current = ++co_brc_it;
			continue;
		}
		m_expr_vector.push_back(Expr(*current));
		++current;
	}
	
}

Expr Expr::operator+(Expr right_obj) {
	std::vector<Expr> operands = right_obj.get_args_vec();
	float tmp_num = 0;
	for (auto& obj: operands) {
			Expr tmp_obj = obj.resolve();
			auto tmp = obj.get_obj_type();
		tmp_num += str_to_number(tmp_obj.get_obj_value());
	}
	if (float(tmp_num) == int(tmp_num)) {
	this->set_var_value(std::to_string(int(tmp_num)));
	this->m_value = std::to_string(int(tmp_num));
		this->set_var_type("int");
	} else {
	this->set_var_value(std::to_string(tmp_num));
	this->m_value = std::to_string(tmp_num);
		this->set_var_type("float");
	}
	return *this;
}

Expr Expr::operator-(Expr right_obj) {
	std::vector<Expr> operands = right_obj.get_args_vec();
	float tmp_num = 0;
	if (1 != operands.size()) {
		tmp_num = 2*str_to_number(operands[0].get_obj_value());
	}
	for (auto& obj: operands) {
			Expr tmp_obj = obj.resolve();
			auto tmp = obj.get_obj_type();
		tmp_num -= str_to_number(tmp_obj.get_obj_value());
	}
	if (float(tmp_num) == int(tmp_num)) {
	this->set_var_value(std::to_string(int(tmp_num)));
	this->m_value = std::to_string(int(tmp_num));
		this->set_var_type("int");
	} else {
	this->set_var_value(std::to_string(tmp_num));
	this->m_value = std::to_string(tmp_num);
		this->set_var_type("float");
	}
	return *this;
}

Expr Expr::operator*(Expr right_obj) {
	std::vector<Expr> operands = right_obj.get_args_vec();
	float tmp_num = 1;
	for (auto& obj: operands) {
			Expr tmp_obj = obj.resolve();
			auto tmp = obj.get_obj_type();
		tmp_num *= str_to_number(tmp_obj.get_obj_value());
	}
	if (float(tmp_num) == int(tmp_num)) {
	this->set_var_value(std::to_string(int(tmp_num)));
	this->m_value = std::to_string(int(tmp_num));
		this->set_var_type("int");
	} else {
	this->set_var_value(std::to_string(tmp_num));
	this->m_value = std::to_string(tmp_num);
		this->set_var_type("float");
	}
	return *this;
}

Expr Expr::operator/(Expr right_obj) {
	std::vector<Expr> operands = right_obj.get_args_vec();
	float tmp_num = 1;
	for (auto& obj: operands) {
			Expr tmp_obj = obj.resolve();
			auto tmp = obj.get_obj_type();
		tmp_num /= str_to_number(tmp_obj.get_obj_value());
	}
	if (float(tmp_num) == int(tmp_num)) {
	this->set_var_value(std::to_string(int(tmp_num)));
	this->m_value = std::to_string(int(tmp_num));
		this->set_var_type("int");
	} else {
	this->set_var_value(std::to_string(tmp_num));
	this->m_value = std::to_string(tmp_num);
		this->set_var_type("float");
	}
	return *this;
}

void set_parameters(Expr* this_obj) {
	this_obj->set_obj_value("#f");
	this_obj->set_var_value("#f");
}

Expr Expr::operator<(Expr right_obj) {
	std::vector<Expr> operands = right_obj.get_args_vec();
	std::string str_1, str_2 = operands[1].resolve().get_obj_value();
	for (auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if (is_number(str_1) || is_number(str_1)) {
			if (is_number(str_2) && is_number(str_1)) {
				auto number_1 = str_to_number(str_1);
				auto number_2 = str_to_number(str_2);
				if (number_1 >= number_2) {set_parameters(this);} 
				else {continue;};
				return *this;
			}
		} else if (is_string(str_1) || is_string(str_2)) {
			if (is_string(str_2) && is_string(str_1)) {
				if (str_1 >= str_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		}
		str_1 = str_2;
	}
	return *this;
}

Expr Expr::operator>(Expr right_obj) {
	std::vector<Expr> operands = right_obj.get_args_vec();
	std::string str_1, str_2 = operands[1].resolve().get_obj_value();
	for (auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if (is_number(str_1) || is_number(str_1)) {
			if (is_number(str_2) && is_number(str_1)) {
				auto number_1 = str_to_number(str_1);
				auto number_2 = str_to_number(str_2);
				if (number_1 <= number_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		} else if (is_string(str_1) || is_string(str_2)) {
			if (is_string(str_2) && is_string(str_1)) {
				if (str_1 <= str_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		}
		str_1 = str_2;
	}
	return *this;
}

Expr Expr::operator<=(Expr right_obj) {
	std::vector<Expr> operands = right_obj.get_args_vec();
	std::string str_1, str_2 = operands[1].resolve().get_obj_value();
	for (auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if (is_number(str_1) || is_number(str_1)) {
			if (is_number(str_2) && is_number(str_1)) {
				auto number_1 = str_to_number(str_1);
				auto number_2 = str_to_number(str_2);
				if (number_1 > number_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		} else if (is_string(str_1) || is_string(str_2)) {
			if (is_string(str_2) && is_string(str_1)) {
				if (str_1 > str_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		}
		str_1 = str_2;
	}
	return *this;
}

Expr Expr::operator>=(Expr right_obj) {
	std::vector<Expr> operands = right_obj.get_args_vec();
	std::string str_1, str_2 = operands[1].resolve().get_obj_value();
	for (auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if (is_number(str_1) || is_number(str_1)) {
			if (is_number(str_2) && is_number(str_1)) {
				auto number_1 = str_to_number(str_1);
				auto number_2 = str_to_number(str_2);
				if (number_1 < number_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		} else if (is_string(str_1) || is_string(str_2)) {
			if (is_string(str_2) && is_string(str_1)) {
				if (str_1 < str_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		}
		str_1 = str_2;
	}
	return *this;
}

Expr Expr::operator== (Expr right_obj) {
	std::vector<Expr> operands = right_obj.get_args_vec();
	std::string str_1 = operands[0].resolve().get_obj_value(), str_2;
	for (auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if (is_number(str_1) || is_number(str_1)) {
			if (is_number(str_2) && is_number(str_1)) {
				auto number_1 = str_to_number(str_1);
				auto number_2 = str_to_number(str_2);
				if (number_1 < number_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		} else if (is_string(str_1) || is_string(str_2)) {
			if (is_string(str_2) && is_string(str_1)) {
				if (str_1 != str_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		} else if (is_array(str_1) || is_array(str_2)) {
			if (is_array(str_2) && is_array(str_1)) {
				if (str_1 != str_2) {set_parameters(this);} 
				else {continue;}
				return *this;
			}
		} 
		str_1 = str_2;
	}
	return *this;
}

Expr Expr::And(std::vector<Expr>& arg_obj_vac) {
	for (auto obj: arg_obj_vac) {
		Expr tmp_obj = obj.resolve();
		if (is_bool(tmp_obj.get_obj_value())) {
			if ("#f" == obj.get_obj_value()) {
				return Expr("#f");
			}
		}
	}	
	return Expr("#t");
}

Expr Expr::Or(std::vector<Expr>& arg_obj_vac) {
	for (auto obj: arg_obj_vac) {
		Expr tmp_obj = obj.resolve();
		if (is_bool(tmp_obj.get_obj_value())) {
			if ("#t" == tmp_obj.get_obj_value()) {
				return Expr("#t");
			}
		}
	}	
	return Expr("#f");
}

Expr Expr::Not(Expr& arg_obj) {
	Expr tmp_obj = arg_obj.resolve();
	if (is_bool(tmp_obj.get_obj_value())) {
		if ("#t" == tmp_obj.get_obj_value()) {
			return(Expr("#f"));
		} else {
			return(Expr("#t"));
		}
	}
	return(Expr());
}
