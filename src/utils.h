#pragma once

#include <string>
#include <filesystem>
#include <Windows.h>
#include <chrono>

#define TO_STR(x) #x

inline const std::filesystem::path EXE_DIR = []() {
    std::wstring buf;
    buf.resize(32768);
    DWORD len = GetModuleFileNameW(nullptr, buf.data(), static_cast<DWORD>(buf.size()));
    buf.resize(len);
    return std::filesystem::path(buf).parent_path();
}();

inline std::chrono::local_time<std::chrono::system_clock::duration> get_local_time() {
    return std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
}
