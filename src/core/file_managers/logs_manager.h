#pragma once

#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

namespace fs = std::filesystem;

class LogsManager {
    static constexpr char extension[] = ".txt";
    static constexpr char main_log_name[] = "log";
    static constexpr char logs_folder_name[] = "logs";
    static const fs::path current_log_file;

    static std::ofstream &get_log_stream();

public:
    static void init();

    template<class T>
    static void log(const T &msg, const char *file, int line) {
        auto &out = get_log_stream();
        out << file << ':' << line << " -> " << msg << std::endl;
    }

    template<class T>
    static void error(const T &msg, const char *file, int line) {
        std::cerr << "ERROR: " << file << ':' << line << " -> " << msg << std::endl;
        auto &out = get_log_stream();
        out << "ERROR: " << file << ':' << line << " -> " << msg << std::endl;
        exit(1);
    }

    static void log(const std::map<std::string, std::string>& state, const char *file, int line);
};
