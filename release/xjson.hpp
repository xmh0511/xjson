#pragma once
#include <map>
#include <vector>
#include <string>
#include <sstream>
namespace xmh {
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
		json_error(const std::string& msg) :message(msg)
		{

		}
		char const* what() const noexcept
		{
			return message.c_str();
		}
	private:
		std::string message;
	};

	template<typename T>
	class json_iterator
	{
	public:
		json_iterator(typename std::vector<T>::iterator arr_iter, value_type v_t) :arr_iter_(arr_iter), json_type(v_t)
		{

		}
		json_iterator(typename std::map<std::string, T>::iterator map_iter, value_type v_t) :map_iter_(map_iter), json_type(v_t)
		{

		}
	public:
		json_iterator & operator++()
		{
			if (json_type == value_type::ARRAY)
			{
				++arr_iter_;
			}
			if (json_type == value_type::OBJECT)
			{
				++map_iter_;
			}
			return *this;
		}
		json_iterator& operator++(int)
		{
			return operator++();
		}

		json_iterator & operator--()
		{
			if (json_type == value_type::ARRAY)
			{
				--arr_iter_;
			}
			if (json_type == value_type::OBJECT)
			{
				--map_iter_;
			}
			return *this;
		}
		json_iterator& operator--(int)
		{
			return operator--();
		}
		bool operator!=(const json_iterator& it)
		{
			if (json_type == value_type::ARRAY) {
				return arr_iter_ != it.arr_iter_;
			}
			if (json_type == value_type::OBJECT) {
				return map_iter_ != it.map_iter_;
			}
			throw json_error("illegality operator for the type");
		}
	public:
		T & operator*()
		{
			if (json_type == value_type::ARRAY) {
				return *arr_iter_;
			}
			if (json_type == value_type::OBJECT) {
				return map_iter_->second;
			}
			throw json_error("illegality operator for the type");
		}
		std::string key()
		{
			if (json_type != value_type::OBJECT) {
				throw json_error("illegality operator for the type,because it's not object");
			}
			return map_iter_->first;
		}
		T& value()
		{
			if (json_type != value_type::OBJECT) {
				throw json_error("illegality operator for the type,because it's not object");
			}
			return map_iter_->second;
		}
	private:
		typename std::vector<T>::iterator arr_iter_;
		typename std::map<std::string, T>::iterator map_iter_;
		value_type json_type;
	};

	class json
	{
	private:
		std::string value_type_name(value_type v) const
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
		json()
		{
			jtype = value_type::UNKNOW;
		}
		/////construct params
		json(int v)
		{
			value = std::to_string(v);
			jtype = value_type::INT;
		}
		json(const std::string& v)
		{
			value = v;
			jtype = value_type::STRING;
		}
		json(const char* v)
		{
			value = std::string(v);
			jtype = value_type::STRING;
		}

		json(double v)
		{
			value = std::to_string(v);
			jtype = value_type::DOUBLE;
		}
		json(float v)
		{
			value = std::to_string(v);
			jtype = value_type::DOUBLE;
		}
		json(bool v)
		{
			value = v == true ? "true" : "false";
			jtype = value_type::BOOL;
		}
		/////
		json& operator[](const std::string& key)
		{
			if (jtype == value_type::UNKNOW) {
				jtype = value_type::OBJECT;
			}
			if (jtype != value_type::OBJECT)
			{
				std::string message = "illegality operator[],because the value type is" + value_type_name(jtype);
				throw json_error(message);
			}
			return jmap[key];
		}
		json& operator[](const char* str)
		{
			return operator[](std::string(str));
		}
		json& operator[](int index)
		{
			if (jtype != value_type::ARRAY)
			{
				std::string message = "illegality operator[],because the value type is" + value_type_name(jtype);
				throw json_error(message);
			}
			if (jarray.size() <= (std::size_t)index) {
				std::string message = "out of json array range";
				throw json_error(message);
			}
			return jarray[index];
		}

		json& operator[](std::size_t index)
		{
			if (jtype != value_type::ARRAY)
			{
				std::string message = "illegality operator[],because the value type is" + value_type_name(jtype);
				throw json_error(message);
			}
			if (jarray.size() <= (std::size_t)index) {
				std::string message = "out of json array range";
				throw json_error(message);
			}
			return jarray[index];
		}
		////const operator[]
		json const& operator[](const std::string& key) const
		{
			if (jtype != value_type::OBJECT)
			{
				std::string message = "illegality operator[],because the value type is" + value_type_name(jtype);
				throw json_error(message);
			}
			return jmap.at(key);
		}
		json const& operator[](const char* str) const
		{
			return operator[](std::string(str));
		}
		json const& operator[](int index) const
		{
			if (jtype != value_type::ARRAY)
			{
				std::string message = "illegality operator[],because the value type is" + value_type_name(jtype);
				throw json_error(message);
			}
			if (jarray.size() <= (std::size_t)index) {
				std::string message = "out of json array range";
				throw json_error(message);
			}
			return jarray[index];
		}

		json const& operator[](std::size_t index) const
		{
			if (jtype != value_type::ARRAY)
			{
				std::string message = "illegality operator[],because the value type is" + value_type_name(jtype);
				throw json_error(message);
			}
			if (jarray.size() <= (std::size_t)index) {
				std::string message = "out of json array range";
				throw json_error(message);
			}
			return jarray[index];
		}

		template<typename T>
		typename std::enable_if<std::is_same<int, T>::value, int>::type get() const
		{
			if (jtype != value_type::INT) {
				std::string message = "illegality get,the value type is not int";
				throw json_error(message);
			}
			return atoi(value.c_str());
		}

		template<typename T>
		typename std::enable_if<std::is_same<double, T>::value, double>::type get() const
		{
			if (jtype != value_type::DOUBLE) {
				std::string message = "illegality get,the value type is not double";
				throw json_error(message);
			}
			return atof(value.c_str());
		}

		template<typename T>
		typename std::enable_if<std::is_same<std::string, T>::value, std::string>::type get() const
		{
			if (jtype != value_type::STRING) {
				std::string message = "illegality get,the value type is not string";
				throw json_error(message);
			}
			return value;
		}

		template<typename T>
		typename std::enable_if<std::is_same<std::nullptr_t, T>::value, std::nullptr_t>::type get() const
		{
			if (jtype != value_type::NILL) {
				std::string message = "illegality get,the value type is not null";
				throw json_error(message);
			}
			return std::nullptr_t{};
		}
		template<typename T>
		typename std::enable_if<std::is_same<bool, T>::value, bool>::type get() const
		{
			if (jtype != value_type::BOOL) {
				std::string message = "illegality get,the value type is not bool";
				throw json_error(message);
			}
			return value == "true" ? true : false;
		}
	public:
		json & operator=(int v)
		{
			value = std::to_string(v);
			jtype = value_type::INT;
			return *this;
		}
		json& operator=(const std::string& v)
		{
			value = v;
			jtype = value_type::STRING;
			return *this;
		}
		json& operator=(const char* v)
		{
			value = std::string(v);
			jtype = value_type::STRING;
			return *this;
		}

		json& operator=(double v)
		{
			value = std::to_string(v);
			jtype = value_type::DOUBLE;
			return *this;
		}
		json& operator=(float v)
		{
			value = std::to_string(v);
			jtype = value_type::DOUBLE;
			return *this;
		}
		json& operator=(bool v)
		{
			value = v == true ? "true" : "false";
			jtype = value_type::BOOL;
			return *this;
		}


		operator std::string() const
		{
			if (jtype == value_type::STRING) {
				return value;
			}
			else {
				throw json_error("type is not string");
			}
		}

		operator int() const
		{
			if (jtype == value_type::INT) {
				return atoi(value.c_str());
			}
			else {
				throw json_error("type is not int");
			}
		}

		operator double() const
		{
			if (jtype == value_type::DOUBLE) {
				return atof(value.c_str());
			}
			else {
				throw json_error("type is not int");
			}
		}

		operator bool() const
		{
			if (jtype == value_type::BOOL) {
				return value == "true" ? true : false;
			}
			else {
				throw json_error("type is not bool");
			}
		}
	public:
		std::string stringify() const
		{
			std::stringstream buff;
			dump(buff);
			return buff.str();
		}
	public:
		bool is_object() const
		{
			return jtype == value_type::OBJECT;
		}

		bool is_array() const
		{
			return jtype == value_type::ARRAY;
		}

		bool is_number() const
		{
			return jtype == value_type::INT || jtype == value_type::DOUBLE;
		}
		bool is_integer() const
		{
			return jtype == value_type::INT;
		}
		bool is_double() const
		{
			return jtype == value_type::DOUBLE;
		}
		bool is_string() const
		{
			return jtype == value_type::STRING;
		}
		bool is_boolean() const
		{
			return jtype == value_type::BOOL;
		}
		bool is_null() const
		{
			return jtype == value_type::NILL;
		}
	public:
		json_iterator<json> begin()
		{
			std::string message = "illegality operator begin,the type is not array or object";
			if ((jtype != value_type::ARRAY) && (jtype != value_type::OBJECT))
			{
				throw json_error(message);
			}
			if (jtype == value_type::ARRAY)
			{
				return json_iterator<json>(jarray.begin(), jtype);
			}
			if (jtype == value_type::OBJECT)
			{
				return json_iterator<json>(jmap.begin(), jtype);
			}
			throw json_error(message);
		}
		json_iterator<json> end()
		{
			std::string message = "illegality operator begin,the type is not array or object";
			if (jtype != value_type::ARRAY && jtype != value_type::OBJECT)
			{
				throw json_error(message);
			}
			if (jtype == value_type::ARRAY)
			{
				return json_iterator<json>(jarray.end(), jtype);
			}
			if (jtype == value_type::OBJECT)
			{
				return json_iterator<json>(jmap.end(), jtype);
			}
			throw json_error(message);
		}
	public:
		void push_back(const json& j)
		{
			if (jtype == value_type::UNKNOW) {
				jtype = value_type::ARRAY;
			}
			if (jtype != value_type::ARRAY) {
				throw json_error("illegality operator push_back ,the type is not array");
			}
			jarray.push_back(j);
		}
	private:
		value_type jtype;
		std::string value;
		std::map<std::string, json> jmap;
		std::vector<json> jarray;
	private:
		void dump(std::stringstream& buff) const
		{
			if (jtype == value_type::OBJECT) {
				auto last_iter = jmap.end();
				last_iter--;
				buff << '{';
				for (auto iter = jmap.begin(); iter != jmap.end(); ++iter) {
					buff << '"' << iter->first << "\":";
					//std::cout << buff.str() << std::endl;
					iter->second.dump(buff);
					if (iter != last_iter) {
						buff << ',';
					}
				}
				buff << '}';
			}
			if (jtype == value_type::ARRAY) {
				auto last_iter = jarray.end();
				if (!jarray.empty()) {
					last_iter--;
				}
				buff << '[';
				for (auto iter = jarray.begin(); iter != jarray.end(); ++iter) {
					iter->dump(buff);
					if (iter != last_iter) {
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
		void trim_all(std::string &s)
		{
			int index = 0;
			if (!s.empty())
			{
				while ((index = s.find(' ', index)) != std::string::npos)
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
			return token == "true" || token == "false";
		}

		bool is_string_str(const std::string& token)
		{
			return token[0] == '"';
		}

		bool is_nill_str(const std::string& token)
		{
			if (token == "null" || token == "NULL") {
				return true;
			}
			return false;
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
			const char* first = iter;
			while (*iter && *iter != ',' && *iter != '}' && *iter != ']' && *iter > 32) {
				iter++;
			}
			return std::string(first, iter);
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
			const char* first = iter;
			while (*iter && *iter != '"') {
				iter++;
			}
			std::string name(first, iter);
			iter++;
			return name;
		}

		void get_value_str(const char*& iter)
		{
			const char* first = iter;
			while (*iter && (*iter != '"' || *(iter - 1) == '\\')) {
				iter++;
			}
			auto stack = json_stack.back();
			stack->value = std::string(first, iter);
		}

		void get_value_number(const char*& iter)
		{
			std::string str = read_token_value(iter);
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
			stack->value = std::move(str);
		}

		void get_bool_str(const char*& iter)
		{
			std::string bool_token = read_token_value(iter);
			if (bool_token == "true" || bool_token == "false") {
				auto stack = json_stack.back();
				stack->jtype = value_type::BOOL;
				stack->value = std::move(bool_token);
			}
		}

		void get_nill_str(const char*& iter)
		{
			std::string nill_token = read_token_value(iter);
			if (nill_token == "null" || nill_token == "NULL") {
				auto stack = json_stack.back();
				stack->jtype = value_type::NILL;
				stack->value = "null";
			}
		}

		void parse_value(const char*& iter)
		{
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
			}

			if (*iter == '-' || is_number_str(*iter)) {  //maybe a number
				get_value_number(iter);
				--iter;
			}

			if (*iter == 'f' || *iter == 't') {  //maybe a bool
				get_bool_str(iter);
			}

			if (*iter == 'n') {
				get_nill_str(iter);
			}
			json_stack.pop_back();
		}

		void parse_block_object(const char*& iter)
		{
			skip_white_space(iter);
			while (*iter)
			{
				skip_white_space(iter);
				if (*iter == '}') {
					break;
				}
				if (*iter == '"') {
					iter++;
					auto key = get_key_name(iter);
					eat_white_space_and_char(iter, ':');
					auto stack = json_stack.back();
					switch (stack->jtype) {  //if parent is object
					case value_type::OBJECT:
					{
						json_stack.push_back(&stack->jmap[key]);
					}
					break;
					}
					parse_value(iter);
				}
				iter++;
			}
		}

		void parse_block_array(const char*& iter)
		{
			skip_white_space(iter);
			while (*iter) {
				skip_white_space(iter);
				if (*iter == ']') {
					break;
				}
				auto stack = json_stack.back();
				if (*iter != ',' && *iter > 32) {
					if (stack->jtype == value_type::ARRAY) {
						stack->jarray.push_back(json{});
						json_stack.push_back(&stack->jarray.back());
					}
				}
				if (*iter == '{') {  // element is ojbect
					auto stack = json_stack.back();
					stack->jtype = value_type::OBJECT;
					iter++;
					parse_block_object(iter);
					json_stack.pop_back();
				}
				else if (*iter == '"') {  //element is string
					char open_token = *iter;
					iter++;
					const char* first = iter;
					while (*iter && (*iter != open_token || *(iter - 1) == '\\')) {
						iter++;
					}
					auto stack = json_stack.back();
					stack->jtype = value_type::STRING;
					stack->value = std::string(first, iter);
					json_stack.pop_back();
				}
				else {  //element is bool or null or number
					const char* first = iter;
					auto stack = json_stack.back();
					while (*iter && *iter != ',' && *iter != ']' && *iter > 32) {
						iter++;
					}
					std::string str(first, iter);
					if (*iter == ']') {
						--iter;
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
					else if (is_nill_str(str))
					{
						stack->value = std::move(str);
						stack->jtype = value_type::NILL;
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
}
