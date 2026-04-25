/* * ============================================================================
 * HOW TO USE THE LOGGING SYSTEM / COMMENT UTILISER LE SYSTEME DE LOGS
 * ============================================================================
 * * 1. INITIALIZATION / INITIALISATION :
 * Call XK_LOG_INIT() once at the start of your main().
 * Appelez XK_LOG_INIT() une fois au début de votre main().
 * * 2. LOGGING MACROS / MACROS DE LOG :
 * XK_INFO(...)    : General information / Information générale
 * XK_SUCCESS(...) : Success messages (Green-ish) / Messages de succès (Vert)
 * XK_WARN(...)    : Warnings (Yellow) / Avertissements (Jaune)
 * XK_ERROR(...)   : Errors (Red) / Erreurs (Rouge)
 * XK_CRITICAL(...): Fatal errors / Erreurs fatales
 * * 3. FEATURES / FONCTIONNALITES :
 * - Automatic folder creation ("logs/") / Création auto du dossier ("logs/")
 * - Keeps only the last 5 sessions / Garde uniquement les 5 dernières sessions
 * - UTF-8 Support for Windows Console / Support UTF-8 pour la console Windows
 * - Dual output: Console + .log file / Double sortie : Console + fichier .log
 * ============================================================================
 */

#define _CRT_SECURE_NO_WARNINGS
#include "XkLogs.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <chrono>

namespace Xk {
    std::shared_ptr<spdlog::logger> s_coreLogger;

    void Logs::Init() {

#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif

        namespace fs = std::filesystem;
        const std::string logDir = "logs";
        const size_t maxLogFiles = 5;

        try {
            if (!fs::exists(logDir)) {
                fs::create_directories(logDir);
            }
            else {
                std::vector<fs::directory_entry> entries;
                for (const auto& entry : fs::directory_iterator(logDir)) {
                    if (entry.is_regular_file() && entry.path().extension() == ".log") {
                        entries.push_back(entry);
                    }
                }

                if (entries.size() >= maxLogFiles) {
                    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
                        return fs::last_write_time(a) < fs::last_write_time(b);
                        });

                    size_t numToDelete = entries.size() - maxLogFiles + 1;
                    for (size_t i = 0; i < numToDelete; ++i) {
                        fs::remove(entries[i].path());
                    }
                }
            }
        }
        catch (...) { }

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss << logDir << "/Xk_" << std::put_time(std::localtime(&now), "%Y-%m-%d_%Hh%Mm%S") << ".log";
        std::string logPath = ss.str();

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("%^[%T] %n: %v%$");

        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath, true);
        fileSink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

        s_coreLogger = std::make_shared<spdlog::logger>("MOTEUR", spdlog::sinks_init_list{ consoleSink, fileSink });

        s_coreLogger->set_level(spdlog::level::trace);
        s_coreLogger->flush_on(spdlog::level::trace);

        spdlog::set_default_logger(s_coreLogger);

        XK_INFO("Logs connectes ! Session : {0}", logPath);
    }
}