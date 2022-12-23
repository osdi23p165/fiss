#include "Config.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

int Config::LOG_WORKER_NUM = 5;

int Config::getLogWorkerNum() {
    return LOG_WORKER_NUM;
}
