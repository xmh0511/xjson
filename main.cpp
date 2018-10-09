#include <iostream>
#include "release/xjson.hpp"
int main()
{
	//std::cout << sizeof(json) << std::endl;

	//	std::string test = "{\"a\":\"tenxt\",\"b12\":{\"name\":\"text2\"},\"c1\":\"text3erewrewr\"}";
	//	std::cout << test << std::endl;
	//    auto j = json_parse(test);
	//    std::cout << j.jmap["a"].value << std::endl;
	//    std::cout << j.jmap["b12"].jmap["name"].value << std::endl;
	//    std::cout << j.jmap["c1"].value << std::endl;

	std::string test2 = "{\"a\":{\"child\":\"text1234\",\"child2\":\"text123456789\",\"cnumber\":{\"v\":12.0123,\"success\":false  ,\"message\":\"this is just \\\" ','a message\"}},\"bb\":\"0000\",\"number\":-123456 ,\"list\":[{\"age\":  18 },{\"age\": 20  },\"ha\\\"   hahah\",      null ],\"time\":null}";  //"1","2","3"
	auto j2 = xmh::json::json_parse(test2);
	std::cout<<j2.stringify()<<std::endl;
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
	std::cout << "--------------------------------" << std::endl;
	auto& jjj = j2["a"]["cnumber"];
	for (auto iter = jjj.begin(); iter != jjj.end();iter++)
	{
		std::cout << iter.key()<<"-----"<<iter.value().stringify() << std::endl;
	}
	auto& jja = j2["list"];
	for (auto iter = jja.begin(); iter != jja.end(); ++iter) {
		std::cout << (*iter).stringify() << std::endl;
	}
	for (auto iter : jjj) {
		std::cout << "range===" << iter.stringify() << std::endl;
	}
	std::cout << "--------------------------------" << std::endl;
	xmh::json root;
	xmh::json iter;
	xmh::json iter2;
	iter["age"] = 18;
	iter2["age"] = 26;
	root["abc"] = "text";
	root["list"].push_back(iter);
	std::cout << root.stringify() << std::endl;
	root["list"][0] = iter2;
	root["list"].push_back(iter);
	std::cout << root.stringify() << std::endl;


//	std::string tstr = " {   \"list\"       :  [    {    \"halo\"  :      18         }     ,       {\"age\":    20 }       ] }";
//	auto jj = xmh::json::json_parse(tstr);
//	std::cout<<jj.stringify()<<std::endl;
//	std::cout<<jj["list"][0]["halo"].get<int>()<<std::endl;
	return 0;
}
