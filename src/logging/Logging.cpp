#include "Logging.hpp"
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <unistd.h>

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
    : m_Mutex()
    , m_LogLevel(LogLevel::Info)
{
// do nothing
}

Logger::~Logger()
{
// do nothing
}

void Logger::_Log(const LogLevel& logLevel, const std::string& where, const std::string& msg)
{
    if (logLevel > this->m_LogLevel) return;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    struct tm t;
    localtime_r(&ts.tv_sec, &t);

    char buf[32] = {};
    strftime(buf, 32, "%Y.%m.%d %H:%M:%S", &t);

    char time_str[32] = {};
    const int msec = ts.tv_nsec / 1000000;
    snprintf(time_str, 32, "%s.%03d", buf, msec);

    std::string log_msg = StringFormat::Format("%s [%s][%ld:%ld][%s] %s", time_str, _GetLogLevelString(logLevel), getpid(), std::this_thread::get_id(), where, msg);

    std::lock_guard<std::mutex> lock(this->m_Mutex);
    std::cout << log_msg << std::endl;
}

std::string Logger::_GetLogLevelString(LogLevel logLevel)
{
    switch (logLevel)
    {
    case LogLevel::Critical:    return "CRITICAL";
    case LogLevel::Error:       return "ERROR";
    case LogLevel::Warning:     return "WARNING";
    case LogLevel::Info:        return "INFO";
    case LogLevel::Debug:       return "DEBUG";
    default:                    return "UNKNOWN";
    }
}
