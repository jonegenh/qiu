cmake_minimum_required(VERSION 2.8)
project(qiu)

include(cmake/utils.cmake)

set(CAMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)
include_directories(/home/qiu/include)
link_directories(/home/qiu/yaml-cpp/build)

find_library(YAMLCPP yaml-cpp)
message("***", ${LIB_LIB})

set(LIB_SRC
	scripts/address.cc
	scripts/bytearray.cc
	scripts/config.cc
	scripts/fd_manager.cc
	scripts/fiber.cc
	scripts/hook.cc
	scripts/iomanager.cc
	scripts/log.cc
	scripts/scheduler.cc
	scripts/socket.cc
	scripts/timer.cc
	scripts/thread.cc
	scripts/util.cc
	)

add_library(scripts SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(scripts) # __FILE__
#add_library(scripts_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES {scripts_static PROPERTIES OPUTPUT_NAME "scripts")

set(LIB_LIB
	scripts
	pthread
	dl
	${YAMLCPP})

add_executable(test tests/test.cc)
add_dependencies(test scripts)
force_redefine_file_macro_for_sources(test) # __FILE__
target_link_libraries(test ${LIB_LIB})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config scripts)
force_redefine_file_macro_for_sources(test_config) # __FILE__
target_link_libraries(test_config ${LIB_LIB}) 

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread scripts)
force_redefine_file_macro_for_sources(test_thread) # __FILE__
target_link_libraries(test_thread ${LIB_LIB}) 

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util scripts)
force_redefine_file_macro_for_sources(test_util) # __FILE__
target_link_libraries(test_util ${LIB_LIB}) 

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber scripts)
force_redefine_file_macro_for_sources(test_fiber) # __FILE__
target_link_libraries(test_fiber ${LIB_LIB}) 

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler scripts)
force_redefine_file_macro_for_sources(test_scheduler) # __FILE__
target_link_libraries(test_scheduler ${LIB_LIB}) 

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager scripts)
force_redefine_file_macro_for_sources(test_iomanager) # __FILE__
target_link_libraries(test_iomanager ${LIB_LIB}) 

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook scripts)
force_redefine_file_macro_for_sources(test_hook) # __FILE__
target_link_libraries(test_hook ${LIB_LIB}) 

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address scripts)
force_redefine_file_macro_for_sources(test_address) # __FILE__
target_link_libraries(test_address ${LIB_LIB}) 

add_executable(test_socket tests/test_socket.cc)
add_dependencies(test_socket scripts)
force_redefine_file_macro_for_sources(test_socket) # __FILE__
target_link_libraries(test_socket ${LIB_LIB}) 

add_executable(test_bytearray tests/test_bytearray.cc)
add_dependencies(test_bytearray scripts)
force_redefine_file_macro_for_sources(test_bytearray) # __FILE__
target_link_libraries(test_bytearray ${LIB_LIB}) 

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
