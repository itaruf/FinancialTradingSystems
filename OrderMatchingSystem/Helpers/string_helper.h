#pragma once

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "../order.h"

// Helper class for string-centric helper functions
class StringHelper
{
public:
    static inline std::vector<std::string> parse_string(const std::string& str, const char& separator)
    {
        std::vector<std::string> result;
        std::string substr;
        std::istringstream str_stream(str);

        while (std::getline(str_stream, substr, separator))
            result.emplace_back(substr);

        return result;
    }

    static inline Order::Side string_to_enum(const std::string& str)
    {
        if (str == "BUY") return Order::Side::BUY;
        if (str == "SELL") return Order::Side::SELL;

        return Order::Side::SELL;
    }

    static inline std::string double_to_string(double value, int precision)
    {
        std::ostringstream out;
        out << std::fixed << std::setprecision(precision) << value;
        std::string str = out.str();

        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.')
            str.pop_back();

        return str;
    }
};