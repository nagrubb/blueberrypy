cmake_minimum_required(VERSION 2.8.3)
set(Python_ADDITIONAL_VERSIONS 2.7 2.6 2.5)

include_directories(bluez)
include_directories(bluez/lib)

link_libraries(bluetooth)
add_library(blueznative SHARED
  linux/BtAdapter.h
  linux/BtAdapter.cpp)

find_package(PythonInterp)
find_package(PythonLibs)
find_package(Boost COMPONENTS python)

include_directories(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
include_directories(linux)

link_libraries(${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
link_libraries(blueznative)

python_add_module(blueberrypy python_bindings/blueberrypy.cpp)
