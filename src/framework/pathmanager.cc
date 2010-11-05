#include <iostream>
#include <sstream>
#include <cstdio>
#include "pathmanager.h"

using namespace std;

namespace framework {

std::string PathManager::ResolvePath(const std::string& path) {
    std::ostringstream stm;
    stm << base_path_ << path;
#ifdef DEBUG
    cout << "Resolving " << path << " into " << stm.str();
    FILE* fp = fopen(stm.str().c_str(), "r");
    if(fp == NULL)
        cout << " (WARNING: file not found)";
    else
        fclose(fp);
    cout << endl;
#endif
	return stm.str();
}

} // namespace framework
