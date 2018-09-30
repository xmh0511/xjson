#include <iostream>
#include "xjson.h"
int main()
{
	//std::cout << sizeof(json) << std::endl;

	//	std::string test = "{\"a\":\"tenxt\",\"b12\":{\"name\":\"text2\"},\"c1\":\"text3erewrewr\"}";
	//	std::cout << test << std::endl;
	//    auto j = json_parse(test);
	//    std::cout << j.jmap["a"].value << std::endl;
	//    std::cout << j.jmap["b12"].jmap["name"].value << std::endl;
	//    std::cout << j.jmap["c1"].value << std::endl;

	std::string test2 = "{\"a\":{\"child\":\"text1234\",\"child2\":\"text123456789\",\"cnumber\":{\"v\":12.0123,\"success\":false  ,\"message\":\"this is just ','a message\"}},\"bb\":\"0000\",\"number\":-123456 ,\"list\":[{\"age\":18},{\"age\":20},\"hahahah\"],\"time\":null}";  //"1","2","3"
	auto j2 = xmh::json::json_parse(test2);
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
	auto& bb = j2["a"]["cnumber"]["success"] = true;
	//bb = false;
	std::cout << j2["list"][1]["age"].get<int>() << std::endl;
	std::cout << j2.stringify() << std::endl;
	std::string v = j2["bb"];
	std::cout << v << std::endl;
	int num = j2["number"];
	std::cout << num << std::endl;
	bool flag = j2["a"]["cnumber"]["success"];
	std::cout << flag << std::endl;
	std::cin.get();
	return 0;
}
