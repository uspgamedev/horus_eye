
#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR}")
set (CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/generated")
set (GENERATED_SRC "")

# Does this work everywhere?
find_package (SWIG REQUIRED)

macro (horus_add_scriptlang lang)

  swig_add_module (ugdk_${lang} ${lang} ${HORUS_MODULE_SRC} ${GAME_SRC})
  set (TEMP_ONLY_CXX)
  foreach (it ${swig_generated_sources})
    if (${it} MATCHES ".cc$")
      set (TEMP_ONLY_CXX ${TEMP_ONLY_CXX} ${it})
    elseif (${it} MATCHES ".py$")
      set (GENERATED_PY_FILES ${GENERATED_PY_FILES} ${it})
    endif ()
  endforeach (it)
  set (GENERATED_SRC ${GENERATED_SRC} ${TEMP_ONLY_CXX})
  unset (TEMP_ONLY_CXX)

endmacro (horus_add_scriptlang lang)

include (${CMAKE_CURRENT_LIST_DIR}/UseSWIG.cmake)

# Is HORUS_MODULE_SRC defined?
if (NOT HORUS_MODULE_SRC)
	message (FATAL_ERROR "Variable HORUS_MODULE_SRC not defined! Please do so in the file src/module_list.cmake!")
endif (NOT HORUS_MODULE_SRC)

set_source_files_properties (${HORUS_MODULE_SRC} PROPERTIES CPLUSPLUS ON)
set_source_files_properties (${HORUS_MODULE_SRC} PROPERTIES SWIG_FLAGS "")

set(UGDK_SWIG_ENABLED True)

set(MODULES_LIST "")
foreach(it ${HORUS_MODULE_SRC})
	get_filename_component(val ${it} NAME_WE)
	set(MODULES_LIST "${MODULES_LIST} \\\n    ACTION(LANG, ${val})")
endforeach()

set(LANGUAGES_LIST "")
foreach(it ${HORUS_LANGUAGES_LIST})
	horus_add_scriptlang (${it})
	string(TOUPPER ${it} itUPPER)
	set(LANGUAGES_LIST "${LANGUAGES_LIST} \\\n    ACTION(${itUPPER})")
endforeach()

set(MODULES_NAME HORUS)
set(GENERATED_SRC ${GENERATED_SRC} modules.cc)
configure_file(${CMAKE_CURRENT_LIST_DIR}/modules.cc.in modules.cc)
