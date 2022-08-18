#include "expr.hpp"
#include <algorithm>

/*
Expr::Expr(Expr& other_obj)
{
	m_expr_vector = (other_obj.get_args_vec());
	m_value = (other_obj.get_obj_value());
	m_this_obj_type = (other_obj.get_obj_type());
	m_var_type = (other_obj.get_var_type());
	m_var_value = (other_obj.get_var_value());
}
*/

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

/*
Expr::Expr(const Expr& other_obj)
	:m_expr_vector(other_obj.get_args_vec())
	,m_value(other_obj.get_obj_value())
	,m_this_obj_type(other_obj.get_obj_type())
	,m_var_type(other_obj.get_var_type())
	,m_var_value(other_obj.get_var_value())
{}
*/

Expr::Expr(token_vec::iterator& begin, token_vec::iterator& end):
	m_this_obj_type(list)
{
	++obj_num;
	define_obj_vector(++begin, --end);
	/*
	   std::cout << m_expr_vector.size();
	   std::cout << std::endl;
	   std::cout << this->m_this_obj_type  << "           "  << this->m_value << "               "  <<  this->obj_num << std::endl;
	   */
	/*
	   for(auto obj: m_expr_vector)
	   {
	   std::cout << obj.m_this_obj_type  << "   "  << obj.m_value << "   "  <<  obj_num << std::endl;
	   }
	   */
}

Expr::Expr(const std::string& arg_str):
	m_value(arg_str)
{
	++obj_num;
	define_type(arg_str);
	/*
	   std::cout << this->m_this_obj_type  << "           "  << this->m_value << "               "  <<  this->obj_num << std::endl;
	   std::cout << m_expr_vector.size();
	   std::cout << std::endl;
	   */
}

int Expr::get_obj_type()
{
	return m_this_obj_type;
}

std::string Expr::get_var_type()
{
	return m_var_type;
}

std::string Expr::get_obj_value()
{
	return m_value;
}

std::string Expr::get_var_value()
{
	return m_var_value;
}

std::vector<Expr> Expr::get_args_vec()
{
	return std::vector<Expr>(m_expr_vector.begin(), m_expr_vector.end());
}

Expr Expr::resolve()
{
	std::string tmp_value = m_expr_vector[0].get_obj_value();
	std::vector<Expr> operands_vec(m_expr_vector.begin() + 1, m_expr_vector.end());
	Expr tmp_var("");
	if("+" == tmp_value){
		tmp_var = tmp_var + operands_vec;
	} else if("-" == tmp_value) {
		tmp_var = tmp_var - operands_vec;
	} else if("*" == tmp_value) {
		tmp_var = tmp_var * operands_vec;
	} else if("/" == tmp_value) {
		tmp_var = tmp_var / operands_vec;
	} else if("<" == tmp_value) {
		tmp_var = tmp_var < operands_vec;
	} else if(">" == tmp_value) {
		tmp_var = tmp_var > operands_vec;
	} else if("<=" == tmp_value) {
		tmp_var = tmp_var <= operands_vec;
	} else if(">=" == tmp_value) {
		tmp_var = tmp_var >= operands_vec;
	} else if("=" == tmp_value) {
		tmp_var = (tmp_var = operands_vec);
	}
	return tmp_var;
}

void Expr::set_var_type(const std::string& arg_str)
{
	if(is_number(arg_str)) {
		auto it = find(arg_str.begin(), arg_str.end(), '.');
		if(it == arg_str.end()) {
			m_var_type = "int";
		} else {
			m_var_type = "float";
		}
	} else if(is_string(arg_str)) {
		m_var_type = "string";
	} else if(is_array(arg_str)) {
		m_var_type = "array";
	} else if(is_bool(arg_str)) {
		m_var_type = "bool";
	} 
}

void Expr::set_string(const std::string& arg_str)
{
	auto begin = arg_str.begin() + 1;
	auto end = arg_str.end() - 1;
	if(is_string(arg_str)) {
		m_var_value = std::string(begin, end);
	}
}

void Expr::set_var_value(const std::string& arg_str)
{
	if(is_array(arg_str)) {
		set_array(arg_str);
	} else if(is_string(arg_str)) {
		set_string(arg_str);
	} else if(is_number(arg_str) || is_bool(arg_str)) {
		m_var_value = arg_str;
	}
}

void Expr::set_array(const std::string& arg_str)
{
	auto begin = arg_str.begin() + 2;
	auto end = arg_str.end() - 1;
	if(is_array(arg_str)) {
		std::vector<std::string> str_vec = tokenize_script(std::string(begin, end));
		for(auto str: str_vec) {
			m_expr_vector.push_back(Expr(str));
		}
	}
}

void Expr::define_type(const std::string& arg_value)
{
	if(-1 == m_this_obj_type) {
		if(is_literal(arg_value)) {
			m_this_obj_type = literal;
		} else {
			m_this_obj_type = identifier;
		}
	}
}

void Expr::define_obj_vector(token_vec::iterator& begin, token_vec::iterator& end)
{
	token_vec::iterator current = begin;
	while(true) {
		if(current == end) {
			if(")" != *current) {
				m_expr_vector.push_back(Expr(*current));
			}
			return;
		}
		if(")" == *current) {
			++current;
			continue;
		}
		if("(" == *current) {
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

Expr Expr::operator+(Expr right_obj)
{
	std::vector<Expr> operands = right_obj.get_args_vec();
	float tmp_num = 0;
	for(auto& obj: operands) {
		if(list == obj.get_obj_type()) {
			Expr obj = obj.resolve();
		} // else if(obj.get_obj_value())
		tmp_num += str_to_number(obj.get_obj_value());
	}
	Expr obj_to_return(std::to_string(tmp_num));
	if(float(tmp_num) == int(tmp_num)) {
		obj_to_return.set_var_type("int");
	} else {
		obj_to_return.set_var_type("float");
	}
	obj_to_return.set_var_value(std::to_string(tmp_num));
	return(obj_to_return);
}

Expr Expr::operator-(Expr right_obj)
{
	std::vector<Expr> operands = right_obj.get_args_vec();
	float tmp_num = 0;
	for(auto& obj: operands) {
		obj = obj.resolve();
		tmp_num -= str_to_number(obj.get_obj_value());
	}
	Expr obj_to_return(std::to_string(tmp_num));
	if(float(tmp_num) == int(tmp_num)) {
		obj_to_return.set_var_type("int");
	} else {
		obj_to_return.set_var_type("float");
	}
	obj_to_return.set_var_value(std::to_string(tmp_num));
	return(obj_to_return);
}

Expr Expr::operator*(Expr right_obj)
{
	std::vector<Expr> operands = right_obj.get_args_vec();
	float tmp_num = 1;
	for(auto& obj: operands) {
		obj = obj.resolve();
		tmp_num *= str_to_number(obj.get_obj_value());
	}
	Expr obj_to_return(std::to_string(tmp_num));
	if(float(tmp_num) == int(tmp_num)) {
		obj_to_return.set_var_type("int");
	} else {
		obj_to_return.set_var_type("float");
	}
	obj_to_return.set_var_value(std::to_string(tmp_num));
	return(obj_to_return);
}

Expr Expr::operator/(Expr right_obj)
{
	std::vector<Expr> operands = right_obj.get_args_vec();
	auto tmp_obj = operands[0].resolve();
	float tmp_num = str_to_number(tmp_obj.get_obj_value());
	for(auto& obj: operands) {
		obj = obj.resolve();
		tmp_num /= str_to_number(obj.get_obj_value());
	}
	Expr obj_to_return(std::to_string(tmp_num));
	if(float(tmp_num) == int(tmp_num)) {
		obj_to_return.set_var_type("int");
	} else {
		obj_to_return.set_var_type("float");
	}
	obj_to_return.set_var_value(std::to_string(tmp_num));
	return(obj_to_return);
}

Expr Expr::operator<(Expr right_obj)
{
	std::vector<Expr> operands = right_obj.get_args_vec();
	std::string str_1, str_2 = operands[1].resolve().get_obj_value();
	for(auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if(is_number(str_1) || is_number(str_1)) {
			if(is_number(str_2) && is_number(str_1)) {
				auto number_1 = str_to_number(str_1);
				auto number_2 = str_to_number(str_2);
				if (str_1 <= str_2) {
					return Expr("#f");
				} else {
					continue;
				}
			}
		} else if(is_string(str_1) || is_string(str_2)) {
			if(is_string(str_2) && is_string(str_1)) {
				if (str_1 >= str_2) {
					return Expr("#f");
				} else {
					continue;
				}
			}
		}
		str_1 = str_2;
	}
	return(Expr("#t"));
}

Expr Expr::operator>(Expr right_obj)
{
	std::vector<Expr> operands = right_obj.get_args_vec();
	/*
	Expr *tmp_obj_1 = new Expr(arg_obj_1.resolve() < arg_obj_2.resolve());
	std::string bool_str_1 = tmp_obj_1->get_obj_value();
	tmp_obj_1 = &Expr(arg_obj_1.resolve() = arg_obj_2.resolve());
	std::string bool_str_2 = tmp_obj_1->get_obj_value();
	delete tmp_obj_1;
	if("#f" == bool_str_1 && "#f" == bool_str_2) {
		return Expr("#t");
	} else {
		return("#f");
	}
	return Expr();
	*/
	std::string str_1, str_2 = operands[1].resolve().get_obj_value();
	for(auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if(str_1 < str_2 || str_1 != str_2){
			return Expr("#f");
		}
		str_1 = str_2;
	}
	return Expr("#t");
}

Expr Expr::operator<=(Expr right_obj)
{
	std::vector<Expr> operands = right_obj.get_args_vec();
	std::string str_1 = operands[0].resolve().get_obj_value(), str_2;
	for(auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if(str_1 > str_2){
			return Expr("#f");
		}
		str_1 = str_2;
	}
	return Expr("#t");
}

Expr Expr::operator>=(Expr right_obj)
{
	std::vector<Expr> operands = right_obj.get_args_vec();
	std::string str_1 = operands[0].resolve().get_obj_value(), str_2;
	for(auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if(str_1 < str_2){
			return Expr("#f");
		}
		str_1 = str_2;
	}
	return Expr("#t");
}

Expr Expr::operator=(Expr right_obj)
{
	std::vector<Expr> operands = right_obj.get_args_vec();
	std::string str_1 = operands[0].resolve().get_obj_value(), str_2;
	for(auto& each_obj: operands) {
		str_2 = each_obj.resolve().get_obj_value();
		if(is_number(str_1) || is_number(str_1)) {
			if(is_number(str_2) && is_number(str_1)) {
				auto number_1 = str_to_number(str_1);
				auto number_2 = str_to_number(str_2);
				if (number_1!=number_2) {
					return Expr("#f");
				} else {
					continue;
				}
			}
		} else if(is_string(str_1) || is_string(str_2)) {
			if(is_string(str_2) && is_string(str_1)) {
				if (str_1 != str_2) {
					return Expr("#f");
				} else {
					continue;
				}
			}
		} else if(is_array(str_1) || is_array(str_2)) {
			if(is_array(str_2) && is_array(str_1)) {
				if (str_1 != str_2) {
					return Expr("#f");
				} else {
					continue;
				}
			}
		} 
		str_1 = str_2;
	}
	return(Expr("t"));
}

/*
Expr MOD(Expr& arg_obj_1, Expr& arg_obj_2)
{
	
}
*/

Expr Expr::And(std::vector<Expr>& arg_obj_vac)
{
	for(auto obj: arg_obj_vac){
		Expr tmp_obj = obj.resolve();
		if(is_bool(tmp_obj.get_obj_value())) {
			if("#f" == obj.get_obj_value()) {
				return Expr("#f");
			}
		}
	}	
	return Expr("#t");
}

Expr Expr::Or(std::vector<Expr>& arg_obj_vac)
{
	for(auto obj: arg_obj_vac) {
		Expr tmp_obj = obj.resolve();
		if(is_bool(tmp_obj.get_obj_value())) {
			if("#t" == tmp_obj.get_obj_value()) {
				return Expr("#t");
			}
		}
	}	
	return Expr("#f");
}

Expr Expr::Not(Expr& arg_obj)
{
	Expr tmp_obj = arg_obj.resolve();
	if(is_bool(tmp_obj.get_obj_value())) {
		if("#t" == tmp_obj.get_obj_value()) {
			return(Expr("#f"));
		} else {
			return(Expr("#t"));
		}
	}
	return(Expr());
}
