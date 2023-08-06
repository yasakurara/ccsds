#pragma once

#include "logging/StringFormat.hpp"

#include <string>
#include <mutex>

class Logger final
{
public:
    enum class LogLevel : int
    {
        Off = 0,
        Critical,
        Error,
        Warning,
        Info,
        Debug
    };

    template<typename ... Args>
    static void Log(const LogLevel& logLevel, const char* fileName, const char* funcName, const int lineNum, const std::string& format, Args&& ... args)
    {
        std::string where = StringFormat::Format("%s:%s:%d", fileName, funcName, lineNum);
        Logger::GetInstance()._Log(logLevel, where, StringFormat::Format(format, std::forward<Args>(args)...));
    }

    template<typename ... Args>
    static void Log(const LogLevel& logLevel, const char* fileName, const char* funcName, const int lineNum, const std::string& format)
    {
        std::string where = StringFormat::Format("%s:%s:%d", fileName, funcName, lineNum);
        Logger::GetInstance()._Log(logLevel, where, format);
    }

    static void SetLogLevel(const LogLevel& logLevel)
    {
        Logger::GetInstance().m_LogLevel = logLevel;
    }

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

private:
    static Logger& GetInstance();
    Logger();
    ~Logger();
    void _Log(const LogLevel& logLevel, const std::string& where, const std::string& msg);
    std::string _GetLogLevelString(LogLevel logLevel);

    std::mutex m_Mutex;
    LogLevel m_LogLevel;
};

#define LOG_CRITICAL(...)   Logger::Log(Logger::LogLevel::Critical, __FILE_NAME__, __func__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(...)      Logger::Log(Logger::LogLevel::Error,    __FILE_NAME__, __func__, __LINE__, __VA_ARGS__);
#define LOG_WARNING(...)    Logger::Log(Logger::LogLevel::Warning,  __FILE_NAME__, __func__, __LINE__, __VA_ARGS__);
#define LOG_INFO(...)       Logger::Log(Logger::LogLevel::Info,     __FILE_NAME__, __func__, __LINE__, __VA_ARGS__);
#define LOG_DEBUG(...)      Logger::Log(Logger::LogLevel::Debug,    __FILE_NAME__, __func__, __LINE__, __VA_ARGS__);