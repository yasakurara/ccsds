#pragma once

#include <string>
#include <vector>

namespace StringFormat 
{
    template<
        typename T,
        std::enable_if_t<std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value, std::nullptr_t> = nullptr
    >
    auto _stdStringToChar(T&& value)
    {
        return value.c_str();
    }
    template<
        typename T,
        std::enable_if_t<!std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value, std::nullptr_t> = nullptr
    >
    auto _stdStringToChar(T&& value)
    {
        return value;
    }

    template <typename... Args>
    std::string _Format(const std::string& fmt, Args&& ... args ){
        size_t len = std::snprintf( nullptr, 0, fmt.c_str(), std::forward<Args>(args)... );
        std::vector<char> buf(len + 1);
        std::snprintf(&buf[0], len + 1, fmt.c_str(), std::forward<Args>(args)... );
        return std::string(&buf[0], &buf[0] + len);
    }

    template <typename... Args>
    std::string Format(const std::string& fmt, Args&& ... args ){
        return _Format(fmt, _stdStringToChar(std::forward<Args>(args)) ...);
    }
}