#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>
#include <mutex>
#include <iostream>
#include "MyQueue.h"
#include <set>
#include <shared_mutex>

class InvertedIndex
{
	std::unordered_map<std::string, std::vector<std::string>> hm;
	std::set<std::string> words;
	std::shared_mutex hm_mutex;

public:
	InvertedIndex();

	void create_index(std::vector<std::string> files);
	void create_index1(std::vector<std::string> files, MyQueue& q);

	void add_index_1file(const std::string& path);
	void update_index(std::vector<std::string> files);
	std::vector<std::string> find_index(std::string word);

	std::set<std::string> get_words();
	void print_index();
	~InvertedIndex() ;
};
