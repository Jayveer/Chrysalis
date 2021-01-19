#pragma once
#include <filesystem>

inline
void updateDir(const std::string& path, std::string& output) {
	std::filesystem::path p{ output };
	p.append(path);
	output = p.u8string();
}

inline
void resetDir(std::string& output) {
	std::filesystem::path p{ output };
	output = p.parent_path().u8string();
}

inline
std::string getCurrentDir(std::string& output) {
	std::filesystem::path p{ output };
	return p.filename().u8string();
}

inline
bool isDirectory(std::string& output) {
	std::filesystem::path p{ output };
	return std::filesystem::is_directory(p);
}

inline
bool fileExists(std::string& output) {
	std::filesystem::path p{ output };
	return std::filesystem::exists(p);
}