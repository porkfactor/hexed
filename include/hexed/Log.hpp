#ifndef HEXED_LOG_HPP_
#define HEXED_LOG_HPP_

#include <spdlog/spdlog.h>

#define INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)

#endif
