#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
namespace xmh{

}

//enum class json_bool
//{
//    TRUE,
//    FALSE,
//    UNKNOW
//};
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
class json_error :public std::exception
{
public:
	json_error(const std::string& msg):message(msg)
	{

	}
	char const* what() const
	{
		return message.c_str();
	}
private:
	std::string message;
};
struct json
{
private:
	std::string value_type_name(value_type v)
	{
		switch (v) {
		case value_type::INT:
			return "integer";
			break;
		case value_type::DOUBLE:
			return "double";
			break;
		case value_type::STRING:
			return "string";
			break;
		case value_type::BOOL:
			return "bool";
			break;
		case value_type::NILL:
			return "null";
			break;
		case value_type::OBJECT:
			return "object";
			break;
		case value_type::ARRAY:
			return "array";
			break;
		case value_type::UNKNOW:
			return "unknow";
			break;
		default:
			return "unknow";
			break;
		}
	}
public:
	json() = default;
	json& operator[](const std::string& key)
	{
		if (jtype != value_type::OBJECT)
		{
			std::string message = "illegality operator[],because the value type is" + value_type_name(jtype);
			throw json_error(message);
		}
		return jmap[key];
	}
	json& operator[](std::size_t index)
	{
		if (jtype != value_type::ARRAY)
		{
			std::string message = "illegality operator[],because the value type is" + value_type_name(jtype);
			throw json_error(message);
		}
		if (jarray.size() <= index) {
			std::string message = "out of json array range";
			throw json_error(message);
		}
		return jarray[index];
	}
	template<typename T>
	T get()
	{

	}
	template<>
	int get<int>()
	{
		if (jtype != value_type::INT) {
			std::string message = "illegality get,the value type is not int";
			throw json_error(message);
		}
		return atoi(value.c_str());
	}

	template<>
	double get<double>()
	{
		if (jtype != value_type::DOUBLE) {
			std::string message = "illegality get,the value type is not double";
			throw json_error(message);
		}
		return atof(value.c_str());
	}

	template<>
	std::string get<std::string>()
	{
		if (jtype != value_type::STRING) {
			std::string message = "illegality get,the value type is not string";
			throw json_error(message);
		}
		return value;
	}

	template<>
	std::nullptr_t get<std::nullptr_t>()
	{
		if (jtype != value_type::NILL) {
			std::string message = "illegality get,the value type is not null";
			throw json_error(message);
		}
		return std::nullptr_t{};
	}
	template<>
	bool get<bool>()
	{
		if (jtype != value_type::BOOL) {
			std::string message = "illegality get,the value type is not bool";
			throw json_error(message);
		}
		return value == "true" ? true:false;
	}
public:
	json& operator=(int v)
	{
		if (jtype == value_type::INT) {
			value = std::to_string(v);
		}
		else {
			std::string message = "illegality operator =,because the value type is" + value_type_name(jtype)+" set value type is int";
			throw json_error(message);
		}
	}
	json& operator=(const std::string& v)
	{
		if (jtype == value_type::STRING) {
			value = v;
		}
		else {
			std::string message = "illegality operator =,because the value type is" + value_type_name(jtype) + " set value type is string";
			throw json_error(message);
		}
	}
	json& operator=(double v)
	{
		if (jtype == value_type::DOUBLE) {
			value = std::to_string(v);
		}
		else {
			std::string message = "illegality operator =,because the value type is" + value_type_name(jtype) + " set value type is double";
			throw json_error(message);
		}
	}
	json& operator=(float v)
	{
		if (jtype == value_type::DOUBLE) {
			value = std::to_string(v);
		}
		else {
			std::string message = "illegality operator =,because the value type is" + value_type_name(jtype) + " set value type is float";
			throw json_error(message);
		}
	}
	json& operator=(bool v)
	{
		if (jtype == value_type::BOOL) {
			value = v == true ? "true" : "false";
		}
		else {
			std::string message = "illegality operator =,because the value type is" + value_type_name(jtype) + " set value type is bool";
			throw json_error(message);
		}
	}

	//operator bool()
	//{
	//	if (jtype == value_type::BOOL) {
	//		return value == "true" ? true : false;
	//	}
	//	else {
	//		throw json_error("type is not bool");
	//	}
	//}
public:
	std::string stringify()
	{
		std::stringstream buff;
		dump(buff);
		return buff.str();
	}
public:
	bool is_object()
	{
		return jtype == value_type::OBJECT;
	}

	bool is_array()
	{
		return jtype == value_type::ARRAY;
	}

	bool is_number()
	{
		return jtype == value_type::INT || jtype == value_type::DOUBLE;
	}
	bool is_integer()
	{
		return jtype == value_type::INT;
	}
	bool is_double()
	{
		return jtype == value_type::DOUBLE;
	}
	bool is_string()
	{
		return jtype == value_type::STRING;
	}
	bool is_boolean()
	{
		return jtype == value_type::BOOL;
	}
	bool is_null()
	{
		return jtype == value_type::NILL;
	}
private:
	value_type jtype;
	std::string value;
	std::map<std::string, json> jmap;
	std::vector<json> jarray;
private:
	void dump(std::stringstream& buff)
	{
		if (jtype == value_type::OBJECT) {
			buff << '{';
			for (auto iter = jmap.begin(); iter != jmap.end(); ++iter) {
				buff << '"' << iter->first << "\":";
				//std::cout << buff.str() << std::endl;
				iter->second.dump(buff);
				auto next = iter;
				next++;
				if (next != jmap.end()) {
					buff << ',';
				}
			}
			buff << '}';
		}
		if (jtype == value_type::ARRAY) {
			buff << '[';
			for (auto iter = jarray.begin(); iter != jarray.end(); ++iter) {
				iter->dump(buff);
				auto next = iter;
				next++;
				if (next != jarray.end()) {
					buff << ',';
				}
			}
			buff << ']';
		}
		if (jtype == value_type::BOOL || jtype == value_type::DOUBLE || jtype == value_type::INT || jtype == value_type::NILL) {
			buff << value;
		}
		if (jtype == value_type::STRING) {
			buff << '"' << value << '"';
		}
	}
protected:
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
		while (*iter && *iter <= 32) {
			iter++;
		}
	}
	void trim_all(string &s)
	{
		int index = 0;
		if (!s.empty())
		{
			while ((index = s.find(' ', index)) != string::npos)
			{
				s.erase(index, 1);
			}
		}
	}

	void remove_string_dot(std::string& str)
	{
		auto beg = str.find("\"");
		auto end = str.rfind("\"");
		str = str.substr(beg + 1, end - 1);
	}

	bool is_number_str(char c)
	{
		return isdigit(c) || c == '.' || c == '-';
	}

	bool is_number_str(const char* str)
	{
		bool flag = false;
		do {
			if (*str) {
				flag = is_number_str(*str);
			}
			else {
				break;
			}
			str++;
		} while (flag);
		return flag;
	}

	bool is_bool_str(const std::string& token)
	{
		std::string ntoken = token;
		trim_all(ntoken);
		return ntoken == "true" || ntoken == "false";
	}

	bool is_string_str(const std::string& token)
	{
		return token[0] == '"';
	}

	int count_find_char_size(const char* ptr, char c)
	{
		int count = 0;
		while (*ptr)
		{
			if (*ptr == c)
			{
				count++;
			}
			ptr++;
		}
		return count;
	}
	std::string read_token_value(const char*& iter)
	{
		std::string value;
		while (*iter && *iter != ',' && *iter != '}' && *iter != ']') {
			value.append({ *iter });
			iter++;
		}
		return value;
	}

	void eat_white_space_and_char(const char*& iter, char c)
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
		trim_all(str);
		auto stack = json_stack.back();
		if (!is_number_str(str.c_str())) {
			stack->jtype = value_type::UNKNOW;
		}
		else {
			auto pos_dot = str.find(".");
			if (pos_dot == std::string::npos) {  //is a INT number;
				stack->jtype = value_type::INT;
			}
			if (pos_dot != 0 && count_find_char_size(str.c_str(), '.') == 1) {  //is a double number
				stack->jtype = value_type::DOUBLE;
			}
		}
		//std::cout << str<<"  type===="<<value_type_name(stack->jtype) << std::endl;
		stack->value = std::move(str);
		//std::cout << "next str===" << *iter << *(iter + 1) << *(iter + 2) << *(iter + 3) << std::endl;
		//json_stack.pop_back();
	}

	void get_bool_str(const char*& iter)
	{
		std::string bool_token = read_token_value(iter);
		trim_all(bool_token);
		if (bool_token == "true" || bool_token == "false") {
			auto stack = json_stack.back();
			stack->jtype = value_type::BOOL;
			stack->value = std::move(bool_token);
		}
		//json_stack.pop_back();
	}

	void get_nill_str(const char*& iter)
	{
		std::string nill_token = read_token_value(iter);
		trim_all(nill_token);
		if (nill_token == "null" || nill_token == "NULL") {
			auto stack = json_stack.back();
			stack->jtype = value_type::NILL;
			stack->value = "null";
		}
		//json_stack.pop_back();
	}

	void parse_value(const char*& iter)
	{
		//std::cout << "parse value" << std::endl;
		auto stack = json_stack.back();
		skip_white_space(iter);
		if (*iter == '{') {  //the value is a json object
			eat_white_space_and_char(iter, '{');
			stack->jtype = value_type::OBJECT;
			parse_block_object(iter);
		}

		if (*iter == '[') {  //the value is a json array
			eat_white_space_and_char(iter, '[');
			stack->jtype = value_type::ARRAY;
			parse_block_array(iter);
		}

		if (*iter == '"') {  //the value is a string
			iter++;
			stack->jtype = value_type::STRING;
			get_value_str(iter);
			//std::cout << *iter << *(iter + 1) << std::endl;
		}

		if (*iter == '-' || is_number_str(*iter)) {  //maybe a number
													 //std::cout << "maybe a number" << std::endl;
			get_value_number(iter);
			--iter;
			//std::cout << "a number after==" << *iter<< *(iter+1) << std::endl;
		}

		if (*iter == 'f' || *iter == 't') {  //maybe a bool
			get_bool_str(iter);
		}

		if (*iter == 'n') {
			get_nill_str(iter);
		}
		json_stack.pop_back();
		//skip_white_space(iter);
		//std::cout << "out of getvalue===" << *iter << std::endl;
	}

	void parse_block_object(const char*& iter)
	{
		skip_white_space(iter);
		//	std::cout <<"init==="<< *iter <<*(iter+1)<<*(iter+2)<< std::endl;
		while (*iter)
		{
			//std::cout << "init===" << *iter << *(iter + 1) << *(iter + 2) << std::endl;
			if (*iter == '}') {
				//json_stack.pop_back();
				//std::cout << "init===" << *iter << *(iter - 1) << *(iter - 2) << std::endl;
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
				eat_white_space_and_char(iter, ':');
				auto stack = json_stack.back();
				switch (stack->jtype) {  //if parent is object
				case value_type::OBJECT:
				{
					stack->jmap[key] = json{};
					json_stack.push_back(&stack->jmap[key]);
				}
				break;
				//			    case value_type::ARRAY:
				//                {
				////                    std::cout<<"parent is array"<<std::endl;
				//                    stack->jarray.push_back(json{});
				//                    json_stack.push_back(&stack->jarray.back());
				//                }
				//                    break;
				}
				parse_value(iter);
			}
			iter++;
		}
		//std::cout << "end token }" << std::endl;
	}

	void parse_block_array(const char*& iter)
	{
		skip_white_space(iter);
		while (*iter) {
			if (*iter == ']') {
				break;
			}
			//std::cout << "in block array==" << *iter << std::endl;
			auto stack = json_stack.back();
			if (*iter != ',' && *iter>32) {
				if (stack->jtype == value_type::ARRAY) {
					stack->jarray.push_back(json{});
					json_stack.push_back(&stack->jarray.back());
				}
			}
			if (*iter == '{') {  // element is ojbect
								 //            std::cout<<"first ===="<< stack->jarray.size()<<std::endl;
				auto stack = json_stack.back();
				stack->jtype = value_type::OBJECT;
				iter++;
				parse_block_object(iter);
				//            std::cout<<"total size===="<<json_stack.size()<<std::endl;
				json_stack.pop_back();
			}
			else if (*iter == '\"') {  //element is string or bool or null or number
									   //std::cout << "is string value element===" << json_stack.size() << std::endl;
				char open_token = *iter;
				iter++;
				std::string str;
				bool in_string = true;
				while (*iter && *iter != open_token && (*iter != ',' || in_string)) {
					str.append({ *iter });
					iter++;
				}
				//            std::cout<<"value string =="<<str<<std::endl;
				auto stack = json_stack.back();
				stack->jtype = value_type::STRING;
				stack->value = std::move(str);
				json_stack.pop_back();
			}
			else {  //element is bool or null or number
				std::string str;
				auto stack = json_stack.back();
				while (*iter && *iter != ',' && *iter != ']') {
					str.append({ *iter });
					iter++;
				}
				if (is_bool_str(str)) {
					stack->jtype = value_type::BOOL;
					stack->value = std::move(str);
					json_stack.pop_back();
				}
				else if (is_number_str(str.c_str())) {
					stack->value = std::move(str);
					auto pos_dot = str.find(".");
					if (pos_dot == std::string::npos) {  //is a INT number;
						stack->jtype = value_type::INT;
					}
					if (pos_dot != 0 && count_find_char_size(str.c_str(), '.') == 1) {  //is a double number
						stack->jtype = value_type::DOUBLE;
					}
					json_stack.pop_back();
				}
			}
			iter++;
		}
	}
public:
	static json json_parse(const std::string& jsonstr)
	{
		const char* iter = jsonstr.c_str();
		json root;
		root.jtype = value_type::OBJECT;
		root.json_stack.push_back(&root);
		root.skip_open_object_sign(iter);
		root.parse_block_object(iter);
		return root;
	}
};









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


int main()
{
	//std::cout << sizeof(json) << std::endl;

	//	std::string test = "{\"a\":\"tenxt\",\"b12\":{\"name\":\"text2\"},\"c1\":\"text3erewrewr\"}";
	//	std::cout << test << std::endl;
	//    auto j = json_parse(test);
	//    std::cout << j.jmap["a"].value << std::endl;
	//    std::cout << j.jmap["b12"].jmap["name"].value << std::endl;
	//    std::cout << j.jmap["c1"].value << std::endl;

	std::string test2 = "{\"a\":{\"child\":\"text1234\",\"child2\":\"text123456789\",\"cnumber\":{\"v\":12.0123,\"success\":true  ,\"message\":\"this is just a message\"}},\"bb\":\"0000\",\"number\":-123456 ,\"list\":[{\"age\":18},{\"age\":20},\"hahahah\"],\"time\":null}";  //"1","2","3"
	auto j2 = json::json_parse(test2);
	/*std::cout << j2.jmap["a"].jmap["child"].value << std::endl;*/
	//std::cout << j2.jmap["a"].jmap["child2"].value << std::endl;
	//std::cout << j2.jmap["a"].jmap["cnumber"].jmap["v"].value << std::endl;
	//std::cout << j2.jmap["a"].jmap["cnumber"].jmap["success"].value << std::endl;
	//std::cout << j2.jmap["a"].jmap["cnumber"].jmap["message"].value << std::endl;
	//std::cout << j2.jmap["bb"].value << std::endl;
	//std::cout << j2.jmap["number"].value << std::endl;
	//std::cout << j2.jmap["list"].jarray[0].jmap["age"].value << std::endl;
	//std::cout <<j2.jmap["list"].jarray[1].jmap["age"].value <<std::endl;
	//std::cout <<j2.jmap["list"].jarray.size()<<std::endl;

	//std::string test = "{\"list\":[{\"age\":18 },\"hahahahahha\"]}";
	//auto j3 = json_parse(test);
	//std::cout << j3.jmap["list"].jarray[0].jmap["age"].value << std::endl;
	//std::cout << j3.jmap["list"].jarray[1].value << std::endl;
	std::cout << j2["number"].get<int>() << std::endl;
	std::cout << j2["a"]["child"].get<std::string>()<< std::endl;
	std::cout << j2["a"]["cnumber"]["success"].get<bool>() << std::endl;
	std::cout << j2["list"][1]["age"].get<int>() << std::endl;
	std::cout << j2.stringify() << std::endl;
	cin.get();
	return 0;
}
