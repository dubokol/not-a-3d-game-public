#pragma once

#include "utils.h"
#include <map>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class WorldsManager {
    static constexpr int version = 1;
    static constexpr char extension[] = ".txt";
    static constexpr char worlds_folder_name[] = "worlds";
    static inline bool initialized = false;

    static void init();

public:

    static bool exists(const std::string &name);

    static void save(const std::map<std::string, std::string> &state, const std::string &name);

    static std::map<std::string, std::string> load(const std::string &name);
};
