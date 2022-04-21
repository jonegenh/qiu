cmake_minimum_required(VERSION 3.0)
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
	scripts/application.cc
	scripts/bytearray.cc
	scripts/config.cc
	scripts/daemon.cc
	scripts/env.cc
	scripts/fd_manager.cc
	scripts/fiber.cc
	scripts/http/http.cc
	scripts/http/http_connection.cc
	scripts/http/http_parser.cc
	scripts/http/http_session.cc
	scripts/http/http_server.cc
	scripts/http/servlet.cc
	scripts/hook.cc
	scripts/iomanager.cc
	scripts/log.cc
	scripts/scheduler.cc
	scripts/socket.cc
	scripts/socket_stream.cc
	scripts/stream.cc
	scripts/tcp_server.cc
	scripts/timer.cc
	scripts/thread.cc
	scripts/util.cc
	)

ragelmaker(scripts/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/scripts/http)
ragelmaker(scripts/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/scripts/http)

ragelmaker(scripts/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/scripts)

add_library(scripts SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(scripts) # __FILE__
#add_library(scripts_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES {scripts_static PROPERTIES OPUTPUT_NAME "scripts")

set(LIB_LIB
	scripts
	pthread
	dl
	${YAMLCPP})

# add_executable(test tests/test.cc)
# add_dependencies(test scripts)
# force_redefine_file_macro_for_sources(test) # __FILE__
# target_link_libraries(test ${LIB_LIB})

qiu_add_executable(test "tests/test.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_config "tests/test_config.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_thread "tests/test_thread.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_util "tests/test_util.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_fiber "tests/test_fiber.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_scheduler "tests/test_scheduler.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_iomanager "tests/test_iomanager.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_hook "tests/test_hook.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_address "tests/test_address.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_socket "tests/test_socket.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_bytearray "tests/test_bytearray.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_http "tests/test_http.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_http_parser "tests/test_http_parser.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_tcp_server "tests/test_tcp_server.cc" scripts "${LIB_LIB}")
qiu_add_executable(echo_server "examples/echo_server.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_http_server "tests/test_http_server.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_http_connection "tests/test_http_connection.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_uri "tests/test_uri.cc" scripts "${LIB_LIB}")
qiu_add_executable(my_http_server "samples/my_http_server.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_daemon "tests/test_daemon.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_env "tests/test_env.cc" scripts "${LIB_LIB}")
qiu_add_executable(test_application "tests/test_application.cc" scripts "${LIB_LIB}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
