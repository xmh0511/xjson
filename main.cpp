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
	NILL,
	OBJECT,
	ARRAY,
    UNKNOW
};
//enum class json_bool
//{
//    TRUE,
//    FALSE,
//    UNKNOW
//};
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

void remove_string_dot(std::string& str)
{
    auto beg = str.find("\"");
    auto end = str.rfind("\"");
    str = str.substr(beg+1,end-1);
}

bool is_number_str(char c)
{
    return std::isdigit(c) || c=='.';
}

bool is_number_str(const char* str)
{
    bool flag = false;
    do{
        flag = is_number_str(*str);
        str++;
    }while(flag);
    return flag;
}

bool is_bool_str(const std::string& token)
{
    return token == "true" || token=="false";
}

bool is_string_str(const std::string& token)
{
    return token[0] == '"';
}

int count_find_char_size(const char* ptr,char c)
{
    int count = 0;
    while(*ptr)
    {
        if(*ptr == c)
        {
            count++;
        }
        ptr++;
    }
    return count;
}

//json_bool validate_bool(const char* iter)
//{
//    // true or false
//    json_bool result = json_bool::UNKNOW;
//    if(*iter == 't')  //maybe true?
//    {
//        iter++;
//        int size = 0;
//        const char validate_str_true[3] = {'r','u','e'};
//        while(size< 3)
//        {
//           if(*iter != validate_str_true[size]) {
//               break;
//           }
//            result = json_bool::TRUE;
//            iter++;
//            size++;
//        }
//    }else if(*iter =='f'){ //maybe false?
//        iter++;
//        int size = 0;
//        const char validate_str_true[4] = {'a','l','s','e'};
//        while(size< 4)
//        {
//            if(*iter != validate_str_true[size]) {
//                break;
//            }
//            result = json_bool::FALSE;
//            iter++;
//            size++;
//        }
//    }
//    return result;
//}

std::string read_token_value(const char*& iter)
{
    std::string value;
    while(*iter && *iter!=',' && *iter!='}'){
        value.append({*iter});
        iter++;
    }
    return value;
}

void eat_white_space_and_char(const char*& iter,char c)
{
    skip_white_space(iter);
    if (*iter == c) {
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
	auto stack = json_stack.back();
	stack->value = std::move(str);
	//json_stack.pop_back();
}

void get_value_number(const char*& iter)
{
    std::string str = read_token_value(iter);
    auto stack = json_stack.back();
    if(!is_number_str(str.c_str())){
        stack->jtype = value_type::UNKNOW;
    }else{
       auto pos_dot = str.find(".");
       if(pos_dot == std::string::npos){  //is a INT number;
           stack->jtype = value_type::INT;
       }
       if(pos_dot!=0 && count_find_char_size(str.c_str(),'.')==1){  //is a double number
           stack->jtype = value_type::DOUBLE;
       }
    }
    stack->value = std::move(str);
    //json_stack.pop_back();
}

void get_bool_str(const char*& iter)
{
    std::string bool_token = read_token_value(iter);
    if(bool_token =="true" || bool_token =="false"){
        auto stack = json_stack.back();
        stack->jtype = value_type::BOOL;
        stack->value = std::move(bool_token);
    }
    //json_stack.pop_back();
}

void get_nill_str(const char*& iter)
{
    std::string nill_token = read_token_value(iter);
    if(nill_token == "null" || nill_token =="NULL"){
        auto stack = json_stack.back();
        stack->jtype = value_type::NILL;
        stack->value = "null";
    }
    //json_stack.pop_back();
}



void parse_block_object(const char*& iter);
void parse_block_array(const char*& iter);
void parse_value(const char*& iter);

void parse_value(const char*& iter)
{
	//std::cout << "parse value" << std::endl;
    auto stack = json_stack.back();
	skip_white_space(iter);
	if(*iter == '{'){  //the value is a json object
        eat_white_space_and_char(iter,'{');
        stack->jtype = value_type::OBJECT;
        parse_block_object(iter);
	}

	if(*iter =='['){  //the value is a json array
        eat_white_space_and_char(iter,'[');
        stack->jtype = value_type::ARRAY;
        parse_block_array(iter);
	}

    if (*iter == '"') {  //the value is a string
        iter++;
        stack->jtype = value_type::STRING;
        get_value_str(iter);
        //std::cout << *iter << *(iter + 1) << std::endl;
    }

	if(*iter =='-' || is_number_str(*iter)){  //maybe a number
        get_value_number(iter);
	}

	if(*iter == 'f' || *iter =='t'){  //maybe a bool
	    get_bool_str(iter);
	}

	if(*iter == 'n'){

	}
    json_stack.pop_back();
    skip_white_space(iter);
}

void parse_block_object(const char*& iter)
{
	skip_white_space(iter);
//	std::cout <<"init==="<< *iter <<*(iter+1)<<*(iter+2)<< std::endl;
	while (*iter)
	{
	    if(*iter == '}'){
            //json_stack.pop_back();
            break;
	    }
//		if (*iter == ',') {
//            json_stack.pop_back();
//			iter++;
//		}
		if (*iter == '"') {
			//std::cout << *(iter + 1) << std::endl;
			//std::cout << *iter << *(iter + 1) << std::endl;
			iter++;
			auto key = get_key_name(iter);
			//std::cout<<"name====" << key << std::endl;
			eat_white_space_and_char(iter,':');
			auto stack = json_stack.back();
			switch (stack->jtype) {  //if parent is object
			case value_type::OBJECT:
			{
				stack->jmap[key] = json{};
				json_stack.push_back(&stack->jmap[key]);
			}
				break;
			    case value_type::ARRAY:
                {
                    stack->jarray.push_back(json{});
                    json_stack.push_back(&stack->jarray.back());
                }
                    break;
			}
			parse_value(iter);
			//std::cout<<"after parse===" << *iter << *(iter + 1) << std::endl;
		}
		iter++;
	}
	//std::cout << "end token }" << std::endl;
}

void parse_block_array(const char*& iter)
{
    skip_white_space(iter);
    while(*iter){
        if(*iter == ']'){
            break;
        }
//        std::cout<<"in block array=="<<*iter<<std::endl;
        if(*iter !=',' && *iter>32){
            auto stack = json_stack.back();
            if(stack->jtype == value_type::ARRAY){
                stack->jarray.push_back(json{});
                json_stack.push_back(&stack->jarray.back());
            }
        }
        if(*iter == '{'){  // element is ojbect
             iter++;
            parse_block_object(iter);
        }else if(*iter == '\"'){  //element is string or bool or null or number
//            std::cout<<"is string value element==="<<json_stack.size()<<std::endl;
            char open_token = *iter;
            iter++;
            std::string str;
            bool in_string = true;
            while(*iter && *iter!=open_token && (*iter!=',' || in_string)){
                str.append({*iter});
                iter++;
            }
//            std::cout<<"value string =="<<str<<std::endl;
           auto stack = json_stack.back();
           stack->jtype = value_type::STRING;
           stack->value = std::move(str);
           json_stack.pop_back();
        }else{  //element is bool or null or number
            std::string str;
            auto stack = json_stack.back();
            while(*iter && *iter!=',' && *iter !=']'){
                str.append({*iter});
                iter++;
            }
            if(is_bool_str(str)){
                stack->jtype = value_type::BOOL;
                stack->value = std::move(str);
                json_stack.pop_back();
            }else if(is_number_str(str.c_str())){
                stack->value = std::move(str);
                auto pos_dot = str.find(".");
                if(pos_dot == std::string::npos){  //is a INT number;
                    stack->jtype = value_type::INT;
                }
                if(pos_dot!=0 && count_find_char_size(str.c_str(),'.')==1){  //is a double number
                    stack->jtype = value_type::DOUBLE;
                }
             json_stack.pop_back();
            }
        }
        iter++;
    }
}

json json_parse(const std::string& jsonstr)
{
	const char* iter = jsonstr.c_str();
	json root;
	root.jtype = value_type::OBJECT;
	json_stack.push_back(&root);
	skip_open_object_sign(iter);
    parse_block_object(iter);
	return root;
}
int main()
{
	//std::cout << sizeof(json) << std::endl;

//	std::string test = "{\"a\":\"tenxt\",\"b12\":{\"name\":\"text2\"},\"c1\":\"text3erewrewr\"}";
//	std::cout << test << std::endl;
//    auto j = json_parse(test);
//    std::cout << j.jmap["a"].value << std::endl;
//    std::cout << j.jmap["b12"].jmap["name"].value << std::endl;
//    std::cout << j.jmap["c1"].value << std::endl;

    std::string test2 = "Rjson("
                        "{\"a\":{\"child\":\"text1234\",\"child2\":\"text123456789\","
                        "\"cnumber\":{\"v\":12.0123,\"success\":false,\"message\":\"this is just a message\"}},"
                        "\"bb\":\"0000\",\"number\":-123456,\"list\":[\"1\",\"2\",\"3\"]}"
                        ")json";
    auto j2 = json_parse(test2);
    std::cout <<j2.jmap["a"].jmap["child"].value<<std::endl;
    std::cout <<j2.jmap["a"].jmap["child2"].value<<std::endl;
    std::cout <<j2.jmap["a"].jmap["cnumber"].jmap["v"].value<<std::endl;
    std::cout <<j2.jmap["a"].jmap["cnumber"].jmap["success"].value<<std::endl;
    std::cout <<j2.jmap["a"].jmap["cnumber"].jmap["message"].value<<std::endl;
    std::cout <<j2.jmap["bb"].value<<std::endl;
    std::cout <<j2.jmap["number"].value<<std::endl;
    std::cout <<j2.jmap["list"].jarray[0].value<<std::endl;
    std::cout <<j2.jmap["list"].jarray[1].value<<std::endl;
    std::cout <<j2.jmap["list"].jarray[2].value<<std::endl;
	return 0;
}
