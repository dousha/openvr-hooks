/**
 * @file HigThingsLogger.cpp
 * @author dousha
 * @date 2023/11/28
 */

#include "logger.h"

#include <utility>
#include "spdlog/sinks/stdout_color_sinks.h"

namespace vrhook {

static std::shared_ptr<spdlog::logger> currentLogger = nullptr;

std::shared_ptr<spdlog::logger> ObtainLogger() {
    if (currentLogger == nullptr) {
        currentLogger = spdlog::stdout_color_mt("vrhook");
        currentLogger->set_level(spdlog::level::trace);
    }
    return currentLogger;
}

void SetLogger(std::shared_ptr<spdlog::logger> logger) {
    currentLogger = std::move(logger);
}

} // namespace vrhook
