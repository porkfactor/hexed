#ifndef HEXED_LOG_HPP_
#define HEXED_LOG_HPP_

#include <spdlog/spdlog.h>
#include <blessed/span.hpp>

#define ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)

extern void log_data(blessed::span<blessed::byte const> s);
extern void log_uint32(blessed::span<blessed::byte const> s);

#endif
