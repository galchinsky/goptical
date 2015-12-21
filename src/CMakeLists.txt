set(SOURCES "")

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${SUBDIRS})
    add_subdirectory(${subdir})
endforeach(subdir)

message(STATUS ${SOURCES})

find_package(GSL REQUIRED)
find_package(Dime REQUIRED)
find_package(GD REQUIRED)
find_package(PLplot REQUIRED)

include_directories(${GSL_INCLUDE_DIRS})
include_directories(${Dime_INCLUDE_PATH})
include_directories(${GD_INCLUDE_DIR})
include_directories(${PLplot_INCLUDE_DIR})

set(LIBS ${GSL_LIBRARIES} ${Dime_LIBRARY} ${GD_LIBRARIES} ${PLplot_LIBRARIES})

add_library(${PROJECT_NAME}_static STATIC ${SOURCES})
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME}_static ${LIBS})
target_link_libraries(${PROJECT_NAME} ${LIBS})