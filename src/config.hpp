#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

struct OutputPaths
{
	std::filesystem::path img, img_proc, face, num_faces, folder;
	OutputPaths();
	OutputPaths(const std::string& folder);
};

class Config
{
	std::string config_filename = "config";
	// should put OutputPaths here? but need to define in separate file
	std::string capture_path;

	enum class Entry
	{
		PrinterName = 0,
		TopText,
		BottomText
	};
	std::string printer_name, top_text, bottom_text;

	OutputPaths output_paths;
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

	void set_output_paths(const std::string& folder);
	auto get_output_paths() const -> OutputPaths { return output_paths; }
};

#endif //__CONFIG_HPP_
