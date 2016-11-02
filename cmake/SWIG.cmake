
# Is HORUS_MODULE_SRC defined?
if (NOT HORUS_MODULE_SRC)
    message (FATAL_ERROR "Variable HORUS_MODULE_SRC not defined! Please do so in the file src/module_list.cmake!")
endif (NOT HORUS_MODULE_SRC)

#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR}")
set (CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/generated")

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${SRC_DIR}/module")

set(CMAKE_SWIG_FLAGS "${CMAKE_SWIG_FLAGS}"
            "-I${horus_eye_SOURCE_DIR}/src")

if(ANDROID)
    set (HORUS_LANGUAGES_LIST "Lua")
else()
    set (HORUS_LANGUAGES_LIST "Lua;Python")
endif()

include (${ugdk_SOURCE_DIR}/cmake/CreateBindings.cmake)
create_modules(HORUS_LANGUAGES_LIST HORUS_MODULE_SRC "HORUS" GENERATED_SRC)
