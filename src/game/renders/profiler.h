#ifndef HORUSEYE_RENDERS_PROFILER_H_
#define HORUSEYE_RENDERS_PROFILER_H_

#include <ugdk/graphic.h>
#include <ugdk/debug.h>
#include <memory>

namespace renders {

void DrawSectionData(ugdk::graphic::Canvas& canvas, const std::shared_ptr<const ugdk::debug::SectionData>& data);

} // namespace renders

#endif // HORUSEYE_RENDERS_PROFILER_H_