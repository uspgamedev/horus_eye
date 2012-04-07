
#include <ugdk/util/gdd/simpleloader.th>
#include <ugdk/util/gdd/parser.h>

namespace ugdk {

namespace gdd {

using std::string;

template <class T>
T* SimpleLoader<T>::Load(const string& gddfile_path) {
    Parser<T> parser(AbstractLoader<T>::protocol());
    switch(parser.Parse(gddfile_path)) {
      case ParseStatus::OK:
        return loaded_data_;
        break;
      case ParseStatus::FILE_NOT_FOUND:
        fputs("NOT FILE", stderr);
        break;
      case ParseStatus::SYNTAX_ERROR:
        fputs("NOT SYNTAX", stderr);
        break;
      case ParseStatus::LOAD_ERROR:
        fputs("NOT LOAD", stderr);
        break;
    }
    return NULL;
}

} /* namespace gdd */

} /* namespace ugdk */
