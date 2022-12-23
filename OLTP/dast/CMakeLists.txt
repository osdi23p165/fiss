cmake_minimum_required(VERSION 2.8.4)
project(rococo)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")

find_package(Threads)
find_package(PkgConfig REQUIRED)
pkg_check_modules(APR REQUIRED apr-1)
pkg_check_modules(APRUTIL REQUIRED apr-util-1)

pkg_check_modules(PYTHON2 REQUIRED python2)

pkg_check_modules(YAML_CPP REQUIRED yaml-cpp)
#pkg_check_modules(PROFILER REQUIRED libprofiler)

FIND_PACKAGE(Boost COMPONENTS system filesystem coroutine REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG_CODE")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOG_LEVEL_AS_INFO")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DLOG_LEVEL_AS_DEBUG")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCPU_PROFILE")

#FIND_LIBRARY (DMALLOC_LIBRARY dmalloc /usr/lib /sw/lib)
#FIND_PATH (DMALLOC_INCLUDE_DIR dmalloc.h PATHS /usr/include /sw/include)
#IF(DMALLOC_LIBRARY)
#  ADD_DEFINITIONS(-D_USE_DMALLOC)
#  SET(LINK_TO_DMALLOC 1)
#ENDIF(DMALLOC_LIBRARY)


# GRPC

find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if (CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else ()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif ()

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
if (CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else ()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif ()

get_filename_component(ap_proto "../rpc/state_transfer.proto" REALPATH)
get_filename_component(ap_proto_path "${ap_proto}" PATH)

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

add_library(ap_proto
        ${ap_grpc_srcs}
        ${ap_grpc_hdrs}
        ${ap_proto_srcs}
        ${ap_proto_hdrs})

target_link_libraries(ap_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/deptran/mdcc/mdcc_rpc.h
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/rpcgen --cpp deptran/mdcc/mdcc_rpc.rpc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/deptran/mdcc/mdcc_rpc.rpc
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/deptran/rcc_rpc.h
        ${CMAKE_CURRENT_SOURCE_DIR}/deptran/rcc_rpc.py
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/rpcgen --cpp --python deptran/rcc_rpc.rpc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/deptran/rcc_rpc.rpc
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(gen-rpc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/deptran/mdcc/mdcc_rpc.h
        ${CMAKE_CURRENT_SOURCE_DIR}/deptran/rcc_rpc.h
        ${CMAKE_CURRENT_SOURCE_DIR}/deptran/rcc_rpc.py)

include_directories(
        ${PYTHON2_INCLUDE_DIRS}
        ${APR_INCLUDE_DIRS}
        #    dependencies/yaml-cpp/include
        #    ${PROFILER_INCLUDE_DIRS}
        .
        rrr
        memdb
        deptran
        #    deptran/tpl
        #    deptran/rcc
        #    deptran/ro6
        #    deptran/brq
        #    deptran/none
        #    bench
        #    bench/micro
        #    bench/rw_benchmark
        #    bench/tpca
        #    bench/tpcc
        #    bench/tpcc_dist
        #    bench/tpcc_real_dist
)

link_directories(
        ${PYTHON2_LIBRARY_DIRS}
        ${APR_LIBRARY_DIRS}
        ${YAML_CPP_LIBRARY_DIRS}
        #    ${PROFILER_LIBRARY_DIRS}
)


#set (CMAKE_SHARED_LINKER_FLAGS "-Wl, -lyaml-cpp")


file(GLOB RRR_SRC
        "rrr/base/*.hpp"
        "rrr/base/*.cpp"
        "rrr/misc/*.hpp"
        "rrr/misc/*.cpp"
        "rrr/coroutine/*.h"
        "rrr/coroutine/*.cc"
        "rrr/mq/*.cpp"
        #    "rrr/mq/*.h"
        "rrr/pylib/simplerpc/_pyrpc.cpp"
        "rrr/rpc/*.cpp"
        "rrr/rpc/*.hpp"
        "rrr/utils/*.h"
        "rrr/rrr.h"
        "rrr/rrr.hpp"
        )

file(GLOB MEMDB_SRC
        "memdb/*.h"
        "memdb/*.cc")

file(GLOB BENCH_SRC
        "bench/*/*.h"
        "bench/*/*.cc"
        )

file(GLOB DEPTRAN_SRC

        "deptran/*.h"
        "deptran/*.cc"
        "deptran/*/*.cc"
        "deptran/*/*.h"
        )

add_library(RRR
        ${RRR_SRC})

add_library(MEMDB
        ${MEMDB_SRC} memdb/row_mv.cc memdb/row_mv.h)

#add_library(ROCOCO
##    ${RRR_SRC}
##    ${MEMDB_SRC}
#    ${BENCH_SRC}
#    ${DEPTRAN_SRC})

add_executable(ROCOCO_SERVER
        ${DEPTRAN_SRC}
        ${BENCH_SRC}
        deptran/chronos/frame.cc
        deptran/chronos/frame.h deptran/chronos/commo.h deptran/chronos/commo.cc deptran/chronos/commo.h deptran/chronos/coordinator.cc deptran/chronos/coordinator.h deptran/chronos/scheduler.cc deptran/chronos/scheduler.h deptran/chronos/tx.cc deptran/chronos/tx.h deptran/ocean_vista/ov-gossiper.cc deptran/ocean_vista/ov-gossiper.h deptran/ocean_vista/ov-commo.h deptran/ocean_vista/ov-tx.cc deptran/chronos/types.h deptran/chronos/types-marshall.cc
        deptran/slog/auxiliary_raft.cc
        deptran/slog/auxiliary_raft.h deptran/chronos/manager.cc deptran/chronos/manager.h deptran/slog/manager.h deptran/slog/manager.cc)

add_dependencies(ROCOCO_SERVER gen-rpc)

target_link_libraries(MEMDB
        RRR)


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
        ${APRUTIL_LIBRARIES})

#add_executable(ROCOCO_SERVER, deptran/s_main.cc)
#add_executable(ROCOCO_CLIENT ${SOURCE_FILES})
