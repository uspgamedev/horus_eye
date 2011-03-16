#ifndef HORUSEYE_FRAMEWORK_TYPES_H_
#define HORUSEYE_FRAMEWORK_TYPES_H_

#include <SDL/SDL.h>

namespace framework {

typedef enum {
    LIGHT_IGNORE,
    LIGHT_SOURCE,
    LIGHT_ILLUMINATED
} LightType;

typedef struct Color {
  float r;
  float g;
  float b;
} Color;

typedef Uint8 uint8;
typedef Uint16 uint16;
typedef Uint32 uint32;
typedef Uint64 uint64;
typedef Sint8 int8;
typedef Sint16 int16;
typedef Sint32 int32;
typedef Sint64 int64;

}  // namespace framework

#endif /* HORUSEYE_FRAMEWORK_TYPES_H_ */
