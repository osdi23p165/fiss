  
#pragma once

//C++ standard library
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <string>
#include <queue>
#include <vector>
#include <list>
#include <chrono>
#include <thread>
#include <iostream>
#include <condition_variable>
#include <atomic>
#include <cstdint>
#include <algorithm>
#include <set>
#include <functional>
using std::map;
using std::queue;
using std::unordered_map;
using std::string;
using std::vector;
using std::list;
using std::set;
using std::unordered_set;
using std::pair;
using std::function;

// system library
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

// yaml-cpp
#include <yaml-cpp/yaml.h>

// c library
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cinttypes>

// google library

// User include files
//

// rpc library
#include "cmake/build/state_transfer.grpc.pb.h"
#include "cmake/build/state_transfer.pb.h"

//file IO
#include <fstream>
#include <iostream>

//mutex
#include <mutex>