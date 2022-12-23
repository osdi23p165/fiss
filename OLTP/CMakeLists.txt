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

SET(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")


find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(ZLIB REQUIRED)

pkg_check_modules(APR REQUIRED apr-1)
pkg_check_modules(APRUTIL REQUIRED apr-util-1)

pkg_check_modules(PYTHON2 REQUIRED python2)

pkg_check_modules(YAML_CPP REQUIRED yaml-cpp)

FIND_PACKAGE(Boost COMPONENTS system filesystem coroutine REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})


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


### STUPID FIX
# this is not necessary, just let the IDE to do the indexing
include_directories(build/CMakeFiles)


add_library(ap_proto SHARED
        ${ap_grpc_srcs}
        ${ap_grpc_hdrs}
        ${ap_proto_srcs}
        ${ap_proto_hdrs})

target_link_libraries(ap_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
	-lz
	)

add_executable(fake_ap_server
        fake_ap_server.cc)

target_link_libraries(
        fake_ap_server
        ap_proto
        ZLIB::ZLIB
	-lz
)

add_executable(fake_tp_client
        fake_tp_client.cc)

target_link_libraries(
        fake_tp_client
        ap_proto
        ZLIB::ZLIB
	-lz
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/dast/deptran/mdcc/mdcc_rpc.h
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/dast/bin/rpcgen --cpp deptran/mdcc/mdcc_rpc.rpc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/dast/deptran/mdcc/mdcc_rpc.rpc
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dast)


add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/dast/deptran/rcc_rpc.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dast/deptran/rcc_rpc.py
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/dast/bin/rpcgen --cpp --python deptran/rcc_rpc.rpc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/dast/deptran/rcc_rpc.rpc
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dast)

add_custom_target(gen-rpc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/dast/deptran/mdcc/mdcc_rpc.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dast/deptran/rcc_rpc.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dast/deptran/rcc_rpc.py)

include_directories(
        ${PYTHON2_INCLUDE_DIRS}
        ${APR_INCLUDE_DIRS}
        dast
        dast/rrr
        dast/memdb
        dast/deptran
)


file(GLOB RRR_SRC
        "dast/rrr/base/*.hpp"
        "dast/rrr/base/*.cpp"
        "dast/rrr/misc/*.hpp"
        "dast/rrr/misc/*.cpp"
        "dast/rrr/coroutine/*.h"
        "dast/rrr/coroutine/*.cc"
        "dast/rrr/mq/*.cpp"
        "dast/rrr/pylib/simplerpc/_pyrpc.cpp"
        "dast/rrr/rpc/*.cpp"
        "dast/rrr/rpc/*.hpp"
        "dast/rrr/utils/*.h"
        "dast/rrr/rrr.h"
        "dast/rrr/rrr.hpp"
        )

file(GLOB MEMDB_SRC
        "dast/memdb/*.h"
        "dast/memdb/*.cc")

file(GLOB BENCH_SRC
        "dast/bench/*/*.h"
        "dast/bench/*/*.cc"
        )

file(GLOB DEPTRAN_SRC

        "dast/deptran/*.h"
        "dast/deptran/*.cc"
        "dast/deptran/*/*.cc"
        "dast/deptran/*/*.h"
        )

link_directories(
        ${PYTHON2_LIBRARY_DIRS}
        ${APR_LIBRARY_DIRS}
        ${YAML_CPP_LIBRARY_DIRS}
)

add_library(RRR
        ${RRR_SRC})

add_library(MEMDB
        ${MEMDB_SRC} dast/memdb/row_mv.cc dast/memdb/row_mv.h)

add_executable(ROCOCO_SERVER
        ${DEPTRAN_SRC}
        ${BENCH_SRC} dast/deptran/cdc_client.h dast/deptran/cdc_client.cpp)
#        deptran/chronos/frame.cc
#        deptran/chronos/frame.h deptran/chronos/commo.h deptran/chronos/commo.cc deptran/chronos/commo.h deptran/chronos/coordinator.cc deptran/chronos/coordinator.h deptran/chronos/scheduler.cc deptran/chronos/scheduler.h deptran/chronos/tx.cc deptran/chronos/tx.h deptran/ocean_vista/ov-gossiper.cc deptran/ocean_vista/ov-gossiper.h deptran/ocean_vista/ov-commo.h deptran/ocean_vista/ov-tx.cc deptran/chronos/types.h deptran/chronos/types-marshall.cc
#        deptran/slog/auxiliary_raft.cc
#        deptran/slog/auxiliary_raft.h deptran/chronos/manager.cc deptran/chronos/manager.h deptran/slog/manager.h deptran/slog/manager.cc)

add_dependencies(ROCOCO_SERVER gen-rpc)

target_link_libraries(MEMDB
        RRR
	-lz
	)


target_link_libraries(
        ROCOCO_SERVER
        RRR
        MEMDB
        ap_proto
        ${CMAKE_THREAD_LIBS_INIT}
        ${YAML_CPP_LIBRARIES}
        ${Boost_LIBRARIES}
        ${PYTHON2_LIBRARIES}
        ${APR_LIBRARIES}
        ${APRUTIL_LIBRARIES}
	-lz
	)
