#include "functions.hpp"
#include "expr.hpp"
#include <iostream>
#include <vector>
#include <string>

typedef std::vector<std::string> token_vec;

void print_script(std::vector<token_vec > obj_vec)
{
    for(auto module: obj_vec)
    {
        auto it_begin = module.begin();
        auto it_end = module.end();
        while(true) {
            std::cout << *it_begin << " ";
	    ++it_begin;
	    if(it_begin == it_end){
		    break;
	    }
        }
	std::cout << std::endl;
	std::cout << std::endl;
    }
}

int Expr::obj_num = 0;

int main()
{
	//std::string str1 =
//"(defun area-circle(rad)(terpri)(format t \" Radius: ~5f\" rad)(format t \"~%Area: ~10f\" (* 3.141592 rad rad)))(area-circle 10)";
	//std::string str1 = "(display '(+ 1500 700))(display \"(+ 1500 700)\")";
	//std::string str1 = "(display '(+ 1500 700) \"barev bolorin\")";
	//std::string str1 = "(display)";
	std::string str1 = "(defun gcd(a b)(print a))";
	//std::string str1 = read_script("tailrec.lisp");
	//std::cout << str1 << std::endl;
	token_vec vec = tokenize_script(str1);
	std::vector<token_vec> arg_token_vec = split_script_into_modules(vec);
	//print_script(arg_token_vec);
	std::vector<Expr> modules_vector = generate_modules_vector(arg_token_vec);
	interprate(modules_vector);
	//std::cout << Expr::obj_num << std::endl;
}
