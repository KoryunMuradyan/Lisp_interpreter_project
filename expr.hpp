#ifndef __CLASS__DECLARATION__HPP__
#define __CLASS__DECLARATION__HPP__

#include "built_in_functions.hpp"
#include "functions.hpp"
#include <vector>
#include <iterator>
#include <iostream>

typedef std::vector<std::string> token_vec;

enum type {literal, list, identifier};

class Expr
{
	private:
		std::vector<Expr> m_expr_vector = {};
		std::string m_value = "";
		int m_this_obj_type = -1;
		std::string m_var_type = "";
		std::string m_var_value = "";
		static int obj_num;  // technical purpose...  to be teleted

		friend bool is_number(const std::string&);
		friend bool is_bool(const std::string&);
		friend bool is_array(const std::string&);
		friend bool is_string(const std::string&);
		friend bool is_literal(const std::string&);
		friend token_vec::iterator find_coobrace(token_vec::iterator&);
	public:
		Expr()
		{}

		Expr(Expr&);
		Expr(std::vector<Expr>&);
		//Expr(const Expr&);
		Expr(Expr&&);
		Expr(token_vec::iterator&, token_vec::iterator&);
		Expr(const std::string&);

		Expr operator+(Expr);
		Expr operator-(Expr);
		Expr operator*(Expr);
		Expr operator/(Expr);
		Expr operator<(Expr);
		Expr operator>(Expr);
		Expr operator=(Expr);
		Expr operator<=(Expr);
		Expr operator>=(Expr);
		Expr And(std::vector<Expr>&);
		Expr Or(std::vector<Expr>&);
		Expr Not(Expr&);

		int get_obj_type();
		std::string get_var_type();
		std::string get_obj_value();
		std::string get_var_value();
		Expr resolve();
		void set_var_type(const std::string&);
		void set_string(const std::string&);
		void set_var_value(const std::string&);
		void set_array(const std::string&);
		void define_type(const std::string&);
		void define_obj_vector(token_vec::iterator&, token_vec::iterator&);
		std::vector<Expr> get_args_vec();

};

#endif // __CLASS__DECLARATION__HPP__
