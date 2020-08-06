cmake_minimum_required(VERSION 2.8)

# Cloth simulation source
set(POINT2MESH_SOURCE
    # Mesh datastructures
    edge.cpp
    sphere.cpp
    triangle.cpp
    vertex.cpp
    utils.cpp

    # Spcae datastructures
    octree.cpp
    ocnode.cpp

    # Application
    main.cpp
 )

# Windows-only sources
# if(WIN32)
# list(APPEND CLOTHSIM_VIEWER_SOURCE
    # For get-opt
    # misc/getopt.c
# )
# endif(WIN32)

#-------------------------------------------------------------------------------
# Set definitions
#-------------------------------------------------------------------------------
# add_definitions(${NANOGUI_EXTRA_DEFS})

#-------------------------------------------------------------------------------
# Set include directories
#-------------------------------------------------------------------------------
include_directories(
  ${CGL_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  # ${NANOGUI_EXTRA_INCS}
)

#-------------------------------------------------------------------------------
# Set link directories
#-------------------------------------------------------------------------------
link_directories(
  ${CGL_LIBRARY_DIRS}
  ${FREETYPE_LIBRARY_DIRS}
)

#-------------------------------------------------------------------------------
# Add executable
#-------------------------------------------------------------------------------
add_executable(p2m ${POINT2MESH_SOURCE})

target_link_libraries(p2m
    CGL ${CGL_LIBRARIES}
    # nanogui ${NANOGUI_EXTRA_LIBS}
    ${FREETYPE_LIBRARIES}
    ${CMAKE_THREADS_INIT}
)

#-------------------------------------------------------------------------------
# Platform-specific configurations for target
#-------------------------------------------------------------------------------
if(APPLE)
  set_property( TARGET p2m APPEND_STRING PROPERTY COMPILE_FLAGS
                "-Wno-deprecated-declarations -Wno-c++11-extensions")
endif(APPLE)

# Put executable in build directory root
set(EXECUTABLE_OUTPUT_PATH ..)

# Install to project root
install(TARGETS p2m DESTINATION ${Point2Mesh_SOURCE_DIR})