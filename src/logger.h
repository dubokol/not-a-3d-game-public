#pragma once

#include "core/file_managers/logs_manager.h"

#define LOG(msg) LogsManager::log(msg, __FILE__, __LINE__)
#define ERR(msg) LogsManager::error(msg, __FILE__, __LINE__)
