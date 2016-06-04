cmake_minimum_required(VERSION 2.8)
#
# Python Bindings for VXL using Boost.Python
#
# Author: Daniel Crispell, Vision Systems, Inc.
#

if (POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()

find_package(VXL)
if(VXL_FOUND)
  include(${VXL_CMAKE_DIR}/UseVXL.cmake)
else()
  message(FATAL_ERROR "VXL_DIR should be set to the VXL build directory.")
endif()

find_package(Boost REQUIRED COMPONENTS python)

# We want to link to the python libraries corresponding to the python exe in the user's path.
find_program(PYTHON_EXE python)
# follow symlink
get_filename_component(PYTHON_EXE ${PYTHON_EXE} REALPATH)
# get the directory containing the python exe
get_filename_component(PYTHON_EXE_DIR ${PYTHON_EXE} PATH)
# back up one directory
get_filename_component(PYTHON_BASE_DIR ${PYTHON_EXE_DIR} PATH)
set(CMAKE_PREFIX_PATH ${PYTHON_BASE_DIR})

find_package(PythonLibs REQUIRED)

message("BOOST_INCLUDE_PATH  = ${BOOST_INCLUDE_PATH}")
include_directories(${PYTHON_INCLUDE_PATH})
#include_directories(${BOOST_INCLUDE_PATH})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${VXL_VCL_INCLUDE_DIRS})
include_directories(${VXL_CORE_INCLUDE_DIRS})

#link_directories(${BOOST_LIB_PATH})
link_directories(${Boost_LIBRARY_DIRS})
link_directories(${VXL_LIBRARY_DIR})

set(PYVXL_SOURCES
  pyvxl.cxx
  pyvnl.h pyvnl.cxx
  pyvgl.h pyvgl.cxx
  pyvpgl.h pyvpgl.cxx
  )
add_library(pyvxl SHARED ${PYVXL_SOURCES})

target_link_libraries(pyvxl vgl vnl vpgl vil ${Boost_PYTHON_LIBRARY_RELEASE} ${PYTHON_LIBRARIES})
set_target_properties(pyvxl PROPERTIES OUTPUT_NAME vxl)
set_target_properties(pyvxl PROPERTIES PREFIX "")
set_target_properties(pyvxl PROPERTIES SUFFIX ".so")
