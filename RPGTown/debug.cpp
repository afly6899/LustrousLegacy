#include "debug.h"

// This function is strictly for debugging purposes (use this to create console messages)
void console_message(std::string x) {
	std::string temp_string = x;
	std::cout << "SYSTEM MESSAGE: " + temp_string << std::endl;
}

