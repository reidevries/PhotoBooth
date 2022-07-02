#ifndef __OUTPUT_PATHS_HPP_
#define __OUTPUT_PATHS_HPP_

#include <filesystem>

struct OutputPaths
{
	std::filesystem::path img, img_proc, face, num_faces, folder;
	OutputPaths();
	OutputPaths(const std::string& folder);
};

#endif //__OUTPUT_PATHS_HPP_
