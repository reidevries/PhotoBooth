#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_

#include <iostream>
#include <fstream>
#include <string>

class Config
{
	std::string config_filename = "config";
	// should put OutputPaths here? but need to define in separate file
	std::string capture_path;
	std::string printer_name;
public:
	Config() {}
	Config(const std::string& _config_filename) 
		: config_filename(_config_filename) {}
	void load();
	auto get_printer_name() -> const std::string& { return printer_name; }
};

#endif //__CONFIG_HPP_
