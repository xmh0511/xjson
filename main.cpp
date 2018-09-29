#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;
enum class value_type
{
	INT,
	DOUBLE,
	STRING,
	BOOL,
	NIL,
	OBJECT,
	ARRAY
};
struct json
{
	value_type jtype;
	std::string value;
	std::map<std::string, json> jmap;
	std::vector<json> jarray;
};

std::vector<json*> json_stack;

void skip_open_object_sign(const char*& iter)
{
	while (*iter && *iter != '{') {
		iter++;
	}
	iter++;
}

void skip_white_space(const char*& iter)
{
	while (*iter && *iter <=32) {
		iter++;
	}
}

std::string get_key_name(const char*& iter)
{
	std::string name;
	while (*iter && *iter != '"') {
		name.append({ *iter });
		iter++;
	}
	iter++;
	return name;
}

void get_value_str(const char*& iter)
{
	std::string str;
	while (*iter && *iter != '"') {
		str.append({ *iter });
		iter++;
	}
	iter++;
	auto stack = json_stack.back();
	stack->value = str;
	json_stack.pop_back();
}

void eat_white_space_and_char(const char*& iter,char c)
{
	skip_white_space(iter);
	if (*iter == c) {
		iter++;
	}
}

void parse_value(const char*& iter)
{
	//std::cout << "parse value" << std::endl;
	skip_white_space(iter);
	if (*iter == '"') {
		iter++;
		auto stack = json_stack.back();
		stack->jtype = value_type::STRING;
		get_value_str(iter);
		skip_white_space(iter);
		//std::cout << *iter << *(iter + 1) << std::endl;
	}
}

void parse_block(const char*& iter)
{
	skip_white_space(iter);
	//std::cout <<"init==="<< *iter << std::endl;
	while (*iter)
	{
		if (*iter == ',') {
			iter++;
		}
		if (*iter == '"') {
			//std::cout << *(iter + 1) << std::endl;
			std::cout << *iter << *(iter + 1) << std::endl;
			iter++;
			auto key = get_key_name(iter);
			std::cout<<"name====" << key << std::endl;
			eat_white_space_and_char(iter,':');
			auto stack = json_stack.back();
			switch (stack->jtype) {
			case value_type::OBJECT:
			{
				stack->jmap[key] = json{};
				json_stack.push_back(&stack->jmap[key]);
			}
				break;
			}
			parse_value(iter);
			std::cout<<"after parse===" << *iter << *(iter + 1) << std::endl;
		}
		iter++;
	}
	std::cout << "end" << std::endl;
}

json json_parse(const std::string& jsonstr)
{
	const char* iter = jsonstr.c_str();
	json root;
	root.jtype = value_type::OBJECT;
	json_stack.push_back(&root);
	skip_open_object_sign(iter);
	parse_block(iter);
	return root;
}
int main()
{
	//std::cout << sizeof(json) << std::endl;
	std::string test = "{\"a\":\"text\",\"b12\":\"text2\",\"c1\":\"text3erewrewr\"}";
	//std::cout << test << std::endl;
	auto j = json_parse(test);
	std::cout << j.jmap["a"].value << std::endl;
	std::cout << j.jmap["b12"].value << std::endl;
	std::cout << j.jmap["c1"].value << std::endl;
	cin.get();
	return 0;
}
