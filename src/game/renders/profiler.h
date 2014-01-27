#ifndef HORUSEYE_RENDERS_PROFILER_H_
#define HORUSEYE_RENDERS_PROFILER_H_

#include <ugdk/graphic.h>
#include <ugdk/debug.h>
#include <memory>
#include <sstream>

namespace renders {

void SectionDataToString(std::stringstream& out, const std::string& indent, const std::shared_ptr<const ugdk::debug::SectionData>& data);

void DrawSectionData(ugdk::graphic::Canvas& canvas, const std::shared_ptr<const ugdk::debug::SectionData>& data);

} // namespace renders

#endif // HORUSEYE_RENDERS_PROFILER_H_