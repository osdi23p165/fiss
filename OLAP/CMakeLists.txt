# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
cmake_minimum_required(VERSION 3.5.1)

project(htap)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcx16")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHOMEGROWN")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ljemalloc")
find_package(PkgConfig REQUIRED)
pkg_check_modules (JEMALLOC jemalloc)
pkg_search_module(JEMALLOC REQUIRED jemalloc)
include_directories(${JEMALLOC_INCLUDE_DIRS})

find_package(Threads REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
find_package(Protobuf REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if (CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else ()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif ()

find_package(gRPC REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
if (CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else ()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif ()


# Proto file
get_filename_component(ap_proto "../rpc/state_transfer.proto" ABSOLUTE)
get_filename_component(ap_proto_path "${ap_proto}" PATH)
# Generated sources
set(ap_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/state_transfer.pb.cc")
set(ap_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/state_transfer.pb.h")
set(ap_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/state_transfer.grpc.pb.cc")
set(ap_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/state_transfer.grpc.pb.h")
set(my_src_list "Config.cpp" "DeltaManager.cc" "./PAM/tpch/test.cpp")
add_custom_command(
      OUTPUT "${ap_proto_srcs}" "${ap_proto_hdrs}" "${ap_grpc_srcs}" "${ap_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${ap_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${ap_proto}"
      DEPENDS "${ap_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(".")
# Targets greeter_[async_](client|server)
include_directories(
      PAM/c++
      utilities
)
link_directories("/usr/local/lib")
file(GLOB File_SRC
        "PAM/tpch/*.h"
        )
foreach(_target
  APServer)
  add_executable(${_target} "${_target}.cc"
          ${File_SRC}
    ${ap_proto_srcs}
    ${ap_grpc_srcs}
    ${my_src_list})
  target_link_libraries(${_target}
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    ${JEMALLOC_LIBRARIES})
endforeach()


