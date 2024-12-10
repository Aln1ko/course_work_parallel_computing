#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>

class FileFinder {
public:
	FileFinder();
	std::vector<std::string> find_files(std::vector<std::string> data_folders, int start_index, int finish_index,
		int start_index1, int finish_index1);
	bool is_valid(std::string file_path, int start, int finish);
};
