#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <map>

void save(std::string file) {
	std::cout << "You saved the game" << std::endl;
}

void HPitem(std::string item) {
	std::cout << "You have recovered HP using " << item << std::endl;
}

void MPitem(std::string item) {
	std::cout << "You have recovered MP using " << item << std::endl;
}


std::map<std::string, std::function<void(std::string)>> menuFunctions = { {"save",save}, {"HPitem",HPitem},{ "MPitem",MPitem } };









