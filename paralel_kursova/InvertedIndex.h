#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>
#include <mutex>
#include <iostream>

class InvertedIndex
{
	std::unordered_map<std::string, std::vector<std::string>> hm;
	std::vector<std::string> words;
	std::mutex hm_mutex;

public:
	InvertedIndex();

	void create_index(std::vector<std::string> files);
	void add_index_1file(const std::string& path);
	void update_index(std::vector<std::string> files);
	std::vector<std::string> find_index(std::string word);

	std::vector<std::string> get_words();
	void print_index();
	~InvertedIndex() ;
};
