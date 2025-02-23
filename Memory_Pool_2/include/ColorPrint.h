#pragma once

#include <format> // C++20
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#define COLOR_RED    "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE   "\033[34m"
#define COLOR_GREEN  "\033[32m"
#define COLOR_RESET  "\033[0m"

template<typename... Args>
void printf_color(
    const std::string &color, const std::string &format_str, Args &&...args) {
    std::ostringstream oss;
    oss << color;

    oss << std::vformat(
        format_str, std::make_format_args(std::forward<Args>(args)...));

    oss << COLOR_RESET;
    std::cout << oss.str() << std::endl;
}

template<typename... Args>
void print_color(const std::string &color, Args &&...args) {
    std::ostringstream oss;
    oss << color;

    if constexpr (sizeof...(args) > 0) {
        ((oss << std::forward<Args>(args) << ". "), ...);
    }

    oss << COLOR_RESET;
    std::cout << oss.str() << std::endl;
}

#define PRINT_RED(...)    print_color(COLOR_RED, ##__VA_ARGS__)
#define PRINT_YELLOW(...) print_color(COLOR_YELLOW, ##__VA_ARGS__)
#define PRINT_BLUE(...)   print_color(COLOR_BLUE, ##__VA_ARGS__)
#define PRINT_GREEN(...)  print_color(COLOR_GREEN, ##__VA_ARGS__)

#define PRINTF_RED(fmt, ...)    printf_color(COLOR_RED, fmt, ##__VA_ARGS__)
#define PRINTF_YELLOW(fmt, ...) printf_color(COLOR_YELLOW, fmt, ##__VA_ARGS__)
#define PRINTF_BLUE(fmt, ...)   printf_color(COLOR_BLUE, fmt, ##__VA_ARGS__)
#define PRINTF_GREEN(fmt, ...)  printf_color(COLOR_GREEN, fmt, ##__VA_ARGS__)