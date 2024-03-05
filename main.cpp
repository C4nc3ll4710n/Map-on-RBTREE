#include <algorithm>
#include <string>
#include <utility>
#include <iostream>
#include "map.hpp"


int main()
{	
	
    
	Map<std::string, std::string> dict, dict2;
	dict.insert(make_pair("sort", ""));
	dict.insert(make_pair("string", "ַ"));
	dict.insert(make_pair("count", ""));
	dict.insert(make_pair("string", "(ַ)")); 
	Map<std::string, std::string>::iterator ite = dict.begin();

	while (ite != dict.end()){
		cout << ite->first << ":" << ite->second << " \n" << endl;
		++ite;
	}
    

	//std::cout << dict.size();
	//dict.clear();
	std::cout << dict.size();

	dict2.insert(make_pair("1", "cats"));
	dict2.insert(make_pair("20", "dogs"));
	dict2.insert(make_pair("550", "frogs"));
	dict2.insert(make_pair("meme", "foxes")); 
	//dict.swap(dict2);
	if (dict==dict2){
		std::cout << "AAA\n" ; 
	}
	dict = dict2;
	if (dict==dict2){
		std::cout << "AAA\n" ; 
	}
	Map<std::string, std::string>::iterator it = dict.begin();
	it = dict.erase(it);
	std::cout << dict.at("20") <<" at \n";
	
	std::cout << "outputhing new map\n";
	while (it != dict.end()){
		cout << it->first << ":" << it->second << endl;
		++it;
	}
	std:: cout << endl;

	std::cout << dict["1"] << " " << dict["20"];
	return 0;
};
