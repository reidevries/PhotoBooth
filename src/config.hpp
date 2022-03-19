#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_

#include <iostream>
#include <fstream>
#include <string>

class Config
{
	std::string printer_name;
public:
	Config() {}
	Config(const std::string& _printer_name) : printer_name(_printer_name) {}
	void load(const std::string& filename);
	auto get_printer_name() -> const std::string& { return printer_name; }
};

#endif //__CONFIG_HPP_
