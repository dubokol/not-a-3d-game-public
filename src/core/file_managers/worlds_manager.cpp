#include "worlds_manager.h"
#include "logger.h"


void WorldsManager::init() {
    if (initialized) return;

    fs::path folder_path = EXE_DIR / worlds_folder_name;

    if (fs::create_directories(folder_path)) {
        LOG("Folder " + folder_path.string() + " created");
    }

    initialized = true;
}

bool WorldsManager::exists(const std::string &name) {
    fs::path file_path = EXE_DIR / worlds_folder_name / (name + extension);
    return fs::exists(file_path);
}

void WorldsManager::save(const std::map<std::string, std::string>& state, const std::string& name) {
    init();
    fs::path file_path = EXE_DIR / worlds_folder_name / (name + extension);
    std::ofstream out(file_path, std::ios::trunc);
    if (!out.is_open()) {
        ERR("Cannot open file for writing: " + file_path.string());
    }

    out << version << '\n';

    for (const auto& [k, v]: state) {
        out << k << '=' << v << '\n';
    }

    out.flush();
    if (!out.good()) {
        ERR("Write error: " + file_path.string());
    } else {
        LOG("Saved data to " + file_path.string() + " (" + std::to_string(state.size()) + " entries)");
    }
}

std::map<std::string, std::string> WorldsManager::load(const std::string& name) {
    init();
    fs::path file_path = EXE_DIR / worlds_folder_name / (name + extension);
    std::ifstream in(file_path);
    if (!in.is_open()) {
        ERR("Cannot open file for reading: " + file_path.string());
    }

    std::map<std::string, std::string> res;
    std::string line;
    int line_no = 0;

    std::getline(in, line);
    line_no++;

    if (std::stoi(line) != version) {
        ERR("Bad line " + std::to_string(line_no) + " in " + file_path.string() + ": " + line + ". Version must be " +
            std::to_string(version));
    }

    while (std::getline(in, line)) {
        line_no++;
        if (line.empty()) continue;

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        auto pos = line.find('=');
        if (pos == std::string::npos) {
            ERR("Bad line " + std::to_string(line_no) + " in " + file_path.string() + ": " + line);
        }

        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);
        res[key] = val;
    }

    LOG("Loaded data from " + file_path.string() + " (" + std::to_string(res.size()) + " entries)");
    return res;
}
