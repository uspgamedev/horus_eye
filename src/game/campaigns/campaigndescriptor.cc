#include "campaigndescriptor.h"

#include "game/campaigns/exceptions.h"

#include <ugdk/system/engine.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>

#include <cassert>
#include <dirent.h>

template<typename ...Args>
struct RaiseExceptionIfMissingAux;

template<class T, class ...Args>
struct RaiseExceptionIfMissingAux<T, Args...> {
    static void Check(const ugdk::script::VirtualObj& obj, T v, Args... args) {
        if (!obj[v])
            throw campaigns::MissingDescriptorField(v);
        RaiseExceptionIfMissingAux<Args...>::Check(obj, std::forward<Args>(args)...);
    }
};

template<>
struct RaiseExceptionIfMissingAux<> {
    static void Check(const ugdk::script::VirtualObj& obj) {}
};

template<class ...Args>
void RaiseExceptionIfMissing(const ugdk::script::VirtualObj& obj, Args... args) {
    RaiseExceptionIfMissingAux<Args...>::Check(obj, std::forward<Args>(args)...);
}


namespace campaigns {

    namespace {
        std::string ConvertAbsolutePathToScriptPath(const std::string& path) {
            auto scripts_dir = ugdk::system::ResolvePath("scripts/");
            auto scripts_dir_loc = path.find(scripts_dir);
            assert(scripts_dir_loc != std::string::npos);
            return SCRIPT_MANAGER()->ConvertPathToDottedNotation(path.substr(scripts_dir_loc + scripts_dir.size()));
        }
    }

    CampaignDescriptor::CampaignDescriptor(const std::string& path)
        : playable_(true)
    {
        auto script_manager = SCRIPT_MANAGER();
        std::string script_path = ConvertAbsolutePathToScriptPath(path);

        auto descriptor = script_manager->LoadModule(script_path + ".descriptor");
        assert(descriptor);

        RaiseExceptionIfMissing(descriptor, "name");

        name_ = descriptor["name"].value<std::string>();
        path_ = path;
        script_path_ = script_path;

        if (auto playable = descriptor["playable"])
            playable_ = playable.value<bool>();
    }

    std::vector<CampaignDescriptor> FindAllCampaignDescriptorsAt(const std::string& path) {
        std::vector<CampaignDescriptor> result;

        DIR *dir;
        if ((dir = opendir(path.c_str())) != NULL) {
            /* print all the files and directories within directory */
            struct dirent *ent;
            while ((ent = readdir(dir)) != NULL) {
                if (ent->d_name[0] == '.') continue;

                result.emplace_back(path + "/" + ent->d_name);
            }
            closedir(dir);
        }

        return result;
    }

} // namespace campaigns