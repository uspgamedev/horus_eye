#include <sstream>
#include <iostream>
#include <cstdio>
#include "pathmanager.h"

using namespace std;

namespace framework {

std::string PathManager::ResolvePath(const std::string& path) {
    std::ostringstream stm;
    stm << base_path_ << path;
	return stm.str();
}

std::string PathManager::ResolvePath(const std::wstring& path) {
	std::string str(path.begin(), path.end());
	str.assign(path.begin(), path.end());
	return ResolvePath(str);
}

} // namespace framework
