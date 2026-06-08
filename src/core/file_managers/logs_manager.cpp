#include "logs_manager.h"
#include "logger.h"
#include <chrono>
#include <format>

const fs::path LogsManager::current_log_file = []() {
    fs::path folder_path = EXE_DIR / logs_folder_name;
    fs::create_directories(folder_path);

    int main_log_name_size = static_cast<int>(std::strlen(main_log_name));
    int log_extension_size = static_cast<int>(std::strlen(extension));

    int max_id = 0; // current format: "logXXX.txt"

    for (const auto &entry: fs::directory_iterator(logs_folder_name)) {
        if (entry.is_regular_file()) {
            std::string file_name = entry.path().filename().string();

            if (file_name.size() > (main_log_name_size + log_extension_size) &&
                file_name.substr(0, main_log_name_size) == main_log_name &&
                file_name.substr(file_name.size() - log_extension_size) == extension) {

                try {
                    int current_id = std::stoi(file_name.substr(main_log_name_size, file_name.length() - (main_log_name_size + log_extension_size)));
                    if (current_id > max_id) {
                        max_id = current_id;
                    }
                } catch (...) {}
            }
        }
    }

    return EXE_DIR / logs_folder_name / (main_log_name + std::to_string(max_id + 1) + extension);
}();

void LogsManager::init() {
    fs::path folder_path = EXE_DIR / logs_folder_name;
    bool created = fs::create_directories(folder_path);

    if (created) {
        LOG("Folder " + folder_path.string() + " created");
    }

    auto &out = get_log_stream();
    out << std::format("{:%d.%m.%Y %H:%M:%S}\n", get_local_time());
}

std::ofstream &LogsManager::get_log_stream() {
    static std::ofstream out(current_log_file, std::ios::app);
    if (!out.is_open()) {
        std::cerr << "ERROR: Cannot open log file: " << current_log_file.string() << std::endl;
        std::exit(1);
    }
    return out;
}

void LogsManager::log(const std::map<std::string, std::string>& state, const char *file, int line) {
    auto &out = get_log_stream();
    out << file << ':' << line << " -> " << "World settings:" << '\n';
    out << "=======" << '\n';
    for (const auto& [k, v]: state) {
        out << k << '=' << v << '\n';
    }
    out << "=======" << '\n';
    out.flush();
}
