#include "output_paths.hpp"

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

