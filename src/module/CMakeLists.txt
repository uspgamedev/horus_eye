
include (module_list.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/SWIG.cmake)
horus_add_scriptlang (lua)
horus_add_scriptlang (python)
set (GENERATED_SRC ${GENERATED_SRC} CACHE STRING "dunno" FORCE)

