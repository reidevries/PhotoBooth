#include "config.hpp"

OutputPaths::OutputPaths() {}

OutputPaths::OutputPaths(const std::string& folder)
{
	auto path = std::filesystem::path(folder);
	this->folder = path;
	img = path / "avg.jpg";
	img_proc = path / "avg_proc.jpg";
	face = path / "avg.face";
	num_faces = path / "num_faces.u64";
}

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
		if (static_cast<Entry>(line_num) == Entry::PrinterName) {
			printer_name = line;
			std::cout << "printer_name loaded: " << printer_name << std::endl;
		} else if (static_cast<Entry>(line_num) == Entry::TopText) {
			top_text = line;
			std::cout << "top_text loaded: " << top_text << std::endl;
		} else if (static_cast<Entry>(line_num) == Entry::BottomText) {
			bottom_text = line;
			std::cout << "bottom_text loaded: " << bottom_text << std::endl;
		}
		line_num++;
	}
	file.close();
}

void Config::set_output_paths(const std::string& folder)
{
	output_paths = OutputPaths(folder);	
}
