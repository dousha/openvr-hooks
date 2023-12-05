/**
 * @file logger.h
 * @author dousha
 * @date 2023/7/27
 */

#ifndef HIGGUN2_LOGGER_H
#define HIGGUN2_LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace vrhook {

extern std::shared_ptr<spdlog::logger> ObtainLogger();

extern void SetLogger(std::shared_ptr<spdlog::logger> logger);

} // namespace vrhook

#endif // HIGGUN2_LOGGER_H
