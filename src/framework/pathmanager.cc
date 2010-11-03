#include <sstream>
#include "pathmanager.h"

using namespace std;

namespace framework {

std::string PathManager::ResolvePath(const std::string& path) {
    std::ostringstream stm;
    stm << base_path_ << "/" << path;
	return stm.str();
}

} // namespace framework
