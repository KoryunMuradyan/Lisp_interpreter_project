#include <vector>
//#include <iostream>
#include <string>

typedef std::vector<std::string> token_vec;

void push_back(token_vec& arg_vec, std::string& arg_str) {
    arg_vec.push_back(arg_str);
    arg_str = "";
}

void push_back_brace(token_vec& token_vec, char ch, std::string& tmp_str,
		int& counter) {
	if ("" != tmp_str) {
		push_back(token_vec, tmp_str);
	}
	tmp_str += ch;
	if ("(" == tmp_str) {
		++counter;
	} else {
		--counter;
	}
	push_back(token_vec, tmp_str);
}

void push_back_array(token_vec& token_vec, const std::string& arg_str,
		std::string& tmp_str, int& i) {
	tmp_str += arg_str[i++];
	int counter = 0;
	while (true){
		if ('(' == arg_str[i]) {
			++counter;
		} else if (')' == arg_str[i]) {
			--counter;
		}
		tmp_str += arg_str[i++];
		if (0 == counter) {	
			break;
		}
	}
	--i;
	push_back(token_vec, tmp_str);
}

void push_back_string(token_vec& token_vec, const std::string& arg_str,
		std::string& tmp_str, int& i) {
	while (true){
		tmp_str += arg_str[i++];
		if ('"' == arg_str[i]){
			tmp_str += arg_str[i];
			break;
		}
	}
	push_back(token_vec, tmp_str);
}

token_vec tokenize_script(const std::string& arg_str) {
	token_vec token_vec = {};
	int counter = 0;
	std::string tmp_str = "";
	for (int i = 0; i < arg_str.length(); ++i) {
		if (' ' == arg_str[i]) {
			if ("" != tmp_str){
				push_back(token_vec, tmp_str);
			}
			continue;
		} else if ('"' == arg_str[i]) {
			push_back_string(token_vec, arg_str, tmp_str, i);
				continue;
		} else if ('\'' == arg_str[i]) {
			push_back_array(token_vec, arg_str, tmp_str, i);
				continue;
		} else if ('(' == arg_str[i] || ')' == arg_str[i]) {
			push_back_brace(token_vec, arg_str[i], tmp_str, counter);
			continue;
		}
		tmp_str += arg_str[i];
	}
	return token_vec;
}
