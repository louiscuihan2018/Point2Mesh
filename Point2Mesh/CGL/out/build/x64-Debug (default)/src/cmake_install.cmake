# Install script for directory: H:/CS184/Point2Mesh/data_structure/test/CGL/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "H:/CS184/Point2Mesh/data_structure/test/CGL/out/install/x64-Debug (default)")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "H:/CS184/Point2Mesh/data_structure/test/CGL/out/build/x64-Debug (default)/src/CGL.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CGL" TYPE FILE FILES
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/CGL.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/vector2D.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/vector3D.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/vector4D.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/matrix3x3.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/matrix4x4.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/quaternion.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/complex.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/color.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/osdtext.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/viewer.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/base64.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/tinyxml2.h"
    "H:/CS184/Point2Mesh/data_structure/test/CGL/src/renderer.h"
    )
endif()

