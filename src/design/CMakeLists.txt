set(MODULE_SOURCES
  telescope_cassegrain.cpp
  telescope_newton.cpp
  )

set(MODULE_SOURCES_WITH_DIR "")
foreach(src_file ${MODULE_SOURCES})
list(APPEND MODULE_SOURCES_WITH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${src_file})
endforeach(src_file)

list(APPEND SOURCES ${MODULE_SOURCES_WITH_DIR})
set(SOURCES "${SOURCES}" PARENT_SCOPE)
