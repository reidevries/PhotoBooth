#include "config.hpp"

void Config::load()
{
	auto file = std::ifstream(config_filename, std::ifstream::in);
	if (!file) {
		std::cout << "file not found '" << config_filename
			<< "', can't load config" << std::endl;
		return;
	} 
	std::string line;
	int line_num;
	while (std::getline(file, line)) {
		if (line_num == 0) {
			printer_name = line;
			std::cout << "printer_name loaded: " << printer_name << std::endl;
		}
		line_num++;
	}
	file.close();
}
