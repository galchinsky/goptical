set(SOURCES
  opengl_app.cpp
  )

get_filename_component( FNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable(${FNAME}_opengl ${SOURCES})
target_link_libraries(${FNAME}_opengl ${PROJECT_NAME}_static ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

set(SOURCES
  x11_app.cpp
  )
find_package(X11 REQUIRED)
include_directories( ${X11_INCLUDE_DIR} )

add_executable(${FNAME}_x11 ${SOURCES})
target_link_libraries(${FNAME}_x11 ${PROJECT_NAME}_static ${X11_LIBRARIES})