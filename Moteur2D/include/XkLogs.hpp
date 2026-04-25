/* ============================================================================
 * XK_LOGS SYSTEM - HEADER / SYSTEME DE LOGS XK - EN-TETE
 * ============================================================================
 * USAGE / UTILISATION :
 * 1. Include this header in your engine files.
 * Incluez cet en-tête dans vos fichiers moteur.
 * 2. Initialize once in main() using XK_LOG_INIT().
 * Initialisez une fois dans le main() via XK_LOG_INIT().
 * 3. Use XK_INFO, XK_SUCCESS, etc., to log messages.
 * Utilisez XK_INFO, XK_SUCCESS, etc., pour loguer vos messages.
 * ============================================================================
 */

#ifndef XK_LOGS_HPP
#define XK_LOGS_HPP

#include <memory>
#include <spdlog/spdlog.h>

namespace Xk {
    extern std::shared_ptr<spdlog::logger> s_coreLogger;

    class Logs {
    public:
        static void Init();
    };
}

// --- LOGGING MACROS / MACROS DE LOG ---

// Setup the logging system / Configure le système de log
#define XK_LOG_INIT()    ::Xk::Logs::Init()

// Standard logging levels / Niveaux de log standard
#define XK_INFO(...)     ::Xk::s_coreLogger->info(__VA_ARGS__)
#define XK_SUCCESS(...)  ::Xk::s_coreLogger->trace(__VA_ARGS__) // Uses trace for success / Utilise trace pour le succès
#define XK_WARN(...)     ::Xk::s_coreLogger->warn(__VA_ARGS__)
#define XK_ERROR(...)    ::Xk::s_coreLogger->error(__VA_ARGS__)
#define XK_CRITICAL(...) ::Xk::s_coreLogger->critical(__VA_ARGS__)

// Level control / Contrôle du niveau de visibilité
#define XK_SET_LOG_LEVEL_ERROR() ::Xk::s_coreLogger->set_level(spdlog::level::err)
#define XK_SET_LOG_LEVEL_ALL()   ::Xk::s_coreLogger->set_level(spdlog::level::trace)

#endif // XK_LOGS_HPP