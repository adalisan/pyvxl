cmake_minimum_required(VERSION 2.8)
#
# Python Bindings for VXL using Boost.Python
#
# Author: Daniel Crispell, Vision Systems, Inc.
#

find_package(VXL)
if(VXL_FOUND)
  include(${VXL_CMAKE_DIR}/UseVXL.cmake)
else()
  message(FATAL_ERROR "VXL_DIR should be set to the VXL build directory.")
endif()

find_package(Boost REQUIRED COMPONENTS program_options python)
find_package(PythonLibs REQUIRED)


include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${BOOST_INCLUDE_PATH})
include_directories(${VXL_VCL_INCLUDE_DIRS})
include_directories(${VXL_CORE_INCLUDE_DIRS})

link_directories(${BOOST_LIB_PATH})
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
