#include <windows.h>
#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <mutex>
#include <cstdlib>

namespace fs = std::filesystem;

std::mutex consoleMutex;

void SetColor(WORD color)
{
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        color | FOREGROUND_INTENSITY
    );
}

fs::path GetTempPathSafe()
{
    char* buffer = nullptr;
    size_t size = 0;

    if (_dupenv_s(&buffer, &size, "TEMP") == 0 && buffer)
    {
        fs::path tempPath(buffer);
        free(buffer);
        return tempPath;
    }
    return {};
}

void Log(const char* name, const char* status, WORD color)
{
    std::lock_guard<std::mutex> lock(consoleMutex);

    SetColor(5);
    std::cout << "[+] ";
    SetColor(7);
    std::cout << name;

    SetColor(8);
    std::cout << " - ";

    SetColor(color);
    std::cout << status << "\n";

    SetColor(7);
}

void CleanFolder(const char* name, fs::path path)
{
    try
    {
        if (!path.empty() && fs::exists(path))
        {
            for (const auto& entry : fs::directory_iterator(path))
            {
                try
                {
                    fs::remove_all(entry);
                }
                catch (...) {}
            }
        }

        Log(name, "DONE", 10);
    }
    catch (...)
    {
        Log(name, "ACCESS DENIED", 12);
    }
}

int main()
{
    SetConsoleTitleA("System Cleaner");
    SetColor(5);

    std::cout << R"(
 ____  _  _  ____  ____  ____  _  _     ___  __    ____   __   __ _  ____  ____ 
/ ___)( \/ )/ ___)(_  _)(  __)( \/ )   / __)(  )  (  __) / _\ (  ( \(  __)(  _ \
\___ \ )  / \___ \  )(   ) _) / \/ \  ( (__ / (_/\ ) _) /    \/    / ) _)  )   /
(____/(__/  (____/ (__) (____)\_)(_/   \___)\____/(____)\_/\_/\_)__)(____)(__\_)


)";

    fs::path tempPath = GetTempPathSafe();

    const std::vector<std::pair<const char*, fs::path>> targets =
    {
        { "TEMP", tempPath },
        { "Windows Temp", "C:\\Windows\\Temp" },
        { "Prefetch", "C:\\Windows\\Prefetch" },
        { "Update Cache", "C:\\Windows\\SoftwareDistribution\\Download" },
        { "Error Reports", "C:\\ProgramData\\Microsoft\\Windows\\WER" }
    };

    std::vector<std::thread> workers;
    workers.reserve(targets.size());

    for (const auto& t : targets)
        workers.emplace_back(CleanFolder, t.first, t.second);

    for (auto& w : workers)
        w.join();

    SetColor(5);
    std::cout << "\nCleaning completed successfully. Press any key to exit.\n";

    SetColor(7);
    system("pause >nul");
}
