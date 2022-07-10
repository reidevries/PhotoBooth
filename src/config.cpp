#include "config.hpp"

void Config::load()
{
	std::cout << "loading config" << std::endl;
	auto file = std::ifstream(config_filename, std::ifstream::in);
	if (!file) {
		std::cout << "file not found '" << config_filename
			<< "', can't load config" << std::endl;
		return;
	} 
	std::string line;
	int line_num = 0;
	while (std::getline(file, line)) {
		if (static_cast<Entry>(line_num) == Entry::PrinterName) {
			printer_name = line;
			std::cout << "printer_name loaded: " << printer_name << std::endl;
		} else if (static_cast<Entry>(line_num) == Entry::TopText) {
			top_text = line;
			std::cout << "top_text loaded: " << top_text << std::endl;
		} else if (static_cast<Entry>(line_num) == Entry::BottomText) {
			bottom_text = line;
			std::cout << "bottom_text loaded: " << bottom_text << std::endl;
		} else if (static_cast<Entry>(line_num) == Entry::ExtHdPath) {
			ext_hd_path = line;
			std::cout << "ext_hd_path loaded: " << ext_hd_path << std::endl;
		} else if (static_cast<Entry>(line_num) == Entry::Rotation) {
			rotation = std::stoi(line);
			std::cout << "rotation loaded: " << rotation << std::endl;
		}
		line_num++;
	}
	file.close();
}
