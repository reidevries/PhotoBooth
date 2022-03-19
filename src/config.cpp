#include "config.hpp"

void Config::load(const std::string& filename)
{
	auto file = std::ifstream(filename, std::ifstream::in);
	if (!file) {
		std::cout << "file not found '" << filename
			<< "', can't load config" << std::endl;
		return;
	} 
	std::string line;
	int line_num;
	while (std::getline(file, line)) {
		if (line_num == 0) {
			printer_name = line;	
		}
		line_num++;
	}
}
