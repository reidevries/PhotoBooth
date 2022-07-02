#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_

#include <iostream>
#include <fstream>
#include <string>

class Config
{
	std::string config_filename = "config";
	std::string capture_path;

	enum class Entry
	{
		PrinterName = 0,
		TopText,
		BottomText,
		ExtHdPath
	};
	std::string printer_name, top_text, bottom_text, ext_hd_path;
public:
	Config() {}
	Config(const std::string& _config_filename) 
		: config_filename(_config_filename) {}
	void load();

	auto get_printer_name() const -> const std::string&
	{
		return printer_name; 
	}
	auto get_top_text() const -> const std::string& { return top_text; }
	auto get_bottom_text() const -> const std::string& { return bottom_text; }
	auto get_ext_hd_path() const -> const std::string& { return ext_hd_path; }
};

#endif //__CONFIG_HPP_
