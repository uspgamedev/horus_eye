#ifndef HORUSEYE_FRAMEWORK_PATHMANAGER_H_
#define HORUSEYE_FRAMEWORK_PATHMANAGER_H_

#include <string>
#define PATH_MANAGER() framework::Engine::reference()->path_manager()

namespace framework {

class PathManager{
  public:
	PathManager(const std::string& base_path) : base_path_(base_path) {}
    ~PathManager() {}

	std::string ResolvePath(const std::string& path);

  private:
	std::string base_path_;

};

} // namespace framework

#endif
